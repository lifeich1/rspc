#use strict;
#use utf8;
#use open qw/:std :utf8/;

my $ALGO_INC_HINT = q/Placeholder for upcoming un-std algorithm, by rspc/;
my $EDA_INC_HINT = q/End placeholder for upcoming un-std algorithm, by rspc/;
my $SCHEME_INC_HINT = '// Scheme by rspc:';
my $CCALGO_RT = VIM::Eval('l:rspc_ccalgo_rt');

my @templates;
my @lines;
my %deps_for_template;
my %lines_for_template;
my $algo_inc_line;

open(my $log, '>>', '/tmp/rspc.ccfmt.log')
    or VIM::Msg("error open log: $!");
my $log_h = "$$#" . (time) . "# ";
sub info {
    say $log "${log_h}@_";
    say @_;
}

sub load_template {
    my ($name) = @_;
    return if (exists $lines_for_template{$name});
    &info("load_template $name");
    my @files = glob qq("${CCALGO_RT}/*/${name}.hh");
    &info("glob: @files");
    return if (@files == 0);
    my $file = $files[0];
    open(my $fin, '<', $file) or return &info("open $file failed: $!");
    my (@lines, %deps);
    my @ll = <$fin>;
    close $fin;
    foreach(@ll) {
        chomp;
        if (m{#include ".*([^/]+)\.hh"}) {
            $deps{$1}++;
        } elsif (m/\A[^#]/) {
            push @lines, ($_);
        }
    }
    my @deps = keys %deps;
    &info("${name} deps: $#deps @deps");
    &info("$name line count: $#lines");
    $deps_for_template{$name} = \@deps;
    $lines_for_template{$name} = \@lines;
}

sub load_this {
    my $lcnt = $curbuf->Count();
    @lines = $curbuf->Get(1 .. $lcnt);
    my $l = 1;
    my ($s, $t, $tt) = (0, 0);
    foreach(@lines) {
        if (/$ALGO_INC_HINT/) {
            $s = $l;
        } elsif (/$EDA_INC_HINT/) {
            $t = $l;
        } elsif (m{$SCHEME_INC_HINT (.+)$}) {
            my %t = map { $_ => 1 } (split /\s+/, $1);
            @templates = keys %t;
            $tt = $l;
        }
    } continue {
        $l++;
    }
    &info("templates: @templates");
    &info("dd: $s> <$t  $tt");
    $curbuf->Delete($tt);
    if ($t - $s > 1) {
        my ($l,  $r) = ($s + 1, $t - 1);
        &info("del $l, $r");
        $curbuf->Delete($l) foreach($l .. $r);
    }
    $algo_inc_line = $s;
    foreach(@templates) {
        &load_template($_);
    }
}

sub fix_template_list {
    &info("fix_template_list @templates");
    my (@ntemps, %ntemps);
    my %temps = map { $_ => 1 } @templates;
    foreach(values %deps_for_template) {
        next if ($#{$_} == -1);
        my %t = map { $_ => 1 } @{$_};
        @temps{keys %t} = values %t;
    }
    my @temps = keys %temps;
    foreach(@temps) {
        &load_template($_);
    }
    my $changing = 1;
    while ($changing) {
        $changing = 0;
        EACH_TEMP: foreach(@temps) {
            next if (exists $ntemps{$_});
            foreach(@{$deps_for_template{$_}}) {
                next EACH_TEMP if (!exists $ntemps{$_});
            }
            $ntemps{$_}++;
            push @ntemps, ($_);
            $changing++;
        }
    }
    my $res = $#ntemps != $#templates;
    @templates = @ntemps;
    return $res;
}

sub finish_this {
    &info("final templates: @templates");
    VIM::DoCommand("echo 'final templates: @templates'");
    $curbuf->Append($algo_inc_line + 1, "$SCHEME_INC_HINT " . (join ' ', @templates));
    foreach(reverse @templates) {
        $curbuf->Append($algo_inc_line, @{$lines_for_template{$_}});
    }
}

&load_this();
while (&fix_template_list()) {}
&finish_this();

close($log);
