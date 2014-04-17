#!/bin/sh

# Basics
sudo apt-get update
sudo apt-get upgrade
sudo apt-get avahi-daemon netatalk git-core build-essential scons libpcre++-dev xulrunner-dev libboost-dev libboost-program-options-dev libboost-thread-dev libboost-filesystem-dev distcc

# Install Server
chmod 755 vPlotterServer.sh
sudo cp vPlotterServer.sh /etc/init.d
sudo update-rc.d vPlotterServer.sh defaults

# # DistCC
# mkdir $HOME/.distcc/
# echo "ubuntu.local" >> $HOME/.distcc/hosts
# echo "alias rmake='make -j 8 CXX=/usr/lib/distcc/arm-linux-gnueabihf-g++ CC=/usr/lib/distcc/arm-linux-gnueabihf-gcc'" >> ~/.bashrc
#
# # openFrameworks
# cd ~
# curl -O http://www.openframeworks.cc/versions/v0.8.1/of_v0.8.1_linuxarmv6l_release.tar.gz
# mkdir openFrameworks
# tar vxfz of_v0.8.0_linuxarmv6l_release.tar.gz -C openFrameworks --strip-components 1
# cd openFrameworks/scripts/linux/debian_armv6l
# sudo ./install_dependencies.sh
# sed -i 's/VC_IMAGE_TRANSFORM_T/DISPMANX_TRANSFORM_T/g' /home/pi/openFrameworks/libs/openFrameworks/app/ofAppEGLWindow.cpp
# rmake Release -C /home/pi/openFrameworks/libs/openFrameworksCompiled/project

# WiringPi
#cd ~
#git clone git://git.drogon.net/wiringPi
#cd wiringPi
#./build

# Node.js
#cd ~
#wget http://node-arm.herokuapp.com/node_latest_armhf.deb
#sudo dpkg -i node_latest_armhf.deb

# MongoDB
#cd ~
#git clone https://github.com/skrabban/mongo-nonx86
#cd mongo-nonx86
#scons
#sudo scons --prefix=/opt/mongo install
#PATH=$PATH:/opt/mongo/bin/
#export PATH
