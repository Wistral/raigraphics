#!/usr/bin/env bash
set -xe

APT_GET_FLAGS="-y -qq"

version=$(lsb_release -r | awk '{ print $2 }')
yrelease=$( echo "$version" | cut -d. -f1 )
mrelease=$( echo "$version" | cut -d. -f2 )

##########################

sudo apt-get install $APT_GET_FLAGS \
    libglew-dev \
    libsdl2-dev \
    libglm-dev \
    libassimp-dev \
    libsoil-dev \
    libsdl2-ttf-dev \
    libsdl2-image-dev

##########################

# ffmpeg
if [ "$yrelease" -eq "16" ]; then
    sudo apt-get install $APT_GET_FLAGS ffmpeg
else
    if [ "$yrelease" -eq "14" ]; then
        sudo add-apt-repository ppa:mc3man/trusty-media
	sudo apt-get update $APT_GET_FLAGS
	sudo apt-get dist-upgrade $APT_GET_FLAGS
	sudo apt-get install $APT_GET_FLAGS ffmpeg
    fi
fi