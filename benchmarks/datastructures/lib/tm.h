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

//#  include <immintrin.h>
//#  include <rtmintrin.h>
#include <htmxlintrin.h>

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
#  define TM_SHUTDOWN(){ \
    unsigned long read_commits = 0; \
    unsigned long htm_commits = 0; \
    unsigned long htm_conflict_aborts = 0; \
    unsigned long htm_user_aborts = 0; \
    unsigned long htm_self_conflicts = 0; \
    unsigned long htm_trans_conflicts = 0; \
    unsigned long htm_nontrans_conflicts = 0; \
    unsigned long htm_persistent_aborts = 0; \
    unsigned long htm_capacity_aborts = 0; \
    unsigned long htm_other_aborts = 0; \
    unsigned long rot_commits = 0; \
    unsigned long rot_conflict_aborts = 0; \
    unsigned long rot_user_aborts = 0; \
    unsigned long rot_self_conflicts = 0; \
    unsigned long rot_trans_conflicts = 0; \
    unsigned long rot_nontrans_conflicts = 0; \
    unsigned long rot_persistent_aborts = 0; \
    unsigned long rot_capacity_aborts = 0; \
    unsigned long rot_other_aborts = 0; \
    unsigned long gl_commits = 0; \
    int i = 0; \
    for (; i < 80; i++) { \
       read_commits += stats_array[i].read_commits; \
       htm_commits += stats_array[i].htm_commits; \
       htm_conflict_aborts += stats_array[i].htm_conflict_aborts; \
       htm_user_aborts += stats_array[i].htm_user_aborts; \
       htm_self_conflicts += stats_array[i].htm_self_conflicts; \
       htm_trans_conflicts += stats_array[i].htm_trans_conflicts; \
       htm_nontrans_conflicts += stats_array[i].htm_nontrans_conflicts; \
       htm_persistent_aborts += stats_array[i].htm_persistent_aborts; \
       htm_capacity_aborts += stats_array[i].htm_capacity_aborts; \
       htm_other_aborts += stats_array[i].htm_other_aborts; \
       rot_commits += stats_array[i].rot_commits; \
       rot_conflict_aborts += stats_array[i].rot_conflict_aborts; \
       rot_user_aborts += stats_array[i].rot_user_aborts; \
       rot_self_conflicts += stats_array[i].rot_self_conflicts; \
       rot_trans_conflicts += stats_array[i].rot_trans_conflicts; \
       rot_nontrans_conflicts += stats_array[i].rot_nontrans_conflicts; \
       rot_persistent_aborts += stats_array[i].rot_persistent_aborts; \
       rot_capacity_aborts += stats_array[i].rot_capacity_aborts; \
       rot_other_aborts += stats_array[i].rot_other_aborts; \
       gl_commits += stats_array[i].gl_commits; \
    } \
    printf("Total commits: %lu\n\tRead commits: %lu\n\tHTM commits:  %lu\n\tROT commits:  %lu\n\tGL commits: %lu\nTotal aborts: %lu\n\tHTM conflict aborts:  %lu\n\t\tHTM self aborts:  %lu\n\t\tHTM trans aborts:  %lu\n\t\tHTM non-trans aborts:  %lu\n\tHTM user aborts :  %lu\n\tHTM capacity aborts:  %lu\n\t\tHTM persistent aborts:  %lu\n\tHTM other aborts:  %lu\n\tROT conflict aborts:  %lu\n\t\tROT self aborts:  %lu\n\t\tROT trans aborts:  %lu\n\t\tROT non-trans aborts:  %lu\n\tROT user aborts:  %lu\n\tROT capacity aborts:  %lu\n\t\tROT persistent aborts:  %lu\n\tROT other aborts:  %lu\n", read_commits+htm_commits+rot_commits+gl_commits, read_commits, htm_commits, rot_commits, gl_commits,htm_conflict_aborts+htm_user_aborts+htm_capacity_aborts+htm_other_aborts+rot_conflict_aborts+rot_user_aborts+rot_capacity_aborts+rot_other_aborts,htm_conflict_aborts,htm_self_conflicts,htm_trans_conflicts,htm_nontrans_conflicts,htm_user_aborts,htm_capacity_aborts,htm_persistent_aborts,htm_other_aborts,rot_conflict_aborts,rot_self_conflicts,rot_trans_conflicts,rot_nontrans_conflicts,rot_user_aborts,rot_capacity_aborts,rot_persistent_aborts,rot_other_aborts); \
} \ 

