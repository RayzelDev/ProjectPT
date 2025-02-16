#pragma once

#include <Windows.h>
#include <vector>
#include <Engine/CFont.h>

#define PACKET_SEND_CHARDAMAGE_GAME   0x251924

enum Type {
	Defense,
	Block,
	Evade,
	HP,
	MP,
	STM,
	Damage,
	CriticalDamage,
};

struct sDamagePlayer
{
	int size;
	int code;
	int Damage;
};

class cSHOW_DMG
{
public:
	struct sDMG
	{
		int Time;
		DWORD objSerial;
		Type Type;

		char Text[32];
		int TextWidth;
	};

	cSHOW_DMG();
	static cSHOW_DMG* getInstance();
	void AddDmg(int Dmg, DWORD Serial, Type Type);
	void AddDef(DWORD Serial, Type Type, int value = 0);
	void DrawDmg(DWORD TargetSerial, int x, int y);
	void MandaDamageProServer(INT64 damage);
	INT64 FormulaDeExp(INT64 exp);

private:
	std::vector<sDMG*> Damages;
	CFont* pFont;
};
