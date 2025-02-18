/*----------------------------------------------------------------------------*
*	파일명 :  sinTrade.cpp
*	하는일 :  아이템 교환
*	작성일 :  최종업데이트 12월 sinInvenColor
*	적성자 :  박상열
*-----------------------------------------------------------------------------*/

#include "sinLinkHeader.h"
#include "Character/playmain.h"
#include "Character/playsub.h"
#include "netplay.h"
#include "Character/character.h"
#include "fileread.h"
//#define  __SIN_AGING_TEST
#define AGING_MONEY_FREE // 박재원 - 에이징 가격 무료 이벤트
/*----------------------------------------------------------------------------*
*								전역변수
*-----------------------------------------------------------------------------*/
cTRADE cTrade;
sTRADE sTrade;	   //보낼 정보 	
sTRADE sTradeRecv; //받은 정보 

cMIXTURERESET cMixtureReset;					// 석지용 - 믹스쳐 리셋 객체
sMIXTURE_RESET_ITEM	sMixtureResetItem;			// 석지용 - 믹스쳐 리셋 아이템 정보 구조체
sMIXTURE_RESET_ITEM sMixtureResetItemBackUp;	// 석지용 - 믹스쳐 리셋 아이템 정보 구조체 백업
int	MixtureResetCheckFlag = 0;					// 석지용 - 믹스쳐 리셋이 가능한가?
sMIXTURE_RESET_ITEM_SERVER	sMixtureReset_Send;	// 석지용 - 믹스쳐 리셋 서버로 전송할 구조체
sMIXTURE_RESET_ITEM_SERVER	sMixtureReset_Recv;	// 석지용 - 믹스쳐 리셋 서버에서 받을 구조체

cAGING	cAging;		//에이징 
sAGINGITEM sAgingItem;
sAGINGITEM sAgingItemBackUp;


sCRAFTITEM_SERVER sAging_Send; //서버로 보내질 아이템 
sCRAFTITEM_SERVER sAging_Recv; //서버에서 받을 아이템 

cWAREHOUSE cWareHouse;	//창고 
sWAREHOUSE sWareHouse;

int WareHouseSaveFlag = 0;
int CloseWareHouseCheckFlag2 = 0;
int CloseWareHouseCheckFlag3 = 0;

cCRAFTITEM cCraftItem;	//아이템 조합 
sCRAFTITEM sCraftItem;  //조합될 아이템 
sCRAFTITEM sCraftItemBackUp;  //조합될 아이템 

// pluto 제련
CSmeltingItem SmeltingItem;
SSmeltingItem S_smeltingItem;
SSmeltingItem sSmeltingItemBackUp;
sSMELTINGITEM_SERVER sSmeltingItem_Send;
sSMELTINGITEM_SERVER sSmeltingItem_Recv;

// pluto 제작
CManufacture ManufactureItem;
SManufactureItem g_sManufactureItem;
SManufactureItem g_sManufactureItemBackUp;
SManufactureItem_Server g_sManufactureItem_Send;
SManufactureItem_Server g_sManufactureItem_Recv;
SManufacture_Rune g_sManufacture_Rune[MAX_RECIPE_KIND];
SManufacture_ResultItemInfo g_Manufacture_ItemInfo[MAX_HANDMADEITEM]; // 장별 - 제작 // 장별 - 대장장이의 혼
//SManufacture_ResultItemInfo_Server g_Item_Recv;

sCRAFTITEM_SERVER sCraftItem_Send; //서버로 보내질 아이템 
sCRAFTITEM_SERVER sCraftItem_Recv; //서버에서 받을 아이템 

sCRAFTITEM_SERVER sWingItem_Send; //서버로 보내질 윙아이템
sCRAFTITEM_SERVER sWingItem_Recv; //서버에서 받을 아이템 

int MixItemNoCopyFlag = 0;


int CraftItemButtonIndex = 0;
int CraftItemSendServerIndex = 0;

/////////////// 믹스쳐 복사 방지를 위해서 
int CheckMixItemCopyFlag = 0;

int ShowItemCraftMessageFlag = 0; //아이템이 조합됬을때 메세지를 보여준다 
int ShowItemAgingMessageFlag = 0; //아이템이 에이징됬을때 메세지를 보여준다
int ShowItemSmeltingMessageFlag = 0;	// pluto 아이템이 제련 됬을때
int ShowItemManufactureMessageFlag = 0; // pluto 아이템이 제작 됬을때
int ShowItemOverSpace = 0; // pluto 공간부족

char szAgingItemBuff[128];
char szCraftItemBuff[128];
POINT CraftItemMessageBoxPosi = { 0,0 }; //박스 위치 
POINT CraftItemMessageSize = { 0,0 };    //박스 사이즈 


sITEM* BackUpInvenItem2;
sITEM* BackUpInvenTempItem2;
sINVENTORY* BackUpsInven;
int   InvenGold = 0;
int   RealGold = 0;

int TradeCheckFlag = 0;

int CopyItemIndex7[10] = { 0,0,0,0,0,0,0,0,0,0 }; //복사된 아이템을 찾기 편하게 하기위한 플랙 

int TradeColorIndex = 0;			//배경 컬러 인덱스 
int	TradeCrashItemIndex[2] = { 0,0 }; //겹쳐질 아이템 인덱스 
RECT TradeColorRect = { 0,0,0,0 };
int  SelectTradeItemIndex = 0;		//선택아이템 배경 
int RequestTradeButtonFlag = 0;		//버튼 플랙 

int ItemKindFlag = 0; //1은 내꺼  2는 거래자꺼 

/////////////////////// 승인 관련 
int CheckDelayFlag = 0;
int sinCancelFlag = 0; //취소만 할수있다 

/////////////////////// 해킹 방지 
int TempShowMoney = 0;
int TempShowCoin = 0;
///////////////////////
/*        창고        */
int CloseWareHouseFlag = 0;

///////////////////////

/////////////////////// 사기방지 
int ItemPickUpFlag = 0;

//우체국에서 아이템을 받는다
sPOSTBOX_ITEM sPostbox_Item[50];
int PostBoxIndexCnt = 0;
DWORD ExpressItemCode = 0; //서버에서 받을 아이템코드
char  szExpressItemName[64]; //받을 아이템 이름 

///////////////////////
char* PostBoxDocFilePath[] = {
	"image\\Sinimage\\shopall\\PostBox.sin",

};
//haGoon아이템 재구성 시스템 
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


//////거래요구창 좌표 
#define TRADEBUTTONMAIN_X		513
#define TRADEBUTTONMAIN_Y		3

#include "..\\nettype.hpp"

//아이템 크래프트
sCRAFTITEM_INFO sCraftItem_Info[MAX_CRAFTITEM_INFO] = {

#ifdef _W_SERVER
#include "sinCraftItem.h"
#else
#include "_sinCraftItem.h"
#endif
};

//아이템 Make
sMAKEITEM_INFO sMakeItem_Info[MAX_MAKEITEM_INFO] = {
	#include "sinMakeItem.h"
};



/*
	{21,136,21+(22*9),136+(22*4)},	//셋팅박스
	{221,163-3-20,240,182-3-20},	//돈버튼 좌표
	{244,163-3-20,307,180-3-20},	//돈표시 좌표
	{248,191-3-20,308,209-3-20},	//무게
	{251,220,251+26,220+26}	//체크
*/
int UpTradePosi[5][4] = {
	{21,136 + sinInterHeight2,21 + (22 * 9),136 + (22 * 4) + sinInterHeight2},	//셋팅박스 
	{231,170 + sinInterHeight2,250,189 + sinInterHeight2},	//돈버튼
	{251,120,326,137},	//돈표시 좌표 
	{251,118,326,162},	//무게  (왜 이렇게 이상하게 되있는지 나두 멀겠따 -_-
	{252,221 + sinInterHeight2,252 + 26,221 + 26 + sinInterHeight2}	//체크 

};

int DownTradePosi[6][4] = {
	{21,224 + 21 + sinInterHeight2,21 + (22 * 9),224 + 21 + (22 * 4) + sinInterHeight2},	//셋팅박스 
	{225,205,250,226},	//돈버튼 좌표 
	{251,206,326,224},	//돈표시 좌표 
	{251,232,326,250},	//무게 
	{252 + 1,310 + sinInterHeight2,252 + 26,310 + 26 + sinInterHeight2},//체크 
	{305,314 + sinInterHeight2,304 + 20,314 + 20 + sinInterHeight2},	//닫기 

};

//좌표셋팅 
int TradeStartX = DownTradePosi[T_SET_BOX][0], TradeStartY = DownTradePosi[T_SET_BOX][1], TradeEndX = DownTradePosi[T_SET_BOX][2], TradeEndY = DownTradePosi[T_SET_BOX][3];

char szAgingMsgBuff[128];
char* SheltomName[MAX_SHELTOM] = { "Lucidy", "Fadeo", "Sparki", "Raident", "Transparo", "Murki", "Devine", "Celesto", "Miraje", "툶nferna", "Enigma", "Bellum", "Ordo", "Sapphire" };

/////임시적으로 에이징에 관련된 쉘텀을 셋팅한다 

//에이징에 쓰일 쉘텀 

#ifdef __SIN_AGING_TEST

//공격횟수 
int AgingLevelAttack[21] = { 2,4,6,8,10,11,12,13,14,15,10,10,10,10,10,10,10,10,10,10,10 };

//크리티컬 횟수 
int AgingLevelCritical[21] = { 2,4,6,8,10,11,12,13,14,15,10,10,10,10,10,10,10,10,10,10,10 };

//블럭횟수
int AgingLevelBlock[21] = { 2,4,6,8,10,11,12,13,14,15,10,10,10,10,10,10,10,10,10,10,10 };

//타격횟수 
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

	//추가
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	// 16부터
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0},

};
int AgingOkPercent[20] = { 0,5,15,30,50,60,70,80,90,95,10,10,10,10,10,10,10,10,10,10 };


#else 

/////////////////////에이징 레벨별 타격횟수 
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
*							클래스 초기, 종료
*-----------------------------------------------------------------------------*/
cTRADE::cTRADE()
{

}
cTRADE::~cTRADE()
{

}
/*----------------------------------------------------------------------------*
*							     초기화
*-----------------------------------------------------------------------------*/
void cTRADE::Init() {}
/*----------------------------------------------------------------------------*
*						    Bmp파일을 로드한다
*-----------------------------------------------------------------------------*/
void cTRADE::Load() {}
/*----------------------------------------------------------------------------*
*						     Release
*-----------------------------------------------------------------------------*/
void cTRADE::Release() {}
/*----------------------------------------------------------------------------*
*							     그리기
*-----------------------------------------------------------------------------*/
DWORD TradeSpotTimer = 0;

void cTRADE::Draw() {}
/*----------------------------------------------------------------------------*
*							     메인
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
*							     종료
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
			lstrcpy(szCraftItemBuff,"모든속성 +5\r기력 +10\r명중력 +50\r 최대공격력+5\r"); //아이템 정보를 보여준다
			CraftItemMessageSize.x = 14;           //박스 사이즈
			CraftItemMessageSize.y = ((3)*2)+2;  //박스 사이즈


		}
	*/
}

extern int AgingEvento;

void cTRADE::DrawTradeText() {}

/////////////// 트레이드 관련 함수 

//셋팅할 영역을 찾는다 
int cTRADE::SetTradeItemAreaCheck(sITEM* pItem)
{

	int i, j;
	TradeColorIndex = 0; //초기화   (트레이드 창과 창고가 같이 쓴다)
	TradeCrashItemIndex[0] = 0; //충돌된 아이템 Index 초기화 
	InitTradeColorRect(); //초기화

	TradeStartX = DownTradePosi[T_SET_BOX][0];
	TradeStartY = DownTradePosi[T_SET_BOX][1];
	TradeEndX = DownTradePosi[T_SET_BOX][2];
	TradeEndY = DownTradePosi[T_SET_BOX][3];

	for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22) {
		for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22) {
			if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j) { //인벤토리 박스에 들어갔는지를 체크 
				TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
				TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
				TradeColorRect.right = pItem->w;
				TradeColorRect.bottom = pItem->h;
				if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
					InitTradeColorRect(); //컬러 박스 RECT 초기화 (컬러 박스 RECT중 하나라두 0 이면 셋팅할수 없다 
					return FALSE;
				}

				pItem->SetX = TradeColorRect.left;
				pItem->SetY = TradeColorRect.top;
				pItem->ItemPosition = 0; //배열의 인덱스를 저장한다 
				TradeColorIndex = SET_ITEM_CHECK_COLOR; //셋팅할영역

				if (TradeCrashItemIndex[0] = CrashTradeItem(TradeColorRect)) {  //겹치는 아이템이 있나 체크 
					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;
				}
				// 박재원 - 거래 관련 아이템 설정
				//증정용 아이템이나 복사아이템은 트레이드 할수없다  (포지션이 있는 아이템도 트레이드할수없다)
				if (pItem->sItemInfo.SpecialItemFlag[0] == CHECK_COPY_ITEM ||
					pItem->sItemInfo.SpecialItemFlag[0] == CHECK_GIVE_ITEM ||
					pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinQW1 ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinBI1 ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinBI2 ||		//패키지캐쉬 아이템도 트레이드 할수없다.
					//파티복 코스튬 

					(pItem->sItemInfo.CODE == (sinDA1 | sin32)) || //박재원 : 산타아뮬렛sin32과 파티복 sin32 아이템이 겹쳐서 구분함					
					(pItem->sItemInfo.CODE == (sinDA2 | sin32)) || //박재원 : 산타아뮬렛sin32과 파티복 sin32 아이템이 겹쳐서 구분함
					(pItem->sItemInfo.CODE == (sinDA1 | sin33)) || //박재원 : 이벤트아뮬렛sin33과 파티복 sin33 아이템이 겹쳐서 구분함					
					(pItem->sItemInfo.CODE == (sinDA2 | sin33)) || //박재원 : 이벤트아뮬렛sin33과 파티복 sin33 아이템이 겹쳐서 구분함	
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
					(pItem->sItemInfo.CODE == (sinOA2 | sin31)) || (pItem->sItemInfo.CODE == (sinOA2 | sin32)) || // 박재원 - 슈퍼 암릿(7일, 30일) 추가
					(pItem->sItemInfo.CODE == (sinDB1 | sin31)) || (pItem->sItemInfo.CODE == (sinDB1 | sin32)) || // 박재원 - 스피드 부츠(7일, 30일) 추가
					(pItem->sItemInfo.CODE == (sinDA1 | sin54)) || (pItem->sItemInfo.CODE == (sinDA2 | sin54)) || // 박재원 - 수영복 복장 추가 - 거래 금지(거래금지)
					(pItem->sItemInfo.CODE == (sinDA1 | sin55)) || (pItem->sItemInfo.CODE == (sinDA2 | sin55)) || // 박재원 - 수영복 복장 추가 - 거래 금지(거래금지)
//					(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin48 ||	//한복 Kyle수정
pItem->sItemInfo.CODE == (sinGF1 | sin01) || (pItem->sItemInfo.CODE == (sinOR2 | sin33)) || // 장별 - 하트링(7일) 추가
//pItem->sItemInfo.CODE == (sinGF1|sin02) ||
(pItem->sItemInfo.CODE == (sinOA1 | sin36)) ||	 // 장별 - 눈꽃 목걸이(7일) 추가 
(pItem->sItemInfo.CODE == (sinOA1 | sin37)) ||   // 장별 - 캔디데이즈 하트아뮬렛(7일) 추가
(pItem->sItemInfo.CODE == (sinDB1 | sin33)) ||	 // 장별 - 스피드 부츠(1일) 추가
(pItem->sItemInfo.CODE == (sinOA2 | sin33)) ||   // 장별 - 슈퍼 암릿(1일)
(pItem->sItemInfo.CODE == (sinBI1 | sin84)) ||	 // 장별 - 그라비티 스크롤
(pItem->sItemInfo.CODE == (sinDB1 | sin34)) ||	 // 장별 - 스피드 부츠(1시간) 추가
(pItem->sItemInfo.CODE == (sinOA2 | sin34)) ||   // 장별 - 슈퍼 암릿(1시간)
pItem->sItemInfo.CODE == (sinGF1 | sin07) ||	// 장별 - 조사원을 찾아라
pItem->sItemInfo.CODE == (sinGF1 | sin08) ||
pItem->ItemPosition) {
					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;

				}

				int kk = 0;
				//등록된 아이템은 트레이드 할수없다
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

				//이벤트 아이템은 트레이드할수없다 
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
//박스 초기화 
int cTRADE::InitTradeColorRect()
{
	TradeColorRect.left = 0;
	TradeColorRect.top = 0;
	TradeColorRect.right = 0;
	TradeColorRect.bottom = 0;
	return TRUE;

}
//아이템이 겹쳐졌는지를 찾는다 
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
					return i + 1; //충돌 체크된 아이템 인덱스를 하나더해서 리턴한다 
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
					return i + 1; //충돌 체크된 아이템 인덱스를 하나더해서 리턴한다 
				}
			}
		}

	}
	if (Kind == 2) {	//크래프트 
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
					return i + 1; //충돌 체크된 아이템 인덱스를 하나더해서 리턴한다 
				}
			}
		}
	}
	if (Kind == 3) { //에이징 
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
					return i + 1; //충돌 체크된 아이템 인덱스를 하나더해서 리턴한다 
				}
			}
		}
	}
	// pluto 제련
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
	// pluto 제작
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

	// 석지용 - 믹스쳐 리셋
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

				// 해당 아이템의 좌표를 얻는다.
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

