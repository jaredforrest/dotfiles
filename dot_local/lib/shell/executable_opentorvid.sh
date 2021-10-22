#!/bin/sh
#export http_proxy='http://127.0.0.1:8118'
setsid -f mpv --no-terminal $1 # --ytdl-raw-options=proxy=%14%127.0.0.1:8118 $1
