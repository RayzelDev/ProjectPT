#pragma once

#include "Utils/strings.h"
#include "smPacket.h"

#define PACKET_SEND_CHARDAMAGE_GAME   0x251924

struct sDamagePlayer
{
	int size;
	int code;
	int Damage;
};

class DamageShop
{

public:
	static              DamageShop* GetInstance() { static DamageShop instance; return &instance; }

	double getPlayerDamage(rsPLAYINFO* Player);
	void addDamageToPlayer(rsPLAYINFO* Player, sDamagePlayer* Damage);
	void SendDamageToGame(rsPLAYINFO* Player);
};



