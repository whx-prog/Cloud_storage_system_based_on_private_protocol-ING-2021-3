#include"../factory.h"
int query_ls(int ppid,int new_fd)
{  train_t k;
    int ret;
    int count=0;
    char buf[128]={0};
    char str[128]={0};
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    const char* server="localhost";
    const char* user="root";
    const char* password="123";
    const char* database="bandu_wangpan";//要访问的数据库名称
    char query[300]="select file_name from File_system where ppid=";
    sprintf(query,"%s%d",query,ppid);
    //int age=atoi(argv[1]);
    //sprintf(query,"%s%d",query, age);
    //strcpy(query,"select * from Person");
    puts(query);
    unsigned int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
        return -1;
    }else{
        printf("Connected...\n");
    }
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        //	printf("Query made...\n");
        res=mysql_use_result(conn);
        if(res)
        {   //查找成功
            while((row=mysql_fetch_row(res))!=NULL)
            {	memset(buf,0,sizeof(buf));
                memset(str,0,sizeof(str));
                memset(k.buf,0,sizeof(k.buf));
                //printf("num=%d\n",mysql_num_fields(res));//列数
                for(t=0;t<mysql_num_fields(res);t++)//mysql_num_fields获取一行有几列
                {
                    printf("%8s ",row[t]);//右对齐
                    //count=strlen(row[t]);
                   // printf("row length is %d\n",count);

                    strncpy(buf,row[t],strlen(row[t]));
                    count=strlen(row[t]);

                    k.data_len=strlen("1");
                    strcpy(k.buf,"1");
                    ret=send(new_fd,&k,4+k.data_len,MSG_NOSIGNAL);
                    ERROR_CHECK(ret,-1,"send");

                    k.data_len=count;
                    memcpy(k.buf,buf,k.data_len);
                    ret=send(new_fd,&k,4+k.data_len,MSG_NOSIGNAL);
                    ERROR_CHECK(ret,-1,"send")
                }
                printf("\n");
            }
            mysql_free_result(res);
            mysql_close(conn);
            return 1;
        }else{
            printf("Don't find data\n");
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return 0;
}
