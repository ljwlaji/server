#include <MessageBlock.h>
#include <Execption.h>
#include <iostream>
#include <zlib.h>

//For memcpy
#ifdef WIN32
#include <memory>
#else
#include <string.h>
#endif

MessageBlock * MessageBlock::CreateBlock(const char* Buffer)
{
	uLong Lenth = strlen(Buffer);
	uLong AfterLen = 0;
	char* CompressBuff = new (std::nothrow)char();
	if (!CompressBuff)
		throw(CREATE_EXECPTION("Create Buffer Failed!"));

	if (compress((Bytef*)CompressBuff, &AfterLen, (Bytef*)Buffer, Lenth) != Z_OK)
	{
		delete CompressBuff;
		return nullptr;
	}

	MessageBlock* _MessageBlock = new MessageBlock();
	_MessageBlock->SetSize(AfterLen);
	_MessageBlock->SetBuffer(CompressBuff);
	return _MessageBlock;
}

bool MessageBlock::Serialize(char * Dest, uint64 & Size)
{
	ASSERT(m_Buffer && m_Size);
	if (uncompress((Bytef*)Dest, (uLong*)Size, (Bytef*)m_Buffer, m_Size) == Z_OK)
		return true;
	return false;
}

MessageBlock::MessageBlock() : m_TimePoint(0), m_Size(0), m_Cmd(0), m_Buffer(nullptr)

{
}

MessageBlock::~MessageBlock()
{
	if (m_Buffer)
	{
		delete m_Buffer;
		m_Buffer = nullptr;
	}
}

/*
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
*/