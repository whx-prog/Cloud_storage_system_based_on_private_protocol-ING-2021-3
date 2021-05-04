#include "factory.h"
#include<time.h>
#define salt_len 10;
train_t t;
int data_len;
char str[128]={0};
int hashTable[1000]={0};
int login=0;

char *salt_creater(char *salt)
{
    char str[10+1]={0};
    int i,flag;
    srand(time(NULL));
    for(i=0;i<10;i++)
    {
        flag=rand()%3;
        switch(flag)
        {
        case 0:
            str[i]=rand()%26+'a';
            break;
        case 1:
            str[i]=rand()%26+'A';
            break;
        case 2:
            str[i]=rand()%10+'0';
            break;
        }
    }
    printf("%s\n",str);
    strcpy(salt,str);
    return str;
}



int gets_file(int sfd){
    int data_len;
    int ret;
    char buf[1000]={0};
    //接文件类型
    recvn(sfd,&data_len,4);
    recvn(sfd,buf,data_len);
    if(atoi(buf)==0){   //如果是文件类型
        //先接文件名
        recvn(sfd,&data_len,4);//接收火车头
        recvn(sfd,buf,data_len);//文件名
        int fd=open(buf,O_RDWR|O_CREAT,0666);
        ERROR_CHECK(fd,-1,"open");
        //接整个文件长度
        off_t file_size,Cur_size=0,last_size=0,part_size;
        ret=recvn(sfd,&data_len,4);//火车头
        ERROR_CHECK(ret,-1,"recvn");

        recvn(sfd,&file_size,data_len);//长度
        ERROR_CHECK(ret,-1,"recvn");
        printf("传送文件总大小为:%ldMB\n",file_size/(1024*1024));
        part_size=file_size/1000;

        time_t begin,end;
        begin=time(NULL);
        int fds[2];
        pipe(fds);//内核一个管道

        while(Cur_size<file_size)
        {
            ret=splice(sfd,NULL,fds[1],NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);//管道描述符时，必为NULL
            ERROR_CHECK(ret,-1,"splice");
            Cur_size+=ret;
            ret=splice(fds[0],NULL,fd,NULL,ret,SPLICE_F_MORE|SPLICE_F_MOVE);
            ERROR_CHECK(ret,-1,"splice");
        }


    }
    else if(atoi(buf)==1)
    {
        recvn(sfd,&data_len,4);
        // send(sfd,&i,4,MSG_NOSIGNAL);
        recvn(sfd,buf,data_len);
        printf("%s\n",buf); 
    }
    else if(atoi(buf)==2)
    {
        off_t size=0;
        recvn(sfd,&data_len,4);
        recvn(sfd,&size,data_len);

        recvn(sfd,&data_len,4);
        recvn(sfd,buf,data_len);
    }

}


