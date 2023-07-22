#!/usr/bin/env perl
use v5.30;
use strict;
use utf8;
use warnings;
use open qw/:std :utf8/;

my $src = shift;

my $bin = $src;
if ( $src =~ /(.+)\.(?:cc|cpp)/ ) {
  $bin = "./$1";
  &build( $src, $bin );
}

defined( my $cmd = shift ) or exit;
if ( $cmd eq "_" ) {
  &reg_test($bin);
}
elsif ( -f "$cmd.in" ) {
  0 == system( "bash", "-c", "$bin < $cmd.in" )
    or die "$bin abort!";
  say "========== rc code 1 for keep quick window open ==========";
  exit 1;
}

sub smart_diff {
  my ( $n1, $n2 ) = @_;
  0 == system( "diff", "-b", $n1, $n2 )
    or die "$n1 diff $n2";
}

sub reg_test {
  my ($bin) = @_;
  my @list_in = glob '*.in';
  foreach (@list_in) {
    my ($name) = ( $_ =~ /(.+)\.in/ );
    if ( -f "$name.ans" ) {
      0 == system( "bash", "-c", "$bin < $name.in > $name.out" )
        or die("$bin abort!");
      &smart_diff( "$name.out", "$name.ans" );
    }
  }
}

sub build {
  my ( $src, $bin ) = @_;
  return if ( -f $bin && -r _ && -w _ && -x _ && ( ( -M _ ) < ( -M $src ) ) );

  chomp( my $btim = qx(date +%D-%Hh-%Mm-%Ss) );

  my @bear_prefix = ();
  if ( 0 == system(qw{which bear}) ) {
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
