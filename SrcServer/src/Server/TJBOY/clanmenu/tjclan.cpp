//ktj : 부클랜의 권유관련 : netplay.cpp에도 있음.

//====================================================================
//ktj : runmap3d.suo파일때문에 세이브가안되는경우가 있으므로 그럴때는 그 파일을 지울것
//====================================================================
//5.7 P4.28
//sinMain.cpp, sinInterFace.cpp에서 운영됨
#define TJCLAN_CPP

#include "tjclanDEF.h"


#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우




#include "..\\..\\sinbaram\\SinLinkHeader.h"

//#include "HoParty.h"
#include "..\\..\\language.h"


#include "tjclan.h"
#include "tjscroll.h"
#include "cE_Cmake.h"
#include "cE_chip.h"
#include "cE_user.h"
#include "cE_isclanmember.h"
#include "cE_report.h"
#include "cE_chatlist.h"
#include "cE_deletecha.h"
#include "cE_Cjoin.h"
#include "cE_CViewClanInfo.h"
#include "cE_Updatecldata.h"
#include "cE_Notice.h"

#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"
#include "Help.h"
#include "GuideHelp.h" 

#include "..\\ygy\\ygyheader.h"


//#include "NpcWav.h" //npc 웨이브 받아오기
//#include "Zip.h"


//#define USE_YAHEL		//아웰서버만 사용하는경우는 열어둘것




extern char	szConnServerName[16];	//접속한 서버이름이 들어있는 버퍼






#else						//=========== 포리스턴소스가 아닌경우

#include <windows.h>
#include <stdio.h>
#include "tjclan.h"
#include "tjscroll.h"

#include "clan_Enti.h"
#include "cE_Cmake.h"
#include "cE_chip.h"
#include "cE_user.h"
#include "cE_isclanmember.h"
#include "cE_report.h"
#include "cE_chatlist.h"
#include "cE_deletecha.h"
#include "cE_CJoin.h"
#include "cE_CViewClanInfo.h"
#include "cE_Updatecldata.h"
#include "cE_Notice.h"

#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"
#include "../../netplay.h"





#define MAX_PARTY_MEMBER	6

#define PARTY_NONE		500
#define PARTY_START		1000
#define PARTY_PROCESS	2000
#define PARTY_END		3000

char szConnServerName[16];	//접속한 서버이름이 들어있는 버퍼



extern HWND    g_hwnd;





#endif


int ClanCharMove = 0;	//클랜메뉴판안의 마우스의 움직임에 캐릭터가 영향없도록 한다.
int ClanCLCharMove = 0;//클랜채팅리스트메뉴판안의 마우스의 움직임에 캐릭터가 영향없도록 한다.


int  banisF[100];

//int TJwheel=0; //ktj : 휠데이타



//유광열 도움말 마우스 이벤트 갖어오기====START
int GuideHelpButton[3] = { 0,0,0 }; //가이드 도움말
int LevelpUpButton[3] = { 0,0,0 }; //레벨업 도움말
//END

BOOL bKeyDown = FALSE;
int nKeyDown = 0;
BOOL bKeyUp = FALSE;
int nKeyUp = 0;
BOOL bLButtonDown = FALSE;
BOOL bKeyPageDown = FALSE;
BOOL bKeyPageUp = FALSE;

BOOL IsDrawGuideHelp = FALSE; //가이드 도움말이 떠져있음

/*void GuidHelpMouse(int msg,WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_KEYUP:
		switch(wParam)
		{
		case VK_DOWN:
			GuideHelpButton[0] = FALSE; //가이드 도움말
			bKeyUp = FALSE;
			bKeyDown = FALSE;
			break;
		case VK_UP:
			bKeyDown = FALSE;
			bKeyUp = FALSE;
			GuideHelpButton[0] = FALSE; //가이드 도움말
			break;
		}
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_DOWN:
			GuideHelpButton[0] = TRUE; //가이드 도움말
			bKeyDown = TRUE;
			bKeyUp = FALSE;
			nKeyDown++;
			break;
		case VK_UP:
			GuideHelpButton[0] = TRUE; //가이드 도움말
			bKeyUp = TRUE;
			bKeyDown = FALSE;
			nKeyUp++;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		//유광열==============start
		bKeyDown = FALSE;
		bKeyUp = FALSE;
		GuideHelpButton[0] = TRUE; //가이드 도움말
		LevelpUpButton[0]=TRUE; //레벨업 도움말
		//end
		break;
	case WM_LBUTTONUP:
		bKeyDown = FALSE;
		bKeyUp = FALSE;
		GuideHelpButton[0] = FALSE; //가이드 도움말
		LevelpUpButton[0]=FALSE; //레벨업 도움말
		break;
	}
}
*/
void clan_Mouse(int msg, WPARAM wParam)
{
	switch (msg) {
		//유광열 WM_KEYUP추가
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_DOWN:
			GuideHelpButton[0] = FALSE; //가이드 도움말
			bKeyUp = FALSE;
			bKeyDown = FALSE;
			break;
		case VK_UP:
			bKeyDown = FALSE;
			bKeyUp = FALSE;
			GuideHelpButton[0] = FALSE; //가이드 도움말			
			break;

		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_DOWN:
			GuideHelpButton[0] = TRUE; //가이드 도움말
			bKeyDown = TRUE;
			bKeyUp = FALSE;
			nKeyDown++;
			break;
		case VK_UP:
			GuideHelpButton[0] = TRUE; //가이드 도움말
			bKeyUp = TRUE;
			bKeyDown = FALSE;
			nKeyUp++;
			break;
		case VK_ESCAPE://esc키로 도움말 끄기
			g_GuideHelp.GuidHelpEnd();
			g_Help.HelpEnd();
			break;

		}
		break;
		//==============END

	case WM_LBUTTONDOWN:
		TJmb2[0] = TJmb[0] = TRUE;
		//유광열==============start		
		bKeyDown = FALSE;
		bKeyUp = FALSE;

		GuideHelpButton[0] = TRUE; //가이드 도움말
		LevelpUpButton[0] = TRUE; //레벨업 도움말	

		//end
		break;

	case WM_LBUTTONUP:
		TJmb2[0] = TJmb[0] = FALSE;
		//유광열==============start
		bKeyDown = FALSE;
		bKeyUp = FALSE;
		bLButtonDown = FALSE;

		GuideHelpButton[0] = FALSE; //가이드 도움말
		LevelpUpButton[0] = FALSE; //레벨업 도움말
		//==============END
		break;

	case WM_RBUTTONDOWN:
		TJmb2[1] = TJmb[1] = TRUE; //ktj
		break;

	case WM_RBUTTONUP:
		TJmb2[1] = TJmb[1] = FALSE; //ktj
		break;

	case WM_MBUTTONDOWN:
		TJmb2[2] = TJmb[2] = TRUE; //ktj
		break;

	case WM_MBUTTONUP:
		TJmb2[2] = TJmb[2] = FALSE; //ktj
		break;
	}
}









void ktj_imsi_clanMENU_Main() {}

void ktj_imsi_clanMENU_Draw() {}


void clanMENUinit()
{
	Interface_clanmenu.CLANmenu_INIT();
	//ktj : 클랜상에서의 현재 위치를 알아낸다.
	Interface_clanmenu.CLANmenu_clanPOSITION();
}


