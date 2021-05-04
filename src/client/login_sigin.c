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
                printf("hashVal is %d\n",ret);
                

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
                    printf("注册成功\n");
                }
                 if(ret==8){
                     send(sfd,&ret,4,MSG_NOSIGNAL);
                    recv(sfd,str,4,MSG_NOSIGNAL);
                    memset(str,0,sizeof(str));
                    printf("请输入用户名\n");
                    scanf("%s",str);
                    strcat(str,"' and keyword='");
                    printf("请输入密码:\n");
                    scanf("%s",buf);
                    strcat(str,buf);
                    strcat(str,"'");
                    printf("input is %s\n",str);
                    send(sfd,str,strlen(str),MSG_NOSIGNAL);
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(ret==1){//登陆成功
                        printf("登陆成功\n");
                        *login=1;
                        return 0;
                    }
                 }

    
    while(1)
    {   j=0;
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
                printf("hashVal is %d\n",ret);

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
                    printf("注册成功!\n");
                }
                if(ret==8){
                     send(sfd,&ret,4,MSG_NOSIGNAL);
                    recv(sfd,str,4,MSG_NOSIGNAL);
                    memset(str,0,sizeof(str));
                    printf("请输入用户名\n");
                    scanf("%s",str);
                    strcat(str,"' and keyword='");
                    printf("请输入密码:\n");
                    scanf("%s",buf);
                    strcat(str,buf);
                    strcat(str,"'");
                    printf("input is %s\n",str);
                    send(sfd,str,strlen(str),MSG_NOSIGNAL);
                    recv(sfd,&ret,4,MSG_NOSIGNAL);
                    if(ret==1){//登陆成功
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