int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,2);
    //初始化一个socket描述符，用于tcp通信
    hashNode_t InsertNode[]={
        {CD,NULL,"cd"},
        {LS,NULL,"ls"},
        {PUTS,NULL,"puts"},
        {GETS,NULL,"gets"},
        {REMOVE,NULL,"remove"},
        {PWD,NULL,"pwd"},
        {SIGIN,NULL,"sigin"},
        {LOGIN,NULL,"login"},
        {MKDIR,NULL,"mkdir"},
        {RMDIR,NULL,"rmdir"},
        {HELP,NULL,"help"},
        {EXIT,NULL,"exit"},
    };    

    for(unsigned long i=0;i<sizeof(InsertNode)/sizeof(hashNode_t);i++){
        hashInsert(hashTable,&InsertNode[i],i+1);
    }           //哈希表建立完毕
    int count=0;
    for(int i=0;i<1000;i++){
        if(hashTable[i]!=0)
            count++;
    }
    printf("        欢迎连接网盘");
    printf("注册输入sigin,登陆输入login\n");
    printf("hash 已有%d\n",count);


    int thread_num_init=0;
    int capacity_init=0;
    char ip[16]="";
    char port[10]="";
    conf_init(argv[1],ip,port,&thread_num_init,&capacity_init);


    char *argv1=argv[1];
    char *argv2=argv[2];
    char *file_name=NULL;

    //int thread_num=atoi(argv[3]);//线程数量
    //int capacity=atoi(argv[4]);//队列容量
    int thread_num=thread_num_init;
    int capacity=capacity_init;
    factory_t thread_data;//线程数据结构
    factory_init(&thread_data,thread_num,capacity);
    factory_start(&thread_data);
    int sfd;
    
    tcp_init(ip,port,&sfd);
    //tcp_init(argv[1],argv[2],&sfd);
    pnode_t pnew;
    pque_t pq=&thread_data.que;

    int epfd;
    epfd=epoll_create(1);
    struct epoll_event evs[3];
    epoll_init(epfd,sfd);
    epoll_init(epfd,STDIN_FILENO);

    int i;
    char salt[128]={0};
    int k=0;
    int epoll_ready_num,j;
    char buf[128]={0};
    char str[128]={0};
    char tool[4]={0};
    int ret;
    while(1)
    {   j=0;
        k=0;
        epoll_ready_num=epoll_wait(epfd,evs,2,-1);
        for(i=0;i<epoll_ready_num;i++)
        {
            if(evs[i].data.fd==sfd)
            {
                gets_file(sfd);
            }
            if(evs[i].data.fd==STDIN_FILENO)
            {   
                if(login==0)
                {
                    login_sigin(&login,sfd,epfd);
                }

                memset(buf,0,sizeof(buf));
                memset(str,0,sizeof(str));
                ret=read(STDIN_FILENO,buf,sizeof(buf));
                ERROR_CHECK(ret,-1,"read");
                i=0;j=0;
                while(buf[j]!=' '&&buf[j]!='\n'){
                    str[i++]=buf[j++];
                }

                // printf("inter is %s\n",str);
                ret=curtiny(str);
                printf("hashVal is %d\n============================\n",ret);

                //================================================================================== 
                /* if(0==ret)//无效指令
                   {
                   goto over;
                   }*/


                //========================================================================================================
                //strncpy(str,buf,strlen(buf));

                if(ret==1)//cd
                {
                    send(sfd,&ret,4,MSG_NOSIGNAL);
                    recv(sfd,str,4,MSG_NOSIGNAL);
                    memset(str,0,sizeof(str));
                    while(buf[j]!=' '){
                        j++;
                    }
                    buf[j]='\0';
                    j++;
                    while(buf[j]!='\n'){
                        str[k++]=buf[j++];
                    }
                    send(sfd,str,strlen(str),MSG_NOSIGNAL);
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(ret==0){
                        printf("该目录不存在\n");
                    }
                    continue;
                }

                if(ret==2)//ls
                {
                    send(sfd,&ret,4,MSG_NOSIGNAL);
                    continue;
                }
                if(ret==3)//puts
                {
                    
                    pnew=(pnode_t)calloc(1,sizeof(node_t));
//================================================================
                  ret=13;//同步认证
                  int ppid=0;
                  int belong[10]={0};
                 send(sfd,&ret,4,MSG_NOSIGNAL);
                 recv(sfd,belong,10,MSG_NOSIGNAL);
                 send(sfd,tool,4,MSG_NOSIGNAL);
                 recv(sfd,&ppid,4,MSG_NOSIGNAL);
                    pnew->ppid=ppid;
                    strcpy(pnew->belong,belong);

                    while(buf[j]!=' '){
                        j++;
                    }
                    buf[j]='\0';
                    j++;
                    while(buf[j]!='\n'){
                        str[k++]=buf[j++];
                    }
                    pnew->gets_or_puts=0; //puts是0
                    memcpy(&pnew->ip,&argv1,8);
                    memcpy(&pnew->port,&argv2,8);
                    strcpy(pnew->fime_name,str);

                    pthread_mutex_lock(&pq->mutex);
                    que_Tail_insert(pq,pnew);
                    pthread_mutex_unlock(&pq->mutex);
                    pthread_cond_signal(&thread_data.cond);
//===============================================================
                  /* send(sfd,&ret,4,MSG_NOSIGNAL);
                    memset(str,0,sizeof(str));
                    while(buf[j]!=' '){
                        j++;
                    }
                    buf[j]='\0';
                    j++;
                    while(buf[j]!='\n'){
                        str[k++]=buf[j++];
                    }
                    recv(sfd,tool,4,MSG_NOSIGNAL);
                    send(sfd,str,strlen(str),MSG_NOSIGNAL);
                    printf("文件名是:%s\n",str);
                  //  recv(sfd,tool,4,MSG_NOSIGNAL);
                  //  memset(buf,0,sizeof(buf));
                    //buf=md5
                    i=0;
                    char *carry=(char *)calloc(1000,1);
                    int fd=open(str,O_RDONLY);
                    ERROR_CHECK(fd,-1,"open");
                    printf("str is %s\n",str);
                    while((ret=read(fd,&carry[i++],1))!=0){
                        printf("i am reading\n");
                    }
                    printf("A\n");
                    char *res=getMd5(carry);
                    printf("%s\n",res);
                    tran_file(sfd,str,"0",0,0,res);
                    free(carry);
                   free(res);*/
//============================================================================
                    continue;
                    printf("J\n");
                }
                if(ret==4)//gets
                {  

                    pnew=(pnode_t)calloc(1,sizeof(node_t));
//=======================================================================
                  ret=13;//同步认证
                  int ppid=0;
                  int belong[10]={0};
                 send(sfd,&ret,4,MSG_NOSIGNAL);
                 recv(sfd,belong,10,MSG_NOSIGNAL);
                 send(sfd,tool,4,MSG_NOSIGNAL);
                 recv(sfd,&ppid,4,MSG_NOSIGNAL);
                    pnew->ppid=ppid;
                    strcpy(pnew->belong,belong);

      
                    while(buf[j]!=' '){
                        j++;
                    }
                    buf[j]='\0';
                    j++;
                    while(buf[j]!='\n'){
                        str[k++]=buf[j++];
                    }
                     pnew->gets_or_puts=1; //puts是0
                    
                     memcpy(&pnew->ip,&argv1,8);
                    memcpy(&pnew->port,&argv2,8);
    
                    strcpy(pnew->fime_name,str);
                  /*  strcpy(pnew->ip,argv[1]);
                    strcpy(pnew->port,argv[2]);
                    strcpy(pnew->fime_name,str);*/

                    pthread_mutex_lock(&pq->mutex);
                    que_Tail_insert(pq,pnew);
                    pthread_mutex_unlock(&pq->mutex);
                    pthread_cond_signal(&thread_data.cond);
//======================================================================

                  /*  i=0;j=0;
                    send(sfd,&ret,4,MSG_NOSIGNAL);
                    // printf("B\n");
                    recv(sfd,str,4,MSG_NOSIGNAL);
                    memset(str,0,sizeof(str));
                    while(buf[j]!=' '){
                        j++;
                    }
                    buf[j]='\0';
                    j++;
                    while(buf[j]!='\n'){
                        str[k++]=buf[j++];
                    }
                    send(sfd,str,strlen(str),MSG_NOSIGNAL);*/
                    continue;
                }
                if(ret==5)//remove
                {
                    i=0;
                    j=0;
                    send(sfd,&ret,4,MSG_NOSIGNAL);
                    memset(str,0,sizeof(str));
                    while(buf[j]!=' '){
                        j++;
                    }
                    buf[j]='\0';
                    j++;
                    while(buf[j]!='\n'){
                        str[k++]=buf[j++];
                    }
                    recv(sfd,tool,4,MSG_NOSIGNAL);
                  //  recv(sfd,tool,4,MSG_NOSIGNAL);
                  //  memset(buf,0,sizeof(buf));
                    //buf=md5
                    i=0;
                    char *carry=(char *)calloc(1000,1);
                    int fd=open(str,O_RDONLY);
                    ERROR_CHECK(fd,-1,"open");
                    while((ret=read(fd,&carry[i++],1))!=0){
                        printf("i am reading\n");
                    }
                    char *res=getMd5(carry);
                    send(sfd,res,strlen(res),MSG_NOSIGNAL);
                    free(carry);
                   free(res);
                    continue;

                
                }
                if(ret==6)//pwd
                {
                    send(sfd,&ret,4,MSG_NOSIGNAL);
                    continue;
                }
                if(ret==7)//sigin
                {
                    send(sfd,&ret,4,MSG_NOSIGNAL);
                    recv(sfd,str,4,MSG_NOSIGNAL);
                    memset(str,0,sizeof(str));
                    printf("请设置用户名\n");
                    scanf("%s",str);
                    strcat(str,"','");
                    printf("请设置密码:\n");
                    scanf("%s",buf);
                    strcat(str,buf);
                    strcat(str,"','");
                    memset(buf,0,sizeof(buf));
                    salt_creater(salt);
                    strcat(str,salt);
                    strcat(str,"')");
                    printf("input is %s\n",str);
                    send(sfd,str,strlen(str),MSG_NOSIGNAL); 
                    continue;
                }
                //ret=8 login
                if(ret==9)//mkdir
                {
                    send(sfd,&ret,4,MSG_NOSIGNAL);
                    recv(sfd,&str,4,MSG_NOSIGNAL);
                    memset(str,0,sizeof(str));
                    while(buf[j]!=' '){
                        j++;
                    }
                    buf[j]='\0';
                    j++;
                    while(buf[j]!='\n'){
                        str[k++]=buf[j++];
                    }
                    send(sfd,str,strlen(str),MSG_NOSIGNAL);
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(ret==0){
                        printf("创建失败\n");
                    }
                    else
                    {
                        printf("创建成功\n");
                    }
                    continue;

                }
                if(ret==10)//rmdir
                {
                    send(sfd,&ret,4,MSG_NOSIGNAL);
                    recv(sfd,tool,4,MSG_NOSIGNAL);
                    memset(str,0,sizeof(str));
                    while(buf[j]!=' '){
                        j++;
                    }
                    buf[j]='\0';
                    j++;
                    while(buf[j]!='\n'){
                        str[k++]=buf[j++];
                    }
                    send(sfd,str,strlen(str),MSG_NOSIGNAL);
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(ret==1){
                        printf("该目录非空,删除失败\n");
                    }
                    else
                    {
                        printf("删除成功\n");
                    }
                    continue;
                }
                if(ret==11)//help
                {
                    printf("cd:进入目录或返回上一级\nls:查看当前目录文件\ngets 文件名:下载文件\nputs 文件名:上传文件\nremove 文件名:删除文件\npwd:显示当前工作目录\nmkdir 文件夹名/:创建文件夹\nrmdir 文件夹名/:删除文件夹\nexit:退出网盘\n");        
                    continue;
                }
                if(ret==12)
                {
                    send(sfd,&ret,4,MSG_NOSIGNAL);
                    goto over;
                }
                printf("无效命令，输入help查看指令帮助\n");
            }
        }

    }
over:
    close(sfd);
    return 0;
}


int curtiny(char buf[]){
    int hashVal=hash(buf);
    if(hashTable[hashVal]!=0){
        return hashTable[hashVal];
    }
    if(hashTable[hashVal]==0){
        return 0;
    }
}
