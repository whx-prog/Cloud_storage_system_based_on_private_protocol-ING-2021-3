#include<stdio.h>
#include<dirent.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<wait.h>
#include <time.h>
#include <syslog.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include<sys/time.h>
#include<pthread.h>
#include<malloc.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <errno.h>
#include<sys/sendfile.h>
//#include <tcp_net_socket.h>
#define ARGS_CHECK(argc,num) { if(argc!=num){\
        fprintf(stderr,"error\n");\
        return -1;\
}}
#define ERROR_CHECK(ret,num,msg) {if(ret==num){\
        perror(msg);\
        printf("行号：%d\n",__LINE__);\
        return -1;\
} }
#define THREAD_ERROR_CHECK(ret,funcname) {if(ret!=0) {printf("%s failed %s\n",funcname,strerror(ret));return -1;}}

typedef struct{
    pid_t pid;
    int fd;
    short busy_flag;
}process_data_t;

typedef struct{
    int data_len;
    char buf[1000];
}train_t;

#define FILENAME "file1"
int make_child(process_data_t*,int);
void child_work(int);
int tcp_init(char*,char*,int*);
int epoll_init(int,int);
int send_fd(int,int);
int recv_fd(int,int*);
int tran_file(int);