//레벨업을 했을때 함수를 호출해준다.
//=========레벨업 도움말==========================
//int ng = 1;
void InitHelp()
{
	if (!g_GuideHelp.GetIsOn()) return; //레벨업 도움말이 꺼져있을 경우
	//if((sinChar->Level == 20)&&(sinChar->JOB_CODE > 4)) return;
	g_Help.SetStartPos(0, 0);
	if (!g_Help.CheckLevel(sinChar->Level, sinChar->JOB_CODE)) return;
	g_Help.SetHelpN(LEVELUP_HELP);
	g_Help.menuInit(LEVELUP_HELP);
	/*switch(ng)
	{
	case 1:
		g_Help.CheckLevel(1,2);
		break;
	case 2:
		g_Help.CheckLevel(1,5);
		break;
	case 3:
		g_Help.CheckLevel(2,2);
		break;
	case 4:
		g_Help.CheckLevel(3,2);
		break;
	case 5:
		g_Help.CheckLevel(4,2);
		break;
	case 6:
		g_Help.CheckLevel(5,2);
		break;
	case 7:
		g_Help.CheckLevel(6,2);
		break;
	case 8:
		g_Help.CheckLevel(8,2);
		break;
	case 9:
		g_Help.CheckLevel(10,2);
		break;
	case 10:
		g_Help.CheckLevel(10,5);
		break;
	case 11:
		g_Help.CheckLevel(12,2);
		break;
	case 12:
		g_Help.CheckLevel(15,2);
		break;
	case 13:
		g_Help.CheckLevel(17,2);
		break;
	case 14:
		g_Help.CheckLevel(20,2);
		break;
	case 15:
		g_Help.CheckLevel(22,2);
		break;
	case 16:
		g_Help.CheckLevel(25,2);
		break;
	case 17:
		g_Help.CheckLevel(30,2);
		break;
	case 18:
		g_Help.CheckLevel(40,2);
		break;
	case 19:
		g_Help.CheckLevel(50,2);
		break;
	case 20:
		g_Help.CheckLevel(60,2);
		break;
	case 21:
		g_Help.CheckLevel(65,2);
		break;
	case 22:
		g_Help.CheckLevel(70,2);
		break;
	case 23:
		g_Help.CheckLevel(80,2);
		ng = 0;
		break;
	}


	ng++;*/
}

void InitGuideHelp()
{
	g_GuideHelp.SetDoing(TRUE);
	g_GuideHelp.SetStartPos(0, 0);
	g_GuideHelp.CheckLevel(sinChar->Level);
	//g_GuideHelp.CheckLevel(17);
	g_GuideHelp.SetHelpN(GUIDEHELP_LIST);
	g_GuideHelp.menuInit(GUIDEHELP_LIST);

	IsDrawGuideHelp = TRUE;
}



void HelpDraw()
{
	if (g_Help.GetHelpN() != -1)
	{
		switch (g_Help.GetHelpN())
		{
		case LEVELUP_HELP:
			g_Help.Draw(0);
			break;
		case LEVELUP_HELP_CONTENT:

			g_Help.Draw(0);
			break;
		}
	}
}

void CLAN_MENU::GuideHelpDraw()
{
	if (g_GuideHelp.GetHelpN() != -1)
	{
		switch (g_GuideHelp.GetHelpN())
		{
		case GUIDEHELP_LIST:
			DrawBox(NOTICE_X, NOTICE_Y - 50, 9, 9);
			g_GuideHelp.Draw();
			break;
		case GUIDEHELP_CONTENT:
			DrawBox(NOTICE_X, NOTICE_Y - 50, 9, 9);
			g_GuideHelp.Draw();
			break;

		}
	}
}
/*//========서버에서 웨이브 받기 세팅===================================================
int GoWavDown_Check = 0;

char szWavUrl[255] = {0,};
BOOL bWavError = FALSE;

//1.서버 url를 가지고 온다
void GetWaveUrl(char* url)
{
	if((url == NULL)||(url[0] == 0 ))
	{
		bWavError = TRUE;
		return;
	}

	ZeroMemory(szWavUrl,sizeof(szWavUrl));
	strcpy(szWavUrl,url);
	GoWavDown_Check = 1;
}

//2.메인에서 실행하는 함수
int DoWavDown_check(char* FileName)
{
	if(OpenFlag == 1) return -1;  //다른게 열려있으면 안하고 나중에 한다.

	GoWavDown_Check = 1;
	if(GoWavDown_Check == 1)
	{
		void DoWavDown(char* FileName);
		DoWavDown(FileName);
	}

	return 0;
}

void DoWavDown(char* FileName)
{
	C_menuN = -1;
	OpenFlag = 1;
	g_cNpcWav.SetNpcWavNum(NPCWAVE_CALL);
	g_cNpcWav.SetFileName(FileName);
}
*/
//=============겜들어가기직전에 나오는 경고 메세지=====================
int GoNotice_chking = 0;
int m_nNoticeCount = -1;
char szNoticeUrl[255] = { 0, };
//1. 셋팅함수
int GetNoticeUrl(char* url)
{
	GoNotice_chking = 0;
	ZeroMemory(szNoticeUrl, sizeof(szNoticeUrl));
	strcpy(szNoticeUrl, url);

	GoNotice_chking = 1;
	return 1;
}
//2. 메인에서 실행을 권하는 함수
void GoNotice_chk()
{
	if (OpenFlag == 1) return;  //다른게 열려있으면 안하고 나중에 한다.

	if (GoNotice_chking == 1) {
		if (m_nNoticeCount < NOTICECOUNT - 1)
		{
			GoNotice_chking = 0;
			++m_nNoticeCount;
			void GoNotice();
			GoNotice();
		}

	}
}

//3. 실행토록하는 함수
void GoNotice()
{
	C_menuN2 = 0;
	OpenFlag = 1;
	C_menuN = CLAN_NOTICE;

	//cClanNotice.SetNoticeCount(1);

}




/*==================================================================================
//
// 외부에서 사용되는 클랜의 구동 셋팅 함수들								   start
//
==================================================================================*/


/*///////////////////////////////////////////////////////////////////////////
: 클라스명 :
: 하 는 일 : 클랜 맨 처음 셋팅
///////////////////////////////////////////////////////////////////////////*/

//ktj : 클랜메뉴 인터페이스 init
//setN : 0 : 최초 셋팅  1: 두번째셋팅(모든 
void clanMENU_INIT(int setN)
{
}


//hoLogin.cpp에서 사용함.
int isDELETE_character(char* charName)
{
	if (!bCheckHostNameandPort) return 1;		//1은 캐릭터를 지워도된다는뜻


	int web_DB_state_FLAG_WAIT();
	while (1) {
		if (web_DB_state_FLAG_WAIT() == 1)
			break;
	}


	Interface_clanmenu.CLANmenu_clanPOSITION();
	cldata.myPosition = 0;//CLANNO_REGESTER;
	while (1) {
		web_DB_main();					//web db를 사용하는 클랜 메뉴에서만 불리워짐.	
		cIsClanMember.Main();			//ktj : 클랜원의 포지션 체크 메인

		if (cldata.myPosition != 0)
			break;
	}

	while (1) {
		if (web_DB_state_FLAG_WAIT() == 1)
			break;
	}

	/*
	#define CLANUSER					100		//일반유저
	#define CLANCHIP					101		//칲
	#define CLANNO_REGESTER				102		//무등록자
	#define CLAN_NOCONNECT				103		//컨넥트가 안되서 설정불가
	*/
	return cldata.myPosition;
}



void CLAN_MENU::Init2() {}

//notice메시지로드용의 Init2()에 상응하는 함수
void CLAN_MENU::NoticeMessage_Init2() {}




void CLAN_MENU::CLANmenu_INIT()
{
}


/*///////////////////////////////////////////////////////////////////////////
: 클라스명 :
: 하 는 일 : 계정에 만들어진 캐릭터들의 클랜정보 알아오기
///////////////////////////////////////////////////////////////////////////*/
BOOL bGetChaClan = FALSE; //계정에 관련된 캐릭터들의 클랜정보를 읽어오는지 체크
int  nGetChaClan = 0;   //0: 진행안됨 1: 진행되고있음 2: 진행완료  3: 에러
int nStatusCount = 0;


int cntsu = 0;
char* szSelectCha[5][2];


void InitSelectCha()
{
	if (!bCheckHostNameandPort) return;

	nStatusCount = 0;
	ZeroMemory(chacldata, sizeof(chacldata));
	ZeroMemory(szSelectCha, sizeof(szSelectCha));
	cntsu = 0;
}

