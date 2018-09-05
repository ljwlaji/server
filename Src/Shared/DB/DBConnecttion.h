#pragma once
#include <ShareDefine.h>
#include <string>
#ifdef WIN32
#include <Windows.h>
#endif
#include <mysql.h>
class DBConnecttion
{
public:
	DBConnecttion();
	~DBConnecttion();

private:
	char* m_DBHost;
	uint16 m_DBPort;
	std::string m_DBName;
	std::string m_UserName;
	std::string m_Password;
};