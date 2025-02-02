#include <unordered_map>
#include <Queue>
#include "sinbaram/sinLinkHeader.h"
#include "srcServer/onserver.h"
#include "RollDice.h"
#include "Database/SQLConnection.h"
#include "IO/PacketReader.h"
#include "IO/PacketWriter.h"


constexpr unsigned int MaximumDropDistance = 16 * 64 * 16 * 64;
constexpr unsigned int MaxRollDiceTime = 9 * 1000;
constexpr unsigned int DelayForSkippingTime = 2 * 1000;


int RecordLog(char* lpLogName, char* lpBuff) {
	HANDLE	hFile;
	DWORD	dwAcess;
	DWORD	FileLength;
	SYSTEMTIME	st;
	char	szFileName[128];
	char	szBuff[512];

	GetLocalTime(&st);
	wsprintf(szFileName, "Logs\\LogFile\\(%s)%d-%d.log", lpLogName, st.wDay, st.wMonth);
	wsprintf(szBuff, "%d:%d:%d - %s\r\n", st.wHour, st.wMinute, st.wSecond, lpBuff);

	hFile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}

	FileLength = GetFileSize(hFile, NULL);
	SetFilePointer(hFile, FileLength, NULL, FILE_BEGIN);
	WriteFile(hFile, szBuff, lstrlen(szBuff), &dwAcess, NULL);
	CloseHandle(hFile);

	return TRUE;
}


char* FormatString(char* fmt, ...);
int rsSendMessageAll(char* szText, DWORD dwColor);
int SendQuickMessage(rsPLAYINFO* lpPlayInfo, char* szMessage);
extern std::unordered_map<DWORD, std::unordered_map<rsPLAYINFO*, DWORD>> DamageListNew;

RollDice& RollDice::GetInstance()
{
	static RollDice Instance;
	return Instance;
}

RollDice::RollDice()
{
	m_CurrentKey = GetRandomPos(INT32_MIN, INT32_MAX);
}

int srSetItemFromCode(psITEM* lpsItem, char* szCoed);
extern int DefaultItemCount;
extern sDEF_ITEMINFO* DefaultItems;

