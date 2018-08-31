#pragma once

#include <string>
#include <stdio.h>
#include <Log.h>

#define CREATE_EXECPTION(Desc) Execption(Desc, __FILE__, __LINE__, __DATE__, __FUNCTION__)
class Execption
{
public:
	Execption(const char* Desc, const char* FileName, int Line, const char* Date, const char* Function) : m_FileName(FileName),
		m_Line(Line),
		m_Date(Date),
		m_ExecptionDesc(Desc),
		m_Function(Function)
	{

	}
	~Execption() {}

	void Out()
	{
		sLog->OutExecption(___F("%s Catched Execption:\n in <%s> at Line %d:\n Method: %s", m_Date.c_str(), m_ExecptionDesc.c_str(), m_FileName.c_str(), m_Line, m_Function.c_str()));
	}
private:
	std::string m_ExecptionDesc;
	std::string m_FileName;
	int m_Line;
	std::string m_Date;
	std::string m_Function;
};