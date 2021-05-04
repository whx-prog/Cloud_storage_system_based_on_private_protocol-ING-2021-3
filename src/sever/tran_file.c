#include "factory.h"

int tran_file(int new_fd,char *file_name,char *type,int pipe_fd,int pipe_size,pnode_t pcur){
    int ret;
    train_t t;
//发送类型
    t.data_len=strlen(type);
    strcpy(t.buf,type);
    ret=send(new_fd,&t,4+t.data_len,MSG_NOSIGNAL);
    ERROR_CHECK(ret,-1,"send");
  if(atoi(type)==0){
    memset(t.buf,0,sizeof(t.buf));
    t.data_len=strlen(file_name);
    strcpy(t.buf,file_name);
    ret=send(new_fd,&t,4+t.data_len,MSG_NOSIGNAL);
    ERROR_CHECK(ret,-1,"send");
//发文件名
    char *toy=NULL;
    char **md5=&toy;
    query_md5(pcur->pathID_box[pcur->Cur_arry_flag],file_name,pcur->belong,md5);
    int fd=open(*md5,O_RDONLY);//open md5值
    ERROR_CHECK(fd,-1,"open");

    struct stat buf;
    ret=fstat(fd,&buf);
    ERROR_CHECK(ret,-1,"fstat");
    t.data_len=sizeof(buf.st_size);
    memcpy(t.buf,&buf.st_size,t.data_len);
    ret=send(new_fd,&t,4+t.data_len,MSG_NOSIGNAL);
    ERROR_CHECK(ret,-1,"send");
//发文件大小
   ret=sendfile(new_fd,fd,NULL,buf.st_size);
   ERROR_CHECK(ret,-1,"sendfile");
  }
   return 0;
}
