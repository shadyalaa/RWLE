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

export "LD_LIBRARY_PATH=/home/shady/lib/"


backends[1]="herwl"
backends[2]="tle"
backends[3]="rwl"
backends[4]="herwl-lazy"
backends[5]="sgl"
backends[6]="herwl-nostarv"
backends[7]="brlock"
backends[8]="herwl-fixed"

benchmarks[1]="hm-u0-i512-d1000000-r1000000"
benchmarks[2]="hm-u0-i32768-d1000000-r1000000"
benchmarks[3]="hm-u1-i512-d1000000-r1000000"
benchmarks[4]="hm-u1-i32768-d1000000-r1000000"
benchmarks[5]="hm-u5-i512-d1000000-r1000000"
benchmarks[6]="hm-u5-i32768-d1000000-r1000000"
benchmarks[7]="hm-u10-i512-d1000000-r1000000"
benchmarks[8]="hm-u10-i32768-d1000000-r1000000"
benchmarks[9]="hm-u20-i512-d1000000-r1000000"
benchmarks[10]="hm-u20-i32768-d1000000-r1000000"
benchmarks[11]="rbt-u0-d1000000-i32768-r1000000-a1"
benchmarks[12]="rbt-u1-d1000000-i32768-r1000000-a1"
benchmarks[13]="rbt-u5-d1000000-i32768-r1000000-a1"
benchmarks[14]="rbt-u10-d1000000-i32768-r1000000-a1"
benchmarks[15]="rbt-u20-d1000000-i32768-r1000000-a1"
benchmarks[16]="rbt-u0-d1000000-i512-r1000000-a1"
benchmarks[17]="rbt-u1-d1000000-i512-r1000000-a1"
benchmarks[18]="rbt-u5-d1000000-i512-r1000000-a1"
benchmarks[19]="rbt-u10-d1000000-i512-r1000000-a1"
benchmarks[20]="rbt-u20-d1000000-i512-r1000000-a1"
benchmarks[21]="hm-u50-i512-d1000000-r1000000"
benchmarks[22]="hm-u90-i512-d1000000-r1000000"
benchmarks[23]="hm-u100-i512-d1000000-r1000000"
benchmarks[24]="hm-u50-i32768-d1000000-r1000000"
benchmarks[25]="hm-u90-i32768-d1000000-r1000000"
benchmarks[26]="hm-u100-i32768-d1000000-r1000000"
benchmarks[28]="rbt-u50-d1000000-i32768-r1000000-a1"
benchmarks[28]="rbt-u90-d1000000-i32768-r1000000-a1"
benchmarks[29]="rbt-u50-d1000000-i512-r1000000-a1"
benchmarks[30]="rbt-u90-d1000000-i512-r1000000-a1"

benchmarks[31]="high-capacity-low-contetion-u1"
benchmarks[32]="high-capacity-low-contetion-u5"
benchmarks[33]="high-capacity-low-contetion-u10"
benchmarks[34]="high-capacity-low-contetion-u50"
benchmarks[35]="high-capacity-low-contetion-u90"

benchmarks[36]="high-capacity-high-contetion-u1"
benchmarks[37]="high-capacity-high-contetion-u5"
benchmarks[38]="high-capacity-high-contetion-u10"
benchmarks[39]="high-capacity-high-contetion-u50"
benchmarks[40]="high-capacity-high-contetion-u90"

benchmarks[41]="low-capacity-low-contetion-u1"
benchmarks[42]="low-capacity-low-contetion-u5"
benchmarks[43]="low-capacity-low-contetion-u10"
benchmarks[44]="low-capacity-low-contetion-u50"
benchmarks[45]="low-capacity-low-contetion-u90"

benchmarks[46]="low-capacity-high-contetion-u1"
benchmarks[47]="low-capacity-high-contetion-u5"
benchmarks[48]="low-capacity-high-contetion-u10"
benchmarks[49]="low-capacity-high-contetion-u50"
benchmarks[50]="low-capacity-high-contetion-u90"

benchmarks[51]="medium-capacity-medium-contetion-u10"
benchmarks[52]="medium-capacity-medium-contetion-u50"
benchmarks[53]="medium-capacity-medium-contetion-u90"


