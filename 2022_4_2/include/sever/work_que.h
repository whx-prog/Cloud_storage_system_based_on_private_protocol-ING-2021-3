#ifndef _WORK_QUE_ 
#define _WORK_QUE_
#include "head.h"
typedef struct tag_node{
    int new_fd;
    int Cur_arry_flag;//当前数组下标
    int pathID_box[10]; //路径目录id数组
    char *path_Name_box[10];//路径目录名数组
    char Cur_path[30];//当前路径
    char belong[10];
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
int que_get(pque_t pq,pnode_t* ppCur);
int que_set(pque_t pq,pnode_t pnew);
#endif
