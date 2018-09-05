#pragma once
#include <ShareDefine.h>
#include <string>
#ifdef WIN32
#include <Windows.h>
#endif
#include <mysql.h>

#include <Runnable.h>
class DBConnecttion
{
public:
	static DBConnecttion* CreateConnecttion(std::string DBHost, uint16 Port, std::string DBName, std::string UserName, std::string Passwd);
	static void Test();
	DBConnecttion(std::string DBHost, uint16 Port, std::string DBName, std::string UserName, std::string Passwd);
	~DBConnecttion();
	virtual bool Connect();
	void OnUpdate(uint32 diff);
private:
	void Close();
	uint16 m_DBPort;
	std::string m_DBHost;
	std::string m_DBName;
	std::string m_UserName;
	std::string m_Password;
	MYSQL* m_RealConnecttion;
};