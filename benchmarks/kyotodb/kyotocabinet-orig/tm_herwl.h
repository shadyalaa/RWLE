#ifndef TM_HERWL_H
#define TM_HERWL_H 1

#  include <stdio.h>

typedef struct herwl_padded_statistics {
    unsigned long read_commits;
    unsigned long htm_commits;
    unsigned long htm_conflict_aborts;
    unsigned long htm_self_conflicts;
    unsigned long htm_trans_conflicts;
    unsigned long htm_nontrans_conflicts;
    unsigned long htm_user_aborts;
    unsigned long htm_capacity_aborts;
    unsigned long htm_persistent_aborts;
    unsigned long htm_other_aborts;
    unsigned long rot_commits;
    unsigned long rot_conflict_aborts;
    unsigned long rot_self_conflicts;
    unsigned long rot_trans_conflicts;
    unsigned long rot_nontrans_conflicts;
    unsigned long rot_user_aborts;
    unsigned long rot_persistent_aborts;
    unsigned long rot_capacity_aborts;
    unsigned long rot_other_aborts;
    unsigned long gl_commits;
    char suffixPadding[CACHE_LINE_SIZE];
} __attribute__((aligned(CACHE_LINE_SIZE))) herwl_padded_statistics_t;

extern __attribute__((aligned(CACHE_LINE_SIZE))) herwl_padded_statistics_t herwl_stats_array[];

typedef struct padded_scalar {
    volatile long value;
    char suffixPadding[CACHE_LINE_SIZE];
} __attribute__((aligned(CACHE_LINE_SIZE))) padded_scalar_t;

extern __thread int read_only;
extern __thread int htm_budget;
extern __thread int rot_budget;
extern __thread int exec_mode;
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
    for (; i < 80 ; i++) { \
       read_commits += herwl_stats_array[i].read_commits; \
       htm_commits += herwl_stats_array[i].htm_commits; \
       htm_conflict_aborts += herwl_stats_array[i].htm_conflict_aborts; \
       htm_user_aborts += herwl_stats_array[i].htm_user_aborts; \
       htm_self_conflicts += herwl_stats_array[i].htm_self_conflicts; \
       htm_trans_conflicts += herwl_stats_array[i].htm_trans_conflicts; \
       htm_nontrans_conflicts += herwl_stats_array[i].htm_nontrans_conflicts; \
       htm_persistent_aborts += herwl_stats_array[i].htm_persistent_aborts; \
       htm_capacity_aborts += herwl_stats_array[i].htm_capacity_aborts; \
       htm_other_aborts += herwl_stats_array[i].htm_other_aborts; \
       rot_commits += herwl_stats_array[i].rot_commits; \
       rot_conflict_aborts += herwl_stats_array[i].rot_conflict_aborts; \
       rot_user_aborts += herwl_stats_array[i].rot_user_aborts; \
       rot_self_conflicts += herwl_stats_array[i].rot_self_conflicts; \
       rot_trans_conflicts += herwl_stats_array[i].rot_trans_conflicts; \
       rot_nontrans_conflicts += herwl_stats_array[i].rot_nontrans_conflicts; \
       rot_persistent_aborts += herwl_stats_array[i].rot_persistent_aborts; \
       rot_capacity_aborts += herwl_stats_array[i].rot_capacity_aborts; \
       rot_other_aborts += herwl_stats_array[i].rot_other_aborts; \
       gl_commits += herwl_stats_array[i].gl_commits; \
    } \
    printf("Total commits: %lu\n\tRead commits: %lu\n\tHTM commits:  %lu\n\tROT commits:  %lu\n\tGL commits: %lu\nTotal aborts: %lu\n\tHTM conflict aborts:  %lu\n\t\tHTM self aborts:  %lu\n\t\tHTM trans aborts:  %lu\n\t\tHTM non-trans aborts:  %lu\n\tHTM user aborts :  %lu\n\tHTM capacity aborts:  %lu\n\t\tHTM persistent aborts:  %lu\n\tHTM other aborts:  %lu\n\tROT conflict aborts:  %lu\n\t\tROT self aborts:  %lu\n\t\tROT trans aborts:  %lu\n\t\tROT non-trans aborts:  %lu\n\tROT user aborts:  %lu\n\tROT capacity aborts:  %lu\n\t\tROT persistent aborts:  %lu\n\tROT other aborts:  %lu\n", read_commits+htm_commits+rot_commits+gl_commits, read_commits, htm_commits, rot_commits, gl_commits,htm_conflict_aborts+htm_user_aborts+htm_capacity_aborts+htm_other_aborts+rot_conflict_aborts+rot_user_aborts+rot_capacity_aborts+rot_other_aborts,htm_conflict_aborts,htm_self_conflicts,htm_trans_conflicts,htm_nontrans_conflicts,htm_user_aborts,htm_capacity_aborts,htm_persistent_aborts,htm_other_aborts,rot_conflict_aborts,rot_self_conflicts,rot_trans_conflicts,rot_nontrans_conflicts,rot_user_aborts,rot_capacity_aborts,rot_persistent_aborts,rot_other_aborts); \
} \ 


