#include "RollDice.h"
#include "sinbaram/sinLinkHeader.h"
#include "IO/PacketReader.h"
#include "IO/PacketWriter.h"
#include "UI/UIRect2D.h"

constexpr unsigned int RollDiceTimeout = 8 * 1000;
constexpr const char* RollDiceOpenMessage = "Roleta do Chefe> Parabéns, você está participando do sorteio !";
constexpr const char* RollDiceWonItem = "Roleta do Chefe> Parabéns %s, você ganhou o item sorteado.";
constexpr const char* RollDiceItemWinner = "Roleta do Chefe> %s foi o vencedor da vez.";

RollDice& RollDice::GetInstance()
{
	static RollDice Instance;
	return Instance;
}

RollDice::RollDice()
{
	m_Open = false;
	m_WaitDice = false;

	m_ServerKey = 0;

	m_ExpirationTime = 0;
	m_CloseTime = 0;

	ZeroMemory(&m_ShowItem, sizeof(m_ShowItem));

	m_BackgroundTexture = nullptr;
	m_CloseTexture = nullptr;
	m_RollTexture = nullptr;
	m_PassTexture = nullptr;
	m_RankingTexture = nullptr;
}

void RollDice::Initialize()
{
	m_BackgroundTexture = LoadDibSurfaceOffscreen("Game\\images\\RollDice\\ItemBg.png");
	m_CloseTexture = LoadDibSurfaceOffscreen("Game\\images\\RollDice\\RankClose_.png");
	m_RollTexture = LoadDibSurfaceOffscreen("Game\\images\\RollDice\\Roll_.png");
	m_PassTexture = LoadDibSurfaceOffscreen("Game\\images\\RollDice\\Pass_.png");
	m_RankingTexture = LoadDibSurfaceOffscreen("Game\\images\\RollDice\\RankBg.png");
}

void RollDice::Open()
{
	m_Open = true;
}

void RollDice::Close()
{
	m_Open = false;
}

bool RollDice::IsOpen() const
{
	return m_Open;
}

bool RollDice::IsHover() const
{
	if (!m_Open)
		return false;

	auto X = (WinSizeX - 262) / 2;
	auto Y = (WinSizeY - 540) / 2;

	if (pRealCursorPos.x > X && pRealCursorPos.x <= X + 262 &&
		pRealCursorPos.y > Y && pRealCursorPos.y <= Y + 240)
		return true;

	X = (WinSizeX - 500) / 2;
	Y = (WinSizeY - 60) / 2;

	if (pRealCursorPos.x > X && pRealCursorPos.x <= X + 500 &&
		pRealCursorPos.y > Y && pRealCursorPos.y <= Y + 312)
		return true;

	return false;
}

void RollDice::Update()
{
	if (!m_Open)
		return;

	auto BackgroundX = (WinSizeX - 262) / 2;
	auto BackgroundY = (WinSizeY - 540) / 2;

	if (pRealCursorPos.x > BackgroundX + 99 && pRealCursorPos.x <= BackgroundX + 99 + 64 &&
		pRealCursorPos.y > BackgroundY + 72 && pRealCursorPos.y <= BackgroundY + 72 + 86)
	{
		m_ShowItem.x = BackgroundX + 99;
		m_ShowItem.y = BackgroundY + 72;

		sinShowItemInfoFlag = 1;
		cItem.ShowItemInfo(&m_ShowItem, 2);
	}

	if (m_ExpirationTime && m_ExpirationTime < dwPlayTime)
	{
		if (!m_WaitDice)
			Pass();

		// Reset timer.
		m_ExpirationTime = 0;
	}

	if (m_CloseTime && m_CloseTime < dwPlayTime)
	{
		// Self close window.
		Close();

		// Reset timer.
		m_CloseTime = 0;

		// Reset wait dice.
		m_WaitDice = false;
	}
}

