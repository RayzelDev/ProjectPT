
#include "PacketWriter.h"

namespace IOPacket
{
	struct PacketHeader
	{
		unsigned int Size;
		unsigned int Code;
	};

	PacketWriter::PacketWriter(unsigned int OperationCode, unsigned int ExpectedSize)
	{
		m_Memory.reserve((sizeof(int) * 2) + ExpectedSize);

		this->operator<<(0);
		this->operator<<(OperationCode);
	}

	PacketWriter& PacketWriter::operator<<(const bool Value)
	{
		Write(&Value, sizeof(Value));
		return *this;
	}

	PacketWriter& PacketWriter::operator<<(const char Value)
	{
		Write(&Value, sizeof(Value));
		return *this;
	}

	PacketWriter& PacketWriter::operator<<(const unsigned char Value)
	{
		Write(&Value, sizeof(Value));
		return *this;
	}

	PacketWriter& PacketWriter::operator<<(const short Value)
	{
		Write(&Value, sizeof(Value));
		return *this;
	}

	PacketWriter& PacketWriter::operator<<(const unsigned short Value)
	{
		Write(&Value, sizeof(Value));
		return *this;
	}

	PacketWriter& PacketWriter::operator<<(const int Value)
	{
		Write(&Value, sizeof(Value));
		return *this;
	}

	PacketWriter& PacketWriter::operator<<(const unsigned int Value)
	{
		Write(&Value, sizeof(Value));
		return *this;
	}

	PacketWriter& PacketWriter::operator<<(const long long int Value)
	{
		Write(&Value, sizeof(Value));
		return *this;
	}

	PacketWriter& PacketWriter::operator<<(const unsigned long long int Value)
	{
		Write(&Value, sizeof(Value));
		return *this;
	}

	PacketWriter& PacketWriter::operator<<(const long Value)
	{
		Write(&Value, sizeof(Value));
		return *this;
	}

	PacketWriter& PacketWriter::operator<<(const unsigned long Value)
	{
		Write(&Value, sizeof(Value));
		return *this;
	}

	PacketWriter& PacketWriter::operator<<(const char* Value)
	{
		this->operator<<(std::string(Value));
		return *this;
	}

	PacketWriter& PacketWriter::operator<<(const float Value)
	{
		Write(&Value, sizeof(Value));
		return *this;
	}

	PacketWriter& PacketWriter::operator<<(const double Value)
	{
		Write(&Value, sizeof(Value));
		return *this;
	}

	PacketWriter& PacketWriter::operator<<(const std::string& Value)
	{
		auto LastPosition = m_Memory.size();

		m_Memory.resize(m_Memory.size() + Value.length() + 1);
		memcpy_s(&m_Memory[LastPosition], Value.length(), Value.c_str(), Value.length());
		m_Memory[LastPosition + Value.length()] = '\0';

		return *this;
	}

	PacketWriter& PacketWriter::operator<<(const std::pair<const void*, unsigned int>& Value)
	{
		Write(Value.first, Value.second);
		return *this;
	}

	PacketWriter::operator void* ()
	{
		return (void*)m_Memory.data();
	}

	PacketWriter::operator char* ()
	{
		return (char*)m_Memory.data();
	}

	PacketWriter::operator const void* () const
	{
		return (const void*)m_Memory.data();
	}

	PacketWriter::operator const char* () const
	{
		return (const char*)m_Memory.data();
	}

	void PacketWriter::Finalize()
	{
		unsigned int Size = static_cast<unsigned int>(GetSize());
		WriteToBuffer(m_Memory.data(), &Size, sizeof(Size));
	}

	size_t PacketWriter::GetSize() const
	{
		return m_Memory.size();
	}

	void PacketWriter::Write(const void* Buffer, unsigned int Size)
	{
		m_Memory.reserve(m_Memory.size() + Size);
		m_Memory.insert(m_Memory.end(), (const char*)Buffer, (const char*)Buffer + Size);
	}

	void PacketWriter::WriteToBuffer(void* Buffer, const void* Input, unsigned int InputSize)
	{
		memcpy_s(Buffer, InputSize, Input, InputSize);
	}

	void PacketWriter::WriteAtPosition(unsigned int Position, const void* Buffer, unsigned int Size)
	{
		// Get backup size.
		auto BackupSize = m_Memory.size();

		// Resize to fit.
		m_Memory.resize(m_Memory.size() + Size);

		// A way out of sanitizer bugs.
		std::vector<char> Vector(m_Memory.begin() + Position, m_Memory.begin() + BackupSize);

		// Write to buffer.
		WriteToBuffer(m_Memory.data() + Position + Size, Vector.data(), static_cast<unsigned int>(Vector.size()));

		// Write to buffer.
		WriteToBuffer(m_Memory.data() + Position, Buffer, Size);
	}
}