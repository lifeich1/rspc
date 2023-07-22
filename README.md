# RSPC

It is a neovim plugin aiming at easily utilizing algorithm templates **in personal
requirement**.

RSPC: PC for "programing competion", *RS for nothing but just historical redundancy*.

**If you want contest algorithms in rust, please checkout [EbTech/rust-algorithms](https://crates.io/crates/contest-algorithms)**

## Todo

1. Vim plugin: template name completion

## Install & Usage

Use package, vim's document about this mechanic `:help packages<CR>`.

Then set normal mode map to use it for convenience:

```vim
nnoremap <leader>rn :RSPCNew<cr>
nnoremap <leader>rt :RSPCAppend<cr>
nnoremap <leader>m :RSPCMake<cr>

nnoremap <leader>t :RSPCRegTest<cr>
nnoremap <leader>T :RSPCCustomTest<cr>
cnoremap <Bslash>at RSPCAutoTest<cr>
cnoremap <Bslash>qt !rm keeptest<cr>
```
