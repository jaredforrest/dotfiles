#!/bin/zsh

# Enable color in less
export LESS_TERMCAP_mb=$'\E[01;38;5;11m'
export LESS_TERMCAP_md=$'\e[01;38;5;11m' \
export LESS_TERMCAP_me=$'\e[0m' \
export LESS_TERMCAP_se=$'\e[0m' \
export LESS_TERMCAP_so=$'\e[01;38;5;12m' \
export LESS_TERMCAP_ue=$'\e[0m' \
export LESS_TERMCAP_us=$'\e[01;38;5;3m' \

# Colors for ls and lf
LS_COLORS='rs=0:di=38;5;15:ln=38;5;250:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=38;5;190:*.txt=38;5;244:*.csv=38;5;244:*.rtf=38;5;101:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=38;5;34:*.jpeg=38;5;34:*.mjpg=35:*.mjpeg=35:*.gif=38;5;70:*.bmp=38;5;2:*.pbm=38;5;2:*.pgm=38;5;2:*.ppm=38;5;2:*.tga=38;5;35:*.xbm=38;5;2:*.xpm=38;5;2:*.tif=38;5;34:*.tiff=38;5;34:*.png=38;5;35:*.svg=38;5;39:*.svgz=38;5;39:*.mng=35:*.pcx=35:*.mov=38;5;93:*.mpg=38;5;93:*.mpeg=38;5;63:*.m2v=38;5;93:*.mkv=38;5;63:*.webm=38;5;92:*.webp=38;5;92:*.ogm=38;5;92:*.mp4=38;5;63:*.m4v=38;5;93:*.mp4v=38;5;93:*.vob=38;5;63:*.qt=01;35:*.nuv=01;35:*.wmv=38;5;92:*.asf=38;5;92:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=38;5;93:*.fli=01;35:*.flv=38;5;92:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=38;5;168:*.ogx=38;5;168:*.aac=38;5;168:*.au=38;5;168:*.flac=38;5;168:*.m4a=38;5;168:*.mid=38;5;168:*.midi=38;5;168:*.mka=38;5;168:*.mp3=38;5;168:*.mpc=38;5;168:*.ogg=38;5;168:*.ra=38;5;168:*.wav=38;5;168:*.oga=38;5;168:*.opus=38;5;168:*.spx=00;36:*.xspf=00;36:*.pdf=38;5;4:*.docx=38;5;12:*.doc=38;5;12:*.odt=38;5;12:*.pub=38;5;10:*.odg=38;5;12:*.pptx=38;5;160:*.ppt=38;5;160:*.odp=38;5;160:*.xlsx=38;5;76:*.xls=38;5;76::*.ods=38;5;76:*.iso=38;5;21:*.img=38;5;21:*.c=38;5;195:*.h=38;5;195:*.cpp=38;5;153:*.py=38;5;185:*.hs=38;5;194:*.html=38;5;231';
export LS_COLORS


if systemctl -q is-active graphical.target && [[ ! $DISPLAY && $XDG_VTNR -eq 1   ]]; then
    exec xinit
fi
