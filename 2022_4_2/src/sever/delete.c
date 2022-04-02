#include "factory.h"
int delete_mysql(int argc,char* argv[])
{
	MYSQL *conn;
    char server[20]=""; //DataBase_status.sever_name;
      char user[20]="";//DataBase_status.user_name;
      char password[20]="";//DataBase_status.user_passwor
      char database[20]="";//DataBase_status.dataBase_nam
      extrat_conf_database(server,user,password,database);
  



    char query[200]="delete from client where ID=";
	int t;
    int ID=atoi(argv[1]);
    sprintf(query,"%s%d",query,ID);
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
		printf("delete success,delete row=%ld\n",(long)mysql_affected_rows(conn));
	}
	mysql_close(conn);
	return 0;
}



int delete_md5_table(char *md5)
{
	MYSQL *conn;
    char server[20]=""; //DataBase_status.sever_name;
      char user[20]="";//DataBase_status.user_name;
      char password[20]="";//DataBase_status.user_passwor
      char database[20]="";//DataBase_status.dataBase_nam
      extrat_conf_database(server,user,password,database);
 char query[200]="delete from md5_count where md5sum='";
	int t;
    sprintf(query,"%s%s%s",query,md5,"'");
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
		printf("delete success,delete row=%ld\n",(long)mysql_affected_rows(conn));
	}
	mysql_close(conn);
	return 0;
}




int delete_file_table(int ppid,char *md5)
{
	MYSQL *conn;
    char server[20]=""; //DataBase_status.sever_name;
      char user[20]="";//DataBase_status.user_name;
      char password[20]="";//DataBase_status.user_passwor
      char database[20]="";//DataBase_status.dataBase_nam
      extrat_conf_database(server,user,password,database);
 char query[200]="delete from File_system where ppid=";
	int t;
    sprintf(query,"%s%d%s%s%s",query,ppid," and md5sum='",md5,"'");
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
		printf("delete success,delete row=%ld\n",(long)mysql_affected_rows(conn));
	}
	mysql_close(conn);
	return 0;
}


int delete_dir(int dirID)
{
	MYSQL *conn;
    char server[20]=""; //DataBase_status.sever_name;
      char user[20]="";//DataBase_status.user_name;
      char password[20]="";//DataBase_status.user_passwor
      char database[20]="";//DataBase_status.dataBase_nam
      extrat_conf_database(server,user,password,database);
 char query[200]="delete from File_system where file_id=";
	int t;
    sprintf(query,"%s%d",query,dirID);
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
		printf("delete success,delete row=%ld\n",(long)mysql_affected_rows(conn));
	}
	mysql_close(conn);
	return 0;
}
