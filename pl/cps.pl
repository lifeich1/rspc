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

my $lis = IO::Socket::INET->new(
    LocalHost=>'127.0.0.1',
    LocalPort=>'27121',
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
        if ($in_body) {
            say;
        } elsif (/content-length/i) {
            &info("header: $_");
        } elsif (/\A\s*\Z/) {
            &info("in body");
            $in_body++;
        }
    }
}
