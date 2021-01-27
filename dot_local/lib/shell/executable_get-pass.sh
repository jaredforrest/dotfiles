#!/bin/sh

# Decrypts and reencrypts passwords using temporary session password for OfflineImap, Mutt, etc

umask 077
passname="$1"
[ -z "$passname" ] && exit 1
logfile="${XDG_DATA_HOME:-$HOME/.local/share}/get-pass/log"
storefile="${XDG_DATA_HOME:-$HOME/.local/share}/get-pass/store"

# check if a temp directory already exists
[ -f "$storefile" -a -d "$(cat $storefile 2> /dev/null)" ] &&
    passdir="$(cat $storefile)" ||
    { passdir="$(mktemp -d)"; printf '%s' "$passdir" > $storefile; }

sessionpassfile="$passdir/session_pass"
[ -f "${sessionpassfile}" ] ||
    { printf '%s\n' 'Session pass does not exist, creating now' >> "$logfile"; LC_ALL=C tr -dc "${PASH_PATTERN:-_,#@+A-Z-a-z-0-9}" < /dev/urandom |
    dd ibs=1 obs=1 count="${PASH_LENGTH:-50}" > "$sessionpassfile" 2>/dev/null ;}

passfile="$passdir/pass/$passname.gpg"
mkdir -p "${passfile%/*}"
[ -f "${passfile}" ] || 
    { printf '%s\n' "Passfile for $passname does not exist, creating now" >> "$logfile";
    for _ in 1 2 3
    do
        pass=$(pach h "$passname") && break
    done
    [ -z "$pass" ] && exit 1 
    printf '%s' "$pass" | gpg --symmetric --quiet --pinentry-mode loopback --passphrase-file "$sessionpassfile" -o "$passfile" ;}
gpg --decrypt --quiet --pinentry-mode loopback --passphrase-file "$sessionpassfile" "$passfile"
