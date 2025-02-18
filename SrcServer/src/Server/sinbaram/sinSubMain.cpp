/*----------------------------------------------------------------------------*
*	파일명 :  sinSubMain.cpp	
*	하는일 :  각종 함수들을 관리한다 
*	작성일 :  최종업데이트 12월
*	적성자 :  박상열 
*-----------------------------------------------------------------------------*/	

#include "sinLinkHeader.h"
#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\Help.h"
#include "Character/playmain.h"
#include "Character/playsub.h"
#include "netplay.h"
#include "Character/character.h"
#include "fileread.h"


/*----------------------------------------------------------------------------*
*								전역변수 			
*-----------------------------------------------------------------------------*/	
HFONT sinFont = 0;   //폰트 
HFONT sinMessageFont = 0;   //메세지 폰트 
HFONT sinBoldFont = 0;   //메세지 폰트 


int sinTestFlag;     //테스트 플랙 
int sinSecretFlag = 0;  //암호 플랙 


int sinMouseButton[2] = { 0,0 };
int sinKeyClick[256];

int MouseButtonUpEvent = 0;   

int MatCursorDefault;       //아이템 팔기 커서 
int MatCursorSell;          //아이템 팔기 커서 
int MatCursorBuy;			//아이템 사기 커서 
int MatCursorRepair;	   	//고치기 커서 이미지 	
int MatCursorAttack;		//공격 
int MatCursorGetItem1;		//아이템 줍기 
int MatCursorGetItem2;		//아이템 줍기 2
int MatCursorTalk;			//말하기 

int CursorClass = 1;        //커서종류 (default로 1을 준다  아이템을 집을 때 CursorClass = 0으로준다 )

int sinMoveKindInter[MAX_SIN_KIND_INTER]={0,0,0,0,0,0,0,0,0,0,0,0,0}; //각종 폼이 움직이는 값 // 석지용 - 믹스쳐 리셋 추가로 값을 하나더 추가

int sinSec = 0;         //초를 구한다 
int sinMainCounter = 0; //메인루프를 돌때 카운트해준다 

float DeCreaSTM  = 0;
float InCreaSTM  = 0;
float InCreaLIFE = 0;
float InCreaMANA = 0;


char szTestBuff2[64]; //이미지 로드시 이미지가 없는 것을 확인해준다 
POINT TestImageSize = {0,0};

int Accuracy_Table[][2] = {
	   
#include "sinTable.h"
	-1
};


int ItemIndex1[3] = {0,0,0};	//전직을 위한 날치기 인덱스 저장 
int ItemIndex2[3] = {0,0,0};

int ChangeJobButonCheck2 = 0; //버튼다운을 한번 거너뛰는 플랙(이것두 역시 땡빵이다 씨바!!)
int ItemImageNotExitFlag = 0;
int DownNum = 0;


void InitSub()
{
	
	sinCreatFont(); //폰트생성 	
}
/*----------------------------------------------------------------------------*
*						서브메인 메인
*-----------------------------------------------------------------------------*/	
void MainSub()
{
	sinProc(SINKEYDOWN);		//키입력이 있을때 	
	sinSec++;
	sinMainCounter++; //메인의 카운터 

	if(sinSec >= 70){ // 1초마다 한번씩 실행 
		sinRegen();      //스테미나재생 (현재는 스테미나만 재생한다 )
		sinUseStamina(); //스테미나를 사용한다 
		sinSec = 0;

	}
	if((sinMainCounter&3) == 0) //70의 1/4일때 실행 
		sinSetRegen();

	if((sinMainCounter % 70) == 0)
		AgingRing();

	////////////////////////////////////미니맵때문에 이곳으로뺐다
	cSkill.CheckSkillMastery(); //마스터리를 체크한다 
	CheckContinueSkill(); //지속적으로 쓰는 스킬을 체크해서 시간이 지나면 없애준다 
	

}
/*----------------------------------------------------------------------------*
*						서브메인 그리기 
*-----------------------------------------------------------------------------*/	
void DrawSub()     
{

}
/*----------------------------------------------------------------------------*
*						서브메인 종료 
*-----------------------------------------------------------------------------*/	
void CloseSub()
{
    
}
/*----------------------------------------------------------------------------*
*							  커서 
*-----------------------------------------------------------------------------*/	
void InitCursor() {}
void DrawCursor() {}
/*----------------------------------------------------------------------------*
*				     스프라이트 이미지를 그린다 
*-----------------------------------------------------------------------------*/	
void DrawSprite(int winX, int winY, void* pdds, int x, int y, int width, int height, int Bltflag) {}
/*----------------------------------------------------------------------------*
*		아이템의 정보(ASE)를 구해와서 포인터(*szImagePath)에 셋팅한다 
*-----------------------------------------------------------------------------*/	
int  sinGetItemInfo(DWORD CODE, char* szImagePath) { return TRUE; }

/*----------------------------------------------------------------------------*
*					   서버에서 Data를 받는다 
*-----------------------------------------------------------------------------*/	
extern int DecodeCompress( BYTE *lpSrcData , BYTE *lpDestData ); //압축데이타 해독 ( Z/NZ 방식 )
int sinRecvMessage( DWORD dwCode , void *RecvBuff )
{
	sITEMINFO *lpItemInfo;
	smTRANS_SELL_ITEM		*lpTransSellItem;
	BYTE *lpBuff;
	int cnt;

	switch(dwCode) {
	case smTRANSCODE_SHOP_ITEMLIST:
		lpTransSellItem = (smTRANS_SELL_ITEM *)RecvBuff;

		lpItemInfo = new sITEMINFO[ lpTransSellItem->ItemCounter ];
		lpBuff = (BYTE *)RecvBuff+sizeof(smTRANS_SELL_ITEM);
		for( cnt=0;cnt<lpTransSellItem->ItemCounter;cnt++) {
			lpBuff += DecodeCompress( lpBuff , (BYTE *)&lpItemInfo[cnt] );
		}
		sinSetShopItem(lpItemInfo ,lpTransSellItem->ItemCounter, lpTransSellItem->ItemKind ); //무기만 셋팅 

		delete lpItemInfo;
		break;

	}

	return TRUE;
}
int RecvCountFlag = 0; //무기 방어구를 받을때는 초기화를 한번만 한다 

//상점에 대한 정보와 아이템을 받는다 
int sinSetShopItem(sITEMINFO *pItem,int ItemCount, int ItemKind) { return TRUE; }

/*----------------------------------------------------------------------------*
*			    마우스 or 인벤토리로 아이템을 셋팅한다 
*-----------------------------------------------------------------------------*/	
int sinSetQuestItem(sITEMINFO *sItemInfo )
{
	
	sinSetSpecialItemCode(sItemInfo); //스페셜아이템을 구분한다 

	int sArrowPosi=0 , i=0;
	sITEM sinTempItem;  //아이템에 관련된 사항을 잠시 넣어두는 Temp

	//if(cInvenTory.SearchItemCode((sinQT1|sin01)))return FALSE; //1개씩 밖에 가질수없다
	//if(cInvenTory.SearchItemCode((sinQT1|sin02)))return FALSE;
	//if(cInvenTory.SearchItemCode((sinQT1|sin03)))return FALSE;

	if(sinChar->ChangeJob == 0 && !sinQuest_ChangeJob.CODE && !sinQuest_ChangeJob2.CODE){
		if((sItemInfo->CODE == (sinQT1|sin01)) || (sItemInfo->CODE == (sinQT1|sin02)) || (sItemInfo->CODE == (sinQT1|sin03)) ||
		    (sItemInfo->CODE == (sinQT1|sin04)) || (sItemInfo->CODE == (sinQT1|sin05)) || (sItemInfo->CODE == (sinQT1|sin06))){
			return FALSE;
			}
	}

	//////////각 퀘스트에 맞지않는 아이템은 셋팅할수없다

	if(sinQuest_ChangeJob.CODE == SIN_QUEST_CODE_CHANGEJOB){
		if(cInvenTory.SearchItemCode(sItemInfo->CODE))return FALSE; //같은 코드의 아이템이 있으면 리턴한다
		if(sItemInfo->CODE == (sinQT1|sin04))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin05))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin06))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin16))return FALSE;

	}


	if(sinQuest_ChangeJob2.CODE == SIN_QUEST_CODE_CHANGEJOB2_NPC_M || 
		sinQuest_ChangeJob2.CODE == SIN_QUEST_CODE_CHANGEJOB2_NPC_D  ){
			if(sItemInfo->CODE == (sinQT1|sin01))return FALSE;
			if(sItemInfo->CODE == (sinQT1|sin02))return FALSE;
			if(sItemInfo->CODE == (sinQT1|sin03))return FALSE;
			if(sItemInfo->CODE == (sinQT1|sin06))return FALSE;

	}

	//퀘스트를 끝내고 전업아이템이 들어올경우 리턴한다
	if(sinChar->ChangeJob >= 1 ){
		if((sItemInfo->CODE == (sinQT1|sin01)) || (sItemInfo->CODE == (sinQT1|sin02)) || (sItemInfo->CODE == (sinQT1|sin03)) ||
		    (sItemInfo->CODE == (sinQT1|sin04)) || (sItemInfo->CODE == (sinQT1|sin05))){
			return FALSE;
			}
	}

	if(sinQuest_ChangeJob3.CODE == SIN_QUEST_CODE_CHANGEJOB3){
		if(sItemInfo->CODE == (sinQT1|sin01))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin02))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin03))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin04))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin05))return FALSE;

	}
	if(!sinQuest_Level80_2.CODE){
		if(sItemInfo->CODE == (sinQT1|sin09))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin10))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin11))return FALSE;
	}
	else{
		if(cInvenTory.SearchItemCode(sinQT1|sin09))
			if(sItemInfo->CODE == (sinQT1|sin09))return FALSE;
		if(cInvenTory.SearchItemCode(sinQT1|sin10))
			if(sItemInfo->CODE == (sinQT1|sin10))return FALSE;
		if(cInvenTory.SearchItemCode(sinQT1|sin11))
			if(sItemInfo->CODE == (sinQT1|sin11))return FALSE;

	}

	//if(sItemInfo->CODE

	//퀘스트 무기를 받은후에는 뱀프를 받을수없다
	if(sItemInfo->CODE == (sinQT1|sin06) && sinQuest_ChangeJob3.State >=3){
		return FALSE;
	}

	//퀘스트를 끝내고 전업아이템이 들어올경우 리턴한다
	if(sinChar->ChangeJob >= 2 && sItemInfo->CODE == (sinQT1|sin06)){
		return FALSE;
	}

	///////코드를 셋팅한 다
	switch(sItemInfo->CODE & sinITEM_MASK2){
		case sinQT1:
			sItemInfo->ItemKindCode = ITEM_KIND_QUEST;

		break;
		case sinSP1:
			sItemInfo->ItemKindCode = ITEM_KIND_EVENT;
		break;
	}


	if(LoadItemImage(sItemInfo,&sinTempItem)){
		if(sinTempItem.sItemInfo.PotionCount <= 0 )     //포션카운트를 보정한다
			sinTempItem.sItemInfo.PotionCount = 1;

		if(!cInvenTory.AutoSetInvenItem(&sinTempItem,1)){ //퀘스트아이템은 무게에 상관없이 셋팅할수있다
			if(ArrowState[0]==0)sArrowPosi = 2;
			else sArrowPosi = 1;
			sinButtonFlag = sArrowPosi;
			SelectInvenItemIndex = 0; //에테르 코어남발을 막기위해 초기화
			cInvenTory.ChangeABItem(sArrowPosi); // 2보다 작을때 인벤 박스 
			if(!cInvenTory.AutoSetInvenItem(&sinTempItem,1)){
				//if(sinThrowItemToFeild(&sinTempItem)){          //아이템을 버린다 
				sinTempItem.Flag = 0;
				return FALSE;
				
			}
		}
	}

	return TRUE;
}

/*----------------------------------------------------------------------------*
*			    마우스 or 인벤토리로 아이템을 셋팅한다 
*-----------------------------------------------------------------------------*/
//물약 복사 방지 플랙
int CheckPotionDelayFlag = 0;
int sinSetInvenItem(sITEMINFO *sItemInfo ) { return TRUE; }


//퀘스트 인벤토리에 셋팅될아이템
DWORD sinSpecialCODE[] = {(sinQT1|sin01),(sinQT1|sin02),(sinQT1|sin03),(sinQT1|sin04),(sinQT1|sin05),(sinQT1|sin06),
						(sinMA1|sin01),(sinMA2|sin01),(sinMA2|sin02),(sinQW1|sin01),(sinQW1|sin02),(sinQW1|sin03),
						(sinQW1|sin04),(sinQW1|sin05),(sinQT1|sin07),(sinQT1|sin08),
						(sinQT1|sin09),(sinQT1|sin10),(sinQT1|sin11),(sinQT1|sin12),(sinQT1|sin13),
						(sinQT1|sin14),(sinQT1|sin15),(sinQT1|sin16),(sinQW1|sin06),0};
//로스트 아일랜드 익스트림윙 성근추가 (sinQW1|sin06)
int sinSetSpecialItemCode(sITEMINFO *pItemInfo)
{
	int Count = 0;
	while(1){
		if(sinSpecialCODE[Count]){
			if(pItemInfo->CODE == sinSpecialCODE[Count]){
				pItemInfo->SpecialItemFlag[1] = ITEM_KIND_SPECIAL;
				break;

			}
			Count++;
		}
		else break;
	}
	cInvenTory.ChangeSpecialItem(2); //퀘스트아이템을 정리한다
	return TRUE;
}

 
//아이템의 이미지를 로드한다 
int LoadItemImage(sITEMINFO *sItemInfo , sITEM *TempItem) { return TRUE; }

/*----------------------------------------------------------------------------*
*				         키버튼  및 마우스 함수 
*-----------------------------------------------------------------------------*/	
void sinLButtonUp()
{
	MouseButtonUpEvent = SINLBUTTONUP;
}
void sinRButtonUp()
{
	MouseButtonUpEvent = SINRBUTTONUP;
}
void sinLButtonDown() 
{

}
/*----------------------------------------------------------------------------*
*					키입력과 마우스 입력을 한번으로 제한한다 
*-----------------------------------------------------------------------------*/	
int sinGetMouseButton(int num) { return TRUE; }
//-------------------------------------------------------------------------------
void sinMouseMain()
{
}
/*----------------------------------------------------------------------------*
*					키보드 입력을 한번만 체크하게한다
*-----------------------------------------------------------------------------*/	
int sinGetKeyClick(int num) { return TRUE; }
//-----------------------------------------------------------------------------
void sinKeyClickMain() {}


class sinFlag{
public:
	int	OpenFlag;
};

int FirstMove[MAX_SIN_KIND_INTER] = {0,0,0,0,0,0,0,0,0,0,0,0,0}; // 석지용 - 믹스쳐 리셋 추가로 값을 하나더 추가

void ShowInterFace() {}

//폰트 생성 
int sinCreatFont() { return TRUE; }

//텍스트가 놓을 위치를 보정한다
int CheckEditSize(int x , int lx , char *String)
{
	int len = 0;  
	int ResultX = 0;
	int Temp=0,Temp2=0,Temp3=0;

	len = lstrlen(String);

	Temp = lx - x;
	Temp2 = (Temp - len*6)/2;

	return Temp2+x;

}

