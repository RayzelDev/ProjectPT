#pragma warning( disable : 4244 )

#include "sinbaram/sinLinkHeader.h"
#include "cSkinChanger.h"
#include "map"

bool isMousePos(int x, int y, int w, int h)
{
	if (pRealCursorPos.x > x && pRealCursorPos.x < x + w && pRealCursorPos.y > y && pRealCursorPos.y < y + h)
		return true;

	return false;
}

cSKINCHANGER cSkinChanger;


std::map<DWORD, DWORD> vSkinCodes = {
	//Armaduras//
	//Armaduras//
 { sinSK1 | sin01, sinDA1 | sin04  },
 { sinSK1 | sin02, sinDA1 | sin05  },
 { sinSK1 | sin03, sinDA1 | sin06  },
 { sinSK1 | sin04, sinDA1 | sin07  },
 { sinSK1 | sin05, sinDA1 | sin08  },
 { sinSK1 | sin06, sinDA1 | sin09  },
 { sinSK1 | sin07, sinDA1 | sin10  },
 { sinSK1 | sin08, sinDA1 | sin11  },

 //Linha Angelical//
 { sinSK1 | sin09, sinDS1 | sin07  },
 { sinSK1 | sin10, sinWA1 | sin07  }, 
 { sinSK1 | sin11, sinWA1 | sin08  },//ok
 { sinSK1 | sin12, sinWC1 | sin07  },//ok
 { sinSK1 | sin13, sinWH1 | sin07  },//ok
 { sinSK1 | sin14, sinWM1 | sin08  },//ok
 { sinSK1 | sin15, sinWP1 | sin07  },//ok
 { sinSK1 | sin16, sinWS1 | sin07  },//ok
 { sinSK1 | sin17, sinWS2 | sin09  },//ok
 { sinSK1 | sin18, sinWS2 | sin10  },//ok
 { sinSK1 | sin19, sinWT1 | sin07  },//ok

 //Linha Dark//
 { sinSK1 | sin20, sinDS1 | sin10  },//ok
 { sinSK1 | sin21, sinWA1 | sin09 },//ok
 { sinSK1 | sin22, sinWC1 | sin08 },//ok
 { sinSK1 | sin23, sinWH1 | sin08  },//ok
 { sinSK1 | sin24, sinWM1 | sin09  },//ok
 { sinSK1 | sin25, sinWP1 | sin08  },//ok
 { sinSK1 | sin26, sinWS1 | sin19  },//ok
 { sinSK1 | sin27, sinWS2 | sin13  },//ok
 { sinSK1 | sin28, sinWT1 | sin08  },//ok

 //Linha Dragon//
 { sinSK1 | sin29, sinDS1 | sin11  },//ok
 { sinSK1 | sin30, sinWA1 | sin10  },//ok
 { sinSK1 | sin31, sinWC1 | sin09  },//ok
 { sinSK1 | sin32, sinWH1 | sin10  },//ok
 { sinSK1 | sin33, sinWM1 | sin10  },//ok
 { sinSK1 | sin34, sinWP1 | sin09  },//ok
 { sinSK1 | sin35, sinWS1 | sin11  },//ok
 { sinSK1 | sin36, sinWS2 | sin18  },//ok
 { sinSK1 | sin37, sinWT1 | sin09  },//ok

 // Linha Negra Energy
 { sinSK1 | sin38, sinWA1 | sin13  },//ok
 { sinSK1 | sin39, sinWC1 | sin14  },//ok
 { sinSK1 | sin40, sinWH1 | sin14  },//ok
 { sinSK1 | sin41, sinWM1 | sin14  },//ok
 { sinSK1 | sin42, sinWP1 | sin13  },//ok
 { sinSK1 | sin43, sinWS1 | sin15  },//ok
 { sinSK1 | sin44, sinWS2 | sin16  },//ok
 { sinSK1 | sin45, sinWT1 | sin13  },//ok
 { sinSK1 | sin46, sinDS1 | sin12  },//ok

 // Linha HERODES
 { sinSK1 | sin47, sinWA1 | sin15  },//ok
 { sinSK1 | sin48, sinWC1 | sin15  },//ok
 { sinSK1 | sin49, sinWH1 | sin15  },//ok
 { sinSK1 | sin50, sinWM1 | sin12  },//ok
 { sinSK1 | sin51, sinWP1 | sin15  },//ok
 { sinSK1 | sin52, sinWS1 | sin16  },//ok
 { sinSK1 | sin53, sinWS2 | sin19  },//ok
 { sinSK1 | sin54, sinWT1 | sin15  },//ok
 { sinSK1 | sin55, sinDS1 | sin15  },//ok

 //Linha Godly
 { sinSK1 | sin56, sinWA1 | sin16  },//ok
 { sinSK1 | sin57, sinWC1 | sin16  },//ok
 { sinSK1 | sin58, sinWH1 | sin16  },//ok
 { sinSK1 | sin59, sinWM1 | sin17  },//ok
 { sinSK1 | sin60, sinWM1 | sin18  },//ok
 { sinSK1 | sin61, sinWS1 | sin12  },//ok
 { sinSK1 | sin62, sinWS2 | sin20  },//ok
 { sinSK1 | sin63, sinWT1 | sin10  },//ok
 { sinSK1 | sin64, sinDS1 | sin16  },//ok
 { sinSK1 | sin65, sinWP1 | sin15  },//ok
 { sinSK1 | sin66, sinWA1 | sin18  },//ok
 { sinSK1 | sin67, sinWS2 | sin06 },//ok

 //Linha Custom
  { sinSK1 | sin69, sinDS1 | sin05 },//ok
  { sinSK1 | sin70, sinDS1 | sin08 },//ok
  { sinSK1 | sin71, sinDS1 | sin09 },//ok
  { sinSK1 | sin72, sinDS1 | sin13 },//ok
  { sinSK1 | sin73, sinDS1 | sin14 },//ok
  { sinSK1 | sin74, sinWA1 | sin06 },//ok
  { sinSK1 | sin75, sinWA1 | sin12 },//ok
  { sinSK1 | sin76, sinWA1 | sin17 },//ok
  { sinSK1 | sin77, sinWC1 | sin10 },//ok
  { sinSK1 | sin78, sinWC1 | sin11 },//ok
  { sinSK1 | sin79, sinWC1 | sin12 },//ok
  { sinSK1 | sin80, sinWH1 | sin05 },//ok
  { sinSK1 | sin81, sinWH1 | sin06 },//ok
  { sinSK1 | sin82, sinWP1 | sin14 },//ok
  { sinSK1 | sin83, sinWP1 | sin16 },//ok
  { sinSK1 | sin84, sinWP1 | sin17 },//ok
  { sinSK1 | sin85, sinWS1 | sin14 },//ok
  { sinSK1 | sin86, sinWS1 | sin18 },//ok
  { sinSK1 | sin87, sinWS1 | sin23 },//ok
  { sinSK1 | sin88, sinWS2 | sin29 },//ok
  { sinSK1 | sin89, sinWS2 | sin32 },//ok
  { sinSK1 | sin90, sinWS2 | sin33 },//ok
  { sinSK1 | sin91, sinWT1 | sin11 },//ok
  { sinSK1 | sin92, sinWT1 | sin12 },//ok
  { sinSK1 | sin93, sinWT1 | sin14 },//ok
  { sinSK1 | sin94, sinWM1 | sin06 },//ok
  { sinSK1 | sin95, sinWM1 | sin16 },//ok
	 //Roupao
 { sinSK1 | sin96, sinDA2 | sin04 },//ok
 { sinSK1 | sin97, sinDA2 | sin05 },//ok
 { sinSK1 | sin98, sinDA2 | sin06 },//ok
 { sinSK2 | sin01, sinDA2 | sin07 },//ok
 { sinSK2 | sin02, sinDA2 | sin08 },//ok
 { sinSK2 | sin03, sinDA2 | sin09 },//ok
 { sinSK2 | sin04, sinDA2 | sin10 },//ok
 { sinSK2 | sin05, sinDA2 | sin11 },//ok

 //Skins Novas de Foice By Daniel
 { sinSK2 | sin10, sinWP1 | sin05 },//ok
 { sinSK2 | sin11, sinWP1 | sin06 },//ok
 { sinSK2 | sin12, sinWP1 | sin10 },//ok
 { sinSK2 | sin13, sinWP1 | sin11 },//ok
 { sinSK2 | sin14, sinWP1 | sin24 },//ok
 { sinSK2 | sin15, sinWP1 | sin25 },//ok
 { sinSK2 | sin16, sinWP1 | sin12 },//ok
 { sinSK2 | sin17, sinWP1 | sin21 },//ok
 { sinSK2 | sin18, sinWP1 | sin30 },//ok

 //Skins Novas de Machado By Daniel
 { sinSK2 | sin20, sinWA1 | sin19 },//ok 
 { sinSK2 | sin21, sinWA1 | sin20 },//ok
 { sinSK2 | sin22, sinWA1 | sin25 },//ok
 { sinSK2 | sin23, sinWA1 | sin26 },//ok
 { sinSK2 | sin24, sinWA1 | sin29 },//ok
 { sinSK2 | sin25, sinWA1 | sin30 },//ok
 { sinSK2 | sin26, sinWA1 | sin23 },//ok

 //Skins Novas de Garra By Daniel
 { sinSK2 | sin30, sinWC1 | sin18 },//ok 
 { sinSK2 | sin31, sinWC1 | sin19 },//ok
 { sinSK2 | sin32, sinWC1 | sin20 },//ok

 //Skins Novas de Arco By Daniel
 { sinSK2 | sin40, sinWS1 | sin05 },//ok 
 { sinSK2 | sin41, sinWS1 | sin06 },//ok
 { sinSK2 | sin42, sinWS1 | sin25 },//ok
 { sinSK2 | sin43, sinWS1 | sin26 },//ok
 { sinSK2 | sin44, sinWS1 | sin31 },//ok
 { sinSK2 | sin45, sinWS1 | sin32 },//ok

 //Skins Novas de Espada By Daniel
 { sinSK2 | sin50, sinWS2 | sin21 },//ok 
 { sinSK2 | sin51, sinWS2 | sin23 },//ok
 { sinSK2 | sin52, sinWS2 | sin24 },//ok

 //Skins Novas de Lança By Daniel
 { sinSK2 | sin60, sinWT1 | sin19 },//ok 
 { sinSK2 | sin61, sinWT1 | sin20 },//ok
 { sinSK2 | sin62, sinWT1 | sin21 },//ok
 { sinSK2 | sin63, sinWT1 | sin22 },//ok
 { sinSK2 | sin64, sinWT1 | sin23 },//ok
 { sinSK2 | sin65, sinWT1 | sin25 },//ok

 //Skins Novas de Escudo By Daniel
 { sinSK2 | sin70, sinDS1 | sin19 },//ok 
 { sinSK2 | sin71, sinDS1 | sin20 },//ok
 { sinSK2 | sin72, sinDS1 | sin21 },//ok
 { sinSK2 | sin73, sinDS1 | sin22 },//ok
 { sinSK2 | sin74, sinDS1 | sin23 },//ok
 { sinSK2 | sin75, sinDS1 | sin24 },//ok
 { sinSK2 | sin76, sinDS1 | sin04 },//ok
 { sinSK2 | sin77, sinDS1 | sin06 },//ok
 { sinSK2 | sin78, sinDS1 | sin17 },//ok

 //Skins Novas de Armaduras By Daniel
 { sinSK2 | sin80, sinDA1 | sin24 },//ok 
 { sinSK2 | sin81, sinDA1 | sin25 },//ok
 { sinSK2 | sin82, sinDA1 | sin27 },//ok
 { sinSK2 | sin83, sinDA1 | sin28 },//ok
 { sinSK2 | sin84, sinDA1 | sin30 },//ok






};

