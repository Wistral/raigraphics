FROM ubuntu:16.04

RUN apt-get update -qq && apt-get -y -qq upgrade

# essentials
RUN apt-get install -y -qq build-essential git software-properties-common

# cmake
RUN apt-get install -y -qq cmake

# g++ 
RUN add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y -qq g++-7 g++-6

# other dependencies 
RUN apt-get install -y -qq libeigen3-dev libboost-all-dev libglew-dev libglm-dev libsdl2-dev \
libassimp-dev libsoil-dev libsdl2-ttf-dev

# raicommon dependencies
RUN apt-get install -y -qq gnuplot5

# install raicommon
RUN cd /tmp/ && \
    git clone https://github.com/eastskykang/raicommon.git

RUN mkdir /tmp/raicommon/build && \
    cd /tmp/raicommon/build && \
    cmake ../ && \
    make install -j4

# raigraphics dependencies
RUN apt-get install -y -qq libsdl2-image-dev ffmpeg

# pip install (for python)
RUN apt-get -y install python3-pip

RUN pip3 install --upgrade pip

# cython install
RUN pip3 install cython twine