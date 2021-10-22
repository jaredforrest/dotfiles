#!/bin/sh
git clone https://github.com/k-takata/minpac.git ~/.config/nvim/pack/minpac/opt/minpac

sudo pacman -S neovim trash-cli
export PATH="$HOME/.local/bin:$PATH"
gidtown dwm-jared
gidtown dmenu-jared
gidtown st-jared
gidtown tabbed-jared
gidtown hnotify-utils
systemctl --user daemon-reload
systemctl enable --user --now dunst.service notify-time.timer offlineimap-oneshot.timer sxhkd.service vdirsyncer.timer
