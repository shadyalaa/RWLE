#ifndef TM_HERWL_H
#define TM_HERWL_H 1

#  include <stdio.h>

typedef struct herwl_padded_statistics {
    unsigned long read_commits;
    unsigned long write_commits;
    char suffixPadding[CACHE_LINE_SIZE];
} __attribute__((aligned(CACHE_LINE_SIZE))) herwl_padded_statistics_t;

extern __attribute__((aligned(CACHE_LINE_SIZE))) herwl_padded_statistics_t herwl_stats_array[];

typedef struct padded_scalar {
    volatile long value;
    char suffixPadding[CACHE_LINE_SIZE];
} __attribute__((aligned(CACHE_LINE_SIZE))) padded_scalar_t;

extern __thread int read_only;
extern __thread int local_thread_id;
extern int global_num_threads;
#  include <math.h>


#include <asm/unistd.h>
#define rmb()           asm volatile ("sync" ::: "memory")
#define cpu_relax()     asm volatile ("" ::: "memory");

#include <htmxlintrin.h>


extern __inline long
__attribute__ ((__gnu_inline__, __always_inline__, __artificial__))
__TM_begin_rot (void* const TM_buff)
{
  *_TEXASRL_PTR (TM_buff) = 0;
  if (__builtin_expect (__builtin_tbegin (1), 1)){
    return _HTM_TBEGIN_STARTED;
  }
#ifdef __powerpc64__
  *_TEXASR_PTR (TM_buff) = __builtin_get_texasr ();
#else
  *_TEXASRU_PTR (TM_buff) = __builtin_get_texasru ();
  *_TEXASRL_PTR (TM_buff) = __builtin_get_texasr ();
#endif
  *_TFIAR_PTR (TM_buff) = __builtin_get_tfiar ();
  return 0;
}

#  define TM_STARTUP(numThread, bId)
#  define TM_SHUTDOWN_HERWL(){ \
    unsigned long read_commits = 0; \
    unsigned long write_commits = 0; \
    int i = 0; \
    for (; i < 80 ; i++) { \
       read_commits += herwl_stats_array[i].read_commits; \
       write_commits += herwl_stats_array[i].write_commits; \
    } \
    printf("Total commits: %lu\n\tRead commits: %lu\n\tWrite commits: %lu\n", read_commits+write_commits, read_commits, write_commits); \
} \ 


# define IS_LOCKED(lock)        *((volatile int*)(lock)) != 0

# define IS_GLOBAL_LOCKED(lock)        *((volatile int*)(lock)) == 2

# define ACQUIRE_READ_LOCK() { \
        while(1){ \
                while(IS_LOCKED(&reader_locks[local_thread_id])){ \
                        cpu_relax(); \
                } \
                if(pthread_spin_trylock(&reader_locks[local_thread_id]) == 0){ \
                        break; \
                } \
        } \
}; \

# define RELEASE_READ_LOCK() pthread_spin_unlock(&reader_locks[local_thread_id]); herwl_stats_array[local_thread_id].read_commits++;


# define ACQUIRE_WRITE_LOCK(mutex) { \
        long num_threads = global_num_threads; \
        long index;\
        while(1){ \
                while(IS_LOCKED(mutex)){ \
                        cpu_relax(); \
                } \
                if(pthread_spin_trylock(mutex) == 0){ \
                        break; \
                } \
        } \
        for(index=0; index < 80; index++){ \
                if (index == num_threads) break; \
                while(1){ \
                        while(IS_LOCKED(&reader_locks[index])){ \
                                cpu_relax(); \
                        } \
                        if(pthread_spin_trylock(&reader_locks[index]) == 0){ \
                                break; \
                        } \
                } \
        }\
};

# define RELEASE_WRITE_LOCK(mutex){ \
        long num_threads = global_num_threads; \
        long index;\
        for(index=0; index < 80; index++){ \
                if (index == num_threads) break; \
                pthread_spin_unlock(&reader_locks[index]);\
        }\
        pthread_spin_unlock(mutex); \
        herwl_stats_array[local_thread_id].write_commits++; \
};


# define TM_BEGIN_HERWL(mutex) { \
	if(read_only){ \
		ACQUIRE_READ_LOCK(); \
	} \
	else{ \
		ACQUIRE_WRITE_LOCK(mutex); \
	} \
};

# define TM_END_HERWL(mutex){ \
	if(read_only){ \
		RELEASE_READ_LOCK(); \
	} \
	else{ \
		RELEASE_WRITE_LOCK(mutex); \
	} \
};



#endif
