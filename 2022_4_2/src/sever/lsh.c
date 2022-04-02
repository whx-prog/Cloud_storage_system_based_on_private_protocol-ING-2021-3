#include "factory.h"
int count=0;
char buf[1000]={0};
train_t t;
int lsh(int new_fd){  
    /* if(argc!=2){
       printf("error");
       return 0;  }*/
    DIR* dirp= opendir("./");
    if(dirp==NULL){
        perror("opendir");
    }
    struct dirent *pdirent;
    while((pdirent=readdir(dirp))){
        memset(buf,0,sizeof(buf));
        count=0;
        struct stat statbuf;
        int ret =stat(pdirent->d_name,&statbuf);
        if(dirp==NULL){          
            perror("opendir");    
        }
        if(pdirent->d_name[0]=='.')
            continue;

        strncpy(buf,pdirent->d_name,strlen(pdirent->d_name));
        count+=strlen(pdirent->d_name);

        t.data_len=strlen("1");
        strcpy(t.buf,"1");
        ret=send(new_fd,&t,4+t.data_len,MSG_NOSIGNAL);
        ERROR_CHECK(ret,-1,"send");

        t.data_len=count;
        memcpy(t.buf,buf,t.data_len);
        ret=send(new_fd,&t,4+t.data_len,MSG_NOSIGNAL);
        ERROR_CHECK(ret,-1,"send");
    }                     
    /*    while((pdirent=readdir(dirp))!=NULL){
          if(strcmp(pdirent->d_name,argv[2])==0){
          break;
          }
          }

          if(pdirent==NULL){
          printf("it is not here!");
          }
          else if (pdirent!=NULL){
          printf("it is right here");
          }*/
    closedir(dirp);
    //printf("Hello world\n");
    return 0;
}

