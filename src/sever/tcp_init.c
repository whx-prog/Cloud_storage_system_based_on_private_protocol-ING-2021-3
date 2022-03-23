#include "head.h"

int tcp_init(char *ip,char *port,int *psfd)
{ //  ARGS_CHECK(argc,2);
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sfd,-1,"socket");
    struct sockaddr_in ser_addr;
    memset(&ser_addr,0,sizeof(ser_addr));
    ser_addr.sin_family=AF_INET;
    ser_addr.sin_port=htons(atoi(port));
    ser_addr.sin_addr.s_addr=inet_addr(ip);
    int reuse=1;
    int ret;
    ret=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    ERROR_CHECK(ret,-1,"setsockopt");
    ret=bind(sfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
    listen(sfd,10);
    *psfd=sfd;
    return 0;
}

