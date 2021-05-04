#ifndef _FACTORY_
#define _FACTORY_
#include "head.h"
#include "work_que.h"
//#include "ftp.h"
#define MAXKEY 10;
//#include "process_pool.h"
typedef void* (*thread_func_t)(void *);
typedef struct{
    que_t que;
    pthread_cond_t cond;
    pthread_t *pthid;
    int pthread_num;
    thread_func_t download_thread;
    int start_flag;
}factory_t;

typedef struct{
    int data_len;
    char buf[1000];
}train_t;
#define FILENAME "file1"


int query_dirID(int ppid,char *dirl_name);
int query_file_isexist(int ppid);
int delete_dir(int dirID);
int delete_file_table(int ppid,char *md5);
int query_md5_exist_count(char *md5);
int delete_md5_table(char *md5);
int query_md5_count(char *md5);
int  first_insert_md5(char *md5);
int update_md5_count(char *md5,int new_count);
int query_md5_isexist(char *md5);
int query_md5(int ppid,char *file_name,char *belong,char **md5);
int  insert_file(int ppid,char *file_name,char *belong,char *md5);
int insert_dir(int ppid,char *dir_name,char *belong);
int query_fileID(int ppid,char *dirl_name);
int query_root_fileID(char *belong);
int query_ls(int ppid,int new_fd);
int query_mysql(char *name_code);
int sigin_insert_client(char *name_code_salt);
int sigin_insert_file(char *belong);
int factory_init(factory_t*,int,int);
void *thread_func(void*);
int factory_start(factory_t*);
int tran_file(int new_fd,char *file_name,char *type,int goal_fd,int pipe_size,pnode_t pcur);
int tcp_init(char*,char*,int*);
int epoll_init(int epfd,int fd);
int lsh(int new_fd);
int conf_init(char *path,char *ip,char *port,int *size,int *capac);

typedef struct hashTable{
    int token;
    struct hashTable *next;
    char *word;
}hashNode_t,*phashNode_t;

void Findhash(phashNode_t *hashNode,phashNode_t node);
void hashInsert(phashNode_t *hashNode,phashNode_t node);
int hash(char *key);

enum Token{
 CD=1,
 LS,
 PUTS,
 GETS,
 REMOVE,
 PWD,
 sigin,//注册
};
#endif