# define IS_LOCKED(lock)        *((volatile int*)(lock)) != 0

# define IS_GLOBAL_LOCKED(lock)        *((volatile int*)(lock)) == 2

# define ACQUIRE_READ_LOCK(mutex) { \
	while(1){ \
		readers_counters[local_thread_id] = 1; \
		rmb(); \
		if(IS_GLOBAL_LOCKED(mutex)){ \
			readers_counters[local_thread_id] = 0; \
		        while(IS_GLOBAL_LOCKED(mutex)){ \
                   		cpu_relax(); \
                	} \
		} \
		else{ \
			break; \
		} \
	} \
}; \

# define RELEASE_READ_LOCK()  readers_counters[local_thread_id] = 0; herwl_stats_array[local_thread_id].read_commits++;


# define USE_HTM(mutex){ \
	while(htm_budget > 0){ \
		htm_status = 1; \
		TM_buff_type TM_buff; \	
		while(IS_LOCKED(mutex)){ \
                        cpu_relax(); \
                } \
		unsigned char tx_status = __TM_begin(&TM_buff); \
		if (tx_status == _HTM_TBEGIN_STARTED) { \
			if(IS_LOCKED(mutex)){ \
				__TM_abort(); \
			} \
			break; \
		} \
		else if(__TM_is_conflict(&TM_buff)){ \
			herwl_stats_array[local_thread_id].htm_conflict_aborts ++; \
			if(__TM_is_self_conflict(&TM_buff)) {herwl_stats_array[local_thread_id].htm_self_conflicts++; }\
			else if(__TM_is_trans_conflict(&TM_buff)) herwl_stats_array[local_thread_id].htm_trans_conflicts++; \
                        else if(__TM_is_nontrans_conflict(&TM_buff)) herwl_stats_array[local_thread_id].htm_nontrans_conflicts++; \
                        htm_status = 0; \
                        htm_budget--; \
		} \
		else if (__TM_is_user_abort(&TM_buff)) { \
			herwl_stats_array[local_thread_id].htm_user_aborts ++; \
                        htm_status = 0; \
                        htm_budget--; \
                } \
		else if(__TM_is_footprint_exceeded(&TM_buff)){ \
			htm_status = 0; \
			herwl_stats_array[local_thread_id].htm_capacity_aborts ++; \
			if(__TM_is_failure_persistent(&TM_buff)) herwl_stats_array[local_thread_id].htm_persistent_aborts ++; \
			break; \
		} \
		else{ \
			herwl_stats_array[local_thread_id].htm_other_aborts ++; \
			htm_status = 0; \
			htm_budget--; \
		} \
	} \
};


