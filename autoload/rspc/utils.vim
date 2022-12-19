let s:plugin_dir = fnamemodify(expand("<sfile>"), ":h:h:h")
let s:cc_temdir = s:plugin_dir . '/ccalgo'
let s:cc_makefile = s:cc_temdir . '/arena.mk'

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
