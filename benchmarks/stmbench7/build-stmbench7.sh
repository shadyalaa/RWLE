#!/bin/sh

if [ $# -eq 0 ] ; then
    echo " === ERROR At the very least, we need the backend name in the first parameter. === "
    exit 1
fi

backend=$1  # e.g.: herwl

htm_retries=5
rot_retries=2

if [ $# -eq 3 ] ; then
    htm_retries=$2 # e.g.: 5
    rot_retries=$3 # e.g.: 2 
fi

cd code;
make clean

rm Defines.common.mk
rm Makefile.flags
rm src/thread/thread.h
rm src/thread/thread_backend.cc
rm src/thread/tm.h

cp ../../../backends/$backend/Defines.common.mk .
cp ../../../backends/$backend/Makefile.flags Makefile.flags
cp ../../../backends/$backend/thread.h src/thread/thread.h
cp ../../../backends/$backend/thread.c src/thread/thread_backend.cc
cp ../../../backends/$backend/tm.h src/thread/tm.h

if [[ $backend == htm-sgl || $backend == tle ]] ; then
	make_command="make -f Makefile HTM_RETRIES=-DHTM_RETRIES=$htm_retries RETRY_POLICY=-DRETRY_POLICY=$rot_retries"
else
	make_command="make -f Makefile HTM_RETRIES=-DHTM_RETRIES=$htm_retries ROT_RETRIES=-DROT_RETRIES=$rot_retries"
fi
$make_command
rc=$?
if [[ $rc != 0 ]] ; then
    echo ""
    echo "=================================== ERROR BUILDING $F - $name ===================================="
    echo ""
    exit 1
fi