void ClearSelectCha()
{
	if (!bCheckHostNameandPort) return;
	bGetChaClan = FALSE;
	ZeroMemory(szSelectCha, sizeof(szSelectCha));
	ZeroMemory(chacldata, sizeof(chacldata));
	ZeroMemory(&cldata, sizeof(CLANDATA));
	nStatusCount = 0;

}

//캐릭터들의 이름을 입력
int HONG_GetChaClanName(char* id, char* chaname)
{
	if (!bCheckHostNameandPort) return 0;


	int i;
	int nReturn = 0;;
	for (i = 0; i < 5; i++)
	{
		if (szSelectCha[i][0] == NULL)
		{
			szSelectCha[i][0] = id;
			szSelectCha[i][1] = chaname;
			nReturn = 1;

			break;
		}
	}

	cntsu = i + 1;

	if (i == 5) return -1;

	return nReturn;
}




int  main_chaC()
{
	return 0;
}


int GetChaClanName(char* id, char* chaname, char* gserver)
{
	return 1;
}









/*///////////////////////////////////////////////////////////////////////////
: 클라스명 :
: 하 는 일 : 클랜원으로서의 위치를 알아냄.
///////////////////////////////////////////////////////////////////////////*/

#ifdef VOICE_CHAT
BOOL bCheckClan = FALSE;
#endif
int CLAN_MENU::CLANmenu_clanPOSITION()
{

	if (!bCheckHostNameandPort) {
		cldata.myPosition = CLAN_NOCONNECT;
		return 0;
	}

	if (OpenFlag == 1) return 0;		//이 메뉴가 닫혀있을때만 가능함.

	C_menuN2 = 0;
	OpenFlag = 1;
	joinAnswer = 0;
	C_menuN = CLAN_isClanWon;
	menuInit(C_menuN);
	ClanCharMove = 1;			//ktj : 마우스가 첵크하는 박스첵크 

#ifdef VOICE_CHAT	
	bCheckClan = TRUE;
#endif
	return 1;
}


/*///////////////////////////////////////////////////////////////////////////
: 클라스명 :
: 하 는 일 : NPC와 만났을때
///////////////////////////////////////////////////////////////////////////*/
void CLANmenu_meetNPC()	//newplay.cpp에서 사용함.
{
	Interface_clanmenu.CLANmenu_meetNPC();
}
int CLAN_MENU::CLANmenu_meetNPC()
{
	if (OpenFlag == 1) {
		if (C_menuN == CLAN_DISPLAY_INFO)
		{
			C_menuN = CLAN_CLOSE_END_BEFORE;

		}
		return 0;		//이 메뉴가 닫혀있을때만 가능함.
	}


	C_menuN2 = 0;
	OpenFlag = 1;
	joinAnswer = 0;
	if (!bCheckHostNameandPort)
	{
		C_menuN = CLANSERVER_INSPECT_BEFORE;

	}
	else
	{
		C_menuN = CLAN_GO_NPCSTART;
	}


	//menuInit(C_menuN );

	ClanCharMove = 1;			//ktj : 마우스가 첵크하는 박스첵크 
	return 1;
}






/*///////////////////////////////////////////////////////////////////////////
: 클라스명 :
: 하 는 일 : 클랜원들의 채팅리스트를 볼때
///////////////////////////////////////////////////////////////////////////*/
void CLANmenu_ChatList(int sw)	//newplay.cpp에서 사용함.l
{
	Interface_clanmenu.CLANmenu_ChatList(sw);
}
void CLAN_MENU::CLANmenu_ChatList(int sw)
{

	if (!bCheckHostNameandPort) return;
	if (OpenFlag == 1) return;
	if (bOpenChatList) return;

	ChatList_CmenuN2 = 0;

	switch (sw)
	{
	case 0: //스크롤 되게

		cClanChatting.InitClanChatList();
		if (bIsReadData) ChatList_CmenuN = CLAN_DISPLAY_OPEN;
		else	ChatList_CmenuN = CHATLIST_READDATA;

		cClanChatting.menuInit(ChatList_CmenuN);

		break;
	case 1: //스크롤 안되게
		cClanChatting.NoScroll();
		//if(bIsReadData) ChatList_CmenuN = CLAN_DISPLAY_OPEN;
		//else	ChatList_CmenuN = CHATLIST_READDATA;		

		ChatList_CmenuN = CLAN_DISPLAY_INFO;
		cClanChatting.menuInit(ChatList_CmenuN);

#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우

#else
		OpenFlag = 1;
		C_menuN = ChatList_CmenuN;

#endif


		break;
	}
}




void CLANmenu_ChatList_Main()	//newplay.cpp에서 사용함.l
{
	cClanChatting.Main();			//클랜 채팅리스트
}
void CLANmenu_ChatList_Draw()	//newplay.cpp에서 사용함.l
{
	cClanChatting.Draw();			//클랜 채팅리스트
}


/*///////////////////////////////////////////////////////////////////////////
: 클라스명 :
: 하 는 일 : cldata업데이트 할때
///////////////////////////////////////////////////////////////////////////*/
void Updatecldata()
{
	//Interface_clanmenu.Updatecldata();
	g_UpdateCldata.Updatecldata();
}


/*///////////////////////////////////////////////////////////////////////////
: 클라스명 : 권유하는 메뉴들.
: 하 는 일 : 일반인에게 클랜의 가입을 권유하면 권유한다는 메뉴가 뜬다.
///////////////////////////////////////////////////////////////////////////*/

//권유받는 사람이 사용하는 함수
void ClanJoin(char* oneClanName, char* oneChipCharid)
{
	//Interface_clanmenu.SetJoinData("울트라","헌트");
	Interface_clanmenu.SetJoinData(oneClanName, oneChipCharid);
	Interface_clanmenu.ClanJoin();
}

//ktj : 권유받은자가 리턴해주는 값(이함수를 사용해 리턴해준다.)
//0:취소 1:OK
int JoinAnswer()
{
	return cClanJoin.nJoinAnswer;
}

void CLAN_MENU::ClanJoin()
{
	if (OpenFlag == 1) return;
	C_menuN2 = 0;
	OpenFlag = 1;
	ClanCLCharMove = 1;			//ktj : 마우스가 첵크하는 박스첵크 	
	C_menuN = CLAN_JOIN_before;

}

/*///////////////////////////////////////////////////////////////////////////
: 클라스명 :권유하는사람이 사용
: 하 는 일 : 클랜장이 클랜원을 등록시킬때
///////////////////////////////////////////////////////////////////////////*/

//===================================
//ktj:사용안함.
//====================================
void joinClanMember()
{
	Interface_clanmenu.joinClanMember();
}

void CLAN_MENU::joinClanMember()
{
	if (OpenFlag == 1) return;


	C_menuN2 = 0;
	OpenFlag = 1;
	ClanCLCharMove = 1;			//ktj : 마우스가 첵크하는 박스첵크 	
	C_menuN = CLAN_JOIN_chip;
}

//ygy: 캐릭터 삭제 임시
void CLANmenu_DeleteCha(char* id, char* gserver, char* chaname)
{
	cClanDeleteCha.SetDeleteChaInfo(id, gserver, chaname);
	Interface_clanmenu.CLANmenu_DeleteCha();

	cClanDeleteCha.bFlag = FALSE;
}


/*///////////////////////////////////////////////////////////////////////////
: 클라스명 :
: 하 는 일 : 캐릭터 삭제시
///////////////////////////////////////////////////////////////////////////*/
void ktj_IMSI_DELETE()
{
	//extern char UserAccount[256];		//id가 들어있는 버퍼
	//extern char szConnServerName[16];	//접속한 서버이름이 들어있는 버퍼
	//CLANmenu_DeleteCha_SetCharacter(UserAccount, szConnServerName, "허걱덕");
}



//ktj :  1. 캐릭 지우기전에 셋팅하는 함수
void CLANmenu_DeleteCha_SetCharacter(char* id, char* gserver, char* chaname)
{
	cClanDeleteCha.SetDeleteChaInfo(id, gserver, chaname);

	//ktj : 2. 지우는 메뉴를 셋팅한다.
	Interface_clanmenu.CLANmenu_DeleteCha();
}

