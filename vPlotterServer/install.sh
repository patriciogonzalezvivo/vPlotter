#!/bin/sh

# Install Server
chmod 755 vPlotterServer.sh
sudo cp vPlotterServer.sh /etc/init.d
sudo update-rc.d vPlotterServer.sh defaults
