#include "factory.h"

char path_save[50]="";

void extrat_conf_database(char *sever_name,char *user_name,char *user_password,char *dataBase_name)
{
     FILE *conf=fopen(path_save,"r");
    //sever name
     
    char Data[128]="";
    fgets(Data,128,conf);
    fgets(Data,128,conf);
    fgets(Data,128,conf);
    fgets(Data,128,conf);
     memset(Data,0,128);

    fgets(Data,128,conf);
    Data[strlen(Data)-1]='\0';
    char *ret=strchr(Data,'=');
    strcpy(sever_name,ret+1);
    // user name
     memset(Data,0,128);
    fgets(Data,128,conf);
    Data[strlen(Data)-1]='\0';
    ret=strchr(Data,'=');
    strcpy(user_name,ret+1);
   //user pass word
  memset(Data,0,128);
    fgets(Data,128,conf);
    Data[strlen(Data)-1]='\0';
    ret=strchr(Data,'=');
    strcpy(user_password,ret+1);
   //database name
 memset(Data,0,128);
    fgets(Data,128,conf);
    Data[strlen(Data)-1]='\0';
    ret=strchr(Data,'=');
    strcpy(dataBase_name,ret+1);
   fclose(conf);

}


int conf_init(char *path,char *ip,char *port,int *size,int *capac)
{ //  ARGS_CHECK(argc,2);
    FILE *conf=fopen(path,"r");
    memcpy(path_save,path,strlen(path));
    char Data[128]="";
    fgets(Data,128,conf);
    Data[strlen(Data)-1]='\0';
    char *ret=strchr(Data,'=');
    strcpy(ip,ret+1);

    memset(Data,0,128);
    fgets(Data,128,conf);
    Data[strlen(Data)-1]='\0';
    ret=strchr(Data,'=');
    strcpy(port,ret+1);
   
    memset(Data,0,128);
    fgets(Data,128,conf);
    Data[strlen(Data)-1]='\0';
    ret=strchr(Data,'=');
    *size=atoi(ret+1);

     memset(Data,0,128);
    fgets(Data,128,conf);
    Data[strlen(Data)-1]='\0';
    ret=strchr(Data,'=');
     *capac=atoi(ret+1); 
    fclose(conf);
     return 0;
}
