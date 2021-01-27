set tabstop=4
set shiftwidth=4
set expandtab

let g:ycm_clangd_binary_path = "/path/to/clangd"
" Specify a directory for plugins
call plug#begin('~/.local/share/nvim/plugged')

Plug 'lervag/vimtex'
Plug 'ycm-core/YouCompleteMe'
Plug 'udalov/kotlin-vim'
Plug 'xavierd/clang_complete'
Plug 'yegappan/taglist'
Plug 'fatih/molokai'

call plug#end()

" REQUIRED. This makes vim invoke Latex-Suite when you open a tex file.
"filetype plugin on

" OPTIONAL: Starting with Vim 7, the filetype of empty .tex files defaults to
" 'plaintex' instead of 'tex', which results in vim-latex not being loaded.
" The following changes the default filetype back to 'tex':
let g:tex_flavor='latex'
let g:molokai_original = 1

colorscheme molokai

