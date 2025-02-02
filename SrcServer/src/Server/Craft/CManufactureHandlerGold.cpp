#include "sinbaram/sinLinkHeader.h"
#include "..\\Database\SQLConnection.h"
#include "field.h"
#include "srcserver/onserver.h"
#include "Chat/ChatServer.h"

extern int srSetItemFromCode(psITEM* lpsItem, char* szCoed);

void CManufactureHandlerGold::SendOpenManufactureWindow(rsPLAYINFO* lpPlayInfo)
{
	if (!lpPlayInfo->lpsmSock)
		return;

	PacketOpenManufactureGoldWindow sPacket;
	sPacket.size = sizeof(sPacket);
	sPacket.code = smTRANSCODE_OPEN_MANUFACTUREGOLD;
	lpPlayInfo->lpsmSock->Send((char*)&sPacket, sPacket.size,TRUE);
}

void CManufactureHandlerGold::HandlePacket(rsPLAYINFO* lpPlayInfo, PacketGetRecipeGold* psPacket)
{
	auto db = SQLConnection::GetConnection(DATABASEID_ServerDB);


	PacketRecipeInfoGold sPacket = {};

	sPacket.code = smTRANSCODE_GET_RECIPEGOLD;
	sPacket.size = sizeof(sPacket);

	int index = 1;
	char ItemCode[12][32];


	psITEM psItem;

	if (db->Open())
	{
		if (db->Prepare("SELECT  Slot1, Slot2, Slot3, cItem1, cItem2, cItem3, cItem4, cItem5, cItem6, cItem7, cItem8, cItem9, cItem10, cItem11, Chance, Price FROM CraftGold Where RecipeCode = ?"))
		{
			db->BindInputParameter(psPacket->RecipeCode, 1, PARAMTYPE_String);

			if (db->Execute())
			{

				if (!sPacket.StoneCode[0][0])
				{
					for (int i = 0; i < 3; i++)
						db->GetData(sPacket.StoneCode[i], 32, i + index, PARAMTYPE_String);

					index += 3;

					for (int i = 0; i < 11; i++)
					{
						db->GetData(ItemCode[i], 32, i + index, PARAMTYPE_String);

						if (srSetItemFromCode(&psItem, ItemCode[i]))
						{
							memcpy(&sPacket.ResultItem[sPacket.Count++], &psItem.ItemInfo, sizeof(sITEMINFO));
						}
					}

					index += 11;

					db->GetData(&sPacket.Chance, 4, index, PARAMTYPE_Integer);

					index += 1;

					db->GetData(&sPacket.Price, 4, index, PARAMTYPE_Integer);

				}

			}
		}
		db->Close();
	}

	lpPlayInfo->lpsmSock->Send((char*)&sPacket, sPacket.size,TRUE);
}

extern int rsDeleteInvenItem(rsPLAYINFO* lpPlayInfo, DWORD dwCode, DWORD dwHead, DWORD dwChkSum);
extern int SendQuickMessage2(rsPLAYINFO* lpPlayInfo, char* szMessage);
extern int CreateCommandItem(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, char* szItem);
extern int spJobCode;
//extern int SendToAll(char* Buff, int Size);
int rsSendServerChatMessageToAllUser(int Color, char* msg, ...);
int rsSendServerChatAlertToAllUser(char* msg, ...);

extern int RecordLog1(const char* szLogName, const char* szMessage);
extern char* FormatString(char* fmt, ...);


