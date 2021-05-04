#include "work_que.h"

int que_init(pque_t pq,int capacity)
{ //  ARGS_CHECK(argc,2);
    memset(pq,0,sizeof(que_t));
    pq->que_capacity=capacity;
    pthread_mutex_init(&pq->mutex,NULL);
    pthread_mutex_init(&pq->log_mutex,NULL);
    return 0;
}
int que_get(pque_t pq,pnode_t *ppcur)//出队
{
    *ppcur=pq->que_head;
    pq->que_head=pq->que_head->pnext;
    pq->que_size--;
    return 0;
}
int que_set(pque_t pq,pnode_t pnew)//入队，尾插法
{
    if(!pq->que_size){
        pq->que_head=pnew;
        pq->que_tail=pnew;
    }else{
        pq->que_tail->pnext=pnew;
        pq->que_tail=pnew;
    }
    pq->que_size++;
    return 0;
}
