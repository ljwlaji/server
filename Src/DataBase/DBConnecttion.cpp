#include <DBConnecttion.h>

DBConnecttion::DBConnecttion(std::string DBHost, uint16 Port, std::string DBName, std::string UserName, std::string Passwd) : 
	m_DBHost(DBHost),
	m_DBName(DBName), 
	m_DBPort(Port),
	m_UserName(UserName),
	m_Password(Passwd),
	m_RealConnecttion(nullptr)
{
}

DBConnecttion::~DBConnecttion()
{
	Close();
}

void DBConnecttion::Close()
{
	mysql_close(m_RealConnecttion);
	m_RealConnecttion = nullptr;
}

#define STRING_SIZE 50

#define DROP_SAMPLE_TABLE "DROP TABLE IF EXISTS test_table"
#define CREATE_SAMPLE_TABLE "CREATE TABLE test_table(col1 INT,\
col2 VARCHAR(40),\
col3 SMALLINT,\
col4 TIMESTAMP)"
#define INSERT_SAMPLE "INSERT INTO test_table(col1,col2,col3) VALUES(?,?,?)"

DBConnecttion * DBConnecttion::CreateConnecttion(std::string DBHost, uint16 Port, std::string DBName, std::string UserName, std::string Passwd)
{
	DBConnecttion* _DBConnecttion = new DBConnecttion(DBHost, Port, DBName, UserName, Passwd);
	if (!_DBConnecttion->Connect())
		return nullptr;

	return _DBConnecttion;
}

bool DBConnecttion::Connect()
{
	m_RealConnecttion = mysql_init(NULL);
	if (!m_RealConnecttion)
		return false;

	mysql_real_connect(m_RealConnecttion, "192.168.0.6", "root", "19900530Aa", "test", 0, NULL, 0);
	if (!m_RealConnecttion)
	{
		mysql_close(m_RealConnecttion);
		return false;
	}
	return true;
}

