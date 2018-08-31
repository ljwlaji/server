#ifndef __LOG_H__
#define __LOG_H__

#include <LockedSTL.h>
#include <ShareDefine.h>

using namespace std;
#define sLog Log::GetInstance()

#define ___F(s, ...) Log::format(s, ##__VA_ARGS__)
enum LogColor
{
	Cmd_Normal,
	Cmd_White,
	Cmd_Red,
	Cmd_Green,
	Cmd_Blue,
	Cmd_Yellow,
	Cmd_Pink,
};

struct SignleLog
{
	SignleLog() {}
	SignleLog(LogColor color, std::string str) : Color(color) , str(str)
	{}
	LogColor Color = Cmd_Pink;
	std::string str;
};

class Log
{
public:
	static std::string format(const char* args, ...);
	static Log* GetInstance();
	void OutBug(std::string args);
	void OutLog(std::string args);
	void OutWarning(std::string args);
	void OutSuccess(std::string args);
	void OutExecption(std::string args);
	void SetColor(LogColor _color);
	void ReSetColor();
	void OnUpdate(uint32 diff);
private:
	Log();
	~Log();
	LockedList<SignleLog> m_addlist;
#define ThreadLocker		std::lock_guard<std::mutex>
};

#endif