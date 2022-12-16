function! rspc#ccyank#NewMain() abort
    let l:template_file = rspc#utils#plugin_dir() . "/ccalgo/main_template.cc"
    let l:text = readfile(l:template_file)
    call append(0, l:text)
    execute "?First todo"
endfunction
