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



void *thread_func(void*p){
    factory_t *pfac=(factory_t*)p;
    pque_t pq=&pfac->que;
    pnode_t pcur;
    int count=0;
    train_t t;

    // memset(pcur,0,sizeof(node_t));
    //pcur->login=0;
    while(1)
    {
        pthread_mutex_lock(&pq->mutex);
        if(!pq->que_size)
        {
            pthread_cond_wait(&pfac->cond,&pq->mutex);
        }
        que_pop(pq,&pcur);
        pthread_mutex_unlock(&pq->mutex);
       int epfd;
        epfd=epoll_create(1);
        struct epoll_event evs[1];
        epoll_init(epfd,pcur->new_fd);
        int epoll_ready_num,j,k;
        char buf[128]={0};
        char str[128]={0};
        char mid[128]={0};
        int ret=0;
        int dirID=0;
        int i;
        char tool[4]={0}; 
        int fd=open("log",O_CREAT|O_RDWR|O_APPEND,0666);
        time_t a;
#ifdef DEBUG
        while(1)
        {   j=0;
            k=0;
            memset(buf,0,sizeof(buf));
            memset(str,0,sizeof(buf));
            memset(mid,0,sizeof(mid));
            ret=0;
            dirID=0;
            epoll_ready_num=epoll_wait(epfd,evs,1,-1);
            for(i=0;i<epoll_ready_num;i++)
            {
                if(evs[i].data.fd==pcur->new_fd)
                {
                    // recvn(pcur->new_fd,&data_len,4);
                    // recvn(pcur->new_fd,buf,data_len);
                    // ret=atoi(buf);
                    recv(pcur->new_fd,&ret,4,0);

                    //   if(pcur->login==1)
                    //  {                 
                    switch (ret)
                    {
                     case 1://cd
                        send(pcur->new_fd,tool,4,MSG_NOSIGNAL);
                        recv(pcur->new_fd,buf,sizeof(buf),0);
                        pthread_mutex_lock(&pq->log_mutex);
                        time(&a);
                        sprintf(str,"%s%s%s%s%s%s",pcur->belong,":cd ",buf,"  time:",ctime(&a),"\n");
                        write(fd,str,strlen(str));
                        memset(str,0,sizeof(str));
                        pthread_mutex_unlock(&pq->log_mutex);
                        int ret_num;
                        int count=0;
                        int count_sum=0;
                        k=0;
                        if(buf[0]=='.')
                        {
                            while(buf[k]!='\0')
                            {   
                                if(buf[k]!='.'&&buf[k]!='/')
                                {
                                    count=-1;
                                    break;
                                }
                                if(buf[k]=='/')
                                    count++;
                                k++;
                                count_sum++;
                            }
                            if(count_sum%3!=0)
                            {  
                                count=-1;
                            }
                            if(pcur->Cur_arry_flag-count<0||count<0)
                            {  
                                ret=0;
                                send(pcur->new_fd,&ret,4,MSG_NOSIGNAL);
                            }
                            else
                            {
                                k=0;
                                ret=1;
                                pcur->Cur_arry_flag-=count;
                                 while(pcur->Cur_path[k++]!='\0');
                                 count+=1;
                                while(count!=0)
                                {
                                    if(pcur->Cur_path[k-1]=='/')
                                        count--;
                                    pcur->Cur_path[k--]='\0';
                                }
                                send(pcur->new_fd,&ret,4,MSG_NOSIGNAL);
                            }
                        }
                        else{
                            pcur->Cur_arry_flag+=1;
                            ret_num=query_fileID(pcur->pathID_box[pcur->Cur_arry_flag-1],buf);
                            if(ret_num!=0){
                                pcur->pathID_box[pcur->Cur_arry_flag]=ret_num;
                                pcur->path_Name_box[pcur->Cur_arry_flag]=buf;
                                strcat(pcur->Cur_path,buf);
                                send(pcur->new_fd,&ret_num,4,MSG_NOSIGNAL);
                            }
                            else
                            {
                                pcur->Cur_arry_flag-=1;//复原
                                send(pcur->new_fd,&ret_num,4,MSG_NOSIGNAL);
                            }
                        }
                        memset(str,0,sizeof(str));
                        strcpy(str,"cd");
                        strcat(str,buf);
                        strcat(str,"\n");
                        break;
                    case 2://ls
                        pthread_mutex_lock(&pq->log_mutex);
                        time(&a);
                        sprintf(str,"%s%s%s%s%s",pcur->belong,":ls","  time:",ctime(&a),"\n");
                        write(fd,str,strlen(str));
                        memset(str,0,sizeof(str));
                        pthread_mutex_unlock(&pq->log_mutex);
                        query_ls(pcur->pathID_box[pcur->Cur_arry_flag],pcur->new_fd);
                        //lsh(pcur->new_fd);
                        break;
                    case 3://puts
                        send(pcur->new_fd,tool,4,MSG_NOSIGNAL);
                        recv(pcur->new_fd,buf,sizeof(buf),MSG_NOSIGNAL);//文件名
                         pthread_mutex_lock(&pq->log_mutex);
                        time(&a);
                        sprintf(str,"%s%s%s%s%s%s",pcur->belong,":puts ",buf,"  time:",ctime(&a),"\n");
                        write(fd,str,strlen(str));
                        memset(str,0,sizeof(str));
                        pthread_mutex_unlock(&pq->log_mutex);
                     //   send(pcur->new_fd,tool,4,MSG_NOSIGNAL);
                        gets_file(pcur->new_fd,buf,pcur);//md5
                        ret=0;
                        break;
                    case 4://gets
                        send(pcur->new_fd,tool,4,MSG_NOSIGNAL);
                        recv(pcur->new_fd,buf,sizeof(buf),0);
                        pthread_mutex_lock(&pq->log_mutex);
                        time(&a);
                        sprintf(str,"%s%s%s%s%s%s",pcur->belong,":gets ",buf,"  time:",ctime(&a),"\n");
                        write(fd,str,strlen(str));
                        memset(str,0,sizeof(str));
                        pthread_mutex_unlock(&pq->log_mutex);
                        tran_file(pcur->new_fd,buf,"0",0,0,pcur);
                        break;
                    case 5://remove
                        send(pcur->new_fd,tool,4,MSG_NOSIGNAL);
                        recv(pcur->new_fd,buf,sizeof(buf),0);//md5值
                         pthread_mutex_lock(&pq->log_mutex);
                        time(&a);
                        sprintf(str,"%s%s%s%s%s%s",pcur->belong,":remove ",buf,"  time:",ctime(&a),"\n");
                        write(fd,str,strlen(str));
                        memset(str,0,sizeof(str));
                        pthread_mutex_unlock(&pq->log_mutex);
                        ret=query_md5_isexist(buf);
                        if(ret==0)
                        {
                            printf("指令无效\n");
                        }
                        if(ret==1)
                        {
                            ret=query_md5_count(buf);
                            ret--;
                            if(ret==0)
                            {
                                goto delete_table;
                            }
                            delete_file_table(pcur->pathID_box[pcur->Cur_arry_flag],buf);
                            update_md5_count(buf,ret);

                        }
                         if(ret==2){
delete_table:               
                            delete_file_table(pcur->pathID_box[pcur->Cur_arry_flag],buf);
                            delete_md5_table(buf);
                            unlink(buf);
                        }

                        ret=0;
                        break;
                    case 6://pwd
                         pthread_mutex_lock(&pq->log_mutex);
                        time(&a);
                        sprintf(str,"%s%s%s%s%s%s",pcur->belong,":pwd","  time:",ctime(&a),"\n");
                        write(fd,str,strlen(str));
                        memset(str,0,sizeof(str));
                        pthread_mutex_unlock(&pq->log_mutex);
                        strncpy(buf,pcur->Cur_path,strlen(pcur->Cur_path));
                        count=strlen(pcur->Cur_path);

                        t.data_len=strlen("1");
                        strcpy(t.buf,"1");
                        ret=send(pcur->new_fd,&t,4+t.data_len,MSG_NOSIGNAL);

                        t.data_len=count;
                        memcpy(t.buf,buf,t.data_len);
                        send(pcur->new_fd,&t,4+t.data_len,MSG_NOSIGNAL);
                        break;
                    case 7://sigin
                         pthread_mutex_lock(&pq->log_mutex);
                        time(&a);
                        sprintf(str,"%s%s%s%s%s",pcur->belong,":someone sigin","  time:",ctime(&a),"\n");
                        write(fd,str,strlen(str));
                        memset(str,0,sizeof(str));
                        pthread_mutex_unlock(&pq->log_mutex);
                        send(pcur->new_fd,tool,4,MSG_NOSIGNAL);
                        //client_data(buf,mid,str);
                        recv(pcur->new_fd,buf,sizeof(buf),0);
                        while(buf[k]!='\'')
                            str[j++]=buf[k++];    
                        sigin_insert_client(buf);
                        sigin_insert_file(str);
                        break;
                    case 8://login
                        memset(pcur->pathID_box,0,sizeof(pcur->pathID_box));
                        memset(pcur->path_Name_box,0,sizeof(pcur->path_Name_box));
                        send(pcur->new_fd,tool,4,MSG_NOSIGNAL);
                        recv(pcur->new_fd,buf,sizeof(buf),0);
                         pthread_mutex_lock(&pq->log_mutex);
                        time(&a);
                        sprintf(str,"%s%s%s%s%s%s",pcur->belong,":login ",buf,"  time:",ctime(&a),"\n");
                        write(fd,str,strlen(str));
                        memset(str,0,sizeof(str));
                        pthread_mutex_unlock(&pq->log_mutex);
                        j=query_mysql(buf);
                        if(j==0){
                            send(pcur->new_fd,&j,4,MSG_NOSIGNAL);
                        }
                        if(j==1){//登陆成功
                            send(pcur->new_fd,&j,4,MSG_NOSIGNAL);
                            k=0;j=0;
                            while(buf[k]!='\'')
                                str[j++]=buf[k++];

                            memcpy(pcur->belong,str,10);
                            strcpy(pcur->Cur_path,str);
                            strcat(pcur->Cur_path,":~/");
                            pcur->path_Name_box[0]=(pcur->Cur_path);
                            pcur->Cur_arry_flag=0;
                            pcur->pathID_box[0]=query_root_fileID(str);
                           /* printf("belong is %s\n Cur_path is %s\n path_Name_box is %s\n Cur_flag is %d\n pathIDbox is %d\n"
                                   ,pcur->belong,pcur->Cur_path,pcur->path_Name_box[0],pcur->Cur_arry_flag,pcur->pathID_box[0]);*/
                            // strcpy(pcur->Cur_path,pcur->path_Name_box[0]);
                            // query_fileID(str);
                        }
                        break;
                     case 9://mkdir
                        ret=1;
                        send(pcur->new_fd,tool,4,MSG_NOSIGNAL);
                        recv(pcur->new_fd,buf,sizeof(buf),0);
                         pthread_mutex_lock(&pq->log_mutex);
                        time(&a);
                        sprintf(str,"%s%s%s%s%s%s",pcur->belong,":mkdir ",buf,"  time:",ctime(&a),"\n");
                        write(fd,str,strlen(str));
                        memset(str,0,sizeof(str));
                        pthread_mutex_unlock(&pq->log_mutex);
                        insert_dir(pcur->pathID_box[pcur->Cur_arry_flag],buf,pcur->belong);
                        send(pcur->new_fd,&ret,4,MSG_NOSIGNAL);
                        break;
                    case 10://rmdir
                        send(pcur->new_fd,tool,4,MSG_NOSIGNAL);
                        recv(pcur->new_fd,buf,sizeof(buf),0);
                         pthread_mutex_lock(&pq->log_mutex);
                        time(&a);
                        sprintf(str,"%s%s%s%s%s%s",pcur->belong,":rmdir ",buf,"  time:",ctime(&a),"\n");
                        write(fd,str,strlen(str));
                        memset(str,0,sizeof(str));
                        pthread_mutex_unlock(&pq->log_mutex);
                        dirID=query_dirID(pcur->pathID_box[pcur->Cur_arry_flag],buf);
                        ret=query_file_isexist(dirID);
                        if(ret==1)
                        {
                            send(pcur->new_fd,&ret,4,MSG_NOSIGNAL);
                        }
                        else
                        {
                            delete_dir(dirID);
                            send(pcur->new_fd,&ret,4,MSG_NOSIGNAL);
                        }
                        break;
                    case 12:
                         pthread_mutex_lock(&pq->log_mutex);
                        time(&a);
                        sprintf(str,"%s%s%s%s%s",pcur->belong,":exit","  time:",ctime(&a),"\n");
                        write(fd,str,strlen(str));
                        memset(str,0,sizeof(str));
                        pthread_mutex_unlock(&pq->log_mutex);
                        goto END;
                        break;
                
                }//switch

                }
                }
            }
#endif
END:
            close(fd);
            free(pcur);
            pcur=NULL;
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