/*----------------------------------------------------------------------------*
*					모션이 끝난다음에 포션을 사용한다 
*-----------------------------------------------------------------------------*/	
#define SIN_POTION_LIFE			1
#define SIN_POTION_MANA			2
#define SIN_POTION_STAMINA		3
#define SIN_POTION_SONGPYEUN	4

int sinUsePotion() //모션이 끝난다음에 포션을 사용한다 
{
	
	CheckCharForm();//인증 
	
	int PotionKind = 0;
	int ResultPotion = 0;
	int TempRandPotion = 0;

	if(pUsePotion == 0) return FALSE;
	if(!pUsePotion->Flag)return FALSE;
	if(MouseItem.Flag){//마우스에 집고있는 아이템과 사용될 아이템이 같으면 리턴한다 (왜 그런거쥐 -_-; 험험 기억이안난다 먼가이유가있을테니 냅둔다)
		if(MouseItem.sItemInfo.ItemHeader.Head == pUsePotion->sItemInfo.ItemHeader.Head)
			return FALSE;

	}

	if((pUsePotion->CODE & sinITEM_MASK2) ==sinPL1 ){ //라이프일경우 
		TempRandPotion = pUsePotion->sItemInfo.Life[1] -  pUsePotion->sItemInfo.Life[0];
		ResultPotion = pUsePotion->sItemInfo.Life[0]+(rand()%TempRandPotion);
		sinSetLife((sinGetLife()+ResultPotion));	//라이프 셋팅 
		PotionKind = SIN_POTION_LIFE;
	}
	if((pUsePotion->CODE & sinITEM_MASK2) == sinPM1){ //마나일경우 
		TempRandPotion = pUsePotion->sItemInfo.Mana[1] -  pUsePotion->sItemInfo.Mana[0];
		ResultPotion = pUsePotion->sItemInfo.Mana[0]+(rand()%TempRandPotion);
		sinSetMana((sinGetMana()+ResultPotion));	//라이프 셋팅 
		PotionKind = SIN_POTION_MANA;
	}
	if((pUsePotion->CODE & sinITEM_MASK2) == sinPS1 ){ //스테미나 포션일경우 
		TempRandPotion = pUsePotion->sItemInfo.Stamina[1] -  pUsePotion->sItemInfo.Stamina[0];
		ResultPotion = pUsePotion->sItemInfo.Stamina[0]+(rand()%TempRandPotion);
		sinSetStamina((sinGetStamina()+ResultPotion));	//라이프 셋팅 
		PotionKind = SIN_POTION_STAMINA;
	}
	if((pUsePotion->CODE & sinITEM_MASK2) == sinSP1)
	{
		if( (pUsePotion->CODE & sinITEM_MASK3) == sin01 || (pUsePotion->CODE & sinITEM_MASK3) == sin02 ||
			(pUsePotion->CODE & sinITEM_MASK3) == sin03 || (pUsePotion->CODE & sinITEM_MASK3) == sin15 || // 박재원 - 수박 아이템 추가
			(pUsePotion->CODE & sinITEM_MASK3) == sin35 ||  // 장별 - 발렌타인 초콜릿 아이템 추가
			 (pUsePotion->CODE & sinITEM_MASK3) == sin36 || (pUsePotion->CODE & sinITEM_MASK3) == sin42 ) // 장별 - 캔디데이즈 // 장별 - 수박
		{
			pUsePotion->sItemInfo.PotionCount = 0;			//송편 포션 카운트 0
			if(cInvenTory.EatSongPyeun(pUsePotion)){
				PotionKind = SIN_POTION_SONGPYEUN;
			}
			else
				PotionKind = 5;
		}
		// 선물상자
		if( (pUsePotion->CODE & sinITEM_MASK3) == sin05 || (pUsePotion->CODE & sinITEM_MASK3) == sin06 || (pUsePotion->CODE & sinITEM_MASK3) == sin07 ||
			(pUsePotion->CODE & sinITEM_MASK3) == sin08 || (pUsePotion->CODE & sinITEM_MASK3) == sin09 || (pUsePotion->CODE & sinITEM_MASK3) == sin10 )
		{
			PotionKind = 6;		// pluto 선물상자
		}

		// 박재원 - 캡슐 아이템(호랑이 캡슐 사용)
		if( (pUsePotion->CODE & sinITEM_MASK3) == sin34 )
		{
			PotionKind = 7;		// 박재원 - 호랑이 캡슐 아이템을 사용할때 야호 모션을 한다. 
		}

	}
	if(PotionKind){
		if(pUsePotion->sItemInfo.PotionCount >=2){ //포션의 수를 줄인다 
			pUsePotion->sItemInfo.PotionCount--;
			/*
			if(( pUsePotion->sItemInfo.CODE & sinITEM_MASK3 ) == sin03 ||
				( pUsePotion->sItemInfo.CODE & sinITEM_MASK3 ) == sin04 ){
				sinPlaySound(SIN_SOUND_EAT_POTION2); //물약 먹는 소리 
			}
			else
			*/

		}
		else{
			pUsePotion->Flag =0;
			sInven[pUsePotion->ItemPosition-1].ItemIndex = 0; //사용되지 않는다 
			if(PotionKind != 5){
			/*			
				if(( pUsePotion->sItemInfo.CODE & sinITEM_MASK3 ) == sin03 ||
					( pUsePotion->sItemInfo.CODE & sinITEM_MASK3 ) == sin04 ){
					sinPlaySound(SIN_SOUND_EAT_POTION2); //물약 먹는 소리 
				}
				else
			*/

			}

		} 
	
	pUsePotion = 0; //포인터 초기화 		
	ReformCharForm();//재인증 
	cInvenTory.CheckWeight(); //무게를 체크한다 
	cInvenTory.ReFormPotionNum();	//포션 갯수를 체크한다 
	cInvenTory.ReFormInvenItem();   //인벤토리 아이템 인증
	return PotionKind;
	}

	return FALSE;	

}

//신바람 재생 
void sinRegen()
{

	InCreaSTM  = 0;
	InCreaMANA = 0;
	InCreaLIFE = 0;

	float	fLevel=0;
	float	fStrength=0;
	float	fHealth=0;
	float	fSpirit=0;

	//스테미나 재생 
	if(lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_STAND){
		switch(sinChar->StaminaFunction){
			case 1:
				InCreaSTM = (float)(3.8 +sinChar->Level/7+sinChar->Stamina_Regen);
			break;
			case 2:
				InCreaSTM = (float)(3.3 +sinChar->Level/7+sinChar->Stamina_Regen);
			break;
			case 3:
				InCreaSTM = (float)(2.9 +sinChar->Level/7+sinChar->Stamina_Regen);
			break;
			default: //혹시나 해서 하나 넣어둔다 
				InCreaSTM = (float)(3.8 +sinChar->Level/7+sinChar->Stamina_Regen);
			break;
		}

	}
	if(lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_WALK){
		switch(sinChar->StaminaFunction){
			case 1:
				InCreaSTM = (float)((3.8 +sinChar->Level/7+sinChar->Stamina_Regen)*0.6);
			break;
			case 2:
				InCreaSTM = (float)((3.3 +sinChar->Level/7+sinChar->Stamina_Regen)*0.6);
			break;
			case 3:
				InCreaSTM = (float)((2.9 +sinChar->Level/7+sinChar->Stamina_Regen)*0.6);
			break;
			default:
				InCreaSTM = (float)((3.8 +sinChar->Level/7+sinChar->Stamina_Regen)*0.6);
			break;

		}

	}
	fLevel = (float)sinChar->Level;
	fStrength = (float)sinChar->Strength;
	fHealth = (float)sinChar->Health;
	fSpirit = (float)sinChar->Spirit;

	
	if(lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD){
		//라이프 재생  
		InCreaLIFE = (float)(((fLevel+(fStrength/2)+fHealth)/180 + sinChar->Life_Regen)/1.5);
		//마나 재생 
		InCreaMANA = (fLevel+(fSpirit*1.2f)+(fHealth/2))/115 + sinChar->Mana_Regen;
	}

}

void sinUseStamina()
{

	DeCreaSTM = 0;
	if(lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_RUN){
		//DeCreaSTM = (float)(100+(sinChar->Weight[0]/2))/(100+(sinChar->Strength/5)); 
		//DeCreaSTM = (float)1000/(((sinChar->Weight[1] - sinChar->Weight[0])+(sinChar->Strength/2)+500)+0.5);
		DeCreaSTM = (float)((1000+sinChar->Weight[0]) / (sinChar->Weight[1]+(sinChar->Strength/2)+500)+0.4);
	
	}
}


float fTempLIFE = 0;
float fTempMANA = 0;
float fTempSTM_Incre  = 0;
float fTempSTM_Decre  = 0;

//스테미나를 셋팅한다 
void sinSetRegen()
{
	
	CheckCharForm();//인증 

	//스테미나 감소 
	if(DeCreaSTM){ 
		fTempSTM_Decre += DeCreaSTM/(70/4);
		if(fTempSTM_Decre >=1){
			sinSetStamina(sinGetStamina()-(short)fTempSTM_Decre);
			fTempSTM_Decre -=1;
		}

	}
	//스테미나 증가 
	if(InCreaSTM){
		fTempSTM_Incre += InCreaSTM/(70/4);
		if(fTempSTM_Incre >= 1){
			sinSetStamina(sinGetStamina()+(short)fTempSTM_Incre);
			while(1){
				if(fTempSTM_Incre > 1)
					fTempSTM_Incre -=1;
				else break;

			}

		}

	}

	//라이프 증가 
	if(InCreaLIFE){
		fTempLIFE += InCreaLIFE/(70/4);
		if(fTempLIFE >=1){
			sinSetLife(sinGetLife()+(short)fTempLIFE);
			while(1){
				if(fTempLIFE > 1)
					fTempLIFE -=1;
				else break;
			}

		}
		

	}
	
	//마나 증가 
	if(InCreaMANA){
		fTempMANA += InCreaMANA/(70/4);
		if(fTempMANA >= 1){
			sinSetMana(sinGetMana()+(short)fTempMANA);
			while(1){
				if(fTempMANA > 1)
					fTempMANA -=1;
				else break;
			}


		}

	}
	ReformCharForm();//재인증 

}
/*----------------------------------------------------------------------------*
*				 (각종 공식을 적용후 값을 리턴한다 )
*-----------------------------------------------------------------------------*/	
int sinGetAC(int AC)
{
	int cnt = 0;
	while(1){
		if(Accuracy_Table[cnt][0] == 0)break;
		if( AC > Accuracy_Table[cnt][0] && AC < Accuracy_Table[cnt+1][0] ){
			return Accuracy_Table[cnt+1][1];	
		}
		cnt++;
	}
	return FALSE;
}

int cnt  = 0;
int AC_R = 0;    //계산으로 나온 값 
int RealAC=0;    //테이블에서 나온 실제 수치 
int Result=0;
float fDesLV = 0;
float fMyLV  = 0;
float fWeifht[2] = {0,0};


//명중률을 리턴한다 (정확하다) (무게의 요인이있으니 의심하지마세요)
int sinGetAccuracy(int desLV , int desDefense)
{
	cnt = 0;
	AC_R = (int)(((float)sinChar->Attack_Rating - (float)desDefense)*1.4);

	while(1){
		if(AC_R <-190){
			RealAC = 50;
			break;
		}
		if(AC_R >2100){
			RealAC = 95;
			break;
		}
		if(Accuracy_Table[cnt][1] == 0)break; 
		if( AC_R >= Accuracy_Table[cnt][0] && AC_R <= Accuracy_Table[cnt+1][0] ){
			RealAC = Accuracy_Table[cnt+1][1];	
			break;

		}
		cnt++;
	}
	fDesLV = (float)desLV;
	fMyLV  = (float)sinChar->Level;
	fWeifht[0] = sinChar->Weight[0];
	fWeifht[1] = sinChar->Weight[1];


	//Result = (int)(RealAC - (((fDesLV - fMyLV)/100)*28)-(( fWeifht[0] / fWeifht[1])*3));
	Result = (int)(RealAC - ((fDesLV - fMyLV)/100)*28);

	if(Result < 30) Result = 30;
	if(Result > 95) Result = 95;
	return Result;
}

//몬스터 명중률  
int sinGetMonsterAccuracy(int MonsterLV , int MonsterAttack_Rating) 
{
	cnt = 0;
	AC_R = (MonsterAttack_Rating - sinChar->Defence)*2;

	while(1){
		if(Accuracy_Table[cnt][0] == 0)break;
		if( AC_R > Accuracy_Table[cnt][0] && AC_R < Accuracy_Table[cnt+1][0] ){
			RealAC = Accuracy_Table[cnt+1][1];	
			break;

		}
		cnt++;
	}
	fDesLV = (float)sinChar->Level;
	fMyLV  = (float)MonsterLV;

	Result = (int)(RealAC - ((fDesLV - fMyLV)/100)*50);

	if(Result < 30) Result = 30;
	if(Result > 95) Result = 95;

	return Result;
}

//몬스터 VS 몬스터 명중률
int sinGetPVPAccuracy(int MyLevel , int MyAttack_Rating , int DesLevel , int DesDefense)
{
	cnt = 0;
	AC_R = (int)(((float)MyAttack_Rating - (float)DesDefense)*1.4);

	while(1){
		if(AC_R <-190){
			RealAC = 50;
			break;
		}
		if(AC_R >2100){
			RealAC = 95;
			break;
		}
		if(Accuracy_Table[cnt][1] == 0)break; 
		if( AC_R > Accuracy_Table[cnt][0] && AC_R < Accuracy_Table[cnt+1][0] ){
			RealAC = Accuracy_Table[cnt+1][1];	
			break;

		}
		cnt++;
	}
	fDesLV = (float)DesLevel;
	fMyLV  = (float)MyLevel;

	Result = (int)(RealAC - ((fDesLV - fMyLV)/100)*28);

	if(Result < 30) Result = 30;
	if(Result > 95) Result = 95;
	return Result;
}

//실제 크리티컬 
int sinGetCritical(int desLV)
{

	fDesLV = (float)desLV;
	fMyLV  = (float)sinChar->Level;

	Result = (int)(sinChar->Critical_Hit + ((fMyLV - fDesLV)/100)*25);
	if(Result > 70)
		Result = 70;
	
	return Result;

}

int sinGetBlockRating(int desLV)
{

	fDesLV = (float)desLV;
	fMyLV  = (float)sinChar->Level;

	Result = (int)(sinChar->Chance_Block+((fMyLV - fDesLV)/100)*25);
	return Result;
	
}

#define SIN_MONSTER_UNDEAD		1
//흡수율을 리턴한다 
int sinGetAbsorb(int Type)
{

	switch(Type){
		case SIN_MONSTER_UNDEAD:
			return sinChar->Absorption+sinUndeadAbsorb;
		break;
		default:
			return sinChar->Absorption;

		break;
	}
	return TRUE;
}


void sinReload() {}

