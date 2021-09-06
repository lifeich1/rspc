function! rspc#interact#New() abort
    execute "tabe"
    call rspc#yank#NewMain()
    call rspc#yank#AppendTemplate("rw")
endfunction

function! rspc#interact#Template() abort
    let l:name = input("input template name: ")
    if match(l:name, '^\w\+$') !=# 0
        echom "Require template name!"
        return
    endif
    call rspc#yank#AppendTemplate(l:name)
endfunction
