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

	MessageBlock* _MessageBlock = new (std::nothrow)MessageBlock();
	if (!_MessageBlock)
	{
		delete CompressBuff;
		throw(CREATE_EXECPTION("Create MessageBlock Failed, Not Enough Memory."));
	}
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