# define USE_ROT(mutex){ \
	while(1){ \
		while(IS_LOCKED(mutex)){ \
			cpu_relax(); \
		} \
		/*if(__sync_val_compare_and_swap(&writers_lock.value, 0, 1) == 0){  */\
		if(pthread_spin_trylock(mutex) == 0){ \
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
                        herwl_stats_array[local_thread_id].rot_conflict_aborts ++; \
			if(__TM_is_self_conflict(&TM_buff)) herwl_stats_array[local_thread_id].rot_self_conflicts++; \
			else if(__TM_is_trans_conflict(&TM_buff)) herwl_stats_array[local_thread_id].rot_trans_conflicts++; \
			else if(__TM_is_nontrans_conflict(&TM_buff)) herwl_stats_array[local_thread_id].rot_nontrans_conflicts++; \
                        rot_status = 0; \
                        rot_budget--; \
                } \
                else if (__TM_is_user_abort(&TM_buff)) { \
                        herwl_stats_array[local_thread_id].rot_user_aborts ++; \
                        rot_status = 0; \
                        rot_budget--; \
                } \
                else if(__TM_is_footprint_exceeded(&TM_buff)){ \
			rot_status = 0; \
			herwl_stats_array[local_thread_id].rot_capacity_aborts ++; \
			if(__TM_is_failure_persistent(&TM_buff)) herwl_stats_array[local_thread_id].rot_persistent_aborts ++; \
                        break; \
		} \
                else{ \
			rot_status = 0; \
                        rot_budget--; \
			herwl_stats_array[local_thread_id].rot_other_aborts ++; \
		} \
	} \
};

# define ACQUIRE_GLOBAL_LOCK(mutex){ \
	*mutex = 2; \
	rmb(); \
	QUIESCENCE_CALL_1(); \
};

# define ACQUIRE_WRITE_LOCK(mutex) { \
	while(IS_LOCKED(mutex)){ \
		cpu_relax(); \
	} \
	int htm_status = 0; \
	USE_HTM(mutex); \
	if(!htm_status){ \ 
		exec_mode = 1; \
		int rot_status = 0; \
		USE_ROT(mutex); \
		if(!rot_status){ \
			exec_mode = 2; \
			ACQUIRE_GLOBAL_LOCK(mutex); \
		} \
	} \
};\

# define QUIESCENCE_CALL(){ \
	/*long num_threads = global_numThread; */\
	int index;\
	volatile __attribute__((aligned(CACHE_LINE_SIZE))) padded_scalar_t temp; \
	/*long counters_snapshot[80]; */ \
	__attribute__((aligned(CACHE_LINE_SIZE))) padded_scalar_t counters_snapshot[80]; \
	/*long* counters_snapshot = new long[80];*/\
	for(index=0; index < 80; index++){ \
		temp.value = readers_counters[index]; \
		if((temp.value & 1) != 0){ \
			/*counters_snapshot[index].value = temp.value; */\
		}\
		else{ \
			/*counters_snapshot[index].value = 0; */\ 
		} \
	} \
	for(index=0; index < 80; index++){ \
		if(counters_snapshot[index] != 0){ \
			temp.value = readers_counters[index]; \
			while(temp.value == counters_snapshot[index]) temp.value = readers_counters[index]; \
		} \
	} \
};

# define QUIESCENCE_CALL_1(){ \
        /*__attribute__((aligned(CACHE_LINE_SIZE))) padded_scalar_t num_threads; \
        num_threads.value = global_numThread; 
        __attribute__((aligned(CACHE_LINE_SIZE))) padded_scalar_t index; */ int index;\
        for(index=0; index < 80; index++){ \
			if (index == global_num_threads) break; \
                        while(readers_counters[index] != 0) cpu_relax();\
        } \
};

# define RELEASE_WRITE_LOCK(mutex){ \
	if(exec_mode == 0){ \
		__TM_suspend(); \
		QUIESCENCE_CALL_1(); \
		__TM_resume(); \
		__TM_end(); \
		herwl_stats_array[local_thread_id].htm_commits++; \
	} \
	else if(exec_mode == 1){ \
		QUIESCENCE_CALL_1(); \
		__TM_end(); \
		herwl_stats_array[local_thread_id].rot_commits++; \
		pthread_spin_unlock(mutex); \
	} \
	else{ \
		herwl_stats_array[local_thread_id].gl_commits++; \
		pthread_spin_unlock(mutex); \
	} \
};

# define TM_BEGIN_HERWL(mutex) { \
	if(read_only){ \
		ACQUIRE_READ_LOCK(mutex); \
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
