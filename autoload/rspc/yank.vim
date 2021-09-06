let s:plugin_dir = fnamemodify(expand("<sfile>"), ":h:h:h")

function! rspc#yank#AppendTemplate(name) abort
    let l:template_dir = s:plugin_dir . "/pcalgo/src"
    let l:template_file = l:template_dir . "/" . a:name . ".rs"
    let l:text = readfile(l:template_file)
    let l:output = ["mod " . a:name . " {"]
    for l:line in l:text
        if line == '#[cfg(test)]'
            break
        endif
        call add(l:output, "    " . l:line)
    endfor
    call add(l:output, "}")
    call append(line('$'), l:output)
endfunction

function! rspc#yank#NewMain() abort
    let l:text = ["use std::io::Write;",
                \ "",
                \ "fn main() {",
                \ "    let stdin = std::io::stdin();",
                \ "    let stdout = std::io::stdout();",
                \ "    let mut scan = rw::stdio_scanner(&stdin);",
                \ "    let mut out = rw::stdio_bufwriter(&stdout);",
                \ "",
                \ "    // solve here",
                \ "",
                \ "}"]
    call append(0, l:text)
    execute "?solve here"
endfunction

