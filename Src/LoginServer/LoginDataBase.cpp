#include <LoginDataBase.h>
#include <Config.h>

LoginDataBase::LoginDataBase() :
	DBConnecttion(
		sConfig->GetStringDefault("LoginDataBase.Host", "127.0.0.1"),
		sConfig->GetIntDefault("LoginDataBase.Port", 3306),
		sConfig->GetStringDefault("LoginDataBase.DBName", "Login"),
		sConfig->GetStringDefault("LoginDataBase.UserName", "root"),
		sConfig->GetStringDefault("LoginDataBase.Password", "root")
		)
{

}

LoginDataBase::~LoginDataBase()
{
}

bool LoginDataBase::Connect()
{
	return DBConnecttion::Connect();
}
