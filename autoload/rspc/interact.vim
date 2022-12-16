function! rspc#interact#New() abort
    let l:name = input("input filename(full, default 'main.rs', 'cc' for 'main.cc'): ")
    if l:name == ""
        let l:name = "main.rs"
    endif
    if l:name == "cc"
        let l:name = "main.cc"
    endif
    execute "tabe " . l:name
    if rspc#utils#is_rs_f(l:name)
        call rspc#interact#RsNew()
    elseif rspc#utils#is_cc_f(l:name)
        call rspc#interact#CcNew()
    else
        throw "File format of " . l:name . " is unexpected!"
    endif
    execute "w"
endfunction

function! rspc#interact#Template() abort
    let l:name = input("input template name: ")
    if match(l:name, '^\w\+$') !=# 0
        throw "Require template name!"
    endif
    if rspc#utils#is_rs_f(bufname())
        call rspc#yank#AppendTemplate(l:name)
    elseif rspc#utils#is_cc_f(l:name)
        throw 'Cpp append template not implemented yet!'
    else
        throw "File format of " . l:name . " is unexpected!"
    endif
endfunction

function! rspc#interact#ToggleTrace() abort
    call rspc#yank#ToggleTrace()
endfunction

function! rspc#interact#RsNew() abort
    call rspc#yank#NewMain()
    call rspc#yank#AppendTemplate("rw")
endfunction

function! rspc#interact#CcNew() abort
    call rspc#ccyank#NewMain()
endfunction
