#!/bin/sh
# changeVolume

# Arbitrary but unique message id
msgId="991049"

# Change the volume using alsa(might differ if you use pulseaudio)
if [ "$1" = "mute" ]; then
    pactl set-sink-mute @DEFAULT_SINK@ toggle
else
    pactl set-sink-volume @DEFAULT_SINK@ "$@"> /dev/null
fi

#    pactl set-sink-volume @DEFAULT_SINK@ +1000 # 655 ~= 1%

#    pactl set-sink-volume @DEFAULT_SINK@ -1000
    
#    pactl set-sink-mute @DEFAULT_SINK@ toggle



# Query amixer for the current volume and whether or not the speaker is muted
volume="$(pacmd list-sinks|grep -A 15 '* index'| awk '/volume: front/{ print $5 }' | sed 's/[%|,]//g')"
mute="$(pacmd list-sinks|grep -A 15 '* index'|awk '/muted:/{ print $2 }')"
if [ "$volume" -eq 0 ] || [ "$mute" = "yes" ]; then
    # Show the sound muted notification
    dunstify -a "changeVolume" -u low -i notification-audio-volume-muted -h string:x-canonical-private-synchronous:volume \
        "Volume: ${volume}%" "$($SHELL_LIB_DIR/get-progress-string 10 "$volume" "■ " "□ ")"
else
    # Show the volume notification
    dunstify -a "changeVolume" -u low -i notification-audio-volume-high -h string:x-canonical-private-synchronous:volume \
        "Volume: ${volume}%" "$($SHELL_LIB_DIR/get-progress-string 10 "$volume" "■ " "□ ")"
fi

# Play the volume changed sound
#canberra-gtk-play -i audio-volume-change -d "changeVolume"
