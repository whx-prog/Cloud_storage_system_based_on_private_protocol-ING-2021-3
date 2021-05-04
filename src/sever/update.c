#include "factory.h"
int update_mysql(int argc,char* argv[])
{
	if(argc!=2)
	{
		printf("error args\n");
		return -1;
	}
	MYSQL *conn;
	//MYSQL_RES *res;
	//MYSQL_ROW row;
	const char* server="localhost";
	const char* user="root";
	const char* password="123";
	const char* database="bandu_wangpan";
	char query[200]="update File_system set LastName='"; 
	sprintf(query,"%s%s%s",query,argv[1],"' where personID=13");
	puts(query);
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		printf("update success\n");
	}
	mysql_close(conn);
	return 0;
}


int update_md5_count(char *md5,int new_count)
{

	MYSQL *conn;
	//MYSQL_RES *res;
	//MYSQL_ROW row;
	const char* server="localhost";
	const char* user="root";
	const char* password="123";
	const char* database="bandu_wangpan";
	char query[200]="update md5_count set count="; 
	sprintf(query,"%s%d%s%s%s",query,new_count," where md5sum='",md5,"'");
	puts(query);
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		printf("update success\n");
	}
	mysql_close(conn);
	return 0;
}