#  define TM_THREAD_ENTER()
#  define TM_THREAD_EXIT()

# define IS_LOCKED(lock)        *((volatile int*)(&lock)) != 0

# define IS_GLOBAL_LOCKED(lock)        *((volatile int*)(&lock)) == 2

# define TM_BEGIN(b) TM_BEGIN_EXT(b,0)

# define ACQUIRE_READ_LOCK() { \
	while(1){ \
		while(IS_GLOBAL_LOCKED(writers_lock)){ \
                	cpu_relax(); \
                } \
		counters[local_thread_id].value++; \
		if(IS_GLOBAL_LOCKED(writers_lock)){ \
			counters[local_thread_id].value++; \
		} \
		else{ \
			break; \
		} \
	} \
	rmb(); \
}; \

# define RELEASE_READ_LOCK() counters[local_thread_id].value++; stats_array[local_thread_id].read_commits++;


# define USE_HTM(){ \
	int htm_budget = HTM_RETRIES; \
	while(htm_budget > 0){ \
		htm_status = 1; \
		TM_buff_type TM_buff; \	
		while(IS_LOCKED(writers_lock)){ \
                        cpu_relax(); \
                } \
		unsigned char tx_status = __TM_begin(&TM_buff); \
		if (tx_status == _HTM_TBEGIN_STARTED) { \
			if(IS_LOCKED(writers_lock)){ \
				__TM_abort(); \
			} \
			break; \
		} \
		else if(__TM_is_conflict(&TM_buff)){ \
			stats_array[local_thread_id].htm_conflict_aborts ++; \
			if(__TM_is_self_conflict(&TM_buff)) {stats_array[local_thread_id].htm_self_conflicts++; }\
			else if(__TM_is_trans_conflict(&TM_buff)) stats_array[local_thread_id].htm_trans_conflicts++; \
                        else if(__TM_is_nontrans_conflict(&TM_buff)) stats_array[local_thread_id].htm_nontrans_conflicts++; \
                        htm_status = 0; \
                        htm_budget--; \
		} \
		else if (__TM_is_user_abort(&TM_buff)) { \
			stats_array[local_thread_id].htm_user_aborts ++; \
                        htm_status = 0; \
                        htm_budget--; \
                } \
		else if(__TM_is_footprint_exceeded(&TM_buff)){ \
			htm_status = 0; \
			stats_array[local_thread_id].htm_capacity_aborts ++; \
			if(__TM_is_failure_persistent(&TM_buff)) stats_array[local_thread_id].htm_persistent_aborts ++; \
			break; \
		} \
		else{ \
			stats_array[local_thread_id].htm_other_aborts ++; \
			htm_status = 0; \
			htm_budget--; \
		} \
	} \
};