void RollDice::Render()
{
	if (!m_Open)
		return;

	auto r = RECT{ 0 };

	auto BackgroundX = (WinSizeX - 262) / 2;
	auto BackgroundY = (WinSizeY - 540) / 2;
	DrawSprite(BackgroundX, BackgroundY, m_BackgroundTexture, 0, 0, 262, 240);

	auto RankBgX = (WinSizeX - 500) / 2;
	auto RankBgY = (WinSizeY - 60) / 2;
	DrawSprite(RankBgX, RankBgY, m_RankingTexture, 0, 0, 500, 312);

	if (m_ShowItem.Flag)
	{
		auto BackgroundX = (WinSizeX - 262) / 2;
		auto BackgroundY = (WinSizeY - 540) / 2;

		auto Rect = RECT{ BackgroundX + 99, BackgroundY + 72, BackgroundX + 99 + 64, BackgroundY + 72 + 86 };

		if (pRealCursorPos.x > Rect.left && pRealCursorPos.x <= Rect.right &&
			pRealCursorPos.y > Rect.top && pRealCursorPos.y <= Rect.bottom)
		{
			dsDrawColorBox(sinInvenColor[4], Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top);
		}

		auto Rect2D = UI::AdjustTextureToOrigin(UI::Rect2D(Rect), m_ShowItem.lpItem, true);
		DrawSprite(Rect2D.GetX(), Rect2D.GetY(), m_ShowItem.lpItem, 0, 0, m_ShowItem.w, m_ShowItem.h, -1, Rect2D.GetWidth(), Rect2D.GetHeight());

		r.left = BackgroundX + 20; r.top = BackgroundY + 162;
		r.right = r.left + 222; r.bottom = r.top + 20;

		DX::DrawTextFont(r.left, r.top, m_ShowItem.sItemInfo.ItemName, RGB(255, 255, 255), 0);
	}

	if ((m_ShowItem.CODE & sinITEM_MASK2) == sinFO1)
		cInvenTory.DrawForceOrbEffect(m_ShowItem.x, m_ShowItem.y);

	for (size_t i = 0; i < m_Users.size(); i++)
	{
		auto FixX = RankBgX + (238 * ((i / 10) % 2));
		auto FixY = RankBgY + ((20 * ((i % 10))) + (200 * (i / 20)));

		r.left = 13 + FixX; r.top = 104 + FixY;
		r.right = r.left + 20; r.bottom = r.top + 19;
		DX::DrawTextFont(r.left, r.top, std::to_string(i + 1).c_str(), RGB(255, 255, 255), 0);

		r.left = 34 + FixX; r.top = 104 + FixY;
		r.right = r.left + 137; r.bottom = r.top + 19;

		DX::DrawTextFont(r.left, r.top, std::get<2>(m_Users[i]).c_str(), RGB(255, 255, 255), 0);

		r.left = 172 + FixX; r.top = 104 + FixY;
		r.right = r.left + 78; r.bottom = r.top + 19;

		DX::DrawTextFont(r.left, r.top, std::to_string(std::get<0>(m_Users[i])).c_str(), RGB(255, 255, 255), 0);
	}

	{
		auto TimeLeft = (m_ExpirationTime >= dwPlayTime ? ((m_ExpirationTime + 500) - dwPlayTime) / 1000 : 0);
		r.left = BackgroundX + 40; r.top = BackgroundY + 180;
		r.right = r.left + 181; r.bottom = r.top + 19;

		DX::DrawTextFont(r.left, r.top, (std::to_string(TimeLeft) + "s").c_str(), sinInvenColor[4], 0);
	}

	// Roll
	if (pRealCursorPos.x > BackgroundX + 36 && pRealCursorPos.x <= BackgroundX + 36 + 88 &&
		pRealCursorPos.y > BackgroundY + 199 && pRealCursorPos.y <= BackgroundY + 199 + 25)
	{
		DrawSprite(BackgroundX + 36, BackgroundY + 199, m_RollTexture, 0, 0, 88, 25);
	}

	// Pass
	if (pRealCursorPos.x > BackgroundX + 136 && pRealCursorPos.x <= BackgroundX + 136 + 88 &&
		pRealCursorPos.y > BackgroundY + 199 && pRealCursorPos.y <= BackgroundY + 199 + 25)
	{
		DrawSprite(BackgroundX + 136, BackgroundY + 199, m_PassTexture, 0, 0, 88, 25);
	}

	// Close
	if (pRealCursorPos.x > BackgroundX + 244 && pRealCursorPos.x <= BackgroundX + 244 + 16 &&
		pRealCursorPos.y > BackgroundY + 5 && pRealCursorPos.y <= BackgroundY + 5 + 15)
	{
		DrawSprite(BackgroundX + 238, BackgroundY + 8, m_CloseTexture, 0, 0, 16, 15);
	}
}