int StopCharMotion(int x ,int y) { return TRUE; }
//테스트시 사용될 메세지를 보여준다 
int CheatHelpOpenFlag = 0;
void sinTestDrawText() {}
/*
//Tempscron 4개 직업별 얼굴...
static char *szTempFighterFaceName[] = {
	"char\\tmABCD\\tmh-b01.inf",
	"char\\tmABCD\\tmh-b02.inf",
	"char\\tmABCD\\tmh-b03.inf"
};

static char *szTempMechFaceName[] = {
	"char\\tmABCD\\tmh-a01.inf",
	"char\\tmABCD\\tmh-a02.inf",
	"char\\tmABCD\\tmh-a03.inf"
};

static char *szTempArcherFaceName[] = {
	"char\\tmABCD\\tfh-D01.inf",
	"char\\tmABCD\\tfh-d02.inf",
	"char\\tmABCD\\tfh-d03.inf"
};
static char *szTempPikeFaceName[] = {
	"char\\tmABCD\\tmh-c01.inf",
	"char\\tmABCD\\tmh-c02.inf",
	"char\\tmABCD\\tmh-c03.inf"
};
*/

/////////////////////////////////TEST KEY
void sinTestKey() {}


//트레이드 요구가 들어오면 값을 셋팅한다 
int sinRequestTrade(DWORD CODE , char *CharName)
{
	if(cTrade.OpenFlag)
		return FALSE;

	cTrade.TradeRequestFlag = 1;
	cTrade.TradeCharCode = CODE;
	lstrcpy(cTrade.szTradeCharName,CharName);
	return TRUE;
	
}

DWORD dwCurrentTime = 0;

//스킬을 배우고 전업을 한다 
int sinSkillMaster(int ChangeJob) { return TRUE; }

//스킬 배우기를 마친다 
int sinSkillMasterClose()
{
	/*
	if(ChangeJobButtonclick){
		cInterFace.CheckAllBox(SIN_SKILL); //스킬창을 띄워준다 

	}
	else{
	*/
		SkillMasterFlag = 0;	//스킬을 할당할수있는 값을 초기화한다 
		cSkill.OpenFlag = 0;	//스킬 창을 닫아준다 
dwCurrentTime = dwPlayTime+5000;
	//}
	return TRUE;
}


static int LastPartyMode = -1;
int haSiegeBoardTempFlag = 0; //공성전 보드창을 숨기기 위한  
//파티 창을 보여준다 
int ShowParty() { return TRUE; }

//퀘스트 창을 보여준다 
int ShowQuest() { return TRUE; }

///////////////////////////////// 라이프 , 마나 , 스테미나 관련 코드화 

//라이프를 읽어온다 
short sinGetLife()
{

		return (sinChar->Life[0])^CodeXorLife;
}

//마나를 얻어온다 
short sinGetMana()
{

	return sinChar->Mana[0];
}

//스테미나를 얻어온다 
short sinGetStamina()
{

	return sinChar->Stamina[0];
}

//라이프를 셋팅한다				//kyle xtrapHeap
void sinSetLife(short Life)
{
#ifdef _XTRAP_GUARD_4_CLIENT //HEAP MEMORY TEST
	XTrap_CE1_Func11_Protect( &(sinChar->Life), sizeof(sinChar->Life) );	//보호영역 무결성 체크
#endif

	//CheckCharForm(); //인증 
	sinChar->Life[0] = Life;
	if(sinChar->Life[0] < 0 ) sinChar->Life[0] = 0;
	if(sinChar->Life[0] >= sinChar->Life[1] )sinChar->Life[0] = sinChar->Life[1];
	sinChar->Life[0] ^= CodeXorLife;
	ReformCharForm(); //재인증 

#ifdef _XTRAP_GUARD_4_CLIENT //HEAP MEMORY TEST
	XTrap_CE1_Func12_Protect( &(sinChar->Life), sizeof(sinChar->Life) );	//보호영역 보호
	XTrap_CE1_Func13_Free( &(sinChar->Life), sizeof(sinChar->Life) );		//보호영역 해제
#endif
	
}

//마나를 셋팅한다 
void sinSetMana(short Mana)
{
#ifdef _XTRAP_GUARD_4_CLIENT //HEAP MEMORY TEST
	XTrap_CE1_Func11_Protect( &(sinChar->Mana), sizeof(sinChar->Mana) );	//보호영역 무결성 체크
#endif

	//CheckCharForm(); //인증 
	sinChar->Mana[0] = Mana;
	if(sinChar->Mana[0] < 0 ) sinChar->Mana[0] = 0;
	if(sinChar->Mana[0] >= sinChar->Mana[1] )sinChar->Mana[0] = sinChar->Mana[1];
	ReformCharForm(); //재인증 

#ifdef _XTRAP_GUARD_4_CLIENT //HEAP MEMORY TEST
	XTrap_CE1_Func12_Protect( &(sinChar->Mana), sizeof(sinChar->Mana) );	//보호영역 보호
	XTrap_CE1_Func13_Free( &(sinChar->Mana), sizeof(sinChar->Mana) );		//보호영역 해제
#endif
}

//스테미나를 셋팅한다 
void sinSetStamina(short Stamina)
{
#ifdef _XTRAP_GUARD_4_CLIENT //HEAP MEMORY TEST
	XTrap_CE1_Func11_Protect( &(sinChar->Stamina), sizeof(sinChar->Stamina) );	//보호영역 무결성 체크
#endif

	//CheckCharForm(); //인증 
	sinChar->Stamina[0] = Stamina;
	if(sinChar->Stamina[0] < 0 ) sinChar->Stamina[0] = 0;
	if(sinChar->Stamina[0] >= sinChar->Stamina[1] )sinChar->Stamina[0] = sinChar->Stamina[1];
	ReformCharForm(); //재인증 

#ifdef _XTRAP_GUARD_4_CLIENT //HEAP MEMORY TEST
	XTrap_CE1_Func12_Protect( &(sinChar->Stamina), sizeof(sinChar->Stamina) );	//보호영역 보호
	XTrap_CE1_Func13_Free( &(sinChar->Stamina), sizeof(sinChar->Stamina) );		//보호영역 해제
#endif

}

//	short   ItemAgingNum[2];	//0은 에이징+몇 1은 에이징이 되고있나 아닌가?
//	short   ItemAgingCount[2];	//0아이템 숙련 카운트 1숙련치 최대 

//////에이징 레벨을 체크한후 올려준다(에이징수치)
/*
char *AgingMsgDamageMax = "최대공격력";
char *AgingMsgDamageMin = "최소공격력";
char *AgingMsgAttack_Rate = "명중력";
char *AgingMsgDur = "내구력";
char *AgingMsgCritical = "크리티컬";
char *AgingMsgShooting_Range = "사정거리";
char *AgingMsgMana = "기력";
char *AgingMsgBlock = "블럭율";
char *AgingMsgAbsorb = "흡수율";
char *AgingMsgDefense = "방어율";
*/
//내구력
int sinSetDurabilityAging(sITEMINFO *pItem)
{
	int Temp = 0;
	Temp = pItem->Durability[1]/100;
	Temp += 1;
	pItem->Durability[1] -= Temp;
	if(pItem->Durability[1] < 0 ) pItem->Durability[1] = 0;
	if(pItem->Durability[1] < pItem->Durability[0] ) pItem->Durability[0] = pItem->Durability[1]; //현제 내구력 보정 

	return TRUE;
}
//공격력
int sinSetDamageAging(sITEMINFO *pItem)
{
	pItem->Damage[0]++;
	pItem->Damage[1]++;

	if(pItem->ItemAgingNum[0] >= 9){ //10차부터는 데미지 +2
		pItem->Damage[0]++;
		pItem->Damage[1]++;
	}

	// 박재원 - 에이징 설정 변경(무기류 레벨별 공격력 추가)
	if(pItem->ItemAgingNum[0] >= 19){ //20차부터는 데미지 +3
		pItem->Damage[0]++;
		pItem->Damage[1]++;
	}

	return TRUE;
}
//방어력
int sinSetDefenseAging(sITEMINFO *pItem , int Percent)
{
	float Temp = 0;
	Temp = (float)pItem->Defence/(100.0f / (float)Percent);
	pItem->Defence += (int)Temp;
	return TRUE;
}

//명중력
int sinSetAttackRateAging(sITEMINFO *pItem , int AddNum)
{
	pItem->Attack_Rating += AddNum;
	return TRUE;
}
//흡수율
int sinSetAbsorbAging(sITEMINFO *pItem , float Percent)
{
	pItem->fAbsorb += Percent;

	if(pItem->ItemAgingNum[0] >= 9){ //10차부터는 2배
		pItem->fAbsorb += Percent;
	}

	// 박재원 - 에이징 설정 변경(무기류 레벨별 공격력 추가)
	if(pItem->ItemAgingNum[0] >= 19){ //20차부터는 3배
		pItem->fAbsorb += Percent;
	}
	return TRUE;
}

//크리티컬
int sinSetCriticalAging(sITEMINFO *pItem)
{
	if(pItem->ItemAgingNum[0] && (pItem->ItemAgingNum[0]%2) == 1){
		pItem->Critical_Hit += 1;

	}
	return TRUE;
}

//블럭율
int sinSetBlockRateAging(sITEMINFO *pItem)
{
	if(pItem->ItemAgingNum[0] && (pItem->ItemAgingNum[0]%2) == 1){
		pItem->fBlock_Rating += 1;

	}
	return TRUE;
}

int sinSetAgingItemIncreState(sITEMINFO *pItem)
{
	float TempDefense[2] = {0,0};
	//에이징 초기화
	switch(pItem->CODE & sinITEM_MASK2){
		case sinWA1: //도끼 
			//공격력 
			sinSetDamageAging(pItem);
			//명중력
			sinSetAttackRateAging(pItem , 10);
			//내구력
			sinSetDurabilityAging(pItem); 

		break;
		case sinWC1: //클러
			//공격력 
			sinSetDamageAging(pItem);
			//명중력
			sinSetAttackRateAging(pItem , 5);
			//크리티컬  
			sinSetCriticalAging(pItem);

		break;

		case sinWH1: //해머 
			//공격력 
			sinSetDamageAging(pItem);

			//명중력
			sinSetAttackRateAging(pItem , 10);
			//내구력
			sinSetDurabilityAging(pItem); 

		break;
		case sinWM1: //메지컬 스테프 
			//공격력 
			sinSetDamageAging(pItem);

			//명중력
			sinSetAttackRateAging(pItem , 10);

			//기력  
			pItem->fIncrease_Mana += 10;

		break;
		case sinWP1: //창 
			//공격력 
			sinSetDamageAging(pItem);

			//명중력
			sinSetAttackRateAging(pItem , 5);

			//크리티컬  
			sinSetCriticalAging(pItem);

		break;
		case sinWS1: //활 
			//공격력 
			sinSetDamageAging(pItem);

			//크리티컬  
			sinSetCriticalAging(pItem);

			//내구력
			sinSetDurabilityAging(pItem); 
		break;

		case sinWS2: //칼  
			//공격력 
			sinSetDamageAging(pItem);

			//명중력
			sinSetAttackRateAging(pItem , 5);
			
			//크리티컬  
			sinSetCriticalAging(pItem);

		break;

		case sinWT1: //자벨린 
			//공격력 
			sinSetDamageAging(pItem);

			//크리티컬  
			sinSetCriticalAging(pItem);

			//내구력
			//sinSetDurabilityAging(pItem); 
		break;
			
		case sinDS1: //방패 
			//블럭율 
			sinSetBlockRateAging(pItem);
			//흡수율
			sinSetAbsorbAging(pItem , 0.2f);
			//내구력
			sinSetDurabilityAging(pItem); 
		break;

		case sinOM1: //매지컬 감자 
			//방어력 
			sinSetDefenseAging(pItem,10);

			//흡수율
			sinSetAbsorbAging(pItem , 0.5f);

			//내구력
			sinSetDurabilityAging(pItem); 

		break;
		case sinDA1: case sinDA2://갑옷 
			//방어력 
			sinSetDefenseAging(pItem,5);

			//흡수율
			sinSetAbsorbAging(pItem , 0.5f);

			//내구력
			sinSetDurabilityAging(pItem); 

		break; 
		case sinDB1: // 박재원 - 에이징 아이템 추가(부츠)
			//방어력 
			sinSetDefenseAging(pItem,10);
			//흡수율
			sinSetAbsorbAging(pItem , 0.5f);
			//내구력
			sinSetDurabilityAging(pItem); 
			break;
		case sinDG1: // 박재원 - 에이징 아이템 추가(장갑)
			//방어력 
			sinSetDefenseAging(pItem,10);
			//흡수율
			sinSetAbsorbAging(pItem , 0.5f);
			//내구력
			sinSetDurabilityAging(pItem); 
			break;
		case sinOA2: // 박재원 - 에이징 아이템 추가(암릿)
			//방어력 
			sinSetDefenseAging(pItem,10);
			//명중력
			sinSetAttackRateAging(pItem , 5);
			//내구력
			sinSetDurabilityAging(pItem); 
			break;
		}
		
		

		pItem->ItemAgingNum[0]++;	//아이템의 레벨을 올려준다 
			
		pItem->ItemAgingNum[1] = 0; //초기화 
		pItem->ItemAgingCount[0] = 0;
		pItem->ItemAgingCount[1] = 0;

		
		if( pItem->Level == 100 || pItem->Level == 102 || pItem->Level == 105 ||	// pluto 102레벨 아템 레벨 고정 할려고 // 장별 - 105렙 아이템 고정
			cAging.AginStoneKind == 3 || cAging.AginStoneKind == 4 || // 박재원 - 엘더 코퍼 오어, 슈퍼 에이징 스톤 사용시에는 에이징 차수가 상승해도 아이템 레벨이 올라가지 않는다.
			cAging.AginStoneKind == 5  ) // 장별 - 슈퍼 에이징 스톤 1.5
		{
			pItem->Level += 0;
		}
		else
		{
		}
				
	ReformItem(pItem);  //아이템 인증 받기 
	return TRUE;
}



