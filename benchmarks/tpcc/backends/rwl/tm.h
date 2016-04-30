#ifndef TM_H
#define TM_H 1

#  include <stdio.h>

#define CACHE_LINE_SIZE 128

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

extern pthread_rwlock_t rw_lock;

#define CACHE_LINE_SIZE 128

#include <htmxlintrin.h>


#  define TM_STARTUP(numThread, bId)
#  define TM_SHUTDOWN() 


# define IS_LOCKED(lock)        *((volatile int*)(&lock)) != 0


# define TM_BEGIN(b,ro) { \
	if(ro){ \
		pthread_rwlock_rdlock(&rw_lock); \
	} \
	else{ \
		pthread_rwlock_wrlock(&rw_lock); \
	} \
};

# define TM_END() {\
        pthread_rwlock_unlock(&rw_lock); \
};


#endif