//ktj : 캐릭 지우기전에 불리우는 함수
void CLAN_MENU::CLANmenu_DeleteCha()
{
	C_menuN2 = 0;
	OpenFlag = 1;
	ClanCLCharMove = 1;			//ktj : 마우스가 첵크하는 박스첵크 	
	C_menuN = CLAN_DELETECHA_START;

	cClanDeleteCha.SetHowDeleteChaFlag();
}

//ktj : 3. 상태 플래그를 보고 2이면 캐릭을 삭제하고 3이면 삭제 못함.(0,1이면 2,3될때까지 기다린다)
int CLANmenu_DeleteCha_flag()
{
	//삭제전 : 0
	//삭제중 : 1
	//삭제 ok : 2
	//삭제 안됨 : 3
	return cClanDeleteCha.GetHowDeleteChaFlag();
}



//////////////////////////////////////////////////////////////////////////////////
//클랜 정보 갖어오기
//
/////////////////////////////////////////////////////////////////////////////////


int g_ClanCount = -1;
int ClanMark[10] = { 0,0,0,0,0,0,0,0,0,0 };

//============= 디버깅용 클랜인포 데이타세이브
int ReadClanInfo(DWORD dwClanNum)
{
	//디버깅용으로 그리기 위한것
	g_ClanCount++;
	if (g_ClanCount > 9) g_ClanCount = 0;
	ClanMark[g_ClanCount] = dwClanNum;
	//============================

	/*
	if(OpenFlag==1) {
		return -1;		//이 메뉴가 닫혀있을때만 가능함.
	}
	*/

	g_ViewClanInfo.SearchClan(dwClanNum);
	return ClanInfo_nNum;		//0~99;
}


// 현재 16*16 으로 된 그림만 로드되있으나 32*32가 필요한 경우 부른다.
// 32*32가 메모리를 많이 차지해서 이렇게 따로 필요한 극소수의 경우에만 부른다.
int ReadClanInfo_32X32(DWORD dwClanNum)
{
	int claninfo_n = ReadClanInfo(dwClanNum);
	if (claninfo_n >= 0) {
		int clanINFO_Load32X32(int claninfo_n);
		clanINFO_Load32X32(claninfo_n);
	}
	return claninfo_n;
}







//==================================================================================
// 외부에서 사용되는 클랜의 구동 셋팅 함수들								    end
//=================================================================================







/*==================================================================================
//
// 클랜메뉴																	   start
//
==================================================================================*/
CLAN_MENU::CLAN_MENU()
{
	nClanWon_Num = 0;
	nScl_Bar = 0;           //스크롤바
	g_nClanIconNum;			//헬프아이콘 총수
	g_HelpIconNumber = 0;
	g_HelpDisPlay = 0;

}

CLAN_MENU::~CLAN_MENU()
{
}


//마우스위치를 첵크안토록하는것 hoLinkHeader.cpp에서 사용
POINT clanBoxPos;
POINT clanCLBoxPos;
void CLAN_MENU::Init()
{
	OpenFlag = 0;

	C_menuN = 0;
	C_menuN2 = 0;



	//BackStartPos.x = 70;
	//BackStartPos.y = 94;

	//BackStartPos.x = 100;
	//BackStartPos.y = 94;

	BackStartPos.x = 101;
	BackStartPos.y = 114;


	//클랜메뉴에서 월드쪽에 마우스가 움직임이 전달안되도록한다.
	clanBoxPos.x = BackStartPos.x;
	clanBoxPos.y = BackStartPos.y;

	//클랜채팅 리스트메뉴에서 월드쪽에 마우스가 움직임이 전달안되도록한다.
	clanCLBoxPos.x = 540 + 60;		//60은 그림이 오른편에 붙어있기 때문임
	clanCLBoxPos.y = CHATLIST_BOX_Y;




	ZeroMemory(szRemoveClanWon, sizeof(char) * 500);
	ZeroMemory(ClanHelpIcon, sizeof(CLANHELPICON) * CLANHELPICON_NUM);
	ZeroMemory(cldata.szJoinClanName, sizeof(char) * 64);   //조인 클랜 이름
	ZeroMemory(cldata.ChaName, sizeof(char) * 64);  //조인하는 유저 캐릭터 이름
	ZeroMemory(cldata.szJoinClanChipName, sizeof(char) * 64);   //조인하는 클랜장 이름
	ZeroMemory(szReconfReleaseMsg, sizeof(char) * 2048);


	nClanWon_Num = 0;			//클랜원파싱후 클랜원의 명수(클랜장제외)
	nReleaseClanWon = 0;		//삭제 클랜원
	//membmp = new MENUBMP;

	//hMark = NULL;
#ifndef USE_PROSTONTALE		//=========== 포리스턴소스인경우
	int LoadClanMessage();
	LoadClanMessage();
#endif

	Load();
	cClanChatting.Load();

	//ktj : 메뉴 관련 클라스 셋팅 ===================

	cBaseMenu.Load();


	cIsClanMember.Init(1);
	cIsClanMember.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cIsClanMember);		//리소스 카피

	cClanChip.Init(1);
	cClanChip.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanChip);		//리소스 카피
	cBaseMenu.SetBase(&cClanChip);

	cClanUser.Init(1);
	cClanUser.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanUser);		//리소스 카피
	cBaseMenu.SetBase(&cClanUser);

	cClanMake.Init(1);
	cClanMake.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanMake);		//리소스 카피

	cClanReport.Init(1);
	cClanReport.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanReport);		//리소스 카피


	cClanChatting.Init(1);
	cClanChatting.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanChatting);		//리소스 카피
	cBaseMenu.SetBase(&cClanChatting);


	cClanDeleteCha.Init(1);
	cClanDeleteCha.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanDeleteCha);		//리소스 카피

	cClanJoin.Init(1);
	cClanJoin.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanJoin);		//리소스 카피	


	cClanNotice.Init(1);
	cClanNotice.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanNotice);		//리소스 카피	

	//도움말 초기화
	//g_Help.Init(1);
	//g_Help.setParent(&cBaseMenu);
	//cBaseMenu.copy_LoadResource(&g_Help);		//리소스 카피	

	//guie 초기화
	g_GuideHelp.Init(1);
	g_GuideHelp.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&g_GuideHelp);		//리소스 카피	
	g_GuideHelp.InitLevelHelpLabel();

}


//notice메시지는 따로 셋팅되며 Init()과 동시에 쎗팅할수없음.
//클랜 서버가 죽어 있을 경우 세팅
void CLAN_MENU::NoticeMessage_Init()
{
	BackStartPos.x = 101;
	BackStartPos.y = 114;

	cBaseMenu.Load();

	cClanNotice.Init(1);
	cClanNotice.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanNotice);		//리소스 카피	

	g_Help.Load();
	g_GuideHelp.Load();

	g_GuideHelp.Init(1);
	g_GuideHelp.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&g_GuideHelp);		//리소스 카피	
	g_GuideHelp.InitLevelHelpLabel();

}


/*///////////////////////////////////////////////////////////////////////////
: 함수명   : menuInit
: 하 는 일 : 메뉴 총괄 Init
: 반 환 값 :
: 파라미터 :
///////////////////////////////////////////////////////////////////////////*/
void CLAN_MENU::menuInit(int num)
{

	cIsClanMember.menuInit(num);		//클랜만들기 메뉴 initialize
	cClanMake.menuInit(num);		//클랜만들기 메뉴 initialize
	cClanChip.menuInit(num);		//클렌칲 메뉴 initialize
	cClanUser.menuInit(num);		//클랜유저 메뉴 initialize
	cClanChatting.menuInit(num);
	cClanDeleteCha.menuInit(num);
	cClanJoin.menuInit(num);
}







/*///////////////////////////////////////////////////////////////////////////
: 클라스 명:
: 하 는 일 : 마우스로 누른 메뉴 번호 첵크함수.
: 반 환 값 :
:파라미터  :
///////////////////////////////////////////////////////////////////////////*/
int CLAN_MENU::chkMenu(int x, int y, RECT* r)
{
	if (r->left <= x && x <= r->right)
		if (r->top <= y && y <= r->bottom)
			return 1;
	return 0;			//아무것도 선택안함.
}
int CLAN_MENU::chkeckMenuN()
{

	return 1;
}





