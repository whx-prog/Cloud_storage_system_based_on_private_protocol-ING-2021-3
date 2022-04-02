#ifndef _FACTORY_
#define _FACTORY_
#include "head.h"
#include "work_que.h"
#include"md5.h"
#include "cJSON.h"
#define MAXKEY 1000;
//#include "process_pool.h"


#define PUBLIC_KEY_PATH  ("./rsa_public_key.pem")
#define PRIVATE_KEY_PATH ("./rsa_private_key.pem")
#define SERVER_PUBLICK_KEY_PATH ("./Server_publicKey.pem")

#define isUseSha256    (1)

#if isUseSha256
#define SHA_WHICH        NID_sha256
#define WHICH_DIGEST_LENGTH    SHA256_DIGEST_LENGTH
#else
#define SHA_WHICH        NID_sha512
#define WHICH_DIGEST_LENGTH    SHA512_DIGEST_LENGTH
#endif



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
typedef struct hashTable{
    int token;
    struct hashTable *next;
    char *word;
}hashNode_t,*phashNode_t;



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





int recvn(int sfd,void* pstart,int len);
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
int gets_file(int sfd);
char *salt_creater(char *slat);
int curtiny(char *str);
int input_curtiny(char *buf);
int epoll_init(int epfd,int fd);
int factory_init(factory_t*,int,int);
void *thread_func(void*);
int factory_start(factory_t*);
int tran_file(int new_fd,char *file_name,char *type,int goal_fd,int pipe_size,char *md5,int ppid,char belong[]);
int tcp_init(char*,char*,int*);
void Findhash(phashNode_t *hashNode,phashNode_t node);
void hashInsert(int hashNode[],phashNode_t node,int num);
int hash(char *key);
int login_sigin(int *login,int sfd,int epfd);
int conf_init(char * path,char *ip,char * port,int *size,int *capacity);
int do_jsonTest();
//bool Client_Sigin_hashGenerator(char * Sign_buf,char *data);

bool Client_Sigin_hashGenerator(char *a,char *b);
void RSA_encrypt(char *data,char *goal_structBuff);
void RSA_decrypt(char *encrypt_str, char * decrypt_str);


enum Token{
 CD=1,
 LS,
 PUTS,
 GETS,
 REMOVE,
 PWD,
SIGIN,
LOGIN,
MKDIR,
RMDIR,
HELP,
EXIT,
LOCAL
};
#endif
