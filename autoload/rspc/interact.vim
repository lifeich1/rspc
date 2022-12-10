function! rspc#interact#New() abort
    let l:name = input("input filename(full, default 'main.rs'): ")
    if l:name =~ ""
        let l:name = "main.rs"
    endif
    execute "tabe " . l:name
    call rspc#yank#NewMain()
    call rspc#yank#AppendTemplate("rw")
    execute "w"
endfunction

function! rspc#interact#Template() abort
    let l:name = input("input template name: ")
    if match(l:name, '^\w\+$') !=# 0
        throw "Require template name!"
    endif
    call rspc#yank#AppendTemplate(l:name)
endfunction

function! rspc#interact#ToggleTrace() abort
    call rspc#yank#ToggleTrace()
endfunction