void RollDice::PushMonsterKilled(const smCHAR* Monster)
{
	// Create drop infos.
	if (Monster->smMonsterInfo.FallItemPerMax == 0 || Monster->smMonsterInfo.FallItemCount == 0)
		return;

	RollDiceDrops Drops = { 0 };
	auto lpMonInfo = &Monster->smMonsterInfo;

	for (int i = 0; i < lpMonInfo->FallItemMax; i++)
	{
		auto Chance = static_cast<int>(((rand() << 7) | ((GetCurrentTime() >> 2) & 0x7F)) % lpMonInfo->FallItemPerMax);
		auto Rating = 0;

		for (int j = 0; j < lpMonInfo->FallItemCount; j++)
		{
			if (Chance >= Rating && Chance < (Rating + lpMonInfo->FallItems[j].Percentage))
			{
				if (lpMonInfo->FallItems[j].dwItemCode && lpMonInfo->FallItems[j].dwItemCode != (sinGG1 | sin01))
				{
					DWORD dwCode = lpMonInfo->FallItems[j].dwItemCode;

					for (int c = 0; c < DefaultItemCount; c++)
					{
						if (DefaultItems[c].Item.CODE == dwCode && !DefaultItems[c].Item.UniqueItem)
						{
							if (DefaultItems[c].sGenDay[0] > 0)
							{
								if (DefaultItems[c].sGenDay[0] <= DefaultItems[c].sGenDay[1])
								{
									break;
								}
								else
								{
									DefaultItems[c].sGenDay[1]++;
								}
							}

							sITEMINFO ItemInfo = { 0 };
							CreateDefItem(&ItemInfo, &DefaultItems[c]);
		
							
							ZeroMemory(&ItemInfo.ItemHeader, sizeof(ItemInfo.ItemHeader));
							Drops.ItemQueue.push(ItemInfo);
						}
					}
				}

				break;
			}

			Rating += lpMonInfo->FallItems[j].Percentage;
		}
	}

	if (Drops.ItemQueue.empty())
		return;

	if (DamageListNew[Monster->dwObjectSerial].empty())
		return;

	auto Players = std::vector<std::pair<rsPLAYINFO*, DWORD>>{ DamageListNew[Monster->dwObjectSerial].begin(), DamageListNew[Monster->dwObjectSerial].end() };
	for (auto it = Players.begin(); it != Players.end();)
	{
		bool ErasePlayer = true;

		if (it->first && it->first->lpsmSock && it->first->smCharInfo.Life[0] > 0)
		{
			const int x = (it->first->Position.x - Monster->pX) >> FLOATNS;
			const int z = (it->first->Position.z - Monster->pZ) >> FLOATNS;
			const int dist = x * x + z * z;

			if (dist <= MaximumDropDistance)
			{
				ErasePlayer = false;
			};
		}

		if (ErasePlayer)
		{
			it = Players.erase(it);
			continue;
		}

		++it;
	}


	if (Players.empty())
		return;

	std::sort(Players.begin(), Players.end(), [](const std::pair<rsPLAYINFO*, DWORD>& a, const std::pair<rsPLAYINFO*, DWORD>& b)
		{
			return a.second < b.second;
		});

	for (auto it = Players.begin(); it != Players.end();)
	{
		bool ErasePlayer = false;

		if (it->first && it->first->lpsmSock)
		{
			for (size_t i = 0; i < Players.size(); i++)
			{
				if (Players[i].first && Players[i].first->lpsmSock && Players[i].first != it->first)
				{
					if (it->first->lpsmSock->acc_sin.sin_addr.S_un.S_addr == Players[i].first->lpsmSock->acc_sin.sin_addr.S_un.S_addr)
					{
						ErasePlayer = true;
						break;
					}
				}
			}
		}

		if (ErasePlayer)
		{
			it = Players.erase(it);
			continue;
		}

		++it;
	}

	std::sort(Players.begin(), Players.end(), [](const std::pair<rsPLAYINFO*, DWORD>& a, const std::pair<rsPLAYINFO*, DWORD>& b)
		{
			return a.second > b.second;
		});

	// Max players allowed is defined only here.
	unsigned int MaxAllowedPlayers = 20;

	if (Players.size() > MaxAllowedPlayers)
		Players.resize(MaxAllowedPlayers);

	std::vector<RollDicePlayerInfo> RollDicePlayers(Players.size());

	for (size_t i = 0; i < Players.size(); i++)
	{
		RollDicePlayers[i].Number = -1;
		RollDicePlayers[i].ChooseOrder = -1;
		RollDicePlayers[i].Player = Players[Players.size() - (i + 1)].first;
	}

	Drops.BeginDice = true;
	Drops.Players = RollDicePlayers;

	// Add drop to queue.
	m_DropMap[m_CurrentKey++] = Drops;
}

