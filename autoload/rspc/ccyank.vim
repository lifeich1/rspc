let s:ALGO_INC_HINT = "Placeholder for upcoming un-std algorithm, by rspc"
let s:EDA_INC_HINT = "End placeholder for upcoming un-std algorithm, by rspc"
let s:SCHEME_INC_HINT = '// Scheme by rspc:'

let s:TEM_BUF = {}
let s:TEM_DEPS = {}
let s:TEM_STD = {}

function! rspc#ccyank#ClearBuffer() abort
    let s:TEM_BUF = {}
    let s:TEM_DEPS = {}
    let s:TEM_STD = {}
endfunction

function! rspc#ccyank#NewMain() abort
    let l:template_file = rspc#utils#plugin_dir() . "/ccalgo/main_template.cc"
    let l:text = readfile(l:template_file)
    call append(0, l:text)
    execute "?First todo"
endfunction

function! rspc#ccyank#FixStdDeps(scheme) abort
    echo 'fix std includes ...'
    let l:stds = a:scheme->mapnew('s:TEM_STD[v:val]')->flatten()
    for l:std in l:stds
        let l:text = '#include <' . l:std . '>'
        if search(l:text) == 0
            call append(0, l:text)
        endif
    endfor
endfunction

function! rspc#ccyank#GetScheme() abort
    return search(s:SCHEME_INC_HINT)->getline()->matchstr('\m:.*$')->split('\v\W+')
endfunction

function! rspc#ccyank#SetScheme(scheme) abort
    call setline(search(s:SCHEME_INC_HINT), (s:SCHEME_INC_HINT . ' ' . a:scheme->join()))
endfunction

function! rspc#ccyank#FixScheme(scheme)
    let l:scheme = a:scheme->copy()->sort()->uniq()

    " fix missing deps
    let l:flag = 1
    while l:flag
        let l:flag = 0
        let l:t = l:scheme->copy()
        for l:name in l:t
            for l:v in s:TEM_DEPS[l:name]
                if -1 == l:scheme->index(l:v)
                    call add(l:scheme, l:v)
                    call rspc#ccyank#AppendTemplate(l:v)
                    let l:flag = 1
                endif
            endfor
        endfor
    endwhile
    echo 'fix missing is done.'
    " fix order
    let l:lps = 0
    let l:out = []
    while l:out->len() != l:scheme->len() && l:lps < 500
        let l:lps += 1
        for l:v in l:scheme
            if l:out->index(l:v) == -1
                let l:flag = 1
                for l:d in s:TEM_DEPS[l:v]
                    if l:out->index(l:d) == -1
                        let l:flag = 0
                        break
                    endif
                endfor
                if l:flag
                    call add(l:out, l:v)
                endif
            endif
        endfor
    endwhile
    if l:out->len() != l:scheme->len()
        throw 'Error fix scheme, lps(' . l:lps . '), l.scheme: ' . l:scheme
    endif
    return l:out
endfunction

function! rspc#ccyank#AppendTemplate(name, sync_scheme = 1, do_append = 1) abort
    echo ''
    if s:TEM_BUF->has_key(a:name)
        let l:text = s:TEM_BUF[a:name]
        echo ' text buffered:' a:name
        if !a:do_append
            return " no append means just rebuild buffer"
        endif
    else
        let l:file = rspc#utils#find_cc_tem(a:name)
        echo ' file:' l:file
        if l:file->len() == 0
            throw 'Cannot find template ' . a:name
        endif
        let l:text = readfile(l:file)
    endif
    echo 'append template' a:name

    let l:deps = []
    let l:stds = []
    let l:toins = []
    for l:line in l:text
        if l:line->match('\m^#pragma') == 0
            " discard
        elseif l:line->match('\m^#include <') == 0
            let l:std = l:line->matchstr('\m<.*>')->slice(1, -1)
            echo ' std:' l:std
            call add(l:stds, l:std)
        elseif l:line->match('\m^#include "') == 0
            let l:dep = l:line->matchstr('\v["/]\w+.hh"')->slice(1, -4)
            echo ' dep:' l:dep
            call add(l:deps, l:dep)
        else
            call add(l:toins, l:line)
        endif
    endfor
    if !has_key(s:TEM_BUF, a:name)
        let s:TEM_BUF[a:name] = l:text
        let s:TEM_DEPS[a:name] = l:deps
        let s:TEM_STD[a:name] = l:stds
    endif

    if a:do_append
        call append(search(s:EDA_INC_HINT) - 1, l:toins)
    endif
    if a:sync_scheme
        let l:n = search(s:SCHEME_INC_HINT)
        call setline(l:n, (l:n->getline() . ' ' . a:name))
    endif
endfunction

function! rspc#ccyank#RebuildBuf4Scheme() abort
    echo 'rebuilding buffer ...'
    let l:scheme = rspc#ccyank#GetScheme()
    call map(l:scheme->deepcopy()->sort()->uniq(), 'rspc#ccyank#AppendTemplate(v:val, 0, 0)')
endfunction

function! rspc#ccyank#FixTemplates(force_reload = 0) abort
    echo 'fix templates ...'
    let l:scheme = rspc#ccyank#GetScheme()
    let l:n_scheme = rspc#ccyank#FixScheme(l:scheme)
    if l:scheme !=# l:n_scheme || a:force_reload
        echo 'rearranging templates ...'
        let l:s = search(s:ALGO_INC_HINT) + 1
        let l:t = search(s:EDA_INC_HINT) - 1
        if l:s <= l:t
            call deletebufline(bufname(), l:s, l:t)
            for l:v in l:n_scheme
                call rspc#ccyank#AppendTemplate(l:v, 0)
            endfor
            call rspc#ccyank#SetScheme(l:n_scheme)
        else
            throw 'Hints is broken! s('. l:s . ') t(' . l:t . ')'
        endif
    endif
    call rspc#ccyank#FixStdDeps(l:n_scheme)
    echo 'Result scheme:' l:n_scheme->join()
endfunction
