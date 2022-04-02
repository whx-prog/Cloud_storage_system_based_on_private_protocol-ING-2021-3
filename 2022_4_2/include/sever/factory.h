#ifndef _FACTORY_
#define _FACTORY_
#include "head.h"
#include "work_que.h"
//#include "ftp.h"
#define MAXKEY 10

#define PUBLIC_KEY_PATH  ("./rsa_public_key.pem")
#define PRIVATE_KEY_PATH ("./rsa_private_key.pem")
#define CLIENT_PUBLIC_KEY_PATH ("../client/rsa_public_key.pem")

#define isUseSha256    (1)

#if isUseSha256
#define SHA_WHICH        NID_sha256
#define WHICH_DIGEST_LENGTH    SHA256_DIGEST_LENGTH
#else
#define SHA_WHICH        NID_sha512
#define WHICH_DIGEST_LENGTH    SHA512_DIGEST_LENGTH
#endif

;
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

#pragma pack(push)
#pragma pack(4)

typedef struct
{
    char Sign[128];
}Sign_Hash_box;

typedef struct message
{
    uint16_t command_type;
    //uint16_t message_head_length;
    //uint16_t Is_Follow_package;
    uint16_t Follow_package_length;
//    char Sign_Hash[128];
    
}Message ;

typedef struct Client_private_message
{
    char user_data[128];
}User_private_pack ;

typedef struct Short
{
   char contend[32];
   // uint16_t int_data;
}Shor_contend_pack;


 typedef struct short_int                                                               
  {
      uint16_t contend;
  }Short_int;

#pragma pack(pop)




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

void printHex(unsigned char *md, int len);
RSA* ReadPrivateKey(char* p_KeyPath);
RSA* ReadPublicKey(char* p_KeyPath);
int test_RSA_sign_verify(void);
void RSA_decrypt(char *encrypt_str, char * decrypt_str);
void RSA_encrypt(char *data,char *goal_structBuff);

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
