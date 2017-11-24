echo 'export RAI_GRAPHICS_OPENGL_ROOT='$PWD'' >> ~/.bashrc
sudo apt-get install -qq libglew-dev freeglut3-dev libsdl2-dev libglm-dev glee-dev libsdl2-image-dev libassimp-dev libsoil-dev libfreeimage3 libfreeimage-dev libsdl2-ttf-dev


### graphic
# 3D rendering
sudo apt-get install $APT_GET_FLAGS libglew-dev freeglut3-dev libsdl2-dev libglm-dev glee-dev libsdl2-image-dev libassimp-dev libsoil-dev libfreeimage3 libfreeimage-dev libsdl2-ttf-dev

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