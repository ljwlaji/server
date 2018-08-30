#include "Log.h"
#ifdef WIN32
#include <Windows.h>
#else
#include <stdarg.h>
#endif
#include <iostream>

static Log* _log = nullptr;
static mutex LocInsLoc;

std::string Log::format(const char* args, ...)
{
	std::string ret = "";
	va_list ap;
	char szQuery[1024];
	va_start(ap, args);
	int ares = vsnprintf(szQuery, 1024, args, ap);
	va_end(ap);

	if (ares == -1)
		_ASSERT(false);

	ret = szQuery;

	return ret;
}

Log * Log::GetInstance()
{
	if (!_log)
	{
		ThreadLocker loc(LocInsLoc);
		if (!_log)
			_log = new Log();
	}
	return _log;
}

void Log::OutBug(const char* args, ...)
{
	m_addlist.PushBack(SignleLog(Cmd_Red, ___F(args)));
}

void Log::OutLog(const char* args, ...)
{
	m_addlist.PushBack(SignleLog(Cmd_White, ___F(args)));
}

void Log::OutSuccess(const char* args, ...)
{
	m_addlist.PushBack(SignleLog(Cmd_Green, ___F(args)));
}

void Log::OutExecption(const char* args, ...)
{
	m_addlist.PushBack(SignleLog(Cmd_Pink, ___F(args)));
}

void Log::OutWarning(const char* args, ...)
{
	m_addlist.PushBack(SignleLog(Cmd_Yellow, ___F(args)));
}

void Log::OnStart()
{
	SetColor(Cmd_Pink);
	OutLog("日志服务启动");
	ReSetColor();
}

void Log::ReSetColor()
{
	SetColor(Cmd_Normal);
}

void Log::OnUpdate(uint32 diff)
{
	if (!m_addlist.Size())
		return;
	while (m_addlist.Size())
	{
		SignleLog _log;
		m_addlist.PopNext(_log);

		SetColor(_log.Color);
		printf("%s\n", _log.str.c_str());
	}
}

void Log::SetColor(LogColor _color)
{
#ifdef WIN32
	switch (_color)
	{
	case Cmd_Normal:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
		break;
	case Cmd_White:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case Cmd_Red:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		break;
	case Cmd_Green:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		break;
	case Cmd_Blue:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case Cmd_Yellow:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case Cmd_Pink:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
		break;
	}
#endif
}

Log::Log()
{

}

Log::~Log()
{
}