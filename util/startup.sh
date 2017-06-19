#!/bin/sh
# Creates a named pipe in /tmp/ for reading heartbeat information,
# then starts the radiance process

sudo service watchdog start

mkfifo /tmp/stdout_log
sudo /home/pi/RADIANCE-main/radiance 2>1 | tee /tmp/stdout_log
