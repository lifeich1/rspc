# RSPC

It is a vim plugin aiming at easily utilizing algorithm templates **in personal
requirement**.

RSPC: PC for "programing competion", *RS for nothing but just historical redundancy*.

**If you want contest algorithms in rust, please checkout [EbTech/rust-algorithms](https://crates.io/crates/contest-algorithms)**

## Todo

1. Vim plugin: choose template in buff win;

## Install & Usage

Currently unstable, recommend to get a snapshot then plug into vim like:

```vim
call plug#begin('~/.vim/plugged')
Plug '/dir/to/lifeich1/rspc'
call plug#end()
```

Then set normal mode map to use it for convenience:

```vim
" create prog from zero
nnoremap <leader>rn :RSPCNew<cr>

" add algorithm template to current file
nnoremap <leader>rt :RSPCAppend<cr>

" toggle rust prog's TRACE help macro
nnoremap <leader>tt :RSPCToggleTrace<cr>

" make current file with rustc or c++17
nnoremap <leader>m :RSPCMake<cr>
```