void cSKINCHANGER::Init()
{
	lpBackground = LoadDibSurfaceOffscreen("game\\Skins\\backgound.bmp");
	lpButton = LoadDibSurfaceOffscreen("game\\Skins\\Trocar.bmp");
}

void cSKINCHANGER::Draw()
{
	if (!OpenFlag)
		return;

	BaseX = 10;
	BaseY = 40;

	DrawSprite(BaseX, BaseY, lpBackground, 0, 0, 320, 208);

	if (isMousePos(BaseX + 5, BaseY + 166, 26, 26))
		DrawSprite(BaseX + 5, BaseY + 166, lpButton, 0, 0, 26, 26);

	if (scItem.Flag)
	{
		if (scItem.lpItem)
			DrawSprite(scItem.x, scItem.y, scItem.lpItem, 0, 0, scItem.w, scItem.h);
	}

	if (scStone.Flag)
	{
		if (scStone.lpItem)
			DrawSprite(scStone.x, scStone.y, scStone.lpItem, 0, 0, scStone.w, scStone.h);
	}

	//Posição sombra verde ou vermelha do item 
	if (isMousePos(BaseX + 40, BaseY + 58, 66, 88))
	{
		if (scItem.Flag)
		{
			cItem.ShowItemInfo(&scItem, 1);
			sinShowItemInfoFlag = 1;
		}

		if (MouseItem.Flag)
		{
			if (!SetSkinChangerItemAreaCheck(&MouseItem, 1))
				dsDrawColorBoxFloat(D3DCOLOR_RGBA(255, 0, 0, 128), (float)BaseX + 40, (float)BaseY + 74, 66, 88);
			else
				dsDrawColorBoxFloat(D3DCOLOR_RGBA(0, 255, 0, 128), (float)BaseX + 40, (float)BaseY + 74, 66, 88);
		}
	}

	//Posição da skins (Sombreado verde ou vermelho_)
	if (isMousePos(BaseX + 200, BaseY + 58, 66, 88))
	{
		if (scStone.Flag)
		{
			cItem.ShowItemInfo(&scStone, 1);
			sinShowItemInfoFlag = 1;
		}

		//Seleção da skin
		if (MouseItem.Flag)
		{
			if (!SetSkinChangerItemAreaCheck(&MouseItem, 2))
				dsDrawColorBoxFloat(D3DCOLOR_RGBA(255, 0, 0, 128), (float)BaseX + 200, (float)BaseY + 74, 66, 88);
			else
				dsDrawColorBoxFloat(D3DCOLOR_RGBA(0, 255, 0, 128), (float)BaseX + 200, (float)BaseY + 74, 66, 88);
		}
	}

	//if (scStone.Flag && scItem.Flag)
	//{
	//	auto it = vSkinCodes.find(scStone.CODE);

	//	if (it != vSkinCodes.end())
	//	{
	//		sITEM* pItem = GetItem(it->second);
	//		LPDIRECT3DTEXTURE9 lpItem;

	//		lpItem = GetItemImage(pItem);

	//		//Item visualizador
	//		if (lpItem)
	//		{
	//			DrawSprite(BaseX + 280 + ((66 - pItem->w) / 2), BaseY + 137 + ((88 - pItem->h) / 2), lpItem, 0, 0, pItem->w, pItem->h);
	//		}
	//	}
	//}
}

