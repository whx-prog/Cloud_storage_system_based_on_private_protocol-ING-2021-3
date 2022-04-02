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
            printf("接受结束！\n");

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

/*
bool Client_Sigin_hashGenerator(char *a,char *b)
{
    printf("clien sign A\n");
    int test_value=4;
   // char *data = "china";
    char data[4] = {0};
    memcpy(data,&test_value,4);

    char buf[128] = {0};
    RSA *pubKey = NULL;
    RSA *privKey = NULL;
    int nOutLen = sizeof(buf);
    int nRet = 0;

    printf("clien sign A\n");
    //1. 对数据进行sha256算法摘要
    unsigned char md[WHICH_DIGEST_LENGTH];
#if isUseSha256
    SHA256((unsigned char *)data, strlen(data), md);
#else
    SHA512((unsigned char *)data, strlen(data), md);
#endif
    printHex(md, WHICH_DIGEST_LENGTH);

    printf("clien sign A\n");
    // 2. 读取私钥
    privKey = ReadPrivateKey(PRIVATE_KEY_PATH);
    if (!privKey) 
    {  
   //     ERR_print_errors_fp (stderr);    
        return -1;  
    }

    printf("clien sign A\n");
    // 3. 读取公匙
    pubKey = ReadPublicKey(PUBLIC_KEY_PATH);  
    if(!pubKey)
    {
        RSA_free(privKey);   
        printf("Error: can't load public key");
        return -1;
    }

    printf("clien sign A\n");
    //双方都在发送的时候先签名，然后将公钥一起发出去
    // 4. 签名
    nRet = RSA_sign(SHA_WHICH, md, WHICH_DIGEST_LENGTH, buf, &nOutLen, privKey);
    if(nRet != 1)
    {
        printf("RSA_sign err !!! \n");    
        goto quit;
    }
    printf("RSA_sign len = %d:", nOutLen);
    printHex(buf, nOutLen);

    printf("clien sign A\n");

    //双方都只需要收到消息后验签即可
    // 5. 验签 
    nRet = RSA_verify(SHA_WHICH, md, WHICH_DIGEST_LENGTH, buf, nOutLen, pubKey);
    if(nRet != 1)
    {
        printf("RSA_verify err !!! \n");    
        goto quit;
    }
    printf("RSA_verify Success !!! \n");  

    printf("clien sign A\n");
quit:
    RSA_free(privKey);
    RSA_free(pubKey);

    printf("clien sign A\n");
    return false;

}
*/


/*
bool Client_Sigin_hashGenerator(char * Sign_buf,char *data)
{
    printf("client sign A\n");
    RSA *privKey=NULL;
    char *data2="china";
    int nOutLen=128;
    int nRet=0;
    char buf[128]={0};
    //对数据进行sha256算法摘要
   unsigned char md[WHICH_DIGEST_LENGTH];
  #if isUseSha256
      SHA256((unsigned char *)data2, strlen(data), md);
  #else
      SHA512((unsigned char *)data2, strlen(data), md);
  #endif
      printHex(md, WHICH_DIGEST_LENGTH);
  
    printf("client sign b\n");
      // 2. 读取私钥
     privKey = ReadPrivateKey(PRIVATE_KEY_PATH);
     if (!privKey) 
     {  
    //     ERR_print_errors_fp (stderr);    
         return -1;  
     }

    printf("client sign c\n");
     //生成签名
  nRet = RSA_sign(SHA_WHICH, md, WHICH_DIGEST_LENGTH, buf, &nOutLen, privKey);
     
    printf("client sign d\n");
  if(nRet != 1)
     {
         printf("RSA_sign err !!! \n");
         RSA_free(privKey);
         return false;
     }
     printf("RSA_sign len = %d:", nOutLen);
     printHex(buf, nOutLen);
     RSA_free(privKey);
    return true;
}
*/



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
        {LOCAL,NULL,"local"}
    };    

    for(unsigned long i=0;i<sizeof(InsertNode)/sizeof(hashNode_t);i++){
        hashInsert(hashTable,&InsertNode[i],i+1);
    }           //哈希表建立完毕
    int count=0;
    for(int i=0;i<1000;i++){
        if(hashTable[i]!=0)
            count++;
    }
   
   // do_jsonTest();
   //test_RSA_sign_verify();
//Client_Sigin_hashGenerator("asd","awdaw");
    printf("        欢迎连接网盘\n");
    printf("注册输入sigin,登陆输入login\n");
