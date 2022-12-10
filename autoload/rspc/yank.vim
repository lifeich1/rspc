let s:plugin_dir = fnamemodify(expand("<sfile>"), ":h:h:h")

let s:INS_TEM_HINT = "// placeholder for coming template by rspc"
let s:TRACE_TOGGLE_HINT = "// Comment this line to disable trace log, by rspc"
let s:TRACE_DISABLE = "// [disable] "

function! rspc#yank#AppendTemplate(name) abort
    let l:template_dir = s:plugin_dir . "/pcalgo/src"
    let l:template_file = l:template_dir . "/" . a:name . ".rs"
    let l:text = readfile(l:template_file)
    let l:inslin = search(s:INS_TEM_HINT)
    let l:output = []
    if l:inslin == 0
        let l:output = ["mod a {"]
    endif
    for l:line in l:text
        if line == '#[cfg(test)]'
            break
        endif
        call add(l:output, "    " . l:line)
    endfor
    if l:inslin == 0
        call add(l:output, "    " . s:INS_TEM_HINT)
        call add(l:output, "}")
        let l:inslin = line('$')
    else
        let l:inslin = l:inslin - 1
    endif
    call append(l:inslin, l:output)
endfunction

function! rspc#yank#ToggleTrace() abort
    let l:v = winsaveview()
    let l:n = search(s:TRACE_TOGGLE_HINT)
    if l:n == 0
        throw 'Error searching real_trace hint'
    endif
    let l:t = getline(l:n)
    let l:a = split(l:t, '\V' . s:TRACE_DISABLE)
    if len(l:a) == 2
        let l:o = join(l:a, '')
    else
        let l:o = substitute(l:t, '\<', s:TRACE_DISABLE, '')
    endif
    call setline(l:n, l:o)
    call winrestview(l:v)
endfunction

function! rspc#yank#NewMain() abort
    let l:text = ["use std::io::Write;",
                \ "",
                \ "macro_rules! real_trace {",
                \ "    ($($tail:tt)*) => {",
                \ "        {",
                \ "            let stdout = std::io::stdout();",
                \ "            let mut out = a::stdio_bufwriter(&stdout);",
                \ "            writeln!(out, $($tail)*).ok();",
                \ "        }",
                \ "    };",
                \ "}",
                \ "",
                \ "macro_rules! trace {",
                \ "    ($($tail:tt)*) => {",
                \ "        real_trace!($($tail)*) " . s:TRACE_TOGGLE_HINT,
                \ "    };",
                \ "}",
                \ "",
                \ "fn main() {",
                \ "    let stdin = std::io::stdin();",
                \ "    let stdout = std::io::stdout();",
                \ "    let mut scan = a::stdio_scanner(&stdin);",
                \ "    let mut out = a::stdio_bufwriter(&stdout);",
                \ "",
                \ "    // solve here",
                \ "",
                \ "}"]
    call append(0, l:text)
    execute "?solve here"
endfunction

