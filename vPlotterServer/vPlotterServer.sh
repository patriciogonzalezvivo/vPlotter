#!/bin/bash
# /etc/init.d/vPlotterServer.sh

### BEGIN INIT INFO
# Provides:          vPlotterServer
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: vPlotter web server
# Description:       Open GPIO's and run the node.js server that administrate vPlotter
### END INIT INFO

SERVER_PATH=/home/pi/openFrameworks/apps/vPlotter/vPlotterServer
SERVER_JS=server.js
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
