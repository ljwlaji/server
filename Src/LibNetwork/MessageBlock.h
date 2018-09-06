#pragma once
#include <ShareDefine.h>
#include <string>
class MessageBlock
{
public:
	static MessageBlock* CreateBlock(const char* Buffer);
	void SetCMD(uint32 Cmd)								{ m_Cmd = Cmd;			}
	void SetSendTime(uint32 Time)						{ m_TimePoint = Time;	}
	bool Serialize(char* Dest, uint64& Size);
private:
	MessageBlock();
	~MessageBlock();
	void SetSize(uint64 Size)							{ m_Size = Size;		}
	void SetBuffer(char* Buffer)						{ m_Buffer = Buffer;	}
private:
	uint32 m_TimePoint;
	uint64 m_Size;
	uint32 m_Cmd;
	char*  m_Buffer;
};