//칲이 권유하면 권유받은 사람한테 뜨는 메뉴임.(현재 netplay.cpp에서 사용중)
void menu_joinOpen(char* chipCharID)
{
	//Interface_clanmenu.menu_joinOpen(chipCharID);
}


//조인메뉴는 클랜칲이 권유하면 권유받은 사람한테 메뉴가 뜬다.
int CLAN_MENU::menu_joinOpen(char* chipCharID)		//칲의 캐릭터 id
{
	/*
	if(OpenFlag==1) return 0;					//이 메뉴가 닫혀있을때만 가능함.
	if( cldata.myPosition == 100 ||				//100(클랜원), 101:클랜칲에게 권유할수 없다.
		cldata.myPosition == 101)				return 0;

	strcpy(cldata.chipCharID, chipCharID);
	C_menuN   = CLAN_JOIN_before;		//조인에 적합한지 여부.
	C_menuN2  = 0;
	OpenFlag  = 1;
	joinAnswer= 0;

	//ktj
	ClanCharMove = 1;
	*/

	return 1;

}





void menu_joinOpen_Chip(int isOK, char* clanWonID, char* clanWonCharID)
{
	Interface_clanmenu.menu_joinOpen_Chip(isOK, clanWonID, clanWonCharID, 0, 0);		//칲의 캐릭터 id;
}
void menu_joinOpen_Chip(int isOK, char* clanWonID, char* clanWonCharID, int clanWonJob, int clanWonLevel)
{
	Interface_clanmenu.menu_joinOpen_Chip(isOK, clanWonID, clanWonCharID, clanWonJob, clanWonLevel);		//칲의 캐릭터 id;
}

//호동Clan추가
void g_IsCheckClanMember(smCHAR* pChar)
{
	Interface_clanmenu.IsCheckClanMember(pChar);
}

int CLAN_MENU::IsCheckClanMember(smCHAR* pChar)
{
	if (OpenFlag == 1)
		return 0;

	C_menuN = CLAN_IS_CHECK_MEMBER;	//클랜에 가입한지 4일이 지났는지 검사를 한다.
	cldata.TargetChar = NULL;
	if (pChar)
		cldata.TargetChar = pChar;

	//strcpy(cldata.ChaName, pChar->smCharInfo.szName);

	C_menuN2 = 0;
	OpenFlag = 1;
	joinAnswer = 0;
	ClanCharMove = 1;

	return 0;
}



//======================================================================
//권유하는 메뉴를 누르면 이함수를 불러 리턴이 1이면 권유하는 데이타를 보내고 아니면 말것.
// 0 이면 더이상 클랜원을 추가 할수 없음(자체메뉴에서 등록시킬수없다고 나온다)
BOOL CheckClanWon(int level);		//

//======================================================================


//권유받은 사람이 OK하면 그명령이 클랜칲에게 되돌아와 클랜칲이 가입시키는 메뉴임.
//주의 : 화면상에는 메뉴가 뜨지않음.
int CLAN_MENU::menu_joinOpen_Chip(int joinOK, char* clanWonID, char* clanWonCharID, int clanWonJob, int clanWonLevel)		//칲의 캐릭터 id
{
	if (OpenFlag == 1) return 0;		//이 메뉴가 닫혀있을때만 가능함.


#ifdef __CLANSUBCHIP__
	//if(cldata.myPosition != 101 && cldata.myPosition != 104 ) return 0;	//101:클랜칲,부클랜칲만 사용가능한 메뉴임.
	if (cldata.myPosition == 101 || cldata.myPosition == 104) goto jmp1;	//101:클랜칲,부클랜칲만 사용가능한 메뉴임.
	return 0;
jmp1:
#else
	if (cldata.myPosition != 101) return 0;	//101:클랜칲만 사용가능한 메뉴임.
#endif




#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
	int chk = CheckClanWon(sinChar->Level);
	if (chk == 0) {			//클랜원을 추가할수 없습니다.
		return 0;
	}
#endif

	/*
	if(joinOK==0) {
		C_menuN= CLAN_JOIN_chip_NO	;		//클랜에 가입하기를 거부함.
		C_menuN2= -1;
	}
	else {
		*/
	C_menuN = CLAN_JOIN_chip;	//클랜에 등록시킨다.
	strcpy(cldata.clanJoinID, clanWonID);
	strcpy(cldata.clanJoinCharID, clanWonCharID);
	cldata.clanJoinJob = clanWonJob;
	cldata.clanJoinLevel = clanWonLevel;


	C_menuN2 = 0;
	//}
	OpenFlag = 1;
	joinAnswer = 0;

	//ktj
	ClanCharMove = 1;


	return 1;
}








//int chkN11 = -1;
//int chkN22 = -1;

extern BOOL bip_port_error;
int CLAN_MENU::Main(int sw) { return TRUE; }




char loadis[256] = { 0, };
char loadis2[256] = { 0, };
char updateis[256] = { 0, };





//이게 1이면 인포를 드로인한다.
int ktj_imsiDRAWinfo = 0;
//========================================================================나중에 지워야함에 주의

int CLAN_MENU::Draw() { return TRUE; }


//sw==0 : 일반 sw==1: 키입력을 받는경우 sw==2: 클랜이름을 타이틀에 쓸경우사용(그림자처리)
void CLAN_MENU::Draw_C_Text(char* str, int x, int y, int sw = 0) {}






//ycy : 박스 그리기
int g_nTotalWidth = 0;
int CLAN_MENU::DrawBox(int x, int y, int x_width, int y_height) { return TRUE; }

void CLAN_MENU::RenderMenu() {}
/*///////////////////////////////////////////////////////////////////////////
: 클라스 명: class CLAN_MENU
: 하 는 일 :
: 서    식 :
: 인    자 :
: 반 환 값 :
: 논    평 : 웹db를 사용해 데이타를 뽑아내야 할경우 본 클라스를 이용한다.
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
void CLAN_MENU::Draw_C_menuN() {}


//=============드로우 관련 함수 ==================================================================end







/*///////////////////////////////////////////////////////////////////////////
: 클라스 명: 계정에 만들어진 캐릭터들이 가입한 클랜을 알기 위해서
: 하 는 일 :
: 반 환 값 :
///////////////////////////////////////////////////////////////////////////*/
//유저 id, 캐릭터 id, 군서버 이름.
void SetUserData2(char* id, char* chaname, char* gserver)
{
	strcpy(cldata.szID, id);
	strcpy(cldata.ChaName, chaname);
	strcpy(cldata.gserver, gserver);

}

/*///////////////////////////////////////////////////////////////////////////
: 클라스 명: 클랜 스트럭처 데이타 셋팅 함수들
: 하 는 일 :
: 반 환 값 :
///////////////////////////////////////////////////////////////////////////*/
//유저 id, 캐릭터 id, 군서버 이름.
void CLAN_MENU::SetUserData(char* id, char* chaname, char* gserver, int money, int ability)
{
	strcpy(cldata.szID, id);
	strcpy(cldata.ChaName, chaname);
	strcpy(cldata.gserver, gserver);

	cldata.money = money;
	//ktj : 임시
	//if(cldata.money ==0) cldata.money = 100000;
	cldata.ability = ability;
}



//권유를 받는 사람한테오는 데이타
void CLAN_MENU::SetJoinData(char* joinclanname, char* joinclanchip)
{
	strcpy(cldata.szJoinClanName, joinclanname);			//가입될 클랜 이름
	strcpy(cldata.szJoinClanChipName, joinclanchip);		//가입될 클랜의 클랜장 id
}











#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우

void DrawSprite_TJB(int winX, int winY, int winxlen, int winylen, void* pdds, int x, int y, int width, int height, int Bltflag) {}
#endif



void Text_Out1(HDC hdc, int x, int y, char* str)
{
}

