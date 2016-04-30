#!/bin/sh

benchmarks[1]="redblacktree"
benchmarks[2]="redblacktree"
benchmarks[3]="redblacktree"
benchmarks[4]="redblacktree"
benchmarks[5]="hashmap"
benchmarks[6]="hashmap"
benchmarks[7]="hashmap"
benchmarks[8]="hashmap"
benchmarks[9]="skiplist"
benchmarks[10]="skiplist"
benchmarks[11]="skiplist"
benchmarks[12]="skiplist"
benchmarks[13]="linkedlist"
benchmarks[14]="linkedlist"
benchmarks[15]="linkedlist"
benchmarks[16]="linkedlist"

bStr[1]="rbt-l-w"
bStr[2]="rbt-l-r"
bStr[3]="rbt-s-w"
bStr[4]="rbt-s-r"
bStr[5]="hm-l-r"
bStr[6]="hm-l-w"
bStr[7]="hm-s-r"
bStr[8]="hm-s-w"
bStr[9]="sl-l-w"
bStr[10]="sl-l-r"
bStr[11]="sl-s-w"
bStr[12]="sl-s-r"
bStr[13]="ll-l-w"
bStr[14]="ll-l-r"
bStr[15]="ll-s-w"
bStr[16]="ll-s-r"

params[1]="-d 15000000 -i 1048576 -r 1000000 -u 90 -a 1 -n"
params[2]="-d 25000000 -i 1048576 -r 1000000 -u 10 -a 1 -n"
params[3]="-d 15000000 -i 1024 -r 1000000 -u 90 -a 1 -n"
params[4]="-d 25000000 -i 1024 -r 1000000 -u 10 -a 1 -n"
params[5]="-d 50000000 -b 1048576 -i 1048576 -r 1000000 -u 10 -n "
params[6]="-d 35000000 -b 1048576 -i 1048576 -r 1000000 -u 90 -n "
params[7]="-d 150000000 -b 64 -i 64 -r 1000000 -u 10 -n "
params[8]="-d 100000000 -b 64 -i 64 -r 1000000 -u 90 -n "
params[9]="-d 750000 -i 104857 -r 1000000 -u 90 -n "
params[10]="-d 1000000 -i 104857 -r 1000000 -u 10 -n "
params[11]="-d 50000000 -i 512 -r 1000000 -u 90 -n"
params[12]="-d 80000000 -i 512 -r 1000000 -u 10 -n"
params[13]="-d 200000 -i 10485 -r 1000000 -u 90 -n "
params[14]="-d 400000 -i 10485 -r 1000000 -u 10 -n "
params[15]="-d 50000000 -i 64 -r 1000000 -u 90 -n"
params[16]="-d 100000000 -i 64 -r 1000000 -u 10 -n "

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

for b in {1..16}
do
    cd $DIR;
    cd ${benchmarks[$b]};
    echo "${bStr[$b]} | backend $1 | htm_retries $4 | threads $2 | attempt $3 | retry_policy $4"
    ./${benchmarks[$b]} ${params[$b]}$2 > ../../../auto-results/$1-${bStr[$b]}-retries$4-th$2-att$3.data &
    pid3=$!; wait_until_finish $pid3; wait $pid3; rc=$?
    if [ $rc -eq 13 ] ; then
        echo "Error within: backend $1 | ${bStr[$b]} | htm_retries $4 | threads $2 | attempt $3" >> ../../../auto-results/slow.out
    elif [[ $rc != 0 ]] ; then
        echo "Error within: backend $1 | ${bStr[$b]} | htm_retries $4 | threads $2 | attempt $3" >> ../../../auto-results/error.out
    fi
done