void RollDice::HandlePacket(IOPacket::PacketReader& Packet, unsigned int PacketCode)
{
	//	LOGEx("KitaDevs", "RollDice() - PacketCode( 0x%d ).", PacketCode);

	switch (PacketCode)
	{
	case OPCODE_ROLLDICE_START:
	{
		bool ShowMessage = false;

		Packet >> m_ServerKey;
		Packet >> ShowMessage;
		Packet >> m_CloseTime;

		m_ExpirationTime = dwPlayTime + RollDiceTimeout;
		m_CloseTime += dwPlayTime;

		sITEMINFO ItemData = { 0 };
		Packet >> std::make_pair(&ItemData, sizeof(ItemData));

		// Try loading item image.
		LoadItemImage(&ItemData, &m_ShowItem);

		// Reset wait dice.
		m_WaitDice = false;

		// Prepare for new users.
		m_Users.clear();

		// Self open this window.
		Open();

		// Show message if required.
		if (ShowMessage)
			cMessageBox.ShowMessageEvent(const_cast<char*>(RollDiceOpenMessage));
	} break;

	case OPCODE_ROLLDICE_UPDATE:
	{
		// Declare users.
		std::vector<Ranking> Users;

		// Read any ammount of players available.
		while (!Packet.EndOfFile())
		{
			int NumberRolled = 0;
			int ChooseOrder = 0;
			std::string UserName;

			Packet >> NumberRolled;
			Packet >> ChooseOrder;
			Packet >> UserName;

			// Insert into temporary vector.
			Users.emplace_back(std::make_tuple(NumberRolled, ChooseOrder, UserName));
		}

		// Sort order.
		std::sort(Users.begin(), Users.end(), [](const Ranking& a, const Ranking& b)
			{
				// Number is greater (order first).
				if (std::get<0>(a) > std::get<0>(b))
					return true;

				// Number is lesser (order later).
				if (std::get<0>(a) < std::get<0>(b))
					return false;

				// Number is equal, order by the choosing time.
				return std::get<1>(a) < std::get<1>(b);
			});

		m_Users = Users;
	} break;

	case OPCODE_ROLLDICE_END:
	{
		// Get number of items left.
		size_t ItemsLeft = 0;
		Packet >> ItemsLeft;

		// Check if we have a winner.
		bool Winner = false;
		Packet >> Winner;

		if (Winner)
		{
			// Get winner name.
			std::string Name;
			Packet >> Name;

			// Check if it is me.
			if (_strcmpi(lpCurPlayer->smCharInfo.szName, Name.c_str()) == 0)
			{
				char Msg[256] = { 0 };
				sprintf_s(Msg, RollDiceWonItem, Name.c_str());
				cMessageBox.ShowMessageEvent(Msg);
			}
			else
			{
				char Msg[256] = { 0 };
				sprintf_s(Msg, RollDiceItemWinner, Name.c_str());
				cMessageBox.ShowMessageEvent(Msg);
			}
		}

		if (ItemsLeft == 0)
			m_CloseTime = dwPlayTime + 1000;
	} break;
	}
}

bool RollDice::LButtonDown()
{
	auto BackgroundX = (WinSizeX - 262) / 2;
	auto BackgroundY = (WinSizeY - 540) / 2;

	// Roll
	if (pRealCursorPos.x > BackgroundX + 36 && pRealCursorPos.x <= BackgroundX + 36 + 88 &&
		pRealCursorPos.y > BackgroundY + 199 && pRealCursorPos.y <= BackgroundY + 199 + 25)
	{
		if (!m_WaitDice)
			Roll();

		return true;
	}

	// Pass
	if (pRealCursorPos.x > BackgroundX + 136 && pRealCursorPos.x <= BackgroundX + 136 + 88 &&
		pRealCursorPos.y > BackgroundY + 199 && pRealCursorPos.y <= BackgroundY + 199 + 25)
	{
		if (!m_WaitDice)
			Pass();

		return true;
	}

	// Close
	if (pRealCursorPos.x > BackgroundX + 244 && pRealCursorPos.x <= BackgroundX + 244 + 16 &&
		pRealCursorPos.y > BackgroundY + 5 && pRealCursorPos.y <= BackgroundY + 5 + 15)
	{
		m_CloseTime = dwPlayTime;

		if (!m_WaitDice)
			Pass();

		return true;
	}

	return false;
}

void RollDice::Roll()
{
	IOPacket::PacketWriter Packet(OPCODE_ROLLDICE_UPDATE);
	Packet << m_ServerKey;
	Packet << true;
	Packet.Finalize();

	// Send.
	smWsockServer->Send(Packet, Packet.GetSize());

	// Now waiting for server to roll the dice.
	m_WaitDice = true;
}

void RollDice::Pass()
{
	IOPacket::PacketWriter Packet(OPCODE_ROLLDICE_UPDATE);
	Packet << m_ServerKey;
	Packet << false;
	Packet.Finalize();

	// Send.
	smWsockServer->Send(Packet, Packet.GetSize());

	// Now waiting for server to roll the dice.
	m_WaitDice = true;
}