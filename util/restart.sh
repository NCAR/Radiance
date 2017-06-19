#!/bin/sh

PROCESS="$1"
PROCANDARGS=$*

RESULT=`pgrep ${PROCESS}`

if [ "${RESULT:-null}" = null ]; then
    sudo reboot
fi
