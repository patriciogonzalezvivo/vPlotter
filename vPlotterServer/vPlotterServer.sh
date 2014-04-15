#!/bin/bash

SERVER_PATH=/home/pi/openframeworks/apps/vPlotter/vPlotterServer
SERVER_DB=$SERVER_PATH/data/db/
SERVER_JS=$SERVER_PATH/server.js
USER=pi
OUT=$SERVER_PATH/nodejs.log

case "$1" in

start)
  echo "Openning GPIOS 4, 17, 18, 22 and 23"
  sudo -u $USER gpio export 4 OUT
  sudo -u $USER gpio export 17 OUT
  sudo -u $USER gpio export 18 OUT
  sudo -u $USER gpio export 22 OUT
  sudo -u $USER gpio export 23 OUT
  echo "Starting Node.js and MongoDB: $SERVER_DB"
  sudo -u $USER mongod --dbpath $SERVER_DB&
	sudo -u $USER node $SERVER_JS > $OUT 2>$OUT &
	;;

stop)
	killall node
  killall mongodb
	;;

*)
	echo "usage: $0 (start|stop)"
esac

exit 0
