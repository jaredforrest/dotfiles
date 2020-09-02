autoload -U colors && colors

# For vcs (git)
autoload -Uz vcs_info
setopt prompt_subst
zstyle ':vcs_info:*' formats       \
    '%s%F{3}:%f%F{2}%b%f %u%c'
precmd () { vcs_info }


PS1='%B%F{blue}#%b \
%F{6}%n%f \
@ \
%F{green}%m%f \
in \
%B%F{yellow}%~%b%f \
${vcs_info_msg_0_}\
[%*] \
%(?,,C:%F{red}%?)
%B%F{red}$ %b%f'

setopt autocd           # Automatically cd into typed directory.       

# History in cache directory:
HISTSIZE=10000
SAVEHIST=10000
HISTFILE=~/.cache/zsh/history

# Load aliases and shortcuts if existent.
[ -f "${XDG_CONFIG_HOME:-$HOME/.config}/shortcutrc"  ] && source "${XDG_CONFIG_HOME:-$HOME/.config}/shortcutrc"
[ -f "${XDG_CONFIG_HOME:-$HOME/.config}/aliasrc"  ] && source "${XDG_CONFIG_HOME:-$HOME/.config}/aliasrc"
[ -f "${XDG_CONFIG_HOME:-$HOME/.config}/zshnameddirrc"  ] && source "${XDG_CONFIG_HOME:-$HOME/.config}/zshnameddirrc"

# Load other config
source $ZDOTDIR/zsh-keymap.zsh

# Add custom command completions#
fpath=($ZDOTDIR/completions $fpath)

# Basic auto/tab complete:
autoload -U compinit
#zstyle ':completion:*' menu select
#zmodload zsh/complist
compinit

# Use lf to switch directories and bind it to ctrl-o
lfcd () {
    tmp="$(mktemp)"
    lf -last-dir-path="$tmp" "$@"
    if [ -f "$tmp"  ]; then
    dir="$(cat "$tmp")"
    rm -f "$tmp" >/dev/null
    [ -d "$dir"  ] && [ "$dir" != "$(pwd)"  ] && cd "$dir"
fi
}
bindkey -s '^o' 'lfcd\n'

