/*----------------------------------------------------------------------------*
*	ÆÄÀÏ¸í :  sinTrade.cpp
*	ÇÏ´ÂÀÏ :  ¾ÆÀÌÅÛ ±³È¯
*	ÀÛ¼ºÀÏ :  ÃÖÁ¾¾÷µ¥ÀÌÆ® 12¿ù sinInvenColor
*	Àû¼ºÀÚ :  ¹Ú»ó¿­
*-----------------------------------------------------------------------------*/

#include "sinLinkHeader.h"
#include "Character/playmain.h"
#include "Character/playsub.h"
#include "netplay.h"
#include "Character/character.h"
#include "fileread.h"
//#define  __SIN_AGING_TEST
#define AGING_MONEY_FREE // ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ °¡°İ ¹«·á ÀÌº¥Æ®
/*----------------------------------------------------------------------------*
*								Àü¿ªº¯¼ö
*-----------------------------------------------------------------------------*/
cTRADE cTrade;
sTRADE sTrade;	   //º¸³¾ Á¤º¸ 	
sTRADE sTradeRecv; //¹ŞÀº Á¤º¸ 

cMIXTURERESET cMixtureReset;					// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â °´Ã¼
sMIXTURE_RESET_ITEM	sMixtureResetItem;			// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â ¾ÆÀÌÅÛ Á¤º¸ ±¸Á¶Ã¼
sMIXTURE_RESET_ITEM sMixtureResetItemBackUp;	// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â ¾ÆÀÌÅÛ Á¤º¸ ±¸Á¶Ã¼ ¹é¾÷
int	MixtureResetCheckFlag = 0;					// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼ÂÀÌ °¡´ÉÇÑ°¡?
sMIXTURE_RESET_ITEM_SERVER	sMixtureReset_Send;	// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â ¼­¹ö·Î Àü¼ÛÇÒ ±¸Á¶Ã¼
sMIXTURE_RESET_ITEM_SERVER	sMixtureReset_Recv;	// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â ¼­¹ö¿¡¼­ ¹ŞÀ» ±¸Á¶Ã¼

cAGING	cAging;		//¿¡ÀÌÂ¡ 
sAGINGITEM sAgingItem;
sAGINGITEM sAgingItemBackUp;


sCRAFTITEM_SERVER sAging_Send; //¼­¹ö·Î º¸³»Áú ¾ÆÀÌÅÛ 
sCRAFTITEM_SERVER sAging_Recv; //¼­¹ö¿¡¼­ ¹ŞÀ» ¾ÆÀÌÅÛ 

cWAREHOUSE cWareHouse;	//Ã¢°í 
sWAREHOUSE sWareHouse;

int WareHouseSaveFlag = 0;
int CloseWareHouseCheckFlag2 = 0;
int CloseWareHouseCheckFlag3 = 0;

cCRAFTITEM cCraftItem;	//¾ÆÀÌÅÛ Á¶ÇÕ 
sCRAFTITEM sCraftItem;  //Á¶ÇÕµÉ ¾ÆÀÌÅÛ 
sCRAFTITEM sCraftItemBackUp;  //Á¶ÇÕµÉ ¾ÆÀÌÅÛ 

// pluto Á¦·Ã
CSmeltingItem SmeltingItem;
SSmeltingItem S_smeltingItem;
SSmeltingItem sSmeltingItemBackUp;
sSMELTINGITEM_SERVER sSmeltingItem_Send;
sSMELTINGITEM_SERVER sSmeltingItem_Recv;

// pluto Á¦ÀÛ
CManufacture ManufactureItem;
SManufactureItem g_sManufactureItem;
SManufactureItem g_sManufactureItemBackUp;
SManufactureItem_Server g_sManufactureItem_Send;
SManufactureItem_Server g_sManufactureItem_Recv;
SManufacture_Rune g_sManufacture_Rune[MAX_RECIPE_KIND];
SManufacture_ResultItemInfo g_Manufacture_ItemInfo[MAX_HANDMADEITEM]; // Àåº° - Á¦ÀÛ // Àåº° - ´ëÀåÀåÀÌÀÇ È¥
//SManufacture_ResultItemInfo_Server g_Item_Recv;

sCRAFTITEM_SERVER sCraftItem_Send; //¼­¹ö·Î º¸³»Áú ¾ÆÀÌÅÛ 
sCRAFTITEM_SERVER sCraftItem_Recv; //¼­¹ö¿¡¼­ ¹ŞÀ» ¾ÆÀÌÅÛ 

sCRAFTITEM_SERVER sWingItem_Send; //¼­¹ö·Î º¸³»Áú À®¾ÆÀÌÅÛ
sCRAFTITEM_SERVER sWingItem_Recv; //¼­¹ö¿¡¼­ ¹ŞÀ» ¾ÆÀÌÅÛ 

int MixItemNoCopyFlag = 0;


int CraftItemButtonIndex = 0;
int CraftItemSendServerIndex = 0;

/////////////// ¹Í½ºÃÄ º¹»ç ¹æÁö¸¦ À§ÇØ¼­ 
int CheckMixItemCopyFlag = 0;

int ShowItemCraftMessageFlag = 0; //¾ÆÀÌÅÛÀÌ Á¶ÇÕ‰çÀ»¶§ ¸Ş¼¼Áö¸¦ º¸¿©ÁØ´Ù 
int ShowItemAgingMessageFlag = 0; //¾ÆÀÌÅÛÀÌ ¿¡ÀÌÂ¡‰çÀ»¶§ ¸Ş¼¼Áö¸¦ º¸¿©ÁØ´Ù
int ShowItemSmeltingMessageFlag = 0;	// pluto ¾ÆÀÌÅÛÀÌ Á¦·Ã ‰çÀ»¶§
int ShowItemManufactureMessageFlag = 0; // pluto ¾ÆÀÌÅÛÀÌ Á¦ÀÛ ‰çÀ»¶§
int ShowItemOverSpace = 0; // pluto °ø°£ºÎÁ·

char szAgingItemBuff[128];
char szCraftItemBuff[128];
POINT CraftItemMessageBoxPosi = { 0,0 }; //¹Ú½º À§Ä¡ 
POINT CraftItemMessageSize = { 0,0 };    //¹Ú½º »çÀÌÁî 


sITEM* BackUpInvenItem2;
sITEM* BackUpInvenTempItem2;
sINVENTORY* BackUpsInven;
int   InvenGold = 0;
int   RealGold = 0;

int TradeCheckFlag = 0;

int CopyItemIndex7[10] = { 0,0,0,0,0,0,0,0,0,0 }; //º¹»çµÈ ¾ÆÀÌÅÛÀ» Ã£±â ÆíÇÏ°Ô ÇÏ±âÀ§ÇÑ ÇÃ·¢ 

int TradeColorIndex = 0;			//¹è°æ ÄÃ·¯ ÀÎµ¦½º 
int	TradeCrashItemIndex[2] = { 0,0 }; //°ãÃÄÁú ¾ÆÀÌÅÛ ÀÎµ¦½º 
RECT TradeColorRect = { 0,0,0,0 };
int  SelectTradeItemIndex = 0;		//¼±ÅÃ¾ÆÀÌÅÛ ¹è°æ 
int RequestTradeButtonFlag = 0;		//¹öÆ° ÇÃ·¢ 

int ItemKindFlag = 0; //1Àº ³»²¨  2´Â °Å·¡ÀÚ²¨ 

/////////////////////// ½ÂÀÎ °ü·Ã 
int CheckDelayFlag = 0;
int sinCancelFlag = 0; //Ãë¼Ò¸¸ ÇÒ¼öÀÖ´Ù 

/////////////////////// ÇØÅ· ¹æÁö 
int TempShowMoney = 0;
int TempShowCoin = 0;
///////////////////////
/*        Ã¢°í        */
int CloseWareHouseFlag = 0;

///////////////////////

/////////////////////// »ç±â¹æÁö 
int ItemPickUpFlag = 0;

//¿ìÃ¼±¹¿¡¼­ ¾ÆÀÌÅÛÀ» ¹Ş´Â´Ù
sPOSTBOX_ITEM sPostbox_Item[50];
int PostBoxIndexCnt = 0;
DWORD ExpressItemCode = 0; //¼­¹ö¿¡¼­ ¹ŞÀ» ¾ÆÀÌÅÛÄÚµå
char  szExpressItemName[64]; //¹ŞÀ» ¾ÆÀÌÅÛ ÀÌ¸§ 

///////////////////////
char* PostBoxDocFilePath[] = {
	"image\\Sinimage\\shopall\\PostBox.sin",

};
//haGoon¾ÆÀÌÅÛ Àç±¸¼º ½Ã½ºÅÛ 
sRECON_ITEM sReconItem[MAX_SEEL_COUNT] = {
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


//////°Å·¡¿ä±¸Ã¢ ÁÂÇ¥ 
#define TRADEBUTTONMAIN_X		513
#define TRADEBUTTONMAIN_Y		3

#include "..\\nettype.hpp"

//¾ÆÀÌÅÛ Å©·¡ÇÁÆ®
sCRAFTITEM_INFO sCraftItem_Info[MAX_CRAFTITEM_INFO] = {

#ifdef _W_SERVER
#include "sinCraftItem.h"
#else
#include "_sinCraftItem.h"
#endif
};

//¾ÆÀÌÅÛ Make
sMAKEITEM_INFO sMakeItem_Info[MAX_MAKEITEM_INFO] = {
	#include "sinMakeItem.h"
};



/*
	{21,136,21+(22*9),136+(22*4)},	//¼ÂÆÃ¹Ú½º
	{221,163-3-20,240,182-3-20},	//µ·¹öÆ° ÁÂÇ¥
	{244,163-3-20,307,180-3-20},	//µ·Ç¥½Ã ÁÂÇ¥
	{248,191-3-20,308,209-3-20},	//¹«°Ô
	{251,220,251+26,220+26}	//Ã¼Å©
*/
int UpTradePosi[5][4] = {
	{21,136 + sinInterHeight2,21 + (22 * 9),136 + (22 * 4) + sinInterHeight2},	//¼ÂÆÃ¹Ú½º 
	{231,170 + sinInterHeight2,250,189 + sinInterHeight2},	//µ·¹öÆ°
	{251,120,326,137},	//µ·Ç¥½Ã ÁÂÇ¥ 
	{251,118,326,162},	//¹«°Ô  (¿Ö ÀÌ·¸°Ô ÀÌ»óÇÏ°Ô µÇÀÖ´ÂÁö ³ªµÎ ¸Ö°Úµû -_-
	{252,221 + sinInterHeight2,252 + 26,221 + 26 + sinInterHeight2}	//Ã¼Å© 

};

int DownTradePosi[6][4] = {
	{21,224 + 21 + sinInterHeight2,21 + (22 * 9),224 + 21 + (22 * 4) + sinInterHeight2},	//¼ÂÆÃ¹Ú½º 
	{225,205,250,226},	//µ·¹öÆ° ÁÂÇ¥ 
	{251,206,326,224},	//µ·Ç¥½Ã ÁÂÇ¥ 
	{251,232,326,250},	//¹«°Ô 
	{252 + 1,310 + sinInterHeight2,252 + 26,310 + 26 + sinInterHeight2},//Ã¼Å© 
	{305,314 + sinInterHeight2,304 + 20,314 + 20 + sinInterHeight2},	//´İ±â 

};

//ÁÂÇ¥¼ÂÆÃ 
int TradeStartX = DownTradePosi[T_SET_BOX][0], TradeStartY = DownTradePosi[T_SET_BOX][1], TradeEndX = DownTradePosi[T_SET_BOX][2], TradeEndY = DownTradePosi[T_SET_BOX][3];

char szAgingMsgBuff[128];
char* SheltomName[MAX_SHELTOM] = { "Lucidy", "Fadeo", "Sparki", "Raident", "Transparo", "Murki", "Devine", "Celesto", "Miraje", "¹Inferna", "Enigma", "Bellum", "Ordo", "Sapphire" };

/////ÀÓ½ÃÀûÀ¸·Î ¿¡ÀÌÂ¡¿¡ °ü·ÃµÈ ½©ÅÒÀ» ¼ÂÆÃÇÑ´Ù 

//¿¡ÀÌÂ¡¿¡ ¾²ÀÏ ½©ÅÒ 

#ifdef __SIN_AGING_TEST

//°ø°İÈ½¼ö 
int AgingLevelAttack[21] = { 2,4,6,8,10,11,12,13,14,15,10,10,10,10,10,10,10,10,10,10,10 };

//Å©¸®Æ¼ÄÃ È½¼ö 
int AgingLevelCritical[21] = { 2,4,6,8,10,11,12,13,14,15,10,10,10,10,10,10,10,10,10,10,10 };

//ºí·°È½¼ö
int AgingLevelBlock[21] = { 2,4,6,8,10,11,12,13,14,15,10,10,10,10,10,10,10,10,10,10,10 };

//Å¸°İÈ½¼ö 
int AgingLevelHit[21] = { 2,4,6,8,10,11,12,13,14,15,10,10,10,10,10,10,10,10,10,10,10 };

int AgingLevelSheltom[20][12] = {
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},

	//Ãß°¡
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	// 16ºÎÅÍ
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},

};
int AgingOkPercent[20] = { 0,5,15,30,50,60,70,80,90,95,10,10,10,10,10,10,10,10,10,10 };


#else 

/////////////////////¿¡ÀÌÂ¡ ·¹º§º° Å¸°İÈ½¼ö 
#ifdef _W_SERVER


// Aging System
int AgingLevelAttack[MAX_AGING] = { 200,400,800,1500,3000,5000,10000,15000,25000,50000,10,100,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10 };

int AgingLevelCritical[MAX_AGING] = { 40,80,160,300,600,1000,2000,3000,5000,10000,10,100,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10 };

int AgingLevelBlock[MAX_AGING] = { 40,80,160,300,600,1000,2000,3000,5000,10000,10,100,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10 };

int AgingLevelHit[MAX_AGING] = { 100,200,300,750,1500,2500,5000,7500,12500,25000,10,100,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10 };

//Falha do aging
int AgingOkPercent[MAX_AGING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15, 16, 16, 18, 19, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 };
int AgingStoneAgingOkPercent[MAX_AGING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 12, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 };

int SuperAgingStoneAgingFailPercent[MAX_AGING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50, 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50, 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50, 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50, 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 };
int SuperAgingStone1_5AgingFailPercent[MAX_AGING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50, 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50, 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50, 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50, 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 };

int chanceQuebra[MAX_AGING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 8, 10, 11, 12, 13, 14, 15, 16, 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 1818, 18, 18, 18, 18, 18, 18, 18, 18, 18,18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18 };

#endif


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
	{9,9,9,9,0,0,0,0,0,0,0,0}, //+10

	{9,9,9,9,0,0,0,0,0,0,0,0},
	{9,9,9,9,0,0,0,0,0,0,0,0},
	{9,9,9,9,0,0,0,0,0,0,0,0},
	{9,9,9,9,9,0,0,0,0,0,0,0},
	{9,9,9,9,9,0,0,0,0,0,0,0},
	{9,9,9,9,9,0,0,0,0,0,0,0},
	{9,9,9,9,9,0,0,0,0,0,0,0},
	{9,9,9,9,9,0,0,0,0,0,0,0},
	{9,9,9,9,9,9,0,0,0,0,0,0},
	{9,9,9,9,9,9,9,0,0,0,0,0}, //+20

	{9,9,9,9,9,9,9,0,0,0,0,0},
	{9,9,9,9,9,9,9,0,0,0,0,0},
	{9,9,9,9,9,9,9,0,0,0,0,0},
	{9,9,9,9,9,9,9,0,0,0,0,0},
	{9,9,9,9,9,9,9,9,0,0,0,0},
	{9,9,9,9,9,9,9,9,0,0,0,0},
	{9,9,9,9,9,9,9,9,0,0,0,0},
	{9,9,9,9,9,9,9,9,0,0,0,0},
	{9,9,9,9,9,9,9,9,0,0,0,0},
	{9,9,9,9,9,9,9,9,0,0,0,0}, // +30

	{9,9,9,9,9,9,9,9,9,0,0,0},
	{9,9,9,9,9,9,9,9,9,0,0,0},
	{9,9,9,9,9,9,9,9,9,9,0,0},
	{9,9,9,9,9,9,9,9,9,9,0,0},
	{9,9,9,9,9,9,9,9,9,9,0,0},
	{9,9,9,9,9,9,9,9,9,9,0,0},
	{9,9,9,9,9,9,9,9,9,9,9,9},
	{9,9,9,9,9,9,9,9,9,9,9,9},
	{9,9,9,9,9,9,9,9,9,9,9,9},
	{9,9,9,9,9,9,9,9,9,9,9,9}, // +40

	{10,10,0,0,0,0,0,0,0,0,0,0},
	{10,10,0,0,0,0,0,0,0,0,0,0},
	{10,10,0,0,0,0,0,0,0,0,0,0},
	{10,10,0,0,0,0,0,0,0,0,0,0},
	{10,10,10,0,0,0,0,0,0,0,0,0},
	{10,10,10,0,0,0,0,0,0,0,0,0},
	{10,10,10,10,0,0,0,0,0,0,0,0},
	{10,10,10,10,0,0,0,0,0,0,0,0},
	{10,10,10,10,0,0,0,0,0,0,0,0},
	{10,10,10,10,0,0,0,0,0,0,0,0}, // +50

	{10,10,10,10,10,0,0,0,0,0,0,0},
	{10,10,10,10,10,0,0,0,0,0,0,0},
	{10,10,10,10,10,10,0,0,0,0,0,0},
	{10,10,10,10,10,10,10,0,0,0,0,0},
	{10,10,10,10,10,10,10,0,0,0,0,0},
	{10,10,10,10,10,10,10,10,0,0,0,0},
	{10,10,10,10,10,10,10,10,10,0,0,0},
	{10,10,10,10,10,10,10,10,10,10,0,0},
	{10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10}, // +60
};


///////////////////

#endif

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
							16000 ^ SIN_AGING_MASK,17000 ^ SIN_AGING_MASK,18000 ^ SIN_AGING_MASK,19000 ^ SIN_AGING_MASK,20000 ^ SIN_AGING_MASK
							,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK
							,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK
							,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK
							,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK
							,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK
							,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK
							,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK
							,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK ,20000 ^ SIN_AGING_MASK };

POINT AgingLevelSheltomXY[4][3] = {
	{{218,193 + sinInterHeight2}			,{218 + 22,193 + sinInterHeight2}		  ,{218 + 22 + 22,193 + sinInterHeight2}},
	{{218,193 + 22 + sinInterHeight2}		,{218 + 22,193 + 22 + sinInterHeight2}	  ,{218 + 22 + 22,193 + 22 + sinInterHeight2}},
	{{218,193 + 22 + 22 + sinInterHeight2}	,{218 + 22,193 + 22 + 22 + sinInterHeight2}   ,{218 + 22 + 22,193 + 22 + 22 + sinInterHeight2}},
	{{218,193 + 22 + 22 + 22 + sinInterHeight2} ,{218 + 22,193 + 22 + 22 + 22 + sinInterHeight2},{218 + 22 + 22,193 + 22 + 22 + 22 + sinInterHeight2}}

};

// Force Orb
int ForceItemPrice2 = 0;
void* lpForceMain;
void* lpForceButtonInfo;
void* lpForceTitle;


/*----------------------------------------------------------------------------*
*							Å¬·¡½º ÃÊ±â, Á¾·á
*-----------------------------------------------------------------------------*/
cTRADE::cTRADE()
{

}
cTRADE::~cTRADE()
{

}
/*----------------------------------------------------------------------------*
*							     ÃÊ±âÈ­
*-----------------------------------------------------------------------------*/
void cTRADE::Init() {}
/*----------------------------------------------------------------------------*
*						    BmpÆÄÀÏÀ» ·ÎµåÇÑ´Ù
*-----------------------------------------------------------------------------*/
void cTRADE::Load() {}
/*----------------------------------------------------------------------------*
*						     Release
*-----------------------------------------------------------------------------*/
void cTRADE::Release() {}
/*----------------------------------------------------------------------------*
*							     ±×¸®±â
*-----------------------------------------------------------------------------*/
DWORD TradeSpotTimer = 0;

void cTRADE::Draw() {}
/*----------------------------------------------------------------------------*
*							     ¸ŞÀÎ
*-----------------------------------------------------------------------------*/
DWORD	dwTradeDelayTime = 0;
DWORD	dwTradeDelayTime2 = 0;
DWORD	dwTradeDelayTime3 = 0;
DWORD	dwTradeDelayTime4 = 0;

DWORD   dwCraftItemInfoDelayTime = 0;
DWORD   dwMixDelayTime = 0;
DWORD	dwMixItemColyTime = 0;
int		MixCancelButtonDelayFlag = 0;
DWORD   dwForceDelayTime = 0;

void cTRADE::Main() {}
/*----------------------------------------------------------------------------*
*							     Á¾·á
*-----------------------------------------------------------------------------*/
void cTRADE::Close()
{

}
/*----------------------------------------------------------------------------*
*							   LButtonDown
*-----------------------------------------------------------------------------*/
void cTRADE::LButtonDown(int x, int y) {}
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
			lstrcpy(szCraftItemBuff,"¸ğµç¼Ó¼º +5\r±â·Â +10\r¸íÁß·Â +50\r ÃÖ´ë°ø°İ·Â+5\r"); //¾ÆÀÌÅÛ Á¤º¸¸¦ º¸¿©ÁØ´Ù
			CraftItemMessageSize.x = 14;           //¹Ú½º »çÀÌÁî
			CraftItemMessageSize.y = ((3)*2)+2;  //¹Ú½º »çÀÌÁî


		}
	*/
}

extern int AgingEvento;

void cTRADE::DrawTradeText() {}

/////////////// Æ®·¹ÀÌµå °ü·Ã ÇÔ¼ö 

//¼ÂÆÃÇÒ ¿µ¿ªÀ» Ã£´Â´Ù 
int cTRADE::SetTradeItemAreaCheck(sITEM* pItem)
{

	int i, j;
	TradeColorIndex = 0; //ÃÊ±âÈ­   (Æ®·¹ÀÌµå Ã¢°ú Ã¢°í°¡ °°ÀÌ ¾´´Ù)
	TradeCrashItemIndex[0] = 0; //Ãæµ¹µÈ ¾ÆÀÌÅÛ Index ÃÊ±âÈ­ 
	InitTradeColorRect(); //ÃÊ±âÈ­

	TradeStartX = DownTradePosi[T_SET_BOX][0];
	TradeStartY = DownTradePosi[T_SET_BOX][1];
	TradeEndX = DownTradePosi[T_SET_BOX][2];
	TradeEndY = DownTradePosi[T_SET_BOX][3];

	for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22) {
		for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22) {
			if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j) { //ÀÎº¥Åä¸® ¹Ú½º¿¡ µé¾î°¬´ÂÁö¸¦ Ã¼Å© 
				TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
				TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
				TradeColorRect.right = pItem->w;
				TradeColorRect.bottom = pItem->h;
				if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
					InitTradeColorRect(); //ÄÃ·¯ ¹Ú½º RECT ÃÊ±âÈ­ (ÄÃ·¯ ¹Ú½º RECTÁß ÇÏ³ª¶óµÎ 0 ÀÌ¸é ¼ÂÆÃÇÒ¼ö ¾ø´Ù 
					return FALSE;
				}

				pItem->SetX = TradeColorRect.left;
				pItem->SetY = TradeColorRect.top;
				pItem->ItemPosition = 0; //¹è¿­ÀÇ ÀÎµ¦½º¸¦ ÀúÀåÇÑ´Ù 
				TradeColorIndex = SET_ITEM_CHECK_COLOR; //¼ÂÆÃÇÒ¿µ¿ª

				if (TradeCrashItemIndex[0] = CrashTradeItem(TradeColorRect)) {  //°ãÄ¡´Â ¾ÆÀÌÅÛÀÌ ÀÖ³ª Ã¼Å© 
					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;
				}
				// ¹ÚÀç¿ø - °Å·¡ °ü·Ã ¾ÆÀÌÅÛ ¼³Á¤
				//ÁõÁ¤¿ë ¾ÆÀÌÅÛÀÌ³ª º¹»ç¾ÆÀÌÅÛÀº Æ®·¹ÀÌµå ÇÒ¼ö¾ø´Ù  (Æ÷Áö¼ÇÀÌ ÀÖ´Â ¾ÆÀÌÅÛµµ Æ®·¹ÀÌµåÇÒ¼ö¾ø´Ù)
				if (pItem->sItemInfo.SpecialItemFlag[0] == CHECK_COPY_ITEM ||
					pItem->sItemInfo.SpecialItemFlag[0] == CHECK_GIVE_ITEM ||
					pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinQW1 ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinBI1 ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinBI2 ||		//ÆĞÅ°ÁöÄ³½¬ ¾ÆÀÌÅÛµµ Æ®·¹ÀÌµå ÇÒ¼ö¾ø´Ù.
					//ÆÄÆ¼º¹ ÄÚ½ºÆ¬ 

					(pItem->sItemInfo.CODE == (sinDA1 | sin32)) || //¹ÚÀç¿ø : »êÅ¸¾Æ¹Ä·¿sin32°ú ÆÄÆ¼º¹ sin32 ¾ÆÀÌÅÛÀÌ °ãÃÄ¼­ ±¸ºĞÇÔ					
					(pItem->sItemInfo.CODE == (sinDA2 | sin32)) || //¹ÚÀç¿ø : »êÅ¸¾Æ¹Ä·¿sin32°ú ÆÄÆ¼º¹ sin32 ¾ÆÀÌÅÛÀÌ °ãÃÄ¼­ ±¸ºĞÇÔ
					(pItem->sItemInfo.CODE == (sinDA1 | sin33)) || //¹ÚÀç¿ø : ÀÌº¥Æ®¾Æ¹Ä·¿sin33°ú ÆÄÆ¼º¹ sin33 ¾ÆÀÌÅÛÀÌ °ãÃÄ¼­ ±¸ºĞÇÔ					
					(pItem->sItemInfo.CODE == (sinDA2 | sin33)) || //¹ÚÀç¿ø : ÀÌº¥Æ®¾Æ¹Ä·¿sin33°ú ÆÄÆ¼º¹ sin33 ¾ÆÀÌÅÛÀÌ °ãÃÄ¼­ ±¸ºĞÇÔ	
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
					(pItem->sItemInfo.CODE == (sinOA2 | sin31)) || (pItem->sItemInfo.CODE == (sinOA2 | sin32)) || // ¹ÚÀç¿ø - ½´ÆÛ ¾Ï¸´(7ÀÏ, 30ÀÏ) Ãß°¡
					(pItem->sItemInfo.CODE == (sinDB1 | sin31)) || (pItem->sItemInfo.CODE == (sinDB1 | sin32)) || // ¹ÚÀç¿ø - ½ºÇÇµå ºÎÃ÷(7ÀÏ, 30ÀÏ) Ãß°¡
					(pItem->sItemInfo.CODE == (sinDA1 | sin54)) || (pItem->sItemInfo.CODE == (sinDA2 | sin54)) || // ¹ÚÀç¿ø - ¼ö¿µº¹ º¹Àå Ãß°¡ - °Å·¡ ±İÁö(°Å·¡±İÁö)
					(pItem->sItemInfo.CODE == (sinDA1 | sin55)) || (pItem->sItemInfo.CODE == (sinDA2 | sin55)) || // ¹ÚÀç¿ø - ¼ö¿µº¹ º¹Àå Ãß°¡ - °Å·¡ ±İÁö(°Å·¡±İÁö)
//					(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin48 ||	//ÇÑº¹ Kyle¼öÁ¤
pItem->sItemInfo.CODE == (sinGF1 | sin01) || (pItem->sItemInfo.CODE == (sinOR2 | sin33)) || // Àåº° - ÇÏÆ®¸µ(7ÀÏ) Ãß°¡
//pItem->sItemInfo.CODE == (sinGF1|sin02) ||
(pItem->sItemInfo.CODE == (sinOA1 | sin36)) ||	 // Àåº° - ´«²É ¸ñ°ÉÀÌ(7ÀÏ) Ãß°¡ 
(pItem->sItemInfo.CODE == (sinOA1 | sin37)) ||   // Àåº° - Äµµğµ¥ÀÌÁî ÇÏÆ®¾Æ¹Ä·¿(7ÀÏ) Ãß°¡
(pItem->sItemInfo.CODE == (sinDB1 | sin33)) ||	 // Àåº° - ½ºÇÇµå ºÎÃ÷(1ÀÏ) Ãß°¡
(pItem->sItemInfo.CODE == (sinOA2 | sin33)) ||   // Àåº° - ½´ÆÛ ¾Ï¸´(1ÀÏ)
(pItem->sItemInfo.CODE == (sinBI1 | sin84)) ||	 // Àåº° - ±×¶óºñÆ¼ ½ºÅ©·Ñ
(pItem->sItemInfo.CODE == (sinDB1 | sin34)) ||	 // Àåº° - ½ºÇÇµå ºÎÃ÷(1½Ã°£) Ãß°¡
(pItem->sItemInfo.CODE == (sinOA2 | sin34)) ||   // Àåº° - ½´ÆÛ ¾Ï¸´(1½Ã°£)
pItem->sItemInfo.CODE == (sinGF1 | sin07) ||	// Àåº° - Á¶»ç¿øÀ» Ã£¾Æ¶ó
pItem->sItemInfo.CODE == (sinGF1 | sin08) ||
pItem->ItemPosition) {
					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;

				}

				int kk = 0;
				//µî·ÏµÈ ¾ÆÀÌÅÛÀº Æ®·¹ÀÌµå ÇÒ¼ö¾ø´Ù
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

				//ÀÌº¥Æ® ¾ÆÀÌÅÛÀº Æ®·¹ÀÌµåÇÒ¼ö¾ø´Ù 
				/*
				if((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinGP1){
					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;
				}
				*/

				return TRUE;
			}

		}

	}

	return TRUE;
}
//¹Ú½º ÃÊ±âÈ­ 
int cTRADE::InitTradeColorRect()
{
	TradeColorRect.left = 0;
	TradeColorRect.top = 0;
	TradeColorRect.right = 0;
	TradeColorRect.bottom = 0;
	return TRUE;

}
//¾ÆÀÌÅÛÀÌ °ãÃÄÁ³´ÂÁö¸¦ Ã£´Â´Ù 
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
					return i + 1; //Ãæµ¹ Ã¼Å©µÈ ¾ÆÀÌÅÛ ÀÎµ¦½º¸¦ ÇÏ³ª´õÇØ¼­ ¸®ÅÏÇÑ´Ù 
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
					return i + 1; //Ãæµ¹ Ã¼Å©µÈ ¾ÆÀÌÅÛ ÀÎµ¦½º¸¦ ÇÏ³ª´õÇØ¼­ ¸®ÅÏÇÑ´Ù 
				}
			}
		}

	}
	if (Kind == 2) {	//Å©·¡ÇÁÆ® 
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
					return i + 1; //Ãæµ¹ Ã¼Å©µÈ ¾ÆÀÌÅÛ ÀÎµ¦½º¸¦ ÇÏ³ª´õÇØ¼­ ¸®ÅÏÇÑ´Ù 
				}
			}
		}
	}
	if (Kind == 3) { //¿¡ÀÌÂ¡ 
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
					return i + 1; //Ãæµ¹ Ã¼Å©µÈ ¾ÆÀÌÅÛ ÀÎµ¦½º¸¦ ÇÏ³ª´õÇØ¼­ ¸®ÅÏÇÑ´Ù 
				}
			}
		}
	}
	// pluto Á¦·Ã
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
	// pluto Á¦ÀÛ
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

	// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â
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

				// ÇØ´ç ¾ÆÀÌÅÛÀÇ ÁÂÇ¥¸¦ ¾ò´Â´Ù.
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

	return FALSE;

}