void RollDice::Update()
{
	for (auto it = m_DropMap.begin(); it != m_DropMap.end();)
	{
		if (it->second.ExpirationTime == 0)
		{
			// First phase.
			IOPacket::PacketWriter Packet(OPCODE_ROLLDICE_START);

			Packet << it->first;
			Packet << it->second.BeginDice;
			Packet << MaxRollDiceTime;

			// Get item from queue.
			const auto& Item = it->second.ItemQueue.front();

			// Encapsulate it.
			Packet << std::make_pair(&Item, sizeof(Item));

			// Finalize.
			Packet.Finalize();

			for (auto& p : it->second.Players)
			{
				p.Player->lpsmSock->Send(Packet, Packet.GetSize());
			}

			// Begins countdown.
			it->second.BeginDice = false;
			it->second.CurrentOrder = 0;
			it->second.ExpirationTime = GetCurrentTime() + MaxRollDiceTime;
			it->second.SkipTime = 0;
		}
		else if (GetCurrentTime() > it->second.ExpirationTime || (it->second.SkipTime && GetCurrentTime() > it->second.SkipTime))
		{
			// Second phase.
			int LastNumber = -1;
			rsPLAYINFO* Winner = nullptr;

			for (auto& p : it->second.Players)
			{
				if (p.Number > LastNumber && p.Number > 0 && p.Player->lpsmSock)
				{
					LastNumber = p.Number;
					Winner = p.Player;
				}

				p.Number = -1;
			}

			if (Winner)
			{
				RewardPlayer(Winner, it->second.ItemQueue.front(), LastNumber);
				RecordLog("RollDice", (char*)FormatString("UID( %d ) - ID( %s ) - Name( %s ) - Won the item ( %s ) with the number( %d  - Map( %d )).",
					it->first, Winner->szID, Winner->szName, it->second.ItemQueue.front().ItemName, LastNumber, Winner->Position.Area));
			}

			it->second.ItemQueue.pop();
			it->second.CurrentOrder = 0;
			it->second.ExpirationTime = 0;
			it->second.SkipTime = 0;

			{
				IOPacket::PacketWriter Msg(OPCODE_ROLLDICE_END);
				Msg << it->second.ItemQueue.size();

				if (Winner)
				{
					Msg << true;
					Msg << Winner->szName;
				}
				else
				{
					Msg << false;
				}

				Msg.Finalize();

				for (auto& p : it->second.Players)
				{
					if (p.Player && p.Player->lpsmSock)
					{
						p.Player->lpsmSock->Send(Msg, Msg.GetSize());
					}
				}
			}

			if (it->second.ItemQueue.empty())
			{
				it = m_DropMap.erase(it);
				continue;
			}
		}

		++it;
	}
}

void RollDice::HandlePacket(IOPacket::PacketReader& Packet, rsPLAYINFO* Player, unsigned int PacketCode)
{
	if (!Player || !Player->lpsmSock)
		return;

	switch (PacketCode)
	{
	case OPCODE_ROLLDICE_UPDATE:
	{
		int Key = 0;
		Packet >> Key;
		bool Roll = false;
		Packet >> Roll;

		if (m_DropMap.find(Key) != m_DropMap.end())
		{
			if (m_DropMap[Key].ExpirationTime > GetCurrentTime())
			{
				IOPacket::PacketWriter Msg(OPCODE_ROLLDICE_UPDATE);
				bool UpdateGame = false;
				bool SkipTiming = true;

				for (auto& p : m_DropMap[Key].Players)
				{
					if (p.Player == Player)
					{
						if (p.Number == -1)
						{
							// Game will sort based on Number and then the ordering.
							p.ChooseOrder = m_DropMap[Key].CurrentOrder++;

							if (Roll)
							{
								// Choose random number.
								p.Number = GetRandomPos(1, 1000);
							}
							else
							{
								p.Number = 0;
							}

							// Flag to update game.
							UpdateGame = true;
						}
					}
					else if (p.Number == -1)
					{
						SkipTiming = false;
					}

					Msg << p.Number;
					Msg << p.ChooseOrder;
					Msg << p.Player->szName;
				}

				if (UpdateGame)
				{
					Msg.Finalize();

					for (auto& p : m_DropMap[Key].Players)
					{
						p.Player->lpsmSock->Send(Msg, Msg.GetSize());
					}
				}

				if (SkipTiming && m_DropMap[Key].SkipTime == 0)
				{
					m_DropMap[Key].SkipTime = GetCurrentTime() + DelayForSkippingTime;
				}
			}
		}
	} break;
	}
}

int CreateCommandItem(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, char* szItem);

void RollDice::RewardPlayer(rsPLAYINFO* Player, const sITEMINFO& Item, int Number)
{
	if (Player)
	{
		for (int i = 0; i < MAX_ITEM; i++)
		{
			if (sItem[i].CODE == Item.CODE)
			{
				CreateCommandItem(Player, Player, sItem[i].LastCategory);
				rsSendMessageAll((char*)FormatString("Roleta do Chefe> %s conquistou o item %s.", Player->szName, Item.ItemName), 17);
				break;
			}
		}
	}
}

void RollDice::OnPlayerQuitGame(rsPLAYINFO* Player)
{
	for (auto& Map : m_DropMap)
	{
		for (auto it = Map.second.Players.begin(); it != Map.second.Players.end();)
		{
			if (it->Player == Player)
			{
				it = Map.second.Players.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}