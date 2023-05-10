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

sub detect_alive {
    my $conn = IO::Socket::INET->new(
        PeerHost=>'127.0.0.1',
        PeerPort=>$port,
        Proto => 'tcp',
        Type=>IO::Socket::SOCK_STREAM,
        Blocking=>1,
    ) or die "DEAD server: create tcp socket error: $!";
    say $conn "RSPC#echo";
    $conn->autoflush(1);
    $conn->shutdown(SHUT_WR);
    chomp(my $resp_line = <$conn>);
    die "bad response: $resp_line" unless $resp_line =~ /RSPC#cps#alive/;
    say "$resp_line";
    exit;
}

my %cmds = (
    checkalive => '(a|checkalive|detect-alive)',
    help => '(h|help)',
);
while($_ = shift) {
    if (/\A$cmds{checkalive}\Z/) {
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

while (1) {
    &info("accepting");
    my $conn = $lis->accept()
        or die "accept error: $!";
    my $in_body = 0;
    &info("conn $conn");
    while(<$conn>) {
        chomp;
        &info("l: $_");
        if (/\ARSPC#echo/) {
            $conn->autoflush(1);
            say $conn "RSPC#cps#alive $$";
            last;
        } elsif ($in_body) {
            say;
        } elsif (/content-length/i) {
            &info("header: $_");
        } elsif (/\A\s*\Z/) {
            &info("in body");
            $in_body++;
        }
    }
}
