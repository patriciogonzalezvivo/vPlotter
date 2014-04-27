#!/bin/sh

# Basics
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install avahi-daemon netatalk git-core build-essential htop screen

# Install WiringPi
cd ~
git clone git://git.drogon.net/wiringPi
cd wiringPi
./build

# Compile vPlotter
cd ~
cd openFrameworks/apps/vPlotter/vPlotter
make

# Install Node.js
cd ~
wget http://node-arm.herokuapp.com/node_latest_armhf.deb
sudo dpkg -i node_latest_armhf.deb

# Install Server
#
cd openFrameworks/apps/vPlotter/vPlotterServer
./install.sh