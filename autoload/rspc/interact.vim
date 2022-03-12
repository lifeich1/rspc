function! rspc#interact#New() abort
    let l:name = input("input filename(full): ")
    execute "tabe " . l:name
    call rspc#yank#NewMain()
    call rspc#yank#AppendTemplate("rw")
    execute "w"
endfunction

function! rspc#interact#Template() abort
    let l:name = input("input template name: ")
    if match(l:name, '^\w\+$') !=# 0
        echom "Require template name!"
        return
    endif
    call rspc#yank#AppendTemplate(l:name)
endfunction