//겹친 아이템을 셋팅한다  
/*
int cTRADE::ChangeTradeItem(sITEM *pItem)
{

	if(TradeCrashItemIndex[0]){//충돌 된 아이템이 있으면 아이템을 바꿔준다
		memcpy(&TempItem,&sTrade.TradeItem[TradeCrashItemIndex[0]-1],sizeof(sITEM)); //마우스 아이템을 템프로 복사
		sTrade.TradeItem[TradeCrashItemIndex[0]-1].Flag = 0 ; //복사한후에 초기화
		TempItem.x = pItem->x;
		TempItem.y = pItem->y;
		LastSetTradeItem(pItem); //마우스 아이템을 인벤토리로 복사
		memcpy(pItem,&TempItem,sizeof(sITEM));
		SendTradeItem(TradeCharCode);
		return TRUE;

	}
	return FALSE;
}
*/
//트레이드창으로  아이템으로 복사한다 
int cTRADE::LastSetTradeItem(sITEM* pItem)
{
	CheckTradeItemForm();

	for (int j = 0; j < MAX_TRADE_ITEM; j++) {
		if (!sTrade.TradeItem[j].Flag) {
			if (!CheckTrade((void*)&sTrade)) //잘못된 데이타 이면 거래창을 닫는다
				CancelTradeItem();

			memcpy(&sTrade.TradeItem[j], pItem, sizeof(sITEM));
			sTrade.TradeItem[j].x = pItem->SetX;
			sTrade.TradeItem[j].y = pItem->SetY;
			pItem->Flag = 0; //초기화 
			sTrade.CheckFlag = 0; //아이템이 셋팅되면 플랙을 초기화한다 
			//sinPlaySound(sTrade.TradeItem[j].SoundIndex);
			ReformTrade((void*)&sTrade);
			GetWeight(); //무게를 체크한다 
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
	SelectTradeItemIndex = 0;    //집을 아이템 
	ItemKindFlag = 0;			 //아이템 배경을 나타내기 위해서 보정해준다 
	for (i = 0; i < MAX_TRADE_ITEM; i++) {
		if (sTrade.TradeItem[i].Flag) {
			if (sTrade.TradeItem[i].x < x && sTrade.TradeItem[i].x + sTrade.TradeItem[i].w > x &&
				sTrade.TradeItem[i].y < y && sTrade.TradeItem[i].y + sTrade.TradeItem[i].h > y) {
				if (PickUpFlag) {
					CheckTradeItemForm();
					if (!CheckTrade((void*)&sTrade)) //잘못된 데이타 이면 거래창을 닫는다
						CancelTradeItem();
					memcpy(&MouseItem, &sTrade.TradeItem[i], sizeof(sITEM));
					sTrade.TradeItem[i].Flag = 0;
					//sinPlaySound(sTrade.TradeItem[i].SoundIndex);
					TradeColorIndex = 0;
					CheckDelayFlag = 1;
					sTrade.CheckFlag = 0;
					ReformTrade((void*)&sTrade);
					GetWeight(); //무게를 체크한다 
					SendTradeItem(TradeCharCode);
					ReFormTradeItem();
					return TRUE;
				}
				else {
					SelectTradeItemIndex = i + 1;
					TradeColorIndex = SELECT_ITEM_COLOR;
					ItemKindFlag = 1;
					sinShowItemInfoFlag = 1; //아이템의 정보를 보여준다 
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
				sinShowItemInfoFlag = 1; //아이템의 정보를 보여준다 
				sTradeRecv.TradeItem[i].y -= T_REVISION_Y;
				cItem.ShowItemInfo(&sTradeRecv.TradeItem[i], 2);
				sTradeRecv.TradeItem[i].y += T_REVISION_Y;
				return TRUE;

			}
		}
	}

	return FALSE;
}


/* 셋팅 */
int cTRADE::CheckTradeItemSet()
{
	int i = 0;
	int TempWeight = 0;

	memcpy(&CheckItem, &cInvenTory.InvenItem, sizeof(sITEM) * 100); //공간 체크를 위해서 복사 
	memcpy(&CheckRecvItem, &sTradeRecv.TradeItem, sizeof(sITEM) * MAX_TRADE_ITEM); //체크를 위해 복사 

	////무게 관련 
	for (i = 0; i < MAX_TRADE_ITEM; i++) {
		if (CheckRecvItem[i].Flag) {
			if (CheckRecvItem[i].Class == ITEM_CLASS_POTION)//물약일경우는 포션 카운트를 정해준다 
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

	//소지할수있는 돈을 초과했을경우 
	if (sTradeRecv.Money > 0) {
		if (!cInvenTory.CheckMoneyLimit(sTradeRecv.Money - 193))
			return FALSE;

	}

	return TRUE;
}

//임시 셋팅 
int cTRADE::TradeCheckEmptyArea(sITEM* pItem)
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect = { 0,0,0,0 };
	int		flag = 0;

	int StartX = 23;  //인벤 박스의 시작점 X (트레이드 박스가 아님!!!!)
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - pItem->w; //아이템이 들어갈수있는 가장 끝 X좌표 
	cy = (22 * INVENTORY_BOX_COL) - pItem->h; //아이템이 들어갈수있는 가장 끝 Y좌표

	//코드가 같지 않거나 아이템이 비어있지 않은 곳에 셋팅할때 검색해서 위치를 찾아낸다 
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
				if (ChecketInvenItem(pItem)) //오토셋팅일경우에는 인자1을 넘긴다 
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
			pItem->Flag = 0; //초기화 
			return TRUE;

		}

	return FALSE;
}

//인벤토리로 아이템을 셋팅한다 
int cTRADE::CheckOkTradeItem()
{
	int i = 0;
	for (i = 0; i < MAX_TRADE_ITEM; i++) {
		if (sTradeRecv.TradeItem[i].Flag) {
			if (cInvenTory.CheckRequireItemToSet(&sTradeRecv.TradeItem[i])) {         //곧바로 셋팅될수있는지를 체크한다  
				if (!cInvenTory.AutoSetInvenItem(&sTradeRecv.TradeItem[i])) {
					if (sinThrowItemToFeild(&sTradeRecv.TradeItem[i])) {//아이템을 버린다 
						sTradeRecv.TradeItem[i].Flag = 0;
					}

				}
			}
		}
	}
	if (sTradeRecv.Money > 0) {
		CheckCharForm();//인증 
		//sinChar->Money += sTradeRecv.Money-193;
		sinPlusMoney(sTradeRecv.Money - 193);
		ReformCharForm();//재인증 	
		SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 
		//sinPlaySound(SIN_SOUND_COIN);
	}
	cTrade.OpenFlag = FALSE;				//트레이드 성공시 창을 닫는다 
	TempShowMoney = 0;
	ItemPickUpFlag = 0; //플랙 초기화 

	cInvenTory.CheckWeight();	//인벤토리 의 무게를 보정한다 

	return TRUE;
}


//트레이드 취소시 아이템을  인벤토리로 셋팅한다 
int cTRADE::CancelTradeItem()
{
	int sArrowPosi = 0;

	if (TradeItemSucessFlag) {
		memset(&sTrade, 0, sizeof(sTRADE));		//비정상적인 트레이드 종료시 아이템을 날린다 
		memset(&sTradeRecv, 0, sizeof(sTRADE));

	}
	int i = 0;
	for (i = 0; i < MAX_TRADE_ITEM; i++) {
		if (sTrade.TradeItem[i].Flag) {
			if (cInvenTory.CheckRequireItemToSet(&sTrade.TradeItem[i])) { //곧바로 셋팅될수있는지를 체크한다  
				if (!cInvenTory.AutoSetInvenItem(&sTrade.TradeItem[i])) {
					//아이템이 꽉차있을경우는 인벤토리를 뒤집어서 셋팅한다
					if (ArrowState[0] == 0)sArrowPosi = 2; //요기는 인벤창고 
					else sArrowPosi = 1;
					sinButtonFlag = sArrowPosi;
					SelectInvenItemIndex = 0; //에테르 코어남발을 막기위해 초기화
					cInvenTory.ChangeABItem(sArrowPosi); // 2보다 작을때 인벤 박스 

					if (!cInvenTory.AutoSetInvenItem(&sTrade.TradeItem[i])) { //그래두 안되면 버린다 팔자려니 해라
						if (sinThrowItemToFeild(&sTrade.TradeItem[i])) {//아이템을 버린다 
							sTrade.TradeItem[i].Flag = 0;
						}
					}

					/*
					//요기는 무기포지션(현제 사용하고있지않다)
					if(ArrowState[1]==0)sArrowPosi = 4;
					else sArrowPosi = 3;
					sinButtonFlag = sArrowPosi;
					sinChangeSetFlag = sArrowPosi;
					*/

					if (sinThrowItemToFeild(&sTrade.TradeItem[i])) {//아이템을 버린다 
						sTrade.TradeItem[i].Flag = 0;
					}
				}
			}
		}
	}

	if (sTrade.Money > 0) {
		CheckCharForm();//인증 
		//sinChar->Money += sTrade.Money-193;
		sinPlusMoney(sTrade.Money - 193);
		ReformCharForm();//재인증 	
		SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 
	}


	//마우스에 아이템이 있을경우에는 마우스 아이템도 인벤에 곧바로 셋팅한다 
	if (MouseItem.Flag) {
		if (cInvenTory.CheckRequireItemToSet(&MouseItem)) {         //곧바로 셋팅될수있는지를 체크한다  
			if (!cInvenTory.AutoSetInvenItem(&MouseItem)) {
				if (sinThrowItemToFeild(&MouseItem)) {//아이템을 버린다 
					MouseItem.Flag = 0;
				}
			}
		}

	}

	//메세지 박스 관련은 잠시 대기 


	//트레이드 취소시 창을 닫아준다
	cTrade.OpenFlag = FALSE;


	memset(&sTrade, 0, sizeof(sTRADE));		//셋팅된후 값들을 초기화해준다 
	memset(&sTradeRecv, 0, sizeof(sTRADE));	//셋팅된후 값들을 초기화해준다 
	TempShowMoney = 0;
	ItemPickUpFlag = 0; //플랙 초기화 

	CloseEachPlayer();
	cInvenTory.CheckWeight();	//인벤토리 의 무게를 보정한다
	return TRUE;
}

//아이템 이미지를 로드한다 
int cTRADE::LoadTradeItemIamge() { return TRUE; }

int cTRADE::CheckTradeButtonOk()
{
	///////////////트레이드 체크 
	if (!TradeSendSucessFlag && sTrade.CheckFlag && sTradeRecv.CheckFlag) { //교환! 
		SendTradeCheckItem(TradeCharCode);
		TradeSendSucessFlag = TRUE;
	}

	return TRUE;
}

//무게를 얻어온다 
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
/*							여기서 부터는 창고
/*******************************************************************************************/

//아이템이 셋팅될 공간을 찾는다 
int cWAREHOUSE::SetWareHouseItemAreaCheck(sITEM* pItem)
{
	int i, j;
	TradeColorIndex = 0; //초기화
	TradeCrashItemIndex[0] = 0; //충돌된 아이템 Index 초기화 
	cTrade.InitTradeColorRect(); //초기화

	TradeStartX = 21;
	TradeStartY = 136 + sinInterHeight2;
	TradeEndX = TradeStartX + (22 * 9);
	TradeEndY = TradeStartY + (22 * 9);

	for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22) {
		for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22) {
			if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j) { //인벤토리 박스에 들어갔는지를 체크 
				TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
				TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
				TradeColorRect.right = pItem->w;
				TradeColorRect.bottom = pItem->h;
				if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
					cTrade.InitTradeColorRect(); //컬러 박스 RECT 초기화 (컬러 박스 RECT중 하나라두 0 이면 셋팅할수 없다 
					return FALSE;
				}
				//퀘스트 아이템일경우에는 아이템을 창고에 넣을수엄따 
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
				//등록된 아이템은 창고에 넣을수없다
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
				//////////// 이벤트 송편
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
				pItem->ItemPosition = 0; //배열의 인덱스를 저장한다 
				TradeColorIndex = SET_ITEM_CHECK_COLOR; //셋팅할영역

				if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 1)) {  //겹치는 아이템이 있나 체크 
					//요기서 복사아이템을 체크한다 
					if (!CopyItemNotPickUp(&sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1], TradeCrashItemIndex[0] - 1)) {
						TradeColorIndex = NOT_SETTING_COLOR;
						TradeCrashItemIndex[0] = 0; //초기화 
						TradeCrashItemIndex[1] = 0; //초기화 
						return FALSE;

					}
					if (TradeCrashItemIndex[1] = cTrade.CrashTradeItem(TradeColorRect, TradeCrashItemIndex[0], 1)) {  //겹치는 아이템이 있나 체크 
						TradeColorIndex = NOT_SETTING_COLOR;
						TradeCrashItemIndex[1] = 0; //초기화 
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


//아이템을 셋팅한다 
int cWAREHOUSE::LastSetWareHouseItem(sITEM* pItem)
{
	CheckWareHouseForm();

	for (int j = 0; j < 100; j++) {
		if (!sWareHouse.WareHouseItem[j].Flag) {
			memcpy(&sWareHouse.WareHouseItem[j], pItem, sizeof(sITEM));
			sWareHouse.WareHouseItem[j].x = pItem->SetX;
			sWareHouse.WareHouseItem[j].y = pItem->SetY;
			pItem->Flag = 0; //초기화 
			//sinPlaySound(sWareHouse.WareHouseItem[j].SoundIndex);
			GetWeight(); //무게를 체크한다 
			ReFormWareHouse();
			return TRUE;

		}
	}
	ReFormWareHouse();
	return FALSE;
}

//창고를 닫는다 
int cWAREHOUSE::CloseWareHouse()
{
	int RestoreFlag = 1;

	CloseWareHouseCheckFlag3 = 1;
	cWareHouse.OpenFlag = 0; //창고를 닫는다 
	if (WareHouseSaveFlag) {
		if (!SaveWareHouse(&sWareHouse)) { //아이템을 저장시킨다 
			cWareHouse.RestoreInvenItem(); //저장이 되지않으면 복구해준다 

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
	WareHouseSaveFlag = 0;//플랙 초기화 
	ResetInvenItemCode();		//이벤토리 아이템 코드 초기화 ********************
	ResetInvenMoney();			//이벤토리 돈 검사 초기화
	return RestoreFlag;
}



//아이템을 집는다 
int cWAREHOUSE::PickUpWareHouseItem(int x, int y, int PickUpFlag)
{
	int i = 0;
	SelectTradeItemIndex = 0;    //집을 아이템 
	for (i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			if (sWareHouse.WareHouseItem[i].x < x && sWareHouse.WareHouseItem[i].x + sWareHouse.WareHouseItem[i].w > x &&
				sWareHouse.WareHouseItem[i].y < y && sWareHouse.WareHouseItem[i].y + sWareHouse.WareHouseItem[i].h > y) {
				if (PickUpFlag) {

					//수정 하대용
					//창고이용시 인벤토리 빈공간이 없을경우는 이용이 불가능하다/
					//가상의 공간 체크용 임시 아이템
					sITEM TempItem;
					TempItem.w = ITEMSIZE * 2;
					TempItem.h = ITEMSIZE * 4;
					TempItem.Flag = 1;
					if (cInvenTory.CheckSetEmptyArea(&TempItem)) {
						////////////요기서 복사된 아이템은 들수가없다 
						if (CopyItemNotPickUp(&sWareHouse.WareHouseItem[i], i)) {
							CheckWareHouseForm();
							memcpy(&MouseItem, &sWareHouse.WareHouseItem[i], sizeof(sITEM));
							sWareHouse.WareHouseItem[i].Flag = 0;
							//sinPlaySound(sWareHouse.WareHouseItem[i].SoundIndex);
							TradeColorIndex = 0;
							GetWeight(); //무게를 체크한다 
							ReFormWareHouse();
							return TRUE;
						}
					}
					else
					{

						break;
					}

					////////////요기서 복사된 아이템은 들수가없다 
					if (CopyItemNotPickUp(&sWareHouse.WareHouseItem[i], i)) {
						CheckWareHouseForm();
						memcpy(&MouseItem, &sWareHouse.WareHouseItem[i], sizeof(sITEM));
						sWareHouse.WareHouseItem[i].Flag = 0;
						//sinPlaySound(sWareHouse.WareHouseItem[i].SoundIndex);
						TradeColorIndex = 0;
						GetWeight(); //무게를 체크한다 
						ReFormWareHouse();
						return TRUE;
					}

				}
				else {
					SelectTradeItemIndex = i + 1;
					TradeColorIndex = SELECT_ITEM_COLOR;
					sinShowItemInfoFlag = 1; //아이템의 정보를 보여준다 
					cItem.ShowItemInfo(&sWareHouse.WareHouseItem[i], 2);
				}
			}
		}
	}
	return FALSE;
}

//겹친 아이템을 교환한다 
int cWAREHOUSE::ChangeWareHouseItem(sITEM* pItem)
{
	if (pItem->Class == ITEM_CLASS_POTION) {	//물약은 창고에 보관할수없다 

		return TRUE;

	}
	if (pItem->sItemInfo.Weight + (sWareHouse.Weight[0] - 197) > sWareHouse.Weight[1] - 196) {

		return TRUE;

	}

	CheckWareHouseForm();

	if (TradeCrashItemIndex[0]) {//충돌 된 아이템이 있으면 아이템을 바꿔준다 
		if (sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1].Class == ITEM_CLASS_POTION) {
			if (sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1].CODE == pItem->CODE) {
				sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1].sItemInfo.PotionCount += pItem->sItemInfo.PotionCount;
				pItem->Flag = 0;
				GetWeight();
				ReFormWareHouse();
				return TRUE;

			}
		}
		memcpy(&TempItem, &sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1], sizeof(sITEM)); //마우스 아이템을 템프로 복사 
		sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1].Flag = 0; //복사한후에 초기화
		TempItem.x = pItem->x;
		TempItem.y = pItem->y;
		ReFormWareHouse();
		LastSetWareHouseItem(pItem); //마우스 아이템을 인벤토리로 복사 
		memcpy(pItem, &TempItem, sizeof(sITEM));
		GetWeight();
		ReFormWareHouse();
		return TRUE;

	}
	ReFormWareHouse();
	return FALSE;
}

//무게를 구한다 
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
	WareHouseSaveFlag = 1; //아이템이 바뀌게 되면 값을 셋팅해준다 	

	return TRUE;
}

//아이템의 이미지와 아이템 크기를 셋팅한다 
int cWAREHOUSE::LoadWareHouseItemIamge() { return TRUE; }

//인벤토리 아이템을 잠시 백업한다 
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


//아이템을 복구해준다 
int cWAREHOUSE::RestoreInvenItem()
{
	//복구하기전에 현재 착용중인 아이템을 벗는다 (안해두 되는데 걍 한다 )
	sinSetCharItem(cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[0].ItemIndex - 1].SetModelPosi, FALSE);
	sinSetCharItem(cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[1].ItemIndex - 1].SetModelPosi, FALSE);
	sinSetCharItem(cInvenTory.InvenItem[sInven[2].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[2].ItemIndex - 1].SetModelPosi, FALSE);

	memcpy(&cInvenTory.InvenItem, BackUpInvenItem2, sizeof(sITEM) * 100);
	memcpy(&cInvenTory.InvenItemTemp, BackUpInvenTempItem2, sizeof(sITEM) * 100);
	memcpy(&sInven, BackUpsInven, sizeof(sINVENTORY) * INVENTORY_MAX_POS);

	//마우스 아이템을 없애준다
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

	if (sInven[0].ItemIndex) { //무기 
		sinSetCharItem(cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[0].ItemIndex - 1].SetModelPosi, TRUE);

	}

	if (sInven[1].ItemIndex) { //방패 
		sinSetCharItem(cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[1].ItemIndex - 1].SetModelPosi, TRUE);

	}

	if (sInven[2].ItemIndex) { //갑옷 
		sinSetCharItem(cInvenTory.InvenItem[sInven[2].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[2].ItemIndex - 1].SetModelPosi, TRUE);

	}
	if ((InvenGold - 2023) == (RealGold / 3) - 2023) {
		CheckCharForm();//인증 
		sinChar->Money = InvenGold - 2023;
		ReformCharForm();//재인증 	

	}
	else {
		SendSetHackUser(6); //해킹을 하려고했던 못된 유저를 고발 TRUE 접속 종료 

	}
	InvenGold = 0;
	RealGold = 0;

	cInvenTory.SetItemToChar(); //파라메터 셋팅 
	cInvenTory.ReFormInvenItem(); //아이템 인증을 다시 받는다 
	cInvenTory.CheckWeight();	  //무게 셋팅 	

	return TRUE;
}