//°ãÄ£ ¾ÆÀÌÅÛÀ» ¼ÂÆÃÇÑ´Ù  
/*
int cTRADE::ChangeTradeItem(sITEM *pItem)
{

	if(TradeCrashItemIndex[0]){//Ãæµ¹ µÈ ¾ÆÀÌÅÛÀÌ ÀÖÀ¸¸é ¾ÆÀÌÅÛÀ» ¹Ù²ãÁØ´Ù
		memcpy(&TempItem,&sTrade.TradeItem[TradeCrashItemIndex[0]-1],sizeof(sITEM)); //¸¶¿ì½º ¾ÆÀÌÅÛÀ» ÅÛÇÁ·Î º¹»ç
		sTrade.TradeItem[TradeCrashItemIndex[0]-1].Flag = 0 ; //º¹»çÇÑÈÄ¿¡ ÃÊ±âÈ­
		TempItem.x = pItem->x;
		TempItem.y = pItem->y;
		LastSetTradeItem(pItem); //¸¶¿ì½º ¾ÆÀÌÅÛÀ» ÀÎº¥Åä¸®·Î º¹»ç
		memcpy(pItem,&TempItem,sizeof(sITEM));
		SendTradeItem(TradeCharCode);
		return TRUE;

	}
	return FALSE;
}
*/
//Æ®·¹ÀÌµåÃ¢À¸·Î  ¾ÆÀÌÅÛÀ¸·Î º¹»çÇÑ´Ù 
int cTRADE::LastSetTradeItem(sITEM* pItem)
{
	CheckTradeItemForm();

	for (int j = 0; j < MAX_TRADE_ITEM; j++) {
		if (!sTrade.TradeItem[j].Flag) {
			if (!CheckTrade((void*)&sTrade)) //Àß¸øµÈ µ¥ÀÌÅ¸ ÀÌ¸é °Å·¡Ã¢À» ´İ´Â´Ù
				CancelTradeItem();

			memcpy(&sTrade.TradeItem[j], pItem, sizeof(sITEM));
			sTrade.TradeItem[j].x = pItem->SetX;
			sTrade.TradeItem[j].y = pItem->SetY;
			pItem->Flag = 0; //ÃÊ±âÈ­ 
			sTrade.CheckFlag = 0; //¾ÆÀÌÅÛÀÌ ¼ÂÆÃµÇ¸é ÇÃ·¢À» ÃÊ±âÈ­ÇÑ´Ù 
			//sinPlaySound(sTrade.TradeItem[j].SoundIndex);
			ReformTrade((void*)&sTrade);
			GetWeight(); //¹«°Ô¸¦ Ã¼Å©ÇÑ´Ù 
			SendTradeItem(TradeCharCode);
			ReFormTradeItem();
			return TRUE;

		}
	}
	ReFormTradeItem();
	return FALSE;
}

int cTRADE::PickUpTradeItem(int x, int y, int PickUpFlag)
{
	int i = 0;
	SelectTradeItemIndex = 0;    //ÁıÀ» ¾ÆÀÌÅÛ 
	ItemKindFlag = 0;			 //¾ÆÀÌÅÛ ¹è°æÀ» ³ªÅ¸³»±â À§ÇØ¼­ º¸Á¤ÇØÁØ´Ù 
	for (i = 0; i < MAX_TRADE_ITEM; i++) {
		if (sTrade.TradeItem[i].Flag) {
			if (sTrade.TradeItem[i].x < x && sTrade.TradeItem[i].x + sTrade.TradeItem[i].w > x &&
				sTrade.TradeItem[i].y < y && sTrade.TradeItem[i].y + sTrade.TradeItem[i].h > y) {
				if (PickUpFlag) {
					CheckTradeItemForm();
					if (!CheckTrade((void*)&sTrade)) //Àß¸øµÈ µ¥ÀÌÅ¸ ÀÌ¸é °Å·¡Ã¢À» ´İ´Â´Ù
						CancelTradeItem();
					memcpy(&MouseItem, &sTrade.TradeItem[i], sizeof(sITEM));
					sTrade.TradeItem[i].Flag = 0;
					//sinPlaySound(sTrade.TradeItem[i].SoundIndex);
					TradeColorIndex = 0;
					CheckDelayFlag = 1;
					sTrade.CheckFlag = 0;
					ReformTrade((void*)&sTrade);
					GetWeight(); //¹«°Ô¸¦ Ã¼Å©ÇÑ´Ù 
					SendTradeItem(TradeCharCode);
					ReFormTradeItem();
					return TRUE;
				}
				else {
					SelectTradeItemIndex = i + 1;
					TradeColorIndex = SELECT_ITEM_COLOR;
					ItemKindFlag = 1;
					sinShowItemInfoFlag = 1; //¾ÆÀÌÅÛÀÇ Á¤º¸¸¦ º¸¿©ÁØ´Ù 
					cItem.ShowItemInfo(&sTrade.TradeItem[i], 2);
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
				sinShowItemInfoFlag = 1; //¾ÆÀÌÅÛÀÇ Á¤º¸¸¦ º¸¿©ÁØ´Ù 
				sTradeRecv.TradeItem[i].y -= T_REVISION_Y;
				cItem.ShowItemInfo(&sTradeRecv.TradeItem[i], 2);
				sTradeRecv.TradeItem[i].y += T_REVISION_Y;
				return TRUE;

			}
		}
	}

	return FALSE;
}


/* ¼ÂÆÃ */
int cTRADE::CheckTradeItemSet()
{
	int i = 0;
	int TempWeight = 0;

	memcpy(&CheckItem, &cInvenTory.InvenItem, sizeof(sITEM) * 100); //°ø°£ Ã¼Å©¸¦ À§ÇØ¼­ º¹»ç 
	memcpy(&CheckRecvItem, &sTradeRecv.TradeItem, sizeof(sITEM) * MAX_TRADE_ITEM); //Ã¼Å©¸¦ À§ÇØ º¹»ç 

	////¹«°Ô °ü·Ã 
	for (i = 0; i < MAX_TRADE_ITEM; i++) {
		if (CheckRecvItem[i].Flag) {
			if (CheckRecvItem[i].Class == ITEM_CLASS_POTION)//¹°¾àÀÏ°æ¿ì´Â Æ÷¼Ç Ä«¿îÆ®¸¦ Á¤ÇØÁØ´Ù 
				TempWeight += CheckRecvItem[i].sItemInfo.PotionCount;
			else
				TempWeight += CheckRecvItem[i].sItemInfo.Weight;
		}
	}
	if (TempWeight + sinChar->Weight[0] > sinChar->Weight[1])
	{

		return FALSE;

	}
	for (i = 0; i < MAX_TRADE_ITEM; i++) {
		if (CheckRecvItem[i].Flag) {
			if (!TradeCheckEmptyArea(&CheckRecvItem[i]))
			{

				return FALSE;
			}
		}
	}

	//¼ÒÁöÇÒ¼öÀÖ´Â µ·À» ÃÊ°úÇßÀ»°æ¿ì 
	if (sTradeRecv.Money > 0) {
		if (!cInvenTory.CheckMoneyLimit(sTradeRecv.Money - 193))
			return FALSE;

	}

	return TRUE;
}

//ÀÓ½Ã ¼ÂÆÃ 
int cTRADE::TradeCheckEmptyArea(sITEM* pItem)
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect = { 0,0,0,0 };
	int		flag = 0;

	int StartX = 23;  //ÀÎº¥ ¹Ú½ºÀÇ ½ÃÀÛÁ¡ X (Æ®·¹ÀÌµå ¹Ú½º°¡ ¾Æ´Ô!!!!)
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - pItem->w; //¾ÆÀÌÅÛÀÌ µé¾î°¥¼öÀÖ´Â °¡Àå ³¡ XÁÂÇ¥ 
	cy = (22 * INVENTORY_BOX_COL) - pItem->h; //¾ÆÀÌÅÛÀÌ µé¾î°¥¼öÀÖ´Â °¡Àå ³¡ YÁÂÇ¥

	//ÄÚµå°¡ °°Áö ¾Ê°Å³ª ¾ÆÀÌÅÛÀÌ ºñ¾îÀÖÁö ¾ÊÀº °÷¿¡ ¼ÂÆÃÇÒ¶§ °Ë»öÇØ¼­ À§Ä¡¸¦ Ã£¾Æ³½´Ù 
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
				if (ChecketInvenItem(pItem)) //¿ÀÅä¼ÂÆÃÀÏ°æ¿ì¿¡´Â ÀÎÀÚ1À» ³Ñ±ä´Ù 
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
			pItem->Flag = 0; //ÃÊ±âÈ­ 
			return TRUE;

		}

	return FALSE;
}

//ÀÎº¥Åä¸®·Î ¾ÆÀÌÅÛÀ» ¼ÂÆÃÇÑ´Ù 
int cTRADE::CheckOkTradeItem()
{
	int i = 0;
	for (i = 0; i < MAX_TRADE_ITEM; i++) {
		if (sTradeRecv.TradeItem[i].Flag) {
			if (cInvenTory.CheckRequireItemToSet(&sTradeRecv.TradeItem[i])) {         //°ğ¹Ù·Î ¼ÂÆÃµÉ¼öÀÖ´ÂÁö¸¦ Ã¼Å©ÇÑ´Ù  
				if (!cInvenTory.AutoSetInvenItem(&sTradeRecv.TradeItem[i])) {
					if (sinThrowItemToFeild(&sTradeRecv.TradeItem[i])) {//¾ÆÀÌÅÛÀ» ¹ö¸°´Ù 
						sTradeRecv.TradeItem[i].Flag = 0;
					}

				}
			}
		}
	}
	if (sTradeRecv.Money > 0) {
		CheckCharForm();//ÀÎÁõ 
		//sinChar->Money += sTradeRecv.Money-193;
		sinPlusMoney(sTradeRecv.Money - 193);
		ReformCharForm();//ÀçÀÎÁõ 	
		SendSaveMoney(); //±İ¾× Á¶ÀÛÀ» ¸øÇÏ°ÔÇÏ±âÀ§ÇØ È£ÃâÇÑ´Ù 
		//sinPlaySound(SIN_SOUND_COIN);
	}
	cTrade.OpenFlag = FALSE;				//Æ®·¹ÀÌµå ¼º°ø½Ã Ã¢À» ´İ´Â´Ù 
	TempShowMoney = 0;
	ItemPickUpFlag = 0; //ÇÃ·¢ ÃÊ±âÈ­ 

	cInvenTory.CheckWeight();	//ÀÎº¥Åä¸® ÀÇ ¹«°Ô¸¦ º¸Á¤ÇÑ´Ù 

	return TRUE;
}


//Æ®·¹ÀÌµå Ãë¼Ò½Ã ¾ÆÀÌÅÛÀ»  ÀÎº¥Åä¸®·Î ¼ÂÆÃÇÑ´Ù 
int cTRADE::CancelTradeItem()
{
	int sArrowPosi = 0;

	if (TradeItemSucessFlag) {
		memset(&sTrade, 0, sizeof(sTRADE));		//ºñÁ¤»óÀûÀÎ Æ®·¹ÀÌµå Á¾·á½Ã ¾ÆÀÌÅÛÀ» ³¯¸°´Ù 
		memset(&sTradeRecv, 0, sizeof(sTRADE));

	}
	int i = 0;
	for (i = 0; i < MAX_TRADE_ITEM; i++) {
		if (sTrade.TradeItem[i].Flag) {
			if (cInvenTory.CheckRequireItemToSet(&sTrade.TradeItem[i])) { //°ğ¹Ù·Î ¼ÂÆÃµÉ¼öÀÖ´ÂÁö¸¦ Ã¼Å©ÇÑ´Ù  
				if (!cInvenTory.AutoSetInvenItem(&sTrade.TradeItem[i])) {
					//¾ÆÀÌÅÛÀÌ ²ËÂ÷ÀÖÀ»°æ¿ì´Â ÀÎº¥Åä¸®¸¦ µÚÁı¾î¼­ ¼ÂÆÃÇÑ´Ù
					if (ArrowState[0] == 0)sArrowPosi = 2; //¿ä±â´Â ÀÎº¥Ã¢°í 
					else sArrowPosi = 1;
					sinButtonFlag = sArrowPosi;
					SelectInvenItemIndex = 0; //¿¡Å×¸£ ÄÚ¾î³²¹ßÀ» ¸·±âÀ§ÇØ ÃÊ±âÈ­
					cInvenTory.ChangeABItem(sArrowPosi); // 2º¸´Ù ÀÛÀ»¶§ ÀÎº¥ ¹Ú½º 

					if (!cInvenTory.AutoSetInvenItem(&sTrade.TradeItem[i])) { //±×·¡µÎ ¾ÈµÇ¸é ¹ö¸°´Ù ÆÈÀÚ·Á´Ï ÇØ¶ó
						if (sinThrowItemToFeild(&sTrade.TradeItem[i])) {//¾ÆÀÌÅÛÀ» ¹ö¸°´Ù 
							sTrade.TradeItem[i].Flag = 0;
						}
					}

					/*
					//¿ä±â´Â ¹«±âÆ÷Áö¼Ç(ÇöÁ¦ »ç¿ëÇÏ°íÀÖÁö¾Ê´Ù)
					if(ArrowState[1]==0)sArrowPosi = 4;
					else sArrowPosi = 3;
					sinButtonFlag = sArrowPosi;
					sinChangeSetFlag = sArrowPosi;
					*/

					if (sinThrowItemToFeild(&sTrade.TradeItem[i])) {//¾ÆÀÌÅÛÀ» ¹ö¸°´Ù 
						sTrade.TradeItem[i].Flag = 0;
					}
				}
			}
		}
	}

	if (sTrade.Money > 0) {
		CheckCharForm();//ÀÎÁõ 
		//sinChar->Money += sTrade.Money-193;
		sinPlusMoney(sTrade.Money - 193);
		ReformCharForm();//ÀçÀÎÁõ 	
		SendSaveMoney(); //±İ¾× Á¶ÀÛÀ» ¸øÇÏ°ÔÇÏ±âÀ§ÇØ È£ÃâÇÑ´Ù 
	}


	//¸¶¿ì½º¿¡ ¾ÆÀÌÅÛÀÌ ÀÖÀ»°æ¿ì¿¡´Â ¸¶¿ì½º ¾ÆÀÌÅÛµµ ÀÎº¥¿¡ °ğ¹Ù·Î ¼ÂÆÃÇÑ´Ù 
	if (MouseItem.Flag) {
		if (cInvenTory.CheckRequireItemToSet(&MouseItem)) {         //°ğ¹Ù·Î ¼ÂÆÃµÉ¼öÀÖ´ÂÁö¸¦ Ã¼Å©ÇÑ´Ù  
			if (!cInvenTory.AutoSetInvenItem(&MouseItem)) {
				if (sinThrowItemToFeild(&MouseItem)) {//¾ÆÀÌÅÛÀ» ¹ö¸°´Ù 
					MouseItem.Flag = 0;
				}
			}
		}

	}

	//¸Ş¼¼Áö ¹Ú½º °ü·ÃÀº Àá½Ã ´ë±â 


	//Æ®·¹ÀÌµå Ãë¼Ò½Ã Ã¢À» ´İ¾ÆÁØ´Ù
	cTrade.OpenFlag = FALSE;


	memset(&sTrade, 0, sizeof(sTRADE));		//¼ÂÆÃµÈÈÄ °ªµéÀ» ÃÊ±âÈ­ÇØÁØ´Ù 
	memset(&sTradeRecv, 0, sizeof(sTRADE));	//¼ÂÆÃµÈÈÄ °ªµéÀ» ÃÊ±âÈ­ÇØÁØ´Ù 
	TempShowMoney = 0;
	ItemPickUpFlag = 0; //ÇÃ·¢ ÃÊ±âÈ­ 

	CloseEachPlayer();
	cInvenTory.CheckWeight();	//ÀÎº¥Åä¸® ÀÇ ¹«°Ô¸¦ º¸Á¤ÇÑ´Ù
	return TRUE;
}

//¾ÆÀÌÅÛ ÀÌ¹ÌÁö¸¦ ·ÎµåÇÑ´Ù 
int cTRADE::LoadTradeItemIamge() { return TRUE; }

int cTRADE::CheckTradeButtonOk()
{
	///////////////Æ®·¹ÀÌµå Ã¼Å© 
	if (!TradeSendSucessFlag && sTrade.CheckFlag && sTradeRecv.CheckFlag) { //±³È¯! 
		SendTradeCheckItem(TradeCharCode);
		TradeSendSucessFlag = TRUE;
	}

	return TRUE;
}

//¹«°Ô¸¦ ¾ò¾î¿Â´Ù 
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

/*******************************************************************************************/
/*							¿©±â¼­ ºÎÅÍ´Â Ã¢°í
/*******************************************************************************************/

//¾ÆÀÌÅÛÀÌ ¼ÂÆÃµÉ °ø°£À» Ã£´Â´Ù 
int cWAREHOUSE::SetWareHouseItemAreaCheck(sITEM* pItem)
{
	int i, j;
	TradeColorIndex = 0; //ÃÊ±âÈ­
	TradeCrashItemIndex[0] = 0; //Ãæµ¹µÈ ¾ÆÀÌÅÛ Index ÃÊ±âÈ­ 
	cTrade.InitTradeColorRect(); //ÃÊ±âÈ­

	TradeStartX = 21;
	TradeStartY = 136 + sinInterHeight2;
	TradeEndX = TradeStartX + (22 * 9);
	TradeEndY = TradeStartY + (22 * 9);

	for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22) {
		for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22) {
			if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j) { //ÀÎº¥Åä¸® ¹Ú½º¿¡ µé¾î°¬´ÂÁö¸¦ Ã¼Å© 
				TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
				TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
				TradeColorRect.right = pItem->w;
				TradeColorRect.bottom = pItem->h;
				if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
					cTrade.InitTradeColorRect(); //ÄÃ·¯ ¹Ú½º RECT ÃÊ±âÈ­ (ÄÃ·¯ ¹Ú½º RECTÁß ÇÏ³ª¶óµÎ 0 ÀÌ¸é ¼ÂÆÃÇÒ¼ö ¾ø´Ù 
					return FALSE;
				}
				//Äù½ºÆ® ¾ÆÀÌÅÛÀÏ°æ¿ì¿¡´Â ¾ÆÀÌÅÛÀ» Ã¢°í¿¡ ³ÖÀ»¼ö¾öµû 
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
				//µî·ÏµÈ ¾ÆÀÌÅÛÀº Ã¢°í¿¡ ³ÖÀ»¼ö¾ø´Ù
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
				//////////// ÀÌº¥Æ® ¼ÛÆí
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
				pItem->ItemPosition = 0; //¹è¿­ÀÇ ÀÎµ¦½º¸¦ ÀúÀåÇÑ´Ù 
				TradeColorIndex = SET_ITEM_CHECK_COLOR; //¼ÂÆÃÇÒ¿µ¿ª

				if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 1)) {  //°ãÄ¡´Â ¾ÆÀÌÅÛÀÌ ÀÖ³ª Ã¼Å© 
					//¿ä±â¼­ º¹»ç¾ÆÀÌÅÛÀ» Ã¼Å©ÇÑ´Ù 
					if (!CopyItemNotPickUp(&sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1], TradeCrashItemIndex[0] - 1)) {
						TradeColorIndex = NOT_SETTING_COLOR;
						TradeCrashItemIndex[0] = 0; //ÃÊ±âÈ­ 
						TradeCrashItemIndex[1] = 0; //ÃÊ±âÈ­ 
						return FALSE;

					}
					if (TradeCrashItemIndex[1] = cTrade.CrashTradeItem(TradeColorRect, TradeCrashItemIndex[0], 1)) {  //°ãÄ¡´Â ¾ÆÀÌÅÛÀÌ ÀÖ³ª Ã¼Å© 
						TradeColorIndex = NOT_SETTING_COLOR;
						TradeCrashItemIndex[1] = 0; //ÃÊ±âÈ­ 
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


//¾ÆÀÌÅÛÀ» ¼ÂÆÃÇÑ´Ù 
int cWAREHOUSE::LastSetWareHouseItem(sITEM* pItem)
{
	CheckWareHouseForm();

	for (int j = 0; j < 100; j++) {
		if (!sWareHouse.WareHouseItem[j].Flag) {
			memcpy(&sWareHouse.WareHouseItem[j], pItem, sizeof(sITEM));
			sWareHouse.WareHouseItem[j].x = pItem->SetX;
			sWareHouse.WareHouseItem[j].y = pItem->SetY;
			pItem->Flag = 0; //ÃÊ±âÈ­ 
			//sinPlaySound(sWareHouse.WareHouseItem[j].SoundIndex);
			GetWeight(); //¹«°Ô¸¦ Ã¼Å©ÇÑ´Ù 
			ReFormWareHouse();
			return TRUE;

		}
	}
	ReFormWareHouse();
	return FALSE;
}

//Ã¢°í¸¦ ´İ´Â´Ù 
int cWAREHOUSE::CloseWareHouse()
{
	int RestoreFlag = 1;

	CloseWareHouseCheckFlag3 = 1;
	cWareHouse.OpenFlag = 0; //Ã¢°í¸¦ ´İ´Â´Ù 
	if (WareHouseSaveFlag) {
		if (!SaveWareHouse(&sWareHouse)) { //¾ÆÀÌÅÛÀ» ÀúÀå½ÃÅ²´Ù 
			cWareHouse.RestoreInvenItem(); //ÀúÀåÀÌ µÇÁö¾ÊÀ¸¸é º¹±¸ÇØÁØ´Ù 

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
	WareHouseSaveFlag = 0;//ÇÃ·¢ ÃÊ±âÈ­ 
	ResetInvenItemCode();		//ÀÌº¥Åä¸® ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­ ********************
	ResetInvenMoney();			//ÀÌº¥Åä¸® µ· °Ë»ç ÃÊ±âÈ­
	return RestoreFlag;
}



//¾ÆÀÌÅÛÀ» Áı´Â´Ù 
int cWAREHOUSE::PickUpWareHouseItem(int x, int y, int PickUpFlag)
{
	int i = 0;
	SelectTradeItemIndex = 0;    //ÁıÀ» ¾ÆÀÌÅÛ 
	for (i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			if (sWareHouse.WareHouseItem[i].x < x && sWareHouse.WareHouseItem[i].x + sWareHouse.WareHouseItem[i].w > x &&
				sWareHouse.WareHouseItem[i].y < y && sWareHouse.WareHouseItem[i].y + sWareHouse.WareHouseItem[i].h > y) {
				if (PickUpFlag) {

					//¼öÁ¤ ÇÏ´ë¿ë
					//Ã¢°íÀÌ¿ë½Ã ÀÎº¥Åä¸® ºó°ø°£ÀÌ ¾øÀ»°æ¿ì´Â ÀÌ¿ëÀÌ ºÒ°¡´ÉÇÏ´Ù/
					//°¡»óÀÇ °ø°£ Ã¼Å©¿ë ÀÓ½Ã ¾ÆÀÌÅÛ
					sITEM TempItem;
					TempItem.w = ITEMSIZE * 2;
					TempItem.h = ITEMSIZE * 4;
					TempItem.Flag = 1;
					if (cInvenTory.CheckSetEmptyArea(&TempItem)) {
						////////////¿ä±â¼­ º¹»çµÈ ¾ÆÀÌÅÛÀº µé¼ö°¡¾ø´Ù 
						if (CopyItemNotPickUp(&sWareHouse.WareHouseItem[i], i)) {
							CheckWareHouseForm();
							memcpy(&MouseItem, &sWareHouse.WareHouseItem[i], sizeof(sITEM));
							sWareHouse.WareHouseItem[i].Flag = 0;
							//sinPlaySound(sWareHouse.WareHouseItem[i].SoundIndex);
							TradeColorIndex = 0;
							GetWeight(); //¹«°Ô¸¦ Ã¼Å©ÇÑ´Ù 
							ReFormWareHouse();
							return TRUE;
						}
					}
					else
					{

						break;
					}

					////////////¿ä±â¼­ º¹»çµÈ ¾ÆÀÌÅÛÀº µé¼ö°¡¾ø´Ù 
					if (CopyItemNotPickUp(&sWareHouse.WareHouseItem[i], i)) {
						CheckWareHouseForm();
						memcpy(&MouseItem, &sWareHouse.WareHouseItem[i], sizeof(sITEM));
						sWareHouse.WareHouseItem[i].Flag = 0;
						//sinPlaySound(sWareHouse.WareHouseItem[i].SoundIndex);
						TradeColorIndex = 0;
						GetWeight(); //¹«°Ô¸¦ Ã¼Å©ÇÑ´Ù 
						ReFormWareHouse();
						return TRUE;
					}

				}
				else {
					SelectTradeItemIndex = i + 1;
					TradeColorIndex = SELECT_ITEM_COLOR;
					sinShowItemInfoFlag = 1; //¾ÆÀÌÅÛÀÇ Á¤º¸¸¦ º¸¿©ÁØ´Ù 
					cItem.ShowItemInfo(&sWareHouse.WareHouseItem[i], 2);
				}
			}
		}
	}
	return FALSE;
}

//°ãÄ£ ¾ÆÀÌÅÛÀ» ±³È¯ÇÑ´Ù 
int cWAREHOUSE::ChangeWareHouseItem(sITEM* pItem)
{
	if (pItem->Class == ITEM_CLASS_POTION) {	//¹°¾àÀº Ã¢°í¿¡ º¸°üÇÒ¼ö¾ø´Ù 

		return TRUE;

	}
	if (pItem->sItemInfo.Weight + (sWareHouse.Weight[0] - 197) > sWareHouse.Weight[1] - 196) {

		return TRUE;

	}

	CheckWareHouseForm();

	if (TradeCrashItemIndex[0]) {//Ãæµ¹ µÈ ¾ÆÀÌÅÛÀÌ ÀÖÀ¸¸é ¾ÆÀÌÅÛÀ» ¹Ù²ãÁØ´Ù 
		if (sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1].Class == ITEM_CLASS_POTION) {
			if (sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1].CODE == pItem->CODE) {
				sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1].sItemInfo.PotionCount += pItem->sItemInfo.PotionCount;
				pItem->Flag = 0;
				GetWeight();
				ReFormWareHouse();
				return TRUE;

			}
		}
		memcpy(&TempItem, &sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1], sizeof(sITEM)); //¸¶¿ì½º ¾ÆÀÌÅÛÀ» ÅÛÇÁ·Î º¹»ç 
		sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1].Flag = 0; //º¹»çÇÑÈÄ¿¡ ÃÊ±âÈ­
		TempItem.x = pItem->x;
		TempItem.y = pItem->y;
		ReFormWareHouse();
		LastSetWareHouseItem(pItem); //¸¶¿ì½º ¾ÆÀÌÅÛÀ» ÀÎº¥Åä¸®·Î º¹»ç 
		memcpy(pItem, &TempItem, sizeof(sITEM));
		GetWeight();
		ReFormWareHouse();
		return TRUE;

	}
	ReFormWareHouse();
	return FALSE;
}

//¹«°Ô¸¦ ±¸ÇÑ´Ù 
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
	WareHouseSaveFlag = 1; //¾ÆÀÌÅÛÀÌ ¹Ù²î°Ô µÇ¸é °ªÀ» ¼ÂÆÃÇØÁØ´Ù 	

	return TRUE;
}

//¾ÆÀÌÅÛÀÇ ÀÌ¹ÌÁö¿Í ¾ÆÀÌÅÛ Å©±â¸¦ ¼ÂÆÃÇÑ´Ù 
int cWAREHOUSE::LoadWareHouseItemIamge() { return TRUE; }

//ÀÎº¥Åä¸® ¾ÆÀÌÅÛÀ» Àá½Ã ¹é¾÷ÇÑ´Ù 
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


