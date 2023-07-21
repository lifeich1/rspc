#!/usr/bin/env perl
use v5.30;
use strict;
use utf8;
use warnings;
use open qw/:std :utf8/;

my $src = shift;
my $cmd = shift;

my $bin = $src;
if ( $src =~ /(.+)\.(?:cc|cpp)/ ) {
  $bin = $1;
  &build( $src, $bin );
}

sub reg_test {
  my ($bin) = @_;
}

sub build {
  my ( $src, $bin ) = @_;
  return if ( -f $bin && -r _ && -w _ && -x _ && ( ( -M _ ) < ( -M $src ) ) );

  chomp( my $btim = qx(date +%D-%Hh-%Mm-%Ss) );

  my @bear_prefix = ();
  if ( 0 == system(qw{which bear > /dev/null}) ) {
    @bear_prefix = qw/bear --/;
  }

  my @ex = (
    @bear_prefix,
    qw(
      g++ -std=c++17 -g -Wall -DRSPC_TRACE_HINT=1
      -I/usr/include/x86_64-linux-gnu/c++/11
      -I/usr/lib/gcc/x86_64-linux-gnu/11/include
      -I/usr/include/x86_64-linux-gnu
      -I/usr/include/c++/11
      -I/usr/include/c++/11/backward
    ),
    qq(-DRSPC_TRACE_BTIME="${btim}"),
    $src, '-o', $bin,
  );
  say join ' ', @ex;

  system(@ex) == 0 or die('make failed');
}

# vim: ts=2 sw=2
