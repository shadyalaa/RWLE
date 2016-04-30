backend=$1 # e.g: herwl
htm_retries=$2 # e.g.: 5
rot_retries=$3 # e.g.: 2 

cp backends/$backend/tm.h code/

cd code;

# gcc flags used for both debug and opt builds
CXXFLAGS="-g -O2 -w -L/home/shady/lib"
LDFLAGS="-lboost_system -mhtm -pthread -lm -ltcmalloc -pthread"

g++ -DHTM_RETRIES=$htm_retries -DRETRY_POLICY=$rot_retries -DROT_RETRIES=$rot_retries $CXXFLAGS memory.cc pair.cc list.cc hashtable.cc tpcc.cc tpccclient.cc tpccgenerator.cc tpcctables.cc tpccdb.cc clock.cc randomgenerator.cc stupidunit.cc -o tpcc $LDFLAGS -lstdc++
