#!/bin/sh

_readfile () {
read -r pass

while read -r line; do
    key=${line%%:*}
    case $key in
        user) user=${line#*: } ;;
        *) ;;
    esac
done
}