/*****************************************************************************/
/*							아이템 조합										 */
/*****************************************************************************/
//아이템이 들어갈수있는지를 체크한다 
int cCRAFTITEM::SetCraftItemAreaCheck(sITEM* pItem, int Kind)
{

	int i, j;
	TradeColorIndex = 0; //초기화   (트레이드 창과 창고가 같이 쓴다) 
	TradeCrashItemIndex[0] = 0; //충돌된 아이템 Index 초기화 
	cTrade.InitTradeColorRect(); //초기화

	// 박재원 - 리스펙 스톤(리스펙(아이템 재구성)창에서 리스펙 스톤 자리에 아이템을 넣을 때 마우스에 달려있는 상태)
	if (pItem->sItemInfo.CODE == (sinBI1 | sin90))
	{
		int ReconStoneX = 159 + 11, ReconStoneY = 268 + 11 + sinInterHeight2;
		TradeColorIndex = 0; //초기화   (트레이드 창과 창고가 같이 쓴다)
		if (ReconStoneX <= pItem->x + 22 && ReconStoneX > pItem->x && ReconStoneY < pItem->y + 22 && ReconStoneY > pItem->y)
		{
			pItem->SetX = ReconStoneX + (((pItem->x + 22 - ReconStoneX) / 22) * 22) - 11;
			pItem->SetY = ReconStoneY + (((pItem->y + 22 - ReconStoneY) / 22) * 22) - 11;
			pItem->ItemPosition = 0; //배열의 인덱스를 저장한다 
			TradeColorIndex = SET_ITEM_CHECK_COLOR; //셋팅할영역
			if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
				cTrade.InitTradeColorRect(); //컬러 박스 RECT 초기화 (컬러 박스 RECT중 하나라두 0 이면 셋팅할수 없다 
				return FALSE;
			}
			if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 2)) {  //겹치는 아이템이 있나 체크 
				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;
			}
			return TRUE;
		}
	}

	if ((pItem->CODE & sinITEM_MASK2) == sinOS1 || (pItem->CODE & sinITEM_MASK2) == sinSE1) { //쉘텀일 경우 || 아이템 재구성 씰 
		TradeStartX = 218;
		TradeStartY = 193 + sinInterHeight2;
		TradeEndX = TradeStartX + (3 * 22);
		TradeEndY = TradeStartY + (4 * 22);

		for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22) {
			for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22) {
				if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j) { //인벤토리 박스에 들어갔는지를 체크 
					TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
					TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
					TradeColorRect.right = pItem->w;
					TradeColorRect.bottom = pItem->h;
					if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
						cTrade.InitTradeColorRect(); //컬러 박스 RECT 초기화 (컬러 박스 RECT중 하나라두 0 이면 셋팅할수 없다 
						return FALSE;
					}

					pItem->SetX = TradeColorRect.left;
					pItem->SetY = TradeColorRect.top;
					pItem->ItemPosition = 0; //배열의 인덱스를 저장한다 
					TradeColorIndex = SET_ITEM_CHECK_COLOR; //셋팅할영역

					//포스 오브
					if (cCraftItem.ForceFlag)
					{
						//돈이 부족할 경우 
						//--------------------------------------------------------------------------//



						// Kyle 이니그마까지 사용
						if (!smConfig.DebugMode) {
							if ((pItem->sItemInfo.CODE & sinITEM_MASK3) >= sin12) {
								TradeColorIndex = NOT_SETTING_COLOR;
								return FALSE;
							}
						}


						/*
						// pluto 셀레스토까지 사용
						if(!smConfig.DebugMode){
							if((pItem->sItemInfo.CODE & sinITEM_MASK3) >= sin10){
								TradeColorIndex = NOT_SETTING_COLOR;
								return FALSE;
							}
						}
						*/
						/*
						//데비네까지 사용
						if(!smConfig.DebugMode)
						{//gm모드가 되는건 gm모드만 되게한거다. 유저모드는 안되는거다!
							if((pItem->sItemInfo.CODE & sinITEM_MASK3) >= sin09)
							{
								TradeColorIndex = NOT_SETTING_COLOR;
								return FALSE;
							}
						}
						*/
					}

					//퀘스트 아이템일 경우에는 아이템을 셋팅할수 엄따 
					if (pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinQW1 ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinPZ1 || //퍼즐도 올릴수엄따
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinPZ2 ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinFO1 ||
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinSP1 || // pluto 선물상자 송편... 계열
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinWR1 || // 박재원 - 무기 제작서 계열
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinDR1 || // 박재원 - 방어구 제작서 계열
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinGP1 || // 박재원 - 크리스탈 계열
						(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinCH1 || // 박재원 - 초콜릿, 사탕 계열
						(pItem->sItemInfo.CODE >= (sinOR2 | sin06) && pItem->sItemInfo.CODE <= (sinOR2 | sin25)) || // 박재원 : 클랜치프링 계열
						pItem->sItemInfo.SpecialItemFlag[0] == CHECK_GIVE_ITEM ||
						pItem->sItemInfo.CODE == (sinGF1 | sin01) ||
						pItem->sItemInfo.CODE == (sinGF1 | sin02) ||
						pItem->sItemInfo.CODE == (sinGF1 | sin03) ||
						pItem->sItemInfo.CODE == (sinGF1 | sin04) ||
						pItem->sItemInfo.CODE == (sinGF1 | sin05) ||
						pItem->sItemInfo.CODE == (sinGF1 | sin06) ||
						pItem->sItemInfo.CODE == (sinOR2 | sin27) || // 박재원 - 산타 링 추가
						pItem->sItemInfo.CODE == (sinOA1 | sin32) || // 박재원 - 산타 아뮬렛 추가
						pItem->sItemInfo.CODE == (sinOR2 | sin28) || // 박재원 - 이벤트 링(7일) 추가
						pItem->sItemInfo.CODE == (sinOA1 | sin33) || // 박재원 - 이벤트 아뮬렛(7일) 추가
						pItem->sItemInfo.CODE == (sinOR2 | sin29) || // 박재원 - 이벤트 링(1시간) 추가
						pItem->sItemInfo.CODE == (sinOA1 | sin34) || // 박재원 - 이벤트 아뮬렛(1시간) 추가
						pItem->sItemInfo.CODE == (sinOR2 | sin30) || // 박재원 - 이벤트 링(1일) 추가
						pItem->sItemInfo.CODE == (sinOA1 | sin35) || // 박재원 - 이벤트 아뮬렛(1일) 추가
						pItem->sItemInfo.CODE == (sinOA2 | sin31) || // 박재원 - 슈퍼 암릿(7일) 추가
						pItem->sItemInfo.CODE == (sinOA2 | sin32) || // 박재원 - 슈퍼 암릿(30일) 추가
						pItem->sItemInfo.CODE == (sinDB1 | sin31) || // 박재원 - 스피드 부츠(7일) 추가
						pItem->sItemInfo.CODE == (sinDB1 | sin32) || // 박재원 - 스피드 부츠(30일) 추가
						pItem->sItemInfo.CODE == (sinSP1 | sin15) || // 박재원 - 수박 아이템 추가
						pItem->sItemInfo.CODE == (sinSP1 | sin27) || // 박재원 - 알파벳 조합 이벤트 추가
						pItem->sItemInfo.CODE == (sinSP1 | sin28) || // 박재원 - 알파벳 조합 이벤트 추가
						pItem->sItemInfo.CODE == (sinSP1 | sin29) || // 박재원 - 알파벳 조합 이벤트 추가
						pItem->sItemInfo.CODE == (sinSP1 | sin30) || // 박재원 - 알파벳 조합 이벤트 추가
						pItem->sItemInfo.CODE == (sinSP1 | sin31) || // 박재원 - 알파벳 조합 이벤트 추가
						pItem->sItemInfo.CODE == (sinSP1 | sin32) || // 박재원 - 알파벳 조합 이벤트 추가
						pItem->sItemInfo.CODE == (sinSP1 | sin33) || // 박재원 - 알파벳 조합 이벤트 추가
						pItem->sItemInfo.CODE == (sinOR2 | sin31) || // 박재원 - 보스 몬스터 링 추가(바벨)
						pItem->sItemInfo.CODE == (sinOR2 | sin32) ||  // 박재원 - 보스 몬스터 링 추가(퓨리)
						pItem->sItemInfo.CODE == (sinSP1 | sin35) || // 장별 - 발렌타인 초콜릿 아이템 추가
						pItem->sItemInfo.CODE == (sinOR2 | sin33) ||   // 장별 - 하트링(7일) 아이템 추가
						pItem->sItemInfo.CODE == (sinOA1 | sin36) || // 장별 - 눈꽃 목걸이(7일) 아이템 추가
						pItem->sItemInfo.CODE == (sinSP1 | sin36) || // 장별 - 캔디데이즈 캔디 아이템 추가
						pItem->sItemInfo.CODE == (sinOA1 | sin37) ||   // 장별 - 하트아뮬렛(7일) 아이템 추가
						pItem->sItemInfo.CODE == (sinOA2 | sin33) || // 장별 - 슈퍼 암릿(1일)
						pItem->sItemInfo.CODE == (sinDB1 | sin33) || // 장별 - 스피드 부츠(1일) 추가
						pItem->sItemInfo.CODE == (sinSP1 | sin34) || // 박재원 - 호랑이 캡슐 추가
						pItem->sItemInfo.CODE == (sinSP1 | sin37) || // 장별 - 매지컬그린 비취 아이템 추가
						pItem->sItemInfo.CODE == (sinSP1 | sin38) || // 장별 - 매지컬그린 에메랄드 아이템 추가
						pItem->sItemInfo.CODE == (sinBI1 | sin84) || // 장별 - 그라비티 스크롤 추가
						pItem->sItemInfo.CODE == (sinDB1 | sin34) || // 장별 - 스피드 부츠(1시간) 추가
						pItem->sItemInfo.CODE == (sinOA2 | sin34) || // 장별 - 슈퍼 암릿(1시간)
						pItem->sItemInfo.CODE == (sinSP1 | sin39) || // 장별 - 카라의 눈물 아이템 추가
						pItem->sItemInfo.CODE == (sinGF1 | sin07) || // 장별 - 조사원을 찾아라
						pItem->sItemInfo.CODE == (sinGF1 | sin08) ||	// 장별 - 조사원을 찾아라
						pItem->sItemInfo.CODE == (sinOR2 | sin36) || // 장별 - 소울스톤
						pItem->sItemInfo.CODE == (sinOR2 | sin37) ||
						pItem->sItemInfo.CODE == (sinOR2 | sin38) ||
						pItem->sItemInfo.CODE == (sinOR2 | sin39) ||
						pItem->sItemInfo.CODE == (sinOR2 | sin40) ||
						pItem->sItemInfo.CODE == (sinOA1 | sin39) ||
						pItem->sItemInfo.CODE == (sinOA1 | sin40) ||
						pItem->sItemInfo.CODE == (sinOA1 | sin41) ||
						pItem->sItemInfo.CODE == (sinOA1 | sin42) ||
						pItem->sItemInfo.CODE == (sinOA1 | sin38) || // 장별 - 복날 이벤트 아뮬렛, 링 
						pItem->sItemInfo.CODE == (sinOR2 | sin34) ||
						pItem->sItemInfo.CODE == (sinOR2 | sin35))


					{
						TradeColorIndex = NOT_SETTING_COLOR;
						return FALSE;

					}

					// 박재원 - 매직 쉘텀 조합 금지
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
					//등록된 아이템은 믹스할수없다
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

					if (Kind == 0) { //믹스쳐 
						if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 2)) {  //겹치는 아이템이 있나 체크 
							TradeColorIndex = NOT_SETTING_COLOR;
							return FALSE;

						}
						return TRUE;

					}
					if (Kind == 1) { //에이징 
						if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //겹치는 아이템이 있나 체크 
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
		//포스 오브
		if (cCraftItem.ForceFlag)return FALSE;
		TradeColorIndex = 0; //초기화   (트레이드 창과 창고가 같이 쓴다)
		TradeCrashItemIndex[0] = 0; //충돌된 아이템 Index 초기화 
		cTrade.InitTradeColorRect(); //초기화

		TradeStartX = 58;
		TradeStartY = 193 + sinInterHeight2;
		TradeEndX = TradeStartX + (3 * 22);
		TradeEndY = TradeStartY + (4 * 22);

	}
	return TRUE;
}

//아이템을 셋팅한다 
// 박재원 - 포스 오브 제작 비용 수정
int ForceOrbPriceIndex[] = { 200,500,1000,2000,4000,5000,10000,20000,30000,40000,50000,60000,70000,80000 };  // 박재원 - 벨룸, 오르도 포스 비용 추가
// 박재원 - 매직 포스 추가(매직 포스 오프 제작 비용)
int MagicForceOrbPriceIndex[] = { 200,500,1000,2000,4000,5000,10000,20000,30000,40000,50000,60000,70000,80000 };

DWORD SheltomCode2[] = { sin01,sin02,sin03,sin04,sin05,sin06,sin07,sin08,sin09,sin10,sin11,sin12,sin13,sin14,sin15 }; // 박재원 - 벨룸, 오르도 추가 

// 박재원 - 매직 포스 추가
DWORD MagicSheltomCode[] = { sin21,sin22,sin23,sin24,sin25,sin26,sin27,sin28,sin29,sin30,sin31,sin32, sin33, sin34 };

// 박재원 - 빌링 매직 포스 추가
DWORD BillingMagicSheltomCode[] = { sin35, sin36, sin37 };