int sinSetAgingItemIncreStateRestaure(sITEMINFO* pItem)
{
	float TempDefense[2] = { 0,0 };
	//에이징 초기화
	switch (pItem->CODE & sinITEM_MASK2) {
	case sinWA1: //도끼 
		//공격력 
		sinSetDamageAging(pItem);
		//명중력
		sinSetAttackRateAging(pItem, 10);
		//내구력
		sinSetDurabilityAging(pItem);

		break;
	case sinWC1: //클러
		//공격력 
		sinSetDamageAging(pItem);
		//명중력
		sinSetAttackRateAging(pItem, 5);
		//크리티컬  
		sinSetCriticalAging(pItem);

		break;

	case sinWH1: //해머 
		//공격력 
		sinSetDamageAging(pItem);

		//명중력
		sinSetAttackRateAging(pItem, 10);
		//내구력
		sinSetDurabilityAging(pItem);

		break;
	case sinWM1: //메지컬 스테프 
		//공격력 
		sinSetDamageAging(pItem);

		//명중력
		sinSetAttackRateAging(pItem, 10);

		//기력  
		pItem->fIncrease_Mana += 10;

		break;
	case sinWP1: //창 
		//공격력 
		sinSetDamageAging(pItem);

		//명중력
		sinSetAttackRateAging(pItem, 5);

		//크리티컬  
		sinSetCriticalAging(pItem);

		break;
	case sinWS1: //활 
		//공격력 
		sinSetDamageAging(pItem);

		//크리티컬  
		sinSetCriticalAging(pItem);

		//내구력
		sinSetDurabilityAging(pItem);
		break;

	case sinWS2: //칼  
		//공격력 
		sinSetDamageAging(pItem);

		//명중력
		sinSetAttackRateAging(pItem, 5);

		//크리티컬  
		sinSetCriticalAging(pItem);

		break;

	case sinWT1: //자벨린 
		//공격력 
		sinSetDamageAging(pItem);

		//크리티컬  
		sinSetCriticalAging(pItem);

		//내구력
		//sinSetDurabilityAging(pItem); 
		break;

	case sinDS1: //방패 
		//블럭율 
		sinSetBlockRateAging(pItem);
		//흡수율
		sinSetAbsorbAging(pItem, 0.2f);
		//내구력
		sinSetDurabilityAging(pItem);
		break;

	case sinOM1: //매지컬 감자 
		//방어력 
		sinSetDefenseAging(pItem, 10);

		//흡수율
		sinSetAbsorbAging(pItem, 0.5f);

		//내구력
		sinSetDurabilityAging(pItem);

		break;
	case sinDA1: case sinDA2://갑옷 
		//방어력 
		sinSetDefenseAging(pItem, 5);

		//흡수율
		sinSetAbsorbAging(pItem, 0.5f);

		//내구력
		sinSetDurabilityAging(pItem);

		break;
	case sinDB1: // 박재원 - 에이징 아이템 추가(부츠)
		//방어력 
		sinSetDefenseAging(pItem, 10);
		//흡수율
		sinSetAbsorbAging(pItem, 0.5f);
		//내구력
		sinSetDurabilityAging(pItem);
		break;
	case sinDG1: // 박재원 - 에이징 아이템 추가(장갑)
		//방어력 
		sinSetDefenseAging(pItem, 10);
		//흡수율
		sinSetAbsorbAging(pItem, 0.5f);
		//내구력
		sinSetDurabilityAging(pItem);
		break;
	case sinOA2: // 박재원 - 에이징 아이템 추가(암릿)
		//방어력 
		sinSetDefenseAging(pItem, 10);
		//명중력
		sinSetAttackRateAging(pItem, 5);
		//내구력
		sinSetDurabilityAging(pItem);
		break;
	}



	pItem->ItemAgingNum[0]++;	//아이템의 레벨을 올려준다 

	pItem->ItemAgingNum[1] = 0; //초기화 
	pItem->ItemAgingCount[0] = 0;
	pItem->ItemAgingCount[1] = 0;


	if (pItem->Level == 100 || pItem->Level == 102 || pItem->Level == 105 ||	// pluto 102레벨 아템 레벨 고정 할려고 // 장별 - 105렙 아이템 고정
		cAging.AginStoneKind == 3 || cAging.AginStoneKind == 4 || // 박재원 - 엘더 코퍼 오어, 슈퍼 에이징 스톤 사용시에는 에이징 차수가 상승해도 아이템 레벨이 올라가지 않는다.
		cAging.AginStoneKind == 5) // 장별 - 슈퍼 에이징 스톤 1.5
	{
		pItem->Level += 0;
	}
	else
	{
	}

	ReformItemRestaure(pItem);  //아이템 인증 받기 
	return TRUE;
}




//내구력을 내린다
int sinDownDurabilityAging(sITEMINFO *pItem)
{
	int Temp = 0;
	Temp = pItem->Durability[1]/100;
	Temp += 1;
	pItem->Durability[1] += Temp*DownNum;
	if(pItem->Durability[1] < 0 ) pItem->Durability[1] = 0;
	if(pItem->Durability[1] < pItem->Durability[0] ) pItem->Durability[0] = pItem->Durability[1]; //현제 내구력 보정 

	return TRUE;
}	
																																																																																									
// 에이징 공격력
int sinDownDamageAging(sITEMINFO *pItem , int Level)
{
	/*
		DownNum =  Down + 1;		
	*/
	if( Level < 10 ){
		pItem->Damage[0] -= DownNum; 
		pItem->Damage[1] -= DownNum;
	}
	else{
		switch( DownNum ){
			case 1:
				pItem->Damage[0] -= 2; 
				pItem->Damage[1] -= 2;
			break;
			case 2:
				if( Level == 10 ){
					pItem->Damage[0] -= 2+1; 
					pItem->Damage[1] -= 2+1;				
				}
				else{
					pItem->Damage[0] -= 2*2; 
					pItem->Damage[1] -= 2*2;
				}
			break;
			case 3:
				if( Level == 10 ){
					pItem->Damage[0] -= 2+1+1; 
					pItem->Damage[1] -= 2+1+1;
				}
				else if( Level == 11 ){
					pItem->Damage[0] -= 2*2+1; 
					pItem->Damage[1] -= 2*2+1;
				}
				else{
					pItem->Damage[0] -= 2*3; 
					pItem->Damage[1] -= 2*3;
				}
			break;
		}		
	}
	

	return TRUE;
}
//방어력
int sinDownDefenseAging(sITEMINFO *pItem , int Percent)
{

	float Temp = 0;
	int   TempDefence =0;
	for(int i = 0 ; i< DownNum ;i++){
		Temp = (100.0f*(float)pItem->Defence)/(100.0f+(float)Percent);
		TempDefence = pItem->Defence - (int)Temp;
		pItem->Defence -= TempDefence;
	}
	return TRUE;
}

//명중력
int sinDownAttackRateAging(sITEMINFO *pItem , int AddNum)
{

	pItem->Attack_Rating -= AddNum*DownNum;
	return TRUE;

}
//흡수율
int sinDownAbsorbAging(sITEMINFO *pItem , float Percent , int Level)
{

	pItem->fAbsorb -= Percent*(float)DownNum; 

	int Temp = Level-9; //레벨 10이상은 증가한 값만큼을 더빼준다
	if(Temp >0 ){
		if(Temp == 1){
			pItem->fAbsorb -= Percent;
		}
		else{
			pItem->fAbsorb -= Percent*(float)DownNum;

		}

	}
	return TRUE;

}

//크리티컬
int sinDownCriticalAging(sITEMINFO *pItem)
{
	pItem->Critical_Hit -=1; //일단 하나내려주고
	if(DownNum > 2){
		if(pItem->ItemAgingNum[0] && (pItem->ItemAgingNum[0]%2) == 1){
			pItem->Critical_Hit -= 1;
		}
	}
	return TRUE;

}

//블럭율
int sinDownBlockRateAging(sITEMINFO *pItem)
{
	pItem->fBlock_Rating -=1; //일단 하나내려주고
	if(DownNum > 2){
		if(pItem->ItemAgingNum[0] && (pItem->ItemAgingNum[0]%2) == 1){
			pItem->fBlock_Rating -= 1;

		}
	}
	return TRUE;

}

//에이징 레벨초기화
int sinAgingItemInit(sITEMINFO *pItem , int Down)
{

	float TempDefense[2] = {0,0}; 
	DownNum = Down + 1;	
	int AgingLevelTemp = 0;
	AgingLevelTemp = pItem->ItemAgingNum[0];

	pItem->ItemAgingNum[0] -= DownNum;	//먼저 레벨을 내려준다 (크리티컬 계산문제)
/*
	if((pItem->CODE & sinITEM_MASK2) == sinMA1){
	}
	else{
	
*/	
	//에이징 레벨다운 --
	switch(pItem->CODE & sinITEM_MASK2){
		case sinWA1: //도끼 
			//공격력 
			sinDownDamageAging(pItem,AgingLevelTemp);
			//명중력
			sinDownAttackRateAging(pItem , 10);
			//내구력
			sinDownDurabilityAging(pItem); 

		break;
		case sinWC1: //클러
			//공격력 
			sinDownDamageAging(pItem,AgingLevelTemp);
			//명중력
			sinDownAttackRateAging(pItem , 5);
			//크리티컬  
			sinDownCriticalAging(pItem);

		break;

		case sinWH1: //해머 
			//공격력 
			sinDownDamageAging(pItem,AgingLevelTemp);

			//명중력
			sinDownAttackRateAging(pItem , 10);
			//내구력
			sinDownDurabilityAging(pItem); 

		break;
		case sinWM1: //메지컬 스테프 
			//공격력 
			sinDownDamageAging(pItem,AgingLevelTemp);

			//명중력
			sinDownAttackRateAging(pItem , 10);

			//기력  
			pItem->fIncrease_Mana -= 10*DownNum;

		break;
		case sinWP1: //창 
			//공격력 
			sinDownDamageAging(pItem,AgingLevelTemp);

			//명중력
			sinDownAttackRateAging(pItem , 5);

			//크리티컬  
			sinDownCriticalAging(pItem);

		break;
		case sinWS1: //활 
			//공격력 
			sinDownDamageAging(pItem,AgingLevelTemp);

			//크리티컬  
			sinDownCriticalAging(pItem);

			//내구력
			sinDownDurabilityAging(pItem); 
		break;

		case sinWS2: //칼  
			//공격력 
			sinDownDamageAging(pItem,AgingLevelTemp);

			//명중력
			sinDownAttackRateAging(pItem , 5);
			
			//크리티컬  
			sinDownCriticalAging(pItem);

		break;

		case sinWT1: //자벨린 
			//공격력 
			sinDownDamageAging(pItem,AgingLevelTemp);

			//크리티컬  
			sinDownCriticalAging(pItem);

			//내구력
			//sinSetDurabilityAging(pItem); 
		break;
			
		case sinDS1: //방패 
			//블럭율 
			sinDownBlockRateAging(pItem);
			//흡수율
			sinDownAbsorbAging(pItem , 0.2f ,AgingLevelTemp );
			//내구력
			sinDownDurabilityAging(pItem); 
		break;

		case sinOM1: //매지컬 감자 
			//방어력 
			sinDownDefenseAging(pItem,10);

			//흡수율
			sinDownAbsorbAging(pItem , 0.5f,AgingLevelTemp);

			//내구력
			sinDownDurabilityAging(pItem); 

		break;
		case sinDA1: case sinDA2://갑옷 
			//방어력 
			sinDownDefenseAging(pItem,5);

			//흡수율
			sinDownAbsorbAging(pItem , 0.5f,AgingLevelTemp);

			//내구력
			sinDownDurabilityAging(pItem); 

		break; 
		case sinDB1: // 박재원 - 에이징 아이템 추가(부츠)
			//방어력 
			sinDownDefenseAging(pItem,10);
			//흡수율
			sinDownAbsorbAging(pItem , 0.5f, AgingLevelTemp);
			//내구력
			sinDownDurabilityAging(pItem); 
		break;
		case sinDG1: // 박재원 - 에이징 아이템 추가(장갑)
			//방어력 
			sinDownDefenseAging(pItem,10);
			//흡수율
			sinDownAbsorbAging(pItem , 0.5f, AgingLevelTemp);
			//내구력
			sinDownDurabilityAging(pItem); 
		break;
		case sinOA2: // 박재원 - 에이징 아이템 추가(암릿)
			//방어력 
			sinDownDefenseAging(pItem,10);
			//명중력
			sinDownAttackRateAging(pItem , 5);
			//내구력
			sinDownDurabilityAging(pItem); 
		break;
		}

		pItem->ItemAgingNum[1] = 0; //초기화 
		pItem->ItemAgingCount[0] = 0;
		pItem->ItemAgingCount[1] = 0;
	
		// pluto 에이징
		if( pItem->Level >= 100 ){
			pItem->Level -= 0 ;
		}
		else

			pItem->Level -= 1; //레벨을 빼주고
			if(DownNum > 2){
				if((pItem->ItemAgingNum[0] % 2 ) == 1){
					pItem->Level -=1; //에이징레벨이 홀수일경우 레벨을 더 내려준다 
			}
	}


	ReformItem(pItem);  //아이템 인증 받기 
//	ReformMixItem( pItem ); //에이징 재인증 
	return TRUE;
}