bStr[1]="hashmap"
bStr[2]="hashmap"
bStr[3]="hashmap"
bStr[4]="hashmap"
bStr[5]="hashmap"
bStr[6]="hashmap"
bStr[7]="hashmap"
bStr[8]="hashmap"
bStr[9]="hashmap"
bStr[10]="hashmap"
bStr[11]="redblacktree"
bStr[12]="redblacktree"
bStr[13]="redblacktree"
bStr[14]="redblacktree"
bStr[15]="redblacktree"
bStr[16]="redblacktree"
bStr[17]="redblacktree"
bStr[18]="redblacktree"
bStr[19]="redblacktree"
bStr[20]="redblacktree"
bStr[21]="hashmap"
bStr[22]="hashmap"
bStr[23]="hashmap"
bStr[24]="hashmap"
bStr[25]="hashmap"
bStr[26]="hashmap"
bStr[27]="redblacktree"
bStr[28]="redblacktree"
bStr[29]="redblacktree"
bStr[30]="redblacktree"

bStr[31]="hashmap"
bStr[32]="hashmap"
bStr[33]="hashmap"
bStr[34]="hashmap"
bStr[35]="hashmap"

bStr[36]="hashmap"
bStr[37]="hashmap"
bStr[38]="hashmap"
bStr[39]="hashmap"
bStr[40]="hashmap"

bStr[41]="hashmap"
bStr[42]="hashmap"
bStr[43]="hashmap"
bStr[44]="hashmap"
bStr[45]="hashmap"

bStr[46]="hashmap"
bStr[47]="hashmap"
bStr[48]="hashmap"
bStr[49]="hashmap"
bStr[50]="hashmap"

bStr[51]="hashmap"
bStr[52]="hashmap"
bStr[53]="hashmap"

params[1]="-u0 -i512 -d1000000 -r1000000 -n"
params[2]="-u0 -i32768 -d1000000 -r1000000 -n"
params[3]="-u1 -i512 -d1000000 -r1000000 -n"
params[4]="-u1 -i32768 -d1000000 -r1000000 -n"
params[5]="-u5 -i512 -d1000000 -r1000000 -n"
params[6]="-u5 -i32768 -d1000000 -r1000000 -n"
params[7]="-u10 -i512 -d1000000 -r1000000 -n"
params[8]="-u10 -i32768 -d1000000 -r1000000 -n"
params[9]="-u20 -i512 -d1000000 -r1000000 -n"
params[10]="-u20 -i32768 -d1000000 -r1000000 -n"
params[11]="-u0 -d 1000000 -i 32768 -r 1000000 -a 1 -n"
params[12]="-u1 -d 1000000 -i 32768 -r 1000000 -a 1 -n"
params[13]="-u5 -d 1000000 -i 32768 -r 1000000 -a 1 -n"
params[14]="-u10 -d 1000000 -i 32768 -r 1000000 -a 1 -n"
params[15]="-u20 -d 1000000 -i 32768 -r 1000000 -a 1 -n"
params[16]="-u0 -d 1000000 -i 512 -r 1000000 -a 1 -n"
params[17]="-u1 -d 1000000 -i 512 -r 1000000 -a 1 -n"
params[18]="-u5 -d 1000000 -i 512 -r 1000000 -a 1 -n"
params[19]="-u10 -d 1000000 -i 512 -r 1000000 -a 1 -n"
params[20]="-u20 -d 1000000 -i 512 -r 1000000 -a 1 -n"
params[21]="-u50 -i512 -d1000000 -r1000000 -n"
params[22]="-u90 -i512 -d1000000 -r1000000 -n"
params[23]="-u100 -i512 -d1000000 -r1000000 -n"
params[24]="-u50 -i32768 -d1000000 -r1000000 -n"
params[25]="-u90 -i32768 -d1000000 -r1000000 -n"
params[26]="-u100 -i32768 -d1000000 -r1000000 -n"
params[27]="-u50 -d 1000000 -i 32768 -r 1000000 -a 1 -n"
params[28]="-u90 -d 1000000 -i 32768 -r 1000000 -a 1 -n"
params[29]="-u50 -d 1000000 -i 512 -r 1000000 -a 1 -n"
params[30]="-u90 -d 1000000 -i 512 -r 1000000 -a 1 -n"