bool cSKINCHANGER::LButtonDown()
{
	if (!OpenFlag)
		return false;

	//Possivel posição do Click da troca
	if (isMousePos(BaseX + 150, BaseY + 85, 26, 26))
	{
		if (scItem.Flag && scStone.Flag)
		{
			SKINCHANGE_INFO sPacket;
			sPacket.size = sizeof(SKINCHANGE_INFO);
			sPacket.code = smTRANSCODE_SKINCHANGE;

			memcpy(&sPacket.scItem, &scItem, sizeof(sITEM));
			memcpy(&sPacket.scStone, &scStone, sizeof(sITEM));

			auto it = vSkinCodes.find(scStone.CODE);
			if (it != vSkinCodes.end())
				sPacket.newCode = it->second;

			smWsockServer->Send((char*)&sPacket, sPacket.size);

			scItem.Flag = false;
			scStone.Flag = false;
			Close();
		}
		return true;
	}

	//Click do item
	if (isMousePos(BaseX + 40, BaseY + 80, 66, 88))
	{
		if (MouseItem.Flag)
		{
			if (SetSkinChangerItemAreaCheck(&MouseItem, 1))
			{
				if (!scItem.Flag)
				{
					memcpy(&scItem, &MouseItem, sizeof(sITEM));
					ZeroMemory(&MouseItem, sizeof(sITEM));

					scItem.x = scItem.SetX = (BaseX + 40 + ((66 - scItem.w) / 2));
					scItem.y = scItem.SetY = (BaseY + 73 + ((88 - scItem.h) / 2));
				}
				else
				{
					sITEM backup;

					memcpy(&backup, &scItem, sizeof(sITEM));
					memcpy(&scItem, &MouseItem, sizeof(sITEM));
					memcpy(&MouseItem, &backup, sizeof(sITEM));

					scItem.x = scItem.SetX = (BaseX + 40 + ((66 - scItem.w) / 2));
					scItem.y = scItem.SetY = (BaseY + 73 + ((88 - scItem.h) / 2));
				}
			}
		}
		else
		{
			if (scItem.Flag)
			{
				memcpy(&MouseItem, &scItem, sizeof(sITEM));
				ZeroMemory(&scItem, sizeof(sITEM));
			}
		}
		return true;
	}

	//CLique da skin
	if (isMousePos(BaseX + 200, BaseY + 58, 66, 88))
	{
		if (MouseItem.Flag)
		{
			if (SetSkinChangerItemAreaCheck(&MouseItem, 2))
			{
				if (!scStone.Flag)
				{
					memcpy(&scStone, &MouseItem, sizeof(sITEM));
					ZeroMemory(&MouseItem, sizeof(sITEM));

					scStone.x = scStone.SetX = (BaseX + 200 + ((66 - scStone.w) / 2));
					scStone.y = scStone.SetY = (BaseY + 58 + ((88 - scStone.h) / 2));
				}
				else
				{
					sITEM backup;

					memcpy(&backup, &scStone, sizeof(sITEM));
					memcpy(&scStone, &MouseItem, sizeof(sITEM));
					memcpy(&MouseItem, &backup, sizeof(sITEM));

					scStone.x = scStone.SetX = (BaseX + 200 + ((22 - scStone.w) / 2));
					scStone.y = scStone.SetY = (BaseY + 58 + ((22 - scStone.h) / 2));
				}
			}
		}
		else
		{
			if (scStone.Flag)
			{
				memcpy(&MouseItem, &scStone, sizeof(sITEM));
				ZeroMemory(&scStone, sizeof(sITEM));
			}
		}
		return true;
	}

	if (isMousePos(BaseX + 285, BaseY + 170, 23, 21))
	{
		Close();
		return true;
	}

	//não deixa jogar o item no chão
	if (isMousePos(BaseX, BaseY, 352, 362))
		return true;

	return false;
}

