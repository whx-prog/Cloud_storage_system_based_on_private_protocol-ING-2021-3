#include "factory.h"
int main(int argc,char *argv[])
{ //  ARGS_CHECK(argc,2);
    if(argc!=2)
    {
    printf("argv error");
    return -1;
    }

    phashNode_t hashTable[10]={0};
    hashNode_t InsertNode[]={
        {CD,NULL,"cd"},
        {LS,NULL,"ls"},
        {PUTS,NULL,"puts"},
        {GETS,NULL,"gets"},
        {REMOVE,NULL,"remove"},
        {PWD,NULL,"pwd"},
    };    
    
    for(unsigned long i=0;i<sizeof(InsertNode)/sizeof(hashNode_t);i++){
        hashInsert(hashTable,&InsertNode[i]);
    }           //哈希表建立完毕

    int thread_num_init=0;
    int capacity_init=0;
    char ip[16]="";
    char port[10]="";
    conf_init(argv[1],ip,port,&thread_num_init,&capacity_init);

    

   // int thread_num=atoi(argv[3]);//创建线程数量
   // int capacity=atoi(argv[4]);//队列容量
   int thread_num=thread_num_init;
   int capacity=capacity_init;
    factory_t thread_data;//线程次主数据结构
    factory_init(&thread_data,thread_num,capacity);
    factory_start(&thread_data);
    int sfd,new_fd;
   // tcp_init(argv[1],argv[2],&sfd);
    tcp_init(ip,port,&sfd);
    pnode_t pnew;
    pque_t pq=&thread_data.que;


    while(1)
    {
    new_fd=accept(sfd,NULL,NULL);//新用户连接
    pnew=(pnode_t)calloc(1,sizeof(node_t));
    pnew->new_fd=new_fd;
    pnew->new_fd=new_fd;
    pthread_mutex_lock(&pq->mutex);
    que_set(pq,pnew);//将任务放入队列
    pthread_mutex_unlock(&pq->mutex);
    pthread_cond_signal(&thread_data.cond);//激活线程服务
    }
    return 0;
}

