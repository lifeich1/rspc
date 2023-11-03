let s:plugin_dir = fnamemodify(expand("<sfile>"), ":h:h:h")
let s:cc_temdir = s:plugin_dir . '/ccalgo'
let s:cc_makefile = s:cc_temdir . '/arena.mk'
let s:pl_dir = s:plugin_dir . '/pl'
let s:mk_exe = s:pl_dir . '/arena_mk.pl'
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

function! rspc#utils#CcMaker()
    return s:mk_exe
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

function! rspc#utils#submit_pl()
    return s:pl_dir . "/submit.pl"
endfunction

function! rspc#utils#arena_exec(cmd)
    if &modified
        execute 'w'
    endif
    let l:cmd = s:mk_exe . " " . expand('%') . " " . a:cmd
    execute "Start " . l:cmd
endfunction

function! rspc#utils#Luafil()
    return s:plugin_dir . "/lua/rspc.lua"
endfunction

function! rspc#utils#cps_exec(cmd)
    let l:cmd = s:pl_dir . "/cps.pl e " . a:cmd
    execute "Start " . l:cmd
endfunction

function! rspc#utils#restart_cps(verbose)
    if get(b:, "rspc_restarted_cps", 0)
      return
    endif
    let b:rspc_restarted_cps = 1
    let l:cps = rspc#utils#cps_pl()
    let l:will_restart = 0
    if a:verbose
        echo "checking cps alive"
    endif
    call system(l:cps . ' checkalive > /dev/null 2>&1')
    if v:shell_error
        let l:will_restart = 1
    endif
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