# define USE_ROT(){ \
	int rot_budget = ROT_RETRIES; \
	while(1){ \
		while(IS_LOCKED(writers_lock)){ \
			cpu_relax(); \
		} \
		/*if(__sync_val_compare_and_swap(&writers_lock, 0, 1) == 0){  */\
		if(pthread_spin_trylock(&writers_lock) == 0){ \
			break; \
		} \
	} \
	while(rot_budget > 0){ \
		rot_status = 1; \
		TM_buff_type TM_buff; \
		unsigned char tx_status = __TM_begin_rot(&TM_buff); \
		if (tx_status == _HTM_TBEGIN_STARTED) { \
                        break; \
                } \
		else if(__TM_is_conflict(&TM_buff)){ \
                        stats_array[local_thread_id].rot_conflict_aborts ++; \
			if(__TM_is_self_conflict(&TM_buff)) stats_array[local_thread_id].rot_self_conflicts++; \
			else if(__TM_is_trans_conflict(&TM_buff)) stats_array[local_thread_id].rot_trans_conflicts++; \
			else if(__TM_is_nontrans_conflict(&TM_buff)) stats_array[local_thread_id].rot_nontrans_conflicts++; \
                        rot_status = 0; \
                        rot_budget--; \
                } \
                else if (__TM_is_user_abort(&TM_buff)) { \
                        stats_array[local_thread_id].rot_user_aborts ++; \
                        rot_status = 0; \
                        rot_budget--; \
                } \
                else if(__TM_is_footprint_exceeded(&TM_buff)){ \
			rot_status = 0; \
			stats_array[local_thread_id].rot_capacity_aborts ++; \
			if(__TM_is_failure_persistent(&TM_buff)) stats_array[local_thread_id].rot_persistent_aborts ++; \
                        break; \
		} \
                else{ \
			rot_status = 0; \
                        rot_budget--; \
			stats_array[local_thread_id].rot_other_aborts ++; \
		} \
	} \
};

# define ACQUIRE_GLOBAL_LOCK(){ \ 
	writers_lock = 2; \
	rmb(); \
	QUIESCENCE_CALL_1(); \
};

# define ACQUIRE_WRITE_LOCK() { \
	while(IS_LOCKED(writers_lock)){ \
		cpu_relax(); \
	} \
	int htm_status = 0; \
	USE_HTM(); \
	if(!htm_status){ \ 
		exec_mode = 1; \
		int rot_status = 0; \
		USE_ROT(); \
		if(!rot_status){ \
			exec_mode = 2; \
			ACQUIRE_GLOBAL_LOCK(); \
		} \
	} \
};\

# define QUIESCENCE_CALL(){ \
	long num_threads = global_numThread; \
	long index;\
	volatile long temp; \
	long counters_snapshot[80]; \
	for(index=0; index < 80; index++){ \
		if (index == num_threads) break; \
		temp = counters[index].value; \
		if((temp & 1) != 0){ \
			counters_snapshot[index] = temp; \
		}\
		else{ \
			counters_snapshot[index] = 0; \ 
		} \
	} \
	for(index=0; index < 80; index++){ \
		if (index == num_threads) break; \
		if(counters_snapshot[index] != 0){ \
			while(counters[index].value == counters_snapshot[index]){ \
				cpu_relax(); \
			} \
		} \
	} \
};

# define QUIESCENCE_CALL_1(){ \
        __attribute__((aligned(CACHE_LINE_SIZE))) padded_scalar_t num_threads; \
        num_threads.value = global_numThread; \
        /*__attribute__((aligned(CACHE_LINE_SIZE))) padded_scalar_t index; */ int index;\
        for(index=0; index < num_threads.value; index++){ \
                        while((counters[index].value & 1) != 0){ \
                                cpu_relax(); \
                        } \
        } \
};

# define RELEASE_WRITE_LOCK(){ \
	if(exec_mode == 0){ \
		__TM_suspend(); \
		QUIESCENCE_CALL(); \
		__TM_resume(); \
		__TM_end(); \
		stats_array[local_thread_id].htm_commits++; \
	} \
	else if(exec_mode == 1){ \
		QUIESCENCE_CALL(); \
		__TM_end(); \
		stats_array[local_thread_id].rot_commits++; \
		pthread_spin_unlock(&writers_lock); \
	} \
	else{ \
		stats_array[local_thread_id].gl_commits++; \
		pthread_spin_unlock(&writers_lock); \
	} \
};

# define TM_BEGIN_EXT(b,ro) { \
	int exec_mode = 0; \
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
