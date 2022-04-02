#include "factory.h"
int login_sigin(int *login,int sfd,int not_fd)
{ 
char str[128]={0};
char buf[128]={0};
char salt[128]={0};
int ret;
int i,j,k;


 int epoll_ready_num;
    int epfd;
    epfd=epoll_create(1);
    struct epoll_event evs[3];
    epoll_init(epfd,sfd);
    epoll_init(epfd,STDIN_FILENO);
        
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
//                printf("hashVal is %d\n",ret);
                

                if(ret==7)//sigin
                {
                   // send(sfd,&ret,4,MSG_NOSIGNAL);
                   // recv(sfd,str,4,MSG_NOSIGNAL);

                    
                    Message pack_init={0};
                   pack_init.command_type=htons((short)ret); 
                   pack_init.Follow_package_length=htons((short)sizeof(User_private_pack));

                   User_private_pack user_data_pack={0}; 



                    memset(str,0,sizeof(str));
                    system("clear");
                    printf("请设置用户名\n");
                    scanf("%s",str);
                    strcat(str,"','");
                    system("clear");
                    printf("请设置密码:\n");
                    scanf("%s",buf);
                    strcat(str,buf);
                    strcat(str,"','");
                    memset(buf,0,sizeof(buf));
                    salt_creater(salt);
                    strcat(str,salt);
                    strcat(str,"')");
//                    printf("input is %s\n",str);

                     Sign_Hash_box sign_box={0};
                   // memcpy(user_data_pack.user_data,str,sizeof(str));
                   RSA_encrypt(str,user_data_pack.user_data);
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
    

                    
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(0==ret)
                    {
                        printf("验签失败\n");
                        return 0;
                    }
                    else
                         printf("验签成功\n");

  

//  send(sfd,str,strlen(str),MSG_NOSIGNAL);
                    system("clear");
                    printf("注册成功\n");
                }
                 if(ret==8){

                   //  send(sfd,&ret,4,MSG_NOSIGNAL);
                   // recv(sfd,str,4,MSG_NOSIGNAL);
                    Message pack_init={0};
                   pack_init.command_type=htons((short)ret); 
                   pack_init.Follow_package_length=htons((short)sizeof(User_private_pack));

                   User_private_pack user_data_pack={0}; 

                    memset(str,0,sizeof(str));
                    system("clear");
                    printf("请输入用户名\n");
                    scanf("%s",str);
                    strcat(str,"' and keyword='");
                    system("clear");
                    printf("请输入密码:\n");
                    scanf("%s",buf);
                    strcat(str,buf);
                    strcat(str,"'");
                    printf("input is %s\n",str);
                    
                   RSA_encrypt(str,user_data_pack.user_data);

                     Sign_Hash_box sign_box={0};
                   // memcpy(user_data_pack.user_data,str,sizeof(str));
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
    

                    
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(0==ret)
                    {
                        printf("验签失败\n");
                        return 0;
                    }
                    else
                         printf("验签成功\n");

                    //  send(sfd,str,strlen(str),MSG_NOSIGNAL);
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(ret==1){//登陆成功
                    system("clear");
                        printf("登陆成功\n");
                        *login=1;
                        return 0;
                    }
                 }

    
    while(1)
    {   j=0;
                    system("clear");
        printf("注册输入sigin,登陆输入login\n");
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
  //              printf("hashVal is %d\n",ret);

                if(ret==7)//sigin
                {
                  //  send(sfd,&ret,4,MSG_NOSIGNAL);
                  //  recv(sfd,str,4,MSG_NOSIGNAL);

                  Message pack_init={0};
                   pack_init.command_type=htons((short)ret); 
                   pack_init.Follow_package_length=htons((short)sizeof(User_private_pack));

                   User_private_pack user_data_pack={0}; 




                    memset(str,0,sizeof(str));
                    system("clear");
                    printf("请设置用户名\n");
                    scanf("%s",str);
                    strcat(str,"','");
                    system("clear");
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
                  //  memcpy(user_data_pack.user_data,str,sizeof(str));
                   RSA_encrypt(str,user_data_pack.user_data);
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
    

                    
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(0==ret)
                    {
                        printf("验签失败\n");
                        return 0;
                    }
                    else
                         printf("验签成功\n");

                    // send(sfd,str,strlen(str),MSG_NOSIGNAL);
                    system("clear");
                    printf("注册成功!\n");
                }
                if(ret==8){
               //      send(sfd,&ret,4,MSG_NOSIGNAL);
                //    recv(sfd,str,4,MSG_NOSIGNAL);
                    
                  Message pack_init={0};
                   pack_init.command_type=htons((short)ret); 
                   pack_init.Follow_package_length=htons((short)sizeof(User_private_pack));

                   User_private_pack user_data_pack={0}; 



                    memset(str,0,sizeof(str));
                    system("clear");
                    printf("请输入用户名\n");
                    scanf("%s",str);
                    strcat(str,"' and keyword='");
                    system("clear");
                    printf("请输入密码:\n");
                    scanf("%s",buf);
                    strcat(str,buf);
                    strcat(str,"'");
                    printf("input is %s\n",str);
                    
                     Sign_Hash_box sign_box={0};
                   // memcpy(user_data_pack.user_data,str,sizeof(str));
                   RSA_encrypt(str,user_data_pack.user_data);
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
    

                    
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(0==ret)
                    {
                        printf("验签失败\n");
                        return 0;
                    }
                    else
                         printf("验签成功\n");

 
                  //  send(sfd,str,strlen(str),MSG_NOSIGNAL);
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(ret==1){//登陆成功
                    system("clear");
                        printf("登陆成功!\n");
                        *login=1;
                        return 0;
                    }
                }


            }
        }

    }

    close(sfd);
       return 0;
}

