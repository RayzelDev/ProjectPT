
#include "sinbaram/sinLinkHeader.h"
#include "Database\\SQLConnection.h"
#include "cReviewSkinHandler.h"
#include "srcServer/onserver.h"

extern int srSetItemFromCode(psITEM* lpsItem, char* szCoed);

void cReviewSkinHandler::SendOpenReviewSkin(rsPLAYINFO* lpPlayInfo)
{
	if (!lpPlayInfo->lpsmSock)
		return;

	PacketReviewSkin sPacket;
	sPacket.size = sizeof(sPacket);
	sPacket.code = OPCODE_OPEN_REVIEWSKIN;
	lpPlayInfo->lpsmSock->Send((char*)&sPacket, sPacket.size, TRUE);
}

void cReviewSkinHandler::HandlePacket(rsPLAYINFO* lpPlayInfo, PacketReviewSkin* psPacket)
{
	if (!lpPlayInfo || !lpPlayInfo->lpsmSock)
		return;

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);
	PacketReviewSkin sPacket{};
	sPacket.code = OPCODE_GET_REVIEWSKIN;
	sPacket.size = sizeof(sPacket);
	sPacket.Count = 0;
	sPacket.bFirst = true;

	psITEM psItem;
	char ItemCode[32];

	if (db->Open())
	{
		if (db->Prepare("Select * From ReviewSkin Where SubTag = ?"))
		{
			db->BindInputParameter(&psPacket->Sections, 1, PARAMTYPE_Integer);

			if (db->Execute())
			{
				while (db->NextRow())
				{
					db->GetData(ItemCode, 32, 1, PARAMTYPE_String);
					db->GetData(&sPacket.InfoReview[sPacket.Count].Hand, 4, 2, PARAMTYPE_Integer);
					db->GetData(&sPacket.InfoReview[sPacket.Count].Section, 4, 3, PARAMTYPE_Integer);

					if (srSetItemFromCode(&psItem, ItemCode))
					{
						memcpy(&sPacket.InfoReview[sPacket.Count++].ResultItem, &psItem.ItemInfo, sizeof(sITEMINFO));
						sPacket.Count++;

						if (sPacket.Count >= 10)
						{
							lpPlayInfo->lpsmSock->Send((char*)&sPacket, sPacket.size, TRUE);
							ZeroMemory(sPacket.InfoReview, sizeof(sPacket.InfoReview));
							sPacket.Count = 0;
							sPacket.bFirst = false;
						}
					}
				}
			}
		}

		db->Close();
	}

	lpPlayInfo->lpsmSock->Send((char*)&sPacket, sPacket.size, TRUE);
}