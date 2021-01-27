#!/bin/sh

_pachselect () {
printf '%s' "$(pach l)" | dmenu -i -c -h 24 -l 20
}
