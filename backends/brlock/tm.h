#ifndef TM_H
#define TM_H 1

#  include <stdio.h>


#ifndef REDUCED_TM_API

#  define MAIN(argc, argv)              int main (int argc, char** argv)
#  define MAIN_RETURN(val)              return val

#  define GOTO_SIM()                    /* nothing */
#  define GOTO_REAL()                   /* nothing */
#  define IS_IN_SIM()                   (0)

#  define SIM_GET_NUM_CPU(var)          /* nothing */

#  define TM_PRINTF                     printf
#  define TM_PRINT0                     printf
#  define TM_PRINT1                     printf
#  define TM_PRINT2                     printf
#  define TM_PRINT3                     printf

#  define P_MEMORY_STARTUP(numThread)   /* nothing */
#  define P_MEMORY_SHUTDOWN()           /* nothing */

#  include <assert.h>
#  include "memory.h"
#  include "thread.h"
#  include "types.h"
#  include "thread.h"
#  include <math.h>

#  define TM_ARG                        /* nothing */
#  define TM_ARG_ALONE                  /* nothing */
#  define TM_ARGDECL                    /* nothing */
#  define TM_ARGDECL_ALONE              /* nothing */
#  define TM_CALLABLE                   /* nothing */

#  define TM_BEGIN_WAIVER()
#  define TM_END_WAIVER()

#  define P_MALLOC(size)                malloc(size)
#  define P_FREE(ptr)                   free(ptr)
#  define TM_MALLOC(size)               malloc(size)
#  define FAST_PATH_FREE(ptr)            free(ptr)
#  define SLOW_PATH_FREE(ptr)             free(ptr)

# define SETUP_NUMBER_TASKS(n)
# define SETUP_NUMBER_THREADS(n)
# define PRINT_STATS()
# define AL_LOCK(idx)

#endif

#include <asm/unistd.h>
#define rmb()           asm volatile ("sync" ::: "memory")
#define cpu_relax()     asm volatile ("" ::: "memory");
//#define cpu_relax() asm volatile ("or 31,31,31") 
#ifdef REDUCED_TM_API
#    define SPECIAL_THREAD_ID()         get_tid()
#else
#    define SPECIAL_THREAD_ID()         thread_getId()
#endif


#  define TM_STARTUP(numThread, bId)
#  define TM_SHUTDOWN(){ \
    unsigned long read_commits = 0; \
    unsigned long write_commits = 0; \
    int i = 0; \
    for (; i < 80; i++) { \
       read_commits += stats_array[i].read_commits; \
       write_commits += stats_array[i].write_commits; \
    } \
    printf("Total commits: %lu\n\tRead commits: %lu\n\tWrite commits: %lu\n", read_commits+write_commits, read_commits, write_commits); \
} \ 

#  define TM_THREAD_ENTER()
#  define TM_THREAD_EXIT()

# define IS_LOCKED(lock)        *((volatile int*)(&lock)) != 0

# define IS_GLOBAL_LOCKED(lock)        *((volatile int*)(&lock)) == 2

# define TM_BEGIN(b) TM_BEGIN_EXT(b,0)

# define ACQUIRE_READ_LOCK() { \
	while(1){ \
		while(IS_LOCKED(reader_locks[local_thread_id])){ \
                	cpu_relax(); \
                } \
		if(pthread_spin_trylock(&reader_locks[local_thread_id]) == 0){ \
			break; \
		} \
	} \
}; \

# define RELEASE_READ_LOCK() pthread_spin_unlock(&reader_locks[local_thread_id]); stats_array[local_thread_id].read_commits++;



# define ACQUIRE_WRITE_LOCK() { \
	long num_threads = global_numThread; \
	long index;\
        while(1){ \
                while(IS_LOCKED(writers_lock)){ \
                        cpu_relax(); \
                } \
                if(pthread_spin_trylock(&writers_lock) == 0){ \
                        break; \
                } \
        } \
	for(index=0; index < 80; index++){ \
		if (index == num_threads) break; \
		while(1){ \
	                while(IS_LOCKED(reader_locks[index])){ \
                	        cpu_relax(); \
        	        } \
	                if(pthread_spin_trylock(&reader_locks[index]) == 0){ \
                        	break; \
                	} \
        	} \
	}\
};

# define RELEASE_WRITE_LOCK(){ \
	long num_threads = global_numThread; \
        long index;\
	for(index=0; index < 80; index++){ \
                if (index == num_threads) break; \
                pthread_spin_unlock(&reader_locks[index]);\
        }\
	pthread_spin_unlock(&writers_lock); \
	stats_array[local_thread_id].write_commits++; \
};

# define TM_BEGIN_EXT(b,ro) { \
	long local_thread_id = SPECIAL_THREAD_ID();\
	if(ro){ \
		ACQUIRE_READ_LOCK(); \
	} \
	else{ \
		ACQUIRE_WRITE_LOCK(); \
	} \


# define TM_END() \
	if(ro){ \
		RELEASE_READ_LOCK(); \
	} \
	else{ \
		RELEASE_WRITE_LOCK(); \
	} \
};

#    define TM_BEGIN_RO()                 TM_BEGIN(1)
#    define TM_RESTART()                 
#    define TM_EARLY_RELEASE(var)

# define FAST_PATH_RESTART()
# define FAST_PATH_SHARED_READ(var) (var)
# define FAST_PATH_SHARED_READ_P(var) (var)
# define FAST_PATH_SHARED_READ_D(var) (var)
# define FAST_PATH_SHARED_WRITE(var, val) ({var = val; var;})
# define FAST_PATH_SHARED_WRITE_P(var, val) ({var = val; var;})
# define FAST_PATH_SHARED_WRITE_D(var, val) ({var = val; var;})

# define SLOW_PATH_RESTART() FAST_PATH_RESTART()
# define SLOW_PATH_SHARED_READ(var)           FAST_PATH_SHARED_READ(var)
# define SLOW_PATH_SHARED_READ_P(var)         FAST_PATH_SHARED_READ_P(var)
# define SLOW_PATH_SHARED_READ_F(var)         FAST_PATH_SHARED_READ_D(var)
# define SLOW_PATH_SHARED_READ_D(var)         FAST_PATH_SHARED_READ_D(var)
# define SLOW_PATH_SHARED_WRITE(var, val)     FAST_PATH_SHARED_WRITE(var, val)
# define SLOW_PATH_SHARED_WRITE_P(var, val)   FAST_PATH_SHARED_WRITE_P(var, val)
# define SLOW_PATH_SHARED_WRITE_D(var, val)   FAST_PATH_SHARED_WRITE_D(var, val)


#  define TM_LOCAL_WRITE(var, val)      ({var = val; var;})
#  define TM_LOCAL_WRITE_P(var, val)    ({var = val; var;})
#  define TM_LOCAL_WRITE_D(var, val)    ({var = val; var;})


#endif