void cSKINCHANGER::Open()
{
	OpenFlag = true;
}

void cSKINCHANGER::Close()
{
	if (scItem.Flag || scStone.Flag)
	{
		cMessageBox.ShowMessageEvent("Retira os Itens Para fechar o Npc!");
		return;
	}

	if (scItem.Flag)
	{
		scItem.ItemPosition = 0;
		cInvenTory.LastSetInvenItem(&scItem, 1);
	}
	if (scStone.Flag)
	{
		scStone.ItemPosition = 0;
		cInvenTory.LastSetInvenItem(&scStone, 1);
	}

	OpenFlag = false;
}

bool cSKINCHANGER::SetSkinChangerItemAreaCheck(sITEM* pItem, int Pos)
{
	if (Pos == 1)
	{
		if ((pItem->CODE & sinITEM_MASK2) == sinSK1 && (pItem->CODE & sinITEM_MASK2) == sinSK2)
			return false;

		if ((pItem->CODE & sinITEM_MASK2) != sinWA1 && (pItem->CODE & sinITEM_MASK2) != sinWC1 && (pItem->CODE & sinITEM_MASK2) != sinWH1 && (pItem->CODE & sinITEM_MASK2) != sinWM1 && (pItem->CODE & sinITEM_MASK2) != sinWP1 && (pItem->CODE & sinITEM_MASK2) &&
			(pItem->CODE & sinITEM_MASK2) != sinWS1 && (pItem->CODE & sinITEM_MASK2) != sinWS2 && (pItem->CODE & sinITEM_MASK2) != sinWT1 && (pItem->CODE & sinITEM_MASK2) != sinDS1 && (pItem->CODE & sinITEM_MASK2) != sinDA1 && (pItem->CODE & sinITEM_MASK2) != sinDA2 /*&& (pItem->CODE & sinITEM_MASK2) != sinDA3 && (pItem->CODE & sinITEM_MASK2) != sinDA4*/)
			return false;

		if (scStone.Flag)
		{
			if (!isCompatible(pItem, &scStone))
				return false;
		}
	}
	else if (Pos == 2)
	{
		if ((pItem->CODE & sinITEM_MASK2) != sinSK1 && (pItem->CODE & sinITEM_MASK2) != sinSK2)
			return false;

		if (scItem.Flag)
		{
			if (!isCompatible(&scItem, pItem))
				return false;
		}
	}

	return true;
}

