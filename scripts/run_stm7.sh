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

params[1]="-w r -t false -m false"
params[2]="-w rw -t false -m false"
params[3]="-w w -t false -m false"
params[4]="-w r -t true -m false"
params[5]="-w rw -t true -m false"
params[6]="-w w -t true -m false"
params[7]="-t false -m false -p100"
params[8]="-s m -t false -m false -p90"
params[9]="-s m -t false -m false -p50"
params[10]="-s m -t false -m false -p10"
params[11]="-s m -t false -m false -p80"
params[12]="-s m -t false -m false -p50"
params[13]="-t false -m false -p10"
params[14]="-t false -m false -p01"
params[15]="-s s -t false -m false -p99"
params[16]="-s s -t false -m false -p95"
params[17]="-s s -t false -m false -p90"
params[18]="-s s -t false -m false -p80"
params[19]="-s s -t false -m false -p50"
params[20]="-s l -t false -m false -p99"
params[21]="-s l -t false -m false -p95"
params[22]="-s l -t false -m false -p90"
params[23]="-s l -t false -m false -p80"
params[24]="-s l -t false -m false -p50"


benchmarks[1]="r-f-f"
benchmarks[2]="rw-f-f"
benchmarks[3]="w-f-f"
benchmarks[4]="r-t-f"
benchmarks[5]="rw-t-f"
benchmarks[6]="w-t-f"
benchmarks[7]="u0"
benchmarks[8]="u-10"
benchmarks[9]="u-50"
benchmarks[10]="u-90"
benchmarks[11]="m-u20"
benchmarks[12]="m-u50"
benchmarks[13]="u90"
benchmarks[14]="u99"
benchmarks[15]="s-u1"
benchmarks[16]="s-u5"
benchmarks[17]="s-u10"
benchmarks[18]="s-u20"
benchmarks[19]="s-u50"
benchmarks[20]="l-u1"
benchmarks[21]="l-u5"
benchmarks[22]="l-u10"
benchmarks[23]="l-u20"
benchmarks[24]="l-u50"


wait_until_finish() {
    pid3=$1
    echo "process is $pid3"
    LIMIT=10
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
		htm_retries="10 5 0"
		rot_retries="10 5 0"
	fi
	if [[ $c == 2 ]]; then
                htm_retries="10 5"
                rot_retries="0"
        fi
	for h in $htm_retries
	do
		for r in $rot_retries
		do
			cd $workspace;
                        cd benchmarks/stmbench7
                        bash build-stmbench7.sh ${backends[$c]} $h $r
                        cd code
			for b in 8 9 10
			do
        			for t in  1 4 8 16 32 64 80
       				do
            				for a in 1 2 3 4 5 #6 7 8 9 10 #11 12 13 14 15 16 17 18 19 20
            				do
                				echo "${benchmarks[$b]} | ${backends[$c]}-$h-$r | threads $t | attempt $a"
						./sb7_tt -r false -d 5000 ${params[$b]} -n $t > $runsdir/${benchmarks[$b]}-${backends[$c]}-$h-$r-$t-$a.data &	
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
