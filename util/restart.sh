#!/bin/sh

PROCESS="$1"
PROCANDARGS=$*

sleep 20

RESULT=`pgrep ${PROCESS}`

if [ "${RESULT:-null}" = null ]; then
    sudo reboot
fi
