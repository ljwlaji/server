#include <MessageBlock.h>
#include <Execption.h>
#include <iostream>
//For memcpy
#ifdef WIN32
#include <memory>
#else
#include <string.h>
#endif

MessageBlock * MessageBlock::CreateBlock(const char * InBuffer, int Lenth)
{
	MessageBlock* block = new(std::nothrow)MessageBlock(InBuffer, Lenth);
	if (nullptr == block)
	{
		throw(Execption("Create MessageBlock Error : Not Enough Memery", __FILE__, __LINE__, __DATE__, __FUNCTION__));
		return nullptr;
	}
	return block;
}

MessageBlock::MessageBlock(const char * InBuffer, int Lenth) : m_Length(Lenth), m_Buffer(new char[1024]())
{
	memcpy(m_Buffer, InBuffer, Lenth);
}

MessageBlock::~MessageBlock()
{
	delete[] m_Buffer;
}

bool MessageBlock::GetBuffer(char* Dest, int& DataLenth, int& RequireLenth)
{
	if (RequireLenth > m_Length)
		return false;
	memcpy(Dest + DataLenth, m_Buffer, (size_t)RequireLenth);
	memcpy(m_Buffer, m_Buffer + RequireLenth, (size_t)m_Length);
	m_Length -= RequireLenth;
	return true;
}

bool MessageBlock::InsertBuffer(const char * Src, int DataLenth)
{
	memcpy(m_Buffer + m_Length, Src, DataLenth);
	m_Length += DataLenth;
	return true;
}

void MessageBlock::DebugPrint()
{
	for (int i = 0; i != m_Length; i++)
		std::cout << m_Buffer[i];
	std::cout << std::endl;
}