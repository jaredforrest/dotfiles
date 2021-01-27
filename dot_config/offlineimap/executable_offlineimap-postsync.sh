#!/bin/sh

notmuch new
notmuch tag --input="$HOME/.config/notmuch/tagmail.notmuch"
notmuch search --format=json tag:unread and tag:inbox | notify-mail
