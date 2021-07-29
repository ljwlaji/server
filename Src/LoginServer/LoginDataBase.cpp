#include <LoginDataBase.h>
#include <Config.h>

LoginDataBase::LoginDataBase() : DBConnecttion()
{

}

LoginDataBase::~LoginDataBase()
{
}

void LoginDataBase::OnUpdate(const uint32 diff)
{
	
}


bool LoginDataBase::Connect()
{
	uint16 Port				= sConfig->GetIntDefault("LoginDataBase.Port", 3306);
	std::string DBHost		= sConfig->GetStringDefault("LoginDataBase.Host", "127.0.0.1");
	std::string DBName		= sConfig->GetStringDefault("LoginDataBase.DBName", "Login");
	std::string UserName	= sConfig->GetStringDefault("LoginDataBase.UserName", "root");
	std::string PassWord	= sConfig->GetStringDefault("LoginDataBase.Password", "root");
	sLog->OutLog(___F("DBConfiguretion Loaded!"));
	sLog->OutLog(___F("Host:%s\nDBName:%s\nPort:%d\nUserName:%s\nPassword:%s", DBHost.c_str(),DBName.c_str(), Port, UserName.c_str(), PassWord.c_str()));
	return DBConnecttion::Connect(DBHost, Port, DBName, UserName, PassWord);
}