void Text_Out2(HDC hdc, int x, int y, char* str)
{
}

void Text_Out3(HDC hdc, int x, int y, char* str)
{
}
void Text_Out(HDC hdc, int x, int y, char* str) {}



void CLAN_MENU::Load() {}



//simMain.cpp에서 불러서 클로즈 한다.
void clanMENU_close()
{
	Interface_clanmenu.Close();
}

void CLAN_MENU::Close() {}

void CLAN_MENU::Loading() {}


#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
//클랜원 정보 전송용 함수
int SendClanCommandUser(smWINSOCK* lpsmSock, DWORD	dwCode, int Param1, int Param2, CLANWONLIST* lpClanUserList, int ClanUserCount);
#endif


void SetClanWon()
{
}





//savedata\\clanDATA 만들기==============================================start
BOOL SearchDirectory(char* dir)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	char t[50];
	wsprintf(t, "%s\\*.*", dir);
	hFind = FindFirstFile(t, &wfd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	return TRUE;
}

void MakeDirectory(char* dir)
{
	CreateDirectory(dir, NULL);
}


void CreateSaveData_dir()
{
	char szTemp[100];
	if (!SearchDirectory(SAVEDATA_DIR)) MakeDirectory(SAVEDATA_DIR);
	if (!SearchDirectory(CLANDATA_DIR)) MakeDirectory(CLANDATA_DIR);

	ZeroMemory(szTemp, sizeof(szTemp));
	wsprintf(szTemp, "%s", CLANDATA_DIR);
	wsprintf(szTemp, "%s\\%s", szTemp, szConnServerName);

	if (!SearchDirectory(szTemp)) MakeDirectory(szTemp);

}



//savedata\\clanDATA 만들기==============================================end

//npcwav 디렉토리 만들기=========================start

/*void CreateNPCWav_dir()
{
	if(!SearchDirectory(NPCWAV_DIR))
	{
		MakeDirectory(NPCWAV_DIR);
	}
}
*/


//클랜 메세지 출력하기 ==========================================================================start
#define CLANMESSAGE_DIR "image/clanImage/Help/clanText.msg"//msg"