int sinCheckAgingLevel(int Kind, bool bBillingUse)
{

	sITEM *pItem = 0;
	switch(Kind){
		case  SIN_AGING_ATTACK:
			if(!cInvenTory.InvenItem[sInven[0].ItemIndex-1].Flag ||
			!cInvenTory.InvenItem[sInven[0].ItemIndex-1].sItemInfo.ItemAgingNum[1])return FALSE;
			
			if((cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE &sinITEM_MASK2) == sinWA1 ||
				(cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE &sinITEM_MASK2) == sinWH1 ||
				(cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE &sinITEM_MASK2) == sinWM1 ){
					pItem = &cInvenTory.InvenItem[sInven[0].ItemIndex-1];
				}

		break;
		/*
		case  SIN_AGING_DEFENSE:
			//////////// 오브 , 비즈  (두번 체크를 해야하기때문에 이렇게해놓는다)
			if(cInvenTory.InvenItem[sInven[1].ItemIndex-1].Flag ||
				cInvenTory.InvenItem[sInven[1].ItemIndex-1].sItemInfo.ItemAgingNum[1]){
					if((cInvenTory.InvenItem[sInven[1].ItemIndex-1].CODE &sinITEM_MASK2) == sinOM1){
						pItem = &cInvenTory.InvenItem[sInven[1].ItemIndex-1];
						if( pItem && pItem->sItemInfo.ItemKindCode == ITEM_KIND_AGING) {
							///////////호출될때마다 올려준다  
							if(smConfig.DebugMode){
								pItem->sItemInfo.ItemAgingCount[0] +=200;
							}
							else{
								pItem->sItemInfo.ItemAgingCount[0]++;
							}
							if(pItem->sItemInfo.ItemAgingCount[1] &&  pItem->sItemInfo.ItemAgingCount[0] >= pItem->sItemInfo.ItemAgingCount[1] ){
								//서버로 아이템을 보낸다 
								SendAgingUpgradeItemToServer(&pItem->sItemInfo); 
							}
						}
						pItem = 0;
					}
				}

			// 갑옷 , 로브 	
			if(cInvenTory.InvenItem[sInven[2].ItemIndex-1].Flag ||
				cInvenTory.InvenItem[sInven[2].ItemIndex-1].sItemInfo.ItemAgingNum[1]){
				if((cInvenTory.InvenItem[sInven[2].ItemIndex-1].CODE &sinITEM_MASK2) == sinDA1 ||
					(cInvenTory.InvenItem[sInven[2].ItemIndex-1].CODE &sinITEM_MASK2) == sinDA2 ){
					pItem = &cInvenTory.InvenItem[sInven[2].ItemIndex-1];
				}
			}
		
		break;
		*/
		case  SIN_AGING_CRITICAL:
			if(!cInvenTory.InvenItem[sInven[0].ItemIndex-1].Flag ||
			!cInvenTory.InvenItem[sInven[0].ItemIndex-1].sItemInfo.ItemAgingNum[1])return FALSE;
			if((cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE &sinITEM_MASK2) == sinWC1 ||
				(cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE &sinITEM_MASK2) == sinWS2 ||
				(cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE &sinITEM_MASK2) == sinWS1 ||
				(cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE &sinITEM_MASK2) == sinWP1 ||
				(cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE &sinITEM_MASK2) == sinWT1){
					pItem = &cInvenTory.InvenItem[sInven[0].ItemIndex-1];

				}

		break;

		case  SIN_AGING_BLOCK:
			if(	!cInvenTory.InvenItem[sInven[1].ItemIndex-1].Flag ||
				!cInvenTory.InvenItem[sInven[1].ItemIndex-1].sItemInfo.ItemAgingNum[1])return FALSE;

			if((cInvenTory.InvenItem[sInven[1].ItemIndex-1].CODE &sinITEM_MASK2) == sinDS1)
				pItem = &cInvenTory.InvenItem[sInven[1].ItemIndex-1];

		break;

		// 박재원 - 에이징 마스터 아이템
		case SIN_AGING_DEFENSE_ARMOR: // 갑옷, 로브
			// 갑옷 , 로브 	
			if(cInvenTory.InvenItem[sInven[2].ItemIndex-1].Flag ||
				cInvenTory.InvenItem[sInven[2].ItemIndex-1].sItemInfo.ItemAgingNum[1]){
					if((cInvenTory.InvenItem[sInven[2].ItemIndex-1].CODE &sinITEM_MASK2) == sinDA1 ||
						(cInvenTory.InvenItem[sInven[2].ItemIndex-1].CODE &sinITEM_MASK2) == sinDA2 ){
							pItem = &cInvenTory.InvenItem[sInven[2].ItemIndex-1];
						}
				}
		break;
		
		// 박재원 - 에이징 마스터 아이템
		case SIN_AGING_DEFENSE_ORB: // 오브 비즈
			if(cInvenTory.InvenItem[sInven[1].ItemIndex-1].Flag ||
				cInvenTory.InvenItem[sInven[1].ItemIndex-1].sItemInfo.ItemAgingNum[1]){
					if((cInvenTory.InvenItem[sInven[1].ItemIndex-1].CODE &sinITEM_MASK2) == sinOM1){
						pItem = &cInvenTory.InvenItem[sInven[1].ItemIndex-1];
					}
				}
		break;

		// 박재원 - 에이징 아이템 추가(부츠)
		case SIN_AGING_DEFENSE_BOOTS: // 부츠
			if(	!cInvenTory.InvenItem[sInven[9].ItemIndex-1].Flag ||
				!cInvenTory.InvenItem[sInven[9].ItemIndex-1].sItemInfo.ItemAgingNum[1])return FALSE;

			if((cInvenTory.InvenItem[sInven[9].ItemIndex-1].CODE &sinITEM_MASK2) == sinDB1)
				pItem = &cInvenTory.InvenItem[sInven[9].ItemIndex-1];
			break;

			// 박재원 - 에이징 아이템 추가(장갑)
		case SIN_AGING_DEFENSE_GLOVES: // 장갑
			if(	!cInvenTory.InvenItem[sInven[8].ItemIndex-1].Flag ||
				!cInvenTory.InvenItem[sInven[8].ItemIndex-1].sItemInfo.ItemAgingNum[1])return FALSE;

			if((cInvenTory.InvenItem[sInven[8].ItemIndex-1].CODE &sinITEM_MASK2) == sinDG1)
				pItem = &cInvenTory.InvenItem[sInven[8].ItemIndex-1];
			break;

			// 박재원 - 에이징 아이템 추가(암릿)
		case SIN_AGING_DEFENSE_ARMLETS: // 암릿
			if(	!cInvenTory.InvenItem[sInven[7].ItemIndex-1].Flag ||
				!cInvenTory.InvenItem[sInven[7].ItemIndex-1].sItemInfo.ItemAgingNum[1])return FALSE;

			if((cInvenTory.InvenItem[sInven[7].ItemIndex-1].CODE &sinITEM_MASK2) == sinOA2)
				pItem = &cInvenTory.InvenItem[sInven[7].ItemIndex-1];
			break;
	}

	if(!pItem || pItem->sItemInfo.ItemKindCode != ITEM_KIND_AGING)return FALSE;

	///////////호출될때마다 올려준다  
	if(smConfig.DebugMode)
	{
		if(bBillingUse == false)
		{
			pItem->sItemInfo.ItemAgingCount[0] +=200;
		}
		else if(bBillingUse == true)
			pItem->sItemInfo.ItemAgingCount[0] = pItem->sItemInfo.ItemAgingCount[1];
	}
	else
	{   // mature item age xxstr
		if(bBillingUse == false)
		{
			//pItem->sItemInfo.ItemAgingCount[0]++;
			pItem->sItemInfo.ItemAgingCount[0] += 50;
		}
		else if(bBillingUse == true)
		{
			pItem->sItemInfo.ItemAgingCount[0] = pItem->sItemInfo.ItemAgingCount[1];
		}
	}

	if(pItem->sItemInfo.ItemAgingCount[1] && pItem->sItemInfo.ItemAgingCount[0] >= pItem->sItemInfo.ItemAgingCount[1] ){
		//서버로 아이템을 보낸다 
		SendAgingUpgradeItemToServer(&pItem->sItemInfo);
	}



	/*

	//에이징 아이템이 아니거나 에이징이 꽉찬 아이템은 리턴한다 
	if(pItem->sItemInfo.ItemKindCode != ITEM_KIND_AGING ||
		(pItem->sItemInfo.ItemAgingCount[0] >= pItem->sItemInfo.ItemAgingCount[1]) )
		return FALSE;
		

	//////////////초 노가다 작업 
	if(!CheckItemForm(&pItem->sItemInfo)){ //아이템 인증받기 
		SendSetHackUser(12); //해킹을 하려고했던 씨바!! 유저를 고발 TRUE 접속 종료 
		return FALSE;

	}

	//에이징 아이템 인증확인 
	if(!CheckMixItem( &pItem->sItemInfo )){
		SendSetHackUser(52); //해킹을 하려고했던 못된 유저를 고발 TRUE 접속 종료 
		return FALSE;
	}
	*/

	return TRUE;	
}

//능력치가 향상된 아이템을 받는다 
int sinRecvAgingIncreStateItem(sITEMINFO *pItem)
{
	for(int i = 0; i  < INVENTORY_MAXITEM ; i++){ 
		if(cInvenTory.InvenItem[i].sItemInfo.BackUpChkSum  == pItem->BackUpChkSum &&
			cInvenTory.InvenItem[i].sItemInfo.BackUpKey == pItem->BackUpKey &&
			cInvenTory.InvenItem[i].sItemInfo.CODE == pItem->CODE){
				memcpy(&cInvenTory.InvenItem[i].sItemInfo,pItem,sizeof(sITEMINFO));
				cInvenTory.ReFormInvenItem();   //인벤토리 아이템 인증 
				cInvenTory.CheckRequireItemToSet(&cInvenTory.InvenItem[i]); 
				cInvenTory.SetItemToChar(); //능력치 재조정
				break;

		}
	}

	return TRUE;
}

//에이징 아이템에 대한 해킹을 막는다 
int sinCheckAgingItemHack(sITEM *pItem)
{ 
	if(	pItem->sItemInfo.ItemKindCode == ITEM_KIND_CRAFT){
		return FALSE;

	}
	return TRUE;
}

///////////////해킹방지를 위해 돈을 한곳에서 관리한다 
int sinMinusMoney(int Money,int Kind)
{
	//공성전 세율의 영향받는다.
	if(Kind){
		//--------------------------------------------------------------------------//
		#ifdef HASIEGE_MODE
		    //공성전 세율적용 마이너스 머니 적용
			
		    sinChar->Money -= Money+((Money*cSinSiege.GetTaxRate())/100);
			SetTotalSubMoney(Money+(Money*cSinSiege.GetTaxRate())/100);
		#else
			sinChar->Money -=Money;
		#endif
		//---------------------------------------------------------------------------//
	}
	else{
		sinChar->Money -=Money;
		SetTotalSubMoney(Money);
	
	}

	ReformCharForm();//재인증 
	
	SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 

	return TRUE	;		
}

int sinPlusMoney(int Money)
{
	sinChar->Money += Money;

	ReformCharForm();//재인증 
	SetTotalAddMoney(Money);
	SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 

	return TRUE;
}

///////////////해킹방지를 위해 돈을 한곳에서 관리한다 (창고만 따로 관리 )
int sinMinusMoney2(int Money)
{
	sinChar->Money -= Money;

	ReformCharForm();//재인증 
	SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 

	return TRUE;
}

int sinPlusMoney2(int Money)
{
	sinChar->Money += Money;

	ReformCharForm();//재인증 
	SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 

	return TRUE;
}

//원소속성을 구한다 
int sinGetElement(smCHAR_INFO *pCharInfo)
{
/*
	sinSkillResistance[sITEMINFO_BIONIC] += Zenith_Element[sinSkill.UseSkill[i].Point-1]/TempDivide;
	sinSkillResistance[sITEMINFO_FIRE] += Zenith_Element[sinSkill.UseSkill[i].Point-1]/TempDivide;
	sinSkillResistance[sITEMINFO_ICE] += Zenith_Element[sinSkill.UseSkill[i].Point-1]/TempDivide;
	sinSkillResistance[sITEMINFO_LIGHTING] += Zenith_Element[sinSkill.UseSkill[i].Point-1]/TempDivide;
	sinSkillResistance[sITEMINFO_POISON] += Zenith_Element[sinSkill.UseSkill[i].Point-1]/TempDivide;
*/	
	
	
//	pCharInfo->Resistance
	
	return TRUE;
}

// Premiacao dos eventos

char  *PresentWeaponName[5][9] ={
	{"sp103" , 0,0,0,0,0,0,0,0 },
	{"fo104","fo105","fo106",0,0,0,0,0,0},
	{"fo101","fo102","fo103","os106",0,0,0,0,0},
	{"fo107","se103","se102","os107",0,0,0,0,0},
	{"se101","os108","fo108","gp111",0,0,0,0,0},
	
};


char  *PresentArmorName[5][9] ={
	{"pm104" , 0,0,0,0,0,0,0,0 },
	{"gp104","gp105","gp106","gp107",0,0,0,0,0},
	{"gp101","gp102","gp103","os106",0,0,0,0,0},
	{"gp108","gp109","gp110","os107",0,0,0,0,0},
	{"se101","os108","fo108","gp111",0,0,0,0,0},
	
};

int PresentPercentIndex[5] = {5900,2250,1450,250,150};

// Premia豫o evento quebra-cabe�a aqui
char  *PresentWeaponName2[8][9] ={
	{"ca120",0,0,0,0,0,0,0,0 }, // 2500
	{"fo109",0,0,0,0,0,0,0,0 }, // 2500
	{"ca119",0,0,0,0,0,0,0,0 }, // 1590
	{"os108",0,0,0,0,0,0,0,0 }, // 1500
	{"os109",0,0,0,0,0,0,0,0 }, // 700
	{"bi245",0,0,0,0,0,0,0,0 }, // 700
	{"bi256",0,0,0,0,0,0,0,0 }, // 500
	{"bi255",0,0,0,0,0,0,0,0 }, // 10
};

//방어구
char  *PresentArmorName2[8][9] ={
	{"ca120",0,0,0,0,0,0,0,0 }, // 2500
	{"fo129",0,0,0,0,0,0,0,0 }, // 2500
	{"ca119",0,0,0,0,0,0,0,0 }, // 1590
	{"os108",0,0,0,0,0,0,0,0 }, // 1500
	{"os109",0,0,0,0,0,0,0,0 }, // 700
	{"bi245",0,0,0,0,0,0,0,0 }, // 700
	{"bi256",0,0,0,0,0,0,0,0 }, // 500
	{"bi255",0,0,0,0,0,0,0,0 }, // 10
};


int PresentPercentIndex2[8] = {2000,2000,1590,1500,600,600,900,900};

char  *PresentChocoWeaponName3[17][9] ={
	{"sp115",0,0,0,0,0,0,0,0 },		//1
	{"pm104",0,0,0,0,0,0,0,0 },		//2
	{"GP120",0,0,0,0,0,0,0,0 },		//3
	{"GP109",0,0,0,0,0,0,0,0 },		//4
	{"bi106",0,0,0,0,0,0,0,0},		//5
	{"Bi107",0,0,0,0,0,0,0,0},		//6
	{"OS107",0,0,0,0,0,0,0,0},		//7
	{"OS108",0,0,0,0,0,0,0,0},		//8
	{"Da131",0,0,0,0,0,0,0,0},		//9
	{"Da231",0,0,0,0,0,0,0,0},		//10
	{"Da133",0,0,0,0,0,0,0,0},		//11
	{"Da233",0,0,0,0,0,0,0,0},		//12
	{"OR233",0,0,0,0,0,0,0,0},		//13
	{"FO107",0,0,0,0,0,0,0,0},		//14
	{"FO106",0,0,0,0,0,0,0,0},		//15
	{"bi108",0,0,0,0,0,0,0,0},		//16
	{"OS109",0,0,0,0,0,0,0,0},		//17
};

char  *PresentChocoArmorName3[17][9] ={
	{"sp115",0,0,0,0,0,0,0,0 },		//1
	{"pm104",0,0,0,0,0,0,0,0 },		//2
	{"GP120",0,0,0,0,0,0,0,0 },		//3
	{"GP109",0,0,0,0,0,0,0,0 },		//4
	{"bi106",0,0,0,0,0,0,0,0},		//5
	{"Bi107",0,0,0,0,0,0,0,0},		//6
	{"OS107",0,0,0,0,0,0,0,0},		//7
	{"OS108",0,0,0,0,0,0,0,0},		//8
	{"Da131",0,0,0,0,0,0,0,0},		//9
	{"Da231",0,0,0,0,0,0,0,0},		//10
	{"Da133",0,0,0,0,0,0,0,0},		//11
	{"Da233",0,0,0,0,0,0,0,0},		//12
	{"OR233",0,0,0,0,0,0,0,0},		//13
	{"FO107",0,0,0,0,0,0,0,0},		//14
	{"FO106",0,0,0,0,0,0,0,0},		//15
	{"bi108",0,0,0,0,0,0,0,0},		//16
	{"OS109",0,0,0,0,0,0,0,0},		//17
};

int PresentChocoPercentIndex3[17] = {1000,1000,100,200,1200,1200,489,10,600,600,600,600,400,900,900,200,1};


// 장별 - 캔디데이즈 보상 아이템
char  *PresentCandyWeaponName3[17][9] ={
	{"sp115",0,0,0,0,0,0,0,0 },		//1
	{"pm104",0,0,0,0,0,0,0,0 },		//2
	{"GP121",0,0,0,0,0,0,0,0 },		//3
	{"GP109",0,0,0,0,0,0,0,0 },		//4
	{"bi106",0,0,0,0,0,0,0,0},		//5
	{"Bi107",0,0,0,0,0,0,0,0},		//6
	{"OS107",0,0,0,0,0,0,0,0},		//7
	{"OS108",0,0,0,0,0,0,0,0},		//8
	{"Da139",0,0,0,0,0,0,0,0},		//9
	{"Da239",0,0,0,0,0,0,0,0},		//10
	{"Da141",0,0,0,0,0,0,0,0},		//11
	{"DA241",0,0,0,0,0,0,0,0},		//12
	{"OA137",0,0,0,0,0,0,0,0},		//13
	{"FO107",0,0,0,0,0,0,0,0},		//14
	{"FO106",0,0,0,0,0,0,0,0},		//15
	{"bi108",0,0,0,0,0,0,0,0},		//16
	{"OS109",0,0,0,0,0,0,0,0},		//17
};



