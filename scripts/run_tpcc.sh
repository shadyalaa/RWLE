#!/bin/bash

workspace=$1
if [ $4 == "no_random" ];then
resultsdir=$2
runsdir=$resultsdir/runs
else
resultsdir=$2/$RANDOM
mkdir -p $resultsdir/runs
runsdir=$resultsdir/runs
mkdir $resultsdir/plots
mkdir $resultsdir/summary
echo $3 > $resultsdir/desc.txt
fi

export "LD_LIBRARY_PATH=/home/shady/lib"


backends[1]="herwl"
backends[2]="tle"
backends[3]="rwl"
backends[4]="htm-sgl"
backends[5]="sgl"
backends[6]="seq"
backends[7]="brlock"

params[1]="-t 5 -w 1 -s 0 -d 1 -o 99 -p 0 -r 0 -n"
params[2]="-t 5 -w 1 -s 0 -d 25 -o 50 -p 25 -r 0 -n"
params[3]="-t 5 -w 1 -s 0 -d 5 -o 90 -p 5 -r 0 -n"
params[4]="-t 5 -w 1 -s 0 -d 45 -o 10 -p 45 -r 0 -n"

benchmarks[1]="u1-w1-o-d"
benchmarks[2]="u50-w1-o-d-p"
benchmarks[3]="u10-w1-o-d-p"
benchmarks[4]="u90-w1-o-d-p"

wait_until_finish() {
    pid3=$1
    echo "process is $pid3"
    LIMIT=15
    for ((j = 0; j < $LIMIT; ++j)); do
        kill -s 0 $pid3
        rc=$?
        if [[ $rc != 0 ]] ; then
            echo "returning"
            return;
        fi
        sleep 1s
    done
    kill -9 $pid3
}

for b in 1 2 3 
do
for c in 7 
do
        htm_retries="0"
        rot_retries="0"
	if [[ $c == 1 ]]; then
		htm_retries="5 0"
		rot_retries="5"
	fi
	if [[ $c == 2 ]]; then
                htm_retries="5"
                rot_retries="0"
        fi
	for h in $htm_retries
	do
		for r in $rot_retries
		do
			cd $workspace;
                        cd benchmarks/tpcc
                        bash build-tpcc.sh ${backends[$c]} $h $r
                        cd code
			#for b in 3 
			#do
        			for t in  1 4 8 16 32 64 80
       				do
            				for a in 1 2 3 4 5 #6 7 8 9 10 #11 12 13 14 15 16 17 18 19 20
            				do
                				echo "${benchmarks[$b]} | ${backends[$c]}-$h-$r | threads $t | attempt $a"
						./tpcc ${params[$b]} $t > $runsdir/${benchmarks[$b]}-${backends[$c]}-$h-$r-$t-$a.data &	
						pid3=$!
						wait_until_finish $pid3
						wait $pid3
                				rc=$?
                				if [[ $rc != 0 ]] ; then
                    					echo "Error within: | ${benchmarks[$b]} | ${backends[$c]}-$h-$r | threads $t | attempt $a" >> $runsdir/error.out
               					fi
	    				done
        			done
			done
		done
	done
done


exit 0