void CManufactureHandlerGold::HandlePacket(rsPLAYINFO* lpPlayInfo, PacketManufactureRecipeGold* psPacket)
{
	auto db = SQLConnection::GetConnection(DATABASEID_ServerDB);

	bool bCanCreate = true;

	if (rsDeleteInvenItem(lpPlayInfo, psPacket->Recipe.Code, psPacket->Recipe.Head, psPacket->Recipe.ChkSum) >= 0)
	{
		for (int i = 0; i < 3; i++)
		{
			if (!psPacket->Runes[i].Code)
				break;

			if (rsDeleteInvenItem(lpPlayInfo, psPacket->Runes[i].Code, psPacket->Runes[i].Head, psPacket->Runes[i].ChkSum) < 0)
				bCanCreate = false;
		}
	}
	else
		bCanCreate = false;

	if (!bCanCreate)
	{
		SendQuickMessage2(lpPlayInfo, "Falha ao tentar o craft.");
		return;
	}

	char StoneCode[3][32] = {};
	char ItemCode[12][32] = {};
	int Chance = 0;
	int ChanceF = 0;
	int Price = 0;
	int Count = 0;

	int index = 1;

	vector<string> vItemCodes;

	if (db->Open())
	{
		if (db->Prepare("SELECT  Slot1, Slot2, Slot3, cItem1, cItem2, cItem3, cItem4, cItem5, cItem6, cItem7, cItem8, cItem9, cItem10, cItem11, Chance, Price FROM CraftGold Where RecipeCode = ?"))
		{
			db->BindInputParameter(psPacket->Recipe.ItemCode, 1, PARAMTYPE_String);

			if (db->Execute())
			{
				if (!StoneCode[0][0])
				{
					for (int i = 0; i < 3; i++)
					{
						db->GetData(StoneCode[i], 32, i + index, PARAMTYPE_String);
					}

					index += 3;

					for (int i = 0; i < 11; i++)
					{
						db->GetData(ItemCode[i], 32, i + index, PARAMTYPE_String);

						if (strlen(ItemCode[i]) > 2)
							vItemCodes.push_back(ItemCode[i]);
					}

					index += 11;

					db->GetData(&Chance, 4, index, PARAMTYPE_Integer);

					index += 1;

					db->GetData(&Price, 4, index, PARAMTYPE_Integer);


				}

			}
		}
		db->Close();
	}


	if (Price > lpPlayInfo->ServerMoney)
	{
		SendQuickMessage2(lpPlayInfo, "Você não tem gold suficiente.");
		return;
	}
	else
		lpPlayInfo->SubServerMoney(Price, 0);

	for (int i = 0; i < 3; i++)
	{
		if (lstrcmpi(StoneCode[i], psPacket->Runes[i].ItemCode))
		{
			SendQuickMessage2(lpPlayInfo, "Runas incorretas.");
			return;
		}
	}

	if (vItemCodes.size())
	{
		if (Chance < 100)
		{
			if ((rand() % 100) > Chance)
			{
				int rnd = (rand() % vItemCodes.size());
				char temp[32];
				lstrcpy(temp, vItemCodes[rnd].c_str());

				psITEM psItem;

				if (srSetItemFromCode(&psItem, temp))
				{
					SERVERCHAT->SendChatAllEx(EChatColor::CHATCOLOR_Global, "%s falhou ao tentar craftar %s!", lpPlayInfo->szName, psItem.ItemInfo.ItemName);

					RecordLog1("CraftItemOuro", FormatString("Player[%s] falhou no Craft de %s", lpPlayInfo->szName, psItem.ItemInfo.ItemName));
					return;
				}
			}
		}
	}

	if (vItemCodes.size())
	{

		int rnd = (rand() % vItemCodes.size());
		char temp[32];
		lstrcpy(temp, vItemCodes[rnd].c_str());

		CreateCommandItem(lpPlayInfo, lpPlayInfo, temp);

		psITEM psItem;
		if (srSetItemFromCode(&psItem, temp))
		{
			TRANS_CHATMESSAGE sPacket;
			sPacket.code = smTRANSCODE_WHISPERMESSAGE;
			wsprintf(sPacket.szMessage, "Craft Ouro: %s craftou com sucesso %s!", lpPlayInfo->szName, psItem.ItemInfo.ItemName);

			sPacket.size = lstrlen(sPacket.szMessage) + 32;
			//sPacket.dwIP = 3; 
			sPacket.dwIP = 3;
			sPacket.dwObjectSerial = 0;


			for (int cnt = 0; cnt < CONNECTMAX; cnt++)
			{
				if (rsPlayInfo[cnt].lpsmSock)
				{
					rsPlayInfo[cnt].lpsmSock->Send((char*)&sPacket, sPacket.size, TRUE);
				}
			}


			RecordLog1("CraftItemOuro", FormatString("[ %s ] craftou com sucesso [ %s ]!", lpPlayInfo->szName, psItem.ItemInfo.ItemName));
		}

		SERVERCHAT->SendChatAllEx(EChatColor::CHATCOLOR_Global, " Parabéns %s ao Craftar %s!", lpPlayInfo->szName, psItem.ItemInfo.ItemName);
	}
}

