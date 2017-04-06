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

#ifdef REDUCED_TM_API
#    define local_thread_d()         get_tid()
#else
#    define local_thread_d()         thread_getId()
#endif

#include <htmxlintrin.h>

# define CACHE_LINE_SIZE 128

typedef struct padded_statistics {
    unsigned long tle_commits;
    unsigned long gl_commits;
    unsigned long conflicts;
    unsigned long self;
    unsigned long trans;
    unsigned long nontrans;
    unsigned long capacity;
    unsigned long other;
    unsigned long user;
    unsigned long persistent;
    char suffixPadding[CACHE_LINE_SIZE];
} __attribute__((aligned(CACHE_LINE_SIZE))) padded_statistics_t;

extern __attribute__((aligned(CACHE_LINE_SIZE))) padded_statistics_t stats_array[];

extern __thread int local_thread_id;

extern __inline long
__attribute__ ((__gnu_inline__, __always_inline__, __artificial__))
__TM_is_self_conflict(void* const TM_buff)
{ 
  texasr_t texasr = __builtin_get_texasr ();
  return _TEXASR_SELF_INDUCED_CONFLICT (texasr);
}

extern __inline long
__attribute__ ((__gnu_inline__, __always_inline__, __artificial__))
__TM_is_trans_conflict(void* const TM_buff)
{ 
  texasr_t texasr = __builtin_get_texasr ();
  return _TEXASR_TRANSACTION_CONFLICT (texasr);
}

extern __inline long
__attribute__ ((__gnu_inline__, __always_inline__, __artificial__))
__TM_is_nontrans_conflict(void* const TM_buff)
{ 
  texasr_t texasr = __builtin_get_texasr ();
  return _TEXASR_NON_TRANSACTIONAL_CONFLICT (texasr);
}

#  define TM_STARTUP(numThread, bId)
#  define TM_SHUTDOWN() { \
    unsigned long tle_commits = 0; \
    unsigned long gl_commits = 0; \
    unsigned long conflicts = 0; \
    unsigned long self = 0; \
    unsigned long trans = 0; \
    unsigned long nontrans = 0; \
    unsigned long capacity = 0; \
    unsigned long user = 0; \
    unsigned long persistent = 0; \
    unsigned long other = 0; \
    int i = 0; \
    for (; i < 80; i++) { \
       if (stats_array[i].tle_commits+stats_array[i].gl_commits == 0) { break; } \
        tle_commits += stats_array[i].tle_commits; \
        gl_commits += stats_array[i].gl_commits; \
        conflicts += stats_array[i].conflicts; \
        self += stats_array[i].self; \
        trans += stats_array[i].trans; \
        nontrans += stats_array[i].nontrans; \
        capacity += stats_array[i].capacity; \
        user += stats_array[i].user; \
        persistent += stats_array[i].persistent; \ 
        other += stats_array[i].other; \
    } \
    printf("Total commits: %lu\n\tHTM commits: %lu\n\tGL commits: %lu\nTotal aborts: %lu\n\tHTM conflict aborts: %lu\n\t\tHTM trans conflicts: %lu\n\t\tHTM non-trans conflicts: %lu\n\t\tHTM self conflicts: %lu\n\tHTM capacity aborts: %lu\n\tHTM persistent aborts: %lu\n\tHTM user aborts: %lu\n\tHTM other aborts: %lu\n", tle_commits+gl_commits,tle_commits, gl_commits, conflicts+capacity+user+persistent+other,conflicts,trans,nontrans,self,capacity,persistent,user,other); \
}

#  define TM_THREAD_ENTER()
#  define TM_THREAD_EXIT()

# define IS_LOCKED(lock)        *((volatile int*)(&lock)) != 0

# define SPEND_BUDGET(b)	if(RETRY_POLICY == 0) (*b)=0; else if (RETRY_POLICY == 2) (*b)=(*b)/2; else (*b)=--(*b);

# define TM_BEGIN(mutex) { \
        while (1) { \
            while (IS_LOCKED(*mutex)) { \
                    __asm volatile ("" : : : "memory"); \
            } \
            TM_buff_type TM_buff; \
            unsigned char status = __TM_begin(&TM_buff); \
            if (status == _HTM_TBEGIN_STARTED) { \
            	if (IS_LOCKED(*mutex)) { \
            		__TM_abort(); \
            	} \
            	break; \
            } \
	    else{ \
                if(__TM_is_failure_persistent(&TM_buff)){ \
                         SPEND_BUDGET(&htm_budget); \
                         stats_array[local_thread_id].persistent++; \
                } \
                if(__TM_is_conflict(&TM_buff)){ \
                        stats_array[local_thread_id].conflicts++; \
                        if(__TM_is_self_conflict(&TM_buff)) {stats_array[local_thread_id].self++; }\
                        else if(__TM_is_trans_conflict(&TM_buff)) stats_array[local_thread_id].trans++; \
                        else if(__TM_is_nontrans_conflict(&TM_buff)) stats_array[local_thread_id].nontrans++; \
                        htm_budget--; \
                } \
                else if (__TM_is_user_abort(&TM_buff)) { \
                        stats_array[local_thread_id].user++; \
                        htm_budget--; \
                } \
                else if(__TM_is_footprint_exceeded(&TM_buff)){ \
                        stats_array[local_thread_id].capacity++; \
                        htm_budget--; \
                } \
                else{ \
                        stats_array[local_thread_id].other++; \
                        htm_budget--; \
                } \
            } \
            if (htm_budget <= 0) {   \
            	while (::pthread_mutex_lock(mutex) != 0) { \
                    __asm volatile ("" : : : "memory"); \
                } \
                break; \
            } \
        } \
};


# define TM_END(mutex){ \
    if (htm_budget > 0) { \
        __TM_end(); \
        stats_array[local_thread_id].tle_commits++; \
    } else {    \
        ::pthread_mutex_unlock(mutex); \
        stats_array[local_thread_id].gl_commits++; \
    } \
};

#    define TM_BEGIN_RO()                 TM_BEGIN(0)
#    define TM_RESTART()                  __TM_abort();
#    define TM_EARLY_RELEASE(var)

# define FAST_PATH_RESTART() __TM_abort();
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
