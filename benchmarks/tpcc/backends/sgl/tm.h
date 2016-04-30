#ifndef TM_H
#define TM_H 1

#  include <stdio.h>

# define CACHE_LINE_SIZE 128

typedef struct padded_scalar {
    volatile long value;
    char suffixPadding[CACHE_LINE_SIZE];
} __attribute__((aligned(CACHE_LINE_SIZE))) padded_scalar_t;

typedef struct spinlock {
    pthread_spinlock_t lock;
    char suffixPadding[CACHE_LINE_SIZE];
} __attribute__((aligned(CACHE_LINE_SIZE))) spinlock_t;

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

extern __attribute__((aligned(CACHE_LINE_SIZE))) pthread_spinlock_t single_global_lock;


#  define TM_STARTUP(numThread, bId)
#      define TM_SHUTDOWN() 
#  define TM_THREAD_ENTER()
#  define TM_THREAD_EXIT()


# define IS_LOCKED(lock)        *((volatile int*)(&lock)) != 0


# define TM_BEGIN(b,ro) pthread_spin_lock(&single_global_lock)
# define TM_END() pthread_spin_unlock(&single_global_lock)

#endif
