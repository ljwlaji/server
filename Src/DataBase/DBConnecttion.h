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
	static void Test();
	DBConnecttion();
	~DBConnecttion();
	virtual bool Connect(std::string DBHost, uint16 Port, std::string DBName, std::string UserName, std::string Passwd);
	void OnUpdate(uint32 diff);
	void Close();
private:
	uint16 m_DBPort;
	std::string m_DBHost;
	std::string m_DBName;
	std::string m_UserName;
	std::string m_Password;
	MYSQL* m_RealConnecttion;
};