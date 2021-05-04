#include "factory.h"
#define DEBUG 1
train_t t;
int data_len;

int recvn(int sfd,void* pstart,int len)
{
    int total=0,ret;
    char *p=(char *)pstart;
    while(total<len)
    {
        ret=recv(sfd,p+total,len-total,0);
        if(ret==0){
            printf("\n");
            return -1;
        }
        total+=ret;
    }
    return 0;
}



int factory_init(factory_t *p,int thread_num,int capacity)
{ //  ARGS_CHECK(argc,2);
    que_init(&p->que,capacity);
    pthread_cond_init(&p->cond,NULL);
    p->pthread_num=thread_num;
    p->pthid=(pthread_t*)calloc(thread_num,sizeof(pthread_t));
    p->download_thread=thread_func;
    p->start_flag=0;
    return 0;
}



void *thread_func(void*p)
{
    factory_t *pfac=(factory_t*)p;
    pque_t pq=&pfac->que;
    pnode_t pcur;
    int count=0;
    train_t t;
printf("A1\n");
    while(1)
    {
        printf("B1\n");
        pthread_mutex_lock(&pq->mutex);
        if(!pq->que_size)
        {
            pthread_cond_wait(&pfac->cond,&pq->mutex);
        }
        printf("C1\n");
        que_pop(pq,&pcur);
        printf("D1\n");
        pthread_mutex_unlock(&pq->mutex);
        printf("E1\n");
        int sfd;
        tcp_init(pcur->ip,pcur->port,&sfd);
        printf("F1\n");
        int epfd;
        epfd=epoll_create(1);
        struct epoll_event evs[1];
        epoll_init(epfd,sfd);
        int epoll_ready_num,j,k;
        char buf[128]={0};
        char str[128]={0};
        int ret=0;
        int i;
        char tool[4]={0};
        char belong[10]={0};
        int ppid;
        printf("G1\n");
        while(1)
        {
            printf("G2\n");
            memset(buf,0,sizeof(buf));
            memset(str,0,sizeof(str));
            memset(belong,0,sizeof(belong));
            ret=0;
           /* epoll_ready_num=epoll_wait(epfd,evs,1,-1);
            for(i=0;i<epoll_ready_num;i++)
            {
                printf("G3\n");
                if(evs[i].data.fd==sfd)
                {
                    printf("G4\n");
                    gets_file(sfd);
                    goto END;
                }    
            }*/
            printf("before puts\n");
            if(pcur->gets_or_puts==0)//puts
            {
                ret=3;
                send(sfd,&ret,4,MSG_NOSIGNAL);
                recv(sfd,tool,4,MSG_NOSIGNAL);
                send(sfd,pcur->fime_name,8,MSG_NOSIGNAL);
                i=0;
                char *carry=(char *)calloc(1000,1);
                int fd=open(pcur->fime_name,O_RDONLY);
                if(-1==fd)
                    printf("open error,line:101,factory.c\n");
                while((ret=read(fd,&carry[i++],1))!=0){
                    printf("Computing Md5sum\n");
                    if(i==1000){
                        carry[1000]='\0';
                        break;
                    }
                }
                char *res=getMd5(carry);
                printf("%s\n",res);
                printf("ppid is %d belong is %s\n",pcur->ppid,pcur->belong);
                tran_file(sfd,pcur->fime_name,"0",0,0,res,pcur->ppid,pcur->belong);
                free(carry);
                free(res);
                printf("5\n");
                goto END;
            }
            printf("before gets\n");
            if(pcur->gets_or_puts==1)//gets
            {
                i=0;j=0;ret=4;
                int id=0;
                send(sfd,&ret,4,MSG_NOSIGNAL);
                recv(sfd,tool,4,MSG_NOSIGNAL);
                id=pcur->ppid;
                send(sfd,&id,4,MSG_NOSIGNAL);
                recv(sfd,tool,4,MSG_NOSIGNAL);
                send(sfd,pcur->belong,10,MSG_NOSIGNAL);
                recv(sfd,tool,4,MSG_NOSIGNAL);
                send(sfd,pcur->fime_name,8,MSG_NOSIGNAL);
                gets_file(sfd);
                goto END;
            }   

        }

END:        
    close(sfd);
    }
}   


    int factory_start(factory_t *p){
        if(!p->start_flag)
        {
            int i;
            for(i=0;i<p->pthread_num;i++)
            {
                pthread_create(p->pthid+i,NULL,p->download_thread,p);  
            }
            p->start_flag=1;
        }
        return 0;
    } 