void DBConnecttion::Test()
{
	
	//MYSQL   *mysql;
	//MYSQL   *connect = NULL;
	//char    sqlbuf[2048];
	//
	//mysql = mysql_init(NULL);
	//if (mysql == NULL)
	//{
	//	return;
	//}
	//printf("func mysql_init() ok \n");
	//
	//connect = mysql_real_connect(mysql, "192.168.0.6", "root", "19900530Aa", "test", 0, NULL, 0);
	//if (connect == NULL)
	//{
	//	mysql_errno(mysql);
	//	printf("func mysql_init() err \n");
	//}
	//printf("func mysql_real_connect() ok \n");
	//
	//MYSQL_STMT    *stmt;
	//MYSQL_BIND    bind[3];
	//my_ulonglong  affected_rows;
	//int           param_count;
	//short         small_data;
	//int           int_data;
	//char          str_data[STRING_SIZE];
	//unsigned long str_length;
	//my_bool       is_null;
	//
	//if (mysql_query(&mysql, DROP_SAMPLE_TABLE))
	//{
	//	fprintf(stderr, " DROP TABLE failed\n");
	//	fprintf(stderr, " %s\n", mysql_error(mysql));
	//	exit(0);
	//}
	//
	//if (mysql_query(mysql, CREATE_SAMPLE_TABLE))
	//{
	//	fprintf(stderr, " CREATE TABLE failed\n");
	//	fprintf(stderr, " %s\n", mysql_error(mysql));
	//	exit(0);
	//}
	//
	///* Prepare an INSERT query with 3 parameters */
	///* (the TIMESTAMP column is not named; the server */
	///*  sets it to the current date and time) */
	//
	//stmt = mysql_stmt_init(mysql);  //初始化预处理环境句柄  MYSQL_STMT *stmt
	//if (!stmt)
	//{
	//	fprintf(stderr, " mysql_stmt_init(), out of memory\n");
	//	exit(0);
	//}
	//if (mysql_stmt_prepare(stmt, INSERT_SAMPLE, strlen(INSERT_SAMPLE)))  //向预处理环境句柄stmt 中添加sql( 带有占位符)
	//{
	//	fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
	//	fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	//	exit(0);
	//}
	//fprintf(stdout, " prepare, INSERT successful\n");
	//
	///* Get the parameter count from the statement */
	//
	//param_count = mysql_stmt_param_count(stmt);   //获取sql语言中 占位符 的个数
	//fprintf(stdout, " total parameters in INSERT: %d\n", param_count);
	//
	//if (param_count != 3) /* validate parameter count */
	//{
	//	fprintf(stderr, " invalid parameter count returned by MySQL\n");
	//	exit(0);
	//}
	//
	///* Bind the data for all 3 parameters */
	///*
	//memset(bind, 0, sizeof(bind));
	//
	///* INTEGER PARAM */
	///* This is a number type, so there is no need to specify buffer_length */
	//bind[0].buffer_type = MYSQL_TYPE_LONG;  //设置第一个占位符的属性
	//bind[0].buffer = (char *)&int_data;
	//bind[0].is_null = 0;
	//bind[0].length = 0;
	//
	///* STRING PARAM */
	//bind[1].buffer_type = MYSQL_TYPE_STRING; //设置第2个占位符的属性
	//bind[1].buffer = (char *)str_data;
	//bind[1].buffer_length = STRING_SIZE;
	//bind[1].is_null = 0;
	//bind[1].length = &str_length;
	//
	///* SMALLINT PARAM */
	//bind[2].buffer_type = MYSQL_TYPE_SHORT; //设置第3个占位符的属性
	//bind[2].buffer = (char *)&small_data;
	//bind[2].is_null = &is_null;
	//bind[2].length = 0;
	//
	///* Bind the buffers */
	//if (mysql_stmt_bind_param(stmt, bind))  //把设置好的属性 加入预处理环境stmt中
	//{
	//	fprintf(stderr, " mysql_stmt_bind_param() failed\n");
	//	fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	//	exit(0);
	//}
	//
	///* Specify the data values for the first row */    //准备插入数据
	//int_data = 10;             /* integer */
	//strncpy(str_data, "MySQL", STRING_SIZE); /* string  */
	//str_length = strlen(str_data);
	//
	///* INSERT SMALLINT data as NULL */
	//is_null = 1;
	//
	///* Execute the INSERT statement - 1*/
	//if (mysql_stmt_execute(stmt))   //执行预处理环境  插入第一条记录
	//{
	//	fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
	//	fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	//	exit(0);
	//}
	//
	///* Get the total number of affected rows */
	//affected_rows = mysql_stmt_affected_rows(stmt);  //获取受影响的行信息
	//fprintf(stdout, " total affected rows(insert 1): %lu\n",
	//	(unsigned long)affected_rows);
	//
	//if (affected_rows != 1) /* validate affected rows */
	//{
	//	fprintf(stderr, " invalid affected rows by MySQL\n");
	//	exit(0);
	//}
	//
	///* Specify data values for second row, then re-execute the statement */
	//int_data = 1000;
	//strncpy(str_data, "The most popular Open Source database", STRING_SIZE);
	//str_length = strlen(str_data);
	//small_data = 1000;         /* smallint */
	//is_null = 0;               /* reset */
	//
	//						   /* Execute the INSERT statement - 2*/
	//if (mysql_stmt_execute(stmt))    //插入第二条记录
	//{
	//	fprintf(stderr, " mysql_stmt_execute, 2 failed\n");
	//	fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	//	exit(0);
	//}
	//
	///* Get the total rows affected */
	//affected_rows = mysql_stmt_affected_rows(stmt); //有获取受影响的行
	//fprintf(stdout, " total affected rows(insert 2): %lu\n",
	//	(unsigned long)affected_rows);
	//
	//if (affected_rows != 1) /* validate affected rows */
	//{
	//	fprintf(stderr, " invalid affected rows by MySQL\n");
	//	exit(0);
	//}
	//
	///* Close the statement */
	//if (mysql_stmt_close(stmt))
	//{
	//	fprintf(stderr, " failed while closing the statement\n");
	//	fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	//	exit(0);
	//}
	//
	//mysql_close(connect);
	//
	//printf("hello...\n");
	//*/
}

void DBConnecttion::OnUpdate(uint32 diff)
{
}