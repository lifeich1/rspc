let s:plugin_dir = fnamemodify(expand("<sfile>"), ":h:h:h")
let s:cc_temdir = s:plugin_dir . '/ccalgo'
let s:cc_makefile = s:cc_temdir . '/arena.mk'
let s:pl_dir = s:plugin_dir . '/pl'
let s:cps_job = -1

function! rspc#utils#is_rs_f(name)
    return match(a:name, '\m^.*\.rs$') == 0
endfunction

function! rspc#utils#is_cc_f(name)
    return match(a:name, '\v^.*\.(cc|cpp)$') == 0
endfunction

function! rspc#utils#plugin_dir()
    return s:plugin_dir
endfunction

function! rspc#utils#CcMakefile()
    return s:cc_makefile
endfunction

function! rspc#utils#find_cc_tem(name)
    return (a:name . ".hh")->findfile(s:cc_temdir . '/**')
endfunction

function! rspc#utils#cc_tem_dir()
    return s:cc_temdir
endfunction

function! rspc#utils#cc_fmt_pl()
    return s:pl_dir . "/ccfmt.pl"
endfunction

function! rspc#utils#cps_pl()
    return s:pl_dir . "/cps.pl"
endfunction

function! rspc#utils#cps_exec(cmd)
    let l:cmd = s:pl_dir . "/cps.pl e " . a:cmd
    execute "Start " . l:cmd
endfunction

function! rspc#utils#restart_cps(verbose)
    let l:cps = rspc#utils#cps_pl()
    let l:will_restart = 0
    perl << END
    my $verb = VIM::Eval("a:verbose");
    my $cps = VIM::Eval("l:cps");
    say "checking cps alive" if $verb;
    if (0 != system("$cps checkalive > /dev/null 2>&1")) {
        say "will restart" if $verb;
        VIM::DoCommand("let l:will_restart = 1");
    }
END
    if a:verbose
        echo "var willrestart: " . l:will_restart
    endif
    if l:will_restart
        if a:verbose
            echo "Restarting cps"
        endif
        let s:cps_job = jobstart(l:cps)
        if a:verbose
            echo "cps job id: " . s:cps_job
        endif
    endif
endfunction
