#!/bin/sh

PROCESS="$1"
PROCANDARGS=$*

sleep 21

RESULT=`pgrep ${PROCESS}`

if [ "${RESULT:-null}" = null ]; then
    sudo reboot
fi
