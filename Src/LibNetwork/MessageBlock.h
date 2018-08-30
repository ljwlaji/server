#pragma once


class MessageBlock
{
public:
	~MessageBlock();
	static MessageBlock* CreateBlock(const char* InBuffer, int Lenth);
	int Length() { return m_Length; }
	bool GetBuffer(char* Dest, int& DataLenth ,int& RequireLenth);
	bool InsertBuffer(const char* Src, int DataLenth);
	void DebugPrint();
private:
	MessageBlock(const char* InBuffer, int Lenth);

private:
	int m_Length;
	char* m_Buffer;
};