//¾ÆÀÌÅÛÀ» º¹±¸ÇØÁØ´Ù 
int cWAREHOUSE::RestoreInvenItem()
{
	//º¹±¸ÇÏ±âÀü¿¡ ÇöÀç Âø¿ëÁßÀÎ ¾ÆÀÌÅÛÀ» ¹ş´Â´Ù (¾ÈÇØµÎ µÇ´Âµ¥ °Á ÇÑ´Ù )
	sinSetCharItem(cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[0].ItemIndex - 1].SetModelPosi, FALSE);
	sinSetCharItem(cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[1].ItemIndex - 1].SetModelPosi, FALSE);
	sinSetCharItem(cInvenTory.InvenItem[sInven[2].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[2].ItemIndex - 1].SetModelPosi, FALSE);

	memcpy(&cInvenTory.InvenItem, BackUpInvenItem2, sizeof(sITEM) * 100);
	memcpy(&cInvenTory.InvenItemTemp, BackUpInvenTempItem2, sizeof(sITEM) * 100);
	memcpy(&sInven, BackUpsInven, sizeof(sINVENTORY) * INVENTORY_MAX_POS);

	//¸¶¿ì½º ¾ÆÀÌÅÛÀ» ¾ø¾ÖÁØ´Ù
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

	if (sInven[0].ItemIndex) { //¹«±â 
		sinSetCharItem(cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[0].ItemIndex - 1].SetModelPosi, TRUE);

	}

	if (sInven[1].ItemIndex) { //¹æÆĞ 
		sinSetCharItem(cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[1].ItemIndex - 1].SetModelPosi, TRUE);

	}

	if (sInven[2].ItemIndex) { //°©¿Ê 
		sinSetCharItem(cInvenTory.InvenItem[sInven[2].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[2].ItemIndex - 1].SetModelPosi, TRUE);

	}
	if ((InvenGold - 2023) == (RealGold / 3) - 2023) {
		CheckCharForm();//ÀÎÁõ 
		sinChar->Money = InvenGold - 2023;
		ReformCharForm();//ÀçÀÎÁõ 	

	}
	else {
		SendSetHackUser(6); //ÇØÅ·À» ÇÏ·Á°íÇß´ø ¸øµÈ À¯Àú¸¦ °í¹ß TRUE Á¢¼Ó Á¾·á 

	}
	InvenGold = 0;
	RealGold = 0;

	cInvenTory.SetItemToChar(); //ÆÄ¶ó¸ŞÅÍ ¼ÂÆÃ 
	cInvenTory.ReFormInvenItem(); //¾ÆÀÌÅÛ ÀÎÁõÀ» ´Ù½Ã ¹Ş´Â´Ù 
	cInvenTory.CheckWeight();	  //¹«°Ô ¼ÂÆÃ 	

	return TRUE;
}

/*****************************************************************************/
/*							¾ÆÀÌÅÛ Á¶ÇÕ										 */
/*****************************************************************************/
//¾ÆÀÌÅÛÀÌ µé¾î°¥¼öÀÖ´ÂÁö¸¦ Ã¼Å©ÇÑ´Ù 
int cCRAFTITEM::SetCraftItemAreaCheck(sITEM* pItem, int Kind)
{

	int i, j;
	TradeColorIndex = 0; //ÃÊ±âÈ­   (Æ®·¹ÀÌµå Ã¢°ú Ã¢°í°¡ °°ÀÌ ¾´´Ù) 
	TradeCrashItemIndex[0] = 0; //Ãæµ¹µÈ ¾ÆÀÌÅÛ Index ÃÊ±âÈ­ 
	cTrade.InitTradeColorRect(); //ÃÊ±âÈ­

	// ¹ÚÀç¿ø - ¸®½ºÆå ½ºÅæ(¸®½ºÆå(¾ÆÀÌÅÛ Àç±¸¼º)Ã¢¿¡¼­ ¸®½ºÆå ½ºÅæ ÀÚ¸®¿¡ ¾ÆÀÌÅÛÀ» ³ÖÀ» ¶§ ¸¶¿ì½º¿¡ ´Ş·ÁÀÖ´Â »óÅÂ)
	if (pItem->sItemInfo.CODE == (sinBI1 | sin90))
	{
		int ReconStoneX = 159 + 11, ReconStoneY = 268 + 11 + sinInterHeight2;
		TradeColorIndex = 0; //ÃÊ±âÈ­   (Æ®·¹ÀÌµå Ã¢°ú Ã¢°í°¡ °°ÀÌ ¾´´Ù)
		if (ReconStoneX <= pItem->x + 22 && ReconStoneX > pItem->x && ReconStoneY < pItem->y + 22 && ReconStoneY > pItem->y)
		{
			pItem->SetX = ReconStoneX + (((pItem->x + 22 - ReconStoneX) / 22) * 22) - 11;
			pItem->SetY = ReconStoneY + (((pItem->y + 22 - ReconStoneY) / 22) * 22) - 11;
			pItem->ItemPosition = 0; //¹è¿­ÀÇ ÀÎµ¦½º¸¦ ÀúÀåÇÑ´Ù 
			TradeColorIndex = SET_ITEM_CHECK_COLOR; //¼ÂÆÃÇÒ¿µ¿ª
			if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
				cTrade.InitTradeColorRect(); //ÄÃ·¯ ¹Ú½º RECT ÃÊ±âÈ­ (ÄÃ·¯ ¹Ú½º RECTÁß ÇÏ³ª¶óµÎ 0 ÀÌ¸é ¼ÂÆÃÇÒ¼ö ¾ø´Ù 
				return FALSE;
			}
			if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 2)) {  //°ãÄ¡´Â ¾ÆÀÌÅÛÀÌ ÀÖ³ª Ã¼Å© 
				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;
			}
			return TRUE;
		}
	}

	if ((pItem->CODE & sinITEM_MASK2) == sinOS1 || (pItem->CODE & sinITEM_MASK2) == sinSE1) { //½©ÅÒÀÏ °æ¿ì || ¾ÆÀÌÅÛ Àç±¸¼º ¾Á 
		TradeStartX = 218;
		TradeStartY = 193 + sinInterHeight2;
		TradeEndX = TradeStartX + (3 * 22);
		TradeEndY = TradeStartY + (4 * 22);

		for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22) {
			for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22) {
				if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j) { //ÀÎº¥Åä¸® ¹Ú½º¿¡ µé¾î°¬´ÂÁö¸¦ Ã¼Å© 
					TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
					TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
					TradeColorRect.right = pItem->w;
					TradeColorRect.bottom = pItem->h;
					if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
						cTrade.InitTradeColorRect(); //ÄÃ·¯ ¹Ú½º RECT ÃÊ±âÈ­ (ÄÃ·¯ ¹Ú½º RECTÁß ÇÏ³ª¶óµÎ 0 ÀÌ¸é ¼ÂÆÃÇÒ¼ö ¾ø´Ù 
						return FALSE;
					}

					pItem->SetX = TradeColorRect.left;
					pItem->SetY = TradeColorRect.top;
					pItem->ItemPosition = 0; //¹è¿­ÀÇ ÀÎµ¦½º¸¦ ÀúÀåÇÑ´Ù 
					TradeColorIndex = SET_ITEM_CHECK_COLOR; //¼ÂÆÃÇÒ¿µ¿ª

					//Æ÷½º ¿Àºê
					if (cCraftItem.ForceFlag)
					{
						//µ·ÀÌ ºÎÁ·ÇÒ °æ¿ì 
						//--------------------------------------------------------------------------//



						// Kyle ÀÌ´Ï±×¸¶±îÁö »ç¿ë
						if (!smConfig.DebugMode) {
							if ((pItem->sItemInfo.CODE & sinITEM_MASK3) >= sin12) {
								TradeColorIndex = NOT_SETTING_COLOR;
								return FALSE;
							}
						}


						/*
						// pluto ¼¿·¹½ºÅä±îÁö »ç¿ë
						if(!smConfig.DebugMode){
							if((pItem->sItemInfo.CODE & sinITEM_MASK3) >= sin10){
								TradeColorIndex = NOT_SETTING_COLOR;
								return FALSE;
							}
						}
						*/
						/*
						//µ¥ºñ³×±îÁö »ç¿ë
						if(!smConfig.DebugMode)
						{//gm¸ğµå°¡ µÇ´Â°Ç gm¸ğµå¸¸ µÇ°ÔÇÑ°Å´Ù. À¯Àú¸ğµå´Â ¾ÈµÇ´Â°Å´Ù!
							if((pItem->sItemInfo.CODE & sinITEM_MASK3) >= sin09)
							{
								TradeColorIndex = NOT_SETTING_COLOR;
								return FALSE;
							}
						}
						*/
					}

					//Äù½ºÆ® ¾ÆÀÌÅÛÀÏ °æ¿ì¿¡´Â ¾ÆÀÌÅÛÀ» ¼ÂÆÃÇÒ¼ö ¾öµû 
					if (pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinQW1 ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinPZ1 || //ÆÛÁñµµ ¿Ã¸±¼ö¾öµû
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinPZ2 ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinFO1 ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinSP1 || // pluto ¼±¹°»óÀÚ ¼ÛÆí... °è¿­
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinWR1 || // ¹ÚÀç¿ø - ¹«±â Á¦ÀÛ¼­ °è¿­
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinDR1 || // ¹ÚÀç¿ø - ¹æ¾î±¸ Á¦ÀÛ¼­ °è¿­
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinGP1 || // ¹ÚÀç¿ø - Å©¸®½ºÅ» °è¿­
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinCH1 || // ¹ÚÀç¿ø - ÃÊÄİ¸´, »çÅÁ °è¿­
						(pItem->sItemInfo.CODE >= (sinOR2 | sin06) && pItem->sItemInfo.CODE <= (sinOR2 | sin25)) || // ¹ÚÀç¿ø : Å¬·£Ä¡ÇÁ¸µ °è¿­
						pItem->sItemInfo.SpecialItemFlag[0] == CHECK_GIVE_ITEM ||
						pItem->sItemInfo.CODE == (sinGF1 | sin01) ||
						pItem->sItemInfo.CODE == (sinGF1 | sin02) ||
						pItem->sItemInfo.CODE == (sinGF1 | sin03) ||
						pItem->sItemInfo.CODE == (sinGF1 | sin04) ||
						pItem->sItemInfo.CODE == (sinGF1 | sin05) ||
						pItem->sItemInfo.CODE == (sinGF1 | sin06) ||
						pItem->sItemInfo.CODE == (sinOR2 | sin27) || // ¹ÚÀç¿ø - »êÅ¸ ¸µ Ãß°¡
						pItem->sItemInfo.CODE == (sinOA1 | sin32) || // ¹ÚÀç¿ø - »êÅ¸ ¾Æ¹Ä·¿ Ãß°¡
						pItem->sItemInfo.CODE == (sinOR2 | sin28) || // ¹ÚÀç¿ø - ÀÌº¥Æ® ¸µ(7ÀÏ) Ãß°¡
						pItem->sItemInfo.CODE == (sinOA1 | sin33) || // ¹ÚÀç¿ø - ÀÌº¥Æ® ¾Æ¹Ä·¿(7ÀÏ) Ãß°¡
						pItem->sItemInfo.CODE == (sinOR2 | sin29) || // ¹ÚÀç¿ø - ÀÌº¥Æ® ¸µ(1½Ã°£) Ãß°¡
						pItem->sItemInfo.CODE == (sinOA1 | sin34) || // ¹ÚÀç¿ø - ÀÌº¥Æ® ¾Æ¹Ä·¿(1½Ã°£) Ãß°¡
						pItem->sItemInfo.CODE == (sinOR2 | sin30) || // ¹ÚÀç¿ø - ÀÌº¥Æ® ¸µ(1ÀÏ) Ãß°¡
						pItem->sItemInfo.CODE == (sinOA1 | sin35) || // ¹ÚÀç¿ø - ÀÌº¥Æ® ¾Æ¹Ä·¿(1ÀÏ) Ãß°¡
						pItem->sItemInfo.CODE == (sinOA2 | sin31) || // ¹ÚÀç¿ø - ½´ÆÛ ¾Ï¸´(7ÀÏ) Ãß°¡
						pItem->sItemInfo.CODE == (sinOA2 | sin32) || // ¹ÚÀç¿ø - ½´ÆÛ ¾Ï¸´(30ÀÏ) Ãß°¡
						pItem->sItemInfo.CODE == (sinDB1 | sin31) || // ¹ÚÀç¿ø - ½ºÇÇµå ºÎÃ÷(7ÀÏ) Ãß°¡
						pItem->sItemInfo.CODE == (sinDB1 | sin32) || // ¹ÚÀç¿ø - ½ºÇÇµå ºÎÃ÷(30ÀÏ) Ãß°¡
						pItem->sItemInfo.CODE == (sinSP1 | sin15) || // ¹ÚÀç¿ø - ¼ö¹Ú ¾ÆÀÌÅÛ Ãß°¡
						pItem->sItemInfo.CODE == (sinSP1 | sin27) || // ¹ÚÀç¿ø - ¾ËÆÄºª Á¶ÇÕ ÀÌº¥Æ® Ãß°¡
						pItem->sItemInfo.CODE == (sinSP1 | sin28) || // ¹ÚÀç¿ø - ¾ËÆÄºª Á¶ÇÕ ÀÌº¥Æ® Ãß°¡
						pItem->sItemInfo.CODE == (sinSP1 | sin29) || // ¹ÚÀç¿ø - ¾ËÆÄºª Á¶ÇÕ ÀÌº¥Æ® Ãß°¡
						pItem->sItemInfo.CODE == (sinSP1 | sin30) || // ¹ÚÀç¿ø - ¾ËÆÄºª Á¶ÇÕ ÀÌº¥Æ® Ãß°¡
						pItem->sItemInfo.CODE == (sinSP1 | sin31) || // ¹ÚÀç¿ø - ¾ËÆÄºª Á¶ÇÕ ÀÌº¥Æ® Ãß°¡
						pItem->sItemInfo.CODE == (sinSP1 | sin32) || // ¹ÚÀç¿ø - ¾ËÆÄºª Á¶ÇÕ ÀÌº¥Æ® Ãß°¡
						pItem->sItemInfo.CODE == (sinSP1 | sin33) || // ¹ÚÀç¿ø - ¾ËÆÄºª Á¶ÇÕ ÀÌº¥Æ® Ãß°¡
						pItem->sItemInfo.CODE == (sinOR2 | sin31) || // ¹ÚÀç¿ø - º¸½º ¸ó½ºÅÍ ¸µ Ãß°¡(¹Ùº§)
						pItem->sItemInfo.CODE == (sinOR2 | sin32) ||  // ¹ÚÀç¿ø - º¸½º ¸ó½ºÅÍ ¸µ Ãß°¡(Ç»¸®)
						pItem->sItemInfo.CODE == (sinSP1 | sin35) || // Àåº° - ¹ß·»Å¸ÀÎ ÃÊÄİ¸´ ¾ÆÀÌÅÛ Ãß°¡
						pItem->sItemInfo.CODE == (sinOR2 | sin33) ||   // Àåº° - ÇÏÆ®¸µ(7ÀÏ) ¾ÆÀÌÅÛ Ãß°¡
						pItem->sItemInfo.CODE == (sinOA1 | sin36) || // Àåº° - ´«²É ¸ñ°ÉÀÌ(7ÀÏ) ¾ÆÀÌÅÛ Ãß°¡
						pItem->sItemInfo.CODE == (sinSP1 | sin36) || // Àåº° - Äµµğµ¥ÀÌÁî Äµµğ ¾ÆÀÌÅÛ Ãß°¡
						pItem->sItemInfo.CODE == (sinOA1 | sin37) ||   // Àåº° - ÇÏÆ®¾Æ¹Ä·¿(7ÀÏ) ¾ÆÀÌÅÛ Ãß°¡
						pItem->sItemInfo.CODE == (sinOA2 | sin33) || // Àåº° - ½´ÆÛ ¾Ï¸´(1ÀÏ)
						pItem->sItemInfo.CODE == (sinDB1 | sin33) || // Àåº° - ½ºÇÇµå ºÎÃ÷(1ÀÏ) Ãß°¡
						pItem->sItemInfo.CODE == (sinSP1 | sin34) || // ¹ÚÀç¿ø - È£¶ûÀÌ Ä¸½¶ Ãß°¡
						pItem->sItemInfo.CODE == (sinSP1 | sin37) || // Àåº° - ¸ÅÁöÄÃ±×¸° ºñÃë ¾ÆÀÌÅÛ Ãß°¡
						pItem->sItemInfo.CODE == (sinSP1 | sin38) || // Àåº° - ¸ÅÁöÄÃ±×¸° ¿¡¸Ş¶öµå ¾ÆÀÌÅÛ Ãß°¡
						pItem->sItemInfo.CODE == (sinBI1 | sin84) || // Àåº° - ±×¶óºñÆ¼ ½ºÅ©·Ñ Ãß°¡
						pItem->sItemInfo.CODE == (sinDB1 | sin34) || // Àåº° - ½ºÇÇµå ºÎÃ÷(1½Ã°£) Ãß°¡
						pItem->sItemInfo.CODE == (sinOA2 | sin34) || // Àåº° - ½´ÆÛ ¾Ï¸´(1½Ã°£)
						pItem->sItemInfo.CODE == (sinSP1 | sin39) || // Àåº° - Ä«¶óÀÇ ´«¹° ¾ÆÀÌÅÛ Ãß°¡
						pItem->sItemInfo.CODE == (sinGF1 | sin07) || // Àåº° - Á¶»ç¿øÀ» Ã£¾Æ¶ó
						pItem->sItemInfo.CODE == (sinGF1 | sin08) ||	// Àåº° - Á¶»ç¿øÀ» Ã£¾Æ¶ó
						pItem->sItemInfo.CODE == (sinOR2 | sin36) || // Àåº° - ¼Ò¿ï½ºÅæ
						pItem->sItemInfo.CODE == (sinOR2 | sin37) ||
						pItem->sItemInfo.CODE == (sinOR2 | sin38) ||
						pItem->sItemInfo.CODE == (sinOR2 | sin39) ||
						pItem->sItemInfo.CODE == (sinOR2 | sin40) ||
						pItem->sItemInfo.CODE == (sinOA1 | sin39) ||
						pItem->sItemInfo.CODE == (sinOA1 | sin40) ||
						pItem->sItemInfo.CODE == (sinOA1 | sin41) ||
						pItem->sItemInfo.CODE == (sinOA1 | sin42) ||
						pItem->sItemInfo.CODE == (sinOA1 | sin38) || // Àåº° - º¹³¯ ÀÌº¥Æ® ¾Æ¹Ä·¿, ¸µ 
						pItem->sItemInfo.CODE == (sinOR2 | sin34) ||
						pItem->sItemInfo.CODE == (sinOR2 | sin35))


					{
						TradeColorIndex = NOT_SETTING_COLOR;
						return FALSE;

					}

					// ¹ÚÀç¿ø - ¸ÅÁ÷ ½©ÅÒ Á¶ÇÕ ±İÁö
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
					//µî·ÏµÈ ¾ÆÀÌÅÛÀº ¹Í½ºÇÒ¼ö¾ø´Ù
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

					if (Kind == 0) { //¹Í½ºÃÄ 
						if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 2)) {  //°ãÄ¡´Â ¾ÆÀÌÅÛÀÌ ÀÖ³ª Ã¼Å© 
							TradeColorIndex = NOT_SETTING_COLOR;
							return FALSE;

						}
						return TRUE;

					}
					if (Kind == 1) { //¿¡ÀÌÂ¡ 
						if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //°ãÄ¡´Â ¾ÆÀÌÅÛÀÌ ÀÖ³ª Ã¼Å© 
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
		//Æ÷½º ¿Àºê
		if (cCraftItem.ForceFlag)return FALSE;
		TradeColorIndex = 0; //ÃÊ±âÈ­   (Æ®·¹ÀÌµå Ã¢°ú Ã¢°í°¡ °°ÀÌ ¾´´Ù)
		TradeCrashItemIndex[0] = 0; //Ãæµ¹µÈ ¾ÆÀÌÅÛ Index ÃÊ±âÈ­ 
		cTrade.InitTradeColorRect(); //ÃÊ±âÈ­

		TradeStartX = 58;
		TradeStartY = 193 + sinInterHeight2;
		TradeEndX = TradeStartX + (3 * 22);
		TradeEndY = TradeStartY + (4 * 22);

	}
	return TRUE;
}

//¾ÆÀÌÅÛÀ» ¼ÂÆÃÇÑ´Ù 
// ¹ÚÀç¿ø - Æ÷½º ¿Àºê Á¦ÀÛ ºñ¿ë ¼öÁ¤
int ForceOrbPriceIndex[] = { 200,500,1000,2000,4000,5000,10000,20000,30000,40000,50000,60000,70000,80000 };  // ¹ÚÀç¿ø - º§·ë, ¿À¸£µµ Æ÷½º ºñ¿ë Ãß°¡
// ¹ÚÀç¿ø - ¸ÅÁ÷ Æ÷½º Ãß°¡(¸ÅÁ÷ Æ÷½º ¿ÀÇÁ Á¦ÀÛ ºñ¿ë)
int MagicForceOrbPriceIndex[] = { 200,500,1000,2000,4000,5000,10000,20000,30000,40000,50000,60000,70000,80000 };

DWORD SheltomCode2[] = { sin01,sin02,sin03,sin04,sin05,sin06,sin07,sin08,sin09,sin10,sin11,sin12,sin13,sin14,sin15 }; // ¹ÚÀç¿ø - º§·ë, ¿À¸£µµ Ãß°¡ 

// ¹ÚÀç¿ø - ¸ÅÁ÷ Æ÷½º Ãß°¡
DWORD MagicSheltomCode[] = { sin21,sin22,sin23,sin24,sin25,sin26,sin27,sin28,sin29,sin30,sin31,sin32, sin33, sin34 };

// ¹ÚÀç¿ø - ºô¸µ ¸ÅÁ÷ Æ÷½º Ãß°¡
DWORD BillingMagicSheltomCode[] = { sin35, sin36, sin37 };

