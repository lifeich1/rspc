#!/usr/bin/env perl
use v5.30;
use strict;
use utf8;
use warnings;
use open qw/:std :utf8/;

my $src = shift;
my $cmd = shift;

chomp(my $btim = qx(date +%D-%Hh-%Mm-%Ss));
my $bin = $src;
$bin =~ s/(.+)\.(?:cc|cpp)/$1/;

# TODO test modify time for skip

my @ex = (
  qw(g++ -std=c++17 -g -Wall -DRSPC_TRACE_HINT=1),
  qq(-DRSPC_TRACE_BTIME="${btim}"),
  $src,
  '-o', $bin,
);
say join ' ', @ex;

system(@ex) == 0 or die('make failed');


# vim: ts=2 sw=2
