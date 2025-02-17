#include "..\\sinbaram\\sinLinkHeader.h"
#include "DisplayDamage.h"
#include "CustomHud.h"
#include <Engine/CFontHandler.h>
#include <Settings.h>
#include <Eventos/InterfaceDamage.h>

cSHOW_DMG showDmg;

BOOL showDamage = TRUE;

#define OUTLINE 1

cSHOW_DMG::cSHOW_DMG()
{

}

cSHOW_DMG* cSHOW_DMG::getInstance()
{
	return &showDmg;
}

void cSHOW_DMG::AddDmg(int Dmg, DWORD Serial, Type Type)
{
	sDMG* newDmg = new sDMG;

	if (newDmg)
	{
		if (!pFont)
			pFont = CFONTHANDLER->GetFont("Century Schoolbook", 25, true);

		newDmg->Time = GetCurrentTime() + 1000;
		newDmg->objSerial = Serial;
		newDmg->Type = Type;

		wsprintf(newDmg->Text, "%d", Dmg);
		SIZE sz = {};
		pFont->GetTextSize(newDmg->Text, &sz);
		newDmg->TextWidth = sz.cx;

		Damages.push_back(newDmg);
	}
}

int CheckarLevelTable()
{
	int cnt = 0;
	int ChkSum = 0;

	while (1) {
		if (ExpLevelTable[cnt] == -1) break;
		ChkSum += ExpLevelTable[cnt] * (cnt + 1);
		cnt++;
	}

	return ChkSum;
}

void cSHOW_DMG::AddDef(DWORD Serial, Type Type, int value)
{
	sDMG* newDmg = new sDMG;

	if (newDmg)
	{
		if (!pFont)
			pFont = CFONTHANDLER->GetFont("Century Schoolbook", 25, true);

		newDmg->Time = GetCurrentTime() + 1000;
		newDmg->objSerial = Serial;
		newDmg->Type = Type;

		switch (newDmg->Type)
		{
		case Type::Defense:
			lstrcpy(newDmg->Text, "Defense!");
			break;
		case Type::Block:
			lstrcpy(newDmg->Text, "Block!");
			break;
		case Type::Evade:
			lstrcpy(newDmg->Text, "Evade!");
			break;
		case Type::HP:
		case Type::MP:
		case Type::STM:
			wsprintf(newDmg->Text, "+%d", value);
			break;
		case Type::Damage:
		case Type::CriticalDamage:
			//Logica do Exp por Ataque
			INT64 exp64 = CodeXorCharInfo_Exp();
			int mlevel = CHAR_LEVEL_MAX;
			INT64 exp = FormulaDeExp(value);

			//Logica para ADD EXP por Ataque
			if (exp64 < ExpLevelTable[mlevel])
			{
				exp64 += exp;
				wsprintf(newDmg->Text, "%d", value);
				SetExp64(&lpCurPlayer->smCharInfo, exp64);
				CodeXorCharInfo_Exp();
				ReformCharForm();
				char szTemp[32] = { 0 };
				NumLineComa64(exp, szTemp);
				MandaDamageProServer(value);
				CHATGAMEHANDLE->AddChatBoxTextEx(EChatColor::CHATCOLOR_Notice, "> Você Ganhou %s de experiência!", szTemp);

				int Level = lpCurPlayer->smCharInfo.Level;
				INT64 iexp = ExpLevelTable[Level];
				int LevelTableSum = 547991164;

				if (LevelTableSum != CheckarLevelTable())
				{
					DisconnectFlag = GetCurrentTime();
					break;
				}

				while (iexp >= 0 && exp64 >= iexp && lpCurPlayer->smCharInfo.Level < CHAR_LEVEL_MAX)
				{

					lpCurPlayer->smCharInfo.Level++;
					lpCurPlayer->smCharInfo.Next_Exp = ExpLevelTable[Level + 1];
					StartEffect(lpCurPlayer->pX, lpCurPlayer->pY + 32 * fONE, lpCurPlayer->pZ, EFFECT_LEVELUP1);
					esPlaySound(7, 400);
					ReformCharForm();

					SendPlayUpdateInfo();
					SaveGameData();

					Level = lpCurPlayer->smCharInfo.Level;
					iexp = ExpLevelTable[Level];
				}
			}
			break;
		}

		SIZE sz = {};
		pFont->GetTextSize(newDmg->Text, &sz);
		newDmg->TextWidth = sz.cx;

		Damages.push_back(newDmg);
	}
}

