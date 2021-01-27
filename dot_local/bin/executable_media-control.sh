#!/bin/sh
playerctl play-pause &&
    notify-send -i "$(playerctl metadata --format "{{mpris:artUrl}}")" "$(playerctl metadata --format "{{title}}")" "$(playerctl metadata --format "{{artist}}")"