char  *PresentCandyArmorName3[17][9] ={
	{"sp115",0,0,0,0,0,0,0,0 },		//1
	{"pm104",0,0,0,0,0,0,0,0 },		//2
	{"GP121",0,0,0,0,0,0,0,0 },		//3
	{"GP109",0,0,0,0,0,0,0,0 },		//4
	{"bi106",0,0,0,0,0,0,0,0},		//5
	{"Bi107",0,0,0,0,0,0,0,0},		//6
	{"OS107",0,0,0,0,0,0,0,0},		//7
	{"OS108",0,0,0,0,0,0,0,0},		//8
	{"Da139",0,0,0,0,0,0,0,0},		//9
	{"Da239",0,0,0,0,0,0,0,0},		//10
	{"Da141",0,0,0,0,0,0,0,0},		//11
	{"DA241",0,0,0,0,0,0,0,0},		//12
	{"OA137",0,0,0,0,0,0,0,0},		//13
	{"FO107",0,0,0,0,0,0,0,0},		//14
	{"FO106",0,0,0,0,0,0,0,0},		//15
	{"bi108",0,0,0,0,0,0,0,0},		//16
	{"OS109",0,0,0,0,0,0,0,0},		//17
};

int PresentCandyPercentIndex3[17] = {1000,1000,100,200,1200,1200,489,10,600,600,600,600,400,900,900,200,1};


// 장별 - 매지컬그린 에메랄드 보상 아이템
char  *PresentMagicalGreenEmeraldWeaponName3[18][9] ={
	{"pm104",0,0,0,0,0,0,0,0 },	//1
	{"bi119",0,0,0,0,0,0,0,0 },		//2
	{"gp118",0,0,0,0,0,0,0,0 },		//3
	{"gp120",0,0,0,0,0,0,0,0 },		//4
	{"gp119",0,0,0,0,0,0,0,0},		//5
	{"se101",0,0,0,0,0,0,0,0},		//6
	{"os107",0,0,0,0,0,0,0,0},		//7
	{"os108",0,0,0,0,0,0,0,0},		//8
	{"os109",0,0,0,0,0,0,0,0},		//9
	{"os110",0,0,0,0,0,0,0,0},		//10
	{"os111",0,0,0,0,0,0,0,0},		//11
	{"bi112",0,0,0,0,0,0,0,0},		//12
	{"bi108",0,0,0,0,0,0,0,0},		//13
	{"bi107",0,0,0,0,0,0,0,0},		//14
	{"bi106",0,0,0,0,0,0,0,0},		//15
	{"fo107",0,0,0,0,0,0,0,0},		//16
	{"fo108",0,0,0,0,0,0,0,0},		//17
	{"fo109",0,0,0,0,0,0,0,0},		//18
};



char  *PresentMagicalGreenEmeraldArmorName3[18][9] ={
	{"pm104",0,0,0,0,0,0,0,0 },	//1
	{"bi119",0,0,0,0,0,0,0,0 },		//2
	{"gp118",0,0,0,0,0,0,0,0 },		//3
	{"gp120",0,0,0,0,0,0,0,0 },		//4
	{"gp119",0,0,0,0,0,0,0,0},		//5
	{"se101",0,0,0,0,0,0,0,0},		//6
	{"os107",0,0,0,0,0,0,0,0},		//7
	{"os108",0,0,0,0,0,0,0,0},		//8
	{"os109",0,0,0,0,0,0,0,0},		//9
	{"os110",0,0,0,0,0,0,0,0},		//10
	{"os111",0,0,0,0,0,0,0,0},		//11
	{"bi112",0,0,0,0,0,0,0,0},		//12
	{"bi108",0,0,0,0,0,0,0,0},		//13
	{"bi107",0,0,0,0,0,0,0,0},		//14
	{"bi106",0,0,0,0,0,0,0,0},		//15
	{"fo107",0,0,0,0,0,0,0,0},		//16
	{"fo108",0,0,0,0,0,0,0,0},		//17
	{"fo109",0,0,0,0,0,0,0,0},		//18
};

int PresentMagicalGreenEmeraldPercentIndex3[18] = {1400,300,400,200,100,1,300,200,1,1,1,100,1500,1500,1500,1200,800, 496};


// 장별 - 매지컬그린 비취 보상 아이템
char  *PresentMagicalGreenJadeWeaponName3[15][9] ={
	{"pm103",0,0,0,0,0,0,0,0 },	//1
	{"Gp109",0,0,0,0,0,0,0,0 },	//2
	{"Gp121",0,0,0,0,0,0,0,0 },	//3
	{"Fo104",0,0,0,0,0,0,0,0 },		//4
	{"Fo105",0,0,0,0,0,0,0,0},		//5
	{"Fo106",0,0,0,0,0,0,0,0},		//6
	{"Os105",0,0,0,0,0,0,0,0},		//7
	{"Os106",0,0,0,0,0,0,0,0},		//8
	{"Da135",0,0,0,0,0,0,0,0},		//9
	{"Da137",0,0,0,0,0,0,0,0},		//10
	{"da235",0,0,0,0,0,0,0,0},		//11
	{"Da237",0,0,0,0,0,0,0,0},		//12
	{"Bi112",0,0,0,0,0,0,0,0},		//13
	{"Bi119",0,0,0,0,0,0,0,0},		//14
	{"bi121",0,0,0,0,0,0,0,0},		//15
};



char  *PresentMagicalGreedJadeArmorName3[15][9] ={
	{"pm103",0,0,0,0,0,0,0,0 },	//1
	{"Gp109",0,0,0,0,0,0,0,0 },	//2
	{"Gp121",0,0,0,0,0,0,0,0 },	//3
	{"Fo104",0,0,0,0,0,0,0,0 },		//4
	{"Fo105",0,0,0,0,0,0,0,0},		//5
	{"Fo106",0,0,0,0,0,0,0,0},		//6
	{"Os105",0,0,0,0,0,0,0,0},		//7
	{"Os106",0,0,0,0,0,0,0,0},		//8
	{"Da135",0,0,0,0,0,0,0,0},		//9
	{"Da137",0,0,0,0,0,0,0,0},		//10
	{"da235",0,0,0,0,0,0,0,0},		//11
	{"Da237",0,0,0,0,0,0,0,0},		//12
	{"Bi112",0,0,0,0,0,0,0,0},		//13
	{"Bi119",0,0,0,0,0,0,0,0},		//14
	{"bi121",0,0,0,0,0,0,0,0},		//15
};

int PresentMagicalGreedJadePercentIndex3[15] = {1500,1500,200,1500,1500,1000,300,200,500,500,500,500,100,100,100};


// 장별 - 카라의 눈물 보상 아이템
char  *PresentTearOfKaraWeaponName3[18][9] ={
	{"pm104",0,0,0,0,0,0,0,0 },		//1
	{"bi119",0,0,0,0,0,0,0,0 },		//2
	{"gp109",0,0,0,0,0,0,0,0 },		//3
	{"gp121",0,0,0,0,0,0,0,0 },		//4
	{"gp119",0,0,0,0,0,0,0,0},		//5
	{"se101",0,0,0,0,0,0,0,0},		//6
	{"os107",0,0,0,0,0,0,0,0},		//7
	{"os108",0,0,0,0,0,0,0,0},		//8
	{"os109",0,0,0,0,0,0,0,0},		//9
	{"os110",0,0,0,0,0,0,0,0},		//10
	{"os111",0,0,0,0,0,0,0,0},		//11
	{"bi112",0,0,0,0,0,0,0,0},		//12
	{"bi108",0,0,0,0,0,0,0,0},		//13
	{"bi107",0,0,0,0,0,0,0,0},		//14
	{"bi106",0,0,0,0,0,0,0,0},		//15
	{"fo107",0,0,0,0,0,0,0,0},		//16
	{"fo108",0,0,0,0,0,0,0,0},		//17
	{"fo109",0,0,0,0,0,0,0,0},		//18
};



char  *PresentTearOfKaraArmorName3[18][9] ={
	{"pm104",0,0,0,0,0,0,0,0 },		//1
	{"bi119",0,0,0,0,0,0,0,0 },		//2
	{"gp109",0,0,0,0,0,0,0,0 },		//3
	{"gp121",0,0,0,0,0,0,0,0 },		//4
	{"gp119",0,0,0,0,0,0,0,0},		//5
	{"se101",0,0,0,0,0,0,0,0},		//6
	{"os107",0,0,0,0,0,0,0,0},		//7
	{"os108",0,0,0,0,0,0,0,0},		//8
	{"os109",0,0,0,0,0,0,0,0},		//9
	{"os110",0,0,0,0,0,0,0,0},		//10
	{"os111",0,0,0,0,0,0,0,0},		//11
	{"bi112",0,0,0,0,0,0,0,0},		//12
	{"bi108",0,0,0,0,0,0,0,0},		//13
	{"bi107",0,0,0,0,0,0,0,0},		//14
	{"bi106",0,0,0,0,0,0,0,0},		//15
	{"fo107",0,0,0,0,0,0,0,0},		//16
	{"fo108",0,0,0,0,0,0,0,0},		//17
	{"fo109",0,0,0,0,0,0,0,0},		//18
};

int PresentTearOfKaraPercentIndex3[18] = {1400,300,400,200,100,1,300,200,1,1,1,100,1500,1500,1500,1200,800,496};

// Corre豫o quebra cabe�a, estava pegando premia豫o aqui
char  *PresentFindinvestigatorNineWeaponName3[1][9] = {"gf107",0,0,0,0,0,0,0,0 };
char  *PresentFindinvestigatorNineArmorName3[1][9] = {"gf107",0,0,0,0,0,0,0,0 };
int PresentFindinvestigatorNinePercentIndex3[1] = {10000};

// 장별 - 조사원을 찾아라 테일아뮬렛 배포
char  *PresentFindinvestigatorNineTaleWeaponName3[1][9] = {"gf108",0,0,0,0,0,0,0,0 };
char  *PresentFindinvestigatorNineTaleArmorName3[1][9] = {"gf108",0,0,0,0,0,0,0,0 };
int PresentFindinvestigatorNineTalePercentIndex3[1] = {10000};

// 장별 - 조사원을 찾아라 에토 보상 아이템
char  *PresentFindinvestigatorWeaponName3[12][9] ={
	{"bi109",0,0,0,0,0,0,0,0 },		//1
	{"pm104",0,0,0,0,0,0,0,0 },		//2
	{"fo109",0,0,0,0,0,0,0,0 },		//3
	{"fo110",0,0,0,0,0,0,0,0 },		//4
	{"gp110",0,0,0,0,0,0,0,0},		//5
	{"gp119",0,0,0,0,0,0,0,0},		//6
	{"gp120",0,0,0,0,0,0,0,0},		//7
	{"bi178",0,0,0,0,0,0,0,0},		//8
	{"bi183",0,0,0,0,0,0,0,0},		//9
	{"bi177",0,0,0,0,0,0,0,0},		//10
	{"oa135",0,0,0,0,0,0,0,0},		//11
	{"or230",0,0,0,0,0,0,0,0},		//12
};



char  *PresentFindinvestigatorArmorName3[12][9] ={
	{"bi109",0,0,0,0,0,0,0,0 },		//1
	{"pm104",0,0,0,0,0,0,0,0 },		//2
	{"fo109",0,0,0,0,0,0,0,0 },		//3
	{"fo110",0,0,0,0,0,0,0,0 },		//4
	{"gp110",0,0,0,0,0,0,0,0},		//5
	{"gp119",0,0,0,0,0,0,0,0},		//6
	{"gp120",0,0,0,0,0,0,0,0},		//7
	{"bi178",0,0,0,0,0,0,0,0},		//8
	{"bi183",0,0,0,0,0,0,0,0},		//9
	{"bi177",0,0,0,0,0,0,0,0},		//10
	{"oa135",0,0,0,0,0,0,0,0},		//11
	{"or230",0,0,0,0,0,0,0,0},		//12
};

int PresentFindinvestigatorPercentIndex3[12] = {1500,1500,1300,1000,1500,1000,1500,300,200,100,50,50};


// Premia豫o Evento Melancia
char  *PresentWeaponName3[9][9] ={
	{"OS107",0,0,0,0,0,0,0,0 },		//1
	{"SE101",0,0,0,0,0,0,0,0 },		//2
	{"fo109",0,0,0,0,0,0,0,0 },		//3
	{"os107",0,0,0,0,0,0,0,0},		//4
	{"bi119",0,0,0,0,0,0,0,0},		//5
	{"os109",0,0,0,0,0,0,0,0},		//6
	{"bc127",0,0,0,0,0,0,0,0},		//7
	{"os108",0,0,0,0,0,0,0,0},		//8
	{"BI246",0,0,0,0,0,0,0,0 },		//9
};

char  *PresentArmorName3[9][9] ={
	{"SE101",0,0,0,0,0,0,0,0 },		//1
	{"bi177",0,0,0,0,0,0,0,0 },		//2
	{"bc124",0,0,0,0,0,0,0,0 },		//3
	{"bc121",0,0,0,0,0,0,0,0},		//4
	{"SE101",0,0,0,0,0,0,0,0},		//5
	{"os108",0,0,0,0,0,0,0,0},		//6
	{"BI244",0,0,0,0,0,0,0,0},		//7
	{"BI246",0,0,0,0,0,0,0,0},		//8
	{"BI248",0,0,0,0,0,0,0,0 },		//9
};

int PresentPercentIndex3[9] = {2000,2000,1300,1200,1000,1000,1000,600,600};


// 박재원 - 알파벳 조합 이벤트
char  *PresentWeaponName5[11][9] ={
	{"pm104",0,0,0,0,0,0,0,0},													//1
	{"bi108",0,0,0,0,0,0,0,0},													//2
	{"bi106","bi107",0,0,0,0,0,0,0},											//3
	{"fo107",0,0,0,0,0,0,0,0},													//4
	{"or228","oa133",0,0,0,0,0,0,0},											//5
	{"os107","os108","fo109",0,0,0,0,0,0},										//6
	{"bi119",0,0,0,0,0,0,0,0},													//7
	{"gp119","gp120","gp121",0,0,0,0,0,0},										//8
	{"bi112",0,0,0,0,0,0,0,0},													//9
	{"bi213","bi214","bi215","bi216","bi217","bi218","bi219","bi220",0},		//10
	{"os109","se101",0,0,0,0,0,0,0},											//11
};

char  *PresentArmorName5[11][9] ={
	{"pm104",0,0,0,0,0,0,0,0},													//1
	{"bi108",0,0,0,0,0,0,0,0},													//2
	{"bi106","bi107",0,0,0,0,0,0,0},											//3
	{"fo107",0,0,0,0,0,0,0,0},													//4
	{"or228","oa133",0,0,0,0,0,0,0},											//5
	{"os107","os108","fo109",0,0,0,0,0,0},										//6
	{"bi119",0,0,0,0,0,0,0,0},													//7
	{"gp119","gp120","gp121",0,0,0,0,0,0},										//8
	{"bi112",0,0,0,0,0,0,0,0},													//9
	{"bi213","bi214","bi215","bi216","bi217","bi218","bi219","bi220",0},		//10
	{"os109","se101",0,0,0,0,0,0,0},											//11
};

int PresentPercentIndex5[11] = {1450,1300,2600,1198,1000,1000,300,100,50,1000,20};


