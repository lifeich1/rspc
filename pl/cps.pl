#!/usr/bin/env perl
use v5.24;
use strict;
use utf8;
use warnings;
use open qw/:std :utf8/;
use IO::Socket::INET;
use Socket qw/pack_sockaddr_in inet_aton inet_ntoa unpack_sockaddr_in/;

my $log_f = '/tmp/rspc-cps.log';
open(my $log, '>>', $log_f) or die "open $log_f error: $!";
$log->autoflush(1);
sub info {
    say $log "$$# @_";
}
my $port = '27121';

sub cli_conn {
    my $conn = IO::Socket::INET->new(
        PeerHost=>'127.0.0.1',
        PeerPort=>$port,
        Proto => 'tcp',
        Type=>IO::Socket::SOCK_STREAM,
        Blocking=>1,
    ) or die "DEAD server: create tcp socket error: $!";
    $conn->autoflush(1);
    return $conn;
}

sub cli_access {
    my $cmd = shift;
    my $conn = &cli_conn;
    say $conn $cmd;
    $conn->shutdown(SHUT_WR);
    chomp(my $resp_line = (<$conn> or '<empty response>'));
    return $resp_line;
}

sub detect_alive {
    my $resp_line = &cli_access("RSPC#echo");
    die "bad response: $resp_line" unless $resp_line =~ /RSPC#cps#alive/;
    say "$resp_line";
    exit;
}

sub extcmd_v1 {
    my $cmd = shift;
    my $resp = &cli_access("RSPC#v1#" . $cmd);
    my $rc = 0;
    say $resp;
    if ($resp =~ /RSPC#code\((\d+)\)/) {
        $rc = $1;
    }
    return $rc;
}

my %cmds = (
    extcmd => '(e|ext|extcmd)',
    checkalive => '(a|checkalive|detect-alive)',
    help => '(h|help)',
);
while($_ = shift) {
    if (/\A$cmds{extcmd}\Z/) {
        my $cmd = shift;
        exit &extcmd_v1($cmd);
    }elsif (/\A$cmds{checkalive}\Z/) {
        &detect_alive;
        exit;
    } elsif (/\A$cmds{help}\Z/) {
        say "cmds:";
        while(my ($k, $v) = each %cmds) {
            printf "  %-40s%s\n", $v, $k;
        }
        exit;
    }
}

# follow main
my $lis = IO::Socket::INET->new(
    LocalHost=>'127.0.0.1',
    LocalPort=>$port,
    Listen=>8,
    Type=>IO::Socket::SOCK_STREAM,
    Proto=>'tcp',
    Reuse=>1,
    Blocking=>1,
) or die "create tcp socket error: $!";

{
    package de;
    use JSON::MaybeXS qw/decode_json/;
    use Data::Dumper;
    my $info;
    my %cc_for_prog;
    sub parse {
        my ($text) = @_;
        my $data = decode_json($text);
        my $url = $data->{url};
        if (!defined($url)) {
            &info("undefined url");
            return;
        }
        my $key;
        if ($url =~ m{codeforces.com/contest/(\d+)/problem/(\w+)}) {
            $key = "$1/$2";
        } elsif ($url =~ m{atcoder.jp/contests/(\w+)/tasks/\w+_(\w+)}) {
            $key = "$1/$2";
        } else {
            $info->("uncaught url: $url");
            return;
        }
        $key = lc $key;
        $cc_for_prog{$key} = \$data;
    }

    sub validate_path {
        my $p = shift;
        return ("null path") unless defined($p);
        if ($p =~ m{/(\w+/\w+)/?\Z}) {
            my $key = lc $1;
            return ("key $key not exists") unless exists $cc_for_prog{$key};
            return (0, $cc_for_prog{$key});
        } else {
            return ("path not match suffix catcher");
        }
    }

    sub dump_test {
        my $p = shift;
        my ($err, $data) = &validate_path($p);
        if ($err) {
            $info->("dump test err: $err");
            return $err;
        }
        my $n = 0;
        foreach(@{${$data}->{tests}}) {
            my $fin = "$p/sample$n.in";
            open(my $fi, '>', $fin) or return "open $fin error: $!";
            my $fan = "$p/sample$n.ans";
            open(my $fa, '>', $fan) or return "open $fan error: $!";
            print $fi $_->{input};
            print $fa $_->{output};
            $n++;
        }
        return "OK";
    }

    sub showcc {
        print Dumper %cc_for_prog;
    }
    sub setinfo {
        $info = shift;
    }
}

&de::setinfo(\&info);
while (1) {
    &info("accepting");
    my $conn = $lis->accept()
        or die "accept error: $!";
    my $in_body = 0;
    &info("conn $conn");
    while(<$conn>) {
        chomp;
        &info("l: $_");
        if (/\ARSPC#v1#dbgcc4p\b/) {
            &de::showcc();
        } elsif (/\ARSPC#v1#dumptest:(\S+)/) {
            my $p = $1;
            &info("doing dumptest $p");
            my $rs = &de::dump_test($1);
            &info("dumptest result: $rs");
            say $conn $rs;
        } elsif (/\ARSPC#v1#shutdown\b/) {
            my $psa = $conn->peername();
            my ($prt, $ipraw) = unpack_sockaddr_in $psa;
            my $ip = inet_ntoa $ipraw;
            say $conn "shutting down";
            say "SHUTDOWN cmd from $ip:$prt";
            exit;
        } elsif (/\ARSPC#echo/) {
            $conn->autoflush(1);
            say $conn "RSPC#cps#alive $$";
            last;
        } elsif ($in_body) {
            say;
            &de::parse($_);
        } elsif (/content-length/i) {
            &info("header: $_");
        } elsif (/\A\s*\Z/) {
            &info("in body");
            $in_body++;
        }
    }
}
