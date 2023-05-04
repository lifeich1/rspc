let s:SCHEME_INC_HINT = '// Scheme by rspc:'

function! rspc#ccyank#NewMain() abort
    let l:template_file = rspc#utils#plugin_dir() . "/ccalgo/main_template.cc"
    let l:text = readfile(l:template_file)
    call append(0, l:text)
    execute "?First todo"
endfunction

function! rspc#ccyank#AppendTemplate(name) abort
    if rspc#utils#find_cc_tem(a:name)->len() == 0
        throw a:name . " not found"
    endif
    let l:line = search(s:SCHEME_INC_HINT)
    call setline(l:line, getline(l:line) . " " . a:name)
    call rspc#ccyank#FixTemplates()
endfunction

function! rspc#ccyank#FixTemplates() abort
    let l:rspc_ccalgo_rt = rspc#utils#cc_tem_dir()
    execute "perlf " . rspc#utils#cc_fmt_pl()
endfunction