int LoadClanMessage()
{
	char* g_ClanMessage;


	HANDLE hFile = NULL;
	DWORD dwFileSize;
	DWORD dwRead = 0;
	char c;
	c = '\n';
	hFile = CreateFile(CLANMESSAGE_DIR, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == NULL) return -1;

	dwFileSize = GetFileSize(hFile, NULL);

	g_ClanMessage = new char[dwFileSize];

	ReadFile(hFile, g_ClanMessage, dwFileSize, &dwRead, NULL);
	CloseHandle(hFile);

	int ReadMessage(char* save, char* savename, char* readbuf);
	//void CheckMsgPos(CLANMSG* clanmsg,int width, int height);
	ReadMessage(clanMake1.szMsg, "szMsg1", g_ClanMessage);
	CheckMsgPos(&clanMake1, BOX_MAX_WIDTH, 0);
	ReadMessage(clanMake2.szMsg, "szMsg2", g_ClanMessage);
	CheckMsgPos(&clanMake2, BOX_MAX_WIDTH, 0);
	ReadMessage(clanMake4.szMsg, "szMsg4", g_ClanMessage);
	CheckMsgPos(&clanMake4, BOX_MAX_WIDTH, 0);
	ReadMessage(clanMake5.szMsg, "szMsg5", g_ClanMessage);
	CheckMsgPos(&clanMake5, BOX_MAX_WIDTH, 0);
	ReadMessage(clanMake6.szMsg, "szClanMakeFinishMsg", g_ClanMessage);
	CheckMsgPos(&clanMake6, BOX_MAX_WIDTH, 0);


	ReadMessage(szMsg3, "szMsg3", g_ClanMessage);

	ReadMessage(clanMsg21.szMsg, "szMsg21", g_ClanMessage);
	CheckMsgPos(&clanMsg21, BOX_MAX_WIDTH, 0);

	ReadMessage(clanMsg254.szMsg, "szMsg254", g_ClanMessage);  //CLAN_NotMake
	CheckMsgPos(&clanMsg254, BOX_MAX_WIDTH, 0);
	ReadMessage(clanMsg15.szMsg, "szMsg15", g_ClanMessage);
	CheckMsgPos(&clanMsg15, BOX_MAX_WIDTH, 0);	  //CLAN_RELEASE
	ReadMessage(clanJoin.szMsg, "szJoin", g_ClanMessage); //CLAN_JOIN
	CheckMsgPos(&clanJoin, BOX_MAX_WIDTH, 0);
	ReadMessage(clanSecession.szMsg, "szSecession", g_ClanMessage); //CLAN_SECESSION
	CheckMsgPos(&clanSecession, BOX_MAX_WIDTH, 0);
	ReadMessage(clanServerConnectErrorMsg.szMsg, "szServerConnectErrorMsg", g_ClanMessage);
	CheckMsgPos(&clanServerConnectErrorMsg, BOX_MAX_WIDTH, 0);            //ISAO_ERR_TCP_CONNECT
	ReadMessage(clanszError_Msg.szMsg, "szError_Msg", g_ClanMessage); //ISAO_ERR_TCP_CONNECT
	CheckMsgPos(&clanszError_Msg, BOX_MAX_WIDTH, 0);
	ReadMessage(clanCancelMakeClan.szMsg, "szCancelMakeClan", g_ClanMessage);
	CheckMsgPos(&clanCancelMakeClan, BOX_MAX_WIDTH, 0);
	ReadMessage(clanClanNameError.szMsg, "szClanNameError", g_ClanMessage);
	CheckMsgPos(&clanClanNameError, BOX_MAX_WIDTH, 0);


	//ReadMessage(szClanMakeFinishMsg,"szClanMakeFinishMsg",g_ClanMessage);
	//ReadMessage(szMsg254,"szMsg254",g_ClanMessage);
	//ReadMessage(szMsg15,"szMsg15",g_ClanMessage);
	//ReadMessage(szMark1,"szMark1",g_ClanMessage);
	//ReadMessage(szMark2,"szMark2",g_ClanMessage);
	//ReadMessage(szMark3,"szMark3",g_ClanMessage);
	//ReadMessage(szJoin,"szJoin",g_ClanMessage);
	//ReadMessage(szSecession,"szSecession",g_ClanMessage);

	//ReadMessage(szMsg6,"szMsg6",g_ClanMessage);
	//ReadMessage(szServerConnectErrorMsg,"szServerConnectErrorMsg",g_ClanMessage);
	//ReadMessage(szReadClanMarkErrorMsg,"szReadClanMarkErrorMsg",g_ClanMessage);
	//ReadMessage(szCancelMakeClan,"szCancelMakeClan",g_ClanMessage);
	//ReadMessage(szTranzationError,"szTranzationError",g_ClanMessage);
	//ReadMessage(szInsertClanWonError1,"szInsertClanWonError1",g_ClanMessage);
	//ReadMessage(szClanNameError,"szClanNameError",g_ClanMessage);
	//ReadMessage(sz100Error,"sz100Error",g_ClanMessage);
	//ReadMessage(szDefaultError,"szDefaultError",g_ClanMessage);
	//ReadMessage(szReleseError,"szReleseError",g_ClanMessage);
	//ReadMessage(szSecessionSuccess,"szSecessionSuccess",g_ClanMessage);
	//ReadMessage(szSecession6DownMsg,"szSecession6DownMsg",g_ClanMessage);clanSecession6DownMsg
	ReadMessage(clanSecession6DownMsg.szMsg, "szSecession6DownMsg", g_ClanMessage);  //CLAN_DELETECHA_SECESSION
	CheckMsgPos(&clanSecession6DownMsg, BOX_MAX_WIDTH, 0);
	//ReadMessage(szSecessionChipMsg,"szSecessionChipMsg",g_ClanMessage);
	//ReadMessage(szSecession2ErrorMsg,"szSecession2ErrorMsg",g_ClanMessage);
	//ReadMessage(szClanJoinMsg,"szClanJoinMsg",g_ClanMessage);
	ReadMessage(clanClanJoinMsg.szMsg, "szClanJoinMsg", g_ClanMessage);
	CheckMsgPos(&clanClanJoinMsg, BOX_MAX_WIDTH, 0);

	//ReadMessage(szClanJoinError2Msg,"szClanJoinError2Msg",g_ClanMessage);
	//ReadMessage(szClanJoinError3Msg,"szClanJoinError3Msg",g_ClanMessage);
	//ReadMessage(szClanJoinError4Msg,"szClanJoinError4Msg",g_ClanMessage);
	//ReadMessage(szClanJoinCancelMsg,"szClanJoinCancelMsg",g_ClanMessage);CLANMSG clanClanJoinCancelMsg;
	ReadMessage(clanClanJoinCancelMsg.szMsg, "szClanJoinCancelMsg", g_ClanMessage);
	CheckMsgPos(&clanClanJoinCancelMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanClanJoinsucesssMsg.szMsg, "szClanJoinsucesssMsg", g_ClanMessage);
	CheckMsgPos(&clanClanJoinsucesssMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanszReMoveClanWonMsg.szMsg, "szReMoveClanWonMsg", g_ClanMessage);
	CheckMsgPos(&clanszReMoveClanWonMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanReMoveHeaderMsg.szMsg, "szReMoveHeaderMsg", g_ClanMessage);
	CheckMsgPos(&clanReMoveHeaderMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanszReConfReleaseMsg.szMsg, "szReConfReleaseMsg", g_ClanMessage);
	CheckMsgPos(&clanszReConfReleaseMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanNoRecordReleaseMsg.szMsg, "szNoRecordReleaseMsg", g_ClanMessage);
	CheckMsgPos(&clanNoRecordReleaseMsg, BOX_MAX_WIDTH, 0);

	//ReadMessage(szFinishReleaseMsg,"szFinishReleaseMsg",g_ClanMessage);clanFinishReleaseMsg
	ReadMessage(clanFinishReleaseMsg.szMsg, "szFinishReleaseMsg", g_ClanMessage);
	CheckMsgPos(&clanFinishReleaseMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanSecessionMsg.szMsg, "szSecessionMsg", g_ClanMessage);
	CheckMsgPos(&clanSecessionMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanFinishSecessionMsg.szMsg, "szFinishSecessionMsg", g_ClanMessage);
	CheckMsgPos(&clanFinishSecessionMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanisCheckClanJangErrorMsg.szMsg, "szisCheckClanJangErrorMsg", g_ClanMessage);
	CheckMsgPos(&clanisCheckClanJangErrorMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanMoneyErrorMsg.szMsg, "szMoneyErrorMsg", g_ClanMessage);
	CheckMsgPos(&clanMoneyErrorMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanAbilityErrorMsg.szMsg, "szAbilityErrorMsg", g_ClanMessage);
	CheckMsgPos(&clanAbilityErrorMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanszRemoveShortClanWonMsg.szMsg, "szRemoveShortClanWonMsg", g_ClanMessage);
	CheckMsgPos(&clanszRemoveShortClanWonMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanRemoveShowrtClanWonResultMsg.szMsg, "szRemoveShowrtClanWonResultMsg", g_ClanMessage);
	CheckMsgPos(&clanRemoveShowrtClanWonResultMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanNoName.szMsg, "szNoName", g_ClanMessage);
	CheckMsgPos(&clanNoName, BOX_MAX_WIDTH, 0);

	ReadMessage(clanLoading.szMsg, "szLoading", g_ClanMessage);
	CheckMsgPos(&clanLoading, BOX_MAX_WIDTH, 0);

	ReadMessage(clanMsg6.szMsg, "szMsg6", g_ClanMessage);
	CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);



	/////////캐릭터 삭제 메세지
	ReadMessage(szDeleteChaReleaseBefore, "szDeleteChaReleaseBefore", g_ClanMessage);
	ReadMessage(szDeleteChaReleaseFinish, "szDeleteChaReleaseFinish", g_ClanMessage);
	ReadMessage(szDeleteChaSecessionBefore, "szDeleteChaSecessionBefore", g_ClanMessage);
	ReadMessage(szDeleteChaSecessionFinish, "szDeleteChaSecessionFinish", g_ClanMessage);
	//ReadMessage(szDeleteChaError,"szDeleteChaError",g_ClanMessage);

	ReadMessage(clanDeleteChaError.szMsg, "szDeleteChaError", g_ClanMessage);
	CheckMsgPos(&clanDeleteChaError, BOX_MAX_WIDTH, 0);

	ReadMessage(clanMakeFinishClan.szMsg, "szMakeFinishClan", g_ClanMessage);
	CheckMsgPos(&clanMakeFinishClan, BOX_MAX_WIDTH, 0);


	ReadMessage(szClanWon6Down, "szClanWon6Down", g_ClanMessage);

	ReadMessage(clanClanUserNoMakeMsg.szMsg, "szClanUserNoMakeMsg", g_ClanMessage);
	CheckMsgPos(&clanClanUserNoMakeMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanMakeFinishClanWon.szMsg, "szMakeFinishClanWon", g_ClanMessage);
	CheckMsgPos(&clanMakeFinishClanWon, BOX_MAX_WIDTH, 0);

	ReadMessage(clanClanServerInspect.szMsg, "ClanServerInspect", g_ClanMessage);
	CheckMsgPos(&clanClanServerInspect, BOX_MAX_WIDTH, 0);

	ReadMessage(clanClanUserRelease.szMsg, "szClanUserRelease", g_ClanMessage);
	CheckMsgPos(&clanClanUserRelease, BOX_MAX_WIDTH, 0);


	//ReadMessage(Replay,"Replay",g_ClanMessage);

	ReadMessage(clanClanMakedowninfo.szMsg, "szClanMakedowninfo", g_ClanMessage);
	CheckMsgPos(&clanClanMakedowninfo, BOX_MAX_WIDTH, 0);

	ReadMessage(clanClanUserReleaseMsg.szMsg, "szClanUserReleaseMsg", g_ClanMessage);
	CheckMsgPos(&clanClanUserReleaseMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanClanUserRemove.szMsg, "szClanUserRemove", g_ClanMessage);
	CheckMsgPos(&clanClanUserRemove, BOX_MAX_WIDTH, 0);

	ReadMessage(clanClanWonOver.szMsg, "szClanWonOver", g_ClanMessage);
	CheckMsgPos(&clanClanWonOver, BOX_MAX_WIDTH, 0);

	//ReadMessage(szError_Msg,"szError_Msg",g_ClanMessage);
	ReadMessage(clanMsg7.szMsg, "szMsg7", g_ClanMessage);
	CheckMsgPos(&clanMsg7, BOX_MAX_WIDTH, 0);

	ReadMessage(clanNoLevel.szMsg, "szNoLevel", g_ClanMessage);
	CheckMsgPos(&clanNoLevel, BOX_MAX_WIDTH, 0);

	ReadMessage(clanRemoveSecond.szMsg, "szRemoveSecond", g_ClanMessage);
	CheckMsgPos(&clanRemoveSecond, BOX_MAX_WIDTH, 0);

	ReadMessage(clanRemoveSecondResult.szMsg, "szRemoveSecondResult", g_ClanMessage);
	CheckMsgPos(&clanRemoveSecondResult, BOX_MAX_WIDTH, 0);

	ReadMessage(LeaveClanConfirm.szMsg, "szLeaveClanConfirm", g_ClanMessage);
	CheckMsgPos(&LeaveClanConfirm, BOX_MAX_WIDTH, 0);

	ReadMessage(LeaveClanNoMoney.szMsg, "szLeaveClanNoMoney", g_ClanMessage);
	CheckMsgPos(&LeaveClanNoMoney, BOX_MAX_WIDTH, 0);

	ReadMessage(LeaveClanSuccess.szMsg, "szLeaveClanSuccess", g_ClanMessage);
	CheckMsgPos(&LeaveClanSuccess, BOX_MAX_WIDTH, 0);

	ReadMessage(LeaveClanNoLevel.szMsg, "szLeaveClanNoLevel", g_ClanMessage);
	CheckMsgPos(&LeaveClanNoLevel, BOX_MAX_WIDTH, 0);

	ReadMessage(LeaveClanReConfirm.szMsg, "szLeaveClanReConfirm", g_ClanMessage);
	CheckMsgPos(&LeaveClanReConfirm, BOX_MAX_WIDTH, 0);

	ReadMessage(NoLeaveClan.szMsg, "szNoLeaveClan", g_ClanMessage);
	CheckMsgPos(&NoLeaveClan, BOX_MAX_WIDTH, 0);

	ReadMessage(LeaveClanMessage.szMsg, "szLeaveClanMessage", g_ClanMessage);
	CheckMsgPos(&LeaveClanMessage, BOX_MAX_WIDTH, 0);

	ReadMessage(LeaveClanDownLevel.szMsg, "szLeaveClanDownLevel", g_ClanMessage);
	CheckMsgPos(&LeaveClanDownLevel, BOX_MAX_WIDTH, 0);

	ReadMessage(SubChipAppoint.szMsg, "szSubChipAppoint", g_ClanMessage);
	CheckMsgPos(&SubChipAppoint, BOX_MAX_WIDTH, 0);

	ReadMessage(SubChipAppointEnd.szMsg, "szSubChipAppointEnd", g_ClanMessage);
	CheckMsgPos(&SubChipAppointEnd, BOX_MAX_WIDTH, 0);

	ReadMessage(SubChipRelease.szMsg, "szSubChipRelease", g_ClanMessage);
	CheckMsgPos(&SubChipRelease, BOX_MAX_WIDTH, 0);

	ReadMessage(SubChipReleaseEnd.szMsg, "szSubChipReleseEnd", g_ClanMessage);
	CheckMsgPos(&SubChipReleaseEnd, BOX_MAX_WIDTH, 0);

	ReadMessage(NoSubChip.szMsg, "szNoSubChip", g_ClanMessage);
	CheckMsgPos(&NoSubChip, BOX_MAX_WIDTH, 0);


	//호동Clan추가
	ReadMessage(ClanIs4DayCheckMsg.szMsg, "szClanIs4DayCheckMsg", g_ClanMessage);
	CheckMsgPos(&ClanIs4DayCheckMsg, BOX_MAX_WIDTH, 0);






	delete[] g_ClanMessage;
	g_ClanMessage = NULL;
	return 1;
}