DWORD sinForceOrbCode[] = {(sinFO1|sin01),(sinFO1|sin02),(sinFO1|sin03),(sinFO1|sin04),(sinFO1|sin05),(sinFO1|sin06),(sinFO1|sin07),(sinFO1|sin08),(sinFO1|sin09),(sinFO1|sin10),(sinFO1|sin11),(sinFO1|sin12),(sinFO1|sin13),(sinFO1|sin14),(sinFO1|sin15)}; // 박재원 - 벨룸, 오르도 포스 추가

// 박재원 - 매직 포스 추가
DWORD sinMagincForceOrbCode[] = {(sinFO1|sin21),(sinFO1|sin22),(sinFO1|sin23),(sinFO1|sin24),(sinFO1|sin25),(sinFO1|sin26),(sinFO1|sin27),(sinFO1|sin28),(sinFO1|sin29),(sinFO1|sin30),(sinFO1|sin31),(sinFO1|sin32),(sinFO1|sin33),(sinFO1|sin34)};
// 박재원 - 빌링 매직 포스 추가
DWORD sinBillingMagicForceOrbCode[] = {(sinFO1|sin35),(sinFO1|sin36),(sinFO1|sin37)};

DWORD sinWingItemQuestServer(sCRAFTITEM_SERVER *pWingItem , smCHAR_INFO *pChar_Info)
{
	//3차 전업퀘스트
	int i = 0, j = 0 , k = 0; 
	int CodeCount = 0;
	int ItemCount = 0;
	int ItemCountPuzzle = 0;
	int PresentPercent = 0;
	int PresentItemIndex = 0;
	int PresentPercentSum = 0;
	int PresentItemCount = 0;
	int PotionNum = 0;
	short ItemCount80_2[4] = {0,};

	int ItemIndex = 0; 
	char *((*pPresentWeaponName)[9]);
	char *((*pPresentArmorName)[9]);
	int  *pPresentPercentIndex;

	int RandomTemp = 0 ;

	int CristalCount=0;
	int CristalFlag =0;
	//일곱가지 크리스탈을 다시체크한다.
	for( i = 0 ; i < 7 ; i++ ){
		for( j = 0 ; j < 12 ; j++ ){
			if( pWingItem->SheltomCode[i]==CristalEventCODE[j] ){
				CristalCount = i+1;
			}
		}
	}
	
	int WatermelonCount=0;
	int WatermelonFlag =0;
	//박재원 - 수박 모아오기(7개의 수박을 다시체크한다.)
	for(int n = 0 ; n < 7 ; n++ )
	{
		if( pWingItem->SheltomCode[n]==WatermelonEventCODE )
		{
			WatermelonCount = n+1;
		}
	}

	int ValentineCount=0;
	int ValentineFlag =0;
	//장별 - 발렌타인 초콜릿 모아오기(7개의 초콜릿을 다시체크한다.)
	for(int n = 0 ; n < 7 ; n++ )
	{
		if( pWingItem->SheltomCode[n]==ValentineEventCODE )
		{
			ValentineCount = n+1;
		}
	}


	int CandydaysCount=0;
	int CandydaysFlag =0;
	// 장별 - 캔디데이즈 (7개의 캔디 다시체크한다.)
	for(int n = 0 ; n < 7 ; n++ )
	{
		if( pWingItem->SheltomCode[n]==CandydaysEventCODE )
		{
			CandydaysCount = n+1;
		}
	}


	int MagicalGreenEmeraldCount=0;
	int MagicalGreenEmeraldFlag =0;
	int MagicalGreenJadeCount=0;
	int MagicalGreenJadeFlag =0;
	// 장별 - 매지컬그린 (7개의 에메랄드 다시체크한다.)
	for(int a = 0 ; a < 7 ; a++ )
	{
		if( pWingItem->SheltomCode[a]==MagicalGreenEventCODE[1] )
		{
			MagicalGreenEmeraldCount = a+1;
		}
	}

	// 장별 - 매지컬그린 (7개의 비취 다시체크한다.)
	for(int b = 0 ; b < 7 ; b++ )
	{
		if( pWingItem->SheltomCode[b]==MagicalGreenEventCODE[0] )
		{
			MagicalGreenJadeCount = b+1;
		}
	}

	int TearOfKaraCount=0;
	int TearOfKaraFlag =0;
	// 장별 - 카라의 눈물 (7개의 눈물 다시체크한다.)
	for(int n = 0 ; n < 7 ; n++ )
	{
		if( pWingItem->SheltomCode[n]==TeatOfKaraEventCODE )
		{
			TearOfKaraCount = n+1;
		}
	}


	// 장별 - 조사원을 찾아라
	int FindinvestigatorNineCount=0;
	int FindinvestigatorNineFlag =0;
	int FindinvestigatorTaleCount=0;
	int FindinvestigatorTaleFlag =0;
	
	for(int a = 0 ; a < 1 ; a++ )
	{
		if( pWingItem->SheltomCode[a]==FindinvestigatorEventCODE[0] )
		{
			FindinvestigatorNineCount = a+1;
		}
	}

	for(int b = 0 ; b < 1 ; b++ )
	{
		if( pWingItem->SheltomCode[b]==FindinvestigatorEventCODE[1] )
		{
			FindinvestigatorTaleCount = b+1;
		}
	}


	// 박재원 - 알파벳 조합 이벤트
	int PristonAlphabetCount = 0;
	int PristonAlphabetFlag = 0;
	for( int c = 0 ; c < 7 ; c++ )
	{
		for( int d = 0 ; d < 7 ; d++ )
		{
			if( pWingItem->SheltomCode[c]==PristonAlphabetEventCODE[d] )
			{
				PristonAlphabetCount = c+1;
			}
		}
	}

	//바벨아이템 
	if( pWingItem->SheltomCode[0] == (sinGF1 | sin02) ){
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 5;
		pPresentWeaponName = PresentWeaponName; //포인터 연결
		pPresentArmorName = PresentArmorName;
		pPresentPercentIndex = PresentPercentIndex;
		PotionNum = 30;
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;

		}
	}
	// 구미호아이템
	else if(pWingItem->SheltomCode[0] == (sinGF1 | sin03) &&  pWingItem->SheltomCode[1] == (sinGF1 | sin04)){
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 6;
		pPresentWeaponName = PresentWeaponName2;
		pPresentArmorName = PresentArmorName2;
		pPresentPercentIndex = PresentPercentIndex2;
		PotionNum = 20;
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;

		}
	}

	// 빛나는 가루
	else if(pWingItem->SheltomCode[0] == (sinGF1|sin05) ) { //&&  pWingItem->SheltomCode[1] == (sinGF1 | sin04)){
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 6;
		pPresentWeaponName = PresentWeaponName2;
		pPresentArmorName = PresentArmorName2;
		pPresentPercentIndex = PresentPercentIndex2;
		PotionNum = 20;
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;

		}
	}
	//일곱가지 크리스탈의 보은 아이템 주기
	else if(CristalCount==7){
		CristalFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 5;
		//임시로
		pPresentWeaponName   = PresentWeaponName3; //포인터 연결
		pPresentArmorName    = PresentArmorName3;
		pPresentPercentIndex = PresentPercentIndex3;
		PotionNum = 40;
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}
	//박재원 - 수박 모아오기
	else if(WatermelonCount==7)
	{
		WatermelonFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 9; // 박재원 - 수박 이벤트 12가지 중 한가지 아이템을 보상한다. // 호박 이벤트 9가지 중 한가지를 보상한다. // 밤하늘의 소원 이벤트 9가지중 한가지를 보상한다.
		//임시로
		pPresentWeaponName   = PresentWeaponName3; //포인터 연결
		pPresentArmorName    = PresentArmorName3;
		pPresentPercentIndex = PresentPercentIndex3;
		PotionNum = 5; // 박재원 - 수박 이벤트 보상 아이템이 물약일 경우 물약 개수를 셋팅한다. //박재원 - 밤하늘의 소원 이벤트(물약 보상 5개)
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}

	//장별 - 발렌타인 초콜릿 모아오기
	else if(ValentineCount==7)
	{
		ValentineFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 17; // 장별 - 발렌타인 초콜릿 이벤트 17가지 중 한가지 아이템을 보상한다. 
		//임시로
		pPresentWeaponName   = PresentChocoWeaponName3; //포인터 연결
		pPresentArmorName    = PresentChocoArmorName3;
		pPresentPercentIndex = PresentChocoPercentIndex3;
		PotionNum = 5; // 장별 - 발렌타인 이벤트 보상 아이템이 물약일 경우 물약 개수를 셋팅한다.
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}
	// 박재원 - 알파벳 조합 이벤트
	else if(PristonAlphabetCount==7){
		PristonAlphabetFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 11;
		//임시로
		pPresentWeaponName   = PresentWeaponName5; //포인터 연결
		pPresentArmorName    = PresentArmorName5;
		pPresentPercentIndex = PresentPercentIndex5;
		PotionNum = 20;
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}


	// 장별 - 캔디데이즈 캔디 모아오기
	else if(CandydaysCount==7)
	{
		CandydaysFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 17; // 장별 - 캔디데이즈 이벤트 17가지 중 한가지 아이템을 보상한다. 
		//임시로
		pPresentWeaponName   = PresentCandyWeaponName3; //포인터 연결
		pPresentArmorName    = PresentCandyArmorName3;
		pPresentPercentIndex = PresentCandyPercentIndex3;
		PotionNum = 5; // 장별 - 캔디데이즈 이벤트 보상 아이템이 물약일 경우 물약 개수를 셋팅한다.
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}

	// 장별 - 매지컬그린 -  비취 모아오기
	else if(MagicalGreenJadeCount==7)
	{
		MagicalGreenJadeFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 15; // 장별 - 매지컬그린 이벤트 15가지 중 한가지 아이템을 보상한다. 
		//임시로
		pPresentWeaponName    = PresentMagicalGreenJadeWeaponName3; //포인터 연결
		pPresentArmorName	 = PresentMagicalGreedJadeArmorName3;
		pPresentPercentIndex = PresentMagicalGreedJadePercentIndex3;
		PotionNum = 10; // 장별 - 매지컬그린 이벤트 보상 아이템이 물약일 경우 물약 개수를 셋팅한다.
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}


	// 장별 - 매지컬그린 -  에메랄드 모아오기
	else if(MagicalGreenEmeraldCount==7)
	{
		MagicalGreenEmeraldFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 18; // 장별 - 매지컬그린 이벤트 18가지 중 한가지 아이템을 보상한다. 
		//임시로
		pPresentWeaponName    = PresentMagicalGreenEmeraldWeaponName3; //포인터 연결
		pPresentArmorName	 = PresentMagicalGreenEmeraldArmorName3;
		pPresentPercentIndex = PresentMagicalGreenEmeraldPercentIndex3;
		PotionNum = 20; // 장별 - 매지컬그린 이벤트 보상 아이템이 물약일 경우 물약 개수를 셋팅한다.
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}

	// 장별 - 카라의 눈물  눈물 모아오기
	else if(TearOfKaraCount==7)
	{
		TearOfKaraFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 18; // 카라의 눈물 이벤트 18가지 중 한가지 아이템을 보상한다. 
		//임시로
		pPresentWeaponName   = PresentTearOfKaraWeaponName3; //포인터 연결
		pPresentArmorName    = PresentTearOfKaraArmorName3;
		pPresentPercentIndex = PresentTearOfKaraPercentIndex3;
		PotionNum = 10; // 장별 - 캔디데이즈 이벤트 보상 아이템이 물약일 경우 물약 개수를 셋팅한다.
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}


	// Corre豫o quebra cabe�a
	else if( FindinvestigatorNineCount == 1 && FindinvestigatorTaleCount == 1 &&  pWingItem->DesCraftItem.CODE != 1000  )
	{
		for(i = 0 ; i < 5 ; i++)
		{
			

			if(sWingQuest[pWingItem->Index].ChangeItemCode)
			{
				if(sWingQuest[pWingItem->Index].NeedItemCode[i]  == pWingItem->SheltomCode[i]) break;
			}


			switch(pWingItem->SheltomCode[i])
			{
				case (sinQT1|sin09):
				case (sinQT1|sin10):
				case (sinQT1|sin11):
				case (sinQT1|sin06):
				case (sinQT1|sin12):
				break;
				default:
					FindinvestigatorNineFlag = 1;
					RandomTemp = GetRandomPos(0,100);
					ItemIndex = 1; 

					pPresentWeaponName   = PresentFindinvestigatorNineWeaponName3;
					pPresentArmorName    = PresentFindinvestigatorNineArmorName3;
					pPresentPercentIndex = PresentFindinvestigatorNinePercentIndex3;
					PotionNum = 10; 
					if(RandomTemp < 50){
						ItemCountPuzzle = 8;
					}
					else{
						ItemCountPuzzle = 16;
					}
					break;
			}

/*
		//	if( pWingItem->SheltomCode[i] == sinQT1|sin06 || pWingItem->SheltomCode[i] == sinQT1|sin09 || pWingItem->SheltomCode[i] == sinQT1|sin10 
		//			|| pWingItem->SheltomCode[i] == sinQT1|sin11 || pWingItem->SheltomCode[i] == sinQT1|sin12 ) break;
				

		//		if(sWingQuest[pWingItem->Index].NeedItemCode[i]  != pWingItem->SheltomCode[i])
		//		{
					FindinvestigatorNineFlag = 1;
					RandomTemp = GetRandomPos(0,100);
					ItemIndex = 1; // 조사원을 찾아라 나인아뮬렛 배포
					//임시로
					pPresentWeaponName   = PresentFindinvestigatorNineWeaponName3; //포인터 연결
					pPresentArmorName    = PresentFindinvestigatorNineArmorName3;
					pPresentPercentIndex = PresentFindinvestigatorNinePercentIndex3;
					PotionNum = 10; 
					if(RandomTemp < 50){
						ItemCountPuzzle = 8;
					}
					else{
						ItemCountPuzzle = 16;
					}
					break;
	//			}
			*/
		}
	}

	// 장별 - 조사원을 찾아라( 나인아뮬렛을 소지하고 있을때)
	else if( FindinvestigatorNineCount == 1 )
	{
		FindinvestigatorNineFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 1; // 조사원을 찾아라 테일아뮬렛 배포
		//임시로
		pPresentWeaponName   = PresentFindinvestigatorNineTaleWeaponName3; //포인터 연결
		pPresentArmorName    = PresentFindinvestigatorNineTaleArmorName3;
		pPresentPercentIndex = PresentFindinvestigatorNineTalePercentIndex3;
		PotionNum = 10; 
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}

	// 장별 - 조사원을 찾아라( 테일아뮬렛을 소지하고 있을때)
	else if( FindinvestigatorTaleCount == 1 ) 
	{
		FindinvestigatorTaleFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 12; 
		//임시로
		pPresentWeaponName   = PresentFindinvestigatorWeaponName3; //포인터 연결
		pPresentArmorName    = PresentFindinvestigatorArmorName3;
		pPresentPercentIndex = PresentFindinvestigatorPercentIndex3;
		PotionNum = 10; 
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}
	else{
		ItemIndex = 8;
		pPresentWeaponName = PresentWeaponName2;
		pPresentArmorName = PresentArmorName2;
		pPresentPercentIndex = PresentPercentIndex2;
		PotionNum = 10;

		//퍼즐
		for(i = 0 ; i < 8 ; i++){
			if((pWingItem->SheltomCode[i] & sinITEM_MASK2) == sinPZ1 ||
				(pWingItem->SheltomCode[i] & sinITEM_MASK2) == sinPZ2 ){
					for( j = 0 ; j < 8 ; j++){
						if(i == j)continue;
						if(pWingItem->SheltomCode[i] == pWingItem->SheltomCode[j]){
							pWingItem->Result = FALSE;
							return FALSE;

						}
					}
					if((pWingItem->SheltomCode[i] & sinITEM_MASK2) == sinPZ1)
						ItemCountPuzzle++;
					if((pWingItem->SheltomCode[i] & sinITEM_MASK2) == sinPZ2)
						ItemCountPuzzle +=2;

			}
		}

	}
	// Premiacao evento quebra cabe�a
	if(ItemCountPuzzle == 8 || ItemCountPuzzle == 16){
		PresentPercent = GetRandomPos(0,10000);
		for(i = 0 ; i < ItemIndex ; i++){
			if(PresentPercent >= PresentPercentSum && (pPresentPercentIndex[i]+PresentPercentSum) >=PresentPercent ){
				PresentItemCount = 0;

				if(ItemCountPuzzle == 8){
					for(k = 0 ; k < 9 ; k++){
						if(pPresentWeaponName[i][k]){
							PresentItemCount++;
						}
					}
					PresentItemIndex = GetRandomPos(0,PresentItemCount-1);
					while(1)
					{
						if(pPresentWeaponName[i][PresentItemIndex])
						{
							for(j=0;j<MAX_ITEM;j++) 
							{
								if ( lstrcmpi( pPresentWeaponName[i][PresentItemIndex] , sItem[j].LastCategory )==0 ) 
								{
									pWingItem->DesCraftItem.sItemInfo.CODE = sItem[j].CODE;
									if((pWingItem->DesCraftItem.sItemInfo.CODE & sinITEM_MASK2) == sinPM1) 
										pWingItem->DesCraftItem.sItemInfo.PotionCount = PotionNum;
									if(CristalFlag)pWingItem->Result = SIN_WINGRESULT_CRISTAL;
									else pWingItem->Result = TRUE;
									return TRUE;
								}
							}
						}
						PresentItemIndex--;
						if(PresentItemIndex < 0 )break; 
					}
				}

				PresentItemCount = 0;
				if(ItemCountPuzzle == 16){
					for(k = 0 ; k < 9 ; k++){
						if(pPresentArmorName[i][k]){
							PresentItemCount++;
						}
					}
					PresentItemIndex = GetRandomPos(0,PresentItemCount-1);
					while(1){
						if(pPresentArmorName[i][PresentItemIndex]){
							for(j=0;j<MAX_ITEM;j++) {
								if ( lstrcmpi( pPresentArmorName[i][PresentItemIndex] , sItem[j].LastCategory )==0 ) {
									pWingItem->DesCraftItem.sItemInfo.CODE = sItem[j].CODE;
									if((pWingItem->DesCraftItem.sItemInfo.CODE & sinITEM_MASK2) == sinPM1) 
										pWingItem->DesCraftItem.sItemInfo.PotionCount = PotionNum;
									if(CristalFlag)pWingItem->Result = SIN_WINGRESULT_CRISTAL;
									else pWingItem->Result = TRUE;
									return TRUE;
								}
							}
						}
						PresentItemIndex--;
						if(PresentItemIndex < 0 )break; //무한루프 방지
					}
				}
			}
			else PresentPercentSum += pPresentPercentIndex[i];
		}
	}
	
	//80_2 Lv퀘스트
	for(i = 0 ; i < 3 ; i++){
		switch(pWingItem->SheltomCode[i]){
			case (sinQT1|sin09):
				ItemCount80_2[0] = 1;
			break;
			case (sinQT1|sin10):
				ItemCount80_2[1] = 1;
			break;
			case (sinQT1|sin11):
				ItemCount80_2[2] = 1;
			break;

		}
	}
	if(ItemCount80_2[0] && ItemCount80_2[1] && ItemCount80_2[2]){
		pWingItem->Result = TRUE;
		pWingItem->DesCraftItem.sItemInfo.CODE = (sinQT1|sin12);
		return TRUE;

	}
	for(i = 0 ; i < 3 ; i++){
		if(pWingItem->SheltomCode[i] == (sinQT1|sin06)){
			ItemCount++;
		}
	}
	if(ItemCount == 3 && pChar_Info){
		pWingItem->Result = TRUE;
		switch(pChar_Info->JOB_CODE){
			case 1:		//파이터
				pWingItem->DesCraftItem.sItemInfo.CODE = (sinWA1 | sin08);
			break;
			case 2:		//메카니션
				pWingItem->DesCraftItem.sItemInfo.CODE = (sinWC1 | sin08);
			break;
			case 3:		//아쳐
				pWingItem->DesCraftItem.sItemInfo.CODE = (sinWS1 | sin10);
			break;
			case 4:		//파이크맨
				pWingItem->DesCraftItem.sItemInfo.CODE = (sinWP1 | sin09);
			break;
			case 5:		//아탈란타
				pWingItem->DesCraftItem.sItemInfo.CODE = (sinWT1 | sin09);
			break;
			case 6:		//나이트
				pWingItem->DesCraftItem.sItemInfo.CODE = (sinWS2 | sin10);
			break;
			case 7:		//매지션
				pWingItem->DesCraftItem.sItemInfo.CODE = (sinWM1 | sin09);
			break;
			case 8:		//프리스티스
				pWingItem->DesCraftItem.sItemInfo.CODE = (sinWM1 | sin09);
			break;
		}
		//서버에서 셋팅해준다
		pWingItem->DesCraftItem.sItemInfo.ItemKindCode = ITEM_KIND_QUEST_WEAPON;
		//SetChangeJob3QuestItem(&pWingItem->DesCraftItem.sItemInfo); //아이템 초기셋팅
		return TRUE;

	}
	//포스 오브
	if(pWingItem->DesCraftItem.CODE == 1000){
		for( i = 0 ; i < 12 ; i++){
			for(j = 0 ; j < 16 ; j++){ // 박재원 - 벨룸, 오르도 포스 추가(12 -> 14))
				if((pWingItem->SheltomCode[i] & sinITEM_MASK3) == SheltomCode2[j]){
					pWingItem->Head[i] = 0;
					pWingItem->CheckSum[i] = 0;
					pWingItem->SheltomCode[i] = sinForceOrbCode[j];
					break;
				}
				// 박재원 - 매직 포스 추가
				else if((pWingItem->SheltomCode[i] & sinITEM_MASK3) == MagicSheltomCode[j]){
					pWingItem->Head[i] = 0;
					pWingItem->CheckSum[i] = 0;
					pWingItem->SheltomCode[i] = sinMagincForceOrbCode[j];
					break;
				}
			}
		}
		pWingItem->Result = TRUE;
		return 0x100;
	}
	else{
		for(i = 0 ; i < 5 ; i++){
			if(sWingQuest[pWingItem->Index].ChangeItemCode){
				CodeCount++;
				if(sWingQuest[pWingItem->Index].NeedItemCode[i]  == pWingItem->SheltomCode[i]){
					ItemCount++;
				}
			}
		}
		if(CodeCount == ItemCount){
			pWingItem->DesCraftItem.sItemInfo.CODE = sWingQuest[pWingItem->Index].ChangeItemCode;
			pWingItem->DesCraftItem.sItemInfo.ItemKindCode = 0;
			pWingItem->Result = TRUE; 
			return TRUE;

		}
	}
	pWingItem->Result = FALSE;
	return FALSE;
}