params[31]="-u1 -i2000000 -b10000 -r2000000 -d100000 -n"
params[32]="-u5 -i2000000 -b10000 -r2000000 -d100000 -n"
params[33]="-u10 -i2000000 -b10000 -r2000000 -d100000 -n"
params[34]="-u50 -i2000000 -b10000 -r2000000 -d100000 -n"
params[35]="-u90 -i2000000 -b10000 -r2000000 -d100000 -n"

params[36]="-u1 -i200 -b1 -r200 -d100000 -n"
uurams[37]="-u5 -i200 -b1 -r200 -d100000 -n"
params[38]="-u10 -i200 -b1 -r200 -d100000 -n"
params[39]="-u50 -i200 -b1 -r200 -d100000 -n"
params[40]="-u90 -i200 -b1 -r200 -d100000 -n"

params[41]="-u1 -d 5000000 -i50000 -b1000 -r5000 -n"
params[42]="-u5 -d 5000000 -i50000 -b1000 -r5000 -n"
params[43]="-u10 -d 5000000 -i50000 -b1000 -r5000 -n"
params[44]="-u50 -d 5000000 -i50000 -b1000 -r5000 -n"
params[45]="-u90 -d 5000000 -i50000 -b1000 -r5000 -n"

params[46]="-u1 -d 1000000 -b1 -i50 -r50 -n"
params[47]="-u5 -d 1000000 -b1 -i50 -r50 -n"
params[48]="-u10 -d 1000000 -b1 -i50 -r50 -n"
params[49]="-u50 -d 1000000 -b1 -i50 -r50 -n"
params[50]="-u90 -d 1000000 -b1 -i50 -r50 -n"


params[51]="-u10 -d 5000000 -b500 -i50000 -r50000 -n"
params[52]="-u50 -d 1000000 -b500 -i50000 -r50000 -n"
params[53]="-u90 -d 1000000 -b500 -i50000 -r50000 -n"

wait_until_finish() {
    pid3=$1
    echo "process is $pid3"
    LIMIT=30
    for ((j = 0; j < $LIMIT; ++j)); do
        kill -s 0 $pid3
        rc=$u
        if [[ $rc != 0 ]] ; then
            echo "returning"
            return;
        fi
        sleep 1s
    done
    kill -9 $pid3
}

for b in 36 #44 49 #39 44 49 #38 39 40  #31 33 35 36 38 40 41 43 45 46 48 50 #31 33 35 #41 43 45 #31 33 34 35 36 38 39 40 41 43 44 45 46 48 49 50 #13 14 15 16 17 18 19 20 27 28 39 30 #21 22 24 25 3 4 5 6 7 8 9 10 #11 12 13 14 15 16 17 18 19 20
do
for c in 1 6
do
        htm_retries="0"
        rot_retries="0"
	if [[ $c == 1 || $c == 4 || $c == 6 || $c == 8 ]]; then
		htm_retries="5"
		rot_retries="0"
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
			bash build-all.sh ${backends[$c]} $h $r
			cd benchmarks/datastructures
			#for b in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 #1 2 3 4 5 6
			#do
				cd ${bStr[$b]};
        			for t in  1 4 8 16 32 64 80
       				do
					#for bu in 1 32 128 512 1024
					#do
            				for a in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
            				do
                				echo "${benchmarks[$b]} | ${backends[$c]}-$h-$r | threads $t | attempt $a"
                				./${bStr[$b]} ${params[$b]}$t > $runsdir/${benchmarks[$b]}-${backends[$c]}-$h-$r-$t-$a.data 2> $runsdir/${benchmarks[$b]}-${backends[$c]}-$h-$r-$t-$a.err & 
						pid3=$!
						wait_until_finish $pid3
						wait $pid3
                				rc=$?
                				if [[ $rc != 0 ]] ; then
                    					echo "Error within: | ${benchmarks[$b]} | ${backends[$c]}-$h-$r | threads $t | attempt $a" >> $runsdir/error.out
               					fi
	    				done
					#done
        			done
				cd ..
			done
		done
	done
done


exit 0
