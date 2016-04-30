## Hardware Read-Write Lock Elision ##

**Requirements:**

* Install: tcmalloc (needed for all backends)


This source code has two directories: 
1. *backends*, which includes impelementation of different synchronization techniques and 
2. *benchmarks*, which includes the benchmarks used in the experiments of the paper.


**backends:**

* herwl: Implemenetation of Hardware Read-Write Lock Elision

* tle: Power8 HTM used with a single global lock fallback

* rwl: read-write lock of pthreads

* sgl: single global lock

* brlock: Implementation of Big Reader-Writer Lock



**benchmarks:**

* datastructures: Concurrent data-structures (linked-list, red-black tree, hash map)

* tpcc: in-memory port of TPC-C

* stmbench7

**usage:**

To compile a benchmark, go to its folder under the benchmarks folder and use the "build-XXX.sh" script.
 
These scripts expect three parameters (in this order):

1. the backend name, corresponding to the folder under "backends"

2. the number of retries for HTM usage, (default value 5)

3. the number of retries for ROT usage after falling back from normal HTM (default value 2), must be >= 1 
   or in case of TLE: the retry policy to adopt in case of HTM persistent aborts, i.e., how the number of retries is updated upon a 
   persistent abort, which may be omitted (possible values: 0, do not retry in htm and resort to the fallback path; 1, decrease by
   one; 2, divide by two)

*NB*: For brlock, sgl and rwl the number of retries is not needed.

Hence, a common usage shall be: 
```
bash build-XXX.sh herwl 5 5
bash build-XXX.sh tle 5 0
bash build-XXX.sh brlock
```


