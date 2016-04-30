#!/bin/sh


benchmarks[1]="-w r -t false -m false"
benchmarks[2]="-w rw -t false -m false"
benchmarks[3]="-w w -t false -m false"
benchmarks[4]="-w r -t true -m false"
benchmarks[5]="-w rw -t true -m false"
benchmarks[6]="-w w -t true -m false"
benchmarks[7]="-w r -t false -m true"
benchmarks[8]="-w rw -t false -m true"
benchmarks[9]="-w w -t false -m true"
benchmarks[10]="-w r -t true -m true"
benchmarks[11]="-w rw -t true -m true"
benchmarks[12]="-w w -t true -m true"

bStr[1]="r-f-f"
bStr[2]="rw-f-f"
bStr[3]="w-f-f"
bStr[4]="r-t-f"
bStr[5]="rw-t-f"
bStr[6]="w-t-f"
bStr[7]="r-f-t"
bStr[8]="rw-f-t"
bStr[9]="w-f-t"
bStr[10]="r-t-t"
bStr[11]="rw-t-t"
bStr[12]="w-t-t"

opComb[1]="-o 01p90 -o 11p10"
opComb[2]="-o 07p30 -o 15p70"
opComb[3]="-o 43p100"
opComb[4]="-o 01p60 -o 07p30 -o 20p10"
opComb[5]="-o 02p100"

wait_until_finish() {
    pid3=$1
    echo "process is $pid3"
    LIMIT=180
    for ((j = 0; j < $LIMIT; ++j)); do
        kill -s 0 $pid3
        rc=$?
        if [[ $rc != 0 ]] ; then
            echo "returning"
            return;
        fi
        sleep 1s
    done
    kill $pid3
}

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

for b in {1..3}
do
    cd $DIR;
    cd code;
    RUNONLYONEOP=0
    USEPROBCOMB=1 #use combination of operations with probabilities
    if [[ $RUNONLYONEOP == 1  ]] ; then
	    for op in {0..44}
	    do
	    echo "${bStr[$b]} | backend $1 | htm_retries $4 | threads $2 | attempt $3 | retry_policy $4 | operation $op"
	    ./sb7_tt -r false -s m -d 150000 ${benchmarks[$b]} -n $2 -o $op > ../../../auto-results/$1-${bStr[$b]}-retries$4-th$2-att$3-op$op.data &
	    pid3=$!; wait_until_finish $pid3; wait $pid3; rc=$?
	    if [ $rc -eq 13 ] ; then
	        echo "Error within: backend $1 | ${bStr[$b]} | htm_retries $4 | threads $2 | attempt $3" >> ../../../auto-results/slow.out
	    elif [[ $rc != 0 ]] ; then
	        echo "Error within: backend $1 | ${bStr[$b]} | htm_retries $4 | threads $2 | attempt $3" >> ../../../auto-results/error.out
	    fi
    	done
   	elif [[ $USEPROBCOMB == 1  ]] ; then
   		for comb in {3..5}
	    do
	    echo "${bStr[$b]} | backend $1 | htm_retries $4 | threads $2 | attempt $3 | retry_policy $4 | ${opComb[$comb]}"
	    ./sb7_tt -r false -s m -d 150000 ${benchmarks[$b]} -n $2 ${opComb[$comb]} > ../../../auto-results/$1-${bStr[$b]}-retries$4-th$2-att$3-comb$comb.data &
	    pid3=$!; wait_until_finish $pid3; wait $pid3; rc=$?
	    if [ $rc -eq 13 ] ; then
	        echo "Error within: backend $1 | ${bStr[$b]} | htm_retries $4 | threads $2 | attempt $3" >> ../../../auto-results/slow.out
	    elif [[ $rc != 0 ]] ; then
	        echo "Error within: backend $1 | ${bStr[$b]} | htm_retries $4 | threads $2 | attempt $3" >> ../../../auto-results/error.out
	    fi
	    done
   	else
   		echo "${bStr[$b]} | backend $1 | htm_retries $4 | threads $2 | attempt $3 | retry_policy $4"
	    ./sb7_tt -r false -s m -d 150000 ${benchmarks[$b]} -n $2 > ../../../auto-results/$1-${bStr[$b]}-retries$4-th$2-att$3.data &
	    pid3=$!; wait_until_finish $pid3; wait $pid3; rc=$?
	    if [ $rc -eq 13 ] ; then
	        echo "Error within: backend $1 | ${bStr[$b]} | htm_retries $4 | threads $2 | attempt $3" >> ../../../auto-results/slow.out
	    elif [[ $rc != 0 ]] ; then
	        echo "Error within: backend $1 | ${bStr[$b]} | htm_retries $4 | threads $2 | attempt $3" >> ../../../auto-results/error.out
	    fi
	fi
done
