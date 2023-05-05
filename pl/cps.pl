#!/usr/bin/env perl
use v5.24;
use strict;
use utf8;
use warnings;
use open qw/:std :utf8/;
use IO::Socket::UNIX;

my $log_f = '/tmp/rspc-cps.log';
open(my $log, '>>', $log_f) or die "open $log_f error: $!";

pipe(my $rjson, my $wjson) or die "pipe error: $!";

sub info {
    say $log "$$# @_";
}

sub cargo_from_ccp {
    $wjson->autoflush(1);
    &info("cargo_from_ccp");
    while (1) {
        &info("start");
        my $msg = qx/nc -l 27121/;
        if ($?) {
            &info("nc exit $?");
            sleep 1;
        } else {
            say $wjson $msg;
        }
    }
    exit;
}

my $cargo_tid = fork;
&cargo_from_ccp unless $cargo_tid;

&info("recv loop");
while (<$rjson>) {
    print;
}
say $!;
