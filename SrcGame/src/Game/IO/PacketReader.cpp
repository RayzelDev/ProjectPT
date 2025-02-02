#include "PacketReader.h"

namespace IOPacket
{
	struct PacketHeader
	{
		unsigned int Size;
		unsigned int Code;
	};

	PacketReader::PacketReader(const char* Data, unsigned int Size)
	{
		m_Memory = Data;
		m_Size = Size;
		m_Position = 8;
	}

	PacketReader& PacketReader::operator>>(bool& Value)
	{
		Read(&Value, sizeof(Value));
		return *this;
	}

	PacketReader& PacketReader::operator>>(char& Value)
	{
		Read(&Value, sizeof(Value));
		return *this;
	}

	PacketReader& PacketReader::operator>>(unsigned char& Value)
	{
		Read(&Value, sizeof(Value));
		return *this;
	}

	PacketReader& PacketReader::operator>>(short& Value)
	{
		Read(&Value, sizeof(Value));
		return *this;
	}

	PacketReader& PacketReader::operator>>(unsigned short& Value)
	{
		Read(&Value, sizeof(Value));
		return *this;
	}

	PacketReader& PacketReader::operator>>(int& Value)
	{
		Read(&Value, sizeof(Value));
		return *this;
	}

	PacketReader& PacketReader::operator>>(unsigned int& Value)
	{
		Read(&Value, sizeof(Value));
		return *this;
	}

	PacketReader& PacketReader::operator>>(long long int& Value)
	{
		Read(&Value, sizeof(Value));
		return *this;
	}

	PacketReader& PacketReader::operator>>(unsigned long long int& Value)
	{
		Read(&Value, sizeof(Value));
		return *this;
	}

	PacketReader& PacketReader::operator>>(float& Value)
	{
		Read(&Value, sizeof(Value));
		return *this;
	}

	PacketReader& PacketReader::operator>>(double& Value)
	{
		Read(&Value, sizeof(Value));
		return *this;
	}

	PacketReader& PacketReader::operator>>(std::string& Value)
	{
		while (m_Position + 1 <= m_Size)
		{
			if (m_Memory[m_Position] == '\0')
			{
				m_Position++;
				return *this;
			}

			Value.push_back(m_Memory[m_Position++]);
		}

		return *this;
	}

	PacketReader& PacketReader::operator>>(std::pair<void*, unsigned int> Value)
	{
		Read(Value.first, Value.second);
		return *this;
	}

	void PacketReader::Seek(unsigned int Position, std::ios::seekdir Direction /*= std::ios::beg*/)
	{
		switch (Direction)
		{
		case std::ios::beg:
			m_Position = Position;
			break;

		case std::ios::cur:
			m_Position += Position;
			break;

		case std::ios::end:
			m_Position = m_Size - Position;
			break;
		}
	}

	bool PacketReader::EndOfFile() const
	{
		return m_Position >= m_Size;
	}

	const char* PacketReader::Get() const
	{
		return &m_Memory[m_Position];
	}

	void PacketReader::Read(void* Buffer, unsigned int Size)
	{
		if (m_Position + Size > m_Size)
		{
			// Get pointer to header.
			auto* pHeader = reinterpret_cast<const PacketHeader*>(m_Memory);

			// Declare buffer.
			char Buffer[128] = { 0 };

			// Format buffer message.
			sprintf_s(Buffer, "IO::PacketReader::Read(0x%X) - Trying to read past the buffer size ((%d + %d) > %d).", pHeader->Code, m_Position, Size, m_Size);

			// Throws our message.
			throw Exception(Buffer);
		}

		memcpy_s(Buffer, Size, &m_Memory[m_Position], Size);
		m_Position += Size;
	}
}