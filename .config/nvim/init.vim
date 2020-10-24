set tabstop=4
set shiftwidth=4
set expandtab

" Specify a directory for plugins
" - For Neovim: stdpath('data') . '/plugged'
" - Avoid using standard Vim directory names like 'plugin'
call plug#begin('~/.local/share/nvim/plugged')

Plug 'lervag/vimtex'
Plug 'Shougo/deoplete.nvim'
Plug 'udalov/kotlin-vim'

call plug#end()

" REQUIRED. This makes vim invoke Latex-Suite when you open a tex file.
"filetype plugin on

" OPTIONAL: Starting with Vim 7, the filetype of empty .tex files defaults to
" 'plaintex' instead of 'tex', which results in vim-latex not being loaded.
" The following changes the default filetype back to 'tex':
let g:tex_flavor='latex'

" Enable deoplete
let g:deoplete#enable_at_startup = 1

" This is new style
call deoplete#custom#var('omni', 'input_patterns', {
      \ 'tex': g:vimtex#re#deoplete
      \})
