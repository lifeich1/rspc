#!/usr/bin/env perl
use v5.30;
use strict;
use utf8;
use warnings;

my $src = shift;
my $url;

if ( $src =~ m{/(\d+)/(\w)/\w+\.cc\Z} ) {
  my $p     = uc $2;
  my $plain = shift;
  if ($plain) {
    $url = "https://codeforces.com/contest/$1/submit#$p";
  }
  else {
    $url = "https://m1.codeforces.com/contest/$1/submit#$p";
  }
}
elsif ( $src =~ m{/((?:abc|arc|agc)\d+)/(\w)/\w+\.cc\Z} ) {
  my $p = "$1" . "_$2";
  $url = "https://atcoder.jp/contests/$1/submit?taskScreenName=$p";
}
else {
  die "submit.pl: Unsupport contest type for $src";
}

say "url: $url";
exec 'google-chrome', ($url);