int cCRAFTITEM::CheckForceOrbPrice()
{
	ForceItemPrice = 0;
	for (int j = 1; j < 15; j++) {
		if (sCraftItem.CraftItem[j].Flag) {
			for (int t = 0; t < 14; t++) // ¹ÚÀç¿ø - º§·ë, ¿À¸£µµ Æ÷½º Ãß°¡
			{
				if ((sCraftItem.CraftItem[j].sItemInfo.CODE & sinITEM_MASK3) <= sin20)
				{
					if ((sCraftItem.CraftItem[j].sItemInfo.CODE & sinITEM_MASK3) == SheltomCode2[t])
					{
						ForceItemPrice += ForceOrbPriceIndex[t];
						break;
					}
				}
				else // ½©ÅÒ ÄÚµå sin21 ~
				{ // ¹ÚÀç¿ø - ¸ÅÁ÷ Æ÷½º Ãß°¡
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
int cCRAFTITEM::CheckForceOrbPrice(sITEM* pItem)
{

	int TempPrice = 0;
	for (int j = 1; j < 15; j++) {
		if (sCraftItem.CraftItem[j].Flag) {
			for (int t = 0; t < 14; t++) {
				if ((sCraftItem.CraftItem[j].sItemInfo.CODE & sinITEM_MASK3) == SheltomCode2[t]) {
					TempPrice += ForceOrbPriceIndex[t];
					break;
				}
				// ¹ÚÀç¿ø - ¸ÅÁ÷ Æ÷½º Ãß°¡
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
		// ¹ÚÀç¿ø - ¸ÅÁ÷ Æ÷½º Ãß°¡
		if ((pItem->sItemInfo.CODE & sinITEM_MASK3) == MagicSheltomCode[t]) {
			return TempPrice + MagicForceOrbPriceIndex[t];
		}
	}
	return FALSE;
}
int cCRAFTITEM::LastSetCraftItem(sITEM* pItem, int Kind)
{
	int TempLevel = 0;
	int TempLevel2 = 0;
	if (Kind == 0) { //Å©·¡ÇÁÆ® ¾ÆÀÌÅÛ 
		CheckCraftItemForm(); //Á¶ÀÛÇÏ´Â »ó³ğÀÇ ÀÚ½ÄµéÀ» Á×¿©¹ö¸®ÀÚ Æ¡!!
		if ((pItem->CODE & sinITEM_MASK2) == sinOS1) { //½©ÅÒÀÏ °æ¿ì 
			for (int j = 1; j < 15; j++) {
				if (!sCraftItem.CraftItem[j].Flag) {
					memcpy(&sCraftItem.CraftItem[j], pItem, sizeof(sITEM));
					sCraftItem.CraftItem[j].x = pItem->SetX;
					sCraftItem.CraftItem[j].y = pItem->SetY;
					pItem->Flag = 0; //ÃÊ±âÈ­ 
					//sinPlaySound(sCraftItem.CraftItem[j].SoundIndex);
					ReFormCraftItem();
					CheckForceOrbPrice();
					return TRUE;
				}
			}
		}
		//HaGoon¾ÆÀÌÅÛ Àç±¸¼º ===================================================================================================
		else if ((pItem->CODE & sinITEM_MASK2) == sinSE1) {
			if (sCraftItem.CraftItem[0].Flag) {
				TempLevel = sCraftItem.CraftItem[0].sItemInfo.Level;
				for (int k = 0; k < MAX_SEEL_COUNT; k++) { // ¹ÚÀç¿ø - Å×ÀÌ¿ÍÁî ¾Á Ãß°¡ (3Á¾·ù -> 4Á¾·ù)
					if (pItem->CODE == sReconItem[k].dwCODE) {
						if (!(TempLevel >= sReconItem[k].iLevel[0] && TempLevel <= sReconItem[k].iLevel[1])) {

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
					pItem->Flag = 0; //ÃÊ±âÈ­ 
					//sinPlaySound(sCraftItem.CraftItem[j].SoundIndex);
					ReFormCraftItem();
					return TRUE;
				}
			}

		}
		//=======================================================================================================================
		// ¹ÚÀç¿ø - ¸®½ºÆå ½ºÅæ
		else if ((pItem->CODE & sinITEM_MASK2) == sinBI1)
		{
			if ((pItem->CODE & sinITEM_MASK3) == sin90)
			{
				if (!sCraftItem.CraftItem[14].Flag) {
					memcpy(&sCraftItem.CraftItem[14], pItem, sizeof(sITEM));
					sCraftItem.CraftItem[14].x = pItem->SetX;
					sCraftItem.CraftItem[14].y = pItem->SetY;
					pItem->Flag = 0; //ÃÊ±âÈ­ 
					//sinPlaySound(sCraftItem.CraftItem[14].SoundIndex);
					return TRUE;
				}
			}
		}
		else {
			if (!sCraftItem.CraftItem[0].Flag) {
				memcpy(&sCraftItem.CraftItem[0], pItem, sizeof(sITEM));
				sCraftItem.CraftItem[0].x = pItem->SetX;
				sCraftItem.CraftItem[0].y = pItem->SetY;
				pItem->Flag = 0; //ÃÊ±âÈ­ 
				//sinPlaySound(sCraftItem.CraftItem[0].SoundIndex);
				ReFormCraftItem();
				return TRUE;
			}

		}
	}

	if (Kind == 1) {	//¾ÆÀÌÅÛ ¿¡ÀÌÂ¡ 
		//CheckCraftItemForm(); //Á¶ÀÛÇÏ´Â »ó³ğÀÇ ÀÚ½ÄµéÀ» Á×¿©¹ö¸®ÀÚ Æ¡!!
		if ((pItem->CODE & sinITEM_MASK2) == sinOS1) { //½©ÅÒÀÏ °æ¿ì 
			for (int j = 1; j < 15; j++) {
				if (!sAgingItem.AgingItem[j].Flag) {
					memcpy(&sAgingItem.AgingItem[j], pItem, sizeof(sITEM));
					sAgingItem.AgingItem[j].x = pItem->SetX;
					sAgingItem.AgingItem[j].y = pItem->SetY;
					pItem->Flag = 0; //ÃÊ±âÈ­ 
					//sinPlaySound(sAgingItem.AgingItem[j].SoundIndex);
					//			ReFormCraftItem();
					return TRUE;
				}
			}
		}
		else {
			if (!sAgingItem.AgingItem[0].Flag) {
				memcpy(&sAgingItem.AgingItem[0], pItem, sizeof(sITEM));
				sAgingItem.AgingItem[0].x = pItem->SetX;
				sAgingItem.AgingItem[0].y = pItem->SetY;
				pItem->Flag = 0; //ÃÊ±âÈ­ 
				//sinPlaySound(sAgingItem.AgingItem[0].SoundIndex);
				//		ReFormCraftItem();
				return TRUE;
			}

		}

	}
	if (Kind == 2) {	//¾ÆÀÌÅÛ Make
		for (int j = 0; j < 15; j++) {
			if (!sAgingItem.AgingItem[j].Flag) {
				memcpy(&sAgingItem.AgingItem[j], pItem, sizeof(sITEM));
				sAgingItem.AgingItem[j].x = pItem->SetX;
				sAgingItem.AgingItem[j].y = pItem->SetY;
				pItem->Flag = 0; //ÃÊ±âÈ­ 
				//sinPlaySound(sAgingItem.AgingItem[j].SoundIndex);
				return TRUE;
			}
		}
	}
	if (Kind == 3) {  //¿¡ÀÌÂ¡ ½ºÅæ // ÄÚÆÛ ¿À¾î // ¹ÚÀç¿ø - ¿¤´õ ÄÚÆÛ ¿À¾î, ½´ÆÛ ¿¡ÀÌÂ¡ ½ºÅæ   // Àåº° - ½´ÆÛ ¿¡ÀÌÂ¡ ½ºÅæ 1.5
		if (!sAgingItem.AgingItem[14].Flag) {
			memcpy(&sAgingItem.AgingItem[14], pItem, sizeof(sITEM));
			sAgingItem.AgingItem[14].x = pItem->SetX;
			sAgingItem.AgingItem[14].y = pItem->SetY;
			pItem->Flag = 0; //ÃÊ±âÈ­ 
			//sinPlaySound(sAgingItem.AgingItem[14].SoundIndex);
			return TRUE;
		}
	}

	return TRUE;

}

//¾ÆÀÌÅÛÀ» ¼±ÅÃÇÑ´Ù 
int cCRAFTITEM::PickUpCraftItem(int x, int y, int PickUpFlag, int Kind)
{

	int i = 0;
	SelectTradeItemIndex = 0;    //ÁıÀ» ¾ÆÀÌÅÛ 
	if (Kind == 0) {
		for (i = 0; i < 15; i++) {
			if (sCraftItem.CraftItem[i].Flag) {
				if (sCraftItem.CraftItem[i].x < x && sCraftItem.CraftItem[i].x + sCraftItem.CraftItem[i].w > x &&
					sCraftItem.CraftItem[i].y < y && sCraftItem.CraftItem[i].y + sCraftItem.CraftItem[i].h > y) {
					if (PickUpFlag) {
						//½©ÅÒ Á¶ÇÕ »óÅÂ¿¡¼­ ¿¡ÀÌÂ¡À» Äµ½½ÇÏÁö ¸øÇÑ´Ù.======================================================
						int cnt = 0;
						for (cnt = 1; cnt < 15; cnt++) {
							if (i == 0 && sCraftItem.CraftItem[cnt].Flag)
							{

								cnt = 100;
								break;
							}
						}
						if (cnt == 100) break;
						//=================================================================================================	
						CheckCraftItemForm();
						memcpy(&MouseItem, &sCraftItem.CraftItem[i], sizeof(sITEM));
						sCraftItem.CraftItem[i].Flag = 0;
						haCraftSortItem(&sCraftItem);  //¾ÆÀÌÅÛ Àç±¸¼º ¶§¹®¿¡ ¹è¿­ ÀÌµ¿
						//memset(&sCraftItem.CraftItem[i],0,sizeof(sITEM));  //¼­¹ö ÇØÅ· ¿¡·¯ ¶§¹®¿¡ ÀÓ½Ã ¹æÆíÀÌ´Ù.

						//sinPlaySound(sCraftItem.CraftItem[i].SoundIndex);
						TradeColorIndex = 0;
						ReFormCraftItem();
						if (ForceFlag)
							CheckForceOrbPrice(); // Forece Orb ÇÃ·¢ÀÖÀ»°æ¿ì¿£ Æ÷½º°¡°İÀ» Ç¥½Ã

						if (i == 0) {  //¾Á¾ÆÀÌÅÛ ¶§¹®¿¡ 
							for (int i = 1; i < 15; i++) {
								if (sCraftItem.CraftItem[i].Flag) {
									if ((sCraftItem.CraftItem[i].CODE & sinITEM_MASK2) == sinSE1) {
										if (!cInvenTory.AutoSetInvenItem(&sCraftItem.CraftItem[i])) {
											sinThrowItemToFeild(&sCraftItem.CraftItem[i]);
											sCraftItem.CraftItem[i].Flag = 0;
											memset(&sCraftItem.CraftItem[i], 0, sizeof(sCRAFTITEM)); //¾ÆÀÌÅÛÀ» ´Ù ³¯·Á¹ö¸°´Ù 
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
						sinShowItemInfoFlag = 1; //¾ÆÀÌÅÛÀÇ Á¤º¸¸¦ º¸¿©ÁØ´Ù 
						cItem.ShowItemInfo(&sCraftItem.CraftItem[i], 2);
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
						//½©ÅÒ Á¶ÇÕ »óÅÂ¿¡¼­ ¿¡ÀÌÂ¡À» Äµ½½ÇÏÁö ¸øÇÑ´Ù.======================================================
						int cnt = 0;
						for (cnt = 1; cnt < 15; cnt++) {
							if (i == 0 && sAgingItem.AgingItem[cnt].Flag) {

								cnt = 100;
								break;
							}
						}
						if (cnt == 100) break;
						//===================================================================================================

						memcpy(&MouseItem, &sAgingItem.AgingItem[i], sizeof(sITEM));
						sAgingItem.AgingItem[i].Flag = 0;
						//sinPlaySound(sAgingItem.AgingItem[i].SoundIndex);
						//¿¡ÀÌÂ¡ ½ºÅæ // ÄÚÆÛ ¿À¾î
						if (sAgingItem.AgingItem[i].sItemInfo.CODE == (sinBI1 | sin10) ||
							sAgingItem.AgingItem[i].sItemInfo.CODE == (sinBI1 | sin11) ||
							// ¹ÚÀç¿ø - ¿¤´õ ÄÚÆÛ ¿À¾î, ½´ÆÛ ¿¡ÀÌÂ¡ ½ºÅæ Ãß°¡
							// Àåº° - ½´ÆÛ ¿¡ÀÌÂ¡ ½ºÅæ 1.5
							sAgingItem.AgingItem[i].sItemInfo.CODE == (sinBI1 | sin60) ||
							sAgingItem.AgingItem[i].sItemInfo.CODE == (sinBI1 | sin61) ||
							sAgingItem.AgingItem[i].sItemInfo.CODE == (sinBI1 | sin85)) break;
						TradeColorIndex = 0;
						//Make Item   (ÇÃ·º¾øÀÌ ±×³É KindCode¸¸ Ã¼Å©ÇÑ´Ù )ÇÃ·ºÀº À§¿¡¼­ Áö¿ü´Ù -_-;
						if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
							(MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinMA2) {
							sinMakeItemCheck();

						}
						else {
							if (i == 0) {
								for (int y = 0; y < 4; y++) { //½©ÅÒ ÀÎµ¦½º ÃÊ±âÈ­ 
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
						cItem.ShowItemInfo(&sAgingItem.AgingItem[i], 2);
					}
				}
			}
		}
	}
	return FALSE;
}

DWORD SheltomCode[MAX_SHELTOM] = { sin01,sin02,sin03,sin04,sin05,sin06,sin07,sin08,sin09, sin10, sin11, sin12, sin13, sin14 };

int cCRAFTITEM::MixingItem()
{
	int i, j, t, k, CheckFlag = 0, Temp = 0, LineCount = 0;
	DWORD TempCode2 = 0;
	int  ReConCnt = 0;

	if (sCraftItem_Recv.DocIndex == 2000)
	{
		for (i = 0; i < 15; i++) {
			if (sCraftItem_Recv.SheltomCode[i]) {
				TempCode2 = sCraftItem_Recv.SheltomCode[i];
				break;
			}
		}

		for (j = 0; j < MAX_SEEL_COUNT; j++)
		{
			if (sReconItem[j].dwCODE == TempCode2) {
				ReConCnt = sReconItem[j].iCount;
			}
		}

		if (!ReConCnt)
		{
			sCraftItem_Recv.Result = FALSE;
			return FALSE;
		}

		for (i = 0; i < ReConCnt; i++)
		{
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

	for (j = 0; j < 12; j++) {
		if ((sCraftItem_Recv.SheltomCode[j] & sinITEM_MASK2) == sinSE1) {
			sCraftItem_Recv.Result = FALSE;
			return FALSE;
		}
	}

	for (j = 0; j < 15; j++)
		sCraftItem.SheltomIndex[j] = 0;

	for (i = 0; i < 15; i++)
	{
		if (sCraftItem_Recv.SheltomCode[i] & sinITEM_MASK2) {
			for (j = 0; j < 15; j++) {
				if ((sCraftItem_Recv.SheltomCode[i] & sinITEM_MASK3) == SheltomCode[j]) {
					sCraftItem.SheltomIndex[j]++;
					break;
				}
			}
		}
	}

	for (i = 0; i < MAX_CRAFTITEM_INFO; i++) {

		for (j = 0; j < 8; j++)
		{
			if (sCraftItem_Recv.DesCraftItem.Flag)
			{

				if ((sCraftItem_Info[i].MainItemCode[j] & sinITEM_MASK2) == (sCraftItem_Recv.DesCraftItem.sItemInfo.CODE & sinITEM_MASK2))
				{
					for (t = 0; t < MAX_SHELTOM; t++)
					{
						if (sCraftItem_Info[i].AddItem[t] != sCraftItem.SheltomIndex[t])
						{
							CheckFlag = 1;
							break;
						}
						CheckFlag = 0;

					}
					if (!CheckFlag)
					{
						for (k = 0; k < 14; k++)
						{
							if (sCraftItem.SheltomIndex[k])
								sCraftItem_Recv.DesCraftItem.sItemInfo.ItemAgingNum[0] = k + 1;

							if (sCraftItem_Info[i].PlusElementCode[k])
							{
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
								case SIN_ADD_CRITICAL:
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Critical_Hit += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Critical_Hit += (sCraftItem_Recv.DesCraftItem.sItemInfo.Critical_Hit * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Critical_Hit +=  (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Critical_Hit*(sCraftItem_Info[i].AddElement									[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_CRITICAL;
									break;
								case SIN_ADD_ATTACK_RATE:	//¸íÁß·Â 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Rating += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Rating += (sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Rating * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Rating +=  (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Rating*(sCraftItem_Info[i].											AddElement[k]/100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_ATTACK_RATE;
									break;
								case SIN_ADD_DAMAGE_MIN:	//ÃÖ¼Ò´ë¹ÌÁö
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[0] += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[0] += (sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[0] * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[0] +=  (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[0]*(sCraftItem_Info[i].AddElement[k]/100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_DAMAGE_MIN;
									break;

								case SIN_ADD_DAMAGE_MAX:	//ÃÖ´ë´ë¹ÌÁö
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[1] += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[1] += (sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[1] * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[1] +=  (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[1]*(sCraftItem_Info[i].AddElement[k]/100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_DAMAGE_MAX;
									break;
								case SIN_ADD_ATTACK_SPEED:	//°ø°İ¼Óµµ
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Speed += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Speed += (sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Speed * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Speed += (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Speed*(sCraftItem_Info[i].AddElement[k]/											100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_ATTACK_SPEED;
									break;
								case SIN_ADD_ABSORB:	//Èí¼ö·Â 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fAbsorb += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fAbsorb += (GetItemAbsorb(&sCraftItem_Recv.DesCraftItem.sItemInfo) * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_ABSORB;
									break;
								case SIN_ADD_DEFENCE:	//¹æ¾î·Â 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Defence += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Defence += (sCraftItem_Recv.DesCraftItem.sItemInfo.Defence * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Defence += (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Defence*(sCraftItem_Info[i].AddElement[k]/100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_DEFENCE;
									break;
								case SIN_ADD_BLOCK_RATE: //ºí·°À² 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fBlock_Rating += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fBlock_Rating += (sCraftItem_Recv.DesCraftItem.sItemInfo.fBlock_Rating * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_BLOCK_RATE;
									break;
								case SIN_ADD_MOVE_SPEED: //ÀÌµ¿¼Óµµ 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fSpeed += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fSpeed += (sCraftItem_Recv.DesCraftItem.sItemInfo.fSpeed * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_MOVE_SPEED;
									break;

								case SIN_ADD_LIFE: //ÃÖ´ë »ı¸í·Â 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Life += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Life += (sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Life * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_LIFE;
									break;

								case SIN_ADD_MANA: //ÃÖ´ë ±â·Â
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Mana += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Mana += (sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Mana * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_MANA;
									break;
								case SIN_ADD_STAMINA: //ÃÖ´ë ±Ù·Â
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Stamina += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Stamina += (sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Stamina * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_STAMINA;
									break;
								case SIN_ADD_LIFEREGEN: //»ı¸í·ÂÀç»ı
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fLife_Regen += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fLife_Regen += (sCraftItem_Recv.DesCraftItem.sItemInfo.fLife_Regen * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_LIFEREGEN;
									break;
								case SIN_ADD_MANAREGEN: //±â·ÂÀç»ı
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fMana_Regen += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fMana_Regen += (sCraftItem_Recv.DesCraftItem.sItemInfo.fMana_Regen * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_MANAREGEN;

									break;
								case SIN_ADD_STAMINAREGEN: //±Ù·ÂÀç»ı
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fStamina_Regen += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fStamina_Regen += (sCraftItem_Recv.DesCraftItem.sItemInfo.fStamina_Regen * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_STAMINAREGEN;

									break;

								}
							}
						}

						ReformItem(&sCraftItem_Recv.DesCraftItem.sItemInfo);

						if (!CheckMixItem(&sCraftItem_Recv.DesCraftItem.sItemInfo)) {
							SendSetHackUser(50);
							return FALSE;
						}

						sCraftItem_Recv.DesCraftItem.sItemInfo.iMixType = sCraftItem_Info[i].iMixType;
						sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindCode = ITEM_KIND_CRAFT;
						ReformMixItem(&sCraftItem_Recv.DesCraftItem.sItemInfo);

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

//¾ÆÀÌÅÛÀÌ µé¾î°¥¼öÀÖ´ÂÁö Ã¼Å©ÇÑ´Ù 
int cCRAFTITEM::CraftCheckEmptyArea(sITEM* pItem)
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;
	int StartX = 23;  //ÀÎº¥ ¹Ú½ºÀÇ ½ÃÀÛÁ¡ X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - pItem->w; //¾ÆÀÌÅÛÀÌ µé¾î°¥¼öÀÖ´Â °¡Àå ³¡ XÁÂÇ¥ 
	cy = (22 * INVENTORY_BOX_COL) - pItem->h; //¾ÆÀÌÅÛÀÌ µé¾î°¥¼öÀÖ´Â °¡Àå ³¡ YÁÂÇ¥

	//ÄÚµå°¡ °°Áö ¾Ê°Å³ª ¾ÆÀÌÅÛÀÌ ºñ¾îÀÖÁö ¾ÊÀº °÷¿¡ ¼ÂÆÃÇÒ¶§ °Ë»öÇØ¼­ À§Ä¡¸¦ Ã£¾Æ³½´Ù 
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

//¹Í½ºÃÄ ¾ÆÀÌÅÛÀ» Ãë¼ÒÇÑ´Ù 
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

	cCraftItem.OpenFlag = 0; //Ã¢À» ´İ´Â´Ù 
	memset(&sCraftItem, 0, sizeof(sCRAFTITEM)); //¾ÆÀÌÅÛÀ» ´Ù ³¯·Á¹ö¸°´Ù 
	ResetInvenItemCode();				//ÀÌº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­

	memset(&sCraftItem_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //¹ŞÀº ¹Í½ºÃÄ ±¸Á¶Ã¼ ÃÊ±âÈ­  (ÀÏ´Ü °°ÀÌÇØÁØ´Ù)
	memset(&sCraftItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //º¸³½ ¹Í½ºÃÄ ±¸Á¶Ã¼ ÃÊ±âÈ­ 

	//Force Orb ÃÊ±âÈ­
	ForceFlag = 0;
	ForceItemPrice = 0;
	ForceItemPrice2 = 0;
	return TRUE;
}

int CraftItemCheckDelayTime = 0;

//Å©·¡ÇÁÆ® ¾ÆÀÌÅÛÀ» Á¶ÀÛÇÏ´Â ¸ÁÇÒ À¯Àú¸¦ º´½Å ¸¸µç´Ù 
int cCRAFTITEM::CheckHackCraftItem()
{
	//ÇöÁ¦ ¼­¹ö¿¡¼­ ÇÏµµ·Ï µÇ¾îÀÖ¾î Àû¿ëÀÌ ¾ÈµÈ´Ù 
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
		SendSetHackUser(8); //ÇØÅ·À» ÇÏ·Á°íÇß´ø ¸øµÈ À¯Àú¸¦ °í¹ß TRUE Á¢¼Ó Á¾·á 

	}

	return TRUE;
}

//////////¹Í½ºÃÄ ¾ÆÀÌÅÛ Á¶ÀÛ ¹æÁö¸¦ À§ÇÑ Èû°Ü¿î ³ë·Â 
int cCRAFTITEM::CheckCraftItemForm()
{ //Ã¼Å©  
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
		//if(TempCheckDataSum != CraftItemCheckSum) //ÇÊ¿ä¾ø´À°Å °°¼­¸® »¯´Ù ÇÏÇÖ
			//SendSetHackUser(9); //ÇØÅ·À» ÇÏ·Á°íÇß´ø ¸øµÈ À¯Àú¸¦ °í¹ß TRUE Á¢¼Ó Á¾·á 
	}
	return TRUE;

}

int cCRAFTITEM::ReFormCraftItem()
{ //ÀÎÁõ 

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

//////////Æ®·¹ÀÌµå  ¾ÆÀÌÅÛ Á¶ÀÛ ¹æÁö¸¦ À§ÇÑ Èû°Ü¿î ³ë·Â ÇíÇíÇí
//ReFormTradeItem();
//CheckTradeItemForm();
int cTRADE::CheckTradeItemForm()
{ //Ã¼Å©  
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
		SendSetHackUser(10); //ÇØÅ·À» ÇÏ·Á°íÇß´ø ¸øµÈ À¯Àú¸¦ °í¹ß TRUE Á¢¼Ó Á¾·á 

	return TRUE;

}

int cTRADE::ReFormTradeItem()
{ //ÀÎÁõ 

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
////////////////////Ã¢°í¸¦ Á¶ÀÛÇÏ·Á´Â ³ª»Û ÀÚ½ÄµéÀ» ¹úÁÖ±â À§ÇÑ Èû°Ü¿î ³ë·Â !!!
int cWAREHOUSE::CheckWareHouseForm()
{ //Ã¼Å©  
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
		SendSetHackUser(11); //ÇØÅ·À» ÇÏ·Á°íÇß´ø ¸øµÈ À¯Àú¸¦ °í¹ß TRUE Á¢¼Ó Á¾·á 

	return TRUE;

}

int cWAREHOUSE::ReFormWareHouse()
{ //ÀÎÁõ 

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

////////////////////////////////////// ¿¡ÀÌÂ¡

//¾ÆÀÌÅÛÀ» ¿¡ÀÌÂ¡ÇÑ´Ù 
int cAGING::AgingItem(int MakeItemFlag)
{
	int ResultCount = 0;
	int ResultCountCheck = 0;
	DWORD TempCODE[16] = { 0, };
	int  i = 0;
	//ÄÚµå º¹»ç

	if (sAging_Recv.A_StoneItemCode == (sinBI1 | sin10))      //¿¡ÀÌÂ¡ ½ºÅæ
		cAging.AginStoneKind = 1;
	else if (sAging_Recv.A_StoneItemCode == (sinBI1 | sin11)) //ÄÚÆÛ ¿À¾î
		cAging.AginStoneKind = 2;
	else if (sAging_Recv.A_StoneItemCode == (sinBI1 | sin60)) // ¹ÚÀç¿ø - ¿¤´õ ÄÚÆÛ ¿À¾î
		cAging.AginStoneKind = 3;
	else if (sAging_Recv.A_StoneItemCode == (sinBI1 | sin61)) // ¹ÚÀç¿ø - ½´ÆÛ ¿¡ÀÌÂ¡ ½ºÅæ
		cAging.AginStoneKind = 4;
	else if (sAging_Recv.A_StoneItemCode == (sinBI1 | sin85)) // Àåº° - ½´ÆÛ ¿¡ÀÌÂ¡ ½ºÅæ 1.5
		cAging.AginStoneKind = 5;
	else
		cAging.AginStoneKind = 0;

	if (sAging_Recv.DesCraftItem.sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON) { //Äù½ºÆ® ¾ÆÀÌÅÛÀÏ°æ¿ì
		if (SetChangeJob3QuestItem(&sAging_Recv.DesCraftItem.sItemInfo)) {
			ReformMixItem(&sAging_Recv.DesCraftItem.sItemInfo); //ÀçÀÎÁõ (¾ÆÀÌÅÛÀ» ÀÎº¥Åä¸®¿¡ ¼ÂÆÃÇÏ±âÀü¿¡ ...)
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


	if (MakeItemFlag > 0)return FALSE;
	int DestroyPersent = 0;

	DWORD SheltomCODE[MAX_SHELTOM] = { sinOS1 | sin01,sinOS1 | sin02,sinOS1 | sin03,sinOS1 | sin04,sinOS1 | sin05,sinOS1 | sin06,sinOS1 | sin07,sinOS1 | sin08,sinOS1 | sin09,sinOS1 | sin10,sinOS1 | sin11,sinOS1 | sin12,sinOS1 | sin13, sinOS1 | sin14 };
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
	if (MakeItemFlag == -1)
		DestroyPersent = 100;
	else
		DestroyPersent = GetRandomPos(0, 100);

	int AgingKind = 0;

	// chance de subir +2
	int PlusAgingPercent[16] = { 12, 12, 12, 10, 10, 10, 8, 8, 8, 6, 6, 6, 4, 4, 4, 0 };

	// Aging quebrando aqui, 0 = quebra padrão = 3
	int AgingItemFaildNum = 3;

	int DownPersent = GetRandomPos(0, 100);
	int PlusPersent = GetRandomPos(0, 100);

	if (cAging.AginStoneKind == 2) // Cooper ore
		DestroyPersent = 0;

	// Define se o item vai subir +2, voltar ou quebrar

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

	else if (cAging.AginStoneKind == 5) // Pedra 100% (Não voltar mas pode quebrar)
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

	
	char msg[80]{ 0 };
	sprintf_s(msg, sizeof(msg), "Chance de Quebrar: %d, Chance de Falhar: %d, Resultado: %d", DestroyPersent, DownPersent, AgingItemFaildNum);

	for (int cnt = 0; cnt < CONNECTMAX; cnt++)
	{
		if (rsPlayInfo[cnt].lpsmSock && rsPlayInfo[cnt].szName[0])
		{
			GameMasters::getInstance()->Alert(&rsPlayInfo[cnt], msg);
		}
	}


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

	for (int y = 0; y < 4; y++) { //½©ÅÒ ÀÎµ¦½º ÃÊ±âÈ­ 
		for (int h = 0; h < 3; h++) {
			AgingLevelSheltomIndex[y][h] = 0;
		}

	}

	AgingCheckSheltomFlag = 0; //¿¡ÀÌÂ¡ °¡´É ÇÃ·¢ ÃÊ±âÈ­ 
	AgingSheltomCnt2 = 0;
	if (!Flag)
		cAging.OpenFlag = 0; //Ã¢À» ´İ´Â´Ù 

	ResetInvenItemCode();				//ÀÌº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­
	return TRUE;
}

//¿¡ÀÌÂ¡ÇÒ¼öÀÖ´Â ¾ÆÀÌÅÛÀÎÁö¸¦ Ã¼Å©ÇÑ´Ù 
int cAGING::CheckItem(sITEM* pItem)
{
	int k = 0;
	int SheltomCnt = 0;
	if ((pItem->CODE & sinITEM_MASK2) == sinOS1) {
		AgingSheltomCnt2++;

	}
	else {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				AgingLevelSheltomIndex[i][j] = AgingLevelSheltom[pItem->sItemInfo.ItemAgingNum[0]][k++]; //½©ÅÒÀÇ Á¾·ù¿Í ÀÎµ¦½º¸¦ ³Ö´Â´Ù 

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

// ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¾ÆÀÌÅÛ Ãß°¡(Àå°©, ºÎÃ÷, ¾Ï¸´)
DWORD AgingItemCode3[12] = { sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1,sinDA1,sinDS1,sinOM1,sinDA2 };//sinDG1,sinDB1,sinOA2};
//¿¡ÀÌÂ¡ÇÒ ¾ÆÀÌÅÛÀÇ °ø°£À» Ã¼Å©ÇÑ´Ù 
int cAGING::SetAgingItemAreaCheck(sITEM* pItem)
{
	// ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¼³Á¤ º¯°æ(¿À¸£µµ Ãß°¡)
	DWORD SheltomCODE[MAX_SHELTOM] = { sin01,sin02,sin03,sin04,sin05,sin06,sin07,sin08,sin09,sin10,sin11,sin12,sin13,sin14,sin15 };//sin15}; 
	int SheltomIndex2 = 0;
	int i, j;
	//¿¡ÀÌÂ¡ ½ºÅæ/ ÄÚÆÛ ¿À¾î-------------------------------------------------------------
	if (pItem->sItemInfo.CODE == (sinBI1 | sin10) || pItem->sItemInfo.CODE == (sinBI1 | sin11) ||
		pItem->sItemInfo.CODE == (sinBI1 | sin60) || pItem->sItemInfo.CODE == (sinBI1 | sin61) || // ¹ÚÀç¿ø - ¿¤´õ ÄÚÆÛ ¿À¾î, ½´ÆÛ ¿¡ÀÌÂ¡ ½ºÅæ Ãß°¡
		pItem->sItemInfo.CODE == (sinBI1 | sin85)) // Àåº° - ½´ÆÛ ¿¡ÀÌÂ¡ ½ºÅæ 1.5
	{
		cAging.AginStoneKind = 0;
		int AgingStoneX = 159 + 11, AgingStoneY = 268 + 11 + sinInterHeight2;
		TradeColorIndex = 0; //ÃÊ±âÈ­   (Æ®·¹ÀÌµå Ã¢°ú Ã¢°í°¡ °°ÀÌ ¾´´Ù)
		cTrade.InitTradeColorRect(); //ÃÊ±âÈ­
		if (AgingStoneX <= pItem->x + 22 && AgingStoneX > pItem->x && AgingStoneY< pItem->y + 22 && AgingStoneY > pItem->y) {
			pItem->SetX = AgingStoneX + (((pItem->x + 22 - AgingStoneX) / 22) * 22) - 11;
			pItem->SetY = AgingStoneY + (((pItem->y + 22 - AgingStoneY) / 22) * 22) - 11;
			pItem->ItemPosition = 0; //¹è¿­ÀÇ ÀÎµ¦½º¸¦ ÀúÀåÇÑ´Ù 
			TradeColorIndex = SET_ITEM_CHECK_COLOR; //¼ÂÆÃÇÒ¿µ¿ª
			if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
				cTrade.InitTradeColorRect(); //ÄÃ·¯ ¹Ú½º RECT ÃÊ±âÈ­ (ÄÃ·¯ ¹Ú½º RECTÁß ÇÏ³ª¶óµÎ 0 ÀÌ¸é ¼ÂÆÃÇÒ¼ö ¾ø´Ù 
				return FALSE;
			}
			if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //°ãÄ¡´Â ¾ÆÀÌÅÛÀÌ ÀÖ³ª Ã¼Å© 
				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;
			}
			return TRUE;
		}
	}

	if (pItem->sItemInfo.CODE == (sinBI1 | sin61)) // ¹ÚÀç¿ø - ¿¤´õ ÄÚÆÛ ¿À¾î, ½´ÆÛ ¿¡ÀÌÂ¡ ½ºÅæ Ãß°¡
	{
		cAging.AginStoneKind = 4;
		int AgingStoneX = 159 + 11, AgingStoneY = 268 + 11 + sinInterHeight2;
		TradeColorIndex = 0; //ÃÊ±âÈ­   (Æ®·¹ÀÌµå Ã¢°ú Ã¢°í°¡ °°ÀÌ ¾´´Ù)
		cTrade.InitTradeColorRect(); //ÃÊ±âÈ­
		if (AgingStoneX <= pItem->x + 22 && AgingStoneX > pItem->x && AgingStoneY< pItem->y + 22 && AgingStoneY > pItem->y) {
			pItem->SetX = AgingStoneX + (((pItem->x + 22 - AgingStoneX) / 22) * 22) - 11;
			pItem->SetY = AgingStoneY + (((pItem->y + 22 - AgingStoneY) / 22) * 22) - 11;
			pItem->ItemPosition = 0; //¹è¿­ÀÇ ÀÎµ¦½º¸¦ ÀúÀåÇÑ´Ù 
			TradeColorIndex = SET_ITEM_CHECK_COLOR; //¼ÂÆÃÇÒ¿µ¿ª
			if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
				cTrade.InitTradeColorRect(); //ÄÃ·¯ ¹Ú½º RECT ÃÊ±âÈ­ (ÄÃ·¯ ¹Ú½º RECTÁß ÇÏ³ª¶óµÎ 0 ÀÌ¸é ¼ÂÆÃÇÒ¼ö ¾ø´Ù 
				return FALSE;
			}
			if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //°ãÄ¡´Â ¾ÆÀÌÅÛÀÌ ÀÖ³ª Ã¼Å© 
				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;
			}
			return TRUE;
		}
	}


	// Àåº° - ½´ÆÛ ¿¡ÀÌÂ¡ ½ºÅæ 1.5
	if (pItem->sItemInfo.CODE == (sinBI1 | sin85))
	{
		cAging.AginStoneKind = 5;
		int AgingStoneX = 159 + 11, AgingStoneY = 268 + 11 + sinInterHeight2;
		TradeColorIndex = 0; //ÃÊ±âÈ­   (Æ®·¹ÀÌµå Ã¢°ú Ã¢°í°¡ °°ÀÌ ¾´´Ù)
		cTrade.InitTradeColorRect(); //ÃÊ±âÈ­
		if (AgingStoneX <= pItem->x + 22 && AgingStoneX > pItem->x && AgingStoneY< pItem->y + 22 && AgingStoneY > pItem->y) {
			pItem->SetX = AgingStoneX + (((pItem->x + 22 - AgingStoneX) / 22) * 22) - 11;
			pItem->SetY = AgingStoneY + (((pItem->y + 22 - AgingStoneY) / 22) * 22) - 11;
			pItem->ItemPosition = 0; //¹è¿­ÀÇ ÀÎµ¦½º¸¦ ÀúÀåÇÑ´Ù 
			TradeColorIndex = SET_ITEM_CHECK_COLOR; //¼ÂÆÃÇÒ¿µ¿ª
			if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
				cTrade.InitTradeColorRect(); //ÄÃ·¯ ¹Ú½º RECT ÃÊ±âÈ­ (ÄÃ·¯ ¹Ú½º RECTÁß ÇÏ³ª¶óµÎ 0 ÀÌ¸é ¼ÂÆÃÇÒ¼ö ¾ø´Ù 
				return FALSE;
			}
			if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //°ãÄ¡´Â ¾ÆÀÌÅÛÀÌ ÀÖ³ª Ã¼Å© 
				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;
			}
			return TRUE;
		}
	}


	//-----------------------------------------------------------------------------------
	if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2) {
		if (sAgingItem.AgingItem[0].Flag && (sAgingItem.AgingItem[0].sItemInfo.CODE & sinITEM_MASK2) == sinMA1) {
			TradeColorIndex = 0; //ÃÊ±âÈ­   (Æ®·¹ÀÌµå Ã¢°ú Ã¢°í°¡ °°ÀÌ ¾´´Ù)
			TradeCrashItemIndex[0] = 0; //Ãæµ¹µÈ ¾ÆÀÌÅÛ Index ÃÊ±âÈ­ 
			cTrade.InitTradeColorRect(); //ÃÊ±âÈ­
			TradeStartX = 218;
			TradeStartY = 193 + sinInterHeight2;
			TradeEndX = TradeStartX + (3 * 22);
			TradeEndY = TradeStartY + (4 * 22);

			for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22) {
				for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22) {
					if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j) { //ÀÎº¥Åä¸® ¹Ú½º¿¡ µé¾î°¬´ÂÁö¸¦ Ã¼Å© 
						TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
						TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
						TradeColorRect.right = pItem->w;
						TradeColorRect.bottom = pItem->h;
						if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
							cTrade.InitTradeColorRect(); //ÄÃ·¯ ¹Ú½º RECT ÃÊ±âÈ­ (ÄÃ·¯ ¹Ú½º RECTÁß ÇÏ³ª¶óµÎ 0 ÀÌ¸é ¼ÂÆÃÇÒ¼ö ¾ø´Ù 
							return FALSE;
						}

						pItem->SetX = TradeColorRect.left;
						pItem->SetY = TradeColorRect.top;
						pItem->ItemPosition = 0; //¹è¿­ÀÇ ÀÎµ¦½º¸¦ ÀúÀåÇÑ´Ù 
						TradeColorIndex = SET_ITEM_CHECK_COLOR; //¼ÂÆÃÇÒ¿µ¿ª

						if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //°ãÄ¡´Â ¾ÆÀÌÅÛÀÌ ÀÖ³ª Ã¼Å© 
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
		TradeColorIndex = 0; //ÃÊ±âÈ­   (Æ®·¹ÀÌµå Ã¢°ú Ã¢°í°¡ °°ÀÌ ¾´´Ù)
		TradeCrashItemIndex[0] = 0; //Ãæµ¹µÈ ¾ÆÀÌÅÛ Index ÃÊ±âÈ­ 
		cTrade.InitTradeColorRect(); //ÃÊ±âÈ­

		TradeStartX = 58;
		TradeStartY = 193 + sinInterHeight2;
		TradeEndX = TradeStartX + (3 * 22);
		TradeEndY = TradeStartY + (4 * 22);
	}

	/////////¿©±â´Â ±âº»¿¡ÀÌÂ¡ 
	for (int y = 0; y < MAX_SHELTOM; y++) {
		if (SheltomCODE[y] == (pItem->sItemInfo.CODE & sinITEM_MASK3)) {
			SheltomIndex2 = y + 1;
			break;
		}
	}

	int AgingOkWeaponGroupFlag = 0;	//¿¡ÀÌÂ¡À» ÇÒ¼öÀÖ´Â ¾ÆÀÌÅÛ ±ºÀÎ°¡?
	TradeColorIndex = 0; //ÃÊ±âÈ­   (Æ®·¹ÀÌµå Ã¢°ú Ã¢°í°¡ °°ÀÌ ¾´´Ù)
	TradeCrashItemIndex[0] = 0; //Ãæµ¹µÈ ¾ÆÀÌÅÛ Index ÃÊ±âÈ­ 
	cTrade.InitTradeColorRect(); //ÃÊ±âÈ­

	if ((pItem->CODE & sinITEM_MASK2) == sinOS1) { //½©ÅÒÀÏ °æ¿ì 
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
							if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j) { //ÀÎº¥Åä¸® ¹Ú½º¿¡ µé¾î°¬´ÂÁö¸¦ Ã¼Å© 
								TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
								TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
								TradeColorRect.right = pItem->w;
								TradeColorRect.bottom = pItem->h;
								if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
									cTrade.InitTradeColorRect(); //ÄÃ·¯ ¹Ú½º RECT ÃÊ±âÈ­ (ÄÃ·¯ ¹Ú½º RECTÁß ÇÏ³ª¶óµÎ 0 ÀÌ¸é ¼ÂÆÃÇÒ¼ö ¾ø´Ù 
									return FALSE;
								}

								pItem->SetX = TradeColorRect.left;
								pItem->SetY = TradeColorRect.top;
								pItem->ItemPosition = 0; //¹è¿­ÀÇ ÀÎµ¦½º¸¦ ÀúÀåÇÑ´Ù 
								if ((pItem->CODE & sinITEM_MASK3) > sin20) // ¹ÚÀç¿ø - ¸ÅÁ÷ Æ÷½º(¿¡ÀÌÂ¡ÇÒ¶§ ¸ÅÁ÷ ½©ÅÒÀº ¿Ã·Á³õÁö ¸øÇÏµµ·Ï ÇÑ´Ù)
								{
									TradeColorIndex = NOT_SETTING_COLOR;
								}
								else
									TradeColorIndex = SET_ITEM_CHECK_COLOR; //¼ÂÆÃÇÒ¿µ¿ª

								if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //°ãÄ¡´Â ¾ÆÀÌÅÛÀÌ ÀÖ³ª Ã¼Å© 
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
		TradeColorIndex = 0; //ÃÊ±âÈ­   (Æ®·¹ÀÌµå Ã¢°ú Ã¢°í°¡ °°ÀÌ ¾´´Ù)
		TradeCrashItemIndex[0] = 0; //Ãæµ¹µÈ ¾ÆÀÌÅÛ Index ÃÊ±âÈ­ 
		cTrade.InitTradeColorRect(); //ÃÊ±âÈ­

		TradeStartX = 58;
		TradeStartY = 193 + sinInterHeight2;
		TradeEndX = TradeStartX + (3 * 22);
		TradeEndY = TradeStartY + (4 * 22);

	}


	return TRUE;
}
/*
int CraftItemCheckDelayTime = 0;

//Å©·¡ÇÁÆ® ¾ÆÀÌÅÛÀ» Á¶ÀÛÇÏ´Â ¸ÁÇÒ À¯Àú¸¦ º´½Å ¸¸µç´Ù  (¼­¹ö¿¡¼­ ÇÏ´Â°ü°è·Î ÀÇ¹Ì°¡ ¾ø±äÇÏ³ª È¤½Ã³ªÇØ¼­ ÁÖ¼®Ã³¸®ÇÔ)
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
		SendSetHackUser(8); //ÇØÅ·À» ÇÏ·Á°íÇß´ø ¸øµÈ À¯Àú¸¦ °í¹ß TRUE Á¢¼Ó Á¾·á

	}

	return TRUE;
}

*/

int AgingItemCheckDelayTime = 0;

//¿¡ÀÌÂ¡ µ¥ÀÌÅ¸ÀÇ Á¶ÀÛ¿©ºÎ¸¦ Á¶Á¤ÇÑ´Ù 
int cAGING::CheckAgingData()
{
	/*
	#ifdef __SIN_AGING_TEST


	#else
	AgingItemCheckDelayTime++;
	if(AgingItemCheckDelayTime < 70*10)return FALSE;
	AgingItemCheckDelayTime = 0;

	unsigned int AddItemCheckSum = 0;
	const unsigned int CheckAddItemData = 1848296; //ÀÌ·¸°Ô ÇØ³õÀ¸¸é ÀĞ±â Àü¿ëÀ¸·Î¸¸ µÈ´Ù³×... À½ÇÏÇÏÇÏ


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
		SendSetHackUser(107); //ÇØÅ·À» ÇÏ·Á°íÇß´ø ¸øµÈ À¯Àú¸¦ °í¹ß TRUE Á¢¼Ó Á¾·á

	}
#endif
	*/

	return TRUE;
}

////////////Ã¢°í¿¡ º¹»çµÈ¾ÆÀÌÅÛÀÌ ÀÖ´ÂÁö¸¦ Ã£´Â´Ù 
int cWAREHOUSE::CopyItemNotPickUp(sITEM* pItem, int JumpIndex)
{
	int i;
	if (smConfig.DebugMode)return TRUE; //µğ¹ö±× ¸ğµåÀÏ¶§´Â µé¼öÀÖµû Å©Å©Å©
	for (i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			if (JumpIndex == i)continue; //ÀÚ½ÅÀÇ ¾ÆÀÌÅÛÀº Ã£Áö¾Ê´Â´Ù
			if (CompareItems(&pItem->sItemInfo, &sWareHouse.WareHouseItem[i].sItemInfo)) {
				CopyItemIndex7[i] = i + 1;
				return FALSE;

			}

		}

	}

	return TRUE;
}

int SendServerFlag7 = 0;
//º¹»çµÈ ¾ÆÀÌÅÛÀÌ ÀÖ´ÂÁö¸¦ Ã¼Å©ÇÑ´Ù 
int cWAREHOUSE::CheckCopyItem()
{
	int i, j;
	if (SendServerFlag7)return FALSE;
	for (i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			for (j = 0; j < 100; j++) {
				if (i != j && sWareHouse.WareHouseItem[j].Flag) {
					if (CompareItems(&sWareHouse.WareHouseItem[i].sItemInfo, &sWareHouse.WareHouseItem[j].sItemInfo)) {
						SendSetHackUser2(1010, sWareHouse.WareHouseItem[i].CODE); //ÇØÅ·ÇÑ À¯Àú¸¦ ½Å°íÇÑ´Ù
						SendServerFlag7 = 1; //ÇÑ¹ø¸¸ Ã¼Å©ÇÏµµ·ÏÇÑ´Ù 
						break;
					}
				}
			}
		}
	}
	/*
	//µ¢´Ş¾Æ µ·°íµµ Ã¼Å© (À½... ÂóÂóÇÏ´Ù)
	if(8000000 < sWareHouse.Money-2023-1000000){
			SendSetHackUser2(1110,sWareHouse.Money-2023); //ÇØÅ·ÇÑ À¯Àú¸¦ ½Å°íÇÑ´Ù

	}
	*/


	return TRUE;
}
int CheckCraftMoney = 0;

///////// ¹Í½ºÃÄ ¾ÆÀÌÅÛÀ» ¼­¹ö¿¡ º¸³½´Ù 
int cCRAFTITEM::sinSendCraftItem()
{

	if (MixItemNoCopyFlag)return FALSE;
	int i = 0;


	//º¹»ç¸¦ Ã¼Å©ÇÏ±âÀ§ÇÑ ¹é¾÷º»À» ¸¸µç´Ù
	memcpy(&sCraftItemBackUp, &sCraftItem, sizeof(sCRAFTITEM));

	MixItemNoCopyFlag = 1; //¾ÆÀÌÅÛÀÌ º¹»çµÇ´Â°É ¹æÁöÇÏ±âÀ§ÇØ ÇÃ·ºÀ»ÁØ´Ù

	if (ForceFlag) { //Force Orb ÀÌ¸é 
		memset(&sCraftItem_Send.DesCraftItem, 0, sizeof(sITEM));
		memset(&sCraftItemBackUp, 0, sizeof(sCRAFTITEM));
		sCraftItem_Send.DesCraftItem.CODE = 1000;
		sCraftItem_Send.Money = ForceItemPrice;

	}
	//haGoon¾ÆÀÌÅÛ Àç±¸¼º À» ¼­¹ö·Î º¸³½´Ù.---------------------------------------------------------------------------
	if (cCraftItem.iReconItemFlag) {
		memset(&sCraftItem_Send.DesCraftItem, 0, sizeof(sITEM));
		memset(&sCraftItemBackUp, 0, sizeof(sCRAFTITEM));
		sCraftItem_Send.DocIndex = 2000;

	}
	//----------------------------------------------------------------------------------------------------------------
	if (sCraftItem.CraftItem[0].Flag) {
		sCraftItem_Send.DesCraftItem = sCraftItem.CraftItem[0];
		sCraftItem_Send.Money = sCraftItem.CraftItem[0].sItemInfo.Price;
	}
	for (i = 0; i < 12; i++) {
		if (sCraftItem.CraftItem[i + 1].Flag) {
			sCraftItem_Send.SheltomCode[i] = sCraftItem.CraftItem[i + 1].CODE;
			sCraftItem_Send.CheckSum[i] = sCraftItem.CraftItem[i + 1].sItemInfo.ItemHeader.dwChkSum;
			sCraftItem_Send.Head[i] = sCraftItem.CraftItem[i + 1].sItemInfo.ItemHeader.Head;
		}
	}

	// ¹ÚÀç¿ø - ¸®½ºÆå ½ºÅæµµ ¼­¹ö·Î º¸³½´Ù.
	if (sCraftItem.CraftItem[14].Flag) {
		sCraftItem_Send.A_StoneItemCode = sCraftItem.CraftItem[14].CODE;
		sCraftItem_Send.A_StoneCheckSum = sCraftItem.CraftItem[14].sItemInfo.ItemHeader.dwChkSum;
		sCraftItem_Send.A_StoneHead = sCraftItem.CraftItem[14].sItemInfo.ItemHeader.Head;
	}

	CheckCraftMoney = sCraftItem_Send.Money;

	CraftItemSendServerIndex++; //ÀÎµ¦½º¸¦ º¸³½´Ù 
	sCraftItem_Send.Index = CraftItemSendServerIndex;
	//ÀúÀåµÈ ±¸Á¶Ã¼¸¦ ¼­¹ö¿¡ º¸³½´Ù 
	cCraftItem.iReconItemFlag = 0;

	if (ForceFlag) { //Æ÷½º ¿Àºê´Â À®À¸·Î º¸³½´Ù
		SendWingItemToServer(&sCraftItem_Send);
	}
	else {
		SendCraftItemToServer(&sCraftItem_Send);
	}

	return TRUE;
}

extern int AgingEvento;
extern int AgingPrice;

///////// ¿¡ÀÌÂ¡ ¾ÆÀÌÅÛÀ» ¼­¹ö¿¡ º¸³½´Ù 
int cAGING::sinSendAgingItem()
{
	int i = 0;
	//¾ÆÀÌÅÛÀ» º¸³¾¶§ º¹»ç¸¦ ¹æÁöÇÏ±âÀ§ÇÑ ¹é¾÷º»À» ¸¸µç´Ù
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
	//¿¡ÀÌÂ¡ ½ºÅæµµ ¼­¹ö·Î º¸³½´Ù.
	if (sAgingItem.AgingItem[14].Flag)
	{
		sAging_Send.A_StoneItemCode = sAgingItem.AgingItem[14].CODE;
		sAging_Send.A_StoneCheckSum = sAgingItem.AgingItem[14].sItemInfo.ItemHeader.dwChkSum;
		sAging_Send.A_StoneHead = sAgingItem.AgingItem[14].sItemInfo.ItemHeader.Head;
	}

#ifdef AGING_MONEY_FREE // ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ °¡°İ ¹«·á ÀÌº¥Æ®
	sAging_Send.Money = 0;
	CheckCraftMoney = sAging_Send.Money;
#else
	sAging_Send.Money = sAgingItem.AgingItem[0].sItemInfo.Price;
	CheckCraftMoney = sAging_Send.Money;
#endif

	CraftItemSendServerIndex++; //ÀÎµ¦½º¸¦ º¸³½´Ù 
	sAging_Send.Index = CraftItemSendServerIndex;

	//ÀúÀåµÈ ±¸Á¶Ã¼¸¦ ¼­¹ö¿¡ º¸³½´Ù 
	SendAgingItemToServer(&sAging_Send);


	//////¾ÆÀÌÅÛÀ» º¸³»¸é¼­ µ·À» »«´Ù
#ifdef AGING_MONEY_FREE // ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ °¡°İ ¹«·á ÀÌº¥Æ®
	int Price = 0; // ¹«·á
#else
	int Price = ((sAgingItem.AgingItem[0].sItemInfo.ItemAgingNum[0] + 1) * sAgingItem.AgingItem[0].sItemInfo.Price) / 2;
#endif


	CheckCharForm();		//ÀÎÁõ 
	sinMinusMoney(Price, 1);
	ReformCharForm();		//ÀçÀÎÁõ 
	SendSaveMoney();		//±İ¾× Á¶ÀÛÀ» ¸øÇÏ°ÔÇÏ±âÀ§ÇØ È£ÃâÇÑ´Ù 
	return TRUE;
}
///////// ¼­¹ö¿¡¼­ ¹Í½ºÃÄ ¾ÆÀÌÅÛÀÇ ±¸Á¶¸¦ ¹Ş´Â´Ù 
int cCRAFTITEM::sinRecvCraftItem(sCRAFTITEM_SERVER* pCraftItem_Server)
{
	memcpy(&sCraftItem_Recv, pCraftItem_Server, sizeof(sCRAFTITEM_SERVER));

	return TRUE;
}

///////// ¼­¹ö¿¡¼­ ¿¡ÀÌÂ¡ ¾ÆÀÌÅÛÀÇ ±¸Á¶¸¦ ¹Ş´Â´Ù
int cAGING::sinRecvAgingItem(sCRAFTITEM_SERVER* pCraftItem_Server)
{
	memcpy(&sAging_Recv, pCraftItem_Server, sizeof(sCRAFTITEM_SERVER));

	return TRUE;
}

//¿¡ÀÌÂ¡ ¾ÆÀÌÅÛÀÇ °á°ú¸¦ ¼­¹ö¿¡¼­ ¹Ş´Â´Ù 
int cAGING::sinRecvAgingItemResult(sCRAFTITEM_SERVER* pCraftItem_Server)
{

	//Äù½ºÆ® ¾ÆÀÌÅÛÀÏ°æ¿ì
	if (pCraftItem_Server->DesCraftItem.sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON) {
		if (pCraftItem_Server->Result) {
			cInvenTory.DeleteInvenItemToServer(TempQuestItem.sItemInfo.CODE, TempQuestItem.sItemInfo.ItemHeader.Head, TempQuestItem.sItemInfo.ItemHeader.dwChkSum);
			memset(&TempQuestItem, 0, sizeof(sITEM));
			//Ä«¿îÆ® °»½Å
			pCraftItem_Server->DesCraftItem.sItemInfo.ItemAgingCount[0] = pCraftItem_Server->DesCraftItem.sItemInfo.ItemAgingCount[1];
			if (cInvenTory.AutoSetInvenItem(&pCraftItem_Server->DesCraftItem))
			{
				sinQuest_ChangeJob3.Monster[0] = pCraftItem_Server->DesCraftItem.sItemInfo.ItemAgingNum[0];


			}
			//Äù½ºÆ® ¸Ş¼¼Áö¸¦ º¸¿©ÁØ´Ù
			if (pCraftItem_Server->DesCraftItem.sItemInfo.ItemAgingNum[0] == 4) {
				sinQuest_ChangeJob3.State = 4;
			}
		}
		else {
			//Áö¿ü´ø ¾ÆÀÌÅÛÀ» º¹±¸ÇØÁØ´Ù
			cInvenTory.AutoSetInvenItem(&TempQuestItem);

		}
		memset(&sAging_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //¹ŞÀº ¹Í½ºÃÄ ±¸Á¶Ã¼ ÃÊ±âÈ­ 
		memset(&sAging_Send, 0, sizeof(sCRAFTITEM_SERVER)); //º¸³½ ¹Í½ºÃÄ ±¸Á¶Ã¼ ÃÊ±âÈ­ 
		NotChangeSetItemFlag = 0; //¹«±â¼ÂÆÃ Ã¼ÀÎÁö Á¦ÇÑÀ» ÃÊ±âÈ­ÇØÁØ´Ù
		return TRUE;

	}

	if (!cAging.OpenFlag) {
		AgingCheckSheltomFlag = 0; //¿¡ÀÌÂ¡ °¡´É ÇÃ·¢ ÃÊ±âÈ­ 
		AgingSheltomCnt2 = 0;
		MixItemNoCopyFlag = 0; //º¹»ç¹æÁö ÇÃ·º ÃÊ±âÈ­
		memset(&sCraftItem_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //¹ŞÀº ¹Í½ºÃÄ ±¸Á¶Ã¼ ÃÊ±âÈ­ 
		memset(&sCraftItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //º¸³½ ¹Í½ºÃÄ ±¸Á¶Ã¼ ÃÊ±âÈ­ 
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
			CheckCharForm();
			SendSaveMoney(); //±İ¾× Á¶ÀÛÀ» ¸øÇÏ°ÔÇÏ±âÀ§ÇØ È£ÃâÇÑ´Ù 
			ReformCharForm(); //ÀçÀÎÁõ 
		}
	}

	for (int y = 0; y < 4; y++) { //½©ÅÒ ÀÎµ¦½º ÃÊ±âÈ­ 
		for (int h = 0; h < 3; h++) {
			AgingLevelSheltomIndex[y][h] = 0;
		}

	}

	for (int i = 0; i < 15; i++) { //¿¡ÀÌÂ¡ ¾ÆÀÌÅÛÀ» ³¯¸°´Ù
		if (sAgingItemBackUp.AgingItem[i].Flag) {
			cInvenTory.DeleteInvenItemToServer(sAgingItemBackUp.AgingItem[i].sItemInfo.CODE,
				sAgingItemBackUp.AgingItem[i].sItemInfo.ItemHeader.Head,
				sAgingItemBackUp.AgingItem[i].sItemInfo.ItemHeader.dwChkSum);
		}

	}


	memset(&sAgingItem, 0, sizeof(sAGINGITEM));
	memset(&sAgingItemBackUp, 0, sizeof(sAGINGITEM));
	cAging.OpenFlag = 0; //¿¡ÀÌÂ¡ ÀÎÅÍÆäÀÌ½º¸¦ ´İ´Â´Ù 
	AgingCheckSheltomFlag = 0; //¿¡ÀÌÂ¡ °¡´É ÇÃ·¢ ÃÊ±âÈ­ 
	AgingSheltomCnt2 = 0;
	MixItemNoCopyFlag = 0; //º¹»ç¹æÁö ÇÃ·º ÃÊ±âÈ­
	MixCancelButtonDelayFlag = 0;
	memset(&sAging_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //¹ŞÀº ¹Í½ºÃÄ ±¸Á¶Ã¼ ÃÊ±âÈ­ 
	memset(&sAging_Send, 0, sizeof(sCRAFTITEM_SERVER)); //º¸³½ ¹Í½ºÃÄ ±¸Á¶Ã¼ ÃÊ±âÈ­ 
	cInvenTory.CheckDamage(); //ÃÖ´ëµ¥¹ÌÁö°¡ ³ª¿Ã ¹«±â·Î °è»êÇÏ¿© ¼­¹ö·Î º¸³»ÁØ´Ù 
	return TRUE;
}

//Å©·¡ÇÁÆ® ¾ÆÀÌÅÛÀÇ °á°ú¹°À» ¼­¹ö¿¡¼­ ¹Ş´Â´Ù 
int cCRAFTITEM::sinRecvCraftItemResult(sCRAFTITEM_SERVER* pCraftItem_Server)
{
	CheckMixItemCopyFlag = 0; //º¹»ç¹æÁö ÇÃ·º  
	if (!cCraftItem.OpenFlag) {
		MixItemNoCopyFlag = 0; //º¹»ç¹æÁö ÇÃ·º ÃÊ±âÈ­
		memset(&sCraftItem, 0, sizeof(sCRAFTITEM)); //±¸Á¶Ã¼ ÃÊ±âÈ­ 
		memset(&sCraftItemBackUp, 0, sizeof(sCRAFTITEM)); //±¸Á¶Ã¼ ÃÊ±âÈ­ 
		memset(&sCraftItem_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //¹ŞÀº ¹Í½ºÃÄ ±¸Á¶Ã¼ ÃÊ±âÈ­ 
		memset(&sCraftItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //º¸³½ ¹Í½ºÃÄ ±¸Á¶Ã¼ ÃÊ±âÈ­ 
		MixCancelButtonDelayFlag = 0;
		return FALSE;

	}

	int i, LineCount = 0, Temp;

	/*   µ·°ü·Ã ºÎºĞÀº »«´Ù
	if(CheckCraftMoney != pCraftItem_Server->Money) //µ·ÀÌ ¸ÂÁö ¾ÊÀ¸¸é ½ÇÆĞ·Î °£ÁÖÇÑ´Ù
		pCraftItem_Server->Result = 0;
	*/

	if (pCraftItem_Server->Result) {
		if (pCraftItem_Server->DocIndex == 2000) { //¾ÆÀÌÅÛ Àç±¸¼º½Ã½ºÅÛ 
			//¾ÆÀÌÅÛÀÏ °æ¿ì 
			sITEM TempItem;
			if (LoadItemImage(&pCraftItem_Server->DesCraftItem.sItemInfo, &TempItem)) {
				if (cInvenTory.CheckRequireItemToSet(&TempItem)) {         //°ğ¹Ù·Î ¼ÂÆÃµÉ¼öÀÖ´ÂÁö¸¦ Ã¼Å©ÇÑ´Ù  
					//haGoon¼öÁ¤================================================================================
					if (!cInvenTory.AutoSetInvenItem(&TempItem, 1)) {
						sinThrowItemToFeild(&pCraftItem_Server->DesCraftItem);
						memset(&sCraftItem, 0, sizeof(sCRAFTITEM)); //±¸Á¶Ã¼ ÃÊ±âÈ­ 
						cCraftItem.OpenFlag = 0;
						return FALSE;
					}
					//===========================================================================================
				}

			}

		}
		else {
			ShowItemCraftMessageFlag = 1; //¾ÆÀÌÅÛÀÌ Á¶ÇÕ‰çÀ»¶§ ¸Ş¼¼Áö¸¦ º¸¿©ÁØ´Ù 
			lstrcpy(szCraftItemBuff, sCraftItem_Info[pCraftItem_Server->DocIndex].Doc); //¾ÆÀÌÅÛ Á¤º¸¸¦ º¸¿©ÁØ´Ù 

			Temp = lstrlen(szCraftItemBuff);
			for (i = 0; i < Temp; i++) {
				if (szCraftItemBuff[i] == '\r') {
					LineCount++;
				}
			}

			CraftItemMessageSize.x = 14;           //¹Ú½º »çÀÌÁî 
			CraftItemMessageSize.y = ((LineCount) * 2) + 2;  //¹Ú½º »çÀÌÁî 




			//CheckCharForm(); //ÀÎÁõ 
			//sinChar->Money -= sCraftItem_Recv.DesCraftItem.sItemInfo.Price;
			//sinMinusMoney(pCraftItem_Server->DesCraftItem.sItemInfo.Price,1);
			//ReformCharForm(); //ÀçÀÎÁõ 
			//SendSaveMoney(); //±İ¾× Á¶ÀÛÀ» ¸øÇÏ°ÔÇÏ±âÀ§ÇØ È£ÃâÇÑ´Ù 
			//½ÇÆĞµç ¼º°øÀÌµç°£¿¡ ¾ÆÀÌÅÛÀ» ¿ø»ó º¹±Í ½ÃÅ²´Ù 
			//cCraftItem.OpenFlag = 0; //Ã¢À» ´İ´Â´Ù
			//cInvenTory.AutoSetInvenItem(&pCraftItem_Server->DesCraftItem); //¾ÆÀÌÅÛÀ» ¼ÂÆÃÇÑ´Ù 

			//haGoon¼öÁ¤ ============================================================================
			if (!cInvenTory.AutoSetInvenItem(&pCraftItem_Server->DesCraftItem)) {
				sinThrowItemToFeild(&pCraftItem_Server->DesCraftItem);
			}
			//=======================================================================================

			//¹«±â°¡ º¹»çµÇ¾úÀ¸¸é ¹«±â¸¦ Áö¿î´Ù
			if (sCraftItemBackUp.CraftItem[0].Flag) {
				cInvenTory.DeleteInvenItemToServer(sCraftItemBackUp.CraftItem[0].sItemInfo.CODE,
					sCraftItemBackUp.CraftItem[0].sItemInfo.ItemHeader.Head,
					sCraftItemBackUp.CraftItem[0].sItemInfo.ItemHeader.dwChkSum);
			}

			/*
			for(int y=1;y<15;y++){ //³¯¶ó°£ ½©ÅÒÀ» ¼­¹ö¿¡ ¾Ë·ÁÁØ´Ù
				if(sCraftItem.CraftItem[y].Flag)
					SendSaveThrowItem(&sCraftItem.CraftItem[y].sItemInfo);

			}
			*/
		}
		cCraftItem.OpenFlag = 0; //Ã¢À» ´İ´Â´Ù
		CheckCharForm(); //ÀÎÁõ 
		sinMinusMoney(sCraftItem.CraftItem[0].sItemInfo.Price, 1);
		ReformCharForm(); //ÀçÀÎÁõ 
		SendSaveMoney(); //±İ¾× Á¶ÀÛÀ» ¸øÇÏ°ÔÇÏ±âÀ§ÇØ È£ÃâÇÑ´Ù 
		SaveGameData();
	}
	else {


		//haGoon¼öÁ¤============================================================================
		if (!cInvenTory.AutoSetInvenItem(&sCraftItem.CraftItem[0])) { //¾ÆÀÌÅÛÀ» ¼ÂÆÃÇÑ´Ù 
			sinThrowItemToFeild(&sCraftItem.CraftItem[0]);
		}
		//=======================================================================================
		/*
		for(int y=1;y<15;y++){ //³¯¶ó°£ ½©ÅÒÀ» ¼­¹ö¿¡ ¾Ë·ÁÁØ´Ù
			if(sCraftItem.CraftItem[y].Flag)
				SendSaveThrowItem(&sCraftItem.CraftItem[y].sItemInfo);

		}
		*/

		//¹Í½ºÃÄ ÇÒ¶§ µ·À» »«´Ù 
		CheckCharForm(); //ÀÎÁõ 
		sinMinusMoney(sCraftItem.CraftItem[0].sItemInfo.Price, 1);
		ReformCharForm(); //ÀçÀÎÁõ 
		SendSaveMoney(); //±İ¾× Á¶ÀÛÀ» ¸øÇÏ°ÔÇÏ±âÀ§ÇØ È£ÃâÇÑ´Ù 

		cCraftItem.OpenFlag = 0; //Ã¢À» ´İ´Â´Ù 

		//sinPlaySound(SIN_SOUND_FAILD_MIXITEM);
		//memset(&sCraftItem_Recv.DesCraftItem,&TempItem,sizeof(sITEM)); //¾ÆÀÌÅÛ º¹±¸ 

	}

	//º¹»ç°¡µÇ¾ú°Å³ªÇÑ ¾ÆÀÌÅÛÀ» Áö¿î´Ù (½©ÅÒ¸¸Áö¿î´Ù)
	for (i = 1; i < 15; i++) {
		if (sCraftItemBackUp.CraftItem[i].Flag) {
			cInvenTory.DeleteInvenItemToServer(sCraftItemBackUp.CraftItem[i].sItemInfo.CODE,
				sCraftItemBackUp.CraftItem[i].sItemInfo.ItemHeader.Head,
				sCraftItemBackUp.CraftItem[i].sItemInfo.ItemHeader.dwChkSum);
		}

	}
	memset(&sCraftItem, 0, sizeof(sCRAFTITEM)); //±¸Á¶Ã¼ ÃÊ±âÈ­ 
	memset(&sCraftItemBackUp, 0, sizeof(sCRAFTITEM)); //±¸Á¶Ã¼ ÃÊ±âÈ­ 
	memset(&sCraftItem_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //¹ŞÀº ¹Í½ºÃÄ ±¸Á¶Ã¼ ÃÊ±âÈ­ 
	memset(&sCraftItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //º¸³½ ¹Í½ºÃÄ ±¸Á¶Ã¼ ÃÊ±âÈ­ 

	MixItemNoCopyFlag = 0; //º¹»ç¹æÁö ÇÃ·º ÃÊ±âÈ­
	MixCancelButtonDelayFlag = 0;
	return TRUE;
}

////////º¹»ç ¾ÆÀÌÅÛÀ» ¾ø¾ÖÁØ´Ù 
int cWAREHOUSE::DeleteCopyItem()
{
	int i, j;
	for (i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			for (j = 0; j < 100; j++) {
				if (i != j && sWareHouse.WareHouseItem[j].Flag) {
					if (CompareItems(&sWareHouse.WareHouseItem[i].sItemInfo, &sWareHouse.WareHouseItem[j].sItemInfo)) {
						//SendSetHackUser2(1010,sWareHouse.WareHouseItem[i].CODE); //ÇØÅ·ÇÑ À¯Àú¸¦ ½Å°íÇÑ´Ù
						//SendServerFlag7 = 1; //ÇÑ¹ø¸¸ Ã¼Å©ÇÏµµ·ÏÇÑ´Ù 
						sWareHouse.WareHouseItem[j].Flag = 0; //º¹»ç ¾ÆÀÌÅÛÀ» ³¯¸°´Ù 
						cWareHouse.ReFormWareHouse();
						break;
					}
				}
			}
		}
	}
	return TRUE;
}

////////¹Í½º¿¡¼­ º¹»çµÈ ¾ÆÀÌÅÛÀ» ¾ø¾ÖÁØ´Ù 
int CopyMixItemCheckDelete(sITEM* pItem)
{
	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (cInvenTory.InvenItem[i].Flag) {
			if (CompareItems(&cInvenTory.InvenItem[i].sItemInfo, &pItem->sItemInfo)) {
				cInvenTory.InvenItem[i].Flag = 0; //º¹»çµÈ ¾ÆÀÌÅÛÀ» Áö¿î´Ù 
			}
		}
		if (cInvenTory.InvenItemTemp[i].Flag) {
			if (CompareItems(&cInvenTory.InvenItemTemp[i].sItemInfo, &pItem->sItemInfo)) {
				cInvenTory.InvenItemTemp[i].Flag = 0; //º¹»çµÈ ¾ÆÀÌÅÛÀ» Áö¿î´Ù 
			}
		}
	}
	return TRUE;
}

//MakeÇÒ ¾ÆÀÌÅÛÀÌ ¸Â´Â°¡ ÄÚµå¸¦ È®ÀÎÇÑ´Ù
int sinMakeItemCheck()
{
	int ResultCount = 0;
	int ResultCountCheck = 0;
	DWORD TempCODE[15] = { 0, };
	int  i = 0;
	//ÄÚµå º¹»ç 
	for (i = 0; i < MAX_MAKEITEM_INFO; i++) {
		for (int p = 0; p < 15; p++) {  //ÄÚµå¸¦ ÇÑ¹ø¾¿ ÃÊ±âÈ­ÇØÁØ´Ù 
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
/*							PostBox ½Ã½ºÅÛ
/*****************************************************************************/
int sinPosBoxNpc() { return TRUE; }
//haGoon¾ÆÀÌÅÛ Àç±¸¼ºÀ» Ã¼Å©ÇÑ´Ù.----------------------------------------------------------------------------
int cCRAFTITEM::haCheckReconItem()
{
	int cnt[MAX_SEEL_COUNT] = { 0 }; // ¹ÚÀç¿ø - Å×ÀÌ¿ÍÁî ¾Á Ãß°¡ (3Á¾·ù -> 4Á¾·ù)
	int CheckNotReconItem = 0;
	int CheckBlockCount = 0;
	cCraftItem.iReconItemFlag = 0;

	//¾Á¾ÆÀÌÅÛÀÌ ¾Æ´Ñ°ÍÀº ´Ù Ã¼Å©ÇÑ´Ù.
	for (int k = 1; k < 14; k++) { // ¹ÚÀç¿ø - ¸®½ºÆå ½ºÅæ(sCraftItem.CraftItem[14]´Â ¸®½ºÆå ½ºÅæÀÌ¹Ç·Î Ã¼Å©¿¡¼­ Á¦¿ÜÇÑ´Ù) for(int k=1;k<15;k++) -> for(int k=1;k<14;k++)
		if (sCraftItem.CraftItem[k].Flag) {
			if ((sCraftItem.CraftItem[k].CODE & sinITEM_MASK2) == sinSE1)
				CheckBlockCount++;   //¾Á ¾ÆÀÌÅÛ Ã¼Å©
			else
				CheckNotReconItem++; //¾Á¿Ü¾ÆÀÌÅÛ Ã¼Å©
		}
		else
			memset(&sCraftItem.CraftItem[k], 0, sizeof(sITEM));  //¼­¹ö ÇØÅ· ¿¡·¯ ¶§¹®¿¡ ÀÓ½Ã ¹æÆíÀÌ´Ù.
	}
	//¾ÁÀ» Ã¼Å©ÇÑ´Ù.
	for (int i = 1; i < 14; i++) { // ¹ÚÀç¿ø - ¸®½ºÆå ½ºÅæ(sCraftItem.CraftItem[14]´Â ¸®½ºÆå ½ºÅæÀÌ¹Ç·Î Ã¼Å©¿¡¼­ Á¦¿ÜÇÑ´Ù) for(int k=1;k<15;k++) -> for(int k=1;k<14;k++)
		if ((sCraftItem.CraftItem[i].CODE & sinITEM_MASK2) == sinSE1 && sCraftItem.CraftItem[i].Flag) {
			for (int k = 0; k < MAX_SEEL_COUNT; k++) { // ¹ÚÀç¿ø - Å×ÀÌ¿ÍÁî ¾Á Ãß°¡ (3Á¾·ù -> 4Á¾·ù)
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
*Desc: ºó¹è¿­À» ´ç°ÜÁØ´Ù.
*-----------------------------------------------------------------------------*/
int cCRAFTITEM::haCraftSortItem(sCRAFTITEM* pCraftItem)
{
	//ºó ¾ÆÀÌÅÛ ¹è¿­À» ´ç°ÜÁØ´Ù.
	int i;
	for (i = 1; i < 13; i++) {
		if (!pCraftItem->CraftItem[i].Flag) {
			pCraftItem->CraftItem[i] = pCraftItem->CraftItem[i + 1];
			pCraftItem->CraftItem[i + 1].Flag = 0;
		}
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*
*Desc: ÀÎº¥Åä¸®ÀÇ 22*22Å©±âÀÇ ºóÄ­À» ±¸ÇØ ¸®ÅÏÇÑ´Ù.
*-----------------------------------------------------------------------------*/
int cCRAFTITEM::GetCraftCheckEmptyArea()
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;
	int StartX = 23;  //ÀÎº¥ ¹Ú½ºÀÇ ½ÃÀÛÁ¡ X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - 22; //¾ÆÀÌÅÛÀÌ µé¾î°¥¼öÀÖ´Â °¡Àå ³¡ XÁÂÇ¥ 
	cy = (22 * INVENTORY_BOX_COL) - 22; //¾ÆÀÌÅÛÀÌ µé¾î°¥¼öÀÖ´Â °¡Àå ³¡ YÁÂÇ¥

	int count = 0;
	//ÄÚµå°¡ °°Áö ¾Ê°Å³ª ¾ÆÀÌÅÛÀÌ ºñ¾îÀÖÁö ¾ÊÀº °÷¿¡ ¼ÂÆÃÇÒ¶§ °Ë»öÇØ¼­ À§Ä¡¸¦ Ã£¾Æ³½´Ù 
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

// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â ¾ÆÀÌÅÛÀÌ µé¾î°¥ °ø°£À» Ã¼Å©ÇÏ´Â ÇÔ¼ö ±¸Çö
int cMIXTURERESET::SetMixtureItemResetAreaCheck(sITEM* pItem)
{
	TradeColorIndex = 0;			// ÃÊ±âÈ­
	TradeCrashItemIndex[0] = 0;		// Ãæµ¹µÈ ¾ÆÀÌÅÛ Index ÃÊ±âÈ­ 
	cTrade.InitTradeColorRect();	// ¿µ¿ª ÃÊ±âÈ­

	if ((pItem->CODE & sinITEM_MASK2) == sinBI1)		// ºô¸µ ¾ÆÀÌÅÛÀÌ°í...
	{
		// ¹Í½ºÃÄ ¸®¼Â ½ºÅæÀÌ ¾Æ´Ï¸é ¸®ÅÏ
		if ((pItem->sItemInfo.CODE & sinITEM_MASK3) != sin89)
			return FALSE;

		// ½ºÅæÀÌ À§Ä¡ÇÒ ÁÂÇ¥¸¦ °è»êÇØ ³½´Ù.
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
					//ÀÎº¥Åä¸® ¹Ú½º¿¡ µé¾î°¬´ÂÁö¸¦ Ã¼Å© 
					TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
					TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
					TradeColorRect.right = pItem->w;
					TradeColorRect.bottom = pItem->h;

					if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11)
					{
						cTrade.InitTradeColorRect(); //ÄÃ·¯ ¹Ú½º RECT ÃÊ±âÈ­ (ÄÃ·¯ ¹Ú½º RECTÁß ÇÏ³ª¶óµÎ 0 ÀÌ¸é ¼ÂÆÃÇÒ¼ö ¾ø´Ù 
						return FALSE;
					}

					// ¾ÆÀÌÅÛÀÇ À§Ä¡¸¦ ÁöÁ¤ÇÑ´Ù.
					pItem->SetX = TradeColorRect.left;
					pItem->SetY = TradeColorRect.top;
					pItem->ItemPosition = 0; //¹è¿­ÀÇ ÀÎµ¦½º¸¦ ÀúÀåÇÑ´Ù

					TradeColorIndex = SET_ITEM_CHECK_COLOR;		//¼ÂÆÃÇÒ¿µ¿ª

					// °ãÄ¡´Â ¾ÆÀÌÅÛÀÌ ÀÖ³ª Ã¼Å© 
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
		if (pItem->sItemInfo.ItemKindCode == ITEM_KIND_CRAFT)
		{
			TradeColorIndex = 0;				//ÃÊ±âÈ­   (Æ®·¹ÀÌµå Ã¢°ú Ã¢°í°¡ °°ÀÌ ¾´´Ù)
			TradeCrashItemIndex[0] = 0;		//Ãæµ¹µÈ ¾ÆÀÌÅÛ Index ÃÊ±âÈ­ 
			cTrade.InitTradeColorRect();	//ÃÊ±âÈ­

			// ¹Í½ºÃÄ°¡ ¼öÇàµÈ ¾ÆÀÌÅÛÀÌ µé¾î°¥ ÀÚ¸®ÀÇ ÁÂÇ¥¸¦ ±¸ÇÔ
			TradeStartX = 58;
			TradeStartY = 193 + sinInterHeight2;
			TradeEndX = TradeStartX + (3 * 22);
			TradeEndY = TradeStartY + (4 * 22);

			//ÀÎº¥Åä¸® ¹Ú½º¿¡ µé¾î°¬´ÂÁö¸¦ Ã¼Å© 
		}
		else
		{
			TradeStartX = 58;
			TradeStartY = 193 + sinInterHeight2;
			TradeEndX = TradeStartX + (3 * 22);
			TradeEndY = TradeStartY + (4 * 22);
			//	TradeColorIndex = NOT_SETTING_COLOR;
			//	return FALSE;
		}
	}

	return TRUE;
}

// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â ¾ÆÀÌÅÛÀÇ Á¤º¸ Ã¼Å©
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

// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â ¾ÆÀÌÅÛÀÇ Á¤º¸ ´Ù½Ã Ã¼Å©
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

// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â ¾ÆÀÌÅÛÀ» Áı´Â ÇÔ¼ö
int cMIXTURERESET::PickUpMixtureResetItem(int x, int y, int PickUpFlag, int Kind)
{
	int i = 0;
	SelectTradeItemIndex = 0;    //ÁıÀ» ¾ÆÀÌÅÛ 

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
							MixtureResetCheckFlag = 0;	// Á¶ÇÕ ¹öÆ° ºñÈ°¼ºÈ­

						// ½ºÅæÀÌ ÀÖ´Â »óÅÂ¿¡¼± ¹Í½ºÃÄ µÈ ¾ÆÀÌÅÛ µé±â ºÒ°¡
						if (sMixtureResetItem.MixtureResetItem[1].Flag && i == 0)
						{

							return FALSE;
						}

						CheckMixtureResetItemForm();
						memcpy(&MouseItem, &sMixtureResetItem.MixtureResetItem[i], sizeof(sITEM));
						sMixtureResetItem.MixtureResetItem[i].Flag = 0;
						memset(&sMixtureResetItem.MixtureResetItem[i], 0, sizeof(sITEM));  //¼­¹ö ÇØÅ· ¿¡·¯ ¶§¹®¿¡ ÀÓ½Ã ¹æÆíÀÌ´Ù.

						//sinPlaySound(sMixtureResetItem.MixtureResetItem[i].SoundIndex);
						TradeColorIndex = 0;
						ReformMixtureResetItem();

						return TRUE;
					}
					else
					{
						SelectTradeItemIndex = i + 1;
						TradeColorIndex = SELECT_ITEM_COLOR;
						sinShowItemInfoFlag = 1; //¾ÆÀÌÅÛÀÇ Á¤º¸¸¦ º¸¿©ÁØ´Ù 
						cItem.ShowItemInfo(&sMixtureResetItem.MixtureResetItem[i], 2);
					}
				}
			}
		}
	}

	return FALSE;
}

// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â ¾ÆÀÌÅÛÀ» Ã¢¿¡ À§Ä¡½ÃÅ°µµ·Ï ÇÏ´Â ÇÔ¼ö
int cMIXTURERESET::LastSetMixtureResetItem(sITEM* pItem, int kind)
{
	//CheckMixtureResetItemForm();

	if (kind == 1)
	{
		// ¹Í½ºÃÄµÈ ¾ÆÀÌÅÛÀº ¿Ã·ÁÁ® ÀÖ°í, ½ºÅæÀº ³õ¿©ÀÖÁö ¾Ê´Ù¸é...
		if (sMixtureResetItem.MixtureResetItem[0].Flag && !sMixtureResetItem.MixtureResetItem[1].Flag)
		{
			memcpy(&sMixtureResetItem.MixtureResetItem[1], pItem, sizeof(sITEM));
			sMixtureResetItem.MixtureResetItem[1].x = pItem->SetX;
			sMixtureResetItem.MixtureResetItem[1].y = pItem->SetY;
			pItem->Flag = 0; //ÃÊ±âÈ­ 
			//sinPlaySound(sMixtureResetItem.MixtureResetItem[1].SoundIndex);
			//ReformMixtureResetItem();
			return TRUE;
		}
	}
	else if (kind == 2)		// ¹Í½ºÃÄµÈ ¾ÆÀÌÅÛÀÌ¸é...
	{
		// ¹Í½ºÃÄµÈ ¾ÆÀÌÅÛÀÌ ¿Ã·ÁÁ® ÀÖÁö ¾Ê´Ù¸é...
		if (!sMixtureResetItem.MixtureResetItem[0].Flag)
		{
			memcpy(&sMixtureResetItem.MixtureResetItem[0], pItem, sizeof(sITEM));
			sMixtureResetItem.MixtureResetItem[0].x = pItem->SetX;
			sMixtureResetItem.MixtureResetItem[0].y = pItem->SetY;
			pItem->Flag = 0; //ÃÊ±âÈ­ 
			//sinPlaySound(sMixtureResetItem.MixtureResetItem[0].SoundIndex);
			//ReformMixtureResetItem();
			return TRUE;
		}
	}

	return TRUE;
}

// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼ÂÀ» Ãë¼ÒÇÑ´Ù.
int cMIXTURERESET::CancelMixtureItemReset(int Flag)
{
	// º¹»ç ¹æÁö ÇÃ·¢ÀÌ È°¼ºÈ­ µÇ¾î ÀÖ´Ù¸é....
	if (MixItemNoCopyFlag)
		return FALSE;

	// ¹Í½ºÃÄ ¸®¼Â¿¡ ¿Ã·È´ø ¾ÆÀÌÅÛÀÌ ÀÖ´Ù¸é ÀÎº¥ÀÇ ºó °ø°£À¸·Î ÀÚµ¿ ¼ÂÆÃÇÏ°í, Áö¿î´Ù.
	if (sMixtureResetItem.MixtureResetItem[0].Flag)
	{
		if (!cInvenTory.AutoSetInvenItem(&sMixtureResetItem.MixtureResetItem[0]))
			sinThrowItemToFeild(&sMixtureResetItem.MixtureResetItem[0]);

		sMixtureResetItem.MixtureResetItem[0].Flag = 0;
	}

	MixtureResetCheckFlag = 0;	// ¹Í½ºÃÄ ¸®¼Â ºÒ°¡´É

	// ¹Í½ºÃÄ ¸®¼Â Ã¢À» ´İ´Â´Ù.
	if (!Flag)
		cMixtureReset.OpenFlag = 0;

	ResetInvenItemCode();	// ÀÎº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­

	return TRUE;
}

// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼ÂµÉ ¾ÆÀÌÅÛÀ» ¼­¹ö·Î Àü¼ÛÇÑ´Ù.
int cMIXTURERESET::sinSendMixtureItemReset()
{
	if (MixItemNoCopyFlag)
		return FALSE;

	memcpy(&sMixtureResetItemBackUp, &sMixtureResetItem, sizeof(sMIXTURE_RESET_ITEM));	// º¹»ç¸¦ Ã¼Å©ÇÏ±âÀ§ÇÑ ¹é¾÷º»À» ¸¸µç´Ù
	MixItemNoCopyFlag = 1;	//¾ÆÀÌÅÛÀÌ º¹»çµÇ´Â°É ¹æÁöÇÏ±âÀ§ÇØ ÇÃ·ºÀ»ÁØ´Ù
	memset(&sMixtureReset_Send, 0, sizeof(sMIXTURE_RESET_ITEM_SERVER));	// ¼­¹ö¿¡ º¸³¾ ±¸Á¶Ã¼ ÃÊ±âÈ­

	// ¹Í½ºÃÄ ¸®¼Â ½ÃÅ³ ¾ÆÀÌÅÛÀ» ÀúÀå
	if (sMixtureResetItem.MixtureResetItem[0].Flag)
		sMixtureReset_Send.DesMixtureResetItem = sMixtureResetItem.MixtureResetItem[0];

	// ¹Í½ºÃÄ ¸®¼Â ½ºÅæÀ» ÀúÀå
	if (sMixtureResetItem.MixtureResetItem[1].Flag)
	{
		sMixtureReset_Send.dwMixtureResetStoneItemCode = sMixtureResetItem.MixtureResetItem[1].CODE;
		sMixtureReset_Send.dwMixtureResetStoneCheckSum = sMixtureResetItem.MixtureResetItem[1].sItemInfo.ItemHeader.dwChkSum;
		sMixtureReset_Send.dwMixtureResetStoneHead = sMixtureResetItem.MixtureResetItem[1].sItemInfo.ItemHeader.Head;
	}

	// ÀÎµ¦½º¸¦ º¸³½´Ù 
	CraftItemSendServerIndex++;
	sMixtureReset_Send.Index = CraftItemSendServerIndex;

	// ÀúÀåµÈ ±¸Á¶Ã¼¸¦ ¼­¹ö¿¡ º¸³½´Ù 
	SendMixtureResetItemToServer(&sMixtureReset_Send);


	return TRUE;
}

// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼ÂµÈ ¾ÆÀÌÅÛÀ» Recv¿ë º¯¼ö·Î º¹»ç
int	cMIXTURERESET::sinRecvMixtureItemReset(sMIXTURE_RESET_ITEM_SERVER* pMixtureResetItem)
{
	memcpy(&sMixtureReset_Recv, pMixtureResetItem, sizeof(sMIXTURE_RESET_ITEM_SERVER));

	return TRUE;
}

// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â ¾ÆÀÌÅÛ ÃÖÁ¾ °á°ú Ã³¸®
int cMIXTURERESET::sinRecvMixtureItemResetResult(sMIXTURE_RESET_ITEM_SERVER* pMixtureResetItem_Server)
{
	CheckMixItemCopyFlag = 0; //º¹»ç¹æÁö ÇÃ·º  

	// ¹Í½ºÃÄ¸®¼Â Ã¢ÀÌ ¿­·ÁÀÖÁö ¾Ê´Ù¸é ¸ğµÎ ÃÊ±âÈ­
	if (!cMixtureReset.OpenFlag)
	{
		MixItemNoCopyFlag = 0; //º¹»ç¹æÁö ÇÃ·º ÃÊ±âÈ­
		memset(&sMixtureResetItem, 0, sizeof(sMIXTURE_RESET_ITEM));			//±¸Á¶Ã¼ ÃÊ±âÈ­ 
		memset(&sMixtureResetItemBackUp, 0, sizeof(sMIXTURE_RESET_ITEM));	//±¸Á¶Ã¼ ÃÊ±âÈ­ 
		memset(&sMixtureReset_Recv, 0, sizeof(sMIXTURE_RESET_ITEM_SERVER)); //¹ŞÀº ¹Í½ºÃÄ¸®¼Â ±¸Á¶Ã¼ ÃÊ±âÈ­ 
		memset(&sMixtureReset_Send, 0, sizeof(sMIXTURE_RESET_ITEM_SERVER)); //º¸³½ ¹Í½ºÃÄ¸®¼Â ±¸Á¶Ã¼ ÃÊ±âÈ­ 
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

			CraftItemMessageSize.x = 14;				//¹Ú½º »çÀÌÁî 
			CraftItemMessageSize.y = ((LineCount) * 2) + 2; //¹Ú½º »çÀÌÁî 

			if (cInvenTory.CheckRequireItemToSet(&TempItem))
			{
				//°ğ¹Ù·Î ¼ÂÆÃµÉ¼öÀÖ´ÂÁö¸¦ Ã¼Å©ÇÑ´Ù  
				if (!cInvenTory.AutoSetInvenItem(&TempItem, 1))
				{
					sinThrowItemToFeild(&pMixtureResetItem_Server->DesMixtureResetItem);
				}
			}

		}



		CheckCharForm();	//ÀÎÁõ 
		ReformCharForm();	//ÀçÀÎÁõ 
		SendSaveMoney();	//±İ¾× Á¶ÀÛÀ» ¸øÇÏ°ÔÇÏ±âÀ§ÇØ È£ÃâÇÑ´Ù 
		SaveGameData();
		cMixtureReset.OpenFlag = 0; //Ã¢À» ´İ´Â´Ù
	}
	else
	{
		CheckCharForm();	//ÀÎÁõ 
		ReformCharForm();	//ÀçÀÎÁõ 
		SendSaveMoney();	//±İ¾× Á¶ÀÛÀ» ¸øÇÏ°ÔÇÏ±âÀ§ÇØ È£ÃâÇÑ´Ù 
		cMixtureReset.OpenFlag = 0; //Ã¢À» ´İ´Â´Ù

	}

	//º¹»ç°¡µÇ¾ú°Å³ªÇÑ ¾ÆÀÌÅÛÀ» Áö¿î´Ù
	for (i = 0; i < 2; i++)
	{
		if (sMixtureResetItemBackUp.MixtureResetItem[i].Flag)
		{
			cInvenTory.DeleteInvenItemToServer(sMixtureResetItemBackUp.MixtureResetItem[i].sItemInfo.CODE,
				sMixtureResetItemBackUp.MixtureResetItem[i].sItemInfo.ItemHeader.Head,
				sMixtureResetItemBackUp.MixtureResetItem[i].sItemInfo.ItemHeader.dwChkSum);
		}

	}

	memset(&sMixtureResetItem, 0, sizeof(sMIXTURE_RESET_ITEM));			//±¸Á¶Ã¼ ÃÊ±âÈ­ 
	memset(&sMixtureResetItemBackUp, 0, sizeof(sMIXTURE_RESET_ITEM));		//±¸Á¶Ã¼ ÃÊ±âÈ­ 
	memset(&sMixtureReset_Recv, 0, sizeof(sMIXTURE_RESET_ITEM_SERVER));	//¹ŞÀº ¹Í½ºÃÄ¸®¼Â ±¸Á¶Ã¼ ÃÊ±âÈ­ 
	memset(&sMixtureReset_Send, 0, sizeof(sMIXTURE_RESET_ITEM_SERVER));	//º¸³½ ¹Í½ºÃÄ¸®¼Â ±¸Á¶Ã¼ ÃÊ±âÈ­ 

	MixItemNoCopyFlag = 0; //º¹»ç¹æÁö ÇÃ·º ÃÊ±âÈ­
	MixCancelButtonDelayFlag = 0;


	return TRUE;
}

// pluto Á¦·Ã
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
	memset(&S_smeltingItem, 0, sizeof(SSmeltingItem)); //¾ÆÀÌÅÛÀ» ´Ù ³¯·Á¹ö¸°´Ù 
	ResetInvenItemCode();				//ÀÎº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­

	memset(&sSmeltingItem_Recv, 0, sizeof(sSMELTINGITEM_SERVER)); //¹ŞÀº ¹Í½ºÃÄ ±¸Á¶Ã¼ ÃÊ±âÈ­  (ÀÏ´Ü °°ÀÌÇØÁØ´Ù)
	memset(&sSmeltingItem_Send, 0, sizeof(sSMELTINGITEM_SERVER)); //º¸³½ ¹Í½ºÃÄ ±¸Á¶Ã¼ ÃÊ±âÈ­ 

	return TRUE;
}

// pluto Á¦·Ã
int CSmeltingItem::SetSmeltingItemAreaCheck(sITEM* pItem, int Kind)
{
	//±¤¼®	  ÄÉÀÌ¾Æ½º ÀÌÇÁ¸®¾Æ ÆæÅ¸³ª  ¹Ù·ç³ª µµ·ç³×¾Æ »çÆ¼Äí¾Æ ¸Ş·çÄ«¹Ù
	int SmeltingPriceIndexOre = 25000;

	//¼öÁ¤    ÄÉÀÌ¾Æ½º ÀÌÇÁ¸®¾Æ ÆæÅ¸³ª  ¹Ù·ç³ª µµ·ç³×¾Æ »çÆ¼Äí¾Æ ¸Ş·çÄ«¹Ù ´«°áÁ¤ ½º³ë¿ìÇÃ¶ó¿ö ÇÏ¾á´«¹°
	int SmeltingPriceIndexCrystal = 50000;
	//	int SmeltingPriceIndexCrystal = 10000; // Àåº° - ÀÏº» ¿äÃ»À¸·Î ÀÓ½Ã·Î ¸¸¿ø
		// Àåº° - ´ëÀåÀåÀÌÀÇ È¥
	DWORD SmeltingCode[] = { sin01, sin02, sin03, sin04, sin05, sin06, sin07, sin08, sin09, sin10, sin11, sin12, sin13, sin14 };

	int i, j;
	TradeColorIndex = 0; // ÃÊ±âÈ­
	TradeCrashItemIndex[0] = 0; //Ãæµ¹µÈ ¾ÆÀÌÅÛ Index ÃÊ±âÈ­ 
	cTrade.InitTradeColorRect(); //ÃÊ±âÈ­

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
		//		{ //ÀÎº¥Åä¸® ¹Ú½º¿¡ µé¾î°¬´ÂÁö¸¦ Ã¼Å© 
		//			TradeColorRect.left =  TradeStartX +( ( ( i - TradeStartX) /22)*22) ; 
		//			TradeColorRect.top  =  TradeStartY +( ( ( j - TradeStartY) /22)*22) ; 
		//			TradeColorRect.right  = pItem->w;
		//			TradeColorRect.bottom = pItem->h;
		//			if(TradeEndX < (TradeColorRect.left + TradeColorRect.right)-11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom)-11)
		//			{
		//				cTrade.InitTradeColorRect(); //ÄÃ·¯ ¹Ú½º RECT ÃÊ±âÈ­ (ÄÃ·¯ ¹Ú½º RECTÁß ÇÏ³ª¶óµÎ 0 ÀÌ¸é ¼ÂÆÃÇÒ¼ö ¾ø´Ù 
		//				return FALSE;
		//			}

		//			pItem->SetX = TradeColorRect.left;
		//			pItem->SetY = TradeColorRect.top;
		//			pItem->ItemPosition = 0; //¹è¿­ÀÇ ÀÎµ¦½º¸¦ ÀúÀåÇÑ´Ù 
		//			TradeColorIndex = SET_ITEM_CHECK_COLOR; //¼ÂÆÃÇÒ¿µ¿ª
		//			if( !S_smeltingItem.SmeltingItem[0].Flag )
		//			{
		//				TradeColorIndex = NOT_SETTING_COLOR;
		//				return FALSE;
		//			}
		//			if(Kind == 0)
		//			{ //¹Í½ºÃÄ 
		//				if(TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect,0,4))
		//				{  //°ãÄ¡´Â ¾ÆÀÌÅÛÀÌ ÀÖ³ª Ã¼Å© 
		//					TradeColorIndex = NOT_SETTING_COLOR;
		//					return FALSE;

		//				}
		//				return TRUE;
		//			}
		//		}

		//	}

		//}


		//int SmeltingSheltomAray[4][3] = { 0, 0, 0,		// ½©ÅÒ °¹¼ö ´Ã¾î ³ª¸é ¾²ÀÚ
		//								  0, 0, 0,
		//								  0, 0, 0,
		//								  0, 0, 0 };

		int SmeltingSheltomIndex = 0;
		SmeltingSheltomIndex = GetSheltomIndex(S_smeltingItem.SmeltingItem[0].CODE);
		int Smelting_SheltomIndex = 0;
		DWORD SheltomAray[MAX_SHELTOM] = { sin01, sin02, sin03, sin04, sin05, sin06, sin07, sin08, sin09, sin10, sin11, sin12, sin13, sin14 };

		for (int q = 0; q < MAX_SHELTOM; q++)
		{
			if (SheltomAray[q] == (pItem->sItemInfo.CODE & sinITEM_MASK3))
			{
				Smelting_SheltomIndex = q + 1;
				break;
			}
		}
		//for(int p = 0 ; p < 4 ; p++)			// ½©ÅÒ °¹¼ö ´Ã¾î ³ª¸é ¾²ÀÚ
		{
			//for(int m=0; m < 3 ; m++)
			{
				if (SmeltingSheltomIndex == Smelting_SheltomIndex)
				{
					TradeStartX = AgingLevelSheltomXY[0][0].x + 22;			// ÁÂÇ¥´Â °°¾Æ¼­ °Á ›§À½
					TradeStartY = AgingLevelSheltomXY[0][0].y + 30;
					TradeEndX = TradeStartX + 22;
					TradeEndY = TradeStartY + 22;
					for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22)
					{
						for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22)
						{
							if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j)
							{ //ÀÎº¥Åä¸® ¹Ú½º¿¡ µé¾î°¬´ÂÁö¸¦ Ã¼Å© 
								TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
								TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
								TradeColorRect.right = pItem->w;
								TradeColorRect.bottom = pItem->h;
								if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11)
								{
									cTrade.InitTradeColorRect(); //ÄÃ·¯ ¹Ú½º RECT ÃÊ±âÈ­ (ÄÃ·¯ ¹Ú½º RECTÁß ÇÏ³ª¶óµÎ 0 ÀÌ¸é ¼ÂÆÃÇÒ¼ö ¾ø´Ù 
									return FALSE;
								}

								pItem->SetX = TradeColorRect.left;
								pItem->SetY = TradeColorRect.top;
								pItem->ItemPosition = 0; //¹è¿­ÀÇ ÀÎµ¦½º¸¦ ÀúÀåÇÑ´Ù

								TradeColorIndex = SET_ITEM_CHECK_COLOR; //¼ÂÆÃÇÒ¿µ¿ª
								if (!S_smeltingItem.SmeltingItem[0].Flag)
								{
									TradeColorIndex = NOT_SETTING_COLOR;
									return FALSE;
								}

								if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 4))
								{  //°ãÄ¡´Â ¾ÆÀÌÅÛÀÌ ÀÖ³ª Ã¼Å© 
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
		TradeColorIndex = 0; //ÃÊ±âÈ­   (Æ®·¹ÀÌµå Ã¢°ú Ã¢°í°¡ °°ÀÌ ¾´´Ù)
		TradeCrashItemIndex[0] = 0; //Ãæµ¹µÈ ¾ÆÀÌÅÛ Index ÃÊ±âÈ­ 
		cTrade.InitTradeColorRect(); //ÃÊ±âÈ­

		//TradeStartX = 58;
		//TradeStartY = 193+sinInterHeight2;
		//TradeEndX = TradeStartX+(3*22);
		//TradeEndY = TradeStartY+(3*22);

		TradeStartX = 69;
		TradeStartY = 212 + sinInterHeight2;
		TradeEndX = TradeStartX + (2 * 18);
		TradeEndY = TradeStartY + (2 * 18);
	}
	return TRUE;
}

// pluto Á¦·Ã
int CSmeltingItem::LastSetSmeltingItem(sITEM* pItem, int Kind)
{
	if (Kind == 0)
	{
		CheckSmeltingItemForm();
		if ((pItem->CODE & sinITEM_MASK2) == sinOS1)
		{ //½©ÅÒÀÏ °æ¿ì
			//for(int j=12 ; j < 24 ; j++)
			//{
			//	if(!S_smeltingItem.SmeltingItem[j].Flag)
			//	{
			//		memcpy(&S_smeltingItem.SmeltingItem[j],pItem,sizeof(sITEM));
			//		S_smeltingItem.SmeltingItem[j].x = pItem->SetX;
			//		S_smeltingItem.SmeltingItem[j].y = pItem->SetY;
			//		pItem->Flag = 0; //ÃÊ±âÈ­ 
			//		//sinPlaySound(S_smeltingItem.SmeltingItem[j].SoundIndex);
			//		ReFormSmeltingItem();
			//		return TRUE;
			//	}
			//}
			if (!S_smeltingItem.SmeltingItem[4].Flag)	// pluto ½©ÅÒ ÇÑ°³¸¸ ³õ´Â´Ù 
			{
				memcpy(&S_smeltingItem.SmeltingItem[4], pItem, sizeof(sITEM));
				S_smeltingItem.SmeltingItem[4].x = pItem->SetX;
				S_smeltingItem.SmeltingItem[4].y = pItem->SetY;
				pItem->Flag = 0; //ÃÊ±âÈ­ 
				//sinPlaySound(S_smeltingItem.SmeltingItem[4].SoundIndex);
				ReFormSmeltingItem();
				return TRUE;
			}

		}
		else
		{
			int k = 3; // Àåº° - Á¦·Ã ½Ã½ºÅÛ ¼öÁ¤
			//if( (pItem->CODE & sinITEM_MASK2) == sinPR1 )
			//{
			//	k = 5;		// ±¤¼®
			//}
			//else if( (pItem->CODE & sinITEM_MASK2) == sinPR2 )
			//{
			//	k = 3;		// ¼öÁ¤
			//}

			for (int i = 0; i < k; i++)
			{
				if (!S_smeltingItem.SmeltingItem[i].Flag)		// ±¤¼®, ¼öÁ¤
				{
					memcpy(&S_smeltingItem.SmeltingItem[i], pItem, sizeof(sITEM));
					S_smeltingItem.SmeltingItem[i].x = pItem->SetX;
					S_smeltingItem.SmeltingItem[i].y = pItem->SetY;
					pItem->Flag = 0; //ÃÊ±âÈ­ 
					//sinPlaySound(sCraftItem.CraftItem[i].SoundIndex);
					ReFormSmeltingItem();
					CheckSmeltingPrice();
					return TRUE;
				}
			}
		}
	}
	return TRUE;
}

// pluto Á¦·Ã
int CSmeltingItem::CheckSmeltingItemForm()
{ //Ã¼Å©  
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

// pluto Á¦·Ã
int CSmeltingItem::ReFormSmeltingItem()
{ //ÀÎÁõ 

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

// pluto Á¦·Ã ¾ÆÀÌÅÛ Áı±â
int CSmeltingItem::PickUpSmeltingItem(int x, int y, int PickUpFlag, int Kind)
{
	int i = 0;
	SelectTradeItemIndex = 0;    //ÁıÀ» ¾ÆÀÌÅÛ 
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

							return FALSE;
						}
						CheckSmeltingItemForm();
						memcpy(&MouseItem, &S_smeltingItem.SmeltingItem[i], sizeof(sITEM));
						S_smeltingItem.SmeltingItem[i].Flag = 0;
						memset(&S_smeltingItem.SmeltingItem[i], 0, sizeof(sITEM));  //¼­¹ö ÇØÅ· ¿¡·¯ ¶§¹®¿¡ ÀÓ½Ã ¹æÆíÀÌ´Ù.

						//sinPlaySound(S_smeltingItem.SmeltingItem[i].SoundIndex);
						TradeColorIndex = 0;
						ReFormSmeltingItem();
						return TRUE;
					}
					else
					{
						SelectTradeItemIndex = i + 1;
						TradeColorIndex = SELECT_ITEM_COLOR;
						sinShowItemInfoFlag = 1; //¾ÆÀÌÅÛÀÇ Á¤º¸¸¦ º¸¿©ÁØ´Ù 
						cItem.ShowItemInfo(&S_smeltingItem.SmeltingItem[i], 2);
					}
				}
			}
		}
	}
	return FALSE;
}

// pluto Á¦·Ã
int CSmeltingItem::sinSendSmeltingItem()
{
	if (MixItemNoCopyFlag)
	{
		return FALSE;
	}

	int i = 0;

	//º¹»ç¸¦ Ã¼Å©ÇÏ±âÀ§ÇÑ ¹é¾÷º»À» ¸¸µç´Ù
	memcpy(&sSmeltingItemBackUp, &S_smeltingItem, sizeof(SSmeltingItem));
	MixItemNoCopyFlag = 1; //¾ÆÀÌÅÛÀÌ º¹»çµÇ´Â°É ¹æÁöÇÏ±âÀ§ÇØ ÇÃ·ºÀ»ÁØ´Ù
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

	CraftItemSendServerIndex++; //ÀÎµ¦½º¸¦ º¸³½´Ù 
	sSmeltingItem_Send.Index = CraftItemSendServerIndex;
	//ÀúÀåµÈ ±¸Á¶Ã¼¸¦ ¼­¹ö¿¡ º¸³½´Ù 


	SendSmeltingItemToServer(&sSmeltingItem_Send);

	return TRUE;
}

int CSmeltingItem::sinRecvSmeltingItem(sSMELTINGITEM_SERVER* pSmeltingItem_Server)
{
	memcpy(&sSmeltingItem_Recv, pSmeltingItem_Server, sizeof(sSMELTINGITEM_SERVER));

	return TRUE;
}

// pluto Á¦·Ã ¾ÆÀÌÅÛÀÇ °á°ú¹°À» ¼­¹ö¿¡¼­ ¹Ş´Â´Ù 
int CSmeltingItem::sinRecvSmeltingItemResult(sSMELTINGITEM_SERVER* pSmeltingItem_Server)
{
	CheckMixItemCopyFlag = 0; //º¹»ç¹æÁö ÇÃ·º  
	if (!SmeltingItem.OpenFlag)
	{
		MixItemNoCopyFlag = 0; //º¹»ç¹æÁö ÇÃ·º ÃÊ±âÈ­
		memset(&S_smeltingItem, 0, sizeof(SSmeltingItem)); //±¸Á¶Ã¼ ÃÊ±âÈ­ 
		memset(&sSmeltingItemBackUp, 0, sizeof(SSmeltingItem)); //±¸Á¶Ã¼ ÃÊ±âÈ­ 
		memset(&sSmeltingItem_Recv, 0, sizeof(sSMELTINGITEM_SERVER)); //¹ŞÀº Á¦·Ã ±¸Á¶Ã¼ ÃÊ±âÈ­ 
		memset(&sSmeltingItem_Send, 0, sizeof(sSMELTINGITEM_SERVER)); //º¸³½ Á¦·Ã ±¸Á¶Ã¼ ÃÊ±âÈ­ 
		MixCancelButtonDelayFlag = 0;
		return FALSE;

	}

	int i, LineCount = 0, Temp;


	if (pSmeltingItem_Server->Result)
	{
		ShowItemSmeltingMessageFlag = 1; // ¾ÆÀÌÅÛÀÌ Á¦·Ã‰çÀ»¶§ ¸Ş¼¼Áö¸¦ º¸¿©ÁØ´Ù

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
			CraftItemMessageSize.x = 14;           //¹Ú½º »çÀÌÁî 
			CraftItemMessageSize.y = ((LineCount) * 2) + 2;  //¹Ú½º »çÀÌÁî 

			if (cInvenTory.CheckRequireItemToSet(&TempItem))
			{
				//°ğ¹Ù·Î ¼ÂÆÃµÉ¼öÀÖ´ÂÁö¸¦ Ã¼Å©ÇÑ´Ù  
				if (!cInvenTory.AutoSetInvenItem(&TempItem, 1))
				{
					sinThrowItemToFeild(&pSmeltingItem_Server->DesSmeltingItem);
				}
			}

		}

		CheckCharForm(); //ÀÎÁõ 
		sinMinusMoney(m_SmeltingPrice);
		ReformCharForm(); //ÀçÀÎÁõ 
		SendSaveMoney(); //±İ¾× Á¶ÀÛÀ» ¸øÇÏ°ÔÇÏ±âÀ§ÇØ È£ÃâÇÑ´Ù 
		SaveGameData();
		SmeltingItem.OpenFlag = 0; //Ã¢À» ´İ´Â´Ù
	}
	else
	{
		CheckCharForm(); //ÀÎÁõ 
		sinMinusMoney(m_SmeltingPrice);
		ReformCharForm(); //ÀçÀÎÁõ 
		SendSaveMoney(); //±İ¾× Á¶ÀÛÀ» ¸øÇÏ°ÔÇÏ±âÀ§ÇØ È£ÃâÇÑ´Ù 
		SmeltingItem.OpenFlag = 0; //Ã¢À» ´İ´Â´Ù 

	}

	//º¹»ç°¡µÇ¾ú°Å³ªÇÑ ¾ÆÀÌÅÛÀ» Áö¿î´Ù
	for (i = 0; i < 5; i++)
	{
		if (sSmeltingItemBackUp.SmeltingItem[i].Flag)
		{
			cInvenTory.DeleteInvenItemToServer(sSmeltingItemBackUp.SmeltingItem[i].sItemInfo.CODE,
				sSmeltingItemBackUp.SmeltingItem[i].sItemInfo.ItemHeader.Head,
				sSmeltingItemBackUp.SmeltingItem[i].sItemInfo.ItemHeader.dwChkSum);
		}

	}
	memset(&S_smeltingItem, 0, sizeof(SSmeltingItem)); //±¸Á¶Ã¼ ÃÊ±âÈ­ 
	memset(&sSmeltingItemBackUp, 0, sizeof(SSmeltingItem)); //±¸Á¶Ã¼ ÃÊ±âÈ­ 
	memset(&sSmeltingItem_Recv, 0, sizeof(sSMELTINGITEM_SERVER)); //¹ŞÀº Á¦·Ã ±¸Á¶Ã¼ ÃÊ±âÈ­ 
	memset(&sSmeltingItem_Send, 0, sizeof(sSMELTINGITEM_SERVER)); //º¸³½ Á¦·Ã ±¸Á¶Ã¼ ÃÊ±âÈ­ 

	MixItemNoCopyFlag = 0; //º¹»ç¹æÁö ÇÃ·º ÃÊ±âÈ­
	MixCancelButtonDelayFlag = 0;
	m_SmeltingPrice = 0;
	ForceItemPrice2 = 0;
	return TRUE;
}

// pluto Á¦·Ã
int CSmeltingItem::SmeltingCheckEmptyArea(sITEM* pItem)
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;
	int StartX = 23;  //ÀÎº¥ ¹Ú½ºÀÇ ½ÃÀÛÁ¡ X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - pItem->w; //¾ÆÀÌÅÛÀÌ µé¾î°¥¼öÀÖ´Â °¡Àå ³¡ XÁÂÇ¥ 
	cy = (22 * INVENTORY_BOX_COL) - pItem->h; //¾ÆÀÌÅÛÀÌ µé¾î°¥¼öÀÖ´Â °¡Àå ³¡ YÁÂÇ¥

	//ÄÚµå°¡ °°Áö ¾Ê°Å³ª ¾ÆÀÌÅÛÀÌ ºñ¾îÀÖÁö ¾ÊÀº °÷¿¡ ¼ÂÆÃÇÒ¶§ °Ë»öÇØ¼­ À§Ä¡¸¦ Ã£¾Æ³½´Ù 
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
			if (flag == 1) { // Àåº° - Á¦·Ã ½Ã½ºÅÛ ¼öÁ¤
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
	int StartX = 23;  //ÀÎº¥ ¹Ú½ºÀÇ ½ÃÀÛÁ¡ X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - 22; //¾ÆÀÌÅÛÀÌ µé¾î°¥¼öÀÖ´Â °¡Àå ³¡ XÁÂÇ¥ 
	cy = (22 * INVENTORY_BOX_COL) - 22; //¾ÆÀÌÅÛÀÌ µé¾î°¥¼öÀÖ´Â °¡Àå ³¡ YÁÂÇ¥

	int count = 0;
	//ÄÚµå°¡ °°Áö ¾Ê°Å³ª ¾ÆÀÌÅÛÀÌ ºñ¾îÀÖÁö ¾ÊÀº °÷¿¡ ¼ÂÆÃÇÒ¶§ °Ë»öÇØ¼­ À§Ä¡¸¦ Ã£¾Æ³½´Ù 
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
			if (flag == 1) { // Àåº° - Á¦·Ã ½Ã½ºÅÛ ¼öÁ¤
				count++;
			}
		}
	}
	if (count)
		return count;
	else
		return 0;
}

// pluto ±¤¼®, ¼öÁ¤ Á¦·Ã ºñ¿ë
int CSmeltingItem::CheckSmeltingPrice()
{						//±¤¼®	  ÄÉÀÌ¾Æ½º ÀÌÇÁ¸®¾Æ ÆæÅ¸³ª  ¹Ù·ç³ª µµ·ç³×¾Æ »çÆ¼Äí¾Æ ¸Ş·çÄ«¹Ù
	//int SmeltingPriceIndexOre[] = { 100000, 200000, 300000, 400000, 500000, 600000, 700000 };
	int SmeltingPriceIndexOre = 25000;

	//¼öÁ¤    ÄÉÀÌ¾Æ½º ÀÌÇÁ¸®¾Æ ÆæÅ¸³ª  ¹Ù·ç³ª µµ·ç³×¾Æ »çÆ¼Äí¾Æ ¸Ş·çÄ«¹Ù
//int SmeltingPriceIndexCrystal[] = { 200000, 400000, 600000, 800000, 1000000, 1200000, 1400000 };
	int SmeltingPriceIndexCrystal = 50000;

	//	int SmeltingPriceIndexCrystal = 10000; // Àåº° - ÀÏº» ¿äÃ»À¸·Î ÀÓ½Ã·Î ¸¸¿ø

		//DWORD SmeltingCode[] = { sin01, sin02, sin03, sin04, sin05, sin06, sin07, sin08 };
	m_SmeltingPrice = 0;
	if (S_smeltingItem.SmeltingItem[0].Flag)
	{
		if ((S_smeltingItem.SmeltingItem[0].sItemInfo.CODE & sinITEM_MASK2) == sinPR1) // ±¤¼®
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
		else if ((S_smeltingItem.SmeltingItem[0].sItemInfo.CODE & sinITEM_MASK2) == sinPR2) // ¼öÁ¤
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

// pluto Á¦ÀÛ Ãë¼Ò
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
	memset(&g_sManufactureItem, 0, sizeof(SManufactureItem)); //¾ÆÀÌÅÛÀ» ´Ù ³¯·Á¹ö¸°´Ù 
	ResetInvenItemCode();				//ÀÎº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­

	memset(&g_sManufactureItem_Recv, 0, sizeof(SManufactureItem_Server)); //¹ŞÀº Á¦ÀÛ ±¸Á¶Ã¼ ÃÊ±âÈ­
	memset(&g_sManufactureItem_Send, 0, sizeof(SManufactureItem_Server)); //º¸³½ Á¦ÀÛ ±¸Á¶Ã¼ ÃÊ±âÈ­ 

	return TRUE;
}

// pluto Á¦ÀÛ ¾ÆÀÌÅÛ µé¾î°¥ ÀÚ¸® °Ë»ç
int CManufacture::SetManufactureItemAreaCheck(sITEM* pItem)
{
	////±¤¼®	  ÄÉÀÌ¾Æ½º ÀÌÇÁ¸®¾Æ ÆæÅ¸³ª  ¹Ù·ç³ª µµ·ç³×¾Æ »çÆ¼Äí¾Æ ¸Ş·çÄ«¹Ù
	//int SmeltingPriceIndexOre[] = { 100000, 200000, 300000, 400000, 500000, 600000, 700000 };

	////¼öÁ¤    ÄÉÀÌ¾Æ½º ÀÌÇÁ¸®¾Æ ÆæÅ¸³ª  ¹Ù·ç³ª µµ·ç³×¾Æ »çÆ¼Äí¾Æ ¸Ş·çÄ«¹Ù
	//int SmeltingPriceIndexCrystal[] = { 200000, 400000, 600000, 800000, 1000000, 1200000, 1400000 };
	//DWORD SmeltingCode[] = { sin01, sin02, sin03, sin04, sin05, sin06, sin07 };

	POINT ManufactureRuneXY[3] = { {240, 159},  {215, 190}, {265, 190} };

	int i, j;
	TradeColorIndex = 0; // ÃÊ±âÈ­
	TradeCrashItemIndex[0] = 0; //Ãæµ¹µÈ ¾ÆÀÌÅÛ Index ÃÊ±âÈ­ 
	cTrade.InitTradeColorRect(); //ÃÊ±âÈ­

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
		//		{ //ÀÎº¥Åä¸® ¹Ú½º¿¡ µé¾î°¬´ÂÁö¸¦ Ã¼Å© 
		//			TradeColorRect.left =  TradeStartX +( ( ( i - TradeStartX) /22)*22) ; 
		//			TradeColorRect.top  =  TradeStartY +( ( ( j - TradeStartY) /22)*22) ; 
		//			TradeColorRect.right  = pItem->w;
		//			TradeColorRect.bottom = pItem->h;
		//			if(TradeEndX < (TradeColorRect.left + TradeColorRect.right)-11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom)-11)
		//			{
		//				cTrade.InitTradeColorRect(); //ÄÃ·¯ ¹Ú½º RECT ÃÊ±âÈ­ (ÄÃ·¯ ¹Ú½º RECTÁß ÇÏ³ª¶óµÎ 0 ÀÌ¸é ¼ÂÆÃÇÒ¼ö ¾ø´Ù 
		//				return FALSE;
		//			}

		//			pItem->SetX = TradeColorRect.left;
		//			pItem->SetY = TradeColorRect.top;
		//			pItem->ItemPosition = 0; //¹è¿­ÀÇ ÀÎµ¦½º¸¦ ÀúÀåÇÑ´Ù 
		//			TradeColorIndex = SET_ITEM_CHECK_COLOR; //¼ÂÆÃÇÒ¿µ¿ª

		//			if(TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect,0,5))
		//			{  //°ãÄ¡´Â ¾ÆÀÌÅÛÀÌ ÀÖ³ª Ã¼Å© 
		//				TradeColorIndex = NOT_SETTING_COLOR;
		//				return FALSE;
		//			}
		//			return TRUE;
		//		}

		//	}

		//}

		// Á¶ÇÕ °¡´ÉÇÑ ·é¸¸ ³õ´Â´Ù // Àåº° - ·é Ãß°¡ 11±îÁö // Àåº° - ´ëÀåÀåÀÌÀÇ È¥ 11->14
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
							TradeStartX = ManufactureRuneXY[p].x;			// ÁÂÇ¥´Â °°¾Æ¼­ °Á ›§À½
							TradeStartY = ManufactureRuneXY[p].y;
							TradeEndX = TradeStartX + 22;
							TradeEndY = TradeStartY + 22;
							for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22)
							{
								for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22)
								{
									if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j)
									{ //ÀÎº¥Åä¸® ¹Ú½º¿¡ µé¾î°¬´ÂÁö¸¦ Ã¼Å© 
										TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
										TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
										TradeColorRect.right = pItem->w;
										TradeColorRect.bottom = pItem->h;
										if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11)
										{
											cTrade.InitTradeColorRect(); //ÄÃ·¯ ¹Ú½º RECT ÃÊ±âÈ­ (ÄÃ·¯ ¹Ú½º RECTÁß ÇÏ³ª¶óµÎ 0 ÀÌ¸é ¼ÂÆÃÇÒ¼ö ¾ø´Ù 
											return FALSE;
										}

										pItem->SetX = TradeColorRect.left;
										pItem->SetY = TradeColorRect.top;
										pItem->ItemPosition = 0; //¹è¿­ÀÇ ÀÎµ¦½º¸¦ ÀúÀåÇÑ´Ù

										TradeColorIndex = SET_ITEM_CHECK_COLOR; //¼ÂÆÃÇÒ¿µ¿ª

										if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 5))
										{  //°ãÄ¡´Â ¾ÆÀÌÅÛÀÌ ÀÖ³ª Ã¼Å© 
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
		TradeColorIndex = 0; //ÃÊ±âÈ­   (Æ®·¹ÀÌµå Ã¢°ú Ã¢°í°¡ °°ÀÌ ¾´´Ù)
		TradeCrashItemIndex[0] = 0; //Ãæµ¹µÈ ¾ÆÀÌÅÛ Index ÃÊ±âÈ­ 
		cTrade.InitTradeColorRect(); //ÃÊ±âÈ­

		TradeStartX = 58;
		TradeStartY = 193 + sinInterHeight2;
		TradeEndX = TradeStartX + (3 * 22);
		TradeEndY = TradeStartY + (4 * 22);
	}
	return TRUE;
}

// pluto Á¦ÀÛ ¾ÆÀÌÅÛ ³õ´Â´Ù
int CManufacture::LastSetManufactureItem(sITEM* pItem)
{
	CheckManufactureItem();
	if ((pItem->CODE & sinITEM_MASK2) == sinPR3 || (pItem->CODE & sinITEM_MASK2) == sinPR4)
	{
		// ·éÀÏ °æ¿ì
		for (int j = 1; j < 4; j++)
		{
			if (!g_sManufactureItem.ManufactureItem[j].Flag)
			{
				memcpy(&g_sManufactureItem.ManufactureItem[j], pItem, sizeof(sITEM));
				g_sManufactureItem.ManufactureItem[j].x = pItem->SetX;
				g_sManufactureItem.ManufactureItem[j].y = pItem->SetY;
				pItem->Flag = 0; //ÃÊ±âÈ­ 
				//sinPlaySound(g_sManufactureItem.ManufactureItem[j].SoundIndex);
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
			pItem->Flag = 0; //ÃÊ±âÈ­ 
			//sinPlaySound(g_sManufactureItem.ManufactureItem[0].SoundIndex);
			ReFormManufactureItem();
			return TRUE;
		}
	}
	return TRUE;

}

// pluto Á¦ÀÛ
int CManufacture::CheckManufactureItem()
{ //Ã¼Å©  
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

// pluto Á¦ÀÛ
int CManufacture::ReFormManufactureItem()
{
	//ÀÎÁõ 
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
	SelectTradeItemIndex = 0;    //ÁıÀ» ¾ÆÀÌÅÛ 

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

							return FALSE;
						}
					}

					CheckManufactureItem();
					memcpy(&MouseItem, &g_sManufactureItem.ManufactureItem[i], sizeof(sITEM));
					g_sManufactureItem.ManufactureItem[i].Flag = 0;
					memset(&g_sManufactureItem.ManufactureItem[i], 0, sizeof(sITEM));  //¼­¹ö ÇØÅ· ¿¡·¯ ¶§¹®¿¡ ÀÓ½Ã ¹æÆíÀÌ´Ù.

					//sinPlaySound(g_sManufactureItem.ManufactureItem[i].SoundIndex);
					TradeColorIndex = 0;
					ReFormManufactureItem();
					return TRUE;
				}
				else
				{
					SelectTradeItemIndex = i + 1;
					TradeColorIndex = SELECT_ITEM_COLOR;
					sinShowItemInfoFlag = 1; //¾ÆÀÌÅÛÀÇ Á¤º¸¸¦ º¸¿©ÁØ´Ù 
					cItem.ShowItemInfo(&g_sManufactureItem.ManufactureItem[i], 2);
				}
			}
		}
	}
	return FALSE;
}


// pluto Á¦ÀÛ
int CManufacture::ManufactureCheckEmptyArea(sITEM* pItem)
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;
	int StartX = 23;  //ÀÎº¥ ¹Ú½ºÀÇ ½ÃÀÛÁ¡ X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - pItem->w; //¾ÆÀÌÅÛÀÌ µé¾î°¥¼öÀÖ´Â °¡Àå ³¡ XÁÂÇ¥ 
	cy = (22 * INVENTORY_BOX_COL) - pItem->h; //¾ÆÀÌÅÛÀÌ µé¾î°¥¼öÀÖ´Â °¡Àå ³¡ YÁÂÇ¥

	//ÄÚµå°¡ °°Áö ¾Ê°Å³ª ¾ÆÀÌÅÛÀÌ ºñ¾îÀÖÁö ¾ÊÀº °÷¿¡ ¼ÂÆÃÇÒ¶§ °Ë»öÇØ¼­ À§Ä¡¸¦ Ã£¾Æ³½´Ù 
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

	//º¹»ç¸¦ Ã¼Å©ÇÏ±âÀ§ÇÑ ¹é¾÷º»À» ¸¸µç´Ù
	memcpy(&g_sManufactureItemBackUp, &g_sManufactureItem, sizeof(SManufactureItem));
	MixItemNoCopyFlag = 1; //¾ÆÀÌÅÛÀÌ º¹»çµÇ´Â°É ¹æÁöÇÏ±âÀ§ÇØ ÇÃ·ºÀ»ÁØ´Ù
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

	CraftItemSendServerIndex++; //ÀÎµ¦½º¸¦ º¸³½´Ù 
	g_sManufactureItem_Send.Index = CraftItemSendServerIndex;

	//ÀúÀåµÈ ±¸Á¶Ã¼¸¦ ¼­¹ö¿¡ º¸³½´Ù 
	SendManufactureItemToServer(&g_sManufactureItem_Send);

	return TRUE;
}

// pluto Á¦ÀÛ
int CManufacture::sinRecvManufactureItem(SManufactureItem_Server* pManufactureItem_Server)
{
	memcpy(&g_sManufactureItem_Recv, pManufactureItem_Server, sizeof(SManufactureItem_Server));
	return TRUE;
}

// pluto Á¦ÀÛ // Àåº° - Á¦ÀÛ
void CManufacture::RuneIndexInit()
{
	// pluto Á¦ÀÛ ÇÒ¶§ ÇÊ¿äÇÑ ·é º¸¿©ÁÖ´Â°Å
	for (int i = 0; i < MAX_RECIPE_KIND; i++)
	{
		memset(&g_sManufacture_Rune[i], 0, sizeof(SManufacture_Rune));
	}

	g_sManufacture_Rune[0].RecipeCode = EXTREME_RECIPE; // ÀØÇôÁø ·¹½ÃÇÇ
	g_sManufacture_Rune[0].NeedRuneIndex[0] = 1;
	g_sManufacture_Rune[0].NeedRuneIndex[1] = 2;
	g_sManufacture_Rune[0].NeedRuneIndex[2] = 3;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[1].RecipeCode = ANCIENT_RECIPE; // °í´ëÀÇ ·¹½ÃÇÇ
	g_sManufacture_Rune[1].NeedRuneIndex[0] = 2;
	g_sManufacture_Rune[1].NeedRuneIndex[1] = 3;
	g_sManufacture_Rune[1].NeedRuneIndex[2] = 4;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[2].RecipeCode = MINOTAUR_RECIPE; // ´ëÁöÀÇ ·¹½ÃÇÇ
	g_sManufacture_Rune[2].NeedRuneIndex[0] = 3;
	g_sManufacture_Rune[2].NeedRuneIndex[1] = 4;
	g_sManufacture_Rune[2].NeedRuneIndex[2] = 5;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[3].RecipeCode = DOOM_RECIPE; // ¾îµÒÀÇ ·¹½ÃÇÇ
	g_sManufacture_Rune[3].NeedRuneIndex[0] = 3;
	g_sManufacture_Rune[3].NeedRuneIndex[1] = 4;
	g_sManufacture_Rune[3].NeedRuneIndex[2] = 5;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[4].RecipeCode = SALAMANDER_RECIPE; // È­¿°ÀÇ ·¹½ÃÇÇ
	g_sManufacture_Rune[4].NeedRuneIndex[0] = 3;
	g_sManufacture_Rune[4].NeedRuneIndex[1] = 4;
	g_sManufacture_Rune[4].NeedRuneIndex[2] = 5;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[5].RecipeCode = WYVERN_RECIPE; // ¹Ù¶÷ÀÇ ·¹½ÃÇÇ
	g_sManufacture_Rune[5].NeedRuneIndex[0] = 3;
	g_sManufacture_Rune[5].NeedRuneIndex[1] = 4;
	g_sManufacture_Rune[5].NeedRuneIndex[2] = 5;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[6].RecipeCode = PHOENIX_RECIPE; // ÅÂ¾çÀÇ ·¹½ÃÇÇ
	g_sManufacture_Rune[6].NeedRuneIndex[0] = 4;
	g_sManufacture_Rune[6].NeedRuneIndex[1] = 5;
	g_sManufacture_Rune[6].NeedRuneIndex[2] = 6;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[7].RecipeCode = FRENZY_RECIPE; // ±¤Æ÷ÇÑ ·¹½ÃÇÇ
	g_sManufacture_Rune[7].NeedRuneIndex[0] = 5;
	g_sManufacture_Rune[7].NeedRuneIndex[1] = 6;
	g_sManufacture_Rune[7].NeedRuneIndex[2] = 7;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[8].RecipeCode = HEAVENS_RECIPE; // Ãµ»óÀÇ ·¹½ÃÇÇ
	g_sManufacture_Rune[8].NeedRuneIndex[0] = 6;
	g_sManufacture_Rune[8].NeedRuneIndex[1] = 7;
	g_sManufacture_Rune[8].NeedRuneIndex[2] = 8;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[9].RecipeCode = SNOW_RECIPE; // Æ÷¼³ÀÇ ·¹½ÃÇÇ // Àåº° - Á¦ÀÛ
	g_sManufacture_Rune[9].NeedRuneIndex[0] = 9;
	g_sManufacture_Rune[9].NeedRuneIndex[1] = 10;
	g_sManufacture_Rune[9].NeedRuneIndex[2] = 11;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[10].RecipeCode = MEMORY_RECIPE; // ±â¾ïÀÇ ·¹½ÃÇÇ // Àåº° - ´ëÀåÀåÀÌÀÇ È¥
	g_sManufacture_Rune[10].NeedRuneIndex[0] = 12;
	g_sManufacture_Rune[10].NeedRuneIndex[1] = 13;
	g_sManufacture_Rune[10].NeedRuneIndex[2] = 14;


}

// pluto Á¦ÀÛ ·¹½ÃÇÇ ÀÎµ¦½º´Ù ÇÊ¿äÇÑ ·é ±×¸² ¼ÂÆÃ ÇÒ¶ó±¸ // Àåº° - Á¦ÀÛ // Àåº° - ´ëÀåÀåÀÌÀÇ È¥
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
		RecipeIndex = SNOW_RECIPE; // Àåº° - Á¦ÀÛ : Æ÷¼³ÀÇ Á¦ÀÛ¼­
		break;

	case sin11:
		RecipeIndex = MEMORY_RECIPE; //  ±â¾ïÀÇ Á¦ÀÛ¼­	// Àåº° - ´ëÀåÀåÀÌÀÇ È¥
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

// pluto Á¦ÀÛ °á°ú
int CManufacture::RecvManufactureItemResult(SManufactureItem_Server* pManufactureItem_Server)
{
	CheckMixItemCopyFlag = 0; //º¹»ç¹æÁö ÇÃ·º  
	if (!ManufactureItem.m_OpenFlag)
	{
		MixItemNoCopyFlag = 0; //º¹»ç¹æÁö ÇÃ·º ÃÊ±âÈ­
		memset(&g_sManufactureItem, 0, sizeof(SManufactureItem)); //±¸Á¶Ã¼ ÃÊ±âÈ­ 
		memset(&g_sManufactureItemBackUp, 0, sizeof(SManufactureItem)); //±¸Á¶Ã¼ ÃÊ±âÈ­ 
		memset(&g_sManufactureItem_Recv, 0, sizeof(SManufactureItem_Server)); //¹ŞÀº Á¦ÀÛ ±¸Á¶Ã¼ ÃÊ±âÈ­ 
		memset(&g_sManufactureItem_Send, 0, sizeof(SManufactureItem_Server)); //º¸³½ Á¦ÀÛ ±¸Á¶Ã¼ ÃÊ±âÈ­ 
		MixCancelButtonDelayFlag = 0;
		return FALSE;

	}

	int i, LineCount = 0, Temp;


	if (pManufactureItem_Server->Result)
	{
		ShowItemManufactureMessageFlag = 1; // ¾ÆÀÌÅÛÀÌ Á¦ÀÛ‰çÀ»¶§ ¸Ş¼¼Áö¸¦ º¸¿©ÁØ´Ù

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
			CraftItemMessageSize.x = 14;           //¹Ú½º »çÀÌÁî 
			CraftItemMessageSize.y = ((LineCount) * 2) + 2;  //¹Ú½º »çÀÌÁî 

			if (cInvenTory.CheckRequireItemToSet(&TempItem))
			{
				//°ğ¹Ù·Î ¼ÂÆÃµÉ¼öÀÖ´ÂÁö¸¦ Ã¼Å©ÇÑ´Ù  
				if (!cInvenTory.AutoSetInvenItem(&TempItem, 1))
				{
					sinThrowItemToFeild(&pManufactureItem_Server->DesManufactureItem);
				}
			}

		}

		CheckCharForm(); //ÀÎÁõ 
		sinMinusMoney(m_ManufacturePrice);
		ReformCharForm(); //ÀçÀÎÁõ 
		SendSaveMoney(); //±İ¾× Á¶ÀÛÀ» ¸øÇÏ°ÔÇÏ±âÀ§ÇØ È£ÃâÇÑ´Ù 
		SaveGameData();
		ManufactureItem.m_OpenFlag = 0; //Ã¢À» ´İ´Â´Ù
	}
	else
	{
		CheckCharForm(); //ÀÎÁõ 
		sinMinusMoney(m_ManufacturePrice);
		ReformCharForm(); //ÀçÀÎÁõ 
		SendSaveMoney(); //±İ¾× Á¶ÀÛÀ» ¸øÇÏ°ÔÇÏ±âÀ§ÇØ È£ÃâÇÑ´Ù 
		ManufactureItem.m_OpenFlag = 0; //Ã¢À» ´İ´Â´Ù 

	}

	//º¹»ç°¡µÇ¾ú°Å³ªÇÑ ¾ÆÀÌÅÛÀ» Áö¿î´Ù
	for (i = 0; i < 4; i++)
	{
		if (g_sManufactureItemBackUp.ManufactureItem[i].Flag)
		{
			cInvenTory.DeleteInvenItemToServer(g_sManufactureItemBackUp.ManufactureItem[i].sItemInfo.CODE,
				g_sManufactureItemBackUp.ManufactureItem[i].sItemInfo.ItemHeader.Head,
				g_sManufactureItemBackUp.ManufactureItem[i].sItemInfo.ItemHeader.dwChkSum);
		}

	}
	memset(&g_sManufactureItem, 0, sizeof(SManufactureItem)); //±¸Á¶Ã¼ ÃÊ±âÈ­ 
	memset(&g_sManufactureItemBackUp, 0, sizeof(SManufactureItem)); //±¸Á¶Ã¼ ÃÊ±âÈ­ 
	memset(&g_sManufactureItem_Recv, 0, sizeof(SManufactureItem_Server)); //¹ŞÀº Á¦ÀÛ ±¸Á¶Ã¼ ÃÊ±âÈ­ 
	memset(&g_sManufactureItem_Send, 0, sizeof(SManufactureItem_Server)); //º¸³½ Á¦ÀÛ ±¸Á¶Ã¼ ÃÊ±âÈ­ 

	MixItemNoCopyFlag = 0; //º¹»ç¹æÁö ÇÃ·º ÃÊ±âÈ­
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
	int StartX = 23;  //ÀÎº¥ ¹Ú½ºÀÇ ½ÃÀÛÁ¡ X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - 22; //¾ÆÀÌÅÛÀÌ µé¾î°¥¼öÀÖ´Â °¡Àå ³¡ XÁÂÇ¥ 
	cy = (22 * INVENTORY_BOX_COL) - 22; //¾ÆÀÌÅÛÀÌ µé¾î°¥¼öÀÖ´Â °¡Àå ³¡ YÁÂÇ¥

	int count = 0;
	//ÄÚµå°¡ °°Áö ¾Ê°Å³ª ¾ÆÀÌÅÛÀÌ ºñ¾îÀÖÁö ¾ÊÀº °÷¿¡ ¼ÂÆÃÇÒ¶§ °Ë»öÇØ¼­ À§Ä¡¸¦ Ã£¾Æ³½´Ù 
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

// pluto Á¦·Ã È¸»ö ÀÌ¹ÌÁö ½©ÅÒ // Àåº° - Á¦·Ã // Àåº° - ´ëÀåÀåÀÌÀÇ È¥
int CSmeltingItem::GetSheltomIndex(DWORD SmeltingItemCode)
{
	DWORD SmeltingItemAray[] = { (sinPR1 | sin01), (sinPR1 | sin02), (sinPR1 | sin03), (sinPR1 | sin04), (sinPR1 | sin05), (sinPR1 | sin06), (sinPR1 | sin07), (sinPR1 | sin08),
								 (sinPR2 | sin01), (sinPR2 | sin02), (sinPR2 | sin03), (sinPR2 | sin04), (sinPR2 | sin05), (sinPR2 | sin06), (sinPR2 | sin07), (sinPR2 | sin08),
								 (sinPR2 | sin09), (sinPR2 | sin10), (sinPR2 | sin11), (sinPR2 | sin12), (sinPR2 | sin13), (sinPR2 | sin14) };
	int SheltomAray[] = { 6, 7, 8, 9, 10, 11, 12, 13,
						  7, 8, 9, 10, 11, 12, 13, 14,
							3, 4, 5, 3, 4, 5 }; // ±¤¼®ÀÌ³ª ¼öÁ¤ ¹øÈ£
	for (int i = 0; i < 22; i++)
	{
		if (SmeltingItemCode == SmeltingItemAray[i])
		{
			return SheltomAray[i];
		}
	}
	return FALSE;
}

// pluto Á¦ÀÛµÉ ¾ÆÀÌÅÛ ÄÚµå. ¹«°Ô °Ë»ç ÇÒ·Á°í // Àåº° - Á¦ÀÛ
SManufacture_ResultItemInfo CManufacture::ManufactureCheckItem(sITEM* pItemCode, smCHAR_INFO* charInfo) // Àåº° - ´ëÀåÀåÀÌÀÇ È¥
{
	int RuneCnt = 0;
	SManufacture_ResultItemInfo Result;
	memset(&Result, 0, sizeof(SManufacture_ResultItemInfo));
	DWORD Arm_Armor_Code[MAX_RECIPE_KIND] = { sin01, sin02, sin03, sin04, sin05, sin06, sin07, sin08, sin09, sin10, sin11 }; // ·¹½ÃÇÇ Á¾·ù // Àåº° - ´ëÀåÀåÀÌÀÇ È¥
	DWORD CheckRuneCode[MAX_HANDMADEITEM][8][3] = { {	{ (sinPR3 | sin01), (sinPR3 | sin02), (sinPR3 | sin03) },  // ·é Á¶ÇÕ½ÄÀÌ´Ù
										{ (sinPR3 | sin01), (sinPR3 | sin02), (sinPR4 | sin03) },
										{ (sinPR3 | sin01), (sinPR4 | sin02), (sinPR3 | sin03) },
										{ (sinPR3 | sin01), (sinPR4 | sin02), (sinPR4 | sin03) },
										{ (sinPR4 | sin01), (sinPR3 | sin02), (sinPR3 | sin03) },
										{ (sinPR4 | sin01), (sinPR3 | sin02), (sinPR4 | sin03) },
										{ (sinPR4 | sin01), (sinPR4 | sin02), (sinPR3 | sin03) },
										{ (sinPR4 | sin01), (sinPR4 | sin02), (sinPR4 | sin03) } },
		////////////////////////////////////////////////////////////////////////////////////////////	ÀØÇôÁø Á¦ÀÛ¼­
	 {  { (sinPR3 | sin02), (sinPR3 | sin03), (sinPR3 | sin04) },
		{ (sinPR3 | sin02), (sinPR3 | sin03), (sinPR4 | sin04) },
		{ (sinPR3 | sin02), (sinPR4 | sin03), (sinPR3 | sin04) },
		{ (sinPR3 | sin02), (sinPR4 | sin03), (sinPR4 | sin04) },
		{ (sinPR4 | sin02), (sinPR3 | sin03), (sinPR3 | sin04) },
		{ (sinPR4 | sin02), (sinPR3 | sin03), (sinPR4 | sin04) },
		{ (sinPR4 | sin02), (sinPR4 | sin03), (sinPR3 | sin04) },
		{ (sinPR4 | sin02), (sinPR4 | sin03), (sinPR4 | sin04) } },
		////////////////////////////////////////////////////////////////////////////////////////////	°í´ëÀÇ Á¦ÀÛ¼­
	 {  { (sinPR3 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) } },
		////////////////////////////////////////////////////////////////////////////////////////////	´ëÁöÀÇ Á¦ÀÛ¼­
	 {  { (sinPR3 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) } },
		////////////////////////////////////////////////////////////////////////////////////////////	¾îµÒÀÇ Á¦ÀÛ¼­
	 {	{ (sinPR3 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) } },
		////////////////////////////////////////////////////////////////////////////////////////////	È­¿°ÀÇ Á¦ÀÛ¼­
	 {  { (sinPR3 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) } },
		////////////////////////////////////////////////////////////////////////////////////////////	¹Ù¶÷ÀÇ Á¦ÀÛ¼­
	 {  { (sinPR3 | sin04), (sinPR3 | sin05), (sinPR3 | sin06) },
		{ (sinPR3 | sin04), (sinPR3 | sin05), (sinPR4 | sin06) },
		{ (sinPR3 | sin04), (sinPR4 | sin05), (sinPR3 | sin06) },
		{ (sinPR3 | sin04), (sinPR4 | sin05), (sinPR4 | sin06) },
		{ (sinPR4 | sin04), (sinPR3 | sin05), (sinPR3 | sin06) },
		{ (sinPR4 | sin04), (sinPR3 | sin05), (sinPR4 | sin06) },
		{ (sinPR4 | sin04), (sinPR4 | sin05), (sinPR3 | sin06) },
		{ (sinPR4 | sin04), (sinPR4 | sin05), (sinPR4 | sin06) } },
		////////////////////////////////////////////////////////////////////////////////////////////	ÅÂ¾çÀÇ Á¦ÀÛ¼­
	 {  { (sinPR3 | sin05), (sinPR3 | sin06), (sinPR3 | sin07) },
		{ (sinPR3 | sin05), (sinPR3 | sin06), (sinPR4 | sin07) },
		{ (sinPR3 | sin05), (sinPR4 | sin06), (sinPR3 | sin07) },
		{ (sinPR3 | sin05), (sinPR4 | sin06), (sinPR4 | sin07) },
		{ (sinPR4 | sin05), (sinPR3 | sin06), (sinPR3 | sin07) },
		{ (sinPR4 | sin05), (sinPR3 | sin06), (sinPR4 | sin07) },
		{ (sinPR4 | sin05), (sinPR4 | sin06), (sinPR3 | sin07) },
		{ (sinPR4 | sin05), (sinPR4 | sin06), (sinPR4 | sin07) } },
		////////////////////////////////////////////////////////////////////////////////////////////	±¤Æ÷ÇÑ Á¦ÀÛ¼­
	 {  { (sinPR3 | sin06), (sinPR3 | sin07), (sinPR3 | sin08) },
		{ (sinPR3 | sin06), (sinPR3 | sin07), (sinPR4 | sin08) },
		{ (sinPR3 | sin06), (sinPR4 | sin07), (sinPR3 | sin08) },
		{ (sinPR3 | sin06), (sinPR4 | sin07), (sinPR4 | sin08) },
		{ (sinPR4 | sin06), (sinPR3 | sin07), (sinPR3 | sin08) },
		{ (sinPR4 | sin06), (sinPR3 | sin07), (sinPR4 | sin08) },
		{ (sinPR4 | sin06), (sinPR4 | sin07), (sinPR3 | sin08) },
		{ (sinPR4 | sin06), (sinPR4 | sin07), (sinPR4 | sin08) } },
		////////////////////////////////////////////////////////////////////////////////////////////	Ãµ»óÀÇ Á¦ÀÛ¼­

	 {  { (sinPR3 | sin09), (sinPR4 | sin10), (sinPR3 | sin11) },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 } },
		////////////////////////////////////////////////////////////////////////////////////////////	Æ÷¼³ÀÇ Á¦ÀÛ¼­ //Àåº°

		{  { (sinPR3 | sin12), (sinPR3 | sin13), (sinPR3 | sin14) },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 } },
		////////////////////////////////////////////////////////////////////////////////////////////	±â¾ïÀÇ Á¦ÀÛ¼­ // Àåº° - ´ëÀåÀåÀÌÀÇ È¥
	};

	// Àåº° - ´ëÀåÀåÀÌÀÇ È¥
	DWORD	dwItem = 0;

	// ÇÁ¸®½ºÆ¼½º ÀÏ °æ¿ì
	if (charInfo->JOB_CODE == JOBCODE_PRIESTESS) dwItem = (sinDA2 | sin52);
	// ¸ŞÁö¼Ç ÀÏ °æ¿ì
	else if (charInfo->JOB_CODE == JOBCODE_MAGICIAN) dwItem = (sinDA2 | sin51);
	// ¾ÆÃ³, ¾ÆÆ²¶õÅ¸ ÀÏ °æ¿ì
	else if (charInfo->JOB_CODE == JOBCODE_ARCHER || charInfo->JOB_CODE == JOBCODE_ATALANTA) dwItem = (sinDA1 | sin52);
	// ÆÄÀÌÅÍ, ÆÄÀÌÅ©¸Ç, ¸ŞÄ«´Ï¼Ç, ³ªÀÌÆ® ÀÏ °æ¿ì
	else dwItem = (sinDA1 | sin51);


	// Àåº° ¹è¿­ ¼ıÀÚ [a][b][c] a = ¹«±¸ or °©ÁÖ / b = ·¹½ÃÇÇ Á¾·ù / c = »ı¼ºµÉ ¾ÆÀÌÅÛ Á¾·ù(°íÁ¤)
	// ·¹½ÃÇÇ¿Í ·éÁ¶ÇÕ¿¡ ¸Â´Â ¹ß»ı ¾ÆÀÌÅÛ
	DWORD CreateItemCode[2][MAX_HANDMADEITEM][8] = { {	{ (sinWA1 | sin14), (sinWC1 | sin14), (sinWH1 | sin15), (sinWP1 | sin15), (sinWS1 | sin16), (sinWS2 | sin17), (sinWT1 | sin15), (sinWM1 | sin15) }, // ÀØÇôÁø ·¹½ÃÇÇ ¹«±¸
											{ (sinWA1 | sin15), (sinWC1 | sin15), (sinWH1 | sin16), (sinWP1 | sin16), (sinWS1 | sin17), (sinWS2 | sin18), (sinWT1 | sin16), (sinWM1 | sin16) }, // °í´ëÀÇ ·¹½ÃÇÇ ¹«±¸
											{ (sinWA1 | sin16), (sinWC1 | sin16), (sinWH1 | sin17), (sinWP1 | sin17), (sinWS1 | sin18), (sinWS2 | sin19), (sinWT1 | sin17), (sinWM1 | sin17) }, // ´ëÁöÀÇ ¹«±¸
											{ (sinWA1 | sin17), (sinWC1 | sin17), (sinWH1 | sin18), (sinWP1 | sin18), (sinWS1 | sin19), (sinWS2 | sin20), (sinWT1 | sin18), (sinWM1 | sin18) }, // ¾îµÒÀÇ ·¹½ÃÇÇ ¹«±¸
											{ (sinWA1 | sin18), (sinWC1 | sin18), (sinWH1 | sin19), (sinWP1 | sin19), (sinWS1 | sin20), (sinWS2 | sin21), (sinWT1 | sin19), (sinWM1 | sin19) }, // È­¿°ÀÇ ·¹½ÃÇÇ ¹«±¸
											{ (sinWA1 | sin19), (sinWC1 | sin19), (sinWH1 | sin20), (sinWP1 | sin20), (sinWS1 | sin21), (sinWS2 | sin22), (sinWT1 | sin20), (sinWM1 | sin20) }, // ¹Ù¶÷ÀÇ ·¹½ÃÇÇ ¹«±¸
											{ (sinWA1 | sin21), (sinWC1 | sin21), (sinWH1 | sin22), (sinWP1 | sin22), (sinWS1 | sin23), (sinWS2 | sin24), (sinWT1 | sin22), (sinWM1 | sin22) }, // ÅÂ¾çÀÇ ·¹½ÃÇÇ ¹«±¸
											{ (sinWA1 | sin22), (sinWC1 | sin22), (sinWH1 | sin23), (sinWP1 | sin23), (sinWS1 | sin24), (sinWS2 | sin25), (sinWT1 | sin23), (sinWM1 | sin23) }, // ±¤Æ÷ÇÑ ·¹½ÃÇÇ ¹«±¸
											{ 0, 0, 0, 0, 0, 0, 0, 0 } , // Ãµ»óÀÇ ·¹½ÃÇÇ ¹«±¸
											{ 0, 0, 0, 0, 0, 0, 0, 0 } , // Æ÷¼³ÀÇ ·¹½ÃÇÇ // Àåº°
											{ 0, 0, 0, 0, 0, 0, 0, 0 } }, // ±â¾ïÀÇ ·¹½ÃÇÇ // Àåº° - ´ëÀåÀåÀÌÀÇ È¥
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
										{	{ (sinDA1 | sin16), (sinDA2 | sin16), (sinDS1 | sin14), (sinOM1 | sin15), (sinDG1 | sin14), (sinOA2 | sin14), (sinDB1 | sin14), 0 }, // ÀØÇôÁø ·¹½ÃÇÇ °©ÁÖ
											{ (sinDA1 | sin17), (sinDA2 | sin17), (sinDS1 | sin15), (sinOM1 | sin16), (sinDG1 | sin15), (sinOA2 | sin15), (sinDB1 | sin15), 0 }, // °í´ëÀÇ ·¹½ÃÇÇ °©ÁÖ
											{ (sinDA1 | sin18), (sinDA2 | sin18), (sinDS1 | sin16), (sinOM1 | sin17), (sinDG1 | sin16), (sinOA2 | sin16), (sinDB1 | sin16), 0 }, // ´ëÁöÀÇ ·¹½ÃÇÇ °©ÁÖ
											{ (sinDA1 | sin19), (sinDA2 | sin19), (sinDS1 | sin17), (sinOM1 | sin18), (sinDG1 | sin17), (sinOA2 | sin17), (sinDB1 | sin17), 0 }, // ¾îµÒÀÇ ·¹½ÃÇÇ °©ÁÖ
											{ (sinDA1 | sin20), (sinDA2 | sin20), (sinDS1 | sin18), (sinOM1 | sin19), (sinDG1 | sin18), (sinOA2 | sin18), (sinDB1 | sin18), 0 }, // È­¿°ÀÇ ·¹½ÃÇÇ °©ÁÖ
											{ (sinDA1 | sin21), (sinDA2 | sin21), (sinDS1 | sin19), (sinOM1 | sin20), (sinDG1 | sin19), (sinOA2 | sin19), (sinDB1 | sin19), 0 }, // ¹Ù¶÷ÀÇ ·¹½ÃÇÇ °©ÁÖ
											{ (sinDA1 | sin23), (sinDA2 | sin23), (sinDS1 | sin21), (sinOM1 | sin22), (sinDG1 | sin21), (sinOA2 | sin21), (sinDB1 | sin21), 0 }, // ÅÂ¾çÀÇ ·¹½ÃÇÇ °©ÁÖ
											{ (sinDA1 | sin24), (sinDA2 | sin24), (sinDS1 | sin22), (sinOM1 | sin23), (sinDG1 | sin22), (sinOA2 | sin22), (sinDB1 | sin22), 0 }, // ±¤Æ÷ÇÑ ·¹½ÃÇÇ °©ÁÖ
											{ 0, 0, 0, 0, 0, 0, 0, 0 } , // Ãµ»óÀÇ ·¹½ÃÇÇ °©ÁÖ
											{ (sinOA1 | sin36), 0, 0, 0, 0, 0, 0, 0 } , // Æ÷¼³ÀÇ ·¹½ÃÇÇ //Àåº°
											{ dwItem, 0, 0, 0, 0, 0, 0, 0 } } // ±â¾ïÀÇ ·¹½ÃÇÇ // Àåº° - ´ëÀåÀåÀÌÀÇ È¥
	};

	if ((pItemCode[0].CODE & sinITEM_MASK2) == sinWR1)		// ¹«±¸
	{
		for (int i = 0; i < MAX_RECIPE_KIND; i++)
		{
			if (Arm_Armor_Code[i] == (pItemCode[0].CODE & sinITEM_MASK3))
			{
				for (int j = 0; j < MAX_RUNE; j++)   // ·é Á¶ÇÕ 
				{
					RuneCnt = 0;
					for (int x = 0; x < 3; x++) // ÇÊ¿äÇÑ ·é
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
							for (int c = 0; c < MAX_HANDMADEITEM; c++) // Àåº° - ´ëÀåÀåÀÌÀÇ È¥
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
	else if ((pItemCode[0].CODE & sinITEM_MASK2) == sinDR1)		// °©ÁÖ
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
							for (int c = 0; c < MAX_HANDMADEITEM; c++) // Àåº° - ´ëÀåÀåÀÌÀÇ È¥
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
								for (int c = 0; c < MAX_HANDMADEITEM; c++) // Àåº° - ´ëÀåÀåÀÌÀÇ È¥
								{
									if (CreateItemCode[1][i][0] == g_Manufacture_ItemInfo[c].ItemCode)
									{
										Result.ItemCode = g_Manufacture_ItemInfo[c].ItemCode;
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

// pluto Á¦ÀÛ ¾ÆÀÌÅÛ ¹«°Ô, °¡°İ Á¤º¸
int ManufactureItemResultInfo(SManufacture_ResultItemInfo_Server* pResultItemInfo)
{
	for (int i = 0; i < MAX_HANDMADEITEM; i++) // Àåº° - Á¦ÀÛ MAX_HANDMADEITEM = Á¦ÀÛ ÇÒ ¼ö ÀÖ´Â ¾ÆÀÌÅÛ °¹¼ö(·¹½ÃÇÇ°¡ Ãß°¡µÇ¸é Áõ°¡) // Àåº° - ´ëÀåÀåÀÌÀÇ È¥
	{
		g_Manufacture_ItemInfo[i].ItemCode = pResultItemInfo->ItemCode[i];
		lstrcpy(g_Manufacture_ItemInfo[i].ItemName, pResultItemInfo->ItemName[i]);
		g_Manufacture_ItemInfo[i].Weight = pResultItemInfo->Weight[i];
		g_Manufacture_ItemInfo[i].Price = pResultItemInfo->Price[i];
	}
	return TRUE;
}