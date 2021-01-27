#!/bin/sh
URL="${1%videos*}api/v1/videos${1#*watch}"
URL=$(curl -s "$URL" | jq -M '.files|.[0]|.torrentUrl')
URL=${URL%*\"}
URL=${URL#\"*}
VIDEODIR=$(mktemp -d /tmp/peertubeXXXXX)
aria2c $URL --dir "$VIDEODIR" & 
sleep 5
setsid -f mpv --no-terminal "$VIDEODIR"/*.mp4
