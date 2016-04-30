#!/bin/sh

RO_PERC="5 10 20 30 40 50 70 80 90 99"         #RO percentage
#WL="r w rw"    #Workload type
LT="false true"    #Long traversals
STRUCT="false true"             #Structural modifications
ROOT="false true"  #Modifications to the root
SIZES="small large medium" #"small medium big"  #Size of the graph
DURATION="30000"

PREFIX="params"
if [ "$#" -ne 0 ]; then
   INIT_INDEX=$1
   OUT=$2
else
   INIT_INDEX="1"
   OUT="gen_bench_temp"
fi
TEST_INDEX=${INIT_INDEX}
STRING=""  # Default: "-w w -t true -m true"


for s in ${SIZES};do
for ro in ${RO_PERC}; do
   #for wl in ${WL}; do
      for lt in ${LT}; do
         for st in ${STRUCT};do
            for r in ${ROOT};do
            #STRING="${PREFIX}[${TEST_INDEX}]=\"-w${wl} -t${lt} -m${st} -p${ro} -r false -s m -d 15000 -n\""
            STRING="${PREFIX}[${TEST_INDEX}]=\"-t${lt} -m${st} -p${ro} -r ${r} -s ${s} -d ${DURATION} -n\""
            ((TEST_INDEX++))
            echo ${STRING}  >> ${OUT}
            done
         done #st
      done #lt
      done #sizes
   #done #wl
done #ro

PREFIX="folder"
TEST_INDEX=${INIT_INDEX}

for s in ${SIZES};do
for ro in ${RO_PERC}; do
   #for wl in ${WL}; do
      for lt in ${LT}; do
         for st in ${STRUCT};do
         for r in ${ROOT};do
            STRING="${PREFIX}[${TEST_INDEX}]=\"benchmarks/stmbench7/code\""
            ((TEST_INDEX++))
            echo ${STRING}  >> ${OUT}
            done #root
            done #sizes
         done #st
      done #lt
   #done #wl
done #ro

PREFIX="benchmarks"
TEST_INDEX=${INIT_INDEX}

for s in ${SIZES};do
for ro in ${RO_PERC}; do
   #for wl in ${WL}; do
      for lt in ${LT}; do
         for st in ${STRUCT};do
         for r in ${ROOT};do
            STRING="${PREFIX}[${TEST_INDEX}]=\"sb7_tt\""
            ((TEST_INDEX++))
            echo ${STRING}  >> ${OUT}
            done #root
            done      #size
         done #st
      done #lt
   #done #wl
done #ro

PREFIX="bStr"
TEST_INDEX=${INIT_INDEX}

for s in ${SIZES};do
for ro in ${RO_PERC}; do
   #for wl in ${WL}; do
      for lt in ${LT}; do
         for st in ${STRUCT};do
         for r in ${ROOT};do
            #STRING="${PREFIX}[${TEST_INDEX}]=\"stmb7_w${wl}_t${lt}_m${st}_p${ro}_rfalse_sm_d15000\""
            STRING="${PREFIX}[${TEST_INDEX}]=\"stmb7_t${lt}_m${st}_p${ro}_r${r}_s${s}_d${DURATION}\""
            ((TEST_INDEX++))
            echo ${STRING}  >> ${OUT}
            done #root
            done      #size
         done #st
      done #lt
   #done #wl
done #ro

echo ${TEST_INDEX}