void cSHOW_DMG::MandaDamageProServer(INT64 damage)
{
	if (InterfaceDamage.damage < 200000000)
	{
		sDamagePlayer sPacket;
		sPacket.size = sizeof(sPacket);
		sPacket.code = PACKET_RECEBE_DAMAGE_CHAR;
		sPacket.Damage = damage / 100;
		smWsockServer->Send((char*)&sPacket, sPacket.size, TRUE);
	}
}

INT64 cSHOW_DMG::FormulaDeExp(INT64 exp)
{
	exp = exp * 2000;

	//ModoPK 10% 
	if (lpCurPlayer->PlayerPvPMode == TRUE)
	{
		exp = exp + (exp * 0.1);
	}

	//Fenix 20% chaPremiumitem?
	if (chaPremiumitem.m_PhenixPetTime > 0)
	{
		exp = exp + (exp * 0.2);
	}

	//VIP 20%
	if (lpCurPlayer->vipLevel > 0)
	{
		exp = exp + (exp * 0.2);
	}

	//Poção de Exp 30%
	if (chaPremiumitem.m_ExpUpPotionTime > 0)
	{
		exp = exp + (exp * 0.3);
	}

	return exp;
}
void cSHOW_DMG::DrawDmg(DWORD TargetSerial, int x, int y)
{
	char szBuff[255] = { 0 };

	if (smConfig.showDamage) return;
	{

#ifdef _DEBUG
		for (auto& it : Damages)
		{
			sDMG* DmgInfo = it;
#else
		for (std::vector<sDMG*>::iterator it = Damages.begin(); it < Damages.end(); it++)
		{
			sDMG* DmgInfo = (*it);
#endif
			if (DmgInfo && DmgInfo->objSerial == TargetSerial)
			{
				int Time = DmgInfo->Time - GetCurrentTime();
				if (Time > 0)
				{
					int Alpha = int(255 * (Time / 1000.f));
					// Defesa
					DWORD dwColor = D3DCOLOR_ARGB(Alpha, 50, 185, 240);
					switch (DmgInfo->Type)
					{
					case Type::HP:
						dwColor = D3DCOLOR_ARGB(Alpha, 185, 50, 50);
						break;
					case Type::MP:
						dwColor = D3DCOLOR_ARGB(Alpha, 75, 100, 210);
						break;
					case Type::STM:
						dwColor = D3DCOLOR_ARGB(Alpha, 50, 185, 70);
						break;

						// Block
					case Type::Block:
						dwColor = D3DCOLOR_ARGB(Alpha, 255, 220, 0);
						break;
						// Evade // Dano
					case Type::Evade:
					case Type::Damage:
					case Type::CriticalDamage:
						dwColor = D3DCOLOR_ARGB(Alpha, 255, 160, 57);
						break;
					}

					RECT rc = { (x - DmgInfo->TextWidth / 2) - OUTLINE, (y - 83 + (Time / 16)), WinSizeX, WinSizeY };
					pFont->SetFontColor(D3DCOLOR_ARGB(Alpha, 1, 1, 1));
					pFont->DrawText(DmgInfo->Text, &rc, 0);

					rc = { (x - DmgInfo->TextWidth / 2), (y - 83 + (Time / 16)) - OUTLINE, WinSizeX, WinSizeY };
					pFont->DrawText(DmgInfo->Text, &rc, 0);

					rc = { (x - DmgInfo->TextWidth / 2) + OUTLINE, (y - 83 + (Time / 16)), WinSizeX, WinSizeY };
					pFont->DrawText(DmgInfo->Text, &rc, 0);

					rc = { (x - DmgInfo->TextWidth / 2), (y - 83 + (Time / 16)) + OUTLINE, WinSizeX, WinSizeY };
					pFont->DrawText(DmgInfo->Text, &rc, 0);

					rc = { (x - DmgInfo->TextWidth / 2), (y - 83 + (Time / 16)), WinSizeX, WinSizeY };
					pFont->SetFontColor(dwColor);
					pFont->DrawText(DmgInfo->Text, &rc, 0);


				}
				else {
					delete DmgInfo;
#ifndef _DEBUG
					Damages.erase(it);
#endif

				}
			}
		}
	}
}