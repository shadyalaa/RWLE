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

export "LD_LIBRARY_PATH=/home/shady/herwl/svn/code/benchmarks/kyotodb/kyotocabinet/lib/"

backends[1]="herwl"
backends[2]="tle"
backends[3]="rwl"
backends[4]="herwl-lazy"
backends[5]="mutexes"
backends[6]="orig"
backends[7]="brlock"

params[1]="-it 1 -capcnt 262487 -capsiz 29695255 1000000"
benchmarks[1]="u5-wicked-capcnt262487-capsiz29695255-runm1000000"

wait_until_finish() {
    pid3=$1
    echo "process is $pid3"
    LIMIT=5
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

for c in 7
do
        htm_retries="0"
        rot_retries="0"
	if [[ $c == 1 ]]; then
		htm_retries="5"
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
                        cd benchmarks/kyotodb
			cd kyotocabinet-${backends[$c]}
			make clean; make HTM_RETRIES=-DHTM_RETRIES=$h RETRY_POLICY=-DRETRY_POLICY=$r ROT_RETRIES=-DROT_RETRIES=$r; make install
			for b in 1
			do
        			for t in  1 4 8 16 32 64 80
       				do
            				for a in 1 2 3 4 5 #6 7 8 9 10 #11 12 13 14 15 16 17 18 19 20
            				do
                				echo "${benchmarks[$b]} | ${backends[$c]}-$h-$r | threads $t | attempt $a"
						echo "./kccachetest wicked -th $t ${params[$b]} > $runsdir/${benchmarks[$b]}-${backends[$c]}-$h-$r-$t-$a.data"
						./kccachetest wicked -th $t ${params[$b]} > $runsdir/${benchmarks[$b]}-${backends[$c]}-$h-$r-$t-$a.data &
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
