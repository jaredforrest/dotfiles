#!/bin/sh
git clone https://github.com/k-takata/minpac.git ~/.config/nvim/pack/minpac/opt/minpac
systemctl enable --user --now dunst.service notify-time.timer offline-imap.timer sxhkd.service vdirsyncer.timer