bool cSKINCHANGER::isCompatible(sITEM* pItem, sITEM* pStone)
{
	auto it = vSkinCodes.find(pStone->CODE);

	if (it != vSkinCodes.end())
	{
		for (int i = 0; i < MAX_ITEM; i++)
		{
			if (sItem[i].CODE == it->second)
			{
				if ((pItem->CODE & sinITEM_MASK2) == sinWA1 && (sItem[i].CODE & sinITEM_MASK2) == sinWA1 && pItem->Class == sItem[i].Class ||
					(pItem->CODE & sinITEM_MASK2) == sinWC1 && (sItem[i].CODE & sinITEM_MASK2) == sinWC1 && pItem->Class == sItem[i].Class ||
					(pItem->CODE & sinITEM_MASK2) == sinWH1 && (sItem[i].CODE & sinITEM_MASK2) == sinWH1 && pItem->Class == sItem[i].Class ||
					(pItem->CODE & sinITEM_MASK2) == sinWM1 && (sItem[i].CODE & sinITEM_MASK2) == sinWM1 && pItem->Class == sItem[i].Class ||
					(pItem->CODE & sinITEM_MASK2) == sinWP1 && (sItem[i].CODE & sinITEM_MASK2) == sinWP1 && pItem->Class == sItem[i].Class ||
					(pItem->CODE & sinITEM_MASK2) == sinWS1 && (sItem[i].CODE & sinITEM_MASK2) == sinWS1 && pItem->Class == sItem[i].Class ||
					(pItem->CODE & sinITEM_MASK2) == sinWS2 && (sItem[i].CODE & sinITEM_MASK2) == sinWS2 && pItem->Class == sItem[i].Class ||
					(pItem->CODE & sinITEM_MASK2) == sinWT1 && (sItem[i].CODE & sinITEM_MASK2) == sinWT1 && pItem->Class == sItem[i].Class ||
					(pItem->CODE & sinITEM_MASK2) == sinDS1 && (sItem[i].CODE & sinITEM_MASK2) == sinDS1 && pItem->Class == sItem[i].Class ||
					(pItem->CODE & sinITEM_MASK2) == sinDA1 && (sItem[i].CODE & sinITEM_MASK2) == sinDA1 || /*(sItem[i].CODE & sinITEM_MASK2) == sinDA3 && pItem->Class == sItem[i].Class ||*/
					/*(pItem->CODE & sinITEM_MASK2) == sinDA3 && (sItem[i].CODE & sinITEM_MASK2) == sinDA3 ||*/ (sItem[i].CODE & sinITEM_MASK2) == sinDA1 && pItem->Class == sItem[i].Class ||
					/*(pItem->CODE & sinITEM_MASK2) == sinDA4 &&*/ /*(sItem[i].CODE & sinITEM_MASK2) == sinDA4 || */(sItem[i].CODE & sinITEM_MASK2) == sinDA2 && pItem->Class == sItem[i].Class ||
					(pItem->CODE & sinITEM_MASK2) == sinDA2 && (sItem[i].CODE & sinITEM_MASK2) == sinDA2 /*|| (sItem[i].CODE & sinITEM_MASK2) == sinDA4 && pItem->Class == sItem[i].Class*/)
					return true;

				/*if ((pItem->CODE & sinITEM_MASK2) == (sItem[i].CODE & sinITEM_MASK2) && pItem->Class == sItem[i].Class)
					return true;*/
			}
		}
	}

	return false;
}

LPDIRECT3DTEXTURE9 cSKINCHANGER::GetItemImage(sITEM* pItem)
{
	char szFilePath[255];

	if (pItem)
	{
		if (pItem->lpTempItem)
			return pItem->lpTempItem;
		wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", pItem->ItemFilePath, pItem->LastCategory);

		pItem->lpTempItem = LoadDibSurfaceOffscreen(szFilePath);
		return pItem->lpTempItem;
	}

	return nullptr;
}

sITEM* cSKINCHANGER::GetItem(DWORD CODE)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (sItem[i].CODE == CODE)
		{
			return &sItem[i];
		}
	}
	return nullptr;
}
