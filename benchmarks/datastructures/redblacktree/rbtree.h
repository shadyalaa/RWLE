#ifndef RBTREE_H_
#define RBTREE_H_ 1

#include <stdint.h>

typedef struct node {
    intptr_t k;
    intptr_t v;
    struct node* p;
    struct node* l;
    struct node* r;
    intptr_t c;
} node_t;


typedef struct rbtree {
    node_t* root;
} rbtree_t;


#endif
