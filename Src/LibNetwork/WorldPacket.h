#program once

#include <ShareDefine.h>
#include "ByteBuffer.h"

class WorldPacket : public ByteBuffer
{
public:
	WorldPacket() : ByteBuffer(0), m_opcode(0)
	{

	}

	WorldPacket(const WorldPacket& packet) : ByteBuffer(packet), m_opcode(packet.m_opcode)
	{

	}

	~WorldPacket() 
	{

	}

	uint16 GetOpcode() const
	{
		return m_opcode
	}

    void SetOpcode(uint16 opcode)
    {
        m_opcode = opcode;
    }

	uint16 m_opcode;
	uint32 m_size;


};