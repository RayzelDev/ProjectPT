#include "globals.h"
#include "sinLinkHeader.h"
#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "field.h"
#include "srcServer\onserver.h"
#include "GlobalsShared.h"
#include "..\\TitleBox\\TitleBox.h"
#include "Craft/CManufactureGoldWindow.h"
#include "Craft/CManufactureWindow.h"

int LValRing = 0;
int RValRing = 0;
int KelAmulet = 0;
int KelSheltom = 0;
int SetBonus1;
int SetBonus2;
int SetBossHP = 50;
int SetBossDef = 50;

int LbuttonUpCheckFlag = 0;

int sinInterHeight2 = -50;

int sinTempDamage2[2] = { 0,0 };
int sinTempAbsorb = 0;
int sinTempBlock = 0;

sITEMINFO TempLinkCoreItem;
POINT UnionCorePosi = { 0,0 };
int   sinOverWeightCnt = 0;
int sinMyShopItemPass = 0;

sITEM TempPerfectItem;
sITEM TempPerfectItem2;

int nName = 0;
int nYesButton = 0;
int nNoButton = 0;
int BackUpPosX, BackUpPosY = 0;
int BackUpButton2X, BackUpButton2Y = 0;
int BackUpButton3X, BackUpButton3Y = 0;

cINVENTORY	cInvenTory;

sINVENTORY_EFFECT	sInven_Effect[MAX_INVEN_PARTICLE];
sINVENTORY_EFFECT   UnionCore_Effect[100];
sINVENTORY_EFFECT   UnionCore_Effect_Bone[10];
sINVENTORY_EFFECT   UnionCore_Effect_Bomb[50];

sINVENTORY_EFFECT   ForceOrb_Effect;
int MatForceOrbEffect[8] = { 0 };
sITEM MyShopMouseItem;

sELEMENT_ATTACK	sElement_Attack;
sUNIONCORE_EFFECT   sUCore_Effect[10];

int StartX;
int StartY;

int EndX;
int EndY;

int ColorIndex;
RECT ColorRect = { 0,0,0,0 };

int CrashItemIndex[2] = { 0,0 };
int AutoSetItemIndex = 0;

int SelectInvenItemIndex = 0;

int SelectPotionIndex = 0;

int ArrowState[2] = { 0,0 };

int InvenArrowPosi = 0;

sITEM* pUsePotion = 0;

int sinLoadOK = 0;

int PotionIndex = 0;

sITEM* sinPosionItem = 0;
int BackUpItemIndex = 0;
int NotPotionFlag = 0;

char sinTestBuff[128];

int GhostPotionCheckFlag = 0;

int sinChangeSetFlag = 0;

int sinUsePotionDelayFlag = 0;
int PotionCheckNumJumpFlag = 1;

int sinUndeadAbsorb = 0;

int sinItemPickUpDelayFlag = 0;
float   sinAdd_fMagic_Mastery = 0;
int sinDamageFunction[3] = { 0,0,0 };

LPDIRECT3DTEXTURE9	lpInvenMain;

LPDIRECT3DTEXTURE9	lpUnionCoreEffect[4];

int MatUnionCoreEffect = 0;

LPDIRECT3DTEXTURE9   lpGold7 = 0;
LPDIRECT3DTEXTURE9   lpExp7 = 0;


LPDIRECT3DTEXTURE9   lpSpecialItem[2];
LPDIRECT3DTEXTURE9   lpSpecialItemInfo[2];

LPDIRECT3DTEXTURE9	lpCostumeButton;
LPDIRECT3DTEXTURE9	lpCostumeInfo;
LPDIRECT3DTEXTURE9	lpCostumeCover;

LPDIRECT3DTEXTURE9   lpSpecialInven;


int IndiShopIconFlag = 0;

sWINGQUEST sWingQuest[MAX_WINGQUEST] = {
	#include "sinWingItem.h"
};


int SpecialItemShowFlag = 0;
int NotChangeSetItemFlag = 0;
int PuzzleOkFlag = 0;

int PuzzleOkY = 0;

int sinTestFlag2 = 0;

BOOL IsPositionCostumeSlot()
{
	if ((pCursorPos.x > 650) && (pCursorPos.y > 415 + (256 - sinMoveKindInter[SIN_INVENTORY])) && (pCursorPos.x < 717) && (pCursorPos.y < 547 + (256 - sinMoveKindInter[SIN_INVENTORY])))
		return TRUE;
	else
		return FALSE;

	return FALSE;
}

BOOL IsPositionCostumeSlot2()
{
	if (cInvenTory.OpenFlag)
	{
		if ((pCursorPos.x > 650) && (pCursorPos.y > smConfig.ScreenSize.y - 190) && (pCursorPos.x < 717) && (pCursorPos.y < smConfig.ScreenSize.y - 54))
			return TRUE;
		else
			return FALSE;
	}

	return FALSE;
}


// Posi��o dos itens no invent�rio
sINVENTORY sInven[INVENTORY_MAX_POS] = {
	{ INVENTORY_POS_RHAND  ,{ 574 + 1 , 433 + 1 , 640 , 544 } },
	{ INVENTORY_POS_LHAND  ,{ 722 + 1 , 433 + 1 , 788 , 544 } },
	{ INVENTORY_POS_ARMOR  ,{ 648 + 1 , 433 + 1 , 714 , 544 } },
	{ INVENTORY_POS_AMULET ,{ 304 + 1 , 438 + 1 , 326 , 460 } },
	{ INVENTORY_POS_RRING  ,{ 356 + 1 , 438 + 1 , 378 , 460 } },
	{ INVENTORY_POS_LRING  ,{ 330 + 1 , 438 + 1 , 352 , 460 } },
	{ INVENTORY_POS_SHELTOM,{ 382 + 1 , 438 + 1 , 404 , 460 } },
	{ INVENTORY_POS_ARMLET ,{ 411 + 1 , 438 + 1 , 455 , 482 } },
	{ INVENTORY_POS_GLOVES ,{ 462 + 1 , 438 + 1 , 506 , 482 } },
	{ INVENTORY_POS_BOOTS  ,{ 513 + 1 , 438 + 1 , 557 , 482 } },
	{ INVENTORY_POS_POTION ,{ 495   , 565   , 495 + 26 , 565 + 26 } },
	{ INVENTORY_POS_POTION ,{ 495 + 26 , 565  , 495 + 26 + 26 , 565 + 26 } },
	{ INVENTORY_POS_POTION ,{ 495 + 26 + 26 , 565 , 495 + 26 + 26 + 26 , 565 + 26 } },
	{ INVENTORY_POS_COSTUME ,{ 648 + 1 , 433 + 1 , 714 , 544 } },
	{ INVENTORY_POS_EARRINGSL  ,{ 508 + 1 , 491 + 1 , 508 + 22 , 491 + 22 } },
	{ INVENTORY_POS_EARRINGSR  ,{ 535 + 1 , 491 + 1 , 535 + 22 , 491 + 22 } },
};

int ArrowPosi[10][4] = {
	{ 26,572,26 + 22,591 },
	{ 48,572,48 + 22,591 },
	{ 520,527,539,546 },
	{ 542,527,561,546 },
	{ 266,572,266 + 22,591 },
	{ 302,467,321,486 },
	{ 120,572,120 + 20,591 },
	{ 70,572,70 + 22,591 },
	{ 98,572,98 + 22,591 },
	{ 498,527,518,546 },
};


int InvenTextPosi[2][4] = {
	{146,577,209,592},
	{322,476,404,493},
};

sITEM InvenCostume;
sITEM InvenCostumeTemp;
int isItemCostume = 0;


int ForceOrbUseLevel[][2] = {
	{1,19},{1,29},{1,39},{1,49},{50,200},{50,200},{50,200},{50,200},{50,200},{50,200},{50,200},{50,200},{50,200},{50,200},{50,200},
};

int MagicForceOrbUseLevel[][2] = {
	{1,19},{1,29},{1,39},{1,49},{50,200},{50,200},{60,200},{70,200},{80,200},{80,200},{90,200},{90,200}, {90,200}
};

int BillingMagicForceOrbUseLevel[][2] = {
	{1,200},{1,200},{1,200},
};

int ForceOrbUseTime[] = { 500,500,500,500,700,900,1800,2400,3600,4800,6000,7200,8400,10000,12000 };

int MagicForceOrbUseTime[] = { 500,500,500,500,700,900,1800,2400,3600,4800,6000,7200,8400,3600 };

int BillingMagicForceOrbUseTime[] = { 3600, 10800, 86400 };

int ForceOrbDamage[] = { 2,4,7,10,15,25,40,60,80,100,120,140,160,180,200 };
int MagicForceOrbDamage[] = { 2,4,7,10,15,25,40,60,80,100,120,140,160,180 };
int BillingMagicForceOrbDamage[] = { 0,0,0 };

int BoosterItem_UseTime[] = { 3600,10800,86400 };
int BoosterItem_DataPercent[] = { 5,5,5 };

int nSkillDelay_UseTime[] = { 300,10800,86400 };

#define NUM_COSTUMES 27

enum ECostumeClass
{
	CLASS_Male,
	CLASS_Female,
};

struct sCOSTUME
{
	char* szItemCode;
	ECostumeClass	  eCostumeClass;
};

sCOSTUME saCostume[NUM_COSTUMES] =
{
	{ "ca101", CLASS_Male },
	{ "ca103", CLASS_Male },
	{ "ca105", CLASS_Male },
	{ "ca107", CLASS_Male },
	{ "ca109", CLASS_Male },
	{ "ca111", CLASS_Male },
	{ "ca113", CLASS_Male },
	{ "ca115", CLASS_Male },
	{ "ca117", CLASS_Male },
	{ "ca119", CLASS_Male },
	{ "ca121", CLASS_Male },
	{ "ca123", CLASS_Male },
	{ "ca135", CLASS_Male },
	{ "ca102", CLASS_Female },
	{ "ca104", CLASS_Female },
	{ "ca106", CLASS_Female },
	{ "ca108", CLASS_Female },
	{ "ca110", CLASS_Female },
	{ "ca112", CLASS_Female },
	{ "ca114", CLASS_Female },
	{ "ca116", CLASS_Female },
	{ "ca118", CLASS_Female },
	{ "ca120", CLASS_Female },
	{ "ca122", CLASS_Female },
	{ "ca124", CLASS_Female },
	{ "ca137", CLASS_Female },
	{ "", CLASS_Male },
};

DWORD sinInvenColor[7] = { D3DCOLOR_RGBA(160,235,256,128) , D3DCOLOR_RGBA(255,255,255,128) ,D3DCOLOR_RGBA(255,0,0,128),
					D3DCOLOR_RGBA(255,255,255,128) , D3DCOLOR_RGBA(255,200,0,128) ,D3DCOLOR_RGBA(0,231,123,128),
					D3DCOLOR_RGBA(173,203,247,128)

};

DWORD CristalEventCODE[12] = { (sinGP1 | sin01),(sinGP1 | sin02),(sinGP1 | sin03),(sinGP1 | sin04),(sinGP1 | sin05)
						   ,(sinGP1 | sin06),(sinGP1 | sin07),(sinGP1 | sin08),(sinGP1 | sin09),
							(sinGP1 | sin11),(sinGP1 | sin12),(sinGP1 | sin13) };

DWORD WatermelonEventCODE = (sinSP1 | sin42);
DWORD ValentineEventCODE = (sinSP1 | sin35);

DWORD PristonAlphabetEventCODE[7] = { (sinSP1 | sin27), (sinSP1 | sin28), (sinSP1 | sin29),
									  (sinSP1 | sin30), (sinSP1 | sin31), (sinSP1 | sin32), (sinSP1 | sin33) };

DWORD CandydaysEventCODE = (sinSP1 | sin36);

DWORD MagicalGreenEventCODE[2] = { (sinSP1 | sin37), (sinSP1 | sin38) };

DWORD TeatOfKaraEventCODE = (sinSP1 | sin39);

DWORD FindinvestigatorEventCODE[2] = { (sinGF1 | sin07), (sinGF1 | sin08) };


cINVENTORY::cINVENTORY()
{
	for (int i = 0; i < INVENTORY_MAXITEM; i++)
		memset(&InvenItem[i], 0, sizeof(sITEM));

	// new costume xxstr
	OpenFlag = 0;
	CostumeFlag = 0;
	CostumeChangeFlag = 0;
	CostumeTimer = 0;

}
cINVENTORY::~cINVENTORY()
{

}

void cINVENTORY::Init()
{
	// costume xxstr
	CostumeFlag = FALSE;

	StartX = 23;
	StartY = 436;

	EndX = StartX + ((INVENTORY_BOX_ROW) * 22);
	EndY = StartY + ((INVENTORY_BOX_COL) * 22);
	Load();

	ReformCharForm();
	SearchJobAndSetting();
	CheckWeight();
	SetItemToChar();

	sinSetLife(sinChar->Life[1]);
	sinSetMana(sinChar->Mana[1]);
	sinSetStamina(sinChar->Stamina[1]);
	ReformCharForm();

	int i = 0;
	char szBuff[128];
	for (i = 0; i < 5; i++) {
		wsprintf(szBuff, "Image\\SinImage\\Effect\\Particle\\InvenPaticle%d.tga", i);
		MatInvenParticle[i] = CreateTextureMaterial(szBuff, 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

	}
	MatUnionCoreEffect = CreateTextureMaterial("image\\Sinimage\\Effect\\InterEffect\\UC-hall.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

	LoadCharState();


}

void cINVENTORY::Load()
{
	char szBuff[128];
	for (int i = 0; i < 4; i++) {
		wsprintf(szBuff, "image\\Sinimage\\Effect\\InterEffect\\UC-0%d.bmp", i + 1);
		lpUnionCoreEffect[i] = LoadDibSurfaceOffscreen(szBuff);

	}

	lpIndiShopIcon = LoadDibSurfaceOffscreen("Image\\SinImage\\indishop\\indishop_icon.bmp");
	lpMyShopIcon_T = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\InfoBox\\inven-SaleT.bmp");

	lpMyShopIcon = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\inven-Sale.bmp");

	lpSpecialItem[0] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\QuestItem_.bmp");
	lpSpecialItem[1] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\QuestItem.bmp");

	lpCostumeButton = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\ButtonCostume.bmp");
	lpCostumeInfo = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\InfoBox\\Costume.bmp");
	lpCostumeCover = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\Costume-UI.bmp");

	lpSpecialItemInfo[0] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\InfoBox\\Questitem.bmp");
	lpSpecialItemInfo[1] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\InfoBox\\close.bmp");

	lpSpecialInven = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\QuestItembox.bmp");

	lpInvenMain = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\Inven.bmp");
	lpInvenArrow[0] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\LeftArrow.bmp");
	lpInvenArrow[1] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\RightArrow.bmp");

	lpCoverSmall = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\CoverSmall.bmp");
	lpCoverMiddle = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\CoverMiddle.bmp");
	lpCoverLarge = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\CoverLarge.bmp");

	lpPotionBack = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\PotionBack.bmp");

	lpChangeABInfo[0] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\InfoBox\\ButtonA.bmp");
	lpChangeABInfo[1] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\InfoBox\\ButtonB.bmp");

	lpExitInfo = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\InfoBox\\ButtonExit.bmp");
	lpWeightInfo = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\InfoBox\\ButtonWeight.bmp");

	lpItemABInfo[0] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\InfoBox\\ButtonItemA.bmp");
	lpItemABInfo[1] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\InfoBox\\ButtonItemB.bmp");

	lpShowWeaponClass[0] = LoadDibSurfaceOffscreen("Image\\SinImage\\Items\\Weapon_Onehand.bmp");
	lpShowWeaponClass[1] = LoadDibSurfaceOffscreen("Image\\SinImage\\Items\\Weapon_Twohand.bmp");
	lpShowWeaponClass[2] = LoadDibSurfaceOffscreen("Image\\SinImage\\Items\\Weapon_Knight.bmp");
	lpShowWeaponClass[3] = LoadDibSurfaceOffscreen("Image\\SinImage\\Items\\Weapon_Wizard.bmp");


	lpPuzzleInven = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\NYEAR_00.bmp");
	lpPuzzlewidth = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\NYEAR_01.bmp");
	lpPuzzleHeight = LoadDibSurfaceOffscreen("Image\\SinImage\\Inven\\NYEAR_02.bmp");

}
void cINVENTORY::InvenItemLoad()
{
	char szFilePath[256];
	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			for (int j = 0; j < MAX_ITEM; j++) {
				if (InvenItem[i].CODE == sItem[j].CODE) {
					wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[j].ItemFilePath, sItem[j].LastCategory);
					if (!sItem[j].lpTempItem)
						sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);
					InvenItem[i].lpItem = sItem[j].lpTempItem;
					break;
				}
			}
		}
		if (InvenItemTemp[i].Flag) {
			for (int j = 0; j < MAX_ITEM; j++) {
				if (InvenItemTemp[i].CODE == sItem[j].CODE) {
					wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[j].ItemFilePath, sItem[j].LastCategory);
					if (!sItem[j].lpTempItem)
						sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);
					InvenItemTemp[i].lpItem = sItem[j].lpTempItem;
					break;
				}
			}
		}
	}
}
//�������� Release�Ѵ� 
void cINVENTORY::InvenItemRelease()
{
	int i = 0;
	for (i = 0; i < MAX_ITEM; i++) {
		if (sItem[i].lpTempItem) {
			sItem[i].lpTempItem->Release();
			sItem[i].lpTempItem = 0;

		}
	}

	for (i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (InvenItem[i].lpItem) {
				InvenItem[i].lpItem->Release();
				InvenItem[i].lpItem = 0;
			}

		}
		if (InvenItemTemp[i].Flag) {
			if (InvenItemTemp[i].lpItem) {
				InvenItemTemp[i].lpItem->Release();
				InvenItemTemp[i].lpItem = 0;
			}

		}

	}

}
BOOL bIsPerfect = FALSE;
BOOL bIsPerfect2 = FALSE;
/*----------------------------------------------------------------------------*
*						     Release
*-----------------------------------------------------------------------------*/
void cINVENTORY::Release()
{
	int i;

	for (i = 0; i < 2; i++) {
		if (lpInvenArrow[i]) {
			lpInvenArrow[i]->Release();
			lpInvenArrow[i] = 0;
		}
		if (lpChangeABInfo[i]) {
			lpChangeABInfo[i]->Release();
			lpChangeABInfo[i] = 0;
		}
		if (lpItemABInfo[i]) {
			lpItemABInfo[i]->Release();
			lpItemABInfo[i] = 0;
		}

	}
	if (lpCoverSmall) {
		lpCoverSmall->Release();
		lpCoverSmall = 0;
	}
	if (lpCoverMiddle) {
		lpCoverMiddle->Release();
		lpCoverMiddle = 0;
	}
	if (lpCoverLarge) {
		lpCoverLarge->Release();
		lpCoverLarge = 0;
	}
	if (lpPotionBack) {
		lpPotionBack->Release();
		lpPotionBack = 0;
	}
	if (lpExitInfo) {
		lpExitInfo->Release();
		lpExitInfo = 0;
	}
	if (lpWeightInfo) {
		lpWeightInfo->Release();
		lpWeightInfo = 0;
	}
	if (lpShowWeaponClass[0]) {
		lpShowWeaponClass[0]->Release();
		lpShowWeaponClass[0] = 0;

	}

	if (lpShowWeaponClass[1]) {
		lpShowWeaponClass[1]->Release();
		lpShowWeaponClass[1] = 0;

	}
	if (lpCostumeButton)
	{
		lpCostumeButton->Release();
		lpCostumeButton = 0;
	}


}
/*----------------------------------------------------------------------------*
*							     �׸���
*-----------------------------------------------------------------------------*/
int CoreEffectTime = 0;
int CoreEffectFlag = 0;

ECostumeClass GetCharacterKind()
{
	if ((lpCurPlayer->smCharInfo.JOB_CODE == 1) || (lpCurPlayer->smCharInfo.JOB_CODE == 2) || (lpCurPlayer->smCharInfo.JOB_CODE == 4) || (lpCurPlayer->smCharInfo.JOB_CODE == 6) || (lpCurPlayer->smCharInfo.JOB_CODE == 7))
		return CLASS_Male;
	else
		return CLASS_Female;

	return CLASS_Male;
}

bool bControlHold = false;

void cINVENTORY::Draw()
{
	int i = 0;

	DrawSprite(0, 600 - sinMoveKindInter[SIN_INVENTORY] + 56, lpInvenMain, 0, 0, 800, 200);

	//Showing Perfect Item?
	if (InvenItem[SelectInvenItemIndex - 1].Flag)
	{
		if (VRKeyBuff['P'])
			bControlHold = true;
		else
		{
			ZeroMemory(&TempPerfectItem2, sizeof(sITEM));
			bIsPerfect2 = FALSE;
			bControlHold = false;
		}
	}
	else
	{
		ZeroMemory(&TempPerfectItem2, sizeof(sITEM));
		bIsPerfect2 = FALSE;
		bControlHold = false;
	}
	// costume xxstr

	if (CostumeFlag)
		DrawSprite(573, 688 - sinMoveKindInter[SIN_INVENTORY], lpCostumeCover, 0, 0, 217, 114);

	if (nName == 1)
	{
		sinChatEnter = 0;
		cInterFace.ChatFlag = 0;

		Interface_clanmenu.DrawBox(BackStartPos.x, BackStartPos.y, 2, 3);
		dsDrawTexImage(MatHelpBoxImage[9], BackStartPos.x + 7, BackStartPos.y + 35, 180, 15, 255);


		dsDrawTexImage(MatMyShopPRBoxLine, BackStartPos.x + 28, BackStartPos.y - 120, 254, 30, 255);

		DrawSprite(BackStartPos.x + BackUpButton2X, BackStartPos.y + BackUpButton2Y - 200, cMessageBox.lpButtonBox07, 0, 0, 48, 23, 1);
		DrawSprite(BackStartPos.x + BackUpButton3X, BackStartPos.y + BackUpButton3Y - 200, cMessageBox.lpButtonBox07, 0, 0, 48, 23, 1);

		if (pCursorPos.x >= BackStartPos.x + BackUpButton2X && pCursorPos.x <= BackStartPos.x + BackUpButton2X + 48 &&
			pCursorPos.y >= BackStartPos.y + BackUpButton2Y - 30 && pCursorPos.y <= BackStartPos.y + BackUpButton2Y - 7)
		{
			nYesButton = 1;
		}
		else nYesButton = 0;

		if (pCursorPos.x >= BackStartPos.x + BackUpButton3X && pCursorPos.x <= BackStartPos.x + BackUpButton3X + 48 &&
			pCursorPos.y >= BackStartPos.y + BackUpButton2Y - 30 && pCursorPos.y <= BackStartPos.y + BackUpButton2Y - 7)
		{

			nNoButton = 1;

			nName = 0;
			isDrawClanMenu = 0;
			SetIME_Mode(0);
			cInterFace.ChatFlag = 0;
			sinChatEnter = 0;
			sinMessageBoxShowFlag = 0;
			hFocusWnd = 0;
			cInterFace.ChatFlag = 0;

		}
		else nNoButton = 0;

		if (nYesButton == 0) DrawSprite(BackStartPos.x + BackUpButton2X + 7, BackStartPos.y + BackUpButton2Y - 200 + 3, cMessageBox.lpButtonYesGlay07, 0, 0, 32, 16, 1);
		else DrawSprite(BackStartPos.x + BackUpButton2X + 7, BackStartPos.y + BackUpButton2Y - 200 + 3, cMessageBox.lpButtonYes07, 0, 0, 32, 16, 1);

		if (nNoButton == 0) DrawSprite(BackStartPos.x + BackUpButton3X + 7, BackStartPos.y + BackUpButton3Y - 200 + 3, cMessageBox.lpButtonNoGlay07, 0, 0, 32, 16, 1);
		else DrawSprite(BackStartPos.x + BackUpButton3X + 7, BackStartPos.y + BackUpButton3Y - 200 + 3, cMessageBox.lpButtonNo07, 0, 0, 32, 16, 1);



	}

	// set costume xxstr
	for (i = 0; i < 3; i++)
	{
		if (CostumeFlag)
		{
			if (i == 0 || i == 2)
				continue;
			if (i == 1 && sInven[i].Position != INVENTORY_POS_COSTUME)
				continue;
		}

		if (sInven[i].ItemIndex && InvenItem[sInven[i].ItemIndex - 1].Flag)
			DrawSprite(sInven[i].Rect.left, sInven[i].Rect.top + (256 - sinMoveKindInter[SIN_INVENTORY]), lpCoverLarge, 0, 0, 66, 110);
	}

	// set costume xxstr
	if (CostumeFlag && sInven[13].ItemIndex)
		DrawSprite(sInven[13].Rect.left, sInven[13].Rect.top + (256 - sinMoveKindInter[SIN_INVENTORY]), lpCoverLarge, 0, 0, 66, 110);


	for (i = 0; i < 3; i++) {
		if (sInven[i].ItemIndex && InvenItem[sInven[i].ItemIndex - 1].Flag)
			DrawSprite(sInven[i].Rect.left, sInven[i].Rect.top + (256 - sinMoveKindInter[SIN_INVENTORY]), lpCoverLarge, 0, 0, 66, 110);
	}

	for (i = 3; i < 7; i++) {
		if (sInven[i].ItemIndex && InvenItem[sInven[i].ItemIndex - 1].Flag)
			DrawSprite(sInven[i].Rect.left, sInven[i].Rect.top + (256 - sinMoveKindInter[SIN_INVENTORY]), lpCoverSmall, 0, 0, 22, 22);
	}

	for (i = 7; i < 10; i++) {
		if (sInven[i].ItemIndex && InvenItem[sInven[i].ItemIndex - 1].Flag)
			DrawSprite(sInven[i].Rect.left, sInven[i].Rect.top + (256 - sinMoveKindInter[SIN_INVENTORY]), lpCoverMiddle, 0, 0, 44, 44);
	}

	for (i = 14; i < 16; i++) {
		if (sInven[i].ItemIndex && InvenItem[sInven[i].ItemIndex - 1].Flag)
			DrawSprite(sInven[i].Rect.left, sInven[i].Rect.top + (256 - sinMoveKindInter[SIN_INVENTORY]), lpCoverSmall, 0, 0, 22, 22);
	}

	if (ArrowState[0])
		DrawSprite(ArrowPosi[1][0], ArrowPosi[1][1] + (256 - sinMoveKindInter[SIN_INVENTORY]), lpInvenArrow[1], 0, 0, 20, 20);
	else
		DrawSprite(ArrowPosi[0][0], ArrowPosi[0][1] + (256 - sinMoveKindInter[SIN_INVENTORY]), lpInvenArrow[0], 0, 0, 20, 20);

	if (ArrowState[1])
		DrawSprite(ArrowPosi[3][0], ArrowPosi[3][1] + (256 - sinMoveKindInter[SIN_INVENTORY]), lpInvenArrow[1], 0, 0, 20, 20);
	else
		DrawSprite(ArrowPosi[2][0], ArrowPosi[2][1] + (256 - sinMoveKindInter[SIN_INVENTORY]), lpInvenArrow[0], 0, 0, 20, 20);

	DrawSprite(sInven[10].Rect.left, sInven[10].Rect.top, lpPotionBack, 0, 0, 77, 25);

	if (!SpecialItemShowFlag) {
		DrawSprite(ArrowPosi[7][0], ArrowPosi[7][1] + (256 - sinMoveKindInter[SIN_INVENTORY]), lpSpecialItem[0], 0, 0, 20, 20);
	}
	else {
		for (i = 0; i < 6; i++) {
			for (int j = 0; j < 3; j++) {
				DrawSprite(StartX + (i * 44), StartY + (j * 44) + (256 - sinMoveKindInter[SIN_INVENTORY]), lpSpecialInven, 0, 0, 44, 44);
			}
		}

		DrawSprite(ArrowPosi[7][0], ArrowPosi[7][1] + (256 - sinMoveKindInter[SIN_INVENTORY]), lpSpecialItem[1], 0, 0, 20, 20);
	}

	switch (ColorIndex) {
	case SET_ITEM_CHECK_COLOR:
		if (MouseItem.Flag) {
			dsDrawColorBox(sinInvenColor[0], ColorRect.left, ColorRect.top + (256 - sinMoveKindInter[SIN_INVENTORY]), ColorRect.right, ColorRect.bottom);
		}
		break;
	case OVERLAP_ITEM_COLOR:
		if (CrashItemIndex[0]) {
			dsDrawColorBox(sinInvenColor[1], InvenItem[CrashItemIndex[0] - 1].x,
				InvenItem[CrashItemIndex[0] - 1].y + (256 - sinMoveKindInter[SIN_INVENTORY]), InvenItem[CrashItemIndex[0] - 1].w, InvenItem[CrashItemIndex[0] - 1].h);
		}

		break;
	case NOT_SETTING_COLOR:
		dsDrawColorBox(sinInvenColor[2], ColorRect.left, ColorRect.top + (256 - sinMoveKindInter[SIN_INVENTORY]), ColorRect.right, ColorRect.bottom);
		break;

	case NOT_USE_COLOR:
		dsDrawColorBox(sinInvenColor[2], ColorRect.left, ColorRect.top + (256 - sinMoveKindInter[SIN_INVENTORY]), ColorRect.right, ColorRect.bottom);
		break;

	case OVERLAP_BOX_COLOR:
		if (CrashItemIndex[0]) {
			dsDrawColorBox(sinInvenColor[1], sInven[InvenItem[CrashItemIndex[0] - 1].ItemPosition - 1].Rect.left, sInven[InvenItem[CrashItemIndex[0] - 1].ItemPosition - 1].Rect.top + (256 - sinMoveKindInter[SIN_INVENTORY]),
				sInven[InvenItem[CrashItemIndex[0] - 1].ItemPosition - 1].Rect.right - sInven[InvenItem[CrashItemIndex[0] - 1].ItemPosition - 1].Rect.left,
				sInven[InvenItem[CrashItemIndex[0] - 1].ItemPosition - 1].Rect.bottom - sInven[InvenItem[CrashItemIndex[0] - 1].ItemPosition - 1].Rect.top);
		}

		break;
	case SELECT_ITEM_COLOR:
		if (!SelectInvenItemIndex)break;
		if (InvenItem[SelectInvenItemIndex - 1].ItemPosition) {
			dsDrawColorBox(sinInvenColor[4], sInven[InvenItem[SelectInvenItemIndex - 1].ItemPosition - 1].Rect.left, sInven[InvenItem[SelectInvenItemIndex - 1].ItemPosition - 1].Rect.top + (256 - sinMoveKindInter[SIN_INVENTORY]),
				sInven[InvenItem[SelectInvenItemIndex - 1].ItemPosition - 1].Rect.right - sInven[InvenItem[SelectInvenItemIndex - 1].ItemPosition - 1].Rect.left,
				sInven[InvenItem[SelectInvenItemIndex - 1].ItemPosition - 1].Rect.bottom - sInven[InvenItem[SelectInvenItemIndex - 1].ItemPosition - 1].Rect.top);

		}
		else {
			dsDrawColorBox(sinInvenColor[4], InvenItem[SelectInvenItemIndex - 1].x,
				InvenItem[SelectInvenItemIndex - 1].y + (256 - sinMoveKindInter[SIN_INVENTORY]), InvenItem[SelectInvenItemIndex - 1].w, InvenItem[SelectInvenItemIndex - 1].h);
		}

		break;

	}

	for (i = 0; i < 10; i++) {
		if (sUCore_Effect[i].Flag && sUCore_Effect[i].Time > 0) {
			dsDrawTexImage(sUCore_Effect[i].MatNum, sUCore_Effect[i].x, sUCore_Effect[i].y, sUCore_Effect[i].w,
				sUCore_Effect[i].h, sUCore_Effect[i].Alpha);
		}
	}


	for (int cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (InvenItem[cnt].Flag == 1) {
			if ((InvenItem[cnt].sItemInfo.CODE & sinITEM_MASK2) == sinPZ1 || (InvenItem[cnt].sItemInfo.CODE & sinITEM_MASK2) == sinPZ2) {
				DrawSprite(StartX + (22 * 8), sInven[0].Rect.top + (256 - sinMoveKindInter[SIN_INVENTORY]) + 2, lpPuzzleInven, 0, 0, 88, 88);
			}

			if (InvenItem[cnt].sItemInfo.NotUseFlag && !MyShopItemIndex[cnt]) {
				dsDrawColorBox(sinInvenColor[2], InvenItem[cnt].x, InvenItem[cnt].y + (256 - sinMoveKindInter[SIN_INVENTORY]), InvenItem[cnt].w, InvenItem[cnt].h);
				if (InvenItem[cnt].ItemPosition != 0) {
					dsDrawColorBox(sinInvenColor[2], sInven[InvenItem[cnt].ItemPosition - 1].Rect.left, sInven[InvenItem[cnt].ItemPosition - 1].Rect.top + (256 - sinMoveKindInter[SIN_INVENTORY]),
						sInven[InvenItem[cnt].ItemPosition - 1].Rect.right - sInven[InvenItem[cnt].ItemPosition - 1].Rect.left,
						sInven[InvenItem[cnt].ItemPosition - 1].Rect.bottom - sInven[InvenItem[cnt].ItemPosition - 1].Rect.top);
				}
			}
		}
	}

	for (i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (InvenItem[i].ItemPosition != 11 && InvenItem[i].ItemPosition != 12 && InvenItem[i].ItemPosition != 13) {
				if (MyShopItemIndex[i]) {
					dsDrawColorBox(D3DCOLOR_RGBA(45, 133, 255, 180), InvenItem[i].x, InvenItem[i].y + (256 - sinMoveKindInter[SIN_INVENTORY]), InvenItem[i].w, InvenItem[i].h);
				}

				if (CostumeFlag)
				{
					if (InvenItem[i].ItemPosition - 1 >= 0 && InvenItem[i].ItemPosition - 1 < INVENTORY_MAX_POS)
					{
						if (sInven[InvenItem[i].ItemPosition - 1].Position == INVENTORY_POS_ARMOR)
							continue;
						else if (sInven[InvenItem[i].ItemPosition - 1].Position == INVENTORY_POS_LHAND)
							continue;
						else if (sInven[InvenItem[i].ItemPosition - 1].Position == INVENTORY_POS_RHAND)
							continue;
					}
				}
				else
				{
					if (InvenItem[i].ItemPosition - 1 >= 0 && InvenItem[i].ItemPosition - 1 < INVENTORY_MAX_POS)
					{
						if (sInven[InvenItem[i].ItemPosition - 1].Position == INVENTORY_POS_COSTUME)
							continue;
					}
				}

				DrawSprite(InvenItem[i].x, InvenItem[i].y + (256 - sinMoveKindInter[SIN_INVENTORY]), InvenItem[i].lpItem, 0, 0, InvenItem[i].w, InvenItem[i].h);
				//�̹����� ���� �������� �׷��ش� 
				if (!InvenItem[i].lpItem) {
					if (InvenItem[i].x > 10) {//���� -_-
						dsDrawColorBox(D3DCOLOR_RGBA(125 + (i * 10), 125 + (i * 10), 255 + (i * 10), 125 + (i * 10)), InvenItem[i].x, InvenItem[i].y + (256 - sinMoveKindInter[SIN_INVENTORY]), InvenItem[i].w, InvenItem[i].h);
					}

				}
				if ((InvenItem[i].CODE & sinITEM_MASK2) == sinFO1) {
					DrawForceOrbEffect(InvenItem[i].x, InvenItem[i].y + (256 - sinMoveKindInter[SIN_INVENTORY]));
				}
			}
			else {
				DrawSprite(InvenItem[i].x, InvenItem[i].y, InvenItem[i].lpItem, 0, 0, InvenItem[i].w, InvenItem[i].h);
			}
		}
	}

	//�ϼ��� ���� ����Ʈ
	if (PuzzleOkFlag) {
		dsDrawColorBox(D3DCOLOR_RGBA(255, 255, 0, 30), StartX + (22 * 8), sInven[0].Rect.top + (256 - sinMoveKindInter[SIN_INVENTORY]) + 2, 88, 88);

		dsDrawColorBox(D3DCOLOR_RGBA(255, 123, 0, 180), StartX + (22 * 8), PuzzleOkY + sInven[0].Rect.top + (256 - sinMoveKindInter[SIN_INVENTORY]) + 4, 88, 2);
		dsDrawColorBox(D3DCOLOR_RGBA(255, 123, 0, 180), StartX + (22 * 8), (80 - PuzzleOkY) + sInven[0].Rect.top + (256 - sinMoveKindInter[SIN_INVENTORY]) + 4, 88, 2);


	}

	int len = 0;
	if (CoreEffectFlag == 0) {
		CoreEffectTime++;
	}
	else
		CoreEffectTime--;

	if (CoreEffectTime > 38)
		CoreEffectFlag = 1;
	else if (CoreEffectTime < 1)
		CoreEffectFlag = 0;


	for (i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {



			// Core de clan
			if (InvenItem[i].sItemInfo.CODE == (sinEC1 | sin05)) {
				len = lstrlen(InvenItem[i].sItemInfo.ItemName);
				if (InvenItem[i].sItemInfo.ItemName[len + 1] != NULL) {
					DrawSprite(InvenItem[i].x, InvenItem[i].y + (256 - sinMoveKindInter[SIN_INVENTORY]), lpUnionCoreEffect[CoreEffectTime / 10], 0, 0, 44, 22);

				}
			}

		}
	}

	// costume xxstr
	if (CostumeFlag)
		DrawSprite(ArrowPosi[9][0], ArrowPosi[9][1] + (256 - sinMoveKindInter[SIN_INVENTORY]), lpCostumeButton, 0, 0, 20, 20);

	switch (InvenArrowPosi) {
	case 1:
		DrawSprite(ArrowPosi[0][0], ArrowPosi[0][1] + (256 - sinMoveKindInter[SIN_INVENTORY]), lpInvenArrow[0], 0, 0, 20, 20);
		DrawSprite(ArrowPosi[0][0], ArrowPosi[0][1] - 27 + (256 - sinMoveKindInter[SIN_INVENTORY]), lpItemABInfo[0], 0, 0, 57, 27);

		break;
	case 2:
		DrawSprite(ArrowPosi[1][0], ArrowPosi[1][1] + (256 - sinMoveKindInter[SIN_INVENTORY]), lpInvenArrow[1], 0, 0, 20, 20);
		DrawSprite(ArrowPosi[1][0], ArrowPosi[1][1] - 27 + (256 - sinMoveKindInter[SIN_INVENTORY]), lpItemABInfo[1], 0, 0, 57, 27);

		break;
	case 3:
		DrawSprite(ArrowPosi[2][0], ArrowPosi[2][1] + (256 - sinMoveKindInter[SIN_INVENTORY]), lpInvenArrow[0], 0, 0, 20, 20);
		DrawSprite(ArrowPosi[2][0], ArrowPosi[2][1] - 27 + (256 - sinMoveKindInter[SIN_INVENTORY]), lpChangeABInfo[0], 0, 0, 70, 27);
		break;
	case 4:
		DrawSprite(ArrowPosi[3][0], ArrowPosi[3][1] + (256 - sinMoveKindInter[SIN_INVENTORY]), lpInvenArrow[1], 0, 0, 20, 20);
		DrawSprite(ArrowPosi[3][0], ArrowPosi[3][1] - 27 + (256 - sinMoveKindInter[SIN_INVENTORY]), lpChangeABInfo[1], 0, 0, 70, 27);
		break;

	case 5:
		DrawSprite(ArrowPosi[4][0], ArrowPosi[4][1] + (256 - sinMoveKindInter[SIN_INVENTORY]), cShop.lpExit, 0, 0, 20, 20);
		DrawSprite(ArrowPosi[4][0] - 8, ArrowPosi[4][1] - 27 + (256 - sinMoveKindInter[SIN_INVENTORY]), lpExitInfo, 0, 0, 47, 27);
		break;
	case 6:
		//DrawSprite(346, 515-27+(256-sinMoveKindInter[SIN_INVENTORY]),lpWeightInfo,0 ,0 , 70,27);
		break;
	case 8:
		if (!SpecialItemShowFlag) {
			DrawSprite(ArrowPosi[7][0], ArrowPosi[7][1] + (256 - sinMoveKindInter[SIN_INVENTORY]), lpSpecialItem[1], 0, 0, 20, 20);
			DrawSprite(ArrowPosi[7][0], ArrowPosi[7][1] - 27 + (256 - sinMoveKindInter[SIN_INVENTORY]), lpSpecialItemInfo[0], 0, 0, 100, 27);
		}
		else {
			DrawSprite(ArrowPosi[7][0], ArrowPosi[7][1] + (256 - sinMoveKindInter[SIN_INVENTORY]), lpSpecialItem[1], 0, 0, 20, 20);
			DrawSprite(ArrowPosi[7][0], ArrowPosi[7][1] - 27 + (256 - sinMoveKindInter[SIN_INVENTORY]), lpSpecialItemInfo[1], 0, 0, 70, 27);
		}
		break;
	case 9:
		DrawSprite(ArrowPosi[8][0], ArrowPosi[8][1] + (256 - sinMoveKindInter[SIN_INVENTORY]), lpMyShopIcon, 0, 0, 20, 20);
		DrawSprite(ArrowPosi[8][0] - 39, ArrowPosi[8][1] - 27 + (256 - sinMoveKindInter[SIN_INVENTORY]), lpMyShopIcon_T, 0, 0, 100, 26);
		break;
	case 10:
		//DrawSprite( ArrowPosi[ 9 ][ 0 ], ArrowPosi[ 9 ][ 1 ] + ( 256 - sinMoveKindInter[ SIN_INVENTORY ] ), lpCostumeButton, 0, 0, 20, 20 );
		DrawSprite(ArrowPosi[9][0], ArrowPosi[9][1] - 27 + (256 - sinMoveKindInter[SIN_INVENTORY]), lpCostumeInfo, 0, 0, 80, 27);

		break;

	}
	//traje xxstr
	/*if( bCostumeSlot )
		DrawSprite( ArrowPosi[ 9 ][ 0 ], ArrowPosi[ 9 ][ 1 ] + ( 256 - sinMoveKindInter[ SIN_INVENTORY ] ), lpCostumeButton, 0, 0, 20, 20 );*/

	if (sinSkill.pLeftSkill) {		 //���� ��ų 
		if (sinSkill.pLeftSkill->UseSkillFlag)
			DrawSprite(sLeftRightSkill[0].BoxRect.left, sLeftRightSkill[0].BoxRect.top, //��ư 
				CSKILL->lpSkillButton[sinSkill.pLeftSkill->Position - 1], 0, 0, 40, 40);
		else
			DrawSprite(sLeftRightSkill[0].BoxRect.left, sLeftRightSkill[0].BoxRect.top, //��ư 
				CSKILL->lpSkillButton_Gray[sinSkill.pLeftSkill->Position - 1], 0, 0, 40, 40);

	}
	if (sinSkill.pRightSkill) {		//������ ��ų  
		if (sinSkill.pRightSkill->UseSkillFlag)
			DrawSprite(sLeftRightSkill[1].BoxRect.left, sLeftRightSkill[1].BoxRect.top, //��ư 
				CSKILL->lpSkillButton[sinSkill.pRightSkill->Position - 1], 0, 0, 40, 40);
		else
			DrawSprite(sLeftRightSkill[1].BoxRect.left, sLeftRightSkill[1].BoxRect.top, //��ư 
				CSKILL->lpSkillButton_Gray[sinSkill.pRightSkill->Position - 1], 0, 0, 40, 40);
	}


	//���Ͽ� �ھ�
	//��ƼŬ
	for (i = 0; i < 100; i++) {
		if (UnionCore_Effect[i].Flag) {
			cInvenTory.sinDrawInvenTexImage(&UnionCore_Effect[i].InvenFace);
		}
	}

	for (i = 0; i < 50; i++) {
		if (UnionCore_Effect_Bomb[i].Flag) {
			cInvenTory.sinDrawInvenTexImage(&UnionCore_Effect_Bomb[i].InvenFace);


		}
	}
	//////////���� ����
	if (IndiShopIconFlag) {
		DrawSprite(225, 570, lpIndiShopIcon, 0, 0, 27, 25);
		DrawSprite(225 - 35, 570 - 27, cShop.lpMyShop_T, 0, 0, 100, 26);

	}

}

int InvenMainCount = 0;
int SetDelayFlag = 0;
bool bGiftBoxDelay = false;
DWORD dwUsePotionDelayTime = 0;
DWORD dwCheckHackCharStateTime = 0;
DWORD dwCheckHackCharStateFlag = 0;
DWORD dwCheckPickUpItemTime = 0;
int	  CharToItemFirstFlag = 0;
DWORD CharToItemFirstTime = 0;
DWORD dwUseCristalDelayTime = 0;
int   sinUseCristalDelayFlag = 0;


DWORD dwCheckAutoMouseTime = 0;

int   CheckPosiFlag = 0;
#define   CHECKAUTOMOUSE_DISTANCE		(200*200)
#define   CHECKAUTOMOUSE_SETITEM_MAX	500
#define   CHECKAUTOMOUSE_TIME			(70*60*30)

//#define   CHECKAUTOMOUSE_SETITEM_MAX	100
//#define   CHECKAUTOMOUSE_TIME			(70*60*10)

int   CheckX = 0, CheckZ = 0;
int   CheckResultX = 0, CheckResultZ = 0;

DWORD dwPuzzleOkTime = 0;
int ReversEffectFlag = 0;
DWORD dwForceOrbTime = 0;
int   ForceOrbEffectIndex = 0;

int iCheckTime = 0;

void deleteCoinFromInventory()
{
	sITEMINFO* itemToDelete = nullptr;

	extern DWORD dwTotal_InvenItemCode;
	extern DWORD dwTotal_InvenItemPlayTime;
	extern DWORD GetInvenItemCode();

	itemToDelete = FindItemFromName("Coins");

	if (itemToDelete != nullptr)
	{
		cInvenTory.DeleteInvenItemToServer(itemToDelete->CODE,
			itemToDelete->ItemHeader.Head, itemToDelete->ItemHeader.dwChkSum);

		DWORD dwCode = GetInvenItemCode();
		dwTotal_InvenItemCode = dwPlayTime ^ dwCode;
		dwTotal_InvenItemPlayTime = dwPlayTime;
	}
}

// costume xxstr
bool cINVENTORY::setCostumer()
{
	if (GetTickCount() - CostumeTimer <= 5000)
	{
		TitleBox::GetInstance()->SetText("Aguarde 5 segundos para trocar!", 3);
		return false;
	}

	else
	{
		CostumeFlag = !CostumeFlag;
		CostumeChangeFlag = TRUE;
		CostumeTimer = GetTickCount();

		return true;
	}
}

#include <boost\\algorithm\\string\\predicate.hpp>
#include "../AntiCheat.h"

bool flagCheckCoin = false;
bool cancheckCoin = true;

void cINVENTORY::Main()
{
	dwForceOrbTime++;
	if (dwForceOrbTime > 8) {
		dwForceOrbTime = 0;
		ForceOrbEffectIndex++;
		if (ForceOrbEffectIndex > 7)
			ForceOrbEffectIndex = 0;
	}

	if (PuzzleOkFlag) {
		dwPuzzleOkTime++;
		if (dwPuzzleOkTime > 2) {
			dwPuzzleOkTime = 0;
			if (PuzzleOkY > 82)ReversEffectFlag = 1;
			if (ReversEffectFlag) {
				PuzzleOkY--;
				if (PuzzleOkY <= 0)
					ReversEffectFlag = 0;
			}
			else PuzzleOkY++;

		}
	}
	else {
		PuzzleOkY = 0;
		dwPuzzleOkTime = 0;
	}

	int dist = 0;
	if (!CheckPosiFlag) {
		CheckX = lpCurPlayer->pX;
		CheckZ = lpCurPlayer->pZ;
		CheckPosiFlag = 1;
	}

	dwCheckAutoMouseTime++;
	if (dwCheckAutoMouseTime >= CHECKAUTOMOUSE_TIME) {
		if (sinOverWeightCnt > CHECKAUTOMOUSE_SETITEM_MAX) {
			CheckResultX = (lpCurPlayer->pX - CheckX) >> FLOATNS;
			CheckResultZ = (lpCurPlayer->pZ - CheckZ) >> FLOATNS;
			dist = CheckResultX * CheckResultX + CheckResultZ * CheckResultZ;
			if (CHECKAUTOMOUSE_DISTANCE >= dist) {
				sinOverWeightCnt = 0;
				CheckPosiFlag = 0;
				dwCheckAutoMouseTime = 0;
			}
			else {
				sinOverWeightCnt = 0;
				CheckPosiFlag = 0;
				dwCheckAutoMouseTime = 0;
			}

		}
		else {
			sinOverWeightCnt = 0;
			CheckPosiFlag = 0;
			dwCheckAutoMouseTime = 0;
		}

	}

	if (sinUseCristalDelayFlag) {
		dwUseCristalDelayTime++;
		if (dwUseCristalDelayTime > 70) {
			sinUseCristalDelayFlag = 0;
			dwUseCristalDelayTime = 0;
		}
	}

	UnionCoreParticleProcess();
	dwCheckHackCharStateTime++;
	if (dwCheckHackCharStateTime >= 30 * 70) {
		SetItemToChar(TRUE);
		dwCheckHackCharStateTime = 0;

	}


	if (sinUsePotionDelayFlag) {
		dwUsePotionDelayTime++;
		if (dwUsePotionDelayTime > 50) {
			dwUsePotionDelayTime = 0;
			sinUsePotionDelayFlag = 0;

		}

	}

	if (SetDelayFlag) {
		InvenMainCount++;
		if (InvenMainCount >= 70) {
			SetDelayFlag = 0;
			InvenMainCount = 0;
		}

	}


	if (nName) {
		BackUpButton2X = 35;
		BackUpButton2Y = 157;
		BackUpButton3X = 115;
		BackUpButton3Y = 157;

		BackUpPosX = BackStartPos.x;
		BackUpPosY = BackStartPos.y;

		if (pCursorPos.x >= BackStartPos.x + BackUpButton2X && pCursorPos.x <= BackStartPos.x + BackUpButton2X + 48 &&
			pCursorPos.y >= BackStartPos.y + BackUpButton2Y - 30 && pCursorPos.y <= BackStartPos.y + BackUpButton2Y - 7)
		{
			nYesButton = 1;
		}

		if (pCursorPos.x >= BackStartPos.x + BackUpButton3X && pCursorPos.x <= BackStartPos.x + BackUpButton3X + 48 &&
			pCursorPos.y >= BackStartPos.y + BackUpButton2Y - 30 && pCursorPos.y <= BackStartPos.y + BackUpButton2Y - 7)
		{

			nNoButton = 1;
		}

		BackStartPos.x = BackUpPosX;
		BackStartPos.y = BackUpPosY;

	}

	if (bGiftBoxDelay)
	{
		InvenMainCount++;
		if (InvenMainCount >= 140)
		{
			bGiftBoxDelay = false;
			InvenMainCount = 0;
		}
	}
	InvenArrowPosi = 0;
	int i = 0;
	if (OpenFlag)
		for (i = 0; i < 10; i++)
			if (ArrowPosi[i][0] < pCursorPos.x && ArrowPosi[i][2] > pCursorPos.x &&
				ArrowPosi[i][1] < pCursorPos.y && ArrowPosi[i][3] > pCursorPos.y)
				InvenArrowPosi = i + 1;

	for (i = 0; i < 3; i++)
		if (sInven[i + 10].Rect.left < pCursorPos.x && sInven[i + 10].Rect.right > pCursorPos.x &&
			sInven[i + 10].Rect.top < pCursorPos.y && sInven[i + 10].Rect.bottom > pCursorPos.y) {
			cHelpPet.PetMessage("*Potion_ShortKey", 0);
		}


	PickUpInvenItem(pCursorPos.x, pCursorPos.y);

	int BackdsDrawOffsetArray = 0;
	if (MouseItem.Flag) {

		if (!flagCheckCoin && cancheckCoin)
		{
			if (boost::iequals(MouseItem.DorpItem, "BI254"))
				flagCheckCoin = true;

			cancheckCoin = false;
		}

		SetInvenItemAreaCheck(&MouseItem);
		sinItemPickUpDelayFlag = 1;

	}
	if (sinItemPickUpDelayFlag) {
		dwCheckPickUpItemTime++;
		if (dwCheckPickUpItemTime > 70) {
			sinItemPickUpDelayFlag = 0;
			dwCheckPickUpItemTime = 0;
		}

	}
	else {
		// Deleta os coins do invent�rio se o player tiver segurando eles
		if (flagCheckCoin)
		{
			deleteCoinFromInventory();
			flagCheckCoin = false;
		}

		cancheckCoin = true;
		sinItemPickUpDelayFlag = 0;
		dwCheckPickUpItemTime = 0;
	}

	if (!PotionCheckNumJumpFlag)
		CheckPotionNumForm();

	if (OpenFlag) {
	}

	if (!CharToItemFirstFlag) {
		CharToItemFirstTime++;
		if (CharToItemFirstTime >= 70 * 3) {
			SetItemToChar(0);
			SetItemToChar();
			CharToItemFirstFlag = 1;
		}
	}

}
void cINVENTORY::Close()
{
	Release();
}
int sinButtonFlag = 0;

void cINVENTORY::LButtonDown(int x, int y)
{
	// Verifica se o char ta fazendo alguma animacao, se tiver ele nao coloca o item no inventario
	if ((lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_YAHOO) || (lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_WALK)
		|| (lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_RUN) || (lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_ATTACK) || (lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_SKILL))
		return;

	if (nYesButton == 1)
	{
		strcpy_s(cInvenTory.szSendDoc, cInvenTory.szDoc);
		nName = 0;
		isDrawClanMenu = 0;
		SetIME_Mode(0);
		cInterFace.ChatFlag = 0;
		sinChatEnter = 0;
		sinMessageBoxShowFlag = 0;
		hFocusWnd = 0;
		cInterFace.ChatFlag = 0;
		SendsServerDoc(cInvenTory.szSendDoc);
	}

	if (nNoButton == 1)
	{
		nName = 0;
		isDrawClanMenu = 0;
		SetIME_Mode(0);
		cInterFace.ChatFlag = 0;
		sinChatEnter = 0;
		sinMessageBoxShowFlag = 0;
		hFocusWnd = 0;
		cInterFace.ChatFlag = 0;
	}

	//PetMessage
	int i = 0;
	for (i = 0; i < 3; i++)
		if (sInven[i + 10].Rect.left < pCursorPos.x && sInven[i + 10].Rect.right > pCursorPos.x &&
			sInven[i + 10].Rect.top < pCursorPos.y && sInven[i + 10].Rect.bottom > pCursorPos.y) {
			cHelpPet.PetMessage("*Potion_ShortKey", 1);

		}

	if (InvenArrowPosi == 9) {
		if (!MouseItem.Flag) {
			cInterFace.CheckAllBox(SIN_MYSHOP);
		}

	}
	// traje xxstr
	if (InvenArrowPosi == 10)
	{
		if (!MouseItem.Flag)
		{
			setCostumer();
		}
	}

	if (bGiftBoxDelay)
		return;

	if (InvenArrowPosi == 8) {
		if (!cTrade.OpenFlag && !cWareHouse.OpenFlag && !cCraftItem.OpenFlag && !cMyShop.OpenFlag && !cCharShop.OpenFlag && !MyShopSendButton && !Caravana::GetInstance()->OpenFlag)
			ChangeSpecialItem();

	}
	if (InvenArrowPosi == 5) {
		OpenFlag = 0;

	}
	if (InvenArrowPosi == 7) {
		if (!MouseItem.Flag) {
			if (!cTrade.OpenFlag && !cMyShop.OpenFlag && !MyShopSendButton && !cCharShop.OpenFlag && !sSinWarpGate.ShowFlag)
				cMessageBox.ShowMessage2(MESSAGE_THROW_MONEY);
			if (cWareHouse.OpenFlag)
				cMessageBox.ShowMessage2(MESSAGE_MOVE_MONEY_MOVE);
		}
	}

	if (InvenArrowPosi && InvenArrowPosi < 5) {
		if (!cTrade.OpenFlag) {
			if (sinButtonFlag == InvenArrowPosi)return;
			sinButtonFlag = InvenArrowPosi;
			if (InvenArrowPosi <= 2)
				ChangeABItem(InvenArrowPosi);
			if (InvenArrowPosi >= 3)
				sinChangeSetFlag = InvenArrowPosi;
		}

	}
	if (sinCraftItemStartFlag || StartInterEffectAging2)
	{
		return;
	}
	else if ((ManufactureItem.m_OpenFlag || SmeltingItem.OpenFlag) && MixCancelButtonDelayFlag)
	{
		return;
	}

	if (MouseItem.Flag) {
		if (cInvenTory.OpenFlag && MouseItem.sItemInfo.SpecialItemFlag[1] == ITEM_KIND_SPECIAL) {
			if (ColorIndex == OVERLAP_ITEM_COLOR || ColorIndex == SET_ITEM_CHECK_COLOR) {
				if (!SpecialItemShowFlag) {
					cInvenTory.UnionCoreParticle2(MouseItem.x, MouseItem.y);
					sinPlaySound(SIN_SOUND_EAT_POTION2);
				}
				SetSpecialItem(&MouseItem);
				MouseItem.Flag = 0;
				ReFormInvenItem();
				ReformCharForm();
				return;
			}

		}
		if (SpecialItemShowFlag)return;
		if (SetDelayFlag)return;

		if (!sSinWarpGate.ShowFlag)
			ThrowInvenItemToField(&MouseItem);

		if (ColorIndex == NO_BOX_COLOR || ColorIndex == NOT_SETTING_COLOR)return;
		if (!CheckSetOk(&MouseItem))
			return;
		if (MouseItem.Class == ITEM_CLASS_POTION) {
			if (!MouseSetPotion(&MouseItem)) {
				if (MouseItem.Flag) {
					NoSettingThrowItem(&MouseItem);
				}

			}
			else
				CheckWeight();

		}
		else {
			if (!ChangeInvenItem(&MouseItem)) {
				if (MouseItem.ItemPosition) {
					if (sInven[MouseItem.ItemPosition - 1].ItemIndex && InvenItem[sInven[MouseItem.ItemPosition - 1].ItemIndex - 1].Flag) {
						return;
					}

				}
				if (!LastSetInvenItem(&MouseItem)) {
					if (MouseItem.Flag) {
						if ((MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
							(MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
							(MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinQT1 ||
							(MouseItem.sItemInfo.CODE & sinITEM_MASK2) == sinQW1) {

						}
						else {
							NoSettingThrowItem(&MouseItem);
						}
					}
				}
			}
		}
	}
	else {
		if (lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_ATTACK ||
			lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_SKILL)return;

		pMessageItem = 0;
		CheckInvenItemForm();
		if (cMyShop.OpenFlag) {
			PickUpInvenItem(pCursorPos.x, pCursorPos.y, 2);
		}
		else {
			if (!MyShopSendButton && !cCharShop.OpenFlag && !sSinWarpGate.ShowFlag) {
				PickUpInvenItem(pCursorPos.x, pCursorPos.y, 1);
			}
		}

		ReFormInvenItem();
		if (MouseItem.Flag) {
			if (MouseItem.Class != ITEM_CLASS_POTION) {
				CopyItemClear(&MouseItem);
			}
		}
	}

	if (MouseItem.Flag)
		LbuttonUpCheckFlag = 1;


}

void cINVENTORY::LButtonUp(int x, int y)
{
	LbuttonUpCheckFlag = 0;
	sinMyShopItemPass = 0;
}

DWORD dwBoosterItemCode[] = { sin21,sin22,sin23,sin24,sin25,sin26,sin27,sin28,sin29,0 };
DWORD dwCastlItemCODE[] = { sin01,sin02,sin03,sin04,sin05,sin06,sin07,sin08,sin09,sin10,sin11,sin12,sin13,sin14,sin15,sin16,0 };
DWORD dwNotUseVillageItemCODE[3] = { (sinBI1 | sin05),(sinBI1 | sin06),(sinBI1 | sin07) };
DWORD dwNotUseItemCODE[6][3] = { {SCROLL_INVULNERABILITY,(sinBC1 | sin01),1},{SCROLL_CRITICAL,(sinBC1 | sin02),2},{SCROLL_EVASION,(sinBC1 | sin03),3},
							  {SCROLL_P_INVULNERABILITY,(sinBI1 | sin05),1},{SCROLL_P_CRITICAL,(sinBI1 | sin06),2},{SCROLL_P_EVASION,(sinBI1 | sin07),3} };
DWORD dwSkillDelayItemCode[] = { sin30,sin31,sin32, 0 };

extern POINT AgingLevelSheltomXY[4][3];
extern int AgingLevelSheltomIndex[4][3];
extern DWORD dwTickCrystal;

void cINVENTORY::RButtonDown(int x, int y)
{
	//Botao direito aging
	/*if( cAging.OpenFlag )
	{
		int iSheltomCode;

		if( !sAgingItem.AgingItem[ 0 ].Flag )
			return;

		if( strnicmp( InvenItem[ SelectInvenItemIndex - 1 ].LastCategory, "os", 2 ) == 0 )
		{
			if( InvenItem[ SelectInvenItemIndex - 1 ].CODE == ( sinOS1 | sin03 ) )
				iSheltomCode = 3;
			else if( InvenItem[ SelectInvenItemIndex - 1 ].CODE == ( sinOS1 | sin04 ) )
				iSheltomCode = 4;
			else if( InvenItem[ SelectInvenItemIndex - 1 ].CODE == ( sinOS1 | sin05 ) )
				iSheltomCode = 5;
			else if( InvenItem[ SelectInvenItemIndex - 1 ].CODE == ( sinOS1 | sin06 ) )
				iSheltomCode = 6;
			else if( InvenItem[ SelectInvenItemIndex - 1 ].CODE == ( sinOS1 | sin07 ) )
				iSheltomCode = 7;
			else if( InvenItem[ SelectInvenItemIndex - 1 ].CODE == ( sinOS1 | sin08 ) )
				iSheltomCode = 8;
			else if( InvenItem[ SelectInvenItemIndex - 1 ].CODE == ( sinOS1 | sin09 ) )
				iSheltomCode = 9;
			else if( InvenItem[ SelectInvenItemIndex - 1 ].CODE == ( sinOS1 | sin10 ) )
				iSheltomCode = 10;
			else if( InvenItem[ SelectInvenItemIndex - 1 ].CODE == ( sinOS1 | sin11 ) )
				iSheltomCode = 11;
			else if( InvenItem[ SelectInvenItemIndex - 1 ].CODE == ( sinOS1 | sin12 ) )
				iSheltomCode = 12;
			else if( InvenItem[ SelectInvenItemIndex - 1 ].CODE == ( sinOS1 | sin13 ) )
				iSheltomCode = 13;
			else if( InvenItem[ SelectInvenItemIndex - 1 ].CODE == ( sinOS1 | sin14 ) )
				iSheltomCode = 14;
			else if( InvenItem[ SelectInvenItemIndex - 1 ].CODE == ( sinOS1 | sin15 ) )
				iSheltomCode = 15;
			else if( InvenItem[ SelectInvenItemIndex - 1 ].CODE == ( sinOS1 | sin16 ) )
				iSheltomCode = 16;

			//Slots Rows
			for( int i = 0; i < 4; i++ )
			{
				//Slots Columns
				for( int j = 0; j < 3; j++ )
				{
					//Get Sheltom Code belonging to this slot
					int iCorrectSheltomCode = AgingLevelSheltomIndex[i][j];

					//Given Sheltom of correct code for this slot?
					if( iSheltomCode == iCorrectSheltomCode )
					{
						//Check if this slot already has a Sheltom
						BOOL bAlreadySet = FALSE;
						for( int k = 1; k < 15; k++ )
						{
							sITEM * psCurrentItem = sAgingItem.AgingItem + k;

							if( psCurrentItem->Flag )
							{
								if( psCurrentItem->x == AgingLevelSheltomXY[ i ][ j ].x && psCurrentItem->y == AgingLevelSheltomXY[ i ][ j ].y )
								{
									bAlreadySet = TRUE;
									break;
								}
							}
						}

						//Continue if already Sheltom present in this slot
						if( bAlreadySet )
							continue;

						//Place Item
						InvenItem[ SelectInvenItemIndex - 1 ].SetX = AgingLevelSheltomXY[ i ][ j ].x;
						InvenItem[ SelectInvenItemIndex - 1 ].SetY = AgingLevelSheltomXY[ i ][ j ].y;
						InvenItem[ SelectInvenItemIndex - 1 ].ItemPosition = 0;

						if( cCraftItem.LastSetCraftItem( &InvenItem[ SelectInvenItemIndex - 1 ], 1 ) )
						{
							cAging.CheckItem( &InvenItem[ SelectInvenItemIndex - 1 ] );
							InvenItem[ SelectInvenItemIndex - 1 ].Flag = 0;
							ReFormInvenItem();
							ReformCharForm();
							cInvenTory.SetItemToChar();
							cInvenTory.CheckWeight();
							return;
						}
					}
				}
			}
		}
	}*/

	//Colcoar item com botao direito
	/*if( cCraftItem.OpenFlag )
	{
		if( !sCraftItem.CraftItem[ 0 ].Flag )
			return;

		if( strnicmp( InvenItem[ SelectInvenItemIndex - 1 ].LastCategory, "os", 2 ) == 0 )
		{
			//Slots Rows
			for( int i = 0; i < 4; i++ )
			{
				//Slots Columns
				for( int j = 0; j < 3; j++ )
				{
					//Get XY offset of this slot

					//Check if this slot already has an Item
					BOOL bAlreadySet = FALSE;
					for( int k = 1; k < 15; k++ )
					{
						sITEM * psCurrentItem = sCraftItem.CraftItem + k;

						if( psCurrentItem->Flag )
						{
							if( psCurrentItem->x == AgingLevelSheltomXY[ i ][ j ].x && psCurrentItem->y == AgingLevelSheltomXY[ i ][ j ].y )
							{
								bAlreadySet = TRUE;
								break;
							}
						}
					}

					//Continue if already Sheltom present in this slot
					if( bAlreadySet )
						continue;

					//Place Item
					InvenItem[ SelectInvenItemIndex - 1 ].SetX = AgingLevelSheltomXY[ i ][ j ].x;
					InvenItem[ SelectInvenItemIndex - 1 ].SetY = AgingLevelSheltomXY[ i ][ j ].y;
					InvenItem[ SelectInvenItemIndex - 1 ].ItemPosition = 0;

					BOOL bReturnValue = cCraftItem.LastSetCraftItem( &InvenItem[ SelectInvenItemIndex - 1 ], 0 );

					if( bReturnValue )
					{
						InvenItem[ SelectInvenItemIndex - 1 ].Flag = FALSE;
						ReFormInvenItem();
						ReformCharForm();
						return;
					}
				}
			}
		}
	}*/


	if (MouseItem.Flag || sinItemPickUpDelayFlag)
	{
		return;
	}


	int i = 0;
	for (i = 0; i < 3; i++)
	{
		if (sInven[i + 10].Rect.left < pCursorPos.x && sInven[i + 10].Rect.right > pCursorPos.x &&
			sInven[i + 10].Rect.top < pCursorPos.y && sInven[i + 10].Rect.bottom > pCursorPos.y)
		{
			cHelpPet.PetMessage("*Potion_ShortKey", 2);
		}
	}
	for (int j = 0; j < SIN_MAX_HELP_NUMBER; j++)
	{
		if (sSinHelp[j].KindFlag == SIN_HELP_KIND_TELEPORT_SCROLL)
			return;
	}

	if (MouseItem.Flag)
	{
		return;
	}
	int Index = SelectInvenItemIndex - 1;

	// BOTAO DIREITO AGE
	if (cAging.OpenFlag) // Aging window XXSTR
	{
		if (SelectInvenItemIndex)
		{
			int Index = SelectInvenItemIndex - 1;

			sITEM& Item = InvenItem[Index];
			POINT MousePosition = { Item.x + 11, Item.y + 11 };
			extern DWORD SpecialSheltomCode;

			if ((InvenItem[Index].CODE & sinITEM_MASK2) == sinBI1)
			{
				POINT PremiumArea = { 159, 268 + sinInterHeight2 };

				if (cAging.isAgeableItem(&Item))
				{
					if (cAging.isStoneAreaEmpty())
					{
						cInvenTory.PickUpInvenItem(MousePosition.x, MousePosition.y, TRUE);

						MouseItem.x = PremiumArea.x;
						MouseItem.y = PremiumArea.y;

						if (cAging.SetAgingItemAreaCheck(&MouseItem)) {
							cCraftItem.LastSetCraftItem(&MouseItem, 3);
							SpecialSheltomCode = MouseItem.sItemInfo.CODE;
						}
					}
					else
					{
						TitleBox::GetInstance()->SetText("Uma pedra especial j� foi selecionada", 3);
					}
				}
				else
				{
					TitleBox::GetInstance()->SetText("O item n�o pode ser usado no aging", 3);
				}
			}
			else if ((InvenItem[Index].CODE & sinITEM_MASK2) == sinOS1)
			{
				//Verifica as posi��es dos sheltons e pega a mais pr�xima.
				bool isEmpty = false;
				int i = 0;
				for (i = 0; i < 12; i++)
				{
					if (cAging.isSheltomAreaEmpty(&Item, i))
					{
						isEmpty = true;
						break;
					}
				}
				if (isEmpty)
				{
					int PosX = 218 + ((i % 3) * 22);
					int PosY = (193 + sinInterHeight2) + ((i / 3) * 22);

					if (cAging.isAgeableItem(&Item))
					{
						cInvenTory.PickUpInvenItem(MousePosition.x, MousePosition.y, TRUE);

						//Atualiza o mouse.
						MouseItem.x = PosX;
						MouseItem.y = PosY;

						if (cAging.SetAgingItemAreaCheck(&MouseItem))
						{
							cCraftItem.LastSetCraftItem(&MouseItem, 1);
							cAging.CheckItem(&MouseItem);
						}
					}
					else
					{
						TitleBox::GetInstance()->SetText("O item n�o pode ser usado no aging", 3);
					}
				}
				else
				{
					TitleBox::GetInstance()->SetText("Pedra incompat�vel ou janela de aging lotada", 3);
				}
			}
			else if (cAging.isAgeableItem(&Item))
			{
				int Price = ((Item.sItemInfo.ItemAgingNum[0] + 1) * Item.sItemInfo.Price) / 2;
				if (cShop.haBuyMoneyCheck(Price) > 0)
				{
					if (cAging.isItemAreaEmpty())
					{
						cInvenTory.PickUpInvenItem(Item.x + 1, Item.y + 1, TRUE);

						int StartX = 58;
						int StartY = 193 + sinInterHeight2;
						int MaxX = StartX + (3 * 22);
						int MaxY = StartY + (4 * 22);

						MouseItem.SetX = StartX + ((MaxX - StartX) - MouseItem.w) / 2;
						MouseItem.SetY = StartY + ((MaxY - StartY) - MouseItem.h) / 2;

						if (cAging.SetAgingItemAreaCheck(&MouseItem))
						{
							cCraftItem.LastSetCraftItem(&MouseItem, 1);
							cAging.CheckItem(&MouseItem);
						}
					}
					else
					{
						TitleBox::GetInstance()->SetText("J� existe um item na janela de aging!", 3);
					}
				}
				else
				{
					TitleBox::GetInstance()->SetText("Ouro Insuficiente!", 3);
				}
			}
			else
			{
				TitleBox::GetInstance()->SetText("O item n�o pode ser usado no aging!", 3);
			}
		}
	}
	else if (CManufactureGoldWindow::getInstance()->isOpen()) //Craft por GOLD
	{
		if (SelectInvenItemIndex)
		{
			int Index = SelectInvenItemIndex - 1;
			sITEM& Item = InvenItem[Index];
			DWORD itemCode = (Item.CODE & sinITEM_MASK2);
			POINT MousePosition = { Item.x + 11, Item.y + 11 };
			UINewWindow* janelaCraftGold = CManufactureGoldWindow::getInstance()->pWindow1;
			auto pMainFrame = janelaCraftGold->GetElement<UINewItemFrame>(CraftGold_Receita);
			sITEM* pRecipe = pMainFrame->getItem();

			if (itemCode == sinDR1 || itemCode == sinOA2 || itemCode == sinDB1 || itemCode == sinDG1 || itemCode == sinWP1 || itemCode == sinWS2 || itemCode == sinWA1 ||
				itemCode == sinWT1 || itemCode == sinWC1 || itemCode == sinWS1 || itemCode == sinWM1 || itemCode == sinWH1 || itemCode == sinDA1 || itemCode == sinDA2 ||
				itemCode == sinDS1 || itemCode == sinOR1 || itemCode == sinOA1 || itemCode == sinOM1 || itemCode == sinOE1)
			{
				if (!pRecipe->Flag)
				{
					int PosX = 218 + ((i % 3) * 22);
					int PosY = (193 + sinInterHeight2) + ((i / 3) * 22);

					cInvenTory.PickUpInvenItem(MousePosition.x, MousePosition.y, TRUE);

					//Atualiza o mouse.
					MouseItem.x = PosX;
					MouseItem.y = PosY;

					CManufactureGoldWindow::getInstance()->OnPutRecipe(pMainFrame);
					pMainFrame->setItem(&MouseItem);
					sinPlaySound(sCraftItem.CraftItem[0].SoundIndex);
				}
				else
				{
					TitleBox::GetInstance()->SetText("J� existe uma Receita no Craft!", 3);
				}

			}
			else if((InvenItem[Index].CODE & sinITEM_MASK2) == sinOS1) //COLOCAR A PEDRA
			{
				if (pRecipe->Flag)
				{
					int PosX = 218 + ((i % 3) * 22);
					int PosY = (193 + sinInterHeight2) + ((i / 3) * 22);
					int qtdRunasAdd = 0;

					cInvenTory.PickUpInvenItem(MousePosition.x, MousePosition.y, TRUE);

					//Atualiza o mouse.
					MouseItem.x = PosX;
					MouseItem.y = PosY;
					for (size_t i = 0; i < 3; i++)
					{
						auto pItemFrame = janelaCraftGold->GetElement<UINewItemFrame>(CraftGold_Slots + i); //Verifica quantas runas/pedras j� tem
						if (pItemFrame)
						{
							auto pItem = pItemFrame->getItem();
							if (pItem->Flag)
							{	
								if (!lstrcmpi(MouseItem.LastCategory, CManufactureGoldWindow::getInstance()->StoneCode[i]))
								{
									qtdRunasAdd++;
								}
							}
						}
					}

					if (qtdRunasAdd == 3)
					{
						CHATGAMEHANDLE->AddChatBoxTextEx(EChatColor::CHATCOLOR_Notice, "> Craft j� est� completo");
					}
					else
					{
						if (!lstrcmpi(MouseItem.LastCategory, CManufactureGoldWindow::getInstance()->StoneCode[0])
							|| !lstrcmpi(MouseItem.LastCategory, CManufactureGoldWindow::getInstance()->StoneCode[1])
							|| !lstrcmpi(MouseItem.LastCategory, CManufactureGoldWindow::getInstance()->StoneCode[2]))
						{
							//ROTINA DE ADD O SHELTON VAI AQUI
							for (size_t s = 0; s < 3; s++)
							{
								auto janelaDaRuna = janelaCraftGold->GetElement<UINewItemFrame>(CraftGold_Slots + s);
								if (janelaDaRuna)
								{
									janelaDaRuna->setItem(&MouseItem);
									sinPlaySound(sCraftItem.CraftItem[0].SoundIndex);
								}
								
							}
						}
					}
					
				}
				else
				{
					TitleBox::GetInstance()->SetText("Adicione uma Receita primeiro!", 3);
				}
			}
			else
			{
				TitleBox::GetInstance()->SetText("Receita Invalida!", 3);
			}

		}
	}
	else if (CManufactureWindow::getInstance()->isOpen()) //Craft por COIN
	{
		if (SelectInvenItemIndex)
		{
			int Index = SelectInvenItemIndex - 1;
			sITEM& Item = InvenItem[Index];
			DWORD itemCode = (Item.CODE & sinITEM_MASK2);
			POINT MousePosition = { Item.x + 11, Item.y + 11 };
			UINewWindow* janelaCraftGold = CManufactureWindow::getInstance()->pWindow1;
			auto pMainFrame = janelaCraftGold->GetElement<UINewItemFrame>(Craft_Receita);
			sITEM* pRecipe = pMainFrame->getItem();

			if (itemCode == sinDR1 || itemCode == sinOA2 || itemCode == sinDB1 || itemCode == sinDG1 || itemCode == sinWP1 || itemCode == sinWS2 || itemCode == sinWA1 ||
				itemCode == sinWT1 || itemCode == sinWC1 || itemCode == sinWS1 || itemCode == sinWM1 || itemCode == sinWH1 || itemCode == sinDA1 || itemCode == sinDA2 ||
				itemCode == sinDS1 || itemCode == sinOR1 || itemCode == sinOA1 || itemCode == sinOM1 || itemCode == sinOE1)
			{
				if (!pRecipe->Flag)
				{
					int PosX = 218 + ((i % 3) * 22);
					int PosY = (193 + sinInterHeight2) + ((i / 3) * 22);

					cInvenTory.PickUpInvenItem(MousePosition.x, MousePosition.y, TRUE);

					//Atualiza o mouse.
					MouseItem.x = PosX;
					MouseItem.y = PosY;

					CManufactureWindow::getInstance()->OnPutRecipe(pMainFrame);
					pMainFrame->setItem(&MouseItem);
					sinPlaySound(sCraftItem.CraftItem[0].SoundIndex);
				}
				else
				{
					TitleBox::GetInstance()->SetText("J� existe uma Receita no Craft!", 3);
				}

			}
			else if ((InvenItem[Index].CODE & sinITEM_MASK2) == sinOS1) //COLOCAR A PEDRA
			{
				if (pRecipe->Flag)
				{
					int PosX = 218 + ((i % 3) * 22);
					int PosY = (193 + sinInterHeight2) + ((i / 3) * 22);
					int qtdRunasAdd = 0;

					cInvenTory.PickUpInvenItem(MousePosition.x, MousePosition.y, TRUE);

					//Atualiza o mouse.
					MouseItem.x = PosX;
					MouseItem.y = PosY;
					for (size_t i = 0; i < 3; i++)
					{
						auto pItemFrame = janelaCraftGold->GetElement<UINewItemFrame>(Craft_Slots + i); //Verifica quantas runas/pedras j� tem
						if (pItemFrame)
						{
							auto pItem = pItemFrame->getItem();
							if (pItem->Flag)
							{
								if (!lstrcmpi(MouseItem.LastCategory, CManufactureWindow::getInstance()->StoneCode[i]))
								{
									qtdRunasAdd++;
								}
							}
						}
					}

					if (qtdRunasAdd == 3)
					{
						CHATGAMEHANDLE->AddChatBoxTextEx(EChatColor::CHATCOLOR_Notice, "> Craft j� est� completo");
					}
					else
					{
						if (!lstrcmpi(MouseItem.LastCategory, CManufactureWindow::getInstance()->StoneCode[0])
							|| !lstrcmpi(MouseItem.LastCategory, CManufactureWindow::getInstance()->StoneCode[1])
							|| !lstrcmpi(MouseItem.LastCategory, CManufactureWindow::getInstance()->StoneCode[2]))
						{
							//ROTINA DE ADD O SHELTON VAI AQUI
							for (size_t s = 0; s < 3; s++)
							{
								auto janelaDaRuna = janelaCraftGold->GetElement<UINewItemFrame>(CraftGold_Slots + s);
								if (janelaDaRuna)
								{
									janelaDaRuna->setItem(&MouseItem);
									sinPlaySound(sCraftItem.CraftItem[0].SoundIndex);
								}

							}
						}
					}

				}
				else
				{
					TitleBox::GetInstance()->SetText("Adicione uma Receita primeiro!", 3);
				}
			}
			else
			{
				TitleBox::GetInstance()->SetText("Receita Invalida!", 3);
			}

		}
	}
	else if (cCraftItem.OpenFlag) // Mix window
	{
		if (SelectInvenItemIndex)
		{
			int Index = SelectInvenItemIndex - 1;

			sITEM& Item = InvenItem[Index];
			POINT MousePosition = { Item.x + 11, Item.y + 11 };

			if ((InvenItem[Index].CODE & sinITEM_MASK2) == sinOS1 || (InvenItem[Index].CODE & sinITEM_MASK2) == sinSE1)
			{
				//Verifica as posi��es dos sheltons e pega a mais pr�xima.
				bool isEmpty = false;

				int i = 0;
				for (i = 0; i < 12; i++)
				{
					if (cCraftItem.isSheltomAreaEmpty(&Item, i))
					{
						isEmpty = true;
						break;
					}
				}

				if (isEmpty)
				{
					int PosX = 218 + ((i % 3) * 22);
					int PosY = (193 + sinInterHeight2) + ((i / 3) * 22);

					if (cCraftItem.isMixableItem(&Item))
					{
						cInvenTory.PickUpInvenItem(MousePosition.x, MousePosition.y, TRUE);

						//Atualiza o mouse.
						MouseItem.x = PosX;
						MouseItem.y = PosY;

						if (cCraftItem.SetCraftItemAreaCheck(&MouseItem))
						{
							cCraftItem.LastSetCraftItem(&MouseItem, 0);
						}
					}
					else
					{
						TitleBox::GetInstance()->SetText("Este item n�o pode ser mixado", 3);
					}
				}
				else
				{
					TitleBox::GetInstance()->SetText("Pedra incompat�vel ou espa�o insuficiente.", 3);
				}
			}
			else if (cCraftItem.isMixableItem(&Item))
			{
				int Price = Item.sItemInfo.Price;

				if (cShop.haBuyMoneyCheck(Price) > 0)
				{
					if (cCraftItem.isItemAreaEmpty())
					{
						cInvenTory.PickUpInvenItem(Item.x + 1, Item.y + 1, TRUE);

						int StartX = 58;
						int StartY = 193 + sinInterHeight2;
						int MaxX = StartX + (3 * 22);
						int MaxY = StartY + (4 * 22);

						MouseItem.SetX = StartX + ((MaxX - StartX) - MouseItem.w) / 2;
						MouseItem.SetY = StartY + ((MaxY - StartY) - MouseItem.h) / 2;

						if (cCraftItem.SetCraftItemAreaCheck(&MouseItem))
						{
							cCraftItem.LastSetCraftItem(&MouseItem, 0);
						}
					}
					else
					{
						TitleBox::GetInstance()->SetText("J� existe um item na Janela de Mix", 3);
					}
				}
				else
				{
					TitleBox::GetInstance()->SetText("Ouro Insuficiente", 3);
				}
			}
			else
			{
				cMessageBox.ShowMessage(MESSAGE_NOT_CRAFT_ITEM);
			}
		}
	}
	else if (cMixtureReset.OpenFlag) {
		if (SelectInvenItemIndex) {
			sITEM& Item = InvenItem[Index];
			POINT MousePosition = { Item.x + 11, Item.y + 11 };

			if (cMixtureReset.isItemOkay(&Item)) {
				if (cMixtureReset.isEmpty(0)) {
					cInvenTory.PickUpInvenItem(MousePosition.x, MousePosition.y, TRUE);

					int StartX = 58;
					int StartY = 193 + sinInterHeight2;
					int MaxX = StartX + (3 * 22);
					int MaxY = StartY + (4 * 22);

					MouseItem.SetX = StartX + ((MaxX - StartX) - MouseItem.w) / 2;
					MouseItem.SetY = StartY + ((MaxY - StartY) - MouseItem.h) / 2;

					//Atualizar o mouse.
					MouseItem.x = MouseItem.SetX;
					MouseItem.y = MouseItem.SetY;

					if (cMixtureReset.SetMixtureItemResetAreaCheck(&MouseItem))
						cMixtureReset.LastSetMixtureResetItem(&MouseItem, 2);
				}
				else {
					cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
				}
			}
			else if (cMixtureReset.isStoneOkay(&Item)) {
				if (cMixtureReset.isEmpty(1)) {
					if (!cMixtureReset.isEmpty(0)) {
						cInvenTory.PickUpInvenItem(MousePosition.x, MousePosition.y, TRUE);

						int StartX = 218 + 23;
						int StartY = 193 + sinInterHeight2 + 31;
						int MaxX = StartX + (1 * 22);
						int MaxY = StartY + (1 * 22);

						MouseItem.SetX = StartX + ((MaxX - StartX) - MouseItem.w) / 2;
						MouseItem.SetY = StartY + ((MaxY - StartY) - MouseItem.h) / 2;

						//Atualizar o mouse.
						MouseItem.x = MouseItem.SetX;
						MouseItem.y = MouseItem.SetY;

						if (cMixtureReset.SetMixtureItemResetAreaCheck(&MouseItem)) {
							cMixtureReset.LastSetMixtureResetItem(&MouseItem, 1);
							MixtureResetCheckFlag = 1;
						}
					}
					else {
						cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
					}
				}
				else {
					cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
				}
			}
			else {
				cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
			}
		}
		return;
	}

	//Clique direito para enviar itens do invent�rio para a fundi��o
	else if (SmeltingItem.OpenFlag) {
		if (SelectInvenItemIndex) {
			sITEM& Item = InvenItem[Index];
			POINT MousePosition = { Item.x + 11, Item.y + 11 };

			if ((InvenItem[Index].CODE & sinITEM_MASK2) == sinPR1 ||
				(InvenItem[Index].CODE & sinITEM_MASK2) == sinPR2) {
				bool Flag = false;
				for (int i = 0; i < 3; i++) {
					if (SmeltingItem.isAreaEmpty(&Item, i)) {
						POINT Area;
						if (SmeltingItem.GetEmptyArea(&Area)) {
							cInvenTory.PickUpInvenItem(MousePosition.x, MousePosition.y, TRUE);

							//Atualizar o mouse.
							MouseItem.x = Area.x;
							MouseItem.y = Area.y;

							if (SmeltingItem.SetSmeltingItemAreaCheck(&MouseItem))
								SmeltingItem.LastSetSmeltingItem(&MouseItem);

							Flag = true;
							break;
						}
					}
				}

				if (!Flag) {
					cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
				}
			}
			else if ((InvenItem[Index].CODE & sinITEM_MASK2) == sinOS1) {
				POINT SheltomArea = { 218 + 22, 193 + sinInterHeight2 + 30 };
				if (SmeltingItem.isAreaEmpty(&Item, 4)) {
					cInvenTory.PickUpInvenItem(MousePosition.x, MousePosition.y, TRUE);

					//Atualizar o mouse.
					MouseItem.x = SheltomArea.x;
					MouseItem.y = SheltomArea.y;

					if (SmeltingItem.SetSmeltingItemAreaCheck(&MouseItem))
						SmeltingItem.LastSetSmeltingItem(&MouseItem);
				}
			}
			else {
				cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
			}
		}
	}

	//Clique direito para enviar itens do invent�rio para a cria��o
	else if (ManufactureItem.m_OpenFlag) {
		if (SelectInvenItemIndex) {
			sITEM& Item = InvenItem[Index];
			POINT MousePosition = { Item.x + 11, Item.y + 11 };

			if ((InvenItem[Index].CODE & sinITEM_MASK2) == sinWR1 ||
				(InvenItem[Index].CODE & sinITEM_MASK2) == sinDR1) {
				// Tentando colocar receitas.
				if (ManufactureItem.isAreaEmpty(0)) {
					POINT ItemArea = { 58, 193 + sinInterHeight2 };
					cInvenTory.PickUpInvenItem(MousePosition.x, MousePosition.y, TRUE);

					int StartX = 58;
					int StartY = 193 + sinInterHeight2;
					int MaxX = StartX + (3 * 22);
					int MaxY = StartY + (4 * 22);

					MouseItem.SetX = StartX + ((MaxX - StartX) - MouseItem.w) / 2;
					MouseItem.SetY = StartY + ((MaxY - StartY) - MouseItem.h) / 2;

					//Atualizar o mouse.
					MouseItem.x = MouseItem.SetX;
					MouseItem.y = MouseItem.SetY;

					if (ManufactureItem.SetManufactureItemAreaCheck(&MouseItem))
						ManufactureItem.LastSetManufactureItem(&MouseItem);
				}
				else {
					cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
				}
			}
			else if ((InvenItem[Index].CODE & sinITEM_MASK2) == sinPR3 ||
				(InvenItem[Index].CODE & sinITEM_MASK2) == sinPR4) {
				// Tentando colocar runas.
				if (!ManufactureItem.isAreaEmpty(0)) {
					bool Flag = false;
					for (int i = 1; i < 4; i++) {
						if (ManufactureItem.isAreaEmpty(i)) {
							POINT Area;
							if (ManufactureItem.GetEmptyArea(&Item, &Area)) {
								cInvenTory.PickUpInvenItem(MousePosition.x, MousePosition.y, TRUE);

								//Atualizar o mouse.
								MouseItem.x = Area.x;
								MouseItem.y = Area.y;

								if (ManufactureItem.SetManufactureItemAreaCheck(&MouseItem))
									ManufactureItem.LastSetManufactureItem(&MouseItem);
								Flag = true;
								break;
							}
						}
					}
					if (!Flag) {
						cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
					}
				}
				else {
					cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
				}
			}
			else {
				cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
			}
		}
	}

	//Clique direito para enviar itens do invent�rio para a troca
	else if (cTrade.OpenFlag) {
		if (SelectInvenItemIndex) {
			sITEM& Item = InvenItem[Index];
			POINT MousePosition = { Item.x + 11, Item.y + 11 };

			if (cTrade.IsItemOkay(&Item)) {
				POINT Pos = { 0 };
				if (cTrade.GetEmptyArea(&Item, &Pos)) {
					cInvenTory.PickUpInvenItem(MousePosition.x, MousePosition.y, TRUE);

					Item.SetX = Pos.x;
					Item.SetY = Pos.y;

					MouseItem.SetX = Item.SetX;
					MouseItem.SetY = Item.SetY;

					MouseItem.x = Item.SetX + 1;
					MouseItem.y = Item.SetY + 1;

					if (cTrade.SetTradeItemAreaCheck(&MouseItem))
						cTrade.LastSetTradeItem(&MouseItem);
				}
				else {
					cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
				}
			}
			else {
				cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
			}
		}
		return;
	}

	//Clique direito para enviar itens do invent�rio para o armaz�m
	else if (cWareHouse.OpenFlag) {
		if (SelectInvenItemIndex) {
			sITEM& Item = InvenItem[Index];
			POINT MousePosition = { Item.x + 11, Item.y + 11 };

			if (cWareHouse.CanStoreItem(&Item))
			{

				if (Item.Class == ITEM_CLASS_POTION)
				{
					cMessageBox.ShowMessage(MESSAGE_POTION_NOT_SETTING);
					return;
				}

				POINT Pos = { 0 };
				if (cWareHouse.GetEmptyArea(&Item, &Pos)) {
					cInvenTory.PickUpInvenItem(MousePosition.x, MousePosition.y, TRUE);

					Item.SetX = Pos.x;
					Item.SetY = Pos.y;

					MouseItem.SetX = Item.SetX;
					MouseItem.SetY = Item.SetY;

					MouseItem.x = Item.SetX + 1;
					MouseItem.y = Item.SetY + 1;

					if (cWareHouse.SetWareHouseItemAreaCheck(&MouseItem))
						if (!cWareHouse.ChangeWareHouseItem(&MouseItem))
							cWareHouse.LastSetWareHouseItem(&MouseItem);
				}
				else {
					cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
				}
			}
			else {
				cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
			}
		}
		return;
	}

	// Bot�o direito caravana
	else if (Caravana::GetInstance()->OpenFlag)
	{
		if (SelectInvenItemIndex)
		{
			sITEM& Item = InvenItem[Index];
			POINT MousePosition = { Item.x + 11, Item.y + 11 };

			if (Caravana::GetInstance()->CanStoreItem(&Item))
			{
				if (!cInvenTory.CheckSetOk(&Item, 1))
				{
					cMessageBox.ShowMessageEvent("Excede o peso limite.");
					return;
				}

				POINT Pos = { 0 };

				if (Caravana::GetInstance()->GetEmptyArea(&Item, &Pos))
				{
					cInvenTory.PickUpInvenItem(MousePosition.x, MousePosition.y, TRUE);

					Item.SetX = Pos.x;
					Item.SetY = Pos.y;

					MouseItem.SetX = Item.SetX;
					MouseItem.SetY = Item.SetY;

					MouseItem.x = Item.SetX + 1;
					MouseItem.y = Item.SetY + 1;

					if (Caravana::GetInstance()->SetCaravanItemAreaCheck(&MouseItem))
						if (!Caravana::GetInstance()->ChangeCaravanItem(&MouseItem))
							Caravana::GetInstance()->LastSetCaravanItem(&MouseItem);
				}
				else {
					cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
				}
			}
			else {
				cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
			}
		}
		return;
	}

	//Clique direito para enviar itens do invent�rio para a loja pessoal
	else if (cMyShop.OpenFlag)
	{
		if (SelectInvenItemIndex) {
			sITEM& Item = InvenItem[Index];
			POINT MousePosition = { Item.x + 11, Item.y + 11 };

			// verifica se pode ir para shop
			if (cMyShop.CanSellItem(&Item)) {
				// pega item do inventario para o shop.
				CheckInvenItemForm();
				PickUpInvenItem(MousePosition.x, MousePosition.y, 2);
				ReFormInvenItem();

				// se pegou com sucesso
				if (MyShopMouseItem.Flag) {
					POINT Pos = { 0 };
					// pega posicao livre
					if (cMyShop.GetEmptyArea(&Item, &Pos)) {
						MyShopMouseItem.x = Pos.x;
						MyShopMouseItem.y = Pos.y;

						if (!cMyShop.SetMyShopItem(&MyShopMouseItem)) {
							memset(&MyShopMouseItem, 0, sizeof(sITEM));
						}
					}
				}
			}
			else {
				cMessageBox.ShowMessage(MESSAGE_NOT_MYSHOP_ITEM);
			}
		}
	}
	else if (cShop.OpenFlag) // selling shop window xxstr
	{
		if (SelectInvenItemIndex) // check item select 
		{
			int Index = SelectInvenItemIndex - 1;

			//sITEM & pItem = InvenItem[Index];

			sITEM pItem;
			memset(&pItem, 0, sizeof(sITEM));
			pItem = InvenItem[Index];

			cInvenTory.PickUpInvenItem(pItem.x + 1, pItem.y + 1, TRUE);

			if (MouseItem.Flag)
			{
				ThrowInvenItemToField(&MouseItem, 1);
			}

		}
	}
	char szBuff[128] = { 0 };

	if (cWareHouse.OpenFlag || cTrade.OpenFlag || cCraftItem.OpenFlag || cShop.OpenFlag || cMixtureReset.OpenFlag ||
		cMyShop.OpenFlag || cCharShop.OpenFlag || SmeltingItem.OpenFlag || ManufactureItem.m_OpenFlag || Caravana::GetInstance()->OpenFlag)return;
	UsePotion(x, y);

	if (SelectInvenItemIndex)
	{
		cHelpPet.PetMessage("*InvenTory_Item", 2);

		if ((InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE & sinITEM_MASK2) == sinPZ1 || (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE & sinITEM_MASK2) == sinPZ2) {
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.PotionCount <= 1) {
				if (sinChar->Money >= 1000) {
					ChangePuzzle(&InvenItem[SelectInvenItemIndex - 1]);
				}
				else
					cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);
			}
		}

		// Ativa as forces quando clica com o bot�o direito aqui
		if ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK2) == sinFO1)
		{
			if (((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK3) >= sin01) && ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK3) <= sin15))
				UseForceOrb(&InvenItem[SelectInvenItemIndex - 1]);
			else if (((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK3) >= sin21) && ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK3) <= sin32))
				UseMagicForceOrb(&InvenItem[SelectInvenItemIndex - 1]);
			else if (((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK3) >= sin35) && ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK3) <= sin37))
				UseBillingMagicForceOrb(&InvenItem[SelectInvenItemIndex - 1]);
		}


		if ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK2) == sinEC1) {
			UseEcore(&InvenItem[SelectInvenItemIndex - 1]);
		}

		if ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK2) == sinSP1) {
			if (InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin16))
			{
				return;
			}
			if (InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin05) ||
				InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin06) ||
				InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin07) ||
				InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin08) ||
				InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin09) ||
				InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin10))
			{
				sITEM temp;
				temp = InvenItem[SelectInvenItemIndex - 1];
				if (!CheckInvenEmpty(&temp))
				{
					cMessageBox.ShowMessage(MESSAGE_OVER_SPACE); //��������
					return;
				}
				if (sinChar->Weight[0] + 10 >= sinChar->Weight[1])
				{
					cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT); //�����ʰ�
					return;
				}
				bGiftBoxDelay = true;
				if (sinActionPotion()) {
					pUsePotion = &InvenItem[SelectInvenItemIndex - 1]; //����� ������ ����Ѵ�
					sinUsePotionDelayFlag = 1;
				}
			}
			else if (InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin34))  // ����� - ȣ���� ĸ�� ���
			{
				UseCapsuleItem(&InvenItem[SelectInvenItemIndex - 1]);
			}
			else if (InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin15)) // ����� - ���� ������ ���
			{
				if (sinActionPotion()) {
					pUsePotion = &InvenItem[SelectInvenItemIndex - 1];
				}
			}
			else if (InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin26)) // ����� - ���ϴ��� �ҿ��̺�Ʈ // ����� - ������ ������ ���
			{
				return;
			}
			else if (InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin27) || InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin28) ||
				InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin29) || InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin30) ||
				InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin31) || InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin32) ||
				InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin33)) // ����� - ���ĺ� ���� �̺�Ʈ
			{
				return;
			}

			else if (InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin35)) // �庰 - �߷�Ÿ�� ���ݷ� ������ ���
			{
				if (sinActionPotion()) {
					pUsePotion = &InvenItem[SelectInvenItemIndex - 1];
				}
			}

			else if (InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin36)) // �庰 - ĵ������ ĵ������� ���
			{
				if (sinActionPotion()) {
					pUsePotion = &InvenItem[SelectInvenItemIndex - 1];
				}
			}

			else if (InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin37) || InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin38))
			{
				return;
			}

			else if (InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin39))
			{
				return;
			}
			else if (InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin40))
			{
				if (lpCurPlayer->smCharInfo.dwEventTime_T)
				{
					cMessageBox.ShowMessage(MESSAGE_ASATTRIBUTE_USE);
					return;
				}
				UseSoccerBallPotionItem(&InvenItem[SelectInvenItemIndex - 1]);
			}
			else if (InvenItem[SelectInvenItemIndex - 1].CODE == (sinSP1 | sin42))
			{
			}
			else
			{
				if (sinActionPotion()) {
					pUsePotion = &InvenItem[SelectInvenItemIndex - 1];

				}
			}
		}
		if ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK2) == sinGP1) {

			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.Level > sinChar->Level)return;

			extern int limitebuff;

			// Limite de uso dos cristais aqui
			if (limitebuff >= 5) {
				TitleBox::GetInstance()->SetText("Voc� s� pode usar 5 cristais de cada vez!", 3);
				return;
			}

			//Delay do Cristal
			if ((GetTickCount() - dwTickCrystal) <= 5000)
			{
				TitleBox::GetInstance()->SetText("Aguarde 5 segundos para usar outro cristal!", 3);
				return;
			}
			else
				dwTickCrystal = 0;

			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinGP1 | sin10) ||
				InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinGP1 | sin22)) {
				if (!sinChar->ClassClan || cldata.stats[0] != '1') {
					cMessageBox.ShowMessage(MESSAGE_USE_REGULAR_CLAN_ONLY);
					return;
				}
				else {
					if (!sinUseCristalDelayFlag)sinUseCristalDelayFlag = 1;
					else return;
				}
			}
			if (!sinItemPickUpDelayFlag)
			{
				InvenItem[SelectInvenItemIndex - 1].sItemInfo.PotionCount = 100;
				sinThrowItemToFeild(&InvenItem[SelectInvenItemIndex - 1]);
				InvenItem[SelectInvenItemIndex - 1].Flag = 0;
				cInvenTory.CheckWeight();
				ReFormInvenItem();
				chaPremiumitem.haItemUseDelayFlag = true;
				dwTickCrystal = GetTickCount();
			}
		}
		if ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK2) == sinGP2)
		{

			if (chaPremiumitem.haItemUseDelayFlag)
			{
				cMessageBox.ShowMessage(MESSAGE_NOT_DELAYTIME);
				return;
			}

			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.Level > sinChar->Level)return;

			if (!sinItemPickUpDelayFlag)
			{
				InvenItem[SelectInvenItemIndex - 1].sItemInfo.PotionCount = 100;
				sinThrowItemToFeild(&InvenItem[SelectInvenItemIndex - 1]);
				InvenItem[SelectInvenItemIndex - 1].Flag = 0;
				cInvenTory.CheckWeight();
				ReFormInvenItem();
				chaPremiumitem.haItemUseDelayFlag = true;

			}
		}

		for (i = 0; i < 6; i++) {
			if (CSKILL->SearchContiueSkillCODE(dwNotUseItemCODE[i][0])) {
				for (int j = 0; j < 6; j++) {
					if (InvenItem[SelectInvenItemIndex - 1].CODE == dwNotUseItemCODE[j][1] && dwNotUseItemCODE[j][1] != dwNotUseItemCODE[i][1]) {
						if (dwNotUseItemCODE[j][2] == dwNotUseItemCODE[i][2]) {
							cMessageBox.ShowMessage(MESSAGE_ASATTRIBUTE_USE);
							return;
						}
					}
				}
			}
		}

		/////////������ ���� ������ 
		if ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK2) == sinBC1)
		{
			if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->FieldCode != rsCASTLE_FIELD &&
				(InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK3) <= sin16)  // ����� - �ν��� �������� �����ʵ�ܿ��� ����� �� �ִ�.
			{
				return;
			}

			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBC1 | sin05)) {
				return;
			}
			for (i = 0; i < 16; i++) {
				if ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK3) == dwCastlItemCODE[i]) {
					chaSiege.SetCastleItemSkill(i + 1);
					SetItemToChar();
					break;
				}
			}

			// ����� - �ν��� ������
			if (((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK3) >= sin21) && ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK3) <= sin29)) // �庰 - ��ų ������
				CSKILL->UseBoosterItem(&InvenItem[SelectInvenItemIndex - 1]);

			// �庰 - ��ų ������
			if (((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK3) >= sin30))
				CSKILL->UseSkillDelayItem(&InvenItem[SelectInvenItemIndex - 1]);

			if (haCastleSkillUseFlag != 1)return; //������� �������̴�.

			if (!sinItemPickUpDelayFlag) { //�������� ����Ҽ������� �����Ѵ�
				InvenItem[SelectInvenItemIndex - 1].sItemInfo.PotionCount = 200;
				sinThrowItemToFeild(&InvenItem[SelectInvenItemIndex - 1]);
				InvenItem[SelectInvenItemIndex - 1].Flag = 0;
				cInvenTory.CheckWeight();//���Ը� �������ش�
				ReFormInvenItem();
			}

		}
		if ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK2) == sinBI1 || (InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK2) == sinBI2) {
			chaPremiumitem.TelePortCore.ItemIndex = 0;
			//�������� ���Ÿ� �ȵŴ� ������--------------------------------------------------------------------------------
			if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_VILLAGE) {
				/*if( (InvenItem[SelectInvenItemIndex-1].CODE &sinITEM_MASK2 ) == sinBI2 )
					return;*/
				for (int i = 0; i < 3; i++) {
					if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == dwNotUseVillageItemCODE[i])
						return;
				}
			}
			//--------------------------------------------------------------------------------------------------------------

			//Contiue �ʱ�ȭ
			iEventBuyItemKind = 0;
			//��罺�� ���
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin01))
				chaPremiumitem.UsePremiumItem(1);
			//���彺��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin02))
				chaPremiumitem.UsePremiumItem(2);
			//�׸�����
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin03))
				chaPremiumitem.UsePremiumItem(3);
			//��Ȱ�ֹ���
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin04))
				return;
			//���ͳ� ������
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin05))
				chaPremiumitem.UsePremiumItem(5);
			//����Ʋ ����
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin06))
			{
				for (i = 0; i < MAX_CONTINUE_SKILL; i++)			 // pluto ���Ʈ ��ũ�� ������̸� ���´�
				{
					if (ContinueSkill[i].CODE == SCROLL_P_EVASION)
					{
						if (ContinueSkill[i].Flag)
						{
							cMessageBox.ShowMessage(MESSAGE_NO_USE_ITEM);
							return;
						}
					}
				}
				chaPremiumitem.UsePremiumItem(6);
			}
			//���Ʈ ��ũ��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin07))
			{
				for (i = 0; i < MAX_CONTINUE_SKILL; i++)
				{
					if (ContinueSkill[i].CODE == SCROLL_P_CRITICAL)	// pluto ����Ʋ ��ũ�� ������̸� ���´�
					{
						if (ContinueSkill[i].Flag)
						{
							cMessageBox.ShowMessage(MESSAGE_NO_USE_ITEM);
							return;
						}
					}
				}
				chaPremiumitem.UsePremiumItem(7);
			}
			//�ڷ���Ʈ �ھ�
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin08)) {
				chaPremiumitem.TelePortCore.ItemIndex = SelectInvenItemIndex - 1;
				chaPremiumitem.UsePremiumItem(8);
			}
			//��ū�� ����
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin09)) {
				chaPremiumitem.UsePremiumItem(9);
			}

			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin95)) {
				chaPremiumitem.UsePremiumItem(9);
			}
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin96)) {
				chaPremiumitem.UsePremiumItem(9);
			}
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin97)) {
				chaPremiumitem.UsePremiumItem(9);
			}
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin98)) {
				chaPremiumitem.UsePremiumItem(9);
			}

			//����¡ ����
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin10))
				chaPremiumitem.UsePremiumItem(10);
			//���� ����
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin11))
				chaPremiumitem.UsePremiumItem(11);
			//��� ������(1��)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin12))
			{
				if (chaPremiumitem.m_ThirdEyesTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(12);
			}
			//����ġ �� ������(1��)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin13))
			{
				if (chaPremiumitem.m_ExpUpPotionTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(13);
			}
			//��� ������(7��)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin14))
			{
				if (chaPremiumitem.m_ThirdEyesTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(14);
			}
			//����ġ �� ������(7��)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin15))
			{
				if (chaPremiumitem.m_ExpUpPotionTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(15);
			}
			//���ƾƮ ����(A)

			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE >= (sinBI2 | sin69) && InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE <= (sinBI2 | sin83))
				haCastleSkillUseFlag = 1;

			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin16))
				chaPremiumitem.UsePremiumItem(16);
			//���ƾƮ ����(B)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin17))
				chaPremiumitem.UsePremiumItem(17);
			//���ƾƮ ����(C)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin18))
				chaPremiumitem.UsePremiumItem(18);
			// pluto ���Ǹ� Ŀ���� 3�ð�
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin19))
			{
				if (chaPremiumitem.m_VampiricCuspidTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(19);
			}
			// pluto ���Ǹ� Ŀ���� 1��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin20))
			{
				if (chaPremiumitem.m_VampiricCuspidTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(20);
			}
			// pluto ���� ����¡ ���� 3�ð�
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin21))
			{
				if (chaPremiumitem.m_ManaRechargingPTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(21);
			}
			// pluto ���� ����¡ ���� 1��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin22))
			{
				if (chaPremiumitem.m_ManaRechargingPTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(22);
			}
			//xxstr bi123
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin23))
			{
				// �庰 - �Է�â
				/*nName = 1;

				sinChatEnter = 0; //���Ƿ� �۾��� �Ⱥ��̰��Ѵ�
				cInterFace.ChatFlag = 0;*/
				chaPremiumitem.UsePremiumItem(23);
			}


			//����� ��Ű�� ������ (3�ð� )		//�����߰�
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin01))
				chaPremiumitem.UsePremiumItem(24);
			//����� ��Ű�� ������ (1�� )		
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin02))
				chaPremiumitem.UsePremiumItem(25);
			//����� ��Ű�� ������ (7�� )		
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin03))
				chaPremiumitem.UsePremiumItem(26);
			//����� ��Ű�� ������ (30�� )		
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin04))
				chaPremiumitem.UsePremiumItem(27);

			//�ǹ� ��Ű�� ������ (3�ð� )		
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin05))
				chaPremiumitem.UsePremiumItem(28);
			//�ǹ� ��Ű�� ������ (1�� )		
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin06))
				chaPremiumitem.UsePremiumItem(29);
			//�ǹ� ��Ű�� ������ (7�� )		
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin07))
				chaPremiumitem.UsePremiumItem(30);
			//�ǹ� ��Ű�� ������ (30�� )		
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin08))
				chaPremiumitem.UsePremiumItem(31);

			//���  ��Ű�� ������ (3�ð� )		
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin09))
				chaPremiumitem.UsePremiumItem(32);
			//��� ��Ű�� ������ (1�� )		
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin10))
				chaPremiumitem.UsePremiumItem(33);
			//��� ��Ű�� ������ (7�� )		
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin11))
				chaPremiumitem.UsePremiumItem(34);
			//��� ��Ű�� ������ (30�� )		
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin12))
				chaPremiumitem.UsePremiumItem(35);
			//===================== 2�� ���ƾƮ ���� ========= ���� ===== 060526
			//���ƾƮ ����(D)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin13))
				chaPremiumitem.UsePremiumItem(36);
			//���ƾƮ ����(E)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin14))
				chaPremiumitem.UsePremiumItem(37);
			//���ƾƮ ����(F)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin15))
				chaPremiumitem.UsePremiumItem(38);
			//���ƾƮ ����(G)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin16))
				chaPremiumitem.UsePremiumItem(39);
			//���ƾƮ ����(H)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin17))
				chaPremiumitem.UsePremiumItem(40);

			//===================== 3�� ���ƾƮ ���� ========= ���� ===== 060809
			//���ƾƮ ����(I)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin18))
				chaPremiumitem.UsePremiumItem(41);
			//���ƾƮ ����(J)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin19))
				chaPremiumitem.UsePremiumItem(42);
			//���ƾƮ ����(K)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin20))
				chaPremiumitem.UsePremiumItem(43);
			//���ƾƮ ����(L)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin21))
				chaPremiumitem.UsePremiumItem(44);
			//���ƾƮ ����(M)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin22))
				chaPremiumitem.UsePremiumItem(45);
			// novos cabelo xxstr
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin39))
				chaPremiumitem.UsePremiumItem(140);
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin40))
				chaPremiumitem.UsePremiumItem(141);
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin41))
				chaPremiumitem.UsePremiumItem(142);
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin42))
				chaPremiumitem.UsePremiumItem(143);
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin43))
				chaPremiumitem.UsePremiumItem(144);


			if(InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin88))
				haCastleSkillUseFlag = 1;
			if(InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin89))
				haCastleSkillUseFlag = 1;
			if(InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin90))
				haCastleSkillUseFlag = 1;
			if(InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin91))
				haCastleSkillUseFlag = 1;

			// Novos premiuns
			// Lucky Box
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin44))
			{
				chaPremiumitem.UsePremiumItem(147, InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE,
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.Head,
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.dwChkSum);
			}

			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin45))
			{
				chaPremiumitem.UsePremiumItem(148, InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE,
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.Head,
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.dwChkSum);
			}

			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin46))
			{
				chaPremiumitem.UsePremiumItem(149, InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE,
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.Head,
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.dwChkSum);
			}

			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin47))
			{
				chaPremiumitem.UsePremiumItem(150, InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE,
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.Head,
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.dwChkSum);
			}

			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin48))
			{
				chaPremiumitem.UsePremiumItem(151, InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE,
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.Head,
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.dwChkSum);
			}

			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin49))
			{
				chaPremiumitem.UsePremiumItem(152, InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE,
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.Head,
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.dwChkSum);
			}

			// Vale level
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin50))
				chaPremiumitem.UsePremiumItem(153, InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE, InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.Head,
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.dwChkSum);
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin51))
				chaPremiumitem.UsePremiumItem(154, InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE, InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.Head,
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.dwChkSum);
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin86))
				chaPremiumitem.UsePremiumItem(167, InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE, InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.Head,
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.dwChkSum);
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin87))
				chaPremiumitem.UsePremiumItem(168, InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE, InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.Head,
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemHeader.dwChkSum);
			

			//***********************��Ʈ����û ��ġ 50%============================			
			// 1�� 
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin24))
				chaPremiumitem.UsePremiumItem(46);
			//7��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin25))
				chaPremiumitem.UsePremiumItem(47);


			// pluto ����Ʈ ���� ���� 1��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin26))
			{
				chaPremiumitem.UsePremiumItem(48);
			}
			// pluto ����Ʈ ���� ���� 30��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin27))
			{
				chaPremiumitem.UsePremiumItem(49);
			}
			// pluto ���� ���ེ ���� 1��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin28))
			{
				if (chaPremiumitem.m_ManaReducePotiontime > 0) return;

				chaPremiumitem.UsePremiumItem(50);
			}
			// pluto ���� ���ེ ���� 7��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin29))
			{
				if (chaPremiumitem.m_ManaReducePotiontime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(51);
			}
			// pluto ����� ��Ű��2 3�ð�
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin26))
			{
				chaPremiumitem.UsePremiumItem(52);
			}
			// pluto ����� ��Ű��2 1��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin27))
			{
				chaPremiumitem.UsePremiumItem(53);
			}
			// pluto ����� ��Ű��2 7��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin28))
			{
				chaPremiumitem.UsePremiumItem(54);
			}
			// pluto �ǹ� ��Ű��2 3�ð�
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin29))
			{
				chaPremiumitem.UsePremiumItem(55);
			}
			// pluto �ǹ� ��Ű��2 1��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin30))
			{
				chaPremiumitem.UsePremiumItem(56);
			}
			// pluto �ǹ� ��Ű��2 7��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin31))
			{
				chaPremiumitem.UsePremiumItem(57);
			}
			// pluto ��� ��Ű��2 3�ð�
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin32))
			{
				chaPremiumitem.UsePremiumItem(58);
			}
			// pluto ��� ��Ű��2 1��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin33))
			{
				chaPremiumitem.UsePremiumItem(59);
			}
			// pluto ��� ��Ű��2 7��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin34))
			{
				chaPremiumitem.UsePremiumItem(60);
			}
			// pluto ���丮�� ��Ű��2 3�ð�
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin35))
			{
				chaPremiumitem.UsePremiumItem(61);
			}
			// pluto ���丮�� ��Ű��2 1��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin36))
			{
				chaPremiumitem.UsePremiumItem(62);
			}
			// pluto ���丮�� ��Ű��2 7��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin37))
			{
				chaPremiumitem.UsePremiumItem(63);
			}
			// pluto ����Ʈ ���� ����2 1��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin30))
			{
				chaPremiumitem.UsePremiumItem(64);
			}
			// pluto ����Ʈ ���� ����2 30��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin31))
			{
				chaPremiumitem.UsePremiumItem(65);
			}
			// pluto ���丮�� ��Ű�� 3�ð�
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin23))
			{
				chaPremiumitem.UsePremiumItem(66);
			}
			// pluto ���丮�� ��Ű�� 1��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin24))
			{
				chaPremiumitem.UsePremiumItem(67);
			}
			// pluto ���丮�� ��Ű�� 7��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin25))
			{
				chaPremiumitem.UsePremiumItem(68);
			}
			// pluto ��(�ؿ�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin32))
			{
				chaPremiumitem.UsePremiumItem(69);
			}
			// pluto ��(�ؿ�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin33))
			{
				chaPremiumitem.UsePremiumItem(70);
			}
			// pluto ��(�ؿ�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin34))
			{
				chaPremiumitem.UsePremiumItem(71);
			}
			// ����� - ����¡ ������(����)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin36))
			{
				// ���� ����¡ ������ ������(��չ��⵵ �޼� ���Ը� üũ�ϸ� �ȴ�.)
				if (!cInvenTory.InvenItem[sInven[0].ItemIndex - 1].Flag || // ���⸦ �κ��� �������� �������
					!cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.ItemAgingNum[1]) // �޼չ���(����¡�� ���Ⱑ �ƴҰ��)
				{
					cMessageBox.ShowMessage(MESSAGE_HAVE_NOT_AGINGITEM);
					return;
				}
				chaPremiumitem.UsePremiumItem(73);
			}
			// ����� - ����¡ ������(����, ����, ����)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin37))
			{
				// ���� ���� ���� ����¡ ������ ������
				if (!cInvenTory.InvenItem[sInven[1].ItemIndex - 1].Flag || // ���⸦ �κ��� �������� �������
					!cInvenTory.InvenItem[sInven[1].ItemIndex - 1].sItemInfo.ItemAgingNum[1]) // ����¡�� �������� �ƴҰ��
				{
					cMessageBox.ShowMessage(MESSAGE_HAVE_NOT_AGINGITEM);
					return;
				}
				chaPremiumitem.UsePremiumItem(74);
			}
			// ����� - ����¡ ������(����, �κ�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin38))
			{
				// ���� ����¡ ������ ������
				if (!cInvenTory.InvenItem[sInven[2].ItemIndex - 1].Flag || // ���ʸ� �κ��� �������� �������
					!cInvenTory.InvenItem[sInven[2].ItemIndex - 1].sItemInfo.ItemAgingNum[1]) // ����(����¡�� ���Ⱑ �ƴҰ��)
				{
					cMessageBox.ShowMessage(MESSAGE_HAVE_NOT_AGINGITEM);
					return;
				}
				chaPremiumitem.UsePremiumItem(75);
			}
			// ����� - ��ų ������(1��)(��ų ���õ��� �������� �ִ�ġ�� �Ѵ�.)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin39))
			{
				if (CSKILL->CheckMaturedSkill(1) == FALSE)
				{
					// �̹� 1�� ���� ��ų�� ��� ���õ�
					cMessageBox.ShowMessage(MESSAGE_HAVE_NOT_MATURESKILL);
					return;
				}
				chaPremiumitem.UsePremiumItem(76);

			}
			// ����� - ��ų ������(2��)(��ų ���õ��� �������� �ִ�ġ�� �Ѵ�.)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin40))
			{
				if (CSKILL->CheckMaturedSkill(2) == FALSE)
				{
					// �̹� 2�� ���� ��ų�� ��� ���õ�
					cMessageBox.ShowMessage(MESSAGE_HAVE_NOT_MATURESKILL);
					return;
				}
				chaPremiumitem.UsePremiumItem(77);

			}
			// ����� - ��ų ������(3��)(��ų ���õ��� �������� �ִ�ġ�� �Ѵ�.)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin41))
			{
				if (CSKILL->CheckMaturedSkill(3) == 0)
				{
					// �̹� 3�� ���� ��ų�� ��� ���õ�
					cMessageBox.ShowMessage(MESSAGE_HAVE_NOT_MATURESKILL);
					return;
				}
				chaPremiumitem.UsePremiumItem(78);

			}
			// ����� - �̵� ���� ������
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin42))
			{
				// lpPlayInfo->Position.Area==rsCASTLE_FIELD
				// StageField[lpCurPlayer->OnStageField]->State==FIELD_STATE_ROOM || // �����
				if (StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_ROOM || // �����
					StageField[lpCurPlayer->OnStageField]->FieldCode == START_FIELD_NUM || // ��ī����
					StageField[lpCurPlayer->OnStageField]->FieldCode == START_FIELD_MORYON || // �ʶ���
					StageField[lpCurPlayer->OnStageField]->FieldCode == rsCASTLE_FIELD || // ����
					StageField[lpCurPlayer->OnStageField]->FieldCode == rsSOD_FIELD || // ����Ʈ��
					StageField[lpCurPlayer->OnStageField]->FieldCode == 29) // ���� ������
				{
					cMessageBox.ShowMessage(MESSAGE_NO_USE_ITEM);
					return;
				}
				iEventBuyItemKind = 79;
				chaPremiumitem.Using_SelectInvenItemIndex = SelectInvenItemIndex - 1;
				cMessageBox.ShowMessage3(MESSAGE_EVENT_ITEMOK, InvenItem[SelectInvenItemIndex - 1].sItemInfo.ItemName);
			}

			// ����� - ����ġ���� ����(100% 1�� �߰�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin43))
			{
				if (chaPremiumitem.m_ExpUpPotionTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(80);
			}

			// ����� - ����ġ���� ����(100% 7�� �߰�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin44))
			{
				if (chaPremiumitem.m_ExpUpPotionTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(81);
			}

			// ����� - ĳ���� �Ӽ��� ���� �ʱ�ȭ ������(5�� - ��, ���ŷ�, ���, ��ø��, �ǰ�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin45)) // ��
				chaPremiumitem.UsePremiumItem(82);
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin46)) // ���ŷ�
				chaPremiumitem.UsePremiumItem(83);
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin47)) // ���
				chaPremiumitem.UsePremiumItem(84);
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin48)) // ��ø��
				chaPremiumitem.UsePremiumItem(85);
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin49)) // �ǰ�
				chaPremiumitem.UsePremiumItem(86);

			// ����� - ����ġ���� ����(100% 30�� �߰�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin50))
			{
				if (chaPremiumitem.m_ExpUpPotionTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(87);
			}

			// // ����� - �ȴн� ��(30��) �߰�
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin51))
			{
				chaPremiumitem.UsePremiumItem(88);
			}

			// ����� - ���� ���� ���� �߰�
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin60))
			{
				chaPremiumitem.UsePremiumItem(97);
			}

			// ����� - ���� ����¡ ���� �߰�
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin61))
			{
				chaPremiumitem.UsePremiumItem(98);
			}

			// ����� - ���� ����� �� �߰�(�׸�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin52))
			{
				if (sinChar->Level < 10)
				{
					cMessageBox.ShowMessage(MESSAGE_PET_USE_LEVEL);
					return;
				}
				chaPremiumitem.UsePremiumItem(99);
			}
			// ����� - ���� ����� �� �߰�(�ܽý�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin53))
			{
				if (sinChar->Level < 10)
				{
					cMessageBox.ShowMessage(MESSAGE_PET_USE_LEVEL);
					return;
				}
				chaPremiumitem.UsePremiumItem(100);
			}
			// ����� - ���� ����� �� �߰�(�̿�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin54))
			{
				if (sinChar->Level < 10)
				{
					cMessageBox.ShowMessage(MESSAGE_PET_USE_LEVEL);
					return;
				}
				chaPremiumitem.UsePremiumItem(101);
			}
			// ����� - ���� ����� �� �߰�(��Ʈ)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin55))
			{
				if (sinChar->Level < 10)
				{
					cMessageBox.ShowMessage(MESSAGE_PET_USE_LEVEL);
					return;
				}
				chaPremiumitem.UsePremiumItem(102);
			}


			// �庰 - ���� ����� �� �߰�(�׸�) // 1��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin69))
			{
				if (sinChar->Level < 10)
				{
					cMessageBox.ShowMessage(MESSAGE_PET_USE_LEVEL);
					return;
				}
				chaPremiumitem.UsePremiumItem(103);
			}
			// �庰 - ���� ����� �� �߰�(�ܽý�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin70))
			{
				if (sinChar->Level < 10)
				{
					cMessageBox.ShowMessage(MESSAGE_PET_USE_LEVEL);
					return;
				}
				chaPremiumitem.UsePremiumItem(104);
			}
			// �庰 - ���� ����� �� �߰�(�̿�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin71))
			{
				if (sinChar->Level < 10)
				{
					cMessageBox.ShowMessage(MESSAGE_PET_USE_LEVEL);
					return;
				}
				chaPremiumitem.UsePremiumItem(105);
			}
			// �庰 - ���� ����� �� �߰�(��Ʈ)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin72))
			{
				if (sinChar->Level < 10)
				{
					cMessageBox.ShowMessage(MESSAGE_PET_USE_LEVEL);
					return;
				}
				chaPremiumitem.UsePremiumItem(106);
			}

			// �庰 - ����ġ���� ����(100% 1�ð�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin80))
			{
				if (chaPremiumitem.m_ExpUpPotionTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(107);
			}

			// �庰 - �Ǵн���(1�ð�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin77))
			{
				chaPremiumitem.UsePremiumItem(108);
			}

			// �庰 - ���� ����� �� �߰�(�׸�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin65))
			{
				if (sinChar->Level < 10)
				{
					cMessageBox.ShowMessage(MESSAGE_PET_USE_LEVEL);
					return;
				}
				chaPremiumitem.UsePremiumItem(109);
			}
			// �庰 - ���� ����� �� �߰�(�ܽý�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin66))
			{
				if (sinChar->Level < 10)
				{
					cMessageBox.ShowMessage(MESSAGE_PET_USE_LEVEL);
					return;
				}
				chaPremiumitem.UsePremiumItem(110);
			}
			// �庰 - ���� ����� �� �߰�(�̿�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin67))
			{
				if (sinChar->Level < 10)
				{
					cMessageBox.ShowMessage(MESSAGE_PET_USE_LEVEL);
					return;
				}
				chaPremiumitem.UsePremiumItem(111);
			}
			// �庰 - ���� ����� �� �߰�(��Ʈ)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin68))
			{
				if (sinChar->Level < 10)
				{
					cMessageBox.ShowMessage(MESSAGE_PET_USE_LEVEL);
					return;
				}
				chaPremiumitem.UsePremiumItem(112);
			}
			// ����� - ����¡ ������(2��) - �ϸ�
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin62))
			{
				// �ϸ� ����¡ ������ ������
				if (!cInvenTory.InvenItem[sInven[7].ItemIndex - 1].Flag || // �ϸ��� �κ��� �������� �������
					!cInvenTory.InvenItem[sInven[7].ItemIndex - 1].sItemInfo.ItemAgingNum[1]) // ������ �ϸ��� ����¡�� ���� �ƴҰ��
				{
					cMessageBox.ShowMessage(MESSAGE_HAVE_NOT_AGINGITEM);
					return;
				}
				chaPremiumitem.UsePremiumItem(113);
			}
			// ����� - ����¡ ������(2��) - �尩
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin63))
			{
				// �尩 ����¡ ������ ������
				if (!cInvenTory.InvenItem[sInven[8].ItemIndex - 1].Flag || // �尩�� �κ��� �������� �������
					!cInvenTory.InvenItem[sInven[8].ItemIndex - 1].sItemInfo.ItemAgingNum[1]) // // ������ �尩�� ����¡�� ���� �ƴҰ��
				{
					cMessageBox.ShowMessage(MESSAGE_HAVE_NOT_AGINGITEM);
					return;
				}
				chaPremiumitem.UsePremiumItem(114);
			}
			// ����� - ����¡ ������(2��) - ����
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin64))
			{
				// ���� ����¡ ������ ������
				if (!cInvenTory.InvenItem[sInven[9].ItemIndex - 1].Flag || // ������ �κ��� �������� �������
					!cInvenTory.InvenItem[sInven[9].ItemIndex - 1].sItemInfo.ItemAgingNum[1]) // // ������ ������ ����¡�� ���� �ƴҰ��
				{
					cMessageBox.ShowMessage(MESSAGE_HAVE_NOT_AGINGITEM);
					return;
				}
				chaPremiumitem.UsePremiumItem(115);
			}


			// �庰 - �׶��Ƽ ��ũ��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin84))
			{
				chaPremiumitem.UsePremiumItem(116);
			}

			// �庰 - ���� ����� �� �߰�(�׸� 1�ð�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin73))
			{
				if (sinChar->Level < 10)
				{
					cMessageBox.ShowMessage(MESSAGE_PET_USE_LEVEL);
					return;
				}
				chaPremiumitem.UsePremiumItem(117);


			}
			// �庰 - ���� ����� �� �߰�(�ܽý� 1�ð�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin74))
			{
				if (sinChar->Level < 10)
				{
					cMessageBox.ShowMessage(MESSAGE_PET_USE_LEVEL);
					return;
				}
				chaPremiumitem.UsePremiumItem(118);
			}
			// �庰 - ���� ����� �� �߰�(�̿� 1�ð�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin75))
			{
				if (sinChar->Level < 10)
				{
					cMessageBox.ShowMessage(MESSAGE_PET_USE_LEVEL);
					return;
				}
				chaPremiumitem.UsePremiumItem(119);
			}
			// �庰 - ���� ����� �� �߰�(��Ʈ 1�ð�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin76))
			{
				if (sinChar->Level < 10)
				{
					cMessageBox.ShowMessage(MESSAGE_PET_USE_LEVEL);
					return;
				}
				chaPremiumitem.UsePremiumItem(120);
			}

			// �庰 - ��� ������(1�ð�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin78))
			{
				if (chaPremiumitem.m_ThirdEyesTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(121);
			}

			// �庰 - ���� ����¡ ���� 1.5
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin85))
			{
				chaPremiumitem.UsePremiumItem(122);
			}

			// �庰 - ����ġ��������(1�ð�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin79))
			{
				if (chaPremiumitem.m_ExpUpPotionTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(123);
			}

			// �庰 - ���Ǹ� Ŀ����(1�ð�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin81))
			{
				if (chaPremiumitem.m_VampiricCuspidTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(124);
			}

			// �庰 - ���� ����¡ ����(1�ð�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin82))
			{
				if (chaPremiumitem.m_ManaRechargingPTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(125);
			}

			// �庰 - ���� ���ེ(1�ð�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin83))
			{
				if (chaPremiumitem.m_ManaReducePotiontime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(126);
			}

			// �庰 - ���Ǹ� Ŀ���� EX(1�ð�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin86))
			{
				if (chaPremiumitem.m_VampiricCuspidEXTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(127);
			}

			// �庰 - ���Ǹ� Ŀ���� EX(3�ð�)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin87))
			{
				if (chaPremiumitem.m_VampiricCuspidEXTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(128);
			}

			// �庰 - ���Ǹ� Ŀ���� EX(1��)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin88))
			{
				if (chaPremiumitem.m_VampiricCuspidEXTime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(129);
			}
			// ������ - �ͽ��� ���� ����
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin89))
				chaPremiumitem.UsePremiumItem(130);
			// ����� - ������ ����


			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin91))
			{
				if (chaPremiumitem.m_StaminaReducePotiontime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(132);
			}
			// ����� - �ٷ� ���ེ ����(7��)
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin92))
			{
				if (chaPremiumitem.m_StaminaReducePotiontime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(133);
			}
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin93))
			{
				if (chaPremiumitem.m_StaminaReducePotiontime > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(134);
			}
			// ������ - �ʵ� �ھ� 1��
			/*
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin93))
			{
				if (sinChar->Level > 19)
				{
					chaPremiumitem.TelePortCore.ItemIndex = SelectInvenItemIndex - 1;		// �κ��丮������ �������� �ε��� ���
					chaPremiumitem.UsePremiumItem(134);									// ������ ���
				}
				else
					cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_LEVEL);
			} */
			// ������ - �ʵ� �ھ� 7��
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin94))
			{
				if (sinChar->Level > 19)
				{
					chaPremiumitem.TelePortCore.ItemIndex = SelectInvenItemIndex - 1;		// �κ��丮������ �������� �ε��� ���
					chaPremiumitem.UsePremiumItem(135);									// ������ ���
				}
				else
					cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_LEVEL);
			}

			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin95))
				chaPremiumitem.UsePremiumItem(136);

			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin96))
				chaPremiumitem.UsePremiumItem(137);

			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin97))
				chaPremiumitem.UsePremiumItem(138);

			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI1 | sin98))
				chaPremiumitem.UsePremiumItem(139);

			// Caravanas
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin57))
			{
				if (StageField[UNITDATA->OnStageField]->FieldCode == START_FIELD_CASTLE)
				{
					TitleBox::GetInstance()->SetText("N�o pode ser utilizado neste mapa", 3);
					return;
				}

				if (chaPremiumitem.m_CaravanArma > 0 || chaPremiumitem.m_CaravanHopy > 0 || chaPremiumitem.m_CaravanBuma > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(155);
			}

			// Caravanas
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin58))
			{
				if (StageField[UNITDATA->OnStageField]->FieldCode == START_FIELD_CASTLE)
				{
					TitleBox::GetInstance()->SetText("N�o pode ser utilizado neste mapa", 3);
					return;
				}

				if (chaPremiumitem.m_CaravanArma > 0 || chaPremiumitem.m_CaravanHopy > 0 || chaPremiumitem.m_CaravanBuma > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(156);
			}

			// Caravanas
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin59))
			{
				if (StageField[UNITDATA->OnStageField]->FieldCode == START_FIELD_CASTLE)
				{
					TitleBox::GetInstance()->SetText("N�o pode ser utilizado neste mapa", 3);
					return;
				}

				if (chaPremiumitem.m_CaravanArma > 0 || chaPremiumitem.m_CaravanHopy > 0 || chaPremiumitem.m_CaravanBuma > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(157);
			}



			// Caravanas
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin60))
			{
				if (StageField[UNITDATA->OnStageField]->FieldCode == START_FIELD_CASTLE)
				{
					TitleBox::GetInstance()->SetText("N�o pode ser utilizado neste mapa", 3);
					return;
				}

				if (chaPremiumitem.m_CaravanArma > 0 || chaPremiumitem.m_CaravanHopy > 0 || chaPremiumitem.m_CaravanBuma > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(158);
			}

			// Caravanas
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin61))
			{
				if (StageField[UNITDATA->OnStageField]->FieldCode == START_FIELD_CASTLE)
				{
					TitleBox::GetInstance()->SetText("N�o pode ser utilizado neste mapa", 3);
					return;
				}

				if (chaPremiumitem.m_CaravanArma > 0 || chaPremiumitem.m_CaravanHopy > 0 || chaPremiumitem.m_CaravanBuma > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(159);
			}

			// Caravanas
			if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin62))
			{
				if (StageField[UNITDATA->OnStageField]->FieldCode == START_FIELD_CASTLE)
				{
					TitleBox::GetInstance()->SetText("N�o pode ser utilizado neste mapa", 3);
					return;
				}

				if (chaPremiumitem.m_CaravanArma > 0 || chaPremiumitem.m_CaravanHopy > 0 || chaPremiumitem.m_CaravanBuma > 0)
				{
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					return;
				}

				chaPremiumitem.UsePremiumItem(160);
			}


			if (!lpCurPlayer->smCharInfo.dwEventTime_T || lpCurPlayer->smCharInfo.dwEventTime_T < GetPlayTime_T())
			{
				if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin63)) // ovelha
					chaPremiumitem.UsePremiumItem(161);
				if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin64)) // girafa
					chaPremiumitem.UsePremiumItem(162);
				if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin65)) // futebol
					chaPremiumitem.UsePremiumItem(163);
				if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin66)) // cartola
					chaPremiumitem.UsePremiumItem(164);
				if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin67)) // cartola
					chaPremiumitem.UsePremiumItem(165);
				if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin68)) // cartola
					chaPremiumitem.UsePremiumItem(166);
			}

			// Uso das montarias
			if (!chaPremiumitem.m_MountTime)
			{
				if (InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin69) ||
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin70) ||
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin71) ||
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin72) ||
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin73) ||
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin74) ||
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin75) ||
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin76) ||
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin77) ||
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin78) ||
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin79) ||
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin80) ||
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin81) ||
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin82) ||
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin83) ||
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin84) ||
					InvenItem[SelectInvenItemIndex - 1].sItemInfo.CODE == (sinBI2 | sin85))
				{
					haCastleSkillUseFlag = 1;
				}
			}


			// Deleta item do invent�rio depois do uso
			if (haCastleSkillUseFlag != 1) return;

			if (!sinItemPickUpDelayFlag)
			{
				InvenItem[SelectInvenItemIndex - 1].sItemInfo.PotionCount = 300;
				sinThrowItemToFeild(&InvenItem[SelectInvenItemIndex - 1]);
				InvenItem[SelectInvenItemIndex - 1].Flag = 0;
				cInvenTory.CheckWeight();
				ReFormInvenItem();
				SaveGameData();
			}
		}

		int UseFlag2 = 0;

		if ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK2) == sinCH1)
		{
			if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_VILLAGE) return;

			for (int i = 0; i < MAX_CONTINUE_SKILL; i++)
			{
				if (ContinueSkill[i].Flag) {
					if (ContinueSkill[i].CODE == SKILL_VIRTUAL_LIFE)
					{
						return;
					}
				}
			}

			if ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK3) == sin01)
			{
				switch (sinChar->JOB_CODE)
				{
				case JOBCODE_ARCHER:
				case JOBCODE_ATALANTA:
				case JOBCODE_PRIESTESS:
					cMessageBox.ShowMessage(MESSAGE_NOT_USE_FEMALE);
					break;
				case JOBCODE_MECHANICIAN:
				case JOBCODE_FIGHTER:
				case JOBCODE_PIKEMAN:
				case JOBCODE_KNIGHT:
				case JOBCODE_MAGICIAN:
					UseFlag2 = 1;

					break;
				}
			}
			else if ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK3) == sin02)
			{
				switch (sinChar->JOB_CODE)
				{
				case JOBCODE_ARCHER:
				case JOBCODE_ATALANTA:
				case JOBCODE_PRIESTESS:
					cMessageBox.ShowMessage(MESSAGE_NOT_USE_FEMALE);
					break;
				case JOBCODE_MECHANICIAN:
				case JOBCODE_FIGHTER:
				case JOBCODE_PIKEMAN:
				case JOBCODE_KNIGHT:
				case JOBCODE_MAGICIAN:
					UseFlag2 = 1;

					break;
				}
			}
			else if ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK3) == sin03)
			{
				switch (sinChar->JOB_CODE)
				{
				case JOBCODE_MECHANICIAN:
				case JOBCODE_FIGHTER:
				case JOBCODE_PIKEMAN:
				case JOBCODE_KNIGHT:
				case JOBCODE_MAGICIAN:
					cMessageBox.ShowMessage(MESSAGE_NOT_USE_MALE);
					break;
				case JOBCODE_ARCHER:
				case JOBCODE_ATALANTA:
				case JOBCODE_PRIESTESS:
					UseFlag2 = 1;

					break;
				}
			}
			else if ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK3) == sin04)
			{
				switch (sinChar->JOB_CODE)
				{
				case JOBCODE_MECHANICIAN:
				case JOBCODE_FIGHTER:
				case JOBCODE_PIKEMAN:
				case JOBCODE_KNIGHT:
				case JOBCODE_MAGICIAN:
					cMessageBox.ShowMessage(MESSAGE_NOT_USE_MALE);
					break;
				case JOBCODE_ARCHER:
				case JOBCODE_ATALANTA:
				case JOBCODE_PRIESTESS:
					UseFlag2 = 1;

					break;
				}
			}

			else if ((InvenItem[SelectInvenItemIndex - 1].CODE & sinITEM_MASK3) == sin47) 
			{
				switch (sinChar->JOB_CODE) {
				case JOBCODE_ARCHER:
				case JOBCODE_ATALANTA:
				case JOBCODE_PRIESTESS:
					cMessageBox.ShowMessage(MESSAGE_NOT_USE_FEMALE);

					break;
				case JOBCODE_MECHANICIAN:
				case JOBCODE_FIGHTER:
				case JOBCODE_PIKEMAN:
				case JOBCODE_KNIGHT:
				case JOBCODE_MAGICIAN:
					UseFlag2 = 1;
					break;
				}
			}

			if (cItem.GetItemLimitTime(&InvenItem[SelectInvenItemIndex - 1]) == FALSE)
			{
				InvenItem[SelectInvenItemIndex - 1].Flag = 0;
				sinUsePotionDelayFlag = 1;
				UseFlag2 = 0;
				cInvenTory.CheckWeight();
				ReFormInvenItem();
			}

			if (!sinItemPickUpDelayFlag && UseFlag2)
			{
				InvenItem[SelectInvenItemIndex - 1].sItemInfo.PotionCount = 100;
				sinThrowItemToFeild(&InvenItem[SelectInvenItemIndex - 1]);
				InvenItem[SelectInvenItemIndex - 1].Flag = 0;
				cInvenTory.CheckWeight();
				ReFormInvenItem();
			}
		}
	}

	ReFormPotionNum();

}
/*----------------------------------------------------------------------------*
*							   RButtonUp
*-----------------------------------------------------------------------------*/
void cINVENTORY::RButtonUp(int x, int y)
{

}

int bHoldingSpace = false;
float progressHoldingSpace = 0.f;

#include "..\\Shop\\NewShop.h"
#include <Quest/Quest.h>
#include "Montarias\\CMountHandler.h"

void ToggleMountVisible()
{
	lpCurPlayer->pMount->SetVisible(!lpCurPlayer->pMount->IsVisible());
}

/*----------------------------------------------------------------------------*
*							    KeyDown
*-----------------------------------------------------------------------------*/
void cINVENTORY::KeyDown()
{
	sITEM TempPotion;

	memset(&TempPotion, 0, sizeof(sITEM));
	int CountTemp = 0;
	int sArrowPosi = 0;
	char szKeyBuff[3] = { '1','2','3' }; //Ű���� 

	// Deixar invisivel/visivel montaria
	if (sinGetKeyClick(VK_SPACE))
	{
		if (lpCurPlayer->pMount && lpCurPlayer->MotionInfo && lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD)
		{
			if (lpCurPlayer->pMount->CanUse())
			{
				bHoldingSpace = true;
				progressHoldingSpace = 0.f;
			}
		}

		cMessageBox.CloseMessage();

		if (!cCraftItem.OpenFlag && !cAging.OpenFlag && !SmeltingItem.OpenFlag && !ManufactureItem.m_OpenFlag || !cMixtureReset.OpenFlag) //????�?? ??????????? ???????????? ESC?? ??�? // ?????? - ????? ???? � ???
			cInterFace.CheckAllBox(SIN_ALL_CLOSE);
	}

	// DESATIVADO: ITEM PERFEITO 

	if (InvenItem[SelectInvenItemIndex - 1].Flag)
	{
		if (sinGetKeyClick('P'))
		{
			struct Pck
			{
				int size;
				int code;
				int spec;
				BYTE trans[sizeof(sITEM)];
			};

			Pck smPacket;

			ZeroMemory(&smPacket, sizeof(Pck));
			smPacket.code = 0x43550004;
			smPacket.spec = cItem.GetJobCode(InvenItem[SelectInvenItemIndex - 1].sItemInfo.JobCodeMask); // Envia no spec do item
			int len = EecodeCompress((BYTE*)&InvenItem[SelectInvenItemIndex - 1], smPacket.trans, sizeof(sITEM));
			smPacket.size = len + 12;

			smWsockDataServer->Send2((char*)&smPacket, smPacket.size, TRUE);

			bIsPerfect2 = FALSE;
		}
	}

	if (sinGetKeyClick('V')) {
		cHelpPet.PetMessage("*InvenTory", 3);
		if (cInvenTory.OpenFlag)cInvenTory.OpenFlag = SIN_CLOSE;
		else cInvenTory.OpenFlag = SIN_OPEN;
		cInterFace.CheckAllBox(SIN_INVENTORY); //â�� �ϳ��� ���� 
		if (cInvenTory.OpenFlag) {
			if (!sinFireShow) {
				StartMenuFlame(0, 350);
				sinPlaySound(0);
			}
			else
				sinPlaySound(SIN_SOUND_SHOW_INTER);
			sinFireShow = 1;
		}

	}
	// traje xxstr
	if (sinGetKeyClick('T'))
		setCostumer();

	if (sinGetKeyClick('F'))
	{
		if (!QuestWindow::GetInstance()->openFlag)
		{
			smTRANS_COMMAND Packet;
			Packet.code = smTRANSCODE_OPEN_NPC;
			Packet.size = sizeof(smTRANS_COMMAND);
			Packet.WParam = 0;
			Packet.LParam = 0;
			Packet.SParam = 0;

			if (smWsockServer)
				smWsockServer->Send((char*)&Packet, Packet.size, TRUE);
		}
		else
		{
			QuestWindow::GetInstance()->openFlag = FALSE;
		}
	}

	if (sinGetKeyClick('4'))
	{
		if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_ARCHER && lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State != FIELD_STATE_VILLAGE)
		{
			for (int cnt = 0; cnt < MAX_CONTINUE_SKILL; cnt++)
			{
				if (ContinueSkill[cnt].Flag)
				{
					if (ContinueSkill[cnt].CODE == SKILL_EVASION_MASTERY)
					{
						SummonSkillCodeIndex[3] ^= 1;

						if (lpCurPlayer->PlayEvasionMastery)
						{
							lpCurPlayer->PlayEvasionMastery = 0;
						}
						else
						{
							lpCurPlayer->PlayEvasionMastery = 1;
						}

						SetItemToChar();

						break;
					}
				}
			}
		}
	}

	int TempPotionCount = 0;

	for (int i = 0; i < 3; i++) {
		if (sinGetKeyClick(szKeyBuff[i])) {
			if (SelectPotionIndex) {
				if (VRKeyBuff[VK_SHIFT]) {
					if (MouseItem.Flag)break;
					if (MyShopSendButton || cMyShop.OpenFlag)break;
					if (InvenItem[SelectPotionIndex - 1].sItemInfo.PotionCount <= 0) {
						InvenItem[SelectPotionIndex - 1].Flag = 0;
						ReFormInvenItem();
						ReFormPotionNum();
						break;

					}
					if (sInven[i + 10].ItemIndex) {
						if (InvenItem[sInven[i + 10].ItemIndex - 1].CODE == InvenItem[SelectPotionIndex - 1].CODE) {
							TempPotionCount = InvenItem[SelectPotionIndex - 1].sItemInfo.PotionCount;
							for (int p = 0; p < TempPotionCount; p++) {
								if (sinChar->Potion_Space > InvenItem[sInven[i + 10].ItemIndex - 1].sItemInfo.PotionCount) {
									InvenItem[sInven[i + 10].ItemIndex - 1].sItemInfo.PotionCount++;
									InvenItem[SelectPotionIndex - 1].sItemInfo.PotionCount--;
									sinPlaySound(InvenItem[SelectPotionIndex - 1].SoundIndex);
									if (InvenItem[SelectPotionIndex - 1].sItemInfo.PotionCount <= 0) {
										InvenItem[SelectPotionIndex - 1].Flag = 0;
										break;
									}
								}
							}
						}
					}
					else {
						memcpy(&TempPotion, &InvenItem[SelectPotionIndex - 1], sizeof(sITEM));
						TempPotion.SetX = sInven[i + 10].Rect.left + (((sInven[i + 10].Rect.right - sInven[i + 10].Rect.left) - InvenItem[SelectPotionIndex - 1].w) / 2);  //��ǥ ���� 							 
						TempPotion.SetY = sInven[i + 10].Rect.top + (((sInven[i + 10].Rect.bottom - sInven[i + 10].Rect.top) - InvenItem[SelectPotionIndex - 1].h) / 2);

						TempPotion.sItemInfo.PotionCount = 0;
						TempPotion.Flag = 1;
						TempPotion.ItemPosition = i + 10 + 1;
						CountTemp = InvenItem[SelectPotionIndex - 1].sItemInfo.PotionCount;
						for (int f = 0; f < CountTemp; f++) {
							if (sinChar->Potion_Space > TempPotion.sItemInfo.PotionCount) {
								TempPotion.sItemInfo.PotionCount++;
								InvenItem[SelectPotionIndex - 1].sItemInfo.PotionCount--;
								if (InvenItem[SelectPotionIndex - 1].sItemInfo.PotionCount <= 0) {
									InvenItem[SelectPotionIndex - 1].Flag = 0;
									break;
								}

							}

						}
						LastSetInvenItem(&TempPotion, 1);
					}

				}

				else {
					if (VRKeyBuff[VK_SHIFT])break;
					if (cWareHouse.OpenFlag || cTrade.OpenFlag || cCraftItem.OpenFlag || Caravana::GetInstance()->OpenFlag)break;
					if (sInven[i + 10].ItemIndex) {
						if (sinActionPotion()) {
							pUsePotion = &InvenItem[sInven[i + 10].ItemIndex - 1];
							break;
						}
					}
				}

			}
			else {
				if (VRKeyBuff[VK_SHIFT])break;
				if (cWareHouse.OpenFlag || cTrade.OpenFlag || cCraftItem.OpenFlag || Caravana::GetInstance()->OpenFlag)break;
				if (sInven[i + 10].ItemIndex) {
					if (sinActionPotion()) {
						cHelpPet.PetMessage("*Potion_ShortKey", 3 + i);
						pUsePotion = &InvenItem[sInven[i + 10].ItemIndex - 1];
						break;
					}
				}
			}
		}
	}

	if (!cTrade.OpenFlag && !sinUsePotionDelayFlag) {
		if (sinGetKeyClick('E')) {
			if (ArrowState[0] == 0)
			{
				sArrowPosi = 2;
			}
			else
			{
				sArrowPosi = 1;
			}
			sinButtonFlag = sArrowPosi;
			SelectInvenItemIndex = 0;
			ChangeABItem(sArrowPosi);
		}
		if (sinGetKeyClick('W')) {
			if (ArrowState[1] == 0)sArrowPosi = 4;
			else sArrowPosi = 3;
			sinButtonFlag = sArrowPosi;

			sinChangeSetFlag = sArrowPosi;
			SetItemToChar();
		}
	}


}

int cINVENTORY::LoadItemInfo()
{
	int i;

	int TempItemIndex[15] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	int TempItemIndex2[15] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	char szFilePath[256];

	sITEM sinTempItem;
	int TEstFlag = 1;
	int cnt = 0;


	for (i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItemTemp[i].Flag) {
			sinSetSpecialItemCode(&InvenItemTemp[i].sItemInfo);
		}
		if (InvenItem[i].Flag) {
			sinSetSpecialItemCode(&InvenItem[i].sItemInfo);
			for (int j = 0; j < MAX_ITEM; j++) {
				if (InvenItem[i].sItemInfo.CODE == sItem[j].CODE) {
					memcpy(&sinTempItem, &sItem[j], sizeof(sITEM));
					memcpy(&sinTempItem.sItemInfo, &InvenItem[i].sItemInfo, sizeof(sITEMINFO));
					wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[j].ItemFilePath, sItem[j].LastCategory);

					if (!sItem[j].lpTempItem) {
						sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);
						if (!sItem[j].lpTempItem) {
							TEstFlag = 0;

						}
					}
					if ((InvenItem[i].sItemInfo.CODE & sinITEM_MASK2) == sinPZ1 || (InvenItem[i].sItemInfo.CODE & sinITEM_MASK2) == sinPZ2) {
						if (InvenItem[i].sItemInfo.PotionCount <= 1) {
							for (cnt = 0; cnt < MAX_ITEM; cnt++) {
								if (sItem[cnt].sItemInfo.CODE == (sinPZ1 | sin00)) {
									break;
								}
							}
							wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[cnt].ItemFilePath, sItem[cnt].LastCategory);
							sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);
							sinTempItem.w = sItem[cnt].w;
							sinTempItem.h = sItem[cnt].h;
						}
						else {
							wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[j].ItemFilePath, sItem[j].LastCategory);
							sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);
							sinTempItem.w = sItem[j].w;
							sinTempItem.h = sItem[j].h;

						}
					}

					sinTempItem.lpItem = sItem[j].lpTempItem;
					sinTempItem.x = InvenItem[i].x;
					sinTempItem.y = InvenItem[i].y;
					sinTempItem.ItemPosition = InvenItem[i].ItemPosition;
					sinTempItem.Flag = 1;
					memcpy(&InvenItem[i], &sinTempItem, sizeof(sITEM));
					if (InvenItem[i].ItemPosition) {
						sInven[InvenItem[i].ItemPosition - 1].ItemIndex = i + 1;
						sinSetCharItem(InvenItem[i].CODE, InvenItem[i].SetModelPosi, TRUE);
					}
					break;

				}

			}

		}
	}

	for (i = 0; i < 84; i++) {
		if (InvenItemTemp[i].Flag) {
			for (int j = 0; j < MAX_ITEM; j++) {
				if (InvenItemTemp[i].sItemInfo.CODE == sItem[j].CODE) {
					memcpy(&sinTempItem, &sItem[j], sizeof(sITEM));
					memcpy(&sinTempItem.sItemInfo, &InvenItemTemp[i].sItemInfo, sizeof(sITEMINFO));
					wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[j].ItemFilePath, sItem[j].LastCategory);
					if (!sItem[j].lpTempItem) {
						sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);
						if (!sItem[j].lpTempItem) {
							TEstFlag = 0;

						}

					}

					if ((InvenItemTemp[i].sItemInfo.CODE & sinITEM_MASK2) == sinPZ1 || (InvenItemTemp[i].sItemInfo.CODE & sinITEM_MASK2) == sinPZ2) {
						if (InvenItemTemp[i].sItemInfo.PotionCount <= 1) {

							for (cnt = 0; cnt < MAX_ITEM; cnt++) {
								if (sItem[cnt].sItemInfo.CODE == (sinPZ1 | sin00)) {
									break;
								}
							}
							wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[cnt].ItemFilePath, sItem[cnt].LastCategory);
							sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);
							sinTempItem.w = sItem[cnt].w;
							sinTempItem.h = sItem[cnt].h;
						}
						else {
							wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[j].ItemFilePath, sItem[j].LastCategory);
							sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);
							sinTempItem.w = sItem[j].w;
							sinTempItem.h = sItem[j].h;

						}

					}

					sinTempItem.lpItem = sItem[j].lpTempItem;
					sinTempItem.x = InvenItemTemp[i].x;
					sinTempItem.y = InvenItemTemp[i].y;
					sinTempItem.ItemPosition = InvenItemTemp[i].ItemPosition;
					sinTempItem.Flag = 1;
					memcpy(&InvenItemTemp[i], &sinTempItem, sizeof(sITEM));
					break;
				}
			}
		}
	}


	for (i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItemTemp[i].Flag) {
			cItem.GetItemLimitTime(&InvenItemTemp[i]);

			if (InvenItemTemp[i].x < StartX || InvenItemTemp[i].x > 800 || InvenItemTemp[i].y < StartY || InvenItemTemp[i].y > 600) {
				ReSetPotionXY(&InvenItemTemp[i], 1);
			}

			if (InvenItemTemp[i].ItemPosition) {
				if (!TempItemIndex2[InvenItemTemp[i].ItemPosition]) {
					TempItemIndex2[InvenItemTemp[i].ItemPosition] = i + 1;
				}
				else {
					InvenItemTemp[i].Flag = 0;
					InvenItemTemp[TempItemIndex2[InvenItemTemp[i].ItemPosition] - 1].Flag = 0;
				}
			}

			if (InvenItemTemp[i].ItemPosition == 2) {
				if (InvenItemTemp[i].Class == ITEM_CLASS_WEAPON_TWO) {
					break;
				}
			}

			if (!CheckItemForm(&InvenItemTemp[i].sItemInfo)) {
				SendSetHackUser(FALSE);
				InvenItemTemp[i].Flag = 0;
			}

		}
	}
	bool ArmorFlag = false;
	for (i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			cItem.GetItemLimitTime(&InvenItem[i]);
			if (InvenItem[i].x < StartX || InvenItem[i].x > 800 || InvenItem[i].y < StartY || InvenItem[i].y > 600) {
				ReSetPotionXY(&InvenItem[i]);
			}

			if (InvenItem[i].ItemPosition > 0) {
				if (InvenItem[i].ItemPosition >= 15)continue;
				if (InvenItem[i].ItemPosition == 3)
					ArmorFlag = true;

				if (!TempItemIndex[InvenItem[i].ItemPosition]) {
					TempItemIndex[InvenItem[i].ItemPosition] = i + 1;
				}
				else {
					InvenItem[i].Flag = 0;
					InvenItem[TempItemIndex[InvenItem[i].ItemPosition] - 1].Flag = 0;
					if (InvenItem[TempItemIndex[InvenItem[i].ItemPosition] - 1].Class == ITEM_CLASS_WEAPON_TWO) {
						if (InvenItem[TempItemIndex[InvenItem[i].ItemPosition] - 1].ItemPosition == 1) {
							if (InvenItem[TempItemIndex[InvenItem[i].ItemPosition] - 1].CODE == InvenItem[sInven[1].ItemIndex - 1].CODE) {
								InvenItem[sInven[1].ItemIndex - 1].Flag = 0;
								sInven[1].ItemIndex = 0;

							}
						}
						if (InvenItem[TempItemIndex[InvenItem[i].ItemPosition] - 1].ItemPosition == 2) {
							if (InvenItem[TempItemIndex[InvenItem[i].ItemPosition] - 1].CODE == InvenItem[sInven[0].ItemIndex - 1].CODE) {
								InvenItem[sInven[0].ItemIndex - 1].Flag = 0;
								sInven[0].ItemIndex = 0;

							}

						}

					}
					if (InvenItem[TempItemIndex[InvenItem[i].ItemPosition] - 1].ItemPosition) {
						sInven[TempItemIndex[InvenItem[i].ItemPosition] - 1].ItemIndex = 0;
					}

					if (InvenItem[i].SetModelPosi)
						sinSetCharItem(InvenItem[i].CODE, InvenItem[i].SetModelPosi, FALSE);
					if (InvenItem[TempItemIndex[InvenItem[i].ItemPosition] - 1].SetModelPosi)
						sinSetCharItem(InvenItem[TempItemIndex[InvenItem[i].ItemPosition] - 1].CODE, InvenItem[TempItemIndex[InvenItem[i].ItemPosition] - 1].SetModelPosi, FALSE);
					if (InvenItem[i].ItemPosition) {
						sInven[InvenItem[i].ItemPosition - 1].ItemIndex = 0;

					}
					if (InvenItem[i].Class == ITEM_CLASS_WEAPON_TWO) {
						if (InvenItem[i].ItemPosition == 1) {
							if (InvenItem[i].CODE == InvenItem[sInven[1].ItemIndex - 1].CODE) {
								InvenItem[sInven[1].ItemIndex - 1].Flag = 0;
								sInven[1].ItemIndex = 0;

							}
						}
						if (InvenItem[i].ItemPosition == 2) {
							if (InvenItem[i].CODE == InvenItem[sInven[0].ItemIndex - 1].CODE) {
								InvenItem[sInven[0].ItemIndex - 1].Flag = 0;
								sInven[0].ItemIndex = 0;

							}
						}
					}
				}

				if (InvenItem[i].Class == ITEM_CLASS_WEAPON_TWO) {
					if (InvenItem[i].ItemPosition == 1) {
						if (InvenItem[i].CODE != InvenItem[sInven[1].ItemIndex - 1].CODE) {
							InvenItem[sInven[1].ItemIndex - 1].Flag = 0;
							InvenItem[i].Flag = 0;
							sInven[1].ItemIndex = 0;
							sInven[0].ItemIndex = 0;
							if (InvenItem[i].SetModelPosi)
								sinSetCharItem(InvenItem[i].CODE, InvenItem[i].SetModelPosi, FALSE);

						}
					}
					if (InvenItem[i].ItemPosition == 2) {
						if (InvenItem[i].CODE != InvenItem[sInven[0].ItemIndex - 1].CODE) {
							InvenItem[sInven[0].ItemIndex - 1].Flag = 0;
							InvenItem[i].Flag = 0;
							sInven[0].ItemIndex = 0;
							sInven[1].ItemIndex = 0;
							if (InvenItem[i].SetModelPosi)
								sinSetCharItem(InvenItem[i].CODE, InvenItem[i].SetModelPosi, FALSE);

						}
					}

				}


			}


			if (!InvenItem[i].lpItem) {
				cMessageBox.ShowMessage(MESSAGE_NOTEXIT_ITEMIMAGE);
			}
			if (InvenItem[i].ItemPosition == 2) {
				if (InvenItem[i].Class == ITEM_CLASS_WEAPON_TWO)
					continue;

			}

			if (!CheckItemForm(&InvenItem[i].sItemInfo)) {
				SendSetHackUser(FALSE);
				InvenItem[i].Flag = 0;
				if (InvenItem[i].ItemPosition) {
					sInven[InvenItem[i].ItemPosition - 1].ItemIndex = 0;
					sinSetCharItem(InvenItem[i].CODE, InvenItem[i].SetModelPosi, FALSE);

				}
				if (InvenItem[i].Class == ITEM_CLASS_WEAPON_TWO) {
					InvenItem[sInven[1].ItemIndex - 1].Flag = 0;
					sInven[1].ItemIndex = 0;
				}
			}

		}

	}
	if (!ArmorFlag)
		sinSetCharItem(NULL, INVENTORY_POS_ARMOR, FALSE);


	ReformCharForm();
	LoadCharState();
	SearchJobAndSetting();
	ReFormInvenItem();

	CheckWeight();

	CSKILL->SearchUseSkill();
	SetItemToChar();

	ReFormPotionNum();
	PotionCheckNumJumpFlag = 0;
	CheckRequireItem();

	if (MouseItem.Flag) {
		for (int j = 0; j < MAX_ITEM; j++) {
			if (MouseItem.sItemInfo.CODE == sItem[j].CODE) {
				memcpy(&sinTempItem, &sItem[j], sizeof(sITEM));
				memcpy(&sinTempItem.sItemInfo, &MouseItem.sItemInfo, sizeof(sITEMINFO));
				wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[j].ItemFilePath, sItem[j].LastCategory);
				if (!sItem[j].lpTempItem)
					sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);
				sinTempItem.lpItem = sItem[j].lpTempItem;
				sinTempItem.x = MouseItem.x;
				sinTempItem.y = MouseItem.y;
				sinTempItem.ItemPosition = MouseItem.ItemPosition;
				sinTempItem.Flag = 1;
				ReFormInvenItem();
				memcpy(&MouseItem, &sinTempItem, sizeof(sITEM));
				cInvenTory.AutoSetInvenItem(&MouseItem);
				break;

			}

		}

	}
	CheckResetInvenItem();
	CSKILL->InitReformSkillPointForm();
	ReFormMousePotionNum();
	CheckSpecialItemPosi();
	ChangeSpecialItem(2);
	CheckChangeJob_QuestItem();
	CheckPuzzle();
	cHelpPet.CheckPetData();
	cPCBANGPet.CheckPetData();
	ReSetQuestItemPosi();
	return TRUE;
}

int cINVENTORY::ReSetQuestItemPosi()
{

	sITEM* pItem = 0;
	for (int i = 0; i < INVENTORY_MAXITEM * 2; i++) {
		if (i < 100) {
			if (!InvenItem[i].Flag)continue;
			pItem = &InvenItem[i];
		}
		else {
			if (!InvenItemTemp[i - 100].Flag)continue;
			pItem = &InvenItemTemp[i - 100];
		}
		if (pItem->x > 1000 && pItem->x < 2000) {
			pItem->x += 9000;
		}
	}
	return TRUE;
}

int cINVENTORY::SetSpecialItem(sITEM* pItem)
{
	if (pItem->sItemInfo.SpecialItemFlag[1] != ITEM_KIND_SPECIAL)return FALSE;

	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;

	cx = (44 * 6) - pItem->w;
	cy = (44 * 3) - pItem->h;

	int TempWidth = 0;
	if (SpecialItemShowFlag) {
		TempWidth = 0;
	}
	else
		TempWidth = 10000;

	for (cntx = (StartX + TempWidth); cntx <= StartX + TempWidth + cx; cntx += 44) {
		for (cnty = StartY; cnty <= StartY + cy; cnty += 44) {
			flag = 0;
			for (int i = 84; i < INVENTORY_MAXITEM; i++) {
				if (InvenItem[i].Flag && InvenItem[i].sItemInfo.SpecialItemFlag[1] == ITEM_KIND_SPECIAL) {
					if (InvenItem[i].x < TempWidth) {
						rect.left = InvenItem[i].x + TempWidth;
						rect.right = InvenItem[i].x + TempWidth + InvenItem[i].w;
					}
					else {
						rect.left = InvenItem[i].x;
						rect.right = InvenItem[i].x + InvenItem[i].w;
					}
					rect.top = InvenItem[i].y;
					rect.bottom = InvenItem[i].y + InvenItem[i].h;

					if (cntx > rect.left - pItem->w && cntx< rect.right &&
						cnty > rect.top - pItem->h && cnty < rect.bottom)
						flag++;
				}
			}
			if (flag == 0) {
				pItem->SetX = cntx + ((44 - pItem->w) / 2);
				pItem->SetY = cnty + ((44 - pItem->h) / 2);
				pItem->ItemPosition = 0;

				for (int j = 84; j < INVENTORY_MAXITEM; j++) {
					if (InvenItem[j].Flag == 0) {
						CheckInvenItemForm();
						memcpy(&InvenItem[j], pItem, sizeof(sITEM));
						InvenItem[j].x = pItem->SetX;
						InvenItem[j].y = pItem->SetY;
						ReFormInvenItem();
						SetItemToChar();
						sinPlaySound(InvenItem[j].SoundIndex);
						CheckWeight();

						SendCheckItemToServer(&InvenItem[j].sItemInfo);
						CheckDamage();
						ReFormPotionNum();
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

extern bool bPlaceItem;
int cINVENTORY::AutoSetInvenItem(sITEM* pItem, int PostBox)
{
	if (sinChar->ChangeJob >= 1) {
		if (pItem->CODE == (sinQT1 | sin01)) // fix force shelton create QT101 xxstr
			return TRUE;
	}
	/*if (pItem->CODE == (sinQT1 | sin01)) // removed block item intentory QT101 xxstr
		return TRUE;*/

	char szBuff[128];
	if (SetSpecialItem(pItem))return TRUE;

	ChangeSpecialItem(2);

	if (!PostBox) {
		if (!CheckSetOk(pItem, 1))
			return FALSE;
	}

	if (PostBox == 2) {
		pItem->SetX = StartX + (22 * 8);
		pItem->SetY = StartY;
		if (LastSetInvenItem(pItem, 1)) {
			wsprintf(szBuff, "%s %s", pItem->sItemInfo.ItemName, RecvItemTT);
			cMessageBox.ShowMessageEvent(szBuff);
			return TRUE;
		}
	}
	if (PostBox == 3) {
		wsprintf(szBuff, "%s %s", pItem->sItemInfo.ItemName, RecvItemTT);
		cMessageBox.ShowMessageEvent(szBuff);
	}

	if (AutoSetPotion(pItem))
		return TRUE;

	for (int i = 0; i < INVENTORY_MAX_POS; i++)
	{
		if (pItem->Class & sInven[i].Position)
		{
			/*if( isItemCostume )//costume xxstr
				break;*/

			if (!sInven[i].ItemIndex)
			{
				if (sInven[i].ItemIndex)
					break;

				if (pItem->Class == ITEM_CLASS_WEAPON_TWO)
					if (sInven[0].ItemIndex || sInven[1].ItemIndex)
						break;

				if (pItem->sItemInfo.NotUseFlag)
					break;

				pItem->SetX = sInven[i].Rect.left + (((sInven[i].Rect.right - sInven[i].Rect.left) - pItem->w) / 2);
				pItem->SetY = sInven[i].Rect.top + (((sInven[i].Rect.bottom - sInven[i].Rect.top) - pItem->h) / 2);
				pItem->ItemPosition = i + 1;

				if (LastSetInvenItem(pItem, 1))
					return TRUE;
			}
		}
	}

	if (InvenEmptyAearCheck(pItem))
		return TRUE;

	cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
	sinOverWeightCnt++;

	return FALSE;
}

//�κ��丮������ ����� üũ�Ѵ� 
int cINVENTORY::InvenEmptyAearCheck(sITEM* pItem)
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;

	if (AutoSetPotionToBox(pItem))return TRUE;

	cx = (22 * INVENTORY_BOX_ROW) - pItem->w; //�������� �����ִ� ���� �� X��ǥ 
	cy = (22 * INVENTORY_BOX_COL) - pItem->h; //�������� �����ִ� ���� �� Y��ǥ

	//�ڵ尡 ���� �ʰų� �������� ������� ���� ���� �����Ҷ� �˻��ؼ� ��ġ�� ã�Ƴ��� 
	for (cntx = StartX; cntx <= StartX + cx; cntx += 22) {
		for (cnty = StartY; cnty <= StartY + cy; cnty += 22) {
			flag = 0;
			for (int i = 0; i < INVENTORY_MAXITEM; i++) {
				if (InvenItem[i].Flag) {
					rect.left = InvenItem[i].x;
					rect.right = InvenItem[i].x + InvenItem[i].w;
					rect.top = InvenItem[i].y;
					rect.bottom = InvenItem[i].y + InvenItem[i].h;

					if (cntx > rect.left - pItem->w && cntx< rect.right &&
						cnty > rect.top - pItem->h && cnty < rect.bottom)
						flag++;
				}

			}
			if (flag == 0) {
				pItem->SetX = cntx;
				pItem->SetY = cnty;
				pItem->ItemPosition = 0;
				LastSetInvenItem(pItem, 1); //��������ϰ�쿡�� ����1�� �ѱ�� 
				return TRUE;
			}
		}
	}
	return FALSE;
}

extern void Utils_Log(DWORD type, char* msg, ...);

//�κ��丮 ���������� �����Ѵ� 
int cINVENTORY::LastSetInvenItem(sITEM* pItem, int AutoSet)
{
	int PassFlag = 0;
	if (!AutoSet)
		if (!InvenNotOpenCheck(pItem))
			return FALSE;

	for (int j = 0; j < INVENTORY_MAXITEM; j++) {
		if (InvenItem[j].Flag == 0) {

			if (sinPosionItem)
			{
				if (sinPosionItem->sItemInfo.ItemHeader.Head == InvenItem[j].sItemInfo.ItemHeader.Head)
					continue;
			}

			CheckInvenItemForm();

			memcpy(&InvenItem[j], pItem, sizeof(sITEM));

			InvenItem[j].x = pItem->SetX;
			InvenItem[j].y = pItem->SetY;


			if ((!CostumeFlag) && (bPlaceItem) && (((InvenItem[j].CODE & sinITEM_MASK2) == sinCA1) || ((InvenItem[j].CODE & sinITEM_MASK2) == sinCA2)))
			{
				CopyMemory(&InvenCostume, &InvenItem[j], sizeof(sITEM));

				char szFileCostume[128] = { 0 };
				wsprintf(szFileCostume, "Image\\sinImage\\Items\\%s\\it%s.bmp", InvenCostume.ItemFilePath, InvenCostume.LastCategory);
				InvenCostume.lpItem = LoadDibSurfaceOffscreen(szFileCostume);

				CostumeFlag = true;

				InvenCostume.x = 649;
				InvenCostume.y = 445;

				sinSetCharItem(InvenCostume.CODE, InvenCostume.SetModelPosi, TRUE); //����� ������ �����Ѵ� 

				SetItemToChar();
				SaveGameData();

				pItem->Flag = 0;
				ReFormInvenItem();
				ReformCharForm();
				SetItemToChar();
				sinPlaySound(InvenItem[j].SoundIndex);
				CheckWeight();

				ZeroMemory(&InvenItem[j], sizeof(sITEM));

				return TRUE;
			}
			else
			{
				if (InvenItem[j].ItemPosition)
				{
					SetTwoHandItem(&InvenItem[j]);
					sInven[InvenItem[j].ItemPosition - 1].ItemIndex = j + 1;
					sinSetCharItem(InvenItem[j].CODE, pItem->SetModelPosi, TRUE);

				}


				pItem->Flag = 0;
				ReFormInvenItem();
				SetItemToChar();
				sinPlaySound(InvenItem[j].SoundIndex);
				CheckWeight();

				if (InvenItem[j].x < StartX || InvenItem[j].x > 800 || InvenItem[j].y < StartY || InvenItem[j].y > 600)
				{
					AutoSetInvenItem(&InvenItem[j]);
				}

				if (pItem->sItemInfo.CODE & sinITEM_MASK2 != sinCA1)
					SendCheckItemToServer(&InvenItem[j].sItemInfo);
			}

			CheckDamage();
			ReFormPotionNum();

			CheckPuzzle();
			return TRUE;

		}
	}
	ReFormInvenItem();
	return FALSE;
}
//////////////////////////////////
//         *���� 
//////////////////////////////////
int cINVENTORY::UsePotion(int x, int y)
{   //������ ����Ѵ� {
	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (InvenItem[i].x < x && InvenItem[i].x + InvenItem[i].w > x &&
				InvenItem[i].y < y && InvenItem[i].y + InvenItem[i].h > y) {
				if (InvenItem[i].ItemPosition == 11 || InvenItem[i].ItemPosition == 12 || InvenItem[i].ItemPosition == 13) {
					if (InvenItem[i].Class == ITEM_CLASS_POTION) { //�����ϰ��� ����Ѵ� 
						if (sinActionPotion()) { //������ ����ϴ� ����� ���Ѵ�  ����� ������ ������ ����Ѵ� 
							pUsePotion = &InvenItem[i]; //����� ������ ����Ѵ� 
							sinUsePotionDelayFlag = 1;
						}

					}

				}
				else {
					if (!OpenFlag)break;
					if (InvenItem[i].Class == ITEM_CLASS_POTION) { //�����ϰ��� ����Ѵ� 
						if (sinActionPotion()) { //������ ����ϴ� ����� ���Ѵ�  ����� ������ ������ ����Ѵ� 
							pUsePotion = &InvenItem[i]; //����� ������ ����Ѵ� 
							sinUsePotionDelayFlag = 1;
						}
					}
				}
			}
		}
	}
	ReFormPotionNum();	//���� ������ üũ�Ѵ�
	return FALSE;

}

int cINVENTORY::ReSettingPotion()
{ //���� ������ ������ ��� ������ �����Ѵ� 
	sITEM TempPotionItem;
	int i = 0, j = 0, cnt = 0;

	memset(&TempPotionItem, 0, sizeof(sITEM));
	for (i = 10; i < 13; i++) { //���� ���ùڽ��� �˻��Ѵ� 
		if (sInven[i].ItemIndex) {
			if (InvenItem[sInven[i].ItemIndex - 1].Flag) {
				memcpy(&TempPotionItem, &InvenItem[sInven[i].ItemIndex - 1], sizeof(sITEM));
				cnt = TempPotionItem.sItemInfo.PotionCount;
				if (sinChar->Potion_Space < InvenItem[sInven[i].ItemIndex - 1].sItemInfo.PotionCount) { //���� �Ҵ� �������� ������ �������� 
					TempPotionItem.sItemInfo.PotionCount = InvenItem[sInven[i].ItemIndex - 1].sItemInfo.PotionCount - sinChar->Potion_Space;
					InvenItem[sInven[i].ItemIndex - 1].sItemInfo.PotionCount -= TempPotionItem.sItemInfo.PotionCount;
					if (!InvenEmptyAearCheck(&TempPotionItem)) {
						NoSettingThrowItem(&TempPotionItem);// ������ �ȵɶ��� �ٴڿ� �������� ������ 
					}

				}
			}

		}

	}
	ReFormPotionNum();	//���� ������ üũ�Ѵ�

	return TRUE;
}
int BackUpX, BackUpY, BackUpPosi = 0;
int cINVENTORY::MouseSetPotion(sITEM* pItem)
{   //������ ���콺�� �����Ѵ� 

	int i = 0, cnt;
	sITEM TempPotionItem;
	sinPosionItem = 0; //�ʱ�ȭ 

	if (!pItem->Flag)return FALSE;
	BackUpItemIndex = 0;
	if (CrashItemIndex[0]) { //������ �������� ������ 
		if (pMessageItem) {
			BackUpX = pMessageItem->x;
			BackUpY = pMessageItem->y;
			BackUpPosi = pMessageItem->ItemPosition;
			sinPosionItem = &InvenItem[CrashItemIndex[0] - 1];
			BackUpItemIndex = CrashItemIndex[0];

		}
		if (pItem->ItemPosition) { //���ǹ۽����� ��ĥ��쿡 
			memcpy(&TempPotionItem, pItem, sizeof(sITEM));
			if (pItem->CODE == InvenItem[CrashItemIndex[0] - 1].CODE) { //���� �ڵ��ϰ�� 
				cnt = pItem->sItemInfo.PotionCount;
				for (i = 0; i < cnt; i++) {
					if (sinChar->Potion_Space > InvenItem[CrashItemIndex[0] - 1].sItemInfo.PotionCount) {
						InvenItem[CrashItemIndex[0] - 1].sItemInfo.PotionCount++;
						pItem->sItemInfo.PotionCount--;
						if (pItem->sItemInfo.PotionCount <= 0) {
							pItem->Flag = 0; //�ε����� �� ���������� �������� �����ش� 
							sinPlaySound(pItem->SoundIndex);
							ReFormPotionNum();	//���� ������ üũ�Ѵ�
							return TRUE; //�ε����� �������ٰ� ������ ���� 
						}
					}
					else {
						if (InvenEmptyAearCheck(pItem)) { //������ ������ â��� ���� 						
							ReFormPotionNum();	//���� ������ üũ�Ѵ�
							return TRUE;
						}
						else {
							ResetPotion2();
							if (sinThrowItemToFeild(pItem)) {
								ReformCharForm();//������
								cInvenTory.ReFormPotionNum();	//���� ���� üũ
								return TRUE;
							}
							else {
								ReformCharForm();//������
								cInvenTory.ReFormPotionNum();	//���� ���� üũ
								return FALSE;

							}
						}

					}
				}

			}
			else { //�ٸ� �ڵ��� ��� 
				memcpy(&TempItem, &InvenItem[CrashItemIndex[0] - 1], sizeof(sITEM)); //�κ��丮 �������� ������ ���� 
				InvenItem[CrashItemIndex[0] - 1].Flag = 0;
				if (sinPosionItem)
					sinPosionItem->Flag = 0;
				TempPotionItem.sItemInfo.PotionCount = 0; //���� ī��Ʈ �ʱ�ȭ 
				cnt = pItem->sItemInfo.PotionCount;
				for (i = 0; i < cnt; i++) {
					if (sinChar->Potion_Space < pItem->sItemInfo.PotionCount) {
						pItem->sItemInfo.PotionCount--;
						TempPotionItem.sItemInfo.PotionCount++;
					}
					else {
						ReFormInvenItem();
						if (LastSetInvenItem(pItem)) {  //���Ǽ�����ġ�� ���� �� (�������������� ������ ������ 2�� �ǳʶ��� �Ѵ� )
							if (TempPotionItem.sItemInfo.PotionCount > 0) {
								if (InvenEmptyAearCheck(&TempPotionItem)) { //������ ������ â��� ���� 
									memcpy(pItem, &TempItem, sizeof(sITEM));
									pMessageItem = sinPosionItem;
									pMessageItem->x = BackUpX;
									pMessageItem->y = BackUpY;
									pMessageItem->ItemPosition = BackUpPosi;
									sinPosionItem = 0;
									ReFormPotionNum();	//���� ������ üũ�Ѵ�
									return TRUE;
								}
								else {
									//���� ����� ����
									memcpy(pItem, &TempItem, sizeof(sITEM));
									if (sinPosionItem) {
										pMessageItem = sinPosionItem;
										pMessageItem->x = BackUpX;
										pMessageItem->y = BackUpY;
										pMessageItem->ItemPosition = BackUpPosi;
										sinPosionItem = 0;
									}
									sinThrowItemToFeild(&TempPotionItem);
									ReFormPotionNum();	//���� ������ üũ�Ѵ�
									return TRUE;
								}
							}
							memcpy(pItem, &TempItem, sizeof(sITEM));
							if (sinPosionItem) {
								pMessageItem = sinPosionItem;
								pMessageItem->x = BackUpX;
								pMessageItem->y = BackUpY;
								pMessageItem->ItemPosition = BackUpPosi;
								sinPosionItem = 0;
							}
							ReFormPotionNum();	//���� ������ üũ�Ѵ�
							return TRUE;
						}
						else {
							ReFormPotionNum();	//���� ������ üũ�Ѵ�
							return FALSE;
						}

					}

				}

			}

		}
		else { //â���� ��ĥ��쿡 
			if (pItem->CODE == InvenItem[CrashItemIndex[0] - 1].CODE) { //���� �ڵ��ϰ�� 
				/////////////// ������ �����ϴ� ����� ������ؼ� �������Ǵ� 
				if (!cInvenTory.OpenFlag) {
					SendSetHackUser2(1960, 3);
					return TRUE;
				}

				InvenItem[CrashItemIndex[0] - 1].sItemInfo.PotionCount += pItem->sItemInfo.PotionCount;
				pItem->Flag = 0; //���� ī��Ʈ�� �����ش��� �ʱ�ȭ�Ѵ� 
				sinPlaySound(pItem->SoundIndex);
				ReFormPotionNum();	//���� ������ üũ�Ѵ�
				return TRUE;
			}
			else {
				memcpy(&TempItem, &InvenItem[CrashItemIndex[0] - 1], sizeof(sITEM)); //���콺 �������� ������ ���� 
				InvenItem[CrashItemIndex[0] - 1].Flag = 0; //�������Ŀ� �ʱ�ȭ

				TempItem.x = pItem->x;
				TempItem.y = pItem->y;
				ReFormInvenItem();
				LastSetInvenItem(pItem); //���콺 �������� �κ��丮�� ���� 
				memcpy(pItem, &TempItem, sizeof(sITEM)); //������������ ���콺�� ���� 
				if (sinPosionItem) { //�����ϰ�쿡�� �����Ϳ� ���� ���� 
					pMessageItem = sinPosionItem;
					pMessageItem->x = BackUpX;
					pMessageItem->y = BackUpY;
					pMessageItem->ItemPosition = BackUpPosi;
					sinPosionItem = 0;

				}
				ReFormPotionNum();	//���� ������ üũ�Ѵ�
				return TRUE;
			}
		}
	}
	else { //�ƴϸ� 
		if (pItem->ItemPosition) {  //���� �ڽ��̸� 
			memcpy(&TempPotionItem, pItem, sizeof(sITEM));
			TempPotionItem.sItemInfo.PotionCount = 0; //���� ī��Ʈ �ʱ�ȭ 
			cnt = pItem->sItemInfo.PotionCount;
			for (i = 0; i < cnt; i++) {
				if (sinChar->Potion_Space < pItem->sItemInfo.PotionCount) {
					pItem->sItemInfo.PotionCount--;
					TempPotionItem.sItemInfo.PotionCount++;

				}
				else {
					ReFormInvenItem();
					if (LastSetInvenItem(pItem)) {  //���Ǽ�����ġ�� ���� �� 
						if (TempPotionItem.sItemInfo.PotionCount > 0) {
							if (InvenEmptyAearCheck(&TempPotionItem)) { //������ ������ â��� ���� 
								ReFormPotionNum();	//���� ������ üũ�Ѵ�
								return TRUE;
							}
							else {
								//���� ����� ���� 
								sinThrowItemToFeild(&TempPotionItem);//�������� ������
								ReFormPotionNum();	//���� ������ üũ�Ѵ�
								return FALSE;
							}
						}
						ReFormPotionNum();	//���� ������ üũ�Ѵ�
						return TRUE;
					}
					else {
						ReFormPotionNum();	//���� ������ üũ�Ѵ�
						return FALSE;
					}
				}
			}
		}
		else { //â���̸� �׳� �����Ѵ� 
			ReFormInvenItem();
			if (LastSetInvenItem(pItem)) {
				ReFormPotionNum();	//���� ������ üũ�Ѵ�
				return TRUE;
			}

		}

	}
	ReFormPotionNum();	//���� ������ üũ�Ѵ�
	return FALSE;
}

int cINVENTORY::AutoSetPotionToBox(sITEM* pItem)
{ //������ â�� �����Ѵ� 

	if (pItem->Class != ITEM_CLASS_POTION)return FALSE; //������ �ƴϸ� �����Ѵ� 
	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (pItem->CODE == InvenItem[i].CODE) {//���� �����̸�
				if (InvenItem[i].ItemPosition)continue; //���� �ڽ��� ������ continue
				InvenItem[i].sItemInfo.PotionCount += pItem->sItemInfo.PotionCount;
				pItem->Flag = 0;
				sinPlaySound(pItem->SoundIndex);
				///////////////////// ������ �߸����� ��츦 üũ�ؼ� �������ش� 
				if (InvenItem[i].x < 3 || InvenItem[i].y < 10)
					ReSetPotionXY(&InvenItem[i]);
				ReformCharForm();//������
				cInvenTory.ReFormPotionNum();	//���� ���� üũ

				return TRUE;

			}
		}
	}

	return FALSE;
}
int cINVENTORY::AutoSetPotion(sITEM* pItem)
{	//������ �ڵ����� �����Ѵ� 


	int i = 0, j = 0, cnt = 0;
	sITEM TempPotion;


	if (pItem->Class != ITEM_CLASS_POTION)return FALSE; //������ �ƴϸ� �����Ѵ�  
	for (i = 0; i < INVENTORY_MAX_POS; i++) {
		if (pItem->Class & sInven[i].Position) { //������ ���õɼ� �ִ� �����̸� 
			if (sInven[i].ItemIndex) { //���� ���� �ڽ��� �������� ������ �ڵ带 �˻��� �������� ���Ѵ� 
				if (pItem->CODE == InvenItem[sInven[i].ItemIndex - 1].CODE) { //���� �ڵ��� �����ϰ�� 
					if (sinChar->Potion_Space == InvenItem[sInven[i].ItemIndex - 1].sItemInfo.PotionCount)continue; //�ڵ�� ������ ������ �����ϸ� 
					cnt = pItem->sItemInfo.PotionCount;
					for (j = 0; j < cnt; j++) { //������ ������ŭ ������ ���� 
						if (sinChar->Potion_Space > InvenItem[sInven[i].ItemIndex - 1].sItemInfo.PotionCount) { //���� ������������ ������ 
							InvenItem[sInven[i].ItemIndex - 1].sItemInfo.PotionCount++;
							pItem->sItemInfo.PotionCount--;
							sinPlaySound(pItem->SoundIndex);
							if (pItem->sItemInfo.PotionCount <= 0) {
								pItem->Flag = 0;
								ReformCharForm();//������
								cInvenTory.ReFormPotionNum();	//���� ���� üũ
								return TRUE; //������ �ٽ����� ���� 
							}
						}
						else { //���� �������� ��ŭ ũ�Ⱑ ���ԵǸ� 	
							if (InvenEmptyAearCheck(pItem)) { //������ ������ �κ��丮 ������ �־��ش� 
								ReformCharForm();//������
								cInvenTory.ReFormPotionNum();	//���� ���� üũ
								return TRUE;
							}
							else { //�ϴ��� ������ ���� �غ��� 
								ResetPotion2();
								if (sinThrowItemToFeild(pItem)) {
									ReformCharForm();//������
									cInvenTory.ReFormPotionNum();	//���� ���� üũ
									return TRUE;

								}
							}
						}
					}
				}
				else //�ڵ尡 �ٸ��� �ٽ� for������ ���� 
					continue;

			}
			else { //�������� ������ �����Ѵ� 
				pItem->SetX = sInven[i].Rect.left + (((sInven[i].Rect.right - sInven[i].Rect.left) - pItem->w) / 2);
				pItem->SetY = sInven[i].Rect.top + (((sInven[i].Rect.bottom - sInven[i].Rect.top) - pItem->h) / 2);

				cnt = pItem->sItemInfo.PotionCount;
				//pItem->SetX = sInven[i].Rect.left; //��ǥ���� 
				//pItem->SetY = sInven[i].Rect.top;
				pItem->ItemPosition = i + 1;
				memcpy(&TempPotion, pItem, sizeof(sITEM)); //�������� �����Ѵ� (������ ������ ������� 2������ ��������ϱ⶧����)
				TempPotion.sItemInfo.PotionCount = 0; //���� ���� �ʱ�ȭ 
				for (j = 0; j < cnt; j++) {
					if (sinChar->Potion_Space < pItem->sItemInfo.PotionCount) { //���� �������� ũ�� 
						pItem->sItemInfo.PotionCount--;
						TempPotion.sItemInfo.PotionCount++;

					}
					else { //������ �°� ���� 
						if (LastSetInvenItem(pItem, 1)) {//���Ǽ�����ġ�� ���� �� 
							if (TempPotion.sItemInfo.PotionCount > 0) {
								if (InvenEmptyAearCheck(&TempPotion)) { //������ ������ â��� ���� 
									ReformCharForm();//������
									cInvenTory.ReFormPotionNum();	//���� ���� üũ
									return TRUE;
								}
							}
							else {
								ReformCharForm();//������
								cInvenTory.ReFormPotionNum();	//���� ���� üũ
								return TRUE; //�ڵ� ������ �ȵ� ���� ������ġ�� �����ϸ� �������ش� 
							}
						}

					}
				}
			}
		}
	}

	return FALSE;
}
//////////////////////////////////
//         *���ڵ�
//////////////////////////////////
int cINVENTORY::ClearTwoHandPosiAndItem(sITEM* pItem)
{
	if (pItem->Class == ITEM_CLASS_WEAPON_TWO) {
		if (sInven[0].ItemIndex) {  //������ 
			InvenItem[sInven[0].ItemIndex - 1].Flag = 0;
			sInven[0].ItemIndex = 0;
			if (InvenItem[sInven[0].ItemIndex - 1].ItemPosition) {
				if (InvenItem[sInven[0].ItemIndex - 1].SetModelPosi) {
					sinSetCharItem(InvenItem[sInven[0].ItemIndex - 1].CODE, InvenItem[sInven[0].ItemIndex - 1].SetModelPosi, FALSE); //����� ������ �����Ѵ� 
				}

			}

		}
		if (sInven[1].ItemIndex) {
			InvenItem[sInven[1].ItemIndex - 1].Flag = 0;
			sInven[1].ItemIndex = 0;
			if (InvenItem[sInven[1].ItemIndex - 1].ItemPosition) {
				if (InvenItem[sInven[1].ItemIndex - 1].SetModelPosi) {
					sinSetCharItem(InvenItem[sInven[1].ItemIndex - 1].CODE, InvenItem[sInven[1].ItemIndex - 1].SetModelPosi, FALSE); //����� ������ �����Ѵ� 
				}
			}
		}
	}
	return TRUE;
}
int cINVENTORY::SetTwoHandItem(sITEM* pItem)
{ //���� 

	sITEM TempTwoHandItem;

	memset(&TempTwoHandItem, 0, sizeof(sITEM)); //�ʱ�ȭ ���ش� 

	if (pItem->Class == ITEM_CLASS_WEAPON_TWO) { //���ڵ� ���⸦ üũ�Ѵ� 
		if (pItem->ItemPosition == 2) { //�޼��̸� 
			pItem->ItemPosition = 1;   //�������� ������ ���������� �ٲ۴�  ��ǥ�� �����Ѵ� 
			pItem->x = sInven[0].Rect.left + (((sInven[0].Rect.right - sInven[0].Rect.left) - pItem->w) / 2);
			pItem->y = sInven[0].Rect.top + (((sInven[0].Rect.bottom - sInven[0].Rect.top) - pItem->h) / 2);


		}
		TempTwoHandItem.x = sInven[1].Rect.left + (((sInven[1].Rect.right - sInven[1].Rect.left) - pItem->w) / 2);
		TempTwoHandItem.y = sInven[1].Rect.top + (((sInven[1].Rect.bottom - sInven[1].Rect.top) - pItem->h) / 2);
		TempTwoHandItem.w = pItem->w;
		TempTwoHandItem.h = pItem->h;
		TempTwoHandItem.ItemPosition = 2; //�������� ��ġ�� �޼� 
		TempTwoHandItem.Class = pItem->Class;
		TempTwoHandItem.sItemInfo.CODE = pItem->CODE; //�ڵ带 �־��ش� (������Load�� ������ �������ڷ� ����)
		TempTwoHandItem.sItemInfo.Price = pItem->sItemInfo.Price; //������ �־��ش� 
		memcpy(&TempTwoHandItem.lpItem, &pItem->lpItem, sizeof(LPDIRECT3DTEXTURE9));
		TempTwoHandItem.Flag = 1;
		for (int j = 0; j < INVENTORY_MAXITEM; j++) {
			if (InvenItem[j].Flag == 0) {
				memcpy(&InvenItem[j], &TempTwoHandItem, sizeof(sITEM));
				sInven[1].ItemIndex = j + 1; //���õɶ� �κ��������� ������ �ε����� �����Ѵ�
				break;
			}
		}

	}

	return TRUE;
}

int cINVENTORY::PickUpTwoHandItem(sITEM* pItem)
{//���� 
	sITEM TempPickItem;
	memset(&TempPickItem, 0, sizeof(sITEM));

	if (pItem->ItemPosition) { //���� �ڽ����� ������ ��쿡�� 
		if (sInven[0].ItemIndex && sInven[1].ItemIndex) {
			memcpy(&TempPickItem, &InvenItem[sInven[0].ItemIndex - 1], sizeof(sITEM)); //�������� �������� �����Ѵ� 
			InvenItem[sInven[0].ItemIndex - 1].Flag = 0; //������ �������� �����ش� 
			InvenItem[sInven[1].ItemIndex - 1].Flag = 0; //�޼� �������� �����ش� 
			sInven[0].ItemIndex = 0; //������� �ڽ��� ������ �����ǵ� �����ش� 
			sInven[1].ItemIndex = 0; //������� �ڽ��� ������ �����ǵ� �����ش� 
			memcpy(pItem, &TempPickItem, sizeof(sITEM));
			ReFormInvenItem();
			ReformCharForm();//������ 
			ReFormPotionNum();
			return TRUE;
		}
	}
	return FALSE;
}


int cINVENTORY::OverlapTwoHandItem(sITEM* pItem)
{
	AutoSetItemIndex = 0;
	if (pItem->Class == ITEM_CLASS_WEAPON_TWO) {
		if (sInven[1].ItemIndex) {
			if (InvenItem[sInven[1].ItemIndex - 1].Class != ITEM_CLASS_WEAPON_TWO) {
				if (pItem->ItemPosition == 1) {
					if (sInven[1].ItemIndex)
						AutoSetItemIndex = sInven[1].ItemIndex;
				}
				if (pItem->ItemPosition == 2) {
					if (sInven[0].ItemIndex)
						AutoSetItemIndex = sInven[0].ItemIndex;
				}
			}
		}
	}
	return TRUE;

}

int cINVENTORY::OverlapTwoHandSwitch(sITEM* pItem)
{
	if (pItem->Class == ITEM_CLASS_WEAPON_TWO) {
		if (pItem->ItemPosition == 1)
			if (sInven[1].ItemIndex)
				CrashItemIndex[0] = sInven[1].ItemIndex;
		if (pItem->ItemPosition == 2)
			if (sInven[0].ItemIndex)
				CrashItemIndex[0] = sInven[0].ItemIndex;

	}
	return TRUE;
}


int cINVENTORY::InvenNotOpenCheck(sITEM* pItem)
{
	if (!OpenFlag)
		if (pItem->ItemPosition != 11 && pItem->ItemPosition != 12 && pItem->ItemPosition != 13)
			return FALSE;
	return TRUE;
}

int cINVENTORY::PickUpInvenItem(int x, int y, int PickUpFlag)
{
	NotPotionFlag = 0;
	sITEMPRICE sinItemPriceTemp;
	SelectInvenItemIndex = 0;
	SelectPotionIndex = 0;
	int kk = 0;
	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (InvenItem[i].x < x && InvenItem[i].x + InvenItem[i].w > x &&
				InvenItem[i].y < y && InvenItem[i].y + InvenItem[i].h > y) {
				if (!InvenNotOpenCheck(&InvenItem[i]))return FALSE;
				if (PickUpFlag == 1) {

					// costume fix xxstr
					if (x >= sInven[0].Rect.left && x <= sInven[1].Rect.right)
					{
						if (y >= sInven[0].Rect.top && y <= sInven[1].Rect.bottom)
						{
							if (CostumeFlag)
							{
								if (InvenItem[i].Class != ITEM_CLASS_COSTUME)
									continue;
							}
							else
							{
								if (InvenItem[i].Class == ITEM_CLASS_COSTUME)
									continue;
							}
						}
					}

					if (MyShopItemIndex[i])return FALSE;
					if (InvenItem[i].ItemPosition) {
						if (sinChar->Weight[0] > sinChar->Weight[1]) {
							cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
							sinOverWeightCnt++;
							return FALSE;
						}
					}

					if (!InvenItem[i].lpItem) {
						cMessageBox.ShowMessage(MESSAGE_NOTEXIT_ITEMIMAGE);
						return FALSE;
					}

					for (int j = 0; j < SIN_MAX_HELP_NUMBER; j++)
					{
						if (sSinHelp[j].KindFlag == SIN_HELP_KIND_TELEPORT_SCROLL)
							return FALSE;
					}

					if (CursorClass == SIN_CURSOR_REPAIR) {
						sinItemPriceTemp = GetInvenItemPrice(&InvenItem[i]);
						if (RepairInvenItem(&InvenItem[i], sinItemPriceTemp.RepairCost)) {
							sinPlaySound(SIN_SOUND_REPAIR_ITEM);
						}
						return TRUE;
					}
					InvenItem[i].OldX = InvenItem[i].x;
					InvenItem[i].OldY = InvenItem[i].y;
					pMessageItem = &InvenItem[i];
					PotionIndex = i + 1;
					BackUpItemIndex = i + 1;

					if (CursorClass == SIN_CURSOR_SELL) {
						if (!InvenItem[i].sItemInfo.ItemHeader.dwChkSum)return TRUE;

						for (kk = 0; kk < NotSell_Item_CODECnt; kk++) {
							if (NotSell_Item_CODE[kk] == InvenItem[i].sItemInfo.CODE)return TRUE;

						}
						for (kk = 0; kk < NotSell_Item_MASKCnt; kk++) {
							if ((InvenItem[i].sItemInfo.CODE & sinITEM_MASK2) == NotSell_Item_MASK[kk])return TRUE;

						}
						for (kk = 0; kk < NotSell_Item_KINDCnt; kk++) {
							if (NotSell_Item_KIND[kk] == InvenItem[i].sItemInfo.ItemKindCode)return TRUE;

						}

						if (InvenItem[i].Class == ITEM_CLASS_POTION ||
							InvenItem[i].sItemInfo.CODE == (sinGF1 | sin01)) {
							return TRUE;
						}
						else {
							if (!cShop.SellItemToShop(&InvenItem[i]))
								return TRUE;
						}

					}
					else {
						if (InvenItem[i].Class == ITEM_CLASS_WEAPON_TWO) {
							PickUpTwoHandItem(&InvenItem[i]);
						}
						memcpy(&MouseItem, &InvenItem[i], sizeof(sITEM));

					}

					InvenItem[i].Flag = 0;
					if (InvenItem[i].ItemPosition) {

						sinSetCharItem(InvenItem[i].CODE, InvenItem[i].SetModelPosi, FALSE);
						sInven[InvenItem[i].ItemPosition - 1].ItemIndex = 0;

					}
					ReFormInvenItem();  //������ �������ֱ����� üũ�� ���ش� 
					CheckWeight();		//���� üũ 
					SetItemToChar();	//������ ���� 
					sinPlaySound(InvenItem[i].SoundIndex);
					ColorIndex = 0;
					//������ üũ�Ѵ�
					CheckPuzzle();
					cHelpPet.PetMessage("*InvenTory_Item", 1);
					return TRUE;
				}
				//���� ������ ������ ��� 
				else if (PickUpFlag == 2) {
					if (MyShopSendButton)return FALSE;
					if (MyShopItemIndex[i])return FALSE;
					if (InvenItem[i].ItemPosition)return FALSE;
					//����ǰ���� �ȼ�����!
					if (InvenItem[i].sItemInfo.CODE == (sinGF1 | sin01))return FALSE;
					//if(InvenItem[i].sItemInfo.CODE == (sinGF1|sin02))return FALSE;

					// �庰 - ��Ʈ���� �ȼ� ����
					if (InvenItem[i].sItemInfo.CODE == (sinOR2 | sin33))return FALSE;

					// �庰 - ������ ����̴� �ȼ� ����
					if (InvenItem[i].sItemInfo.CODE == (sinOA1 | sin36))return FALSE;

					// �庰 - ĵ������ ��Ʈ�ƹķ��� �ȼ� ����
					if (InvenItem[i].sItemInfo.CODE == (sinOA1 | sin37))return FALSE;

					//��ϵ� ������ �ڵ�� ����ũ ������ �ȼ�����
					for (kk = 0; kk < NotSell_Item_CODECnt; kk++) {
						if (NotSell_Item_CODE[kk] == InvenItem[i].sItemInfo.CODE)return FALSE;

					}
					for (kk = 0; kk < NotSell_Item_MASKCnt; kk++) {
						if ((InvenItem[i].sItemInfo.CODE & sinITEM_MASK2) == NotSell_Item_MASK[kk])return FALSE;

					}
					for (kk = 0; kk < NotSell_Item_KINDCnt; kk++) {
						if (NotSell_Item_KIND[kk] == InvenItem[i].sItemInfo.ItemKindCode)return FALSE;

					}

					memcpy(&MyShopMouseItem, &InvenItem[i], sizeof(sITEM)); //������ ����Ѵ�
					memset(&MyShopMouseItem.sItemInfo, 0, sizeof(sITEMINFO));


					MyShopMouseItem.sItemInfo.PotionCount = InvenItem[i].sItemInfo.PotionCount; //�����ϰ�츦 ������ ī��Ʈ�� ī���Ѵ�
					MyShopMouseItem.sItemInfo.ItemHeader = InvenItem[i].sItemInfo.ItemHeader; //����� ī���Ѵ�
					MyShopMouseItem.sItemInfo.Index = i + 1;				 //������ �ε��� ����
					sinMyShopItemPass = 1; //�̺�Ʈ�� �ѹ� �ǳʶٱ����� �÷�

				}
				else {

					// costume xxstr
					if (x >= sInven[0].Rect.left && x <= sInven[1].Rect.right)
					{
						if (y >= sInven[0].Rect.top && y <= sInven[1].Rect.bottom)
						{
							if (CostumeFlag)
							{
								if (InvenItem[i].Class != ITEM_CLASS_COSTUME)
									continue;
							}
							else
							{
								if (InvenItem[i].Class == ITEM_CLASS_COSTUME)
									continue;
							}
						}
					}

					if (InvenItem[i].Class == ITEM_CLASS_POTION) {
						if (!InvenItem[i].ItemPosition)
							SelectPotionIndex = i + 1;
					}

					//Nao mostrar info do item
					/*if( bCostumeSlot )
					{
						if( ( pCursorPos.x > 572 ) && ( pCursorPos.y > 431 + ( 256 - sinMoveKindInter[ SIN_INVENTORY ] ) ) && ( pCursorPos.x < 792 ) && ( pCursorPos.y < 547 + ( 256 - sinMoveKindInter[ SIN_INVENTORY ] ) ) )
							return FALSE;
					}*/

					SelectInvenItemIndex = i + 1;
					ColorIndex = SELECT_ITEM_COLOR;
					sinShowItemInfoFlag = 2;
					cHelpPet.PetMessage("*InvenTory_Item", 0);

					// Mostrar os status do item Perfeito
					if (bIsPerfect2)
					{
						if (GAMECOREHANDLE->pcItemInfoBox->IsViewingPerfectItem())
							return TRUE;

						GAMECOREHANDLE->pcItemInfoBox->SetCompareItem(&TempPerfectItem2);
						GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&InvenItem[i], FALSE, TRUE, FALSE);
					}
					else
					{
						if (GAMECOREHANDLE->pcItemInfoBox->bCompareItem)
							GAMECOREHANDLE->pcItemInfoBox->ClearCompareItem();

						if (cMyShop.OpenFlag)
							GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&InvenItem[i], FALSE, TRUE, FALSE);
						else
						{
							if (GAMECOREHANDLE->pcItemInfoBox->IsSameItem(&InvenItem[i], FALSE, FALSE) == FALSE)
							{
								GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&InvenItem[i], 2, FALSE, FALSE);
							}
						}
					}
				}
			}
		}
	}
	return FALSE;
}


//���콺�� �������� ������ �������� ���õ� ������ üũ�Ѵ� 
int cINVENTORY::SetInvenItemAreaCheck(sITEM* pItem)
{

	int i, j;
	ColorIndex = 0; //�ʱ�ȭ 
	CrashItemIndex[0] = 0; //�浹�� ������ Index �ʱ�ȭ 
	CrashItemIndex[1] = 0;
	AutoSetItemIndex = 0; //�ڵ����� ���õ� �������� �ε��� 

	///////////���� ���� &���� ���� 
	if (!cInvenTory.OpenFlag)return FALSE; //�κ��丮�� �������� �����ÿ��� �������� �����Ҽ� ����
	for (i = 0; i < INVENTORY_MAX_POS; i++) {

		// costume xxstr
		if (CostumeFlag)
		{
			if (i >= 0 && i <= 2)
				continue;
		}
		else
		{
			if (i == 13)
				continue;
		}

		if (CheckInvenPos(pItem->x + 11, pItem->y + 11, pItem->x + pItem->w - 11, pItem->y + pItem->h - 11, sInven[i].Rect)) {
			ColorRect.left = sInven[i].Rect.left;     //�÷� �ڽ��� RECT
			ColorRect.top = sInven[i].Rect.top;
			ColorRect.right = sInven[i].Rect.right - sInven[i].Rect.left;
			ColorRect.bottom = sInven[i].Rect.bottom - sInven[i].Rect.top;
			pItem->SetX = sInven[i].Rect.left + ((ColorRect.right - pItem->w) / 2);
			pItem->SetY = ColorRect.top + ((ColorRect.bottom - pItem->h) / 2);
			pItem->ItemPosition = i + 1;

			if (!(sInven[i].Position & pItem->Class)) { //Ŭ������ ���� �ʴ´� 
				ColorIndex = NOT_SETTING_COLOR;
				return FALSE;
			}
			if (pItem->sItemInfo.NotUseFlag) { //Ŭ������ ���� �ʰų� ������ �ɼ�������  
				ColorIndex = NOT_USE_COLOR;
				return FALSE;

			}
			/*
			if(CrashItemIndex[0] = CrashInvenItem(ColorRect)){  //��ġ�� �������� �ֳ� üũ
				if(InvenItem[CrashItemIndex[0]-1].sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON &&
					InvenItem[CrashItemIndex[0]-1].sItemInfo.ItemAgingNum[0] < 4){
					ColorIndex = NOT_SETTING_COLOR;
					return FALSE;
				}
			}
			*/

			if (CrashItemIndex[0] = CrashInvenItem(ColorRect)) {  //��ġ�� �������� �ֳ� üũ 
				OverlapTwoHandItem(pItem); //���ڵ� ����� ��ġ�� �� �������� üũ�Ѵ�
				ColorIndex = OVERLAP_BOX_COLOR;
				return TRUE;
			}
			OverlapTwoHandSwitch(pItem); //���ڵ� �������� �ݴ��� ��ġ�� üũ 
			ColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ��� 
			return TRUE;
		}

	}

	///////////�ڽ� ���� 
	for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22) {
		for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22) {
			if (StartX <= i && EndX > i && StartY <= j && EndY > j) { //�κ��丮 �ڽ��� �������� üũ 
				ColorRect.left = StartX + (((i - StartX) / 22) * 22);
				ColorRect.top = StartY + (((j - StartY) / 22) * 22);
				ColorRect.right = pItem->w;
				ColorRect.bottom = pItem->h;
				if (EndX < (ColorRect.left + ColorRect.right) - 11 || EndY < (ColorRect.top + ColorRect.bottom) - 11) {
					InitColorRect(); //�÷� �ڽ� RECT �ʱ�ȭ (�÷� �ڽ� RECT�� �ϳ���� 0 �̸� �����Ҽ� ���� 
					return FALSE;
				}
				pItem->SetX = ColorRect.left;
				pItem->SetY = ColorRect.top;
				pItem->ItemPosition = 0; //�迭�� �ε����� �����Ѵ� 
				ColorIndex = SET_ITEM_CHECK_COLOR; //�����ҿ���


				/*
				if(CrashItemIndex[0] = CrashInvenItem(ColorRect)){  //��ġ�� �������� �ֳ� üũ
					if(InvenItem[CrashItemIndex[0]-1].sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON &&
						InvenItem[CrashItemIndex[0]-1].sItemInfo.ItemAgingNum[0] < 4){
						ColorIndex = NOT_SETTING_COLOR;
						return FALSE;
					}

				}
				*/
				if (CrashItemIndex[0] = CrashInvenItem(ColorRect)) {  //��ġ�� �������� �ֳ� üũ
					/*
					if(!CheckSpecialItem(&InvenItem[CrashItemIndex[0]-1])){ //���� ������ Ư���� �������� üũ
						ColorIndex = NOT_SETTING_COLOR;
						return FALSE;
					}
					*/

					if (CrashItemIndex[1] = CrashInvenItem(ColorRect, CrashItemIndex[0])) { //�ΰ� �̻� ��ġ�� üũ 
						ColorIndex = NOT_SETTING_COLOR;
						CrashItemIndex[1] = 0; //�ʱ�ȭ 
						return FALSE;

					}
					ColorIndex = OVERLAP_ITEM_COLOR;
					return TRUE;
				}
				return TRUE;
			}

		}

	}

	return TRUE;
}

//�÷� �ڽ� RECT �ʱ�ȭ 
int cINVENTORY::InitColorRect()
{
	ColorRect.left = 0;
	ColorRect.top = 0;
	ColorRect.right = 0;
	ColorRect.bottom = 0;
	return TRUE;

}

//������ ������ �浹������ üũ�Ѵ�
int cINVENTORY::CrashInvenItem(RECT& desRect, int PassItemIndex)
{

	RECT rect;

	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (PassItemIndex != 0)
				if (PassItemIndex == i + 1)continue;
			rect.left = InvenItem[i].x;
			rect.right = InvenItem[i].x + InvenItem[i].w;
			rect.top = InvenItem[i].y;
			rect.bottom = InvenItem[i].y + InvenItem[i].h;

			if (desRect.left > rect.left - desRect.right && desRect.left < rect.right &&
				desRect.top > rect.top - desRect.bottom && desRect.top < rect.bottom)
			{
				//costume xxstr
				if (CostumeFlag && InvenItem[i].ItemPosition == 3)
					continue;
				if (!CostumeFlag && InvenItem[i].ItemPosition == 14)
					continue;

				return i + 1;
			}
		}
	}

	return FALSE;

}
//�������� ���� ���� �浹 üũ 
int cINVENTORY::CheckInvenPos(int x, int y, int lx, int ly, RECT& rect)
{
	if (x <= rect.left) {
		if (rect.left >= lx)return FALSE;
	}
	else {
		if (x >= rect.right)return FALSE;
	}

	if (y <= rect.top) {
		if (rect.top >= ly)return FALSE;
	}
	else {
		if (y >= rect.bottom)return FALSE;
	}

	return TRUE;

}



int cINVENTORY::ChangeInvenItem(sITEM* pItem)
{

	int BackUpX = 0, BackUpY = 0, BackUpPosi = 0;
	for (int j = 0; j < SIN_MAX_HELP_NUMBER; j++)				//�ڷ���Ʈ ����â�� �������� ���� ü���� ���´� (���� �߰� )
	{
		if (sSinHelp[j].KindFlag == SIN_HELP_KIND_TELEPORT_SCROLL) {
			return TRUE;
			if (NoSettingThrowItem(&MouseItem)) {
				return FALSE;
			}
		}

	}

	//����Ʈ �������� ���������������� ��������
	/*
	if(CrashItemIndex[0]){
		if(InvenItem[CrashItemIndex[0]-1].sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON &&
			InvenItem[CrashItemIndex[0]-1].sItemInfo.ItemAgingNum[0] < 4){
			cMessageBox.ShowMessage(MESSAGE_NOT_PICKUP_ITEM);
			return FALSE;
		}
	}
	*/

	// pluto ���� ��ų ���� ���´� ������ ���� -_- //�ؿ�
	if (lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_ATTACK || lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_SKILL)
	{
		//if( pCursorPos.x > 575 && pCursorPos.x < 785 && pCursorPos.y > 435 && pCursorPos.y < 540 ) // ���� ���â ��ǥ�� ��~��~
		//if( pCursorPos.x > 515 && pCursorPos.x < 785 && pCursorPos.y > 345 && pCursorPos.y < 600 ) // EPT�������� �ٽ� ���� ���â ��ǥ�� ��~��~
		//�ٽ� ������ �Ʒ����� �������� ��Ƴ�� , CL���׸� ����ϴ���. �׷��� �ƾ� ��ų ����߿��� ������ �����Ƴ�����
		{
			return FALSE;
		}
	}
	if (!InvenNotOpenCheck(pItem))return FALSE; //�κ� �������࿩�θ� üũ�Ѵ� 
	if (CrashItemIndex[0]) {//�浹 �� �������� ������ �������� �ٲ��ش� 
		CheckInvenItemForm();
		if (InvenItem[CrashItemIndex[0] - 1].Class == ITEM_CLASS_POTION) {
			if (pMessageItem) {
				BackUpX = pMessageItem->x;
				BackUpY = pMessageItem->y;
				BackUpPosi = pMessageItem->ItemPosition;
				sinPosionItem = &InvenItem[CrashItemIndex[0] - 1];
				BackUpItemIndex = CrashItemIndex[0];
			}
		}
		if (InvenItem[CrashItemIndex[0] - 1].Class == ITEM_CLASS_WEAPON_TWO) //��չ����ϰ�쿡�� ���ڱ���ü�� ������ �������� �����ϰ� �������� �����ش� 
			PickUpTwoHandItem(&InvenItem[CrashItemIndex[0] - 1]); //��� ����

		memcpy(&TempItem, &InvenItem[CrashItemIndex[0] - 1], sizeof(sITEM)); //���콺 �������� ������ ���� 
		if (InvenItem[CrashItemIndex[0] - 1].ItemPosition) //������ �ִ� �����ϰ�쿡�� ���⸦ ���ش�
			sinSetCharItem(InvenItem[CrashItemIndex[0] - 1].CODE, InvenItem[CrashItemIndex[0] - 1].SetModelPosi, FALSE); //����� ������ �����Ѵ� 			
		InvenItem[CrashItemIndex[0] - 1].Flag = 0; //�������Ŀ� �ʱ�ȭ
		TempItem.x = pItem->x;
		TempItem.y = pItem->y;
		if (AutoSetItemIndex) { //���� ���õ� �������� ������ 
			ReFormInvenItem(); //���� ���õȴ������� üũ���� �����Ѵ� 
			sinSetCharItem(InvenItem[AutoSetItemIndex - 1].CODE, InvenItem[AutoSetItemIndex - 1].SetModelPosi, FALSE); //���и� ���ش� 
			if (!InvenEmptyAearCheck(&InvenItem[AutoSetItemIndex - 1])) {//������ �������� ��������Ѵ� 
				if (AutoSetItemIndex)
					NoSettingThrowItem(&InvenItem[AutoSetItemIndex - 1], 1);//������ �ȵɶ��� ������ 

			}

		}
		ReFormInvenItem();
		LastSetInvenItem(pItem); //���콺 �������� �κ��丮�� ���� 
		memcpy(pItem, &TempItem, sizeof(sITEM));
		if (sinPosionItem) { //�����ϰ�쿡�� �����Ϳ� ���� ���� 
			if (InvenItem[CrashItemIndex[0] - 1].Class == ITEM_CLASS_POTION) {
				pMessageItem = sinPosionItem;
				pMessageItem->x = BackUpX;
				pMessageItem->y = BackUpY;
				pMessageItem->ItemPosition = BackUpPosi;
				sinPosionItem = 0;
			}

		}
		ReFormInvenItem();
		return TRUE;

	}

	return FALSE;
}

//������ �ȵɶ����� �ٴڿ� �������� ������ 
int cINVENTORY::NoSettingThrowItem(sITEM* pItem, int Flag)
{
	//��ġ�� ���� ��� ���Ƶд� 
	if (!pItem)return FALSE;
	if (!pItem->Flag)return FALSE;
	if (!Flag) { //�������� �������� �ٴۿ� ������ (������������ ���¿����� ���� �� Ʈ���̵� â���� ��������쿡�� ����������) // pluto ����
		if (cTrade.OpenFlag || cWareHouse.OpenFlag || cCraftItem.OpenFlag || cAging.OpenFlag || SmeltingItem.OpenFlag || ManufactureItem.m_OpenFlag || cMixtureReset.OpenFlag || Caravana::GetInstance()->OpenFlag)return FALSE; //Ʈ���̵峪 â�� �������� �������� ������ �ʴ´� // ������ - �ͽ��� ���� â �߰�
	}
	if (!pItem->sItemInfo.ItemHeader.Head || pItem->sItemInfo.ItemHeader.Head < 0)return FALSE;
	if (pItem->sItemInfo.CODE == (sinGF1 | sin01))return FALSE;
	//if(pItem->sItemInfo.CODE == ( sinGF1|sin02))return FALSE;

	int kk = 0;
	//��ϵ� �������� ���������پ������� ���������� 
	for (kk = 0; kk < NotDrow_Item_CODECnt; kk++) {
		if (NotDrow_Item_CODE[kk] == pItem->sItemInfo.CODE)return FALSE;

	}
	for (kk = 0; kk < NotDrow_Item_MASKCnt; kk++) {
		if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == NotDrow_Item_MASK[kk])return FALSE;

	}
	for (kk = 0; kk < NotDrow_Item_KINDCnt; kk++) {
		if (NotDrow_Item_KIND[kk] == pItem->sItemInfo.ItemKindCode)return FALSE;

	}

	if (sinThrowItemToFeild(pItem)) {
		sinPlaySound(pItem->SoundIndex);
		pItem->Flag = 0;
	}
	else {
		pItem->Flag = 0; //������ ���ص� ������ �ȵǹǷ� ���������� 

	}

	return TRUE;
}

//�������� ������ 
int cINVENTORY::ThrowInvenItemToField(sITEM* pItem, int SellRight)
{
	///////////�̺�Ʈ �������� ������ ����  �ȼ������� 
	//if((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinGP1)return TRUE;
	//if((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinBI2)return TRUE;							//��Ű��ĳ���������� ������ ����.
	if (cTrade.OpenFlag || cWareHouse.OpenFlag || cCraftItem.OpenFlag || cAging.OpenFlag || SmeltingItem.OpenFlag || CRAFTWINDOW->isOpen() || CRAFTGOLDWINDOW->isOpen() || ManufactureItem.m_OpenFlag || cMixtureReset.OpenFlag || Caravana::GetInstance()->OpenFlag)		//Ʈ���̵峪 â�� �������� �������� ������ �ʴ´�  // ������ - �ͽ��� ����
		return TRUE;

	if (sMessageBox3[MESSAGE_TELEPORT_CORE].Flag)return TRUE;		// �ڷ���Ʈ ����â�� �������� �������� ������ ���� (�����߰�)
	for (int i = 0; i < SIN_MAX_HELP_NUMBER; i++)
	{
		if (sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT_SCROLL)
		{
			return TRUE;
		}

	}

	int kk = 0;
	//��ϵ� �������� ���������پ������� ���������� 
	for (kk = 0; kk < NotDrow_Item_CODECnt; kk++) {
		if (NotDrow_Item_CODE[kk] == pItem->sItemInfo.CODE)return TRUE;

	}
	for (kk = 0; kk < NotDrow_Item_MASKCnt; kk++) {
		if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == NotDrow_Item_MASK[kk])return TRUE;

	}
	for (kk = 0; kk < NotDrow_Item_KINDCnt; kk++) {
		if (NotDrow_Item_KIND[kk] == pItem->sItemInfo.ItemKindCode)return TRUE;

	}


	if (cCraftItem.OpenFlag) {
		if (0 < pCursorPos.x && 256 + 128 > pCursorPos.x &&
			130 < pCursorPos.y && 130 + (22 * 10) > pCursorPos.y) {
			return TRUE;

		}

	}
	if (pItem->sItemInfo.CODE == (sinGF1 | sin01))return TRUE; //��ǰ���� ����������
//	if(pItem->sItemInfo.CODE == (sinGF1|sin02))return TRUE; //��ǰ���� ����������


	if (cShop.OpenFlag) {
		//		if ( 18+(22*10)  < pCursorPos.x  && 18+(22*10)+100  > pCursorPos.x && 
		//			 134 < pCursorPos.y  && 134+(22*10) > pCursorPos.y ){
		//			return TRUE;

		//		}
				// vende item botao direito xxstr
		if (SellRight)
		{
			if (cShop.CheckHighRankItem(pItem)) {
				CursorClass = 1; //Ŀ���� �������ش�
				pItem->Flag = 0; //Ŀ�������� ���콺 �������� ��� �ʱ�ȭ��
				ResetInvenItemCode(); //���콺 �÷��� ���ٶ� �ٽ� üũ���ش�
				cMessageBox.ShowMessage3(MESSAGE_SELL_HIGHRANK_ITEM, pItem->sItemInfo.ItemName);

			}
			else cShop.SellItemToShop(pItem);
		}

		int kk = 0;
		if (18 < pCursorPos.x && 18 + (22 * 10) > pCursorPos.x &&
			134 + sinInterHeight2 < pCursorPos.y && 134 + (22 * 10) + sinInterHeight2 > pCursorPos.y) {
			//��ϵ� ������ �ڵ�� ����ũ ������ �ȼ�����
			for (kk = 0; kk < NotSell_Item_CODECnt; kk++) {
				if (NotSell_Item_CODE[kk] == pItem->sItemInfo.CODE)return TRUE;

			}
			for (kk = 0; kk < NotSell_Item_MASKCnt; kk++) {
				if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == NotSell_Item_MASK[kk])return TRUE;

			}
			for (kk = 0; kk < NotSell_Item_KINDCnt; kk++) {
				if (NotSell_Item_KIND[kk] == pItem->sItemInfo.ItemKindCode)return TRUE;

			}

			if (pItem->Class == ITEM_CLASS_POTION) {
				return TRUE; //���ǰ� ��ǰ���� ���� �ʴ´� 
			}

			/*
			if(pItem->sItemInfo.PotionCount == 1){ //������ �Ѱ��� ��쿡�� �׳� �Ǵ�
				cShop.SellItemToShop(pItem);
				return TRUE;

			}
			if(pMessageItem){ //�κ��丮���� ���� �������ϰ�쿡��
				cMessageBox.ShowMessage2(MESSAGE_SELL_ITEM);
				pItem->Flag=0; //���콺 ������ �ʱ�ȭ
				pMessageItem->Flag = 1; //�������� ������ �����ص״� �����ͷ� �÷��� �츰��
				sInven[pMessageItem->ItemPosition-1].ItemIndex = BackUpItemIndex;
				CheckOverlapItem(pMessageItem,BackUpItemIndex);
				ReSettingPotion();      //���ǰ��� üũ
				CheckWeight();          //���� ����
				ReFormPotionNum();		//���ǰ��� ����
				if(pMessageItem->ItemPosition) //������ �������� �־�����쿡�� ��ġ�� ����ش�
					sInven[pMessageItem->ItemPosition-1].ItemIndex = PotionIndex;
				return TRUE;

			}
			*/

			else {
				if (cShop.CheckHighRankItem(pItem)) {
					CursorClass = 1; //Ŀ���� �������ش�
					pItem->Flag = 0; //Ŀ�������� ���콺 �������� ��� �ʱ�ȭ��
					ResetInvenItemCode(); //���콺 �÷��� ���ٶ� �ٽ� üũ���ش�
					cMessageBox.ShowMessage3(MESSAGE_SELL_HIGHRANK_ITEM, pItem->sItemInfo.ItemName);

				}
				else cShop.SellItemToShop(pItem);
			}

		}
	}
	else { //������ �ƴϰ��� ����� �����۰� ������������� ���������� 
		if (pItem->sItemInfo.SpecialItemFlag[0] == CHECK_COPY_ITEM ||
			pItem->sItemInfo.SpecialItemFlag[0] == CHECK_GIVE_ITEM ||
			pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
			(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
			(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
			(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinQW1 ||
			(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinBI1 ||
			(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinBI2 ||	//�����߰�(��Ű��ĳ�������ۿ� ���� ��� ����)

			(pItem->sItemInfo.CODE == (sinDA1 | sin31)) ||
			(pItem->sItemInfo.CODE == (sinDA2 | sin31)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin32)) ||
			(pItem->sItemInfo.CODE == (sinDA2 | sin32)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin33)) ||
			(pItem->sItemInfo.CODE == (sinDA2 | sin33)) ||
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
			(pItem->sItemInfo.CODE == (sinOA2 | sin31)) || (pItem->sItemInfo.CODE == (sinOA2 | sin32)) ||
			(pItem->sItemInfo.CODE == (sinDB1 | sin31)) || (pItem->sItemInfo.CODE == (sinDB1 | sin32)) ||

			(pItem->sItemInfo.CODE == (sinOR2 | sin33)) ||
			(pItem->sItemInfo.CODE == (sinOA1 | sin36)) ||
			(pItem->sItemInfo.CODE == (sinOA1 | sin37)) ||
			(pItem->sItemInfo.CODE == (sinDB1 | sin33)) ||
			(pItem->sItemInfo.CODE == (sinOA2 | sin33)) ||
			(pItem->sItemInfo.CODE == (sinDB1 | sin34)) ||
			(pItem->sItemInfo.CODE == (sinOA2 | sin34)) ||
			(pItem->sItemInfo.CODE == (sinBI1 | sin84)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin54)) || (pItem->sItemInfo.CODE == (sinDA2 | sin54)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin55)) || (pItem->sItemInfo.CODE == (sinDA2 | sin55)) ||
			(pItem->sItemInfo.CODE == (sinGF1 | sin07)) || (pItem->sItemInfo.CODE == (sinGF1 | sin08))
			)
		{
			return TRUE;

		}
		//if((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinGP1)return TRUE;
	}


	if (cShop.OpenFlag) //���� ������ ó�������� ó�����ش� (���������� �������� ������ �ʴ´� )
		return TRUE;


	if (!pItem->Flag)return FALSE;
	if (OpenFlag) {
		//pItem->x = pCursorPos.x-(MouseItem.w/2);
		//pItem->y = pCursorPos.y -(MouseItem.h/2);
		if (pItem->y + pItem->h < (600) - 180) { //�κ��丮 ������ ������� 
			//������ ��� 
			if (pItem->Class == ITEM_CLASS_POTION) {
				if (pItem->sItemInfo.PotionCount == 1) { //������ �Ѱ��� ��쿡�� �׳� ������ 
					if (sinThrowItemToFeild(pItem)) {
						sinPlaySound(pItem->SoundIndex);
						pItem->Flag = 0;
						ReFormPotionNum();		//���ǰ��� ���� 
						return TRUE;
					}

				}
				if (pMessageItem) { //�κ��丮���� ���� �������ϰ�쿡�� 
					if (GhostPotionCheckFlag) {
						if (sinThrowItemToFeild(pItem)) {
							sinPlaySound(pItem->SoundIndex);
							pItem->Flag = 0;
							GhostPotionCheckFlag = 0;
							ReFormPotionNum();		//���ǰ��� ���� 
							return TRUE;

						}

					}
					cMessageBox.ShowMessage2(MESSAGE_THROW_ITEM);
					pItem->Flag = 0; //���콺 ������ �ʱ�ȭ
					pMessageItem->Flag = 1; //�������� ������ �����ص״� �����ͷ� �÷��� �츰��
					sInven[pMessageItem->ItemPosition - 1].ItemIndex = BackUpItemIndex;
					CheckOverlapItem(pMessageItem, BackUpItemIndex);
					ReSettingPotion();      //���ǰ��� üũ 
					CheckWeight();          //���� ���� 
					ReFormPotionNum();		//���ǰ��� ���� 
					if (pMessageItem->ItemPosition) //������ �������� �־�����쿡�� ��ġ�� ����ش� 
						sInven[pMessageItem->ItemPosition - 1].ItemIndex = PotionIndex;
					return TRUE;
				}
			}
			if (sinThrowItemToFeild(pItem)) {
				sinPlaySound(pItem->SoundIndex);
				pItem->Flag = 0;
			}

		}
	}
	else {
		if (pItem->y + pItem->h < 540) {       //�κ��丮�� ����������� �������̽� â�� �ƴҰ�� ������ 
			//������ ��� 
			if (pItem->Class == ITEM_CLASS_POTION) {
				if (pItem->sItemInfo.PotionCount == 1) { //������ �Ѱ��� ��쿡�� �׳� ������ 
					if (sinThrowItemToFeild(pItem)) {
						sinPlaySound(pItem->SoundIndex);
						pItem->Flag = 0;
						return TRUE;
					}
				}

				if (pMessageItem) {
					if (GhostPotionCheckFlag) {
						if (sinThrowItemToFeild(pItem)) {
							sinPlaySound(pItem->SoundIndex);
							pItem->Flag = 0;
							GhostPotionCheckFlag = 0;
							ReFormPotionNum();		//���ǰ��� ���� 
							return TRUE;
						}
					}
					cMessageBox.ShowMessage2(MESSAGE_THROW_ITEM);
					pItem->Flag = 0; //���콺 ������ �ʱ�ȭ
					pMessageItem->Flag = 1; //�������� ������ �����ص״� �����ͷ� �÷��� �츰�� 
					sInven[pMessageItem->ItemPosition - 1].ItemIndex = BackUpItemIndex;
					CheckOverlapItem(pMessageItem, BackUpItemIndex);
					ReSettingPotion();      //���ǰ��� üũ 
					CheckWeight();          //���� ���� 
					ReFormPotionNum();		//���ǰ��� ���� 
					if (pMessageItem->ItemPosition)
						sInven[pMessageItem->ItemPosition - 1].ItemIndex = PotionIndex;
					return TRUE;
				}
			}
			if (sinThrowItemToFeild(pItem)) {
				sinPlaySound(pItem->SoundIndex);
				pItem->Flag = 0;
			}

		}
	}
	return TRUE;
}

// Itens por sexo aqui
int cINVENTORY::CharOnlySetItem(sITEM* pItem)
{
	for (int i = 0; i < NUM_COSTUMES - 1; i++)
	{
		if (saCostume[i].szItemCode != NULL)
		{
			if (strcmpi(saCostume[i].szItemCode, pItem->LastCategory) == 0)
			{
				if (saCostume[i].eCostumeClass != GetCharacterKind())
				{
					pItem->sItemInfo.NotUseFlag = 1;
					break;
				}
			}
		}
	}

	if (sinChar->JOB_CODE == 7 || sinChar->JOB_CODE == 8)
	{
		if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinDA1)
		{
			pItem->sItemInfo.NotUseFlag = 1;

		}
	}
	else
	{
		if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinDA2 ||
			(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinOM1)
		{
			pItem->sItemInfo.NotUseFlag = 1;

		}
	}


	//if (sinChar->JOB_CODE == 1 || sinChar->JOB_CODE == 2 || sinChar->JOB_CODE == 4 || sinChar->JOB_CODE == 6 || sinChar->JOB_CODE == 7)
	//{
	//	if ((pItem->sItemInfo.CODE == (sinDA1 | sin48)))
	//	{
	//		pItem->sItemInfo.NotUseFlag = 1;

	//	}
	//}

	if (sinChar->JOB_CODE != 8)
	{
		if ((pItem->sItemInfo.CODE == (sinDA2 | sin37) ||
			(pItem->sItemInfo.CODE == (sinDA2 | sin38) ||
				pItem->sItemInfo.CODE == (sinDA2 | sin45) ||
				pItem->sItemInfo.CODE == (sinDA2 | sin46))
			))
		{
			pItem->sItemInfo.NotUseFlag = 1;
		}
	}


	if (sinChar->JOB_CODE != 7)
	{
		if ((pItem->sItemInfo.CODE == (sinDA2 | sin35) ||
			(pItem->sItemInfo.CODE == (sinDA2 | sin36) ||
				pItem->sItemInfo.CODE == (sinDA2 | sin43) ||
				pItem->sItemInfo.CODE == (sinDA2 | sin44))
			))
		{
			pItem->sItemInfo.NotUseFlag = 1;
		}
	}

	//if (sinChar->JOB_CODE == 3 || sinChar->JOB_CODE == 5 || sinChar->JOB_CODE == 8)
	//{
	//	if ((pItem->sItemInfo.CODE == (sinDA1 | sin47)))
	//	{
	//		pItem->sItemInfo.NotUseFlag = 1;

	//	}
	//}

	if (sinChar->JOB_CODE == 3 || sinChar->JOB_CODE == 5)
	{
		if ((pItem->sItemInfo.CODE == (sinDA1 | sin43) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin44)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin35)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin36)
				)))
		{
			pItem->sItemInfo.NotUseFlag = 1;

		}
	}

	if (sinChar->JOB_CODE == 1 || sinChar->JOB_CODE == 2 || sinChar->JOB_CODE == 4 || sinChar->JOB_CODE == 6)
	{
		if ((pItem->sItemInfo.CODE == (sinDA1 | sin45) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin46)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin37)) ||
			(pItem->sItemInfo.CODE == (sinDA1 | sin38))
			))
		{
			pItem->sItemInfo.NotUseFlag = 1;

		}
	}



	return TRUE;
}

//���� ���¸� üũ�Ѵ� (���� ,�� ,��ø ....) 
int cINVENTORY::CheckRequireItem()
{
	int NotUseItemFlag = 0;
	int i;

	for (i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (InvenItem[i].sItemInfo.Level > sinChar->Level)NotUseItemFlag = 1;
			if (InvenItem[i].sItemInfo.Dexterity > sinChar->Dexterity)NotUseItemFlag = 1;
			if (InvenItem[i].sItemInfo.Strength > sinChar->Strength)NotUseItemFlag = 1;
			if (InvenItem[i].sItemInfo.Talent > sinChar->Talent)NotUseItemFlag = 1;
			if (InvenItem[i].sItemInfo.Spirit > sinChar->Spirit)NotUseItemFlag = 1;
			if (InvenItem[i].sItemInfo.Health > sinChar->Health)NotUseItemFlag = 1;
			if (DeleteEventItem_TimeOut(&InvenItem[i].sItemInfo) == TRUE) NotUseItemFlag = 1;  //	����� : �Ⱓ�� ������ ����Ǿ�����(Ŭ��ġ���� ���)
			if (NotUseItemFlag) {
				InvenItem[i].sItemInfo.NotUseFlag = 1;
				NotUseItemFlag = 0;
			}
			else
				InvenItem[i].sItemInfo.NotUseFlag = 0;
			CharOnlySetItem(&InvenItem[i]); //ĳ���ͺ� �������� üũ�Ѵ�
			if ((InvenItem[i].sItemInfo.CODE & sinITEM_MASK2) == sinFO1) {
				int j = 0;
				for (j = 0; j < 16; j++) { // ����� - ���� ���� �߰�
					if ((InvenItem[i].sItemInfo.CODE & sinITEM_MASK3) == SheltomCode2[j] || (InvenItem[i].sItemInfo.CODE & sinITEM_MASK3) == MagicSheltomCode[j]) {
						break;
					}
				}
				if ((InvenItem[i].sItemInfo.CODE & sinITEM_MASK3) < sin21) // ����� - �Ϲ� ����
				{
					if (sinChar->Level < ForceOrbUseLevel[j][0] ||
						sinChar->Level > ForceOrbUseLevel[j][1]) {
						InvenItem[i].sItemInfo.NotUseFlag = 1;
						NotUseItemFlag = 0;

					}
					else {
						InvenItem[i].sItemInfo.NotUseFlag = 0;

					}
				}
				else if ((InvenItem[i].sItemInfo.CODE & sinITEM_MASK3) < sin33) // ����� - ���� ����
				{
					if (sinChar->Level < MagicForceOrbUseLevel[j][0] ||
						sinChar->Level > MagicForceOrbUseLevel[j][1]) {
						InvenItem[i].sItemInfo.NotUseFlag = 1;
						NotUseItemFlag = 0;

					}
					else {
						InvenItem[i].sItemInfo.NotUseFlag = 0;

					}
				}
			}

		}

	}
	return TRUE;
}

//�����Ҽ� �ִ����� üũ 
int cINVENTORY::CheckRequireItemToSet(sITEM* pItem)
{
	int NotUseItemFlag = 0;

	if (pItem->sItemInfo.Level > sinChar->Level)NotUseItemFlag = 1;
	if (pItem->sItemInfo.Dexterity > sinChar->Dexterity)NotUseItemFlag = 1;
	if (pItem->sItemInfo.Strength > sinChar->Strength)NotUseItemFlag = 1;
	if (pItem->sItemInfo.Talent > sinChar->Talent)NotUseItemFlag = 1;
	if (pItem->sItemInfo.Spirit > sinChar->Spirit)NotUseItemFlag = 1;
	if (pItem->sItemInfo.Health > sinChar->Health)NotUseItemFlag = 1;
	if (NotUseItemFlag) {
		pItem->sItemInfo.NotUseFlag = 1;
		NotUseItemFlag = 0;
	}
	else {
		pItem->sItemInfo.NotUseFlag = 0;


	}
	CharOnlySetItem(pItem); //ĳ���ͺ� �������� üũ�Ѵ�
	return TRUE;

}

//�κ��丮 �ؽ�Ʈ 
int cINVENTORY::DrawInvenText()
{
	//if(!sinMoveKindInter[SIN_INVENTORY])return FALSE; //ĳ���� �������ͽ��� ���������� �����Ѵ� 

	HDC hdc = NULL;

	char strBuff[128];


	memset(strBuff, 0, sizeof(strBuff));
	//lpDDSBack->GetDC( &hdc );
	SelectObject(hdc, sinFont);
	//SetBkMode( hdc, TRANSPARENT );
	SetFontTextColor(RGB(255, 255, 255));

	POINT NoImageMessagePosi = { 0,0 };
	int i = 0;

	if (OpenFlag) {
		NumLineComa(sinChar->Money, strBuff);
		dsTextLineOut(hdc, CheckEditSize(InvenTextPosi[0][0] + 26, InvenTextPosi[0][2], strBuff), InvenTextPosi[0][1] + (256 - sinMoveKindInter[SIN_INVENTORY]),
			strBuff, lstrlen(strBuff));

		wsprintf(strBuff, "%d/%d", sinChar->Weight[0], sinChar->Weight[1]);
		dsTextLineOut(hdc, CheckEditSize(InvenTextPosi[1][0], InvenTextPosi[1][2], strBuff), InvenTextPosi[1][1] + (250 - sinMoveKindInter[SIN_INVENTORY]),
			strBuff, lstrlen(strBuff));
	}

	for (i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			///////////////////////// ������ �̹����� ���� ��� ǥ�����ش�
			if (!InvenItem[i].lpItem) {
				if (InvenItem[i].x > 10) {//���� -_-
					NoImageMessagePosi.x = InvenItem[i].x + (InvenItem[i].w / 2) - 5;
					NoImageMessagePosi.y = InvenItem[i].y + (InvenItem[i].h / 2) - 20;
					lstrcpy(strBuff, "NO");
					dsTextLineOut(hdc, NoImageMessagePosi.x, NoImageMessagePosi.y + (256 - sinMoveKindInter[SIN_INVENTORY]), strBuff, lstrlen(strBuff));

					NoImageMessagePosi.x = InvenItem[i].x + (InvenItem[i].w / 2) - 15;
					NoImageMessagePosi.y = InvenItem[i].y + (InvenItem[i].h / 2) - 13;
					lstrcpy(strBuff, "IMAGE");
					dsTextLineOut(hdc, NoImageMessagePosi.x, NoImageMessagePosi.y + (256 - sinMoveKindInter[SIN_INVENTORY]) + 20, strBuff, lstrlen(strBuff));
				}
			}
			if (InvenItem[i].Class == ITEM_CLASS_POTION) {
				wsprintf(strBuff, "%d", InvenItem[i].sItemInfo.PotionCount);
				if (InvenItem[i].ItemPosition == 11 || InvenItem[i].ItemPosition == 12 || InvenItem[i].ItemPosition == 13) {
					dsTextLineOut(hdc, InvenItem[i].x, InvenItem[i].y, strBuff, lstrlen(strBuff));
				}
				else {
					if (OpenFlag)
						dsTextLineOut(hdc, InvenItem[i].x, InvenItem[i].y + (256 - sinMoveKindInter[SIN_INVENTORY]), strBuff, lstrlen(strBuff));
				}
			}
		}
	}


	int BackUpPosX = 0;
	int BackUpPosY = 0;
	int Start = 0, End = 0, Conut = 0;
	char szBuff2[128];


	// �庰 - �Է�â
	if (nName == 1)
	{
		//	BackUpPosX = BackStartPos.x;
		//	BackUpPosY = BackStartPos.y; 

	//		BackStartPos.x = 300;
	//		BackStartPos.y = 100; 

		SelectObject(hdc, sinBoldFont);
		//SetBkMode( hdc, TRANSPARENT ); 
		SetFontTextColor(RGB(65, 177, 240));
		dsTextLineOut(hdc, BackStartPos.x + 60, BackStartPos.y, "Name", lstrlen("Name"));


		SetIME_Mode(1);
		hFocusWnd = hTextWnd;
		cInterFace.ChatFlag = 0;
		sinChatEnter = 0;
		sinMessageBoxShowFlag = 1;

		hFocusWnd = hTextWnd;
		GetWindowText(hFocusWnd, cInvenTory.szDoc, 20);


		SelectObject(hdc, sinFont);
		//SetBkMode( hdc, TRANSPARENT );
		SetFontTextColor(RGB(255, 255, 255));


		int len = 0;
		int line = 1;
		len = lstrlen(cInvenTory.szDoc);

		line += (len / 20);

		for (int i = 0; i < len; i++) {
			memset(szBuff2, 0, sizeof(szBuff2));
			while (1) {
				if (cInvenTory.szDoc[End] == NULL)break;
				if (cInvenTory.szDoc[End] & 0x80) { //�ѱ��̸�..
					End += 2;
					Conut += 2;
				}
				else { //�����̸�..
					End++;
					Conut++;
				}
				if (Conut > 20)break;
			}
			memcpy(szBuff2, &cInvenTory.szDoc[Start], End - Start);

			dsTextLineOut(hdc, BackStartPos.x + 32, BackStartPos.y + (i * 20) - 110, szBuff2, lstrlen(szBuff2));

			if (cInvenTory.szDoc[End] != NULL) {
				if (cInvenTory.szDoc[End] == ' ')
					End += 1;
				Start = End;
				Conut = 0;

			}
			else break;
		}


	}

	//�׽�Ʈ�� �˼��ִ� �޼��� ���߿� ��������
	/*
	if(sinTestFlag2){
		if(sinTestFlag2 <= 5){
			cMessageBox.ShowMessage(MESSAGE_TEST_DAMAGE_FUC);
		}
	}
	*/

	/////////////Test

/*
		wsprintf(strBuff,"%d  %d",BackUpX,BackUpY);
		dsTextLineOut(hdc,400,100,strBuff,lstrlen(strBuff));

		if(sinPosionItem){
			wsprintf(strBuff,"%d  %d",sinPosionItem->x , sinPosionItem->y);
			dsTextLineOut(hdc,200,100,strBuff,lstrlen(strBuff));


		}
		if(pMessageItem){
		wsprintf(strBuff,"%d  %d",pMessageItem->x , pMessageItem->y);
		dsTextLineOut(hdc,200,150,strBuff,lstrlen(strBuff));
		}

		wsprintf(strBuff,"%d  %d",InvenItem[sInven[10].ItemIndex-1].x , InvenItem[sInven[10].ItemIndex-1].y);
		dsTextLineOut(hdc,200,200,strBuff,lstrlen(strBuff));
		wsprintf(strBuff,"%d  %d",InvenItem[sInven[11].ItemIndex-1].x , InvenItem[sInven[11].ItemIndex-1].y);
		dsTextLineOut(hdc,200,300,strBuff,lstrlen(strBuff));
		wsprintf(strBuff,"%d  %d",InvenItem[sInven[12].ItemIndex-1].x , InvenItem[sInven[12].ItemIndex-1].y);
		dsTextLineOut(hdc,200,400,strBuff,lstrlen(strBuff));


	/*
	if(MouseItem.Flag){
		wsprintf(strBuff,"%d  %d",MouseItem.SetX , MouseItem.SetY);
		dsTextLineOut(hdc,200,200,strBuff,lstrlen(strBuff));
		wsprintf(strBuff,"%d  %d",MouseItem.OldX , MouseItem.OldY);
		dsTextLineOut(hdc,300,200,strBuff,lstrlen(strBuff));
		wsprintf(strBuff,"%d  %d",MouseItem.x , MouseItem.y);
		dsTextLineOut(hdc,400,200,strBuff,lstrlen(strBuff));

	}
	*/
	//lpDDSBack->ReleaseDC( hdc );

	return TRUE;
}

//������ ������ ���ؿ´� 
int cINVENTORY::SearchJobAndSetting()
{
	int cnt;

	JobName[0] = 0;

	cnt = 0;
	//���߿� ��������!
	/*
	if(smConfig.DebugMode){
		if(sinChar->ChangeJob > 1)
			sinChar->ChangeJob = 1;
	}
	*/
	while (1) {
		if (JobDataBase[cnt].JobCode == 0) break;
		if (JobDataBase[cnt].JobCode == sinChar->JOB_CODE) {
			sinJobList = GetJobDataCode(sinChar->JOB_CODE, sinChar->ChangeJob);
			lstrcpy(JobName, sinJobList->szName2);
			sinChar->JobBitMask = JobDataBase[cnt].JobBitCode;
			sinChar->LifeFunction = JobDataBase[cnt].LifeFunction; //����� �Լ� 
			sinChar->ManaFunction = JobDataBase[cnt].ManaFunction; //��� �Լ� 
			sinChar->StaminaFunction = JobDataBase[cnt].StaminaFunction; //�ٷ��Լ� 
			sinDamageFunction[0] = JobDataBase[cnt].DamageFunction[0];  //���� ���� 
			sinDamageFunction[1] = JobDataBase[cnt].DamageFunction[1];  //���Ÿ� ���� 
			sinDamageFunction[2] = JobDataBase[cnt].DamageFunction[2];  //���� ����

			break;
		}
		cnt++;
	}
	return TRUE;

}

extern BOOL bReiPVP;
extern BOOL bTopLVL;

extern DWORD sodcode;
int playtime = 0;
//ĳ���� �ɷ�ġ ���� 
int cINVENTORY::SetItemToChar(int CheckHackFlag)
{

	//CheckWeight();//���� ����  //��ŷ�������� ������ ���� �̰����� ���Ը� �ѹ��� �������ش�(Speed����)
	memset(&sElement_Attack, 0, sizeof(sELEMENT_ATTACK)); //�Ӽ� ���� �������� �̰������� �����Ѵ� 
	CheckCharForm();//����  

	sinTempDamage2[0] = 0;   //�������⸸ �ϴ� ������ (����� �������� )
	sinTempDamage2[1] = 0;

	int cnt = 0;
	int sinAttack_Rating = 0;   //���� ���߷� 
	int sinAttack_Damage[2] = { 0,0 }; //���ݷ� 
	int sinCritical = 0;      //ũ��Ƽ���� 100������ ��Ÿ���� 
	int sinDefense = 0;		  //���� 
	float sinAbsorption = 0;  //����� 
	int   sinTempAbsorption = 0; //������� �����Ѵ�
	float sinTempAbsorption2 = 0; //������� �����Ѵ�
	float sinBlock_Rate = 0;  //���� 
	int sinWeight = 0;        //���� 
	float sinMoveSpeed = 0;	  //���ǵ�
	int sinWeaponSpeed = 0;   //����ӵ� 
	int sinShooting_Range = 0;  //���ݰŸ�
	int sinSight = 0;		//�þ� (�߰� ����)
	int sinResistance[8] = { 0,0,0,0,0,0,0,0 };
	int sinPotion_Space = 2; //�⺻ �� 2�� ���� ����
	int sinSkillMasteryItem = 0;

	/////////// ��ų �ɷ� ġ 
	int sinSkillDamage[2] = { 0,0 };
	int sinSkillResistance[8] = { 0,0,0,0,0,0,0,0 };
	int sinSkillWeaponSpeed = 0;

	/////////// ĳ���� ���� ������ �ɷ�ġ 
	int sinCharItemDamage = 0;

	////////////Ưȭ ������ ���� //////////////////////
	float	sinAdd_fAbsorb = 0;			//����� 
	int     sinAdd_Defence = 0;			//���� 
	float   sinAdd_fSpeed = 0;			//�̵� �ӵ� 
	float   sinAdd_fBlock_Rating = 0;			//(����)���� 
	int     sinAdd_Attack_Speed = 0;			//���ݼӵ�
	int     sinAdd_Critical_Hit = 0;			//1.5�� ������Ȯ��
	int     sinAdd_Shooting_Range = 0;			//�����Ÿ� 
	short	sinAdd_Resistance[8] = { 0,0,0,0,0,0,0,0 };		//���ҿ����� ���׷� 

	////////////////////////////////////////////////

	short	sinLev_Attack_Resistance[8] = { 0,0,0,0,0,0,0,0 };	//���ҿ����� ���ݷ�
	int		sinLev_Mana = 0;	//����ȸ��(�ּ�)(�ִ�)
	int		sinLev_Life = 0;	//������ȸ��(�ּ�)(�ִ�) 
	int     sinLev_Attack_Rating = 0; //���߷� 
	short	sinLev_Damage[2] = { 0,0 };//���ݷ�

	float	sinPer_Mana_Regen = 0;//���� ���  (��������� float������ )
	float	sinPer_Life_Regen = 0;//������ ��� 
	float 	sinPer_Stamina_Regen = 0;//���׹̳� ���

	float fstrength, fhealth, ftalent, fLevel, fMaxWeight, fNowWeight, fSpirit, fDexterity;
	float fAttack_Rating, fDefense;

	sinAdd_fMagic_Mastery = 0;

	///////////////////////////////////////////////// 

	float sinfIncreLife = 0; //�ִ�ġ ���� 
	float sinfIncreMana = 0;
	float sinfIncreStamina = 0;

	float sinfRegenLife = 0; //Regen ���� 
	float sinfRegenMana = 0;
	float sinfRegenStamina = 0;

	/////////////////////////////////////////////////
	//�̻ڰ� �ٽøͱ� ��ų
	sinUndeadAbsorb = 0; //�𵥵� ����� �ʱ�ȭ 

	//////////////////////////////////////////////////
	int InvenCheckItem[15] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	sITEM sinTempItem;
	memset(&sinTempItem, 0, sizeof(sITEM));

	int CountAbsorbItem = 0;

	//Adicionais Costume xxstr
	/*if( isItemCostume && (InvenCostume.sItemInfo.Defence < 5000) )
	{
		sinDefense += InvenCostume.sItemInfo.Defence;
		sinAbsorption += InvenCostume.sItemInfo.fAbsorb;
		sinBlock_Rate += InvenCostume.sItemInfo.fBlock_Rating;
		sinMoveSpeed += InvenCostume.sItemInfo.fSpeed;
		sinfIncreLife += InvenCostume.sItemInfo.fIncrease_Life;
		sinfIncreMana += InvenCostume.sItemInfo.fIncrease_Mana;
		sinfIncreStamina += InvenCostume.sItemInfo.fIncrease_Stamina;

		for( int i = 0; i < 8; i++ )
			sinResistance[ i ] += InvenCostume.sItemInfo.Resistance[ i ];
	}*/

	//Big Head Adicionais
	if (chaPremiumitem.GetBigHeadTime() > 0 && chaPremiumitem.GetBigHeadType())
	{
		sinTempDamage2[0] += (int)(sinChar->Attack_Damage[0] * 5) / 100;
		sinTempDamage2[1] += (int)(sinChar->Attack_Damage[1] * 5) / 100;
	}

	if (bTopLVL)
	{
		sinTempDamage2[0] += (int)(sinChar->Attack_Damage[0] * 5) / 100;
		sinTempDamage2[1] += (int)(sinChar->Attack_Damage[1] * 5) / 100;
	}

	//Rei PvP adicional xxstr
	if (bReiPVP)
	{
		sinTempDamage2[0] += (int)(sinChar->Attack_Damage[0] * 5) / 100;
		sinTempDamage2[1] += (int)(sinChar->Attack_Damage[1] * 5) / 100;
	}

	if (lpCurPlayer->vipLevel)
	{
		sinTempDamage2[0] += (int)(sinChar->Attack_Damage[0] * 2) / 100;
		sinTempDamage2[1] += (int)(sinChar->Attack_Damage[1] * 2) / 100;
	}

     switch( sodcode )
	{
		case SODSKILL_GOLD:
			sinfIncreLife += 150;
			break;
		case SODSKILL_SILVER:
			sinfIncreLife += 100;
			break;
		case SODSKILL_BRONZE:
			sinfIncreLife += 50;
			break;
	}
	
	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {//���õ��ִ� �������̸� 
			if (InvenItem[i].ItemPosition) {
				if (InvenItem[i].sItemInfo.CODE > sinPM1)continue;
				if (InvenItem[i].sItemInfo.NotUseFlag)continue; //���õɼ� ���� �������̸� �Ķ��Ÿ ������ �����ʴ´�

				// N�o acrescenta status de item expirado
				if (InvenItem[i].sItemInfo.expireTime > 0)
				{
					if (DeleteEventItem_TimeOut(&InvenItem[i].sItemInfo) == TRUE)
						continue;
				}

				if (!CheckRequireItemToSet2(&InvenItem[i]))continue;

				if (InvenItem[i].Class == ITEM_CLASS_WEAPON_TWO)
					if (InvenItem[i].sItemInfo.ItemHeader.dwChkSum == 0)continue; //��չ��ⶫ�� ��¦ ���� 

				if (!InvenCheckItem[InvenItem[i].ItemPosition]) {  //�������� ���������� �ɷ�ġ�� �ݿ����Ѵ� 
					InvenCheckItem[InvenItem[i].ItemPosition] = i + 1;
				}
				else
					continue;

				////////////////////////ġƮ�� �������� ���������� 
				if (!CheckItemForm(&InvenItem[i].sItemInfo)) {
					SendSetHackUser(1); //��ŷ�� �Ϸ����ߴ� ���� ������ ��� TRUE ���� ���� 
					InvenItem[i].Flag = 0;  //ġƮ�� �������� ���ֹ����� 
					if (InvenItem[i].ItemPosition) {
						sInven[InvenItem[i].ItemPosition - 1].ItemIndex = 0;
						sinSetCharItem(InvenItem[i].CODE, InvenItem[i].SetModelPosi, FALSE); //����� ������ �����Ѵ� 

					}
					if (InvenItem[i].Class == ITEM_CLASS_WEAPON_TWO) {
						InvenItem[sInven[1].ItemIndex - 1].Flag = 0;
						sInven[1].ItemIndex = 0;
					}
					continue;
				}
				////////////////////////

				sinAttack_Rating += InvenItem[i].sItemInfo.Attack_Rating; //���� ���߷� 				
				sinAttack_Damage[0] += InvenItem[i].sItemInfo.Damage[0];
				sinAttack_Damage[1] += InvenItem[i].sItemInfo.Damage[1];
				sinCritical += InvenItem[i].sItemInfo.Critical_Hit;
				sinDefense += InvenItem[i].sItemInfo.Defence;
				//sinTempAbsorption = (int)(InvenItem[i].sItemInfo.fAbsorb*10.000001f);
				sinTempAbsorption = (int)(GetItemAbsorb(&InvenItem[i].sItemInfo) * 10.000001f);
				sinTempAbsorption2 = ((float)sinTempAbsorption / 10.0f);
				sinTempAbsorption2 += 0.000001f;  //��������

				if (InvenItem[i].sItemInfo.fAbsorb) {
					CountAbsorbItem++;

				}

				sinAbsorption += sinTempAbsorption2; //�޺κ��� �ڸ��� ����Ѵ�
//				sinAbsorption += InvenItem[i].sItemInfo.fAbsorb;

				sinBlock_Rate += InvenItem[i].sItemInfo.fBlock_Rating;
				sinMoveSpeed += InvenItem[i].sItemInfo.fSpeed;

				//����Ʈ �����۶����� �̷��� �Ѵ� ���Ӵ� -_-
				if (CheckQuestItemDownFlag && InvenItem[i].sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON) {
					sinWeaponSpeed += InvenItem[i].sItemInfo.Attack_Speed - 2;
				}
				else
					sinWeaponSpeed += InvenItem[i].sItemInfo.Attack_Speed;

				sinShooting_Range += InvenItem[i].sItemInfo.Shooting_Range;
				sinSight += InvenItem[i].sItemInfo.Sight;
				sinfRegenLife += InvenItem[i].sItemInfo.fLife_Regen;
				sinfRegenMana += InvenItem[i].sItemInfo.fMana_Regen;
				sinfRegenStamina += InvenItem[i].sItemInfo.fStamina_Regen;
				sinAdd_fMagic_Mastery = InvenItem[i].sItemInfo.fMagic_Mastery;

				sinfIncreLife += InvenItem[i].sItemInfo.fIncrease_Life;
				sinfIncreMana += InvenItem[i].sItemInfo.fIncrease_Mana;
				sinfIncreStamina += InvenItem[i].sItemInfo.fIncrease_Stamina;
				if (InvenItem[i].sItemInfo.Potion_Space) //�Ϸ� ����� �� �Ϸ��� ���� ������������  ����
				{
					// ����� - ���� �ϸ� ������ �߰�
					if (DeleteEventItem_TimeOut(&InvenItem[i].sItemInfo) == TRUE)
					{
						sinPotion_Space = 2;
					}
					else
						sinPotion_Space = InvenItem[i].sItemInfo.Potion_Space;
				}
				///////////////////////ĳ���� Ưȭ �ɷ�
				if (sinChar->JobBitMask & InvenItem[i].sItemInfo.JobCodeMask) {
					sinTempAbsorption = (int)(InvenItem[i].sItemInfo.JobItem.Add_fAbsorb * 10.000001f);
					sinTempAbsorption2 = ((float)sinTempAbsorption / 10.0f);
					sinTempAbsorption2 += 0.000001f;  //��������
					sinAdd_fAbsorb += sinTempAbsorption2; //�޺κ��� �ڸ��� ����Ѵ�

					if (InvenItem[i].sItemInfo.JobItem.Add_fAbsorb) {
						CountAbsorbItem++;

					}

					//sinAdd_fAbsorb    += InvenItem[i].sItemInfo.JobItem.Add_fAbsorb;   //����� 
					sinAdd_Defence += InvenItem[i].sItemInfo.JobItem.Add_Defence;   //���� 
					sinAdd_fSpeed += InvenItem[i].sItemInfo.JobItem.Add_fSpeed;	//�̵� �ӵ� 
					sinAdd_fBlock_Rating += InvenItem[i].sItemInfo.JobItem.Add_fBlock_Rating;	//(����)���� 
					sinAdd_Attack_Speed += InvenItem[i].sItemInfo.JobItem.Add_Attack_Speed;	//���ݼӵ�
					sinAdd_Critical_Hit += InvenItem[i].sItemInfo.JobItem.Add_Critical_Hit;	//1.5�� ������Ȯ��
					sinAdd_Shooting_Range += InvenItem[i].sItemInfo.JobItem.Add_Shooting_Range;	//�����Ÿ� 
					sinAdd_fMagic_Mastery += InvenItem[i].sItemInfo.JobItem.Add_fMagic_Mastery; //�������õ� 

					if (InvenItem[i].sItemInfo.JobItem.Lev_Mana) //���� �ִ뷮 
						sinLev_Mana += (sinChar->Level / InvenItem[i].sItemInfo.JobItem.Lev_Mana);				//����ȸ��(�ּ�)(�ִ�)
					if (InvenItem[i].sItemInfo.JobItem.Lev_Life)  //������ �ִ뷮
						sinLev_Life += (sinChar->Level / InvenItem[i].sItemInfo.JobItem.Lev_Life);				//������ȸ��(�ּ�)(�ִ�) 
					if (InvenItem[i].sItemInfo.JobItem.Lev_Attack_Rating) //���÷����� 
						sinLev_Attack_Rating += (sinChar->Level / InvenItem[i].sItemInfo.JobItem.Lev_Attack_Rating);		//���߷� 
					//if(InvenItem[i].sItemInfo.JobItem.Lev_Damage[0]) //������(�ּ�)
					//	sinLev_Damage[0] +=  (sinChar->Level /InvenItem[i].sItemInfo.JobItem.Lev_Damage[0]);			//���ݷ�
					if (InvenItem[i].sItemInfo.JobItem.Lev_Damage[1]) // LV/x Ưȭ +������ sinLev_Damage[0]�� ����ϰ������ʴ�
						sinLev_Damage[1] += (sinChar->Level / InvenItem[i].sItemInfo.JobItem.Lev_Damage[1]);			//���ݷ�
					if (InvenItem[i].sItemInfo.JobItem.Per_Mana_Regen)//��������
						sinPer_Mana_Regen += (InvenItem[i].sItemInfo.JobItem.Per_Mana_Regen / 2.0f);			//���� ��� 
					if (InvenItem[i].sItemInfo.JobItem.Per_Life_Regen)//������ ����
						sinPer_Life_Regen += (InvenItem[i].sItemInfo.JobItem.Per_Life_Regen / 2.0f);			//������ ��� 
					if (InvenItem[i].sItemInfo.JobItem.Per_Stamina_Regen)//���׹̳� ����
						sinPer_Stamina_Regen += (InvenItem[i].sItemInfo.JobItem.Per_Stamina_Regen / 2.0f);		//���׹̳� ���

					for (int t = 0; t < 8; t++) { //Ưȭ ���װ���  
						sinAdd_Resistance[t] += (int)InvenItem[i].sItemInfo.JobItem.Add_Resistance[t];	//���ҿ����� ���׷� 
						if (InvenItem[i].sItemInfo.JobItem.Lev_Attack_Resistance[t])
							sinLev_Attack_Resistance[t] += (int)InvenItem[i].sItemInfo.JobItem.Lev_Attack_Resistance[t] +
							(sinChar->Level / InvenItem[i].sItemInfo.JobItem.Lev_Attack_Resistance[t]);//���ҿ����� ���ݷ�

					}//if for
				}// Ưȭ�ɷ� 
				for (int j = 0; j < 8; j++) //�⺻ ���� 
					sinResistance[j] += (int)InvenItem[i].sItemInfo.Resistance[j];

			} //if ItemPosition

		} //end if InvenFlag
	}//end for

	float TempSkillData = 0;
	float TempDamage[2] = { 0,0 };
	int   TempAttack_Rating = 0;
	int   TempDefense = 0;
	int   TempDefense2 = 0;
	int   TempWeaponSpeed = 0;
	float TempCritical = 0;
	float TempMoveSpeed = 0;

	if (sInven[0].ItemIndex)
	{
		// N�o acrescenta status de item expirado
		if (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.expireTime > 0)
		{
			if (DeleteEventItem_TimeOut(&cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo) == FALSE)
			{
				float TempDamage2[2] = { cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0],cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1] };

				TempAttack_Rating = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Attack_Rating;
				memcpy(TempDamage, TempDamage2, sizeof(float) * 2);
				TempWeaponSpeed = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Attack_Speed;
				if (sinChar->JobBitMask & cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.JobCodeMask) {
					TempWeaponSpeed += cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.JobItem.Add_Attack_Speed;

				}
				TempCritical = (float)cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Critical_Hit;
			}
		}
		else
		{
			float TempDamage2[2] = { cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0],cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1] };

			TempAttack_Rating = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Attack_Rating;
			memcpy(TempDamage, TempDamage2, sizeof(float) * 2);
			TempWeaponSpeed = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Attack_Speed;
			if (sinChar->JobBitMask & cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.JobCodeMask) {
				TempWeaponSpeed += cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.JobItem.Add_Attack_Speed;

			}
			TempCritical = (float)cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Critical_Hit;
		}
	}

	if (chaPremiumitem.m_Hat)// Hat B�nus / B�nus dos chapeus
	{
		switch (chaPremiumitem.HatKind)
		{
		case 1:
			sinfIncreLife += 100.0f; // ovelha
			break;
		case 2:
			sinCritical += 5; // carnaval
			break;
		case 3:
			sinAdd_fSpeed += 10; // Futebol
			break;
		case 4:
			sinfRegenLife += 4;
			break;
		case 5:
			sinfRegenStamina += 4;
			break;
		case 6:
			sinfRegenMana += 4; // Cartola
			break;
		}
	}

	//Ưȭ�� �־��Ѵ�
	int MetalArmor = 0;
	if (sInven[2].ItemIndex) {
		TempDefense = cInvenTory.InvenItem[sInven[2].ItemIndex - 1].sItemInfo.Defence;
		if (sinChar->JobBitMask & cInvenTory.InvenItem[sInven[2].ItemIndex - 1].sItemInfo.JobCodeMask) {
			TempDefense += cInvenTory.InvenItem[sInven[2].ItemIndex - 1].sItemInfo.JobItem.Add_Defence;
			MetalArmor = 1; //��Ż �Ƹ� �÷�

		}

	}

	if (sInven[1].ItemIndex) {
		TempDefense2 = cInvenTory.InvenItem[sInven[1].ItemIndex - 1].sItemInfo.Defence;
		if (sinChar->JobBitMask & cInvenTory.InvenItem[sInven[1].ItemIndex - 1].sItemInfo.JobCodeMask) {
			TempDefense2 += cInvenTory.InvenItem[sInven[1].ItemIndex - 1].sItemInfo.JobItem.Add_Defence;


		}

	}

	//SETBOSS #!
	//Set Boss #1
	if (cInvenTory.InvenItem[sInven[4].ItemIndex - 1].sItemInfo.CODE == (sinOR1 | sin15)) {
		LValRing = 1;
	}
	else { LValRing = 0; }
	if (cInvenTory.InvenItem[sInven[5].ItemIndex - 1].sItemInfo.CODE == (sinOR1 | sin15)) {
		RValRing = 1;
	}
	else { RValRing = 0; }
	if (cInvenTory.InvenItem[sInven[3].ItemIndex - 1].sItemInfo.CODE == (sinOA1 | sin15)) {
		KelAmulet = 1;
	}
	else { KelAmulet = 0; }

	if (cInvenTory.InvenItem[sInven[6].ItemIndex - 1].sItemInfo.CODE == (sinOS1 | sin20)) {
		KelSheltom = 1;
	}
	else { KelSheltom = 0; }

	int SetBossHP = 50;
	int SetBossDef = 50;

	SetBonus1 = (LValRing + RValRing + KelAmulet + KelSheltom);//Set Boss #1
	float BonusHP1 = SetBossHP + 0.f;
	//Bonus #1
	if (SetBonus1 >= 3)
	{
		sinAdd_Defence += SetBossDef;
	}
	if (SetBonus1 >= 4)
	{
		sinfIncreLife += BonusHP1;
		SkillTextColor[SIN2_LIFE] = SIN_TEXT_COLOR_GOLD;
	}

	

	int sinSkillTimeDamage[2] = { 0,0 };
	sinTempBlock = 0;
	sinTempAbsorb = 0;

	int Count2 = 0;
	int Point2 = 0;
	int p = 0;
	int Flag3 = 0; //��ų�� ����Ѵ� ������ ����Ʈ���̼Ǹ�
	int Flag4 = 0; //��ų�� ����Ѵ� ������ ������Ʈ���� 

	//PC��� ���߷¶����� �̰����� �ʱ�ȭ���ش�
	SkillTextColor[SIN2_ATTACK_RATE] = SIN_TEXT_COLOR_WHITE;
	SkillTextColor[SIN2_LIFE] = SIN_TEXT_COLOR_WHITE;
	SkillTextColor[SIN2_MANA] = SIN_TEXT_COLOR_WHITE; // ����� - �ν��� ������(���)
	SkillTextColor[SIN2_STAMINA] = SIN_TEXT_COLOR_WHITE; // ����� - �ν��� ������(�ٷ�)
	SkillTextColor[SIN2_SPEED] = SIN_TEXT_COLOR_WHITE;

	int TempChangeDamageFlag = 0;
	int TempChangeAbsorbFlag = 0;

	for (int i = 0; i < 19; i++)
	{
		if (ContinueSkill[i].Flag) {
			switch (ContinueSkill[i].CODE) {
			case SKILL_EXTREME_SHIELD:
				sinTempBlock += ContinueSkill[i].PlusState[0];
				//sinBlock_Rate += ContinueSkill[i].PlusState[0];
				break;
			case SKILL_PHYSICAL_ABSORB:
				sinTempAbsorb += ContinueSkill[i].PlusState[0];
				break;
			case SKILL_AUTOMATION:
				if (sinWS1 == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2) ||
					sinWT1 == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) {
					//�������� �������� ó���Ѵ�
					sinTempDamage2[0] += (int)(((float)Automation_Damage[ContinueSkill[i].Point - 1] / 100) * TempDamage[0]); //���÷��̷� �����ֱ⸸�Ѵ�
					sinTempDamage2[1] += (int)(((float)Automation_Damage[ContinueSkill[i].Point - 1] / 100) * TempDamage[1]);
					sinSkillWeaponSpeed += ContinueSkill[i].PlusState[0];
					SendProcessSKillToServer(SKILL_PLAY_AUTOMATION, ContinueSkill[i].Point, 0, 0);
				}
				else
					SendCancelSkillToServer(SKILL_PLAY_AUTOMATION, ContinueSkill[i].Point, 0, 0); //��ų�� ��ҵɶ� ������ �˷��ش� 

				break;
			case SKILL_MAXIMIZE:
				SendProcessSKillToServer(SKILL_PLAY_MAXIMIZE, ContinueSkill[i].Point, 0, 0);
				sinTempDamage2[1] += (int)(((float)ContinueSkill[i].PlusState[0] / 100.0f) * TempDamage[1]);

				break;
			case SKILL_WINDY: //���߷� ���  
				if (sinWT1 == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) {
					sinAttack_Rating += (Windy_Attack_Rating[ContinueSkill[i].Point - 1] * TempAttack_Rating) / 100;
					sinShooting_Range += 30; //�����Ÿ� 30�÷��� 
					SkillTextColor[SIN2_ATTACK_RATE] = SIN_TEXT_COLOR_ORANGE;

				}
				else {
					ContinueSkill[i].CheckTime = ContinueSkill[i].UseTime * 70; //��â�� �ƴҰ�� ��ҽ�Ų�� 
					SkillTextColor[SIN2_ATTACK_RATE] = SIN_TEXT_COLOR_WHITE;
				}
				break;
			case SKILL_METAL_ARMOR:
				if (MetalArmor)
				{
					sinDefense += (Metal_Armor_Defense[ContinueSkill[i].Point - 1] * (TempDefense)) / 100;
					SkillTextColor[SIN2_DEFENSE] = SIN_TEXT_COLOR_BLUE;
					Point2 = CSKILL->GetSkillPoint(SKILL_PHYSICAL_ABSORB);
					if (Point2) {
						sinTempAbsorb += (int)(((P_Absorb[Point2 - 1][0] + P_Absorb[Point2 - 1][1]) * 1.2f) / 2);
					}
				}
				break;
			case SKILL_SPARK_SHIELD:
				if (sInven[1].ItemIndex) {
					if (sinDS1 == (cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE & sinITEM_MASK2)) {
						sinDefense += Spark_Shield_Defense[ContinueSkill[i].Point - 1];
					}
				}
				break;
			case SKILL_SWIFT_AXE:
				if (sInven[0].ItemIndex) {
					if (ContinueSkill[i].Skill_Info.UseWeaponCode[0] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) {
						sinSkillWeaponSpeed += (int)((Swift_Axe_Speed[ContinueSkill[i].Point - 1] * TempWeaponSpeed) / 100);
						Flag4 = 1;
					}
				}
				//��ų��빫�Ⱑ �ƴϸ� ���ش�
				//if(!Flag4){
				///	ContinueSkill[i].CheckTime = ContinueSkill[i].UseTime*70; //������ �ƴҰ��� ����Ѵ�
				//}
				break;
			case SKILL_TRIUMPH_OF_VALHALLA:
				//��Ƽ���� �ݸ� ����ȴ�
				sinTempDamage2[1] += (T_Of_Valhalla_Damage[ContinueSkill[i].Point - 1] + (SetT_Of_ValhallaLV / 4)) / (SetT_Of_ValhallaFlag + 1);
				SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_GOLD;
				break;
			case SKILL_SPIRIT_ELEMENTAL:
				sinfRegenMana += Spirit_Elemental_RegenMana[ContinueSkill[i].Point - 1];

				break;
			case SKILL_GOLDEN_FALCON:
				sinfRegenLife += Golden_Falcon_LifeRegen[ContinueSkill[i].Point - 1];
				break;
			case SKILL_VIRTUAL_LIFE:
				SkillTextColor[SIN2_LIFE] = SIN_TEXT_COLOR_PINK;
				break;
			case SKILL_DIVINE_INHALATION:
				sinTempBlock += D_Inhalation_Block[ContinueSkill[i].Point - 1];
				break;
			case SKILL_MAGNETIC_SPHERE:
				//sinTempDamage2[0]  += Magnetic_Sphere_AddDamage[ContinueSkill[i].Point-1][0];
				//sinTempDamage2[1]  += Magnetic_Sphere_AddDamage[ContinueSkill[i].Point-1][1];
				//sinTempDamage2[1]  += Magnetic_Sphere_AddDamage[ContinueSkill[i].Point-1];
				//SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_GOLD;
				break;
			case SKILL_BERSERKER:
				sinTempDamage2[0] += Berserker_AddAttack[ContinueSkill[i].Point - 1];
				sinTempDamage2[1] += Berserker_AddAttack[ContinueSkill[i].Point - 1];
				sinTempAbsorb += Berserker_SubAbsorb[ContinueSkill[i].Point - 1];
				SkillTextColor[SIN2_ABSORB] = SIN_TEXT_COLOR_RED;
				break;
			case SKILL_ASSASSIN_EYE:
				//sinCritical += Assassin_Eye_AddCritical[ContinueSkill[i].Point-1];
				break;
			case SKILL_EVASION_MASTERY:
				if (lpCurPlayer->PlayEvasionMastery)
				{
					TempMoveSpeed += (int)Evasion_Mastery_AddSpeed[ContinueSkill[i].Point - 1];
					SkillTextColor[SIN2_SPEED] = SIN_TEXT_COLOR_GREEN;
				}
				break;
			case SKILL_FORCE_OF_NATURE: //Flag ���� 1 ��Ƽ 2
				sinTempDamage2[0] += Force_Of_Nature_AddDamage[ContinueSkill[i].Point - 1] / ContinueSkill[i].Flag;
				sinTempDamage2[1] += Force_Of_Nature_AddDamage[ContinueSkill[i].Point - 1] / ContinueSkill[i].Flag;
				sinAttack_Rating += Force_Of_Nature_AddHit[ContinueSkill[i].Point - 1] / ContinueSkill[i].Flag;
				SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_GOLD;
				SkillTextColor[SIN2_ATTACK_RATE] = SIN_TEXT_COLOR_ORANGE;
				break;
			case SKILL_GOD_BLESS:
				sinTempDamage2[0] += God_Bless_AddDamage[ContinueSkill[i].Point - 1];
				sinTempDamage2[1] += God_Bless_AddDamage[ContinueSkill[i].Point - 1];
				SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_GOLD;
				break;
			case SKILL_HALL_OF_VALHALLA: //��ô��
				if (SetT_Of_ValhallaPOINT) {
					sinTempDamage2[1] += (T_Of_Valhalla_Damage[SetT_Of_ValhallaPOINT - 1] + (SetH_Of_ValhallaLV / 4) /*) /ContinueSkill[i].Flag*/); //xxstr
					SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_GOLD;
				}
				break;
			case SKILL_COMPULSION:
				sinTempAbsorb += Compulsion_AddAbsorb[ContinueSkill[i].Point - 1];
				SkillTextColor[SIN2_ABSORB] = SIN_TEXT_COLOR_BLUE;
				break;
			case SKILL_FROST_JAVELIN: //��ô��
				if (sinWT1 == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) {
					sinTempDamage2[0] += Frost_Javelin_IceAddDamage[ContinueSkill[i].Point - 1][0];
					sinTempDamage2[1] += Frost_Javelin_IceAddDamage[ContinueSkill[i].Point - 1][1];
				}
				else {
					ContinueSkill[i].CheckTime = ContinueSkill[i].UseTime * 70; //��â�� �ƴҰ�� ��ҽ�Ų�� 
					SkillTextColor[SIN2_ATTACK_RATE] = SIN_TEXT_COLOR_WHITE;
					SendCancelSkillToServer(SKILL_PLAY_FROST_JAVELIN, 0, 0, 0);
				}
				break;
			case CLANSKILL_ABSORB:   //�����+20 
				sinTempAbsorb += 5;	 // ����� - ������ ���� Ŭ�� ��ų ���� ����(10 -> 20)
				SkillTextColor[SIN2_ABSORB] = SIN_TEXT_COLOR_BLUE;
				break;
			case SCROLL_INVULNERABILITY:  //���� ��ũ�� ���ݷ� 1/2
			case SCROLL_P_INVULNERABILITY:
				sinTempDamage2[0] -= (sinChar->Attack_Damage[0]) / 3;
				sinTempDamage2[1] -= (sinChar->Attack_Damage[1]) / 3;
				SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_RED;
				break;

				/*----------------------------------------------------------------------------*
				*							SOD SKILL XXSTR
				*-----------------------------------------------------------------------------*/
			case SODSKILL_GOLD:
				sinfIncreLife += 150.0f;
				SkillTextColor[SIN2_LIFE] = SIN_TEXT_COLOR_GOLD;
				break;
			case SODSKILL_SILVER:
				sinfIncreLife += 100.0f;
				SkillTextColor[SIN2_LIFE] = SIN_TEXT_COLOR_GOLD;
				break;
			case SODSKILL_BRONZE:
				sinfIncreLife += 50.0f;
				SkillTextColor[SIN2_LIFE] = SIN_TEXT_COLOR_GOLD;
				break;

			case SKILL_GODLY_SHIELD:
			{
				//extern short Godly_Shield_AbsorbPercent[10];
				//sinChar->Absorption += (Godly_Shield_AbsorbPercent[ContinueSkill[i].Point - 1] / 100) * 100;

				TempChangeAbsorbFlag = 1;
			}
			break;

			case BOOSTER_ITEM_LIFE: // ����� - �ν��� ������(�����)
				SkillTextColor[SIN2_LIFE] = SIN_TEXT_COLOR_PINK; // ������϶� �������̽� ���ڻ��� �ٲ��ش�

				if (AddBoosterLife)
				{
					sinfRegenLife += ((short)BoosterItem_DataPercent[0]);
				}
				else
					tempLife[0] = 0;

				break;
			case BOOSTER_ITEM_MANA: // ����� - �ν��� ������(���)
				SkillTextColor[SIN2_MANA] = SIN_TEXT_COLOR_PINK; // ������϶� �������̽� ���ڻ��� �ٲ��ش�

				if (AddVirtualMana[1])
				{
					sinfRegenMana += ((short)BoosterItem_DataPercent[0]);
				}
				else
					tempMana[0] = 0;

				break;
			case BOOSTER_ITEM_STAMINA: // ����� - �ν��� ������(�ٷ�)
				SkillTextColor[SIN2_STAMINA] = SIN_TEXT_COLOR_PINK; // ������϶� �������̽� ���ڻ��� �ٲ��ش�

				if (AddVirtualStamina[1])
				{
					sinfRegenStamina += ((short)BoosterItem_DataPercent[0]);
				}
				else
					tempStamina[0] = 0;

				break;

			}
		}
	}

	//////// ���׷� ����ġ 
	for (int i = 1; i < MAX_USESKILL; i++)
	{ //��� ������ ���ش� 
		if (sinSkill.UseSkill[i].Flag && sinSkill.UseSkill[i].Point) {//��� ����Ʈ�� ������
			switch (sinSkill.UseSkill[i].CODE) {
			case SKILL_POISON_ATTRIBUTE:       //������ ���� 
				sinSkillResistance[sITEMINFO_POISON] = PlusPoison[sinSkill.UseSkill[i].Point - 1];

				break;
			case SKILL_ICE_ATTRIBUTE:
				sinSkillResistance[sITEMINFO_ICE] = PlusIce[sinSkill.UseSkill[i].Point - 1];

				break;
			case SKILL_FIRE_ATTRIBUTE:
				sinSkillResistance[sITEMINFO_FIRE] = PlusFire[sinSkill.UseSkill[i].Point - 1];

				break;
			}
		}
	}

	//���� �ɷ�ġ ����Ǳ��� ���ڸ� �÷��� ���ִ°�
	int TempDivide = 1;
	for (int i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (ContinueSkill[i].Flag) {
			switch (ContinueSkill[i].CODE) {
			case SKILL_ZENITH:
				if (ContinueSkill[i].PartyFlag)TempDivide = 2; //��Ƽ���� ���� 50%�� �����Ѵ� 
				sinSkillResistance[sITEMINFO_BIONIC] += Zenith_Element[ContinueSkill[i].Point - 1] / TempDivide;
				sinSkillResistance[sITEMINFO_FIRE] += Zenith_Element[ContinueSkill[i].Point - 1] / TempDivide;
				sinSkillResistance[sITEMINFO_ICE] += Zenith_Element[ContinueSkill[i].Point - 1] / TempDivide;
				sinSkillResistance[sITEMINFO_LIGHTING] += Zenith_Element[ContinueSkill[i].Point - 1] / TempDivide;
				sinSkillResistance[sITEMINFO_POISON] += Zenith_Element[ContinueSkill[i].Point - 1] / TempDivide;
				break;
			}
		}
	}
	/////////////////////////////////////////////
	for (int i = 0; i < 8; i++)
	{ //���׷� 
		sinChar->Resistance[i] = sinResistance[i] + sinAdd_Resistance[i] + sinSkillResistance[i];

	}

	//////////////////////// ������ ����ġ 
	for (int i = 1; i < MAX_USESKILL; i++)
	{ //��� ������ ���ش� 
		if (sinSkill.UseSkill[i].Flag && sinSkill.UseSkill[i].Point) { //��� ����Ʈ�� ������ 
			//////////// ���⿡ ������� �ɷ�ġ�� �����Ѵ� 
			//switch(sinSkill.UseSkill[i].CODE){

			//}
			//////////////���� �迭�����ϰ�쿡 �ش� 
			for (cnt = 0; cnt < 8; cnt++) {
				switch (sinSkill.UseSkill[i].CODE) {
				case SKILL_MELEE_MASTERY:       //������ ���� 
					if (sInven[0].ItemIndex) {
						if (sinSkill.UseSkill[i].Skill_Info.UseWeaponCode[cnt] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) {
							sinSkillDamage[0] = (int)(((float)Melee_Mastery_DamagePercent[sinSkill.UseSkill[i].Point - 1] / 100) * TempDamage[0]);
							sinSkillDamage[1] = (int)(((float)Melee_Mastery_DamagePercent[sinSkill.UseSkill[i].Point - 1] / 100) * TempDamage[1]);
						}
					}
					break;
				case SKILL_SHOOTING_MASTERY:    //Ȱ ������ ����  
					if (sInven[0].ItemIndex) {
						if (sinSkill.UseSkill[i].Skill_Info.UseWeaponCode[cnt] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) {
							sinSkillDamage[0] = (int)(((float)S_Mastery_DamagePercent[sinSkill.UseSkill[i].Point - 1] / 100) * TempDamage[0]);
							sinSkillDamage[1] = (int)(((float)S_Mastery_DamagePercent[sinSkill.UseSkill[i].Point - 1] / 100) * TempDamage[1]);

						}
					}
					break;
				case SKILL_WEAPONE_DEFENCE_MASTERY:
					if (sInven[0].ItemIndex) {
						if (sinSkill.UseSkill[i].Skill_Info.UseWeaponCode[cnt] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) {
							if (sInven[1].ItemIndex) //���а� ������ �극��ũ 
								if (sinDS1 == (cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE & sinITEM_MASK2)) //���и� �������� �������� ���� 
									break;

							sinBlock_Rate += W_D_Mastery_Block[sinSkill.UseSkill[i].Point - 1];

						}
					}
					break;
				case SKILL_DIONS_EYE:
					if (sInven[0].ItemIndex) {
						if (sinSkill.UseSkill[i].Skill_Info.UseWeaponCode[cnt] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2))
							sinAttack_Rating += (D_Eye_Attack_Rate[sinSkill.UseSkill[i].Point - 1] * TempAttack_Rating) / 100;
					}

					break;

				case SKILL_THROWING_MASTERY:
					if (sInven[0].ItemIndex) {
						if (sinSkill.UseSkill[i].Skill_Info.UseWeaponCode[cnt] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) {
							sinSkillDamage[0] = (int)(((float)T_Mastery_Damage[sinSkill.UseSkill[i].Point - 1] / 100) * TempDamage[0]);
							sinSkillDamage[1] = (int)(((float)T_Mastery_Damage[sinSkill.UseSkill[i].Point - 1] / 100) * TempDamage[1]);
						}
					}
					break;
				case SKILL_MECHANIC_WEAPON:
					if (sInven[0].ItemIndex) {
						if (sinChar->JobBitMask & InvenItem[sInven[0].ItemIndex - 1].sItemInfo.JobCodeMask) {
							sinSkillDamage[0] = (int)(((float)M_Weapon_Mastey[sinSkill.UseSkill[i].Point - 1] / 100) * TempDamage[0]);
							sinSkillDamage[1] = (int)(((float)M_Weapon_Mastey[sinSkill.UseSkill[i].Point - 1] / 100) * TempDamage[1]);
						}
						//��ī�ϼ� ����ũ ������
						if (InvenItem[sInven[0].ItemIndex - 1].sItemInfo.UniqueItem == 2) {
							sinSkillDamage[0] = (int)(((float)M_Weapon_Mastey[sinSkill.UseSkill[i].Point - 1] / 100) * TempDamage[0]);
							sinSkillDamage[1] = (int)(((float)M_Weapon_Mastey[sinSkill.UseSkill[i].Point - 1] / 100) * TempDamage[1]);
						}

					}
					break;
				case SKILL_CRITICAL_MASTERY:
					if (sInven[0].ItemIndex) {
						if (sinSkill.UseSkill[i].Skill_Info.UseWeaponCode[cnt] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) {
							sinCritical += Critical_Mastery_Critical[sinSkill.UseSkill[i].Point - 1];

						}
					}
					break;
				case SKILL_SWORD_MASTERY:
					if (sInven[0].ItemIndex) {
						if (sinSkill.UseSkill[i].Skill_Info.UseWeaponCode[cnt] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) {
							sinSkillDamage[0] = (int)(((float)Sword_Mastery_DamagePercent[sinSkill.UseSkill[i].Point - 1] / 100) * TempDamage[0]);
							sinSkillDamage[1] = (int)(((float)Sword_Mastery_DamagePercent[sinSkill.UseSkill[i].Point - 1] / 100) * TempDamage[1]);

						}
					}
					break;
				case SKILL_EVASION_MASTERY: //���ϴ� ���ϱ� -_-?


					break;
				}
			}
		}
	}

	/////////ĳ���Ϳ� �ش��ϴ� ������� ������ �߰� 
	switch (sinChar->JOB_CODE) {
	case SIN_CHAR_FIGHTER:
		if (sinWA1 == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2))
			sinCharItemDamage = sinChar->Level / 6;

		break;

	case SIN_CHAR_MECANICIAN:
		if (sInven[0].ItemIndex) {
			if (sinChar->JobBitMask & InvenItem[sInven[0].ItemIndex - 1].sItemInfo.JobCodeMask)
				sinCharItemDamage = sinChar->Level / 6;
			//��ī�ϼ� ����ũ ������ �ֹ��ⱺ ���� 
			if (InvenItem[sInven[0].ItemIndex - 1].sItemInfo.UniqueItem == 2)
				sinCharItemDamage = sinChar->Level / 6;
		}

		break;

	case SIN_CHAR_ARCHER:
		if (sinWS1 == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2))
			sinCharItemDamage = sinChar->Level / 6;

		break;

	case SIN_CHAR_PIKEMAN:
		if (sinWP1 == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2))
			sinCharItemDamage = sinChar->Level / 6;
		break;
	case SIN_CHAR_KNIGHT:
		if (sinWS2 == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2))
			sinCharItemDamage = sinChar->Level / 6;

		break;
	case SIN_CHAR_ATALANTA:
		if (sinWT1 == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2))
			sinCharItemDamage = sinChar->Level / 6;

		break;
	case SIN_CHAR_PRIESTESS:
		if (sinWM1 == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2))
			sinCharItemDamage = sinChar->Level / 6;

		break;
	case SIN_CHAR_MAGICIAN:
		if (sinWM1 == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2))
			sinCharItemDamage = sinChar->Level / 6;

		break;


	}

	for (int i = 0; i < 10; i++)
	{
		if (ContinueSkill[i].Flag) {
			for (cnt = 0; cnt < 8; cnt++) {
				switch (ContinueSkill[i].CODE) {
				case SKILL_AUTOMATION:
				case SKILL_MAXIMIZE:
					if (ContinueSkill[i].Skill_Info.UseWeaponCode[cnt] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) {
						TempChangeDamageFlag = 1;
					}
					break;
				case SKILL_METAL_ARMOR:
					if (MetalArmor) {
						TempChangeAbsorbFlag = 1;
					}
					break;
				case SKILL_PHYSICAL_ABSORB:
					TempChangeAbsorbFlag = 1;
					break;
				case SKILL_FORCE_ORB:
					TempChangeDamageFlag = 1;
					break;
				case SKILL_COMPULSION:
					TempChangeAbsorbFlag = 1;
					break;
				case CLANSKILL_ABSORB:
					TempChangeAbsorbFlag = 1;
					break;
				case CLANSKILL_ATTACK:
					TempChangeDamageFlag = 1;
					break;
				case SCROLL_INVULNERABILITY:
					TempChangeDamageFlag = 1;
					break;
				}
			}
		}
	}

	if (sinChar->dwEventTime_T && sinChar->SizeLevel > 1)
	{
		TempChangeDamageFlag = 1;
	}

	if (sinCharItemDamage)
		SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_GOLD;
	else if (TempChangeDamageFlag)
		SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_GOLD;
	else SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_WHITE;

	//���������
	if (TempChangeAbsorbFlag)
		SkillTextColor[SIN2_ABSORB] = SIN_TEXT_COLOR_BLUE;
	else SkillTextColor[SIN2_ABSORB] = SIN_TEXT_COLOR_WHITE;

	sinChar->Potion_Space = sinPotion_Space; //������ ������ �ִ� �Ѱ�ġ 

	fstrength = (float)sinChar->Strength;
	fhealth = (float)sinChar->Health;
	ftalent = (float)sinChar->Talent;
	fSpirit = (float)sinChar->Spirit;
	fDexterity = (float)sinChar->Dexterity;
	fLevel = (float)sinChar->Level;


	//���� ���߷� 
	sinChar->Attack_Rating = (int)((sinAttack_Rating + (fDexterity * 3.1) + (fLevel * 1.9) + (ftalent * 1.5)) + sinLev_Attack_Rating);

	if (sinDamageFunction[0] == 1 && !sInven[0].ItemIndex) {
		sinChar->Attack_Damage[0] = 1 + ((sinChar->Strength + 130) / 130) + ((sinChar->Talent + sinChar->Dexterity) / 40) + sinLev_Damage[0];
		sinChar->Attack_Damage[1] = 2 + ((sinChar->Strength + 130) / 130) + ((sinChar->Talent + sinChar->Dexterity) / 35) + sinLev_Damage[1];
	}
	if ((sinDamageFunction[0] == 2 || sinDamageFunction[0] == 3 || sinDamageFunction[0] == 4) && !sInven[0].ItemIndex) {
		sinChar->Attack_Damage[0] = 1 + ((sinChar->Strength + 200) / 200) + ((sinChar->Talent + sinChar->Dexterity) / 50) + sinLev_Damage[0];
		sinChar->Attack_Damage[1] = 2 + ((sinChar->Strength + 200) / 200) + ((sinChar->Talent + sinChar->Dexterity) / 45) + sinLev_Damage[1];
	}

	////(������1)
	if ((sinDamageFunction[0] == 1 && (InvenItem[sInven[0].ItemIndex - 1].WeaponClass == 1 || InvenItem[sInven[0].ItemIndex - 1].WeaponClass == 3))) {
		sinChar->Attack_Damage[0] = 1 + (sinAttack_Damage[0] * (sinChar->Strength + 130) / 130) +
			((sinChar->Talent + sinChar->Dexterity) / 40) + sinLev_Damage[0] + sinSkillDamage[0] + sinCharItemDamage + ((sinAttack_Damage[0] + sinAttack_Damage[1]) / 16);
		sinChar->Attack_Damage[1] = 3 + (sinAttack_Damage[1] * (sinChar->Strength + 130) / 130) +
			((sinChar->Talent + sinChar->Dexterity) / 40) + sinLev_Damage[1] + sinSkillDamage[1] + sinCharItemDamage;
	}
	if ((sinDamageFunction[0] == 2 && (InvenItem[sInven[0].ItemIndex - 1].WeaponClass == 1 || InvenItem[sInven[0].ItemIndex - 1].WeaponClass == 3))) {
		////(������2)
		sinChar->Attack_Damage[0] = 1 + (sinAttack_Damage[0] * (sinChar->Strength + 150) / 150) +
			((sinChar->Talent + sinChar->Dexterity) / 45) + sinLev_Damage[0] + sinSkillDamage[0] + sinCharItemDamage + ((sinAttack_Damage[0] + sinAttack_Damage[1]) / 16);
		sinChar->Attack_Damage[1] = 3 + (sinAttack_Damage[1] * (sinChar->Strength + 150) / 150) +
			((sinChar->Talent + sinChar->Dexterity) / 45) + sinLev_Damage[1] + sinSkillDamage[1] + sinCharItemDamage;
	}
	if ((sinDamageFunction[0] == 3 && (InvenItem[sInven[0].ItemIndex - 1].WeaponClass == 1 || InvenItem[sInven[0].ItemIndex - 1].WeaponClass == 3))) {
		////(������3)
		sinChar->Attack_Damage[0] = 1 + (sinAttack_Damage[0] * (sinChar->Strength + 190) / 190) +
			((sinChar->Talent + sinChar->Dexterity) / 50) + sinLev_Damage[0] + sinSkillDamage[0] + sinCharItemDamage + ((sinAttack_Damage[0] + sinAttack_Damage[1]) / 16);
		sinChar->Attack_Damage[1] = 3 + (sinAttack_Damage[1] * (sinChar->Strength + 190) / 190) +
			((sinChar->Talent + sinChar->Dexterity) / 50) + sinLev_Damage[1] + sinSkillDamage[1] + sinCharItemDamage;
	}

	if (sinDamageFunction[1] == 1 && InvenItem[sInven[0].ItemIndex - 1].WeaponClass == 2) {
		////(�߻���1)
		sinChar->Attack_Damage[0] = 1 + (sinAttack_Damage[0] * (sinChar->Dexterity + 130) / 130) +
			((sinChar->Talent + sinChar->Strength) / 40) + sinLev_Damage[0] + sinSkillDamage[0] + sinCharItemDamage + ((sinAttack_Damage[0] + sinAttack_Damage[1]) / 16);
		sinChar->Attack_Damage[1] = 3 + (sinAttack_Damage[1] * (sinChar->Dexterity + 130) / 130) +
			((sinChar->Talent + sinChar->Strength) / 40) + sinLev_Damage[1] + sinSkillDamage[1] + sinCharItemDamage;
	}
	if (sinDamageFunction[1] == 2 && InvenItem[sInven[0].ItemIndex - 1].WeaponClass == 2) {
		////(�߻���2)
		sinChar->Attack_Damage[0] = 1 + (sinAttack_Damage[0] * (sinChar->Dexterity + 190) / 190) +
			((sinChar->Talent + sinChar->Strength) / 50) + sinLev_Damage[0] + sinSkillDamage[0] + sinCharItemDamage + ((sinAttack_Damage[0] + sinAttack_Damage[1]) / 16);
		sinChar->Attack_Damage[1] = 3 + (sinAttack_Damage[1] * (sinChar->Dexterity + 190) / 190) +
			((sinChar->Talent + sinChar->Strength) / 50) + sinLev_Damage[1] + sinSkillDamage[1] + sinCharItemDamage;
	}

	if (sinDamageFunction[2] == 1 && InvenItem[sInven[0].ItemIndex - 1].WeaponClass == 3) {
		////(������1)
		sinChar->Attack_Damage[0] = 1 + (sinAttack_Damage[0] * (sinChar->Spirit + 150) / 150) +
			((sinChar->Talent) / 30) + sinLev_Damage[0] + sinSkillDamage[0] + sinCharItemDamage + ((sinAttack_Damage[0] + sinAttack_Damage[1]) / 16);
		sinChar->Attack_Damage[1] = 3 + (sinAttack_Damage[1] * (sinChar->Spirit + 150) / 150) +
			((sinChar->Talent) / 30) + sinLev_Damage[1] + sinSkillDamage[1] + sinCharItemDamage;
	}
	if (sinDamageFunction[2] == 2 && InvenItem[sInven[0].ItemIndex - 1].WeaponClass == 3) {
		////(������2)
		sinChar->Attack_Damage[0] = 1 + (sinAttack_Damage[0] * (sinChar->Spirit + 170) / 170) +
			((sinChar->Talent) / 30) + sinLev_Damage[0] + sinSkillDamage[0] + sinCharItemDamage + ((sinAttack_Damage[0] + sinAttack_Damage[1]) / 16);
		sinChar->Attack_Damage[1] = 3 + (sinAttack_Damage[1] * (sinChar->Spirit + 170) / 170) +
			((sinChar->Talent) / 30) + sinLev_Damage[1] + sinSkillDamage[1] + sinCharItemDamage;
	}
	sinChar->Attack_Damage[0] += 1;
	sinChar->Attack_Damage[1] += 1;

	// Pedido chars melee aqui
	/*
	if (sinChar->Health > 30 && sinChar->JOB_CODE == JOBCODE_MECHANICIAN || sinChar->JOB_CODE == JOBCODE_FIGHTER
		|| sinChar->JOB_CODE == JOBCODE_PIKEMAN || sinChar->JOB_CODE == JOBCODE_KNIGHT)
	{
		sinChar->Attack_Damage[0] -= (sinChar->Health);
		sinChar->Attack_Damage[1] -= (sinChar->Health);
	} */

	if (ViewDamagePercent) {
		sinTempDamage2[0] += (sinChar->Attack_Damage[0] * ViewDamagePercent) / 100;
		sinTempDamage2[1] += (sinChar->Attack_Damage[1] * ViewDamagePercent) / 100;
	}
	else if (ViewDamageNum) {
		sinTempDamage2[0] += ViewDamageNum;
		sinTempDamage2[1] += ViewDamageNum;
	}

	sinChar->Critical_Hit = sinCritical + sinAdd_Critical_Hit;

	if (sinChar->Critical_Hit > 50)sinChar->Critical_Hit = 50;
	fstrength = (float)sinChar->Strength;
	fhealth = (float)sinChar->Health;
	ftalent = (float)sinChar->Talent;
	fSpirit = (float)sinChar->Spirit;

	sinChar->Defence = (int)((fDexterity / 2) + (ftalent / 4) + fLevel * 1.4 + sinDefense) + sinAdd_Defence;
	fDefense = (float)sinChar->Defence;

	CountAbsorbItem /= 2;
	if (CountAbsorbItem > 2) CountAbsorbItem = 2;
	sinChar->Absorption = (int)((fDefense / 100 + (fLevel / 10) + sinAbsorption + sinAdd_fAbsorb) + ((fstrength + ftalent) / 40)) + 1 + CountAbsorbItem;
	sinChar->Shooting_Range = sinShooting_Range + sinAdd_Shooting_Range;

	sinChar->Sight = sinSight + 180;
	sinChar->Chance_Block = (int)(sinBlock_Rate + sinAdd_fBlock_Rating);

	// Formula do peso xxstr
	{
		short sWeightBase = (short)((fstrength * 2) + (fhealth * 1.5) + (fLevel * 3) + 60 + fSpirit + fDexterity);

		// Premium para aumentar o peso
		if (chaPremiumitem.m_MightofAwellTime > 0)
			sinChar->Weight[1] = (short)(sWeightBase + chaPremiumitem.m_MightofAwellWeight);
		else
			sinChar->Weight[1] = sWeightBase;

		// Premium est� ativo?
		if (sinChar->GravityScroolCheck[0] > 0)
			sinChar->Weight[1] = (short)sWeightBase + ((sinChar->GravityScroolCheck[0]) * 50);

		// Atualizar o peso do personagem
		fMaxWeight = (float)sinChar->Weight[1];
		fNowWeight = (float)sinChar->Weight[0];
	}

	fAttack_Rating = (float)sinChar->Attack_Rating;

	int Accuracy_Temp;
	while (1) {
		if (Accuracy_Table[cnt][0] == 0)break;
		Accuracy_Temp = sinChar->Attack_Rating;
		if (Accuracy_Temp > Accuracy_Table[cnt][0] && Accuracy_Temp < Accuracy_Table[cnt + 1][0]) {
			sinChar->Accuracy = Accuracy_Table[cnt + 1][1];
			break;

		}
		cnt++;
	}

	sinChar->Move_Speed = (int)((((((ftalent + fhealth + fLevel + 60) / 150) - (fNowWeight / fMaxWeight))) + sinMoveSpeed) + sinAdd_fSpeed + TempMoveSpeed);
	sinChar->Move_Speed += 1;

	//Formula de HP
	switch (sinChar->JOB_CODE)
	{
	case JOBCODE_FIGHTER:
		sinChar->Life[1] = (short)((fSpirit * 0.1) + (ftalent * 0.2) + (fDexterity * 0.1) + (fLevel * 2.1) + (fhealth * 1.0) + (fstrength * 0.5));
		sinChar->Life[1] += (short)(sinLev_Life + sinfIncreLife);
		break;
	case JOBCODE_MECHANICIAN:
		sinChar->Life[1] = (short)((fSpirit * 0.1) + (ftalent * 0.2) + (fDexterity * 0.1) + (fLevel * 2.0) + (fhealth * 1.0) + (fstrength * 0.5));
		sinChar->Life[1] += (short)(sinLev_Life + sinfIncreLife);
		break;
	case JOBCODE_KNIGHT:
		sinChar->Life[1] = (short)((fSpirit * 0.2) + (ftalent * 0.3) + (fDexterity * 0.1) + (fLevel * 2.0) + (fhealth * 1.0) + (fstrength * 0.5));
		sinChar->Life[1] += (short)(sinLev_Life + sinfIncreLife);
		break;
	case JOBCODE_PIKEMAN:
		sinChar->Life[1] = (short)((fSpirit * 0.1) + (ftalent * 0.1) + (fDexterity * 0.2) + (fLevel * 2.0) + (fhealth * 1.0) + (fstrength * 0.4));
		sinChar->Life[1] += (short)(sinLev_Life + sinfIncreLife);
		break;
	case JOBCODE_PRIESTESS:
		sinChar->Life[1] = (short)((fSpirit * 0.3) + (ftalent * 0.2) + (fDexterity * 0.1) + (fLevel * 1.5) + (fhealth * 2.8) + (fstrength * 0.1));
		sinChar->Life[1] += (short)(sinLev_Life + sinfIncreLife);
		break;
	case JOBCODE_MAGICIAN:
		sinChar->Life[1] = (short)((fSpirit * 0.2) + (ftalent * 0.2) + (fDexterity * 0.1) + (fLevel * 1.5) + (fhealth * 2.4) + (fstrength * 0.1));
		sinChar->Life[1] += (short)(sinLev_Life + sinfIncreLife);
		break;
	case JOBCODE_ATALANTA:
		sinChar->Life[1] = (short)((fSpirit * 0.2) + (ftalent * 0.1) + (fDexterity * 0.1) + (fLevel * 1.2) + (fhealth * 3.0) + (fstrength * 0.3));
		sinChar->Life[1] += (short)(sinLev_Life + sinfIncreLife);
		break;
	case JOBCODE_ARCHER:
		sinChar->Life[1] = (short)((fSpirit * 0.1) + (ftalent * 0.2) + (fDexterity * 0.5) + (fLevel * 1.2) + (fhealth * 2.0) + (fstrength * 0.2));
		sinChar->Life[1] += (short)(sinLev_Life + sinfIncreLife);
		break;
	}

	/*switch(sinChar->LifeFunction){
		case 1:
			sinChar->Life[1] = (short)(fLevel * 2.1 +((fhealth*2.4)+(fstrength*0.8)));
			sinChar->Life[1] += (short)(sinLev_Life+sinfIncreLife);
		break;
		case 2:
			sinChar->Life[1] = (short)(fLevel * 2.1 +((fhealth*2.2)+(fstrength*0.6)));
			sinChar->Life[1] += (short)(sinLev_Life+sinfIncreLife);
		break;
		case 3:
			sinChar->Life[1] = (short)(fLevel * 1.8 +((fhealth*6.0)+(fstrength*0.4)));
			sinChar->Life[1] += (short)(sinLev_Life+sinfIncreLife);
		break;
		case 4:
			sinChar->Life[1] = (short)((fLevel *2.8) + (fhealth *2.8));
			sinChar->Life[1] += (short)(sinLev_Life+sinfIncreLife);
		break;
		case 5:
			sinChar->Life[1] = (short)((fLevel *1.8) + (fhealth *2.0));
			sinChar->Life[1] += (short)(sinLev_Life+sinfIncreLife);
		break;

	}*/

	// Item de quest +15HP
	if (!sinQuest_ChangeJob3.CODE && !SearchItemCode(0, ITEM_KIND_QUEST_WEAPON) &&
		sinChar->ChangeJob >= 2 && sinChar->Level >= 40) {
		sinChar->Life[1] += 15;
	}

	// Quest do lvl 90 + 40HP
	if ((sinQuest_levelLog & QUESTBIT_LEVEL_90) == QUESTBIT_LEVEL_90) {
		if (sinChar->Level >= 90) {
			sinChar->Life[1] += 40;
		}

	}

	switch (sinChar->ManaFunction) {
	case 1:
		sinChar->Mana[1] = (short)((fLevel * 1.5f + (fSpirit * 3.8f)));
		sinChar->Mana[1] += (short)(sinfIncreMana);
		break;
	case 2:
		sinChar->Mana[1] = (short)(sinChar->Level * 0.9f + (sinChar->Spirit * 2.7f));
		sinChar->Mana[1] += (short)(sinLev_Mana + sinfIncreMana);
		break;
	case 3:
		sinChar->Mana[1] = (short)(sinChar->Level * 0.6f + (sinChar->Spirit * 2.2f));
		sinChar->Mana[1] += (short)(sinLev_Mana + sinfIncreMana);
		break;

	}

	sinChar->Stamina[1] = (short)((fhealth * 1.4) + (fstrength + ftalent) / 2 + (fLevel * 2.3) + 80 + fSpirit);
	sinChar->Stamina[1] += ((short)(sinfIncreStamina));

	// Regen aqui
	sinChar->Life_Regen = sinfRegenLife + sinPer_Life_Regen;
	sinChar->Mana_Regen = sinfRegenMana + sinPer_Mana_Regen;
	sinChar->Stamina_Regen = sinfRegenStamina + sinPer_Stamina_Regen;

	if (sinChar->Life_Regen > 35)
	{
		PacketLogCheat sPacket;
		sPacket.iLength = sizeof(PacketLogCheat);
		sPacket.iHeader = 0x6F6A0001;
		sPacket.iCheatID = 8961;
		sPacket.SParam = 97;

		if (smWsockDataServer)
		{
			smWsockDataServer->Send((char*)&sPacket, sPacket.iLength, TRUE);
		}
	}

	if (sinChar->Mana_Regen > 35)
	{
		PacketLogCheat sPacket;
		sPacket.iLength = sizeof(PacketLogCheat);
		sPacket.iHeader = 0x6F6A0001;
		sPacket.iCheatID = 8961;
		sPacket.SParam = 96;

		if (smWsockDataServer)
		{
			smWsockDataServer->Send((char*)&sPacket, sPacket.iLength, TRUE);
		}
	}

	if (sinChar->Stamina_Regen > 35)
	{
		PacketLogCheat sPacket;
		sPacket.iLength = sizeof(PacketLogCheat);
		sPacket.iHeader = 0x6F6A0001;
		sPacket.iCheatID = 8961;
		sPacket.SParam = 95;

		if (smWsockDataServer)
		{
			smWsockDataServer->Send((char*)&sPacket, sPacket.iLength, TRUE);
		}
	}

	// Velocidade de ataque aqui
	sinChar->Attack_Speed = sinWeaponSpeed + sinAdd_Attack_Speed + sinSkillWeaponSpeed;

	if (sinChar->Attack_Speed > 30)
	{
		PacketLogCheat sPacket;
		sPacket.iLength = sizeof(PacketLogCheat);
		sPacket.iHeader = 0x6F6A0001;
		sPacket.iCheatID = 8965;
		sPacket.SParam = sinChar->Attack_Speed;

		if (smWsockDataServer)
		{
			smWsockDataServer->Send((char*)&sPacket, sPacket.iLength, TRUE);
		}
	}

	for (int i = 1; i < MAX_USESKILL; i++)
	{
		if (sinSkill.UseSkill[i].Flag && sinSkill.UseSkill[i].Point) {
			switch (sinSkill.UseSkill[i].CODE) {
			case SKILL_PHYSICAL_TRANING:
				TempSkillData = (((float)P_Traning_Stamina[sinSkill.UseSkill[i].Point - 1]) * (float)sinChar->Stamina[1] / 100.0f);
				sinChar->Stamina[1] += (short)TempSkillData;
				break;
			case SKILL_FIRE_JAVELIN:
				sElement_Attack.Fire[0] += Fire_Javelin_Damage[sinSkill.UseSkill[i].Point - 1][0];
				sElement_Attack.Fire[1] += Fire_Javelin_Damage[sinSkill.UseSkill[i].Point - 1][1];
				break;
			case SKILL_MEDITATION:
				sinChar->Mana_Regen += Meditation_Regen[sinSkill.UseSkill[i].Point - 1];
				break;
			case SKILL_MENTAL_MASTERY:
				TempSkillData = (float)((float)(Mental_Mastery_Mana[sinSkill.UseSkill[i].Point - 1]) * ((float)sinChar->Mana[1] / 100.0f));
				sinChar->Mana[1] += (short)TempSkillData;
				break;
			case SKILL_BOOST_HEALTH:
				sinChar->Life[1] += (short)Boost_Health_Life[sinSkill.UseSkill[i].Point - 1];

				break;

			}
		}
	}

	SkillTextColor[SIN2_DEFENSE] = SIN_TEXT_COLOR_WHITE;

	int OrbDamageTemp2 = 0;
	for (int i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (ContinueSkill[i].Flag) {
			switch (ContinueSkill[i].CODE) {
			case SKILL_HOLY_BODY:
				TempSkillData = ((float)HolyBody_Absorb[ContinueSkill[i].Point - 1] / 100.0f);
				sinUndeadAbsorb = (int)((float)sinChar->Absorption * TempSkillData);
				break;
			case SKILL_DRASTIC_SPIRIT:
				TempSkillData = ((float)Drastic_Spirit_Defense[ContinueSkill[i].Point - 1] / 100.0f);
				sinChar->Defence += (int)((float)sinChar->Defence * TempSkillData);
				SkillTextColor[SIN2_DEFENSE] = SIN_TEXT_COLOR_BLUE;
				break;
			case SKILL_SPARK_SHIELD: //����� �������ϰ� ���� ���� �������ش�
			case SKILL_METAL_ARMOR:
				if (MetalArmor) { //��ī�ϼ� Ưȭ�ƸӰ� ������쿡�� ����ȴ� 
					SkillTextColor[SIN2_DEFENSE] = SIN_TEXT_COLOR_BLUE;
				}
				break;
			case SKILL_CONCENTRATION:  //�ڽ��� ���߷��� ���õǱ⶧���� �ؿ��� ������ش�
				for (p = 0; p < 8; p++) { //�ٸ� ���ⱺ�� üũ�ȴ�   
					if (sInven[0].ItemIndex) {
						if (ContinueSkill[i].Skill_Info.UseWeaponCode[p] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) {
							sinChar->Attack_Rating += Concentration_AttackRate[ContinueSkill[i].Point - 1];
							SkillTextColor[SIN2_ATTACK_RATE] = SIN_TEXT_COLOR_ORANGE;
							Flag3 = 1;
							break;
						}
					}

				}
				if (!Flag3) {
					//��ų��빫�Ⱑ �ƴϸ� ���븸�ȵǰ��Ѵ�
				//	ContinueSkill[i].CheckTime = ContinueSkill[i].UseTime*70; 
					SkillTextColor[SIN2_ATTACK_RATE] = SIN_TEXT_COLOR_WHITE;
				}
				break;
			case SKILL_FORCE_ORB:
				// ����� - ���� ����, ���� ���� ���� �߰��� ĳ���� ����â�� �������� ���ݷ�(���÷��̽ÿ��� ������ ������)
				if (ContinueSkill[i].ElementIndex == 1000) // ���� ���� ���� ������϶�
				{
					// fo135 ~ fo137 // ���� ���� ����
					OrbDamageTemp2 = (sinChar->Attack_Damage[0] + ((sinChar->Attack_Damage[1] - sinChar->Attack_Damage[0]) / 2)) / 15;
					sinTempDamage2[0] += OrbDamageTemp2;
					sinTempDamage2[1] += OrbDamageTemp2;
				}
				else // �Ϲ� ���� ���� ������϶� // �庰 - ���� ���丣��, �̴ϱ׸� ���� ����
				{
					// fo107 ~ fo110 // ��Ű ���� ~ �̶��� ���� - ���ݷ°��� 10% 
					if (ContinueSkill[i].Point >= 7 && ContinueSkill[i].Point <= 10)
					{
						OrbDamageTemp2 = (sinChar->Attack_Damage[0] + ((sinChar->Attack_Damage[1] - sinChar->Attack_Damage[0]) / 2)) / 10;
						sinTempDamage2[0] += OrbDamageTemp2;
						sinTempDamage2[1] += OrbDamageTemp2;
					}
					else if (ContinueSkill[i].Point == 11) // ���丣�� ����
					{
						OrbDamageTemp2 = (((sinChar->Attack_Damage[1] + sinChar->Attack_Damage[0]) / 2) * 15) / 100;
						sinTempDamage2[0] += OrbDamageTemp2;
						sinTempDamage2[1] += OrbDamageTemp2;
					}
					else if (ContinueSkill[i].Point >= 12) // �̴ϱ׸� ����
					{
						OrbDamageTemp2 = (((sinChar->Attack_Damage[1] + sinChar->Attack_Damage[0]) / 2) * 20) / 100;
						sinTempDamage2[0] += OrbDamageTemp2;
						sinTempDamage2[1] += OrbDamageTemp2;
					}
					sinTempDamage2[0] += ForceOrbDamage[ContinueSkill[i].Point - 1];
					sinTempDamage2[1] += ForceOrbDamage[ContinueSkill[i].Point - 1];
				}
				break;
			case SKILL_REGENERATION_FIELD:
				sinChar->Mana_Regen += Regeneration_Field_ManaRegen[ContinueSkill[i].Point - 1] / ContinueSkill[i].Flag;
				sinChar->Life_Regen += Regeneration_Field_LifeRegen[ContinueSkill[i].Point - 1];

				break;
			case SKILL_BERSERKER:
				SkillTextColor[SIN2_ABSORB] = SIN_TEXT_COLOR_RED;
				break;
			case SKILL_EVASION_MASTERY:
				if (lpCurPlayer->PlayEvasionMastery)
				{
					sinChar->Attack_Rating -= (int)(sinChar->Attack_Rating * Evasion_Mastery_AddReduce[ContinueSkill[i].Point - 1]) / 100;
					SkillTextColor[SIN2_ATTACK_RATE] = SIN_TEXT_COLOR_RED;
				}
				break;
			case SCROLL_INVULNERABILITY:
			case SCROLL_P_INVULNERABILITY:
				SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_RED;
				break;
			case CLANSKILL_ATTACK:
				sinTempDamage2[0] += (int)(sinChar->Attack_Damage[0] * 8) / 100;
				sinTempDamage2[1] += (int)(sinChar->Attack_Damage[1] * 8) / 100;
				SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_GOLD;
				break;


			}
		}
	}

	if (chaPremiumitem.m_HelpPetTime > 0 && sinChar->GravityScroolCheck[1] == 2)
	{
		sinTempDamage2[0] += (sinTempDamage2[0] * 10) / 100;
		sinTempDamage2[1] += (sinTempDamage2[1] * 10) / 100;
	}

	//90LV QUEST 90 xxstr 
	if (sinQuest_Level90_2.CODE && sinQuest_Level90_2.State >= 2) {
		sinTempDamage2[0] = (sinTempDamage2[0] * 70) / 100;
		sinTempDamage2[1] = (sinTempDamage2[1] * 70) / 100;

		sinChar->Attack_Damage[0] = (sinChar->Attack_Damage[0] * 70) / 100;
		sinChar->Attack_Damage[1] = (sinChar->Attack_Damage[1] * 70) / 100;

	}

	/////////////////////////////////////////////
	CheckRequireItem(); //����ɼ� �ִ� ������������ üũ�Ѵ� 

	ReformCharForm();   //������ 

	//���� ���������� üũ�Ѵ� 
	ReSettingPotion();



	//���÷��̿� ī�� 
	if (!CheckHackFlag)
		memcpy(&sinCharDisplay, sinChar, sizeof(smCHAR_INFO));
	else //��ŷ����
		HackCharStateCheck(sinChar, &sinCharDisplay);

	return TRUE;
}

//ĳ���� ������ �о�´� 
void cINVENTORY::LoadCharState()
{

	if (!sinChar->ArrowPosi[0])sinChar->ArrowPosi[0] = 1; //�κ� AB������ ȭ��ǥ ǥ�ð��� ������ �⺻�����Ѵ� 
	if (!sinChar->ArrowPosi[1])sinChar->ArrowPosi[1] = 3;

	//ȭ��ǥ ��ġ ���� 
	if (sinChar->ArrowPosi[0] == 1)
		ArrowState[0] = 0;
	if (sinChar->ArrowPosi[0] == 2)
		ArrowState[0] = 1;
	if (sinChar->ArrowPosi[1] == 3)
		ArrowState[1] = 0;
	if (sinChar->ArrowPosi[1] == 4)
		ArrowState[1] = 1;

	OldLevel = sinChar->Level;
	OldLevelTemp = OldLevel + 176;
	sinLoadOK = 1;

}

//�������� �ٲ۴� 
int cINVENTORY::ChangeABItem(int Posi)
{
	SetDelayFlag = 1; //AB ü������ �ϸ� ������ �÷��� �ش� 

	//if(cTrade.OpenFlag)return TRUE; //��ġ�� ����!! 
	if (NotChangeSetItemFlag)return TRUE;
	if (cMyShop.OpenFlag)return FALSE;
	if (MyShopSendButton)return FALSE;
	if (ChangeSpecialItem(1))return TRUE; //����Ʈ�������� ����������쿡�� ĵ����Ų��

	if (cWareHouse.OpenFlag || Caravana::GetInstance()->OpenFlag) {
		//���� �ϴ��
		//â�� �������� �κ��丮�� �ű�ÿ� �κ����� �ٲܼ� ����./
		if (MouseItem.Flag)
			return TRUE;  //â�� ����������쿡�� ĵ����Ų��
	}

	if (sMessageBox3[MESSAGE_SELL_HIGHRANK_ITEM].Flag)return TRUE; //�������ȱ� �޼����ڽ��������ܿ�� ����
	if (SpecialItemShowFlag)return TRUE;

	//	if( cSinHelp.sinShowHelp()) 
	CheckInvenItemForm();

	int desPosi;
	int i, j, t;
	//============ �ڷ���Ʈ�ھ� ���ѻ���� ���´� (����060612)
	//�ڷ���Ʈ����â�� �������� �κ� AB ����
	if (sMessageBox3[MESSAGE_TELEPORT_CORE].Flag)return TRUE;
	for (i = 0; i < SIN_MAX_HELP_NUMBER; i++)
	{
		if (sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT_SCROLL)
		{
			return TRUE;
		}

	}

	// pluto ���� ABü���� ���´�
	if (sinCraftItemStartFlag || StartInterEffectAging2)
	{
		return TRUE;
	}
	else if ((ManufactureItem.m_OpenFlag || SmeltingItem.OpenFlag) && MixCancelButtonDelayFlag)
	{
		return TRUE;
	}

	//============== ���ۻ���� ABü���� ���´�

	if (Posi <= 2)desPosi = 1;
	if (Posi > 2)desPosi = 2;

	if (Posi == 1) {
		ArrowState[0] = 0;
		sinChar->ArrowPosi[0] = 1;

	}
	if (Posi == 2) {
		ArrowState[0] = 1;
		sinChar->ArrowPosi[0] = 2;

	}
	if (Posi == 3) {
		ArrowState[1] = 0;
		sinChar->ArrowPosi[1] = 3;

	}
	if (Posi == 4) {
		ArrowState[1] = 1;
		sinChar->ArrowPosi[1] = 4;

	}

	switch (desPosi) {
	case 1: //Box
		memcpy(&BackUpInvenItemTemp, &InvenItemTemp, sizeof(sITEM) * 80); //��� �������� ����Ѵ� 
		for (t = 0; t < 80; t++)InvenItemTemp[t].Flag = 0;
		for (i = 0; i < 80; i++) { //Temp�� ���� 
			if (InvenItem[i].Flag) {
				if (InvenItem[i].ItemPosition == 0) {
					for (j = 0; j < 80; j++) {
						if (!InvenItemTemp[j].Flag) {
							memcpy(&InvenItemTemp[j], &InvenItem[i], sizeof(sITEM));
							InvenItem[i].Flag = 0;
							break;
						}
					}
				}
			}
		}
		for (i = 0; i < 80; i++) { //�κ��丮 ���������� ���� 
			if (BackUpInvenItemTemp[i].Flag) {
				for (j = 0; j < 80; j++) {
					if (!InvenItem[j].Flag) {
						memcpy(&InvenItem[j], &BackUpInvenItemTemp[i], sizeof(sITEM));
						BackUpInvenItemTemp[i].Flag = 0;
						break;

					}

				}

			}

		}

		break;

	case 2: //���� ������ 
		ContinueSkillCancelFlag = 1;
		memcpy(&BackUpInvenItemTemp[80], &InvenItemTemp[80], sizeof(sITEM) * 2);
		for (t = 80; t < 82; t++)InvenItemTemp[t].Flag = 0;
		for (i = 0; i < 80; i++) {
			if (InvenItem[i].Flag) {
				if (InvenItem[i].ItemPosition == 1 || InvenItem[i].ItemPosition == 2) {
					for (j = 80; j < 82; j++) {
						if (!InvenItemTemp[j].Flag) {
							memcpy(&InvenItemTemp[j], &InvenItem[i], sizeof(sITEM));
							InvenItem[i].Flag = 0;
							sInven[InvenItem[i].ItemPosition - 1].ItemIndex = 0;
							sinSetCharItem(InvenItem[i].CODE, InvenItem[i].SetModelPosi, FALSE);
							break;
						}
					}
				}
			}
		}

		for (i = 80; i < 82; i++) { //�κ��丮 ���������� ���� 
			if (BackUpInvenItemTemp[i].Flag) {
				for (j = 0; j < 80; j++) {
					if (!InvenItem[j].Flag) {
						memcpy(&InvenItem[j], &BackUpInvenItemTemp[i], sizeof(sITEM));
						BackUpInvenItemTemp[i].Flag = 0;
						sInven[InvenItem[j].ItemPosition - 1].ItemIndex = j + 1;
						sinSetCharItem(InvenItem[j].CODE, InvenItem[j].SetModelPosi, TRUE); //�������� �ٲܶ� 
						if (InvenItem[j].SoundIndex)
							sinPlaySound(InvenItem[j].SoundIndex);
						break;

					}

				}

			}

		}
		SetItemToChar();
		break;
	}

	//��� �������� �Ѽտ� ������쿡�� �����ش� 
	for (i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (InvenItem[i].ItemPosition) {
				//��վ������� �Ѽտ� ������쿡�� �����ش� 
				if (InvenItem[i].Class == ITEM_CLASS_WEAPON_TWO) {
					if (InvenItem[i].ItemPosition == 1) {//������ 
						if (InvenItem[i].sItemInfo.CODE != InvenItem[sInven[1].ItemIndex - 1].sItemInfo.CODE) {
							InvenItem[sInven[1].ItemIndex - 1].Flag = 0;
							InvenItem[i].Flag = 0;
							sInven[1].ItemIndex = 0; //�κ��丮 �ڽ� �ʱ�ȭ 
							sInven[0].ItemIndex = 0; //�κ��丮 �ڽ� �ʱ�ȭ 
							if (InvenItem[i].SetModelPosi)
								sinSetCharItem(InvenItem[i].CODE, InvenItem[i].SetModelPosi, FALSE);

						}
					}
					if (InvenItem[i].ItemPosition == 2) {//�޼� 
						if (InvenItem[i].sItemInfo.CODE != InvenItem[sInven[0].ItemIndex - 1].sItemInfo.CODE) {
							InvenItem[sInven[0].ItemIndex - 1].Flag = 0;
							InvenItem[i].Flag = 0;
							sInven[0].ItemIndex = 0; //�κ��丮 �ڽ� �ʱ�ȭ 
							sInven[1].ItemIndex = 0; //�κ��丮 �ڽ� �ʱ�ȭ 
							if (InvenItem[i].SetModelPosi)
								sinSetCharItem(InvenItem[i].CODE, InvenItem[i].SetModelPosi, FALSE);

						}
					}
				}
			}
		}
	}

	///////////'E'Ű������ �������� ������ ã������ ���ܿ� ���!!!
	if (MouseItem.Flag) {
		if (MouseItem.Class == ITEM_CLASS_POTION) {
			GhostPotionCheckFlag = 1;

		}
		else
			GhostPotionCheckFlag = 0;

	}
	else
		GhostPotionCheckFlag = 0;

	CheckRequireItem(); //�������� ������ �����Ѵ� (�䱸ġ ����)
	ReFormInvenItem(); //���� ���õȴ������� üũ���� �����Ѵ� 
	cInvenTory.CheckPuzzle(); //���� üũ
	return TRUE;
}

// change costume xxstr
int cINVENTORY::ChangeACItem(bool Costume)
{
	SetItemToChar();
	if (Costume)
	{
		int Index = sInven[13].ItemIndex;

		if (Index)
		{
			sITEM& Item = InvenItem[Index - 1];
			sinSetCharItem(Item.CODE, Item.SetModelPosi, TRUE);
		}
		else
		{
			sinSetCharItem(sinDA1 | sin01, INVENTORY_POS_ARMOR, TRUE); //sinSetCharItem(NULL, INVENTORY_POS_ARMOR, TRUE); sinSetCharItem(sinDA1|sin01, INVENTORY_POS_ARMOR, TRUE);
		}
	}
	else
	{
		int Index = sInven[2].ItemIndex;

		if (Index)
		{
			sITEM& Item = InvenItem[Index - 1];
			sinSetCharItem(Item.CODE, Item.SetModelPosi, TRUE);
		}
		else
		{
			sinSetCharItem(sinDA1 | sin01, INVENTORY_POS_ARMOR, TRUE); //sinSetCharItem(NULL, INVENTORY_POS_COSTUME, TRUE);
		}
	}
	CheckRequireItem();
	ReFormInvenItem();
	return TRUE;
}

// Pre�o de venda dos itens no NPC
sITEMPRICE cINVENTORY::GetInvenItemPrice(sITEM* pItem)
{
	sITEMPRICE sinItemPrice;

	if (pItem->Class == ITEM_CLASS_WEAPON_TWO) {
		if (pItem->ItemPosition) {
			pItem = &InvenItem[sInven[0].ItemIndex - 1];

		}
	}

	float Dur[2];
	float Price;

	Dur[0] = pItem->sItemInfo.Durability[0];
	Dur[1] = pItem->sItemInfo.Durability[1];
	Price = (float)pItem->sItemInfo.Price;
	if (Dur[0] == 0)Dur[0] = 1;
	if (Dur[1] == 0)Dur[1] = 1;

	// Pre�o do item no NPC
	sinItemPrice.PureSellPrice = (int)((Price * (Dur[0] / Dur[1])) + (Price - (Price * (Dur[0] / Dur[1]))) * 0.25);

	// Se for Gold bar, vende pelo mesmo pre�o
	if (pItem->sItemInfo.itemType == 3 || pItem->sItemInfo.itemType == 4 || pItem->sItemInfo.itemType == 5)
		sinItemPrice.SellPrice = (int)Price;
	else
		sinItemPrice.SellPrice = (int)sinItemPrice.PureSellPrice / 5;

	sinItemPrice.RepairCost = (pItem->sItemInfo.Price - sinItemPrice.PureSellPrice) / 8;

	return sinItemPrice;

}

int cINVENTORY::RepairInvenItem(sITEM* pItem, int RepairCost)
{
	if (pItem->Class == ITEM_CLASS_WEAPON_TWO) {
		if (pItem->ItemPosition) {
			pItem = &InvenItem[sInven[0].ItemIndex - 1];

		}
	}

	CheckCharForm();//���� 
	//ReformCharForm();//������
	if (!pItem->sItemInfo.Durability[1])return FALSE;
	if (sinChar->Money - RepairCost > 0) {
		if (pItem->sItemInfo.Durability[0] < pItem->sItemInfo.Durability[1]) {
			//sinChar->Money -= RepairCost;
			sinMinusMoney(RepairCost);
			pItem->sItemInfo.Durability[0] = pItem->sItemInfo.Durability[1];
			ReformCharForm();//������ 
			SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
			return TRUE;
		}
	}
	return FALSE;

}

//���Ը� üũ�Ѵ� 
void cINVENTORY::CheckWeight()
{

	CheckCharForm();//����

	sinChar->Weight[0] = 0; //���� �ʱ�ȭ �� �ٽ� ���� 
	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (InvenItem[i].Class == ITEM_CLASS_POTION) {
				if (InvenItem[i].sItemInfo.PotionCount < 0)continue;
				sinChar->Weight[0] += InvenItem[i].sItemInfo.PotionCount;
				continue;

			}
			if (InvenItem[i].sItemInfo.Weight < 0)continue;
			sinChar->Weight[0] += InvenItem[i].sItemInfo.Weight;
		}
	}
	for (int j = 0; j < INVENTORY_MAXITEM; j++) {
		if (InvenItemTemp[j].Flag) {
			if (InvenItemTemp[j].Class == ITEM_CLASS_POTION) {
				if (InvenItemTemp[j].sItemInfo.PotionCount < 0)continue;
				sinChar->Weight[0] += InvenItemTemp[j].sItemInfo.PotionCount;
				continue;

			}
			if (InvenItemTemp[j].sItemInfo.Weight < 0)continue;
			sinChar->Weight[0] += InvenItemTemp[j].sItemInfo.Weight;
		}
	}

	ReformCharForm();//������ 

}

// �����Ҽ��ִ����� üũ�Ѵ� 
int cINVENTORY::CheckSetOk(sITEM* pItem, int AutoFlag)
{
	if (pItem->Class == ITEM_CLASS_POTION) {
		if (sinChar->Weight[0] + pItem->sItemInfo.PotionCount > sinChar->Weight[1]) { //���Ը� �Ѿ�� 
			cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
			sinOverWeightCnt++;
			return FALSE;
		}

	}
	else {
		if (sinChar->Weight[0] + pItem->sItemInfo.Weight > sinChar->Weight[1]) { //���Ը� �Ѿ�� 
			if (pItem->sItemInfo.ItemKindCode != ITEM_KIND_QUEST_WEAPON) {
				cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
				sinOverWeightCnt++;
				return FALSE;
			}
		}
	}
	if (!AutoFlag) { //��������� �ƴ� ��쿡�� üũ�Ѵ� 
		if (pItem->ItemPosition) {
			if (pItem->sItemInfo.NotUseFlag) {
				cMessageBox.ShowMessage(MESSAGE_NO_USE_ITEM);
				return FALSE; //�޼����� ������ش� â��� ��������� �ȴ� 
			}
		}
	}
	return TRUE;
}


//����� �������� ���ش�  
int cINVENTORY::CopyItemClear(sITEM* pItem)
{ //���콺�� ����ִ� �����۰� ���� �������� �ִ����� �˻��Ѵ�

	int CopyItemCnt = 0;
	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {		//�ո� �κ��丮 
			if (CompareItems(&pItem->sItemInfo, &InvenItem[i].sItemInfo)) {
				//����� �������� �����ش� 
				InvenItem[i].Flag = 0;
				if (InvenItem[i].ItemPosition) {
					sInven[InvenItem[i].ItemPosition - 1].ItemIndex = 0;
					sinSetCharItem(InvenItem[i].CODE, InvenItem[i].SetModelPosi, FALSE); //����� ������ �����Ѵ� 
					ClearTwoHandPosiAndItem(&InvenItem[i]);//���ڵ� �����̸� ���ڵ� ���⸦ �����ش� 

				}
				CopyItemCnt++;


			}

		}
		if (InvenItemTemp[i].Flag) {	//�޸� �κ��丮 
			if (CompareItems(&pItem->sItemInfo, &InvenItemTemp[i].sItemInfo)) {//����� �������� �����ش� 
				InvenItemTemp[i].Flag = 0;
				if (InvenItemTemp[i].Class == ITEM_CLASS_WEAPON_TWO) {
					if (InvenItemTemp[i].ItemPosition == 1 || InvenItemTemp[i].ItemPosition == 2) {
						for (int k = 0; k < INVENTORY_MAXITEM; k++) {
							if (InvenItemTemp[k].Flag) {
								if (InvenItemTemp[k].ItemPosition == 1 || InvenItemTemp[k].ItemPosition == 2) {
									if (InvenItemTemp[i].sItemInfo.CODE == InvenItemTemp[k].sItemInfo.CODE) {
										InvenItemTemp[k].Flag = 0;
									}
								}
							}
						}
					}
				}
				CopyItemCnt++;
			}
		}
	}
	if (CopyItemCnt) {
		SendCopyItemUser(CopyItemCnt); //������ ī�ǵ� �������� ������ ������ 
		CopyItemCnt = 0;
	}
	return TRUE;

}

POINT ResultSize;
//�׸��� ����� ���ؼ� �����Ѵ� 
POINT cINVENTORY::CheckImageSize(LPDIRECT3DTEXTURE9 lpdd)
{
	ResultSize.x = 0;
	ResultSize.y = 0;
	D3DSURFACE_DESC	ddTemp;
	memset(&ddTemp, 0, sizeof(D3DSURFACE_DESC));
	lpdd->GetLevelDesc(0, &ddTemp);
	ResultSize.y = ddTemp.Height;
	ResultSize.x = ddTemp.Width;
	return ResultSize;
}

//�ӽ÷� ������ �̹����� üũ�ϱ����ؼ� ���δ� 
int cINVENTORY::ImsiCheckItemImageSize()
{
	sITEM TestItem;
	POINT TestPoint = { 0,0 };
	char szFilePath[256];
	sinTestBuff[0] = 0;
	for (int j = 0; j < MAX_ITEM; j++) {
		wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[j].ItemFilePath, sItem[j].LastCategory);
		memcpy(&TestItem, &sItem[j], sizeof(sITEM));  //����ü�� ���ǵ� ������ �����Ѵ� 
		TestItem.lpItem = LoadDibSurfaceOffscreen(szFilePath);
		TestPoint.x = 0;
		TestPoint.y = 0;
		if (TestItem.lpItem) {
			TestPoint = CheckImageSize(TestItem.lpItem);
			if (TestPoint.x != sItem[j].w || TestPoint.y != sItem[j].h) {
				lstrcpy(sinTestBuff, sItem[j].LastCategory);
			}

		}

	}
	return TRUE;
}


//�߸��� ��ǥ�� �������ش� 
int cINVENTORY::CheckOverlapItem(sITEM* pItem, int PassItemIndex)
{
	int ReSetXYFlag = 0;
	if (!pItem)return FALSE;
	if (pItem->ItemPosition) {
		if (pItem->ItemPosition < 11) { //���� �������� �ƴϸ� 
			sInven[pItem->ItemPosition - 1].ItemIndex = 0; //������ �ʱ�ȭ 
			pItem->ItemPosition = 0;
			ReSetXYFlag = 1;
		}
		else
			ReSetXYFlag = 0;
	}

	RECT Rect;
	int  TempPosiFlag = 0;
	Rect.left = pItem->x;
	Rect.top = pItem->y;
	Rect.right = pItem->w;
	Rect.bottom = pItem->h;

	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;

	cx = (22 * INVENTORY_BOX_ROW) - pItem->w; //�������� �����ִ� ���� �� X��ǥ 
	cy = (22 * INVENTORY_BOX_COL) - pItem->h; //�������� �����ִ� ���� �� Y��ǥ

	//�ڵ尡 ���� �ʰų� �������� ������� ���� ���� �����Ҷ� �˻��ؼ� ��ġ�� ã�Ƴ��� 

	if (CrashInvenItem(Rect, PassItemIndex) || ReSetXYFlag) {
		for (cntx = StartX; cntx <= StartX + cx; cntx += 22) {
			for (cnty = StartY; cnty <= StartY + cy; cnty += 22) {
				flag = 0;
				for (int i = 0; i < INVENTORY_MAXITEM; i++) {
					if (InvenItem[i].Flag) {
						rect.left = InvenItem[i].x;
						rect.right = InvenItem[i].x + InvenItem[i].w;
						rect.top = InvenItem[i].y;
						rect.bottom = InvenItem[i].y + InvenItem[i].h;

						if (cntx > rect.left - pItem->w && cntx< rect.right &&
							cnty > rect.top - pItem->h && cnty < rect.bottom)
							flag++;
					}
				}
				if (flag == 0) {
					pItem->x = cntx;
					pItem->y = cnty;
					sInven[pItem->ItemPosition - 1].ItemIndex = 0;
					pItem->ItemPosition = 0;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

//�ӽ÷� �߸��� ��ǥ�� �������ش� 
int cINVENTORY::ReSetPotionXY(sITEM* pItem, int kind)
{

	RECT Rect;
	Rect.left = pItem->x;
	Rect.top = pItem->y;
	Rect.right = pItem->w;
	Rect.bottom = pItem->h;

	int cntx, cnty;
	int cx, cy;

	RECT	rect;
	int		flag;

	cx = (22 * INVENTORY_BOX_ROW) - pItem->w; //�������� �����ִ� ���� �� X��ǥ 
	cy = (22 * INVENTORY_BOX_COL) - pItem->h; //�������� �����ִ� ���� �� Y��ǥ

	//�ڵ尡 ���� �ʰų� �������� ������� ���� ���� �����Ҷ� �˻��ؼ� ��ġ�� ã�Ƴ��� 

	if (kind == 1) {
		for (cntx = StartX; cntx <= StartX + cx; cntx += 22) {
			for (cnty = StartY; cnty <= StartY + cy; cnty += 22) {
				flag = 0;
				for (int i = 0; i < INVENTORY_MAXITEM; i++) {
					if (InvenItemTemp[i].Flag) {
						rect.left = InvenItemTemp[i].x;
						rect.right = InvenItemTemp[i].x + InvenItemTemp[i].w;
						rect.top = InvenItemTemp[i].y;
						rect.bottom = InvenItemTemp[i].y + InvenItemTemp[i].h;

						if (cntx > rect.left - pItem->w && cntx< rect.right &&
							cnty > rect.top - pItem->h && cnty < rect.bottom)
							flag++;
					}
				}
				if (flag == 0) {
					pItem->x = cntx;
					pItem->y = cnty;
					//sInven[pItem->ItemPosition-2].ItemIndex =0; //���� �����������̶� �� ���д� 
					pItem->ItemPosition = 0;
					return TRUE;
				}

			}
		}
	}
	else {
		for (cntx = StartX; cntx <= StartX + cx; cntx += 22) {
			for (cnty = StartY; cnty <= StartY + cy; cnty += 22) {
				flag = 0;
				for (int i = 0; i < INVENTORY_MAXITEM; i++) {
					if (InvenItem[i].Flag) {
						rect.left = InvenItem[i].x;
						rect.right = InvenItem[i].x + InvenItem[i].w;
						rect.top = InvenItem[i].y;
						rect.bottom = InvenItem[i].y + InvenItem[i].h;

						if (cntx > rect.left - pItem->w && cntx< rect.right &&
							cnty > rect.top - pItem->h && cnty < rect.bottom)
							flag++;
					}
				}
				if (flag == 0) {
					pItem->x = cntx;
					pItem->y = cnty;
					sInven[pItem->ItemPosition - 2].ItemIndex = 0;
					pItem->ItemPosition = 0;
					return TRUE;
				}

			}
		}
	}
	return FALSE;

}

int cINVENTORY::UseEcore(sITEM* pItem)
{
	char szLinkName[24];
	int len = lstrlen(pItem->sItemInfo.ItemName);
	int partyFlag = 0, foundPlayerGroup = 0;
	if (sinItemPickUpDelayFlag)return FALSE;

	if (pItem->sItemInfo.CODE == (sinEC1 | sin01) && StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_ROOM)
	{
		cMessageBox.ShowMessage(MESSAGE_NO_USE_ITEM);
		return FALSE;
	}

	// Core de Party
	if (pItem->sItemInfo.CODE == (sinEC1 | sin04) || pItem->sItemInfo.CODE == (sinEC1 | sin05)) {

		if (pItem->sItemInfo.CODE == (sinEC1 | sin04)) {

			for (int cnt = 0; cnt < InterfaceParty.PartyMemberCount; cnt++) {
				if (InterfaceParty.PartyMember[cnt].ChrCode == lpCurPlayer->dwObjectSerial) {
					partyFlag = 1;
				}
			}

			if (pItem->sItemInfo.ItemName[len + 1] == NULL) {
				UnionCorePosi.x = pItem->x;
				UnionCorePosi.y = pItem->y;
				memset(&TempLinkCoreItem, 0, sizeof(sITEMINFO));
				memcpy(&TempLinkCoreItem, &pItem->sItemInfo, sizeof(sITEMINFO));
				cMessageBox.ShowMessage3(MESSAGE_LINKCORE, "");

			}
			else {
				if (stricmp(lpCurPlayer->smCharInfo.szName, szLinkName) == 0) return FALSE;

				if (partyFlag) {
					lstrcpy(szLinkName, &pItem->sItemInfo.ItemName[len + 1]);
					for (int cnt = 0; cnt < InterfaceParty.PartyMemberCount; cnt++) {
						if (stricmp(InterfaceParty.PartyMember[cnt].Name, szLinkName) == 0) {

							if (stricmp(InterfaceParty.PartyMember[cnt].Name, lpCurPlayer->smCharInfo.szName) == 0)
								foundPlayerGroup = 0;
							else
							{
								SendLinkCoreToServer(&pItem->sItemInfo);
								foundPlayerGroup = 1;
							}
						}
					}
				}
				else {
					TitleBox::GetInstance()->SetText("Voc� deve estar em grupo", 3);
				}

				if (partyFlag && !foundPlayerGroup) CHATGAMEHANDLE->AddChatBoxText("> Membro do grupo n�o encontrado!", EChatColor::CHATCOLOR_Error);
			}
		}

		// Core de Clan
		if (pItem->sItemInfo.CODE == (sinEC1 | sin05)) {
			if (sinChar->ClassClan) {
				if (cldata.stats[0] == '1') {
					if (pItem->sItemInfo.ItemName[len + 1] == NULL) {
						UnionCorePosi.x = pItem->x;
						UnionCorePosi.y = pItem->y;
						memset(&TempLinkCoreItem, 0, sizeof(sITEMINFO));
						memcpy(&TempLinkCoreItem, &pItem->sItemInfo, sizeof(sITEMINFO));
						cMessageBox.ShowMessage3(MESSAGE_LINKCORE, "");

					}
					else {
						lstrcpy(szLinkName, &pItem->sItemInfo.ItemName[len + 1]);
						if (stricmp(sinChar->szName, szLinkName) != 0) {
							SendLinkCoreToServer(&pItem->sItemInfo);
						}
					}
				}
				else {
					cMessageBox.ShowMessage(MESSAGE_USE_REGULAR_CLAN_ONLY);
				}
			}
			else {
				cMessageBox.ShowMessage(MESSAGE_USE_CLAN_ONLY);
			}
		}
	}
	else {
		CheckInvenItemForm();
		ActionEtherCore(pItem);
		pItem->Flag = 0;
		CheckWeight();
		ReFormInvenItem();
	}

	sinUsePotionDelayFlag = 1;
	return TRUE;
}

int cINVENTORY::EatSongPyeun(sITEM* pItem)
{
	if ((pItem->CODE & sinITEM_MASK3) == sin05 || (pItem->CODE & sinITEM_MASK3) == sin06 || (pItem->CODE & sinITEM_MASK3) == sin07 ||
		(pItem->CODE & sinITEM_MASK3) == sin08 || (pItem->CODE & sinITEM_MASK3) == sin09 || (pItem->CODE & sinITEM_MASK3) == sin10)
	{
		ReFormInvenItem();
		sinUsePotionDelayFlag = 1;
		return TRUE;
	}
	char szBuff[128];

	//////////////��������� ���� ������ �Ծ����� 
	if (cItem.GetItemLimitTime(pItem) == FALSE) {
		//if(pItem->LimitTimeFlag){
			//cMessageBox.ShowMessage(MESSAGE_DEAD_SONGPYUEN);
		wsprintf(szBuff, "%s : %s", sinChar->szName, sinDeadSongPyeunEat);
		SendChatMessageToServer(szBuff);
		ReFormInvenItem();
		sinUsePotionDelayFlag = 1;
		return FALSE;
	}
	int TempRandPotion = 0;
	int ResultPotion = 0;
	int Life = 0, Mana = 0, Stamina = 0;

	//CheckInvenItemForm();

	TempRandPotion = pItem->sItemInfo.Life[1] - pItem->sItemInfo.Life[0];
	ResultPotion = pItem->sItemInfo.Life[0] + (rand() % TempRandPotion);
	sinSetLife((sinGetLife() + ResultPotion));		//������ ���� 
	Life = (sinGetLife() + ResultPotion);

	TempRandPotion = pItem->sItemInfo.Mana[1] - pItem->sItemInfo.Mana[0];
	ResultPotion = pItem->sItemInfo.Mana[0] + (rand() % TempRandPotion);
	sinSetMana((sinGetMana() + ResultPotion));		//���� ����
	Mana = (sinGetLife() + ResultPotion);

	TempRandPotion = pItem->sItemInfo.Stamina[1] - pItem->sItemInfo.Stamina[0];
	ResultPotion = pItem->sItemInfo.Stamina[0] + (rand() % TempRandPotion);
	sinSetStamina((sinGetStamina() + ResultPotion));	//���׹̳� ����
	Stamina = (sinGetLife() + ResultPotion);

	smTRANS_COMMAND	smTransCommand;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_PARTY_POTION;
	smTransCommand.WParam = Life;	//�����
	smTransCommand.LParam = Mana;	//���
	smTransCommand.SParam = Stamina;//�ٷ�
	smTransCommand.EParam = lpCurPlayer->dwObjectSerial;

	if (smWsockUserServer && InterfaceParty.PartyMemberCount)
		smWsockUserServer->Send((char*)&smTransCommand, smTransCommand.size, TRUE);


	ReFormInvenItem();
	sinUsePotionDelayFlag = 1;


	return TRUE;

}
//���콺�� �ִ� �������� �����Ѵ� 
int cINVENTORY::ResetMouseItem()
{
	if (MouseItem.Flag) {
		if (cInvenTory.AutoSetInvenItem(&MouseItem)) {
			MouseItem.Flag = 0;
		}
	}

	return TRUE;
}
//CheckInvenItemForm();
//ReFormInvenItem();
//////////////////////�κ��丮 �������� ������ �����Ѵ� 
int cINVENTORY::CheckInvenItemForm()
{ //����Ÿ�� ���� ���ؼ� ������ �־��ش� 
	DWORD TempCheckDataSum = 0;

	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (InvenItem[i].Class != ITEM_CLASS_POTION) {
				//TempCheckDataSum += (i+1)*InvenItem[i].x;
				TempCheckDataSum += (i + 1) * InvenItem[i].y;
				TempCheckDataSum += (i + 1) * InvenItem[i].Class;
				TempCheckDataSum += InvenItem[i].sItemInfo.ItemHeader.dwChkSum;
				TempCheckDataSum += InvenItem[i].sItemInfo.ItemHeader.Head;
			}

		}
		if (InvenItemTemp[i].Flag) {
			if (InvenItemTemp[i].Class != ITEM_CLASS_POTION) {
				//TempCheckDataSum += (i+1)*InvenItemTemp[i].x;
				TempCheckDataSum += (i + 1) * InvenItemTemp[i].y;
				TempCheckDataSum += (i + 1) * InvenItemTemp[i].Class;
				TempCheckDataSum += InvenItemTemp[i].sItemInfo.ItemHeader.dwChkSum;
				TempCheckDataSum += InvenItemTemp[i].sItemInfo.ItemHeader.Head;
			}

		}

	}
	/*if(TempCheckDataSum != InvenDataCheckSum){
		SendSetHackUser(2); //��ŷ�� �Ϸ����ߴ� ���� ������ ��� TRUE ���� ����
		//TempCheckDataSum = 0;

	}*/
	return TRUE;
}

//����Ÿ�� ���� �����Ѵ� 
int cINVENTORY::ReFormInvenItem()
{
	DWORD Temp = 0;
	InvenDataCheckSum = 0;

	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (InvenItem[i].Class != ITEM_CLASS_POTION) {
				//InvenDataCheckSum += (i+1)*InvenItem[i].x;
				InvenDataCheckSum += (i + 1) * InvenItem[i].y;
				InvenDataCheckSum += (i + 1) * InvenItem[i].Class;
				InvenDataCheckSum += InvenItem[i].sItemInfo.ItemHeader.dwChkSum;
				InvenDataCheckSum += InvenItem[i].sItemInfo.ItemHeader.Head;
			}

		}
		if (InvenItemTemp[i].Flag) {
			if (InvenItemTemp[i].Class != ITEM_CLASS_POTION) {
				//InvenDataCheckSum += (i+1)*InvenItemTemp[i].x;
				InvenDataCheckSum += (i + 1) * InvenItemTemp[i].y;
				InvenDataCheckSum += (i + 1) * InvenItemTemp[i].Class;
				InvenDataCheckSum += InvenItemTemp[i].sItemInfo.ItemHeader.dwChkSum;
				InvenDataCheckSum += InvenItemTemp[i].sItemInfo.ItemHeader.Head;
			}

		}

	}
	Temp = InvenDataCheckSum;

	return TRUE;

}


int cINVENTORY::CheckMoneyLimit(int Money)
{

	int CheckMoney = 0;
	if (sinChar->Level <= 10) {
		if ((sinChar->Money + Money) <= MAX_GOLD_LOW_LEVEL)
			return TRUE;

	}
	else {
		switch (sinChar->ChangeJob) {
		case 0:
			if ((sinChar->Money + Money) <= (sinChar->Level * MAX_GOLD_LOW_LEVEL) - 1800000)
				return TRUE;
			break;

		case 1:
			if ((sinChar->Money + Money) <= MAX_GOLD_TIER2)
				return TRUE;
			break;
		case 2:
			if ((sinChar->Money + Money) <= MAX_GOLD_TIER3)
				return TRUE;

			break;
		case 3:
			if ((sinChar->Money + Money) <= MAX_GOLD_TIER4)
				return TRUE;
			break;

		}
	}

	cMessageBox.ShowMessage(MESSAGE_OVER_MONEY);
	return FALSE;
}

//////������ ������ ���Ѵ� 
int sinPotionNum[4] = { 0,0,0,0 };
int CheckPotionNum[4] = { 0,0,0,0 };
int cINVENTORY::ReFormPotionNum()
{
	PotionCheckNumJumpFlag = 1;

	for (int j = 0; j < 4; j++) {
		sinPotionNum[j] = 0;

	}
	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (InvenItem[i].Class == ITEM_CLASS_POTION) {
				switch (InvenItem[i].CODE & sinITEM_MASK3) {
				case sin01:
					sinPotionNum[0] += InvenItem[i].sItemInfo.PotionCount;

					break;
				case sin02:
					sinPotionNum[1] += InvenItem[i].sItemInfo.PotionCount;

					break;
				case sin03:
					sinPotionNum[2] += InvenItem[i].sItemInfo.PotionCount;

					break;
				case sin04:
					sinPotionNum[3] += InvenItem[i].sItemInfo.PotionCount;

					break;
				}
			}
		}
		if (InvenItemTemp[i].Flag) {
			if (InvenItemTemp[i].Class == ITEM_CLASS_POTION) {
				switch (InvenItemTemp[i].CODE & sinITEM_MASK3) {
				case sin01:
					sinPotionNum[0] += InvenItemTemp[i].sItemInfo.PotionCount;

					break;
				case sin02:
					sinPotionNum[1] += InvenItemTemp[i].sItemInfo.PotionCount;

					break;
				case sin03:
					sinPotionNum[2] += InvenItemTemp[i].sItemInfo.PotionCount;

					break;
				case sin04:
					sinPotionNum[3] += InvenItemTemp[i].sItemInfo.PotionCount;

					break;
				}
			}
		}
	}

	PotionCheckNumJumpFlag = 0;
	return TRUE;
}

DWORD PotionCheckDelayTime = 0;

int cINVENTORY::CheckPotionNumForm()
{
	for (int k = 0; k < 4; k++)
	{
		CheckPotionNum[k] = 0;
	}

	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (InvenItem[i].Class == ITEM_CLASS_POTION) {
				switch (InvenItem[i].CODE & sinITEM_MASK3) {
				case sin01:
					CheckPotionNum[0] += InvenItem[i].sItemInfo.PotionCount;

					break;
				case sin02:
					CheckPotionNum[1] += InvenItem[i].sItemInfo.PotionCount;

					break;
				case sin03:
					CheckPotionNum[2] += InvenItem[i].sItemInfo.PotionCount;

					break;
				case sin04:
					CheckPotionNum[3] += InvenItem[i].sItemInfo.PotionCount;

					break;
				}
			}
		}
		if (InvenItemTemp[i].Flag) {
			if (InvenItemTemp[i].Class == ITEM_CLASS_POTION) {
				switch (InvenItemTemp[i].CODE & sinITEM_MASK3) {
				case sin01:
					CheckPotionNum[0] += InvenItemTemp[i].sItemInfo.PotionCount;

					break;
				case sin02:
					CheckPotionNum[1] += InvenItemTemp[i].sItemInfo.PotionCount;

					break;
				case sin03:
					CheckPotionNum[2] += InvenItemTemp[i].sItemInfo.PotionCount;

					break;
				case sin04:
					CheckPotionNum[3] += InvenItemTemp[i].sItemInfo.PotionCount;

					break;
				}
			}
		}
	}

	for (int j = 0; j < 4; j++)
	{
		if (CheckPotionNum[j] != sinPotionNum[j])
		{
			SendSetHackUser2(1510, j);
		}
	}

	return TRUE;
}

int cINVENTORY::CheckSpecialItem(sITEM* pItem)
{
	if (pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST || pItem->sItemInfo.UniqueItem)
		return FALSE;
	return TRUE;
}

int dwInvenParticleShowTime = 0;
int cINVENTORY::ShowInvenEffectParticle()
{
	dwInvenParticleShowTime++;
	if (dwInvenParticleShowTime <= 5)return FALSE;
	dwInvenParticleShowTime = 0;

	int TempX, TempY;

	if (MouseItem.Flag) {
		if (MouseItem.sItemInfo.ItemKindCode == ITEM_KIND_QUEST || MouseItem.sItemInfo.UniqueItem) {
			TempX = rand() % MouseItem.w;
			TempY = rand() % MouseItem.h;
			SetInvenParticle(MouseItem.x + TempX, MouseItem.y + TempY, SIN_EFFECT_SHOW_INVEN);

		}
	}

	for (int i = 0; i < INVENTORY_MAXITEM; i++)
	{
		if (InvenItem[i].Flag) {
			if (InvenItem[i].sItemInfo.ItemKindCode == ITEM_KIND_QUEST || InvenItem[i].sItemInfo.UniqueItem)
			{
				TempX = rand() % InvenItem[i].w;
				TempY = rand() % InvenItem[i].h;
				SetInvenParticle(InvenItem[i].x + TempX, InvenItem[i].y + TempY, SIN_EFFECT_SHOW_INVEN);

				if (InvenItem[i].Class == ITEM_CLASS_WEAPON_TWO)
				{
					if (i == sInven[0].ItemIndex - 1)
					{
						TempX = rand() % (sInven[1].Rect.right - 10 - sInven[1].Rect.left + 10);
						TempY = rand() % (sInven[1].Rect.bottom - 10 - sInven[1].Rect.top + 10);
						SetInvenParticle(sInven[1].Rect.left + TempX, sInven[1].Rect.top + TempY, SIN_EFFECT_SHOW_INVEN);
					}

				}

			}
		}
	}

	for (int i = 0; i < MAX_TRADE_ITEM; i++)
	{
		if (sTrade.TradeItem[i].Flag) {
			if (sTrade.TradeItem[i].sItemInfo.UniqueItem) {
				TempX = rand() % sTrade.TradeItem[i].w;
				TempY = rand() % sTrade.TradeItem[i].h;
				SetInvenParticle(sTrade.TradeItem[i].x + TempX, sTrade.TradeItem[i].y + TempY, SIN_EFFECT_SHOW_TRADE);

			}
		}
		if (sTradeRecv.TradeItem[i].Flag) {
			if (sTradeRecv.TradeItem[i].sItemInfo.UniqueItem) {
				TempX = rand() % sTradeRecv.TradeItem[i].w;
				TempY = rand() % sTradeRecv.TradeItem[i].h;
				SetInvenParticle(sTradeRecv.TradeItem[i].x + TempX, sTradeRecv.TradeItem[i].y + TempY, SIN_EFFECT_SHOW_TRADE);

			}
		}

	}

	for (int i = 0; i < 100; i++)
	{
		if (sWareHouse.WareHouseItem[i].Flag) {
			if (sWareHouse.WareHouseItem[i].sItemInfo.UniqueItem) {
				TempX = rand() % sWareHouse.WareHouseItem[i].w;
				TempY = rand() % sWareHouse.WareHouseItem[i].h;
				SetInvenParticle(sWareHouse.WareHouseItem[i].x + TempX, sWareHouse.WareHouseItem[i].y + TempY, SIN_EFFECT_SHOW_WAREHOUSE);
			}
		}
	}

	for (int i = 0; i < 100; i++)
	{
		if (sCaravan.CaravanItem[i].Flag) {
			if (sCaravan.CaravanItem[i].sItemInfo.UniqueItem) {
				TempX = rand() % sCaravan.CaravanItem[i].w;
				TempY = rand() % sCaravan.CaravanItem[i].h;
				SetInvenParticle(sCaravan.CaravanItem[i].x + TempX, sCaravan.CaravanItem[i].y + TempY, SIN_EFFECT_SHOW_WAREHOUSE);
			}
		}
	}
	return TRUE;
}

int cINVENTORY::SetInvenParticle(int x, int y, int Kind)
{

	int i, k;
	int TempSize;
	int Count = 0;

	for (i = 0; i < MAX_INVEN_PARTICLE; i++) {
		if (!sInven_Effect[i].Time) {
			sInven_Effect[i].Kind = Kind;
			sInven_Effect[i].Time = 1;
			sInven_Effect[i].Max_Time = rand() % 20 + 3;

			sInven_Effect[i].InvenFace.x = x;
			sInven_Effect[i].InvenFace.y = y;
			TempSize = rand() % 13 + 1;
			sInven_Effect[i].InvenFace.width = TempSize;
			sInven_Effect[i].InvenFace.height = TempSize;

			sInven_Effect[i].Speed.y = rand() % 12 + 3;

			if (Kind == SIN_EFFECT_SHOW_GYUNGFUM)
			{
				sInven_Effect[i].InvenFace.MatNum = MatInvenParticle[4];
				sInven_Effect[i].InvenFace.r = 255;
				sInven_Effect[i].InvenFace.g = 255;
				sInven_Effect[i].InvenFace.b = 255;
				sInven_Effect[i].InvenFace.Transparency = rand() % 55 + 200;
				sInven_Effect[i].FadeType = 1;
				if (sInven_Effect[i].InvenFace.width > 5) {
					sInven_Effect[i].InvenFace.width -= 2;
					sInven_Effect[i].InvenFace.height -= 2;

				}
				if (sInven_Effect[i].InvenFace.width < 5) {
					memset(&sInven_Effect[i], 0, sizeof(sINVENTORY_EFFECT));

				}

			}
			else {
				sInven_Effect[i].InvenFace.MatNum = MatInvenParticle[0];
				sInven_Effect[i].InvenFace.r = 255;
				sInven_Effect[i].InvenFace.g = 255;
				sInven_Effect[i].InvenFace.b = 255;
				sInven_Effect[i].InvenFace.Transparency = rand() % 55 + 200;
				sInven_Effect[i].FadeType = 1;
			}

			if (Kind == 1)
			{
				for (k = 0; k < MAX_INVEN_PARTICLE; k++) {
					if (!sInven_Effect[k].Time) {
						Count++;
						if (Count > 2) {
							Count = 0;
							break;

						}
						memcpy(&sInven_Effect[k], &sInven_Effect[i], sizeof(sINVENTORY_EFFECT));
					}

				}
			}
			break;
		}

	}

	return TRUE;
}

DWORD dwInvenParticleMoveTime = 0;

int cINVENTORY::MoveInvenParticle()
{
	dwInvenParticleMoveTime++;
	if (dwInvenParticleMoveTime <= 5)return FALSE;
	dwInvenParticleMoveTime = 0;

	int i = 0;
	for (i = 0; i < MAX_INVEN_PARTICLE; i++) {
		if (sInven_Effect[i].Time) {
			sInven_Effect[i].Time++;
			sInven_Effect[i].InvenFace.y -= sInven_Effect[i].Speed.y;

			if (sInven_Effect[i].FadeType == 1)
				sInven_Effect[i].InvenFace.Transparency += 14;
			if (sInven_Effect[i].FadeType == 2)
				sInven_Effect[i].InvenFace.Transparency -= 30;

			if (sInven_Effect[i].InvenFace.Transparency >= 255)
				sInven_Effect[i].FadeType = 2;

			if (sInven_Effect[i].InvenFace.Transparency <= 0)
				sInven_Effect[i].Time = 0;

			if (sInven_Effect[i].Kind != SIN_EFFECT_SHOW_GYUNGFUM) {
				if ((sInven_Effect[i].Time % 5) == 0) {
					sInven_Effect[i].AniCount++;
					if (sInven_Effect[i].AniCount > 3)sInven_Effect[i].AniCount = 0;
					sInven_Effect[i].InvenFace.MatNum = MatInvenParticle[sInven_Effect[i].AniCount];

				}
			}

			if (sInven_Effect[i].InvenFace.Transparency <= 0)sInven_Effect[i].Time = 0;

		}

	}

	return TRUE;
}

int cINVENTORY::sinDrawInvenTexImage(smFACE2D* Face)
{

	D3DTLVERTEX tlVertex[4];
	int cnt;
	int x, y;
	x = Face->x;
	y = Face->y;
	dsGetDrawOffset(&x, &y);
	for (cnt = 0; cnt < 4; cnt++)

	{
		tlVertex[cnt].rhw = 1;
		tlVertex[cnt].color = D3DCOLOR_RGBA(Face->r, Face->g, Face->b, Face->Transparency);
		tlVertex[cnt].specular = D3DCOLOR_RGBA(0, 0, 0, 0);
		tlVertex[cnt].sz = 0;
	}
	tlVertex[0].sx = (float)x;
	tlVertex[0].sy = (float)y;
	tlVertex[0].tu = 0;
	tlVertex[0].tv = 0;
	tlVertex[1].sx = (float)x + (float)Face->width;
	tlVertex[1].sy = (float)y;
	tlVertex[1].tu = 1;
	tlVertex[1].tv = 0;
	tlVertex[2].sx = (float)x;
	tlVertex[2].sy = (float)y + (float)Face->height;
	tlVertex[2].tu = 0;
	tlVertex[2].tv = 1;
	tlVertex[3].sx = ((float)x + (float)Face->width);
	tlVertex[3].sy = ((float)y + (float)Face->height);
	tlVertex[3].tu = 1;
	tlVertex[3].tv = 1;
	DrawTexture(smMaterial[Face->MatNum].smTexture[0]->lpD3DTexture, tlVertex, true);
	return TRUE;
}


int cINVENTORY::CheckSetEmptyArea(sITEM* pItem)
{
	int cntx, cnty;
	int cx, cy;

	RECT	rect = { 0,0,0,0 };
	int		flag = 0;

	cx = (22 * INVENTORY_BOX_ROW) - pItem->w;
	cy = (22 * INVENTORY_BOX_COL) - pItem->h;

	for (cntx = StartX; cntx <= StartX + cx; cntx += 22) {
		for (cnty = StartY; cnty <= StartY + cy; cnty += 22) {
			flag = 0;
			for (int i = 0; i < INVENTORY_MAXITEM; i++) {
				if (InvenItem[i].Flag) {
					rect.left = InvenItem[i].x;
					rect.right = InvenItem[i].x + InvenItem[i].w;
					rect.top = InvenItem[i].y;
					rect.bottom = InvenItem[i].y + InvenItem[i].h;

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

int cINVENTORY::ResetInvenItemToServer(DWORD CODE, DWORD Head, DWORD CheckSum, DWORD Time)
{
	int i = 0;
	for (i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (InvenItem[i].CODE == CODE &&
				InvenItem[i].sItemInfo.ItemHeader.Head == Head &&
				InvenItem[i].sItemInfo.ItemHeader.dwChkSum == CheckSum) {
				InvenItem[i].sItemInfo.ItemHeader.dwTime = Time;
				return TRUE;

			}

		}
		if (InvenItemTemp[i].Flag) {
			if (InvenItemTemp[i].CODE == CODE &&
				InvenItemTemp[i].sItemInfo.ItemHeader.Head == Head &&
				InvenItemTemp[i].sItemInfo.ItemHeader.dwChkSum == CheckSum) {
				InvenItemTemp[i].sItemInfo.ItemHeader.dwTime = Time;
				return TRUE;

			}

		}
	}
	return FALSE;
}

//������ ���� (���� �޼���)
int cINVENTORY::DeleteInvenItemToServer(DWORD CODE, DWORD Head, DWORD CheckSum)
{

	//���콺 �������� üũ���ش� 
	if (MouseItem.Flag && MouseItem.CODE == CODE &&
		MouseItem.sItemInfo.ItemHeader.Head == Head &&
		MouseItem.sItemInfo.ItemHeader.dwChkSum == CheckSum) {
		MouseItem.Flag = 0;
	}

	int i = 0;
	for (i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (InvenItem[i].CODE == CODE &&
				InvenItem[i].sItemInfo.ItemHeader.Head == Head &&
				InvenItem[i].sItemInfo.ItemHeader.dwChkSum == CheckSum) {
				InvenItem[i].Flag = 0;
				if (InvenItem[i].ItemPosition) {
					//��վ������� �Ѽտ� ������쿡�� �����ش� 
					if (InvenItem[i].Class == ITEM_CLASS_WEAPON_TWO) {
						if (InvenItem[i].ItemPosition == 1) {//������ 
							if (InvenItem[i].CODE != InvenItem[sInven[1].ItemIndex - 1].CODE) {
								InvenItem[sInven[1].ItemIndex - 1].Flag = 0;
								InvenItem[i].Flag = 0;
								sInven[1].ItemIndex = 0; //�κ��丮 �ڽ� �ʱ�ȭ 
								sInven[0].ItemIndex = 0; //�κ��丮 �ڽ� �ʱ�ȭ 
								if (InvenItem[i].SetModelPosi)
									sinSetCharItem(InvenItem[i].CODE, InvenItem[i].SetModelPosi, FALSE);

							}
						}
						if (InvenItem[i].ItemPosition == 2) {//�޼� 
							if (InvenItem[i].CODE != InvenItem[sInven[0].ItemIndex - 1].CODE) {
								InvenItem[sInven[0].ItemIndex - 1].Flag = 0;
								InvenItem[i].Flag = 0;
								sInven[0].ItemIndex = 0; //�κ��丮 �ڽ� �ʱ�ȭ 
								sInven[1].ItemIndex = 0; //�κ��丮 �ڽ� �ʱ�ȭ 
								if (InvenItem[i].SetModelPosi)
									sinSetCharItem(InvenItem[i].CODE, InvenItem[i].SetModelPosi, FALSE);

							}
						}

					}
					sInven[InvenItem[i].ItemPosition - 1].ItemIndex = 0;
					if (InvenItem[i].SetModelPosi)
						sinSetCharItem(InvenItem[i].CODE, InvenItem[i].SetModelPosi, FALSE);

				}
				ReFormInvenItem();
				ReformCharForm();//������ 
				ReFormPotionNum();
				SetItemToChar();
				break;

			}

		}
		if (InvenItemTemp[i].Flag) {
			if (InvenItemTemp[i].CODE == CODE &&
				InvenItemTemp[i].sItemInfo.ItemHeader.Head == Head &&
				InvenItemTemp[i].sItemInfo.ItemHeader.dwChkSum == CheckSum) {
				InvenItemTemp[i].Flag = 0;
				ReFormInvenItem();
				ReformCharForm();//������ 
				ReFormPotionNum();
				SetItemToChar();
				break;

			}
		}
	}

	//��� �������� ������� üũ�ؼ� �ٽ��ѹ� �����ش�
	if (!sInven[0].ItemIndex) {
		if (sInven[1].ItemIndex) {
			if (InvenItem[sInven[1].ItemIndex - 1].Flag) {
				if (InvenItem[sInven[1].ItemIndex - 1].Class == ITEM_CLASS_WEAPON_TWO) {
					if (!InvenItem[sInven[1].ItemIndex - 1].sItemInfo.ItemHeader.Head) {
						InvenItem[sInven[1].ItemIndex - 1].Flag = 0;
						sInven[1].ItemIndex = 0;
						ReFormInvenItem();
						ReformCharForm();//������ 
						ReFormPotionNum();
						SetItemToChar();
					}
				}
			}
		}
	}
	cInvenTory.CheckWeight();
	return TRUE;
}

//ĳ���� �Ķ���� ������ �����Ѵ� 
int cINVENTORY::HackCharStateCheck(smCHAR_INFO* pChar_Info, smCHAR_INFO* desChar_Info)
{
	int HackFlag = 0;

	if (pChar_Info->Attack_Damage[0] < desChar_Info->Attack_Damage[0]) {
		HackFlag = 1;

	}
	if (pChar_Info->Attack_Damage[1] < desChar_Info->Attack_Damage[1]) {
		HackFlag = 1;

	}
	if (pChar_Info->Defence < desChar_Info->Defence) {
		HackFlag = 1;

	}
	if (pChar_Info->Absorption < desChar_Info->Absorption) {
		HackFlag = 1;

	}
	if (pChar_Info->Accuracy < desChar_Info->Accuracy) {
		HackFlag = 1;

	}
	if (pChar_Info->Attack_Speed < desChar_Info->Attack_Speed) {
		HackFlag = 1;

	}
	if (pChar_Info->Chance_Block < desChar_Info->Chance_Block) {
		HackFlag = 1;

	}
	if (pChar_Info->Critical_Hit < desChar_Info->Critical_Hit) {
		HackFlag = 1;

	}
	if (pChar_Info->Shooting_Range < desChar_Info->Shooting_Range) {
		HackFlag = 1;

	}
	if (HackFlag) {
		smTRANS_COMMAND	smTransCommand;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_FIX_ATTACK;
		smTransCommand.WParam = pChar_Info->Attack_Damage[1];
		smTransCommand.LParam = pChar_Info->Defence;
		smTransCommand.SParam = (int)(pChar_Info->Absorption * 256);
		smTransCommand.EParam = 0;
		if (smWsockDataServer)
			smWsockDataServer->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

		return FALSE;
	}
	return TRUE;
}

//�߸� ��ġ�� �������� ���ġ�Ѵ� 
int cINVENTORY::CheckResetInvenItem()
{
	RECT rect;
	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (InvenItem[i].ItemPosition) {
				if (!sInven[InvenItem[i].ItemPosition - 1].ItemIndex || sInven[InvenItem[i].ItemPosition - 1].ItemIndex - 1 != i)continue;
				rect.left = sInven[InvenItem[i].ItemPosition - 1].Rect.left;
				rect.top = sInven[InvenItem[i].ItemPosition - 1].Rect.top;
				rect.right = sInven[InvenItem[i].ItemPosition - 1].Rect.right;
				rect.bottom = sInven[InvenItem[i].ItemPosition - 1].Rect.bottom;

				if (InvenItem[i].x <	rect.left || InvenItem[i].y < rect.top ||
					InvenItem[i].x + InvenItem[i].w > rect.right + 2 || InvenItem[i].y + InvenItem[i].h > rect.bottom + 2) {

					InvenItem[i].x = rect.left + (((rect.right - rect.left) - InvenItem[i].w) / 2);
					InvenItem[i].y = rect.top + (((rect.bottom - rect.top) - InvenItem[i].h) / 2);
					ReFormInvenItem();
				}
			}

		}
		if (InvenItemTemp[i].Flag) {
			if (InvenItemTemp[i].ItemPosition) {
				rect.left = sInven[InvenItemTemp[i].ItemPosition - 1].Rect.left;
				rect.top = sInven[InvenItemTemp[i].ItemPosition - 1].Rect.top;
				rect.right = sInven[InvenItemTemp[i].ItemPosition - 1].Rect.right;
				rect.bottom = sInven[InvenItemTemp[i].ItemPosition - 1].Rect.bottom;

				if (InvenItemTemp[i].x <	rect.left || InvenItemTemp[i].y < rect.top ||
					InvenItemTemp[i].x + InvenItemTemp[i].w > rect.right + 2 || InvenItemTemp[i].y + InvenItemTemp[i].h > rect.bottom + 2) {

					InvenItemTemp[i].x = rect.left + (((rect.right - rect.left) - InvenItemTemp[i].w) / 2);
					InvenItemTemp[i].y = rect.top + (((rect.bottom - rect.top) - InvenItemTemp[i].h) / 2);
					ReFormInvenItem();

				}
			}
		}
	}
	return TRUE;
}

int cINVENTORY::SearchItemByName(char name[64], int numberOfItems) {
	int cnt = 0;
	if (numberOfItems) {
		for (int i = 0; i < INVENTORY_MAXITEM; i++) {
			if (InvenItem[i].Flag) {
				if (lstrcmpi(InvenItem[i].sItemInfo.ItemName, name) == 0)cnt++;
			}
			if (InvenItemTemp[i].Flag) {
				if (lstrcmpi(InvenItem[i].sItemInfo.ItemName, name) == 0)cnt++;
			}
		}
		return cnt;
	}

	return FALSE;
}

int cINVENTORY::SearchItemCode(DWORD CODE, int KindFlag)
{

	int ItemCount = 0;
	if (CODE) {
		for (int i = 0; i < INVENTORY_MAXITEM; i++) {
			if (InvenItem[i].Flag) {
				if (InvenItem[i].CODE == CODE)ItemCount++;
			}
			if (InvenItemTemp[i].Flag) {
				if (InvenItemTemp[i].CODE == CODE)ItemCount++;
			}
		}
		return ItemCount;
	}

	if (KindFlag) {
		for (int i = 0; i < INVENTORY_MAXITEM; i++) {
			if (InvenItem[i].Flag) {
				if (InvenItem[i].sItemInfo.ItemKindCode == KindFlag)ItemCount++;
			}
			if (InvenItemTemp[i].Flag) {
				if (InvenItemTemp[i].sItemInfo.ItemKindCode == KindFlag)ItemCount++;
			}
		}
		return ItemCount;
	}
	return FALSE;
}

int cINVENTORY::CheckDamage()
{
	int Damage[2] = { 0,0 };
	int Critical = 0;
	int RingCritical = 0;
	int AmuletCritical = 0;
	int SheltomCritical = 0;
	int SendDamage[2] = { 0,0 };
	int SendCritical = 0;
	int RealDamage[2] = { 0,0 };
	int RealSendCritical = 0;

	int i = 0, j = 0, k = 0, t = 0;

	DWORD WeaponCode[8] = { sinWA1,sinWS1,sinWP1,sinWS2,sinWT1,sinWM1,sinWM1 };
	int   CharCode[7] = { SIN_CHAR_FIGHTER,SIN_CHAR_ARCHER,SIN_CHAR_PIKEMAN,SIN_CHAR_KNIGHT,SIN_CHAR_ATALANTA,SIN_CHAR_PRIESTESS,SIN_CHAR_MAGICIAN };

	sITEM* pItem = 0;
	pItem = new sITEM;
	for (i = 0; i < INVENTORY_MAXITEM * 2; i++) {
		Damage[0] = 0; Damage[1] = 0; Critical = 0;
		if (i < 100) {
			if (!InvenItem[i].Flag)continue;
			if (InvenItem[i].NotUseFlag)continue;
			memcpy(pItem, &InvenItem[i], sizeof(sITEM));

		}
		else {
			if (!InvenItemTemp[i - 100].Flag)continue;
			if (InvenItemTemp[i - 100].NotUseFlag)continue;
			memcpy(pItem, &InvenItemTemp[i - 100], sizeof(sITEM));
		}
		if (pItem->sItemInfo.Damage[0] || pItem->sItemInfo.Damage[1] || pItem->sItemInfo.Critical_Hit)
		{
			Damage[0] += pItem->sItemInfo.Damage[0];
			Damage[1] += pItem->sItemInfo.Damage[1];
			Critical += pItem->sItemInfo.Critical_Hit;


			// Cr�tico dos aneis amuletos e sheltons
			if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinOR1)
			{
				if (pItem->sItemInfo.Critical_Hit)
					RingCritical += pItem->sItemInfo.Critical_Hit;
			}

			if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinOR2)
			{
				if (pItem->sItemInfo.Critical_Hit)
					RingCritical += pItem->sItemInfo.Critical_Hit;
			}

			if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinOS1)
			{
				if (pItem->sItemInfo.Critical_Hit)
					SheltomCritical += pItem->sItemInfo.Critical_Hit;
			}

			if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinOA1)
			{
				if (pItem->sItemInfo.Critical_Hit)
					AmuletCritical += pItem->sItemInfo.Critical_Hit;
			}

			if ((sinDamageFunction[0] == 1 && (pItem->WeaponClass == 1 || pItem->WeaponClass == 3))) {
				SendDamage[0] = 1 + (Damage[0] * (sinChar->Strength + 130) / 130) +
					((sinChar->Talent + sinChar->Dexterity) / 40) + ((Damage[0] + Damage[1]) / 16);
				SendDamage[1] = 3 + (Damage[1] * (sinChar->Strength + 130) / 130) +
					((sinChar->Talent + sinChar->Dexterity) / 40);
			}

			if ((sinDamageFunction[0] == 2 && (pItem->WeaponClass == 1 || pItem->WeaponClass == 3))) {
				SendDamage[0] = 1 + (Damage[0] * (sinChar->Strength + 150) / 150) +
					((sinChar->Talent + sinChar->Dexterity) / 45) + ((Damage[0] + Damage[1]) / 16);
				SendDamage[1] = 3 + (Damage[1] * (sinChar->Strength + 150) / 150) +
					((sinChar->Talent + sinChar->Dexterity) / 45);
			}
			if ((sinDamageFunction[0] == 3 && (pItem->WeaponClass == 1 || pItem->WeaponClass == 3))) {
				SendDamage[0] = 1 + (Damage[0] * (sinChar->Strength + 190) / 190) +
					((sinChar->Talent + sinChar->Dexterity) / 50) + ((Damage[0] + Damage[1]) / 16);
				SendDamage[1] = 3 + (Damage[1] * (sinChar->Strength + 190) / 190) +
					((sinChar->Talent + sinChar->Dexterity) / 50);
			}
			if (sinDamageFunction[1] == 1 && pItem->WeaponClass == 2) {
				SendDamage[0] = 1 + (Damage[0] * (sinChar->Dexterity + 130) / 130) +
					((sinChar->Talent + sinChar->Strength) / 40) + ((Damage[0] + Damage[1]) / 16);
				SendDamage[1] = 3 + (Damage[1] * (sinChar->Dexterity + 130) / 130) +
					((sinChar->Talent + sinChar->Strength) / 40);
			}

			if (sinDamageFunction[1] == 2 && pItem->WeaponClass == 2) {
				SendDamage[0] = 1 + (Damage[0] * (sinChar->Dexterity + 190) / 190) +
					((sinChar->Talent + sinChar->Strength) / 50) + ((Damage[0] + Damage[1]) / 16);
				SendDamage[1] = 3 + (Damage[1] * (sinChar->Dexterity + 190) / 190) +
					((sinChar->Talent + sinChar->Strength) / 50);
			}



			if (sinDamageFunction[2] == 1 && pItem->WeaponClass == 3) {
				SendDamage[0] = 1 + (Damage[0] * (sinChar->Spirit + 150) / 150) +
					((sinChar->Talent) / 30) + ((Damage[0] + Damage[1]) / 16);
				SendDamage[1] = 3 + (Damage[1] * (sinChar->Spirit + 150) / 150) +
					((sinChar->Talent) / 30);
			}

			if (sinDamageFunction[2] == 2 && pItem->WeaponClass == 3) {
				SendDamage[0] = 1 + (Damage[0] * (sinChar->Spirit + 180) / 180) +
					((sinChar->Talent) / 30);
				SendDamage[1] = 2 + (Damage[1] * (sinChar->Spirit + 180) / 180) +
					((sinChar->Talent) / 25);
			}
			if (sinDamageFunction[2] == 2 && pItem->WeaponClass == 3) {
				SendDamage[0] = 1 + (Damage[0] * (sinChar->Spirit + 170) / 170) +
					((sinChar->Talent) / 30) + ((Damage[0] + Damage[1]) / 16);
				SendDamage[1] = 3 + (Damage[1] * (sinChar->Spirit + 170) / 170) +
					((sinChar->Talent) / 30);
			}

			for (j = 0; j < 7; j++) {
				if (CharCode[j] == sinChar->JOB_CODE) {
					if (WeaponCode[j] == (pItem->sItemInfo.CODE & sinITEM_MASK2)) {
						SendDamage[0] += sinChar->Level / 6;
						SendDamage[1] += sinChar->Level / 6;
					}
				}
			}

			if (sinChar->JOB_CODE == SIN_CHAR_MECANICIAN) {
				if (sinChar->JobBitMask & pItem->sItemInfo.JobCodeMask) {
					SendDamage[0] += sinChar->Level / 6;
					SendDamage[1] += sinChar->Level / 6;
				}
			}

			for (k = 1; k < MAX_USESKILL; k++) {
				if (sinSkill.UseSkill[k].Flag && sinSkill.UseSkill[k].Point) {
					for (t = 0; t < 8; t++) {
						switch (sinSkill.UseSkill[k].CODE) {
						case SKILL_MELEE_MASTERY:
							if (sinSkill.UseSkill[k].Skill_Info.UseWeaponCode[t] == (pItem->sItemInfo.CODE & sinITEM_MASK2)) {
								SendDamage[0] += (int)(((float)Melee_Mastery_DamagePercent[sinSkill.UseSkill[k].Point - 1] / 100) * pItem->sItemInfo.Damage[0]);
								SendDamage[1] += (int)(((float)Melee_Mastery_DamagePercent[sinSkill.UseSkill[k].Point - 1] / 100) * pItem->sItemInfo.Damage[1]);
							}
							break;
						case SKILL_SHOOTING_MASTERY:
							if (sinSkill.UseSkill[k].Skill_Info.UseWeaponCode[t] == (pItem->sItemInfo.CODE & sinITEM_MASK2)) {
								SendDamage[0] += (int)(((float)S_Mastery_DamagePercent[sinSkill.UseSkill[k].Point - 1] / 100) * pItem->sItemInfo.Damage[0]);
								SendDamage[1] += (int)(((float)S_Mastery_DamagePercent[sinSkill.UseSkill[k].Point - 1] / 100) * pItem->sItemInfo.Damage[1]);

							}
							break;
						case SKILL_THROWING_MASTERY:
							if (sinSkill.UseSkill[k].Skill_Info.UseWeaponCode[t] == (pItem->sItemInfo.CODE & sinITEM_MASK2)) {
								SendDamage[0] += (int)(((float)T_Mastery_Damage[sinSkill.UseSkill[k].Point - 1] / 100) * pItem->sItemInfo.Damage[0]);
								SendDamage[1] += (int)(((float)T_Mastery_Damage[sinSkill.UseSkill[k].Point - 1] / 100) * pItem->sItemInfo.Damage[1]);
							}
							break;
						case SKILL_CRITICAL_MASTERY:
							if (sinSkill.UseSkill[k].Skill_Info.UseWeaponCode[t] == (pItem->sItemInfo.CODE & sinITEM_MASK2)) {
								Critical += Critical_Mastery_Critical[sinSkill.UseSkill[k].Point - 1];

							}
							break;
						case SKILL_SWORD_MASTERY:
							if (sinSkill.UseSkill[k].Skill_Info.UseWeaponCode[t] == (pItem->sItemInfo.CODE & sinITEM_MASK2)) {
								SendDamage[0] += (int)(((float)Sword_Mastery_DamagePercent[sinSkill.UseSkill[k].Point - 1] / 100) * pItem->sItemInfo.Damage[0]);
								SendDamage[1] += (int)(((float)Sword_Mastery_DamagePercent[sinSkill.UseSkill[k].Point - 1] / 100) * pItem->sItemInfo.Damage[1]);

							}
							break;

						}
					}
				}
			}

			if (sinChar->JobBitMask & pItem->sItemInfo.JobCodeMask)
			{
				if (pItem->sItemInfo.JobItem.Lev_Damage[1])
					SendDamage[1] += (sinChar->Level / pItem->sItemInfo.JobItem.Lev_Damage[1]);
				if (pItem->sItemInfo.JobItem.Add_Critical_Hit)
					Critical += pItem->sItemInfo.JobItem.Add_Critical_Hit;
			}

			SendCritical = Critical;
			SendDamage[0] += 1;
			SendDamage[1] += 1;

			if (RealDamage[0] < SendDamage[0])
			{
				RealDamage[0] = SendDamage[0];
			}
			if (RealDamage[1] < SendDamage[1])
			{
				RealDamage[1] = SendDamage[1];
			}

			if (RealSendCritical < SendCritical)
				RealSendCritical = SendCritical;

			RealSendCritical += (RingCritical + AmuletCritical + SheltomCritical);
		}
	}

	// Envia o dano e critico m�ximo que o char pode causar para o servidor
	SendMaxDamageToServer(RealDamage[0], RealDamage[1], RealSendCritical);
	return TRUE;
}


int cINVENTORY::ClearQuestItem()
{
	sITEM* pItem = 0;
	for (int i = 0; i < INVENTORY_MAXITEM * 2; i++) {
		if (i < 100) {
			if (!InvenItem[i].Flag)continue;
			pItem = &InvenItem[i];
		}
		else {
			if (!InvenItemTemp[i - 100].Flag)continue;
			pItem = &InvenItemTemp[i - 100];
		}
		if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2) {
			pItem->Flag = 0;
		}
	}
	cInvenTory.ReFormInvenItem();
	ResetInvenItemCode();
	return TRUE;
}

int cINVENTORY::DeleteCoupleRing()
{
	sITEM* pItem = 0;
	for (int i = 0; i < INVENTORY_MAXITEM * 2; i++)
	{
		if (i < 100)
		{
			if (!InvenItem[i].Flag)continue;
			pItem = &InvenItem[i];
		}
		else
		{
			if (!InvenItemTemp[i - 100].Flag)continue;
			pItem = &InvenItemTemp[i - 100];
		}
		if (pItem->sItemInfo.CODE == (sinSP1 | sin16))
		{
			pItem->Flag = 0;
			break;
		}
	}
	cInvenTory.ReFormInvenItem();
	ResetInvenItemCode();
	return TRUE;
}

//��ũ�ھ� ����Ÿ�� �������� �޴´�
int cINVENTORY::RecvLinkCore(sITEMINFO* pItemInfo)
{
	sITEMINFO* pItemInfo2 = 0;
	for (int i = 0; i < INVENTORY_MAXITEM * 2; i++) {
		if (i < 100) {
			if (!InvenItem[i].Flag)continue;
			pItemInfo2 = &InvenItem[i].sItemInfo;
		}
		else {
			if (!InvenItemTemp[i - 100].Flag)continue;
			pItemInfo2 = &InvenItemTemp[i - 100].sItemInfo;
		}
		if (pItemInfo->BackUpChkSum == pItemInfo2->BackUpChkSum &&
			pItemInfo->BackUpKey == pItemInfo2->BackUpKey &&
			pItemInfo->CODE == pItemInfo2->CODE) {
			memcpy(pItemInfo2, pItemInfo, sizeof(sITEMINFO));
			CheckWeight();//���� ���� 
			ReFormInvenItem();
			return TRUE;

		}
	}
	return FALSE;
}
int sinMousePotionNum[4] = { 0,0,0,0 };
int ChecMousekPotionNum[4] = { 0,0,0,0 };

//���콺�� ������ üũ�Ѵ�
int cINVENTORY::CheckMousePotionNumForm()
{
	for (int k = 0; k < 4; k++) {
		ChecMousekPotionNum[k] = 0;

	}

	if (MouseItem.Flag) {
		if (MouseItem.Class == ITEM_CLASS_POTION) {
			switch (MouseItem.sItemInfo.CODE & sinITEM_MASK3) {
			case sin01:
				ChecMousekPotionNum[0] += MouseItem.sItemInfo.PotionCount;

				break;
			case sin02:
				ChecMousekPotionNum[1] += MouseItem.sItemInfo.PotionCount;

				break;
			case sin03:
				ChecMousekPotionNum[2] += MouseItem.sItemInfo.PotionCount;

				break;
			case sin04:
				ChecMousekPotionNum[3] += MouseItem.sItemInfo.PotionCount;

				break;
			}
		}
	}
	for (int j = 0; j < 4; j++) {
		if (ChecMousekPotionNum[j] != sinMousePotionNum[j]) {
			//SendSetHackUser(TRUE); //��ŷ�� �Ϸ����ߴ� ���� ������ ��� TRUE ���� ���� 
			SendSetHackUser2(1510, j); //��ŷ�� ������ �Ű��Ѵ�


		}

	}

	return TRUE;
}
//���콺�� ������ �����Ѵ� 
int cINVENTORY::ReFormMousePotionNum()
{
	for (int j = 0; j < 4; j++) {
		sinMousePotionNum[j] = 0;

	}
	if (MouseItem.Flag) {
		if (MouseItem.Class == ITEM_CLASS_POTION) {
			switch (MouseItem.sItemInfo.CODE & sinITEM_MASK3) {
			case sin01:
				sinMousePotionNum[0] += MouseItem.sItemInfo.PotionCount;

				break;
			case sin02:
				sinMousePotionNum[1] += MouseItem.sItemInfo.PotionCount;

				break;
			case sin03:
				sinMousePotionNum[2] += MouseItem.sItemInfo.PotionCount;

				break;
			case sin04:
				sinMousePotionNum[3] += MouseItem.sItemInfo.PotionCount;

				break;
			}
		}
	}
	return TRUE;
}

//���Ͽ� �ھ� ����Ʈ�� �����Ѵ�
int cINVENTORY::SetUnionCoreEffect(int x, int y)
{
	for (int i = 0; i < 10; i++) {
		if (!UnionCore_Effect_Bone[i].Flag) {
			UnionCore_Effect_Bone[i].DesPosi.x = x + 22; //���Ͽ� �ھ��� �߽� ��ǥ��üũ�Ѵ�
			UnionCore_Effect_Bone[i].DesPosi.y = y;//-500;
			UnionCore_Effect_Bone[i].Flag = 1;
			UnionCore_Effect_Bone[i].Posi.y -= 350;


			UnionCore_Effect_Bone[i].InvenFace.MatNum = *(int*)AssaSearchRes("par2.tga", SMMAT_BLEND_ALPHA);
			//UnionCore_Effect_Bone[i].InvenFace.x = x;
			//UnionCore_Effect_Bone[i].InvenFace.y = y;

			//UnionCore_Effect_Bone[i].InvenFace.x = 200;
			//UnionCore_Effect_Bone[i].InvenFace.y = 200;

			UnionCore_Effect_Bone[i].Max_Time = 100;
			UnionCore_Effect_Bone[i].InvenFace.r = 255;
			UnionCore_Effect_Bone[i].InvenFace.g = 255;
			UnionCore_Effect_Bone[i].InvenFace.b = 255;
			UnionCore_Effect_Bone[i].InvenFace.Transparency = 255;
			UnionCore_Effect_Bone[i].InvenFace.height = 32;
			UnionCore_Effect_Bone[i].InvenFace.width = 32;
			UnionCore_Effect_Bone[i].InvenFace.TexRect = sinTexRect;
			UnionCore_Effect_Bone[i].Flag = 1;

		}
	}
	return TRUE;
}

int cINVENTORY::UnionCoreParticleProcess()
{
	int i = 0;
	//��
	for (i = 0; i < 10; i++) {
		if (UnionCore_Effect_Bone[i].Flag) {
			UnionCore_Effect_Bone[i].Time += GetRandomPos(1, 3);
			UnionCore_Effect_Bone[i].Posi.y += 30;
			UnionCore_Effect_Bone[i].InvenFace.y = UnionCore_Effect_Bone[i].Posi.y;
			UnionCore_Effect_Bone[i].Posi.x = UnionCore_Effect_Bone[i].DesPosi.x - 5;
			UnionCore_Effect_Bone[i].InvenFace.x = UnionCore_Effect_Bone[i].Posi.x;
			//if((UnionCore_Effect_Bone[i].Time % 5 )==0)
			if (UnionCore_Effect_Bone[i].Posi.y <= UnionCore_Effect_Bone[i].DesPosi.y - 10)
				UnionCoreParticle(UnionCore_Effect_Bone[i].Posi.x, UnionCore_Effect_Bone[i].Posi.y);

			if (UnionCore_Effect_Bone[i].Posi.y >= UnionCore_Effect_Bone[i].DesPosi.y) {
				UnionCoreParticle2(UnionCore_Effect_Bone[i].InvenFace.x, UnionCore_Effect_Bone[i].InvenFace.y);
				memset(&UnionCore_Effect_Bone[i], 0, sizeof(sINVENTORY_EFFECT));

			}
		}
	}

	//��ƼŬ
	for (i = 0; i < 100; i++) {
		if (UnionCore_Effect[i].Flag) {
			UnionCore_Effect[i].Time++;
			if (UnionCore_Effect[i].Direction == 1)
				UnionCore_Effect[i].InvenFace.x -= GetRandomPos(0, 1);
			else UnionCore_Effect[i].InvenFace.x += GetRandomPos(0, 1);
			//UnionCore_Effect[i].InvenFace.Transparency -=5;


			//UnionCore_Effect[i].InvenFace.x +=GetRandomPos(-5,5);
			//UnionCore_Effect[i].InvenFace.y +=GetRandomPos(-5,5);
			if (UnionCore_Effect[i].Time >= UnionCore_Effect[i].Max_Time) {
				memset(&UnionCore_Effect[i], 0, sizeof(sINVENTORY_EFFECT));
			}
		}
	}


	for (i = 0; i < 50; i++) {
		if (UnionCore_Effect_Bomb[i].Flag) {
			UnionCore_Effect_Bomb[i].Time++;
			/*
			if(UnionCore_Effect_Bomb[i].Direction == 1){
				if(UnionCore_Effect_Bomb[i].Time < 6){
					UnionCore_Effect_Bomb[i].InvenFace.width += 40;
					UnionCore_Effect_Bomb[i].InvenFace.x -=20;
				}
				else{
					UnionCore_Effect_Bomb[i].InvenFace.width -= 20;
					UnionCore_Effect_Bomb[i].InvenFace.x +=10;
				}
			}
			else{
				if(UnionCore_Effect_Bomb[i].Time < 6){
					UnionCore_Effect_Bomb[i].InvenFace.height +=20;
					UnionCore_Effect_Bomb[i].InvenFace.y -=10;
				}
				else{
					UnionCore_Effect_Bomb[i].InvenFace.height -=10;
					UnionCore_Effect_Bomb[i].InvenFace.y +=5;

				}
			}
			*/

			//UnionCore_Effect_Bomb[i].InvenFace.height += UnionCore_Effect_Bomb[i].Direction*2;

			//UnionCore_Effect_Bomb[i].InvenFace.x -= (UnionCore_Effect_Bomb[i].Direction/2)-2;
			//UnionCore_Effect_Bomb[i].InvenFace.y -= (UnionCore_Effect_Bomb[i].Direction/2)-2;

//			UnionCore_Effect_Bomb[i].InvenFace.Transparency -= 5;

			if (UnionCore_Effect_Bomb[i].Time >= UnionCore_Effect_Bomb[i].Max_Time) {
				memset(&UnionCore_Effect_Bomb[i], 0, sizeof(sINVENTORY_EFFECT));
			}

		}
	}

	for (int i = 0; i < 10; i++) {
		if (sUCore_Effect[i].Flag) {
			sUCore_Effect[i].Time++;
			if (sUCore_Effect[i].Time > 0) {
				sUCore_Effect[i].h += 2;
				sUCore_Effect[i].w += 2;
				sUCore_Effect[i].x--;
				sUCore_Effect[i].y--;
				sUCore_Effect[i].Alpha -= 6;
			}

			if (sUCore_Effect[i].Time > sUCore_Effect[i].Max_Time) {
				memset(&sUCore_Effect[i], 0, sizeof(sUNIONCORE_EFFECT));
			}
		}
	}




	return TRUE;
}

int cINVENTORY::UnionCoreParticle(int x, int y)
{
	for (int i = 0; i < 100; i++) {
		if (!UnionCore_Effect[i].Flag) {
			//UnionCore_Effect[i].InvenFace.MatNum = *(int *)AssaSearchRes("star05B_02.bmp",SMMAT_BLEND_LAMP);
			UnionCore_Effect[i].InvenFace.MatNum = *(int*)AssaSearchRes("par2.tga", SMMAT_BLEND_ALPHA);
			UnionCore_Effect[i].InvenFace.x = x + GetRandomPos(-3, 3);
			UnionCore_Effect[i].InvenFace.y = y + GetRandomPos(-15, 15);
			UnionCore_Effect[i].Max_Time = GetRandomPos(8, 15);
			UnionCore_Effect[i].InvenFace.r = 255;
			UnionCore_Effect[i].InvenFace.g = 255;
			UnionCore_Effect[i].InvenFace.b = 255;
			UnionCore_Effect[i].InvenFace.Transparency = 255;
			UnionCore_Effect[i].InvenFace.height = GetRandomPos(10, 11);
			UnionCore_Effect[i].InvenFace.width = UnionCore_Effect[i].InvenFace.height;
			UnionCore_Effect[i].InvenFace.TexRect = sinTexRect;
			UnionCore_Effect[i].Flag = 1;
			if ((i % 2) == 0)
				UnionCore_Effect[i].Direction = 1;
			else UnionCore_Effect[i].Direction = -1;

			break;

		}
	}
	return TRUE;
}

//���Ͽ� �ھ� ������ ����Ʈ 
int cINVENTORY::UnionCoreParticle2(int x, int y, int Flag)
{

	int Count = 0;
	for (int i = 0; i < 10; i++) {
		if (!sUCore_Effect[i].Flag) {
			if (Flag)
				sUCore_Effect[i].MatNum = MatInterEffectAging[0];
			else
				sUCore_Effect[i].MatNum = MatUnionCoreEffect;

			sUCore_Effect[i].x = x + 12;
			sUCore_Effect[i].y = y;
			sUCore_Effect[i].Max_Time = 40;
			sUCore_Effect[i].Alpha = 255;
			sUCore_Effect[i].w = 20;
			sUCore_Effect[i].h = 20;
			sUCore_Effect[i].Flag = 1;
			//sUCore_Effect[i].Time = -20*Count;
			Count++;
			if (Count > 1)break;

		}
	}


	/*
	for(int i =0 ; i < 50 ; i++){
		if(!UnionCore_Effect_Bomb[i].Flag){
			UnionCore_Effect_Bomb[i].InvenFace.MatNum = MatUnionCoreEffect;
			UnionCore_Effect_Bomb[i].InvenFace.x = x;
			UnionCore_Effect_Bomb[i].InvenFace.y = y-10;
			UnionCore_Effect_Bomb[i].Max_Time = 200;
			UnionCore_Effect_Bomb[i].InvenFace.r = 255;
			UnionCore_Effect_Bomb[i].InvenFace.g = 255;
			UnionCore_Effect_Bomb[i].InvenFace.b = 255;
			UnionCore_Effect_Bomb[i].InvenFace.Transparency = 255;
			UnionCore_Effect_Bomb[i].InvenFace.height = 32;
			UnionCore_Effect_Bomb[i].InvenFace.width  = UnionCore_Effect_Bomb[i].InvenFace.height;
			UnionCore_Effect_Bomb[i].InvenFace.TexRect = sinTexRect;
			UnionCore_Effect_Bomb[i].Flag = 1;
		}
	}
	*/
	return TRUE;
}


//�ڵ�� �κ��丮 ������ üũ�Ѵ� 
int cINVENTORY::CheckInvenEmpty(sITEM* pItem)
{
	//��������ġ�� ������ üũ�����ʴ´�
	if (pItem->CODE == (sinGG1 | sin01) || pItem->CODE == (sinGG1 | sin02))return TRUE;

	char szFilePath[256];
	for (int j = 0; j < MAX_ITEM; j++) {
		if (pItem->CODE == sItem[j].CODE) {
			wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[j].ItemFilePath, sItem[j].LastCategory);
			if (!sItem[j].lpTempItem) //�̹����� ������ �ε��Ѵ�
				sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);
			pItem->lpItem = sItem[j].lpTempItem; //������ �ִ� �̹��� �����͸� �־��ش� 
			pItem->w = sItem[j].w;
			pItem->h = sItem[j].h;
		}

	}

	memcpy(&cTrade.CheckItem, &cInvenTory.InvenItem, sizeof(sITEM) * 100); //���� üũ�� ���ؼ� ���� 
	if (cTrade.TradeCheckEmptyArea(pItem))
		return TRUE;
	else return FALSE;
}

//���̹����� ����ġ �̹����� �ε��Ѵ� 
int cINVENTORY::LoadMoneyExpImage(sITEM* pItem)
{
	//��
	if (pItem->CODE == (sinGG1 | sin01)) {
		if (!lpGold7)lpGold7 = LoadDibSurfaceOffscreen("image\\Sinimage\\MessageBox\\Gold7.bmp");
		pItem->lpItem = lpGold7;

	}
	if (pItem->CODE == (sinGG1 | sin02)) {
		if (!lpExp7)lpExp7 = LoadDibSurfaceOffscreen("image\\Sinimage\\MessageBox\\Exp7.bmp");
		pItem->lpItem = lpExp7;

	}
	pItem->w = 44;
	pItem->h = 44;
	return TRUE;
}

//sItemInfo�� �������� �κ��丮�� �����Ѵ�
int cINVENTORY::SetInvenToItemInfo(sITEMINFO* pItemInfo)
{

	//���� ���
	if (pItemInfo->CODE == (sinGG1 | sin01)) {
		CheckCharForm();//���� 
		sinPlusMoney(pItemInfo->Money);
		ReformCharForm();//������ 	
		SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
		sinPlaySound(SIN_SOUND_COIN);
		return TRUE;
	}

	//����ġ�ϰ�� 
	if (pItemInfo->CODE == (sinGG1 | sin02)) {
		AddExp(pItemInfo->Money);
		sinPlaySound(SIN_SOUND_EAT_POTION2);
		return TRUE;

	}

	//�������� ��� 
	sITEM TempItem;
	if (LoadItemImage(pItemInfo, &TempItem)) {
		if (cInvenTory.CheckRequireItemToSet(&TempItem)) {         //��ٷ� ���õɼ��ִ����� üũ�Ѵ�  
			if (!cInvenTory.AutoSetInvenItem(&TempItem, 1)) {
				TempItem.Flag = 0;
				return FALSE;
			}
		}
	}
	return TRUE;
}

extern BOOL bNewTeleportWindow;

/*******************************************************************************
*						Wing Item
********************************************************************************/
int cINVENTORY::sinWingQuestNpc() //�������� ����Ʈ 
{

	sITEM* pItem = 0;
	int   Index = 0;
	int   i = 0;

	WingItemQuestIndex = 0;//�ʱ�ȭ 
	memset(&sWingItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //�ʱ�ȭ

	for (i = 0; i < INVENTORY_MAXITEM * 2; i++) {
		if (i < 100) {
			if (!InvenItem[i].Flag)continue;
			pItem = &InvenItem[i];
		}
		else {
			if (!InvenItemTemp[i - 100].Flag)continue;
			pItem = &InvenItemTemp[i - 100];
		}
		switch (pItem->sItemInfo.CODE) {
		case (sinQW1 | sin01): //��Ż��
			Index = 1;
			memcpy(&sWingItem_Send.DesCraftItem, pItem, sizeof(sITEM));

			break;
		case (sinQW1 | sin02): //�ǹ���
			Index = 2;
			memcpy(&sWingItem_Send.DesCraftItem, pItem, sizeof(sITEM));

			break;
		case (sinQW1 | sin03): //�����
			Index = 3;
			memcpy(&sWingItem_Send.DesCraftItem, pItem, sizeof(sITEM));
			break;
		case (sinQW1 | sin04): //���̾���
			Index = 4;
			memcpy(&sWingItem_Send.DesCraftItem, pItem, sizeof(sITEM));
			break;
		case (sinQW1 | sin05): //���̾ƽ� ��
			Index = 5;
			memcpy(&sWingItem_Send.DesCraftItem, pItem, sizeof(sITEM));
			break;
		case (sinQW1 | sin06): //�ͽ�Ʈ���� �ν�Ʈ ���Ϸ��� ���� �߰�
			if (smConfig.DebugMode)
				if (WingItemQuestIndex = 1000)  //������������ ���� ��쿡���� ���� ����
					cSinHelp.sinShowHelp(SIN_HELP_KIND_QUEST_WING, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), WingQuestFilePath_b[6]);
			return FALSE;

			break;
		}
	}
	int CodeCount = 0;
	int ItemCount = 0;

	for (int j = 0; j < 6; j++) {
		if (sWingQuest[Index].NeedItemCode[j]) {
			CodeCount++;
			for (i = 0; i < INVENTORY_MAXITEM * 2; i++) {
				if (i < 100) {
					if (!InvenItem[i].Flag)continue;
					pItem = &InvenItem[i];
				}
				else {
					if (!InvenItemTemp[i - 100].Flag)continue;
					pItem = &InvenItemTemp[i - 100];
				}
				if (sWingQuest[Index].NeedItemCode[j] == pItem->sItemInfo.CODE) {
					sWingItem_Send.SheltomCode[ItemCount] = pItem->sItemInfo.CODE;
					sWingItem_Send.Head[ItemCount] = pItem->sItemInfo.ItemHeader.Head;
					sWingItem_Send.CheckSum[ItemCount] = pItem->sItemInfo.ItemHeader.dwChkSum;
					ItemCount++;
					break;
				}
			}
		}
	}


	if (CodeCount == ItemCount && sinChar->Money >= sWingQuest[Index].Money) {
		WingItemQuestIndex = Index;
		sWingItem_Send.Index = Index;
		sWingItem_Send.Money = sWingQuest[Index].Money;
		cSinHelp.sinShowHelp(SIN_HELP_KIND_QUEST_WING, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), WingQuestFilePath_b[Index]);
	}
	else if (bNewTeleportWindow == FALSE)
	{
		WingItemQuestIndex = 1000; //������������ ���� ��쿡���� ���� ����
		cSinHelp.sinShowHelp(SIN_HELP_KIND_QUEST_WING, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), WingQuestFilePath_a[Index]);
	}

	return TRUE;
}

//�����Ҽ��ִ� ������������ �ѹ��� üũ�Ѵ� 
int cINVENTORY::CheckRequireItemToSet2(sITEM* pItem)
{
	int NotUseItemFlag = 0;

	if (pItem->sItemInfo.Level > sinChar->Level)NotUseItemFlag = 1;
	if (pItem->sItemInfo.Dexterity > sinChar->Dexterity)NotUseItemFlag = 1;
	if (pItem->sItemInfo.Strength > sinChar->Strength)NotUseItemFlag = 1;
	if (pItem->sItemInfo.Talent > sinChar->Talent)NotUseItemFlag = 1;
	if (pItem->sItemInfo.Spirit > sinChar->Spirit)NotUseItemFlag = 1;
	if (pItem->sItemInfo.Health > sinChar->Health)NotUseItemFlag = 1;
	if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinFO1) {
		int i = 0;
		for (i = 0; i < 16; i++) {
			if ((pItem->CODE & sinITEM_MASK3) == SheltomCode2[i]) {
				break;
			}
		}
		if (sinChar->Level < ForceOrbUseLevel[i][0] ||
			sinChar->Level > ForceOrbUseLevel[i][1]) {
			NotUseItemFlag = 1;
		}
		else {
			NotUseItemFlag = 0;

		}

	}

	if (NotUseItemFlag) {
		return FALSE;
	}
	return TRUE;
}

//����Ⱦ����� ����
int cINVENTORY::CheckSpecialItemPosi()
{
	int j = 0;
	for (int i = 0; i < 84; i++) {
		if (InvenItem[i].Flag) {
			if (InvenItem[i].sItemInfo.SpecialItemFlag[1] == ITEM_KIND_SPECIAL) {
				for (j = 84; j < INVENTORY_MAXITEM; j++) {
					if (!InvenItem[j].Flag) {
						memcpy(&InvenItem[j], &InvenItem[i], sizeof(sITEM));
						memset(&InvenItem[i], 0, sizeof(sITEM));

					}
				}
			}
		}
		if (InvenItemTemp[i].Flag) {
			if (InvenItemTemp[i].sItemInfo.SpecialItemFlag[1] == ITEM_KIND_SPECIAL) {
				for (j = 84; j < INVENTORY_MAXITEM; j++) {
					if (!InvenItem[j].Flag) {
						memcpy(&InvenItem[j], &InvenItemTemp[i], sizeof(sITEM));
						memset(&InvenItemTemp[i], 0, sizeof(sITEM));

					}
				}
			}
		}

	}
	ReFormInvenItem();
	ReformCharForm();//������ 
	ReFormPotionNum();
	SetItemToChar();
	return TRUE;
}

//����Ⱦ������� �����ش�
int cINVENTORY::ChangeSpecialItem(int Flag)
{
	if (Flag == 1) { //AB��ư���� ������ �յڰ� �ٲ� ����Ⱦ�����â�� �������� �ٲ��ش�
		if (SpecialItemShowFlag) {
			SpecialItemShowFlag = 0;

		}
		else return FALSE;
	}
	else if (Flag == 2) { //�������� �Լ��Ҷ��� �ŷ��Ҷ� â�� ���� ������ ���� ��� â�� �ٲ۴�
		SpecialItemShowFlag = 0;

	}
	else SpecialItemShowFlag ^= 1;


	//����Ⱦ������� ��ǥ�� �̵���Ų��
	for (int i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag) {
			if (SpecialItemShowFlag) { //Ư���� �������� ���÷��̸� �ٸ����Ѵ�
				if (InvenItem[i].sItemInfo.SpecialItemFlag[1] == ITEM_KIND_SPECIAL) {
					if (InvenItem[i].x > 10000) {
						InvenItem[i].x -= 10000;
					}
				}
				else {
					if (!InvenItem[i].ItemPosition && InvenItem[i].x < 10000) {
						InvenItem[i].x += 10000;
					}
				}
			}
			else {
				if (InvenItem[i].sItemInfo.SpecialItemFlag[1] == ITEM_KIND_SPECIAL) {
					if (InvenItem[i].x < 10000) {
						InvenItem[i].x += 10000;
					}
				}
				else {
					if (!InvenItem[i].ItemPosition) {
						if (InvenItem[i].x > 10000) {
							InvenItem[i].x -= 10000;
						}
					}
				}
			}
		}
	}
	return TRUE;
}
//�ڵ峪 ������ ������ �κ��丮 ������ �ε����� �����Ѵ�
int cINVENTORY::SearchItemIndex(DWORD CODE, int KindFlag)
{
	int i = 0;
	if (CODE) {
		for (i = 0; i < INVENTORY_MAXITEM; i++) {
			if (InvenItem[i].Flag) {
				if (InvenItem[i].CODE == CODE) {
					return i + 1;

				}
			}
		}
		for (i = 0; i < INVENTORY_MAXITEM; i++) {
			if (InvenItemTemp[i].Flag) {
				if (InvenItemTemp[i].CODE == CODE) {
					return 100 + i + 1;
				}
			}
		}
	}

	if (KindFlag) {
		for (int i = 0; i < INVENTORY_MAXITEM; i++) {
			if (InvenItem[i].Flag) {
				if (InvenItem[i].sItemInfo.ItemKindCode == KindFlag) {
					return i + 1;

				}
			}
			if (InvenItemTemp[i].Flag) {
				if (InvenItemTemp[i].sItemInfo.ItemKindCode == KindFlag) {
					return 100 + i + 1;

				}
			}
		}
	}
	return FALSE;
}

//����� - ���� ��ƿ���
int WatermelonEventItemIndex[INVENTORY_MAXITEM * 2] = { 0, };

//�庰 - �߷�Ÿ�� ���ݷ� ��ƿ���
int ValentineEventItemIndex[INVENTORY_MAXITEM * 2] = { 0, };

// �庰 - ĵ������
int CandydaysEventItemIndex[INVENTORY_MAXITEM * 2] = { 0, };

// �庰 - �����ñ׸�
int MagicalGreenEventItemIndex[INVENTORY_MAXITEM * 2] = { 0, };
int MagicalGreenEmeraldEventItemIndex[INVENTORY_MAXITEM * 2] = { 0, };

// �庰 - ī���� ����
int TearOfKaraEventItemIndex[INVENTORY_MAXITEM * 2] = { 0, };

// �庰 - ������� ã�ƶ� 
int FindinvestigatorNineEventItemIndex[INVENTORY_MAXITEM * 2] = { 0, }; // ���� �ƹķ�
int FindinvestigatorTaleEventItemIndex[INVENTORY_MAXITEM * 2] = { 0, }; // ���� �ƹķ�

// ����� - ���� �������� ������ �����Ѵ�.(�κ����� ���� ���� �����ϱ�)
int cINVENTORY::CheckItemCount(DWORD CODE)
{
	int i = 0;
	int item_count = 0;
	if (CODE)
	{
		for (i = 0; i < INVENTORY_MAXITEM; i++)
		{
			if (InvenItem[i].Flag)
			{
				if (InvenItem[i].CODE == CODE)
				{
					item_count++;
					WatermelonEventItemIndex[item_count - 1] = i + 1;
				}
			}
		}

		for (i = 0; i < INVENTORY_MAXITEM; i++)
		{
			if (InvenItemTemp[i].Flag)
			{
				if (InvenItemTemp[i].CODE == CODE)
				{
					item_count++;
					WatermelonEventItemIndex[item_count - 1] = 100 + i + 1;
				}
			}
		}
	}

	return item_count;
}


// �庰 - ���� �������� ������ �����Ѵ�.(�κ����� �߷�Ÿ�� ���ݷ� ���� �����ϱ�)
int cINVENTORY::CheckValentineItemCount(DWORD CODE)
{
	int i = 0;
	int item_count = 0;
	if (CODE)
	{
		for (i = 0; i < INVENTORY_MAXITEM; i++)
		{
			if (InvenItem[i].Flag)
			{
				if (InvenItem[i].CODE == CODE)
				{
					item_count++;
					ValentineEventItemIndex[item_count - 1] = i + 1;
				}
			}
		}

		for (i = 0; i < INVENTORY_MAXITEM; i++)
		{
			if (InvenItemTemp[i].Flag)
			{
				if (InvenItemTemp[i].CODE == CODE)
				{
					item_count++;
					ValentineEventItemIndex[item_count - 1] = 100 + i + 1;
				}
			}
		}
	}

	return item_count;
}


// �庰 - ĵ������ ���� �������� ������ �����Ѵ�.(�κ����� ĵ�� ���� �����ϱ�)
int cINVENTORY::CheckCandyItemCount(DWORD CODE)
{
	int i = 0;
	int item_count = 0;
	if (CODE)
	{
		for (i = 0; i < INVENTORY_MAXITEM; i++)
		{
			if (InvenItem[i].Flag)
			{
				if (InvenItem[i].CODE == CODE)
				{
					item_count++;
					CandydaysEventItemIndex[item_count - 1] = i + 1;
				}
			}
		}

		for (i = 0; i < INVENTORY_MAXITEM; i++)
		{
			if (InvenItemTemp[i].Flag)
			{
				if (InvenItemTemp[i].CODE == CODE)
				{
					item_count++;
					CandydaysEventItemIndex[item_count - 1] = 100 + i + 1;
				}
			}
		}
	}

	return item_count;
}


// �庰 - �����ñ׸�
int cINVENTORY::CheckMagicalGreenItemCount(DWORD CODE)
{
	int i = 0;
	int item_count = 0;
	if (CODE)
	{
		for (i = 0; i < INVENTORY_MAXITEM; i++)
		{
			if (InvenItem[i].Flag)
			{
				if (InvenItem[i].CODE == CODE)
				{
					item_count++;
					MagicalGreenEventItemIndex[item_count - 1] = i + 1;
				}
			}
		}

		for (i = 0; i < INVENTORY_MAXITEM; i++)
		{
			if (InvenItemTemp[i].Flag)
			{
				if (InvenItemTemp[i].CODE == CODE)
				{
					item_count++;
					MagicalGreenEventItemIndex[item_count - 1] = 100 + i + 1;
				}
			}
		}
	}

	return item_count;
}

// �庰 - �����ñ׸�
int cINVENTORY::CheckMagicalGreenItemEmeraldCount(DWORD CODE)
{
	int i = 0;
	int item_count = 0;
	if (CODE)
	{
		for (i = 0; i < INVENTORY_MAXITEM; i++)
		{
			if (InvenItem[i].Flag)
			{
				if (InvenItem[i].CODE == CODE)
				{
					item_count++;
					MagicalGreenEmeraldEventItemIndex[item_count - 1] = i + 1;
				}
			}
		}

		for (i = 0; i < INVENTORY_MAXITEM; i++)
		{
			if (InvenItemTemp[i].Flag)
			{
				if (InvenItemTemp[i].CODE == CODE)
				{
					item_count++;
					MagicalGreenEmeraldEventItemIndex[item_count - 1] = 100 + i + 1;
				}
			}
		}
	}

	return item_count;
}


// �庰 - ī���� ���� - �̺�Ʈ ������ ���� ����
int cINVENTORY::CheckTearOfKaraItemCount(DWORD CODE)
{
	int i = 0;
	int item_count = 0;
	if (CODE)
	{
		for (i = 0; i < INVENTORY_MAXITEM; i++)
		{
			if (InvenItem[i].Flag)
			{
				if (InvenItem[i].CODE == CODE)
				{
					item_count++;
					TearOfKaraEventItemIndex[item_count - 1] = i + 1;
				}
			}
		}

		for (i = 0; i < INVENTORY_MAXITEM; i++)
		{
			if (InvenItemTemp[i].Flag)
			{
				if (InvenItemTemp[i].CODE == CODE)
				{
					item_count++;
					TearOfKaraEventItemIndex[item_count - 1] = 100 + i + 1;
				}
			}
		}
	}

	return item_count;
}


// �庰 - ������� ã�ƶ� - ���� �ƹķ� ���� ����
int cINVENTORY::CheckFindinvestigatorNineItemCount(DWORD CODE)
{
	int i = 0;
	int item_count = 0;
	if (CODE)
	{
		for (i = 0; i < INVENTORY_MAXITEM; i++)
		{
			if (InvenItem[i].Flag)
			{
				if (InvenItem[i].CODE == CODE)
				{
					item_count++;
					FindinvestigatorNineEventItemIndex[item_count - 1] = i + 1;
				}
			}
		}

		for (i = 0; i < INVENTORY_MAXITEM; i++)
		{
			if (InvenItemTemp[i].Flag)
			{
				if (InvenItemTemp[i].CODE == CODE)
				{
					item_count++;
					FindinvestigatorNineEventItemIndex[item_count - 1] = 100 + i + 1;
				}
			}
		}
	}

	return item_count;
}


// �庰 - ������� ã�ƶ� - ���� �ƹķ� ���� ����
int cINVENTORY::CheckFindinvestigatorTaleItemCount(DWORD CODE)
{
	int i = 0;
	int item_count = 0;
	if (CODE)
	{
		for (i = 0; i < INVENTORY_MAXITEM; i++)
		{
			if (InvenItem[i].Flag)
			{
				if (InvenItem[i].CODE == CODE)
				{
					item_count++;
					FindinvestigatorTaleEventItemIndex[item_count - 1] = i + 1;
				}
			}
		}

		for (i = 0; i < INVENTORY_MAXITEM; i++)
		{
			if (InvenItemTemp[i].Flag)
			{
				if (InvenItemTemp[i].CODE == CODE)
				{
					item_count++;
					FindinvestigatorTaleEventItemIndex[item_count - 1] = 100 + i + 1;
				}
			}
		}
	}

	return item_count;
}

//���ָӴϿ��� ����� �����Ѵ�
int cINVENTORY::ChangePuzzle(sITEM* pItem)
{
	char szTestBuff2[256];
	szTestBuff2[0] = 0; //�����ʱ�ȭ  
	if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinPZ1 || (pItem->sItemInfo.CODE & sinITEM_MASK2) == sinPZ2) {
		if (pItem->sItemInfo.PotionCount <= 1) {
			pItem->sItemInfo.PotionCount = 2;
			for (int j = 0; j < MAX_ITEM; j++) {
				if (pItem->sItemInfo.CODE == sItem[j].CODE) {
					CheckInvenItemForm();
					//memcpy(pItem,&sItem[j],sizeof(sITEM));
					wsprintf(szTestBuff2, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[j].ItemFilePath, sItem[j].LastCategory);
					sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szTestBuff2);
					pItem->lpItem = sItem[j].lpTempItem;
					pItem->w = sItem[j].w;
					pItem->h = sItem[j].h;
					pItem->CODE = sItem[j].CODE;   //���� ������ �̿��� ���� �ڵ嵵 �Ѱ��ش�.
					CheckWeight();//���� ���� 
					ReFormInvenItem();
					sinMinusMoney(1000);
					ReformCharForm();//������ 
					SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
					sinUsePotionDelayFlag = 1;
					cInvenTory.UnionCoreParticle2(pItem->x, pItem->y, 1); //����Ʈ 
					cInvenTory.UnionCoreParticle2(pItem->x, pItem->y, 1); //����Ʈ 
					cInvenTory.UnionCoreParticle2(pItem->x, pItem->y, 1); //����Ʈ 
//					SetPuzzleEffect(pItem->x , pItem->y); //����Ʈ 
					sinPlaySound(SIN_SOUND_EAT_POTION2); //���Ͽ��ھ� ����
					SaveGameData();
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

DWORD CheckPuzzleIndex[4][4] = {
	{sin01,0,sin02,0},
	{sin03,sin04,0,sin06},
	{0,sin05,0,0},
	{sin07,0,sin08,0},
};

// Corre��o Puzzle mec�nico
DWORD CheckPuzzleIndex2[4][4] = {
	{sin01,0,sin03,sin04},
	{sin02,0,0,0},
	{sin05,sin06,sin07,0},
	{0,0,sin08,0},
};


/*DWORD CheckPuzzleIndex2[4][4] = {
	{sin01,0,sin02,0},
	{sin03,sin04,0,sin06},
	{0,sin05,0,0},
	{sin07,0,sin08,0},
};
*/
DWORD CheckPuzzleX[4] = { 8,9,10,11 };
int   PuzzleIndex[8] = { 0, };

//������ üũ�Ѵ�
int cINVENTORY::CheckPuzzle()
{
	int Count = 0;
	int Count2 = 0;
	int i, j, k;
	PuzzleOkFlag = 0;
	for (int p = 0; p < 8; p++) {
		PuzzleIndex[p] = 0;
	}

	for (i = 0; i < INVENTORY_MAXITEM; i++) {
		if (InvenItem[i].Flag)
		{
			if ((InvenItem[i].sItemInfo.CODE & sinITEM_MASK2) == sinPZ1) {
				for (j = 0; j < 4; j++) {
					for (k = 0; k < 4; k++) {
						if ((InvenItem[i].sItemInfo.CODE & sinITEM_MASK3) == CheckPuzzleIndex[j][k]) {
							if ((StartX + (CheckPuzzleX[k] * 22)) == InvenItem[i].x &&
								(StartY + (j * 22)) == InvenItem[i].y) {
								PuzzleIndex[Count] = i + 1;
								Count++;

							}
						}
					}
				}
			}

			if ((InvenItem[i].sItemInfo.CODE & sinITEM_MASK2) == sinPZ2) {
				for (j = 0; j < 4; j++)
				{
					for (k = 0; k < 4; k++)
					{
						if ((InvenItem[i].sItemInfo.CODE & sinITEM_MASK3) == CheckPuzzleIndex2[j][k])
						{
							if ((StartX + (CheckPuzzleX[k] * 22)) == InvenItem[i].x &&
								(StartY + (j * 22)) == InvenItem[i].y) {
								PuzzleIndex[Count2] = i + 1;
								Count2++;

							}
						}
					}
				}
			}
		}
	}
	if (Count == 8 || Count2 == 8) {
		//if(!PuzzleOkFlag)
		//	cMessageBox.ShowMessage(MESSAGE_PERPFECT_PUZZLE);
		PuzzleOkFlag = 1;

	}
	return TRUE;
}


//�ٺ��� ���� üũ�Ѵ�
int cINVENTORY::CheckEventItem()
{

	int ItemIndex = SearchItemIndex((sinGF1 | sin02), 0);
	if (ItemIndex && ItemIndex < 100) {
		cInterFace.CheckAllBox(SIN_INVENTORY);
		cMessageBox.ShowMessage3(MESSAGE_EVENTITEM_SEND, BabelHorn);
		return TRUE;
	}
	else
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), BabelEventDoc);
	cMessageBox.ShowMessage3(MESSAGE_FALLGAME, FallGameName);
	return TRUE;
}

//�ٺ��� ���� ������
int cINVENTORY::SendEventItem()
{
	int Count = 0;
	int ItemIndex = 0;
	sITEM TempItem;
	TempItem.w = 22 * 2;
	TempItem.h = 22 * 2;


	memset(&sWingItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //�ʱ�ȭ

	ItemIndex = SearchItemIndex((sinGF1 | sin02), 0);
	if (ItemIndex && ItemIndex < 100) {
		if (sinChar->Weight[0] + 30 > sinChar->Weight[1]) {
			cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
			return FALSE;

		}		//���⼭ ������ ����üũ���غ���
		if (!cCraftItem.CraftCheckEmptyArea(&TempItem)) {
			cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
			return FALSE;
		}
		sWingItem_Send.SheltomCode[0] = InvenItem[ItemIndex - 1].sItemInfo.CODE;
		sWingItem_Send.Head[0] = InvenItem[ItemIndex - 1].sItemInfo.ItemHeader.Head;
		sWingItem_Send.CheckSum[0] = InvenItem[ItemIndex - 1].sItemInfo.ItemHeader.dwChkSum;
	}

	sWingItem_Send.DocIndex = 2;
	return TRUE;

}

//�������� �ƹ����� üũ�Ѵ�
int cINVENTORY::CheckFoxEventItem()
{

	int ItemIndex = SearchItemIndex((sinGF1 | sin03), 0);
	int ItemIndex2 = SearchItemIndex((sinGF1 | sin04), 0);

	if (ItemIndex && ItemIndex < 100 && ItemIndex2 && ItemIndex2 < 100) {
		cInterFace.CheckAllBox(SIN_INVENTORY);
		cMessageBox.ShowMessage3(MESSAGE_FOXEVENTITEM_SEND, NineTailFoxItem);
	}
	else
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), NineTailFoxDoc);
	return TRUE;
}

//�������� �ƹ����� ������
int cINVENTORY::SendFoxEventItem()
{
	int Count = 0;
	int ItemIndex = 0;
	int ItemIndex2 = 0;
	sITEM TempItem;
	TempItem.w = 22 * 3;
	TempItem.h = 22 * 4;


	memset(&sWingItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //�ʱ�ȭ

	ItemIndex = SearchItemIndex((sinGF1 | sin03), 0);
	ItemIndex2 = SearchItemIndex((sinGF1 | sin04), 0);
	if (ItemIndex && ItemIndex < 100 && ItemIndex2 && ItemIndex2 < 100) {
		if (sinChar->Weight[0] + 100 > sinChar->Weight[1]) {
			cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
			return FALSE;

		}
		//���⼭ ������ ����üũ���غ���
		if (!cCraftItem.CraftCheckEmptyArea(&TempItem)) {
			cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
			return FALSE;
		}
		sWingItem_Send.SheltomCode[0] = InvenItem[ItemIndex - 1].sItemInfo.CODE;
		sWingItem_Send.Head[0] = InvenItem[ItemIndex - 1].sItemInfo.ItemHeader.Head;
		sWingItem_Send.CheckSum[0] = InvenItem[ItemIndex - 1].sItemInfo.ItemHeader.dwChkSum;

		sWingItem_Send.SheltomCode[1] = InvenItem[ItemIndex2 - 1].sItemInfo.CODE;
		sWingItem_Send.Head[1] = InvenItem[ItemIndex2 - 1].sItemInfo.ItemHeader.Head;
		sWingItem_Send.CheckSum[1] = InvenItem[ItemIndex2 - 1].sItemInfo.ItemHeader.dwChkSum;

	}
	sWingItem_Send.DocIndex = 2;
	return TRUE;

}


//��¦��¦ ���縦 üũ�Ѵ�
int cINVENTORY::CheckChristMasEventItem()
{

	int ItemIndex = SearchItemIndex((sinGF1 | sin05), 0);

	if (ItemIndex && ItemIndex < 100) {
		cInterFace.CheckAllBox(SIN_INVENTORY);
		//		 cMessageBox.ShowMessage3(MESSAGE_CHRISTMASITEM_SEND, ChristMas );
	}
	else
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), StarDustDoc);
	return TRUE;
}

//��¦��¦ ���縦 ������
int cINVENTORY::SendChristmasEventItem()
{
	int Count = 0;
	int ItemIndex = 0;

	sITEM TempItem;
	TempItem.w = 22 * 3;
	TempItem.h = 22 * 4;


	memset(&sWingItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //�ʱ�ȭ

	ItemIndex = SearchItemIndex((sinGF1 | sin05), 0);

	if (ItemIndex && ItemIndex < 100) {
		if (sinChar->Weight[0] + 100 > sinChar->Weight[1]) {
			cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
			return FALSE;

		}
		//���⼭ ������ ����üũ���غ���
		if (!cCraftItem.CraftCheckEmptyArea(&TempItem)) {
			cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
			return FALSE;
		}
		sWingItem_Send.SheltomCode[0] = InvenItem[ItemIndex - 1].sItemInfo.CODE;
		sWingItem_Send.Head[0] = InvenItem[ItemIndex - 1].sItemInfo.ItemHeader.Head;
		sWingItem_Send.CheckSum[0] = InvenItem[ItemIndex - 1].sItemInfo.ItemHeader.dwChkSum;

	}
	sWingItem_Send.DocIndex = 2;
	return TRUE;


}
//������ ������
int cINVENTORY::CheckSendPuzzle()
{
	int Count = 0;
	memset(&sWingItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //�ʱ�ȭ
	for (int i = 0; i < 8; i++) {
		if (PuzzleIndex[i] && InvenItem[PuzzleIndex[i] - 1].Flag) {
			if ((InvenItem[PuzzleIndex[i] - 1].sItemInfo.CODE & sinITEM_MASK2) == sinPZ1 ||
				(InvenItem[PuzzleIndex[i] - 1].sItemInfo.CODE & sinITEM_MASK2) == sinPZ2) {
				sWingItem_Send.SheltomCode[i] = InvenItem[PuzzleIndex[i] - 1].sItemInfo.CODE;
				sWingItem_Send.Head[i] = InvenItem[PuzzleIndex[i] - 1].sItemInfo.ItemHeader.Head;
				sWingItem_Send.CheckSum[i] = InvenItem[PuzzleIndex[i] - 1].sItemInfo.ItemHeader.dwChkSum;
				Count++;
			}
		}
	}
	if (Count == 8) {
		////////���� �ڵ� ����
		DWORD Temp = 0;
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 8; k++) {
				if (j == k)continue;
				if (sWingItem_Send.SheltomCode[j] == sWingItem_Send.SheltomCode[k]) {
					cMessageBox.ShowMessage(MESSAGE_FAILD_PUZZLE);
					return FALSE;

				}
			}
		}
	}
	else {
		cMessageBox.ShowMessage(MESSAGE_FAILD_PUZZLE);
		return FALSE;
	}
	if (sinChar->Weight[0] + 120 > sinChar->Weight[1]) {		//��������� �κ��丮 ����(����120) (puzzle)
		cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
		return FALSE;

	}
	sWingItem_Send.DocIndex = 1;
	return TRUE;
}

int cINVENTORY::SendPuzzleNpc()
{

	if (PuzzleOkFlag) {
		cInterFace.CheckAllBox(SIN_INVENTORY);
		cMessageBox.ShowMessage3(MESSAGE_PUZZLE_SEND, PuzzleEvent5);
	}
	else {
		WingItemQuestIndex = 1000; //���������� �״�� ���ٿԴ� ������ 
		cSinHelp.sinShowHelp(SIN_HELP_KIND_QUEST_WING, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), WingQuestFilePath_a[6]);  //WingQuestFilePath_a[5]);  //������ //�ؿ�
	}
	return TRUE;
}

//�������� �������� �����Ѵ� 
int cINVENTORY::IncreViewDamage(int Persent, int Num)
{
	ViewDamagePercent = Persent;
	ViewDamageNum = Num;
	SetItemToChar();
	return TRUE;
}

//ForceOrb�� ����Ʈ�� �׸���
int cINVENTORY::DrawForceOrbEffect(int x, int y)
{
	//�̹��� �ε�

	int i = 0;
	char* Path = "image\\Sinimage\\Effect\\ForceOrb\\P_0%d.tga";
	char szBuff[128];
	if (!MatForceOrbEffect[0]) {
		for (i = 0; i < 8; i++) {
			wsprintf(szBuff, Path, i + 1);
			MatForceOrbEffect[i] = CreateTextureMaterial(szBuff, 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

		}
		ReadTextures();	//�ؽ��� �ε�
	}


	//ForceOrbEffectIndex
	ForceOrb_Effect.InvenFace.MatNum = MatForceOrbEffect[ForceOrbEffectIndex];
	ForceOrb_Effect.InvenFace.x = x - 5;
	ForceOrb_Effect.InvenFace.y = y - 5;
	ForceOrb_Effect.InvenFace.r = 255;
	ForceOrb_Effect.InvenFace.g = 255;
	ForceOrb_Effect.InvenFace.b = 255;
	ForceOrb_Effect.InvenFace.Transparency = 255;
	ForceOrb_Effect.InvenFace.height = 32;
	ForceOrb_Effect.InvenFace.width = 32;
	ForceOrb_Effect.InvenFace.TexRect = sinTexRect;

	sinDrawInvenTexImage(&ForceOrb_Effect.InvenFace);
	return TRUE;
}

int cINVENTORY::UseForceOrb(sITEM* pItem, int Flag)
{
	if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_VILLAGE)
		return FALSE;

	if (sinItemPickUpDelayFlag)
		return FALSE;

	int UseFlag = 0;
	sSKILL TempSkill;
	memset(&TempSkill, 0, sizeof(sSKILL));

	int i = 0;
	if (!smConfig.DebugMode) {
		for (i = 0; i < 15; i++) {
			if ((pItem->CODE & sinITEM_MASK3) == SheltomCode2[i]) {
				if (sinChar->Level >= ForceOrbUseLevel[i][0] &&
					sinChar->Level <= ForceOrbUseLevel[i][1]) {
					UseFlag = i + 1;
				}
			}
		}

		if (!UseFlag)
		{
			cMessageBox.ShowMessage(MESSAGE_NOT_USE_FORCEORB);
			return FALSE;

		}
	}
	else {
		for (i = 0; i < 16; i++) {
			if ((pItem->CODE & sinITEM_MASK3) == SheltomCode2[i]) {
				UseFlag = i + 1;

			}
		}
	}

	if (CSKILL->SearchContiueSkill(SKILL_FORCE_ORB))
	{
		cMessageBox.ShowMessage(MESSAGE_NOT_USE_FORCEORB2);
		return FALSE;
	}


	char* Path = "image\\Sinimage\\skill\\ForceOrb\\FOS_0%d.tga";   // ����� : ���� ����(���� ������ 10���� 12�߰�)
	char szBuff[128];

	if (!MatForceOrb[UseFlag - 1])
	{
		wsprintf(szBuff, Path, UseFlag);
		MatForceOrb[UseFlag - 1] = CreateTextureMaterial(szBuff, 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
		ReadTextures();
	}

	TempSkill.CODE = SKILL_FORCE_ORB;
	TempSkill.Point = UseFlag;
	TempSkill.UseTime = ForceOrbUseTime[UseFlag - 1];
	TempSkill.MatIcon = MatForceOrb[UseFlag - 1];
	sinContinueSkillSet(&TempSkill);
	pItem->sItemInfo.PotionCount = 100;
	sinThrowItemToFeild(pItem);
	pItem->Flag = 0;

	cInvenTory.SetItemToChar();
	cInvenTory.ReFormInvenItem();
	cInvenTory.CheckWeight();
	sinUsePotionDelayFlag = 1;
	sinPlaySound(SIN_SOUND_EAT_POTION2);
	return TRUE;
}

int cINVENTORY::UseMagicForceOrb(sITEM* pItem, int Flag)
{
	if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_VILLAGE)
		return FALSE; //���������� ������Ѵ�

	//if(!Flag && sinItemPickUpDelayFlag)return FALSE; //���ѿ��׸� �ھ ���´�    
	if (sinItemPickUpDelayFlag)return FALSE; //���ѿ��׸� �ھ ���´�    
	int UseFlag = 0;
	sSKILL TempSkill;
	memset(&TempSkill, 0, sizeof(sSKILL));

	int i = 0;
	if (!smConfig.DebugMode) {
		for (i = 0; i < 13; i++) {
			if ((pItem->CODE & sinITEM_MASK3) == MagicSheltomCode[i]) {
				if (sinChar->Level >= ForceOrbUseLevel[i][0] &&
					sinChar->Level <= ForceOrbUseLevel[i][1]) {
					UseFlag = i + 1;
				}
			}
		}

		if (!UseFlag) {
			cMessageBox.ShowMessage(MESSAGE_NOT_USE_FORCEORB);
			return FALSE;

		}
	}
	else {
		for (i = 0; i < 13; i++) {
			if ((pItem->CODE & sinITEM_MASK3) == MagicSheltomCode[i]) {
				UseFlag = i + 1;

			}
		}
	}

	if (CSKILL->SearchContiueSkill(SKILL_FORCE_ORB)) {
		cMessageBox.ShowMessage(MESSAGE_NOT_USE_FORCEORB2);
		return FALSE;
	}

	//�ؽ��� �ε�
	char* Path = "image\\Sinimage\\skill\\ForceOrb\\FOS_0%d.tga";   // ����� - ���� ����(���� ������ 21���� 32�߰�)
	char szBuff[128];
	if (!MatForceOrb[UseFlag - 1]) {
		wsprintf(szBuff, Path, UseFlag);
		MatForceOrb[UseFlag - 1] = CreateTextureMaterial(szBuff, 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
		ReadTextures();	//�ؽ��� �ε�
	}

	TempSkill.CODE = SKILL_FORCE_ORB;
	TempSkill.Point = UseFlag;
	TempSkill.ElementIndex = 2000; // ����� - ���� ���� ������ ����.(���� ���� �߰�)
	TempSkill.UseTime = MagicForceOrbUseTime[UseFlag - 1];
	TempSkill.MatIcon = MatForceOrb[UseFlag - 1];
	sinContinueSkillSet(&TempSkill);
	pItem->sItemInfo.PotionCount = 100;
	sinThrowItemToFeild(pItem);
	pItem->Flag = 0;

	cInvenTory.SetItemToChar(); //�������� ���õǸ� �ɷ�ġ�� �����Ѵ� 
	cInvenTory.ReFormInvenItem();
	cInvenTory.CheckWeight();   //���Ը� �����Ѵ� 
	sinUsePotionDelayFlag = 1;
	sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
	return TRUE;
}

// ����� - ���� ���� ���� �߰�(���� ���� ������ ����Ѵ�)
int cINVENTORY::UseBillingMagicForceOrb(sITEM* pItem, int Flag)
{
	if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_VILLAGE)
		return FALSE; //���������� ������Ѵ�

	//if(!Flag && sinItemPickUpDelayFlag)return FALSE; //���ѿ��׸� �ھ ���´�    
	if (sinItemPickUpDelayFlag)return FALSE; //���ѿ��׸� �ھ ���´�    
	int UseFlag = 0;
	sSKILL TempSkill;
	memset(&TempSkill, 0, sizeof(sSKILL));

	int i = 0;
	if (!smConfig.DebugMode)
	{
		for (i = 0; i < 3; i++)
		{
			if ((pItem->CODE & sinITEM_MASK3) == BillingMagicSheltomCode[i])
			{
				if (sinChar->Level >= BillingMagicForceOrbUseLevel[i][0] && sinChar->Level <= BillingMagicForceOrbUseLevel[i][1])
				{
					UseFlag = i + 1;
				}
			}
		}

		if (!UseFlag) {
			cMessageBox.ShowMessage(MESSAGE_NOT_USE_FORCEORB);
			return FALSE;

		}
	}
	else {
		for (i = 0; i < 3; i++) {
			if ((pItem->CODE & sinITEM_MASK3) == BillingMagicSheltomCode[i]) {
				UseFlag = i + 1;

			}
		}
	}

	if (CSKILL->SearchContiueSkill(SKILL_FORCE_ORB)) {
		cMessageBox.ShowMessage(MESSAGE_NOT_USE_FORCEORB2);
		return FALSE;
	}

	//�ؽ��� �ε�
	char* Path = "image\\Sinimage\\skill\\ForceOrb\\FOS_0%d.tga";   // ����� : ���� ���� ����(���� ������ 35���� 37�߰�)
	char szBuff[128];
	if (!MatBillingMagicForceOrb[UseFlag - 1]) {
		wsprintf(szBuff, Path, UseFlag + 34);
		MatBillingMagicForceOrb[UseFlag - 1] = CreateTextureMaterial(szBuff, 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
		ReadTextures();	//�ؽ��� �ε�
	}

	TempSkill.CODE = SKILL_FORCE_ORB;
	TempSkill.Point = UseFlag;
	TempSkill.ElementIndex = 1000; // ���� ���� ���� ������ ����.
	TempSkill.UseTime = BillingMagicForceOrbUseTime[UseFlag - 1];
	TempSkill.MatIcon = MatBillingMagicForceOrb[UseFlag - 1];
	sinContinueSkillSet(&TempSkill);
	pItem->sItemInfo.PotionCount = 100;
	sinThrowItemToFeild(pItem);
	pItem->Flag = 0;

	cInvenTory.SetItemToChar(); //�������� ���õǸ� �ɷ�ġ�� �����Ѵ� 
	cInvenTory.ReFormInvenItem();
	cInvenTory.CheckWeight();   //���Ը� �����Ѵ� 
	sinUsePotionDelayFlag = 1;
	sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
	return TRUE;
}

// ����� - ���� ���� �߰� (���� ������ �����Ѵ�.)
int cINVENTORY::SetMagicForceOrb(DWORD ItemCODE, int Time)
{
	int UseFlag = 0;
	sSKILL TempSkill;
	memset(&TempSkill, 0, sizeof(sSKILL));

	for (int i = 0; i < 12; i++) // ���� ����
	{
		if ((ItemCODE & sinITEM_MASK3) == MagicSheltomCode[i]) // ���� ������ ���� ~ ���� �̴ϱ׸� ����
		{
			UseFlag = i + 1;
		}
	}

	char* Path = "image\\Sinimage\\skill\\ForceOrb\\FOS_0%d.tga";
	char szBuff[128];
	if (!MatForceOrb[UseFlag - 1]) {
		wsprintf(szBuff, Path, UseFlag + 20);
		MatForceOrb[UseFlag - 1] = CreateTextureMaterial(szBuff, 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
		ReadTextures();	//�ؽ��� �ε�
	}

	TempSkill.CODE = SKILL_FORCE_ORB;
	TempSkill.Point = UseFlag;
	TempSkill.ElementIndex = 2000;
	TempSkill.UseTime = MagicForceOrbUseTime[UseFlag - 1];
	TempSkill.CheckTime = (MagicForceOrbUseTime[UseFlag - 1] * 70) - (Time * 70);
	TempSkill.MatIcon = MatForceOrb[UseFlag - 1];
	sinContinueSkillSet(&TempSkill);
	return TRUE;

}

// ����� - ���� ���� ���� �߰� (���� ���� ������ �����Ѵ�.)
int cINVENTORY::SetBillingMagicForceOrb(DWORD ItemCODE, int Time)
{
	int UseFlag = 0;
	sSKILL TempSkill;
	memset(&TempSkill, 0, sizeof(sSKILL));

	for (int i = 0; i < 3; i++) // ���� ���� ����
	{
		if ((ItemCODE & sinITEM_MASK3) == BillingMagicSheltomCode[i]) // ���� ���� ������ ������ ������� ������ �ڵ� ��ȣ�� ���� ����Ѵ�.
		{
			UseFlag = i + 1;
		}
	}

	char* Path = "image\\Sinimage\\skill\\ForceOrb\\FOS_0%d.tga";
	char szBuff[128];
	if (!MatForceOrb[UseFlag - 1]) {
		wsprintf(szBuff, Path, UseFlag + 34);
		MatForceOrb[UseFlag - 1] = CreateTextureMaterial(szBuff, 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
		ReadTextures();	//�ؽ��� �ε�
	}

	TempSkill.CODE = SKILL_FORCE_ORB;
	TempSkill.Point = UseFlag;
	TempSkill.ElementIndex = 1000;
	TempSkill.UseTime = BillingMagicForceOrbUseTime[UseFlag - 1];
	TempSkill.CheckTime = (BillingMagicForceOrbUseTime[UseFlag - 1] * 70) - (Time * 70);
	TempSkill.MatIcon = MatForceOrb[UseFlag - 1];
	sinContinueSkillSet(&TempSkill);
	return TRUE;

}

//������������ �����Ѵ�(���������,��Ÿ���)
int cINVENTORY::SetForceOrb(DWORD ItemCODE, int Time)
{
	int UseFlag = 0;
	sSKILL TempSkill;
	memset(&TempSkill, 0, sizeof(sSKILL));

	for (int i = 0; i < 16; i++) {
		if ((ItemCODE & sinITEM_MASK3) == SheltomCode2[i]) {
			UseFlag = i + 1;

		}
	}
	char* Path = "image\\Sinimage\\skill\\ForceOrb\\FOS_0%d.tga";
	char szBuff[128];
	if (!MatForceOrb[UseFlag - 1]) {
		wsprintf(szBuff, Path, UseFlag);
		MatForceOrb[UseFlag - 1] = CreateTextureMaterial(szBuff, 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
		ReadTextures();	//�ؽ��� �ε�
	}


	TempSkill.CODE = SKILL_FORCE_ORB;
	TempSkill.Point = UseFlag;
	TempSkill.UseTime = ForceOrbUseTime[UseFlag - 1];
	TempSkill.CheckTime = (ForceOrbUseTime[UseFlag - 1] * 70) - (Time * 70);
	TempSkill.MatIcon = MatForceOrb[UseFlag - 1];
	sinContinueSkillSet(&TempSkill);
	return TRUE;
}

// ����� - ĸ�������� ����ϱ�(ȣ���� ĸ�� ���)
int cINVENTORY::UseCapsuleItem(sITEM* pItem)
{
	// ����� : ĸ�������� �������� �־����� �������� ���� ������ Ȯ���Ѵ�.
	sITEM TempItem;
	TempItem.w = 22 * 2;
	TempItem.h = 22 * 2;

	if (!cCraftItem.CraftCheckEmptyArea(&TempItem))
	{
		cMessageBox.ShowMessage(MESSAGE_OVER_SPACE); //��������
		return FALSE;
	}

	if (pItem->CODE == (sinSP1 | sin34))
	{
		if (sinChar->Weight[0] + 5 >= sinChar->Weight[1])
		{
			cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT); //�����ʰ�
			return FALSE;
		}
	}

	bGiftBoxDelay = true; // ĸ���� ��� ���� �ٸ� �ൿ�� ���ϵ��� �����̸� �ش�. �������� �����̿� ����

	// ĸ�� ������ ��� ����� ������ ���ó�� �����ϰ� ����Ѵ�.
	if (sinActionPotion())
	{
		pUsePotion = &InvenItem[SelectInvenItemIndex - 1];
		cInvenTory.ReFormInvenItem();
		cInvenTory.CheckWeight();   //���Ը� �����Ѵ� 
		sinUsePotionDelayFlag = 1;
	}

	return TRUE;
}

int CristalEventItemIndex[7] = { 0, };

int PristonAlphabetEventItemIndex[7] = { 0, }; // ����� - ���ĺ� ���� �̺�Ʈ

//<ha>�ϰ�������  ���� ũ����Ż üũ
int cINVENTORY::CheckCristalEventItem()
{
	int i, cnt = 0;
	for (i = 0; i < 7; i++) {
		CristalEventItemIndex[i] = 0;
	}

	//ũ����Ż�� ã�Ƽ� �ε����� �ѱ��.
	for (i = 0; i < 12; i++) {
		if (cnt < 7) {
			if (CristalEventItemIndex[cnt] == 0) {
				CristalEventItemIndex[cnt] = SearchItemIndex(CristalEventCODE[i], 0);
			}
			if (CristalEventItemIndex[cnt] != 0)
				cnt++;
		}
	}
	//7���� ��Ҵ�.
	if (cnt == 7) {
		cInterFace.CheckAllBox(SIN_INVENTORY);
		cMessageBox.ShowMessage3(MESSAGE_CRISTALEVENTITEM_SEND, CristalItem);

	}
	else
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), CristalEventDoc);

	return TRUE;
}
//<ha>�ϰ������� ���� ũ����Ż�� ������ ������
int cINVENTORY::SendCristalEventItem()
{

	sITEM TempItem;
	TempItem.w = 22 * 3;
	TempItem.h = 22 * 4;

	memset(&sWingItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //�ʱ�ȭ

	//���Ը� üũ�Ѵ�.
	if (sinChar->Weight[0] + 100 > sinChar->Weight[1]) {
		cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
		return FALSE;
	}
	//���⼭ ������ ����üũ���غ���
	if (!cCraftItem.CraftCheckEmptyArea(&TempItem)) {
		cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
		return FALSE;
	}
	//üũ
	for (int i = 0; i < 7; i++) {
		if (CristalEventItemIndex[i] > 100) {
			sWingItem_Send.SheltomCode[i] = InvenItemTemp[CristalEventItemIndex[i] - 100 - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItemTemp[CristalEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItemTemp[CristalEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.dwChkSum;
			continue;
		}
		if (CristalEventItemIndex[i]) {
			sWingItem_Send.SheltomCode[i] = InvenItem[CristalEventItemIndex[i] - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItem[CristalEventItemIndex[i] - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItem[CristalEventItemIndex[i] - 1].sItemInfo.ItemHeader.dwChkSum;

		}
	}
	sWingItem_Send.DocIndex = 2; //auto����!!
	return TRUE;
}

//����� - ���� ��ƿ��� ���� ������ ������ üũ�Ѵ�.
int cINVENTORY::CheckWatermelonEventItem()
{
	int i, cnt = 0;
	for (i = 0; i < 7; i++)
	{
		WatermelonEventItemIndex[i] = 0;
	}

	cnt = CheckItemCount(WatermelonEventCODE);

	//������ 7���̻��� �������
	if (cnt >= 7)
	{
		cInterFace.CheckAllBox(SIN_INVENTORY);
		cMessageBox.ShowMessage3(MESSAGE_WATERMELON_EVENTITEM, "Deseja Trocar as suas Mel�ncias?"); // ������ //����� - ���ϴ��� �ҿ� �̺�Ʈ  
	//	cMessageBox.ShowMessage3(MESSAGE_WATERMELON_EVENTITEM,PumpkinItem); // ȣ��
	//	cMessageBox.ShowMessage3(MESSAGE_WATERMELON_EVENTITEM,WatermelonItem); // ����

	}
	else
		//cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), StarDoc);  // ������ //����� - ���ϴ��� �ҿ� �̺�Ʈ  
	//	cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0,15,128,125),PumpkinDoc);  // ȣ��
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), WatermelonDoc);  // ����

	return TRUE;
}


//�庰 - �߷�Ÿ�� ���ݷ� ��ƿ��� ���ݷ� ������ ������ üũ�Ѵ�.
int cINVENTORY::CheckValentineEventItem()
{
	int i, cnt = 0;
	for (i = 0; i < 7; i++)
	{
		ValentineEventItemIndex[i] = 0;
	}

	cnt = CheckValentineItemCount(ValentineEventCODE);

	//������ 7���̻��� �������
	if (cnt >= 7)
	{
		cInterFace.CheckAllBox(SIN_INVENTORY);
		cMessageBox.ShowMessage3(MESSAGE_VALENTINE_EVENTITEM, ValentineItem); // �庰 - �߷�Ÿ�� �̺�Ʈ  
	}
	else
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), ValentineDoc);  // �庰 - �߷�Ÿ�� �̺�Ʈ  


	return TRUE;
}


// �庰 - ĵ������ ĵ�� ��ƿ��� ĵ�� ������ ������ üũ�Ѵ�.
int cINVENTORY::CheckCandydaysEventItem()
{
	int i, cnt = 0;
	for (i = 0; i < 7; i++)
	{
		CandydaysEventItemIndex[i] = 0;
	}

	cnt = CheckCandyItemCount(CandydaysEventCODE);

	//������ 7���̻��� �������
	if (cnt >= 7)
	{
		cInterFace.CheckAllBox(SIN_INVENTORY);
		cMessageBox.ShowMessage3(MESSAGE_CANDYDAYS_EVENTITEM, CandydaysItem); // �庰 - ĵ������ 
	}
	else
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), CandydaysDoc);  // �庰 - ĵ������


	return TRUE;
}

// �庰 - �����ñ׸�  ���޶���, ���� ������ ������ üũ�Ѵ�.
int cINVENTORY::CheckMagicalGreenEventItem()
{
	int i, cnt = 0, cnt2 = 0;
	for (i = 0; i < 7; i++)
	{
		MagicalGreenEventItemIndex[i] = 0;
		MagicalGreenEmeraldEventItemIndex[i] = 0;
	}

	cnt = CheckMagicalGreenItemCount(MagicalGreenEventCODE[0]);
	cnt2 = CheckMagicalGreenItemEmeraldCount(MagicalGreenEventCODE[1]);

	//������ 7���̻��� �������
	if (cnt >= 7)
	{
		cInterFace.CheckAllBox(SIN_INVENTORY);
		cMessageBox.ShowMessage3(MESSAGE_MAGICALGREEN_EVENTITEM, MagicalGreenJadeItem);
	}

	else if (cnt2 >= 7)
	{
		cInterFace.CheckAllBox(SIN_INVENTORY);
		cMessageBox.ShowMessage3(MESSAGE_MAGICALGREEN_EVENTITEM_EMERALD, MagicalGreenEmeraldItem);
	}

	else
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), MagicalGreenDoc);


	return TRUE;
}

// �庰 - ī���� ���� ������ ������ üũ�Ѵ�.
int cINVENTORY::CheckTearOfKaraEventItem()
{
	int i, cnt = 0;
	for (i = 0; i < 7; i++)
	{
		TearOfKaraEventItemIndex[i] = 0;
	}

	cnt = CheckTearOfKaraItemCount(TeatOfKaraEventCODE);

	//������ 7���̻��� �������
	if (cnt >= 7)
	{
		cInterFace.CheckAllBox(SIN_INVENTORY);
		cMessageBox.ShowMessage3(MESSAGE_TEAROFKARA_EVENTITEM, TearOfKaraItem); // �庰 - ĵ������ 
	}
	else
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), TearOfKaraDoc);


	return TRUE;
}


// �庰 - ������� ã�ƶ� ������ ������ üũ�Ѵ�. - ���� ������
int cINVENTORY::CheckFindinvestigatorEventItem()
{
	int i, cnt = 0, cnt2 = 0;

	for (i = 0; i < 1; i++)
	{
		FindinvestigatorNineEventItemIndex[i] = 0;
		FindinvestigatorTaleEventItemIndex[i] = 0;
	}

	cnt = CheckFindinvestigatorNineItemCount(FindinvestigatorEventCODE[0]);
	cnt2 = CheckFindinvestigatorTaleItemCount(FindinvestigatorEventCODE[1]);

	// ���ξƹķ�, ���Ͼƹķ� �Ѵ� �������� �ʰ� ������
	if (cnt == 0 && cnt2 == 0)
	{
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), Findinvestigator1_1Doc);
		cMessageBox.ShowMessage3(MESSAGE_FIND_EVENTITEM, FindinvestigatorItem);

	}

	// ���ξƹķ� ����(���Ͼ����� ��� ����)
	else if (cnt == 1)
	{
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), Findinvestigator1_2Doc);
	}

	// ���Ͼƹķ��� ������
	else if (cnt == 0 && cnt2 == 1)
	{
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), Findinvestigator1_3Doc);
	}

	return TRUE;
}


// �庰 - ������� ã�ƶ� ������ ������ üũ�Ѵ�. - ī����
int cINVENTORY::CheckFindinvestigatorKamiuEventItem()
{
	int i, cnt = 0;

	for (i = 0; i < 1; i++)
	{
		FindinvestigatorNineEventItemIndex[i] = 0;
	}

	cnt = CheckFindinvestigatorNineItemCount(FindinvestigatorEventCODE[0]);

	// ���ξƹķ��� ����
	if (cnt == 1)
	{
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), Findinvestigator2_1Doc);
		cMessageBox.ShowMessage3(MESSAGE_FINDKAMIU_EVENTITEM, FindinvestigatorNineItem);
	}

	// ���ξƹķ� �������� ���� ��
	else if (cnt == 0)
	{
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), Findinvestigator2_2Doc);
	}



	return TRUE;
}


// �庰 - ������� ã�ƶ� ������ ������ üũ�Ѵ�. - ����
int cINVENTORY::CheckFindinvestigatorEtoEventItem()
{
	int i, cnt = 0;

	for (i = 0; i < 1; i++)
	{
		FindinvestigatorTaleEventItemIndex[i] = 0;
	}

	cnt = CheckFindinvestigatorTaleItemCount(FindinvestigatorEventCODE[1]);

	// ���Ͼƹķ��� ����
	if (cnt == 1)
	{
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), Findinvestigator3_1Doc);
		cMessageBox.ShowMessage3(MESSAGE_FINDETO_EVENTITEM, FindinvestigatorTaleItem);
	}

	// ���Ͼƹķ� �������� ���� ��
	else if (cnt == 0)
	{
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), Findinvestigator3_2Doc);
	}



	return TRUE;
}


//����� - ���� ��ƿ��� ���� �������� ������ ������
int cINVENTORY::SendWatermelonEventItem()
{
	sITEM TempItem;
	TempItem.w = 22 * 3;
	TempItem.h = 22 * 4;

	memset(&sWingItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //�ʱ�ȭ

	//���Ը� üũ�Ѵ�.
	if (sinChar->Weight[0] + 10 > sinChar->Weight[1]) { // ���� üũ (�����̺�Ʈ=130 / ȣ���̺�Ʈ=30 / ���ϴ��� �ҿ� �̺�Ʈ=10  ) 
		cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
		return FALSE;
	}
	//���⼭ ������ ����üũ���غ���
	if (!cCraftItem.CraftCheckEmptyArea(&TempItem)) {
		cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
		return FALSE;
	}
	//üũ
	for (int i = 0; i < 7; i++) {
		if (WatermelonEventItemIndex[i] > 100) {
			sWingItem_Send.SheltomCode[i] = InvenItemTemp[WatermelonEventItemIndex[i] - 100 - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItemTemp[WatermelonEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItemTemp[WatermelonEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.dwChkSum;
			continue;
		}
		if (WatermelonEventItemIndex[i]) {
			sWingItem_Send.SheltomCode[i] = InvenItem[WatermelonEventItemIndex[i] - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItem[WatermelonEventItemIndex[i] - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItem[WatermelonEventItemIndex[i] - 1].sItemInfo.ItemHeader.dwChkSum;

		}
	}
	sWingItem_Send.DocIndex = 2; //auto����!!
	return TRUE;
}




//�庰 - �߷�Ÿ�� �̺�Ʈ // �߷�Ÿ�� ���ݷ� �������� ������ ������
int cINVENTORY::SendValentineEventItem()
{
	sITEM TempItem;
	TempItem.w = 22 * 3;
	TempItem.h = 22 * 4;

	memset(&sWingItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //�ʱ�ȭ

	//���Ը� üũ�Ѵ�.
	if (sinChar->Weight[0] + 50 > sinChar->Weight[1]) { // ���� üũ (�����̺�Ʈ=130 / ȣ���̺�Ʈ=30 / ���ϴ��� �ҿ� �̺�Ʈ=10 / �߷�Ÿ���̺�Ʈ= 50) 
		cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
		return FALSE;
	}
	//���⼭ ������ ����üũ���غ���
	if (!cCraftItem.CraftCheckEmptyArea(&TempItem)) {
		cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
		return FALSE;
	}
	//üũ
	for (int i = 0; i < 7; i++) {
		if (ValentineEventItemIndex[i] > 100) {
			sWingItem_Send.SheltomCode[i] = InvenItemTemp[ValentineEventItemIndex[i] - 100 - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItemTemp[ValentineEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItemTemp[ValentineEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.dwChkSum;
			continue;
		}
		if (ValentineEventItemIndex[i]) {
			sWingItem_Send.SheltomCode[i] = InvenItem[ValentineEventItemIndex[i] - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItem[ValentineEventItemIndex[i] - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItem[ValentineEventItemIndex[i] - 1].sItemInfo.ItemHeader.dwChkSum;

		}
	}
	sWingItem_Send.DocIndex = 2; //auto����!!
	return TRUE;
}

// ����� - ���ĺ� ���� �̺�Ʈ
int cINVENTORY::CheckPristonAlphabetEventItem()
{
	int i, cnt = 0;
	for (i = 0; i < 7; i++) {
		PristonAlphabetEventItemIndex[i] = 0;
	}


	//ũ����Ż�� ã�Ƽ� �ε����� �ѱ��.
	for (i = 0; i < 7; i++) {
		if (cnt < 7) {
			if (PristonAlphabetEventItemIndex[cnt] == 0) {
				PristonAlphabetEventItemIndex[cnt] = SearchItemIndex(PristonAlphabetEventCODE[i], 0);
			}
			if (PristonAlphabetEventItemIndex[cnt] != 0)
				cnt++;
		}
	}


	//7���� ��Ҵ�.
	if (cnt == 7) {
		cInterFace.CheckAllBox(SIN_INVENTORY);
		cMessageBox.ShowMessage3(MESSAGE_PRISTON_ALPHABET_EVENTITEM, PristonAlphabetItem); 	// ����� - ���ĺ� ���� �̺�Ʈ

	}
	else
	{
		cSinHelp.sinShowHelp(SIN_HELP_KIND_BABEL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), PristonAlphabetDoc);
	}
	return TRUE;
}

// ����� - ���ĺ� ���� �̺�Ʈ(���ĺ� �������� ������ ������)
int cINVENTORY::SendPristonAlphabetEventItem()
{

	sITEM TempItem;
	TempItem.w = 22 * 3;
	TempItem.h = 22 * 4;

	memset(&sWingItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //�ʱ�ȭ

	//���Ը� üũ�Ѵ�.
	if (sinChar->Weight[0] + 20 > sinChar->Weight[1]) { // ����� - ���ĺ� ���� �̺�Ʈ(���Ը� 70���� ����) // �庰 20���� ����
		cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
		return FALSE;
	}
	//���⼭ ������ ����üũ���غ���
	if (!cCraftItem.CraftCheckEmptyArea(&TempItem)) {
		cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
		return FALSE;
	}
	//üũ
	for (int i = 0; i < 7; i++) {
		if (PristonAlphabetEventItemIndex[i] > 100) {
			sWingItem_Send.SheltomCode[i] = InvenItemTemp[PristonAlphabetEventItemIndex[i] - 100 - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItemTemp[PristonAlphabetEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItemTemp[PristonAlphabetEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.dwChkSum;
			continue;
		}
		if (PristonAlphabetEventItemIndex[i]) {
			sWingItem_Send.SheltomCode[i] = InvenItem[PristonAlphabetEventItemIndex[i] - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItem[PristonAlphabetEventItemIndex[i] - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItem[PristonAlphabetEventItemIndex[i] - 1].sItemInfo.ItemHeader.dwChkSum;

		}
	}
	sWingItem_Send.DocIndex = 2; //auto����!!
	return TRUE;
}


// �庰 - ĵ������ // ĵ�� �������� ������ ������
int cINVENTORY::SendCandydaysEventItem()
{
	sITEM TempItem;
	TempItem.w = 22 * 3;
	TempItem.h = 22 * 4;

	memset(&sWingItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //�ʱ�ȭ

	//���Ը� üũ�Ѵ�.
	if (sinChar->Weight[0] + 50 > sinChar->Weight[1]) { // ���� üũ (�����̺�Ʈ=130 / ȣ���̺�Ʈ=30 / ���ϴ��� �ҿ� �̺�Ʈ=10 / ĵ������ = 50 ) 
		cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
		return FALSE;
	}
	//���⼭ ������ ����üũ���غ���
	if (!cCraftItem.CraftCheckEmptyArea(&TempItem)) {
		cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
		return FALSE;
	}
	//üũ
	for (int i = 0; i < 7; i++) {
		if (CandydaysEventItemIndex[i] > 100) {
			sWingItem_Send.SheltomCode[i] = InvenItemTemp[CandydaysEventItemIndex[i] - 100 - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItemTemp[CandydaysEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItemTemp[CandydaysEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.dwChkSum;
			continue;
		}
		if (CandydaysEventItemIndex[i]) {
			sWingItem_Send.SheltomCode[i] = InvenItem[CandydaysEventItemIndex[i] - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItem[CandydaysEventItemIndex[i] - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItem[CandydaysEventItemIndex[i] - 1].sItemInfo.ItemHeader.dwChkSum;

		}
	}
	sWingItem_Send.DocIndex = 2; //auto����!!
	return TRUE;
}



// �庰 - �����ñ׸� - �������� ������ ������
int cINVENTORY::SendMagicalGreenEventItem()
{
	sITEM TempItem;
	TempItem.w = 22 * 3;
	TempItem.h = 22 * 4;

	memset(&sWingItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //�ʱ�ȭ

	//���Ը� üũ�Ѵ�.
	if (sinChar->Weight[0] + 70 > sinChar->Weight[1]) { // ���� üũ (ĵ������ = 50 / �����ñ׸� = 20 ) 
		cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
		return FALSE;
	}
	//���⼭ ������ ����üũ���غ���
	if (!cCraftItem.CraftCheckEmptyArea(&TempItem)) {
		cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
		return FALSE;
	}
	//üũ
	for (int i = 0; i < 7; i++) {
		if (MagicalGreenEventItemIndex[i] > 100) {
			sWingItem_Send.SheltomCode[i] = InvenItemTemp[MagicalGreenEventItemIndex[i] - 100 - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItemTemp[MagicalGreenEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItemTemp[MagicalGreenEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.dwChkSum;
			continue;
		}
		if (MagicalGreenEventItemIndex[i]) {
			sWingItem_Send.SheltomCode[i] = InvenItem[MagicalGreenEventItemIndex[i] - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItem[MagicalGreenEventItemIndex[i] - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItem[MagicalGreenEventItemIndex[i] - 1].sItemInfo.ItemHeader.dwChkSum;

		}
	}
	sWingItem_Send.DocIndex = 2; //auto����!!
	return TRUE;
}


// �庰 - �����ñ׸� - �������� ������ ������
int cINVENTORY::SendMagicalGreenEmeraldEventItem()
{
	sITEM TempItem;
	TempItem.w = 22 * 3;
	TempItem.h = 22 * 4;

	memset(&sWingItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //�ʱ�ȭ

	//���Ը� üũ�Ѵ�.
	if (sinChar->Weight[0] + 70 > sinChar->Weight[1]) { // ���� üũ (ĵ������ = 50 / �����ñ׸� = 20 ) 
		cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
		return FALSE;
	}
	//���⼭ ������ ����üũ���غ���
	if (!cCraftItem.CraftCheckEmptyArea(&TempItem)) {
		cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
		return FALSE;
	}
	//üũ
	for (int i = 0; i < 7; i++) {
		if (MagicalGreenEmeraldEventItemIndex[i] > 100) {
			sWingItem_Send.SheltomCode[i] = InvenItemTemp[MagicalGreenEmeraldEventItemIndex[i] - 100 - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItemTemp[MagicalGreenEmeraldEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItemTemp[MagicalGreenEmeraldEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.dwChkSum;
			continue;
		}
		if (MagicalGreenEmeraldEventItemIndex[i]) {
			sWingItem_Send.SheltomCode[i] = InvenItem[MagicalGreenEmeraldEventItemIndex[i] - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItem[MagicalGreenEmeraldEventItemIndex[i] - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItem[MagicalGreenEmeraldEventItemIndex[i] - 1].sItemInfo.ItemHeader.dwChkSum;

		}
	}
	sWingItem_Send.DocIndex = 2; //auto����!!
	return TRUE;
}


// �庰 - ī���� ���� // ���� �������� ������ ������
int cINVENTORY::SendTearOfKaraEventItem()
{
	sITEM TempItem;
	TempItem.w = 22 * 2;
	TempItem.h = 22 * 2;

	memset(&sWingItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //�ʱ�ȭ

	//���Ը� üũ�Ѵ�.
	if (sinChar->Weight[0] + 10 > sinChar->Weight[1]) { // ���� üũ ( ĵ������ = 50 / ī���� ���� = 10) 
		cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
		return FALSE;
	}
	//���⼭ ������ ����üũ���غ���
	if (!cCraftItem.CraftCheckEmptyArea(&TempItem)) {
		cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
		return FALSE;
	}
	//üũ
	for (int i = 0; i < 7; i++) {
		if (TearOfKaraEventItemIndex[i] > 100) {
			sWingItem_Send.SheltomCode[i] = InvenItemTemp[TearOfKaraEventItemIndex[i] - 100 - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItemTemp[TearOfKaraEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItemTemp[TearOfKaraEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.dwChkSum;
			continue;
		}
		if (TearOfKaraEventItemIndex[i]) {
			sWingItem_Send.SheltomCode[i] = InvenItem[TearOfKaraEventItemIndex[i] - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItem[TearOfKaraEventItemIndex[i] - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItem[TearOfKaraEventItemIndex[i] - 1].sItemInfo.ItemHeader.dwChkSum;

		}
	}
	sWingItem_Send.DocIndex = 2; //auto����!!
	return TRUE;
}

// ����� - 2010 ������ �̺�Ʈ - �౸�� ���� ����ϱ�
int cINVENTORY::UseSoccerBallPotionItem(sITEM* pItem)
{
	pItem->sItemInfo.PotionCount = 100;
	sinThrowItemToFeild(pItem);
	pItem->Flag = 0;

	cInvenTory.SetItemToChar(); //�������� ���õǸ� �ɷ�ġ�� �����Ѵ� 
	cInvenTory.ReFormInvenItem();
	cInvenTory.CheckWeight();   //���Ը� �����Ѵ� 
	sinUsePotionDelayFlag = 1;
	sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����

	return TRUE;
}



// �庰 - ������� ã�ƶ� - ���¿�����
int cINVENTORY::SendFindinvestigatorNineEventItem()
{
	sITEM TempItem;
	TempItem.w = 22 * 2;
	TempItem.h = 22 * 2;

	memset(&sWingItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //�ʱ�ȭ


	//���Ը� üũ�Ѵ�.
	if (sinChar->Weight[0] + 5 > sinChar->Weight[1]) { // ���� üũ ( ĵ������ = 50 / ī���� ���� = 10) 
		cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
		return FALSE;
	}
	//���⼭ ������ ����üũ���غ���
	if (!cCraftItem.CraftCheckEmptyArea(&TempItem)) {
		cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
		return FALSE;
	}
	//üũ
	for (int i = 0; i < 1; i++) {
		if (FindinvestigatorNineEventItemIndex[i] > 100) {
			sWingItem_Send.SheltomCode[i] = InvenItemTemp[FindinvestigatorNineEventItemIndex[i] - 100 - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItemTemp[FindinvestigatorNineEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItemTemp[FindinvestigatorNineEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.dwChkSum;
			continue;
		}
		if (FindinvestigatorNineEventItemIndex[i]) {
			sWingItem_Send.SheltomCode[i] = InvenItem[FindinvestigatorNineEventItemIndex[i] - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItem[FindinvestigatorNineEventItemIndex[i] - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItem[FindinvestigatorNineEventItemIndex[i] - 1].sItemInfo.ItemHeader.dwChkSum;

		}
	}
	sWingItem_Send.DocIndex = 2; //auto����!!
	return TRUE;
}


// �庰 - ������� ã�ƶ� - ī����
int cINVENTORY::SendFindinvestigatorTaleEventItem()
{
	sITEM TempItem;
	TempItem.w = 22 * 2;
	TempItem.h = 22 * 2;

	memset(&sWingItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //�ʱ�ȭ

	//���Ը� üũ�Ѵ�.
	if (sinChar->Weight[0] + 5 > sinChar->Weight[1]) { // ���� üũ ( ĵ������ = 50 / ī���� ���� = 10) 
		cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
		return FALSE;
	}
	//���⼭ ������ ����üũ���غ���
	if (!cCraftItem.CraftCheckEmptyArea(&TempItem)) {
		cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
		return FALSE;
	}
	//üũ
	for (int i = 0; i < 1; i++) {
		if (FindinvestigatorNineEventItemIndex[i] > 100) {
			sWingItem_Send.SheltomCode[i] = InvenItemTemp[FindinvestigatorNineEventItemIndex[i] - 100 - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItemTemp[FindinvestigatorNineEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItemTemp[FindinvestigatorNineEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.dwChkSum;
			continue;
		}
		if (FindinvestigatorNineEventItemIndex[i]) {
			sWingItem_Send.SheltomCode[i] = InvenItem[FindinvestigatorNineEventItemIndex[i] - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItem[FindinvestigatorNineEventItemIndex[i] - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItem[FindinvestigatorNineEventItemIndex[i] - 1].sItemInfo.ItemHeader.dwChkSum;

		}
	}
	sWingItem_Send.DocIndex = 2; //auto����!!
	return TRUE;
}


// �庰 - ������� ã�ƶ� - ����
int cINVENTORY::SendFindinvestigatorTaleEtoEventItem()
{
	sITEM TempItem;
	TempItem.w = 22 * 2;
	TempItem.h = 22 * 2;

	memset(&sWingItem_Send, 0, sizeof(sCRAFTITEM_SERVER)); //�ʱ�ȭ

	//���Ը� üũ�Ѵ�.
	if (sinChar->Weight[0] + 10 > sinChar->Weight[1]) { // ���� üũ ( ĵ������ = 50 / ī���� ���� = 10) 
		cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
		return FALSE;
	}
	//���⼭ ������ ����üũ���غ���
	if (!cCraftItem.CraftCheckEmptyArea(&TempItem)) {
		cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
		return FALSE;
	}
	//üũ
	for (int i = 0; i < 1; i++) {
		if (FindinvestigatorTaleEventItemIndex[i] > 100) {
			sWingItem_Send.SheltomCode[i] = InvenItemTemp[FindinvestigatorTaleEventItemIndex[i] - 100 - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItemTemp[FindinvestigatorTaleEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItemTemp[FindinvestigatorTaleEventItemIndex[i] - 100 - 1].sItemInfo.ItemHeader.dwChkSum;
			continue;
		}
		if (FindinvestigatorTaleEventItemIndex[i]) {
			sWingItem_Send.SheltomCode[i] = InvenItem[FindinvestigatorTaleEventItemIndex[i] - 1].sItemInfo.CODE;
			sWingItem_Send.Head[i] = InvenItem[FindinvestigatorTaleEventItemIndex[i] - 1].sItemInfo.ItemHeader.Head;
			sWingItem_Send.CheckSum[i] = InvenItem[FindinvestigatorTaleEventItemIndex[i] - 1].sItemInfo.ItemHeader.dwChkSum;

		}
	}
	sWingItem_Send.DocIndex = 2; //auto����!!
	return TRUE;
}