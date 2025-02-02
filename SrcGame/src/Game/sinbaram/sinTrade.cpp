#include "sinLinkHeader.h"
#include "Utils\\Geometry.h"
#define AGING_MONEY_FREE

cTRADE cTrade;
sTRADE sTrade;
sTRADE sTradeRecv;

extern sITEM TempPerfectItem2;
extern BOOL bIsPerfect2;

cMIXTURERESET cMixtureReset;
sMIXTURE_RESET_ITEM	sMixtureResetItem;
sMIXTURE_RESET_ITEM sMixtureResetItemBackUp;
int	MixtureResetCheckFlag = 0;
sMIXTURE_RESET_ITEM_SERVER	sMixtureReset_Send;
sMIXTURE_RESET_ITEM_SERVER	sMixtureReset_Recv;

cAGING	cAging;
sAGINGITEM sAgingItem;
sAGINGITEM sAgingItemBackUp;


sCRAFTITEM_SERVER sAging_Send;
sCRAFTITEM_SERVER sAging_Recv;

cWAREHOUSE cWareHouse;
sWAREHOUSE sWareHouse;

Caravana cCaravan;
sCARAVAN sCaravan;

int WareHouseSaveFlag = 0;
int CaravanSaveFlag = 0;

int CloseWareHouseCheckFlag2 = 0;
int CloseWareHouseCheckFlag3 = 0;

cCRAFTITEM cCraftItem;
sCRAFTITEM sCraftItem;
sCRAFTITEM sCraftItemBackUp;

CSmeltingItem SmeltingItem;
SSmeltingItem S_smeltingItem;
SSmeltingItem sSmeltingItemBackUp;
sSMELTINGITEM_SERVER sSmeltingItem_Send;
sSMELTINGITEM_SERVER sSmeltingItem_Recv;

CManufacture ManufactureItem;
SManufactureItem g_sManufactureItem;
SManufactureItem g_sManufactureItemBackUp;
SManufactureItem_Server g_sManufactureItem_Send;
SManufactureItem_Server g_sManufactureItem_Recv;
SManufacture_Rune g_sManufacture_Rune[MAX_RECIPE_KIND];
SManufacture_ResultItemInfo g_Manufacture_ItemInfo[MAX_HANDMADEITEM];

sCRAFTITEM_SERVER sCraftItem_Send;
sCRAFTITEM_SERVER sCraftItem_Recv;

sCRAFTITEM_SERVER sWingItem_Send;
sCRAFTITEM_SERVER sWingItem_Recv;

int MixItemNoCopyFlag = 0;


int CraftItemButtonIndex = 0;
int CraftItemSendServerIndex = 0;
int CheckMixItemCopyFlag = 0;

int ShowItemCraftMessageFlag = 0;
int ShowItemAgingMessageFlag = 0;
int ShowItemSmeltingMessageFlag = 0;
int ShowItemManufactureMessageFlag = 0;
int ShowItemOverSpace = 0;

char szAgingItemBuff[128];
char szCraftItemBuff[128];
POINT CraftItemMessageBoxPosi = { 0,0 };
POINT CraftItemMessageSize = { 0,0 };


sITEM* BackUpInvenItem2;
sITEM* BackUpInvenTempItem2;
sINVENTORY* BackUpsInven;
int   InvenGold = 0;
int   RealGold = 0;

int TradeCheckFlag = 0;
DWORD SpecialSheltomCode = 0;
int CopyItemIndex7[10] = { 0,0,0,0,0,0,0,0,0,0 };

int TradeColorIndex = 0;
int	TradeCrashItemIndex[2] = { 0,0 };
RECT TradeColorRect = { 0,0,0,0 };
int  SelectTradeItemIndex = 0;
int RequestTradeButtonFlag = 0;

int ItemKindFlag = 0;

int CheckDelayFlag = 0;
int sinCancelFlag = 0;

int TempShowMoney = 0;
int TempShowCoin = 0;

int CloseWareHouseFlag = 0;
int CloseCaravanFlag = 0;
int CaravanFuncFlag = 0;

int ItemPickUpFlag = 0;

sPOSTBOX_ITEM sPostbox_Item[50];
int PostBoxIndexCnt = 0;
DWORD ExpressItemCode = 0;
char  szExpressItemName[64];


char* PostBoxDocFilePath[] = {
	"image\\Sinimage\\shopall\\PostBox.sin",
};

sRECON_ITEM sReconItem[MAX_SEEL_COUNT] =
{
	//CODE         level    count price
	{ (sinSE1 | sin01), { 0, 80 }, 1, 70000 },
	{ (sinSE1 | sin02), { 0, 0 }, 1, 50000 },
	{ (sinSE1 | sin03), { 0, 0 }, 1, 2000 },
	{ (sinSE1 | sin04), { 81, 130 }, 1, 100000 },
	{ (sinSE1 | sin05), { 0, 134 }, 1, 10000 },
	{ (sinSE1 | sin06), { 0, 134 }, 1, 10000 },
	{ (sinSE1 | sin07), { 0, 134 }, 1, 10000 },
	{ (sinSE1 | sin08), { 0, 134 }, 1, 10000 },
	{ (sinSE1 | sin09), { 0, 134 }, 1, 10000 },
	{ (sinSE1 | sin10), { 0, 134 }, 1, 10000 },
	{ (sinSE1 | sin11), { 0, 134 }, 1, 10000 },
	{ (sinSE1 | sin12), { 0, 134 }, 1, 10000 }
};

#define T_REVISION_Y			22*5

#define TRADEBUTTONMAIN_X		513
#define TRADEBUTTONMAIN_Y		3

#include "..\\nettype.hpp"

sCRAFTITEM_INFO sCraftItem_Info[MAX_CRAFTITEM_INFO] = {

#ifdef _W_SERVER
#include "sinCraftItem.h"
#else
#include "_sinCraftItem.h"
#endif
};

sMAKEITEM_INFO sMakeItem_Info[MAX_MAKEITEM_INFO] = {
	#include "sinMakeItem.h"
};

DWORD sinTradeColor = D3DCOLOR_RGBA(0, 200, 255, 80);

int UpTradePosi[9][4] = {
	{ 21, 136 + sinInterHeight2, 21 + (22 * 9), 136 + (22 * 4) + sinInterHeight2 },
	{ 224, 90, 300, 108 },
	{ 248, 124, 324, 142 },
	{ 248, 174, 298, 192 },
	{ 300, 217 + sinInterHeight2, 300 + 26, 217 + 26 + sinInterHeight2 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 248, 150, 324, 176 },
};

int DownTradePosi[9][4] = {
	{ 21, 224 + 21 + sinInterHeight2, 21 + (22 * 9), 224 + 21 + (22 * 4) + sinInterHeight2 },
	{ 225, 205, 250, 226 },
	{ 248, 212, 324, 230 },
	{ 229, 262, 298, 280 },
	{ 300, 305 + sinInterHeight2, 300 + 26, 305 + 26 + sinInterHeight2 },
	{ 305, 314 + sinInterHeight2, 304 + 20, 314 + 20 + sinInterHeight2 },
	{ 311, 136 + sinInterHeight2, 310 + 20, 136 + 20 + sinInterHeight2 },
	{ 225, 231, 250, 252 },
	{ 248, 238, 324, 256 },
};

int TradeStartX = DownTradePosi[T_SET_BOX][0], TradeStartY = DownTradePosi[T_SET_BOX][1], TradeEndX = DownTradePosi[T_SET_BOX][2], TradeEndY = DownTradePosi[T_SET_BOX][3];

char szAgingMsgBuff[128];
char* SheltomName[MAX_SHELTOM] = { "Lucidy", "Fadeo", "Sparki", "Raident", "Transparo", "Murki", "Devine", "Celesto", "Miraje", "�Inferna", "Enigma", "Bellum", "Ordo", "Sapphire" };

int BaseX = (WinSizeX - 260) / 2;
int BaseY = (WinSizeY - 161) / 2;

// Aging System
int AgingLevelAttack[MAX_AGING] = { 200,400,800,1500,3000,5000,10000,15000,25000,50000,10,100,10,10,10,10,10,10,10,10 };

int AgingLevelCritical[MAX_AGING] = { 40,80,160,300,600,1000,2000,3000,5000,10000,10,100,10,10,10,10,10,10,10,10 };

int AgingLevelBlock[MAX_AGING] = { 40,80,160,300,600,1000,2000,3000,5000,10000,10,100,10,10,10,10,10,10,10,10 };

int AgingLevelHit[MAX_AGING] = { 100,200,300,750,1500,2500,5000,7500,12500,25000,10,100,10,10,10,10,10,10,10,10 };

//Falha do aging
int AgingOkPercent[MAX_AGING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 18, 28, 36, 44, 52, 58, 67, 75, 82, 88 };
int AgingStoneAgingOkPercent[MAX_AGING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 12, 20, 28, 36, 44, 52, 60, 68 };

int SuperAgingStoneAgingFailPercent[MAX_AGING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 100};
int SuperAgingStone1_5AgingFailPercent[MAX_AGING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 100};

int chanceQuebra[MAX_AGING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44 };

int AgingLevelSheltom[MAX_AGING][12] = {
	{9,9,0,0,0,0,0,0,0,0,0,0},
	{9,9,0,0,0,0,0,0,0,0,0,0},
	{9,9,0,0,0,0,0,0,0,0,0,0},
	{9,9,0,0,0,0,0,0,0,0,0,0},
	{9,9,0,0,0,0,0,0,0,0,0,0},
	{9,9,0,0,0,0,0,0,0,0,0,0},
	{9,9,9,9,0,0,0,0,0,0,0,0},
	{9,9,9,9,0,0,0,0,0,0,0,0},
	{9,9,9,9,0,0,0,0,0,0,0,0},
	{9,9,9,9,0,0,0,0,0,0,0,0},   

	{9,9,9,9,0,0,0,0,0,0,0,0},
	{9,9,9,9,0,0,0,0,0,0,0,0},
	{9,9,9,9,0,0,0,0,0,0,0,0},
	{9,9,9,9,9,0,0,0,0,0,0,0},
	{9,9,9,9,9,0,0,0,0,0,0,0},
	{9,9,9,9,9,0,0,0,0,0,0,0},

	{9,9,9,9,9,0,0,0,0,0,0,0},
	{9,9,9,9,9,0,0,0,0,0,0,0},
	{9,9,9,9,9,9,0,0,0,0,0,0},
	{9,9,9,9,9,9,0,0,0,0,0,0},
};


int AgingLevelSheltomIndex[4][3] = {
	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0
};


int AgingCheckSheltomFlag = 0;
int AgingSheltomCnt2 = 0;


#define SIN_AGING_MASK	0x00101011

// Custo do aging
int AgingLevelMoney[MAX_AGING] = { 1000 ^ SIN_AGING_MASK,2000 ^ SIN_AGING_MASK,3000 ^ SIN_AGING_MASK,4000 ^ SIN_AGING_MASK,5000 ^ SIN_AGING_MASK,
							6000 ^ SIN_AGING_MASK,7000 ^ SIN_AGING_MASK,8000 ^ SIN_AGING_MASK,9000 ^ SIN_AGING_MASK,10000 ^ SIN_AGING_MASK,
							11000 ^ SIN_AGING_MASK,12000 ^ SIN_AGING_MASK,13000 ^ SIN_AGING_MASK,14000 ^ SIN_AGING_MASK,15000 ^ SIN_AGING_MASK,
							16000 ^ SIN_AGING_MASK,17000 ^ SIN_AGING_MASK,18000 ^ SIN_AGING_MASK,19000 ^ SIN_AGING_MASK,20000 ^ SIN_AGING_MASK };

POINT AgingLevelSheltomXY[4][3] = {
	{{218,193 + sinInterHeight2}			,{218 + 22,193 + sinInterHeight2}		  ,{218 + 22 + 22,193 + sinInterHeight2}},
	{{218,193 + 22 + sinInterHeight2}		,{218 + 22,193 + 22 + sinInterHeight2}	  ,{218 + 22 + 22,193 + 22 + sinInterHeight2}},
	{{218,193 + 22 + 22 + sinInterHeight2}	,{218 + 22,193 + 22 + 22 + sinInterHeight2}   ,{218 + 22 + 22,193 + 22 + 22 + sinInterHeight2}},
	{{218,193 + 22 + 22 + 22 + sinInterHeight2} ,{218 + 22,193 + 22 + 22 + 22 + sinInterHeight2},{218 + 22 + 22,193 + 22 + 22 + 22 + sinInterHeight2}}

};

// Force Orb
int ForceItemPrice2 = 0;
LPDIRECT3DTEXTURE9	lpForceMain;
LPDIRECT3DTEXTURE9	lpForceButtonInfo;
LPDIRECT3DTEXTURE9	lpForceTitle;

cTRADE::cTRADE()
{

}
cTRADE::~cTRADE()
{

}

void cTRADE::Init()
{
	memset(&sCraftItem, 0, sizeof(sCRAFTITEM));
	memset(&sCraftItem_Recv, 0, sizeof(sCRAFTITEM_SERVER));

	MatTradebuttonMain = CreateTextureMaterial("game\\images\\messagebox\\background_normal.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

	LpButtonNo = CreateTextureMaterial("game\\images\\messagebox\\btnno.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	LpButtonNo_ = CreateTextureMaterial("game\\images\\messagebox\\btnno_.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

	LpButtonYes = CreateTextureMaterial("game\\images\\messagebox\\btnyes.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	LpButtonYes_ = CreateTextureMaterial("game\\images\\messagebox\\btnyes_.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

	sWareHouse.Money += 2023;
	sWareHouse.Weight[0] += 197;
	sWareHouse.Weight[1] += 196;

	ManufactureItem.RuneIndexInit();
	Load();

}

void cTRADE::Load()
{
	char szTemp[256];

	lpCheck = LoadDibSurfaceOffscreen("Image\\SinImage\\shopall\\Trade_Check.bmp");
	lpCheckInfo = LoadDibSurfaceOffscreen("Image\\SinImage\\shopall\\Trade_Check_Info.bmp");
	lpDelayCheck = LoadDibSurfaceOffscreen("Image\\SinImage\\shopall\\Trade_Check_Delay.bmp");

	lpbuttonOk = LoadDibSurfaceOffscreen("Image\\SinImage\\shopall\\TradeButton\\ButtonOk.bmp");

	lpbuttonCancel = LoadDibSurfaceOffscreen("Image\\SinImage\\shopall\\Trade_Cancel.bmp");
	lpbuttonCancelInfo = LoadDibSurfaceOffscreen("Image\\SinImage\\shopall\\Trade_CancelInfo.bmp");

	lpCheck_Glay = LoadDibSurfaceOffscreen("Image\\SinImage\\shopall\\Trade_Check_Glay.bmp");
	lpCenterBox = LoadDibSurfaceOffscreen("Image\\SinImage\\shopall\\Trade_0.bmp");

	for (int i = 0; i < MAX_SHELTOM; i++) {
		wsprintf(szTemp, "Image\\SinImage\\shopall\\Aging\\itos%d_.bmp", 100 + i + 1);
		cAging.lpGraySheltom[i] = LoadDibSurfaceOffscreen(szTemp);
	}
}

void cTRADE::Release()
{
	if (lpCheck) {
		lpCheck->Release();
		lpCheck = 0;
	}
	if (lpCheckInfo) {
		lpCheckInfo->Release();
		lpCheckInfo = 0;

	}
	if (lpDelayCheck) {
		lpDelayCheck->Release();
		lpDelayCheck = 0;

	}
	if (lpbuttonOk) {
		lpbuttonOk->Release();
		lpbuttonOk = 0;
	}
	if (lpbuttonCancel) {
		lpbuttonCancel->Release();
		lpbuttonCancel = 0;

	}
	if (lpbuttonCancelInfo) {
		lpbuttonCancelInfo->Release();
		lpbuttonCancelInfo = 0;

	}
	if (lpCheck_Glay) {
		lpCheck_Glay->Release();
		lpCheck_Glay = 0;
	}

}

DWORD TradeSpotTimer = 0;

void cTRADE::DrawImage(int Mat, int x, int y, int w, int h)
{
	HoDrawTexImage(Mat, x, y, w, h, w, h, 255);
}

void cTRADE::Draw()
{
	sinInterHeight2 = -50;

	int i = 0, j = 0;

	DrawSprite(0 - (256 + 128 - sinMoveKindInter[SIN_WAREHOUSE]), 90 + sinInterHeight2, cShop.lpShopMain, 0, 0, 344, 264);

	if (chaPremiumitem.m_CaravanArma > 0 && chaPremiumitem.m_CaravanArma > chaPremiumitem.m_CaravanBuma && chaPremiumitem.m_CaravanArma > chaPremiumitem.m_CaravanHopy)
		DrawSprite(0 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 90 + sinInterHeight2, cShop.MainCaravan_Arma, 0, 0, 344, 264);

	if (chaPremiumitem.m_CaravanHopy > 0 && chaPremiumitem.m_CaravanHopy > chaPremiumitem.m_CaravanBuma && chaPremiumitem.m_CaravanHopy > chaPremiumitem.m_CaravanArma)
		DrawSprite(0 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 90 + sinInterHeight2, cShop.MainCaravan_Hopy, 0, 0, 344, 264);

	if (chaPremiumitem.m_CaravanBuma > 0 && chaPremiumitem.m_CaravanBuma > chaPremiumitem.m_CaravanHopy && chaPremiumitem.m_CaravanBuma > chaPremiumitem.m_CaravanArma)
		DrawSprite(0 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 90 + sinInterHeight2, cShop.MainCaravan_Buma, 0, 0, 344, 264);

	DrawSprite(0 - (256 + 128 - sinMoveKindInter[SIN_TRADE]), 90 + sinInterHeight2, cShop.lpShopMain, 0, 0, 344, 264);
	DrawSprite(225 - (256 + 128 - sinMoveKindInter[SIN_TRADE]), 47, cShop.lpTitle_Trade, 0, 0, 111, 244);

	DrawSprite(225 - (256 + 128 - sinMoveKindInter[SIN_TRADE]), 47, cShop.lpTitle_T, 0, 0, 111, 32);

	DrawSprite(0 - (256 + 128 - sinMoveKindInter[SIN_CRAFTITEM]), 90 + sinInterHeight2, cShop.lpShopMain, 0, 0, 344, 264);
	DrawSprite(225 - (256 + 128 - sinMoveKindInter[SIN_CRAFTITEM]), 47, cShop.lpTitle_CraftItem, 0, 0, 111, 32);
	DrawSprite(18 - (256 + 128 - sinMoveKindInter[SIN_CRAFTITEM]), 135 + sinInterHeight2, cShop.lpCraftItemMain, 0, 0, 320, 208);

	DrawSprite(200 - (256 + 128 - sinMoveKindInter[SIN_CRAFTITEM]), 314 + sinInterHeight2, cShop.lpGoldEdit, 0, 0, 98, 21);

	if (cCraftItem.ForceFlag) {
		if (!lpForceMain) {
			lpForceMain = LoadDibSurfaceOffscreen("Image\\SinImage\\shopall\\shop_alchemy.bmp");
			lpForceButtonInfo = LoadDibSurfaceOffscreen("Image\\SinImage\\shopall\\Bt_alchemy.bmp");
			lpForceTitle = LoadDibSurfaceOffscreen("Image\\SinImage\\shopall\\title_alchemy.bmp");
		}
		DrawSprite(18 - (256 + 128 - sinMoveKindInter[SIN_CRAFTITEM]), 135 + sinInterHeight2, lpForceMain, 0, 0, 217, 208);
		DrawSprite(225 - (256 + 128 - sinMoveKindInter[SIN_CRAFTITEM]), 47, lpForceTitle, 0, 0, 111, 32);
	}

	DrawSprite(0 - (256 + 128 - sinMoveKindInter[SIN_AGING]), 90 + sinInterHeight2, cShop.lpShopMain, 0, 0, 344, 264);
	DrawSprite(225 - (256 + 128 - sinMoveKindInter[SIN_AGING]), 47, cShop.lpTitle_Aging, 0, 0, 111, 32);
	DrawSprite(18 - (256 + 128 - sinMoveKindInter[SIN_AGING]), 135 + sinInterHeight2, cShop.lpCraftItemMain, 0, 0, 320, 208);
	DrawSprite(200 - (256 + 128 - sinMoveKindInter[SIN_AGING]), 314 + sinInterHeight2, cShop.lpGoldEdit, 0, 0, 98, 21);

	DrawSprite(0 - (256 + 128 - sinMoveKindInter[SIN_SMELTING]), 90 + sinInterHeight2, cShop.lpShopMain, 0, 0, 344, 264);
	DrawSprite(225 - (256 + 128 - sinMoveKindInter[SIN_SMELTING]), 47, cShop.lpTitle_SmeltingItem, 0, 0, 111, 32);
	DrawSprite(18 - (256 + 128 - sinMoveKindInter[SIN_SMELTING]), 135 + sinInterHeight2, cShop.lpSmeltingItemMain, 0, 0, 320, 208);
	DrawSprite(200 - (256 + 128 - sinMoveKindInter[SIN_SMELTING]), 314 + sinInterHeight2, cShop.lpGoldEdit, 0, 0, 98, 21);

	DrawSprite(0 - (256 + 128 - sinMoveKindInter[SIN_MANUFACTURE]), 90 + sinInterHeight2, cShop.lpShopMain, 0, 0, 344, 264);
	DrawSprite(225 - (256 + 128 - sinMoveKindInter[SIN_MANUFACTURE]), 47, cShop.lpTitle_ManufactureItem, 0, 0, 111, 32);
	DrawSprite(18 - (256 + 128 - sinMoveKindInter[SIN_MANUFACTURE]), 135 + sinInterHeight2, cShop.lpManufactureItemMain, 0, 0, 320, 208);
	DrawSprite(200 - (256 + 128 - sinMoveKindInter[SIN_MANUFACTURE]), 314 + sinInterHeight2, cShop.lpGoldEdit, 0, 0, 98, 21);

	DrawSprite(0 - (256 + 128 - sinMoveKindInter[SIN_MIXTURE_RESET]), 90 + sinInterHeight2, cShop.lpShopMain, 0, 0, 344, 264);
	DrawSprite(225 - (256 + 128 - sinMoveKindInter[SIN_MIXTURE_RESET]), 47, cShop.lpMResetTitle, 0, 0, 111, 32);
	DrawSprite(18 - (256 + 128 - sinMoveKindInter[SIN_MIXTURE_RESET]), 135 + sinInterHeight2, cShop.lpMResetMain, 0, 0, 320, 208);

	for (i = 0; i < 9; i++)
		DrawSprite(21 + (i * 22) - (256 + 128 - sinMoveKindInter[SIN_TRADE]), 224 + sinInterHeight2, lpCenterBox, 0, 0, 22, 21);

	if (sTrade.CheckFlag) {
		if (!CheckDelayFlag)
			DrawSprite(DownTradePosi[T_CHECK_POSI][0] - (256 + 128 - sinMoveKindInter[SIN_TRADE]), DownTradePosi[T_CHECK_POSI][1], lpCheck, 0, 0, 26, 26);
	}
	if (sTradeRecv.CheckFlag) {
		DrawSprite(UpTradePosi[T_CHECK_POSI][0] - (256 + 128 - sinMoveKindInter[SIN_TRADE]), UpTradePosi[T_CHECK_POSI][1], lpCheck, 0, 0, 26, 26);

	}
	if (TradeCheckFlag == T_CHECK_POSI) {
		DrawSprite(DownTradePosi[T_CHECK_POSI][0] - (256 + 128 - sinMoveKindInter[SIN_TRADE]), DownTradePosi[T_CHECK_POSI][1] - 27, lpCheckInfo, 0, 0, 47, 27);

	}
	if (TradeCheckFlag == T_TRADE_CLOSE_POSI) {
		DrawSprite(DownTradePosi[T_TRADE_CLOSE_POSI][0] - (256 + 128 - sinMoveKindInter[SIN_TRADE]), DownTradePosi[T_TRADE_CLOSE_POSI][1], lpbuttonCancel, 0, 0, 20, 20);
		DrawSprite(DownTradePosi[T_TRADE_CLOSE_POSI][0] - 27 - (256 + 128 - sinMoveKindInter[SIN_TRADE]), DownTradePosi[T_TRADE_CLOSE_POSI][1] - 27, lpbuttonCancelInfo, 0, 0, 77, 27);

	}

	if (CheckDelayFlag)
	{
		DrawSprite(DownTradePosi[T_CHECK_POSI][0] - (256 + 128 - sinMoveKindInter[SIN_TRADE]), DownTradePosi[T_CHECK_POSI][1], lpCheck_Glay, 0, 0, 26, 26);
	}

	if (CloseWareHouseFlag) {
		DrawSprite(304 - (256 + 128 - sinMoveKindInter[SIN_WAREHOUSE]), 313 + sinInterHeight2, cShop.lpExit, 0, 0, 20, 20);
		DrawSprite(304 - (256 + 128 - sinMoveKindInter[SIN_WAREHOUSE]), 313 - 27 + sinInterHeight2, cInvenTory.lpExitInfo, 0, 0, 47, 27);
	}

	if (CloseCaravanFlag)
	{
		DrawSprite(304 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 313 + sinInterHeight2, cShop.lpExit, 0, 0, 20, 20);
		DrawSprite(304 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 313 - 27 + sinInterHeight2, cInvenTory.lpExitInfo, 0, 0, 47, 27);
	}

	if (CaravanFuncFlag == 1)
	{
		DrawSprite(304 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 313 + sinInterHeight2, cShop.Caravan_BtnOkRename_, 0, 0, 20, 20);
	}

	if (Caravana::GetInstance()->OpenFlag)
	{
		switch (cCaravan.myCaravan.CaravanState)
		{
		case CaravanStates::FOLLOW:
			if (CustomHud::GetInstance()->MouseAction(235 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 100 + 177 + sinInterHeight2, 28, 28))
			{
				DrawSprite(235 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 100 + 177 + sinInterHeight2, cShop.Caravan_Stay_, 0, 0, 28, 28);
			}
			else
			{
				DrawSprite(235 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 100 + 177 + sinInterHeight2, cShop.Caravan_Stay, 0, 0, 28, 28);
			}
			break;
		case CaravanStates::STAY:
			if (CustomHud::GetInstance()->MouseAction(235 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 100 + 177 + sinInterHeight2, 28, 28))
			{
				DrawSprite(235 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 100 + 177 + sinInterHeight2, cShop.Caravan_Follow_, 0, 0, 28, 28);
			}
			else
			{
				DrawSprite(235 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 100 + 177 + sinInterHeight2, cShop.Caravan_Follow, 0, 0, 28, 28);
			}
			break;
		}

		// Bot�o renomear
		if (CustomHud::GetInstance()->MouseAction(270 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 90 + 177 + sinInterHeight2, 28, 28))
		{
			DrawSprite(270 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 100 + 177 + sinInterHeight2, cShop.Caravan_Rename_, 0, 0, 28, 28);
			DrawSprite(270 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]) - 2, 270 - 27 + sinInterHeight2, cShop.Caravan_TipRename, 0, 0, 76, 36);

			if (CustomHud::GetInstance()->Button() == 2)
			{
				Caravana::GetInstance()->ChangeName = true;
			}
		}
		else
		{
			DrawSprite(270 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 100 + 177 + sinInterHeight2, cShop.Caravan_Rename, 0, 0, 28, 28);
		}

		if (cCaravan.myCaravan.CaravanState == CaravanStates::FOLLOW)
		{
			if (CustomHud::GetInstance()->MouseAction(235 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 100 + 177 + sinInterHeight2, 28, 28))
				DrawSprite(220 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]) - 2, 270 - 27 + sinInterHeight2, cShop.Caravan_TipStay, 0, 0, 76, 36);

			if (CustomHud::GetInstance()->Button() == 1)
			{
				cCaravan.myCaravan.CaravanState = CaravanStates::STAY;

				smTRANS_COMMAND	smTransCommand;

				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.code = 0x44550008;
				smTransCommand.WParam = 0;
				smTransCommand.LParam = 0;
				smTransCommand.SParam = 0;
				smTransCommand.EParam = 0;

				if (smWsockUserServer)
					smWsockUserServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);
			}
		}
		else
		{
			if (CustomHud::GetInstance()->MouseAction(235 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 100 + 177 + sinInterHeight2, 28, 28))
				DrawSprite(250 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]) - 2, 270 - 27 + sinInterHeight2, cShop.Caravan_TipFollow, 0, 0, 76, 36);

			if (CustomHud::GetInstance()->Button() == 1)
			{
				cCaravan.myCaravan.CaravanState = CaravanStates::FOLLOW;

				smTRANS_COMMAND	smTransCommand;

				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.code = 0x44550008;
				smTransCommand.WParam = 1;
				smTransCommand.LParam = 0;
				smTransCommand.SParam = 0;
				smTransCommand.EParam = 0;

				if (smWsockUserServer)
					smWsockUserServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);
			}
		}
	}
	else
	{
		CaravanFuncFlag = 0;
		Caravana::GetInstance()->ChangeName = false;
	}

	if (Caravana::GetInstance()->ChangeName)
	{
		CHANGECARAVANNAME->Render();
	}

	if (cAging.OpenFlag) {
		for (i = 0; i < 4; i++) {
			for (int t = 0; t < 3; t++)
				if (AgingLevelSheltomIndex[i][t]) {
					DrawSprite(AgingLevelSheltomXY[i][t].x, AgingLevelSheltomXY[i][t].y,
						cAging.lpGraySheltom[AgingLevelSheltomIndex[i][t] - 1],
						0, 0, 22, 22);
				}
		}
	}

	if (SmeltingItem.OpenFlag)
	{
		int SheltomIndex = 0;
		if (S_smeltingItem.SmeltingItem[0].Flag)
		{
			SheltomIndex = SmeltingItem.GetSheltomIndex(S_smeltingItem.SmeltingItem[0].CODE);
			if (SheltomIndex)
			{
				DrawSprite(AgingLevelSheltomXY[0][0].x + 22, AgingLevelSheltomXY[0][0].y + 30,
					cAging.lpGraySheltom[SheltomIndex - 1], 0, 0, 22, 22);
			}
		}
	}

	switch (TradeColorIndex) {
	case SET_ITEM_CHECK_COLOR:
		if (MouseItem.Flag) {
			dsDrawColorBox(sinInvenColor[0], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_TRADE]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
			dsDrawColorBox(sinInvenColor[0], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_WAREHOUSE]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
			dsDrawColorBox(sinInvenColor[0], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_CRAFTITEM]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
			dsDrawColorBox(sinInvenColor[0], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_AGING]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
			dsDrawColorBox(sinInvenColor[0], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_SMELTING]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
			dsDrawColorBox(sinInvenColor[0], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_MANUFACTURE]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
			dsDrawColorBox(sinInvenColor[0], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_MIXTURE_RESET]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
			dsDrawColorBox(sinInvenColor[0], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
		}
		break;
	case OVERLAP_ITEM_COLOR:
		if (TradeCrashItemIndex[0])
		{
			dsDrawColorBox(sinInvenColor[1], sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1].x - (256 + 128 - sinMoveKindInter[SIN_WAREHOUSE]),
				sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1].y, sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1].w, sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1].h);

			dsDrawColorBox(sinInvenColor[1], sCaravan.CaravanItem[TradeCrashItemIndex[0] - 1].x - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]),
				sCaravan.CaravanItem[TradeCrashItemIndex[0] - 1].y, sCaravan.CaravanItem[TradeCrashItemIndex[0] - 1].w, sCaravan.CaravanItem[TradeCrashItemIndex[0] - 1].h);
		}

		break;
	case NOT_SETTING_COLOR:
		dsDrawColorBox(sinInvenColor[2], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_TRADE]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
		dsDrawColorBox(sinInvenColor[2], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_WAREHOUSE]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
		dsDrawColorBox(sinInvenColor[2], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_CRAFTITEM]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
		dsDrawColorBox(sinInvenColor[2], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_AGING]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
		dsDrawColorBox(sinInvenColor[2], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_SMELTING]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
		dsDrawColorBox(sinInvenColor[2], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_MANUFACTURE]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
		dsDrawColorBox(sinInvenColor[2], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_MIXTURE_RESET]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
		dsDrawColorBox(sinInvenColor[2], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
		break;

	case NOT_AGING_ITEM:
		dsDrawColorBox(sinInvenColor[2], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_AGING]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);

	case NOT_USE_COLOR:
		dsDrawColorBox(sinInvenColor[2], TradeColorRect.left - (256 + 128 - sinMoveKindInter[SIN_TRADE]), TradeColorRect.top, TradeColorRect.right, TradeColorRect.bottom);
		break;

	case SELECT_ITEM_COLOR:
		for (i = 0; i < 10; i++) {
			if (CopyItemIndex7[i]) {
				dsDrawColorBox(sinInvenColor[4], sWareHouse.WareHouseItem[CopyItemIndex7[i] - 1].x - (256 + 128 - sinMoveKindInter[SIN_WAREHOUSE]),
					sWareHouse.WareHouseItem[CopyItemIndex7[i] - 1].y, sWareHouse.WareHouseItem[CopyItemIndex7[i] - 1].w, sWareHouse.WareHouseItem[CopyItemIndex7[i] - 1].h);

				dsDrawColorBox(sinInvenColor[4], sCaravan.CaravanItem[CopyItemIndex7[i] - 1].x - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]),
					sCaravan.CaravanItem[CopyItemIndex7[i] - 1].y, sCaravan.CaravanItem[CopyItemIndex7[i] - 1].w, sCaravan.CaravanItem[CopyItemIndex7[i] - 1].h);
			}
		}
		if (!SelectTradeItemIndex)break;
		if (ItemKindFlag == 1) {
			dsDrawColorBox(sinInvenColor[4], sTrade.TradeItem[SelectTradeItemIndex - 1].x - (256 + 128 - sinMoveKindInter[SIN_TRADE]),
				sTrade.TradeItem[SelectTradeItemIndex - 1].y, sTrade.TradeItem[SelectTradeItemIndex - 1].w, sTrade.TradeItem[SelectTradeItemIndex - 1].h);
		}

		if (ItemKindFlag == 2) {
			dsDrawColorBox(sinInvenColor[4], sTradeRecv.TradeItem[SelectTradeItemIndex - 1].x - (256 + 128 - sinMoveKindInter[SIN_TRADE]),
				sTradeRecv.TradeItem[SelectTradeItemIndex - 1].y - T_REVISION_Y, sTradeRecv.TradeItem[SelectTradeItemIndex - 1].w, sTradeRecv.TradeItem[SelectTradeItemIndex - 1].h);
		}
		if (sWareHouse.WareHouseItem[SelectTradeItemIndex - 1].Flag)
			dsDrawColorBox(sinInvenColor[4], sWareHouse.WareHouseItem[SelectTradeItemIndex - 1].x - (256 + 128 - sinMoveKindInter[SIN_WAREHOUSE]),
				sWareHouse.WareHouseItem[SelectTradeItemIndex - 1].y, sWareHouse.WareHouseItem[SelectTradeItemIndex - 1].w, sWareHouse.WareHouseItem[SelectTradeItemIndex - 1].h);
		if (sCraftItem.CraftItem[SelectTradeItemIndex - 1].Flag)
			dsDrawColorBox(sinInvenColor[4], sCraftItem.CraftItem[SelectTradeItemIndex - 1].x - (256 + 128 - sinMoveKindInter[SIN_CRAFTITEM]),
				sCraftItem.CraftItem[SelectTradeItemIndex - 1].y, sCraftItem.CraftItem[SelectTradeItemIndex - 1].w, sCraftItem.CraftItem[SelectTradeItemIndex - 1].h);
		if (sAgingItem.AgingItem[SelectTradeItemIndex - 1].Flag)
			dsDrawColorBox(sinInvenColor[4], sAgingItem.AgingItem[SelectTradeItemIndex - 1].x - (256 + 128 - sinMoveKindInter[SIN_AGING]),
				sAgingItem.AgingItem[SelectTradeItemIndex - 1].y, sAgingItem.AgingItem[SelectTradeItemIndex - 1].w, sAgingItem.AgingItem[SelectTradeItemIndex - 1].h);
		if (sCaravan.CaravanItem[SelectTradeItemIndex - 1].Flag)
			dsDrawColorBox(sinInvenColor[4], sCaravan.CaravanItem[SelectTradeItemIndex - 1].x - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]),
				sWareHouse.WareHouseItem[SelectTradeItemIndex - 1].y, sWareHouse.WareHouseItem[SelectTradeItemIndex - 1].w, sWareHouse.WareHouseItem[SelectTradeItemIndex - 1].h);
		if (S_smeltingItem.SmeltingItem[SelectTradeItemIndex - 1].Flag)
		{
			dsDrawColorBox(sinInvenColor[4], S_smeltingItem.SmeltingItem[SelectTradeItemIndex - 1].x - (256 + 128 - sinMoveKindInter[SIN_SMELTING]),
				S_smeltingItem.SmeltingItem[SelectTradeItemIndex - 1].y, S_smeltingItem.SmeltingItem[SelectTradeItemIndex - 1].w, S_smeltingItem.SmeltingItem[SelectTradeItemIndex - 1].h);
		}

		if (g_sManufactureItem.ManufactureItem[SelectTradeItemIndex - 1].Flag)
		{
			dsDrawColorBox(sinInvenColor[4], g_sManufactureItem.ManufactureItem[SelectTradeItemIndex - 1].x - (256 + 128 - sinMoveKindInter[SIN_MANUFACTURE]),
				g_sManufactureItem.ManufactureItem[SelectTradeItemIndex - 1].y, g_sManufactureItem.ManufactureItem[SelectTradeItemIndex - 1].w, g_sManufactureItem.ManufactureItem[SelectTradeItemIndex - 1].h);
		}
		if (sMixtureResetItem.MixtureResetItem[SelectTradeItemIndex - 1].Flag)
		{
			dsDrawColorBox(sinInvenColor[4], sMixtureResetItem.MixtureResetItem[SelectTradeItemIndex - 1].x - (256 + 128 - sinMoveKindInter[SIN_MIXTURE_RESET]),
				sMixtureResetItem.MixtureResetItem[SelectTradeItemIndex - 1].y, sMixtureResetItem.MixtureResetItem[SelectTradeItemIndex - 1].w, sMixtureResetItem.MixtureResetItem[SelectTradeItemIndex - 1].h);
		}

		break;

	}
	for (i = 0; i < MAX_TRADE_ITEM; i++) {
		if (sTrade.TradeItem[i].Flag) {
			DrawSprite(sTrade.TradeItem[i].x - (256 + 128 - sinMoveKindInter[SIN_TRADE]), sTrade.TradeItem[i].y, sTrade.TradeItem[i].lpItem, 0, 0, sTrade.TradeItem[i].w, sTrade.TradeItem[i].h);
			if ((sTrade.TradeItem[i].CODE & sinITEM_MASK2) == sinFO1) {
				cInvenTory.DrawForceOrbEffect(sTrade.TradeItem[i].x - (256 + 128 - sinMoveKindInter[SIN_TRADE]), sTrade.TradeItem[i].y);
			}

		}
		if (sTradeRecv.TradeItem[i].Flag) {
			DrawSprite(sTradeRecv.TradeItem[i].x - (256 + 128 - sinMoveKindInter[SIN_TRADE]), sTradeRecv.TradeItem[i].y - T_REVISION_Y, sTradeRecv.TradeItem[i].lpItem, 0, 0, sTradeRecv.TradeItem[i].w, sTradeRecv.TradeItem[i].h);
			if ((sTradeRecv.TradeItem[i].CODE & sinITEM_MASK2) == sinFO1) {
				cInvenTory.DrawForceOrbEffect(sTradeRecv.TradeItem[i].x - (256 + 128 - sinMoveKindInter[SIN_TRADE]), sTradeRecv.TradeItem[i].y - T_REVISION_Y);
			}

		}
	}
	if (TradeRequestFlag)
	{
		TradeSpotTimer++;
		DrawImage(MatTradebuttonMain, BaseX, BaseY, 230, 161);
		DrawImage(LpButtonNo, BaseX + 115, BaseY + 116, 88, 35);
		DrawImage(LpButtonYes, BaseX + 26, BaseY + 116, 88, 35);


		if (RequestTradeButtonFlag == 1)
			DrawImage(LpButtonYes_, BaseX + 26, BaseY + 116, 88, 35);

		if (RequestTradeButtonFlag == 2)
			DrawImage(LpButtonNo_, BaseX + 115, BaseY + 116, 88, 35);

	}

	for (i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			DrawSprite(sWareHouse.WareHouseItem[i].x - (256 + 128 - sinMoveKindInter[SIN_WAREHOUSE]), sWareHouse.WareHouseItem[i].y, sWareHouse.WareHouseItem[i].lpItem, 0, 0, sWareHouse.WareHouseItem[i].w, sWareHouse.WareHouseItem[i].h);
			if ((sWareHouse.WareHouseItem[i].CODE & sinITEM_MASK2) == sinFO1) {
				cInvenTory.DrawForceOrbEffect(sWareHouse.WareHouseItem[i].x - (256 + 128 - sinMoveKindInter[SIN_WAREHOUSE]), sWareHouse.WareHouseItem[i].y);
			}

		}
	}

	DrawSprite(231 - (256 + 128 - sinMoveKindInter[SIN_WAREHOUSE]), 243 + sinInterHeight2, cShop.lpGoldEdit, 0, 0, 99, 24);
	DrawSprite(231 - (256 + 128 - sinMoveKindInter[SIN_WAREHOUSE]), 241 + sinInterHeight2, cShop.lpWeightEdit, 0, 0, 20, 20);
	DrawSprite(243 - (256 + 128 - sinMoveKindInter[SIN_WAREHOUSE]), 100, cShop.lpTitle_WareHouse, 0, 0, 79, 40);


	for (i = 0; i < 100; i++)
	{
		if (sCaravan.CaravanItem[i].Flag)
		{
			DrawSprite(sCaravan.CaravanItem[i].x - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), sCaravan.CaravanItem[i].y, sCaravan.CaravanItem[i].lpItem, 0, 0, sCaravan.CaravanItem[i].w, sCaravan.CaravanItem[i].h);
			if ((sCaravan.CaravanItem[i].CODE & sinITEM_MASK2) == sinFO1) {
				cInvenTory.DrawForceOrbEffect(sCaravan.CaravanItem[i].x - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), sCaravan.CaravanItem[i].y);
			}
		}
	}

	//DrawSprite(231 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 243 + sinInterHeight2, cShop.c, 0, 0, 99, 24);
	//DrawSprite(231 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 241 + sinInterHeight2, cShop.Caravan_Stay, 0, 0, 20, 20);
	//DrawSprite(243 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 100, cShop.lpTitle_WareHouse, 0, 0, 79, 40);

	if (AgingCheckSheltomFlag) {
		DrawSprite(158 - (256 + 128 - sinMoveKindInter[SIN_AGING]), 222 + sinInterHeight2, cShop.lpCraftItemButton, 0, 0, 26, 26);

	}

	if (CraftItemButtonIndex == 1) {
		DrawSprite(158 - (256 + 128 - sinMoveKindInter[SIN_CRAFTITEM]), 222 + sinInterHeight2, cShop.lpCraftItemButton, 0, 0, 26, 26);
		if (cCraftItem.ForceFlag) {
			DrawSprite(148 - (256 + 128 - sinMoveKindInter[SIN_CRAFTITEM]) - 27, 222 - 27 + sinInterHeight2, lpForceButtonInfo, 0, 0, 100, 27);
		}
		else {
			DrawSprite(158 - (256 + 128 - sinMoveKindInter[SIN_CRAFTITEM]) - 27, 222 - 27 + sinInterHeight2, cShop.lpCraftItemButtonInfo, 0, 0, 80, 27);
		}
		if (SmeltingItem.OpenFlag)
		{
			DrawSprite(158 - (256 + 128 - sinMoveKindInter[SIN_SMELTING]), 222 + sinInterHeight2, cShop.lpCraftItemButton, 0, 0, 26, 26);
			DrawSprite(158 - (256 + 128 - sinMoveKindInter[SIN_SMELTING]) - 27, 222 - 27 + sinInterHeight2, cShop.lpSmeltingItemButtonInfo, 0, 0, 80, 27);
		}
		if (ManufactureItem.m_OpenFlag)
		{
			DrawSprite(158 - (256 + 128 - sinMoveKindInter[SIN_MANUFACTURE]), 222 + sinInterHeight2, cShop.lpCraftItemButton, 0, 0, 26, 26);
			DrawSprite(158 - (256 + 128 - sinMoveKindInter[SIN_MANUFACTURE]) - 27, 222 - 27 + sinInterHeight2, cShop.lpManufactureItemButtonInfo, 0, 0, 80, 27);
		}

	}

	if (MixtureResetCheckFlag)
		DrawSprite(158 - (256 + 128 - sinMoveKindInter[SIN_MIXTURE_RESET]), 221 + sinInterHeight2, cShop.lpMResetButton, 0, 0, 26, 26);

	if (CraftItemButtonIndex == 1 && MixtureResetCheckFlag)
	{
		DrawSprite(158 - (256 + 128 - sinMoveKindInter[SIN_MIXTURE_RESET]) - 27, 222 - 27 + sinInterHeight2, cShop.lpMResetButtonInfo, 0, 0, 80, 27);
	}

	if (CraftItemButtonIndex == 1 && AgingCheckSheltomFlag) {
		DrawSprite(158 - (256 + 128 - sinMoveKindInter[SIN_AGING]), 222 + sinInterHeight2, cShop.lpCraftItemButton, 0, 0, 26, 26);
		DrawSprite(158 - (256 + 128 - sinMoveKindInter[SIN_AGING]) - 27, 222 - 27 + sinInterHeight2, cShop.lpAging_Info, 0, 0, 80, 27);
	}

	if (CraftItemButtonIndex == 2) {
		DrawSprite(304 - (256 + 128 - sinMoveKindInter[SIN_CRAFTITEM]) + 2 - 2, 313 + 1 + sinInterHeight2, cShop.lpExit, 0, 0, 20, 20);
		DrawSprite(304 - (256 + 128 - sinMoveKindInter[SIN_CRAFTITEM]) - 2, 313 - 27 + sinInterHeight2, cInvenTory.lpExitInfo, 0, 0, 47, 27);

		DrawSprite(304 - (256 + 128 - sinMoveKindInter[SIN_AGING]) + 2 - 2, 313 + 1 + sinInterHeight2, cShop.lpExit, 0, 0, 20, 20);
		DrawSprite(304 - (256 + 128 - sinMoveKindInter[SIN_AGING]) - 2, 313 - 27 + sinInterHeight2, cInvenTory.lpExitInfo, 0, 0, 47, 27);

		DrawSprite(304 - (256 + 128 - sinMoveKindInter[SIN_SMELTING]) + 2 - 2, 313 + 1 + sinInterHeight2, cShop.lpExit, 0, 0, 20, 20);
		DrawSprite(304 - (256 + 128 - sinMoveKindInter[SIN_SMELTING]) - 2, 313 - 27 + sinInterHeight2, cInvenTory.lpExitInfo, 0, 0, 47, 27);

		DrawSprite(304 - (256 + 128 - sinMoveKindInter[SIN_MANUFACTURE]) + 2 - 2, 313 + 1 + sinInterHeight2, cShop.lpExit, 0, 0, 20, 20);
		DrawSprite(304 - (256 + 128 - sinMoveKindInter[SIN_MANUFACTURE]) - 2, 313 - 27 + sinInterHeight2, cInvenTory.lpExitInfo, 0, 0, 47, 27);

		DrawSprite(304 - (256 + 128 - sinMoveKindInter[SIN_MIXTURE_RESET]) + 2 - 2, 313 + 1 + sinInterHeight2, cShop.lpExit, 0, 0, 20, 20);
		DrawSprite(304 - (256 + 128 - sinMoveKindInter[SIN_MIXTURE_RESET]) - 2, 313 - 27 + sinInterHeight2, cInvenTory.lpExitInfo, 0, 0, 47, 27);
	}

	for (i = 0; i < 15; i++)
	{
		if (sCraftItem.CraftItem[i].Flag) {
			DrawSprite(sCraftItem.CraftItem[i].x - (256 + 128 - sinMoveKindInter[SIN_CRAFTITEM]), sCraftItem.CraftItem[i].y, sCraftItem.CraftItem[i].lpItem, 0, 0, sCraftItem.CraftItem[i].w, sCraftItem.CraftItem[i].h);

		}
	}

	for (i = 0; i < 15; i++) {
		if (sAgingItem.AgingItem[i].Flag) {
			DrawSprite(sAgingItem.AgingItem[i].x - (256 + 128 - sinMoveKindInter[SIN_AGING]), sAgingItem.AgingItem[i].y, sAgingItem.AgingItem[i].lpItem, 0, 0, sAgingItem.AgingItem[i].w, sAgingItem.AgingItem[i].h);

		}
	}

	for (i = 0; i < 2; i++)
	{
		if (sMixtureResetItem.MixtureResetItem[i].Flag)
		{
			DrawSprite(sMixtureResetItem.MixtureResetItem[i].x - (256 + 128 - sinMoveKindInter[SIN_MIXTURE_RESET]), sMixtureResetItem.MixtureResetItem[i].y,
				sMixtureResetItem.MixtureResetItem[i].lpItem, 0, 0, sMixtureResetItem.MixtureResetItem[i].w, sMixtureResetItem.MixtureResetItem[i].h);
		}
	}

	for (i = 0; i < 5; i++)
	{
		if (S_smeltingItem.SmeltingItem[i].Flag)
		{
			DrawSprite(S_smeltingItem.SmeltingItem[i].x - (256 + 128 - sinMoveKindInter[SIN_SMELTING]), S_smeltingItem.SmeltingItem[i].y, S_smeltingItem.SmeltingItem[i].lpItem, 0, 0, S_smeltingItem.SmeltingItem[i].w, S_smeltingItem.SmeltingItem[i].h);
		}
	}
	for (i = 0; i < 4; i++)
	{
		if (g_sManufactureItem.ManufactureItem[i].Flag)
		{
			DrawSprite(g_sManufactureItem.ManufactureItem[i].x - (256 + 128 - sinMoveKindInter[SIN_MANUFACTURE]), g_sManufactureItem.ManufactureItem[i].y, g_sManufactureItem.ManufactureItem[i].lpItem, 0, 0, g_sManufactureItem.ManufactureItem[i].w, g_sManufactureItem.ManufactureItem[i].h);
		}
	}

	CraftItemMessageBoxPosi.x = 230;
	CraftItemMessageBoxPosi.y = 200;

	if (ShowItemCraftMessageFlag || ShowItemAgingMessageFlag || ShowItemSmeltingMessageFlag || ShowItemManufactureMessageFlag || ShowItemOverSpace) {
		for (i = 0; i < CraftItemMessageSize.x; i++) {
			for (int j = 0; j < CraftItemMessageSize.y; j++) {
				if (i == 0 && j == 0)
					dsDrawTexImage(cItem.MatItemInfoBox_TopLeft, CraftItemMessageBoxPosi.x + (i * 16), CraftItemMessageBoxPosi.y + (j * 16), 16, 16, 255);
				if (j == 0 && i != 0 && i + 1 < CraftItemMessageSize.x)
					dsDrawTexImage(cItem.MatItemInfoBox_TopCenter, CraftItemMessageBoxPosi.x + (i * 16), CraftItemMessageBoxPosi.y + (j * 16), 16, 16, 255);
				if (j == 0 && i + 1 == CraftItemMessageSize.x)
					dsDrawTexImage(cItem.MatItemInfoBox_TopRight, CraftItemMessageBoxPosi.x + (i * 16), CraftItemMessageBoxPosi.y + (j * 16), 16, 16, 255);

				if (i == 0 && j != 0 && j + 1 != CraftItemMessageSize.y)
					dsDrawTexImage(cItem.MatItemInfoBox_Left, CraftItemMessageBoxPosi.x + (i * 16), CraftItemMessageBoxPosi.y + (j * 16), 16, 16, 255);
				if (i != 0 && j != 0 && i + 1 != CraftItemMessageSize.x && j + 1 != CraftItemMessageSize.y)
					dsDrawTexImage(cItem.MatItemInfoBox_Center, CraftItemMessageBoxPosi.x + (i * 16), CraftItemMessageBoxPosi.y + (j * 16), 16, 16, 255);
				if (i + 1 == CraftItemMessageSize.x && j != 0 && j + 1 != CraftItemMessageSize.y)
					dsDrawTexImage(cItem.MatItemInfoBox_Right, CraftItemMessageBoxPosi.x + (i * 16), CraftItemMessageBoxPosi.y + (j * 16), 16, 16, 255);

				if (i == 0 && j + 1 == CraftItemMessageSize.y)
					dsDrawTexImage(cItem.MatItemInfoBox_BottomLeft, CraftItemMessageBoxPosi.x + (i * 16), CraftItemMessageBoxPosi.y + (j * 16), 16, 16, 255);
				if (i != 0 && j + 1 == CraftItemMessageSize.y && i + 1 != CraftItemMessageSize.x)
					dsDrawTexImage(cItem.MatItemInfoBox_BottomCenter, CraftItemMessageBoxPosi.x + (i * 16), CraftItemMessageBoxPosi.y + (j * 16), 16, 16, 255);
				if (j + 1 == CraftItemMessageSize.y && i + 1 == CraftItemMessageSize.x)
					dsDrawTexImage(cItem.MatItemInfoBox_BottomRight, CraftItemMessageBoxPosi.x + (i * 16), CraftItemMessageBoxPosi.y + (j * 16), 16, 16, 255);
			}
		}
	}
}

DWORD	dwTradeDelayTime = 0;
DWORD	dwTradeDelayTime2 = 0;
DWORD	dwTradeDelayTime3 = 0;
DWORD	dwTradeDelayTime4 = 0;

DWORD   dwCraftItemInfoDelayTime = 0;
DWORD   dwMixDelayTime = 0;
DWORD	dwMixItemColyTime = 0;
int		MixCancelButtonDelayFlag = 0;
DWORD   dwForceDelayTime = 0;

void cTRADE::Main()
{

	if (cCraftItem.ForceFlag) {
		if (MixItemNoCopyFlag) {
			dwForceDelayTime++;
			if (dwForceDelayTime > 70 * 5) {
				if (MixItemNoCopyFlag)MixItemNoCopyFlag = 0;
				dwForceDelayTime = 0;
			}
		}
		else dwForceDelayTime = 0;
	}

	if (CheckMixItemCopyFlag) {
		dwMixItemColyTime++;
		if (dwMixItemColyTime > 70 * 60 * 2) {
			dwMixItemColyTime = 0;
			CheckMixItemCopyFlag = 0;
		}
	}
	else
		dwMixItemColyTime = 0;

	if (MixCancelButtonDelayFlag) {
		dwMixDelayTime++;
		if (dwMixDelayTime > 70 * 10) {
			MixCancelButtonDelayFlag = 0;
			dwMixDelayTime = 0;
		}
	}
	else {
		dwMixDelayTime = 0;

	}

	cAging.CheckAgingData();
	int i, j = 0;
	CraftItemButtonIndex = 0;
	RequestTradeButtonFlag = 0;
	CloseWareHouseFlag = 0;
	CloseCaravanFlag = 0;

	TradeCheckFlag = 0;
	if (TradeRequestFlag) {
		if (pRealCursorPos.x >= BaseX + 134 && pRealCursorPos.x <= BaseX + 134 + 110 &&
			pRealCursorPos.y >= BaseY + 110 && pRealCursorPos.y <= BaseY + 110 + 44)
		{
			RequestTradeButtonFlag = 2;
		}

		if (pRealCursorPos.x >= BaseX + 20 && pRealCursorPos.x <= BaseX + 20 + 110 &&
			pRealCursorPos.y >= BaseY + 99 && pRealCursorPos.y <= BaseY + 99 + 44)
		{
			RequestTradeButtonFlag = 1;
		}

		dwTradeDelayTime3++;
		if (dwTradeDelayTime3 >= 70 * 10) {
			TradeRequestFlag = 0;
			dwTradeDelayTime3 = 0;

		}
	}

	if (OpenFlag) {
		if (pCursorPos.x >= DownTradePosi[T_CHECK_POSI][0] && pCursorPos.x <= DownTradePosi[T_CHECK_POSI][2] &&
			pCursorPos.y >= DownTradePosi[T_CHECK_POSI][1] && pCursorPos.y <= DownTradePosi[T_CHECK_POSI][3])
		{
			TradeCheckFlag = T_CHECK_POSI;
		}

		if (pCursorPos.x >= DownTradePosi[T_TRADE_CLOSE_POSI][0] && pCursorPos.x <= DownTradePosi[T_TRADE_CLOSE_POSI][2] &&
			pCursorPos.y >= DownTradePosi[T_TRADE_CLOSE_POSI][1] && pCursorPos.y <= DownTradePosi[T_TRADE_CLOSE_POSI][3]) {
			TradeCheckFlag = T_TRADE_CLOSE_POSI;
		}

		if (pCursorPos.x >= DownTradePosi[T_MONEY_POSI][0] + 10 && pCursorPos.x <= DownTradePosi[T_MONEY_POSI][2] + 10 &&
			pCursorPos.y >= DownTradePosi[T_MONEY_POSI][1] && pCursorPos.y <= DownTradePosi[T_MONEY_POSI][3])
		{
			TradeCheckFlag = T_MONEY_POSI;
		}

		if (pCursorPos.x >= DownTradePosi[T_COIN_POSI][0] + 10 && pCursorPos.x <= DownTradePosi[T_COIN_POSI][2] + 10 &&
			pCursorPos.y >= DownTradePosi[T_COIN_POSI][1] && pCursorPos.y <= DownTradePosi[T_COIN_POSI][3]) {
			TradeCheckFlag = T_COIN_POSI;
		}


		PickUpTradeItem(pCursorPos.x, pCursorPos.y);
		if (MouseItem.Flag)
			SetTradeItemAreaCheck(&MouseItem);

		if (CheckDelayFlag) {
			dwTradeDelayTime++;
			if (dwTradeDelayTime >= 70 * 2) {
				CheckDelayFlag = 0;
				dwTradeDelayTime = 0;

			}


		}

		if (sTrade.CheckFlag && sTradeRecv.CheckFlag) {
			dwTradeDelayTime2++;
			if (dwTradeDelayTime2 >= 70 * 5) {
				dwTradeDelayTime2 = 0;
				sinCancelFlag = 1;

			}

		}
		else
			sinCancelFlag = 0;

		if (ItemPickUpFlag) {
			dwTradeDelayTime4++;
			if (dwTradeDelayTime4 > 70 * 2) {
				dwTradeDelayTime4 = 0;
				ItemPickUpFlag = 0;

			}
		}

	}

	if (cWareHouse.OpenFlag) {
		if (pCursorPos.x >= DownTradePosi[T_CLOSE_POSI][0] && pCursorPos.x <= DownTradePosi[T_CLOSE_POSI][2] &&
			pCursorPos.y >= DownTradePosi[T_CLOSE_POSI][1] && pCursorPos.y <= DownTradePosi[T_CLOSE_POSI][3]) {
			CloseWareHouseFlag = 1;
		}
		cWareHouse.PickUpWareHouseItem(pCursorPos.x, pCursorPos.y);
		if (MouseItem.Flag) {
			cWareHouse.SetWareHouseItemAreaCheck(&MouseItem);

		}
		sWareHouse.BuyAreaCount = 10;
		sWareHouse.Weight[1] = (sWareHouse.BuyAreaCount * 100) + 196;

		if (smConfig.DebugMode) {
			for (i = 0; i < 10; i++) {
				CopyItemIndex7[i] = 0;

			}

			for (i = 0; i < 100; i++) {
				if (sWareHouse.WareHouseItem[i].Flag) {
					for (j = 0; j < 100; j++) {
						if (sWareHouse.WareHouseItem[j].Flag) {
							if (i == j)continue;
							if (CompareItems(&sWareHouse.WareHouseItem[i].sItemInfo, &sWareHouse.WareHouseItem[j].sItemInfo)) {
								for (int k = 0; k < 10; k++) {
									if (!CopyItemIndex7[k]) {
										CopyItemIndex7[k] = i + 1;
										CopyItemIndex7[k + 1] = j + 1;
										break;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if (Caravana::GetInstance()->OpenFlag)
	{
		if (pCursorPos.x >= DownTradePosi[T_CLOSE_POSI][0] && pCursorPos.x <= DownTradePosi[T_CLOSE_POSI][2] &&
			pCursorPos.y >= DownTradePosi[T_CLOSE_POSI][1] && pCursorPos.y <= DownTradePosi[T_CLOSE_POSI][3]) {
			CloseCaravanFlag = 1;
		}

		if (pCursorPos.x >= DownTradePosi[T_CLOSE_POSI][0] - 10 && pCursorPos.x <= DownTradePosi[T_CLOSE_POSI][2] - 10 &&
			pCursorPos.y >= DownTradePosi[T_CLOSE_POSI][1] + 40 && pCursorPos.y <= DownTradePosi[T_CLOSE_POSI][3] + 40) {
			CaravanFuncFlag = 1;
		}


		Caravana::GetInstance()->PickUpCaravanItem(pCursorPos.x, pCursorPos.y);

		if (MouseItem.Flag)
		{
			Caravana::GetInstance()->SetCaravanItemAreaCheck(&MouseItem);
		}

		sCaravan.BuyAreaCount = 10;
		sCaravan.Weight[1] = (sCaravan.BuyAreaCount * 100) + 196;

		if (smConfig.DebugMode) {
			for (i = 0; i < 10; i++) {
				CopyItemIndex7[i] = 0;

			}

			for (i = 0; i < 100; i++) {
				if (sCaravan.CaravanItem[i].Flag) {
					for (j = 0; j < 100; j++) {
						if (sCaravan.CaravanItem[j].Flag) {
							if (i == j)continue;
							if (CompareItems(&sCaravan.CaravanItem[i].sItemInfo, &sCaravan.CaravanItem[j].sItemInfo)) {
								for (int k = 0; k < 10; k++) {
									if (!CopyItemIndex7[k]) {
										CopyItemIndex7[k] = i + 1;
										CopyItemIndex7[k + 1] = j + 1;
										break;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if (cCraftItem.OpenFlag || cAging.OpenFlag || SmeltingItem.OpenFlag || ManufactureItem.m_OpenFlag || cMixtureReset.OpenFlag) {
		if (!MouseItem.Flag) {
			if (pCursorPos.x >= 158 && pCursorPos.x <= 158 + 26 &&
				pCursorPos.y >= 222 + sinInterHeight2 && pCursorPos.y <= 222 + 26 + sinInterHeight2) {
				if (!MixCancelButtonDelayFlag) {
					CraftItemButtonIndex = 1;
				}
			}

			if (pCursorPos.x >= DownTradePosi[T_CLOSE_POSI][0] && pCursorPos.x <= DownTradePosi[T_CLOSE_POSI][2] &&
				pCursorPos.y >= DownTradePosi[T_CLOSE_POSI][1] && pCursorPos.y <= DownTradePosi[T_CLOSE_POSI][3]) {
				if (!MixCancelButtonDelayFlag) {
					CraftItemButtonIndex = 2;
				}
			}
			cCraftItem.PickUpCraftItem(pCursorPos.x, pCursorPos.y);
		}
		if (MouseItem.Flag) {
			if (cCraftItem.OpenFlag)
				cCraftItem.SetCraftItemAreaCheck(&MouseItem);

		}

	}
	if (cAging.OpenFlag) {
		if (MouseItem.Flag) {
			cAging.SetAgingItemAreaCheck(&MouseItem);
		}
		else
			cCraftItem.PickUpCraftItem(pCursorPos.x, pCursorPos.y, 0, 1);

	}

	if (SmeltingItem.OpenFlag)
	{
		if (MouseItem.Flag)
		{
			if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinPR1 || (MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinPR2)
			{
				if (S_smeltingItem.SmeltingItem[0].Flag && S_smeltingItem.SmeltingItem[0].CODE != MouseItem.CODE)
				{
					return;
				}

				if (S_smeltingItem.SmeltingItem[0].Flag && S_smeltingItem.SmeltingItem[1].Flag && S_smeltingItem.SmeltingItem[2].Flag) return;

			}
			SmeltingItem.SetSmeltingItemAreaCheck(&MouseItem);
		}
		else
		{
			SmeltingItem.PickUpSmeltingItem(pCursorPos.x, pCursorPos.y);
		}
	}

	if (ManufactureItem.m_OpenFlag)
	{
		if (MouseItem.Flag)
		{
			ManufactureItem.SetManufactureItemAreaCheck(&MouseItem);
		}
		else
		{
			ManufactureItem.PickUpManufactureItem(pCursorPos.x, pCursorPos.y);
		}
	}

	if (cMixtureReset.OpenFlag)
	{
		if (MouseItem.Flag)
			cMixtureReset.SetMixtureItemResetAreaCheck(&MouseItem);
		else
			cMixtureReset.PickUpMixtureResetItem(pCursorPos.x, pCursorPos.y);
	}

	if (ShowItemCraftMessageFlag || ShowItemAgingMessageFlag || ShowItemSmeltingMessageFlag || ShowItemManufactureMessageFlag || ShowItemOverSpace) {
		dwCraftItemInfoDelayTime++;
		if (dwCraftItemInfoDelayTime >= 70 * 10) {
			ShowItemCraftMessageFlag = 0;
			ShowItemAgingMessageFlag = 0;
			dwCraftItemInfoDelayTime = 0;
			ShowItemSmeltingMessageFlag = 0;
			ShowItemManufactureMessageFlag = 0;
			ShowItemOverSpace = 0;

		}

	}

	if (cWareHouse.OpenFlag)
		CloseWareHouseCheckFlag2 = 1;

	if (Caravana::GetInstance()->OpenFlag)
		Caravana::GetInstance()->CloseCaravanCheckFlag2 = 1;


	if (!CloseWareHouseCheckFlag3 && !cWareHouse.OpenFlag && CloseWareHouseCheckFlag2 == 1) {
		cWareHouse.RestoreInvenItem();
		CloseWareHouseCheckFlag3 = 0;
		CloseWareHouseCheckFlag2 = 0;
	}

	if (!Caravana::GetInstance()->CloseCaravanCheckFlag3 && !Caravana::GetInstance()->OpenFlag && Caravana::GetInstance()->CloseCaravanCheckFlag2 == 1) {
		Caravana::GetInstance()->RestoreInvenItem();
		Caravana::GetInstance()->CloseCaravanCheckFlag3 = 0;
		Caravana::GetInstance()->CloseCaravanCheckFlag2 = 0;
	}

}

void cTRADE::Close()
{

}
/*----------------------------------------------------------------------------*
*						Bot�o Esquerdo no trade
*-----------------------------------------------------------------------------*/
void cTRADE::LButtonDown(int x, int y)
{

	if (LbuttonUpCheckFlag)return;

	int i, TempFlag = 0;
	if (TradeCheckFlag == T_CHECK_POSI)
	{
		if (!MouseItem.Flag) {
			if (!CheckDelayFlag)
			{
				if (!ItemPickUpFlag) {
					if (CheckTradeItemSet()) {
						sTrade.CheckFlag = 1;
						SendTradeItem(TradeCharCode);
						CheckTradeButtonOk();
					}
				}
			}
		}
	}

	if (TradeCheckFlag == T_TRADE_CLOSE_POSI)
	{
		if (!dwTradeMaskTime || sinCancelFlag)
		{
			SendRequestTrade(TradeCharCode, 3);
			cTrade.OpenFlag = 0;
			cInvenTory.OpenFlag = 0;
			CancelTradeItem();
			sinCancelFlag = 0;
		}

	}

	if (RequestTradeButtonFlag == 2)
	{
		TradeRequestFlag = 0;
		TradeCharCode = 0;
		dwTradeDelayTime3 = 0;
	}

	if (RequestTradeButtonFlag == 1)
	{
		if (!MouseItem.Flag)
		{
			if (GetTradeDistanceFromCode(TradeCharCode) == TRUE)
				SendRequestTrade(TradeCharCode, 1);
			TradeRequestFlag = 0;
			dwTradeDelayTime3 = 0;
		}
	}

	if (cTrade.OpenFlag)
	{
		if (MouseItem.Flag && !sTrade.CheckFlag)
		{
			cInvenTory.CheckRequireItemToSet(&MouseItem);
			if (TradeColorIndex == NO_BOX_COLOR || TradeColorIndex == NOT_SETTING_COLOR)return;
			if (!LastSetTradeItem(&MouseItem)) {
			}
		}
		else {
			if (!TradeItemSucessFlag && !TradeSendSucessFlag && !sTrade.CheckFlag) {
				PickUpTradeItem(pCursorPos.x, pCursorPos.y, 1);
				if (TradeCheckFlag == T_MONEY_POSI && !sTrade.CheckFlag)
					cMessageBox.ShowMessage2(MESSAGE_MOVE_MONEY_TRADE);
				if (TradeCheckFlag == T_COIN_POSI && !sTrade.CheckFlag)
					cMessageBox.ShowMessage2(MESSAGE_MOVE_COIN_TRADE);
			}

		}

	}

	if (CloseWareHouseFlag)
	{
		cWareHouse.CloseWareHouse();
	}

	if (CloseCaravanFlag)
	{
		Caravana::GetInstance()->CloseCaravan();
	}

	if (cWareHouse.OpenFlag)
	{
		if (MouseItem.Flag) {

			cInvenTory.CheckRequireItemToSet(&MouseItem);
			if (TradeColorIndex == NO_BOX_COLOR || TradeColorIndex == NOT_SETTING_COLOR)return;
			if (!cWareHouse.ChangeWareHouseItem(&MouseItem))
				cWareHouse.LastSetWareHouseItem(&MouseItem);

		}
		else
			cWareHouse.PickUpWareHouseItem(pCursorPos.x, pCursorPos.y, 1);

		if (pCursorPos.x >= 231 && pCursorPos.x <= 231 + 20 &&
			pCursorPos.y >= 213 + sinInterHeight2 && pCursorPos.y <= 213 + 20 + sinInterHeight2) {
			cMessageBox.ShowMessage2(MESSAGE_MOVE_MONEY_RECV);
		}
	}


	if (Caravana::GetInstance()->OpenFlag)
	{
		if (MouseItem.Flag) {

			cInvenTory.CheckRequireItemToSet(&MouseItem);
			if (TradeColorIndex == NO_BOX_COLOR || TradeColorIndex == NOT_SETTING_COLOR)return;
			if (!Caravana::GetInstance()->ChangeCaravanItem(&MouseItem))
				Caravana::GetInstance()->LastSetCaravanItem(&MouseItem);

		}
		else
			Caravana::GetInstance()->PickUpCaravanItem(pCursorPos.x, pCursorPos.y, 1);

		if (pCursorPos.x >= 231 && pCursorPos.x <= 231 + 20 &&
			pCursorPos.y >= 213 + sinInterHeight2 && pCursorPos.y <= 213 + 20 + sinInterHeight2) {
			cMessageBox.ShowMessage2(MESSAGE_MOVE_MONEY_RECV);
		}
	}

	int CloseButtonFlag = 0;
	int SheltomCount = 0;
	if (cCraftItem.OpenFlag)
	{
		if (MouseItem.Flag)
		{
			if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinPM1 || (MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinPL1 ||
				(MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinPS1 ||
				(MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinBI2 || (MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinWR1 ||
				(MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinDR1 || (MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinGP1 ||
				(MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinCH1 ||
				(MouseItem.sItemInfo.CODE == (sinDB1 | sin31)) || (MouseItem.sItemInfo.CODE == (sinDB1 | sin32)) ||
				(MouseItem.sItemInfo.CODE == (sinDB1 | sin33)) ||
				(MouseItem.sItemInfo.CODE == (sinOA2 | sin33)) ||
				(MouseItem.sItemInfo.CODE == (sinDB1 | sin34)) ||
				(MouseItem.sItemInfo.CODE == (sinOA2 | sin34)) ||

				(MouseItem.sItemInfo.CODE == (sinDA1 | sin05)) ||
				(MouseItem.sItemInfo.CODE == (sinDB1 | sin05)) ||
				(MouseItem.sItemInfo.CODE == (sinDG1 | sin05)) ||
				(MouseItem.sItemInfo.CODE == (sinDS1 | sin05)) ||
				(MouseItem.sItemInfo.CODE == (sinOA1 | sin05)) ||
				(MouseItem.sItemInfo.CODE == (sinOA2 | sin10)) ||
				(MouseItem.sItemInfo.CODE == (sinOR1 | sin05)) ||
				(MouseItem.sItemInfo.CODE == (sinWA1 | sin05)) ||
				(MouseItem.sItemInfo.CODE == (sinWH1 | sin06)) ||
				(MouseItem.sItemInfo.CODE == (sinWM1 | sin06)) ||
				(MouseItem.sItemInfo.CODE == (sinWP1 | sin05)) ||
				(MouseItem.sItemInfo.CODE == (sinWS1 | sin05)) ||
				(MouseItem.sItemInfo.CODE == (sinWS2 | sin10)) ||
				(MouseItem.sItemInfo.CODE == (sinWT1 | sin05)) ||

				(MouseItem.sItemInfo.CODE == (sinDA1 | sin48)) ||
				(MouseItem.sItemInfo.CODE == (sinDA1 | sin47))

				)

			{
				TradeColorIndex = NOT_SETTING_COLOR;
			}

			if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinOR2)
			{
				if ((MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin06 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin07 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin08 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin09 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin10 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin11 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin12 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin13 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin14 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin15 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin16 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin17 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin18 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin19 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin20 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin21 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin22 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin23 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin24 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin25 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin27 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin28 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin29 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin30 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin31 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin32 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin34 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin35)

					TradeColorIndex = NOT_SETTING_COLOR;
			}

			if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinOA1)
			{
				if ((MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin32 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin33 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin34 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin35 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin38)
				{
					TradeColorIndex = NOT_SETTING_COLOR;
				}
			}

			if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinSP1)
			{
				if ((MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin34)
				{
					TradeColorIndex = NOT_SETTING_COLOR;
				}
			}

			if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinSP1)
			{
				if ((MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin15)
					TradeColorIndex = NOT_SETTING_COLOR;
			}

			if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinSP1)
			{
				if ((MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin35)
					TradeColorIndex = NOT_SETTING_COLOR;
			}

			if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinOR2)
			{
				if ((MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin33)
					TradeColorIndex = NOT_SETTING_COLOR;
			}

			if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinSP1)
			{
				if ((MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin27 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin28 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin29 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin30 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin31 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin32 ||
					(MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin33)
				{
					TradeColorIndex = NOT_SETTING_COLOR;
				}
			}

			if (!cCraftItem.ForceFlag)
			{
				if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinOS1)
				{
					if ((MouseItem.sItemInfo.CODE & sinITEM_MASK3) >= sin21 && (MouseItem.sItemInfo.CODE & sinITEM_MASK3) <= sin34)
					{
						TradeColorIndex = NOT_SETTING_COLOR;
					}
				}
			}


			if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinOA1)
			{
				if ((MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin36)
					TradeColorIndex = NOT_SETTING_COLOR;
			}

			if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinSP1)
			{
				if ((MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin36)
					TradeColorIndex = NOT_SETTING_COLOR;
			}

			if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinOA1)
			{
				if ((MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin37)
					TradeColorIndex = NOT_SETTING_COLOR;
			}

			if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinSP1)
			{
				if ((MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin37 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin38)
					TradeColorIndex = NOT_SETTING_COLOR;
			}

			if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinSP1)
			{
				if ((MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin39 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin42)
					TradeColorIndex = NOT_SETTING_COLOR;
			}

			if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinGF1)
			{
				if ((MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin07 || (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin08)
					TradeColorIndex = NOT_SETTING_COLOR;
			}

			if (!sCraftItem.CraftItem[0].Flag) {
				if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinSE1) {
					cMessageBox.ShowMessage(MESSAGE_NOT_RECONITEM);
					TradeColorIndex = NOT_SETTING_COLOR;
				}
			}
			if (TradeColorIndex != NO_BOX_COLOR || TradeColorIndex != NOT_SETTING_COLOR) {
				if (MouseItem.sItemInfo.ItemKindCode == ITEM_KIND_AGING || MouseItem.sItemInfo.ItemKindCode == ITEM_KIND_CRAFT || MouseItem.sItemInfo.UniqueItem)
				{
					if (!cCraftItem.ForceFlag) {
						cMessageBox.ShowMessage(MESSAGE_NOT_CRAFT_ITEM);
					}
				}
				else {

					if (TradeColorIndex == NOT_ENOUGH_MONEY) {
						cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);
						return;

					}
					cInvenTory.CheckRequireItemToSet(&MouseItem);
					if (MouseItem.sItemInfo.CODE == (sinBI1 | sin90))
					{
						if (159 + 11 <= MouseItem.x + 22 && 159 + 11 > MouseItem.x && 268 + 11 + sinInterHeight2 < MouseItem.y + 22 && 268 + 11 + sinInterHeight2 > MouseItem.y)
						{
							TradeColorIndex = SET_ITEM_CHECK_COLOR;
						}
					}
					if (TradeColorIndex == NO_BOX_COLOR || TradeColorIndex == NOT_SETTING_COLOR)return;
					cCraftItem.LastSetCraftItem(&MouseItem);
				}
			}
		}
		else {
			if (!StartInterEffectAging2 && !sinCraftItemStartFlag && !CheckMixItemCopyFlag) {
				if (!sinCraftItemStartFlag2 && !MixItemNoCopyFlag) {
					cCraftItem.PickUpCraftItem(pCursorPos.x, pCursorPos.y, 1);
				}
				if (CraftItemButtonIndex == 2)
				{
					SheltomCount = 0;
					if (sCraftItem.CraftItem[0].Flag) {
						for (i = 0; i < sCraftItem.CraftItem[0].h / 22; i++) {
							SheltomCount += sCraftItem.CraftItem[0].w / 22;
						}
						if (!cCraftItem.CraftCheckEmptyArea(&sCraftItem.CraftItem[0]))
							SheltomCount = 100;

					}
					for (i = 1; i < 15; i++) {
						if (sCraftItem.CraftItem[i].Flag) {
							if (cCraftItem.CraftCheckEmptyArea(&sCraftItem.CraftItem[i])) {
								CloseButtonFlag = 0;
							}
							else {
								CloseButtonFlag = 1;
								break;
							}
							SheltomCount++;
						}
					}
					if (!CloseButtonFlag && SheltomCount <= cCraftItem.GetCraftCheckEmptyArea()) {
						cCraftItem.CancelMixItem();
					}
					else {
						cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
					}
				}
				if (CraftItemButtonIndex == 1)
				{
					if (cCraftItem.ForceFlag) {
						SheltomCount = 0;
						for (i = 1; i < 15; i++) {
							if (sCraftItem.CraftItem[i].Flag)
								SheltomCount++;
						}
						if (SheltomCount <= cCraftItem.GetCraftCheckEmptyArea()) {
							cCraftItem.sinSendCraftItem();
						}
						else
							cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
					}

					else if (cCraftItem.haCheckReconItem()) {
						if (sCraftItem.CraftItem[0].Flag && !sCraftItem.CraftItem[0].sItemInfo.ItemKindCode) {
							for (i = 1; i < 15; i++) {
								if (sCraftItem.CraftItem[i].Flag) {
									CopyMixItemCheckDelete(&sCraftItem.CraftItem[i - 1]);
								}
							}
							if (cCraftItem.CraftCheckEmptyArea(&sCraftItem.CraftItem[0])) {
								if (!sinCraftItemStartFlag2) {
									sinCraftItemStartFlag = 1;
									SetInterEffectAging();

								}
							}
							else {
								cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
							}

						}
					}
					else {
						if (sCraftItem.CraftItem[0].Flag && !sCraftItem.CraftItem[0].sItemInfo.ItemKindCode) {
							for (i = 1; i < 15; i++) {
								if (sCraftItem.CraftItem[i].Flag) {
									CopyMixItemCheckDelete(&sCraftItem.CraftItem[i - 1]);
								}
							}
							for (i = 1; i < 15; i++) {
								if (sCraftItem.CraftItem[i].Flag)
								{
									if (cCraftItem.CraftCheckEmptyArea(&sCraftItem.CraftItem[0]) && !sCraftItem.CraftItem[14].Flag)
									{
										if (!sinCraftItemStartFlag2) {
											sinCraftItemStartFlag = 1;
											SetInterEffectAging();
											TempFlag = 1;
											MixCancelButtonDelayFlag = 1;
											CheckMixItemCopyFlag = 1;

										}

									}
									else
									{
										if (sCraftItem.CraftItem[14].Flag)
										{
											cMessageBox.ShowMessage(MESSAGE_CANNOT_RECONITEM);
										}
										else
											cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);

										TempFlag = 1;
									}

								}
							}
							if (!TempFlag)
								cMessageBox.ShowMessage(MESSAGE_NOT_CRAFT_ITEM);

						}
						else {
							cMessageBox.ShowMessage(MESSAGE_NOT_CRAFT_ITEM);
						}
					}
				}
			}
		}
	}

	if (cMixtureReset.OpenFlag)
	{
		if (MouseItem.Flag)
		{
			if (!sMixtureResetItem.MixtureResetItem[0].Flag)
			{
				if ((MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin89)
				{
					cMessageBox.ShowMessage(MESSAGE_NOT_MIXTURE_RESET_STONE);
					TradeColorIndex = NOT_SETTING_COLOR;
				}
			}

			// Limpar itens com aging e mix
			if (TradeColorIndex != NO_BOX_COLOR && TradeColorIndex != NOT_SETTING_COLOR)
			{
				if ((MouseItem.CODE & sinITEM_MASK2) == sinBI1 && (MouseItem.sItemInfo.CODE & sinITEM_MASK3) == sin89)
				{
					cMixtureReset.LastSetMixtureResetItem(&MouseItem, 1);
					MixtureResetCheckFlag = 1;
				}
				else if ((MouseItem.sItemInfo.ItemKindCode == ITEM_KIND_CRAFT || (MouseItem.sItemInfo.ItemKindCode == ITEM_KIND_AGING)) && MouseItem.sItemInfo.expireTime <= 0) {
					cMixtureReset.LastSetMixtureResetItem(&MouseItem, 2);
				}
			}
		}
		else
		{
			if (!StartInterEffectAging2 && !sinCraftItemStartFlag)
			{
				if (!sinCraftItemStartFlag2)
					cMixtureReset.PickUpMixtureResetItem(pCursorPos.x, pCursorPos.y, 1);

				if (CraftItemButtonIndex == 2)
				{
					CloseButtonFlag = 0;
					if (sMixtureResetItem.MixtureResetItem[1].Flag)
						CloseButtonFlag = 1;

					if (CloseButtonFlag == 1)
						cMessageBox.ShowMessage(MESSAGE_NOT_CANCEL_RESET);
					else
					{
						if (sMixtureResetItem.MixtureResetItem[0].Flag)
						{
							if (!cCraftItem.CraftCheckEmptyArea(&sMixtureResetItem.MixtureResetItem[0]))
							{
								int LineCount = 0, Temp;
								ShowItemOverSpace = 1;
								lstrcpy(szCraftItemBuff, "������ Ȯ���� �ּ���");

								strcat(szCraftItemBuff, "\r");
								Temp = lstrlen(szCraftItemBuff);
								for (i = 0; i < Temp; i++)
								{
									if (szCraftItemBuff[i] == '\r')
									{
										LineCount++;
									}
								}
								CraftItemMessageSize.x = 14;
								CraftItemMessageSize.y = ((LineCount) * 2) + 2;
								sinPlaySound(SIN_SOUND_FAILD_MIXITEM);
							}
							else
								cMixtureReset.CancelMixtureItemReset();
						}
						else
							cMixtureReset.CancelMixtureItemReset();
					}
				}

				if (CraftItemButtonIndex == 1 && lpCurPlayer->vipLevel == 3 && MixtureResetCheckFlag == 1) {
					if (cCraftItem.CraftCheckEmptyArea(&sMixtureResetItem.MixtureResetItem[0]))
					{
						if (!sinCraftItemStartFlag2)
						{
							sinCraftItemStartFlag = 1;
							SetInterEffectMixtureReset();
							MixCancelButtonDelayFlag = 1;
							MixtureResetCheckFlag = 0;
						}
					}
					else
						cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
				}

				if (CraftItemButtonIndex == 1 && sMixtureResetItem.MixtureResetItem[1].Flag)
				{
					if (cCraftItem.CraftCheckEmptyArea(&sMixtureResetItem.MixtureResetItem[0]))
					{
						if (!sinCraftItemStartFlag2)
						{
							sinCraftItemStartFlag = 1;
							SetInterEffectMixtureReset();
							MixCancelButtonDelayFlag = 1;
							MixtureResetCheckFlag = 0;
						}
					}
					else
						cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
				}
			}
		}
	}

	if (cAging.OpenFlag) {
		if (MouseItem.Flag) {
			if (TradeColorIndex == NOT_AGING_ITEM) {
				cMessageBox.ShowMessage(MESSAGE_NOT_AGING_ITEM);
			}
			else {
				if (TradeColorIndex == NOT_ENOUGH_MONEY) {
					cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);

				}
				if (TradeColorIndex != NO_BOX_COLOR && TradeColorIndex != NOT_SETTING_COLOR && TradeColorIndex != NOT_ENOUGH_MONEY) {
					if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
						(MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinMA2) {
						cCraftItem.LastSetCraftItem(&MouseItem, 2);
						sinMakeItemCheck();

					}
					else if (MouseItem.sItemInfo.CODE == (sinBI1 | sin10) || MouseItem.sItemInfo.CODE == (sinBI1 | sin11) ||
						MouseItem.sItemInfo.CODE == (sinBI1 | sin60) || MouseItem.sItemInfo.CODE == (sinBI1 | sin61) ||
						MouseItem.sItemInfo.CODE == (sinBI1 | sin85) || MouseItem.sItemInfo.CODE == (sinBI2 | sin38) || MouseItem.sItemInfo.CODE == (sinBI2 | sin52))
					{
						if (!StartInterEffectAging2 && !sinCraftItemStartFlag) {
							cCraftItem.LastSetCraftItem(&MouseItem, 3);
							SpecialSheltomCode = MouseItem.sItemInfo.CODE;
						}
					}
					else {
						cCraftItem.LastSetCraftItem(&MouseItem, 1);
						cAging.CheckItem(&MouseItem);
					}
				}
			}
		}
		else {
			if (!StartInterEffectAging2 && !sinCraftItemStartFlag)
			{
				if (!sinCraftItemStartFlag2)
				{
					cCraftItem.PickUpCraftItem(pCursorPos.x, pCursorPos.y, 1, 1);
				}
				if (CraftItemButtonIndex == 2) {
					CloseButtonFlag = 0;
					for (int i = 1; i < 15; i++) {
						if (sAgingItem.AgingItem[i].Flag) {
							CloseButtonFlag = 1;
							break;
						}
					}
					if (CloseButtonFlag == 1) {
						cMessageBox.ShowMessage(MESSAGE_NOT_AGINGSHELTOM);

					}
					else {
						if (sAgingItem.AgingItem[0].Flag) {
							if (!cCraftItem.CraftCheckEmptyArea(&sAgingItem.AgingItem[0]))
							{
								int LineCount = 0, Temp;
								ShowItemOverSpace = 1;
								lstrcpy(szCraftItemBuff, InvenSpaceSecure);

								strcat(szCraftItemBuff, "\r");
								Temp = lstrlen(szCraftItemBuff);
								for (int j = 0; j < Temp; j++)
								{
									if (szCraftItemBuff[j] == '\r')
									{
										LineCount++;
									}
								}
								CraftItemMessageSize.x = 14;
								CraftItemMessageSize.y = ((LineCount) * 2) + 2;
								sinPlaySound(SIN_SOUND_FAILD_MIXITEM);
							}
							else {
								cAging.CancelAgingItem();
							}
						}
						else
							cAging.CancelAgingItem();

					}
				}

				if (CraftItemButtonIndex == 1 && AgingCheckSheltomFlag) {
					if (cCraftItem.CraftCheckEmptyArea(&sAgingItem.AgingItem[0])) {
						if (!sinCraftItemStartFlag2) {
							sinCraftItemStartFlag = 1;
							SetInterEffectAging();
							MixCancelButtonDelayFlag = 1;
						}
					}
					else
						cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
				}

			}
		}

	}

	if (SmeltingItem.OpenFlag)
	{
		if (MouseItem.Flag)
		{
			if (TradeColorIndex == NO_BOX_COLOR || TradeColorIndex == NOT_SETTING_COLOR)
			{
				return;
			}
			if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinPR1 || (MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinPR2)
			{
				if (S_smeltingItem.SmeltingItem[0].Flag && S_smeltingItem.SmeltingItem[0].CODE != MouseItem.CODE)
				{
					cMessageBox.ShowMessage(MESSAGE_NOT_SAME_SMELTINGITEM);
					return;
				}

				if (S_smeltingItem.SmeltingItem[0].Flag && S_smeltingItem.SmeltingItem[1].Flag && S_smeltingItem.SmeltingItem[2].Flag) return;

			}
			if (TradeColorIndex == NOT_ENOUGH_MONEY)
			{
				cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);
				return;

			}
			SmeltingItem.LastSetSmeltingItem(&MouseItem);
		}
		else
		{
			if (!StartInterEffectAging2 && !sinCraftItemStartFlag && !CheckMixItemCopyFlag)
			{
				if (!sinCraftItemStartFlag2 && !MixCancelButtonDelayFlag)
				{
					SmeltingItem.PickUpSmeltingItem(pCursorPos.x, pCursorPos.y, 1);
				}

				if (CraftItemButtonIndex == 2)
				{
					for (i = 0; i < 5; i++)
					{
						if (S_smeltingItem.SmeltingItem[i].Flag)
						{
							if (SmeltingItem.SmeltingCheckEmptyArea(&S_smeltingItem.SmeltingItem[i]))
							{
								CloseButtonFlag = 0;
							}
							else
							{
								CloseButtonFlag = 1;
								break;
							}
							SheltomCount++;
						}

					}
					if (!CloseButtonFlag && SheltomCount <= SmeltingItem.GetSmeltingCheckEmptyArea())
					{
						SmeltingItem.CancelSmeltingItem();
					}
					else
					{
						int LineCount = 0, Temp;
						ShowItemOverSpace = 1;
						lstrcpy(szCraftItemBuff, InvenSpaceSecure);

						strcat(szCraftItemBuff, "\r");
						Temp = lstrlen(szCraftItemBuff);
						for (i = 0; i < Temp; i++)
						{
							if (szCraftItemBuff[i] == '\r')
							{
								LineCount++;
							}
						}
						CraftItemMessageSize.x = 14;
						CraftItemMessageSize.y = ((LineCount) * 2) + 2;
						sinPlaySound(SIN_SOUND_FAILD_MIXITEM);
					}
				}

				else if (CraftItemButtonIndex == 1)
				{
					if (S_smeltingItem.SmeltingItem[0].Flag && !S_smeltingItem.SmeltingItem[0].sItemInfo.ItemKindCode &&
						S_smeltingItem.SmeltingItem[1].Flag && !S_smeltingItem.SmeltingItem[1].sItemInfo.ItemKindCode &&
						S_smeltingItem.SmeltingItem[2].Flag && !S_smeltingItem.SmeltingItem[2].sItemInfo.ItemKindCode)
					{
						sITEM temp;
						for (int i = 0; i < MAX_ITEM; i++)
						{
							if (sItem[i].CODE == (sinPR1 | sin01))
							{
								temp = sItem[i];
								if (cInvenTory.CheckInvenEmpty(&temp))
								{
									if (S_smeltingItem.SmeltingItem[4].Flag)
									{
										if (!sinCraftItemStartFlag2)
										{
											sinCraftItemStartFlag = 1;
											SetInterEffectSmelting();
											MixCancelButtonDelayFlag = 1;
										}
									}
								}
								else
								{
									cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
								}
								break;
							}
						}
					}
				}
			}
		}
	}

	if (ManufactureItem.m_OpenFlag)
	{
		if (MouseItem.Flag)
		{
			if (TradeColorIndex == NOT_SETTING_COLOR)
			{
				return;
			}
			else if (TradeColorIndex == NO_BOX_COLOR)
			{
				cMessageBox.ShowMessage(MESSAGE_NOT_NEED_RUNE);
				return;
			}
			else if (TradeColorIndex == NOT_ENOUGH_MONEY)
			{
				cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);
				return;
			}
			ManufactureItem.LastSetManufactureItem(&MouseItem);
		}
		else
		{
			if (!StartInterEffectAging2 && !sinCraftItemStartFlag && !CheckMixItemCopyFlag)
			{
				if (!sinCraftItemStartFlag2 && !MixCancelButtonDelayFlag)
				{
					ManufactureItem.PickUpManufactureItem(pCursorPos.x, pCursorPos.y, 1);
				}

				if (CraftItemButtonIndex == 2)
				{
					CloseButtonFlag = 0;
					for (i = 0; i < 4; i++)
					{
						if (g_sManufactureItem.ManufactureItem[i].Flag)
						{
							if (ManufactureItem.ManufactureCheckEmptyArea(&g_sManufactureItem.ManufactureItem[i]))
							{
								CloseButtonFlag = 0;
							}
							else
							{
								CloseButtonFlag = 1;
								break;
							}
							SheltomCount++;
						}
					}
					if (!CloseButtonFlag && SheltomCount <= ManufactureItem.GetManufactureCheckEmptyArea())
					{
						ManufactureItem.CancelManufactureItem();
					}
					else
					{
						int LineCount = 0, Temp;
						ShowItemOverSpace = 1;
						lstrcpy(szCraftItemBuff, InvenSpaceSecure);

						strcat(szCraftItemBuff, "\r");
						Temp = lstrlen(szCraftItemBuff);
						for (i = 0; i < Temp; i++)
						{
							if (szCraftItemBuff[i] == '\r')
							{
								LineCount++;
							}
						}
						CraftItemMessageSize.x = 14;
						CraftItemMessageSize.y = ((LineCount) * 2) + 2;
						sinPlaySound(SIN_SOUND_FAILD_MIXITEM);
					}
				}
				else if (CraftItemButtonIndex == 1)
				{
					if (g_sManufactureItem.ManufactureItem[0].Flag && !g_sManufactureItem.ManufactureItem[0].sItemInfo.ItemKindCode)
					{
						if (g_sManufactureItem.ManufactureItem[1].Flag && g_sManufactureItem.ManufactureItem[2].Flag && g_sManufactureItem.ManufactureItem[3].Flag)
						{
							SManufacture_ResultItemInfo ResultItem;
							memset(&ResultItem, 0, sizeof(SManufacture_ResultItemInfo));
							sITEM temp;
							ResultItem = ManufactureItem.ManufactureCheckItem(g_sManufactureItem.ManufactureItem, sinChar);
							for (int cnt = 0; cnt < MAX_ITEM; cnt++)
							{
								if (sItem[cnt].CODE == ResultItem.ItemCode)
								{
									temp = sItem[cnt];
									if (cInvenTory.CheckInvenEmpty(&temp))
									{
										if (sinChar->Weight[0] + ResultItem.Weight <= sinChar->Weight[1])
										{
											if (ForceItemPrice2 <= sinChar->Money)
											{
												if (!sinCraftItemStartFlag2)
												{
													cMessageBox.ShowMessage3(MESSAGE_MANUFACTURE, ResultItem.ItemName);
												}
											}
											else
											{
												cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);
											}
										}
										else
										{
											cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
										}
									}
									else
									{
										cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
									}
									break;
								}
							}
						}
					}
				}
			}
		}
	}
}

/*----------------------------------------------------------------------------*
*							   LButtonUp
*-----------------------------------------------------------------------------*/
void cTRADE::LButtonUp(int x, int y)
{

}
/*----------------------------------------------------------------------------*
*							   RButtonDown
*-----------------------------------------------------------------------------*/

void cTRADE::RButtonDown(int x, int y)
{
	Point2D sPoint(x, y);

	if (cCraftItem.OpenFlag)
	{
		sITEM* psItemData = NULL;
		for (int i = 1; i < 15; i++)
		{
			sITEM* psCurrentItem = sCraftItem.CraftItem + i;

			if (!psCurrentItem->Flag)
				continue;

			Rectangle2D rRectangle;
			rRectangle.iX = psCurrentItem->x;
			rRectangle.iY = psCurrentItem->y;
			rRectangle.iWidth = psCurrentItem->w;
			rRectangle.iHeight = psCurrentItem->h;

			if (rRectangle.Inside(&sPoint))
			{
				psItemData = psCurrentItem;
				break;
			}
		}

		if (!psItemData)
			return;

		cInvenTory.LastSetInvenItem(psItemData);
		psItemData->Flag = FALSE;
	}

	//Clique direito para remover itens do aging para o invent?io
	if (cAging.OpenFlag) {
		if (!sinCraftItemStartFlag2 && !MixCancelButtonDelayFlag) {
			if (cCraftItem.PickUpCraftItem(x, y, TRUE, 1)) {
				if (cCraftItem.CraftCheckEmptyArea(&MouseItem)) {
					MouseItem.x = MouseItem.SetX;
					MouseItem.y = MouseItem.SetY;
					cInvenTory.LastSetInvenItem(&MouseItem);
				}
				else {
					cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
				}
			}
		}
	}

	//Clique direito para remover itens da limpeza para o invent�rio
	if (cMixtureReset.OpenFlag) {
		if (!sinCraftItemStartFlag2 && !MixCancelButtonDelayFlag) {
			if (cMixtureReset.PickUpMixtureResetItem(x, y, TRUE)) {
				if (cCraftItem.CraftCheckEmptyArea(&MouseItem)) {
					MouseItem.x = MouseItem.SetX;
					MouseItem.y = MouseItem.SetY;
					cInvenTory.LastSetInvenItem(&MouseItem);
				}
				else {
					cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
				}
			}
		}
	}

	//Clique direito para remover itens da fundi��o para o invent�rio
	if (SmeltingItem.OpenFlag) {
		if (!sinCraftItemStartFlag2 && !MixCancelButtonDelayFlag) {
			if (SmeltingItem.PickUpSmeltingItem(x, y, TRUE)) {
				if (cCraftItem.CraftCheckEmptyArea(&MouseItem)) {
					MouseItem.x = MouseItem.SetX;
					MouseItem.y = MouseItem.SetY;
					cInvenTory.LastSetInvenItem(&MouseItem);
				}
				else {
					cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
				}
			}
		}
	}

	//Clique direito para remover itens da cria��o para o invent�rio
	if (ManufactureItem.m_OpenFlag) {
		if (!sinCraftItemStartFlag2 && !MixCancelButtonDelayFlag) {
			if (ManufactureItem.PickUpManufactureItem(x, y, TRUE)) {
				if (cCraftItem.CraftCheckEmptyArea(&MouseItem)) {
					MouseItem.x = MouseItem.SetX;
					MouseItem.y = MouseItem.SetY;
					cInvenTory.LastSetInvenItem(&MouseItem);
				}
				else {
					cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
				}
			}
		}
	}

	//Clique direito para remover itens da troca para o invent�rio
	if (cTrade.OpenFlag) {
		if (cTrade.PickUpTradeItem(x, y, TRUE)) {
			if (cCraftItem.CraftCheckEmptyArea(&MouseItem)) {
				MouseItem.x = MouseItem.SetX;
				MouseItem.y = MouseItem.SetY;
				cInvenTory.LastSetInvenItem(&MouseItem);
			}
			else {
				cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
			}
		}
	}

	//Clique direito para remover itens do armaz�m para o invent�rio
	if (cWareHouse.OpenFlag)
	{
		if (!cInvenTory.CheckSetOk(&MouseItem, 1))
		{
			cMessageBox.ShowMessageEvent("Excede o peso limite.");
			return;
		}

		if (cWareHouse.PickUpWareHouseItem(x, y, TRUE))
		{
			if (cCraftItem.CraftCheckEmptyArea(&MouseItem))
			{
				MouseItem.x = MouseItem.SetX;
				MouseItem.y = MouseItem.SetY;
				cInvenTory.LastSetInvenItem(&MouseItem);
			}
			else
			{
				cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
			}
		}
	}

	//Clique direito para remover itens do armaz�m para o invent�rio
	if (Caravana::GetInstance()->OpenFlag)
	{
		if (Caravana::GetInstance()->PickUpCaravanItem(x, y, TRUE))
		{
			if (cCraftItem.CraftCheckEmptyArea(&MouseItem)) {
				MouseItem.x = MouseItem.SetX;
				MouseItem.y = MouseItem.SetY;
				cInvenTory.LastSetInvenItem(&MouseItem);
				cInvenTory.ReFormPotionNum();
			}
			else {
				cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
			}
		}
	}
}
/*----------------------------------------------------------------------------*
*							   RButtonUp
*-----------------------------------------------------------------------------*/
void cTRADE::RButtonUp(int x, int y)
{

}
/*----------------------------------------------------------------------------*
*							    KeyDown
*-----------------------------------------------------------------------------*/
void cTRADE::KeyDown()
{
	/*
		if(sinGetKeyClick('M')){
			lstrcpy(szCraftItemBuff,"���Ӽ� +5\r��� +10\r���߷� +50\r �ִ���ݷ�+5\r"); //������ ������ �����ش�
			CraftItemMessageSize.x = 14;           //�ڽ� ������
			CraftItemMessageSize.y = ((3)*2)+2;  //�ڽ� ������


		}
	*/
}

extern int AgingEvento;
extern int MixEvento;


void cTRADE::DrawTradeText()
{
	BaseX = (WinSizeX - 260) / 2;
	BaseY = (WinSizeY - 161) / 2;
	SIZE size = {};

	if (ShowItemAgingMessageFlag || ShowItemCraftMessageFlag || TradeRequestFlag || ShowItemSmeltingMessageFlag || ShowItemManufactureMessageFlag || ShowItemOverSpace || sinMoveKindInter[SIN_TRADE] ||
		sinMoveKindInter[SIN_CRAFTITEM] || sinMoveKindInter[SIN_WAREHOUSE] || sinMoveKindInter[SIN_AGING] || sinMoveKindInter[SIN_SMELTING] || sinMoveKindInter[SIN_MANUFACTURE] || sinMoveKindInter[SIN_CARAVANA]) {

		int i;
		HDC hdc = NULL;

		char strBuff[128];

		memset(strBuff, 0, sizeof(strBuff));
		SelectObject(hdc, sinFont);
		SetFontTextColor(RGB(255, 255, 255));

		if (TradeRequestFlag)
		{
			if (cTrade.szTradeCharName[0])
			{
				SetFontTextColor(RGB(244, 209, 66));
				strcpy_s(strBuff, "Confirmar Pedido");
				GetFontTextExtentPoint(strBuff, strlen(strBuff), &size);
				dsTextLineOut(BaseX + (114 - size.cx / 2), BaseY + 17, strBuff, strlen(strBuff));

				SetFontTextColor(RGB(255, 255, 255));
				wsprintf(strBuff, "Fazer Troca com %s ?", cTrade.szTradeCharName);
				GetFontTextExtentPoint(strBuff, strlen(strBuff), &size);
				dsTextLineOut(BaseX + (118 - size.cx / 2), BaseY + 62, strBuff, strlen(strBuff));
			}

		}

		if (cWareHouse.OpenFlag)
		{
			memset(strBuff, 0, sizeof(strBuff));
			NumLineComa(sWareHouse.Money - 2023, strBuff);
			dsTextLineOut(hdc, CheckEditSize(ShopGoldEdit[0][0], ShopGoldEdit[0][2], strBuff) - (256 + 128 - sinMoveKindInter[SIN_WAREHOUSE]), ShopGoldEdit[0][1],
				strBuff, lstrlen(strBuff));

			memset(strBuff, 0, sizeof(strBuff));
			wsprintf(strBuff, "%d/%d", sWareHouse.Weight[0] - 197, sWareHouse.Weight[1] - 196);
			dsTextLineOut(hdc, CheckEditSize(ShopGoldEdit[1][0], ShopGoldEdit[1][2], strBuff) - (256 + 128 - sinMoveKindInter[SIN_WAREHOUSE]), ShopGoldEdit[1][1],
				strBuff, lstrlen(strBuff));

			for (i = 0; i < 100; i++) {
				if (sWareHouse.WareHouseItem[i].Flag) {
					if (sWareHouse.WareHouseItem[i].Class == ITEM_CLASS_POTION) {
						wsprintf(strBuff, "%d", sWareHouse.WareHouseItem[i].sItemInfo.PotionCount);
						dsTextLineOut(hdc, sWareHouse.WareHouseItem[i].x - (256 + 128 - sinMoveKindInter[SIN_WAREHOUSE]), sWareHouse.WareHouseItem[i].y, strBuff, lstrlen(strBuff));
					}
				}
			}
		}

		if (Caravana::GetInstance()->OpenFlag)
		{
			memset(strBuff, 0, sizeof(strBuff));
			//NumLineComa(sCaravan.Money - 2023, strBuff);

			dsTextLineOut(hdc, 240 - lstrlen(Caravana::GetInstance()->myCaravan.caravanName.c_str()) - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), ShopGoldEdit[0][1],
				Caravana::GetInstance()->myCaravan.caravanName.c_str(), lstrlen(Caravana::GetInstance()->myCaravan.caravanName.c_str()));

			memset(strBuff, 0, sizeof(strBuff));
			wsprintf(strBuff, "%d/%d", sCaravan.Weight[0] - 197, sCaravan.Weight[1] - 196);
			dsTextLineOut(hdc, CheckEditSize(ShopGoldEdit[1][0], ShopGoldEdit[1][2], strBuff) - (245 + 128 - sinMoveKindInter[SIN_CARAVANA]), 5 + ShopGoldEdit[1][1],
				strBuff, lstrlen(strBuff));

			for (i = 0; i < 100; i++) {
				if (sCaravan.CaravanItem[i].Flag) {
					if (sCaravan.CaravanItem[i].Class == ITEM_CLASS_POTION) {
						wsprintf(strBuff, "%d", sCaravan.CaravanItem[i].sItemInfo.PotionCount);
						dsTextLineOut(hdc, sCaravan.CaravanItem[i].x - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), sCaravan.CaravanItem[i].y, strBuff, lstrlen(strBuff));
					}
				}
			}
		}
		// nome trade
		if (OpenFlag) {
			lstrcpy(strBuff, cTrade.szTradeCharName);
			dsTextLineOut(hdc, CheckEditSize(229, 332, strBuff) - (256 + 128 - sinMoveKindInter[SIN_TRADE]), 90, strBuff, lstrlen(strBuff));

			if (sTradeRecv.Weight > 0) {
				wsprintf(strBuff, "%d", sTradeRecv.Weight);
				dsTextLineOut(hdc, CheckEditSize(229, 297, strBuff) - (256 + 128 - sinMoveKindInter[SIN_TRADE]), 170, strBuff, lstrlen(strBuff));
			}

			if (sTradeRecv.Money > 0)
			{
				memset(strBuff, 0, sizeof(strBuff));
				NumLineComa(sTradeRecv.Money - 193, strBuff);
				dsTextLineOut(CheckEditSize(UpTradePosi[T_MONEY_TEXT_POSI][0], UpTradePosi[T_MONEY_TEXT_POSI][2], strBuff) - (256 + 128 - sinMoveKindInter[SIN_TRADE]),
					UpTradePosi[T_MONEY_TEXT_POSI][1], strBuff, lstrlen(strBuff));
			}

			if (sTradeRecv.Coin > 0)
			{
				memset(strBuff, 0, sizeof(strBuff));
				NumLineComa(sTradeRecv.Coin, strBuff);
				dsTextLineOut(CheckEditSize(UpTradePosi[T_COIN_TEXT_POSI][0], UpTradePosi[T_COIN_TEXT_POSI][2], strBuff) - (256 + 128 - sinMoveKindInter[SIN_TRADE]),
					UpTradePosi[T_COIN_TEXT_POSI][1], strBuff, lstrlen(strBuff));
			}


			memset(strBuff, 0, sizeof(strBuff));
			wsprintf(strBuff, "%d", sTrade.Weight);
			dsTextLineOut(CheckEditSize(DownTradePosi[T_WEIGHT_TEXT_POSI][0], DownTradePosi[T_WEIGHT_TEXT_POSI][2], strBuff) - (256 + 128 - sinMoveKindInter[SIN_TRADE]),
				DownTradePosi[T_WEIGHT_TEXT_POSI][1], strBuff, lstrlen(strBuff));

			memset(strBuff, 0, sizeof(strBuff));
			NumLineComa(TempShowMoney, strBuff);
			dsTextLineOut(CheckEditSize(DownTradePosi[T_MONEY_TEXT_POSI][0], DownTradePosi[T_MONEY_TEXT_POSI][2], strBuff) - (256 + 128 - sinMoveKindInter[SIN_TRADE]),
				DownTradePosi[T_MONEY_TEXT_POSI][1], strBuff, lstrlen(strBuff));

			memset(strBuff, 0, sizeof(strBuff));
			NumLineComa(TempShowCoin, strBuff);
			dsTextLineOut(CheckEditSize(DownTradePosi[T_COIN_TEXT_POSI][0], DownTradePosi[T_COIN_TEXT_POSI][2], strBuff) - (256 + 128 - sinMoveKindInter[SIN_TRADE]),
				DownTradePosi[T_COIN_TEXT_POSI][1], strBuff, lstrlen(strBuff));

			for (int i = 0; i < MAX_TRADE_ITEM; i++) {
				if (sTrade.TradeItem[i].Flag) {
					if (sTrade.TradeItem[i].Class == ITEM_CLASS_POTION) {
						wsprintf(strBuff, "%d", sTrade.TradeItem[i].sItemInfo.PotionCount);
						dsTextLineOut(hdc, sTrade.TradeItem[i].x - (256 + 128 - sinMoveKindInter[SIN_TRADE]), sTrade.TradeItem[i].y, strBuff, lstrlen(strBuff));

					}
				}
				if (sTradeRecv.TradeItem[i].Flag) {
					if (sTradeRecv.TradeItem[i].Class == ITEM_CLASS_POTION) {
						wsprintf(strBuff, "%d", sTradeRecv.TradeItem[i].sItemInfo.PotionCount);
						dsTextLineOut(hdc, sTradeRecv.TradeItem[i].x - (256 + 128 - sinMoveKindInter[SIN_TRADE]), sTradeRecv.TradeItem[i].y - T_REVISION_Y, strBuff, lstrlen(strBuff));

					}
				}
			}
		}

		char szMoneyTemp[256];

		if (sAgingItem.AgingItem[0].Flag)
		{
			if (AgingEvento == TRUE)
			{
#ifdef HASIEGE_MODE
				wsprintf(szMoneyTemp, "%d", 0);
#else
				wsprintf(szMoneyTemp, "%d", 0);
#endif
			}
			else
			{
#ifdef HASIEGE_MODE
				// F�rmula de valor do aging
				wsprintf(szMoneyTemp, "%d", cShop.haShopItemPrice((sAgingItem.AgingItem[0].sItemInfo.Price + (sAgingItem.AgingItem[0].sItemInfo.Price * sAgingItem.AgingItem[0].sItemInfo.ItemAgingNum[0])) / 2));
#else
				wsprintf(szMoneyTemp, "%d", (sAgingItem.AgingItem[0].sItemInfo.Price + (sAgingItem.AgingItem[0].sItemInfo.Price * sAgingItem.AgingItem[0].sItemInfo.ItemAgingNum[0])) / 2);
#endif
			}
			extern DWORD SpecialSheltomCode;
			int AgingStoneX = 159 + 11, AgingStoneY = 268 + 11 + sinInterHeight2;
			int Chance = 0;
			int ChanceQuebra = 0;
			char chanceStr[32] = { 0 };
			char chanceStrQuebra[32] = { 0 };

			if (SpecialSheltomCode == (sinBI1 | sin10))
				Chance = AgingStoneAgingOkPercent[sAgingItem.AgingItem[0].sItemInfo.ItemAgingNum[0]];
			else if (SpecialSheltomCode == (sinBI1 | sin85))
				Chance = 0;
			else
				Chance = AgingOkPercent[sAgingItem.AgingItem[0].sItemInfo.ItemAgingNum[0]];

			if (sAgingItem.AgingItem[0].sItemInfo.ItemAgingNum[0] >= 5)
			{
				if (SpecialSheltomCode == (sinBI1 | sin11))
					ChanceQuebra = 0;
				else
					ChanceQuebra = chanceQuebra[sAgingItem.AgingItem[0].sItemInfo.ItemAgingNum[0]];
			}

			sprintf_s(chanceStr, sizeof(chanceStr), "Falha: %d%%", Chance);
			sprintf_s(chanceStrQuebra, sizeof(chanceStrQuebra), "Quebra: %d%%", ChanceQuebra);

			dsTextLineOut(hdc, CheckEditSize(200, 325, szMoneyTemp), 318 + sinInterHeight2, szMoneyTemp, lstrlen(szMoneyTemp));
			dsTextLineOut(hdc, CheckEditSize(200, 1, chanceStr), 310 + sinInterHeight2, chanceStr, lstrlen(chanceStr));
			dsTextLineOut(hdc, CheckEditSize(200, 1, chanceStrQuebra), 324 + sinInterHeight2, chanceStrQuebra, lstrlen(chanceStrQuebra));
		}

		if (sCraftItem.CraftItem[0].Flag)
		{
			if (MixEvento == TRUE)
			{
				wsprintf(szMoneyTemp, "%d", 0);
			}
			else
			{
#ifdef HASIEGE_MODE
				wsprintf(szMoneyTemp, "%d", cShop.haShopItemPrice(sCraftItem.CraftItem[0].sItemInfo.Price));
#else
				wsprintf(szMoneyTemp, "%d", sCraftItem.CraftItem[0].sItemInfo.Price);
#endif
			}
			dsTextLineOut(hdc, CheckEditSize(200, 325, szMoneyTemp), 318 + sinInterHeight2, szMoneyTemp, lstrlen(szMoneyTemp));
		}

		if (cCraftItem.ForceFlag) {
#ifdef HASIEGE_MODE
			wsprintf(szMoneyTemp, "%d", cShop.haShopItemPrice(ForceItemPrice2));
#else
			wsprintf(szMoneyTemp, "%d", ForceItemPrice2);
#endif
			dsTextLineOut(hdc, CheckEditSize(200, 325, szMoneyTemp) - (256 + 128 - sinMoveKindInter[SIN_CRAFTITEM]), 318 + sinInterHeight2, szMoneyTemp, lstrlen(szMoneyTemp));

		}

		if (S_smeltingItem.SmeltingItem[0].Flag)
		{
			wsprintf(szMoneyTemp, "%d", ForceItemPrice2);
			dsTextLineOut(hdc, CheckEditSize(200, 325, szMoneyTemp), 318 + sinInterHeight2, szMoneyTemp, lstrlen(szMoneyTemp));
		}

		if (g_sManufactureItem.ManufactureItem[0].Flag)
		{
			wsprintf(szMoneyTemp, "%d", ForceItemPrice2);
			dsTextLineOut(hdc, CheckEditSize(200, 325, szMoneyTemp), 318 + sinInterHeight2, szMoneyTemp, lstrlen(szMoneyTemp));
		}

		char* pCraftItemInfo[10];

		int len, Templen = 0, Textlen = 0;
		int Count = 0;
		int SetTextXposi = 0;
		char TempszCraftItemBuff[128];


		lstrcpy(TempszCraftItemBuff, szCraftItemBuff);

		if (ShowItemCraftMessageFlag || ShowItemAgingMessageFlag || ShowItemSmeltingMessageFlag || ShowItemManufactureMessageFlag || ShowItemOverSpace) {
			SetFontTextColor(RGB(222, 231, 255));
			SelectObject(hdc, sinBoldFont);
			if (ShowItemCraftMessageFlag)
				dsTextLineOut(hdc, CraftItemMessageBoxPosi.x + 53, CraftItemMessageBoxPosi.y + 13, MixResultTitle, lstrlen(MixResultTitle));

			if (ShowItemSmeltingMessageFlag)
			{
				dsTextLineOut(hdc, CraftItemMessageBoxPosi.x + 53, CraftItemMessageBoxPosi.y + 13, SmeltingResultTitle, lstrlen(SmeltingResultTitle));
			}
			if (ShowItemManufactureMessageFlag)
			{
				dsTextLineOut(hdc, CraftItemMessageBoxPosi.x + 53, CraftItemMessageBoxPosi.y + 13, ManufactureResultTitle, lstrlen(ManufactureResultTitle));
			}
			if (ShowItemOverSpace)
			{
				dsTextLineOut(hdc, CraftItemMessageBoxPosi.x + 53, CraftItemMessageBoxPosi.y + 13, InventoryFull, lstrlen("InventoryFull"));
			}
			SelectObject(hdc, sinFont);
			SetFontTextColor(RGB(255, 255, 255));

			len = lstrlen(TempszCraftItemBuff);
			for (i = 0; i < len; i++) {
				if (TempszCraftItemBuff[i] == '\r') {
					pCraftItemInfo[Count] = &TempszCraftItemBuff[Templen];
					TempszCraftItemBuff[i] = 0;
					Templen = i + 1;
					Textlen = lstrlen(pCraftItemInfo[Count]);
					SetTextXposi = ((CraftItemMessageBoxPosi.x + ((CraftItemMessageSize.x * 16) / 2)) - ((Textlen / 2) * 11));
					if (Textlen <= 12)
						SetTextXposi -= 6;

					dsTextLineOut(hdc, SetTextXposi + 27, CraftItemMessageBoxPosi.y + 38 + ((Count) * 18), pCraftItemInfo[Count], Textlen);
					Count++;
				}

			}
		}

	}

}


int cTRADE::SetTradeItemAreaCheck(sITEM* pItem)
{

	int i, j;
	TradeColorIndex = 0;
	TradeCrashItemIndex[0] = 0;
	InitTradeColorRect();

	TradeStartX = DownTradePosi[T_SET_BOX][0];
	TradeStartY = DownTradePosi[T_SET_BOX][1];
	TradeEndX = DownTradePosi[T_SET_BOX][2];
	TradeEndY = DownTradePosi[T_SET_BOX][3];

	for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22) {
		for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22) {
			if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j) {
				TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
				TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
				TradeColorRect.right = pItem->w;
				TradeColorRect.bottom = pItem->h;
				if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
					InitTradeColorRect();
					return FALSE;
				}

				pItem->SetX = TradeColorRect.left;
				pItem->SetY = TradeColorRect.top;
				pItem->ItemPosition = 0;
				TradeColorIndex = SET_ITEM_CHECK_COLOR;

				if (TradeCrashItemIndex[0] = CrashTradeItem(TradeColorRect)) {
					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;
				}

				if (pItem->sItemInfo.SpecialItemFlag[0] == CHECK_COPY_ITEM ||
					pItem->sItemInfo.SpecialItemFlag[0] == CHECK_GIVE_ITEM ||
					pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinQW1 ||

					(pItem->sItemInfo.CODE == (sinDB1 | sin31)) || (pItem->sItemInfo.CODE == (sinDB1 | sin32)) ||
					pItem->sItemInfo.CODE == (sinGF1 | sin01) || (pItem->sItemInfo.CODE == (sinOR2 | sin33)) ||
					(pItem->sItemInfo.CODE == (sinOA1 | sin36)) ||
					(pItem->sItemInfo.CODE == (sinOA1 | sin37)) ||
					(pItem->sItemInfo.CODE == (sinDB1 | sin33)) ||
					(pItem->sItemInfo.CODE == (sinOA2 | sin33)) ||
					(pItem->sItemInfo.CODE == (sinBI1 | sin84)) ||
					(pItem->sItemInfo.CODE == (sinDB1 | sin34)) ||
					(pItem->sItemInfo.CODE == (sinOA2 | sin34)) ||
					pItem->sItemInfo.CODE == (sinGF1 | sin07) ||
					pItem->sItemInfo.CODE == (sinGF1 | sin08) ||
					pItem->ItemPosition) {
					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;

				}

				int kk = 0;
				for (kk = 0; kk < NotSet_Item_CODECnt; kk++) {
					if (NotSet_Item_CODE[kk] == pItem->sItemInfo.CODE) {
						TradeColorIndex = NOT_SETTING_COLOR;
						return FALSE;
					}

				}
				for (kk = 0; kk < NotSet_Item_MASKCnt; kk++) {
					if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == NotSet_Item_MASK[kk]) {
						TradeColorIndex = NOT_SETTING_COLOR;
						return FALSE;
					}
				}
				for (kk = 0; kk < NotSet_Item_KINDCnt; kk++) {
					if (NotSet_Item_KIND[kk] == pItem->sItemInfo.ItemKindCode) {
						TradeColorIndex = NOT_SETTING_COLOR;
						return FALSE;
					}
				}
				return TRUE;
			}

		}

	}

	return TRUE;
}

int cTRADE::InitTradeColorRect()
{
	TradeColorRect.left = 0;
	TradeColorRect.top = 0;
	TradeColorRect.right = 0;
	TradeColorRect.bottom = 0;
	return TRUE;

}

int cTRADE::CrashTradeItem(RECT& desRect, int PassItemIndex, int Kind)
{

	RECT rect;
	int i;
	if (Kind == 0) {
		for (i = 0; i < MAX_TRADE_ITEM; i++) {
			if (sTrade.TradeItem[i].Flag) {
				if (PassItemIndex != 0)
					if (PassItemIndex == i + 1)continue;
				rect.left = sTrade.TradeItem[i].x;
				rect.right = sTrade.TradeItem[i].x + sTrade.TradeItem[i].w;
				rect.top = sTrade.TradeItem[i].y;
				rect.bottom = sTrade.TradeItem[i].y + sTrade.TradeItem[i].h;

				if (desRect.left > rect.left - desRect.right && desRect.left < rect.right &&
					desRect.top > rect.top - desRect.bottom && desRect.top < rect.bottom) {
					return i + 1;
				}
			}
		}
	}
	if (Kind == 1) {
		for (i = 0; i < 100; i++) {
			if (sWareHouse.WareHouseItem[i].Flag) {
				if (PassItemIndex != 0)
					if (PassItemIndex == i + 1)continue;
				rect.left = sWareHouse.WareHouseItem[i].x;
				rect.right = sWareHouse.WareHouseItem[i].x + sWareHouse.WareHouseItem[i].w;
				rect.top = sWareHouse.WareHouseItem[i].y;
				rect.bottom = sWareHouse.WareHouseItem[i].y + sWareHouse.WareHouseItem[i].h;

				if (desRect.left > rect.left - desRect.right && desRect.left < rect.right &&
					desRect.top > rect.top - desRect.bottom && desRect.top < rect.bottom) {
					return i + 1;
				}
			}
		}
	}
	if (Kind == 2) {
		for (i = 0; i < 15; i++) {
			if (sCraftItem.CraftItem[i].Flag) {
				if (PassItemIndex != 0)
					if (PassItemIndex == i + 1)continue;
				rect.left = sCraftItem.CraftItem[i].x;
				rect.right = sCraftItem.CraftItem[i].x + sCraftItem.CraftItem[i].w;
				rect.top = sCraftItem.CraftItem[i].y;
				rect.bottom = sCraftItem.CraftItem[i].y + sCraftItem.CraftItem[i].h;

				if (desRect.left > rect.left - desRect.right && desRect.left < rect.right &&
					desRect.top > rect.top - desRect.bottom && desRect.top < rect.bottom) {
					return i + 1;
				}
			}
		}
	}
	if (Kind == 3) {
		for (i = 0; i < 15; i++) {
			if (sAgingItem.AgingItem[i].Flag) {
				if (PassItemIndex != 0)
					if (PassItemIndex == i + 1)continue;
				rect.left = sAgingItem.AgingItem[i].x;
				rect.right = sAgingItem.AgingItem[i].x + sAgingItem.AgingItem[i].w;
				rect.top = sAgingItem.AgingItem[i].y;
				rect.bottom = sAgingItem.AgingItem[i].y + sAgingItem.AgingItem[i].h;

				if (desRect.left > rect.left - desRect.right && desRect.left < rect.right &&
					desRect.top > rect.top - desRect.bottom && desRect.top < rect.bottom) {
					return i + 1;
				}
			}
		}
	}

	if (Kind == 4)
	{
		for (i = 0; i < 5; i++)
		{
			if (S_smeltingItem.SmeltingItem[i].Flag)
			{
				if (PassItemIndex != 0)
				{
					if (PassItemIndex == i + 1) continue;
				}
				rect.left = S_smeltingItem.SmeltingItem[i].x;
				rect.right = S_smeltingItem.SmeltingItem[i].x + S_smeltingItem.SmeltingItem[i].w;
				rect.top = S_smeltingItem.SmeltingItem[i].y;
				rect.bottom = S_smeltingItem.SmeltingItem[i].y + S_smeltingItem.SmeltingItem[i].h;

				if (desRect.left > rect.left - desRect.right && desRect.left < rect.right &&
					desRect.top > rect.top - desRect.bottom && desRect.top < rect.bottom)
				{
					return i + 1;
				}
			}
		}
	}

	if (Kind == 5)
	{
		for (i = 0; i < 4; i++)
		{
			if (g_sManufactureItem.ManufactureItem[i].Flag)
			{
				if (PassItemIndex != 0)
				{
					if (PassItemIndex == i + 1) continue;
				}
				rect.left = g_sManufactureItem.ManufactureItem[i].x;
				rect.right = g_sManufactureItem.ManufactureItem[i].x + g_sManufactureItem.ManufactureItem[i].w;
				rect.top = g_sManufactureItem.ManufactureItem[i].y;
				rect.bottom = g_sManufactureItem.ManufactureItem[i].y + g_sManufactureItem.ManufactureItem[i].h;

				if (desRect.left > rect.left - desRect.right && desRect.left < rect.right &&
					desRect.top > rect.top - desRect.bottom && desRect.top < rect.bottom)
				{
					return i + 1;
				}
			}
		}
	}

	if (Kind == 6)
	{
		for (i = 0; i < 2; i++)
		{
			if (sMixtureResetItem.MixtureResetItem[i].Flag)
			{
				if (PassItemIndex != 0)
				{
					if (PassItemIndex == i + 1)
						continue;
				}

				rect.left = sMixtureResetItem.MixtureResetItem[i].x;
				rect.right = sMixtureResetItem.MixtureResetItem[i].x + sMixtureResetItem.MixtureResetItem[i].w;
				rect.top = sMixtureResetItem.MixtureResetItem[i].y;
				rect.bottom = sMixtureResetItem.MixtureResetItem[i].y + sMixtureResetItem.MixtureResetItem[i].h;

				if (desRect.left > rect.left - desRect.right && desRect.left < rect.right &&
					desRect.top > rect.top - desRect.bottom && desRect.top < rect.bottom)
				{
					return i + 1;
				}
			}
		}
	}

	if (Kind == 7) {
		for (i = 0; i < 100; i++) {
			if (sCaravan.CaravanItem[i].Flag) {
				if (PassItemIndex != 0)
					if (PassItemIndex == i + 1)continue;
				rect.left = sCaravan.CaravanItem[i].x;
				rect.right = sCaravan.CaravanItem[i].x + sCaravan.CaravanItem[i].w;
				rect.top = sCaravan.CaravanItem[i].y;
				rect.bottom = sCaravan.CaravanItem[i].y + sCaravan.CaravanItem[i].h;

				if (desRect.left > rect.left - desRect.right && desRect.left < rect.right &&
					desRect.top > rect.top - desRect.bottom && desRect.top < rect.bottom) {
					return i + 1;
				}
			}
		}
	}

	return FALSE;

}

bool cTRADE::IsItemOkay(sITEM* pItem)
{
	if (pItem->sItemInfo.SpecialItemFlag[0] == CHECK_COPY_ITEM ||
		pItem->sItemInfo.SpecialItemFlag[0] == CHECK_GIVE_ITEM ||
		pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
		(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
		(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
		(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinQW1)
	{
		return false;
	}

	int kk = 0;
	//��ϵ� �������� Ʈ���̵� �Ҽ�����
	for (kk = 0; kk < NotSet_Item_CODECnt; kk++) {
		if (NotSet_Item_CODE[kk] == pItem->sItemInfo.CODE) {
			return false;
		}
	}
	for (kk = 0; kk < NotSet_Item_KINDCnt; kk++) {
		if (NotSet_Item_KIND[kk] == pItem->sItemInfo.ItemKindCode) {
			return false;
		}
	}
	return true;
}

bool cTRADE::GetEmptyArea(sITEM* pItem, POINT* EmptyPos)
{
	int X = DownTradePosi[T_SET_BOX][0];
	int Y = DownTradePosi[T_SET_BOX][1];
	int Max_X = DownTradePosi[T_SET_BOX][2];
	int Max_Y = DownTradePosi[T_SET_BOX][3];

	RECT Rectangle = { 0 };
	int Indexs[2] = { 0 };

	for (int i = 21; i < Max_X; i += 22) {
		for (int j = Y; j < Max_Y; j += 22) {
			if (X <= i && Max_X > i && Y <= j && Max_Y > j) {
				Rectangle.left = X + (((i - X) / 22) * 22);
				Rectangle.top = Y + (((j - Y) / 22) * 22);
				Rectangle.right = pItem->w;
				Rectangle.bottom = pItem->h;

				if (Rectangle.left + Rectangle.right > Max_X)
					continue;
				if (Rectangle.top + Rectangle.bottom > Max_Y)
					continue;
			}

			if (Indexs[0] = cTrade.CrashTradeItem(Rectangle, 0, 0)) {
				if (Indexs[1] = cTrade.CrashTradeItem(Rectangle, Indexs[0], 0)) {
					continue;
				}
				continue;
			}

			EmptyPos->x = i;
			EmptyPos->y = j;
			return true;
		}
	}
	return false;
}

int cTRADE::LastSetTradeItem(sITEM* pItem)
{
	CheckTradeItemForm();

	for (int j = 0; j < MAX_TRADE_ITEM; j++) {
		if (!sTrade.TradeItem[j].Flag) {
			if (!CheckTrade((void*)&sTrade))
				CancelTradeItem();

			memcpy(&sTrade.TradeItem[j], pItem, sizeof(sITEM));
			sTrade.TradeItem[j].x = pItem->SetX;
			sTrade.TradeItem[j].y = pItem->SetY;
			pItem->Flag = 0;
			sTrade.CheckFlag = 0;
			sinPlaySound(sTrade.TradeItem[j].SoundIndex);
			ReformTrade((void*)&sTrade);
			GetWeight();
			SendTradeItem(TradeCharCode);
			ReFormTradeItem();
			return TRUE;

		}
	}
	cMessageBox.ShowMessage(MESSAGE_TRADE_OVER_NUM);
	ReFormTradeItem();
	return FALSE;
}

int cTRADE::PickUpTradeItem(int x, int y, int PickUpFlag)
{
	int i = 0;
	SelectTradeItemIndex = 0;
	ItemKindFlag = 0;
	for (i = 0; i < MAX_TRADE_ITEM; i++) {
		if (sTrade.TradeItem[i].Flag) {
			if (sTrade.TradeItem[i].x < x && sTrade.TradeItem[i].x + sTrade.TradeItem[i].w > x &&
				sTrade.TradeItem[i].y < y && sTrade.TradeItem[i].y + sTrade.TradeItem[i].h > y) {
				if (PickUpFlag) {
					CheckTradeItemForm();
					if (!CheckTrade((void*)&sTrade))
						CancelTradeItem();
					memcpy(&MouseItem, &sTrade.TradeItem[i], sizeof(sITEM));
					sTrade.TradeItem[i].Flag = 0;
					sinPlaySound(sTrade.TradeItem[i].SoundIndex);
					TradeColorIndex = 0;
					CheckDelayFlag = 1;
					sTrade.CheckFlag = 0;
					ReformTrade((void*)&sTrade);
					GetWeight();
					SendTradeItem(TradeCharCode);
					ReFormTradeItem();
					return TRUE;
				}
				else {
					SelectTradeItemIndex = i + 1;
					TradeColorIndex = SELECT_ITEM_COLOR;
					ItemKindFlag = 1;
					sinShowItemInfoFlag = 1;

					GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&sTrade.TradeItem[i], FALSE, FALSE, FALSE);
				}
			}
		}
	}
	for (i = 0; i < MAX_TRADE_ITEM; i++) {
		if (sTradeRecv.TradeItem[i].Flag) {
			if (sTradeRecv.TradeItem[i].x < x && sTradeRecv.TradeItem[i].x + sTradeRecv.TradeItem[i].w > x &&
				sTradeRecv.TradeItem[i].y - T_REVISION_Y < y && sTradeRecv.TradeItem[i].y - T_REVISION_Y + sTradeRecv.TradeItem[i].h > y) {
				SelectTradeItemIndex = i + 1;
				TradeColorIndex = SELECT_ITEM_COLOR;
				ItemKindFlag = 2;
				sinShowItemInfoFlag = 1;
				sTradeRecv.TradeItem[i].y -= T_REVISION_Y;
				GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&sTradeRecv.TradeItem[i], FALSE, FALSE, FALSE);
				sTradeRecv.TradeItem[i].y += T_REVISION_Y;
				return TRUE;

			}
		}
	}

	return FALSE;
}


int cTRADE::CheckTradeItemSet()
{
	int i = 0;
	int TempWeight = 0;

	memcpy(&CheckItem, &cInvenTory.InvenItem, sizeof(sITEM) * 100);
	memcpy(&CheckRecvItem, &sTradeRecv.TradeItem, sizeof(sITEM) * MAX_TRADE_ITEM);

	for (i = 0; i < MAX_TRADE_ITEM; i++) {
		if (CheckRecvItem[i].Flag) {
			if (CheckRecvItem[i].Class == ITEM_CLASS_POTION)
				TempWeight += CheckRecvItem[i].sItemInfo.PotionCount;
			else
				TempWeight += CheckRecvItem[i].sItemInfo.Weight;
		}
	}
	if (TempWeight + sinChar->Weight[0] > sinChar->Weight[1]) {
		cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
		return FALSE;

	}
	for (i = 0; i < MAX_TRADE_ITEM; i++) {
		if (CheckRecvItem[i].Flag) {
			if (!TradeCheckEmptyArea(&CheckRecvItem[i])) {
				cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
				return FALSE;
			}
		}
	}

	if (sTradeRecv.Money > 0) {
		if (!cInvenTory.CheckMoneyLimit(sTradeRecv.Money - 193))
			return FALSE;

	}

	return TRUE;
}

int cTRADE::TradeCheckEmptyArea(sITEM* pItem)
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect = { 0,0,0,0 };
	int		flag = 0;

	int StartX = 23;
	int StartY = 436;

	cx = (22 * INVENTORY_BOX_ROW) - pItem->w;
	cy = (22 * INVENTORY_BOX_COL) - pItem->h;

	for (cntx = StartX; cntx <= StartX + cx; cntx += 22) {
		for (cnty = StartY; cnty <= StartY + cy; cnty += 22) {
			flag = 0;
			for (int i = 0; i < INVENTORY_MAXITEM; i++) {
				if (CheckItem[i].Flag) {
					rect.left = CheckItem[i].x;
					rect.right = CheckItem[i].x + CheckItem[i].w;
					rect.top = CheckItem[i].y;
					rect.bottom = CheckItem[i].y + CheckItem[i].h;

					if (cntx > rect.left - pItem->w && cntx< rect.right &&
						cnty > rect.top - pItem->h && cnty < rect.bottom)
						flag++;
				}
			}
			if (flag == 0) {
				pItem->SetX = cntx;
				pItem->SetY = cnty;
				pItem->ItemPosition = 0;
				if (ChecketInvenItem(pItem))
					return TRUE;

			}
		}
	}

	return FALSE;

}

int cTRADE::ChecketInvenItem(sITEM* pItem)
{
	for (int j = 0; j < INVENTORY_MAXITEM; j++)
		if (CheckItem[j].Flag == 0) {
			memcpy(&CheckItem[j], pItem, sizeof(sITEM));
			CheckItem[j].x = pItem->SetX;
			CheckItem[j].y = pItem->SetY;
			pItem->Flag = 0;
			return TRUE;

		}

	return FALSE;
}

bool bPlaceItem = false;

int cTRADE::CheckOkTradeItem()
{
	int i = 0;
	for (i = 0; i < MAX_TRADE_ITEM; i++) {
		if (sTradeRecv.TradeItem[i].Flag) {
			//bPlaceItem = true;

			if (cInvenTory.CheckRequireItemToSet(&sTradeRecv.TradeItem[i])) {
				if (!cInvenTory.AutoSetInvenItem(&sTradeRecv.TradeItem[i])) {
					if (sinThrowItemToFeild(&sTradeRecv.TradeItem[i])) {
						sTradeRecv.TradeItem[i].Flag = 0;
					}

				}
			}

			//bPlaceItem = false;
		}
	}
	if (sTradeRecv.Money > 0) {
		CheckCharForm();
		sinPlusMoney(sTradeRecv.Money - 193);
		ReformCharForm();
		SendSaveMoney();
		sinPlaySound(SIN_SOUND_COIN);
	}
	cTrade.OpenFlag = FALSE;
	TempShowMoney = 0;
	TempShowCoin = 0;
	ItemPickUpFlag = 0;

	cInvenTory.CheckWeight();

	return TRUE;
}


int cTRADE::CancelTradeItem()
{
	int sArrowPosi = 0;

	if (TradeItemSucessFlag) {
		memset(&sTrade, 0, sizeof(sTRADE));
		memset(&sTradeRecv, 0, sizeof(sTRADE));

	}
	int i = 0;
	for (i = 0; i < MAX_TRADE_ITEM; i++) {
		if (sTrade.TradeItem[i].Flag) {
			if (cInvenTory.CheckRequireItemToSet(&sTrade.TradeItem[i])) { //��ٷ� ���õɼ��ִ����� üũ�Ѵ�  
				if (!cInvenTory.AutoSetInvenItem(&sTrade.TradeItem[i])) {
					//�������� ������������ �κ��丮�� ����� �����Ѵ�
					if (ArrowState[0] == 0)sArrowPosi = 2; //���� �κ�â�� 
					else sArrowPosi = 1;
					sinButtonFlag = sArrowPosi;
					SelectInvenItemIndex = 0; //���׸� �ھ���� �������� �ʱ�ȭ
					cInvenTory.ChangeABItem(sArrowPosi); // 2���� ������ �κ� �ڽ� 

					if (!cInvenTory.AutoSetInvenItem(&sTrade.TradeItem[i])) { //�׷��� �ȵǸ� ������ ���ڷ��� �ض�
						if (sinThrowItemToFeild(&sTrade.TradeItem[i])) {//�������� ������ 
							sTrade.TradeItem[i].Flag = 0;
						}
					}

					/*
					//���� ����������(���� ����ϰ������ʴ�)
					if(ArrowState[1]==0)sArrowPosi = 4;
					else sArrowPosi = 3;
					sinButtonFlag = sArrowPosi;
					sinChangeSetFlag = sArrowPosi;
					*/

					if (sinThrowItemToFeild(&sTrade.TradeItem[i])) {//�������� ������ 
						sTrade.TradeItem[i].Flag = 0;
					}
				}
			}
		}
	}

	if (sTrade.Money > 0) {
		CheckCharForm();//���� 
		//sinChar->Money += sTrade.Money-193;
		sinPlusMoney(sTrade.Money - 193);
		ReformCharForm();//������ 	
		SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
		sinPlaySound(SIN_SOUND_COIN);
	}


	//���콺�� �������� ������쿡�� ���콺 �����۵� �κ��� ��ٷ� �����Ѵ� 
	if (MouseItem.Flag) {
		if (cInvenTory.CheckRequireItemToSet(&MouseItem)) {         //��ٷ� ���õɼ��ִ����� üũ�Ѵ�  
			if (!cInvenTory.AutoSetInvenItem(&MouseItem)) {
				if (sinThrowItemToFeild(&MouseItem)) {//�������� ������ 
					MouseItem.Flag = 0;
				}
			}
		}

	}

	//�޼��� �ڽ� ������ ��� ��� 

	if (sinMessageBoxShowFlag) { //�޼��� �ڽ��� ������ ���� �ݴ´� 
		if (sMessageBox2[MESSAGE_MOVE_MONEY_TRADE].Flag) {
			sMessageBox2[MESSAGE_MOVE_MONEY_TRADE].Flag = 0;
			sinMessageBoxShowFlag = 0;
		}

		if (sMessageBox2[MESSAGE_MOVE_COIN_TRADE].Flag)
		{
			sMessageBox2[MESSAGE_MOVE_COIN_TRADE].Flag = 0;
			sinMessageBoxShowFlag = 0;
		}
	}

	//Ʈ���̵� ��ҽ� â�� �ݾ��ش�
	cTrade.OpenFlag = FALSE;
	cMessageBox.ShowMessage(MESSAGE_TRADE_CANCEL);

	memset(&sTrade, 0, sizeof(sTRADE));		//���õ��� ������ �ʱ�ȭ���ش� 
	memset(&sTradeRecv, 0, sizeof(sTRADE));	//���õ��� ������ �ʱ�ȭ���ش� 
	TempShowMoney = 0;
	TempShowCoin = 0;
	ItemPickUpFlag = 0; //�÷� �ʱ�ȭ 

	CloseEachPlayer();
	cInvenTory.CheckWeight();	//�κ��丮 �� ���Ը� �����Ѵ�
	return TRUE;
}

//������ �̹����� �ε��Ѵ� 
int cTRADE::LoadTradeItemIamge()
{

	char szFilePath[256];
	int  cnt = 0;
	for (int i = 0; i < MAX_TRADE_ITEM; i++) {
		if (sTradeRecv.TradeItem[i].Flag) {
			for (int j = 0; j < MAX_ITEM; j++) {
				if (sTradeRecv.TradeItem[i].sItemInfo.CODE == sItem[j].CODE) {
					wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[j].ItemFilePath, sItem[j].LastCategory);

					if (!sItem[j].lpTempItem)
						sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);

					//���� �Ӵ� ����
					if ((sTradeRecv.TradeItem[i].sItemInfo.CODE & sinITEM_MASK2) == sinPZ1 || (sTradeRecv.TradeItem[i].sItemInfo.CODE & sinITEM_MASK2) == sinPZ2) {
						sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath); //������ �ε��Ѵ�
						if (sTradeRecv.TradeItem[i].sItemInfo.PotionCount <= 1) {
							//���ָӴϸ� ã�´�.
							for (cnt = 0; cnt < MAX_ITEM; cnt++) {
								if (sItem[cnt].sItemInfo.CODE == (sinPZ1 | sin00)) {
									break;
								}
							}
							wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[cnt].ItemFilePath, sItem[cnt].LastCategory);
							sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath); //�̹����� �����о��ش�
						}
					}
					sTradeRecv.TradeItem[i].lpItem = sItem[j].lpTempItem;
					break;
				}
			}
		}
	}
	return TRUE;
}

int cTRADE::CheckTradeButtonOk()
{
	///////////////Ʈ���̵� üũ 
	if (!TradeSendSucessFlag && sTrade.CheckFlag && sTradeRecv.CheckFlag) { //��ȯ! 
		SendTradeCheckItem(TradeCharCode);
		TradeSendSucessFlag = TRUE;
	}

	return TRUE;
}

//���Ը� ���´� 
int cTRADE::GetWeight()
{
	sTrade.Weight = 0;
	for (int i = 0; i < MAX_TRADE_ITEM; i++) {
		if (sTrade.TradeItem[i].Flag) {
			if (sTrade.TradeItem[i].Class == ITEM_CLASS_POTION)
				sTrade.Weight += sTrade.TradeItem[i].sItemInfo.PotionCount;
			else
				sTrade.Weight += sTrade.TradeItem[i].sItemInfo.Weight;

		}

	}

	return TRUE;
}

bool cWAREHOUSE::CanStoreItem(sITEM* pItem)
{
	if (pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
		(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
		(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
		(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinQW1)
	{
		return false;
	}

	int kk = 0;
	for (kk = 0; kk < NotSet_Item_CODECnt; kk++) {
		if (NotSet_Item_CODE[kk] == pItem->sItemInfo.CODE) {
			return false;
		}
	}
	for (kk = 0; kk < NotSet_Item_KINDCnt; kk++) {
		if (NotSet_Item_KIND[kk] == pItem->sItemInfo.ItemKindCode) {
			return false;
		}
	}

	if (pItem->sItemInfo.ItemKindCode == ITEM_KIND_EVENT) {
		return false;
	}
	if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinCH1) {
		return false;
	}
	return true;
}

bool cWAREHOUSE::GetEmptyArea(sITEM* pItem, POINT* EmptyPos)
{
	int X = 21;
	int Y = 136 + sinInterHeight2;
	int Max_X = X + (22 * 9);
	int Max_Y = Y + (22 * 9);

	RECT Rectangle = { 0 };
	int Indexs[2] = { 0 };

	for (int i = 21; i < Max_X; i += 22) {
		for (int j = Y; j < Max_Y; j += 22) {
			if (X <= i && Max_X > i && Y <= j && Max_Y > j) {
				Rectangle.left = X + (((i - X) / 22) * 22);
				Rectangle.top = Y + (((j - Y) / 22) * 22);
				Rectangle.right = pItem->w;
				Rectangle.bottom = pItem->h;

				if (Rectangle.left + Rectangle.right > Max_X)
					continue;
				if (Rectangle.top + Rectangle.bottom > Max_Y)
					continue;
			}

			if (Indexs[0] = cTrade.CrashTradeItem(Rectangle, 0, 1)) {
				if (!CopyItemNotPickUp(&sWareHouse.WareHouseItem[Indexs[0] - 1], Indexs[0] - 1)) {
					continue;
				}
				if (Indexs[1] = cTrade.CrashTradeItem(Rectangle, Indexs[0], 1)) {
					continue;
				}
				continue;
			}

			EmptyPos->x = i;
			EmptyPos->y = j;
			return true;
		}
	}
	return false;
}

/*******************************************************************************************/
/*							���⼭ ���ʹ� â��
/*******************************************************************************************/

//�������� ���õ� ������ ã�´� 
int cWAREHOUSE::SetWareHouseItemAreaCheck(sITEM* pItem)
{
	int i, j;
	TradeColorIndex = 0; //�ʱ�ȭ
	TradeCrashItemIndex[0] = 0; //�浹�� ������ Index �ʱ�ȭ 
	cTrade.InitTradeColorRect(); //�ʱ�ȭ

	TradeStartX = 21;
	TradeStartY = 136 + sinInterHeight2;
	TradeEndX = TradeStartX + (22 * 9);
	TradeEndY = TradeStartY + (22 * 9);

	for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22) {
		for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22) {
			if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j) { //�κ��丮 �ڽ��� �������� üũ 
				TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
				TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
				TradeColorRect.right = pItem->w;
				TradeColorRect.bottom = pItem->h;
				if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
					cTrade.InitTradeColorRect(); //�÷� �ڽ� RECT �ʱ�ȭ (�÷� �ڽ� RECT�� �ϳ���� 0 �̸� �����Ҽ� ���� 
					return FALSE;
				}
				//����Ʈ �������ϰ�쿡�� �������� â�� ���������� 
				if (pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinQW1 ||
					pItem->sItemInfo.CODE == (sinGF1 | sin01)) {
					//pItem->sItemInfo.CODE == (sinGF1|sin02)){

					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;
				}
				int kk = 0;
				//��ϵ� �������� â�� ����������
				for (kk = 0; kk < NotSet_Item_CODECnt; kk++) {
					if (NotSet_Item_CODE[kk] == pItem->sItemInfo.CODE) {
						TradeColorIndex = NOT_SETTING_COLOR;
						return FALSE;
					}

				}
				for (kk = 0; kk < NotSet_Item_MASKCnt; kk++) {
					if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == NotSet_Item_MASK[kk]) {
						TradeColorIndex = NOT_SETTING_COLOR;
						return FALSE;
					}

				}
				for (kk = 0; kk < NotSet_Item_KINDCnt; kk++) {
					if (NotSet_Item_KIND[kk] == pItem->sItemInfo.ItemKindCode) {
						TradeColorIndex = NOT_SETTING_COLOR;
						return FALSE;
					}
				}
				//////////// �̺�Ʈ ����
				if (pItem->sItemInfo.ItemKindCode == ITEM_KIND_EVENT) {
					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;

				}
				if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinCH1) {
					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;
				}

				//if((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinGP1)return FALSE;

				pItem->SetX = TradeColorRect.left;
				pItem->SetY = TradeColorRect.top;
				pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
				TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���

				if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 1)) {  //��ġ�� �������� �ֳ� üũ 
					//��⼭ ����������� üũ�Ѵ� 
					if (!CopyItemNotPickUp(&sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1], TradeCrashItemIndex[0] - 1)) {
						TradeColorIndex = NOT_SETTING_COLOR;
						TradeCrashItemIndex[0] = 0; //�ʱ�ȭ 
						TradeCrashItemIndex[1] = 0; //�ʱ�ȭ 
						return FALSE;

					}
					if (TradeCrashItemIndex[1] = cTrade.CrashTradeItem(TradeColorRect, TradeCrashItemIndex[0], 1)) {  //��ġ�� �������� �ֳ� üũ 
						TradeColorIndex = NOT_SETTING_COLOR;
						TradeCrashItemIndex[1] = 0; //�ʱ�ȭ 
						return FALSE;
					}
					TradeColorIndex = OVERLAP_ITEM_COLOR;

				}

				return TRUE;
			}

		}

	}

	return TRUE;
}


//�������� �����Ѵ� 
int cWAREHOUSE::LastSetWareHouseItem(sITEM* pItem)
{
	CheckWareHouseForm();

	for (int j = 0; j < 100; j++) {
		if (!sWareHouse.WareHouseItem[j].Flag) {
			memcpy(&sWareHouse.WareHouseItem[j], pItem, sizeof(sITEM));
			sWareHouse.WareHouseItem[j].x = pItem->SetX;
			sWareHouse.WareHouseItem[j].y = pItem->SetY;
			pItem->Flag = 0; //�ʱ�ȭ 
			sinPlaySound(sWareHouse.WareHouseItem[j].SoundIndex);
			GetWeight(); //���Ը� üũ�Ѵ� 
			ReFormWareHouse();
			return TRUE;

		}
	}
	ReFormWareHouse();
	return FALSE;
}

//â�� �ݴ´� 
int cWAREHOUSE::CloseWareHouse()
{
	int RestoreFlag = 1;

	CloseWareHouseCheckFlag3 = 1;
	cWareHouse.OpenFlag = 0; //â�� �ݴ´� 
	if (WareHouseSaveFlag) {
		if (!SaveWareHouse(&sWareHouse)) { //�������� �����Ų�� 
			cWareHouse.RestoreInvenItem(); //������ ���������� �������ش� 
			cMessageBox.ShowMessage(MESSAGE_OVER_ITEM_NUM);
			RestoreFlag = 0;
		}
		else {
			if (BackUpInvenItem2) {
				delete BackUpInvenItem2;
				BackUpInvenItem2 = 0;

			}
			if (BackUpInvenTempItem2) {
				delete BackUpInvenTempItem2;
				BackUpInvenTempItem2 = 0;

			}
			if (BackUpsInven) {
				delete BackUpsInven;
				BackUpsInven = 0;

			}

		}

	}
	WareHouseSaveFlag = 0;//�÷� �ʱ�ȭ 
	ResetInvenItemCode();		//�̺��丮 ������ �ڵ� �ʱ�ȭ ********************
	ResetInvenMoney();			//�̺��丮 �� �˻� �ʱ�ȭ
	return RestoreFlag;
}



//�������� ���´� 
int cWAREHOUSE::PickUpWareHouseItem(int x, int y, int PickUpFlag)
{
	int i = 0;
	SelectTradeItemIndex = 0;    //���� ������ 
	for (i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			if (sWareHouse.WareHouseItem[i].x < x && sWareHouse.WareHouseItem[i].x + sWareHouse.WareHouseItem[i].w > x &&
				sWareHouse.WareHouseItem[i].y < y && sWareHouse.WareHouseItem[i].y + sWareHouse.WareHouseItem[i].h > y) {
				if (PickUpFlag) {

					//���� �ϴ��
					//â���̿�� �κ��丮 ������� �������� �̿��� �Ұ����ϴ�/
					//������ ���� üũ�� �ӽ� ������
					sITEM TempItem;
					TempItem.w = ITEMSIZE * 2;
					TempItem.h = ITEMSIZE * 4;
					TempItem.Flag = 1;
					if (cInvenTory.CheckSetEmptyArea(&TempItem)) {
						////////////��⼭ ����� �������� ��������� 
						if (CopyItemNotPickUp(&sWareHouse.WareHouseItem[i], i)) {
							CheckWareHouseForm();
							memcpy(&MouseItem, &sWareHouse.WareHouseItem[i], sizeof(sITEM));
							sWareHouse.WareHouseItem[i].Flag = 0;
							sinPlaySound(sWareHouse.WareHouseItem[i].SoundIndex);
							TradeColorIndex = 0;
							GetWeight(); //���Ը� üũ�Ѵ� 
							ReFormWareHouse();
							return TRUE;
						}
					}
					else {
						cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
						break;
					}

					////////////��⼭ ����� �������� ��������� 
					if (CopyItemNotPickUp(&sWareHouse.WareHouseItem[i], i)) {
						CheckWareHouseForm();
						memcpy(&MouseItem, &sWareHouse.WareHouseItem[i], sizeof(sITEM));
						sWareHouse.WareHouseItem[i].Flag = 0;
						sinPlaySound(sWareHouse.WareHouseItem[i].SoundIndex);
						TradeColorIndex = 0;
						GetWeight(); //���Ը� üũ�Ѵ� 
						ReFormWareHouse();
						return TRUE;
					}

				}
				else {
					SelectTradeItemIndex = i + 1;
					TradeColorIndex = SELECT_ITEM_COLOR;
					sinShowItemInfoFlag = 1;

					GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&sWareHouse.WareHouseItem[i], FALSE, FALSE, FALSE);
				}
			}
		}
	}
	return FALSE;
}

//��ģ �������� ��ȯ�Ѵ� 
int cWAREHOUSE::ChangeWareHouseItem(sITEM* pItem)
{
	if (pItem->Class == ITEM_CLASS_POTION) {	//������ â�� �����Ҽ����� 
		cMessageBox.ShowMessage(MESSAGE_POTION_NOT_SETTING);
		return TRUE;

	}
	if (pItem->sItemInfo.Weight + (sWareHouse.Weight[0] - 197) > sWareHouse.Weight[1] - 196) {
		cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
		return TRUE;

	}

	CheckWareHouseForm();

	if (TradeCrashItemIndex[0]) {//�浹 �� �������� ������ �������� �ٲ��ش� 
		if (sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1].Class == ITEM_CLASS_POTION) {
			if (sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1].CODE == pItem->CODE) {
				sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1].sItemInfo.PotionCount += pItem->sItemInfo.PotionCount;
				pItem->Flag = 0;
				GetWeight();
				ReFormWareHouse();
				return TRUE;

			}
		}
		memcpy(&TempItem, &sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1], sizeof(sITEM)); //���콺 �������� ������ ���� 
		sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1].Flag = 0; //�������Ŀ� �ʱ�ȭ
		TempItem.x = pItem->x;
		TempItem.y = pItem->y;
		ReFormWareHouse();
		LastSetWareHouseItem(pItem); //���콺 �������� �κ��丮�� ���� 
		memcpy(pItem, &TempItem, sizeof(sITEM));
		GetWeight();
		ReFormWareHouse();
		return TRUE;

	}
	ReFormWareHouse();
	return FALSE;
}

//���Ը� ���Ѵ� 
int cWAREHOUSE::GetWeight()
{
	sWareHouse.Weight[0] = 197;
	for (int i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			if (sWareHouse.WareHouseItem[i].Class == ITEM_CLASS_POTION)
				sWareHouse.Weight[0] += sWareHouse.WareHouseItem[i].sItemInfo.PotionCount;
			else
				sWareHouse.Weight[0] += sWareHouse.WareHouseItem[i].sItemInfo.Weight;

		}

	}

	WareHouseSaveFlag = 1;

	return TRUE;
}

int cWAREHOUSE::LoadWareHouseItemIamge()
{

	char szFilePath[256];
	int  cnt = 0;
	for (int i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			for (int j = 0; j < MAX_ITEM; j++) {
				if (sWareHouse.WareHouseItem[i].sItemInfo.CODE == sItem[j].CODE) {
					sWareHouse.WareHouseItem[i].w = sItem[j].w; //ũ�� ���� 
					sWareHouse.WareHouseItem[i].h = sItem[j].h;
					wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[j].ItemFilePath, sItem[j].LastCategory);
					if (!sItem[j].lpTempItem)
						sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);
					//���� �Ӵ� ����
					if ((sWareHouse.WareHouseItem[i].sItemInfo.CODE & sinITEM_MASK2) == sinPZ1 || (sWareHouse.WareHouseItem[i].sItemInfo.CODE & sinITEM_MASK2) == sinPZ2) {
						if (sWareHouse.WareHouseItem[i].sItemInfo.PotionCount == 2) {
							wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[j].ItemFilePath, sItem[j].LastCategory);
							sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath); //�̹����� �����о��ش�
							sWareHouse.WareHouseItem[i].w = sItem[j].w; //ũ�� ���� 
							sWareHouse.WareHouseItem[i].h = sItem[j].h;

						}
						else {
							//���ָӴϸ� ã�´�.
							for (cnt = 0; cnt < MAX_ITEM; cnt++) {
								if (sItem[cnt].sItemInfo.CODE == (sinPZ1 | sin00)) {
									break;
								}
							}
							wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[cnt].ItemFilePath, sItem[cnt].LastCategory);
							sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath); //�̹����� �����о��ش�
							sWareHouse.WareHouseItem[i].w = sItem[cnt].w; //ũ�� ���� 
							sWareHouse.WareHouseItem[i].h = sItem[cnt].h;
						}

					}
					sWareHouse.WareHouseItem[i].lpItem = sItem[j].lpTempItem;
					break;
				}
			}
		}
	}

	cInterFace.CheckAllBox(SIN_WAREHOUSE);
	return TRUE;
}

//�κ��丮 �������� ��� ����Ѵ� 
int cWAREHOUSE::BackUpInvenItem()
{

	BackUpInvenItem2 = 0;
	BackUpInvenTempItem2 = 0;
	BackUpsInven = 0;

	BackUpInvenItem2 = new sITEM[100];
	BackUpInvenTempItem2 = new sITEM[100];
	BackUpsInven = new sINVENTORY[INVENTORY_MAX_POS];

	memcpy(BackUpInvenItem2, &cInvenTory.InvenItem, sizeof(sITEM) * 100);
	memcpy(BackUpInvenTempItem2, &cInvenTory.InvenItemTemp, sizeof(sITEM) * 100);
	memcpy(BackUpsInven, &sInven, sizeof(sINVENTORY) * INVENTORY_MAX_POS);

	InvenGold = sinChar->Money + 2023;

	RealGold = InvenGold * 3;

	return TRUE;
}

int cWAREHOUSE::RestoreInvenItem()
{
	sinSetCharItem(cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[0].ItemIndex - 1].SetModelPosi, FALSE);
	sinSetCharItem(cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[1].ItemIndex - 1].SetModelPosi, FALSE);
	sinSetCharItem(cInvenTory.InvenItem[sInven[2].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[2].ItemIndex - 1].SetModelPosi, FALSE);

	memcpy(&cInvenTory.InvenItem, BackUpInvenItem2, sizeof(sITEM) * 100);
	memcpy(&cInvenTory.InvenItemTemp, BackUpInvenTempItem2, sizeof(sITEM) * 100);
	memcpy(&sInven, BackUpsInven, sizeof(sINVENTORY) * INVENTORY_MAX_POS);

	//���콺 �������� �����ش�
	if (MouseItem.Flag)
		MouseItem.Flag = 0;

	if (BackUpInvenItem2) {
		delete BackUpInvenItem2;
		BackUpInvenItem2 = 0;

	}
	if (BackUpInvenTempItem2) {
		delete BackUpInvenTempItem2;
		BackUpInvenTempItem2 = 0;

	}
	if (BackUpsInven) {
		delete BackUpsInven;
		BackUpsInven = 0;

	}

	if (sInven[0].ItemIndex) { //���� 
		sinSetCharItem(cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[0].ItemIndex - 1].SetModelPosi, TRUE);

	}

	if (sInven[1].ItemIndex) { //���� 
		sinSetCharItem(cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[1].ItemIndex - 1].SetModelPosi, TRUE);

	}



	if (sInven[2].ItemIndex) {
		sinSetCharItem(cInvenTory.InvenItem[sInven[2].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[2].ItemIndex - 1].SetModelPosi, TRUE);

	}
	if ((InvenGold - 2023) == (RealGold / 3) - 2023) {
		CheckCharForm();//���� 
		sinChar->Money = InvenGold - 2023;
		ReformCharForm();//������ 	

	}
	else {
		SendSetHackUser(6); //��ŷ�� �Ϸ����ߴ� ���� ������ ��� TRUE ���� ���� 

	}
	InvenGold = 0;
	RealGold = 0;

	cInvenTory.SetItemToChar(); //�Ķ���� ���� 
	cInvenTory.ReFormInvenItem(); //������ ������ �ٽ� �޴´� 
	cInvenTory.CheckWeight();	  //���� ���� 	

	return TRUE;
}

/*****************************************************************************/
/*							������ ����										 */
/*****************************************************************************/
//�������� �����ִ����� üũ�Ѵ� 
int cCRAFTITEM::SetCraftItemAreaCheck(sITEM* pItem, int Kind)
{

	int i, j;
	TradeColorIndex = 0; //�ʱ�ȭ   (Ʈ���̵� â�� â�� ���� ����) 
	TradeCrashItemIndex[0] = 0; //�浹�� ������ Index �ʱ�ȭ 
	cTrade.InitTradeColorRect(); //�ʱ�ȭ

	if (((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinCA1) || ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinCA2) || pItem->sItemInfo.expireTime > 0)
	{
		TradeColorIndex = NOT_SETTING_COLOR;
		return FALSE;
	}

	// ����� - ������ ����(������(������ �籸��)â���� ������ ���� �ڸ��� �������� ���� �� ���콺�� �޷��ִ� ����)
	if (pItem->sItemInfo.CODE == (sinBI1 | sin90))
	{
		int ReconStoneX = 159 + 11, ReconStoneY = 268 + 11 + sinInterHeight2;
		TradeColorIndex = 0; //�ʱ�ȭ   (Ʈ���̵� â�� â�� ���� ����)
		if (ReconStoneX <= pItem->x + 22 && ReconStoneX > pItem->x && ReconStoneY < pItem->y + 22 && ReconStoneY > pItem->y)
		{
			pItem->SetX = ReconStoneX + (((pItem->x + 22 - ReconStoneX) / 22) * 22) - 11;
			pItem->SetY = ReconStoneY + (((pItem->y + 22 - ReconStoneY) / 22) * 22) - 11;
			pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
			TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���
			if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
				cTrade.InitTradeColorRect(); //�÷� �ڽ� RECT �ʱ�ȭ (�÷� �ڽ� RECT�� �ϳ���� 0 �̸� �����Ҽ� ���� 
				return FALSE;
			}
			if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 2)) {  //��ġ�� �������� �ֳ� üũ 
				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;
			}
			return TRUE;
		}
	}

	if ((pItem->CODE & sinITEM_MASK2) == sinOS1 || (pItem->CODE & sinITEM_MASK2) == sinSE1) { //������ ��� || ������ �籸�� �� 
		TradeStartX = 218;
		TradeStartY = 193 + sinInterHeight2;
		TradeEndX = TradeStartX + (3 * 22);
		TradeEndY = TradeStartY + (4 * 22);

		for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22) {
			for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22) {
				if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j) { //�κ��丮 �ڽ��� �������� üũ 
					TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
					TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
					TradeColorRect.right = pItem->w;
					TradeColorRect.bottom = pItem->h;
					if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
						cTrade.InitTradeColorRect(); //�÷� �ڽ� RECT �ʱ�ȭ (�÷� �ڽ� RECT�� �ϳ���� 0 �̸� �����Ҽ� ���� 
						return FALSE;
					}

					pItem->SetX = TradeColorRect.left;
					pItem->SetY = TradeColorRect.top;
					pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
					TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���

					//���� ����
					if (cCraftItem.ForceFlag) {
						//���� ������ ��� 
						//--------------------------------------------------------------------------//
#ifdef HASIEGE_MODE
	//������ �������� ���� ����Ӵϸ� üũ�Ѵ�.
						if (cShop.haBuyMoneyCheck(cCraftItem.CheckForceOrbPrice(pItem)) == 0) {
#else
						if (sinChar->Money < cCraftItem.CheckForceOrbPrice(pItem)) {
#endif
							//---------------------------------------------------------------------------//
							TradeColorIndex = NOT_ENOUGH_MONEY;
							return FALSE;
						}

#ifdef	_LANGUAGE_JAPANESE
						// �����  �Ϻ��� �̶������� ����Ѵ�. �Ϲ��������� 
						// os101 ~ os110������ �������������� ������ ������ �� �ִ�.
						if (!smConfig.DebugMode)
						{
							// ����� - ���� ������ ȣ������ ������� ������ �� ������ ������
							// �Ϲ� ������ ���� ���丣�� �̴ϱ׸� ����, �������� ������忡���� ���� �Ұ���
							if (((pItem->sItemInfo.CODE & sinITEM_MASK3) >= sin11) && ((pItem->sItemInfo.CODE & sinITEM_MASK3) <= sin14))
							{
								TradeColorIndex = NOT_SETTING_COLOR;
								return FALSE;
							}
						}
#else
						// Kyle �̴ϱ׸����� ���
						if (!smConfig.DebugMode) {
							if ((pItem->sItemInfo.CODE & sinITEM_MASK3) >= sin33) {
								TradeColorIndex = NOT_SETTING_COLOR;
								return FALSE;
							}
						}
#endif

						/*
						// pluto ����������� ���
						if(!smConfig.DebugMode){
							if((pItem->sItemInfo.CODE & sinITEM_MASK3) >= sin10){
								TradeColorIndex = NOT_SETTING_COLOR;
								return FALSE;
							}
						}
						*/
						/*
						//����ױ��� ���
						if(!smConfig.DebugMode)
						{//gm��尡 �Ǵ°� gm��常 �ǰ��ѰŴ�. �������� �ȵǴ°Ŵ�!
							if((pItem->sItemInfo.CODE & sinITEM_MASK3) >= sin09)
							{
								TradeColorIndex = NOT_SETTING_COLOR;
								return FALSE;
							}
						}
						*/
							}

					//����Ʈ �������� ��쿡�� �������� �����Ҽ� ���� 
					if (pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinQW1 ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinPZ1 || //���� �ø�������
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinPZ2 ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinFO1 ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinSP1 || // pluto �������� ����... �迭
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinWR1 || // ����� - ���� ���ۼ� �迭
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinDR1 || // ����� - �� ���ۼ� �迭
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinGP1 || // ����� - ũ����Ż �迭
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinCH1 || // ����� - ���ݸ�, ���� �迭
						(pItem->sItemInfo.CODE >= (sinOR2 | sin06) && pItem->sItemInfo.CODE <= (sinOR2 | sin25)) || // ����� : Ŭ��ġ���� �迭
						pItem->sItemInfo.SpecialItemFlag[0] == CHECK_GIVE_ITEM ||
						pItem->sItemInfo.CODE == (sinGF1 | sin01) ||
						pItem->sItemInfo.CODE == (sinGF1 | sin02) ||
						pItem->sItemInfo.CODE == (sinGF1 | sin03) ||
						pItem->sItemInfo.CODE == (sinGF1 | sin04) ||
						pItem->sItemInfo.CODE == (sinGF1 | sin05) ||
						pItem->sItemInfo.CODE == (sinGF1 | sin06) ||
						pItem->sItemInfo.CODE == (sinOR2 | sin27) || // ����� - ��Ÿ �� �߰�
						pItem->sItemInfo.CODE == (sinOA1 | sin32) || // ����� - ��Ÿ �ƹķ� �߰�
						pItem->sItemInfo.CODE == (sinOR2 | sin28) || // ����� - �̺�Ʈ ��(7��) �߰�
						pItem->sItemInfo.CODE == (sinOA1 | sin33) || // ����� - �̺�Ʈ �ƹķ�(7��) �߰�
						pItem->sItemInfo.CODE == (sinOR2 | sin29) || // ����� - �̺�Ʈ ��(1�ð�) �߰�
						pItem->sItemInfo.CODE == (sinOA1 | sin34) || // ����� - �̺�Ʈ �ƹķ�(1�ð�) �߰�
						pItem->sItemInfo.CODE == (sinOR2 | sin30) || // ����� - �̺�Ʈ ��(1��) �߰�
						pItem->sItemInfo.CODE == (sinOA1 | sin35) || // ����� - �̺�Ʈ �ƹķ�(1��) �߰�
						pItem->sItemInfo.CODE == (sinDB1 | sin31) || // ����� - ���ǵ� ����(7��) �߰�
						pItem->sItemInfo.CODE == (sinDB1 | sin32) || // ����� - ���ǵ� ����(30��) �߰�
						pItem->sItemInfo.CODE == (sinSP1 | sin15) || // ����� - ���� ������ �߰�
						pItem->sItemInfo.CODE == (sinSP1 | sin27) || // ����� - ���ĺ� ���� �̺�Ʈ �߰�
						pItem->sItemInfo.CODE == (sinSP1 | sin28) || // ����� - ���ĺ� ���� �̺�Ʈ �߰�
						pItem->sItemInfo.CODE == (sinSP1 | sin29) || // ����� - ���ĺ� ���� �̺�Ʈ �߰�
						pItem->sItemInfo.CODE == (sinSP1 | sin30) || // ����� - ���ĺ� ���� �̺�Ʈ �߰�
						pItem->sItemInfo.CODE == (sinSP1 | sin31) || // ����� - ���ĺ� ���� �̺�Ʈ �߰�
						pItem->sItemInfo.CODE == (sinSP1 | sin32) || // ����� - ���ĺ� ���� �̺�Ʈ �߰�
						pItem->sItemInfo.CODE == (sinSP1 | sin33) || // ����� - ���ĺ� ���� �̺�Ʈ �߰�
						pItem->sItemInfo.CODE == (sinOR2 | sin31) || // ����� - ���� ���� �� �߰�(�ٺ�)
						pItem->sItemInfo.CODE == (sinOR2 | sin32) ||  // ����� - ���� ���� �� �߰�(ǻ��)
						pItem->sItemInfo.CODE == (sinSP1 | sin35) || // �庰 - �߷�Ÿ�� ���ݸ� ������ �߰�
						pItem->sItemInfo.CODE == (sinOR2 | sin33) ||   // �庰 - ��Ʈ��(7��) ������ �߰�
						pItem->sItemInfo.CODE == (sinOA1 | sin36) || // �庰 - ���� �����(7��) ������ �߰�
						pItem->sItemInfo.CODE == (sinSP1 | sin36) || // �庰 - ĵ������ ĵ�� ������ �߰�
						pItem->sItemInfo.CODE == (sinOA1 | sin37) ||   // �庰 - ��Ʈ�ƹķ�(7��) ������ �߰�
						pItem->sItemInfo.CODE == (sinOA2 | sin33) || // �庰 - ���� �ϸ�(1��)
						pItem->sItemInfo.CODE == (sinDB1 | sin33) || // �庰 - ���ǵ� ����(1��) �߰�
						pItem->sItemInfo.CODE == (sinSP1 | sin34) || // ����� - ȣ���� ĸ�� �߰�
						pItem->sItemInfo.CODE == (sinSP1 | sin37) || // �庰 - �����ñ׸� ���� ������ �߰�
						pItem->sItemInfo.CODE == (sinSP1 | sin38) || // �庰 - �����ñ׸� ���޶��� ������ �߰�
						pItem->sItemInfo.CODE == (sinBI1 | sin84) || // �庰 - �׶��Ƽ ��ũ�� �߰�
						pItem->sItemInfo.CODE == (sinDB1 | sin34) || // �庰 - ���ǵ� ����(1�ð�) �߰�
						pItem->sItemInfo.CODE == (sinOA2 | sin34) || // �庰 - ���� �ϸ�(1�ð�)
						pItem->sItemInfo.CODE == (sinSP1 | sin39) || // �庰 - ī���� ���� ������ �߰�
						pItem->sItemInfo.CODE == (sinGF1 | sin07) || // �庰 - ������� ã�ƶ�
						pItem->sItemInfo.CODE == (sinGF1 | sin08) ||	// �庰 - ������� ã�ƶ�
						pItem->sItemInfo.CODE == (sinOR2 | sin36) || // �庰 - �ҿｺ��
						pItem->sItemInfo.CODE == (sinOR2 | sin37) ||
						pItem->sItemInfo.CODE == (sinOR2 | sin38) ||
						pItem->sItemInfo.CODE == (sinOR2 | sin39) ||
						pItem->sItemInfo.CODE == (sinOR2 | sin40) ||
						pItem->sItemInfo.CODE == (sinOA1 | sin39) ||
						pItem->sItemInfo.CODE == (sinOA1 | sin40) ||
						pItem->sItemInfo.CODE == (sinOA1 | sin41) ||
						pItem->sItemInfo.CODE == (sinOA1 | sin42) ||
						pItem->sItemInfo.CODE == (sinOA1 | sin38) || // �庰 - ���� �̺�Ʈ �ƹķ�, �� 
						pItem->sItemInfo.CODE == (sinOR2 | sin34) ||
						pItem->sItemInfo.CODE == (sinOR2 | sin35) ||

						pItem->sItemInfo.CODE == (sinDA1 | sin05) ||
						pItem->sItemInfo.CODE == (sinDB1 | sin05) ||
						pItem->sItemInfo.CODE == (sinDG1 | sin05) ||
						pItem->sItemInfo.CODE == (sinDS1 | sin05) ||
						pItem->sItemInfo.CODE == (sinOA1 | sin05) ||
						pItem->sItemInfo.CODE == (sinOA2 | sin10) ||
						pItem->sItemInfo.CODE == (sinOR1 | sin05) ||
						pItem->sItemInfo.CODE == (sinWA1 | sin05) ||
						pItem->sItemInfo.CODE == (sinWH1 | sin06) ||
						pItem->sItemInfo.CODE == (sinWM1 | sin06) ||
						pItem->sItemInfo.CODE == (sinWP1 | sin05) ||
						pItem->sItemInfo.CODE == (sinWS1 | sin05) ||
						pItem->sItemInfo.CODE == (sinWS2 | sin10) ||
						pItem->sItemInfo.CODE == (sinWT1 | sin05) ||

						pItem->sItemInfo.CODE == (sinDA1 | sin48) ||
						pItem->sItemInfo.CODE == (sinDA1 | sin47))




					{
						TradeColorIndex = NOT_SETTING_COLOR;
						return FALSE;

					}

					// ����� - ���� ���� ���� ����
					if (!cCraftItem.ForceFlag)
					{
						if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinOS1)
						{
							if ((pItem->sItemInfo.CODE & sinITEM_MASK3) >= sin21 && (pItem->sItemInfo.CODE & sinITEM_MASK3) <= sin34)
							{
								TradeColorIndex = NOT_SETTING_COLOR;
								return FALSE;
							}
						}
					}

					int kk = 0;
					//��ϵ� �������� �ͽ��Ҽ�����
					for (kk = 0; kk < NotSet_Item_CODECnt; kk++) {
						if (NotSet_Item_CODE[kk] == pItem->sItemInfo.CODE) {
							TradeColorIndex = NOT_SETTING_COLOR;
							return FALSE;
						}

					}
					for (kk = 0; kk < NotSet_Item_MASKCnt; kk++) {
						if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == NotSet_Item_MASK[kk]) {
							TradeColorIndex = NOT_SETTING_COLOR;
							return FALSE;
						}

					}
					for (kk = 0; kk < NotSet_Item_KINDCnt; kk++) {
						if (NotSet_Item_KIND[kk] == pItem->sItemInfo.ItemKindCode) {
							TradeColorIndex = NOT_SETTING_COLOR;
							return FALSE;
						}
					}

					//if((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinGP1)return FALSE;

					if (Kind == 0) { //�ͽ��� 
						if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 2)) {  //��ġ�� �������� �ֳ� üũ 
							TradeColorIndex = NOT_SETTING_COLOR;
							return FALSE;

						}
						return TRUE;

					}
					if (Kind == 1) { //����¡ 
						if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //��ġ�� �������� �ֳ� üũ 
							TradeColorIndex = NOT_SETTING_COLOR;
							return FALSE;

						}
						return TRUE;

					}
						}

						}

					}
				}
	else {
		//���� ����
		if (cCraftItem.ForceFlag)return FALSE;
		TradeColorIndex = 0; //�ʱ�ȭ   (Ʈ���̵� â�� â�� ���� ����)
		TradeCrashItemIndex[0] = 0; //�浹�� ������ Index �ʱ�ȭ 
		cTrade.InitTradeColorRect(); //�ʱ�ȭ

		TradeStartX = 58;
		TradeStartY = 193 + sinInterHeight2;
		TradeEndX = TradeStartX + (3 * 22);
		TradeEndY = TradeStartY + (4 * 22);
		if (TradeStartX <= pCursorPos.x && TradeEndX > pCursorPos.x && TradeStartY <= pCursorPos.y && TradeEndY > pCursorPos.y) { //�κ��丮 �ڽ��� �������� üũ 
			TradeColorRect.left = TradeStartX;
			TradeColorRect.top = TradeStartY;
			TradeColorRect.right = TradeEndX - TradeStartX;
			TradeColorRect.bottom = TradeEndY - TradeStartY;


			pItem->SetX = TradeStartX + ((TradeEndX - TradeStartX) - pItem->w) / 2;
			pItem->SetY = TradeStartY + ((TradeEndY - TradeStartY) - pItem->h) / 2;
			pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
			TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���

			if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 2)) {  //��ġ�� �������� �ֳ� üũ 
				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;

			}

			//���� ������ ��� 
			//--------------------------------------------------------------------------//
			if (MixEvento)
			{
			}
			else
			{
#ifdef HASIEGE_MODE
				//������ �������� �ͽ������ üũ�Ѵ�.
				if (cShop.haBuyMoneyCheck(pItem->sItemInfo.Price) == 0)
				{
#else
				if (sinChar->Money < pItem->sItemInfo.Price) {
#endif
					//---------------------------------------------------------------------------//
					TradeColorIndex = NOT_ENOUGH_MONEY;
					return FALSE;
				}
				}
			//����� �������� �ͽ��Ŀ� ����¡�� �Ҽ����� MakeItem�� �ͽ��Ҽ����� 
			if (pItem->sItemInfo.SpecialItemFlag[0] == CHECK_COPY_ITEM ||
				pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinQW1 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinPZ1 || //���� �ø������� 
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinPZ2 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinFO1 || //�������ȵȴ�
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinSP1 || // pluto �������� ����... �迭
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinWR1 || // ����� - ���� ���ۼ� �迭
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinDR1 || // ����� - �� ���ۼ� �迭
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinGP1 || // ����� - ũ����Ż �迭
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinCH1 || // ����� - ���ݸ�, ���� �迭
				(pItem->sItemInfo.CODE >= (sinOR2 | sin06) && pItem->sItemInfo.CODE <= (sinOR2 | sin25)) || // ����� : Ŭ��ġ���� �迭
				pItem->sItemInfo.CODE == (sinGF1 | sin01) ||
				pItem->sItemInfo.CODE == (sinGF1 | sin02) ||
				//	pItem->sItemInfo.CODE == (sinGF1|sin03) || 
				//	pItem->sItemInfo.CODE == (sinGF1|sin04) ||
				pItem->sItemInfo.CODE == (sinGF1 | sin05) ||
				pItem->sItemInfo.CODE == (sinGF1 | sin06) ||
				pItem->sItemInfo.CODE == (sinOR2 | sin27) || // ����� - ��Ÿ �� �߰�
				pItem->sItemInfo.CODE == (sinOA1 | sin32) || // ����� - ��Ÿ �ƹķ� �߰� 
				pItem->sItemInfo.CODE == (sinOR2 | sin28) || // ����� - �̺�Ʈ �� �߰�(7��)
				pItem->sItemInfo.CODE == (sinOA1 | sin33) || // ����� - �̺�Ʈ �ƹķ� �߰�(7��) 
				pItem->sItemInfo.CODE == (sinOR2 | sin29) || // ����� - �̺�Ʈ �� �߰�(1�ð�)
				pItem->sItemInfo.CODE == (sinOA1 | sin34) || // ����� - �̺�Ʈ �ƹķ� �߰�(1�ð�) 
				pItem->sItemInfo.CODE == (sinOR2 | sin30) || // ����� - �̺�Ʈ �� �߰�(1��)
				pItem->sItemInfo.CODE == (sinOA1 | sin35) || // ����� - �̺�Ʈ �ƹķ� �߰�(1��) 

				pItem->sItemInfo.CODE == (sinDB1 | sin31) || // ����� - ���ǵ� ����(7��) �߰�
				pItem->sItemInfo.CODE == (sinDB1 | sin32) || // ����� - ���ǵ� ����(30��) �߰�
				pItem->sItemInfo.CODE == (sinSP1 | sin15) || // ����� - ���� ������ �߰�
				pItem->sItemInfo.CODE == (sinSP1 | sin27) || // ����� - ���ĺ� ���� �̺�Ʈ �߰�
				pItem->sItemInfo.CODE == (sinSP1 | sin28) || // ����� - ���ĺ� ���� �̺�Ʈ �߰�
				pItem->sItemInfo.CODE == (sinSP1 | sin29) || // ����� - ���ĺ� ���� �̺�Ʈ �߰�
				pItem->sItemInfo.CODE == (sinSP1 | sin30) || // ����� - ���ĺ� ���� �̺�Ʈ �߰�
				pItem->sItemInfo.CODE == (sinSP1 | sin31) || // ����� - ���ĺ� ���� �̺�Ʈ �߰�
				pItem->sItemInfo.CODE == (sinSP1 | sin32) || // ����� - ���ĺ� ���� �̺�Ʈ �߰�
				pItem->sItemInfo.CODE == (sinSP1 | sin33) || // ����� - ���ĺ� ���� �̺�Ʈ �߰�
				pItem->sItemInfo.CODE == (sinSP1 | sin35) || // �庰 - �߷�Ÿ�� ���ݸ� ������ �߰�
				pItem->sItemInfo.CODE == (sinOR2 | sin33) || // �庰 - ��Ʈ�� ������ �߰�
				pItem->sItemInfo.CODE == (sinOR2 | sin31) || // ����� - ���� ���� �� �߰�(�ٺ�)
				pItem->sItemInfo.CODE == (sinOR2 | sin32) || // ����� - ���� ���� �� �߰�(ǻ��)
				pItem->sItemInfo.CODE == (sinOA1 | sin36) ||   // �庰 - ���� �����(7��) ������ �߰�
				pItem->sItemInfo.CODE == (sinSP1 | sin36) || // �庰 - ĵ������ ĵ�� ������ �߰�
				pItem->sItemInfo.CODE == (sinOA1 | sin37) ||   // �庰 - ��Ʈ�ƹķ� ������ �߰�
				pItem->sItemInfo.CODE == (sinOA2 | sin33) || // �庰 - ���� �ϸ�(1��)
				pItem->sItemInfo.CODE == (sinDB1 | sin33) || // �庰 - ���ǵ� ����(1��) �߰�
				pItem->sItemInfo.CODE == (sinSP1 | sin34) || // ����� - ȣ���� ĸ�� �߰� 
				pItem->sItemInfo.CODE == (sinSP1 | sin37) || // �庰 - �����ñ׸� ���� ������ �߰�
				pItem->sItemInfo.CODE == (sinSP1 | sin38) || // �庰 - �����ñ׸� ���޶��� ������ �߰�
				pItem->sItemInfo.CODE == (sinBI1 | sin84) || // �庰 - �׶��Ƽ ��ũ�� �߰�
				pItem->sItemInfo.CODE == (sinDB1 | sin34) || // �庰 - ���ǵ� ����(1�ð�) �߰�
				pItem->sItemInfo.CODE == (sinOA2 | sin34) || // �庰 - ���� �ϸ�(1�ð�)
				pItem->sItemInfo.CODE == (sinSP1 | sin39) || // �庰 - ī���� ���� ������ �߰�
				pItem->sItemInfo.CODE == (sinDA1 | sin54) || pItem->sItemInfo.CODE == (sinDA2 | sin54) ||// ����� - ������ ���� �߰� - ���ձ���
				pItem->sItemInfo.CODE == (sinDA1 | sin55) || pItem->sItemInfo.CODE == (sinDA2 | sin55) ||  // ����� - ������ ���� �߰� - ���ձ���
				pItem->sItemInfo.CODE == (sinOR2 | sin36) || // �庰 - �ҿｺ��
				pItem->sItemInfo.CODE == (sinOR2 | sin37) ||
				pItem->sItemInfo.CODE == (sinOR2 | sin38) ||
				pItem->sItemInfo.CODE == (sinOR2 | sin39) ||
				pItem->sItemInfo.CODE == (sinOR2 | sin40) ||
				pItem->sItemInfo.CODE == (sinOA1 | sin39) ||
				pItem->sItemInfo.CODE == (sinOA1 | sin40) ||
				pItem->sItemInfo.CODE == (sinOA1 | sin41) ||
				pItem->sItemInfo.CODE == (sinOA1 | sin42) ||
				pItem->sItemInfo.CODE == (sinOA1 | sin38) ||
				pItem->sItemInfo.CODE == (sinOR2 | sin34) ||
				pItem->sItemInfo.CODE == (sinOR2 | sin35) ||

				pItem->sItemInfo.CODE == (sinDA1 | sin05) ||
				pItem->sItemInfo.CODE == (sinDB1 | sin05) ||
				pItem->sItemInfo.CODE == (sinDG1 | sin05) ||
				pItem->sItemInfo.CODE == (sinDS1 | sin05) ||
				pItem->sItemInfo.CODE == (sinOA1 | sin05) ||
				pItem->sItemInfo.CODE == (sinOA2 | sin10) ||
				pItem->sItemInfo.CODE == (sinOR1 | sin05) ||
				pItem->sItemInfo.CODE == (sinWA1 | sin05) ||
				pItem->sItemInfo.CODE == (sinWH1 | sin06) ||
				pItem->sItemInfo.CODE == (sinWM1 | sin06) ||
				pItem->sItemInfo.CODE == (sinWP1 | sin05) ||
				pItem->sItemInfo.CODE == (sinWS1 | sin05) ||
				pItem->sItemInfo.CODE == (sinWS2 | sin10) ||
				pItem->sItemInfo.CODE == (sinWT1 | sin05) ||

				pItem->sItemInfo.CODE == (sinDA1 | sin48) ||
				pItem->sItemInfo.CODE == (sinDA1 | sin47)



				)

			{

				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;
			}

			if (!cCraftItem.ForceFlag)
			{
				if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinOS1)
				{
					if ((pItem->sItemInfo.CODE & sinITEM_MASK3) >= sin21 && (pItem->sItemInfo.CODE & sinITEM_MASK3) <= sin34)
					{
						TradeColorIndex = NOT_SETTING_COLOR;
						return FALSE;
					}
				}
			}

			int kk = 0;

			for (kk = 0; kk < NotSet_Item_CODECnt; kk++) {
				if (NotSet_Item_CODE[kk] == pItem->sItemInfo.CODE) {
					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;
				}

			}
			for (kk = 0; kk < NotSet_Item_MASKCnt; kk++) {
				if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == NotSet_Item_MASK[kk]) {
					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;
				}

			}
			for (kk = 0; kk < NotSet_Item_KINDCnt; kk++) {
				if (NotSet_Item_KIND[kk] == pItem->sItemInfo.ItemKindCode) {
					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;
				}

			}
			return TRUE;
			}

		}
	return TRUE;
	}

int ForceOrbPriceIndex[] = { 200,500,1000,2000,4000,5000,10000,20000,30000,40000,50000,60000,70000,80000,80000,80000 };
int MagicForceOrbPriceIndex[] = { 200,500,1000,2000,4000,5000,10000,20000,30000,40000,50000,60000,70000,80000 };

DWORD SheltomCode2[] = { sin01,sin02,sin03,sin04,sin05,sin06,sin07,sin08,sin09,sin10,sin11,sin12,sin13,sin14,sin15 }; // Sheltons que podem ser utilizados no npc de force aqui
DWORD MagicSheltomCode[] = { sin21,sin22,sin23,sin24,sin25,sin26,sin27,sin28,sin29,sin30,sin31,sin32 }; // Sheltons m�gicos que podem ser usados no npc de force aqui

DWORD BillingMagicSheltomCode[] = { sin35, sin36, sin37 };

int cCRAFTITEM::CheckForceOrbPrice()
{
	ForceItemPrice = 0;
	for (int j = 1; j < 15; j++) {
		if (sCraftItem.CraftItem[j].Flag) {
			for (int t = 0; t < 16; t++)
			{
				if ((sCraftItem.CraftItem[j].sItemInfo.CODE & sinITEM_MASK3) <= sin20)
				{
					if ((sCraftItem.CraftItem[j].sItemInfo.CODE & sinITEM_MASK3) == SheltomCode2[t])
					{
						ForceItemPrice += ForceOrbPriceIndex[t];
						break;
					}
				}
				else
				{
					if ((sCraftItem.CraftItem[j].sItemInfo.CODE & sinITEM_MASK3) == MagicSheltomCode[t])
					{
						ForceItemPrice += MagicForceOrbPriceIndex[t];
						break;
					}
				}

			}
		}
	}
	ForceItemPrice2 = ForceItemPrice;
	return TRUE;
}
int cCRAFTITEM::CheckForceOrbPrice(sITEM * pItem)
{

	int TempPrice = 0;
	for (int j = 1; j < 15; j++) {
		if (sCraftItem.CraftItem[j].Flag) {
			for (int t = 0; t < 14; t++) {
				if ((sCraftItem.CraftItem[j].sItemInfo.CODE & sinITEM_MASK3) == SheltomCode2[t]) {
					TempPrice += ForceOrbPriceIndex[t];
					break;
				}
				else if ((sCraftItem.CraftItem[j].sItemInfo.CODE & sinITEM_MASK3) == MagicSheltomCode[t]) {
					TempPrice += MagicForceOrbPriceIndex[t];
					break;
				}
			}
		}
	}

	for (int t = 0; t < 14; t++) {
		if ((pItem->sItemInfo.CODE & sinITEM_MASK3) == SheltomCode2[t]) {
			return TempPrice + ForceOrbPriceIndex[t];

		}

		if ((pItem->sItemInfo.CODE & sinITEM_MASK3) == MagicSheltomCode[t]) {
			return TempPrice + MagicForceOrbPriceIndex[t];
		}
	}
	return FALSE;
}
int cCRAFTITEM::LastSetCraftItem(sITEM * pItem, int Kind)
{
	int TempLevel = 0;
	int TempLevel2 = 0;
	if (Kind == 0) {
		CheckCraftItemForm();
		if ((pItem->CODE & sinITEM_MASK2) == sinOS1) {
			for (int j = 1; j < 15; j++) {
				if (!sCraftItem.CraftItem[j].Flag) {
					memcpy(&sCraftItem.CraftItem[j], pItem, sizeof(sITEM));
					sCraftItem.CraftItem[j].x = pItem->SetX;
					sCraftItem.CraftItem[j].y = pItem->SetY;
					pItem->Flag = 0;
					sinPlaySound(sCraftItem.CraftItem[j].SoundIndex);
					ReFormCraftItem();
					CheckForceOrbPrice();
					return TRUE;
				}
			}
		}

		else if ((pItem->CODE & sinITEM_MASK2) == sinSE1) {
			if (sCraftItem.CraftItem[0].Flag) {
				TempLevel = sCraftItem.CraftItem[0].sItemInfo.Level;
				for (int k = 0; k < MAX_SEEL_COUNT; k++) {
					if (pItem->CODE == sReconItem[k].dwCODE) {
						if (!(TempLevel >= sReconItem[k].iLevel[0] && TempLevel <= sReconItem[k].iLevel[1])) {
							cMessageBox.ShowMessage(MESSAGE_NOT_RECONITEM);
							return FALSE;
						}
					}
				}
			}
			for (int j = 1; j < 15; j++) {
				if (!sCraftItem.CraftItem[j].Flag) {
					memcpy(&sCraftItem.CraftItem[j], pItem, sizeof(sITEM));
					sCraftItem.CraftItem[j].x = pItem->SetX;
					sCraftItem.CraftItem[j].y = pItem->SetY;
					pItem->Flag = 0;
					sinPlaySound(sCraftItem.CraftItem[j].SoundIndex);
					ReFormCraftItem();
					return TRUE;
				}
			}

		}

		else if ((pItem->CODE & sinITEM_MASK2) == sinBI1)
		{
			if ((pItem->CODE & sinITEM_MASK3) == sin90)
			{
				if (!sCraftItem.CraftItem[14].Flag) {
					memcpy(&sCraftItem.CraftItem[14], pItem, sizeof(sITEM));
					sCraftItem.CraftItem[14].x = pItem->SetX;
					sCraftItem.CraftItem[14].y = pItem->SetY;
					pItem->Flag = 0;
					sinPlaySound(sCraftItem.CraftItem[14].SoundIndex);
					return TRUE;
				}
			}
		}
		else {
			if (!sCraftItem.CraftItem[0].Flag) {
				memcpy(&sCraftItem.CraftItem[0], pItem, sizeof(sITEM));
				sCraftItem.CraftItem[0].x = pItem->SetX;
				sCraftItem.CraftItem[0].y = pItem->SetY;
				pItem->Flag = 0;
				sinPlaySound(sCraftItem.CraftItem[0].SoundIndex);
				ReFormCraftItem();
				return TRUE;
			}

		}
	}

	if (Kind == 1)
	{
		if ((pItem->CODE & sinITEM_MASK2) == sinOS1)
		{
			for (int j = 1; j < 15; j++)
			{
				if (!sAgingItem.AgingItem[j].Flag)
				{
					memcpy(&sAgingItem.AgingItem[j], pItem, sizeof(sITEM));
					sAgingItem.AgingItem[j].x = pItem->SetX;
					sAgingItem.AgingItem[j].y = pItem->SetY;
					pItem->Flag = 0;
					sinPlaySound(sAgingItem.AgingItem[j].SoundIndex);
					return TRUE;
				}
			}
		}
		else {
			if (!sAgingItem.AgingItem[0].Flag)
			{
				memcpy(&sAgingItem.AgingItem[0], pItem, sizeof(sITEM));
				sAgingItem.AgingItem[0].x = pItem->SetX;
				sAgingItem.AgingItem[0].y = pItem->SetY;
				pItem->Flag = 0;
				sinPlaySound(sAgingItem.AgingItem[0].SoundIndex);
				return TRUE;
			}

		}

	}
	if (Kind == 2)
	{
		for (int j = 0; j < 15; j++) {
			if (!sAgingItem.AgingItem[j].Flag) {
				memcpy(&sAgingItem.AgingItem[j], pItem, sizeof(sITEM));
				sAgingItem.AgingItem[j].x = pItem->SetX;
				sAgingItem.AgingItem[j].y = pItem->SetY;
				pItem->Flag = 0;
				sinPlaySound(sAgingItem.AgingItem[j].SoundIndex);
				return TRUE;
			}
		}
	}
	if (Kind == 3)
	{
		if (!sAgingItem.AgingItem[14].Flag) {
			memcpy(&sAgingItem.AgingItem[14], pItem, sizeof(sITEM));
			sAgingItem.AgingItem[14].x = pItem->SetX;
			sAgingItem.AgingItem[14].y = pItem->SetY;
			pItem->Flag = 0;
			sinPlaySound(sAgingItem.AgingItem[14].SoundIndex);
			return TRUE;
		}
	}

	return TRUE;

}

int cCRAFTITEM::PickUpCraftItem(int x, int y, int PickUpFlag, int Kind)
{

	int i = 0;
	SelectTradeItemIndex = 0;
	if (Kind == 0) {
		for (i = 0; i < 15; i++) {
			if (sCraftItem.CraftItem[i].Flag) {
				if (sCraftItem.CraftItem[i].x < x && sCraftItem.CraftItem[i].x + sCraftItem.CraftItem[i].w > x &&
					sCraftItem.CraftItem[i].y < y && sCraftItem.CraftItem[i].y + sCraftItem.CraftItem[i].h > y) {
					if (PickUpFlag) {
						int cnt = 0;
						for (cnt = 1; cnt < 15; cnt++) {
							if (i == 0 && sCraftItem.CraftItem[cnt].Flag) {
								cMessageBox.ShowMessage(MESSAGE_NOT_AGINGPICKUP); //  
								cnt = 100;
								break;
							}
						}
						if (cnt == 100) break;
						//=================================================================================================	
						CheckCraftItemForm();
						memcpy(&MouseItem, &sCraftItem.CraftItem[i], sizeof(sITEM));
						sCraftItem.CraftItem[i].Flag = 0;
						haCraftSortItem(&sCraftItem);  //������ �籸�� ������ �迭 �̵�
						//memset(&sCraftItem.CraftItem[i],0,sizeof(sITEM));  //���� ��ŷ ���� ������ �ӽ� �����̴�.

						sinPlaySound(sCraftItem.CraftItem[i].SoundIndex);
						TradeColorIndex = 0;
						ReFormCraftItem();
						if (ForceFlag)
							CheckForceOrbPrice(); // Forece Orb �÷�������쿣 ���������� ǥ��

						if (i == 0) {  //�������� ������ 
							for (int i = 1; i < 15; i++) {
								if (sCraftItem.CraftItem[i].Flag) {
									if ((sCraftItem.CraftItem[i].CODE & sinITEM_MASK2) == sinSE1) {
										if (!cInvenTory.AutoSetInvenItem(&sCraftItem.CraftItem[i])) {
											sinThrowItemToFeild(&sCraftItem.CraftItem[i]);
											sCraftItem.CraftItem[i].Flag = 0;
											memset(&sCraftItem.CraftItem[i], 0, sizeof(sCRAFTITEM)); //�������� �� ���������� 
										}
									}
								}
							}
						}
						return TRUE;
					}
					else {
						SelectTradeItemIndex = i + 1;
						TradeColorIndex = SELECT_ITEM_COLOR;
						sinShowItemInfoFlag = 1;

						GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&sCraftItem.CraftItem[i], FALSE, FALSE, FALSE);
					}
				}
			}
		}
	}
	if (Kind == 1) {
		for (i = 0; i < 15; i++) {
			if (sAgingItem.AgingItem[i].Flag) {
				if (sAgingItem.AgingItem[i].x < x && sAgingItem.AgingItem[i].x + sAgingItem.AgingItem[i].w > x &&
					sAgingItem.AgingItem[i].y < y && sAgingItem.AgingItem[i].y + sAgingItem.AgingItem[i].h > y) {
					if (PickUpFlag) {
						//���� ���� ���¿��� ����¡�� ĵ������ ���Ѵ�.======================================================
						int cnt = 0;
						for (cnt = 1; cnt < 15; cnt++) {
							if (i == 0 && sAgingItem.AgingItem[cnt].Flag) {
								cMessageBox.ShowMessage(MESSAGE_NOT_AGINGPICKUP); //  
								cnt = 100;
								break;
							}
						}
						if (cnt == 100) break;
						//===================================================================================================

						memcpy(&MouseItem, &sAgingItem.AgingItem[i], sizeof(sITEM));
						sAgingItem.AgingItem[i].Flag = 0;
						sinPlaySound(sAgingItem.AgingItem[i].SoundIndex);

						if (sAgingItem.AgingItem[i].sItemInfo.CODE == (sinBI1 | sin10) ||
							sAgingItem.AgingItem[i].sItemInfo.CODE == (sinBI1 | sin11) ||
							sAgingItem.AgingItem[i].sItemInfo.CODE == (sinBI1 | sin60) ||
							sAgingItem.AgingItem[i].sItemInfo.CODE == (sinBI1 | sin61) ||
							sAgingItem.AgingItem[i].sItemInfo.CODE == (sinBI1 | sin85) ||
							sAgingItem.AgingItem[i].sItemInfo.CODE == (sinBI2 | sin38 ||
								MouseItem.sItemInfo.CODE == (sinBI2 | sin52))) {

							SpecialSheltomCode = 0;
							break;
						}

						TradeColorIndex = 0;

						if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
							(MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinMA2) {
							sinMakeItemCheck();

						}
						else {
							if (i == 0) {
								for (int y = 0; y < 4; y++) { //���� �ε��� �ʱ�ȭ 
									for (int h = 0; h < 3; h++) {
										AgingLevelSheltomIndex[y][h] = 0;
										AgingCheckSheltomFlag = 0;
										AgingSheltomCnt2 = 0;
										cAging.CancelAgingItem(1);
									}
								}
							}
							else {
								if (AgingSheltomCnt2) {
									AgingSheltomCnt2--;
									AgingCheckSheltomFlag = 0;
								}
							}
						}
						return TRUE;
					}
					else {
						SelectTradeItemIndex = i + 1;
						TradeColorIndex = SELECT_ITEM_COLOR;
						sinShowItemInfoFlag = 1;

						GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&sAgingItem.AgingItem[i], FALSE, FALSE, FALSE);
					}
				}
			}
		}
	}
	return FALSE;
}

DWORD SheltomCode[MAX_SHELTOM] = { sin01,sin02,sin03,sin04,sin05,sin06,sin07,sin08,sin09, sin10, sin11, sin12, sin13, sin14 };

//�������� �����Ѵ� 
int cCRAFTITEM::MixingItem()
{
	int i, j, t, k, CheckFlag = 0, Temp = 0, LineCount = 0;
	DWORD TempCode2 = 0;
	int  ReConCnt = 0;
	if (sCraftItem_Recv.DocIndex == 2000) { //������ �籸��
		for (i = 0; i < 12; i++) {
			if (sCraftItem_Recv.SheltomCode[i]) {
				TempCode2 = sCraftItem_Recv.SheltomCode[i];
				break;
			}
		}
		for (j = 0; j < MAX_SEEL_COUNT; j++) { // ����� - ���̿��� �� �߰� (3���� -> 4����)
			if (sReconItem[j].dwCODE == TempCode2) {
				ReConCnt = sReconItem[j].iCount;
			}
		}
		if (!ReConCnt) {  //�ڵ尡 Ʋ���� ����
			sCraftItem_Recv.Result = FALSE;
			return FALSE;
		}
		for (i = 0; i < ReConCnt; i++) { //������ �ȸ����� ���� 
			if (sCraftItem_Recv.SheltomCode[i]) {
				if (TempCode2 != sCraftItem_Recv.SheltomCode[i]) {
					sCraftItem_Recv.Result = FALSE;
					return FALSE;
				}
			}
		}
		sCraftItem_Recv.Result = TRUE;
		return 2000;
	}

	//�������� ������ �ͽ��� ����
	for (j = 0; j < 12; j++) {
		if ((sCraftItem_Recv.SheltomCode[j] & sinITEM_MASK2) == sinSE1) {
			sCraftItem_Recv.Result = FALSE;
			return FALSE;
		}
	}


	//sITEM TempItem;

	for (j = 0; j < 14; j++) //�ʱ�ȭ 
		sCraftItem.SheltomIndex[j] = 0;


	/////////////������ �˻��Ѵ� 
	for (i = 0; i < 12; i++) { //1���ʹ� ����
		if (sCraftItem_Recv.SheltomCode[i] & sinITEM_MASK2) {
			for (j = 0; j < 14; j++) {
				if ((sCraftItem_Recv.SheltomCode[i] & sinITEM_MASK3) == SheltomCode[j]) {
					sCraftItem.SheltomIndex[j]++;
					break;
				}
			}
		}
	}



	for (i = 0; i < MAX_CRAFTITEM_INFO; i++) {
		for (j = 0; j < 8; j++) {
			if (sCraftItem_Recv.DesCraftItem.Flag) {

				if ((sCraftItem_Info[i].MainItemCode[j] & sinITEM_MASK2) ==
					(sCraftItem_Recv.DesCraftItem.sItemInfo.CODE & sinITEM_MASK2)) {
					for (t = 0; t < 14; t++) {
						if (sCraftItem_Info[i].AddItem[t] != sCraftItem.SheltomIndex[t]) {
							CheckFlag = 1;
							break;
						}
						CheckFlag = 0;

					}

					if (!CheckFlag) {

						for (k = 0; k < 14; k++) {

							if (sCraftItem.SheltomIndex[k])
								sCraftItem_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0] = k + 1;

							if (sCraftItem_Info[i].PlusElementCode[k]) {
								switch (sCraftItem_Info[i].PlusElementCode[k]) {
								case SIN_ADD_FIRE:
									sCraftItem_Recv.DesCraftItem.sItemInfo.Resistance[sITEMINFO_FIRE] += (int)sCraftItem_Info[i].AddElement[k];
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_FIRE;

									break;
								case SIN_ADD_ICE:
									sCraftItem_Recv.DesCraftItem.sItemInfo.Resistance[sITEMINFO_ICE] += (int)sCraftItem_Info[i].AddElement[k];
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_ICE;
									break;
								case SIN_ADD_LIGHTNING:
									sCraftItem_Recv.DesCraftItem.sItemInfo.Resistance[sITEMINFO_LIGHTING] += (int)sCraftItem_Info[i].AddElement[k];
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_LIGHTNING;
									break;

								case SIN_ADD_POISON:
									sCraftItem_Recv.DesCraftItem.sItemInfo.Resistance[sITEMINFO_POISON] += (int)sCraftItem_Info[i].AddElement[k];
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_POISON;
									break;
								case SIN_ADD_BIO:
									sCraftItem_Recv.DesCraftItem.sItemInfo.Resistance[sITEMINFO_BIONIC] += (int)sCraftItem_Info[i].AddElement[k];
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_BIO;
									break;
								case SIN_ADD_CRITICAL:		//ũ��Ƽ�� 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Critical_Hit += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Critical_Hit += (sCraftItem_Recv.DesCraftItem.sItemInfo.Critical_Hit * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Critical_Hit +=  (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Critical_Hit*(sCraftItem_Info[i].AddElement									[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_CRITICAL;
									break;
								case SIN_ADD_ATTACK_RATE:	//���߷� 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Rating += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Rating += (sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Rating * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Rating +=  (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Rating*(sCraftItem_Info[i].											AddElement[k]/100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_ATTACK_RATE;
									break;
								case SIN_ADD_DAMAGE_MIN:	//�ּҴ����
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[0] += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[0] += (sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[0] * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[0] +=  (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[0]*(sCraftItem_Info[i].AddElement[k]/100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_DAMAGE_MIN;
									break;

								case SIN_ADD_DAMAGE_MAX:	//�ִ�����
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[1] += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[1] += (sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[1] * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[1] +=  (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[1]*(sCraftItem_Info[i].AddElement[k]/100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_DAMAGE_MAX;
									break;
								case SIN_ADD_ATTACK_SPEED:	//���ݼӵ�
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Speed += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Speed += (sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Speed * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Speed += (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Speed*(sCraftItem_Info[i].AddElement[k]/											100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_ATTACK_SPEED;
									break;
								case SIN_ADD_ABSORB:	//����� 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fAbsorb += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fAbsorb += (GetItemAbsorb(&sCraftItem_Recv.DesCraftItem.sItemInfo) * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_ABSORB;
									break;
								case SIN_ADD_DEFENCE:	//���� 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Defence += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Defence += (sCraftItem_Recv.DesCraftItem.sItemInfo.Defence * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Defence += (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Defence*(sCraftItem_Info[i].AddElement[k]/100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_DEFENCE;
									break;
								case SIN_ADD_BLOCK_RATE: //���� 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fBlock_Rating += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fBlock_Rating += (sCraftItem_Recv.DesCraftItem.sItemInfo.fBlock_Rating * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_BLOCK_RATE;
									break;
								case SIN_ADD_MOVE_SPEED: //�̵��ӵ� 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fSpeed += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fSpeed += (sCraftItem_Recv.DesCraftItem.sItemInfo.fSpeed * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_MOVE_SPEED;
									break;

								case SIN_ADD_LIFE: //�ִ� ����� 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Life += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Life += (sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Life * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_LIFE;
									break;

								case SIN_ADD_MANA: //�ִ� ���
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Mana += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Mana += (sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Mana * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_MANA;
									break;
								case SIN_ADD_STAMINA: //�ִ� �ٷ�
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Stamina += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Stamina += (sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Stamina * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_STAMINA;
									break;
								case SIN_ADD_LIFEREGEN: //��������
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fLife_Regen += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fLife_Regen += (sCraftItem_Recv.DesCraftItem.sItemInfo.fLife_Regen * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_LIFEREGEN;
									break;
								case SIN_ADD_MANAREGEN: //������
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fMana_Regen += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fMana_Regen += (sCraftItem_Recv.DesCraftItem.sItemInfo.fMana_Regen * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_MANAREGEN;

									break;
								case SIN_ADD_STAMINAREGEN: //�ٷ����
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fStamina_Regen += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fStamina_Regen += (sCraftItem_Recv.DesCraftItem.sItemInfo.fStamina_Regen * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_STAMINAREGEN;

									break;

								}
							}
						}

						////////��ġ���� �� 
						ReformItem(&sCraftItem_Recv.DesCraftItem.sItemInfo);  //������ ���� �ޱ� 

						//������ ���� �� �ι�° 
						if (!CheckMixItem(&sCraftItem_Recv.DesCraftItem.sItemInfo)) {
							SendSetHackUser(50); //��ŷ�� �Ϸ����ߴ� ���� ������ ��� TRUE ���� ���� 
							return FALSE;
						}


						sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindCode = ITEM_KIND_CRAFT;
						ReformMixItem(&sCraftItem_Recv.DesCraftItem.sItemInfo); //������ 

						sCraftItem_Recv.DocIndex = i;
						sCraftItem_Recv.Result = TRUE;

						return TRUE;

					}
				}
			}
		}
	}


	sCraftItem_Recv.Result = FALSE;

	return FALSE;
}

//�������� �����ִ��� üũ�Ѵ� 
int cCRAFTITEM::CraftCheckEmptyArea(sITEM * pItem)
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;
	int StartX = 23;  //�κ� �ڽ��� ������ X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - pItem->w; //�������� �����ִ� ���� �� X��ǥ 
	cy = (22 * INVENTORY_BOX_COL) - pItem->h; //�������� �����ִ� ���� �� Y��ǥ

	//�ڵ尡 ���� �ʰų� �������� ������� ���� ���� �����Ҷ� �˻��ؼ� ��ġ�� ã�Ƴ��� 
	for (cntx = StartX; cntx <= StartX + cx; cntx += 22) {
		for (cnty = StartY; cnty <= StartY + cy; cnty += 22) {
			flag = 0;
			for (int i = 0; i < INVENTORY_MAXITEM; i++) {
				if (cInvenTory.InvenItem[i].Flag) {
					rect.left = cInvenTory.InvenItem[i].x;
					rect.right = cInvenTory.InvenItem[i].x + cInvenTory.InvenItem[i].w;
					rect.top = cInvenTory.InvenItem[i].y;
					rect.bottom = cInvenTory.InvenItem[i].y + cInvenTory.InvenItem[i].h;

					if (cntx > rect.left - pItem->w && cntx< rect.right &&
						cnty > rect.top - pItem->h && cnty < rect.bottom)
						flag++;
				}
			}
			if (flag == 0) {
				pItem->SetX = cntx;
				pItem->SetY = cnty;
				pItem->ItemPosition = 0;
				return TRUE;
			}
		}
	}

	return FALSE;

}

//�ͽ��� �������� ����Ѵ� 
int cCRAFTITEM::CancelMixItem()
{
	if (MixItemNoCopyFlag)return FALSE;
	for (int i = 0; i < 15; i++) {
		if (sCraftItem.CraftItem[i].Flag) {
			if (!cInvenTory.AutoSetInvenItem(&sCraftItem.CraftItem[i])) {
				sinThrowItemToFeild(&sCraftItem.CraftItem[i]);
				sCraftItem.CraftItem[i].Flag = 0;
			}
		}
	}

	cCraftItem.OpenFlag = 0; //â�� �ݴ´� 
	memset(&sCraftItem, 0, sizeof(sCRAFTITEM)); //�������� �� ���������� 
	ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ

	memset(&sCraftItem_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //���� �ͽ��� ����ü �ʱ�ȭ  (�ϴ� �������ش�)
	memset(&sCraftItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //���� �ͽ��� ����ü �ʱ�ȭ 

	//Force Orb �ʱ�ȭ
	ForceFlag = 0;
	ForceItemPrice = 0;
	ForceItemPrice2 = 0;
	return TRUE;
}

int CraftItemCheckDelayTime = 0;

//ũ����Ʈ �������� �����ϴ� ���� ������ ���� ����� 
int cCRAFTITEM::CheckHackCraftItem()
{
	//���� �������� �ϵ��� �Ǿ��־� ������ �ȵȴ� 
	CraftItemCheckDelayTime++;
	if (CraftItemCheckDelayTime < 70 * 10)return FALSE;
	CraftItemCheckDelayTime = 0;


	unsigned int AddItemCheckSum = 0;
	unsigned int CheckAddItemData = 2774094026;

	for (int i = 0; i < MAX_CRAFTITEM_INFO; i++) {
		for (int j = 0; j < 8; j++) {
			AddItemCheckSum += (int)((j + 1) * sCraftItem_Info[i].AddElement[j]);
			AddItemCheckSum += (j + 1) * sCraftItem_Info[i].AddItem[j];
			AddItemCheckSum += (j + 1) * sCraftItem_Info[i].ElementKind[j];
			AddItemCheckSum += (j + 1) * sCraftItem_Info[i].PlusElementCode[j];
			AddItemCheckSum += (j + 1) * sCraftItem_Info[i].MainItemCode[j];

		}

	}
	if (CheckAddItemData != AddItemCheckSum) {
		SendSetHackUser(8); //��ŷ�� �Ϸ����ߴ� ���� ������ ��� TRUE ���� ���� 

	}

	return TRUE;
}

//////////�ͽ��� ������ ���� ������ ���� ���ܿ� ��� 
int cCRAFTITEM::CheckCraftItemForm()
{ //üũ  
	int TempCheckDataSum = 0;

	if (!cCraftItem.ForceFlag) {
		for (int i = 0; i < 15; i++) {
			if (sCraftItem.CraftItem[i].Flag) {
				TempCheckDataSum += (i + 1) * sCraftItem.CraftItem[i].x;
				TempCheckDataSum += (i + 1) * sCraftItem.CraftItem[i].y;
				TempCheckDataSum += sCraftItem.CraftItem[i].sItemInfo.ItemHeader.dwChkSum;
				TempCheckDataSum += sCraftItem.CraftItem[i].sItemInfo.ItemHeader.Head;

			}
		}
		//if(TempCheckDataSum != CraftItemCheckSum) //�ʿ������ ������ ���� ����
			//SendSetHackUser(9); //��ŷ�� �Ϸ����ߴ� ���� ������ ��� TRUE ���� ���� 
	}
	return TRUE;

}

int cCRAFTITEM::ReFormCraftItem()
{ //���� 

	CraftItemCheckSum = 0;
	for (int i = 0; i < 15; i++) {
		if (sCraftItem.CraftItem[i].Flag) {
			CraftItemCheckSum += (i + 1) * sCraftItem.CraftItem[i].x;
			CraftItemCheckSum += (i + 1) * sCraftItem.CraftItem[i].y;
			CraftItemCheckSum += sCraftItem.CraftItem[i].sItemInfo.ItemHeader.dwChkSum;
			CraftItemCheckSum += sCraftItem.CraftItem[i].sItemInfo.ItemHeader.Head;

		}
	}

	return TRUE;
}

//////////Ʈ���̵�  ������ ���� ������ ���� ���ܿ� ��� ������
//ReFormTradeItem();
//CheckTradeItemForm();
int cTRADE::CheckTradeItemForm()
{ //üũ  
	int TempCheckDataSum = 0;
	for (int i = 0; i < MAX_TRADE_ITEM; i++) {
		if (sTrade.TradeItem[i].Flag) {
			TempCheckDataSum += (i + 1) * sTrade.TradeItem[i].x;
			TempCheckDataSum += (i + 1) * sTrade.TradeItem[i].y;
			TempCheckDataSum += sTrade.TradeItem[i].sItemInfo.ItemHeader.dwChkSum;
			TempCheckDataSum += sTrade.TradeItem[i].sItemInfo.ItemHeader.Head;

		}
	}
	if (TempCheckDataSum != TradeItemCheckSum)
		SendSetHackUser(10); //��ŷ�� �Ϸ����ߴ� ���� ������ ��� TRUE ���� ���� 

	return TRUE;

}

int cTRADE::ReFormTradeItem()
{ //���� 

	TradeItemCheckSum = 0;
	for (int i = 0; i < MAX_TRADE_ITEM; i++) {
		if (sTrade.TradeItem[i].Flag) {
			TradeItemCheckSum += (i + 1) * sTrade.TradeItem[i].x;
			TradeItemCheckSum += (i + 1) * sTrade.TradeItem[i].y;
			TradeItemCheckSum += sTrade.TradeItem[i].sItemInfo.ItemHeader.dwChkSum;
			TradeItemCheckSum += sTrade.TradeItem[i].sItemInfo.ItemHeader.Head;

		}
	}
	return TRUE;
}

//CheckWareHouseForm();
//ReFormWareHouse();
////////////////////â�� �����Ϸ��� ���� �ڽĵ��� ���ֱ� ���� ���ܿ� ��� !!!
int cWAREHOUSE::CheckWareHouseForm()
{ //üũ  
	int TempCheckDataSum = 0;
	for (int i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			TempCheckDataSum += (i + 1) * sWareHouse.WareHouseItem[i].x;
			TempCheckDataSum += (i + 1) * sWareHouse.WareHouseItem[i].y;
			TempCheckDataSum += sWareHouse.WareHouseItem[i].sItemInfo.ItemHeader.dwChkSum;
			TempCheckDataSum += sWareHouse.WareHouseItem[i].sItemInfo.ItemHeader.Head;

		}
	}
	if (TempCheckDataSum != WareHouseCheckSum)
		SendSetHackUser(11); //��ŷ�� �Ϸ����ߴ� ���� ������ ��� TRUE ���� ���� 

	return TRUE;

}

int cWAREHOUSE::ReFormWareHouse()
{ //���� 

	WareHouseCheckSum = 0;
	for (int i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			WareHouseCheckSum += (i + 1) * sWareHouse.WareHouseItem[i].x;
			WareHouseCheckSum += (i + 1) * sWareHouse.WareHouseItem[i].y;
			WareHouseCheckSum += sWareHouse.WareHouseItem[i].sItemInfo.ItemHeader.dwChkSum;
			WareHouseCheckSum += sWareHouse.WareHouseItem[i].sItemInfo.ItemHeader.Head;

		}
	}
	return TRUE;
}

// Processo de aging no item aqui
int cAGING::AgingItem(int MakeItemFlag)
{
	int ResultCount = 0;
	int ResultCountCheck = 0;
	DWORD TempCODE[16] = { 0, };
	int  i = 0;

	if (sAging_Recv.A_StoneItemCode == (sinBI1 | sin10))
		cAging.AginStoneKind = 1; // pedra do aging
	else if (sAging_Recv.A_StoneItemCode == (sinBI1 | sin11))
		cAging.AginStoneKind = 2; // cooper ore
	else if (sAging_Recv.A_StoneItemCode == (sinBI1 | sin60))
		cAging.AginStoneKind = 3;
	else if (sAging_Recv.A_StoneItemCode == (sinBI1 | sin61))
		cAging.AginStoneKind = 4;
	else if (sAging_Recv.A_StoneItemCode == (sinBI1 | sin85))
		cAging.AginStoneKind = 5; // aging 100%
	else if (sAging_Recv.A_StoneItemCode == (sinBI2 | sin38))
		cAging.AginStoneKind = 6;
	else if (sAging_Recv.A_StoneItemCode == (sinBI2 | sin52))
		cAging.AginStoneKind = 7;
	else
		cAging.AginStoneKind = 0;

	if (sAging_Recv.DesCraftItem.sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON)
	{
		if (SetChangeJob3QuestItem(&sAging_Recv.DesCraftItem.sItemInfo))
		{
			ReformMixItem(&sAging_Recv.DesCraftItem.sItemInfo);
			sAging_Recv.Result = 100 + sAging_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0];
			return TRUE;
		}
		sAging_Recv.Result = FALSE;
		return FALSE;
	}

	for (int i = 0; i < MAX_MAKEITEM_INFO; i++)
	{
		TempCODE[0] = sAging_Recv.DesCraftItem.sItemInfo.CODE;
		for (int p = 0; p < 15; p++) {
			if (sAging_Recv.SheltomCode[p])
				TempCODE[p + 1] = sAging_Recv.SheltomCode[p];
		}
		for (int j = 0; j < 15; j++) {
			if (sMakeItem_Info[i].CODE[j]) {
				ResultCount++;
				for (int k = 0; k < 15; k++) {
					if (sAging_Recv.SheltomCode[k] && TempCODE[k]) {
						if (TempCODE[k] == sMakeItem_Info[i].CODE[j]) {
							TempCODE[k] = -1;
							ResultCountCheck++;
						}
					}
				}
				if (sAging_Recv.DesCraftItem.sItemInfo.CODE == sMakeItem_Info[i].CODE[j]) {
					ResultCountCheck++;

				}
			}
		}
		if (ResultCount && ResultCountCheck && ResultCount == ResultCountCheck)
		{
			sAging_Recv.DesCraftItem.sItemInfo.CODE = sMakeItem_Info[i].Result_CODE;
			sAging_Recv.Result = TRUE;
			return TRUE;
		}

		else AgingCheckSheltomFlag = 0;
		ResultCount = 0;
		ResultCountCheck = 0;

	}

	if (MakeItemFlag > 0)
		return FALSE;

	int DestroyPersent = 0;

	// list sheltom aging xxstr
	DWORD SheltomCODE[MAX_SHELTOM] = { sinOS1 | sin01,sinOS1 | sin02,sinOS1 | sin03,sinOS1 | sin04,sinOS1 | sin05,sinOS1 | sin06,sinOS1 | sin07,sinOS1 | sin08,sinOS1 | sin09,sinOS1 | sin10,sinOS1 | sin11,sinOS1 | sin12,sinOS1 | sin13, sinOS1 | sin14/*, sinOS1|sin15*/ };
	DWORD CheckSheltomCODE[12] = { 0, };
	DWORD CheckSheltomCODE2[12] = { 0, };


	DWORD ItemCode[] = { sinOA1,sinOR1,sinOS1 };
	int Cnt = 0;
	while (1) {
		if (!ItemCode[Cnt])break;
		if ((sAging_Recv.DesCraftItem.sItemInfo.CODE & sinITEM_MASK2) == ItemCode[Cnt]) {
			sAging_Recv.Result = FALSE;
			return FALSE;
		}
		Cnt++;
	}


	int   AgingLevel = sAging_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0];

	for (int g = 0; g < 12; g++) {
		if (AgingLevelSheltom[AgingLevel][g]) {
			CheckSheltomCODE[g] = SheltomCODE[AgingLevelSheltom[AgingLevel][g] - 1];
			CheckSheltomCODE2[g] = sAging_Recv.SheltomCode[g];

		}
	}

	DWORD Temp = 0;
	for (int i = 11; i > 0; i--) {
		for (int j = 0; j < i; j++) {
			if (CheckSheltomCODE[j] < CheckSheltomCODE[j + 1]) {
				Temp = CheckSheltomCODE[j + 1];
				CheckSheltomCODE[j + 1] = CheckSheltomCODE[j];
				CheckSheltomCODE[j] = Temp;
			}
			if (CheckSheltomCODE2[j] < CheckSheltomCODE2[j + 1]) {
				Temp = CheckSheltomCODE2[j + 1];
				CheckSheltomCODE2[j + 1] = CheckSheltomCODE2[j];
				CheckSheltomCODE2[j] = Temp;
			}

		}
	}

	for (int i = 0; i < 12; i++)
	{
		if (CheckSheltomCODE[i])
		{
			if (CheckSheltomCODE2[i] != CheckSheltomCODE[i]) {
				return FALSE;
			}
		}
	}

	sITEM* pItem = &sAging_Recv.DesCraftItem;

	if (!sinCheckAgingItemHack(pItem))
	{
		sAging_Recv.Result = FALSE;
		return FALSE;
	}

	// Chance de falha do aging aqui
	if (MakeItemFlag == -1)
		DestroyPersent = 100;
	else
		DestroyPersent = GetRandomPos(0, 100);

	int AgingKind = 0;

	// chance de subir +2
	int PlusAgingPercent[16] = { 12, 12, 12, 10, 10, 10, 8, 8, 8, 6, 6, 6, 4, 4, 4, 0 };

	// Aging quebrando aqui, 0 = quebra padr�o = 3
	int AgingItemFaildNum = 3;

	int DownPersent = GetRandomPos(0, 100);
	int PlusPersent = GetRandomPos(0, 100);

	if (cAging.AginStoneKind == 2) // Cooper ore
		DestroyPersent = 0;

	if (cAging.AginStoneKind == 1)
	{
		if (AgingStoneAgingOkPercent[sAging_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0]] > DownPersent)
		{
			DownPersent = GetRandomPos(0, 100);

			switch (sAging_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0])
			{
			case 6:
				if (DownPersent < 70)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 7:
				if (DownPersent < 60)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 8:
				if (DownPersent < 60)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 9:
				if (DownPersent < 50)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 10:
				if (DownPersent < 50)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 11:
				if (DownPersent < 40)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 12:
				if (DownPersent < 40)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 13:
				if (DownPersent < 30)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 14:
				if (DownPersent < 30)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 15:
				if (DownPersent < 20)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 16:
				if (DownPersent < 20)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			}
		}

		// Aging quebrando
		else if (chanceQuebra[sAging_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0]] > DestroyPersent)
		{
			if (sAging_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0] >= 6)
				AgingItemFaildNum = 0;
		}

		else
		{
			if (sAging_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0] < 15)
			{
				if ((PlusAgingPercent[sAging_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0]] * 2) > PlusPersent)
				{
					sinSetAgingItemIncreState(&sAging_Recv.DesCraftItem.sItemInfo);
					AgingItemFaildNum = 4;
				}
			}
		}
	}

	else if (cAging.AginStoneKind == 2) // Cooper ore
	{
		if (AgingOkPercent[sAging_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0]] > DownPersent)
		{
			if (rand() % 100 < 81)
				AgingItemFaildNum = 1;
			else
				AgingItemFaildNum = 2;
		}
	}

	else if (cAging.AginStoneKind == 5) // Pedra 100% (N�o voltar mas pode quebrar)
	{
		if (chanceQuebra[sAging_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0]] > DestroyPersent)
		{
			AgingItemFaildNum = 0;	// Item quebrou
		}
	}

	else
	{
		// Aging Voltando

		if (AgingOkPercent[sAging_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0]] > DownPersent)
		{
			DownPersent = GetRandomPos(0, 100);

			switch (sAging_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0])
			{
			case 5:
				if (DownPersent < 70)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 6:
				if (DownPersent < 70)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 7:
				if (DownPersent < 60)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 8:
				if (DownPersent < 60)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 9:
				if (DownPersent < 50)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 10:
				if (DownPersent < 50)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 11:
				if (DownPersent < 40)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 12:
				if (DownPersent < 40)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 13:
				if (DownPersent < 30)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 14:
				if (DownPersent < 30)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 15:
				if (DownPersent < 20)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			case 16:
				if (DownPersent < 20)
					AgingItemFaildNum = 1;
				else
					AgingItemFaildNum = 2;
				break;
			}
		}

		// Aging quebrando
		else if (chanceQuebra[sAging_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0]] > DestroyPersent)
		{
			if (sAging_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0] >= 6)
				AgingItemFaildNum = 0;
		}

		else
		{
			if (sAging_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0] < 15)
			{
				if (PlusAgingPercent[sAging_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0]] > PlusPersent)
				{
					sinSetAgingItemIncreState(&sAging_Recv.DesCraftItem.sItemInfo);
					AgingItemFaildNum = 4;
				}
			}
		}
	}

	extern rsPLAYINFO* rsPlayInfo;

	if (!AgingItemFaildNum)
	{
		sAging_Recv.Result = FALSE;
		return FALSE;
	}

	if (AgingItemFaildNum == 1 || AgingItemFaildNum == 2)
	{
		sinAgingItemInit(&pItem->sItemInfo, AgingItemFaildNum); // Aging voltando -1 ou -2
	}

	if ((pItem->CODE & sinITEM_MASK2) == sinWC1 ||
		(pItem->CODE & sinITEM_MASK2) == sinWS2 ||
		(pItem->CODE & sinITEM_MASK2) == sinWP1) {
		pItem->sItemInfo.ItemAgingCount[1] = AgingLevelCritical[pItem->sItemInfo.ItemAgingNum[0]];
		if (AgingItemFaildNum <= 2)
			pItem->sItemInfo.ItemAgingCount[0] = pItem->sItemInfo.ItemAgingCount[1] / 2;

	}

	if ((pItem->CODE & sinITEM_MASK2) == sinWA1 ||
		(pItem->CODE & sinITEM_MASK2) == sinWH1 ||
		(pItem->CODE & sinITEM_MASK2) == sinWM1 ||
		(pItem->CODE & sinITEM_MASK2) == sinWS1 ||
		(pItem->CODE & sinITEM_MASK2) == sinWT1) {
		pItem->sItemInfo.ItemAgingCount[1] = AgingLevelAttack[pItem->sItemInfo.ItemAgingNum[0]];
		if (AgingItemFaildNum <= 2)
			pItem->sItemInfo.ItemAgingCount[0] = pItem->sItemInfo.ItemAgingCount[1] / 2;

	}


	if ((pItem->CODE & sinITEM_MASK2) == sinDS1) {
		pItem->sItemInfo.ItemAgingCount[1] = AgingLevelBlock[pItem->sItemInfo.ItemAgingNum[0]];
		if (AgingItemFaildNum <= 2)
			pItem->sItemInfo.ItemAgingCount[0] = pItem->sItemInfo.ItemAgingCount[1] / 2;

	}


	if ((pItem->CODE & sinITEM_MASK2) == sinDA1 ||
		(pItem->CODE & sinITEM_MASK2) == sinDA2 ||
		(pItem->CODE & sinITEM_MASK2) == sinOM1 ||
		(pItem->CODE & sinITEM_MASK2) == sinDB1 ||
		(pItem->CODE & sinITEM_MASK2) == sinDG1 ||
		(pItem->CODE & sinITEM_MASK2) == sinOA2)
	{
		pItem->sItemInfo.ItemAgingCount[1] = AgingLevelHit[pItem->sItemInfo.ItemAgingNum[0]];
		if (AgingItemFaildNum <= 2)
			pItem->sItemInfo.ItemAgingCount[0] = pItem->sItemInfo.ItemAgingCount[1] / 2;

	}

	if (pItem->CODE == (sinOR2 | sin01))
	{
		pItem->sItemInfo.ItemAgingCount[1] = 60 * 20;
	}

	pItem->sItemInfo.ItemAgingNum[1] = 1;
	pItem->sItemInfo.ItemKindCode = ITEM_KIND_AGING;

	if (AgingItemFaildNum >= 3)
		pItem->sItemInfo.ItemAgingCount[0] = 0;


	memcpy(&sAging_Recv.DesCraftItem, pItem, sizeof(sITEM));
	ReformMixItem(&sAging_Recv.DesCraftItem.sItemInfo);
	sAging_Recv.Result = AgingItemFaildNum;

	return TRUE;
}

int cAGING::CancelAgingItem(int Flag)
{
	if (MixItemNoCopyFlag)return FALSE;
	for (int i = 0; i < 15; i++) {
		if (sAgingItem.AgingItem[i].Flag) {
			if (!cInvenTory.AutoSetInvenItem(&sAgingItem.AgingItem[i])) {
				sinThrowItemToFeild(&sAgingItem.AgingItem[i]);

			}
			sAgingItem.AgingItem[i].Flag = 0;
		}
	}

	for (int y = 0; y < 4; y++) { //���� �ε��� �ʱ�ȭ 
		for (int h = 0; h < 3; h++) {
			AgingLevelSheltomIndex[y][h] = 0;
		}

	}

	AgingCheckSheltomFlag = 0; //����¡ ���� �÷� �ʱ�ȭ 
	AgingSheltomCnt2 = 0;
	if (!Flag)
		cAging.OpenFlag = 0; //â�� �ݴ´� 

	ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ
	return TRUE;
}

//����¡�Ҽ��ִ� ������������ üũ�Ѵ� 
int cAGING::CheckItem(sITEM * pItem)
{
	int k = 0;
	int SheltomCnt = 0;
	if ((pItem->CODE & sinITEM_MASK2) == sinOS1) {
		AgingSheltomCnt2++;

	}
	else {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				AgingLevelSheltomIndex[i][j] = AgingLevelSheltom[pItem->sItemInfo.ItemAgingNum[0]][k++]; //������ ������ �ε����� �ִ´� 

			}

		}

	}
	if (AgingSheltomCnt2) {
		if (sAgingItem.AgingItem[0].Flag) {
			for (int h = 0; h < 12; h++) {
				if (AgingLevelSheltom[sAgingItem.AgingItem[0].sItemInfo.ItemAgingNum[0]][h])
					SheltomCnt++;
			}

		}
		if (AgingSheltomCnt2 == SheltomCnt)
			AgingCheckSheltomFlag = 1;
		else
			AgingCheckSheltomFlag = 0;

	}

	return FALSE;

}

DWORD AgingItemCode3[12] = { sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1,sinDA1,sinDS1,sinOM1,sinDA2 }; // libera block aging bracelete/bota/luva xxstr
//DWORD AgingItemCode3[15] = { sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1,sinDA1,sinDS1,sinOM1,sinDA2,sinDG1,sinDB1,sinOA2 };

//PEDRAS AGE
int cAGING::SetAgingItemAreaCheck(sITEM * pItem)
{
	// ����� - ����¡ ���� ����(������ �߰�)
	DWORD SheltomCODE[MAX_SHELTOM] = { sin01,sin02,sin03,sin04,sin05,sin06,sin07,sin08,sin09,sin10,sin11,sin12,sin13,sin14/*,sin15*/ };
	int SheltomIndex2 = 0;
	int i, j;
	if (pItem->sItemInfo.CODE == (sinBI1 | sin10) || pItem->sItemInfo.CODE == (sinBI1 | sin11) ||
		pItem->sItemInfo.CODE == (sinBI1 | sin60) || pItem->sItemInfo.CODE == (sinBI1 | sin61) || // ����� - ���� ���� ����, ���� ����¡ ���� �߰�
		pItem->sItemInfo.CODE == (sinBI1 | sin85) || pItem->sItemInfo.CODE == (sinBI2 | sin38) || pItem->sItemInfo.CODE == (sinBI2 | sin52)) // �庰 - ���� ����¡ ���� 1.5
	{
		cAging.AginStoneKind = 0;
		int AgingStoneX = 159 + 11, AgingStoneY = 268 + 11 + sinInterHeight2;
		TradeColorIndex = 0; //�ʱ�ȭ   (Ʈ���̵� â�� â�� ���� ����)
		cTrade.InitTradeColorRect(); //�ʱ�ȭ
		if (AgingStoneX <= pItem->x + 22 && AgingStoneX > pItem->x && AgingStoneY< pItem->y + 22 && AgingStoneY > pItem->y) {
			pItem->SetX = AgingStoneX + (((pItem->x + 22 - AgingStoneX) / 22) * 22) - 11;
			pItem->SetY = AgingStoneY + (((pItem->y + 22 - AgingStoneY) / 22) * 22) - 11;
			pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
			TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���
			if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
				cTrade.InitTradeColorRect(); //�÷� �ڽ� RECT �ʱ�ȭ (�÷� �ڽ� RECT�� �ϳ���� 0 �̸� �����Ҽ� ���� 
				return FALSE;
			}
			if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //��ġ�� �������� �ֳ� üũ 
				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;
			}
			return TRUE;
		}
	}

	if (pItem->sItemInfo.CODE == (sinBI1 | sin61)) // ����� - ���� ���� ����, ���� ����¡ ���� �߰�
	{
		cAging.AginStoneKind = 4;
		int AgingStoneX = 159 + 11, AgingStoneY = 268 + 11 + sinInterHeight2;
		TradeColorIndex = 0; //�ʱ�ȭ   (Ʈ���̵� â�� â�� ���� ����)
		cTrade.InitTradeColorRect(); //�ʱ�ȭ
		if (AgingStoneX <= pItem->x + 22 && AgingStoneX > pItem->x && AgingStoneY< pItem->y + 22 && AgingStoneY > pItem->y) {
			pItem->SetX = AgingStoneX + (((pItem->x + 22 - AgingStoneX) / 22) * 22) - 11;
			pItem->SetY = AgingStoneY + (((pItem->y + 22 - AgingStoneY) / 22) * 22) - 11;
			pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
			TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���
			if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
				cTrade.InitTradeColorRect(); //�÷� �ڽ� RECT �ʱ�ȭ (�÷� �ڽ� RECT�� �ϳ���� 0 �̸� �����Ҽ� ���� 
				return FALSE;
			}
			if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //��ġ�� �������� �ֳ� üũ 
				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;
			}
			return TRUE;
		}
	}


	// �庰 - ���� ����¡ ���� 1.5
	if (pItem->sItemInfo.CODE == (sinBI1 | sin85))
	{
		cAging.AginStoneKind = 5;
		int AgingStoneX = 159 + 11, AgingStoneY = 268 + 11 + sinInterHeight2;
		TradeColorIndex = 0; //�ʱ�ȭ   (Ʈ���̵� â�� â�� ���� ����)
		cTrade.InitTradeColorRect(); //�ʱ�ȭ
		if (AgingStoneX <= pItem->x + 22 && AgingStoneX > pItem->x && AgingStoneY< pItem->y + 22 && AgingStoneY > pItem->y) {
			pItem->SetX = AgingStoneX + (((pItem->x + 22 - AgingStoneX) / 22) * 22) - 11;
			pItem->SetY = AgingStoneY + (((pItem->y + 22 - AgingStoneY) / 22) * 22) - 11;
			pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
			TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���
			if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
				cTrade.InitTradeColorRect(); //�÷� �ڽ� RECT �ʱ�ȭ (�÷� �ڽ� RECT�� �ϳ���� 0 �̸� �����Ҽ� ���� 
				return FALSE;
			}
			if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //��ġ�� �������� �ֳ� üũ 
				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;
			}
			return TRUE;
		}
	}

	//-----------------------------------------------------------------------------------
	if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2) {
		if (sAgingItem.AgingItem[0].Flag && (sAgingItem.AgingItem[0].sItemInfo.CODE & sinITEM_MASK2) == sinMA1) {
			TradeColorIndex = 0; //�ʱ�ȭ   (Ʈ���̵� â�� â�� ���� ����)
			TradeCrashItemIndex[0] = 0; //�浹�� ������ Index �ʱ�ȭ 
			cTrade.InitTradeColorRect(); //�ʱ�ȭ
			TradeStartX = 218;
			TradeStartY = 193 + sinInterHeight2;
			TradeEndX = TradeStartX + (3 * 22);
			TradeEndY = TradeStartY + (4 * 22);

			for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22) {
				for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22) {
					if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j) { //�κ��丮 �ڽ��� �������� üũ 
						TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
						TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
						TradeColorRect.right = pItem->w;
						TradeColorRect.bottom = pItem->h;
						if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
							cTrade.InitTradeColorRect(); //�÷� �ڽ� RECT �ʱ�ȭ (�÷� �ڽ� RECT�� �ϳ���� 0 �̸� �����Ҽ� ���� 
							return FALSE;
						}

						pItem->SetX = TradeColorRect.left;
						pItem->SetY = TradeColorRect.top;
						pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
						TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���

						if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //��ġ�� �������� �ֳ� üũ 
							TradeColorIndex = NOT_SETTING_COLOR;
							return FALSE;
						}
						return TRUE;

					}

				}

			}
		}

	}

	if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA1) {
		TradeColorIndex = 0; //�ʱ�ȭ   (Ʈ���̵� â�� â�� ���� ����)
		TradeCrashItemIndex[0] = 0; //�浹�� ������ Index �ʱ�ȭ 
		cTrade.InitTradeColorRect(); //�ʱ�ȭ

		TradeStartX = 58;
		TradeStartY = 193 + sinInterHeight2;
		TradeEndX = TradeStartX + (3 * 22);
		TradeEndY = TradeStartY + (4 * 22);
		if (TradeStartX <= pCursorPos.x && TradeEndX > pCursorPos.x && TradeStartY <= pCursorPos.y && TradeEndY > pCursorPos.y) { //�κ��丮 �ڽ��� �������� üũ 
			TradeColorRect.left = TradeStartX;
			TradeColorRect.top = TradeStartY;
			TradeColorRect.right = TradeEndX - TradeStartX;
			TradeColorRect.bottom = TradeEndY - TradeStartY;


			pItem->SetX = TradeStartX + ((TradeEndX - TradeStartX) - pItem->w) / 2;
			pItem->SetY = TradeStartY + ((TradeEndY - TradeStartY) - pItem->h) / 2;
			pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
			TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���
			if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //��ġ�� �������� �ֳ� üũ 
				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;

			}
			return TRUE;
		}
	}

	/////////����� �⺻����¡ 
	for (int y = 0; y < MAX_SHELTOM; y++) {
		if (SheltomCODE[y] == (pItem->sItemInfo.CODE & sinITEM_MASK3)) {
			SheltomIndex2 = y + 1;
			break;
		}
	}

	int AgingOkWeaponGroupFlag = 0;	//����¡�� �Ҽ��ִ� ������ ���ΰ�?
	TradeColorIndex = 0; //�ʱ�ȭ   (Ʈ���̵� â�� â�� ���� ����)
	TradeCrashItemIndex[0] = 0; //�浹�� ������ Index �ʱ�ȭ 
	cTrade.InitTradeColorRect(); //�ʱ�ȭ

	if ((pItem->CODE & sinITEM_MASK2) == sinOS1) { //������ ��� 
//		TradeStartX = 218;
//		TradeStartY = 193;
//		TradeEndX = TradeStartX+(3*22);
//		TradeEndY = TradeStartY+(4*22);

		for (int p = 0; p < 4; p++) {
			for (int m = 0; m < 3; m++) {
				if (AgingLevelSheltomIndex[p][m] == SheltomIndex2) {
					TradeStartX = AgingLevelSheltomXY[p][m].x;
					TradeStartY = AgingLevelSheltomXY[p][m].y;
					TradeEndX = TradeStartX + 22;
					TradeEndY = TradeStartY + 22;

					for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22) {
						for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22) {
							if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j) { //�κ��丮 �ڽ��� �������� üũ 
								TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
								TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
								TradeColorRect.right = pItem->w;
								TradeColorRect.bottom = pItem->h;
								if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
									cTrade.InitTradeColorRect(); //�÷� �ڽ� RECT �ʱ�ȭ (�÷� �ڽ� RECT�� �ϳ���� 0 �̸� �����Ҽ� ���� 
									return FALSE;
								}

								pItem->SetX = TradeColorRect.left;
								pItem->SetY = TradeColorRect.top;
								pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
								if ((pItem->CODE & sinITEM_MASK3) > sin20) // ����� - ���� ����(����¡�Ҷ� ���� ������ �÷����� ���ϵ��� �Ѵ�)
								{
									TradeColorIndex = NOT_SETTING_COLOR;
								}
								else
									TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���

								if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //��ġ�� �������� �ֳ� üũ 
									TradeColorIndex = NOT_SETTING_COLOR;
									return FALSE;

								}
								return TRUE;
							}
						}
					}
				}
			}
		}
	}
	else {
		TradeColorIndex = 0; //�ʱ�ȭ   (Ʈ���̵� â�� â�� ���� ����)
		TradeCrashItemIndex[0] = 0; //�浹�� ������ Index �ʱ�ȭ 
		cTrade.InitTradeColorRect(); //�ʱ�ȭ

		TradeStartX = 58;
		TradeStartY = 193 + sinInterHeight2;
		TradeEndX = TradeStartX + (3 * 22);
		TradeEndY = TradeStartY + (4 * 22);
		if (TradeStartX <= pCursorPos.x && TradeEndX > pCursorPos.x && TradeStartY <= pCursorPos.y && TradeEndY > pCursorPos.y) { //�κ��丮 �ڽ��� �������� üũ 
			TradeColorRect.left = TradeStartX;
			TradeColorRect.top = TradeStartY;
			TradeColorRect.right = TradeEndX - TradeStartX;
			TradeColorRect.bottom = TradeEndY - TradeStartY;


			pItem->SetX = TradeStartX + ((TradeEndX - TradeStartX) - pItem->w) / 2;
			pItem->SetY = TradeStartY + ((TradeEndY - TradeStartY) - pItem->h) / 2;
			pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
			TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���


			for (int i = 0; i < 12; i++) {
				if ((pItem->CODE & sinITEM_MASK2) == AgingItemCode3[i]) {
					AgingOkWeaponGroupFlag = 1;

				}
			}

			//��Ȱ�� ������ ����¡�Ҽ��ִ� 
			if (pItem->sItemInfo.CODE == (sinOR2 | sin01))AgingOkWeaponGroupFlag = 1;

			//����¡�� �Ҽ����� �����۱�  �ϰ�쿡�� 
			if (!AgingOkWeaponGroupFlag || pItem->sItemInfo.ItemAgingNum[0] >= MAX_AGING) {  //����¡ +18
				TradeColorIndex = NOT_AGING_ITEM;
				return FALSE;
			}

			//����¡ �Ҽ����� �������� ��� 
			if (pItem->sItemInfo.UniqueItem || pItem->sItemInfo.ItemKindCode == ITEM_KIND_CRAFT || MouseItem.sItemInfo.ItemAgingNum[1] == 1 ||
				pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
				pItem->sItemInfo.SpecialItemFlag[0] == CHECK_GIVE_ITEM ||
				pItem->sItemInfo.CODE == (sinGF1 | sin01) ||
				pItem->sItemInfo.CODE == (sinGF1 | sin02) ||
				pItem->sItemInfo.CODE == (sinGF1 | sin07) ||	// �庰 - ������� ã�ƶ�
				pItem->sItemInfo.CODE == (sinGF1 | sin08) ||	// �庰 - ������� ã�ƶ�
				pItem->sItemInfo.CODE == (sinGF1 | sin05) ||
				pItem->sItemInfo.CODE == (sinGF1 | sin06) ||
				//��Ƽ�� �ڽ�Ƭ

				(pItem->sItemInfo.CODE == (sinDB1 | sin31)) || (pItem->sItemInfo.CODE == (sinDB1 | sin32)) ||  // ����� - ���ǵ� ����(7��, 30��) �߰�
				(pItem->sItemInfo.CODE == (sinOR2 | sin33)) || (pItem->sItemInfo.CODE == (sinSP1 | sin35)) || // �庰 - ��Ʈ��, ���ݸ� ����¡ ����
				(pItem->sItemInfo.CODE == (sinOA1 | sin36)) ||	// �庰 - ���� ����� ����¡ ����
				(pItem->sItemInfo.CODE == (sinOA1 | sin37)) || (pItem->sItemInfo.CODE == (sinSP1 | sin36)) || // �庰 - ĵ������ ��Ʈ�ƹķ�, ĵ�� ����¡ ����
				(pItem->sItemInfo.CODE == (sinOA2 | sin33)) || // �庰 - ���� �ϸ�(1��)
				(pItem->sItemInfo.CODE == (sinDB1 | sin33)) || // �庰 - ���ǵ� ����(1��) �߰�
				(pItem->sItemInfo.CODE == (sinSP1 | sin37)) || (pItem->sItemInfo.CODE == (sinSP1 | sin38)) || // �庰 - �����ñ׸� ���޶���, ���� ����¡ ����
				(pItem->sItemInfo.CODE == (sinBI1 | sin84)) || // �庰 - �׶��Ƽ ��ũ��
				(pItem->sItemInfo.CODE == (sinDB1 | sin34)) || // �庰 - ���ǵ� ����(1�ð�) �߰�
				(pItem->sItemInfo.CODE == (sinOA2 | sin34)) ||// �庰 - ���� �ϸ�(1�ð�)
				(pItem->sItemInfo.CODE == (sinSP1 | sin42)) ||


				// Itens que n�o podem dar age
				pItem->sItemInfo.itemType == 7) {
				TradeColorIndex = NOT_AGING_ITEM;
				return FALSE;
			}
			int kk = 0;
			//��ϵ� �������� ����¡�Ҽ�����
			for (kk = 0; kk < NotSet_Item_CODECnt; kk++) {
				if (NotSet_Item_CODE[kk] == pItem->sItemInfo.CODE) {
					TradeColorIndex = NOT_AGING_ITEM;
					return FALSE;
				}

			}
			for (kk = 0; kk < NotSet_Item_MASKCnt; kk++) {
				if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == NotSet_Item_MASK[kk]) {
					TradeColorIndex = NOT_AGING_ITEM;
					return FALSE;
				}

			}
			for (kk = 0; kk < NotSet_Item_KINDCnt; kk++) {
				if (NotSet_Item_KIND[kk] == pItem->sItemInfo.ItemKindCode) {
					TradeColorIndex = NOT_AGING_ITEM;
					return FALSE;
				}

			}


			//�������� ��ĥ��� 
			if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //��ġ�� �������� �ֳ� üũ 
				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;

			}
			//--------------------------------------------------------------------------//
			if (AgingEvento == TRUE)
			{
			}
			else
			{
#ifdef HASIEGE_MODE
				//������ �������� ����¡ ������ üũ�Ѵ�.
				if (cShop.haBuyMoneyCheck((pItem->sItemInfo.Price + (pItem->sItemInfo.Price * pItem->sItemInfo.ItemAgingNum[0])) / 2) == 0) {
#else
				if (sinChar->Money < (pItem->sItemInfo.Price + (pItem->sItemInfo.Price * pItem->sItemInfo.ItemAgingNum[0])) / 2)
				{
#endif
					//---------------------------------------------------------------------------//	
					TradeColorIndex = NOT_ENOUGH_MONEY;
					return FALSE;
				}
				}
			return TRUE;
			}

			}


	return TRUE;
		}
/*
int CraftItemCheckDelayTime = 0;

//ũ����Ʈ �������� �����ϴ� ���� ������ ���� �����  (�������� �ϴ°���� �ǹ̰� �����ϳ� Ȥ�ó��ؼ� �ּ�ó����)
int cCRAFTITEM::CheckHackCraftItem()
{
	CraftItemCheckDelayTime++;
	if(CraftItemCheckDelayTime < 70*10)return FALSE;
	CraftItemCheckDelayTime = 0;


	unsigned int AddItemCheckSum = 0;
	unsigned int CheckAddItemData = 750604968;

	for(int i=0 ;i < MAX_CRAFTITEM_INFO ; i++){
		for(int j=0; j < 8 ;j++){
			AddItemCheckSum += (int)((j+1)*sCraftItem_Info[i].AddElement[j]);
			AddItemCheckSum += (j+1)*sCraftItem_Info[i].AddItem[j];
			AddItemCheckSum += (j+1)*sCraftItem_Info[i].ElementKind[j];
			AddItemCheckSum += (j+1)*sCraftItem_Info[i].PlusElementCode[j];
			AddItemCheckSum += (j+1)*sCraftItem_Info[i].MainItemCode[j];

		}

	}
	if( CheckAddItemData != AddItemCheckSum){
		SendSetHackUser(8); //��ŷ�� �Ϸ����ߴ� ���� ������ ��� TRUE ���� ����

	}

	return TRUE;
}

*/

int AgingItemCheckDelayTime = 0;

//����¡ ����Ÿ�� ���ۿ��θ� �����Ѵ� 
int cAGING::CheckAgingData()
{
	/*
	#ifdef __SIN_AGING_TEST


	#else
	AgingItemCheckDelayTime++;
	if(AgingItemCheckDelayTime < 70*10)return FALSE;
	AgingItemCheckDelayTime = 0;

	unsigned int AddItemCheckSum = 0;
	const unsigned int CheckAddItemData = 1848296; //�̷��� �س����� �б� �������θ� �ȴٳ�... ��������


	int i,j;
	for(i=0; i < 10 ; i++){
		AddItemCheckSum += (i+1)*AgingLevelAttack[i];
		AddItemCheckSum += (i+1)*AgingLevelCritical[i];
		AddItemCheckSum += (i+1)*AgingLevelBlock[i];
		AddItemCheckSum += (i+1)*AgingLevelHit[i];
		AddItemCheckSum += (i+1)*AgingOkPercent[i];
		for(j = 0; j < 12 ; j++){
			AddItemCheckSum += (i+1)*AgingLevelSheltom[i][j];


		}

	}
	if( CheckAddItemData != AddItemCheckSum){
		SendSetHackUser(107); //��ŷ�� �Ϸ����ߴ� ���� ������ ��� TRUE ���� ����

	}
#endif
	*/

	return TRUE;
}

////////////â�� ����Ⱦ������� �ִ����� ã�´� 
int cWAREHOUSE::CopyItemNotPickUp(sITEM * pItem, int JumpIndex)
{
	int i;
	if (smConfig.DebugMode)return TRUE; //����� ����϶��� ����ֵ� ũũũ
	for (i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			if (JumpIndex == i)continue; //�ڽ��� �������� ã���ʴ´�
			if (CompareItems(&pItem->sItemInfo, &sWareHouse.WareHouseItem[i].sItemInfo)) {
				CopyItemIndex7[i] = i + 1;
				return FALSE;

			}

		}

	}

	return TRUE;
}

int SendServerFlag7 = 0;
//����� �������� �ִ����� üũ�Ѵ� 
int cWAREHOUSE::CheckCopyItem()
{
	int i, j;
	if (SendServerFlag7)return FALSE;
	for (i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			for (j = 0; j < 100; j++) {
				if (i != j && sWareHouse.WareHouseItem[j].Flag) {
					if (CompareItems(&sWareHouse.WareHouseItem[i].sItemInfo, &sWareHouse.WareHouseItem[j].sItemInfo)) {
						SendSetHackUser2(1010, sWareHouse.WareHouseItem[i].CODE); //��ŷ�� ������ �Ű��Ѵ�
						SendServerFlag7 = 1; //�ѹ��� üũ�ϵ����Ѵ� 
						break;
					}
				}
			}
		}
	}
	/*
	//���޾� ���� üũ (��... �����ϴ�)
	if(8000000 < sWareHouse.Money-2023-1000000){
			SendSetHackUser2(1110,sWareHouse.Money-2023); //��ŷ�� ������ �Ű��Ѵ�

	}
	*/


	return TRUE;
}
int CheckCraftMoney = 0;

///////// �ͽ��� �������� ������ ������ 
int cCRAFTITEM::sinSendCraftItem()
{

	if (MixItemNoCopyFlag)return FALSE;
	int i = 0;


	//���縦 üũ�ϱ����� ������� �����
	memcpy(&sCraftItemBackUp, &sCraftItem, sizeof(sCRAFTITEM));

	MixItemNoCopyFlag = 1; //�������� ����Ǵ°� �����ϱ����� �÷����ش�

	if (ForceFlag) { //Force Orb �̸� 
		memset(&sCraftItem_Send.DesCraftItem, 0, sizeof(sITEM));
		memset(&sCraftItemBackUp, 0, sizeof(sCRAFTITEM));
		sCraftItem_Send.DesCraftItem.CODE = 1000;
		sCraftItem_Send.Money = ForceItemPrice;

	}
	//haGoon������ �籸�� �� ������ ������.---------------------------------------------------------------------------
	if (cCraftItem.iReconItemFlag) {
		memset(&sCraftItem_Send.DesCraftItem, 0, sizeof(sITEM));
		memset(&sCraftItemBackUp, 0, sizeof(sCRAFTITEM));
		sCraftItem_Send.DocIndex = 2000;

	}
	//----------------------------------------------------------------------------------------------------------------
	if (sCraftItem.CraftItem[0].Flag) {
		sCraftItem_Send.DesCraftItem = sCraftItem.CraftItem[0];

		if (MixEvento)
			sCraftItem_Send.Money = 0;
		else
			sCraftItem_Send.Money = sCraftItem.CraftItem[0].sItemInfo.Price;
	}
	for (i = 0; i < 12; i++) {
		if (sCraftItem.CraftItem[i + 1].Flag) {
			sCraftItem_Send.SheltomCode[i] = sCraftItem.CraftItem[i + 1].CODE;
			sCraftItem_Send.CheckSum[i] = sCraftItem.CraftItem[i + 1].sItemInfo.ItemHeader.dwChkSum;
			sCraftItem_Send.Head[i] = sCraftItem.CraftItem[i + 1].sItemInfo.ItemHeader.Head;
		}
	}

	// ����� - ������ ���浵 ������ ������.
	if (sCraftItem.CraftItem[14].Flag) {
		sCraftItem_Send.A_StoneItemCode = sCraftItem.CraftItem[14].CODE;
		sCraftItem_Send.A_StoneCheckSum = sCraftItem.CraftItem[14].sItemInfo.ItemHeader.dwChkSum;
		sCraftItem_Send.A_StoneHead = sCraftItem.CraftItem[14].sItemInfo.ItemHeader.Head;
	}


	CheckCraftMoney = sCraftItem_Send.Money;

	CraftItemSendServerIndex++; //�ε����� ������ 
	sCraftItem_Send.Index = CraftItemSendServerIndex;
	//����� ����ü�� ������ ������ 
	cCraftItem.iReconItemFlag = 0;

	if (ForceFlag) { //���� ����� ������ ������
		SendWingItemToServer(&sCraftItem_Send);
	}
	else {
		SendCraftItemToServer(&sCraftItem_Send);
	}

	return TRUE;
}
///////// ����¡ �������� ������ ������ 
int cAGING::sinSendAgingItem()
{
	int i = 0;
	//�������� ������ ���縦 �����ϱ����� ������� �����
	memcpy(&sAgingItemBackUp, &sAgingItem, sizeof(sAGINGITEM));

	MixItemNoCopyFlag = 1;
	if (sAgingItem.AgingItem[0].Flag)
	{
		sAging_Send.DesCraftItem = sAgingItem.AgingItem[0];
	}
	for (i = 0; i < 12; i++)
	{
		if (sAgingItem.AgingItem[i + 1].Flag)
		{
			sAging_Send.SheltomCode[i] = sAgingItem.AgingItem[i + 1].CODE;
			sAging_Send.CheckSum[i] = sAgingItem.AgingItem[i + 1].sItemInfo.ItemHeader.dwChkSum;
			sAging_Send.Head[i] = sAgingItem.AgingItem[i + 1].sItemInfo.ItemHeader.Head;
		}
	}
	//����¡ ���浵 ������ ������.
	if (sAgingItem.AgingItem[14].Flag)
	{
		sAging_Send.A_StoneItemCode = sAgingItem.AgingItem[14].CODE;
		sAging_Send.A_StoneCheckSum = sAgingItem.AgingItem[14].sItemInfo.ItemHeader.dwChkSum;
		sAging_Send.A_StoneHead = sAgingItem.AgingItem[14].sItemInfo.ItemHeader.Head;
	}

	if (AgingEvento == TRUE)
	{
		sAging_Send.Money = 0;
		CheckCraftMoney = sAging_Send.Money;
	}
	else
	{
		sAging_Send.Money = sAgingItem.AgingItem[0].sItemInfo.Price;
		CheckCraftMoney = sAging_Send.Money;
	}

	CraftItemSendServerIndex++; //�ε����� ������ 
	sAging_Send.Index = CraftItemSendServerIndex;

	//����� ����ü�� ������ ������ 
	SendAgingItemToServer(&sAging_Send);

	int Price = 0;

	if (AgingEvento == TRUE)
	{
		Price = 0; // ����
	}
	else
	{
		Price = ((sAgingItem.AgingItem[0].sItemInfo.ItemAgingNum[0] + 1) * sAgingItem.AgingItem[0].sItemInfo.Price) / 2;
	}


	CheckCharForm();		//���� 
	sinMinusMoney(Price, 1);
	ReformCharForm();		//������ 
	SendSaveMoney();		//�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
	return TRUE;
}
///////// �������� �ͽ��� �������� ������ �޴´� 
int cCRAFTITEM::sinRecvCraftItem(sCRAFTITEM_SERVER * pCraftItem_Server)
{
	memcpy(&sCraftItem_Recv, pCraftItem_Server, sizeof(sCRAFTITEM_SERVER));

	return TRUE;
}

///////// �������� ����¡ �������� ������ �޴´�
int cAGING::sinRecvAgingItem(sCRAFTITEM_SERVER * pCraftItem_Server)
{
	memcpy(&sAging_Recv, pCraftItem_Server, sizeof(sCRAFTITEM_SERVER));

	return TRUE;
}

//����¡ �������� ����� �������� �޴´� 
int cAGING::sinRecvAgingItemResult(sCRAFTITEM_SERVER * pCraftItem_Server)
{

	//����Ʈ �������ϰ��
	if (pCraftItem_Server->DesCraftItem.sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON) {
		if (pCraftItem_Server->Result) {
			cInvenTory.DeleteInvenItemToServer(TempQuestItem.sItemInfo.CODE, TempQuestItem.sItemInfo.ItemHeader.Head, TempQuestItem.sItemInfo.ItemHeader.dwChkSum);
			memset(&TempQuestItem, 0, sizeof(sITEM));
			//ī��Ʈ ����
			pCraftItem_Server->DesCraftItem.sItemInfo.ItemAgingCount[0] = pCraftItem_Server->DesCraftItem.sItemInfo.ItemAgingCount[1];
			if (cInvenTory.AutoSetInvenItem(&pCraftItem_Server->DesCraftItem)) {
				cMessageBox.ShowMessage(MESSAGE_QUEST_ITEM_AGING);
				sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
				StartEffect(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, EFFECT_AGING); //����¡����Ʈ
				sinQuest_ChangeJob3.Monster[0] = pCraftItem_Server->DesCraftItem.sItemInfo.ItemAgingNum[0];
				StartQuest_Code(sinQuest_ChangeJob3.CODE);

			}
			//����Ʈ �޼����� �����ش�
			if (pCraftItem_Server->DesCraftItem.sItemInfo.ItemAgingNum[0] == 4) {
				sinQuest_ChangeJob3.State = 4;
			}
		}
		else {
			//������ �������� �������ش�
			cInvenTory.AutoSetInvenItem(&TempQuestItem);

		}
		memset(&sAging_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //���� �ͽ��� ����ü �ʱ�ȭ 
		memset(&sAging_Send, 0, sizeof(sCRAFTITEM_SERVER)); //���� �ͽ��� ����ü �ʱ�ȭ 
		NotChangeSetItemFlag = 0; //������� ü���� ������ �ʱ�ȭ���ش�
		return TRUE;

	}

	if (!cAging.OpenFlag) {
		AgingCheckSheltomFlag = 0; //����¡ ���� �÷� �ʱ�ȭ 
		AgingSheltomCnt2 = 0;
		MixItemNoCopyFlag = 0; //������� �÷� �ʱ�ȭ
		memset(&sCraftItem_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //���� �ͽ��� ����ü �ʱ�ȭ 
		memset(&sCraftItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //���� �ͽ��� ����ü �ʱ�ȭ 
		MixCancelButtonDelayFlag = 0;
		return FALSE;
	}

	if (pCraftItem_Server->Result) {
		if ((pCraftItem_Server->DesCraftItem.sItemInfo.CODE & sinITEM_MASK2) == sinQT1) {
			sinSetQuestItem(&pCraftItem_Server->DesCraftItem.sItemInfo);
		}
		else {
			if (!cInvenTory.AutoSetInvenItem(&pCraftItem_Server->DesCraftItem)) {
				sinThrowItemToFeild(&pCraftItem_Server->DesCraftItem);

			}
			CheckCharForm(); //����
			switch (pCraftItem_Server->Result) {
			case 1: //����¡ -1
				cMessageBox.ShowMessage(MESSAGE_AGING_LEVEL_DOWN1);
				sinPlaySound(SIN_SOUND_FAILD_MIXITEM); //���� ���� 
				break;
			case 2: //����¡ -2
				cMessageBox.ShowMessage(MESSAGE_AGING_LEVEL_DOWN2);
				sinPlaySound(SIN_SOUND_FAILD_MIXITEM); //���� ���� 
				break;
			case 3: //����
				cMessageBox.ShowMessage(MESSAGE_AGING_LEVEL_PLUS);
				sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
				break;
			case 4: //2�� 
				cMessageBox.ShowMessage(MESSAGE_AGING_LEVEL_PLUS2);
				sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
				break;
			}
			SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
			ReformCharForm(); //������ 
		}
	}
	else {
		sinPlaySound(SIN_SOUND_FAILD_MIXITEM); //���� ���� 
		cMessageBox.ShowMessage(MESSAGE_FAILD_AGING_ITEM);

	}

	for (int y = 0; y < 4; y++) { //���� �ε��� �ʱ�ȭ 
		for (int h = 0; h < 3; h++) {
			AgingLevelSheltomIndex[y][h] = 0;
		}

	}

	for (int i = 0; i < 15; i++) { //����¡ �������� ������
		if (sAgingItemBackUp.AgingItem[i].Flag) {
			cInvenTory.DeleteInvenItemToServer(sAgingItemBackUp.AgingItem[i].sItemInfo.CODE,
				sAgingItemBackUp.AgingItem[i].sItemInfo.ItemHeader.Head,
				sAgingItemBackUp.AgingItem[i].sItemInfo.ItemHeader.dwChkSum);
		}

	}


	memset(&sAgingItem, 0, sizeof(sAGINGITEM));
	memset(&sAgingItemBackUp, 0, sizeof(sAGINGITEM));
	cAging.OpenFlag = 0; //����¡ �������̽��� �ݴ´� 
	AgingCheckSheltomFlag = 0; //����¡ ���� �÷� �ʱ�ȭ 
	AgingSheltomCnt2 = 0;
	MixItemNoCopyFlag = 0; //������� �÷� �ʱ�ȭ
	MixCancelButtonDelayFlag = 0;
	memset(&sAging_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //���� �ͽ��� ����ü �ʱ�ȭ 
	memset(&sAging_Send, 0, sizeof(sCRAFTITEM_SERVER)); //���� �ͽ��� ����ü �ʱ�ȭ 
	cInvenTory.CheckDamage(); //�ִ뵥������ ���� ����� ����Ͽ� ������ �����ش� 
	return TRUE;
}

//check item aging xxstr
bool cAGING::isAgeableItem(sITEM * pItem)
{
	if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinBI1)
	{
		if ((pItem->sItemInfo.CODE & sinITEM_MASK3) == sin10 ||
			(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin11 ||
			(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin60 ||
			(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin61 ||
			(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin85)
		{
			return true;
		}
		return false;
	}
	else if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
		(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2)
	{
		return true;
	}

	// nao pode dar aging
	else if ((pItem->sItemInfo.CODE & sinITEM_MASK2) != sinOS1)
	{
		bool isAgeable = false;
		for (int i = 0; i < 12; i++)
		{
			if ((pItem->CODE & sinITEM_MASK2) == AgingItemCode3[i])
			{
				isAgeable = true;
				break;
			}
		}

		if (pItem->sItemInfo.CODE == (sinOR2 | sin01))
			isAgeable = false;

		if (!isAgeable || pItem->sItemInfo.ItemAgingNum[0] >= MAX_AGING)
		{
			return false;
		}

		if (pItem->sItemInfo.CODE == (sinDA1 | sin51) ||
			pItem->sItemInfo.CODE == (sinDA2 | sin51))
			return false;

		if (pItem->sItemInfo.UniqueItem || pItem->sItemInfo.ItemKindCode == ITEM_KIND_CRAFT || pItem->sItemInfo.ItemAgingNum[1] > 0 ||
			pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
			pItem->sItemInfo.SpecialItemFlag[0] == CHECK_GIVE_ITEM ||
			pItem->sItemInfo.CODE == (sinGF1 | sin01) ||
			pItem->sItemInfo.CODE == (sinGF1 | sin02) ||
			pItem->sItemInfo.CODE == (sinGF1 | sin07) ||
			pItem->sItemInfo.CODE == (sinGF1 | sin08) ||
			pItem->sItemInfo.CODE == (sinGF1 | sin05) ||
			pItem->sItemInfo.CODE == (sinGF1 | sin06) ||
			((pItem->sItemInfo.CODE == (sinDA1 | sin31))) ||
			((pItem->sItemInfo.CODE == (sinDA2 | sin31))) ||
			((pItem->sItemInfo.CODE == (sinDA1 | sin32))) ||
			((pItem->sItemInfo.CODE == (sinDA2 | sin32))) ||
			((pItem->sItemInfo.CODE == (sinDA1 | sin33))) ||
			((pItem->sItemInfo.CODE == (sinDA2 | sin33))) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin34)) || (pItem->sItemInfo.CODE == (sinDA2 | sin34)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin35)) || (pItem->sItemInfo.CODE == (sinDA2 | sin35)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin36)) || (pItem->sItemInfo.CODE == (sinDA2 | sin36)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin37)) || (pItem->sItemInfo.CODE == (sinDA2 | sin37)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin38)) || (pItem->sItemInfo.CODE == (sinDA2 | sin38)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin39)) || (pItem->sItemInfo.CODE == (sinDA2 | sin39)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin40)) || (pItem->sItemInfo.CODE == (sinDA2 | sin40)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin41)) || (pItem->sItemInfo.CODE == (sinDA2 | sin41)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin42)) || (pItem->sItemInfo.CODE == (sinDA2 | sin42)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin43)) || (pItem->sItemInfo.CODE == (sinDA2 | sin43)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin44)) || (pItem->sItemInfo.CODE == (sinDA2 | sin44)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin45)) || (pItem->sItemInfo.CODE == (sinDA2 | sin45)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin46)) || (pItem->sItemInfo.CODE == (sinDA2 | sin46)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin54)) || (pItem->sItemInfo.CODE == (sinDA2 | sin54)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin55)) || (pItem->sItemInfo.CODE == (sinDA2 | sin55)) ||
			(pItem->sItemInfo.CODE == (sinOA2 | sin31)) || (pItem->sItemInfo.CODE == (sinOA2 | sin32)) ||
			(pItem->sItemInfo.CODE == (sinDB1 | sin31)) || (pItem->sItemInfo.CODE == (sinDB1 | sin32)) ||
			(pItem->sItemInfo.CODE == (sinOR2 | sin33)) || (pItem->sItemInfo.CODE == (sinSP1 | sin35)) ||
			(pItem->sItemInfo.CODE == (sinOA1 | sin36)) ||
			(pItem->sItemInfo.CODE == (sinOA1 | sin37)) || (pItem->sItemInfo.CODE == (sinSP1 | sin36)) ||
			(pItem->sItemInfo.CODE == (sinOA2 | sin33)) ||
			(pItem->sItemInfo.CODE == (sinDB1 | sin33)) ||
			(pItem->sItemInfo.CODE == (sinSP1 | sin37)) || (pItem->sItemInfo.CODE == (sinSP1 | sin38)) ||
			(pItem->sItemInfo.CODE == (sinBI1 | sin84)) ||
			(pItem->sItemInfo.CODE == (sinDB1 | sin34)) ||
			(pItem->sItemInfo.CODE == (sinOA2 | sin34)) ||
			(pItem->sItemInfo.CODE == (sinSP1 | sin42)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin05)) ||
			(pItem->sItemInfo.CODE == (sinDB1 | sin05)) ||
			(pItem->sItemInfo.CODE == (sinDG1 | sin05)) ||
			(pItem->sItemInfo.CODE == (sinDS1 | sin05)) ||
			(pItem->sItemInfo.CODE == (sinOA1 | sin05)) ||
			(pItem->sItemInfo.CODE == (sinOA2 | sin10)) ||
			(pItem->sItemInfo.CODE == (sinOR1 | sin05)) ||
			(pItem->sItemInfo.CODE == (sinWA1 | sin05)) ||
			(pItem->sItemInfo.CODE == (sinWH1 | sin06)) ||
			(pItem->sItemInfo.CODE == (sinWM1 | sin06)) ||
			(pItem->sItemInfo.CODE == (sinWP1 | sin05)) ||
			(pItem->sItemInfo.CODE == (sinWS1 | sin05)) ||
			(pItem->sItemInfo.CODE == (sinWS2 | sin10)) ||
			(pItem->sItemInfo.CODE == (sinWT1 | sin05)) ||

			(pItem->sItemInfo.CODE == (sinDA1 | sin48)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin47))
			)
		{
			return false;
		}
		int kk = 0;
		for (kk = 0; kk < NotSet_Item_CODECnt; kk++)
		{
			if (NotSet_Item_CODE[kk] == pItem->sItemInfo.CODE)
			{
				return false;
			}
		}
		for (kk = 0; kk < NotSet_Item_MASKCnt; kk++)
		{
			if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == NotSet_Item_MASK[kk])
			{
				return false;
			}
		}
		for (kk = 0; kk < NotSet_Item_KINDCnt; kk++)
		{
			if (NotSet_Item_KIND[kk] == pItem->sItemInfo.ItemKindCode)
			{
				return false;
			}
		}
	}

	return true;
}

bool cAGING::isItemAreaEmpty()
{
	return sAgingItem.AgingItem[0].Flag == 0;
}

bool cAGING::isStoneAreaEmpty()
{
	return sAgingItem.AgingItem[14].Flag == 0;

}

bool cAGING::isSheltomAreaEmpty(sITEM * Item, int Area)
{
	DWORD SheltomCODE[MAX_SHELTOM] = { sin01,sin02,sin03,sin04,sin05,sin06,sin07,sin08,sin09,sin10,sin11,sin12,sin13,sin14 };

	int SheltomIndex = 0;
	for (int i = 0; i < MAX_SHELTOM; i++)
	{
		if (SheltomCODE[i] == (Item->CODE & sinITEM_MASK3))
		{
			SheltomIndex = i + 1;
			break;
		}
	}

	if (AgingLevelSheltomIndex[Area / 3][Area % 3] == SheltomIndex)
	{
		int CheckX = 218 + ((Area % 3) * 22);
		int CheckY = (193 + sinInterHeight2) + ((Area / 3) * 22);

		for (int i = 1; i < 15; i++)
		{
			if (sAgingItem.AgingItem[i].Flag && sAgingItem.AgingItem[i].CODE == Item->CODE)
			{
				if (sAgingItem.AgingItem[i].x >= CheckX && sAgingItem.AgingItem[i].x < CheckX + 22)
					if (sAgingItem.AgingItem[i].y >= CheckY && sAgingItem.AgingItem[i].y < CheckY + 22)
						return false;
			}
		}
		return true;
	}

	return false;
}

//ũ����Ʈ �������� ������� �������� �޴´� 
int cCRAFTITEM::sinRecvCraftItemResult(sCRAFTITEM_SERVER * pCraftItem_Server)
{
	CheckMixItemCopyFlag = 0; //������� �÷�  
	if (!cCraftItem.OpenFlag) {
		MixItemNoCopyFlag = 0; //������� �÷� �ʱ�ȭ
		memset(&sCraftItem, 0, sizeof(sCRAFTITEM)); //����ü �ʱ�ȭ 
		memset(&sCraftItemBackUp, 0, sizeof(sCRAFTITEM)); //����ü �ʱ�ȭ 
		memset(&sCraftItem_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //���� �ͽ��� ����ü �ʱ�ȭ 
		memset(&sCraftItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //���� �ͽ��� ����ü �ʱ�ȭ 
		MixCancelButtonDelayFlag = 0;
		return FALSE;

	}

	int i, LineCount = 0, Temp;

	/*   ������ �κ��� ����
	if(CheckCraftMoney != pCraftItem_Server->Money) //���� ���� ������ ���з� �����Ѵ�
		pCraftItem_Server->Result = 0;
	*/

	if (pCraftItem_Server->Result) {
		if (pCraftItem_Server->DocIndex == 2000) { //������ �籸���ý��� 
			//�������� ��� 
			sITEM TempItem;
			if (LoadItemImage(&pCraftItem_Server->DesCraftItem.sItemInfo, &TempItem)) {
				if (cInvenTory.CheckRequireItemToSet(&TempItem)) {         //��ٷ� ���õɼ��ִ����� üũ�Ѵ�  
					//haGoon����================================================================================
					if (!cInvenTory.AutoSetInvenItem(&TempItem, 1)) {
						sinThrowItemToFeild(&pCraftItem_Server->DesCraftItem);
						memset(&sCraftItem, 0, sizeof(sCRAFTITEM)); //����ü �ʱ�ȭ 
						cCraftItem.OpenFlag = 0;
						return FALSE;
					}
					//===========================================================================================
				}

			}
			cMessageBox.ShowMessage(MESSAGE_RECONITEM_OK);
		}
		else {
			ShowItemCraftMessageFlag = 1; //�������� ���Չ����� �޼����� �����ش� 
			lstrcpy(szCraftItemBuff, sCraftItem_Info[pCraftItem_Server->DocIndex].Doc); //������ ������ �����ش� 

			Temp = lstrlen(szCraftItemBuff);
			for (i = 0; i < Temp; i++) {
				if (szCraftItemBuff[i] == '\r') {
					LineCount++;
				}
			}

			CraftItemMessageSize.x = 14;           //�ڽ� ������ 
			CraftItemMessageSize.y = ((LineCount) * 2) + 2;  //�ڽ� ������ 


			sinAgingSuccessFlag2 = 1;

			//CheckCharForm(); //���� 
			//sinChar->Money -= sCraftItem_Recv.DesCraftItem.sItemInfo.Price;
			//sinMinusMoney(pCraftItem_Server->DesCraftItem.sItemInfo.Price,1);
			//ReformCharForm(); //������ 
			//SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
			//���е� �����̵簣�� �������� ���� ���� ��Ų�� 
			//cCraftItem.OpenFlag = 0; //â�� �ݴ´�
			//cInvenTory.AutoSetInvenItem(&pCraftItem_Server->DesCraftItem); //�������� �����Ѵ� 

			//haGoon���� ============================================================================
			if (!cInvenTory.AutoSetInvenItem(&pCraftItem_Server->DesCraftItem)) {
				sinThrowItemToFeild(&pCraftItem_Server->DesCraftItem);
			}
			//=======================================================================================

			//���Ⱑ ����Ǿ����� ���⸦ �����
			if (sCraftItemBackUp.CraftItem[0].Flag) {
				cInvenTory.DeleteInvenItemToServer(sCraftItemBackUp.CraftItem[0].sItemInfo.CODE,
					sCraftItemBackUp.CraftItem[0].sItemInfo.ItemHeader.Head,
					sCraftItemBackUp.CraftItem[0].sItemInfo.ItemHeader.dwChkSum);
			}

			/*
			for(int y=1;y<15;y++){ //���� ������ ������ �˷��ش�
				if(sCraftItem.CraftItem[y].Flag)
					SendSaveThrowItem(&sCraftItem.CraftItem[y].sItemInfo);

			}
			*/
		}
		cCraftItem.OpenFlag = 0; //â�� �ݴ´�
		CheckCharForm(); //���� 

		if (MixEvento)
			sinMinusMoney(0, 1);
		else
			sinMinusMoney(sCraftItem.CraftItem[0].sItemInfo.Price, 1);

		ReformCharForm(); //������ 
		SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
		SaveGameData();
	}
	else {


		//haGoon����============================================================================
		if (!cInvenTory.AutoSetInvenItem(&sCraftItem.CraftItem[0])) { //�������� �����Ѵ� 
			sinThrowItemToFeild(&sCraftItem.CraftItem[0]);
		}
		//=======================================================================================
		/*
		for(int y=1;y<15;y++){ //���� ������ ������ �˷��ش�
			if(sCraftItem.CraftItem[y].Flag)
				SendSaveThrowItem(&sCraftItem.CraftItem[y].sItemInfo);

		}
		*/

		//�ͽ��� �Ҷ� ���� ���� 
		CheckCharForm(); //���� 

		if (MixEvento)
			sinMinusMoney(0, 1);
		else
			sinMinusMoney(sCraftItem.CraftItem[0].sItemInfo.Price, 1);

		ReformCharForm(); //������ 
		SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 

		cCraftItem.OpenFlag = 0; //â�� �ݴ´� 
		cMessageBox.ShowMessage(MESSAGE_FAILD_CRAFT_ITEM);
		sinPlaySound(SIN_SOUND_FAILD_MIXITEM);

	}

	//���簡�Ǿ��ų��� �������� ����� (���Ҹ������)
	for (i = 1; i < 15; i++) {
		if (sCraftItemBackUp.CraftItem[i].Flag) {
			cInvenTory.DeleteInvenItemToServer(sCraftItemBackUp.CraftItem[i].sItemInfo.CODE,
				sCraftItemBackUp.CraftItem[i].sItemInfo.ItemHeader.Head,
				sCraftItemBackUp.CraftItem[i].sItemInfo.ItemHeader.dwChkSum);
		}

	}
	memset(&sCraftItem, 0, sizeof(sCRAFTITEM)); //����ü �ʱ�ȭ 
	memset(&sCraftItemBackUp, 0, sizeof(sCRAFTITEM)); //����ü �ʱ�ȭ 
	memset(&sCraftItem_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //���� �ͽ��� ����ü �ʱ�ȭ 
	memset(&sCraftItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //���� �ͽ��� ����ü �ʱ�ȭ 

	MixItemNoCopyFlag = 0; //������� �÷� �ʱ�ȭ
	MixCancelButtonDelayFlag = 0;
	return TRUE;
}

////////���� �������� �����ش� 
int cWAREHOUSE::DeleteCopyItem()
{
	int i, j;
	for (i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			for (j = 0; j < 100; j++) {
				if (i != j && sWareHouse.WareHouseItem[j].Flag) {
					if (CompareItems(&sWareHouse.WareHouseItem[i].sItemInfo, &sWareHouse.WareHouseItem[j].sItemInfo)) {
						//SendSetHackUser2(1010,sWareHouse.WareHouseItem[i].CODE); //��ŷ�� ������ �Ű��Ѵ�
						//SendServerFlag7 = 1; //�ѹ��� üũ�ϵ����Ѵ� 
						sWareHouse.WareHouseItem[j].Flag = 0; //���� �������� ������ 
						cWareHouse.ReFormWareHouse();
						break;
					}
				}
			}
		}
	}
	return TRUE;
}

////////�ͽ����� ����� �������� �����ش� 
int CopyMixItemCheckDelete(sITEM * pItem)
{
	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (cInvenTory.InvenItem[i].Flag) {
			if (CompareItems(&cInvenTory.InvenItem[i].sItemInfo, &pItem->sItemInfo)) {
				cInvenTory.InvenItem[i].Flag = 0; //����� �������� ����� 
			}
		}
		if (cInvenTory.InvenItemTemp[i].Flag) {
			if (CompareItems(&cInvenTory.InvenItemTemp[i].sItemInfo, &pItem->sItemInfo)) {
				cInvenTory.InvenItemTemp[i].Flag = 0; //����� �������� ����� 
			}
		}
	}
	return TRUE;
}

//Make�� �������� �´°� �ڵ带 Ȯ���Ѵ�
int sinMakeItemCheck()
{
	int ResultCount = 0;
	int ResultCountCheck = 0;
	DWORD TempCODE[15] = { 0, };
	int  i = 0;
	//�ڵ� ���� 
	for (i = 0; i < MAX_MAKEITEM_INFO; i++) {
		for (int p = 0; p < 15; p++) {  //�ڵ带 �ѹ��� �ʱ�ȭ���ش� 
			if (sAgingItem.AgingItem[p].Flag)
				TempCODE[p] = sAgingItem.AgingItem[p].CODE;
		}

		for (int j = 0; j < 15; j++) {
			if (sMakeItem_Info[i].CODE[j]) {
				ResultCount++;
				for (int k = 0; k < 15; k++) {
					if (sAgingItem.AgingItem[k].Flag && TempCODE[k]) {
						if (TempCODE[k] == sMakeItem_Info[i].CODE[j]) {
							ResultCountCheck++;
							TempCODE[k] = -1;
						}
					}
				}
			}
		}
		if (ResultCount && ResultCountCheck && (ResultCount == ResultCountCheck)) {
			AgingCheckSheltomFlag = 1;
			return TRUE;
		}
		else AgingCheckSheltomFlag = 0;
		ResultCount = 0;
		ResultCountCheck = 0;

	}

	return TRUE;
}
/*****************************************************************************/
/*							PostBox �ý���
/*****************************************************************************/
int sinPosBoxNpc()
{
	//���� �ϴ��� ���� ��� �Լ� -_-;
	//�ʱ�ȭ
	//memset(sPostbox_Item,0,sizeof(sPOSTBOX_ITEM));
	//PostBoxIndexCnt = 0; 

	//�޼����� ���� 
	HelpBoxKindIndex = SIN_POST_BOX;
	cSinHelp.sinShowHelp(SIN_HELP_KIND_POSTBOX, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), PostBoxDocFilePath[0]);
	cInvenTory.OpenFlag = 1;
	cInterFace.CheckAllBox(SIN_INVENTORY); //�κ��丮 â�� ���� 
	return TRUE;
}
//haGoon������ �籸���� üũ�Ѵ�.----------------------------------------------------------------------------
int cCRAFTITEM::haCheckReconItem()
{
	int cnt[MAX_SEEL_COUNT] = { 0 }; // ����� - ���̿��� �� �߰� (3���� -> 4����)
	int CheckNotReconItem = 0;
	int CheckBlockCount = 0;
	cCraftItem.iReconItemFlag = 0;

	//���������� �ƴѰ��� �� üũ�Ѵ�.
	for (int k = 1; k < 14; k++) { // ����� - ������ ����(sCraftItem.CraftItem[14]�� ������ �����̹Ƿ� üũ���� �����Ѵ�) for(int k=1;k<15;k++) -> for(int k=1;k<14;k++)
		if (sCraftItem.CraftItem[k].Flag) {
			if ((sCraftItem.CraftItem[k].CODE & sinITEM_MASK2) == sinSE1)
				CheckBlockCount++;   //�� ������ üũ
			else
				CheckNotReconItem++; //���ܾ����� üũ
		}
		else
			memset(&sCraftItem.CraftItem[k], 0, sizeof(sITEM));  //���� ��ŷ ���� ������ �ӽ� �����̴�.
	}
	//���� üũ�Ѵ�.
	for (int i = 1; i < 14; i++) { // ����� - ������ ����(sCraftItem.CraftItem[14]�� ������ �����̹Ƿ� üũ���� �����Ѵ�) for(int k=1;k<15;k++) -> for(int k=1;k<14;k++)
		if ((sCraftItem.CraftItem[i].CODE & sinITEM_MASK2) == sinSE1 && sCraftItem.CraftItem[i].Flag) {
			for (int k = 0; k < MAX_SEEL_COUNT; k++) { // ����� - ���̿��� �� �߰� (3���� -> 4����)
				if (sCraftItem.CraftItem[i].CODE == sReconItem[k].dwCODE) {
					cnt[k]++;
					if (sReconItem[k].iCount == cnt[k] && CheckNotReconItem < 1 && CheckBlockCount == cnt[k]) {
						cCraftItem.iReconItemFlag = 1;
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}
/*----------------------------------------------------------------------------*
*Desc: ��迭�� ����ش�.
*-----------------------------------------------------------------------------*/
int cCRAFTITEM::haCraftSortItem(sCRAFTITEM * pCraftItem)
{
	//�� ������ �迭�� ����ش�.
	int i;
	for (i = 1; i < 13; i++) {
		if (!pCraftItem->CraftItem[i].Flag) {
			pCraftItem->CraftItem[i] = pCraftItem->CraftItem[i + 1];
			pCraftItem->CraftItem[i + 1].Flag = 0;
		}
	}
	return TRUE;
}

// check sheltom mix xxstr
bool cCRAFTITEM::isSheltomAreaEmpty(sITEM * Item, int Area)
{
	int CheckX = 218 + ((Area % 3) * 22);
	int CheckY = (193 + sinInterHeight2) + ((Area / 3) * 22);

	for (int i = 1; i < 15; i++)
	{
		if (sCraftItem.CraftItem[i].Flag)
		{
			if (sCraftItem.CraftItem[i].x >= CheckX && sCraftItem.CraftItem[i].x < CheckX + 22)
			{
				if (sCraftItem.CraftItem[i].y >= CheckY && sCraftItem.CraftItem[i].y < CheckY + 22)
				{
					return false;
				}
			}
		}
	}

	return true;

}
// nao pode dar mix
bool cCRAFTITEM::isMixableItem(sITEM * pItem)
{
	static DWORD MixItemCode[17] = { sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1,sinDA1,sinDS1,sinOM1,sinDA2,sinDG1,sinDB1,sinOA2, sinOA1, sinOR1 };

	if ((pItem->sItemInfo.CODE & sinITEM_MASK2) != sinOS1 && (pItem->sItemInfo.CODE & sinITEM_MASK2) != sinSE1)
	{
		bool isMixable = false;
		for (int i = 0; i < sizeof(MixItemCode) / sizeof(DWORD); i++)
		{
			if ((pItem->CODE & sinITEM_MASK2) == MixItemCode[i])
			{
				isMixable = true;
				break;
			}
		}

		if (pItem->sItemInfo.CODE == (sinOR2 | sin01))
			isMixable = false;


		if (!isMixable)
			return false;

		if (pItem->sItemInfo.CODE == (sinDA1 | sin51) ||
			pItem->sItemInfo.CODE == (sinDA2 | sin51))
			return false;

		if (pItem->sItemInfo.UniqueItem || pItem->sItemInfo.ItemKindCode == ITEM_KIND_CRAFT || pItem->sItemInfo.ItemKindCode == ITEM_KIND_AGING ||
			pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
			pItem->sItemInfo.SpecialItemFlag[0] == CHECK_GIVE_ITEM ||
			pItem->sItemInfo.CODE == (sinGF1 | sin01) ||
			pItem->sItemInfo.CODE == (sinGF1 | sin02) ||
			pItem->sItemInfo.CODE == (sinGF1 | sin07) ||
			pItem->sItemInfo.CODE == (sinGF1 | sin08) ||
			pItem->sItemInfo.CODE == (sinGF1 | sin05) ||
			pItem->sItemInfo.CODE == (sinGF1 | sin06) ||
			((pItem->sItemInfo.CODE == (sinDA1 | sin31))) ||
			((pItem->sItemInfo.CODE == (sinDA2 | sin31))) ||
			((pItem->sItemInfo.CODE == (sinDA1 | sin32))) ||
			((pItem->sItemInfo.CODE == (sinDA2 | sin32))) ||
			((pItem->sItemInfo.CODE == (sinDA1 | sin33))) ||
			((pItem->sItemInfo.CODE == (sinDA2 | sin33))) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin34)) || (pItem->sItemInfo.CODE == (sinDA2 | sin34)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin35)) || (pItem->sItemInfo.CODE == (sinDA2 | sin35)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin36)) || (pItem->sItemInfo.CODE == (sinDA2 | sin36)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin37)) || (pItem->sItemInfo.CODE == (sinDA2 | sin37)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin38)) || (pItem->sItemInfo.CODE == (sinDA2 | sin38)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin39)) || (pItem->sItemInfo.CODE == (sinDA2 | sin39)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin40)) || (pItem->sItemInfo.CODE == (sinDA2 | sin40)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin41)) || (pItem->sItemInfo.CODE == (sinDA2 | sin41)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin42)) || (pItem->sItemInfo.CODE == (sinDA2 | sin42)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin43)) || (pItem->sItemInfo.CODE == (sinDA2 | sin43)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin44)) || (pItem->sItemInfo.CODE == (sinDA2 | sin44)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin45)) || (pItem->sItemInfo.CODE == (sinDA2 | sin45)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin46)) || (pItem->sItemInfo.CODE == (sinDA2 | sin46)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin54)) || (pItem->sItemInfo.CODE == (sinDA2 | sin54)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin55)) || (pItem->sItemInfo.CODE == (sinDA2 | sin55)) ||
			(pItem->sItemInfo.CODE == (sinOA2 | sin31)) || (pItem->sItemInfo.CODE == (sinOA2 | sin32)) ||
			(pItem->sItemInfo.CODE == (sinDB1 | sin31)) || (pItem->sItemInfo.CODE == (sinDB1 | sin32)) ||
			(pItem->sItemInfo.CODE == (sinOR2 | sin33)) || (pItem->sItemInfo.CODE == (sinSP1 | sin35)) ||
			(pItem->sItemInfo.CODE == (sinOA1 | sin36)) ||
			(pItem->sItemInfo.CODE == (sinOA1 | sin37)) || (pItem->sItemInfo.CODE == (sinSP1 | sin36)) ||
			(pItem->sItemInfo.CODE == (sinOA2 | sin33)) ||
			(pItem->sItemInfo.CODE == (sinDB1 | sin33)) ||
			(pItem->sItemInfo.CODE == (sinSP1 | sin37)) || (pItem->sItemInfo.CODE == (sinSP1 | sin38)) ||
			(pItem->sItemInfo.CODE == (sinBI1 | sin84)) ||
			(pItem->sItemInfo.CODE == (sinDB1 | sin34)) ||
			(pItem->sItemInfo.CODE == (sinOA2 | sin34)) ||
			(pItem->sItemInfo.CODE == (sinSP1 | sin42)) ||

			// Itens que n�o podem ser mixados
			(pItem->sItemInfo.itemType == 7) ||
			(pItem->sItemInfo.expireTime > 0)






			)
		{
			return false;
		}

		int kk = 0;
		for (kk = 0; kk < NotSet_Item_CODECnt; kk++)
		{
			if (NotSet_Item_CODE[kk] == pItem->sItemInfo.CODE)
			{
				return false;
			}
		}
		for (kk = 0; kk < NotSet_Item_MASKCnt; kk++)
		{
			if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == NotSet_Item_MASK[kk])
			{
				return false;
			}
		}
		for (kk = 0; kk < NotSet_Item_KINDCnt; kk++)
		{
			if (NotSet_Item_KIND[kk] == pItem->sItemInfo.ItemKindCode)
			{
				return false;
			}
		}
	}

	return true;
}


bool cCRAFTITEM::isItemAreaEmpty()
{
	return sCraftItem.CraftItem[0].Flag == 0;
}

bool cCRAFTITEM::isStoneAreaEmpty()
{
	return sCraftItem.CraftItem[14].Flag == 0;

}

/*----------------------------------------------------------------------------*
*Desc: �κ��丮�� 22*22ũ���� ��ĭ�� ���� �����Ѵ�.
*-----------------------------------------------------------------------------*/
int cCRAFTITEM::GetCraftCheckEmptyArea()
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;
	int StartX = 23;  //�κ� �ڽ��� ������ X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - 22; //�������� �����ִ� ���� �� X��ǥ 
	cy = (22 * INVENTORY_BOX_COL) - 22; //�������� �����ִ� ���� �� Y��ǥ

	int count = 0;
	//�ڵ尡 ���� �ʰų� �������� ������� ���� ���� �����Ҷ� �˻��ؼ� ��ġ�� ã�Ƴ��� 
	for (cntx = StartX; cntx <= StartX + cx; cntx += 22) {
		for (cnty = StartY; cnty <= StartY + cy; cnty += 22) {
			flag = 0;
			for (int i = 0; i < INVENTORY_MAXITEM; i++) {
				if (cInvenTory.InvenItem[i].Flag) {
					rect.left = cInvenTory.InvenItem[i].x;
					rect.right = cInvenTory.InvenItem[i].x + cInvenTory.InvenItem[i].w;
					rect.top = cInvenTory.InvenItem[i].y;
					rect.bottom = cInvenTory.InvenItem[i].y + cInvenTory.InvenItem[i].h;

					if (cntx > rect.left - 22 && cntx< rect.right &&
						cnty > rect.top - 22 && cnty < rect.bottom)
						flag++;
				}
			}
			if (flag == 0) {
				count++;
			}
		}
	}
	if (count)
		return count;
	else
		return 0;
}

//extern int iStateWindow;

// ������ - �ͽ��� ���� �������� �� ������ üũ�ϴ� �Լ� ����
int cMIXTURERESET::SetMixtureItemResetAreaCheck(sITEM * pItem)
{
	TradeColorIndex = 0;			// �ʱ�ȭ
	TradeCrashItemIndex[0] = 0;		// �浹�� ������ Index �ʱ�ȭ 
	cTrade.InitTradeColorRect();	// ���� �ʱ�ȭ

	if ((pItem->CODE & sinITEM_MASK2) == sinBI1)		// ���� �������̰�...
	{
		// �ͽ��� ���� ������ �ƴϸ� ����
		if ((pItem->sItemInfo.CODE & sinITEM_MASK3) != sin89)
			return FALSE;

		// ������ ��ġ�� ��ǥ�� ����� ����.
		TradeStartX = AgingLevelSheltomXY[0][0].x + 23;
		TradeStartY = AgingLevelSheltomXY[0][0].y + 31;
		TradeEndX = TradeStartX + 22;
		TradeEndY = TradeStartY + 22;

		int i, j;
		for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22)
		{
			for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22)
			{
				if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j)
				{
					//�κ��丮 �ڽ��� �������� üũ 
					TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
					TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
					TradeColorRect.right = pItem->w;
					TradeColorRect.bottom = pItem->h;

					if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11)
					{
						cTrade.InitTradeColorRect(); //�÷� �ڽ� RECT �ʱ�ȭ (�÷� �ڽ� RECT�� �ϳ���� 0 �̸� �����Ҽ� ���� 
						return FALSE;
					}

					// �������� ��ġ�� �����Ѵ�.
					pItem->SetX = TradeColorRect.left;
					pItem->SetY = TradeColorRect.top;
					pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ�

					TradeColorIndex = SET_ITEM_CHECK_COLOR;		//�����ҿ���

					// ��ġ�� �������� �ֳ� üũ 
					if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 6))
					{
						TradeColorIndex = NOT_SETTING_COLOR;
						return FALSE;

					}

					return TRUE;
				}
			}
		}
	}
	else
	{
		if ((pItem->sItemInfo.ItemKindCode != ITEM_KIND_CRAFT) && (pItem->sItemInfo.ItemKindCode != ITEM_KIND_AGING))
		{
			TradeColorIndex = 0;				//�ʱ�ȭ   (Ʈ���̵� â�� â�� ���� ����)
			TradeCrashItemIndex[0] = 0;		//�浹�� ������ Index �ʱ�ȭ 
			cTrade.InitTradeColorRect();	//�ʱ�ȭ

			// �ͽ��İ� ����� �������� �� �ڸ��� ��ǥ�� ����
			TradeStartX = 58;
			TradeStartY = 193 + sinInterHeight2;
			TradeEndX = TradeStartX + (3 * 22);
			TradeEndY = TradeStartY + (4 * 22);

			//�κ��丮 �ڽ��� �������� üũ 
			if ((TradeStartX <= pCursorPos.x && TradeEndX > pCursorPos.x && TradeStartY <= pCursorPos.y && TradeEndY > pCursorPos.y))
			{
				TradeColorRect.left = TradeStartX;
				TradeColorRect.top = TradeStartY;
				TradeColorRect.right = TradeEndX - TradeStartX;
				TradeColorRect.bottom = TradeEndY - TradeStartY;


				pItem->SetX = TradeStartX + ((TradeEndX - TradeStartX) - pItem->w) / 2;
				pItem->SetY = TradeStartY + ((TradeEndY - TradeStartY) - pItem->h) / 2;
				pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
				TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���

				//�������� ��ĥ��� 
				if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 6))
				{
					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;

				}

				return TRUE;
			}
		}
		else
		{
			TradeStartX = 58;
			TradeStartY = 193 + sinInterHeight2;
			TradeEndX = TradeStartX + (3 * 22);
			TradeEndY = TradeStartY + (4 * 22);
			if ((TradeStartX <= pCursorPos.x && TradeEndX > pCursorPos.x && TradeStartY <= pCursorPos.y && TradeEndY > pCursorPos.y))
			{ //�κ��丮 �ڽ��� �������� üũ 
				TradeColorRect.left = TradeStartX;
				TradeColorRect.top = TradeStartY;
				TradeColorRect.right = TradeEndX - TradeStartX;
				TradeColorRect.bottom = TradeEndY - TradeStartY;


				pItem->SetX = TradeStartX + ((TradeEndX - TradeStartX) - pItem->w) / 2;
				pItem->SetY = TradeStartY + ((TradeEndY - TradeStartY) - pItem->h) / 2;
				pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
				TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���

				if ((pItem->sItemInfo.ItemKindCode != ITEM_KIND_CRAFT) && (pItem->sItemInfo.ItemKindCode != ITEM_KIND_AGING))
				{
					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;
				}

			}
			//	TradeColorIndex = NOT_SETTING_COLOR;
			//	return FALSE;
		}
	}

	return TRUE;
}

// ������ - �ͽ��� ���� �������� ���� üũ
int cMIXTURERESET::CheckMixtureResetItemForm()
{
	int TempCheckDataSum = 0;

	for (int i = 0; i < 2; i++)
	{
		if (sMixtureResetItem.MixtureResetItem[i].Flag)
		{
			TempCheckDataSum += (i + 1) * sMixtureResetItem.MixtureResetItem[i].x;
			TempCheckDataSum += (i + 1) * sMixtureResetItem.MixtureResetItem[i].y;
			TempCheckDataSum += sMixtureResetItem.MixtureResetItem[i].sItemInfo.ItemHeader.dwChkSum;
			TempCheckDataSum += sMixtureResetItem.MixtureResetItem[i].sItemInfo.ItemHeader.Head;

		}
	}
	return TRUE;
}

// ������ - �ͽ��� ���� �������� ���� �ٽ� üũ
int cMIXTURERESET::ReformMixtureResetItem()
{
	MixtureResetItemCheckSum = 0;

	for (int i = 0; i < 2; i++)
	{
		if (sMixtureResetItem.MixtureResetItem[i].Flag)
		{
			MixtureResetItemCheckSum += (i + 1) * sMixtureResetItem.MixtureResetItem[i].x;
			MixtureResetItemCheckSum += (i + 1) * sMixtureResetItem.MixtureResetItem[i].y;
			MixtureResetItemCheckSum += sMixtureResetItem.MixtureResetItem[i].sItemInfo.ItemHeader.dwChkSum;
			MixtureResetItemCheckSum += sMixtureResetItem.MixtureResetItem[i].sItemInfo.ItemHeader.Head;

		}
	}

	return TRUE;
}

// ������ - �ͽ��� ���� �������� ���� �Լ�
int cMIXTURERESET::PickUpMixtureResetItem(int x, int y, int PickUpFlag, int Kind)
{
	int i = 0;
	SelectTradeItemIndex = 0;    //���� ������ 

	if (Kind == 0)
	{
		for (i = 0; i < 2; i++)
		{
			if (sMixtureResetItem.MixtureResetItem[i].Flag)
			{
				if (sMixtureResetItem.MixtureResetItem[i].x < x && sMixtureResetItem.MixtureResetItem[i].x + sMixtureResetItem.MixtureResetItem[i].w > x &&
					sMixtureResetItem.MixtureResetItem[i].y < y && sMixtureResetItem.MixtureResetItem[i].y + sMixtureResetItem.MixtureResetItem[i].h > y)
				{
					if (PickUpFlag)
					{
						if (i == 1)
							MixtureResetCheckFlag = 0;	// ���� ��ư ��Ȱ��ȭ

						// ������ �ִ� ���¿��� �ͽ��� �� ������ ��� �Ұ�
						if (sMixtureResetItem.MixtureResetItem[1].Flag && i == 0)
						{
							cMessageBox.ShowMessage(MESSAGE_NOT_MIXTURE_RESET_ITEM);
							return FALSE;
						}

						CheckMixtureResetItemForm();
						memcpy(&MouseItem, &sMixtureResetItem.MixtureResetItem[i], sizeof(sITEM));
						sMixtureResetItem.MixtureResetItem[i].Flag = 0;
						memset(&sMixtureResetItem.MixtureResetItem[i], 0, sizeof(sITEM));  //���� ��ŷ ���� ������ �ӽ� �����̴�.

						sinPlaySound(sMixtureResetItem.MixtureResetItem[i].SoundIndex);
						TradeColorIndex = 0;
						ReformMixtureResetItem();

						return TRUE;
					}
					else
					{
						SelectTradeItemIndex = i + 1;
						TradeColorIndex = SELECT_ITEM_COLOR;
						sinShowItemInfoFlag = 1;

						GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&sMixtureResetItem.MixtureResetItem[i], FALSE, FALSE, FALSE);
					}
				}
			}
		}
	}

	return FALSE;
}

bool cMIXTURERESET::isStoneOkay(sITEM * pItem)
{
	if ((pItem->sItemInfo.CODE & sinITEM_MASK3) != sin89 && (pItem->sItemInfo.CODE & sinITEM_MASK3) != sin88)
		return false;
	return true;
}

bool cMIXTURERESET::isItemOkay(sITEM * pItem)
{
	if ((pItem->sItemInfo.ItemKindCode == ITEM_KIND_CRAFT || pItem->sItemInfo.ItemKindCode == ITEM_KIND_AGING) && pItem->sItemInfo.expireTime <= 0 && pItem->sItemInfo.itemType != 99)
		return true;
	return false;
}

bool cMIXTURERESET::isEmpty(int Index)
{
	if (sMixtureResetItem.MixtureResetItem[Index].Flag)
		return false;
	return true;
}

// ������ - �ͽ��� ���� �������� â�� ��ġ��Ű���� �ϴ� �Լ�
int cMIXTURERESET::LastSetMixtureResetItem(sITEM * pItem, int kind)
{
	//CheckMixtureResetItemForm();

	if (kind == 1)
	{
		// �ͽ��ĵ� �������� �÷��� �ְ�, ������ �������� �ʴٸ�...
		if (sMixtureResetItem.MixtureResetItem[0].Flag && !sMixtureResetItem.MixtureResetItem[1].Flag)
		{
			memcpy(&sMixtureResetItem.MixtureResetItem[1], pItem, sizeof(sITEM));
			sMixtureResetItem.MixtureResetItem[1].x = pItem->SetX;
			sMixtureResetItem.MixtureResetItem[1].y = pItem->SetY;
			pItem->Flag = 0; //�ʱ�ȭ 
			sinPlaySound(sMixtureResetItem.MixtureResetItem[1].SoundIndex);
			//ReformMixtureResetItem();
			return TRUE;
		}
	}
	else if (kind == 2)		// �ͽ��ĵ� �������̸�...
	{
		// �ͽ��ĵ� �������� �÷��� ���� �ʴٸ�...
		if (!sMixtureResetItem.MixtureResetItem[0].Flag)
		{
			memcpy(&sMixtureResetItem.MixtureResetItem[0], pItem, sizeof(sITEM));
			sMixtureResetItem.MixtureResetItem[0].x = pItem->SetX;
			sMixtureResetItem.MixtureResetItem[0].y = pItem->SetY;
			pItem->Flag = 0; //�ʱ�ȭ 
			sinPlaySound(sMixtureResetItem.MixtureResetItem[0].SoundIndex);
			//ReformMixtureResetItem();
			return TRUE;
		}
	}

	return TRUE;
}

// ������ - �ͽ��� ������ ����Ѵ�.
int cMIXTURERESET::CancelMixtureItemReset(int Flag)
{
	// ���� ���� �÷��� Ȱ��ȭ �Ǿ� �ִٸ�....
	if (MixItemNoCopyFlag)
		return FALSE;

	// �ͽ��� ���¿� �÷ȴ� �������� �ִٸ� �κ��� �� �������� �ڵ� �����ϰ�, �����.
	if (sMixtureResetItem.MixtureResetItem[0].Flag)
	{
		if (!cInvenTory.AutoSetInvenItem(&sMixtureResetItem.MixtureResetItem[0]))
			sinThrowItemToFeild(&sMixtureResetItem.MixtureResetItem[0]);

		sMixtureResetItem.MixtureResetItem[0].Flag = 0;
	}

	MixtureResetCheckFlag = 0;	// �ͽ��� ���� �Ұ���

	// �ͽ��� ���� â�� �ݴ´�.
	if (!Flag)
		cMixtureReset.OpenFlag = 0;

	ResetInvenItemCode();	// �κ� ������ �ڵ� �ʱ�ȭ

	return TRUE;
}

// ������ - �ͽ��� ���µ� �������� ������ �����Ѵ�.
int cMIXTURERESET::sinSendMixtureItemReset()
{
	if (MixItemNoCopyFlag)
		return FALSE;

	memcpy(&sMixtureResetItemBackUp, &sMixtureResetItem, sizeof(sMIXTURE_RESET_ITEM));	// ���縦 üũ�ϱ����� ������� �����
	MixItemNoCopyFlag = 1;	//�������� ����Ǵ°� �����ϱ����� �÷����ش�
	memset(&sMixtureReset_Send, 0, sizeof(sMIXTURE_RESET_ITEM_SERVER));	// ������ ���� ����ü �ʱ�ȭ

	// �ͽ��� ���� ��ų �������� ����
	if (sMixtureResetItem.MixtureResetItem[0].Flag)
		sMixtureReset_Send.DesMixtureResetItem = sMixtureResetItem.MixtureResetItem[0];

	// �ͽ��� ���� ������ ����
	if (sMixtureResetItem.MixtureResetItem[1].Flag)
	{
		sMixtureReset_Send.dwMixtureResetStoneItemCode = sMixtureResetItem.MixtureResetItem[1].CODE;
		sMixtureReset_Send.dwMixtureResetStoneCheckSum = sMixtureResetItem.MixtureResetItem[1].sItemInfo.ItemHeader.dwChkSum;
		sMixtureReset_Send.dwMixtureResetStoneHead = sMixtureResetItem.MixtureResetItem[1].sItemInfo.ItemHeader.Head;
	}

	// �ε����� ������ 
	CraftItemSendServerIndex++;
	sMixtureReset_Send.Index = CraftItemSendServerIndex;

	// ����� ����ü�� ������ ������ 
	SendMixtureResetItemToServer(&sMixtureReset_Send);


	return TRUE;
}

// ������ - �ͽ��� ���µ� �������� Recv�� ������ ����
int	cMIXTURERESET::sinRecvMixtureItemReset(sMIXTURE_RESET_ITEM_SERVER * pMixtureResetItem)
{
	memcpy(&sMixtureReset_Recv, pMixtureResetItem, sizeof(sMIXTURE_RESET_ITEM_SERVER));

	return TRUE;
}

// ������ - �ͽ��� ���� ������ ���� ��� ó��
int cMIXTURERESET::sinRecvMixtureItemResetResult(sMIXTURE_RESET_ITEM_SERVER * pMixtureResetItem_Server)
{
	CheckMixItemCopyFlag = 0; //������� �÷�  

	// �ͽ��ĸ��� â�� �������� �ʴٸ� ��� �ʱ�ȭ
	if (!cMixtureReset.OpenFlag)
	{
		MixItemNoCopyFlag = 0; //������� �÷� �ʱ�ȭ
		memset(&sMixtureResetItem, 0, sizeof(sMIXTURE_RESET_ITEM));			//����ü �ʱ�ȭ 
		memset(&sMixtureResetItemBackUp, 0, sizeof(sMIXTURE_RESET_ITEM));	//����ü �ʱ�ȭ 
		memset(&sMixtureReset_Recv, 0, sizeof(sMIXTURE_RESET_ITEM_SERVER)); //���� �ͽ��ĸ��� ����ü �ʱ�ȭ 
		memset(&sMixtureReset_Send, 0, sizeof(sMIXTURE_RESET_ITEM_SERVER)); //���� �ͽ��ĸ��� ����ü �ʱ�ȭ 
		MixCancelButtonDelayFlag = 0;

		return FALSE;
	}

	int i, LineCount = 0, Temp;

	if (pMixtureResetItem_Server->Result)
	{
		sITEM TempItem;
		if (LoadItemImage(&pMixtureResetItem_Server->DesMixtureResetItem.sItemInfo, &TempItem))
		{
			lstrcpy(szCraftItemBuff, pMixtureResetItem_Server->DesMixtureResetItem.sItemInfo.ItemName);
			strcat(szCraftItemBuff, "\r");

			Temp = lstrlen(szCraftItemBuff);
			for (i = 0; i < Temp; i++)
			{
				if (szCraftItemBuff[i] == '\r')
				{
					LineCount++;
				}
			}

			CraftItemMessageSize.x = 14;				//�ڽ� ������ 
			CraftItemMessageSize.y = ((LineCount) * 2) + 2; //�ڽ� ������ 

			if (cInvenTory.CheckRequireItemToSet(&TempItem))
			{
				//��ٷ� ���õɼ��ִ����� üũ�Ѵ�  
				if (!cInvenTory.AutoSetInvenItem(&TempItem, 1))
				{
					sinThrowItemToFeild(&pMixtureResetItem_Server->DesMixtureResetItem);
				}
			}

		}

		cMessageBox.ShowMessage(MESSAGE_OK_MIXTURE_RESET_ITEM);

		CheckCharForm();	//���� 
		ReformCharForm();	//������ 
		SendSaveMoney();	//�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
		SaveGameData();
		cMixtureReset.OpenFlag = 0; //â�� �ݴ´�
	}
	else
	{
		CheckCharForm();	//���� 
		ReformCharForm();	//������ 
		SendSaveMoney();	//�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
		cMixtureReset.OpenFlag = 0; //â�� �ݴ´�
		cMessageBox.ShowMessage(MESSAGE_FAIL_MIXTURE_RESET_ITEM);
	}

	//���簡�Ǿ��ų��� �������� �����
	for (i = 0; i < 2; i++)
	{
		if (sMixtureResetItemBackUp.MixtureResetItem[i].Flag)
		{
			cInvenTory.DeleteInvenItemToServer(sMixtureResetItemBackUp.MixtureResetItem[i].sItemInfo.CODE,
				sMixtureResetItemBackUp.MixtureResetItem[i].sItemInfo.ItemHeader.Head,
				sMixtureResetItemBackUp.MixtureResetItem[i].sItemInfo.ItemHeader.dwChkSum);
		}

	}

	memset(&sMixtureResetItem, 0, sizeof(sMIXTURE_RESET_ITEM));			//����ü �ʱ�ȭ 
	memset(&sMixtureResetItemBackUp, 0, sizeof(sMIXTURE_RESET_ITEM));		//����ü �ʱ�ȭ 
	memset(&sMixtureReset_Recv, 0, sizeof(sMIXTURE_RESET_ITEM_SERVER));	//���� �ͽ��ĸ��� ����ü �ʱ�ȭ 
	memset(&sMixtureReset_Send, 0, sizeof(sMIXTURE_RESET_ITEM_SERVER));	//���� �ͽ��ĸ��� ����ü �ʱ�ȭ 

	MixItemNoCopyFlag = 0; //������� �÷� �ʱ�ȭ
	MixCancelButtonDelayFlag = 0;


	return TRUE;
}

// pluto ����
int CSmeltingItem::CancelSmeltingItem()
{

	for (int i = 0; i < 5; i++)
	{
		if (S_smeltingItem.SmeltingItem[i].Flag)
		{
			if (!cInvenTory.AutoSetInvenItem(&S_smeltingItem.SmeltingItem[i]))
			{
				sinThrowItemToFeild(&S_smeltingItem.SmeltingItem[i]);
				S_smeltingItem.SmeltingItem[i].Flag = 0;
			}
		}
	}

	SmeltingItem.OpenFlag = 0;
	m_SmeltingPrice = 0;
	ForceItemPrice2 = 0;
	memset(&S_smeltingItem, 0, sizeof(SSmeltingItem)); //�������� �� ���������� 
	ResetInvenItemCode();				//�κ� ������ �ڵ� �ʱ�ȭ

	memset(&sSmeltingItem_Recv, 0, sizeof(sSMELTINGITEM_SERVER)); //���� �ͽ��� ����ü �ʱ�ȭ  (�ϴ� �������ش�)
	memset(&sSmeltingItem_Send, 0, sizeof(sSMELTINGITEM_SERVER)); //���� �ͽ��� ����ü �ʱ�ȭ 

	return TRUE;
}

bool CSmeltingItem::isAreaEmpty(sITEM * pITEM, int Index)
{
	if (Index != 4) {
		bool Flag = false;
		for (int i = 0; i < 3; i++) {
			if (S_smeltingItem.SmeltingItem[i].Flag) {
				Flag = true;
				int ItemCode = S_smeltingItem.SmeltingItem[i].CODE;
				if (!S_smeltingItem.SmeltingItem[Index].Flag && ItemCode == pITEM->CODE)
					return true;
			}
		}
		if (!Flag)
			return true;

		return false;
	}
	else {
		int MainCode = S_smeltingItem.SmeltingItem[0].CODE;

		if ((MainCode & sinITEM_MASK2) == sinPR1 ||
			(MainCode & sinITEM_MASK2) == sinPR2) {
			int Sheltom = ((MainCode & sinITEM_MASK3) >> 8);
			int Item = ((pITEM->sItemInfo.CODE & sinITEM_MASK3) >> 8);
			if (Item == Sheltom)
				return true;
		}
	}
	return false;
}

bool CSmeltingItem::GetEmptyArea(POINT * Area)
{
	POINT SmeltingArea[] = { { 80,159 },{ 55,190 },{ 105,190 } };
	bool Flags[3] = { false, false, false };
	for (int i = 0; i < 3; i++) {
		if (S_smeltingItem.SmeltingItem[i].Flag) {
			for (int c = 0; c < 3; c++) {
				if (S_smeltingItem.SmeltingItem[i].SetX == SmeltingArea[c].x) {
					Flags[c] = true;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		if (!Flags[i]) {
			Area->x = SmeltingArea[i].x + 2;
			Area->y = SmeltingArea[i].y + 2;
			return true;
		}
	}
	return false;
}

// pluto ����
int CSmeltingItem::SetSmeltingItemAreaCheck(sITEM * pItem, int Kind)
{
	//����	  ���̾ƽ� �������� ��Ÿ��  �ٷ糪 ����׾� ��Ƽ��� �޷�ī��
	int SmeltingPriceIndexOre = 25000;

	//����    ���̾ƽ� �������� ��Ÿ��  �ٷ糪 ����׾� ��Ƽ��� �޷�ī�� ������ ������ö�� �Ͼᴫ��
	int SmeltingPriceIndexCrystal = 50000;
	//	int SmeltingPriceIndexCrystal = 10000; // �庰 - �Ϻ� ��û���� �ӽ÷� ����
		// �庰 - ���������� ȥ
	DWORD SmeltingCode[] = { sin01, sin02, sin03, sin04, sin05, sin06, sin07, sin08, sin09, sin10, sin11, sin12, sin13, sin14 };

	int i, j;
	TradeColorIndex = 0; // �ʱ�ȭ
	TradeCrashItemIndex[0] = 0; //�浹�� ������ Index �ʱ�ȭ 
	cTrade.InitTradeColorRect(); //�ʱ�ȭ

	if ((pItem->CODE & sinITEM_MASK2) == sinOS1)
	{
		//TradeStartX = 218;
		//TradeStartY = 193+sinInterHeight2;
		//TradeEndX = TradeStartX+(3*22);
		//TradeEndY = TradeStartY+(4*22);

		//for(i=pItem->x+11 ; i<pItem->x+pItem->w ; i+=22)
		//{
		//	for( j=pItem->y+11 ; j<pItem->y+pItem->h ; j+=22)
		//	{
		//		if(TradeStartX <= i && TradeEndX > i && TradeStartY <=j && TradeEndY > j)
		//		{ //�κ��丮 �ڽ��� �������� üũ 
		//			TradeColorRect.left =  TradeStartX +( ( ( i - TradeStartX) /22)*22) ; 
		//			TradeColorRect.top  =  TradeStartY +( ( ( j - TradeStartY) /22)*22) ; 
		//			TradeColorRect.right  = pItem->w;
		//			TradeColorRect.bottom = pItem->h;
		//			if(TradeEndX < (TradeColorRect.left + TradeColorRect.right)-11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom)-11)
		//			{
		//				cTrade.InitTradeColorRect(); //�÷� �ڽ� RECT �ʱ�ȭ (�÷� �ڽ� RECT�� �ϳ���� 0 �̸� �����Ҽ� ���� 
		//				return FALSE;
		//			}

		//			pItem->SetX = TradeColorRect.left;
		//			pItem->SetY = TradeColorRect.top;
		//			pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
		//			TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���
		//			if( !S_smeltingItem.SmeltingItem[0].Flag )
		//			{
		//				TradeColorIndex = NOT_SETTING_COLOR;
		//				return FALSE;
		//			}
		//			if(Kind == 0)
		//			{ //�ͽ��� 
		//				if(TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect,0,4))
		//				{  //��ġ�� �������� �ֳ� üũ 
		//					TradeColorIndex = NOT_SETTING_COLOR;
		//					return FALSE;

		//				}
		//				return TRUE;
		//			}
		//		}

		//	}

		//}


		//int SmeltingSheltomAray[4][3] = { 0, 0, 0,		// ���� ���� �þ� ���� ����
		//								  0, 0, 0,
		//								  0, 0, 0,
		//								  0, 0, 0 };

		int SmeltingSheltomIndex = 0;
		SmeltingSheltomIndex = GetSheltomIndex(S_smeltingItem.SmeltingItem[0].CODE);
		int Smelting_SheltomIndex = 0;
		DWORD SheltomAray[MAX_SHELTOM] = { sin01, sin02, sin03, sin04, sin05, sin06, sin07, sin08, sin09, sin10, sin11, sin12, sin13,sin14 };

		for (int q = 0; q < MAX_SHELTOM; q++)
		{
			if (SheltomAray[q] == (pItem->sItemInfo.CODE & sinITEM_MASK3))
			{
				Smelting_SheltomIndex = q + 1;
				break;
			}
		}
		//for(int p = 0 ; p < 4 ; p++)			// ���� ���� �þ� ���� ����
		{
			//for(int m=0; m < 3 ; m++)
			{
				if (SmeltingSheltomIndex == Smelting_SheltomIndex)
				{
					TradeStartX = AgingLevelSheltomXY[0][0].x + 22;			// ��ǥ�� ���Ƽ� �� ����
					TradeStartY = AgingLevelSheltomXY[0][0].y + 30;
					TradeEndX = TradeStartX + 22;
					TradeEndY = TradeStartY + 22;
					for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22)
					{
						for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22)
						{
							if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j)
							{ //�κ��丮 �ڽ��� �������� üũ 
								TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
								TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
								TradeColorRect.right = pItem->w;
								TradeColorRect.bottom = pItem->h;
								if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11)
								{
									cTrade.InitTradeColorRect(); //�÷� �ڽ� RECT �ʱ�ȭ (�÷� �ڽ� RECT�� �ϳ���� 0 �̸� �����Ҽ� ���� 
									return FALSE;
								}

								pItem->SetX = TradeColorRect.left;
								pItem->SetY = TradeColorRect.top;
								pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ�

								TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���
								if (!S_smeltingItem.SmeltingItem[0].Flag)
								{
									TradeColorIndex = NOT_SETTING_COLOR;
									return FALSE;
								}

								if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 4))
								{  //��ġ�� �������� �ֳ� üũ 
									TradeColorIndex = NOT_SETTING_COLOR;
									return FALSE;

								}
								return TRUE;
							}
						}
					}
				}
			}
		}
	}
	else if ((pItem->CODE & sinITEM_MASK2) == sinPR1 || (pItem->CODE & sinITEM_MASK2) == sinPR2)
	{
		//char *p = pItem->LastCategory;
		//if( strcmp( p, "SE101") )
		//{
		//	return false;
		//}
		TradeColorIndex = 0; //�ʱ�ȭ   (Ʈ���̵� â�� â�� ���� ����)
		TradeCrashItemIndex[0] = 0; //�浹�� ������ Index �ʱ�ȭ 
		cTrade.InitTradeColorRect(); //�ʱ�ȭ

		//TradeStartX = 58;
		//TradeStartY = 193+sinInterHeight2;
		//TradeEndX = TradeStartX+(3*22);
		//TradeEndY = TradeStartY+(3*22);

		TradeStartX = 69;
		TradeStartY = 212 + sinInterHeight2;
		TradeEndX = TradeStartX + (2 * 18);
		TradeEndY = TradeStartY + (2 * 18);
		for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22)
		{
			for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22)
			{
				if (TradeStartX <= pCursorPos.x && TradeEndX > pCursorPos.x && TradeStartY <= pCursorPos.y && TradeEndY > pCursorPos.y)
				{ //�κ��丮 �ڽ��� �������� üũ // �ϴ���
					if (!(80 <= pCursorPos.x && 101 >= pCursorPos.x && 143 <= pCursorPos.y && 164 >= pCursorPos.y))
					{
						TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
						TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
						TradeColorRect.right = pItem->w;
						TradeColorRect.bottom = pItem->h;
						if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11)
						{
							cTrade.InitTradeColorRect(); //�÷� �ڽ� RECT �ʱ�ȭ (�÷� �ڽ� RECT�� �ϳ���� 0 �̸� �����Ҽ� ���� 
							return FALSE;
						}

						pItem->SetX = TradeColorRect.left;
						pItem->SetY = TradeColorRect.top;
						pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
						TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���

						if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 4))
						{  //��ġ�� �������� �ֳ� üũ 
							TradeColorIndex = NOT_SETTING_COLOR;
							return FALSE;
						}

						// ���� �Ҽ� �ִ� ���� �ֳ� �˻�
						for (int i = 0; i < 7; i++)
						{
							// ����
							if ((pItem->CODE & sinITEM_MASK2) == sinPR1)
							{
								if ((pItem->sItemInfo.CODE & sinITEM_MASK3) == SmeltingCode[i])
								{
									if (sinChar->Money < SmeltingPriceIndexOre)
									{
										TradeColorIndex = NOT_ENOUGH_MONEY;
										return FALSE;
									}
								}
							}
						}

						for (int i = 0; i < 14; i++) // �庰 - ���� // �庰 - ���������� ȥ
						{
							// ����
							if ((pItem->CODE & sinITEM_MASK2) == sinPR2)
							{
								if ((pItem->sItemInfo.CODE & sinITEM_MASK3) == SmeltingCode[i])
								{
									if (sinChar->Money < SmeltingPriceIndexCrystal)
									{
										TradeColorIndex = NOT_ENOUGH_MONEY;
										return FALSE;
									}
								}
							}
						}

						////����� �������� �ͽ��Ŀ� ����¡�� �Ҽ����� MakeItem�� �ͽ��Ҽ����� 
						if (pItem->sItemInfo.SpecialItemFlag[0] == CHECK_COPY_ITEM ||
							pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
							(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
							(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
							(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinQW1 ||
							(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinPZ1 || //���� �ø������� 
							(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinPZ2 ||
							(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinFO1 || //�������ȵȴ�
							pItem->sItemInfo.CODE == (sinGF1 | sin01) ||
							pItem->sItemInfo.CODE == (sinGF1 | sin02) ||
							pItem->sItemInfo.CODE == (sinGF1 | sin03) ||
							pItem->sItemInfo.CODE == (sinGF1 | sin04) ||
							pItem->sItemInfo.CODE == (sinGF1 | sin05) ||
							pItem->sItemInfo.CODE == (sinGF1 | sin06) ||
							pItem->sItemInfo.CODE == (sinGF1 | sin07) ||	// �庰 - ������� ã�ƶ�
							pItem->sItemInfo.CODE == (sinGF1 | sin08))
						{

							TradeColorIndex = NOT_SETTING_COLOR;
							return FALSE;
						}
						return TRUE;
					}

				}
			}
		}
	}
	return TRUE;
}

// pluto ����
int CSmeltingItem::LastSetSmeltingItem(sITEM * pItem, int Kind)
{
	if (Kind == 0)
	{
		CheckSmeltingItemForm();
		if ((pItem->CODE & sinITEM_MASK2) == sinOS1)
		{ //������ ���
			//for(int j=12 ; j < 24 ; j++)
			//{
			//	if(!S_smeltingItem.SmeltingItem[j].Flag)
			//	{
			//		memcpy(&S_smeltingItem.SmeltingItem[j],pItem,sizeof(sITEM));
			//		S_smeltingItem.SmeltingItem[j].x = pItem->SetX;
			//		S_smeltingItem.SmeltingItem[j].y = pItem->SetY;
			//		pItem->Flag = 0; //�ʱ�ȭ 
			//		sinPlaySound(S_smeltingItem.SmeltingItem[j].SoundIndex);
			//		ReFormSmeltingItem();
			//		return TRUE;
			//	}
			//}
			if (!S_smeltingItem.SmeltingItem[4].Flag)	// pluto ���� �Ѱ��� ���´� 
			{
				memcpy(&S_smeltingItem.SmeltingItem[4], pItem, sizeof(sITEM));
				S_smeltingItem.SmeltingItem[4].x = pItem->SetX;
				S_smeltingItem.SmeltingItem[4].y = pItem->SetY;
				pItem->Flag = 0; //�ʱ�ȭ 
				sinPlaySound(S_smeltingItem.SmeltingItem[4].SoundIndex);
				ReFormSmeltingItem();
				return TRUE;
			}

		}
		else
		{
			int k = 3; // �庰 - ���� �ý��� ����
			//if( (pItem->CODE & sinITEM_MASK2) == sinPR1 )
			//{
			//	k = 5;		// ����
			//}
			//else if( (pItem->CODE & sinITEM_MASK2) == sinPR2 )
			//{
			//	k = 3;		// ����
			//}

			for (int i = 0; i < k; i++)
			{
				if (!S_smeltingItem.SmeltingItem[i].Flag)		// ����, ����
				{
					memcpy(&S_smeltingItem.SmeltingItem[i], pItem, sizeof(sITEM));
					S_smeltingItem.SmeltingItem[i].x = pItem->SetX;
					S_smeltingItem.SmeltingItem[i].y = pItem->SetY;
					pItem->Flag = 0; //�ʱ�ȭ 
					sinPlaySound(sCraftItem.CraftItem[i].SoundIndex);
					ReFormSmeltingItem();
					CheckSmeltingPrice();
					return TRUE;
				}
			}
		}
	}
	return TRUE;
}

// pluto ����
int CSmeltingItem::CheckSmeltingItemForm()
{ //üũ  
	int TempCheckDataSum = 0;



	for (int i = 0; i < 5; i++)
	{
		if (S_smeltingItem.SmeltingItem[i].Flag)
		{
			TempCheckDataSum += (i + 1) * S_smeltingItem.SmeltingItem[i].x;
			TempCheckDataSum += (i + 1) * S_smeltingItem.SmeltingItem[i].y;
			TempCheckDataSum += S_smeltingItem.SmeltingItem[i].sItemInfo.ItemHeader.dwChkSum;
			TempCheckDataSum += S_smeltingItem.SmeltingItem[i].sItemInfo.ItemHeader.Head;

		}
	}
	return TRUE;
}

// pluto ����
int CSmeltingItem::ReFormSmeltingItem()
{ //���� 

	SmeltingItemCheckSum = 0;
	for (int i = 0; i < 5; i++)
	{
		if (S_smeltingItem.SmeltingItem[i].Flag)
		{
			SmeltingItemCheckSum += (i + 1) * S_smeltingItem.SmeltingItem[i].x;
			SmeltingItemCheckSum += (i + 1) * S_smeltingItem.SmeltingItem[i].y;
			SmeltingItemCheckSum += S_smeltingItem.SmeltingItem[i].sItemInfo.ItemHeader.dwChkSum;
			SmeltingItemCheckSum += S_smeltingItem.SmeltingItem[i].sItemInfo.ItemHeader.Head;

		}
	}

	return TRUE;
}

// pluto ���� ������ ����
int CSmeltingItem::PickUpSmeltingItem(int x, int y, int PickUpFlag, int Kind)
{
	int i = 0;
	SelectTradeItemIndex = 0;    //���� ������ 
	if (Kind == 0)
	{
		for (i = 0; i < 5; i++)
		{
			if (S_smeltingItem.SmeltingItem[i].Flag)
			{
				if (S_smeltingItem.SmeltingItem[i].x < x && S_smeltingItem.SmeltingItem[i].x + S_smeltingItem.SmeltingItem[i].w > x &&
					S_smeltingItem.SmeltingItem[i].y < y && S_smeltingItem.SmeltingItem[i].y + S_smeltingItem.SmeltingItem[i].h > y)
				{
					if (PickUpFlag)
					{
						if (i < 4 && S_smeltingItem.SmeltingItem[4].Flag)
						{
							cMessageBox.ShowMessage(MESSAGE_NOT_SMELTING);
							return FALSE;
						}
						CheckSmeltingItemForm();
						memcpy(&MouseItem, &S_smeltingItem.SmeltingItem[i], sizeof(sITEM));
						S_smeltingItem.SmeltingItem[i].Flag = 0;
						memset(&S_smeltingItem.SmeltingItem[i], 0, sizeof(sITEM));  //���� ��ŷ ���� ������ �ӽ� �����̴�.

						sinPlaySound(S_smeltingItem.SmeltingItem[i].SoundIndex);
						TradeColorIndex = 0;
						ReFormSmeltingItem();
						return TRUE;
					}
					else
					{
						SelectTradeItemIndex = i + 1;
						TradeColorIndex = SELECT_ITEM_COLOR;
						sinShowItemInfoFlag = 1;

						GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&S_smeltingItem.SmeltingItem[i], FALSE, FALSE, FALSE);
					}
				}
			}
		}
	}
	return FALSE;
}

// pluto ����
int CSmeltingItem::sinSendSmeltingItem()
{
	if (MixItemNoCopyFlag)
	{
		return FALSE;
	}

	int i = 0;

	//���縦 üũ�ϱ����� ������� �����
	memcpy(&sSmeltingItemBackUp, &S_smeltingItem, sizeof(SSmeltingItem));
	MixItemNoCopyFlag = 1; //�������� ����Ǵ°� �����ϱ����� �÷����ش�
	memset(&sSmeltingItem_Send.DesSmeltingItem, 0, sizeof(sITEM));

	sSmeltingItem_Send.Money = m_SmeltingPrice;

	for (i = 0; i < 5; i++)
	{
		if (S_smeltingItem.SmeltingItem[i].Flag)
		{
			sSmeltingItem_Send.SmeltingCode[i] = S_smeltingItem.SmeltingItem[i].CODE;
			sSmeltingItem_Send.CheckSum[i] = S_smeltingItem.SmeltingItem[i].sItemInfo.ItemHeader.dwChkSum;
			sSmeltingItem_Send.Head[i] = S_smeltingItem.SmeltingItem[i].sItemInfo.ItemHeader.Head;
		}
	}

	CheckCraftMoney = sSmeltingItem_Send.Money;

	CraftItemSendServerIndex++; //�ε����� ������ 
	sSmeltingItem_Send.Index = CraftItemSendServerIndex;
	//����� ����ü�� ������ ������ 


	SendSmeltingItemToServer(&sSmeltingItem_Send);

	return TRUE;
}

int CSmeltingItem::sinRecvSmeltingItem(sSMELTINGITEM_SERVER * pSmeltingItem_Server)
{
	memcpy(&sSmeltingItem_Recv, pSmeltingItem_Server, sizeof(sSMELTINGITEM_SERVER));

	return TRUE;
}

// pluto ���� �������� ������� �������� �޴´� 
int CSmeltingItem::sinRecvSmeltingItemResult(sSMELTINGITEM_SERVER * pSmeltingItem_Server)
{
	CheckMixItemCopyFlag = 0; //������� �÷�  
	if (!SmeltingItem.OpenFlag)
	{
		MixItemNoCopyFlag = 0; //������� �÷� �ʱ�ȭ
		memset(&S_smeltingItem, 0, sizeof(SSmeltingItem)); //����ü �ʱ�ȭ 
		memset(&sSmeltingItemBackUp, 0, sizeof(SSmeltingItem)); //����ü �ʱ�ȭ 
		memset(&sSmeltingItem_Recv, 0, sizeof(sSMELTINGITEM_SERVER)); //���� ���� ����ü �ʱ�ȭ 
		memset(&sSmeltingItem_Send, 0, sizeof(sSMELTINGITEM_SERVER)); //���� ���� ����ü �ʱ�ȭ 
		MixCancelButtonDelayFlag = 0;
		return FALSE;

	}

	int i, LineCount = 0, Temp;


	if (pSmeltingItem_Server->Result)
	{
		ShowItemSmeltingMessageFlag = 1; // �������� ���É����� �޼����� �����ش�

		sITEM TempItem;
		if (LoadItemImage(&pSmeltingItem_Server->DesSmeltingItem.sItemInfo, &TempItem))
		{
			lstrcpy(szCraftItemBuff, pSmeltingItem_Server->DesSmeltingItem.sItemInfo.ItemName);

			strcat(szCraftItemBuff, "\r");
			Temp = lstrlen(szCraftItemBuff);
			for (i = 0; i < Temp; i++)
			{
				if (szCraftItemBuff[i] == '\r')
				{
					LineCount++;
				}
			}
			CraftItemMessageSize.x = 14;           //�ڽ� ������ 
			CraftItemMessageSize.y = ((LineCount) * 2) + 2;  //�ڽ� ������ 

			if (cInvenTory.CheckRequireItemToSet(&TempItem))
			{
				//��ٷ� ���õɼ��ִ����� üũ�Ѵ�  
				if (!cInvenTory.AutoSetInvenItem(&TempItem, 1))
				{
					sinThrowItemToFeild(&pSmeltingItem_Server->DesSmeltingItem);
				}
			}

		}

		CheckCharForm(); //���� 
		sinMinusMoney(m_SmeltingPrice);
		ReformCharForm(); //������ 
		SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
		SaveGameData();
		SmeltingItem.OpenFlag = 0; //â�� �ݴ´�
	}
	else
	{
		CheckCharForm(); //���� 
		sinMinusMoney(m_SmeltingPrice);
		ReformCharForm(); //������ 
		SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
		SmeltingItem.OpenFlag = 0; //â�� �ݴ´� 
		cMessageBox.ShowMessage(MESSAGE_FAILD_SMELTING_ITEM);
	}

	//���簡�Ǿ��ų��� �������� �����
	for (i = 0; i < 5; i++)
	{
		if (sSmeltingItemBackUp.SmeltingItem[i].Flag)
		{
			cInvenTory.DeleteInvenItemToServer(sSmeltingItemBackUp.SmeltingItem[i].sItemInfo.CODE,
				sSmeltingItemBackUp.SmeltingItem[i].sItemInfo.ItemHeader.Head,
				sSmeltingItemBackUp.SmeltingItem[i].sItemInfo.ItemHeader.dwChkSum);
		}

	}
	memset(&S_smeltingItem, 0, sizeof(SSmeltingItem)); //����ü �ʱ�ȭ 
	memset(&sSmeltingItemBackUp, 0, sizeof(SSmeltingItem)); //����ü �ʱ�ȭ 
	memset(&sSmeltingItem_Recv, 0, sizeof(sSMELTINGITEM_SERVER)); //���� ���� ����ü �ʱ�ȭ 
	memset(&sSmeltingItem_Send, 0, sizeof(sSMELTINGITEM_SERVER)); //���� ���� ����ü �ʱ�ȭ 

	MixItemNoCopyFlag = 0; //������� �÷� �ʱ�ȭ
	MixCancelButtonDelayFlag = 0;
	m_SmeltingPrice = 0;
	ForceItemPrice2 = 0;
	return TRUE;
}

// pluto ����
int CSmeltingItem::SmeltingCheckEmptyArea(sITEM * pItem)
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;
	int StartX = 23;  //�κ� �ڽ��� ������ X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - pItem->w; //�������� �����ִ� ���� �� X��ǥ 
	cy = (22 * INVENTORY_BOX_COL) - pItem->h; //�������� �����ִ� ���� �� Y��ǥ

	//�ڵ尡 ���� �ʰų� �������� ������� ���� ���� �����Ҷ� �˻��ؼ� ��ġ�� ã�Ƴ��� 
	for (cntx = StartX; cntx <= StartX + cx; cntx += 22) {
		for (cnty = StartY; cnty <= StartY + cy; cnty += 22) {
			flag = 0;
			for (int i = 0; i < INVENTORY_MAXITEM; i++) {
				if (cInvenTory.InvenItem[i].Flag) {
					rect.left = cInvenTory.InvenItem[i].x;
					rect.right = cInvenTory.InvenItem[i].x + cInvenTory.InvenItem[i].w;
					rect.top = cInvenTory.InvenItem[i].y;
					rect.bottom = cInvenTory.InvenItem[i].y + cInvenTory.InvenItem[i].h;

					if (cntx > rect.left - pItem->w && cntx< rect.right &&
						cnty > rect.top - pItem->h && cnty < rect.bottom)
						flag++;
				}
			}
			if (flag == 1) { // �庰 - ���� �ý��� ����
				pItem->SetX = cntx;
				pItem->SetY = cnty;
				pItem->ItemPosition = 0;
				return TRUE;
			}
		}
	}

	return FALSE;
}

int CSmeltingItem::GetSmeltingCheckEmptyArea()
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;
	int StartX = 23;  //�κ� �ڽ��� ������ X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - 22; //�������� �����ִ� ���� �� X��ǥ 
	cy = (22 * INVENTORY_BOX_COL) - 22; //�������� �����ִ� ���� �� Y��ǥ

	int count = 0;
	//�ڵ尡 ���� �ʰų� �������� ������� ���� ���� �����Ҷ� �˻��ؼ� ��ġ�� ã�Ƴ��� 
	for (cntx = StartX; cntx <= StartX + cx; cntx += 22) {
		for (cnty = StartY; cnty <= StartY + cy; cnty += 22) {
			flag = 0;
			for (int i = 0; i < INVENTORY_MAXITEM; i++) {
				if (cInvenTory.InvenItem[i].Flag) {
					rect.left = cInvenTory.InvenItem[i].x;
					rect.right = cInvenTory.InvenItem[i].x + cInvenTory.InvenItem[i].w;
					rect.top = cInvenTory.InvenItem[i].y;
					rect.bottom = cInvenTory.InvenItem[i].y + cInvenTory.InvenItem[i].h;

					if (cntx > rect.left - 22 && cntx< rect.right &&
						cnty > rect.top - 22 && cnty < rect.bottom)
						flag++;
				}
			}
			if (flag == 1) { // �庰 - ���� �ý��� ����
				count++;
			}
		}
	}
	if (count)
		return count;
	else
		return 0;
}

// pluto ����, ���� ���� ���
int CSmeltingItem::CheckSmeltingPrice()
{						//����	  ���̾ƽ� �������� ��Ÿ��  �ٷ糪 ����׾� ��Ƽ��� �޷�ī��
	//int SmeltingPriceIndexOre[] = { 100000, 200000, 300000, 400000, 500000, 600000, 700000 };
	int SmeltingPriceIndexOre = 25000;

	//����    ���̾ƽ� �������� ��Ÿ��  �ٷ糪 ����׾� ��Ƽ��� �޷�ī��
//int SmeltingPriceIndexCrystal[] = { 200000, 400000, 600000, 800000, 1000000, 1200000, 1400000 };
	int SmeltingPriceIndexCrystal = 50000;

	//	int SmeltingPriceIndexCrystal = 10000; // �庰 - �Ϻ� ��û���� �ӽ÷� ����

		//DWORD SmeltingCode[] = { sin01, sin02, sin03, sin04, sin05, sin06, sin07, sin08 };
	m_SmeltingPrice = 0;
	if (S_smeltingItem.SmeltingItem[0].Flag)
	{
		if ((S_smeltingItem.SmeltingItem[0].sItemInfo.CODE & sinITEM_MASK2) == sinPR1) // ����
		{
			/*
			for( int i = 0; i < 7; i++ )
			{
				if( (S_smeltingItem.SmeltingItem[0].CODE & sinITEM_MASK3) == SmeltingCode[i] )
				{
					m_SmeltingPrice = SmeltingPriceIndexOre;
					ForceItemPrice2 = m_SmeltingPrice;
					return TRUE;
				}
			}
			*/
			m_SmeltingPrice = SmeltingPriceIndexOre;
			ForceItemPrice2 = m_SmeltingPrice;
			return TRUE;
		}
		else if ((S_smeltingItem.SmeltingItem[0].sItemInfo.CODE & sinITEM_MASK2) == sinPR2) // ����
		{
			/*
			for( int i = 0; i < 7; i++ )
			{
				if( (S_smeltingItem.SmeltingItem[0].CODE & sinITEM_MASK3) == SmeltingCode[i] )
				{
					m_SmeltingPrice = SmeltingPriceIndexCrystal[i];
					ForceItemPrice2 = m_SmeltingPrice;
					return TRUE;
				}
			}
			*/
			m_SmeltingPrice = SmeltingPriceIndexCrystal;
			ForceItemPrice2 = m_SmeltingPrice;
			return TRUE;
		}
	}
	return TRUE;
}

// pluto ���� ���
int CManufacture::CancelManufactureItem()
{

	for (int i = 0; i < 4; i++)
	{
		if (g_sManufactureItem.ManufactureItem[i].Flag)
		{
			if (!cInvenTory.AutoSetInvenItem(&g_sManufactureItem.ManufactureItem[i]))
			{
				sinThrowItemToFeild(&g_sManufactureItem.ManufactureItem[i]);
				g_sManufactureItem.ManufactureItem[i].Flag = 0;
			}
		}
	}

	ManufactureItem.m_OpenFlag = 0;
	m_ManufacturePrice = 0;
	ForceItemPrice2 = 0;
	memset(&g_sManufactureItem, 0, sizeof(SManufactureItem)); //�������� �� ���������� 
	ResetInvenItemCode();				//�κ� ������ �ڵ� �ʱ�ȭ

	memset(&g_sManufactureItem_Recv, 0, sizeof(SManufactureItem_Server)); //���� ���� ����ü �ʱ�ȭ
	memset(&g_sManufactureItem_Send, 0, sizeof(SManufactureItem_Server)); //���� ���� ����ü �ʱ�ȭ 

	return TRUE;
}

// pluto ���� ������ �� �ڸ� �˻�
int CManufacture::SetManufactureItemAreaCheck(sITEM * pItem)
{
	////����	  ���̾ƽ� �������� ��Ÿ��  �ٷ糪 ����׾� ��Ƽ��� �޷�ī��
	//int SmeltingPriceIndexOre[] = { 100000, 200000, 300000, 400000, 500000, 600000, 700000 };

	////����    ���̾ƽ� �������� ��Ÿ��  �ٷ糪 ����׾� ��Ƽ��� �޷�ī��
	//int SmeltingPriceIndexCrystal[] = { 200000, 400000, 600000, 800000, 1000000, 1200000, 1400000 };
	//DWORD SmeltingCode[] = { sin01, sin02, sin03, sin04, sin05, sin06, sin07 };

	POINT ManufactureRuneXY[3] = { {240, 159},  {215, 190}, {265, 190} };

	int i, j;
	TradeColorIndex = 0; // �ʱ�ȭ
	TradeCrashItemIndex[0] = 0; //�浹�� ������ Index �ʱ�ȭ 
	cTrade.InitTradeColorRect(); //�ʱ�ȭ

	if ((pItem->CODE & sinITEM_MASK2) == sinPR3 || (pItem->CODE & sinITEM_MASK2) == sinPR4)
	{

		//TradeStartX = 218;
		//TradeStartY = 193+sinInterHeight2;
		//TradeEndX = TradeStartX+(3*22);
		//TradeEndY = TradeStartY+(4*22);

		//for(i=pItem->x+11 ; i<pItem->x+pItem->w ; i+=22)
		//{
		//	for( j=pItem->y+11 ; j<pItem->y+pItem->h ; j+=22)
		//	{
		//		if(TradeStartX <= i && TradeEndX > i && TradeStartY <=j && TradeEndY > j)
		//		{ //�κ��丮 �ڽ��� �������� üũ 
		//			TradeColorRect.left =  TradeStartX +( ( ( i - TradeStartX) /22)*22) ; 
		//			TradeColorRect.top  =  TradeStartY +( ( ( j - TradeStartY) /22)*22) ; 
		//			TradeColorRect.right  = pItem->w;
		//			TradeColorRect.bottom = pItem->h;
		//			if(TradeEndX < (TradeColorRect.left + TradeColorRect.right)-11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom)-11)
		//			{
		//				cTrade.InitTradeColorRect(); //�÷� �ڽ� RECT �ʱ�ȭ (�÷� �ڽ� RECT�� �ϳ���� 0 �̸� �����Ҽ� ���� 
		//				return FALSE;
		//			}

		//			pItem->SetX = TradeColorRect.left;
		//			pItem->SetY = TradeColorRect.top;
		//			pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
		//			TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���

		//			if(TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect,0,5))
		//			{  //��ġ�� �������� �ֳ� üũ 
		//				TradeColorIndex = NOT_SETTING_COLOR;
		//				return FALSE;
		//			}
		//			return TRUE;
		//		}

		//	}

		//}

		// ���� ������ �鸸 ���´� // �庰 - �� �߰� 11���� // �庰 - ���������� ȥ 11->14
		int RuneItemIndex = 0;
		DWORD RuneAray[MAX_RUNE] = { sin01, sin02, sin03, sin04 ,sin05, sin06, sin07, sin08, sin09, sin10, sin11, sin12, sin13, sin14 };
		for (int q = 0; q < MAX_RUNE; q++)
		{
			if (RuneAray[q] == (pItem->sItemInfo.CODE & sinITEM_MASK3))
			{
				RuneItemIndex = q + 1;
				break;
			}
		}
		int RecipeIndex = 0;
		RecipeIndex = ManufactureItem.GetRecipeIndex(g_sManufactureItem.ManufactureItem[0].CODE & sinITEM_MASK3);
		for (int p = 0; p < 3; p++)
		{
			//for(int m=0; m < 3 ; m++)
			{
				for (int r = 0; r < MAX_RECIPE_KIND; r++)
				{
					if (g_sManufacture_Rune[r].RecipeCode == RecipeIndex)
					{
						if (g_sManufacture_Rune[r].NeedRuneIndex[p] == RuneItemIndex)
						{
							TradeStartX = ManufactureRuneXY[p].x;			// ��ǥ�� ���Ƽ� �� ����
							TradeStartY = ManufactureRuneXY[p].y;
							TradeEndX = TradeStartX + 22;
							TradeEndY = TradeStartY + 22;
							for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22)
							{
								for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22)
								{
									if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j)
									{ //�κ��丮 �ڽ��� �������� üũ 
										TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
										TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
										TradeColorRect.right = pItem->w;
										TradeColorRect.bottom = pItem->h;
										if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11)
										{
											cTrade.InitTradeColorRect(); //�÷� �ڽ� RECT �ʱ�ȭ (�÷� �ڽ� RECT�� �ϳ���� 0 �̸� �����Ҽ� ���� 
											return FALSE;
										}

										pItem->SetX = TradeColorRect.left;
										pItem->SetY = TradeColorRect.top;
										pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ�

										TradeColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���

										if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 5))
										{  //��ġ�� �������� �ֳ� üũ 
											TradeColorIndex = NOT_SETTING_COLOR;
											return FALSE;
										}

										return TRUE;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else if ((pItem->CODE & sinITEM_MASK2) == sinWR1 || (pItem->CODE & sinITEM_MASK2) == sinDR1)
	{
		TradeColorIndex = 0;
		TradeCrashItemIndex[0] = 0;
		cTrade.InitTradeColorRect();

		TradeStartX = 58;
		TradeStartY = 193 + sinInterHeight2;
		TradeEndX = TradeStartX + (3 * 22);
		TradeEndY = TradeStartY + (4 * 22);
		if (TradeStartX <= pCursorPos.x && TradeEndX > pCursorPos.x && TradeStartY <= pCursorPos.y && TradeEndY > pCursorPos.y)
		{
			TradeColorRect.left = TradeStartX;
			TradeColorRect.top = TradeStartY;
			TradeColorRect.right = TradeEndX - TradeStartX;
			TradeColorRect.bottom = TradeEndY - TradeStartY;


			pItem->SetX = TradeStartX + ((TradeEndX - TradeStartX) - pItem->w) / 2;
			pItem->SetY = TradeStartY + ((TradeEndY - TradeStartY) - pItem->h) / 2;
			pItem->ItemPosition = 0;
			TradeColorIndex = SET_ITEM_CHECK_COLOR;

			if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 5))
			{
				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;

			}

			if (sinChar->Money < pItem->sItemInfo.Price)
			{
				TradeColorIndex = NOT_ENOUGH_MONEY;
				return FALSE;
			}

			if (pItem->sItemInfo.SpecialItemFlag[0] == CHECK_COPY_ITEM ||
				pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinQW1 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinPZ1 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinPZ2 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinFO1 ||
				pItem->sItemInfo.CODE == (sinGF1 | sin01) ||
				pItem->sItemInfo.CODE == (sinGF1 | sin02) ||
				pItem->sItemInfo.CODE == (sinGF1 | sin03) ||
				pItem->sItemInfo.CODE == (sinGF1 | sin04) ||
				pItem->sItemInfo.CODE == (sinGF1 | sin05) ||
				pItem->sItemInfo.CODE == (sinGF1 | sin06) ||
				pItem->sItemInfo.CODE == (sinGF1 | sin07) ||
				pItem->sItemInfo.CODE == (sinGF1 | sin08))
			{

				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;

			}
			return TRUE;
		}
	}
	return TRUE;
}


bool CManufacture::isAreaEmpty(int Index)
{
	if (g_sManufactureItem.ManufactureItem[Index].Flag)
		return false;
	return true;
}


bool CManufacture::GetEmptyArea(sITEM * pItem, POINT * Area)
{
	POINT ManufactureRuneXY[3] = { { 240, 159 },{ 215, 190 },{ 265, 190 } };
	int RuneItemIndex = ((pItem->CODE & sinITEM_MASK3) >> 8);

	int RecipeIndex = ManufactureItem.GetRecipeIndex(g_sManufactureItem.ManufactureItem[0].CODE & sinITEM_MASK3);

	for (int p = 0; p < 3; p++) {
		for (int r = 0; r < MAX_RECIPE_KIND; r++) {
			if (g_sManufacture_Rune[r].RecipeCode == RecipeIndex) {
				if (g_sManufacture_Rune[r].NeedRuneIndex[p] == RuneItemIndex) {
					RECT Rectangle = { ManufactureRuneXY[p].x, ManufactureRuneXY[p].y, 0, 0 };
					Rectangle.right = Rectangle.left + 22;
					Rectangle.bottom = Rectangle.top + 22;
					if (!cTrade.CrashTradeItem(Rectangle, 0, 5)) {
						Area->x = ManufactureRuneXY[p].x + 2;
						Area->y = ManufactureRuneXY[p].y + 2;
						return true;
					}
				}
			}
		}
	}
	return false;
}

// pluto ���� ������ ���´�
int CManufacture::LastSetManufactureItem(sITEM * pItem)
{
	CheckManufactureItem();
	if ((pItem->CODE & sinITEM_MASK2) == sinPR3 || (pItem->CODE & sinITEM_MASK2) == sinPR4)
	{
		// ���� ���
		for (int j = 1; j < 4; j++)
		{
			if (!g_sManufactureItem.ManufactureItem[j].Flag)
			{
				memcpy(&g_sManufactureItem.ManufactureItem[j], pItem, sizeof(sITEM));
				g_sManufactureItem.ManufactureItem[j].x = pItem->SetX;
				g_sManufactureItem.ManufactureItem[j].y = pItem->SetY;
				pItem->Flag = 0; //�ʱ�ȭ 
				sinPlaySound(g_sManufactureItem.ManufactureItem[j].SoundIndex);
				ReFormManufactureItem();
				return TRUE;
			}
		}
	}
	else
	{
		if (!g_sManufactureItem.ManufactureItem[0].Flag)
		{
			memcpy(&g_sManufactureItem.ManufactureItem[0], pItem, sizeof(sITEM));
			g_sManufactureItem.ManufactureItem[0].x = pItem->SetX;
			g_sManufactureItem.ManufactureItem[0].y = pItem->SetY;
			pItem->Flag = 0; //�ʱ�ȭ 
			sinPlaySound(g_sManufactureItem.ManufactureItem[0].SoundIndex);
			ReFormManufactureItem();
			return TRUE;
		}
	}
	return TRUE;

}

// pluto ����
int CManufacture::CheckManufactureItem()
{ //üũ  
	int TempCheckDataSum = 0;

	for (int i = 0; i < 4; i++)
	{
		if (g_sManufactureItem.ManufactureItem[i].Flag)
		{
			TempCheckDataSum += (i + 1) * g_sManufactureItem.ManufactureItem[i].x;
			TempCheckDataSum += (i + 1) * g_sManufactureItem.ManufactureItem[i].y;
			TempCheckDataSum += g_sManufactureItem.ManufactureItem[i].sItemInfo.ItemHeader.dwChkSum;
			TempCheckDataSum += g_sManufactureItem.ManufactureItem[i].sItemInfo.ItemHeader.Head;
		}
	}
	return TRUE;
}

// pluto ����
int CManufacture::ReFormManufactureItem()
{
	//���� 
	m_ManufactureItemCheckSum = 0;
	for (int i = 0; i < 4; i++)
	{
		if (g_sManufactureItem.ManufactureItem[i].Flag)
		{
			m_ManufactureItemCheckSum += (i + 1) * g_sManufactureItem.ManufactureItem[i].x;
			m_ManufactureItemCheckSum += (i + 1) * g_sManufactureItem.ManufactureItem[i].y;
			m_ManufactureItemCheckSum += g_sManufactureItem.ManufactureItem[i].sItemInfo.ItemHeader.dwChkSum;
			m_ManufactureItemCheckSum += g_sManufactureItem.ManufactureItem[i].sItemInfo.ItemHeader.Head;
		}
	}
	return TRUE;
}


int CManufacture::PickUpManufactureItem(int x, int y, int PickUpFlag)
{
	int i = 0;
	SelectTradeItemIndex = 0;    //���� ������ 

	for (i = 0; i < 4; i++)
	{
		if (g_sManufactureItem.ManufactureItem[i].Flag)
		{
			if (g_sManufactureItem.ManufactureItem[i].x < x && g_sManufactureItem.ManufactureItem[i].x + g_sManufactureItem.ManufactureItem[i].w > x &&
				g_sManufactureItem.ManufactureItem[i].y < y && g_sManufactureItem.ManufactureItem[i].y + g_sManufactureItem.ManufactureItem[i].h > y)
			{
				if (PickUpFlag)
				{
					int cnt = 0;
					for (cnt = 1; cnt < 4; cnt++)
					{
						if (i == 0 && g_sManufactureItem.ManufactureItem[cnt].Flag)
						{
							cMessageBox.ShowMessage(MESSAGE_NOT_MANUFACTURE);
							return FALSE;
						}
					}

					CheckManufactureItem();
					memcpy(&MouseItem, &g_sManufactureItem.ManufactureItem[i], sizeof(sITEM));
					g_sManufactureItem.ManufactureItem[i].Flag = 0;
					memset(&g_sManufactureItem.ManufactureItem[i], 0, sizeof(sITEM));  //���� ��ŷ ���� ������ �ӽ� �����̴�.

					sinPlaySound(g_sManufactureItem.ManufactureItem[i].SoundIndex);
					TradeColorIndex = 0;
					ReFormManufactureItem();
					return TRUE;
				}
				else
				{
					SelectTradeItemIndex = i + 1;
					TradeColorIndex = SELECT_ITEM_COLOR;
					sinShowItemInfoFlag = 1;

					GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&g_sManufactureItem.ManufactureItem[i], FALSE, FALSE, FALSE);
				}
			}
		}
	}
	return FALSE;
}


// pluto ����
int CManufacture::ManufactureCheckEmptyArea(sITEM * pItem)
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;
	int StartX = 23;  //�κ� �ڽ��� ������ X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - pItem->w; //�������� �����ִ� ���� �� X��ǥ 
	cy = (22 * INVENTORY_BOX_COL) - pItem->h; //�������� �����ִ� ���� �� Y��ǥ

	//�ڵ尡 ���� �ʰų� �������� ������� ���� ���� �����Ҷ� �˻��ؼ� ��ġ�� ã�Ƴ��� 
	for (cntx = StartX; cntx <= StartX + cx; cntx += 22) {
		for (cnty = StartY; cnty <= StartY + cy; cnty += 22) {
			flag = 0;
			for (int i = 0; i < INVENTORY_MAXITEM; i++) {
				if (cInvenTory.InvenItem[i].Flag) {
					rect.left = cInvenTory.InvenItem[i].x;
					rect.right = cInvenTory.InvenItem[i].x + cInvenTory.InvenItem[i].w;
					rect.top = cInvenTory.InvenItem[i].y;
					rect.bottom = cInvenTory.InvenItem[i].y + cInvenTory.InvenItem[i].h;

					if (cntx > rect.left - pItem->w && cntx< rect.right &&
						cnty > rect.top - pItem->h && cnty < rect.bottom)
						flag++;
				}
			}
			if (flag == 0) {
				pItem->SetX = cntx;
				pItem->SetY = cnty;
				pItem->ItemPosition = 0;
				return TRUE;
			}
		}
	}

	return FALSE;
}

int CManufacture::sinSendManufactureItem()
{
	if (MixItemNoCopyFlag)
	{
		return FALSE;
	}

	int i = 0;

	//���縦 üũ�ϱ����� ������� �����
	memcpy(&g_sManufactureItemBackUp, &g_sManufactureItem, sizeof(SManufactureItem));
	MixItemNoCopyFlag = 1; //�������� ����Ǵ°� �����ϱ����� �÷����ش�
	memset(&g_sManufactureItem_Send.DesManufactureItem, 0, sizeof(sITEM));

	g_sManufactureItem_Send.Money = m_ManufacturePrice;
	//if( g_sManufactureItem.ManufactureItem[0].Flag )
	//{
	//	g_sManufactureItem_Send.DesManufactureItem = g_sManufactureItem.ManufactureItem[0];
	//}
	for (i = 0; i < 4; i++)
	{
		if (g_sManufactureItem.ManufactureItem[i].Flag)
		{
			g_sManufactureItem_Send.RuneCode[i] = g_sManufactureItem.ManufactureItem[i].CODE;
			g_sManufactureItem_Send.CheckSum[i] = g_sManufactureItem.ManufactureItem[i].sItemInfo.ItemHeader.dwChkSum;
			g_sManufactureItem_Send.Head[i] = g_sManufactureItem.ManufactureItem[i].sItemInfo.ItemHeader.Head;
		}
	}

	CheckCraftMoney = g_sManufactureItem_Send.Money;

	CraftItemSendServerIndex++; //�ε����� ������ 
	g_sManufactureItem_Send.Index = CraftItemSendServerIndex;

	//����� ����ü�� ������ ������ 
	SendManufactureItemToServer(&g_sManufactureItem_Send);

	return TRUE;
}

// pluto ����
int CManufacture::sinRecvManufactureItem(SManufactureItem_Server * pManufactureItem_Server)
{
	memcpy(&g_sManufactureItem_Recv, pManufactureItem_Server, sizeof(SManufactureItem_Server));
	return TRUE;
}

// pluto ���� // �庰 - ����
void CManufacture::RuneIndexInit()
{
	// pluto ���� �Ҷ� �ʿ��� �� �����ִ°�
	for (int i = 0; i < MAX_RECIPE_KIND; i++)
	{
		memset(&g_sManufacture_Rune[i], 0, sizeof(SManufacture_Rune));
	}

	g_sManufacture_Rune[0].RecipeCode = EXTREME_RECIPE; // ������ ������
	g_sManufacture_Rune[0].NeedRuneIndex[0] = 1;
	g_sManufacture_Rune[0].NeedRuneIndex[1] = 2;
	g_sManufacture_Rune[0].NeedRuneIndex[2] = 3;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[1].RecipeCode = ANCIENT_RECIPE; // ����� ������
	g_sManufacture_Rune[1].NeedRuneIndex[0] = 2;
	g_sManufacture_Rune[1].NeedRuneIndex[1] = 3;
	g_sManufacture_Rune[1].NeedRuneIndex[2] = 4;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[2].RecipeCode = MINOTAUR_RECIPE; // ������ ������
	g_sManufacture_Rune[2].NeedRuneIndex[0] = 3;
	g_sManufacture_Rune[2].NeedRuneIndex[1] = 4;
	g_sManufacture_Rune[2].NeedRuneIndex[2] = 5;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[3].RecipeCode = DOOM_RECIPE; // ����� ������
	g_sManufacture_Rune[3].NeedRuneIndex[0] = 3;
	g_sManufacture_Rune[3].NeedRuneIndex[1] = 4;
	g_sManufacture_Rune[3].NeedRuneIndex[2] = 5;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[4].RecipeCode = SALAMANDER_RECIPE; // ȭ���� ������
	g_sManufacture_Rune[4].NeedRuneIndex[0] = 3;
	g_sManufacture_Rune[4].NeedRuneIndex[1] = 4;
	g_sManufacture_Rune[4].NeedRuneIndex[2] = 5;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[5].RecipeCode = WYVERN_RECIPE; // �ٶ��� ������
	g_sManufacture_Rune[5].NeedRuneIndex[0] = 3;
	g_sManufacture_Rune[5].NeedRuneIndex[1] = 4;
	g_sManufacture_Rune[5].NeedRuneIndex[2] = 5;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[6].RecipeCode = PHOENIX_RECIPE; // �¾��� ������
	g_sManufacture_Rune[6].NeedRuneIndex[0] = 4;
	g_sManufacture_Rune[6].NeedRuneIndex[1] = 5;
	g_sManufacture_Rune[6].NeedRuneIndex[2] = 6;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[7].RecipeCode = FRENZY_RECIPE; // ������ ������
	g_sManufacture_Rune[7].NeedRuneIndex[0] = 5;
	g_sManufacture_Rune[7].NeedRuneIndex[1] = 6;
	g_sManufacture_Rune[7].NeedRuneIndex[2] = 7;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[8].RecipeCode = HEAVENS_RECIPE; // õ���� ������
	g_sManufacture_Rune[8].NeedRuneIndex[0] = 6;
	g_sManufacture_Rune[8].NeedRuneIndex[1] = 7;
	g_sManufacture_Rune[8].NeedRuneIndex[2] = 8;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[9].RecipeCode = SNOW_RECIPE; // ������ ������ // �庰 - ����
	g_sManufacture_Rune[9].NeedRuneIndex[0] = 9;
	g_sManufacture_Rune[9].NeedRuneIndex[1] = 10;
	g_sManufacture_Rune[9].NeedRuneIndex[2] = 11;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[10].RecipeCode = MEMORY_RECIPE; // ����� ������ // �庰 - ���������� ȥ
	g_sManufacture_Rune[10].NeedRuneIndex[0] = 12;
	g_sManufacture_Rune[10].NeedRuneIndex[1] = 13;
	g_sManufacture_Rune[10].NeedRuneIndex[2] = 14;


}

// pluto ���� ������ �ε����� �ʿ��� �� �׸� ���� �Ҷ� // �庰 - ���� // �庰 - ���������� ȥ
int CManufacture::GetRecipeIndex(DWORD Code)
{
	int RecipeIndex = -1;

	switch (Code)
	{
	case sin01:
		RecipeIndex = EXTREME_RECIPE;
		break;
	case sin02:
		RecipeIndex = ANCIENT_RECIPE;
		break;
	case sin03:
		RecipeIndex = MINOTAUR_RECIPE;
		break;
	case sin04:
		RecipeIndex = DOOM_RECIPE;
		break;
	case sin05:
		RecipeIndex = SALAMANDER_RECIPE;
		break;
	case sin06:
		RecipeIndex = WYVERN_RECIPE;
		break;
	case sin07:
		RecipeIndex = PHOENIX_RECIPE;
		break;
	case sin08:
		RecipeIndex = FRENZY_RECIPE;
		break;
	case sin09:
		RecipeIndex = HEAVENS_RECIPE;
		break;

	case sin10:
		RecipeIndex = SNOW_RECIPE; // �庰 - ���� : ������ ���ۼ�
		break;

	case sin11:
		RecipeIndex = MEMORY_RECIPE; //  ����� ���ۼ�	// �庰 - ���������� ȥ
		break;
	}
	/*
	if( Code == sin01 )
	{
		RecipeIndex = OLD_RECIPE;
		return RecipeIndex;
	}
	else if( Code == sin02 )
	{
		RecipeIndex = DISCOLORED_RECIPE;
		return RecipeIndex;
	}
	else if( Code == sin03 )
	{
		RecipeIndex = ANCIENT_RECIPE;
		return RecipeIndex;
	}
	else if( Code == sin04 )
	{
		RecipeIndex = DISMAL_RECIPE;
		return RecipeIndex;
	}
	else
	{
		return RecipeIndex;
	}
	*/
	return RecipeIndex;
}

// pluto ���� ���
int CManufacture::RecvManufactureItemResult(SManufactureItem_Server * pManufactureItem_Server)
{
	CheckMixItemCopyFlag = 0; //������� �÷�  
	if (!ManufactureItem.m_OpenFlag)
	{
		MixItemNoCopyFlag = 0; //������� �÷� �ʱ�ȭ
		memset(&g_sManufactureItem, 0, sizeof(SManufactureItem)); //����ü �ʱ�ȭ 
		memset(&g_sManufactureItemBackUp, 0, sizeof(SManufactureItem)); //����ü �ʱ�ȭ 
		memset(&g_sManufactureItem_Recv, 0, sizeof(SManufactureItem_Server)); //���� ���� ����ü �ʱ�ȭ 
		memset(&g_sManufactureItem_Send, 0, sizeof(SManufactureItem_Server)); //���� ���� ����ü �ʱ�ȭ 
		MixCancelButtonDelayFlag = 0;
		return FALSE;

	}

	int i, LineCount = 0, Temp;


	if (pManufactureItem_Server->Result)
	{
		ShowItemManufactureMessageFlag = 1; // �������� ���ۉ����� �޼����� �����ش�

		sITEM TempItem;
		if (LoadItemImage(&pManufactureItem_Server->DesManufactureItem.sItemInfo, &TempItem))
		{
			lstrcpy(szCraftItemBuff, pManufactureItem_Server->DesManufactureItem.sItemInfo.ItemName);

			strcat(szCraftItemBuff, "\r");
			Temp = lstrlen(szCraftItemBuff);
			for (i = 0; i < Temp; i++)
			{
				if (szCraftItemBuff[i] == '\r')
				{
					LineCount++;
				}
			}
			CraftItemMessageSize.x = 14;           //�ڽ� ������ 
			CraftItemMessageSize.y = ((LineCount) * 2) + 2;  //�ڽ� ������ 

			if (cInvenTory.CheckRequireItemToSet(&TempItem))
			{
				//��ٷ� ���õɼ��ִ����� üũ�Ѵ�  
				if (!cInvenTory.AutoSetInvenItem(&TempItem, 1))
				{
					sinThrowItemToFeild(&pManufactureItem_Server->DesManufactureItem);
				}
			}

		}

		CheckCharForm(); //���� 
		sinMinusMoney(m_ManufacturePrice);
		ReformCharForm(); //������ 
		SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
		SaveGameData();
		ManufactureItem.m_OpenFlag = 0; //â�� �ݴ´�
	}
	else
	{
		CheckCharForm(); //���� 
		sinMinusMoney(m_ManufacturePrice);
		ReformCharForm(); //������ 
		SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
		ManufactureItem.m_OpenFlag = 0; //â�� �ݴ´� 
		cMessageBox.ShowMessage(MESSAGE_FAILD_MANUFACTURE_ITEM);
	}

	//���簡�Ǿ��ų��� �������� �����
	for (i = 0; i < 4; i++)
	{
		if (g_sManufactureItemBackUp.ManufactureItem[i].Flag)
		{
			cInvenTory.DeleteInvenItemToServer(g_sManufactureItemBackUp.ManufactureItem[i].sItemInfo.CODE,
				g_sManufactureItemBackUp.ManufactureItem[i].sItemInfo.ItemHeader.Head,
				g_sManufactureItemBackUp.ManufactureItem[i].sItemInfo.ItemHeader.dwChkSum);
		}

	}
	memset(&g_sManufactureItem, 0, sizeof(SManufactureItem)); //����ü �ʱ�ȭ 
	memset(&g_sManufactureItemBackUp, 0, sizeof(SManufactureItem)); //����ü �ʱ�ȭ 
	memset(&g_sManufactureItem_Recv, 0, sizeof(SManufactureItem_Server)); //���� ���� ����ü �ʱ�ȭ 
	memset(&g_sManufactureItem_Send, 0, sizeof(SManufactureItem_Server)); //���� ���� ����ü �ʱ�ȭ 

	MixItemNoCopyFlag = 0; //������� �÷� �ʱ�ȭ
	MixCancelButtonDelayFlag = 0;
	m_ManufacturePrice = 0;
	ForceItemPrice2 = 0;
	return TRUE;
}

int CManufacture::GetManufactureCheckEmptyArea()
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;
	int StartX = 23;  //�κ� �ڽ��� ������ X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - 22; //�������� �����ִ� ���� �� X��ǥ 
	cy = (22 * INVENTORY_BOX_COL) - 22; //�������� �����ִ� ���� �� Y��ǥ

	int count = 0;
	//�ڵ尡 ���� �ʰų� �������� ������� ���� ���� �����Ҷ� �˻��ؼ� ��ġ�� ã�Ƴ��� 
	for (cntx = StartX; cntx <= StartX + cx; cntx += 22)
	{
		for (cnty = StartY; cnty <= StartY + cy; cnty += 22)
		{
			flag = 0;
			for (int i = 0; i < INVENTORY_MAXITEM; i++)
			{
				if (cInvenTory.InvenItem[i].Flag)
				{
					rect.left = cInvenTory.InvenItem[i].x;
					rect.right = cInvenTory.InvenItem[i].x + cInvenTory.InvenItem[i].w;
					rect.top = cInvenTory.InvenItem[i].y;
					rect.bottom = cInvenTory.InvenItem[i].y + cInvenTory.InvenItem[i].h;

					if (cntx > rect.left - 22 && cntx< rect.right &&
						cnty > rect.top - 22 && cnty < rect.bottom)
						flag++;
				}
			}
			if (flag == 0)
			{
				count++;
			}
		}
	}
	if (count)
		return count;
	else
		return 0;
}

// pluto ���� ȸ�� �̹��� ���� // �庰 - ���� // �庰 - ���������� ȥ
int CSmeltingItem::GetSheltomIndex(DWORD SmeltingItemCode)
{
	DWORD SmeltingItemAray[] = { (sinPR1 | sin01), (sinPR1 | sin02), (sinPR1 | sin03), (sinPR1 | sin04), (sinPR1 | sin05), (sinPR1 | sin06), (sinPR1 | sin07), (sinPR1 | sin08),
								 (sinPR2 | sin01), (sinPR2 | sin02), (sinPR2 | sin03), (sinPR2 | sin04), (sinPR2 | sin05), (sinPR2 | sin06), (sinPR2 | sin07), (sinPR2 | sin08),
								 (sinPR2 | sin09), (sinPR2 | sin10), (sinPR2 | sin11), (sinPR2 | sin12), (sinPR2 | sin13), (sinPR2 | sin14) };
	int SheltomAray[] = { 6, 7, 8, 9, 10, 11, 12, 13,
						  7, 8, 9, 10, 11, 12, 13, 14,
							3, 4, 5, 3, 4, 5 }; // �����̳� ���� ��ȣ
	for (int i = 0; i < 22; i++)
	{
		if (SmeltingItemCode == SmeltingItemAray[i])
		{
			return SheltomAray[i];
		}
	}
	return FALSE;
}

// pluto ���۵� ������ �ڵ�. ���� �˻� �ҷ��� // �庰 - ����
SManufacture_ResultItemInfo CManufacture::ManufactureCheckItem(sITEM * pItemCode, smCHAR_INFO * charInfo) // �庰 - ���������� ȥ
{
	int RuneCnt = 0;
	SManufacture_ResultItemInfo Result;
	memset(&Result, 0, sizeof(SManufacture_ResultItemInfo));
	DWORD Arm_Armor_Code[MAX_RECIPE_KIND] = { sin01, sin02, sin03, sin04, sin05, sin06, sin07, sin08, sin09, sin10, sin11 }; // ������ ���� // �庰 - ���������� ȥ
	DWORD CheckRuneCode[MAX_HANDMADEITEM][8][3] = { {	{ (sinPR3 | sin01), (sinPR3 | sin02), (sinPR3 | sin03) },  // �� ���ս��̴�
										{ (sinPR3 | sin01), (sinPR3 | sin02), (sinPR4 | sin03) },
										{ (sinPR3 | sin01), (sinPR4 | sin02), (sinPR3 | sin03) },
										{ (sinPR3 | sin01), (sinPR4 | sin02), (sinPR4 | sin03) },
										{ (sinPR4 | sin01), (sinPR3 | sin02), (sinPR3 | sin03) },
										{ (sinPR4 | sin01), (sinPR3 | sin02), (sinPR4 | sin03) },
										{ (sinPR4 | sin01), (sinPR4 | sin02), (sinPR3 | sin03) },
										{ (sinPR4 | sin01), (sinPR4 | sin02), (sinPR4 | sin03) } },
		////////////////////////////////////////////////////////////////////////////////////////////	������ ���ۼ�
	 {  { (sinPR3 | sin02), (sinPR3 | sin03), (sinPR3 | sin04) },
		{ (sinPR3 | sin02), (sinPR3 | sin03), (sinPR4 | sin04) },
		{ (sinPR3 | sin02), (sinPR4 | sin03), (sinPR3 | sin04) },
		{ (sinPR3 | sin02), (sinPR4 | sin03), (sinPR4 | sin04) },
		{ (sinPR4 | sin02), (sinPR3 | sin03), (sinPR3 | sin04) },
		{ (sinPR4 | sin02), (sinPR3 | sin03), (sinPR4 | sin04) },
		{ (sinPR4 | sin02), (sinPR4 | sin03), (sinPR3 | sin04) },
		{ (sinPR4 | sin02), (sinPR4 | sin03), (sinPR4 | sin04) } },
		////////////////////////////////////////////////////////////////////////////////////////////	����� ���ۼ�
	 {  { (sinPR3 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) } },
		////////////////////////////////////////////////////////////////////////////////////////////	������ ���ۼ�
	 {  { (sinPR3 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) } },
		////////////////////////////////////////////////////////////////////////////////////////////	����� ���ۼ�
	 {	{ (sinPR3 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) } },
		////////////////////////////////////////////////////////////////////////////////////////////	ȭ���� ���ۼ�
	 {  { (sinPR3 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) } },
		////////////////////////////////////////////////////////////////////////////////////////////	�ٶ��� ���ۼ�
	 {  { (sinPR3 | sin04), (sinPR3 | sin05), (sinPR3 | sin06) },
		{ (sinPR3 | sin04), (sinPR3 | sin05), (sinPR4 | sin06) },
		{ (sinPR3 | sin04), (sinPR4 | sin05), (sinPR3 | sin06) },
		{ (sinPR3 | sin04), (sinPR4 | sin05), (sinPR4 | sin06) },
		{ (sinPR4 | sin04), (sinPR3 | sin05), (sinPR3 | sin06) },
		{ (sinPR4 | sin04), (sinPR3 | sin05), (sinPR4 | sin06) },
		{ (sinPR4 | sin04), (sinPR4 | sin05), (sinPR3 | sin06) },
		{ (sinPR4 | sin04), (sinPR4 | sin05), (sinPR4 | sin06) } },
		////////////////////////////////////////////////////////////////////////////////////////////	�¾��� ���ۼ�
	 {  { (sinPR3 | sin05), (sinPR3 | sin06), (sinPR3 | sin07) },
		{ (sinPR3 | sin05), (sinPR3 | sin06), (sinPR4 | sin07) },
		{ (sinPR3 | sin05), (sinPR4 | sin06), (sinPR3 | sin07) },
		{ (sinPR3 | sin05), (sinPR4 | sin06), (sinPR4 | sin07) },
		{ (sinPR4 | sin05), (sinPR3 | sin06), (sinPR3 | sin07) },
		{ (sinPR4 | sin05), (sinPR3 | sin06), (sinPR4 | sin07) },
		{ (sinPR4 | sin05), (sinPR4 | sin06), (sinPR3 | sin07) },
		{ (sinPR4 | sin05), (sinPR4 | sin06), (sinPR4 | sin07) } },
		////////////////////////////////////////////////////////////////////////////////////////////	������ ���ۼ�
	 {  { (sinPR3 | sin06), (sinPR3 | sin07), (sinPR3 | sin08) },
		{ (sinPR3 | sin06), (sinPR3 | sin07), (sinPR4 | sin08) },
		{ (sinPR3 | sin06), (sinPR4 | sin07), (sinPR3 | sin08) },
		{ (sinPR3 | sin06), (sinPR4 | sin07), (sinPR4 | sin08) },
		{ (sinPR4 | sin06), (sinPR3 | sin07), (sinPR3 | sin08) },
		{ (sinPR4 | sin06), (sinPR3 | sin07), (sinPR4 | sin08) },
		{ (sinPR4 | sin06), (sinPR4 | sin07), (sinPR3 | sin08) },
		{ (sinPR4 | sin06), (sinPR4 | sin07), (sinPR4 | sin08) } },
		////////////////////////////////////////////////////////////////////////////////////////////	õ���� ���ۼ�

	 {  { (sinPR3 | sin09), (sinPR4 | sin10), (sinPR3 | sin11) },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 } },
		////////////////////////////////////////////////////////////////////////////////////////////	������ ���ۼ� //�庰

		{  { (sinPR3 | sin12), (sinPR3 | sin13), (sinPR3 | sin14) },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 } },
		////////////////////////////////////////////////////////////////////////////////////////////	����� ���ۼ� // �庰 - ���������� ȥ
	};

	// �庰 - ���������� ȥ
	DWORD	dwItem = 0;

	// ������Ƽ�� �� ���
	if (charInfo->JOB_CODE == JOBCODE_PRIESTESS) dwItem = (sinDA2 | sin52);
	// ������ �� ���
	else if (charInfo->JOB_CODE == JOBCODE_MAGICIAN) dwItem = (sinDA2 | sin51);
	// ��ó, ��Ʋ��Ÿ �� ���
	else if (charInfo->JOB_CODE == JOBCODE_ARCHER || charInfo->JOB_CODE == JOBCODE_ATALANTA) dwItem = (sinDA1 | sin52);
	// ������, ����ũ��, ��ī�ϼ�, ����Ʈ �� ���
	else dwItem = (sinDA1 | sin51);


	// �庰 �迭 ���� [a][b][c] a = ���� or ���� / b = ������ ���� / c = ������ ������ ����(����)
	// �����ǿ� �����տ� �´� �߻� ������
	DWORD CreateItemCode[2][MAX_HANDMADEITEM][8] = { {	{ (sinWA1 | sin14), (sinWC1 | sin14), (sinWH1 | sin15), (sinWP1 | sin15), (sinWS1 | sin16), (sinWS2 | sin17), (sinWT1 | sin15), (sinWM1 | sin15) }, // ������ ������ ����
											{ (sinWA1 | sin15), (sinWC1 | sin15), (sinWH1 | sin16), (sinWP1 | sin16), (sinWS1 | sin17), (sinWS2 | sin18), (sinWT1 | sin16), (sinWM1 | sin16) }, // ����� ������ ����
											{ (sinWA1 | sin16), (sinWC1 | sin16), (sinWH1 | sin17), (sinWP1 | sin17), (sinWS1 | sin18), (sinWS2 | sin19), (sinWT1 | sin17), (sinWM1 | sin17) }, // ������ ����
											{ (sinWA1 | sin17), (sinWC1 | sin17), (sinWH1 | sin18), (sinWP1 | sin18), (sinWS1 | sin19), (sinWS2 | sin20), (sinWT1 | sin18), (sinWM1 | sin18) }, // ����� ������ ����
											{ (sinWA1 | sin18), (sinWC1 | sin18), (sinWH1 | sin19), (sinWP1 | sin19), (sinWS1 | sin20), (sinWS2 | sin21), (sinWT1 | sin19), (sinWM1 | sin19) }, // ȭ���� ������ ����
											{ (sinWA1 | sin19), (sinWC1 | sin19), (sinWH1 | sin20), (sinWP1 | sin20), (sinWS1 | sin21), (sinWS2 | sin22), (sinWT1 | sin20), (sinWM1 | sin20) }, // �ٶ��� ������ ����
											{ (sinWA1 | sin21), (sinWC1 | sin21), (sinWH1 | sin22), (sinWP1 | sin22), (sinWS1 | sin23), (sinWS2 | sin24), (sinWT1 | sin22), (sinWM1 | sin22) }, // �¾��� ������ ����
											{ (sinWA1 | sin22), (sinWC1 | sin22), (sinWH1 | sin23), (sinWP1 | sin23), (sinWS1 | sin24), (sinWS2 | sin25), (sinWT1 | sin23), (sinWM1 | sin23) }, // ������ ������ ����
											{ 0, 0, 0, 0, 0, 0, (sinWS1 | sin25), (sinWS1 | sin26) }, // õ���� ������ ����
											{ (sinWS1 | sin27), (sinWS1 | sin28), (sinWS1 | sin29), (sinWS1 | sin30), (sinWS1 | sin31), (sinWS1 | sin33), (sinWS1 | sin34), 0 }, // ������ ������ // �庰
											{ 0, 0, 0, 0, 0, 0, 0, 0 } }, // ����� ������ // �庰 - ���������� ȥ
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
										{	{ (sinDA1 | sin16), (sinDA2 | sin16), (sinDS1 | sin14), (sinOM1 | sin15), (sinDG1 | sin14), (sinOA2 | sin14), (sinDB1 | sin14), 0 }, // ������ ������ ����
											{ (sinDA1 | sin17), (sinDA2 | sin17), (sinDS1 | sin15), (sinOM1 | sin16), (sinDG1 | sin15), (sinOA2 | sin15), (sinDB1 | sin15), 0 }, // ����� ������ ����
											{ (sinDA1 | sin18), (sinDA2 | sin18), (sinDS1 | sin16), (sinOM1 | sin17), (sinDG1 | sin16), (sinOA2 | sin16), (sinDB1 | sin16), 0 }, // ������ ������ ����
											{ (sinDA1 | sin19), (sinDA2 | sin19), (sinDS1 | sin17), (sinOM1 | sin18), (sinDG1 | sin17), (sinOA2 | sin17), (sinDB1 | sin17), 0 }, // ����� ������ ����
											{ (sinDA1 | sin20), (sinDA2 | sin20), (sinDS1 | sin18), (sinOM1 | sin19), (sinDG1 | sin18), (sinOA2 | sin18), (sinDB1 | sin18), 0 }, // ȭ���� ������ ����
											{ (sinDA1 | sin21), (sinDA2 | sin21), (sinDS1 | sin19), (sinOM1 | sin20), (sinDG1 | sin19), (sinOA2 | sin19), (sinDB1 | sin19), 0 }, // �ٶ��� ������ ����
											{ (sinDA1 | sin23), (sinDA2 | sin23), (sinDS1 | sin21), (sinOM1 | sin22), (sinDG1 | sin21), (sinOA2 | sin21), (sinDB1 | sin21), 0 }, // �¾��� ������ ����
											{ (sinDA1 | sin24), (sinDA2 | sin24), (sinDS1 | sin22), (sinOM1 | sin23), (sinDG1 | sin22), (sinOA2 | sin22), (sinDB1 | sin22), 0 }, // ������ ������ ����
											{ 0, 0, 0, 0, 0, 0, 0, 0 } , // õ���� ������ ����
											{ (sinOA1 | sin36), 0, 0, 0, 0, 0, 0, 0 } , // ������ ������ //�庰
											{ dwItem, 0, 0, 0, 0, 0, 0, 0 } } // ����� ������ // �庰 - ���������� ȥ
	};

	if ((pItemCode[0].CODE & sinITEM_MASK2) == sinWR1)		// ����
	{
		for (int i = 0; i < MAX_RECIPE_KIND; i++)
		{
			if (Arm_Armor_Code[i] == (pItemCode[0].CODE & sinITEM_MASK3))
			{
				for (int j = 0; j < MAX_RUNE; j++)   // �� ���� 
				{
					RuneCnt = 0;
					for (int x = 0; x < 3; x++) // �ʿ��� ��
					{
						for (int y = 0; y < 3; y++)
						{
							if (CheckRuneCode[i][j][x] == pItemCode[y + 1].CODE)
							{
								RuneCnt++;
								break;
							}
						}
						if (RuneCnt == 3)
						{
							for (int c = 0; c < MAX_HANDMADEITEM; c++) // �庰 - ���������� ȥ
							{
								if (CreateItemCode[0][i][j] == g_Manufacture_ItemInfo[c].ItemCode)
								{
									Result.ItemCode = g_Manufacture_ItemInfo[c].ItemCode;
									lstrcpy(Result.ItemName, g_Manufacture_ItemInfo[c].ItemName);
									Result.Weight = g_Manufacture_ItemInfo[c].Weight;
									Result.Price = g_Manufacture_ItemInfo[c].Price / 3;
									ForceItemPrice2 = Result.Price;
									m_ManufacturePrice = Result.Price;
								}
							}
							return Result;
						}
					}
				}
			}
		}
	}
	else if ((pItemCode[0].CODE & sinITEM_MASK2) == sinDR1)		// ����
	{
		for (int i = 0; i < MAX_RECIPE_KIND; i++)
		{
			if (Arm_Armor_Code[i] == (pItemCode[0].CODE & sinITEM_MASK3))
			{
				for (int j = 0; j < MAX_RUNE; j++)
				{
					RuneCnt = 0;
					for (int x = 0; x < 3; x++)
					{
						for (int y = 0; y < 3; y++)
						{
							if (CheckRuneCode[i][j][x] == pItemCode[y + 1].CODE)
							{
								RuneCnt++;
								break;
							}
						}
						if (RuneCnt == 3)
						{
							for (int c = 0; c < MAX_HANDMADEITEM; c++) // �庰 - ���������� ȥ
							{
								if (CreateItemCode[1][i][j] == g_Manufacture_ItemInfo[c].ItemCode)
								{
									Result.ItemCode = g_Manufacture_ItemInfo[c].ItemCode;
									lstrcpy(Result.ItemName, g_Manufacture_ItemInfo[c].ItemName);
									Result.Weight = g_Manufacture_ItemInfo[c].Weight;
									Result.Price = g_Manufacture_ItemInfo[c].Price / 3;
									ForceItemPrice2 = Result.Price;
									m_ManufacturePrice = Result.Price;
								}
							}
							if (j == 7)
							{
								for (int c = 0; c < MAX_HANDMADEITEM; c++) // �庰 - ���������� ȥ
								{
									if (CreateItemCode[1][i][0] == g_Manufacture_ItemInfo[c].ItemCode)
									{
										Result.ItemCode = g_Manufacture_ItemInfo[c].ItemCode;
										lstrcpy(Result.ItemName, ManufacturingTitle);	//fuck pluto//�ؿ�
										Result.Weight = g_Manufacture_ItemInfo[c].Weight;
										Result.Price = g_Manufacture_ItemInfo[c].Price / 3;
										ForceItemPrice2 = Result.Price;
										m_ManufacturePrice = Result.Price;
									}
								}
								return Result;
							}
							return Result;
						}
					}
				}
			}
		}
	}
	return Result;
}

// pluto ���� ������ ����, ���� ����
int ManufactureItemResultInfo(SManufacture_ResultItemInfo_Server * pResultItemInfo)
{
	for (int i = 0; i < MAX_HANDMADEITEM; i++) // �庰 - ���� MAX_HANDMADEITEM = ���� �� �� �ִ� ������ ����(�����ǰ� �߰��Ǹ� ����) // �庰 - ���������� ȥ
	{
		g_Manufacture_ItemInfo[i].ItemCode = pResultItemInfo->ItemCode[i];
		lstrcpy(g_Manufacture_ItemInfo[i].ItemName, pResultItemInfo->ItemName[i]);
		g_Manufacture_ItemInfo[i].Weight = pResultItemInfo->Weight[i];
		g_Manufacture_ItemInfo[i].Price = pResultItemInfo->Price[i];
	}
	return TRUE;
}