//서버에 윙아이템을 보낸다
int SendWingQuestItem() //윙아이템 퀘스트 
{
	SendWingItemToServer( &sWingItem_Send );   
	return TRUE;
}

//서버에서 윙아이템을 받는다
int RecvWingItem(sCRAFTITEM_SERVER *pWingItem)
{
 
	sITEM *pItem=0; 
	int   Index = 0; 
	int   i = 0; 
	sITEM sinTempItem;  //아이템에 관련된 사항을 잠시 넣어두는 Temp

	//if(pWingItem->Result != TRUE)return FALSE;

	//퍼즐일경우는 어케하나 -0-

	for( i = 0; i < INVENTORY_MAXITEM*2 ; i++){
		if(i<100){
			if(!cInvenTory.InvenItem[i].Flag)continue;
			pItem = &cInvenTory.InvenItem[i];
		}
		else{
			if(!cInvenTory.InvenItemTemp[i-100].Flag)continue;
			pItem = &cInvenTory.InvenItemTemp[i-100];
		}

		//결과가 FALSE일경우에는 윙 아이템도 삭제한다
		if(pWingItem->Result == FALSE){
			cInvenTory.DeleteInvenItemToServer(pWingItem->DesCraftItem.sItemInfo.CODE , pWingItem->DesCraftItem.sItemInfo.ItemHeader.Head ,
			 pWingItem->DesCraftItem.sItemInfo.ItemHeader.dwChkSum);

		}

		//인증코드가 없는 투핸드 무기는 체크하지않는다 
		if(pItem->ItemPosition == 2){  
			if(pItem->Class == ITEM_CLASS_WEAPON_TWO)
				continue; 
		}

		for(int j = 0 ; j < 8 ; j++){
			if(pItem->sItemInfo.CODE == pWingItem->SheltomCode[j] &&
				pItem->sItemInfo.ItemHeader.Head == pWingItem->Head[j] &&
				pItem->sItemInfo.ItemHeader.dwChkSum == pWingItem->CheckSum[j]){
					pItem->Flag = 0; //아이템을 지워준다
					cInvenTory.ReFormInvenItem();
					cInvenTory.CheckWeight();
				}
				
		}
	}	
	
	//성공하였을경우에 돈을빼고 아이템을 셋팅한다
	if(pWingItem->Result == TRUE){
		sinMinusMoney(pWingItem->Money);//돈을 뺀다
		SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 

		//아이템을 인벤토리에 셋팅한다
		if(pWingItem->DesCraftItem.sItemInfo.CODE){
			if(LoadItemImage(&pWingItem->DesCraftItem.sItemInfo,&sinTempItem)){
				//퀘스트 아이템을 받고 레벨을 셋팅
				if(sinTempItem.sItemInfo.ItemKindCode ==ITEM_KIND_QUEST_WEAPON){
					sinQuest_ChangeJob3.StartLevel = sinChar->Level;

				}
				sinSetSpecialItemCode(&sinTempItem.sItemInfo); //스페셜아이템을 구분한다
				cInvenTory.AutoSetInvenItem(&sinTempItem,1+pWingItem->DocIndex); //인덱스에 1이있을경우 좌표를 고정한다
				cInvenTory.ReFormInvenItem();
				ResetPotion();
				cInvenTory.ReFormPotionNum();	//포션 갯수를 체크한다
			}
		}
	}

	ReformCharForm();//재인증 
	cInvenTory.SetItemToChar();
	cInvenTory.CheckWeight();
	
	sinbaram_Stop = 0; //이벤트가 정상작동되게한다
	cInvenTory.CheckPuzzle();

	return TRUE;	
}

//링을 에이징 한다
int AgingRing()
{
	if(sInven[4].ItemIndex){
		if(cInvenTory.InvenItem[sInven[4].ItemIndex-1].sItemInfo.ItemAgingCount[1] && cInvenTory.InvenItem[sInven[4].ItemIndex-1].sItemInfo.CODE == (sinOR2|sin01)){
			if(smConfig.DebugMode){
				cInvenTory.InvenItem[sInven[4].ItemIndex-1].sItemInfo.ItemAgingCount[0] +=100;
			}
			else
				cInvenTory.InvenItem[sInven[4].ItemIndex-1].sItemInfo.ItemAgingCount[0]++;
			if(cInvenTory.InvenItem[sInven[4].ItemIndex-1].sItemInfo.ItemAgingCount[0] >= 
				cInvenTory.InvenItem[sInven[4].ItemIndex-1].sItemInfo.ItemAgingCount[1]){
					SendAgingUpgradeItemToServer(&cInvenTory.InvenItem[sInven[4].ItemIndex-1].sItemInfo);

				}
		}
	}
	if(sInven[5].ItemIndex){
		if( cInvenTory.InvenItem[sInven[5].ItemIndex-1].sItemInfo.ItemAgingCount[1] &&cInvenTory.InvenItem[sInven[5].ItemIndex-1].sItemInfo.CODE == (sinOR2|sin01)){
			if(smConfig.DebugMode){
				cInvenTory.InvenItem[sInven[5].ItemIndex-1].sItemInfo.ItemAgingCount[0] +=100;
			}
			else
				cInvenTory.InvenItem[sInven[5].ItemIndex-1].sItemInfo.ItemAgingCount[0]++;
			if(cInvenTory.InvenItem[sInven[5].ItemIndex-1].sItemInfo.ItemAgingCount[0] >= 
				cInvenTory.InvenItem[sInven[5].ItemIndex-1].sItemInfo.ItemAgingCount[1]){
					SendAgingUpgradeItemToServer(&cInvenTory.InvenItem[sInven[5].ItemIndex-1].sItemInfo);

				}

		}
	}

	return TRUE;
}

//숫자에 콤마를찍어서 넘긴다
int NumLineComa(int Num , char *pBuff)
{

	int i = 0;
	char strBuff[128];
	wsprintf(strBuff,"%d",Num);   
	int cnt = 0;
	int cnt2 = 0;
	int cnt3 = 0;

	int len = lstrlen(strBuff);
	while(1){
		if(strBuff[cnt]){ 
			pBuff[cnt+cnt2] = strBuff[cnt];
			cnt3 = (len-1)-cnt;
			if((cnt3 % 3) ==0 && cnt3 != 0){
				cnt2++;
				pBuff[cnt+cnt2] = ',';
			}
			cnt++;

		}
		else break;
	}

	return TRUE;
}

//물약을 버렸을 경우 게임을 저장한다 (현재로선)
int sinSaveGame(DWORD CODE , int Count)
{
	DWORD MASK_CODE = (CODE & sinITEM_MASK2);

	//기약일경우 
	if(MASK_CODE == sinPM1 && Count >= 4 ){
		SaveGameData();

	}
	if((  MASK_CODE == sinPL1 || MASK_CODE == sinPS1 ) && Count >= 10){
		SaveGameData();
	}
	return TRUE;
}

//날짜가 지난아이템이 사라진걸 보여준다
int OverDay_Item_Flag = 0;
int OverDay_Item_Delete(sITEM *pItem)
{ 
	return TRUE;
}


//모든 인터페이스를 닫는다
int sinCloseInterFace() { return TRUE; }

int sinCheck_ShowHelpMap() { return TRUE; }

//포스 아이템을 받는다
int sinRecvForceOrb(sITEMINFO *pItemInfo , int Count) { return TRUE; }

DWORD SummonSkillCode[] = {
	SKILL_METAL_GOLEM,
	SKILL_FIRE_ELEMENTAL,
	SKILL_RECALL_WOLVERIN,
	0,

};
int SummonSkillCodeIndex[10] = {0,};

//더블클릭이벤트
int sinDoubleClick() { return TRUE; }

