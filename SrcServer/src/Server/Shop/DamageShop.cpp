#include "DamageShop.h"
#include <zlib.h>
#include "../SQL.h"
#include "..\\GM\GM.h"
#include "..\\VIP\Vip.h"
#include "..\\Database\\SQLConnection.h"


double DamageShop::getPlayerDamage(rsPLAYINFO* Player)
{
	long double result = 0;

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);


	if (db && db->Open())
	{
		const char* const queryAdd = "IF NOT EXISTS (SELECT 1 FROM [dbo].[UserDamage] WHERE CharName = ?) BEGIN INSERT INTO[dbo].[UserDamage]([CharName], [Damage]) VALUES(?, 0) END";
		const char* const query = "SELECT Damage FROM UserDamage WHERE CharName=?";

		if (db->Prepare(queryAdd))
		{
			db->BindInputParameter(Player->szName, 1, PARAMTYPE_String);
			db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);

			if (db->Execute())
				db->GetData(1, PARAMTYPE_Integer, &result);
		}

		if (db->Prepare(query))
		{
			db->BindInputParameter(Player->szName, 1, PARAMTYPE_String);

			if (db->Execute())
				db->GetData(1, PARAMTYPE_Integer, &result);
		}

		db->Close();
	}

	return result;
}

void DamageShop::addDamageToPlayer(rsPLAYINFO* Player, sDamagePlayer* DamagePlayer)
{
	auto qtdDamage = getPlayerDamage(Player);
	if (qtdDamage < 200000000)
	{
		auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

		if (db && db->Open())
		{
			const char* const queryUpdate = "UPDATE UserDamage SET Damage+=? WHERE CharName=?";
			auto damage = DamagePlayer->Damage;

			if (db->Prepare(queryUpdate))
			{
				db->BindInputParameter(&damage, 1, PARAMTYPE_Integer);
				db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);
				db->Execute();
			}

			db->Close();
		}
	}

	SendDamageToGame(Player);
}

void DamageShop::SendDamageToGame(rsPLAYINFO* Player)
{
	if (Player && Player->lpsmSock)
	{
		sDamagePlayerToGame packet;
		ZeroMemory(&packet, sizeof(packet));

		double PlayerDamage = getPlayerDamage(Player);

		packet.size = sizeof(sDamagePlayerToGame);
		packet.code = PACKET_SEND_CHARDAMAGE_GAME;
		packet.Damage = PlayerDamage;

		Player->lpsmSock->Send((char*)&packet, packet.size, TRUE);
	}
}