//    printf("hash 已有%d\n",count);


    int thread_num_init=0;
    int capacity_init=0;
    char ip[16]="";
    char port[10]="";
    conf_init(argv[1],ip,port,&thread_num_init,&capacity_init);

//    printf("B");

    char *argv1=ip;
    char *argv2=port;
    char *file_name=NULL;

  //  printf("C");
    //int thread_num=atoi(argv[3]);//线程数量
    //int capacity=atoi(argv[4]);//队列容量
    int thread_num=thread_num_init;
    int capacity=capacity_init;
    factory_t thread_data;//线程数据结构
    factory_init(&thread_data,thread_num,capacity);
    factory_start(&thread_data);
    int sfd;
    
   // printf("D");
    tcp_init(ip,port,&sfd);
    //tcp_init(argv[1],argv[2],&sfd);
    pnode_t pnew;
    pque_t pq=&thread_data.que;

    int epfd;
    epfd=epoll_create(1);
    struct epoll_event evs[3];
    epoll_init(epfd,sfd);
    epoll_init(epfd,STDIN_FILENO);

   // printf("E");
    int i;
    char salt[128]={0};
    int k=0;
    int epoll_ready_num,j;
    char buf[128]={0};
    char str[128]={0};
    char tool[4]={0};
    int ret;
   // printf("F");
    while(1)
    {   j=0;
        k=0;
        epoll_ready_num=epoll_wait(epfd,evs,2,-1);
        for(i=0;i<epoll_ready_num;i++)
        {
            if(evs[i].data.fd==sfd)
            {
                gets_file(sfd);
                
//                printf("\n===================================================\n");
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
            
                system("clear");

                printf("cd ../:进入目录或返回上一级\nls:查看当前目录文件\ngets 文件名:下载文件\nputs 文件名:上传文件\nremove 文件名:删除文件\npwd:显示当前工作目录\nmkdir 文件夹名/:创建文件夹\nrmdir 文件夹名/:删除文件夹\nexit:退出网盘\n注意,对问价夹的操作需要在文件夹名尾加上正斜杠\n");        
                printf("\n==========================本地目录文件夹=============================\n");
                system("tree ./");
                printf("\n============================文件具体属性================================\n");
                system("ls -alh");
                printf("\n==========================私有协议网盘云端=============================\n");

                if(ret==1)//cd
                {

                    Message pack_init={0};
                    pack_init.command_type=htons((short)ret); 
                    pack_init.Follow_package_length=htons((short)sizeof(Shor_contend_pack));
                    Shor_contend_pack short_data_pack={0}; 
                    
                 //                      send(sfd,&ret,4,MSG_NOSIGNAL);
  //                  recv(sfd,str,4,MSG_NOSIGNAL);
                    memset(str,0,sizeof(str));
                    while(buf[j]!=' '){
                        j++;
                    }
                    buf[j]='\0';
                    j++;
                    while(buf[j]!='\n'){
                        str[k++]=buf[j++];
                    }
   //                 send(sfd,str,strlen(str),MSG_NOSIGNAL);

                   Sign_Hash_box sign_box={0};
                    memcpy(short_data_pack.contend,str,sizeof(str));
                    int total_length=sizeof(pack_init)+sizeof(short_data_pack);
                    char *send_buff=(char *)malloc(total_length);
                    memset(send_buff,0,total_length);
                    memcpy(send_buff,(char *)&pack_init,sizeof(pack_init));
                    char *tmp=send_buff+sizeof(pack_init);
                    memcpy(tmp,&short_data_pack,sizeof(short_data_pack));
                    
                    int get_hash_total_length=sizeof(sign_box)+total_length;
                    char *get_hash_buff=(char*)malloc(sizeof(sign_box)+total_length);
                
                    char tmp_sign_buff[128]={0};
                    bool sigin_ret =Client_Sigin_hashGenerator(tmp_sign_buff,send_buff);
                    if(false==sigin_ret)
                        ERROR_CHECK(1,1,"SIGN FALIUE");
                    memcpy(sign_box.Sign,tmp_sign_buff,128);

                    memcpy(get_hash_buff,(char *)&sign_box,sizeof(sign_box));
                    char *tmp2=get_hash_buff+sizeof(sign_box);
                    memcpy(tmp2,send_buff,total_length);
                    
                    int send_length=send(sfd,get_hash_buff,get_hash_total_length,MSG_NOSIGNAL);
                    free(get_hash_buff);
                    free(send_buff);
    

                     //服务器对签名的认证结果
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(0==ret) {printf("非法用户端\n");continue;}



                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(ret==0){
                        printf("该目录不存在\n");
                    }


                }

                else  if(ret==2)//ls
                {
//                    send(sfd,&ret,4,MSG_NOSIGNAL);
                   Sign_Hash_box sign_box={0};
                    Message pack={0};
                    short ret_s=(short)ret;
                     pack.command_type=htons(ret_s);

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

                }
                else  if(ret==3)//puts
                {
                    
                    pnew=(pnode_t)calloc(1,sizeof(node_t));
//================================================================
                  ret=13;//同步认证
                  int ppid=0;
                int belong[10]={0};
                
                    Message pack={0};
                    short ret_s=(short)ret;
                    pack.command_type=htons(ret_s);
   
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
                    else printf("验签成功\n");


    //            send(sfd,&ret,4,MSG_NOSIGNAL);
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
                }
                else  if(ret==4)//gets
                {  

                    pnew=(pnode_t)calloc(1,sizeof(node_t));
//=======================================================================
                  ret=13;//同步认证
                  int ppid=0;
                  int belong[10]={0};
  //               send(sfd,&ret,4,MSG_NOSIGNAL);
                   Message pack={0};
                    short ret_s=(short)ret;
                    pack.command_type=htons(ret_s);
                   
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
                }
                else if(ret==5)//remove
                {
                    i=0;
                    j=0;
                    //send(sfd,&ret,4,MSG_NOSIGNAL);
               
                     Message pack_init={0};
                    pack_init.command_type=htons((short)ret); 
                    pack_init.Follow_package_length=htons((short)sizeof(Shor_contend_pack));
                    Shor_contend_pack short_data_pack={0}; 
                   


                    memset(str,0,sizeof(str));
                    while(buf[j]!=' '){
                        j++;
                    }
                    buf[j]='\0';
                    j++;
                    while(buf[j]!='\n'){
                        str[k++]=buf[j++];
                    }
            //                         i=0;
                    char *carry=(char *)calloc(1000,1);
                    int fd=open(str,O_RDONLY);
                    ERROR_CHECK(fd,-1,"open");
                    while((ret=read(fd,&carry[i++],1))!=0){
                        printf("i am reading\n");
                        if(i==1000){
                        carry[1000]='\0';
                        break;
                         }
                    }
                    char *res=getMd5(carry);
              
                    Sign_Hash_box sign_box={0};
                     memcpy(short_data_pack.contend,res,sizeof(str));
                    int total_length=sizeof(pack_init)+sizeof(short_data_pack);
                    char *send_buff=(char *)malloc(total_length);
                    memset(send_buff,0,total_length);
                    memcpy(send_buff,(char *)&pack_init,sizeof(pack_init));
                    char *tmp=send_buff+sizeof(pack_init);
                    memcpy(tmp,(char *)&short_data_pack,sizeof(short_data_pack));
                     
                    int get_hash_total_length=sizeof(sign_box)+total_length;
                    char *get_hash_buff=(char*)malloc(sizeof(sign_box)+total_length);
                
                    char tmp_sign_buff[128]={0};
                    bool sigin_ret =Client_Sigin_hashGenerator(tmp_sign_buff,send_buff);
                    if(false==sigin_ret)
                        ERROR_CHECK(1,1,"SIGN FALIUE");
                    memcpy(sign_box.Sign,tmp_sign_buff,128);

                    memcpy(get_hash_buff,(char *)&sign_box,sizeof(sign_box));
                    char *tmp2=get_hash_buff+sizeof(sign_box);
                    memcpy(tmp2,send_buff,total_length);
                    
                    int send_length=send(sfd,get_hash_buff,get_hash_total_length,MSG_NOSIGNAL);
                    free(get_hash_buff);
                    free(send_buff);
    

                     //服务器对签名的认证结果
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(0==ret) {printf("非法用户端\n");continue;}




                 /*   memcpy(short_data_pack.contend,res,sizeof(str));
                    int total_length=sizeof(pack_init)+sizeof(short_data_pack);
                    char *send_buff=(char *)malloc(total_length);
                    memset(send_buff,0,total_length);
                    memcpy(send_buff,(char *)&pack_init,sizeof(pack_init));
                    char *tmp=send_buff+sizeof(pack_init);
                    memcpy(tmp,(char *)&short_data_pack,sizeof(short_data_pack));
                    int send_length=send(sfd,send_buff,total_length,MSG_NOSIGNAL);
                    free(send_buff);
              */
                    //     send(sfd,res,strlen(res),MSG_NOSIGNAL);
                    free(carry);
                   free(res);

                
                }
                 else if(ret==6)//pwd
                {
                    Message pack={0};
                    short ret_s=(short)ret;
                    pack.command_type=htons(ret_s);
               
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




//                    send(sfd,&ret,4,MSG_NOSIGNAL);
                }
                else if(ret==7)//sigin
                {
                    
                  Message pack_init={0};
                   pack_init.command_type=htons((short)ret); 
                   pack_init.Follow_package_length=htons((short)sizeof(User_private_pack));

                   User_private_pack user_data_pack={0}; 



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
                   
                     Sign_Hash_box sign_box={0};
                    memcpy(user_data_pack.user_data,str,sizeof(str));
                    int total_length=sizeof(pack_init)+sizeof(user_data_pack);
                    char *send_buff=(char *)malloc(total_length);
                    memset(send_buff,0,total_length);
                    memcpy(send_buff,(char *)&pack_init,sizeof(pack_init));
                    char *tmp=send_buff+sizeof(pack_init);
                    memcpy(tmp,&user_data_pack,sizeof(user_data_pack));
                    
                    int get_hash_total_length=sizeof(sign_box)+total_length;
                    char *get_hash_buff=(char*)malloc(sizeof(sign_box)+total_length);
                
                    char tmp_sign_buff[128]={0};
                    bool sigin_ret =Client_Sigin_hashGenerator(tmp_sign_buff,send_buff);
                    if(false==sigin_ret)
                        ERROR_CHECK(1,1,"SIGN FALIUE");
                    memcpy(sign_box.Sign,tmp_sign_buff,128);

                    memcpy(get_hash_buff,(char *)&sign_box,sizeof(sign_box));
                    char *tmp2=get_hash_buff+sizeof(sign_box);
                    memcpy(tmp2,send_buff,total_length);
                    
                    int send_length=send(sfd,get_hash_buff,get_hash_total_length,MSG_NOSIGNAL);
                    free(get_hash_buff);
                    free(send_buff);
    

                    
                  //服务器对签名的认证结果
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(0==ret) {printf("非法用户端\n");continue;}

 


                   // send(sfd,str,strlen(str),MSG_NOSIGNAL); 
                }
                //ret=8 login
                else if(ret==9)//mkdir
                {
                    Message pack_init={0};
                    pack_init.command_type=htons((short)ret); 
                    pack_init.Follow_package_length=htons((short)sizeof(Shor_contend_pack));
                    Shor_contend_pack short_data_pack={0}; 


                 


                    memset(str,0,sizeof(str));
                    while(buf[j]!=' '){
                        j++;
                    }
                    buf[j]='\0';
                    j++;
                    while(buf[j]!='\n'){
                        str[k++]=buf[j++];
                    }
                    if(NULL==strchr(str,'/'))
                    {
                        strcat(str,"/");
                    }

                     Sign_Hash_box sign_box={0};
                     memcpy(short_data_pack.contend,str,sizeof(str));
                    int total_length=sizeof(pack_init)+sizeof(short_data_pack);
                    char *send_buff=(char *)malloc(total_length);
                    memset(send_buff,0,total_length);
                    memcpy(send_buff,(char *)&pack_init,sizeof(pack_init));
                    char *tmp=send_buff+sizeof(pack_init);
                    memcpy(tmp,(char *)&short_data_pack,sizeof(short_data_pack));
                    
                    int get_hash_total_length=sizeof(sign_box)+total_length;
                    char *get_hash_buff=(char*)malloc(sizeof(sign_box)+total_length);
                
                    char tmp_sign_buff[128]={0};
                    bool sigin_ret =Client_Sigin_hashGenerator(tmp_sign_buff,send_buff);
                    if(false==sigin_ret)
                        ERROR_CHECK(1,1,"SIGN FALIUE");
                    memcpy(sign_box.Sign,tmp_sign_buff,128);

                    memcpy(get_hash_buff,(char *)&sign_box,sizeof(sign_box));
                    char *tmp2=get_hash_buff+sizeof(sign_box);
                    memcpy(tmp2,send_buff,total_length);
                    
                    int send_length=send(sfd,get_hash_buff,get_hash_total_length,MSG_NOSIGNAL);
                    free(get_hash_buff);
                    free(send_buff);
    

                    
                  //服务器对签名的认证结果
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(0==ret) {printf("非法用户端\n");continue;}




/*                     memcpy(short_data_pack.contend,str,sizeof(str));
                    int total_length=sizeof(pack_init)+sizeof(short_data_pack);
                    char *send_buff=(char *)malloc(total_length);
                    memset(send_buff,0,total_length);
                    memcpy(send_buff,(char *)&pack_init,sizeof(pack_init));
                    char *tmp=send_buff+sizeof(pack_init);
                    memcpy(tmp,(char *)&short_data_pack,sizeof(short_data_pack));
                    int send_length=send(sfd,send_buff,total_length,MSG_NOSIGNAL);
                    free(send_buff);
  */              

                    //  send(sfd,str,strlen(str),MSG_NOSIGNAL);
                   
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(ret==0){
                        printf("创建失败\n");
                    }
                    else
                    {
                        printf("创建成功\n");
                    }

                }
                else if(ret==10)//rmdir
                {
                   // send(sfd,&ret,4,MSG_NOSIGNAL);
                   // recv(sfd,tool,4,MSG_NOSIGNAL);
                    Message pack_init={0};
                    pack_init.command_type=htons((short)ret); 
                    pack_init.Follow_package_length=htons((short)sizeof(Shor_contend_pack));
                    Shor_contend_pack short_data_pack={0}; 

                 
                    memset(str,0,sizeof(str));
                    while(buf[j]!=' '){
                        j++;
                    }
                    buf[j]='\0';
                    j++;
                    while(buf[j]!='\n'){
                        str[k++]=buf[j++];
                    }

                     Sign_Hash_box sign_box={0};
                    memcpy(short_data_pack.contend,str,sizeof(str));
                    int total_length=sizeof(pack_init)+sizeof(short_data_pack);
                    char *send_buff=(char *)malloc(total_length);
                    memset(send_buff,0,total_length);
                    memcpy(send_buff,(char *)&pack_init,sizeof(pack_init));
                    char *tmp=send_buff+sizeof(pack_init);
                    memcpy(tmp,(char *)&short_data_pack,sizeof(short_data_pack));
                    
                    int get_hash_total_length=sizeof(sign_box)+total_length;
                    char *get_hash_buff=(char*)malloc(sizeof(sign_box)+total_length);
                
                    char tmp_sign_buff[128]={0};
                    bool sigin_ret =Client_Sigin_hashGenerator(tmp_sign_buff,send_buff);
                    if(false==sigin_ret)
                        ERROR_CHECK(1,1,"SIGN FALIUE");
                    memcpy(sign_box.Sign,tmp_sign_buff,128);

                    memcpy(get_hash_buff,(char *)&sign_box,sizeof(sign_box));
                    char *tmp2=get_hash_buff+sizeof(sign_box);
                    memcpy(tmp2,send_buff,total_length);
                    
                    int send_length=send(sfd,get_hash_buff,get_hash_total_length,MSG_NOSIGNAL);
                    free(get_hash_buff);
                    free(send_buff);
    

                    
                  //服务器对签名的认证结果
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(0==ret) {printf("非法用户端\n");continue;}




/*                    memcpy(short_data_pack.contend,str,sizeof(str));
                    int total_length=sizeof(pack_init)+sizeof(short_data_pack);
                    char *send_buff=(char *)malloc(total_length);
                    memset(send_buff,0,total_length);
                    memcpy(send_buff,(char *)&pack_init,sizeof(pack_init));
                    char *tmp=send_buff+sizeof(pack_init);
                    memcpy(tmp,(char *)&short_data_pack,sizeof(short_data_pack));
                    int send_length=send(sfd,send_buff,total_length,MSG_NOSIGNAL);
                    free(send_buff);
  */              



                    //   send(sfd,str,strlen(str),MSG_NOSIGNAL);
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(ret==1){
                        printf("该目录非空,删除失败\n");
                    }
                    else
                    {
                        printf("删除成功\n");
                    }
                }
                else if(ret==11)//help
                {
                    printf("cd:进入目录或返回上一级\nls:查看当前目录文件\ngets 文件名:下载文件\nputs 文件名:上传文件\nremove 文件名:删除文件\npwd:显示当前工作目录\nmkdir 文件夹名/:创建文件夹\nrmdir 文件夹名/:删除文件夹\nexit:退出网盘\n");        
                }
                else if(ret==12)//exit
                {
                    Message pack={0};
                    short ret_s=(short)ret;
                    pack.command_type=htons(ret_s);
   
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


                   // send(sfd,&ret,4,MSG_NOSIGNAL);
                    goto over;
                }else if(ret==13)
                {
                    //指令13用于测试，输入local即可进入
               }
                else
                printf("无效命令，输入help查看指令帮助\n");
            
                //printf("\n===============================================\n");

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
