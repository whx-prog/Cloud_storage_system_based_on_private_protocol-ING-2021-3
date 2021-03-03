#include"../factory.h"
int query_mysql(char * name_code)
{
	/*if(argc!=2)
	{
		printf("error args\n");
		return -1;
	}*/
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char* server="localhost";
	const char* user="root";
	const char* password="123";
	const char* database="bandu_wangpan";//要访问的数据库名称
	char query[300]="select * from client where name='";
	sprintf(query,"%s%s",query,name_code);
    //int age=atoi(argv[1]);
	//sprintf(query,"%s%d",query, age);
	//strcpy(query,"select * from Person");
	puts(query);
	unsigned int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{  //printf("A\n");
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -1;
	}else{
       // printf("B\n");
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
        //printf("C\n");
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
       // printf("D\n");
	//	printf("Query made...\n");
		res=mysql_use_result(conn);
		if(res)
		{   //查找成功
            if(mysql_fetch_row(res)!=0)
            {
            mysql_free_result(res);
            mysql_close(conn);
            return 1;
		    }
 
           // printf("E\n");
			while((row=mysql_fetch_row(res))!=NULL)
			{	
               // printf("123");
				//printf("num=%d\n",mysql_num_fields(res));//列数
				for(t=0;t<mysql_num_fields(res);t++)//mysql_num_fields获取一行有几列
				{
						printf("%8s ",row[t]);
				}
                //printf("is here");
				printf("\n");
			}
           // printf("%d\n",mysql_num_fields(res));
              }else{
                  //printf("G\n");
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
   // printf("H\n");
	mysql_close(conn);
	return 0;
}


int query_root_fileID(char *belong)
{

	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char* server="localhost";
	const char* user="root";
	const char* password="123";
	const char* database="bandu_wangpan";//要访问的数据库名称
	char query[300]="select file_id from File_system where ppid=0 and belong='";
	sprintf(query,"%s%s%s",query,belong,"'");
//    int age=atoi(argv[1]);
//	sprintf(query,"%s%d",query, age);
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
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{	
				//printf("num=%d\n",mysql_num_fields(res));//列数
				for(t=0;t<mysql_num_fields(res);t++)//mysql_num_fields获取一行有几列
				{
						printf("%8s ",row[t]);
                        return atoi(row[t]);
				}
				printf("\n");
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return 0;
}



int query_fileID(int ppid,char *file_name)
{

	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char* server="localhost";
	const char* user="root";
	const char* password="123";
	const char* database="bandu_wangpan";//要访问的数据库名称
	char query[300]="select file_id from File_system where ppid=";
	sprintf(query,"%s%d%s%s%s",query,ppid," and file_name='",file_name,"'");
//    int age=atoi(argv[1]);
//	sprintf(query,"%s%d",query, age);
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
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{	
				//printf("num=%d\n",mysql_num_fields(res));//列数
				for(t=0;t<mysql_num_fields(res);t++)//mysql_num_fields获取一行有几列
				{
						printf("%8s ",row[t]);
                        return atoi(row[t]);
				}
				printf("\n");
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return 0;
}




int query_md5(int ppid,char *file_name,char *belong,char **md5)
{

	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char* server="localhost";
	const char* user="root";
	const char* password="123";
	const char* database="bandu_wangpan";//要访问的数据库名称
	char query[300]="select md5sum from File_system where ppid=";
	sprintf(query,"%s%d%s%s%s%s%s",query,ppid," and file_name='",file_name,"' and belong='",belong,"'");
//    int age=atoi(argv[1]);
//	sprintf(query,"%s%d",query, age);
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
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{	
				//printf("num=%d\n",mysql_num_fields(res));//列数
				for(t=0;t<mysql_num_fields(res);t++)//mysql_num_fields获取一行有几列
				{
						printf("%8s ",row[t]);
                        //strcpy(*md5,row[t]);
                        memcpy(md5,&row[t],8);
                        return 0;
				}
				printf("\n");
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return 0;
}




int query_md5_isexist(char *md5)
{

	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char* server="localhost";
	const char* user="root";
	const char* password="123";
	const char* database="bandu_wangpan";//要访问的数据库名称
	char query[300]="select ppid from File_system where md5sum='";
	sprintf(query,"%s%s%s",query,md5,"'");
//    int age=atoi(argv[1]);
//	sprintf(query,"%s%d",query, age);
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
		{   
            if(mysql_fetch_row(res)!=0)
            {
            mysql_free_result(res);
            mysql_close(conn);
            return 1;
		    }
			while((row=mysql_fetch_row(res))!=NULL)
			{	
				//printf("num=%d\n",mysql_num_fields(res));//列数
				for(t=0;t<mysql_num_fields(res);t++)//mysql_num_fields获取一行有几列
				{
						printf("%8s ",row[t]);
                        //strcpy(*md5,row[t]);
				}
				printf("\n");
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return 0;
}




int query_md5_count(char *md5)
{

	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char* server="localhost";
	const char* user="root";
	const char* password="123";
	const char* database="bandu_wangpan";//要访问的数据库名称
	char query[300]="select count from md5_count where md5sum='";
	sprintf(query,"%s%s%s",query,md5,"'");
//    int age=atoi(argv[1]);
//	sprintf(query,"%s%d",query, age);
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
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{	
				//printf("num=%d\n",mysql_num_fields(res));//列数
				for(t=0;t<mysql_num_fields(res);t++)//mysql_num_fields获取一行有几列
				{
						printf("%8s ",row[t]);
                        return atoi(row[t]);
				}
				printf("\n");
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return 0;
}



int query_md5_exist_count(char *md5)
{

	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char* server="localhost";
	const char* user="root";
	const char* password="123";
	const char* database="bandu_wangpan";//要访问的数据库名称
	char query[300]="select ppid from md5_count where md5sum='";
	sprintf(query,"%s%s%s",query,md5,"'");
//    int age=atoi(argv[1]);
//	sprintf(query,"%s%d",query, age);
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
		{   
            if(mysql_fetch_row(res)!=0)
            {
            mysql_free_result(res);
            mysql_close(conn);
            return 1;
		    }
			while((row=mysql_fetch_row(res))!=NULL)
			{	
				//printf("num=%d\n",mysql_num_fields(res));//列数
				for(t=0;t<mysql_num_fields(res);t++)//mysql_num_fields获取一行有几列
				{
						printf("%8s ",row[t]);
                        //strcpy(*md5,row[t]);
				}
				printf("\n");
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return 0;
}



int query_file_isexist(int ppid)
{

	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char* server="localhost";
	const char* user="root";
	const char* password="123";
	const char* database="bandu_wangpan";//要访问的数据库名称
	char query[300]="select file_name from File_system where ppid=";
	sprintf(query,"%s%d",query,ppid);
//    int age=atoi(argv[1]);
//	sprintf(query,"%s%d",query, age);
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
		{   
            if(mysql_fetch_row(res)!=0)
            {
            mysql_free_result(res);
            mysql_close(conn);
            return 1;
		    }
			while((row=mysql_fetch_row(res))!=NULL)
			{	
				//printf("num=%d\n",mysql_num_fields(res));//列数
				for(t=0;t<mysql_num_fields(res);t++)//mysql_num_fields获取一行有几列
				{
						printf("%8s ",row[t]);
                        //strcpy(*md5,row[t]);
				}
				printf("\n");
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return 0;
}


int query_dirID(int ppid,char *dirl_name)
{

	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char* server="localhost";
	const char* user="root";
	const char* password="123";
	const char* database="bandu_wangpan";//要访问的数据库名称
	char query[300]="select file_id from File_system where ppid=";
	sprintf(query,"%s%d%s%s%s",query,ppid," and file_name='",dirl_name,"'");
//    int age=atoi(argv[1]);
//	sprintf(query,"%s%d",query, age);
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
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{	
				//printf("num=%d\n",mysql_num_fields(res));//列数
				for(t=0;t<mysql_num_fields(res);t++)//mysql_num_fields获取一行有几列
				{
						printf("%8s ",row[t]);
                        return atoi(row[t]);
				}
				printf("\n");
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return 0;
}