int cCRAFTITEM::CheckForceOrbPrice()
{
	ForceItemPrice = 0;
	for (int j = 1; j < 15; j++) {
		if (sCraftItem.CraftItem[j].Flag) {
			for (int t = 0; t < 14; t++) // 박재원 - 벨룸, 오르도 포스 추가
			{
				if ((sCraftItem.CraftItem[j].sItemInfo.CODE & sinITEM_MASK3) <= sin20)
				{
					if ((sCraftItem.CraftItem[j].sItemInfo.CODE & sinITEM_MASK3) == SheltomCode2[t])
					{
						ForceItemPrice += ForceOrbPriceIndex[t];
						break;
					}
				}
				else // 쉘텀 코드 sin21 ~
				{ // 박재원 - 매직 포스 추가
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
				// 박재원 - 매직 포스 추가
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
		// 박재원 - 매직 포스 추가
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
	if (Kind == 0) { //크래프트 아이템 
		CheckCraftItemForm(); //조작하는 상놈의 자식들을 죽여버리자 퉤!!
		if ((pItem->CODE & sinITEM_MASK2) == sinOS1) { //쉘텀일 경우 
			for (int j = 1; j < 15; j++) {
				if (!sCraftItem.CraftItem[j].Flag) {
					memcpy(&sCraftItem.CraftItem[j], pItem, sizeof(sITEM));
					sCraftItem.CraftItem[j].x = pItem->SetX;
					sCraftItem.CraftItem[j].y = pItem->SetY;
					pItem->Flag = 0; //초기화 
					//sinPlaySound(sCraftItem.CraftItem[j].SoundIndex);
					ReFormCraftItem();
					CheckForceOrbPrice();
					return TRUE;
				}
			}
		}
		//HaGoon아이템 재구성 ===================================================================================================
		else if ((pItem->CODE & sinITEM_MASK2) == sinSE1) {
			if (sCraftItem.CraftItem[0].Flag) {
				TempLevel = sCraftItem.CraftItem[0].sItemInfo.Level;
				for (int k = 0; k < MAX_SEEL_COUNT; k++) { // 박재원 - 테이와즈 씰 추가 (3종류 -> 4종류)
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
					pItem->Flag = 0; //초기화 
					//sinPlaySound(sCraftItem.CraftItem[j].SoundIndex);
					ReFormCraftItem();
					return TRUE;
				}
			}

		}
		//=======================================================================================================================
		// 박재원 - 리스펙 스톤
		else if ((pItem->CODE & sinITEM_MASK2) == sinBI1)
		{
			if ((pItem->CODE & sinITEM_MASK3) == sin90)
			{
				if (!sCraftItem.CraftItem[14].Flag) {
					memcpy(&sCraftItem.CraftItem[14], pItem, sizeof(sITEM));
					sCraftItem.CraftItem[14].x = pItem->SetX;
					sCraftItem.CraftItem[14].y = pItem->SetY;
					pItem->Flag = 0; //초기화 
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
				pItem->Flag = 0; //초기화 
				//sinPlaySound(sCraftItem.CraftItem[0].SoundIndex);
				ReFormCraftItem();
				return TRUE;
			}

		}
	}

	if (Kind == 1) {	//아이템 에이징 
		//CheckCraftItemForm(); //조작하는 상놈의 자식들을 죽여버리자 퉤!!
		if ((pItem->CODE & sinITEM_MASK2) == sinOS1) { //쉘텀일 경우 
			for (int j = 1; j < 15; j++) {
				if (!sAgingItem.AgingItem[j].Flag) {
					memcpy(&sAgingItem.AgingItem[j], pItem, sizeof(sITEM));
					sAgingItem.AgingItem[j].x = pItem->SetX;
					sAgingItem.AgingItem[j].y = pItem->SetY;
					pItem->Flag = 0; //초기화 
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
				pItem->Flag = 0; //초기화 
				//sinPlaySound(sAgingItem.AgingItem[0].SoundIndex);
				//		ReFormCraftItem();
				return TRUE;
			}

		}

	}
	if (Kind == 2) {	//아이템 Make
		for (int j = 0; j < 15; j++) {
			if (!sAgingItem.AgingItem[j].Flag) {
				memcpy(&sAgingItem.AgingItem[j], pItem, sizeof(sITEM));
				sAgingItem.AgingItem[j].x = pItem->SetX;
				sAgingItem.AgingItem[j].y = pItem->SetY;
				pItem->Flag = 0; //초기화 
				//sinPlaySound(sAgingItem.AgingItem[j].SoundIndex);
				return TRUE;
			}
		}
	}
	if (Kind == 3) {  //에이징 스톤 // 코퍼 오어 // 박재원 - 엘더 코퍼 오어, 슈퍼 에이징 스톤   // 장별 - 슈퍼 에이징 스톤 1.5
		if (!sAgingItem.AgingItem[14].Flag) {
			memcpy(&sAgingItem.AgingItem[14], pItem, sizeof(sITEM));
			sAgingItem.AgingItem[14].x = pItem->SetX;
			sAgingItem.AgingItem[14].y = pItem->SetY;
			pItem->Flag = 0; //초기화 
			//sinPlaySound(sAgingItem.AgingItem[14].SoundIndex);
			return TRUE;
		}
	}

	return TRUE;

}

//아이템을 선택한다 
int cCRAFTITEM::PickUpCraftItem(int x, int y, int PickUpFlag, int Kind)
{

	int i = 0;
	SelectTradeItemIndex = 0;    //집을 아이템 
	if (Kind == 0) {
		for (i = 0; i < 15; i++) {
			if (sCraftItem.CraftItem[i].Flag) {
				if (sCraftItem.CraftItem[i].x < x && sCraftItem.CraftItem[i].x + sCraftItem.CraftItem[i].w > x &&
					sCraftItem.CraftItem[i].y < y && sCraftItem.CraftItem[i].y + sCraftItem.CraftItem[i].h > y) {
					if (PickUpFlag) {
						//쉘텀 조합 상태에서 에이징을 캔슬하지 못한다.======================================================
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
						haCraftSortItem(&sCraftItem);  //아이템 재구성 때문에 배열 이동
						//memset(&sCraftItem.CraftItem[i],0,sizeof(sITEM));  //서버 해킹 에러 때문에 임시 방편이다.

						//sinPlaySound(sCraftItem.CraftItem[i].SoundIndex);
						TradeColorIndex = 0;
						ReFormCraftItem();
						if (ForceFlag)
							CheckForceOrbPrice(); // Forece Orb 플랙있을경우엔 포스가격을 표시

						if (i == 0) {  //씰아이템 때문에 
							for (int i = 1; i < 15; i++) {
								if (sCraftItem.CraftItem[i].Flag) {
									if ((sCraftItem.CraftItem[i].CODE & sinITEM_MASK2) == sinSE1) {
										if (!cInvenTory.AutoSetInvenItem(&sCraftItem.CraftItem[i])) {
											sinThrowItemToFeild(&sCraftItem.CraftItem[i]);
											sCraftItem.CraftItem[i].Flag = 0;
											memset(&sCraftItem.CraftItem[i], 0, sizeof(sCRAFTITEM)); //아이템을 다 날려버린다 
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
						sinShowItemInfoFlag = 1; //아이템의 정보를 보여준다 
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
						//쉘텀 조합 상태에서 에이징을 캔슬하지 못한다.======================================================
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
						//에이징 스톤 // 코퍼 오어
						if (sAgingItem.AgingItem[i].sItemInfo.CODE == (sinBI1 | sin10) ||
							sAgingItem.AgingItem[i].sItemInfo.CODE == (sinBI1 | sin11) ||
							// 박재원 - 엘더 코퍼 오어, 슈퍼 에이징 스톤 추가
							// 장별 - 슈퍼 에이징 스톤 1.5
							sAgingItem.AgingItem[i].sItemInfo.CODE == (sinBI1 | sin60) ||
							sAgingItem.AgingItem[i].sItemInfo.CODE == (sinBI1 | sin61) ||
							sAgingItem.AgingItem[i].sItemInfo.CODE == (sinBI1 | sin85)) break;
						TradeColorIndex = 0;
						//Make Item   (플렉없이 그냥 KindCode만 체크한다 )플렉은 위에서 지웠다 -_-;
						if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
							(MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinMA2) {
							sinMakeItemCheck();

						}
						else {
							if (i == 0) {
								for (int y = 0; y < 4; y++) { //쉘텀 인덱스 초기화 
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
								case SIN_ADD_ATTACK_RATE:	//명중력 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Rating += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Rating += (sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Rating * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Rating +=  (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Rating*(sCraftItem_Info[i].											AddElement[k]/100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_ATTACK_RATE;
									break;
								case SIN_ADD_DAMAGE_MIN:	//최소대미지
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[0] += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[0] += (sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[0] * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[0] +=  (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[0]*(sCraftItem_Info[i].AddElement[k]/100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_DAMAGE_MIN;
									break;

								case SIN_ADD_DAMAGE_MAX:	//최대대미지
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[1] += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[1] += (sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[1] * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[1] +=  (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Damage[1]*(sCraftItem_Info[i].AddElement[k]/100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_DAMAGE_MAX;
									break;
								case SIN_ADD_ATTACK_SPEED:	//공격속도
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Speed += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Speed += (sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Speed * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Speed += (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Attack_Speed*(sCraftItem_Info[i].AddElement[k]/											100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_ATTACK_SPEED;
									break;
								case SIN_ADD_ABSORB:	//흡수력 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fAbsorb += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fAbsorb += (GetItemAbsorb(&sCraftItem_Recv.DesCraftItem.sItemInfo) * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_ABSORB;
									break;
								case SIN_ADD_DEFENCE:	//방어력 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.Defence += (int)sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.Defence += (sCraftItem_Recv.DesCraftItem.sItemInfo.Defence * ((int)sCraftItem_Info[i].AddElement[k])) / 100;
									//sCraftItem_Recv.DesCraftItem.sItemInfo.Defence += (int)((float)sCraftItem_Recv.DesCraftItem.sItemInfo.Defence*(sCraftItem_Info[i].AddElement[k]/100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_DEFENCE;
									break;
								case SIN_ADD_BLOCK_RATE: //블럭율 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fBlock_Rating += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fBlock_Rating += (sCraftItem_Recv.DesCraftItem.sItemInfo.fBlock_Rating * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_BLOCK_RATE;
									break;
								case SIN_ADD_MOVE_SPEED: //이동속도 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fSpeed += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fSpeed += (sCraftItem_Recv.DesCraftItem.sItemInfo.fSpeed * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_MOVE_SPEED;
									break;

								case SIN_ADD_LIFE: //최대 생명력 
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Life += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Life += (sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Life * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_LIFE;
									break;

								case SIN_ADD_MANA: //최대 기력
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Mana += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Mana += (sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Mana * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_MANA;
									break;
								case SIN_ADD_STAMINA: //최대 근력
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Stamina += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Stamina += (sCraftItem_Recv.DesCraftItem.sItemInfo.fIncrease_Stamina * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_STAMINA;
									break;
								case SIN_ADD_LIFEREGEN: //생명력재생
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fLife_Regen += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fLife_Regen += (sCraftItem_Recv.DesCraftItem.sItemInfo.fLife_Regen * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_LIFEREGEN;
									break;
								case SIN_ADD_MANAREGEN: //기력재생
									if (sCraftItem_Info[i].ElementKind[k] == SIN_ADD_NUM)
										sCraftItem_Recv.DesCraftItem.sItemInfo.fMana_Regen += sCraftItem_Info[i].AddElement[k];
									else
										sCraftItem_Recv.DesCraftItem.sItemInfo.fMana_Regen += (sCraftItem_Recv.DesCraftItem.sItemInfo.fMana_Regen * (sCraftItem_Info[i].AddElement[k] / 100));
									sCraftItem_Recv.DesCraftItem.sItemInfo.ItemKindMask |= SIN_ADD_MANAREGEN;

									break;
								case SIN_ADD_STAMINAREGEN: //근력재생
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

//아이템이 들어갈수있는지 체크한다 
int cCRAFTITEM::CraftCheckEmptyArea(sITEM* pItem)
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;
	int StartX = 23;  //인벤 박스의 시작점 X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - pItem->w; //아이템이 들어갈수있는 가장 끝 X좌표 
	cy = (22 * INVENTORY_BOX_COL) - pItem->h; //아이템이 들어갈수있는 가장 끝 Y좌표

	//코드가 같지 않거나 아이템이 비어있지 않은 곳에 셋팅할때 검색해서 위치를 찾아낸다 
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

//믹스쳐 아이템을 취소한다 
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

	cCraftItem.OpenFlag = 0; //창을 닫는다 
	memset(&sCraftItem, 0, sizeof(sCRAFTITEM)); //아이템을 다 날려버린다 
	ResetInvenItemCode();				//이벤 아이템 코드 초기화

	memset(&sCraftItem_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //받은 믹스쳐 구조체 초기화  (일단 같이해준다)
	memset(&sCraftItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //보낸 믹스쳐 구조체 초기화 

	//Force Orb 초기화
	ForceFlag = 0;
	ForceItemPrice = 0;
	ForceItemPrice2 = 0;
	return TRUE;
}

int CraftItemCheckDelayTime = 0;

//크래프트 아이템을 조작하는 망할 유저를 병신 만든다 
int cCRAFTITEM::CheckHackCraftItem()
{
	//현제 서버에서 하도록 되어있어 적용이 안된다 
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
		SendSetHackUser(8); //해킹을 하려고했던 못된 유저를 고발 TRUE 접속 종료 

	}

	return TRUE;
}

//////////믹스쳐 아이템 조작 방지를 위한 힘겨운 노력 
int cCRAFTITEM::CheckCraftItemForm()
{ //체크  
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
		//if(TempCheckDataSum != CraftItemCheckSum) //필요없느거 같서리 뺏다 하핫
			//SendSetHackUser(9); //해킹을 하려고했던 못된 유저를 고발 TRUE 접속 종료 
	}
	return TRUE;

}

int cCRAFTITEM::ReFormCraftItem()
{ //인증 

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

//////////트레이드  아이템 조작 방지를 위한 힘겨운 노력 헥헥헥
//ReFormTradeItem();
//CheckTradeItemForm();
int cTRADE::CheckTradeItemForm()
{ //체크  
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
		SendSetHackUser(10); //해킹을 하려고했던 못된 유저를 고발 TRUE 접속 종료 

	return TRUE;

}

int cTRADE::ReFormTradeItem()
{ //인증 

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
////////////////////창고를 조작하려는 나쁜 자식들을 벌주기 위한 힘겨운 노력 !!!
int cWAREHOUSE::CheckWareHouseForm()
{ //체크  
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
		SendSetHackUser(11); //해킹을 하려고했던 못된 유저를 고발 TRUE 접속 종료 

	return TRUE;

}

int cWAREHOUSE::ReFormWareHouse()
{ //인증 

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

////////////////////////////////////// 에이징

//아이템을 에이징한다 
int cAGING::AgingItem(int MakeItemFlag)
{
	int ResultCount = 0;
	int ResultCountCheck = 0;
	DWORD TempCODE[16] = { 0, };
	int  i = 0;
	//코드 복사

	if (sAging_Recv.A_StoneItemCode == (sinBI1 | sin10))      //에이징 스톤
		cAging.AginStoneKind = 1;
	else if (sAging_Recv.A_StoneItemCode == (sinBI1 | sin11)) //코퍼 오어
		cAging.AginStoneKind = 2;
	else if (sAging_Recv.A_StoneItemCode == (sinBI1 | sin60)) // 박재원 - 엘더 코퍼 오어
		cAging.AginStoneKind = 3;
	else if (sAging_Recv.A_StoneItemCode == (sinBI1 | sin61)) // 박재원 - 슈퍼 에이징 스톤
		cAging.AginStoneKind = 4;
	else if (sAging_Recv.A_StoneItemCode == (sinBI1 | sin85)) // 장별 - 슈퍼 에이징 스톤 1.5
		cAging.AginStoneKind = 5;
	else
		cAging.AginStoneKind = 0;

	if (sAging_Recv.DesCraftItem.sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON) { //퀘스트 아이템일경우
		if (SetChangeJob3QuestItem(&sAging_Recv.DesCraftItem.sItemInfo)) {
			ReformMixItem(&sAging_Recv.DesCraftItem.sItemInfo); //재인증 (아이템을 인벤토리에 셋팅하기전에 ...)
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

	// Aging quebrando aqui, 0 = quebra padr�o = 3
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

	for (int y = 0; y < 4; y++) { //쉘텀 인덱스 초기화 
		for (int h = 0; h < 3; h++) {
			AgingLevelSheltomIndex[y][h] = 0;
		}

	}

	AgingCheckSheltomFlag = 0; //에이징 가능 플랙 초기화 
	AgingSheltomCnt2 = 0;
	if (!Flag)
		cAging.OpenFlag = 0; //창을 닫는다 

	ResetInvenItemCode();				//이벤 아이템 코드 초기화
	return TRUE;
}

//에이징할수있는 아이템인지를 체크한다 
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
				AgingLevelSheltomIndex[i][j] = AgingLevelSheltom[pItem->sItemInfo.ItemAgingNum[0]][k++]; //쉘텀의 종류와 인덱스를 넣는다 

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

// 박재원 - 에이징 아이템 추가(장갑, 부츠, 암릿)
DWORD AgingItemCode3[12] = { sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1,sinDA1,sinDS1,sinOM1,sinDA2 };//sinDG1,sinDB1,sinOA2};
//에이징할 아이템의 공간을 체크한다 
int cAGING::SetAgingItemAreaCheck(sITEM* pItem)
{
	// 박재원 - 에이징 설정 변경(오르도 추가)
	DWORD SheltomCODE[MAX_SHELTOM] = { sin01,sin02,sin03,sin04,sin05,sin06,sin07,sin08,sin09,sin10,sin11,sin12,sin13,sin14,sin15 };//sin15}; 
	int SheltomIndex2 = 0;
	int i, j;
	//에이징 스톤/ 코퍼 오어-------------------------------------------------------------
	if (pItem->sItemInfo.CODE == (sinBI1 | sin10) || pItem->sItemInfo.CODE == (sinBI1 | sin11) ||
		pItem->sItemInfo.CODE == (sinBI1 | sin60) || pItem->sItemInfo.CODE == (sinBI1 | sin61) || // 박재원 - 엘더 코퍼 오어, 슈퍼 에이징 스톤 추가
		pItem->sItemInfo.CODE == (sinBI1 | sin85)) // 장별 - 슈퍼 에이징 스톤 1.5
	{
		cAging.AginStoneKind = 0;
		int AgingStoneX = 159 + 11, AgingStoneY = 268 + 11 + sinInterHeight2;
		TradeColorIndex = 0; //초기화   (트레이드 창과 창고가 같이 쓴다)
		cTrade.InitTradeColorRect(); //초기화
		if (AgingStoneX <= pItem->x + 22 && AgingStoneX > pItem->x && AgingStoneY< pItem->y + 22 && AgingStoneY > pItem->y) {
			pItem->SetX = AgingStoneX + (((pItem->x + 22 - AgingStoneX) / 22) * 22) - 11;
			pItem->SetY = AgingStoneY + (((pItem->y + 22 - AgingStoneY) / 22) * 22) - 11;
			pItem->ItemPosition = 0; //배열의 인덱스를 저장한다 
			TradeColorIndex = SET_ITEM_CHECK_COLOR; //셋팅할영역
			if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
				cTrade.InitTradeColorRect(); //컬러 박스 RECT 초기화 (컬러 박스 RECT중 하나라두 0 이면 셋팅할수 없다 
				return FALSE;
			}
			if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //겹치는 아이템이 있나 체크 
				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;
			}
			return TRUE;
		}
	}

	if (pItem->sItemInfo.CODE == (sinBI1 | sin61)) // 박재원 - 엘더 코퍼 오어, 슈퍼 에이징 스톤 추가
	{
		cAging.AginStoneKind = 4;
		int AgingStoneX = 159 + 11, AgingStoneY = 268 + 11 + sinInterHeight2;
		TradeColorIndex = 0; //초기화   (트레이드 창과 창고가 같이 쓴다)
		cTrade.InitTradeColorRect(); //초기화
		if (AgingStoneX <= pItem->x + 22 && AgingStoneX > pItem->x && AgingStoneY< pItem->y + 22 && AgingStoneY > pItem->y) {
			pItem->SetX = AgingStoneX + (((pItem->x + 22 - AgingStoneX) / 22) * 22) - 11;
			pItem->SetY = AgingStoneY + (((pItem->y + 22 - AgingStoneY) / 22) * 22) - 11;
			pItem->ItemPosition = 0; //배열의 인덱스를 저장한다 
			TradeColorIndex = SET_ITEM_CHECK_COLOR; //셋팅할영역
			if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
				cTrade.InitTradeColorRect(); //컬러 박스 RECT 초기화 (컬러 박스 RECT중 하나라두 0 이면 셋팅할수 없다 
				return FALSE;
			}
			if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //겹치는 아이템이 있나 체크 
				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;
			}
			return TRUE;
		}
	}


	// 장별 - 슈퍼 에이징 스톤 1.5
	if (pItem->sItemInfo.CODE == (sinBI1 | sin85))
	{
		cAging.AginStoneKind = 5;
		int AgingStoneX = 159 + 11, AgingStoneY = 268 + 11 + sinInterHeight2;
		TradeColorIndex = 0; //초기화   (트레이드 창과 창고가 같이 쓴다)
		cTrade.InitTradeColorRect(); //초기화
		if (AgingStoneX <= pItem->x + 22 && AgingStoneX > pItem->x && AgingStoneY< pItem->y + 22 && AgingStoneY > pItem->y) {
			pItem->SetX = AgingStoneX + (((pItem->x + 22 - AgingStoneX) / 22) * 22) - 11;
			pItem->SetY = AgingStoneY + (((pItem->y + 22 - AgingStoneY) / 22) * 22) - 11;
			pItem->ItemPosition = 0; //배열의 인덱스를 저장한다 
			TradeColorIndex = SET_ITEM_CHECK_COLOR; //셋팅할영역
			if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
				cTrade.InitTradeColorRect(); //컬러 박스 RECT 초기화 (컬러 박스 RECT중 하나라두 0 이면 셋팅할수 없다 
				return FALSE;
			}
			if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //겹치는 아이템이 있나 체크 
				TradeColorIndex = NOT_SETTING_COLOR;
				return FALSE;
			}
			return TRUE;
		}
	}


	//-----------------------------------------------------------------------------------
	if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2) {
		if (sAgingItem.AgingItem[0].Flag && (sAgingItem.AgingItem[0].sItemInfo.CODE & sinITEM_MASK2) == sinMA1) {
			TradeColorIndex = 0; //초기화   (트레이드 창과 창고가 같이 쓴다)
			TradeCrashItemIndex[0] = 0; //충돌된 아이템 Index 초기화 
			cTrade.InitTradeColorRect(); //초기화
			TradeStartX = 218;
			TradeStartY = 193 + sinInterHeight2;
			TradeEndX = TradeStartX + (3 * 22);
			TradeEndY = TradeStartY + (4 * 22);

			for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22) {
				for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22) {
					if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j) { //인벤토리 박스에 들어갔는지를 체크 
						TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
						TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
						TradeColorRect.right = pItem->w;
						TradeColorRect.bottom = pItem->h;
						if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
							cTrade.InitTradeColorRect(); //컬러 박스 RECT 초기화 (컬러 박스 RECT중 하나라두 0 이면 셋팅할수 없다 
							return FALSE;
						}

						pItem->SetX = TradeColorRect.left;
						pItem->SetY = TradeColorRect.top;
						pItem->ItemPosition = 0; //배열의 인덱스를 저장한다 
						TradeColorIndex = SET_ITEM_CHECK_COLOR; //셋팅할영역

						if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //겹치는 아이템이 있나 체크 
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
		TradeColorIndex = 0; //초기화   (트레이드 창과 창고가 같이 쓴다)
		TradeCrashItemIndex[0] = 0; //충돌된 아이템 Index 초기화 
		cTrade.InitTradeColorRect(); //초기화

		TradeStartX = 58;
		TradeStartY = 193 + sinInterHeight2;
		TradeEndX = TradeStartX + (3 * 22);
		TradeEndY = TradeStartY + (4 * 22);
	}

	/////////여기는 기본에이징 
	for (int y = 0; y < MAX_SHELTOM; y++) {
		if (SheltomCODE[y] == (pItem->sItemInfo.CODE & sinITEM_MASK3)) {
			SheltomIndex2 = y + 1;
			break;
		}
	}

	int AgingOkWeaponGroupFlag = 0;	//에이징을 할수있는 아이템 군인가?
	TradeColorIndex = 0; //초기화   (트레이드 창과 창고가 같이 쓴다)
	TradeCrashItemIndex[0] = 0; //충돌된 아이템 Index 초기화 
	cTrade.InitTradeColorRect(); //초기화

	if ((pItem->CODE & sinITEM_MASK2) == sinOS1) { //쉘텀일 경우 
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
							if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j) { //인벤토리 박스에 들어갔는지를 체크 
								TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
								TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
								TradeColorRect.right = pItem->w;
								TradeColorRect.bottom = pItem->h;
								if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
									cTrade.InitTradeColorRect(); //컬러 박스 RECT 초기화 (컬러 박스 RECT중 하나라두 0 이면 셋팅할수 없다 
									return FALSE;
								}

								pItem->SetX = TradeColorRect.left;
								pItem->SetY = TradeColorRect.top;
								pItem->ItemPosition = 0; //배열의 인덱스를 저장한다 
								if ((pItem->CODE & sinITEM_MASK3) > sin20) // 박재원 - 매직 포스(에이징할때 매직 쉘텀은 올려놓지 못하도록 한다)
								{
									TradeColorIndex = NOT_SETTING_COLOR;
								}
								else
									TradeColorIndex = SET_ITEM_CHECK_COLOR; //셋팅할영역

								if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 3)) {  //겹치는 아이템이 있나 체크 
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
		TradeColorIndex = 0; //초기화   (트레이드 창과 창고가 같이 쓴다)
		TradeCrashItemIndex[0] = 0; //충돌된 아이템 Index 초기화 
		cTrade.InitTradeColorRect(); //초기화

		TradeStartX = 58;
		TradeStartY = 193 + sinInterHeight2;
		TradeEndX = TradeStartX + (3 * 22);
		TradeEndY = TradeStartY + (4 * 22);

	}


	return TRUE;
}
/*
int CraftItemCheckDelayTime = 0;

//크래프트 아이템을 조작하는 망할 유저를 병신 만든다  (서버에서 하는관계로 의미가 없긴하나 혹시나해서 주석처리함)
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
		SendSetHackUser(8); //해킹을 하려고했던 못된 유저를 고발 TRUE 접속 종료

	}

	return TRUE;
}

*/

int AgingItemCheckDelayTime = 0;

//에이징 데이타의 조작여부를 조정한다 
int cAGING::CheckAgingData()
{
	/*
	#ifdef __SIN_AGING_TEST


	#else
	AgingItemCheckDelayTime++;
	if(AgingItemCheckDelayTime < 70*10)return FALSE;
	AgingItemCheckDelayTime = 0;

	unsigned int AddItemCheckSum = 0;
	const unsigned int CheckAddItemData = 1848296; //이렇게 해놓으면 읽기 전용으로만 된다네... 음하하하


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
		SendSetHackUser(107); //해킹을 하려고했던 못된 유저를 고발 TRUE 접속 종료

	}
#endif
	*/

	return TRUE;
}

////////////창고에 복사된아이템이 있는지를 찾는다 
int cWAREHOUSE::CopyItemNotPickUp(sITEM* pItem, int JumpIndex)
{
	int i;
	if (smConfig.DebugMode)return TRUE; //디버그 모드일때는 들수있따 크크크
	for (i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			if (JumpIndex == i)continue; //자신의 아이템은 찾지않는다
			if (CompareItems(&pItem->sItemInfo, &sWareHouse.WareHouseItem[i].sItemInfo)) {
				CopyItemIndex7[i] = i + 1;
				return FALSE;

			}

		}

	}

	return TRUE;
}

int SendServerFlag7 = 0;
//복사된 아이템이 있는지를 체크한다 
int cWAREHOUSE::CheckCopyItem()
{
	int i, j;
	if (SendServerFlag7)return FALSE;
	for (i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			for (j = 0; j < 100; j++) {
				if (i != j && sWareHouse.WareHouseItem[j].Flag) {
					if (CompareItems(&sWareHouse.WareHouseItem[i].sItemInfo, &sWareHouse.WareHouseItem[j].sItemInfo)) {
						SendSetHackUser2(1010, sWareHouse.WareHouseItem[i].CODE); //해킹한 유저를 신고한다
						SendServerFlag7 = 1; //한번만 체크하도록한다 
						break;
					}
				}
			}
		}
	}
	/*
	//덩달아 돈고도 체크 (음... 찝찝하다)
	if(8000000 < sWareHouse.Money-2023-1000000){
			SendSetHackUser2(1110,sWareHouse.Money-2023); //해킹한 유저를 신고한다

	}
	*/


	return TRUE;
}
int CheckCraftMoney = 0;

///////// 믹스쳐 아이템을 서버에 보낸다 
int cCRAFTITEM::sinSendCraftItem()
{

	if (MixItemNoCopyFlag)return FALSE;
	int i = 0;


	//복사를 체크하기위한 백업본을 만든다
	memcpy(&sCraftItemBackUp, &sCraftItem, sizeof(sCRAFTITEM));

	MixItemNoCopyFlag = 1; //아이템이 복사되는걸 방지하기위해 플렉을준다

	if (ForceFlag) { //Force Orb 이면 
		memset(&sCraftItem_Send.DesCraftItem, 0, sizeof(sITEM));
		memset(&sCraftItemBackUp, 0, sizeof(sCRAFTITEM));
		sCraftItem_Send.DesCraftItem.CODE = 1000;
		sCraftItem_Send.Money = ForceItemPrice;

	}
	//haGoon아이템 재구성 을 서버로 보낸다.---------------------------------------------------------------------------
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

	// 박재원 - 리스펙 스톤도 서버로 보낸다.
	if (sCraftItem.CraftItem[14].Flag) {
		sCraftItem_Send.A_StoneItemCode = sCraftItem.CraftItem[14].CODE;
		sCraftItem_Send.A_StoneCheckSum = sCraftItem.CraftItem[14].sItemInfo.ItemHeader.dwChkSum;
		sCraftItem_Send.A_StoneHead = sCraftItem.CraftItem[14].sItemInfo.ItemHeader.Head;
	}

	CheckCraftMoney = sCraftItem_Send.Money;

	CraftItemSendServerIndex++; //인덱스를 보낸다 
	sCraftItem_Send.Index = CraftItemSendServerIndex;
	//저장된 구조체를 서버에 보낸다 
	cCraftItem.iReconItemFlag = 0;

	if (ForceFlag) { //포스 오브는 윙으로 보낸다
		SendWingItemToServer(&sCraftItem_Send);
	}
	else {
		SendCraftItemToServer(&sCraftItem_Send);
	}

	return TRUE;
}

extern int AgingEvento;
extern int AgingPrice;

///////// 에이징 아이템을 서버에 보낸다 
int cAGING::sinSendAgingItem()
{
	int i = 0;
	//아이템을 보낼때 복사를 방지하기위한 백업본을 만든다
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
	//에이징 스톤도 서버로 보낸다.
	if (sAgingItem.AgingItem[14].Flag)
	{
		sAging_Send.A_StoneItemCode = sAgingItem.AgingItem[14].CODE;
		sAging_Send.A_StoneCheckSum = sAgingItem.AgingItem[14].sItemInfo.ItemHeader.dwChkSum;
		sAging_Send.A_StoneHead = sAgingItem.AgingItem[14].sItemInfo.ItemHeader.Head;
	}

#ifdef AGING_MONEY_FREE // 박재원 - 에이징 가격 무료 이벤트
	sAging_Send.Money = 0;
	CheckCraftMoney = sAging_Send.Money;
#else
	sAging_Send.Money = sAgingItem.AgingItem[0].sItemInfo.Price;
	CheckCraftMoney = sAging_Send.Money;
#endif

	CraftItemSendServerIndex++; //인덱스를 보낸다 
	sAging_Send.Index = CraftItemSendServerIndex;

	//저장된 구조체를 서버에 보낸다 
	SendAgingItemToServer(&sAging_Send);


	//////아이템을 보내면서 돈을 뺀다
#ifdef AGING_MONEY_FREE // 박재원 - 에이징 가격 무료 이벤트
	int Price = 0; // 무료
#else
	int Price = ((sAgingItem.AgingItem[0].sItemInfo.ItemAgingNum[0] + 1) * sAgingItem.AgingItem[0].sItemInfo.Price) / 2;
#endif


	CheckCharForm();		//인증 
	sinMinusMoney(Price, 1);
	ReformCharForm();		//재인증 
	SendSaveMoney();		//금액 조작을 못하게하기위해 호출한다 
	return TRUE;
}
///////// 서버에서 믹스쳐 아이템의 구조를 받는다 
int cCRAFTITEM::sinRecvCraftItem(sCRAFTITEM_SERVER* pCraftItem_Server)
{
	memcpy(&sCraftItem_Recv, pCraftItem_Server, sizeof(sCRAFTITEM_SERVER));

	return TRUE;
}

///////// 서버에서 에이징 아이템의 구조를 받는다
int cAGING::sinRecvAgingItem(sCRAFTITEM_SERVER* pCraftItem_Server)
{
	memcpy(&sAging_Recv, pCraftItem_Server, sizeof(sCRAFTITEM_SERVER));

	return TRUE;
}

//에이징 아이템의 결과를 서버에서 받는다 
int cAGING::sinRecvAgingItemResult(sCRAFTITEM_SERVER* pCraftItem_Server)
{

	//퀘스트 아이템일경우
	if (pCraftItem_Server->DesCraftItem.sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON) {
		if (pCraftItem_Server->Result) {
			cInvenTory.DeleteInvenItemToServer(TempQuestItem.sItemInfo.CODE, TempQuestItem.sItemInfo.ItemHeader.Head, TempQuestItem.sItemInfo.ItemHeader.dwChkSum);
			memset(&TempQuestItem, 0, sizeof(sITEM));
			//카운트 갱신
			pCraftItem_Server->DesCraftItem.sItemInfo.ItemAgingCount[0] = pCraftItem_Server->DesCraftItem.sItemInfo.ItemAgingCount[1];
			if (cInvenTory.AutoSetInvenItem(&pCraftItem_Server->DesCraftItem))
			{
				sinQuest_ChangeJob3.Monster[0] = pCraftItem_Server->DesCraftItem.sItemInfo.ItemAgingNum[0];


			}
			//퀘스트 메세지를 보여준다
			if (pCraftItem_Server->DesCraftItem.sItemInfo.ItemAgingNum[0] == 4) {
				sinQuest_ChangeJob3.State = 4;
			}
		}
		else {
			//지웠던 아이템을 복구해준다
			cInvenTory.AutoSetInvenItem(&TempQuestItem);

		}
		memset(&sAging_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //받은 믹스쳐 구조체 초기화 
		memset(&sAging_Send, 0, sizeof(sCRAFTITEM_SERVER)); //보낸 믹스쳐 구조체 초기화 
		NotChangeSetItemFlag = 0; //무기셋팅 체인지 제한을 초기화해준다
		return TRUE;

	}

	if (!cAging.OpenFlag) {
		AgingCheckSheltomFlag = 0; //에이징 가능 플랙 초기화 
		AgingSheltomCnt2 = 0;
		MixItemNoCopyFlag = 0; //복사방지 플렉 초기화
		memset(&sCraftItem_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //받은 믹스쳐 구조체 초기화 
		memset(&sCraftItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //보낸 믹스쳐 구조체 초기화 
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
			SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 
			ReformCharForm(); //재인증 
		}
	}

	for (int y = 0; y < 4; y++) { //쉘텀 인덱스 초기화 
		for (int h = 0; h < 3; h++) {
			AgingLevelSheltomIndex[y][h] = 0;
		}

	}

	for (int i = 0; i < 15; i++) { //에이징 아이템을 날린다
		if (sAgingItemBackUp.AgingItem[i].Flag) {
			cInvenTory.DeleteInvenItemToServer(sAgingItemBackUp.AgingItem[i].sItemInfo.CODE,
				sAgingItemBackUp.AgingItem[i].sItemInfo.ItemHeader.Head,
				sAgingItemBackUp.AgingItem[i].sItemInfo.ItemHeader.dwChkSum);
		}

	}


	memset(&sAgingItem, 0, sizeof(sAGINGITEM));
	memset(&sAgingItemBackUp, 0, sizeof(sAGINGITEM));
	cAging.OpenFlag = 0; //에이징 인터페이스를 닫는다 
	AgingCheckSheltomFlag = 0; //에이징 가능 플랙 초기화 
	AgingSheltomCnt2 = 0;
	MixItemNoCopyFlag = 0; //복사방지 플렉 초기화
	MixCancelButtonDelayFlag = 0;
	memset(&sAging_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //받은 믹스쳐 구조체 초기화 
	memset(&sAging_Send, 0, sizeof(sCRAFTITEM_SERVER)); //보낸 믹스쳐 구조체 초기화 
	cInvenTory.CheckDamage(); //최대데미지가 나올 무기로 계산하여 서버로 보내준다 
	return TRUE;
}

//크래프트 아이템의 결과물을 서버에서 받는다 
int cCRAFTITEM::sinRecvCraftItemResult(sCRAFTITEM_SERVER* pCraftItem_Server)
{
	CheckMixItemCopyFlag = 0; //복사방지 플렉  
	if (!cCraftItem.OpenFlag) {
		MixItemNoCopyFlag = 0; //복사방지 플렉 초기화
		memset(&sCraftItem, 0, sizeof(sCRAFTITEM)); //구조체 초기화 
		memset(&sCraftItemBackUp, 0, sizeof(sCRAFTITEM)); //구조체 초기화 
		memset(&sCraftItem_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //받은 믹스쳐 구조체 초기화 
		memset(&sCraftItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //보낸 믹스쳐 구조체 초기화 
		MixCancelButtonDelayFlag = 0;
		return FALSE;

	}

	int i, LineCount = 0, Temp;

	/*   돈관련 부분은 뺀다
	if(CheckCraftMoney != pCraftItem_Server->Money) //돈이 맞지 않으면 실패로 간주한다
		pCraftItem_Server->Result = 0;
	*/

	if (pCraftItem_Server->Result) {
		if (pCraftItem_Server->DocIndex == 2000) { //아이템 재구성시스템 
			//아이템일 경우 
			sITEM TempItem;
			if (LoadItemImage(&pCraftItem_Server->DesCraftItem.sItemInfo, &TempItem)) {
				if (cInvenTory.CheckRequireItemToSet(&TempItem)) {         //곧바로 셋팅될수있는지를 체크한다  
					//haGoon수정================================================================================
					if (!cInvenTory.AutoSetInvenItem(&TempItem, 1)) {
						sinThrowItemToFeild(&pCraftItem_Server->DesCraftItem);
						memset(&sCraftItem, 0, sizeof(sCRAFTITEM)); //구조체 초기화 
						cCraftItem.OpenFlag = 0;
						return FALSE;
					}
					//===========================================================================================
				}

			}

		}
		else {
			ShowItemCraftMessageFlag = 1; //아이템이 조합됬을때 메세지를 보여준다 
			lstrcpy(szCraftItemBuff, sCraftItem_Info[pCraftItem_Server->DocIndex].Doc); //아이템 정보를 보여준다 

			Temp = lstrlen(szCraftItemBuff);
			for (i = 0; i < Temp; i++) {
				if (szCraftItemBuff[i] == '\r') {
					LineCount++;
				}
			}

			CraftItemMessageSize.x = 14;           //박스 사이즈 
			CraftItemMessageSize.y = ((LineCount) * 2) + 2;  //박스 사이즈 




			//CheckCharForm(); //인증 
			//sinChar->Money -= sCraftItem_Recv.DesCraftItem.sItemInfo.Price;
			//sinMinusMoney(pCraftItem_Server->DesCraftItem.sItemInfo.Price,1);
			//ReformCharForm(); //재인증 
			//SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 
			//실패든 성공이든간에 아이템을 원상 복귀 시킨다 
			//cCraftItem.OpenFlag = 0; //창을 닫는다
			//cInvenTory.AutoSetInvenItem(&pCraftItem_Server->DesCraftItem); //아이템을 셋팅한다 

			//haGoon수정 ============================================================================
			if (!cInvenTory.AutoSetInvenItem(&pCraftItem_Server->DesCraftItem)) {
				sinThrowItemToFeild(&pCraftItem_Server->DesCraftItem);
			}
			//=======================================================================================

			//무기가 복사되었으면 무기를 지운다
			if (sCraftItemBackUp.CraftItem[0].Flag) {
				cInvenTory.DeleteInvenItemToServer(sCraftItemBackUp.CraftItem[0].sItemInfo.CODE,
					sCraftItemBackUp.CraftItem[0].sItemInfo.ItemHeader.Head,
					sCraftItemBackUp.CraftItem[0].sItemInfo.ItemHeader.dwChkSum);
			}

			/*
			for(int y=1;y<15;y++){ //날라간 쉘텀을 서버에 알려준다
				if(sCraftItem.CraftItem[y].Flag)
					SendSaveThrowItem(&sCraftItem.CraftItem[y].sItemInfo);

			}
			*/
		}
		cCraftItem.OpenFlag = 0; //창을 닫는다
		CheckCharForm(); //인증 
		sinMinusMoney(sCraftItem.CraftItem[0].sItemInfo.Price, 1);
		ReformCharForm(); //재인증 
		SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 
		SaveGameData();
	}
	else {


		//haGoon수정============================================================================
		if (!cInvenTory.AutoSetInvenItem(&sCraftItem.CraftItem[0])) { //아이템을 셋팅한다 
			sinThrowItemToFeild(&sCraftItem.CraftItem[0]);
		}
		//=======================================================================================
		/*
		for(int y=1;y<15;y++){ //날라간 쉘텀을 서버에 알려준다
			if(sCraftItem.CraftItem[y].Flag)
				SendSaveThrowItem(&sCraftItem.CraftItem[y].sItemInfo);

		}
		*/

		//믹스쳐 할때 돈을 뺀다 
		CheckCharForm(); //인증 
		sinMinusMoney(sCraftItem.CraftItem[0].sItemInfo.Price, 1);
		ReformCharForm(); //재인증 
		SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 

		cCraftItem.OpenFlag = 0; //창을 닫는다 

		//sinPlaySound(SIN_SOUND_FAILD_MIXITEM);
		//memset(&sCraftItem_Recv.DesCraftItem,&TempItem,sizeof(sITEM)); //아이템 복구 

	}

	//복사가되었거나한 아이템을 지운다 (쉘텀만지운다)
	for (i = 1; i < 15; i++) {
		if (sCraftItemBackUp.CraftItem[i].Flag) {
			cInvenTory.DeleteInvenItemToServer(sCraftItemBackUp.CraftItem[i].sItemInfo.CODE,
				sCraftItemBackUp.CraftItem[i].sItemInfo.ItemHeader.Head,
				sCraftItemBackUp.CraftItem[i].sItemInfo.ItemHeader.dwChkSum);
		}

	}
	memset(&sCraftItem, 0, sizeof(sCRAFTITEM)); //구조체 초기화 
	memset(&sCraftItemBackUp, 0, sizeof(sCRAFTITEM)); //구조체 초기화 
	memset(&sCraftItem_Recv, 0, sizeof(sCRAFTITEM_SERVER)); //받은 믹스쳐 구조체 초기화 
	memset(&sCraftItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //보낸 믹스쳐 구조체 초기화 

	MixItemNoCopyFlag = 0; //복사방지 플렉 초기화
	MixCancelButtonDelayFlag = 0;
	return TRUE;
}

////////복사 아이템을 없애준다 
int cWAREHOUSE::DeleteCopyItem()
{
	int i, j;
	for (i = 0; i < 100; i++) {
		if (sWareHouse.WareHouseItem[i].Flag) {
			for (j = 0; j < 100; j++) {
				if (i != j && sWareHouse.WareHouseItem[j].Flag) {
					if (CompareItems(&sWareHouse.WareHouseItem[i].sItemInfo, &sWareHouse.WareHouseItem[j].sItemInfo)) {
						//SendSetHackUser2(1010,sWareHouse.WareHouseItem[i].CODE); //해킹한 유저를 신고한다
						//SendServerFlag7 = 1; //한번만 체크하도록한다 
						sWareHouse.WareHouseItem[j].Flag = 0; //복사 아이템을 날린다 
						cWareHouse.ReFormWareHouse();
						break;
					}
				}
			}
		}
	}
	return TRUE;
}

////////믹스에서 복사된 아이템을 없애준다 
int CopyMixItemCheckDelete(sITEM* pItem)
{
	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (cInvenTory.InvenItem[i].Flag) {
			if (CompareItems(&cInvenTory.InvenItem[i].sItemInfo, &pItem->sItemInfo)) {
				cInvenTory.InvenItem[i].Flag = 0; //복사된 아이템을 지운다 
			}
		}
		if (cInvenTory.InvenItemTemp[i].Flag) {
			if (CompareItems(&cInvenTory.InvenItemTemp[i].sItemInfo, &pItem->sItemInfo)) {
				cInvenTory.InvenItemTemp[i].Flag = 0; //복사된 아이템을 지운다 
			}
		}
	}
	return TRUE;
}

//Make할 아이템이 맞는가 코드를 확인한다
int sinMakeItemCheck()
{
	int ResultCount = 0;
	int ResultCountCheck = 0;
	DWORD TempCODE[15] = { 0, };
	int  i = 0;
	//코드 복사 
	for (i = 0; i < MAX_MAKEITEM_INFO; i++) {
		for (int p = 0; p < 15; p++) {  //코드를 한번씩 초기화해준다 
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
/*							PostBox 시스템
/*****************************************************************************/
int sinPosBoxNpc() { return TRUE; }
//haGoon아이템 재구성을 체크한다.----------------------------------------------------------------------------
int cCRAFTITEM::haCheckReconItem()
{
	int cnt[MAX_SEEL_COUNT] = { 0 }; // 박재원 - 테이와즈 씰 추가 (3종류 -> 4종류)
	int CheckNotReconItem = 0;
	int CheckBlockCount = 0;
	cCraftItem.iReconItemFlag = 0;

	//씰아이템이 아닌것은 다 체크한다.
	for (int k = 1; k < 14; k++) { // 박재원 - 리스펙 스톤(sCraftItem.CraftItem[14]는 리스펙 스톤이므로 체크에서 제외한다) for(int k=1;k<15;k++) -> for(int k=1;k<14;k++)
		if (sCraftItem.CraftItem[k].Flag) {
			if ((sCraftItem.CraftItem[k].CODE & sinITEM_MASK2) == sinSE1)
				CheckBlockCount++;   //씰 아이템 체크
			else
				CheckNotReconItem++; //씰외아이템 체크
		}
		else
			memset(&sCraftItem.CraftItem[k], 0, sizeof(sITEM));  //서버 해킹 에러 때문에 임시 방편이다.
	}
	//씰을 체크한다.
	for (int i = 1; i < 14; i++) { // 박재원 - 리스펙 스톤(sCraftItem.CraftItem[14]는 리스펙 스톤이므로 체크에서 제외한다) for(int k=1;k<15;k++) -> for(int k=1;k<14;k++)
		if ((sCraftItem.CraftItem[i].CODE & sinITEM_MASK2) == sinSE1 && sCraftItem.CraftItem[i].Flag) {
			for (int k = 0; k < MAX_SEEL_COUNT; k++) { // 박재원 - 테이와즈 씰 추가 (3종류 -> 4종류)
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
*Desc: 빈배열을 당겨준다.
*-----------------------------------------------------------------------------*/
int cCRAFTITEM::haCraftSortItem(sCRAFTITEM* pCraftItem)
{
	//빈 아이템 배열을 당겨준다.
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
*Desc: 인벤토리의 22*22크기의 빈칸을 구해 리턴한다.
*-----------------------------------------------------------------------------*/
int cCRAFTITEM::GetCraftCheckEmptyArea()
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;
	int StartX = 23;  //인벤 박스의 시작점 X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - 22; //아이템이 들어갈수있는 가장 끝 X좌표 
	cy = (22 * INVENTORY_BOX_COL) - 22; //아이템이 들어갈수있는 가장 끝 Y좌표

	int count = 0;
	//코드가 같지 않거나 아이템이 비어있지 않은 곳에 셋팅할때 검색해서 위치를 찾아낸다 
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

// 석지용 - 믹스쳐 리셋 아이템이 들어갈 공간을 체크하는 함수 구현
int cMIXTURERESET::SetMixtureItemResetAreaCheck(sITEM* pItem)
{
	TradeColorIndex = 0;			// 초기화
	TradeCrashItemIndex[0] = 0;		// 충돌된 아이템 Index 초기화 
	cTrade.InitTradeColorRect();	// 영역 초기화

	if ((pItem->CODE & sinITEM_MASK2) == sinBI1)		// 빌링 아이템이고...
	{
		// 믹스쳐 리셋 스톤이 아니면 리턴
		if ((pItem->sItemInfo.CODE & sinITEM_MASK3) != sin89)
			return FALSE;

		// 스톤이 위치할 좌표를 계산해 낸다.
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
					//인벤토리 박스에 들어갔는지를 체크 
					TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
					TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
					TradeColorRect.right = pItem->w;
					TradeColorRect.bottom = pItem->h;

					if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11)
					{
						cTrade.InitTradeColorRect(); //컬러 박스 RECT 초기화 (컬러 박스 RECT중 하나라두 0 이면 셋팅할수 없다 
						return FALSE;
					}

					// 아이템의 위치를 지정한다.
					pItem->SetX = TradeColorRect.left;
					pItem->SetY = TradeColorRect.top;
					pItem->ItemPosition = 0; //배열의 인덱스를 저장한다

					TradeColorIndex = SET_ITEM_CHECK_COLOR;		//셋팅할영역

					// 겹치는 아이템이 있나 체크 
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
			TradeColorIndex = 0;				//초기화   (트레이드 창과 창고가 같이 쓴다)
			TradeCrashItemIndex[0] = 0;		//충돌된 아이템 Index 초기화 
			cTrade.InitTradeColorRect();	//초기화

			// 믹스쳐가 수행된 아이템이 들어갈 자리의 좌표를 구함
			TradeStartX = 58;
			TradeStartY = 193 + sinInterHeight2;
			TradeEndX = TradeStartX + (3 * 22);
			TradeEndY = TradeStartY + (4 * 22);

			//인벤토리 박스에 들어갔는지를 체크 
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

// 석지용 - 믹스쳐 리셋 아이템의 정보 체크
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

// 석지용 - 믹스쳐 리셋 아이템의 정보 다시 체크
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

// 석지용 - 믹스쳐 리셋 아이템을 집는 함수
int cMIXTURERESET::PickUpMixtureResetItem(int x, int y, int PickUpFlag, int Kind)
{
	int i = 0;
	SelectTradeItemIndex = 0;    //집을 아이템 

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
							MixtureResetCheckFlag = 0;	// 조합 버튼 비활성화

						// 스톤이 있는 상태에선 믹스쳐 된 아이템 들기 불가
						if (sMixtureResetItem.MixtureResetItem[1].Flag && i == 0)
						{

							return FALSE;
						}

						CheckMixtureResetItemForm();
						memcpy(&MouseItem, &sMixtureResetItem.MixtureResetItem[i], sizeof(sITEM));
						sMixtureResetItem.MixtureResetItem[i].Flag = 0;
						memset(&sMixtureResetItem.MixtureResetItem[i], 0, sizeof(sITEM));  //서버 해킹 에러 때문에 임시 방편이다.

						//sinPlaySound(sMixtureResetItem.MixtureResetItem[i].SoundIndex);
						TradeColorIndex = 0;
						ReformMixtureResetItem();

						return TRUE;
					}
					else
					{
						SelectTradeItemIndex = i + 1;
						TradeColorIndex = SELECT_ITEM_COLOR;
						sinShowItemInfoFlag = 1; //아이템의 정보를 보여준다 
						cItem.ShowItemInfo(&sMixtureResetItem.MixtureResetItem[i], 2);
					}
				}
			}
		}
	}

	return FALSE;
}

// 석지용 - 믹스쳐 리셋 아이템을 창에 위치시키도록 하는 함수
int cMIXTURERESET::LastSetMixtureResetItem(sITEM* pItem, int kind)
{
	//CheckMixtureResetItemForm();

	if (kind == 1)
	{
		// 믹스쳐된 아이템은 올려져 있고, 스톤은 놓여있지 않다면...
		if (sMixtureResetItem.MixtureResetItem[0].Flag && !sMixtureResetItem.MixtureResetItem[1].Flag)
		{
			memcpy(&sMixtureResetItem.MixtureResetItem[1], pItem, sizeof(sITEM));
			sMixtureResetItem.MixtureResetItem[1].x = pItem->SetX;
			sMixtureResetItem.MixtureResetItem[1].y = pItem->SetY;
			pItem->Flag = 0; //초기화 
			//sinPlaySound(sMixtureResetItem.MixtureResetItem[1].SoundIndex);
			//ReformMixtureResetItem();
			return TRUE;
		}
	}
	else if (kind == 2)		// 믹스쳐된 아이템이면...
	{
		// 믹스쳐된 아이템이 올려져 있지 않다면...
		if (!sMixtureResetItem.MixtureResetItem[0].Flag)
		{
			memcpy(&sMixtureResetItem.MixtureResetItem[0], pItem, sizeof(sITEM));
			sMixtureResetItem.MixtureResetItem[0].x = pItem->SetX;
			sMixtureResetItem.MixtureResetItem[0].y = pItem->SetY;
			pItem->Flag = 0; //초기화 
			//sinPlaySound(sMixtureResetItem.MixtureResetItem[0].SoundIndex);
			//ReformMixtureResetItem();
			return TRUE;
		}
	}

	return TRUE;
}

// 석지용 - 믹스쳐 리셋을 취소한다.
int cMIXTURERESET::CancelMixtureItemReset(int Flag)
{
	// 복사 방지 플랙이 활성화 되어 있다면....
	if (MixItemNoCopyFlag)
		return FALSE;

	// 믹스쳐 리셋에 올렸던 아이템이 있다면 인벤의 빈 공간으로 자동 셋팅하고, 지운다.
	if (sMixtureResetItem.MixtureResetItem[0].Flag)
	{
		if (!cInvenTory.AutoSetInvenItem(&sMixtureResetItem.MixtureResetItem[0]))
			sinThrowItemToFeild(&sMixtureResetItem.MixtureResetItem[0]);

		sMixtureResetItem.MixtureResetItem[0].Flag = 0;
	}

	MixtureResetCheckFlag = 0;	// 믹스쳐 리셋 불가능

	// 믹스쳐 리셋 창을 닫는다.
	if (!Flag)
		cMixtureReset.OpenFlag = 0;

	ResetInvenItemCode();	// 인벤 아이템 코드 초기화

	return TRUE;
}

// 석지용 - 믹스쳐 리셋될 아이템을 서버로 전송한다.
int cMIXTURERESET::sinSendMixtureItemReset()
{
	if (MixItemNoCopyFlag)
		return FALSE;

	memcpy(&sMixtureResetItemBackUp, &sMixtureResetItem, sizeof(sMIXTURE_RESET_ITEM));	// 복사를 체크하기위한 백업본을 만든다
	MixItemNoCopyFlag = 1;	//아이템이 복사되는걸 방지하기위해 플렉을준다
	memset(&sMixtureReset_Send, 0, sizeof(sMIXTURE_RESET_ITEM_SERVER));	// 서버에 보낼 구조체 초기화

	// 믹스쳐 리셋 시킬 아이템을 저장
	if (sMixtureResetItem.MixtureResetItem[0].Flag)
		sMixtureReset_Send.DesMixtureResetItem = sMixtureResetItem.MixtureResetItem[0];

	// 믹스쳐 리셋 스톤을 저장
	if (sMixtureResetItem.MixtureResetItem[1].Flag)
	{
		sMixtureReset_Send.dwMixtureResetStoneItemCode = sMixtureResetItem.MixtureResetItem[1].CODE;
		sMixtureReset_Send.dwMixtureResetStoneCheckSum = sMixtureResetItem.MixtureResetItem[1].sItemInfo.ItemHeader.dwChkSum;
		sMixtureReset_Send.dwMixtureResetStoneHead = sMixtureResetItem.MixtureResetItem[1].sItemInfo.ItemHeader.Head;
	}

	// 인덱스를 보낸다 
	CraftItemSendServerIndex++;
	sMixtureReset_Send.Index = CraftItemSendServerIndex;

	// 저장된 구조체를 서버에 보낸다 
	SendMixtureResetItemToServer(&sMixtureReset_Send);


	return TRUE;
}

// 석지용 - 믹스쳐 리셋된 아이템을 Recv용 변수로 복사
int	cMIXTURERESET::sinRecvMixtureItemReset(sMIXTURE_RESET_ITEM_SERVER* pMixtureResetItem)
{
	memcpy(&sMixtureReset_Recv, pMixtureResetItem, sizeof(sMIXTURE_RESET_ITEM_SERVER));

	return TRUE;
}

// 석지용 - 믹스쳐 리셋 아이템 최종 결과 처리
int cMIXTURERESET::sinRecvMixtureItemResetResult(sMIXTURE_RESET_ITEM_SERVER* pMixtureResetItem_Server)
{
	CheckMixItemCopyFlag = 0; //복사방지 플렉  

	// 믹스쳐리셋 창이 열려있지 않다면 모두 초기화
	if (!cMixtureReset.OpenFlag)
	{
		MixItemNoCopyFlag = 0; //복사방지 플렉 초기화
		memset(&sMixtureResetItem, 0, sizeof(sMIXTURE_RESET_ITEM));			//구조체 초기화 
		memset(&sMixtureResetItemBackUp, 0, sizeof(sMIXTURE_RESET_ITEM));	//구조체 초기화 
		memset(&sMixtureReset_Recv, 0, sizeof(sMIXTURE_RESET_ITEM_SERVER)); //받은 믹스쳐리셋 구조체 초기화 
		memset(&sMixtureReset_Send, 0, sizeof(sMIXTURE_RESET_ITEM_SERVER)); //보낸 믹스쳐리셋 구조체 초기화 
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

			CraftItemMessageSize.x = 14;				//박스 사이즈 
			CraftItemMessageSize.y = ((LineCount) * 2) + 2; //박스 사이즈 

			if (cInvenTory.CheckRequireItemToSet(&TempItem))
			{
				//곧바로 셋팅될수있는지를 체크한다  
				if (!cInvenTory.AutoSetInvenItem(&TempItem, 1))
				{
					sinThrowItemToFeild(&pMixtureResetItem_Server->DesMixtureResetItem);
				}
			}

		}



		CheckCharForm();	//인증 
		ReformCharForm();	//재인증 
		SendSaveMoney();	//금액 조작을 못하게하기위해 호출한다 
		SaveGameData();
		cMixtureReset.OpenFlag = 0; //창을 닫는다
	}
	else
	{
		CheckCharForm();	//인증 
		ReformCharForm();	//재인증 
		SendSaveMoney();	//금액 조작을 못하게하기위해 호출한다 
		cMixtureReset.OpenFlag = 0; //창을 닫는다

	}

	//복사가되었거나한 아이템을 지운다
	for (i = 0; i < 2; i++)
	{
		if (sMixtureResetItemBackUp.MixtureResetItem[i].Flag)
		{
			cInvenTory.DeleteInvenItemToServer(sMixtureResetItemBackUp.MixtureResetItem[i].sItemInfo.CODE,
				sMixtureResetItemBackUp.MixtureResetItem[i].sItemInfo.ItemHeader.Head,
				sMixtureResetItemBackUp.MixtureResetItem[i].sItemInfo.ItemHeader.dwChkSum);
		}

	}

	memset(&sMixtureResetItem, 0, sizeof(sMIXTURE_RESET_ITEM));			//구조체 초기화 
	memset(&sMixtureResetItemBackUp, 0, sizeof(sMIXTURE_RESET_ITEM));		//구조체 초기화 
	memset(&sMixtureReset_Recv, 0, sizeof(sMIXTURE_RESET_ITEM_SERVER));	//받은 믹스쳐리셋 구조체 초기화 
	memset(&sMixtureReset_Send, 0, sizeof(sMIXTURE_RESET_ITEM_SERVER));	//보낸 믹스쳐리셋 구조체 초기화 

	MixItemNoCopyFlag = 0; //복사방지 플렉 초기화
	MixCancelButtonDelayFlag = 0;


	return TRUE;
}

// pluto 제련
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
	memset(&S_smeltingItem, 0, sizeof(SSmeltingItem)); //아이템을 다 날려버린다 
	ResetInvenItemCode();				//인벤 아이템 코드 초기화

	memset(&sSmeltingItem_Recv, 0, sizeof(sSMELTINGITEM_SERVER)); //받은 믹스쳐 구조체 초기화  (일단 같이해준다)
	memset(&sSmeltingItem_Send, 0, sizeof(sSMELTINGITEM_SERVER)); //보낸 믹스쳐 구조체 초기화 

	return TRUE;
}

// pluto 제련
int CSmeltingItem::SetSmeltingItemAreaCheck(sITEM* pItem, int Kind)
{
	//광석	  케이아스 이프리아 펜타나  바루나 도루네아 사티쿠아 메루카바
	int SmeltingPriceIndexOre = 25000;

	//수정    케이아스 이프리아 펜타나  바루나 도루네아 사티쿠아 메루카바 눈결정 스노우플라워 하얀눈물
	int SmeltingPriceIndexCrystal = 50000;
	//	int SmeltingPriceIndexCrystal = 10000; // 장별 - 일본 요청으로 임시로 만원
		// 장별 - 대장장이의 혼
	DWORD SmeltingCode[] = { sin01, sin02, sin03, sin04, sin05, sin06, sin07, sin08, sin09, sin10, sin11, sin12, sin13, sin14 };

	int i, j;
	TradeColorIndex = 0; // 초기화
	TradeCrashItemIndex[0] = 0; //충돌된 아이템 Index 초기화 
	cTrade.InitTradeColorRect(); //초기화

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
		//		{ //인벤토리 박스에 들어갔는지를 체크 
		//			TradeColorRect.left =  TradeStartX +( ( ( i - TradeStartX) /22)*22) ; 
		//			TradeColorRect.top  =  TradeStartY +( ( ( j - TradeStartY) /22)*22) ; 
		//			TradeColorRect.right  = pItem->w;
		//			TradeColorRect.bottom = pItem->h;
		//			if(TradeEndX < (TradeColorRect.left + TradeColorRect.right)-11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom)-11)
		//			{
		//				cTrade.InitTradeColorRect(); //컬러 박스 RECT 초기화 (컬러 박스 RECT중 하나라두 0 이면 셋팅할수 없다 
		//				return FALSE;
		//			}

		//			pItem->SetX = TradeColorRect.left;
		//			pItem->SetY = TradeColorRect.top;
		//			pItem->ItemPosition = 0; //배열의 인덱스를 저장한다 
		//			TradeColorIndex = SET_ITEM_CHECK_COLOR; //셋팅할영역
		//			if( !S_smeltingItem.SmeltingItem[0].Flag )
		//			{
		//				TradeColorIndex = NOT_SETTING_COLOR;
		//				return FALSE;
		//			}
		//			if(Kind == 0)
		//			{ //믹스쳐 
		//				if(TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect,0,4))
		//				{  //겹치는 아이템이 있나 체크 
		//					TradeColorIndex = NOT_SETTING_COLOR;
		//					return FALSE;

		//				}
		//				return TRUE;
		//			}
		//		}

		//	}

		//}


		//int SmeltingSheltomAray[4][3] = { 0, 0, 0,		// 쉘텀 갯수 늘어 나면 쓰자
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
		//for(int p = 0 ; p < 4 ; p++)			// 쉘텀 갯수 늘어 나면 쓰자
		{
			//for(int m=0; m < 3 ; m++)
			{
				if (SmeltingSheltomIndex == Smelting_SheltomIndex)
				{
					TradeStartX = AgingLevelSheltomXY[0][0].x + 22;			// 좌표는 같아서 걍 썻음
					TradeStartY = AgingLevelSheltomXY[0][0].y + 30;
					TradeEndX = TradeStartX + 22;
					TradeEndY = TradeStartY + 22;
					for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22)
					{
						for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22)
						{
							if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j)
							{ //인벤토리 박스에 들어갔는지를 체크 
								TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
								TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
								TradeColorRect.right = pItem->w;
								TradeColorRect.bottom = pItem->h;
								if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11)
								{
									cTrade.InitTradeColorRect(); //컬러 박스 RECT 초기화 (컬러 박스 RECT중 하나라두 0 이면 셋팅할수 없다 
									return FALSE;
								}

								pItem->SetX = TradeColorRect.left;
								pItem->SetY = TradeColorRect.top;
								pItem->ItemPosition = 0; //배열의 인덱스를 저장한다

								TradeColorIndex = SET_ITEM_CHECK_COLOR; //셋팅할영역
								if (!S_smeltingItem.SmeltingItem[0].Flag)
								{
									TradeColorIndex = NOT_SETTING_COLOR;
									return FALSE;
								}

								if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 4))
								{  //겹치는 아이템이 있나 체크 
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
		TradeColorIndex = 0; //초기화   (트레이드 창과 창고가 같이 쓴다)
		TradeCrashItemIndex[0] = 0; //충돌된 아이템 Index 초기화 
		cTrade.InitTradeColorRect(); //초기화

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

// pluto 제련
int CSmeltingItem::LastSetSmeltingItem(sITEM* pItem, int Kind)
{
	if (Kind == 0)
	{
		CheckSmeltingItemForm();
		if ((pItem->CODE & sinITEM_MASK2) == sinOS1)
		{ //쉘텀일 경우
			//for(int j=12 ; j < 24 ; j++)
			//{
			//	if(!S_smeltingItem.SmeltingItem[j].Flag)
			//	{
			//		memcpy(&S_smeltingItem.SmeltingItem[j],pItem,sizeof(sITEM));
			//		S_smeltingItem.SmeltingItem[j].x = pItem->SetX;
			//		S_smeltingItem.SmeltingItem[j].y = pItem->SetY;
			//		pItem->Flag = 0; //초기화 
			//		//sinPlaySound(S_smeltingItem.SmeltingItem[j].SoundIndex);
			//		ReFormSmeltingItem();
			//		return TRUE;
			//	}
			//}
			if (!S_smeltingItem.SmeltingItem[4].Flag)	// pluto 쉘텀 한개만 놓는다 
			{
				memcpy(&S_smeltingItem.SmeltingItem[4], pItem, sizeof(sITEM));
				S_smeltingItem.SmeltingItem[4].x = pItem->SetX;
				S_smeltingItem.SmeltingItem[4].y = pItem->SetY;
				pItem->Flag = 0; //초기화 
				//sinPlaySound(S_smeltingItem.SmeltingItem[4].SoundIndex);
				ReFormSmeltingItem();
				return TRUE;
			}

		}
		else
		{
			int k = 3; // 장별 - 제련 시스템 수정
			//if( (pItem->CODE & sinITEM_MASK2) == sinPR1 )
			//{
			//	k = 5;		// 광석
			//}
			//else if( (pItem->CODE & sinITEM_MASK2) == sinPR2 )
			//{
			//	k = 3;		// 수정
			//}

			for (int i = 0; i < k; i++)
			{
				if (!S_smeltingItem.SmeltingItem[i].Flag)		// 광석, 수정
				{
					memcpy(&S_smeltingItem.SmeltingItem[i], pItem, sizeof(sITEM));
					S_smeltingItem.SmeltingItem[i].x = pItem->SetX;
					S_smeltingItem.SmeltingItem[i].y = pItem->SetY;
					pItem->Flag = 0; //초기화 
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

// pluto 제련
int CSmeltingItem::CheckSmeltingItemForm()
{ //체크  
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

// pluto 제련
int CSmeltingItem::ReFormSmeltingItem()
{ //인증 

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

// pluto 제련 아이템 집기
int CSmeltingItem::PickUpSmeltingItem(int x, int y, int PickUpFlag, int Kind)
{
	int i = 0;
	SelectTradeItemIndex = 0;    //집을 아이템 
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
						memset(&S_smeltingItem.SmeltingItem[i], 0, sizeof(sITEM));  //서버 해킹 에러 때문에 임시 방편이다.

						//sinPlaySound(S_smeltingItem.SmeltingItem[i].SoundIndex);
						TradeColorIndex = 0;
						ReFormSmeltingItem();
						return TRUE;
					}
					else
					{
						SelectTradeItemIndex = i + 1;
						TradeColorIndex = SELECT_ITEM_COLOR;
						sinShowItemInfoFlag = 1; //아이템의 정보를 보여준다 
						cItem.ShowItemInfo(&S_smeltingItem.SmeltingItem[i], 2);
					}
				}
			}
		}
	}
	return FALSE;
}

// pluto 제련
int CSmeltingItem::sinSendSmeltingItem()
{
	if (MixItemNoCopyFlag)
	{
		return FALSE;
	}

	int i = 0;

	//복사를 체크하기위한 백업본을 만든다
	memcpy(&sSmeltingItemBackUp, &S_smeltingItem, sizeof(SSmeltingItem));
	MixItemNoCopyFlag = 1; //아이템이 복사되는걸 방지하기위해 플렉을준다
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

	CraftItemSendServerIndex++; //인덱스를 보낸다 
	sSmeltingItem_Send.Index = CraftItemSendServerIndex;
	//저장된 구조체를 서버에 보낸다 


	SendSmeltingItemToServer(&sSmeltingItem_Send);

	return TRUE;
}

int CSmeltingItem::sinRecvSmeltingItem(sSMELTINGITEM_SERVER* pSmeltingItem_Server)
{
	memcpy(&sSmeltingItem_Recv, pSmeltingItem_Server, sizeof(sSMELTINGITEM_SERVER));

	return TRUE;
}

// pluto 제련 아이템의 결과물을 서버에서 받는다 
int CSmeltingItem::sinRecvSmeltingItemResult(sSMELTINGITEM_SERVER* pSmeltingItem_Server)
{
	CheckMixItemCopyFlag = 0; //복사방지 플렉  
	if (!SmeltingItem.OpenFlag)
	{
		MixItemNoCopyFlag = 0; //복사방지 플렉 초기화
		memset(&S_smeltingItem, 0, sizeof(SSmeltingItem)); //구조체 초기화 
		memset(&sSmeltingItemBackUp, 0, sizeof(SSmeltingItem)); //구조체 초기화 
		memset(&sSmeltingItem_Recv, 0, sizeof(sSMELTINGITEM_SERVER)); //받은 제련 구조체 초기화 
		memset(&sSmeltingItem_Send, 0, sizeof(sSMELTINGITEM_SERVER)); //보낸 제련 구조체 초기화 
		MixCancelButtonDelayFlag = 0;
		return FALSE;

	}

	int i, LineCount = 0, Temp;


	if (pSmeltingItem_Server->Result)
	{
		ShowItemSmeltingMessageFlag = 1; // 아이템이 제련됬을때 메세지를 보여준다

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
			CraftItemMessageSize.x = 14;           //박스 사이즈 
			CraftItemMessageSize.y = ((LineCount) * 2) + 2;  //박스 사이즈 

			if (cInvenTory.CheckRequireItemToSet(&TempItem))
			{
				//곧바로 셋팅될수있는지를 체크한다  
				if (!cInvenTory.AutoSetInvenItem(&TempItem, 1))
				{
					sinThrowItemToFeild(&pSmeltingItem_Server->DesSmeltingItem);
				}
			}

		}

		CheckCharForm(); //인증 
		sinMinusMoney(m_SmeltingPrice);
		ReformCharForm(); //재인증 
		SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 
		SaveGameData();
		SmeltingItem.OpenFlag = 0; //창을 닫는다
	}
	else
	{
		CheckCharForm(); //인증 
		sinMinusMoney(m_SmeltingPrice);
		ReformCharForm(); //재인증 
		SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 
		SmeltingItem.OpenFlag = 0; //창을 닫는다 

	}

	//복사가되었거나한 아이템을 지운다
	for (i = 0; i < 5; i++)
	{
		if (sSmeltingItemBackUp.SmeltingItem[i].Flag)
		{
			cInvenTory.DeleteInvenItemToServer(sSmeltingItemBackUp.SmeltingItem[i].sItemInfo.CODE,
				sSmeltingItemBackUp.SmeltingItem[i].sItemInfo.ItemHeader.Head,
				sSmeltingItemBackUp.SmeltingItem[i].sItemInfo.ItemHeader.dwChkSum);
		}

	}
	memset(&S_smeltingItem, 0, sizeof(SSmeltingItem)); //구조체 초기화 
	memset(&sSmeltingItemBackUp, 0, sizeof(SSmeltingItem)); //구조체 초기화 
	memset(&sSmeltingItem_Recv, 0, sizeof(sSMELTINGITEM_SERVER)); //받은 제련 구조체 초기화 
	memset(&sSmeltingItem_Send, 0, sizeof(sSMELTINGITEM_SERVER)); //보낸 제련 구조체 초기화 

	MixItemNoCopyFlag = 0; //복사방지 플렉 초기화
	MixCancelButtonDelayFlag = 0;
	m_SmeltingPrice = 0;
	ForceItemPrice2 = 0;
	return TRUE;
}

// pluto 제련
int CSmeltingItem::SmeltingCheckEmptyArea(sITEM* pItem)
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;
	int StartX = 23;  //인벤 박스의 시작점 X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - pItem->w; //아이템이 들어갈수있는 가장 끝 X좌표 
	cy = (22 * INVENTORY_BOX_COL) - pItem->h; //아이템이 들어갈수있는 가장 끝 Y좌표

	//코드가 같지 않거나 아이템이 비어있지 않은 곳에 셋팅할때 검색해서 위치를 찾아낸다 
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
			if (flag == 1) { // 장별 - 제련 시스템 수정
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
	int StartX = 23;  //인벤 박스의 시작점 X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - 22; //아이템이 들어갈수있는 가장 끝 X좌표 
	cy = (22 * INVENTORY_BOX_COL) - 22; //아이템이 들어갈수있는 가장 끝 Y좌표

	int count = 0;
	//코드가 같지 않거나 아이템이 비어있지 않은 곳에 셋팅할때 검색해서 위치를 찾아낸다 
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
			if (flag == 1) { // 장별 - 제련 시스템 수정
				count++;
			}
		}
	}
	if (count)
		return count;
	else
		return 0;
}

// pluto 광석, 수정 제련 비용
int CSmeltingItem::CheckSmeltingPrice()
{						//광석	  케이아스 이프리아 펜타나  바루나 도루네아 사티쿠아 메루카바
	//int SmeltingPriceIndexOre[] = { 100000, 200000, 300000, 400000, 500000, 600000, 700000 };
	int SmeltingPriceIndexOre = 25000;

	//수정    케이아스 이프리아 펜타나  바루나 도루네아 사티쿠아 메루카바
//int SmeltingPriceIndexCrystal[] = { 200000, 400000, 600000, 800000, 1000000, 1200000, 1400000 };
	int SmeltingPriceIndexCrystal = 50000;

	//	int SmeltingPriceIndexCrystal = 10000; // 장별 - 일본 요청으로 임시로 만원

		//DWORD SmeltingCode[] = { sin01, sin02, sin03, sin04, sin05, sin06, sin07, sin08 };
	m_SmeltingPrice = 0;
	if (S_smeltingItem.SmeltingItem[0].Flag)
	{
		if ((S_smeltingItem.SmeltingItem[0].sItemInfo.CODE & sinITEM_MASK2) == sinPR1) // 광석
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
		else if ((S_smeltingItem.SmeltingItem[0].sItemInfo.CODE & sinITEM_MASK2) == sinPR2) // 수정
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

// pluto 제작 취소
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
	memset(&g_sManufactureItem, 0, sizeof(SManufactureItem)); //아이템을 다 날려버린다 
	ResetInvenItemCode();				//인벤 아이템 코드 초기화

	memset(&g_sManufactureItem_Recv, 0, sizeof(SManufactureItem_Server)); //받은 제작 구조체 초기화
	memset(&g_sManufactureItem_Send, 0, sizeof(SManufactureItem_Server)); //보낸 제작 구조체 초기화 

	return TRUE;
}

// pluto 제작 아이템 들어갈 자리 검사
int CManufacture::SetManufactureItemAreaCheck(sITEM* pItem)
{
	////광석	  케이아스 이프리아 펜타나  바루나 도루네아 사티쿠아 메루카바
	//int SmeltingPriceIndexOre[] = { 100000, 200000, 300000, 400000, 500000, 600000, 700000 };

	////수정    케이아스 이프리아 펜타나  바루나 도루네아 사티쿠아 메루카바
	//int SmeltingPriceIndexCrystal[] = { 200000, 400000, 600000, 800000, 1000000, 1200000, 1400000 };
	//DWORD SmeltingCode[] = { sin01, sin02, sin03, sin04, sin05, sin06, sin07 };

	POINT ManufactureRuneXY[3] = { {240, 159},  {215, 190}, {265, 190} };

	int i, j;
	TradeColorIndex = 0; // 초기화
	TradeCrashItemIndex[0] = 0; //충돌된 아이템 Index 초기화 
	cTrade.InitTradeColorRect(); //초기화

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
		//		{ //인벤토리 박스에 들어갔는지를 체크 
		//			TradeColorRect.left =  TradeStartX +( ( ( i - TradeStartX) /22)*22) ; 
		//			TradeColorRect.top  =  TradeStartY +( ( ( j - TradeStartY) /22)*22) ; 
		//			TradeColorRect.right  = pItem->w;
		//			TradeColorRect.bottom = pItem->h;
		//			if(TradeEndX < (TradeColorRect.left + TradeColorRect.right)-11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom)-11)
		//			{
		//				cTrade.InitTradeColorRect(); //컬러 박스 RECT 초기화 (컬러 박스 RECT중 하나라두 0 이면 셋팅할수 없다 
		//				return FALSE;
		//			}

		//			pItem->SetX = TradeColorRect.left;
		//			pItem->SetY = TradeColorRect.top;
		//			pItem->ItemPosition = 0; //배열의 인덱스를 저장한다 
		//			TradeColorIndex = SET_ITEM_CHECK_COLOR; //셋팅할영역

		//			if(TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect,0,5))
		//			{  //겹치는 아이템이 있나 체크 
		//				TradeColorIndex = NOT_SETTING_COLOR;
		//				return FALSE;
		//			}
		//			return TRUE;
		//		}

		//	}

		//}

		// 조합 가능한 룬만 놓는다 // 장별 - 룬 추가 11까지 // 장별 - 대장장이의 혼 11->14
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
							TradeStartX = ManufactureRuneXY[p].x;			// 좌표는 같아서 걍 썻음
							TradeStartY = ManufactureRuneXY[p].y;
							TradeEndX = TradeStartX + 22;
							TradeEndY = TradeStartY + 22;
							for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22)
							{
								for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22)
								{
									if (TradeStartX <= i && TradeEndX > i && TradeStartY <= j && TradeEndY > j)
									{ //인벤토리 박스에 들어갔는지를 체크 
										TradeColorRect.left = TradeStartX + (((i - TradeStartX) / 22) * 22);
										TradeColorRect.top = TradeStartY + (((j - TradeStartY) / 22) * 22);
										TradeColorRect.right = pItem->w;
										TradeColorRect.bottom = pItem->h;
										if (TradeEndX < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY < (TradeColorRect.top + TradeColorRect.bottom) - 11)
										{
											cTrade.InitTradeColorRect(); //컬러 박스 RECT 초기화 (컬러 박스 RECT중 하나라두 0 이면 셋팅할수 없다 
											return FALSE;
										}

										pItem->SetX = TradeColorRect.left;
										pItem->SetY = TradeColorRect.top;
										pItem->ItemPosition = 0; //배열의 인덱스를 저장한다

										TradeColorIndex = SET_ITEM_CHECK_COLOR; //셋팅할영역

										if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 5))
										{  //겹치는 아이템이 있나 체크 
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
		TradeColorIndex = 0; //초기화   (트레이드 창과 창고가 같이 쓴다)
		TradeCrashItemIndex[0] = 0; //충돌된 아이템 Index 초기화 
		cTrade.InitTradeColorRect(); //초기화

		TradeStartX = 58;
		TradeStartY = 193 + sinInterHeight2;
		TradeEndX = TradeStartX + (3 * 22);
		TradeEndY = TradeStartY + (4 * 22);
	}
	return TRUE;
}

// pluto 제작 아이템 놓는다
int CManufacture::LastSetManufactureItem(sITEM* pItem)
{
	CheckManufactureItem();
	if ((pItem->CODE & sinITEM_MASK2) == sinPR3 || (pItem->CODE & sinITEM_MASK2) == sinPR4)
	{
		// 룬일 경우
		for (int j = 1; j < 4; j++)
		{
			if (!g_sManufactureItem.ManufactureItem[j].Flag)
			{
				memcpy(&g_sManufactureItem.ManufactureItem[j], pItem, sizeof(sITEM));
				g_sManufactureItem.ManufactureItem[j].x = pItem->SetX;
				g_sManufactureItem.ManufactureItem[j].y = pItem->SetY;
				pItem->Flag = 0; //초기화 
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
			pItem->Flag = 0; //초기화 
			//sinPlaySound(g_sManufactureItem.ManufactureItem[0].SoundIndex);
			ReFormManufactureItem();
			return TRUE;
		}
	}
	return TRUE;

}

// pluto 제작
int CManufacture::CheckManufactureItem()
{ //체크  
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

// pluto 제작
int CManufacture::ReFormManufactureItem()
{
	//인증 
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
	SelectTradeItemIndex = 0;    //집을 아이템 

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
					memset(&g_sManufactureItem.ManufactureItem[i], 0, sizeof(sITEM));  //서버 해킹 에러 때문에 임시 방편이다.

					//sinPlaySound(g_sManufactureItem.ManufactureItem[i].SoundIndex);
					TradeColorIndex = 0;
					ReFormManufactureItem();
					return TRUE;
				}
				else
				{
					SelectTradeItemIndex = i + 1;
					TradeColorIndex = SELECT_ITEM_COLOR;
					sinShowItemInfoFlag = 1; //아이템의 정보를 보여준다 
					cItem.ShowItemInfo(&g_sManufactureItem.ManufactureItem[i], 2);
				}
			}
		}
	}
	return FALSE;
}


// pluto 제작
int CManufacture::ManufactureCheckEmptyArea(sITEM* pItem)
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;
	int StartX = 23;  //인벤 박스의 시작점 X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - pItem->w; //아이템이 들어갈수있는 가장 끝 X좌표 
	cy = (22 * INVENTORY_BOX_COL) - pItem->h; //아이템이 들어갈수있는 가장 끝 Y좌표

	//코드가 같지 않거나 아이템이 비어있지 않은 곳에 셋팅할때 검색해서 위치를 찾아낸다 
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

	//복사를 체크하기위한 백업본을 만든다
	memcpy(&g_sManufactureItemBackUp, &g_sManufactureItem, sizeof(SManufactureItem));
	MixItemNoCopyFlag = 1; //아이템이 복사되는걸 방지하기위해 플렉을준다
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

	CraftItemSendServerIndex++; //인덱스를 보낸다 
	g_sManufactureItem_Send.Index = CraftItemSendServerIndex;

	//저장된 구조체를 서버에 보낸다 
	SendManufactureItemToServer(&g_sManufactureItem_Send);

	return TRUE;
}

// pluto 제작
int CManufacture::sinRecvManufactureItem(SManufactureItem_Server* pManufactureItem_Server)
{
	memcpy(&g_sManufactureItem_Recv, pManufactureItem_Server, sizeof(SManufactureItem_Server));
	return TRUE;
}

// pluto 제작 // 장별 - 제작
void CManufacture::RuneIndexInit()
{
	// pluto 제작 할때 필요한 룬 보여주는거
	for (int i = 0; i < MAX_RECIPE_KIND; i++)
	{
		memset(&g_sManufacture_Rune[i], 0, sizeof(SManufacture_Rune));
	}

	g_sManufacture_Rune[0].RecipeCode = EXTREME_RECIPE; // 잊혀진 레시피
	g_sManufacture_Rune[0].NeedRuneIndex[0] = 1;
	g_sManufacture_Rune[0].NeedRuneIndex[1] = 2;
	g_sManufacture_Rune[0].NeedRuneIndex[2] = 3;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[1].RecipeCode = ANCIENT_RECIPE; // 고대의 레시피
	g_sManufacture_Rune[1].NeedRuneIndex[0] = 2;
	g_sManufacture_Rune[1].NeedRuneIndex[1] = 3;
	g_sManufacture_Rune[1].NeedRuneIndex[2] = 4;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[2].RecipeCode = MINOTAUR_RECIPE; // 대지의 레시피
	g_sManufacture_Rune[2].NeedRuneIndex[0] = 3;
	g_sManufacture_Rune[2].NeedRuneIndex[1] = 4;
	g_sManufacture_Rune[2].NeedRuneIndex[2] = 5;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[3].RecipeCode = DOOM_RECIPE; // 어둠의 레시피
	g_sManufacture_Rune[3].NeedRuneIndex[0] = 3;
	g_sManufacture_Rune[3].NeedRuneIndex[1] = 4;
	g_sManufacture_Rune[3].NeedRuneIndex[2] = 5;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[4].RecipeCode = SALAMANDER_RECIPE; // 화염의 레시피
	g_sManufacture_Rune[4].NeedRuneIndex[0] = 3;
	g_sManufacture_Rune[4].NeedRuneIndex[1] = 4;
	g_sManufacture_Rune[4].NeedRuneIndex[2] = 5;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[5].RecipeCode = WYVERN_RECIPE; // 바람의 레시피
	g_sManufacture_Rune[5].NeedRuneIndex[0] = 3;
	g_sManufacture_Rune[5].NeedRuneIndex[1] = 4;
	g_sManufacture_Rune[5].NeedRuneIndex[2] = 5;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[6].RecipeCode = PHOENIX_RECIPE; // 태양의 레시피
	g_sManufacture_Rune[6].NeedRuneIndex[0] = 4;
	g_sManufacture_Rune[6].NeedRuneIndex[1] = 5;
	g_sManufacture_Rune[6].NeedRuneIndex[2] = 6;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[7].RecipeCode = FRENZY_RECIPE; // 광포한 레시피
	g_sManufacture_Rune[7].NeedRuneIndex[0] = 5;
	g_sManufacture_Rune[7].NeedRuneIndex[1] = 6;
	g_sManufacture_Rune[7].NeedRuneIndex[2] = 7;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[8].RecipeCode = HEAVENS_RECIPE; // 천상의 레시피
	g_sManufacture_Rune[8].NeedRuneIndex[0] = 6;
	g_sManufacture_Rune[8].NeedRuneIndex[1] = 7;
	g_sManufacture_Rune[8].NeedRuneIndex[2] = 8;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[9].RecipeCode = SNOW_RECIPE; // 포설의 레시피 // 장별 - 제작
	g_sManufacture_Rune[9].NeedRuneIndex[0] = 9;
	g_sManufacture_Rune[9].NeedRuneIndex[1] = 10;
	g_sManufacture_Rune[9].NeedRuneIndex[2] = 11;

	////////////////////////////////////////////////////

	g_sManufacture_Rune[10].RecipeCode = MEMORY_RECIPE; // 기억의 레시피 // 장별 - 대장장이의 혼
	g_sManufacture_Rune[10].NeedRuneIndex[0] = 12;
	g_sManufacture_Rune[10].NeedRuneIndex[1] = 13;
	g_sManufacture_Rune[10].NeedRuneIndex[2] = 14;


}

// pluto 제작 레시피 인덱스다 필요한 룬 그림 셋팅 할라구 // 장별 - 제작 // 장별 - 대장장이의 혼
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
		RecipeIndex = SNOW_RECIPE; // 장별 - 제작 : 포설의 제작서
		break;

	case sin11:
		RecipeIndex = MEMORY_RECIPE; //  기억의 제작서	// 장별 - 대장장이의 혼
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

// pluto 제작 결과
int CManufacture::RecvManufactureItemResult(SManufactureItem_Server* pManufactureItem_Server)
{
	CheckMixItemCopyFlag = 0; //복사방지 플렉  
	if (!ManufactureItem.m_OpenFlag)
	{
		MixItemNoCopyFlag = 0; //복사방지 플렉 초기화
		memset(&g_sManufactureItem, 0, sizeof(SManufactureItem)); //구조체 초기화 
		memset(&g_sManufactureItemBackUp, 0, sizeof(SManufactureItem)); //구조체 초기화 
		memset(&g_sManufactureItem_Recv, 0, sizeof(SManufactureItem_Server)); //받은 제작 구조체 초기화 
		memset(&g_sManufactureItem_Send, 0, sizeof(SManufactureItem_Server)); //보낸 제작 구조체 초기화 
		MixCancelButtonDelayFlag = 0;
		return FALSE;

	}

	int i, LineCount = 0, Temp;


	if (pManufactureItem_Server->Result)
	{
		ShowItemManufactureMessageFlag = 1; // 아이템이 제작됬을때 메세지를 보여준다

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
			CraftItemMessageSize.x = 14;           //박스 사이즈 
			CraftItemMessageSize.y = ((LineCount) * 2) + 2;  //박스 사이즈 

			if (cInvenTory.CheckRequireItemToSet(&TempItem))
			{
				//곧바로 셋팅될수있는지를 체크한다  
				if (!cInvenTory.AutoSetInvenItem(&TempItem, 1))
				{
					sinThrowItemToFeild(&pManufactureItem_Server->DesManufactureItem);
				}
			}

		}

		CheckCharForm(); //인증 
		sinMinusMoney(m_ManufacturePrice);
		ReformCharForm(); //재인증 
		SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 
		SaveGameData();
		ManufactureItem.m_OpenFlag = 0; //창을 닫는다
	}
	else
	{
		CheckCharForm(); //인증 
		sinMinusMoney(m_ManufacturePrice);
		ReformCharForm(); //재인증 
		SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 
		ManufactureItem.m_OpenFlag = 0; //창을 닫는다 

	}

	//복사가되었거나한 아이템을 지운다
	for (i = 0; i < 4; i++)
	{
		if (g_sManufactureItemBackUp.ManufactureItem[i].Flag)
		{
			cInvenTory.DeleteInvenItemToServer(g_sManufactureItemBackUp.ManufactureItem[i].sItemInfo.CODE,
				g_sManufactureItemBackUp.ManufactureItem[i].sItemInfo.ItemHeader.Head,
				g_sManufactureItemBackUp.ManufactureItem[i].sItemInfo.ItemHeader.dwChkSum);
		}

	}
	memset(&g_sManufactureItem, 0, sizeof(SManufactureItem)); //구조체 초기화 
	memset(&g_sManufactureItemBackUp, 0, sizeof(SManufactureItem)); //구조체 초기화 
	memset(&g_sManufactureItem_Recv, 0, sizeof(SManufactureItem_Server)); //받은 제작 구조체 초기화 
	memset(&g_sManufactureItem_Send, 0, sizeof(SManufactureItem_Server)); //보낸 제작 구조체 초기화 

	MixItemNoCopyFlag = 0; //복사방지 플렉 초기화
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
	int StartX = 23;  //인벤 박스의 시작점 X
	int StartY = 436; // Y

	cx = (22 * INVENTORY_BOX_ROW) - 22; //아이템이 들어갈수있는 가장 끝 X좌표 
	cy = (22 * INVENTORY_BOX_COL) - 22; //아이템이 들어갈수있는 가장 끝 Y좌표

	int count = 0;
	//코드가 같지 않거나 아이템이 비어있지 않은 곳에 셋팅할때 검색해서 위치를 찾아낸다 
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

// pluto 제련 회색 이미지 쉘텀 // 장별 - 제련 // 장별 - 대장장이의 혼
int CSmeltingItem::GetSheltomIndex(DWORD SmeltingItemCode)
{
	DWORD SmeltingItemAray[] = { (sinPR1 | sin01), (sinPR1 | sin02), (sinPR1 | sin03), (sinPR1 | sin04), (sinPR1 | sin05), (sinPR1 | sin06), (sinPR1 | sin07), (sinPR1 | sin08),
								 (sinPR2 | sin01), (sinPR2 | sin02), (sinPR2 | sin03), (sinPR2 | sin04), (sinPR2 | sin05), (sinPR2 | sin06), (sinPR2 | sin07), (sinPR2 | sin08),
								 (sinPR2 | sin09), (sinPR2 | sin10), (sinPR2 | sin11), (sinPR2 | sin12), (sinPR2 | sin13), (sinPR2 | sin14) };
	int SheltomAray[] = { 6, 7, 8, 9, 10, 11, 12, 13,
						  7, 8, 9, 10, 11, 12, 13, 14,
							3, 4, 5, 3, 4, 5 }; // 광석이나 수정 번호
	for (int i = 0; i < 22; i++)
	{
		if (SmeltingItemCode == SmeltingItemAray[i])
		{
			return SheltomAray[i];
		}
	}
	return FALSE;
}

// pluto 제작될 아이템 코드. 무게 검사 할려고 // 장별 - 제작
SManufacture_ResultItemInfo CManufacture::ManufactureCheckItem(sITEM* pItemCode, smCHAR_INFO* charInfo) // 장별 - 대장장이의 혼
{
	int RuneCnt = 0;
	SManufacture_ResultItemInfo Result;
	memset(&Result, 0, sizeof(SManufacture_ResultItemInfo));
	DWORD Arm_Armor_Code[MAX_RECIPE_KIND] = { sin01, sin02, sin03, sin04, sin05, sin06, sin07, sin08, sin09, sin10, sin11 }; // 레시피 종류 // 장별 - 대장장이의 혼
	DWORD CheckRuneCode[MAX_HANDMADEITEM][8][3] = { {	{ (sinPR3 | sin01), (sinPR3 | sin02), (sinPR3 | sin03) },  // 룬 조합식이다
										{ (sinPR3 | sin01), (sinPR3 | sin02), (sinPR4 | sin03) },
										{ (sinPR3 | sin01), (sinPR4 | sin02), (sinPR3 | sin03) },
										{ (sinPR3 | sin01), (sinPR4 | sin02), (sinPR4 | sin03) },
										{ (sinPR4 | sin01), (sinPR3 | sin02), (sinPR3 | sin03) },
										{ (sinPR4 | sin01), (sinPR3 | sin02), (sinPR4 | sin03) },
										{ (sinPR4 | sin01), (sinPR4 | sin02), (sinPR3 | sin03) },
										{ (sinPR4 | sin01), (sinPR4 | sin02), (sinPR4 | sin03) } },
		////////////////////////////////////////////////////////////////////////////////////////////	잊혀진 제작서
	 {  { (sinPR3 | sin02), (sinPR3 | sin03), (sinPR3 | sin04) },
		{ (sinPR3 | sin02), (sinPR3 | sin03), (sinPR4 | sin04) },
		{ (sinPR3 | sin02), (sinPR4 | sin03), (sinPR3 | sin04) },
		{ (sinPR3 | sin02), (sinPR4 | sin03), (sinPR4 | sin04) },
		{ (sinPR4 | sin02), (sinPR3 | sin03), (sinPR3 | sin04) },
		{ (sinPR4 | sin02), (sinPR3 | sin03), (sinPR4 | sin04) },
		{ (sinPR4 | sin02), (sinPR4 | sin03), (sinPR3 | sin04) },
		{ (sinPR4 | sin02), (sinPR4 | sin03), (sinPR4 | sin04) } },
		////////////////////////////////////////////////////////////////////////////////////////////	고대의 제작서
	 {  { (sinPR3 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) } },
		////////////////////////////////////////////////////////////////////////////////////////////	대지의 제작서
	 {  { (sinPR3 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) } },
		////////////////////////////////////////////////////////////////////////////////////////////	어둠의 제작서
	 {	{ (sinPR3 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) } },
		////////////////////////////////////////////////////////////////////////////////////////////	화염의 제작서
	 {  { (sinPR3 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR3 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR3 | sin04), (sinPR4 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR3 | sin05) },
		{ (sinPR4 | sin03), (sinPR4 | sin04), (sinPR4 | sin05) } },
		////////////////////////////////////////////////////////////////////////////////////////////	바람의 제작서
	 {  { (sinPR3 | sin04), (sinPR3 | sin05), (sinPR3 | sin06) },
		{ (sinPR3 | sin04), (sinPR3 | sin05), (sinPR4 | sin06) },
		{ (sinPR3 | sin04), (sinPR4 | sin05), (sinPR3 | sin06) },
		{ (sinPR3 | sin04), (sinPR4 | sin05), (sinPR4 | sin06) },
		{ (sinPR4 | sin04), (sinPR3 | sin05), (sinPR3 | sin06) },
		{ (sinPR4 | sin04), (sinPR3 | sin05), (sinPR4 | sin06) },
		{ (sinPR4 | sin04), (sinPR4 | sin05), (sinPR3 | sin06) },
		{ (sinPR4 | sin04), (sinPR4 | sin05), (sinPR4 | sin06) } },
		////////////////////////////////////////////////////////////////////////////////////////////	태양의 제작서
	 {  { (sinPR3 | sin05), (sinPR3 | sin06), (sinPR3 | sin07) },
		{ (sinPR3 | sin05), (sinPR3 | sin06), (sinPR4 | sin07) },
		{ (sinPR3 | sin05), (sinPR4 | sin06), (sinPR3 | sin07) },
		{ (sinPR3 | sin05), (sinPR4 | sin06), (sinPR4 | sin07) },
		{ (sinPR4 | sin05), (sinPR3 | sin06), (sinPR3 | sin07) },
		{ (sinPR4 | sin05), (sinPR3 | sin06), (sinPR4 | sin07) },
		{ (sinPR4 | sin05), (sinPR4 | sin06), (sinPR3 | sin07) },
		{ (sinPR4 | sin05), (sinPR4 | sin06), (sinPR4 | sin07) } },
		////////////////////////////////////////////////////////////////////////////////////////////	광포한 제작서
	 {  { (sinPR3 | sin06), (sinPR3 | sin07), (sinPR3 | sin08) },
		{ (sinPR3 | sin06), (sinPR3 | sin07), (sinPR4 | sin08) },
		{ (sinPR3 | sin06), (sinPR4 | sin07), (sinPR3 | sin08) },
		{ (sinPR3 | sin06), (sinPR4 | sin07), (sinPR4 | sin08) },
		{ (sinPR4 | sin06), (sinPR3 | sin07), (sinPR3 | sin08) },
		{ (sinPR4 | sin06), (sinPR3 | sin07), (sinPR4 | sin08) },
		{ (sinPR4 | sin06), (sinPR4 | sin07), (sinPR3 | sin08) },
		{ (sinPR4 | sin06), (sinPR4 | sin07), (sinPR4 | sin08) } },
		////////////////////////////////////////////////////////////////////////////////////////////	천상의 제작서

	 {  { (sinPR3 | sin09), (sinPR4 | sin10), (sinPR3 | sin11) },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 } },
		////////////////////////////////////////////////////////////////////////////////////////////	포설의 제작서 //장별

		{  { (sinPR3 | sin12), (sinPR3 | sin13), (sinPR3 | sin14) },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 } },
		////////////////////////////////////////////////////////////////////////////////////////////	기억의 제작서 // 장별 - 대장장이의 혼
	};

	// 장별 - 대장장이의 혼
	DWORD	dwItem = 0;

	// 프리스티스 일 경우
	if (charInfo->JOB_CODE == JOBCODE_PRIESTESS) dwItem = (sinDA2 | sin52);
	// 메지션 일 경우
	else if (charInfo->JOB_CODE == JOBCODE_MAGICIAN) dwItem = (sinDA2 | sin51);
	// 아처, 아틀란타 일 경우
	else if (charInfo->JOB_CODE == JOBCODE_ARCHER || charInfo->JOB_CODE == JOBCODE_ATALANTA) dwItem = (sinDA1 | sin52);
	// 파이터, 파이크맨, 메카니션, 나이트 일 경우
	else dwItem = (sinDA1 | sin51);


	// 장별 배열 숫자 [a][b][c] a = 무구 or 갑주 / b = 레시피 종류 / c = 생성될 아이템 종류(고정)
	// 레시피와 룬조합에 맞는 발생 아이템
	DWORD CreateItemCode[2][MAX_HANDMADEITEM][8] = { {	{ (sinWA1 | sin14), (sinWC1 | sin14), (sinWH1 | sin15), (sinWP1 | sin15), (sinWS1 | sin16), (sinWS2 | sin17), (sinWT1 | sin15), (sinWM1 | sin15) }, // 잊혀진 레시피 무구
											{ (sinWA1 | sin15), (sinWC1 | sin15), (sinWH1 | sin16), (sinWP1 | sin16), (sinWS1 | sin17), (sinWS2 | sin18), (sinWT1 | sin16), (sinWM1 | sin16) }, // 고대의 레시피 무구
											{ (sinWA1 | sin16), (sinWC1 | sin16), (sinWH1 | sin17), (sinWP1 | sin17), (sinWS1 | sin18), (sinWS2 | sin19), (sinWT1 | sin17), (sinWM1 | sin17) }, // 대지의 무구
											{ (sinWA1 | sin17), (sinWC1 | sin17), (sinWH1 | sin18), (sinWP1 | sin18), (sinWS1 | sin19), (sinWS2 | sin20), (sinWT1 | sin18), (sinWM1 | sin18) }, // 어둠의 레시피 무구
											{ (sinWA1 | sin18), (sinWC1 | sin18), (sinWH1 | sin19), (sinWP1 | sin19), (sinWS1 | sin20), (sinWS2 | sin21), (sinWT1 | sin19), (sinWM1 | sin19) }, // 화염의 레시피 무구
											{ (sinWA1 | sin19), (sinWC1 | sin19), (sinWH1 | sin20), (sinWP1 | sin20), (sinWS1 | sin21), (sinWS2 | sin22), (sinWT1 | sin20), (sinWM1 | sin20) }, // 바람의 레시피 무구
											{ (sinWA1 | sin21), (sinWC1 | sin21), (sinWH1 | sin22), (sinWP1 | sin22), (sinWS1 | sin23), (sinWS2 | sin24), (sinWT1 | sin22), (sinWM1 | sin22) }, // 태양의 레시피 무구
											{ (sinWA1 | sin22), (sinWC1 | sin22), (sinWH1 | sin23), (sinWP1 | sin23), (sinWS1 | sin24), (sinWS2 | sin25), (sinWT1 | sin23), (sinWM1 | sin23) }, // 광포한 레시피 무구
											{ 0, 0, 0, 0, 0, 0, 0, 0 } , // 천상의 레시피 무구
											{ 0, 0, 0, 0, 0, 0, 0, 0 } , // 포설의 레시피 // 장별
											{ 0, 0, 0, 0, 0, 0, 0, 0 } }, // 기억의 레시피 // 장별 - 대장장이의 혼
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
										{	{ (sinDA1 | sin16), (sinDA2 | sin16), (sinDS1 | sin14), (sinOM1 | sin15), (sinDG1 | sin14), (sinOA2 | sin14), (sinDB1 | sin14), 0 }, // 잊혀진 레시피 갑주
											{ (sinDA1 | sin17), (sinDA2 | sin17), (sinDS1 | sin15), (sinOM1 | sin16), (sinDG1 | sin15), (sinOA2 | sin15), (sinDB1 | sin15), 0 }, // 고대의 레시피 갑주
											{ (sinDA1 | sin18), (sinDA2 | sin18), (sinDS1 | sin16), (sinOM1 | sin17), (sinDG1 | sin16), (sinOA2 | sin16), (sinDB1 | sin16), 0 }, // 대지의 레시피 갑주
											{ (sinDA1 | sin19), (sinDA2 | sin19), (sinDS1 | sin17), (sinOM1 | sin18), (sinDG1 | sin17), (sinOA2 | sin17), (sinDB1 | sin17), 0 }, // 어둠의 레시피 갑주
											{ (sinDA1 | sin20), (sinDA2 | sin20), (sinDS1 | sin18), (sinOM1 | sin19), (sinDG1 | sin18), (sinOA2 | sin18), (sinDB1 | sin18), 0 }, // 화염의 레시피 갑주
											{ (sinDA1 | sin21), (sinDA2 | sin21), (sinDS1 | sin19), (sinOM1 | sin20), (sinDG1 | sin19), (sinOA2 | sin19), (sinDB1 | sin19), 0 }, // 바람의 레시피 갑주
											{ (sinDA1 | sin23), (sinDA2 | sin23), (sinDS1 | sin21), (sinOM1 | sin22), (sinDG1 | sin21), (sinOA2 | sin21), (sinDB1 | sin21), 0 }, // 태양의 레시피 갑주
											{ (sinDA1 | sin24), (sinDA2 | sin24), (sinDS1 | sin22), (sinOM1 | sin23), (sinDG1 | sin22), (sinOA2 | sin22), (sinDB1 | sin22), 0 }, // 광포한 레시피 갑주
											{ 0, 0, 0, 0, 0, 0, 0, 0 } , // 천상의 레시피 갑주
											{ (sinOA1 | sin36), 0, 0, 0, 0, 0, 0, 0 } , // 포설의 레시피 //장별
											{ dwItem, 0, 0, 0, 0, 0, 0, 0 } } // 기억의 레시피 // 장별 - 대장장이의 혼
	};

	if ((pItemCode[0].CODE & sinITEM_MASK2) == sinWR1)		// 무구
	{
		for (int i = 0; i < MAX_RECIPE_KIND; i++)
		{
			if (Arm_Armor_Code[i] == (pItemCode[0].CODE & sinITEM_MASK3))
			{
				for (int j = 0; j < MAX_RUNE; j++)   // 룬 조합 
				{
					RuneCnt = 0;
					for (int x = 0; x < 3; x++) // 필요한 룬
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
							for (int c = 0; c < MAX_HANDMADEITEM; c++) // 장별 - 대장장이의 혼
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
	else if ((pItemCode[0].CODE & sinITEM_MASK2) == sinDR1)		// 갑주
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
							for (int c = 0; c < MAX_HANDMADEITEM; c++) // 장별 - 대장장이의 혼
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
								for (int c = 0; c < MAX_HANDMADEITEM; c++) // 장별 - 대장장이의 혼
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

// pluto 제작 아이템 무게, 가격 정보
int ManufactureItemResultInfo(SManufacture_ResultItemInfo_Server* pResultItemInfo)
{
	for (int i = 0; i < MAX_HANDMADEITEM; i++) // 장별 - 제작 MAX_HANDMADEITEM = 제작 할 수 있는 아이템 갯수(레시피가 추가되면 증가) // 장별 - 대장장이의 혼
	{
		g_Manufacture_ItemInfo[i].ItemCode = pResultItemInfo->ItemCode[i];
		lstrcpy(g_Manufacture_ItemInfo[i].ItemName, pResultItemInfo->ItemName[i]);
		g_Manufacture_ItemInfo[i].Weight = pResultItemInfo->Weight[i];
		g_Manufacture_ItemInfo[i].Price = pResultItemInfo->Price[i];
	}
	return TRUE;
}