
#include "sinbaram/sinLinkHeader.h"
#include "Alerta.h"
#include "Database/SQLConnection.h"

Alerta cAlertaHandler;

Alerta* Alerta::getInstance()
{
	return &cAlertaHandler;
}

void Alerta::Init()
{
	vEnabled.clear();
	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db->Open())
	{
		if (db->Prepare("SELECT ItemCode FROM DropLogAlert"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					char ItemCode[32];
					db->GetData(1, PARAMTYPE_String, ItemCode, 32);

					for (int cnt = 0; cnt < MAX_ITEM; cnt++)
					{
						if (!lstrcmpi(sItem[cnt].LastCategory, ItemCode))
						{
							vEnabled[sItem[cnt].CODE] = true;
							break;
						}
					}
				}
			}
		}
		db->Close();
	}
}

void Alerta::AddItemLog(rsPLAYINFO* lpPlayInfo, smCHAR* lpChar, sITEMINFO* lpItemInfo)
{
	if (!vEnabled[lpItemInfo->CODE])
		return;

	AlertaInfo sLog;
	sLog.Head = lpItemInfo->ItemHeader.Head;
	sLog.ChkSum = lpItemInfo->ItemHeader.dwChkSum;
	lstrcpy(sLog.ItemName, lpItemInfo->ItemName);
	vAlertaInfo.push_back(sLog);
}

#include <iostream>
using namespace std;

int rsSendMessageAll(char* szText, DWORD dwColor)
{

	TRANS_CHATMESSAGE	TransChatMessage;

	wsprintf(TransChatMessage.szMessage, "%s", szText);
	TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
	TransChatMessage.size = 32 + lstrlen(TransChatMessage.szMessage);
	TransChatMessage.dwIP = dwColor;
	TransChatMessage.dwObjectSerial = 0;

	for (int cnt = 0; cnt < CONNECTMAX; cnt++) {
		if (rsPlayInfo[cnt].lpsmSock)
			rsPlayInfo[cnt].lpsmSock->Send((char*)&TransChatMessage, TransChatMessage.size, TRUE);
	}

	return TRUE;
}

void Alerta::ProcessItemLog(char* PlayerName, DWORD Code, DWORD Head, DWORD ChkSum)
{
	if (!vEnabled[Code])
		return;

	for (auto it = vAlertaInfo.begin(); it != vAlertaInfo.end(); it++)
	{
		auto sLog = (*it);
		if (sLog.Head == Head && sLog.ChkSum == ChkSum)
		{

			char mensagem[70] = { 0 };
			wsprintf(mensagem, " [ %s ] Dropou [ %s ]. ", PlayerName, sLog.ItemName);
			rsSendMessageAll(mensagem, 17);

			vAlertaInfo.erase(it);
			break;
		}
	}
}