int ReadMessage(char* save, char* savename, char* readbuf)
{
	char szTemp[256];
	char* pBuf;
	char szReadData[1024];
	int nCount = 0;
	int nCount1 = 0;
	BOOL bFlag = FALSE;

	ZeroMemory(szReadData, sizeof(szReadData));
	wsprintf(szTemp, "*%s", savename);

	pBuf = strstr(readbuf, szTemp);
	if (pBuf == NULL)	//발견할 수 없었다.
	{
		return -1;
	}

	pBuf = pBuf + strlen(szTemp) + 2;
	nCount = (int)(pBuf - readbuf);

	if (readbuf[nCount] == 0x0a) {
		nCount++;
	}

	//DRZ_EDIT (prevent buffer overrun)
	while (nCount < (int)strlen(szReadData))
	{
		if ((readbuf[nCount] == 0x5c))
		{
			szReadData[nCount1] = '\n';
			nCount1++;
			nCount += 3;
			if (readbuf[nCount] == 0x0a)
			{
				nCount++;
				continue;
			}

		}
		if ((readbuf[nCount] == 0x0d) || (readbuf[nCount] == 0x0a)) break;
		szReadData[nCount1] = readbuf[nCount];
		nCount++;
		nCount1++;

	}

	strcpy(save, szReadData);
	return 1;
}



//xsu는 여러개의 x축중 최대길이임.
void MsgXsu_Ysu(CLANMSG* clanmsg, int* xsu, int* ysu)
{
	char strbuf[256], strbuf2[256];
	ZeroMemory(strbuf, sizeof(strbuf));

	int nCount = 0;
	int maxXsu = 0;
	int cntX = 0, cntY = 0;

	while (1)
	{
		strbuf[cntX] = clanmsg->szMsg[nCount];

		if (clanmsg->szMsg[nCount] == '\n')			//한줄의 끝
		{
			strbuf[cntX] = 0;
			if (cntX > maxXsu) {
				maxXsu = cntX;
				strcpy(strbuf2, strbuf);
			}

			cntY++;
			cntX = -1;
		}

		if (clanmsg->szMsg[nCount] == 0x00)//0x0d)			//끝
		{
			strbuf[cntX] = 0;
			if (cntX > maxXsu) {
				maxXsu = cntX;
				strcpy(strbuf2, strbuf);
			}
			cntY++;
			break;
		}


		cntX++;
		nCount += 1;
	}

	*xsu = maxXsu;
	*ysu = cntY;
}



void CheckMsgPos(CLANMSG* clanmsg, int width, int height)
{
	int nCount = 0;
	int nXMax = 0, nYMax;
	int nX = 0;
	//nX = (((4 * BOXCENTER_WIDTH) + (2*BOXCORNER_WIDTH) + 5) ); //261이 박스의 x길이
	nX = width; //261이 박스의 x길이
	nX = nX / 2;
	int nY = 40;

	MsgXsu_Ysu(clanmsg, &nXMax, &nYMax);
	int strDotlen = nXMax * 6;     //스트링 한바이트의 도트수는 6dot
	strDotlen /= 2;

	if (nYMax == 1) nY = 80;
	if (nYMax == 2) nY = 60;

	clanmsg->pos.x = (nX - strDotlen);
	clanmsg->pos.y = nY;
}



int g_nCount = 0;
void DebugMessage(char* msg, ...)
{
#define DEBUG_MSG_MAX 4096
	HANDLE hFile;
	char buff[DEBUG_MSG_MAX];
	char szDebugMsg[5120];
	DWORD dwWrite = 0;
	int nMsgStrOver = 0;
	SYSTEMTIME	SystemTime;


	va_list args;

	va_start(args, msg);
	vsprintf(buff, msg, args);
	va_end(args);



	//strcat( buff, "\r\n" );

	GetLocalTime(&SystemTime);

	hFile = CreateFile("ClanDebug.txt", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	wsprintf(szDebugMsg, "%d.%d년%d월%d일 : %s\r\n", g_nCount++,
		SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, buff);


	if (hFile != INVALID_HANDLE_VALUE) {
		SetFilePointer(hFile, 0, NULL, FILE_END);

		WriteFile(hFile, szDebugMsg, lstrlen(szDebugMsg), &dwWrite, NULL);
		if (lstrlen(buff) > DEBUG_MSG_MAX - 2)
		{
			SetFilePointer(hFile, 0, NULL, FILE_END);
			char szMsg[256] = "..\n메시지가 너무길어서 잘랐음";
			WriteFile(hFile, szMsg, lstrlen(szMsg), &dwWrite, NULL);
		}



	}


	if (hFile) CloseHandle(hFile);
}

//#define DEBUGOPTION  "image\\clanimage\\help\\clan.inf"
#define DEBUGOPTION  "DDD2.txt"
BOOL	g_bDebugOption = FALSE;
void ReadWEBDebugOption()
{
	HANDLE hFile = NULL;
	DWORD dwRead;
	char DebugOption;
	hFile = CreateFile(DEBUGOPTION, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {
		ReadFile(hFile, &DebugOption, 1, &dwRead, NULL);
		CloseHandle(hFile);

		DebugOption = '1';
	}
	else {
		//파일이 없으면 디버깅 안한다.
		//hFile = CreateFile( DEBUGOPTION, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
		//					NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
		//WriteFile(hFile,&DebugOption,1,&dwRead,NULL);
		//CloseHandle(hFile);
		DebugOption = '0';
	}
	if (DebugOption == '1') g_bDebugOption = TRUE;
	else g_bDebugOption = FALSE;
}

