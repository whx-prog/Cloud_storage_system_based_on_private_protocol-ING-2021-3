#include "factory.h"

int tran_file(int new_fd,char *file_name,char *type,int pipe_fd,int pipe_size,char *md5,int ppid,char belong[]){
    int ret;
    char tool[4]={0};
    train_t t;
    char id[4]={0};
//发送类型
    t.data_len=strlen(type);
    strcpy(t.buf,type);
    ret=send(new_fd,&t,4+t.data_len,MSG_NOSIGNAL);
    ERROR_CHECK(ret,-1,"send");
  if(atoi(type)==0){
    memset(t.buf,0,sizeof(t.buf));
    t.data_len=strlen(md5);
    strcpy(t.buf,md5);
    ret=send(new_fd,&t,4+t.data_len,MSG_NOSIGNAL);
    ERROR_CHECK(ret,-1,"send");

    //发所属人
    memset(t.buf,0,sizeof(t.buf));
    t.data_len=strlen(belong);
    strcpy(t.buf,belong);
    ret=send(new_fd,&t,4+t.data_len,MSG_NOSIGNAL);
    ERROR_CHECK(ret,-1,"send");

    //发文件夹id
    memset(t.buf,0,sizeof(t.buf));
    sprintf(id,"%d",ppid);
    t.data_len=strlen(id);
    strcpy(t.buf,id);
    ret=send(new_fd,&t,4+t.data_len,MSG_NOSIGNAL);
    ERROR_CHECK(ret,-1,"send");
    
    //打开文件
    int fd=open(file_name,O_RDONLY);
    ERROR_CHECK(fd,-1,"open");

    //发文件长度
    struct stat buf;
    ret=fstat(fd,&buf);
    ERROR_CHECK(ret,-1,"fstat");
    t.data_len=sizeof(buf.st_size);
    memcpy(t.buf,&buf.st_size,t.data_len);
    ret=send(new_fd,&t,4+t.data_len,MSG_NOSIGNAL);
    ERROR_CHECK(ret,-1,"send");

int Cur_size=0;
    //确认是否发生了秒传
     recv(new_fd,&ret,4,MSG_NOSIGNAL);
    if(ret==1)
        return 0;
    else if(ret==2)
    {
     recv(new_fd,&Cur_size,4,MSG_NOSIGNAL);//断点续传接受服务器当前的文件大小
        int move=lseek(fd,Cur_size,SEEK_SET);//文件描述符指针移动到断点
        printf("lseek 移动长度为：%d\n",move);
        printf("发生断点续传，服务器已经有的长度为%d\n",Cur_size);
    }


    //发文件本体
   ret=sendfile(new_fd,fd,NULL,buf.st_size);
   ERROR_CHECK(ret,-1,"sendfile");
        printf("传输结束，传输总长：%d\n",ret);
  }
   return 0;



  }
