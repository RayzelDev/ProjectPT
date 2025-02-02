#pragma once
#include <string>
#include <iostream>

namespace IOPacket
{
	// Outdated version of Kenji::Reader/Writer, but useful for most cases.
	class PacketReader
	{
	public:
		using Exception = std::runtime_error;

	public:
		PacketReader(const char* Data, unsigned int Size);
		virtual ~PacketReader() = default;

		// Operator overloading.
		PacketReader& operator>> (bool& Value);
		PacketReader& operator>> (char& Value);
		PacketReader& operator>> (unsigned char& Value);
		PacketReader& operator>> (short& Value);
		PacketReader& operator>> (unsigned short& Value);
		PacketReader& operator>> (int& Value);
		PacketReader& operator>> (unsigned int& Value);
		PacketReader& operator>> (long long int& Value);
		PacketReader& operator>> (unsigned long long int& Value);
		PacketReader& operator>> (float& Value);
		PacketReader& operator>> (double& Value);
		PacketReader& operator>> (std::string& Value);
		PacketReader& operator>> (std::pair<void*, unsigned int> Value);

		void Seek(unsigned int Position, std::ios::seekdir Direction = std::ios::beg);
		bool EndOfFile() const;

		// Returns memory pointer at position.
		const char* Get() const;

	private:
		void Read(void* Buffer, unsigned int Size);

	private:
		const char* m_Memory;
		unsigned int m_Size;
		unsigned int m_Position;
	};
}
