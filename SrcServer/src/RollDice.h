#pragma once

#include <Queue>
namespace IOPacket
{
	class PacketReader;
}

struct RollDicePlayerInfo
{
	rsPLAYINFO* Player;
	int Number;
	int ChooseOrder;
};

struct RollDiceDrops
{
	bool BeginDice;
	int CurrentOrder;

	unsigned int ExpirationTime;
	unsigned int SkipTime;

	std::queue<sITEMINFO> ItemQueue;
	std::vector<RollDicePlayerInfo> Players;
};

namespace Gandalf
{
	class DropsFieldsBoss
	{
	public:

		static DropsFieldsBoss* GetInstance() { static DropsFieldsBoss instance; return &instance; }

		void Init();

		char* Update(const char* Boss);

		void UpdateSql(int Killeds, int ID, char* NameBoss);

		string SelectRandomItem(int ID);

		void GetKilledBoss(rsPLAYINFO* User, int ID);

		void SetKilledBoss(rsPLAYINFO* User, const int ID, const int Increment);

	private:
		struct KilledBoss
		{
			int ID;
			char NameBoss[32];
			int BossCounts;
			int CountsKilled;
		};

		struct DropsBoss
		{
			int ID;
			char Drop[32];
		};

		//ID , NameBoss, QUANTIDADE, Boss qtnd
		std::vector<KilledBoss> LoadInf;
		//id , drop
		std::vector<DropsBoss> Drops;
	};
};

class RollDice
{
public:
	RollDice(const RollDice&) = delete;
	RollDice& operator=(const RollDice&) = delete;
	~RollDice() = default;
	static RollDice& GetInstance();

private:
	RollDice();

public:
	void PushMonsterKilled(const smCHAR* Monster);
	void Update();
	void HandlePacket(IOPacket::PacketReader& Packet, rsPLAYINFO* Player, unsigned int PacketCode);
	void RewardPlayer(rsPLAYINFO* Player, const sITEMINFO& Item, int Number);
	void OnPlayerQuitGame(rsPLAYINFO* Player);

private:
	std::unordered_map<int, RollDiceDrops> m_DropMap;
	int m_CurrentKey;
};
