#ifndef _WORK_QUE_ 
#define _WORK_QUE_
#include "head.h"
typedef struct tag_node{
    char *ip;
    char *port;
    char fime_name[128];
    int gets_or_puts;
    char belong[10];
    int ppid;
    struct tag_node *pnext;
}node_t,*pnode_t;

typedef struct{
    pnode_t que_head,que_tail;
    int que_capacity;
    int que_size;
    pthread_mutex_t mutex;
    pthread_mutex_t log_mutex;
}que_t,*pque_t;

int que_init(pque_t pq,int que_capacity);
int que_pop(pque_t pq,pnode_t* ppcur);
int que_Tail_insert(pque_t pq,pnode_t pnew);
#endif
