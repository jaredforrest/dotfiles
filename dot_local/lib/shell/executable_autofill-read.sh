#!/bin/sh

_readfile () {
read -r _pass

while read -r line; do
    key=${line%%:*}
    case $key in
        user) _user=${line#*: } ;;
        key ) _key=${line#*: } ;;
        *) ;;
    esac
done
}

