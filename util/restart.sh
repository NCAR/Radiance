#!/bin/sh

PROCESS="$1"
PROCANDARGS=$*

sleep 21

RESULT=`pgrep ${PROCESS}`

if [ "${RESULT:-null}" = null ]; then
    sudo reboot
fi

FILENAME=$(ls /mnt/slcdrive/ -t | head -n1)
FILESIZE=$(stat --printf="%s" /mnt/slcdrive/$FILENAME)
# echo $FILESIZE
sleep 6
FILENAME=$(ls /mnt/slcdrive/ -t | head -n1)
FILESIZE2=$(stat --printf="%s" /mnt/slcdrive/$FILENAME)
#CHANGE=$(($FILESIZE2-$FILESIZE))
# echo $CHANGE
if [ "$FILESIZE" -eq "$FILESIZE2" ]; then
	sudo reboot
fi
