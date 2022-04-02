#include "factory.h"
int  sigin_insert_client(char *name_code_salt)
{
	MYSQL *conn;
   // MYSQL_RES *res;
   // MYSQL_ROW row;
	 char server[20]=""; //DataBase_status.sever_name;
	 char user[20]="";//DataBase_status.user_name;
	 char password[20]="";//DataBase_status.user_password;
	 char database[20]="";//DataBase_status.dataBase_name;
     extrat_conf_database(server,user,password,database);
	char query[200]="insert into client(name,keyword,salt) values('";
	sprintf(query,"%s%s",query,name_code_salt);
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
		printf("insert success\n");
	}
	mysql_close(conn);
	return 0;
}


int  sigin_insert_file(char *belong)
{
	MYSQL *conn;
   // MYSQL_RES *res;
   // MYSQL_ROW row;
	 char server[20]=""; //DataBase_status.sever_name;
	 char user[20]="";//DataBase_status.user_name;
	 char password[20]="";//DataBase_status.user_password;
	 char database[20]="";//DataBase_status.dataBase_name;
     extrat_conf_database(server,user,password,database);
	char query[200]="insert into File_system(ppid,file_name,md5sum,belong,type) values(0,'";
	sprintf(query,"%s%s%s%s%s%s",query,belong,":~/','0','",belong,"','d","')");
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
		printf("insert success\n");
	}
	mysql_close(conn);
	return 0;
}



int  insert_dir(int ppid,char *dir_name,char *belong)
{
	MYSQL *conn;
   // MYSQL_RES *res;
   // MYSQL_ROW row;
	 char server[20]=""; //DataBase_status.sever_name;
	 char user[20]="";//DataBase_status.user_name;
	 char password[20]="";//DataBase_status.user_password;
	 char database[20]="";//DataBase_status.dataBase_name;
     extrat_conf_database(server,user,password,database);
	char query[200]="insert into File_system(ppid,file_name,md5sum,belong,type) values(";
	sprintf(query,"%s%d%s%s%s%s%s",query,ppid,",'",dir_name,"','0','",belong,"','d')");
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
		printf("insert success\n");
	}
	mysql_close(conn);
	return 0;
}




int  insert_file(int ppid,char *file_name,char *belong,char *md5)
{
	MYSQL *conn;
   // MYSQL_RES *res;
   // MYSQL_ROW row;
	 char server[20]=""; //DataBase_status.sever_name;
	 char user[20]="";//DataBase_status.user_name;
	 char password[20]="";//DataBase_status.user_password;
	 char database[20]="";//DataBase_status.dataBase_name;
     extrat_conf_database(server,user,password,database);
	char query[200]="insert into File_system(ppid,file_name,md5sum,belong,type) values(";
	sprintf(query,"%s%d%s%s%s%s%s%s%s",query,ppid,",'",file_name,"','",md5,"','",belong,"','f')");
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
		printf("insert success\n");
	}
	mysql_close(conn);
	return 0;
}



int  first_insert_md5(char *md5)
{
	MYSQL *conn;
   // MYSQL_RES *res;
   // MYSQL_ROW row;
	 char server[20]=""; //DataBase_status.sever_name;
	 char user[20]="";//DataBase_status.user_name;
	 char password[20]="";//DataBase_status.user_password;
	 char database[20]="";//DataBase_status.dataBase_name;
     extrat_conf_database(server,user,password,database);
	char query[200]="insert into md5_count (count,md5sum) values(1,'";
	sprintf(query,"%s%s%s",query,md5,"')");
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
		printf("insert success\n");
	}
	mysql_close(conn);
	return 0;
}


