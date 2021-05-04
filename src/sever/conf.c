#include "factory.h"

int conf_init(char *path,char *ip,char *port,int *size,int *capac)
{ //  ARGS_CHECK(argc,2);
    FILE *conf=fopen(path,"r");
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

    
    return 0;
}

