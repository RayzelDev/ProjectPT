#pragma once
#include <queue>
#include <vector>
#include <unordered_map>
#include <string>

#include "smLib3d/smd3d.h"
#include "sinbaram/sinItem.h"

namespace IOPacket
{
	class PacketReader;
}

class RollDice
{
	using Ranking = std::tuple<int, int, std::string>;

public:
	RollDice(const RollDice&) = delete;
	RollDice& operator=(const RollDice&) = delete;
	~RollDice() = default;
	static RollDice& GetInstance();

private:
	RollDice();

public:
	void Initialize();
	void Open();
	void Close();
	bool IsOpen() const;
	bool IsHover() const;
	void Update();
	void Render();

	void HandlePacket(IOPacket::PacketReader& Packet, unsigned int PacketCode);
	bool LButtonDown();

private:
	void Roll();
	void Pass();

private:
	bool m_Open;
	bool m_WaitDice;

	int m_ServerKey;

	unsigned int m_ExpirationTime;
	unsigned int m_CloseTime;

	sITEM m_ShowItem;

	std::vector<Ranking> m_Users;

	IDirect3DTexture9* m_BackgroundTexture;
	IDirect3DTexture9* m_CloseTexture;
	IDirect3DTexture9* m_RollTexture;
	IDirect3DTexture9* m_PassTexture;
	IDirect3DTexture9* m_RankingTexture;
};
