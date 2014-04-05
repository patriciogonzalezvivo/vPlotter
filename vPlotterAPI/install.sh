#!/bin/sh

# R&W priviledge over text files
sudo chmod a+w *.txt
sudo chmod a+w *.csv

# Give PHP priviledge over the data folder
sudo chown www-data data
sudo chgrp www-data data
