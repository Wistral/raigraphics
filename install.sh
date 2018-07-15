echo 'export RAI_GRAPHICS_OPENGL_ROOT='$PWD'' >> ~/.bashrc
sudo apt-get install -qq libglew-dev libsdl2-dev libglm-dev libassimp-dev libsoil-dev libsdl2-ttf-dev

### Check Ubuntu version
version=$(lsb_release -r | awk '{ print $2 }')
yrelease=$( echo "$version" | cut -d. -f1 )
mrelease=$( echo "$version" | cut -d. -f2 )

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