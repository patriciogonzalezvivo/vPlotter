#!/bin/sh

gpio export 4 OUT
gpio export 17 OUT
gpio export 18 OUT
gpio export 23 OUT
gpio export 24 OUT

while true; do
	# Check if is the queue is empty
	#
	nLines=$(wc -m queue.txt | awk '{print $1}')
        if [ "$nLines" -eq "0" ]; then
                sleep 5;
        else
		# Do first element
		#
		file=$(head -n 1 queue.txt)
		options=`cat config.txt`
		echo $file > print.txt
		echo vPlotter -x $options $file		
		./vPlotter -x $options $file
		sed -i '1d' print.txt

		# Extract the first element
		sed -i '1d' queue.txt
		sed -i '1d' queue.csv
	fi
done
