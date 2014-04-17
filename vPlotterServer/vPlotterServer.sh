#!/bin/bash

SERVER_PATH=/home/pi/openframeworks/apps/vPlotter/vPlotterServer
#SERVER_PATH=/Users/Patricio/Desktop/openFrameworks/apps/vPlotter/vPlotterServer
SERVER_JS=server.js
#USER=patricio
USER=pi
OUT=nodejs.log

case "$1" in

start)
  echo "Openning GPIOS 4, 17, 18, 22 and 23"
  sudo -u $USER gpio export 4 OUT
  sudo -u $USER gpio export 17 OUT
  sudo -u $USER gpio export 18 OUT
  sudo -u $USER gpio export 22 OUT
  sudo -u $USER gpio export 23 OUT
  echo "Starting Node.js"
  cd $SERVER_PATH
  sudo -u $USER node $SERVER_JS >> $OUT 2>>$OUT&
	;;

stop)
	killall node
	;;

*)
	echo "usage: $0 (start|stop)"
esac

exit 0
