#pragma once
#include <string>
#include <vector>

namespace IOPacket
{
	// Outdated version of Kenji::Reader/Writer, but useful for most cases.
	class PacketWriter
	{
	public:
		PacketWriter(unsigned int OperationCode, unsigned int ExpectedSize = 64);
		virtual ~PacketWriter() = default;

		// Operator overloading.
		PacketWriter& operator<< (const bool Value);
		PacketWriter& operator<< (const char Value);
		PacketWriter& operator<< (const unsigned char Value);
		PacketWriter& operator<< (const short Value);
		PacketWriter& operator<< (const unsigned short Value);
		PacketWriter& operator<< (const int Value);
		PacketWriter& operator<< (const unsigned int Value);
		PacketWriter& operator<< (const long long int Value);
		PacketWriter& operator<< (const unsigned long long int Value);
		PacketWriter& operator<< (const long Value);
		PacketWriter& operator<< (const unsigned long Value);
		PacketWriter& operator<< (const char* Value);
		PacketWriter& operator<< (const float Value);
		PacketWriter& operator<< (const double Value);
		PacketWriter& operator<< (const std::string& Value);
		PacketWriter& operator<< (const std::pair<const void*, unsigned int>& Value);

		// Implicit conversions.
		operator void* ();
		operator char* ();
		operator const void* () const;
		operator const char* () const;

		// Updates the packet size to match the header.
		void Finalize();

		// Get packet size.
		size_t GetSize() const;

	private:
		void Write(const void* Buffer, unsigned int Size);
		void WriteToBuffer(void* Buffer, const void* Input, unsigned int InputSize);
		void WriteAtPosition(unsigned int Position, const void* Buffer, unsigned int Size);

	private:
		std::vector<char> m_Memory;
	};
}
