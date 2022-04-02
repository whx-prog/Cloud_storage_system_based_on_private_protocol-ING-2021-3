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
//printf("A1\n");
    while(1)
    {
 //       printf("B1\n");
        pthread_mutex_lock(&pq->mutex);
        if(!pq->que_size)
        {
            pthread_cond_wait(&pfac->cond,&pq->mutex);
        }
   //     printf("C1\n");
        que_pop(pq,&pcur);
     //   printf("D1\n");
        pthread_mutex_unlock(&pq->mutex);
      //  printf("E1\n");
        int sfd;
        tcp_init(pcur->ip,pcur->port,&sfd);
       // printf("F1\n");
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
       // printf("G1\n");
        while(1)
        {
         //   printf("G2\n");
            memset(buf,0,sizeof(buf));
            memset(str,0,sizeof(str));
            memset(belong,0,sizeof(belong));
            ret=0;
         

            if(pcur->gets_or_puts==0)//puts
            {
                ret=3;
              //  send(sfd,&ret,4,MSG_NOSIGNAL);
                    Message pack={0};
                    short ret_s=(short)ret;
                    pack.command_type=htons(ret_s);
//                    send(sfd,&pack,sizeof(pack),MSG_NOSIGNAL);


                    Sign_Hash_box sign_box={0};
                      bool sigin_ret =Client_Sigin_hashGenerator(sign_box.Sign,(char *)&pack);
                    if(false==sigin_ret)
                        ERROR_CHECK(1,1,"SIGN FALIUE");
                    
                    int total_length=sizeof(pack)+sizeof(sign_box);
                    char *send_buff=(char *)malloc(total_length);
                    memcpy(send_buff,(char *)&sign_box,sizeof(sign_box));
                    char *tmp=send_buff+sizeof(sign_box);
                    memcpy(tmp,(char *)&pack,sizeof(pack));

                    send(sfd,send_buff,total_length,MSG_NOSIGNAL);
                  //服务器对签名的认证结果
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(0==ret) {printf("非法用户端\n");continue;}




               // recv(sfd,tool,4,MSG_NOSIGNAL);
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
            //    printf("5\n");
                goto END;
            }
           // printf("before gets\n");
            if(pcur->gets_or_puts==1)//gets
            {
                   i=0;j=0;ret=4;
                   int id=0;
             //   send(sfd,&ret,4,MSG_NOSIGNAL);
                    Message pack_init={0};
                    short ret_s=(short)ret;
                    pack_init.command_type=htons(ret_s);
                    Short_int int_pack={0};
                    Shor_contend_pack short_data_pack={0};

                    Sign_Hash_box sign_box={0};

                    id=pcur->ppid;
                    int_pack.contend=htons((short)id);
                    memcpy(short_data_pack.contend,pcur->belong,10);
                    char *tmp=short_data_pack.contend+10;
                    memcpy(tmp,pcur->fime_name,8);
                    printf("id is %d belong is %s file name is %s\n",(int)ntohs(int_pack.contend),pcur->belong,tmp);
                // memcpy(short_data_pack.contend,str,sizeof(str));
                


                    int total_length=sizeof(pack_init)+sizeof(Short_int)+sizeof(short_data_pack);
                    char *send_buff=(char *)malloc(total_length);
                    memset(send_buff,0,total_length);
                    memcpy(send_buff,(char *)&pack_init,sizeof(pack_init));
                    memcpy(&send_buff[sizeof(pack_init)],&int_pack,sizeof(int_pack));
                    char *tmp2=send_buff+sizeof(pack_init)+sizeof(int_pack);
                    memcpy(tmp2,(char *)&short_data_pack,sizeof(short_data_pack));
                    
                    int get_hash_total_length=sizeof(sign_box)+total_length;
                    char *get_hash_buff=(char*)malloc(sizeof(sign_box)+total_length);
                
                    char tmp_sign_buff[128]={0};
                    bool sigin_ret =Client_Sigin_hashGenerator(tmp_sign_buff,send_buff);
                    if(false==sigin_ret)
                        ERROR_CHECK(1,1,"SIGN FALIUE");
                    memcpy(sign_box.Sign,tmp_sign_buff,128);

                    memcpy(get_hash_buff,(char *)&sign_box,sizeof(sign_box));
                    char *tmp3=get_hash_buff+sizeof(sign_box);
                    memcpy(tmp3,send_buff,total_length);
                    
                    int send_length=send(sfd,get_hash_buff,get_hash_total_length,MSG_NOSIGNAL);
                    free(get_hash_buff);
                    free(send_buff);
    

                     //服务器对签名的认证结果
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(0==ret) {printf("非法用户端\n");continue;}

            


                 /*   int total_length=sizeof(pack_init)+sizeof(Short_int)+sizeof(short_data_pack);
                    char *send_buff=(char *)malloc(total_length);
                    memset(send_buff,0,total_length);
                    memcpy(send_buff,(char *)&pack_init,sizeof(pack_init));
                    memcpy(&send_buff[sizeof(pack_init)],&int_pack,sizeof(int_pack));
                    char *tmp2=send_buff+sizeof(pack_init)+sizeof(int_pack);
                    memcpy(tmp2,(char *)&short_data_pack,sizeof(short_data_pack));
                    int send_length=send(sfd,send_buff,total_length,MSG_NOSIGNAL);
                    free(send_buff);
 */
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

