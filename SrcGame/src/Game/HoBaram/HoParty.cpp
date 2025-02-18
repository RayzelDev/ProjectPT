#include "..\globals.h"
#include "..\\sinbaram\\SinLinkHeader.h"
#include "..\\HoBaram\\HoLinkHeader.h"
#include "..\\HoBaram\\HoTextFile.h"
#include "../tjboy/clanmenu/tjclanDEF.h"
#include "../tjboy/clanmenu/tjclan.h"

extern char UserAccount[256];
extern char	szConnServerName[16];

void fd222(char *buff)
{
    HANDLE hFile;
    DWORD dwAcess =0;
    hFile = CreateFile( "abc.txt", GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

    if( hFile != INVALID_HANDLE_VALUE ) {
        SetFilePointer( hFile , 0 , NULL , FILE_END );
        int chk = WriteFile( hFile , buff , lstrlen(buff) , &dwAcess , NULL );
        CloseHandle( hFile );
    }
}


void fd( char *fmt, ...)
{
	return;

    char buff[4096] = "abc";
	
    va_list args;

    va_start( args, fmt );
    vsprintf( buff, fmt, args );
    va_end( args );

    strcat( buff, "\r\n" );
	

	if(strlen(buff)>1024 ) {
		int abc = 1;
		abc=2;
	}

    fd222(buff);
}


void fd2( char *fmt, ...)
{
	return;

	char buff[4096];
	
    va_list args;

    va_start( args, fmt );
    vsprintf( buff, fmt, args );
    va_end( args );

    strcat( buff, "\r\n" );

	if(strlen(buff)>1024 ) {
		int abc = 1;
		abc=2;
	}

    fd222(buff);
}




/*///////////////////////////////////////////////////////////////////////////
: 함수  명 :                 INI읽기
: 하는  일 :
///////////////////////////////////////////////////////////////////////////*/
//데이타셋팅위한 ini를 열어 각종 옵션을 읽는다.([optStr1]의 optStr것을 읽음에 주의)
int read_INI(char *iniF, char *optStr1, char *optStr, char *rStr)
{
    char folder[] = ".\\";
    char strFilename[128];
    strcpy ( strFilename , folder);
    strcat ( strFilename , iniF );

    if(rStr==NULL) { //int형
        return GetPrivateProfileInt(optStr1, optStr, 0, strFilename );
    }
    //string형
    return GetPrivateProfileString(optStr1, optStr, NULL, rStr, 128, strFilename);
    //return 0;
}

int save_INI(char *iniF, char *optStr1, char *optStr, char *rStr, int value)
{
    char folder[] = ".\\";
    char strFilename[128];
    strcpy ( strFilename , folder);
    strcat ( strFilename , iniF );
	char buf[64];


    if(rStr==NULL) { //int형

		wsprintf(buf, "%d", value);
        //return GetPrivateProfileInt(optStr1, optStr, 0, strFilename );
		return WritePrivateProfileString(
			optStr1, optStr,
			buf,
			strFilename);  
    }
    //string형
    //return GetPrivateProfileString(optStr1, optStr, NULL, rStr, 128, strFilename);
	return WritePrivateProfileString(
			optStr1, optStr,
			rStr,
			strFilename);  

		
    //return 0;
}






/*==================================================================================
//
// ktj  : 새로 만든것.													start
//
==================================================================================*/
class HoParty_KTJ
{
public:
	HoParty_KTJ();
	~HoParty_KTJ();



	//PartyInterface 리소스..
	int									MatPartyBackGround;	//배경
	int									MatTitle[4+2] ;		//타이틀.(퀘스트,채팅,동료
	POINT								BackStartPos;		//인터페이스 기본위치
	int xLeft, xRight;

	RECT menuPos[30];		//메뉴셋팅.
	int menuSu;
	int PmenuN, savePmenuN;
	void InitMenuSet(int menuN);


	


	int  questSelN;
	void quest_Progress_Clear();
	void quest_End_Clear();
	void quest_Progress_Set(char *q_name, char *f_name);
	int  qNameProgress_SU;
	void quest_End_Set(char *q_name, char *f_name);
	int  qNameEnd_SU;
	int  qNameS_ENDs , qNameE_ENDs ;		//퀘스트이름의 화면상에 나올 맨위쪽과 맨아래쪽 
	void qN_Pressf_CLEAR();				//퀘스트의 헬프창없애기

	void quest_Sel_Progress();	//퀘스트진행버튼누른걸로 셋팅함.

	int  qNameS, qNameE;		//퀘스트이름의 화면상에 나올 맨위쪽과 맨아래쪽 
	int  qNameSelN;			//퀘스트이름의 선택번호
	char qNameb[64][128];	//퀘스트이름을 넣은 버퍼(퀘스트창에서 선택가능함)
	char qFnameb[64][128];	//퀘스트이름에 따른 해설파일의 이름들

	char qNameb_E[64][128];	//끝난 퀘스트이름을 넣은 버퍼(퀘스트창에서 선택가능함)
	char qFnameb_E[64][128];//끝난 퀘스트이름에 따른 해설파일의 이름들


	char q_explation[64][128];	//퀘스트의 설명(최대64줄이며 한페이지당 6줄임)
	int  q_explationS;
	char q_explationTitle[128];	//퀘스트의 설명에 사용되는 타이틀


	



	//채팅용 데이타저장용
	//int chatButN;			//채팅의 눌린 버튼의 번호
	//최근 귓말한사람의 목록
	char latestb[9][128];	//0~7까지사용
	int  latestb_LOG[9+1];	//로그가되있는지여부(서버쪽에서 사용가능)

	//friend용
	int fNameS, fNameE, fNameSu;		//친구 id의 화면상에 나올 맨위쪽과 맨아래쪽, 총갯수
	char fNameb[100+1][32];	//friend id을 넣은 버퍼(채팅창에서 선택가능함)
	int  fName_LOG[100+1];	//로그가되있는지여부(서버쪽에서 사용가능)
	//deny용
	int dNameS, dNameE, dNameSu;		//거부용 id
	int dNameSelN;			
	char dNameb[100+1][32];
	int  dName_LOG[100+1];	//로그가되있는지여부

	int cN_MenuN ;	//0:친구메뉴중 최근목록 1: 친구들목록 2:거부자목록
	int cN_Pressf;	//리스트중 누른거 첵크
	int cN_PressfDBL;	//더블클릭누른거
	int cN_PressfDBL_Where;	//더블클릭 메뉴번호.

	
	//동료->친구    ================================== start
	void chat_PageChange(int sw);		//동료->친구    id의 이전,다음 리스트를 보여줌.
	void chat_IDdelete();				//동료->친구    id 삭제시 필요함.
	void chat_isLOG(char *name, int flag);
	//cN_Pressf 눌린번호 셋팅(빈 부분은 셋팅이 안되기 때문임
	void chat_cN_Pressf(int press);
	void chat_IDinsert(int sw, char *id);

	void chatINI_read();	//겜처음들어갈때 셋팅함.
	void chatINI_save(int menuN);
	void chatDATA_Insert(int menuN, char *str);
	void chatDATA_delete(int menuN, int num);
	void latest_Insert(char *namestr);
	int chat_IDs(int dataN, int cnt, char *id);
	void chat_WhisperPartyPlayer(int num);
	void chat_WhisperPartyPlayer_close();


	int chatbuild_INIfiles();	//동료->친구
	

	void chat_changeMENU(int sw);	//0:친구->최근목록, 1:친구->친구목록, 2:친구->거부자목록의 메뉴를 바꿈.


	//동료->친구    ================================== end



	





	//ktj : ㅎㅎㄷ이 만든거 그대로씀
	hoPartyMember	PartyMember[MAX_PARTY_MEMBER];	//파티맴버 정보 구조체..
	int				PartyMemberCount;				//현재 인원수...

	//ktj : 일단 삭제
	int									PartyPosState;
	int									MouseDownPartyMember;

	int									MouseDownButton;
	
	//PartyInterface 리소스..
	//int									MatPartyBackGround;
	LPDIRECT3DTEXTURE9				BmpMan[3];

	LPDIRECT3DTEXTURE9				BmpLifeBlue;
	LPDIRECT3DTEXTURE9				BmpLifeRed;

	LPDIRECT3DTEXTURE9				ToolTip[5];
	LPDIRECT3DTEXTURE9				Button[5];

	
	//POINT								BackStartPos;
	POINT								TextStartPos;
	
	//PartyMessageBox 리소스..
	int									MatMessageBoxBackGround;
	POINT								MessageBoxStartPos;
	
	char								PartyMessageBoxChrName[128];
	DWORD								PartyMessageBoxChrCode;

	LPDIRECT3DTEXTURE9				BmpMessageOk[2];
	LPDIRECT3DTEXTURE9				BmpMessageCancel[2];

	int				MessageBoxDrawCount;
	BOOL			MessageBoxOpenFlag;			//Message창 오픈 플래그이다.




	void Init();
	void Load();
	void Close();


	
	int	 Draw();

	//sw=0, 1 //스크롤 안하고 사용시부른다.
	int Main_menuSet(int sw);
	int	 Main(int sw);
	

	//ktj : 주의 : 이함수는 사용안함.
	int  AddMember(char *memberName,DWORD chrCode, int life, int level);


	//ktj : 새로 만듦. ============== start
	int AddMember(hoPartyMember *member,int cnt);
	void AddMember_memberSu(int memberSu);
	int PARTY_PLAYUPDATE(hoPartyMember *member,int cnt);
	int PARTY_RELEASE();
	//ktj : 새로 만듦. ============== end

	






	
	//부수적 함수들 ( 마우스로 누른 메뉴 번호 첵크)
	int chkeckMenuN();
	void menuSet(int sw);
	
	


	void Draw_questExplane(int x, int y);	//퀘스트의 설명글 그리기
	void Init_questExplane(char *string, int sw);	//퀘스트의 설명글 그리기




	//파일의 데이타셋팅함수들			================= start
	int ReadFileData(char *fname);							//파일을 몽땅읽는다.
	void ReadFileData_moveBuf(char *savebuf, int lineLen);	//읽은데이타를 원하는버퍼로 한줄씩 짤라옮긴다.
	//파일의 설명글의 데이타셋팅함수들	================= end

};

HoParty_KTJ	InterfaceParty_KTJ;



/*==================================================================================
//
// ktj  : 새로 만든것.															end
//
==================================================================================*/
















HoParty	InterfaceParty;

//HFONT	hoFont11;

HoParty::HoParty()
{
	MessageBoxDrawCount = 0;
	BackStartPos.x = 800;
	BackStartPos.y = 155;
	
	TextStartPos.x = BackStartPos.x+102;
	TextStartPos.y = BackStartPos.y+38;
	
	BmpLifeBlue = NULL;
	BmpLifeRed  = NULL;

	for(int index = 0; index < 5; index++)
	{
		ToolTip[index] = NULL;
		Button[index] = NULL;
	}

	for ( int index = 0; index < 3; index++ )
		BmpMan[index] = NULL;
	
	//MessageBox용..
	MessageBoxStartPos.x = 513;
	MessageBoxStartPos.y = 3;
	MatMessageBoxBackGround = -1;
	
	for ( int index = 0; index < 2; index++ )
	{
		BmpMessageOk[index] = NULL;
		BmpMessageCancel[index] = NULL;
	}

	MouseDownPartyMember = -1;
	MouseDownButton = -1;
	memset(PartyMember, 0, sizeof(PartyMember));
	PartyMemberCount = 0;
	MatMessageBoxBackGround = -1;

	MessageBoxOpenFlag = FALSE;
	PartyPosState = PARTY_NONE;

/*
#ifdef _LANGUAGE_JAPANESE

#include "..\\Japanese\\j_font.h"

	hoFont11 = 	CreateFont( 8,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
						SHIFTJIS_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        FIXED_PITCH | FF_MODERN,
						j_font );
#endif

	
#ifdef _LANGUAGE_CHINESE

	hoFont11 = 	CreateFont( 8,
                        0,
                        0,
                        0,
			FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
						GB2312_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
						"SimSun" );

#endif

#ifdef _LANGUAGE_KOREAN
	hoFont11 = 	CreateFont( 4,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        HANGEUL_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "굴림체" );

#endif
*/

}


HoParty::~HoParty()
{
	Close();
}






//========================= 퀘스트 메뉴 관련 데이타 셋팅 ============================= start
void HoParty::quest_Progress_Clear()
{
	InterfaceParty_KTJ.quest_Progress_Clear();
}

void HoParty::quest_End_Clear()
{
	InterfaceParty_KTJ.quest_End_Clear();
}

void HoParty::quest_Progress_Set(char *q_name, char *f_name)
{
	InterfaceParty_KTJ.quest_Progress_Set(q_name, f_name);
}

void HoParty::quest_End_Set(char *q_name, char *f_name)
{
	InterfaceParty_KTJ.quest_End_Set(q_name, f_name);
}

void HoParty::qN_Pressf_CLEAR()
{
	InterfaceParty_KTJ.qN_Pressf_CLEAR();
}
void HoParty::quest_Sel_Progress()	//퀘스트진행버튼누른걸로 셋팅함.
{
	InterfaceParty_KTJ.quest_Sel_Progress();
}

//========================= 퀘스트 메뉴 관련 데이타 셋팅 ============================= end


//========================= 동료->친구 메뉴 관련 데이타 셋팅 ============================= start
void HoParty::latest_Insert(char *namestr)	//최근목록의 귓속말한사람의 리스트 추가.
{
	InterfaceParty_KTJ.latest_Insert(namestr);
}

//친구목록에 id넣을때
void HoParty::friend_Insert(char *id)
{
	InterfaceParty_KTJ.chat_IDinsert(3, id);
}


void HoParty::chat_isLOG(char *name, int flag)
{
	InterfaceParty_KTJ.chat_isLOG(name, flag);
}


//log여부를 알아내기 위해 이름을 받아내는것(
//dataN : 동료->친구의 최근목록==0, 친구들==1, 거부자==2
//cnt 0~15까지
//id
int HoParty::chat_IDs(int dataN, int cnt, char *id)
{
	return InterfaceParty_KTJ.chat_IDs(dataN, cnt, id);
}


//0:친구->최근목록, 1:친구->친구목록, 2:친구->거부자목록의 메뉴를 바꿈.
void HoParty::chat_changeMENU(int sw)
{
	
	InterfaceParty_KTJ.chat_changeMENU(sw);
}

//id, 패스워드입력된후 부를것.
int HoParty::chatbuild_INIfiles()	//동료->친구
{
	return InterfaceParty_KTJ.chatbuild_INIfiles();
}


void HoParty::chat_WhisperPartyPlayer_close()
{
	InterfaceParty_KTJ.chat_WhisperPartyPlayer_close();
	
	
	//ktj : clan 챗리슽 메뉴땜에 넣음.
	//void clan_chat_WhisperPartyPlayer_close();	//cE_chatlist.cpp에 정의
	//clan_chat_WhisperPartyPlayer_close();
	void chatlistSPEAKERflagChg(int flag);		//cE_chatlist.cpp에 정의
	chatlistSPEAKERflagChg(0);
}




void HoParty::resize()
{
	extern POINT pHoPartyMsgBox;

	int MidX = (int)((float)smScreenWidth * 0.92f);
	int MidY = (int)((float)smScreenHeight * 0.1f);

	int h = (int)((float)smScreenHeight / 6);
	int w = (int)((float)smScreenHeight * 1.34f / 8);


	pHoPartyMsgBox.x = MidX - (w >> 1) - 180;
	pHoPartyMsgBox.y = MidY - (h >> 1);
}

void resizePartyUI()
{
	InterfaceParty.resize();
}


void HoParty::Main_menuSet(int menuN)
{
	InterfaceParty_KTJ.Main_menuSet(menuN);
}



//퀘스트, 동료메뉴가 같이붙어있어서넣음(퀘스트0, 메뉴, 동료 1)
void HoParty::InitMenuSet(int menuN)
{
	InterfaceParty_KTJ.InitMenuSet(menuN);
}


void HoParty::Init()
{
	memset(PartyMember, 0, sizeof(PartyMember));
	PartyMemberCount = 0;
	MessageBoxDrawCount = 0;
	Close();
	Load();


	//ktj 
	InterfaceParty_KTJ.Init();
}

void HoParty::Close()
{
	if(BmpLifeBlue)
	{
		BmpLifeBlue->Release();
		BmpLifeBlue = NULL;
	}

	if(BmpLifeRed)
	{
		BmpLifeRed->Release();
		BmpLifeRed = NULL;
	}

	for(int index = 0; index < 5; index++)
	{
		if(ToolTip[index] != NULL)
		{
			ToolTip[index]->Release();
			ToolTip[index] = NULL;
		}
	}

	for ( int index = 0; index < 4; index++ )
	{
		if(Button[index] != NULL)
		{
			Button[index]->Release();
			Button[index] = NULL;
		}
	}

	for ( int index = 0; index < 2; index++ )
	{
		if(BmpMessageOk[index] != NULL)
		{
			BmpMessageOk[index]->Release();
			BmpMessageOk[index] = NULL;
		}

		if(BmpMessageCancel[index] != NULL)
		{
			BmpMessageCancel[index]->Release();
			BmpMessageCancel[index] = NULL;
		}
	}

	for ( int index = 0; index < 3; index++ )
	{
		if(BmpMan[index] != NULL)
		{
			BmpMan[index]->Release();
			BmpMan[index] = NULL;
		}
	}

	//폰트 제거
//	DeleteObject( hoFont11);
}

void HoParty::DrawImage(int Mat, int x, int y, int w, int h)
{
	HoDrawTexImage(Mat, x, y, w, h, w, h, 255);
}

void HoParty::Load()
{
	MatPartyBackGround = CreateTextureMaterial( "Effect\\Party\\PartyBackGround.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	BmpLifeBlue = LoadDibSurfaceOffscreen( "Effect\\Party\\LifeBlue.bmp");
	BmpLifeRed  = LoadDibSurfaceOffscreen( "Effect\\Party\\LifeRed.bmp");

	MatMessageBoxBackGround = CreateTextureMaterial("game\\images\\messagebox\\background_normal.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	LpButtonNo = CreateTextureMaterial("game\\images\\messagebox\\btnno.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	LpButtonNo_ = CreateTextureMaterial("game\\images\\messagebox\\btnno_.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	LpButtonYes = CreateTextureMaterial("game\\images\\messagebox\\btnyes.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	LpButtonYes_ = CreateTextureMaterial("game\\images\\messagebox\\btnyes_.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	
	char buffer[256];
	for(int index = 0; index < 5; index++)
	{
		memset(buffer, 0, sizeof(buffer));
		wsprintf(buffer, "%s%d%s", "Effect\\Party\\ToolTip", index+1, ".bmp");
		ToolTip[index] = LoadDibSurfaceOffscreen(buffer);
	}
	
	for ( int index = 0; index < 5; index++ )
	{
		memset(buffer, 0, sizeof(buffer));
		wsprintf(buffer, "%s%d%s", "Effect\\Party\\icon-", index+1, ".bmp");
		Button[index] = LoadDibSurfaceOffscreen(buffer);
	}
	
	for ( int index = 0; index < 3; index++ )
	{
		memset(buffer, 0, sizeof(buffer));
		wsprintf(buffer, "%s%d%s", "Effect\\Party\\Man", index+1, ".bmp");
		BmpMan[index] = LoadDibSurfaceOffscreen(buffer);
	}
	

	//메시지 창관련..			
	//////////////////////////////////////////////////////////////////////////
	/*MatMessageBoxBackGround = CreateTextureMaterial( "Effect\\Party\\MessageBackGround.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	for ( int index = 0; index < 2; index++ )
	{
		memset(buffer, 0, sizeof(buffer));
		wsprintf(buffer, "%s%d%s", "Effect\\Party\\Ok", index+1, ".bmp");
		BmpMessageOk[index] = LoadDibSurfaceOffscreen(buffer);

		memset(buffer, 0, sizeof(buffer));
		wsprintf(buffer, "%s%d%s", "Effect\\Party\\Cancel", index+1, ".bmp");
		BmpMessageCancel[index] = LoadDibSurfaceOffscreen(buffer);
	}*/
	
	ReadTextures();
}

extern int	WhisperPartyPlayer( char *szName );
extern int	PartyButton( int Num , int Button );

BOOL faceFlag = FALSE;
BOOL whisperFlag = FALSE;
BOOL buttonMessageFlag = FALSE;
int HoParty::Draw()
{
	/*
	if(lpD3DDevice == NULL)
		return FALSE;
	
	if(MouseButton[0] == FALSE)
	{
		faceFlag = FALSE;
		buttonMessageFlag = FALSE;
	}
	
	if(MouseDblClick == FALSE)
		whisperFlag = FALSE;
	*/
    /*
	if(PartyPosState == PARTY_START || PartyPosState == PARTY_PROCESS
		|| PartyPosState == PARTY_END)
	{
    */
		//ktj ; 지움.
		/*
		
		//Party창 출력.
		if(MatPartyBackGround != -1)
			dsDrawTexImage( MatPartyBackGround, BackStartPos.x, BackStartPos.y, 256, 256, 255);
		








		for(int index = 0; index < PartyMemberCount; index++)
		{
			//Life를 표시.
			if(BmpLifeBlue != NULL && BmpLifeRed != NULL)
			{
				DrawSprite(TextStartPos.x-3+14, TextStartPos.y+30*index-2+15, BmpLifeRed,  0, 0, 94, 4, 1);
				DrawSprite(TextStartPos.x-3+14, TextStartPos.y+30*index-2+15, BmpLifeBlue, 0, 0, int((94.f/100.f)*(float)PartyMember[index].Life), 4, 1);
			}
			
			if(pCursorPos.x > TextStartPos.x && pCursorPos.y > TextStartPos.y+30*index-10 &&
			   pCursorPos.x < TextStartPos.x +110 && pCursorPos.y < TextStartPos.y+30*index+18)
			{
				if(BmpMan[1] != NULL && PartyMember[index].JangFlag == FALSE)
					DrawSprite(TextStartPos.x-4, TextStartPos.y+30*index-2, BmpMan[1],  0, 0, 13, 17,1);
				
				if(BmpMan[3] != NULL && PartyMember[index].JangFlag)
					DrawSprite(TextStartPos.x-4, TextStartPos.y+30*index-2, BmpMan[2],  0, 0, 13, 17,1);
			
				//캐릭터 선택.
				if(MouseButton[0] && PartyPosState == PARTY_PROCESS)
				{
					MouseDownPartyMember = index;
					
					//얼굴 출력 함수..
					if(strlen(PartyMember[MouseDownPartyMember].Name) != 0 &&
					   strlen(PartyMember[MouseDownPartyMember].ModelName1) != 0 &&
					   strlen(PartyMember[MouseDownPartyMember].ModelName2) != 0 && faceFlag == FALSE)
					{
						if(faceFlag == FALSE)
						{
							faceFlag = TRUE;
							OpenPartyPlayer(PartyMember[MouseDownPartyMember].Name, PartyMember[MouseDownPartyMember].ModelName1, PartyMember[MouseDownPartyMember].ModelName2);
						}
					}
				}
				
				//귀말 보내기...
				if(MouseDblClick && whisperFlag == FALSE)
				{
					//ktj : 홍씨가 만드게 이상해서 MouseDownPartyMember->index로 바꿈.(제대로됨)
					
					//if(lpCurPlayer->dwObjectSerial != PartyMember[MouseDownPartyMember].ChrCode )
					//{
					//	whisperFlag = TRUE;
					//	WhisperPartyPlayer(PartyMember[MouseDownPartyMember].Name);
					//}
					if(lpCurPlayer->dwObjectSerial != PartyMember[index].ChrCode )
					{
						whisperFlag = TRUE;
						WhisperPartyPlayer(PartyMember[index].Name);
					}
				}
			}
			else
			{
				if(BmpMan[0] != NULL && PartyMember[index].JangFlag == FALSE)
					DrawSprite(TextStartPos.x-4, TextStartPos.y+30*index-2, BmpMan[0],  0, 0, 13, 17,1);
				
				if(BmpMan[3] != NULL && PartyMember[index].JangFlag)
					DrawSprite(TextStartPos.x-4, TextStartPos.y+30*index-2, BmpMan[2],  0, 0, 13, 17,1);
			}
		}
		
		if(PartyPosState == PARTY_PROCESS)
		{
			//눌린 멤버 표시하기.
			if(MouseDownPartyMember != -1 && PartyMember[MouseDownPartyMember].JangFlag == FALSE)
				DrawSprite(TextStartPos.x-4, TextStartPos.y+30*MouseDownPartyMember-2, BmpMan[1],  0, 0, 13, 17,1);
			if(MouseDownPartyMember != -1 && PartyMember[MouseDownPartyMember].JangFlag == TRUE)
				DrawSprite(TextStartPos.x-4, TextStartPos.y+30*MouseDownPartyMember-2, BmpMan[2],  0, 0, 13, 17,1);
		}
		
		HDC hdc = NULL;
		HFONT oldFont;
		//lpDDSBack->GetDC(&hdc);
		if(hFont != NULL)
			oldFont = (HFONT)SelectObject( hdc , hFont );
		
		//뒤에 그림자..
		//SetBkMode( hdc, TRANSPARENT );
		
		//Level을 찍기..
		char levelBuffer[256];
		for(index = 0; index < PartyMemberCount; index++)
		{
			SetFontTextColor( hdc, RGB(0, 0, 0));
			dsTextLineOut(hdc, TextStartPos.x+11, TextStartPos.y+30*index+1, PartyMember[index].Name, lstrlen(PartyMember[index].Name));
			memset(levelBuffer, 0, sizeof(levelBuffer));

			if(PartyMember[index].Level > 10)
				wsprintf(levelBuffer, "%s%d%s", "L",PartyMember[index].Level/10,"x");
			else
				wsprintf(levelBuffer, "%s%d", "L", PartyMember[index].Level);
			dsTextLineOut(hdc, TextStartPos.x+110, TextStartPos.y+5+30*index+1, levelBuffer, lstrlen(levelBuffer));
		}
		
		
		//진짜 글씨..
		//SetBkMode( hdc, TRANSPARENT );
		for(index = 0; index < PartyMemberCount; index++)
		{
			if( MouseDownPartyMember != index)
				SetFontTextColor( hdc, RGB(209, 190, 154));		
			else
				SetFontTextColor( hdc, RGB(255, 150, 0));
			
			if(pCursorPos.x > TextStartPos.x && pCursorPos.y > TextStartPos.y+30*index-10 &&
			   pCursorPos.x < TextStartPos.x +110 && pCursorPos.y < TextStartPos.y+30*index+18)
				SetFontTextColor( hdc, RGB(255, 150, 0));
			dsTextLineOut(hdc,TextStartPos.x+10, TextStartPos.y+30*index, PartyMember[index].Name, lstrlen(PartyMember[index].Name));

			memset(levelBuffer, 0, sizeof(levelBuffer));
			
			if(PartyMember[index].Level > 10)
				wsprintf(levelBuffer, "%s%d%s", "L",PartyMember[index].Level/10,"x");
			else
				wsprintf(levelBuffer, "%s%d", "L", PartyMember[index].Level);

			SetFontTextColor( hdc, RGB(255, 190, 107));
			dsTextLineOut(hdc, TextStartPos.x+110, TextStartPos.y+5+30*index, levelBuffer, lstrlen(levelBuffer));
		}
		
		if(oldFont)
			SelectObject( hdc, oldFont);
		//lpDDSBack->ReleaseDC(hdc);
		
		
		//Button찍기.
		for(index = 0; index < 5; index++)
		{
			if(pCursorPos.x > BackStartPos.x+114+index*22     && pCursorPos.y > BackStartPos.y+220 &&
			   pCursorPos.x < BackStartPos.x+114+20+index*22  && pCursorPos.y < BackStartPos.y+220+20)
			{
				if(lpCurPlayer->dwObjectSerial != PartyMember[0].ChrCode)
				{
					if(index == 0 || index == 1 || index == 2)
						continue;
				}
				
				if(PartyMemberCount == 0 && index != 4)
					continue;
				//Button 표시
				DrawSprite(BackStartPos.x+114+index*22, BackStartPos.y+222, Button[index],  0, 0, 20, 20,1);
				
				if(MouseButton[0] && PartyPosState == PARTY_PROCESS)
				{
					
					if(buttonMessageFlag == FALSE)
					{
						MouseDownButton = index;
						buttonMessageFlag = TRUE;
						//파티 버튼 보내기...
						PartyButton(MouseDownPartyMember, index);
						MouseDownPartyMember = -1;
					}
					
					//나가기 버튼..
					if(MouseDownButton == 4)
						PartyPosState = PARTY_END;
				}
			}
		}

		//눌린 버튼 표시하기..
		if(MouseDownButton != -1)
		{
			DrawSprite(BackStartPos.x+114+MouseDownButton*22+1, BackStartPos.y+222+1, Button[MouseDownButton],  0, 0, 20, 20,1);
			MouseDownButton = -1;
		}
		//ToolTip 표시.
		for(index = 0; index < 5; index++)
		{
			if(lpCurPlayer->dwObjectSerial != PartyMember[0].ChrCode )
			{
				if(index == 0 || index == 1 || index == 2)
					continue;
			}

			if(PartyMemberCount == 0)
			{
				if(index == 0 || index == 1 || index == 2 || index == 3)
					continue;
			}

			if(pCursorPos.x > BackStartPos.x+114+index*22     && pCursorPos.y > BackStartPos.y+220 &&
			   pCursorPos.x < BackStartPos.x+114+20+index*22  && pCursorPos.y < BackStartPos.y+220+20
			   && PartyPosState == PARTY_PROCESS)
			{
				//ToolTip 표시
				DrawSprite(BackStartPos.x+114+index*22-15, BackStartPos.y+226-28, ToolTip[index],  0, 0, 47, 27,1);
			}
		}

		*/





		//ktj ; 넣음.
		//InterfaceParty_KTJ.Draw();
	//}
	/*
	//MessageBox 출력시..
	if(MessageBoxOpenFlag == TRUE)
	{
		static PartySpotTimer = 0;
		
		//Message창 출력.
		if(MatMessageBoxBackGround != -1)
			dsDrawTexImage( MatMessageBoxBackGround, MessageBoxStartPos.x, MessageBoxStartPos.y, 256, 64, 255);
			PartySpotTimer++;
		
		if(PartySpotTimer > 15)
		{
			dsDrawColorBox( D3DCOLOR_RGBA( 0,200,255,80 ), MessageBoxStartPos.x+5, MessageBoxStartPos.y+5, 160, 53 ); //깜빡인다 
			if(PartySpotTimer >30)
				PartySpotTimer = 0;
		}
		
		
		//Message 박스 관련..
		////////////////////////////////////////////////////////////////////////////////////////////////
		//ok
		if(pCursorPos.x > MessageBoxStartPos.x+140     && pCursorPos.y > MessageBoxStartPos.y+11 &&
		   pCursorPos.x < MessageBoxStartPos.x+140+23  && pCursorPos.y < MessageBoxStartPos.y+11+23)
		{
			if(BmpMessageOk[1] != NULL)
				DrawSprite(MessageBoxStartPos.x+140, MessageBoxStartPos.y+11, BmpMessageOk[1],  0, 0, 23, 23,1);
			if(MouseButton[0])
			{
				SendJoinPartyUser(PartyMessageBoxChrCode);
				PartyPosState = PARTY_START;
				MessageBoxOpenFlag = FALSE;



				//ktj:...
				InitMenuSet(1);
				//PmenuN=1;


			}
		}
		else
		{
			if(BmpMessageOk[0] != NULL)
				DrawSprite(MessageBoxStartPos.x+140, MessageBoxStartPos.y+11, BmpMessageOk[0],  0, 0, 23, 23,1);
		}
		
		//Cancel
		if(pCursorPos.x > MessageBoxStartPos.x+144     && pCursorPos.y > MessageBoxStartPos.y+37 &&
		   pCursorPos.x < MessageBoxStartPos.x+144+15  && pCursorPos.y < MessageBoxStartPos.y+37+15)
		{
			if(BmpMessageCancel[0] != NULL)
				DrawSprite(MessageBoxStartPos.x+144, MessageBoxStartPos.y+37, BmpMessageCancel[1],  0, 0, 15, 15,1);
			
			if(MouseButton[0])
				MessageBoxOpenFlag = FALSE;
			
		}
		else
		{
			if(BmpMessageCancel[0] != NULL)
				DrawSprite(MessageBoxStartPos.x+144, MessageBoxStartPos.y+37, BmpMessageCancel[0],  0, 0, 15, 15,1);
		}
		
		HDC hdc = NULL;
		HFONT oldFont;
		//lpDDSBack->GetDC(&hdc);
		oldFont = (HFONT)SelectObject( hdc , hFont);
		
		//SetBkMode( hdc, TRANSPARENT );

		SetFontTextColor( hdc, RGB(255, 255, 255));
		
		dsTextLineOut(hdc, MessageBoxStartPos.x+36, MessageBoxStartPos.y+16, PartyMessageBoxChrName, lstrlen(PartyMessageBoxChrName));
		dsTextLineOut(hdc, MessageBoxStartPos.x+28, MessageBoxStartPos.y+38, HoText_PartyMessage, lstrlen(HoText_PartyMessage));
		
		
		
		if(oldFont)
			SelectObject( hdc, oldFont);
		//lpDDSBack->ReleaseDC(hdc);
	}
	*/
    //DrawPartyMessage(MessageBoxStartPos.x, MessageBoxStartPos.y);
	
 	if(GRAPHICDEVICE == NULL)
		return FALSE;
	
	if(MouseButton[0] == FALSE)
	{
		faceFlag = FALSE;
		buttonMessageFlag = FALSE;
	}
	
	if(MouseDblClick == FALSE)
		whisperFlag = FALSE;

	if(PartyPosState == PARTY_START || PartyPosState == PARTY_PROCESS
		|| PartyPosState == PARTY_END)
		InterfaceParty_KTJ.Draw();

 	return TRUE;
}

void HoParty::DrawPartyMessage(int x, int y)
{
	int BaseX = (WinSizeX - 260) / 2;
	int BaseY = (WinSizeY - 161) / 2;
	char strBuff[128];
	SIZE size = {};

	if (MessageBoxOpenFlag == TRUE)
	{
		static int PartySpotTimer = 0;

		//Party
		if (MatMessageBoxBackGround != -1)
		DrawImage(MatMessageBoxBackGround, BaseX, BaseY, 230, 161);
		DrawImage(LpButtonNo, BaseX + 115, BaseY + 116, 88, 35);
		DrawImage(LpButtonYes, BaseX + 26, BaseY + 116, 88, 35);

		PartySpotTimer++;

		if (PartySpotTimer > 15)
		{

			if (PartySpotTimer > 30)
				PartySpotTimer = 0;
		}
		
		SetFontTextColor(RGB(244, 209, 66));
		strcpy_s(strBuff, "Confirmar Pedido");
		GetFontTextExtentPoint(strBuff, strlen(strBuff), &size);
		dsTextLineOut(BaseX + (130 - size.cx / 2), BaseY + 17, strBuff, strlen(strBuff));

		SetFontTextColor(RGB(255, 255, 255));
		wsprintf(strBuff, "Fazer Grupo com %s ?", PartyMessageBoxChrName);
		GetFontTextExtentPoint(strBuff, strlen(strBuff), &size);
		dsTextLineOut(BaseX + (126 - size.cx / 2), BaseY + 62, strBuff, strlen(strBuff));

		//Party
		if (pRealCursorPos.x >= BaseX + 20 && pRealCursorPos.x <= BaseX + 20 + 110 &&
			pRealCursorPos.y >= BaseY + 99 && pRealCursorPos.y <= BaseY + 99 + 35)
		{
			DrawImage(LpButtonYes_, BaseX + 20, BaseY + 99, 110, 44);

			if (MouseButton[0])
			{
				SendJoinPartyUser(PartyMessageBoxChrCode);
				PartyPosState = PARTY_START;
				MessageBoxOpenFlag = FALSE;
				InitMenuSet(1);
			}
		}

		if (pRealCursorPos.x >= BaseX + 134 && pRealCursorPos.x <= BaseX + 134 + 110 &&
			pRealCursorPos.y >= BaseY + 99 && pRealCursorPos.y <= BaseY + 99 + 35)
		{
			DrawImage(LpButtonNo_, BaseX + 134, BaseY + 99, 110, 44);

			if (MouseButton[0])
				MessageBoxOpenFlag = FALSE;
		}
	}
}
void HoParty::MainPartyMessage(int x, int y)
{
	
}

int HoParty::Main()
{
	if(PartyPosState == PARTY_START)
	{
		BackStartPos.x-= 15;
		if(BackStartPos.x <= 549)
		{
			BackStartPos.x = 549;
			PartyPosState = PARTY_PROCESS;
		}
		TextStartPos.x = BackStartPos.x+102;
	}
	else if(PartyPosState == PARTY_END)
	{
		BackStartPos.x += 15;
		if(BackStartPos.x >= 800)
		{
			//ktj : 클랜의 휠부분클리어
			void TJBscrollWheelClear_ChatL();
			TJBscrollWheelClear_ChatL();


			void but_exit_chk();
			but_exit_chk();

			//ret_menuN  = 0;
			//PmenuN=2;



			MouseDownPartyMember = -1;
			MouseDownButton = -1;
			BackStartPos.x = 800;
			PartyPosState = PARTY_NONE;
		}
		TextStartPos.x = BackStartPos.x+102;
	}
	

	
	if(MessageBoxOpenFlag)
	{
		MessageBoxDrawCount++;
		if(MessageBoxDrawCount > 1400)
		{
			MessageBoxOpenFlag = FALSE;
			MessageBoxDrawCount = 0;
		}
	}
	

	//ktj : 새로 넣음.
	int chk = InterfaceParty_KTJ.Main(PartyPosState );
	if(chk == PARTY_END) {

		//ktj : 클랜의 휠부분클리어
		void TJBscrollWheelClear_ChatL();
		TJBscrollWheelClear_ChatL();

		PartyPosState = PARTY_END;		//내가만든인터페이스에서 끝내는거면 나가도록한다.
	}



	return TRUE;
}

//ktj : 주의 : 이함수는 사용안함.
int HoParty::AddMember(char *memberName,DWORD chrCode, int life, int level)
{

	//ktj: 새로넣었음(이함수를 어디서부르는지 몰라서 그냥 여기다 넣었다)
	InterfaceParty_KTJ.AddMember(memberName, chrCode,  life,  level);


	if(memberName == NULL) 
		return FALSE;
	
	if(PartyMemberCount >= MAX_PARTY_MEMBER)
		return FALSE;
	memcpy(PartyMember[PartyMemberCount].Name, memberName, strlen(memberName));
	PartyMember[PartyMemberCount].ChrCode = chrCode;
	PartyMember[PartyMemberCount].Life = life;
	PartyMember[PartyMemberCount].Level = 100;
	PartyMemberCount++;
	return TRUE;
}



//ktj : 새로 만듦. ============== start
//파티용
int HoParty::AddMember(hoPartyMember *member,int cnt)
{
	//ktj : 임시
	InterfaceParty_KTJ.AddMember(member,cnt);
	
	//ktj : 지움.
	
	PartyMember[cnt].ChrCode = member->ChrCode;
	PartyMember[cnt].Life  = member->Life;
	PartyMember[cnt].Level = member->Level;
	PartyMember[cnt].CharacterPos.x = member->CharacterPos.x;
	PartyMember[cnt].CharacterPos.z = member->CharacterPos.z;
	PartyMember[cnt].CharacterPos.y = member->CharacterPos.y;
	lstrcpy( PartyMember[cnt].Name , member->Name );
	lstrcpy( PartyMember[cnt].ModelName1 , member->ModelName1 );
	lstrcpy( PartyMember[cnt].ModelName2 , member->ModelName2 );
	PartyMember[cnt].JangFlag = member->JangFlag ;
	
				
	return TRUE;
}
//파티용
void HoParty::AddMember_memberSu(int memberSu)
{
	//ktj : 임시
	InterfaceParty_KTJ.AddMember_memberSu(memberSu);


	//ktj : 지움.
	
    PartyMemberCount = memberSu;
	
}
//파티용
int HoParty::PARTY_PLAYUPDATE(hoPartyMember *member,int cnt)
{
	//ktj : 임시
	InterfaceParty_KTJ.PARTY_PLAYUPDATE(member,cnt);


	//ktj : 지움.
	
	if ( PartyMember[cnt].ChrCode==member->ChrCode) {
		PartyMember[cnt].Life  = member->Life;
		PartyMember[cnt].Level = member->Level;
		PartyMember[cnt].CharacterPos.x = member->CharacterPos.x;
		PartyMember[cnt].CharacterPos.z = member->CharacterPos.z;
		return 1;
	}
	
	return 0;
}


void HoParty::PARTY_RELEASE()
{
	//ktj : 임시
	int chk = 
		InterfaceParty_KTJ.PARTY_RELEASE();


	
	PartyMemberCount = 0;
	MouseDownPartyMember = -1;


	if(chk == 0) return;

	PartyPosState = PARTY_END;
	
}
//ktj : 새로 만듦. ============== end



































/*==================================================================================
//
// ktj  : 새로 만든것.													start
//
==================================================================================*/
int chkMenu(int x, int y, RECT *r) 
{
	if(r->left <= x   &&   x<= r->right ) 
	{
		if(r->top <= y   &&   y<= r->bottom ) 
			return 1;
	}
	return 0;
}




#define PARTY_BINFOsu 22
LPDIRECT3DTEXTURE9				binfo_Bmp[PARTY_BINFOsu];
#define PARTY_BUTTONsu 54
LPDIRECT3DTEXTURE9				butt_Bmp[PARTY_BUTTONsu];


//메뉴의 l, t, r, b임.
HoParty_KTJ::HoParty_KTJ()
{
	//ktj :
	BackStartPos.x = 800;
	BackStartPos.y = 155;


	xLeft = BackStartPos.x-100;		//ktj : left,right스크롤애니를 위해 설정.
	xRight= BackStartPos.x;


	questSelN = 5;					//퀘스트 메뉴번호(5:진행 6:끝)


	PmenuN=0;	//파티메뉴번호(0:퀘스트, 1:동료, 2:친구)
	savePmenuN = 0;
	//PmenuN=1;	//파티메뉴번호(0:퀘스트, 1:동료, 2:친구)

	cN_MenuN = 0;	//친구메뉴중 0: 최근목록 1: 친구들목록 2:거부자목록
	cN_Pressf = 0;
	cN_PressfDBL = 0;	//더블클릭누른거
	cN_PressfDBL_Where = 0;



	
	MessageBoxDrawCount = 0;
	//BackStartPos.x = 800;
	//BackStartPos.y = 155;
	
	//TextStartPos.x = BackStartPos.x+102;
	//TextStartPos.y = BackStartPos.y+38;
	
	BmpLifeBlue = NULL;
	BmpLifeRed  = NULL;

	for(int index = 0; index < 5; index++)
	{
		ToolTip[index] = NULL;
		Button[index] = NULL;
	}

	for ( int index = 0; index < 3; index++ )
		BmpMan[index] = NULL;
	
	//MessageBox용..
	MessageBoxStartPos.x = 513;
	MessageBoxStartPos.y = 3;
	MatMessageBoxBackGround = -1;
	
	for ( int index = 0; index < 2; index++ )
	{
		BmpMessageOk[index] = NULL;
		BmpMessageCancel[index] = NULL;
	}

	MouseDownPartyMember = -1;
	MouseDownButton = -1;
	memset(PartyMember, 0, sizeof(PartyMember));
	PartyMemberCount = 0;
	MatMessageBoxBackGround = -1;

	MessageBoxOpenFlag = FALSE;
	PartyPosState = PARTY_NONE;



	ZeroMemory(binfo_Bmp, sizeof(binfo_Bmp) );
	ZeroMemory(butt_Bmp, sizeof(butt_Bmp) );

	/*
	//ktj : 삭제
#ifdef _LANGUAGE_JAPANESE
	hoFont11 = 	CreateFont( 8,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
						SHIFTJIS_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
						"MS GOTHIC" );
#endif

	
#ifdef _LANGUAGE_CHINESE

	hoFont11 = 	CreateFont( 8,
                        0,
                        0,
                        0,
			FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
						GB2312_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
						"SimSun" );

#endif

#ifdef _LANGUAGE_KOREAN
	hoFont11 = 	CreateFont( 4,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        HANGEUL_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "굴림체" );

#endif
	*/
}


HoParty_KTJ::~HoParty_KTJ()
{
	Close();
}

void HoParty_KTJ::InitMenuSet(int menuN)
{
	if(menuN==1) {
		if(savePmenuN ==0) {
			PmenuN = menuN;
			savePmenuN = PmenuN ;
		}
		else {
			PmenuN = savePmenuN ;
		}
	}
	else {
		PmenuN = menuN;
	}
}


	











//========================= 퀘스트 메뉴 관련 데이타 셋팅 ============================= start

void HoParty_KTJ::quest_Progress_Clear()
{
	ZeroMemory(qNameb, sizeof(qNameb));
	ZeroMemory(qFnameb, sizeof(qFnameb));
	qNameProgress_SU = 0;
}

void HoParty_KTJ::quest_End_Clear()
{
	
	ZeroMemory(qNameb_E, sizeof(qNameb_E));
	ZeroMemory(qFnameb_E, sizeof(qFnameb_E));
	qNameEnd_SU = 0;
}


void HoParty_KTJ::quest_Progress_Set(char *q_name, char *f_name)
{
	strcpy(qNameb[  qNameProgress_SU  ], q_name);
	strcpy(qFnameb[  qNameProgress_SU  ], f_name);

	qNameProgress_SU++;
}

void HoParty_KTJ::quest_End_Set(char *q_name, char *f_name)
{
	strcpy(qNameb_E[  qNameEnd_SU  ], q_name);
	strcpy(qFnameb_E[  qNameEnd_SU  ], f_name);
	qNameEnd_SU++;
}


//퀘스트의 핼프창 닫기
void HoParty_KTJ::qN_Pressf_CLEAR()
{
	extern int qN_Pressf;	//퀘스트이름의 누름표시(8~12까지로 정해짐)
	extern int QuestnotCharMove, QuestnotCharMove2;	//메뉴판안의 마우스의 움직임에 캐릭터가 영향없도록 한다.
	qN_Pressf = 0;
	QuestnotCharMove = 0;
	QuestnotCharMove2 = 0;
	
}


//퀘스트의 진행 버튼 누른것처럼하기
void HoParty_KTJ::quest_Sel_Progress()
{
	questSelN = 5;
}

//========================= 퀘스트 메뉴 관련 데이타 셋팅 ============================= end







//========================= 동료->친구 메뉴 관련 데이타 셋팅 ============================= start
int HoParty_KTJ::chatbuild_INIfiles()	//동료->친구
{
	HANDLE hFile;
    DWORD dwAcess =0;

	char fname[64];
	//wsprintf(fname, ".\\savedata\\%s.ini", UserAccount );//
	//서버이름과, id가 조합되나온다.

	if(szConnServerName[0] == 0)
		wsprintf(fname, ".\\savedata\\%s.ini", UserAccount );
	else 
		wsprintf(fname, ".\\savedata\\%s\\%s.ini", szConnServerName, UserAccount );


	hFile = CreateFile( fname,
				GENERIC_READ , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );

	if( hFile != INVALID_HANDLE_VALUE ) {		//파일이 존재하므로 내버려 둔다.
        CloseHandle( hFile );
		return 1;
	}



	CreateDirectory(".\\savedata", 0);
	char buf[128];

	if(szConnServerName[0] != 0) {		//서버이름의 savedata디렉토리안에  만든다.
		wsprintf(buf,".\\savedata\\%s", szConnServerName);
		CreateDirectory(buf, 0);
	}


	char inidata0[] = "[FRIEND]\r\n";//ID00=\r\nID01=\r\nID02=\r\nID03=\r\nID04=\r\nID05=\r\nID06=\r\nID07=\r\nID08=\r\nID09=\r\nID10=\r\nID11=\r\nID12=\r\nID13=\r\nID14=\r\nID15=\r\nID16=\r\n\r\n";
	char inidata1[] = "[DENY]\r\n";//ID00=\r\nID01=\r\nID02=\r\nID03=\r\nID04=\r\nID05=\r\nID06=\r\nID07=\r\nID08=\r\nID09=\r\nID10=\r\nID11=\r\nID12=\r\nID13=\r\nID14=\r\nID15=\r\nID16=\r\n\r\n";
	char inidata2[10];


	hFile = CreateFile( fname,
				GENERIC_WRITE , FILE_SHARE_READ|FILE_SHARE_WRITE, 
				NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL , NULL );

	int chk = WriteFile( hFile , inidata0, lstrlen(inidata0) , &dwAcess , NULL );
	for(int i=0; i<(100+1); i++) {
		wsprintf( inidata2, "ID%0.2d=\r\n", i);
		WriteFile( hFile , inidata2, lstrlen(inidata2) , &dwAcess , NULL );
	}

	chk = WriteFile( hFile , inidata1, lstrlen(inidata1) , &dwAcess , NULL );	
	for(int i=0; i<(100+1); i++) {
		wsprintf( inidata2, "ID%0.2d=\r\n", i);
		WriteFile( hFile , inidata2, lstrlen(inidata2) , &dwAcess , NULL );
	}

	CloseHandle( hFile );
	return chk ;		//파일없음.
}




void HoParty_KTJ::chatINI_read()
{
	//ktj : 임시임 지울것.
	//chatbuild_INIfiles();

	char fname[64];
	//wsprintf(fname, ".\\savedata\\%s\\%s.ini", szConnServerName, UserAccount );
	if(szConnServerName[0] == 0)
		wsprintf(fname, ".\\savedata\\%s.ini", UserAccount );
	else 
		wsprintf(fname, ".\\savedata\\%s\\%s.ini", szConnServerName, UserAccount );



	int i, chk;
	char buf[64];
	//ktj :  동료->친구 메뉴의 ini셋팅읽음.  
	ZeroMemory((void*)fNameb, sizeof(fNameb));
	i=0;
	fNameSu = i;
	while(1) {
		wsprintf(buf,"ID%0.2d", i);
		chk = read_INI(		//chating.ini 데이타를 가져온다.
			//".\\image\\party\\party_DATA\\chating.ini", 
			fname,
			"FRIEND", buf, fNameb[i]);
		if(chk==0) break;
		i++;
	}

	//임시로 한다.
	fNameS = 0;
	fNameE = 7;		//채팅id의 화면상에 나올 맨위쪽과 맨아래쪽 
	fNameSu = i;

	ZeroMemory((void*)dNameb, sizeof(dNameb));
	i=0;
	while(1) {
		wsprintf(buf,"ID%0.2d", i); 
		chk = read_INI(		//chating.ini 데이타를 가져온다.
			//".\\image\\party\\party_DATA\\chating.ini", 
			fname,
			"DENY", buf, dNameb[i]);
		if(chk==0) break;
		i++;
	}

	//임시로 한다.
	dNameS= 0;
	dNameE = 7;		//채팅id의 화면상에 나올 맨위쪽과 맨아래쪽 
	dNameSu = i;
}




//chatSave : 12, 13, 14, 15
int save_INI(char *iniF, char *optStr1, char *optStr, char *rStr, int value);

void HoParty_KTJ::chatINI_save(int menuN)
{

	char fname[64];
	//wsprintf(fname, ".\\savedata\\%s.ini", UserAccount );
	//wsprintf(fname, ".\\savedata\\%s\\%s.ini", szConnServerName, UserAccount );
	if(szConnServerName[0] == 0)
		wsprintf(fname, ".\\savedata\\%s.ini", UserAccount );
	else 
		wsprintf(fname, ".\\savedata\\%s\\%s.ini", szConnServerName, UserAccount );



	int i;
	char buf[64];

	switch(menuN) {
	case 0:				//최근목록은 교체 안함.
		return;
	case 1:				//1: 친구들목록 
		for(i=0; i<100; i++) {
			wsprintf(buf,"ID%0.2d", i);
			save_INI(	//".\\image\\party\\party_DATA\\chating.ini", 
						fname,
						"FRIEND", buf, fNameb[i], 0);
		}
		return;

	case 2:				//거부목록
		for(i=0; i<100; i++) {
			wsprintf(buf,"ID%0.2d", i); 
			save_INI(	//".\\image\\party\\party_DATA\\chating.ini", 
						fname,
						"DENY", buf, dNameb[i], 0);
		}
		return;
	}
}

void HoParty_KTJ::chatDATA_Insert(int menuN, char *str)
{
	int i, cnt;
	int l;
	
	switch(menuN) {
	case 0:				//최근목록은 교체 안함.
		return;
	case 1:				//1: 친구들목록 
		l = strlen(str);
		for(i=0; i<100; i++) {
			if( l == strlen(fNameb[i])  &&   (   strcmp(str, fNameb[i]) == 0   )) 
			{
				return;		//같은게 있슴.
			}
		}

		cnt=0;
		for(i=0; i<100; i++) {
			if(fNameb[i][0] == 0) {
				wsprintf(fNameb[i],"%s", str);	//빈곳에 때려넣는다.
				cnt++;
				break;
			}
			cnt++;
		}
		fNameSu = cnt;
		return;
	case 2:			//거부목록

		l = strlen(str);
		for(i=0; i<100; i++) {
			if( l == strlen(dNameb[i])  &&   (   strcmp(str, dNameb[i]) == 0   )) 
			{
				return;		//같은게 있슴.
			}
		}



		cnt=0;
		for(i=0; i<100; i++) {
			if(dNameb[i][0] == 0) {
				wsprintf(dNameb[i],"%s", str);	//빈곳에 때려넣는다.
				cnt++;
				break;
			}
			cnt++;
		}
		dNameSu = cnt;
		return;
	}
}


//ini의 데이타 지울때 사용
void HoParty_KTJ::chatDATA_delete(int menuN, int num)
{
	int i, cnt;
	switch(menuN) {
	case 0:
		for(i=num; i<7; i++) {
			strncpy(latestb[i], latestb[i+1], sizeof(latestb[i]));
			latestb_LOG[i] = latestb_LOG[i+1];
		}
		break;
	case 1:
		for(i=num; i<100; i++) {
			strncpy(fNameb[i], fNameb[i+1], sizeof(fNameb[i]));
			fName_LOG[i] = fName_LOG[i+1];
		}

		cnt=0;
		for(i=0; i<100; i++) {						//갯수다시알아냄
			if(fNameb[i][0] == 0) {
				//cnt++;
				break;
			}
			cnt++;
		}
		fNameSu = cnt;


		if(fNameS &&  fNameb[fNameS][0]==0) {	//시작번호끝번호를 다시 알아냄
			fNameS-=7;
			fNameE=fNameS+7;
		}
		break;



	case 2:
		for(i=num; i<100; i++) {
			strncpy(dNameb[i], dNameb[i+1], sizeof(dNameb[i]));
			dName_LOG[i] = dName_LOG[i+1];
		}

		cnt=0;										//갯수다시알아냄
		for(i=0; i<100; i++) {
			if(dNameb[i][0] == 0) {
				//cnt++;
				break;
			}
			cnt++;
		}
		dNameSu = cnt;

		if(dNameS &&  dNameb[dNameS][0]==0) {		//시작번호끝번호를 다시 알아냄
			dNameS-=7;
			dNameE=dNameS+7;
		}
		break;
	}

}




//친구 메뉴의 최근목록 insert한다.
void HoParty_KTJ::latest_Insert(char *namestr)
{
	int i;


	//같은게 있으면 그것을 삭제한다.
	for(i=0; i<7; i++) {
		if(latestb[i][0] != 0) {
			if( strcmp(latestb[i], namestr ) == 0 ) {
				if( strlen(latestb[i]) ==  strlen(namestr)) {
					chatDATA_delete(0, i);				//최근목록중에서 이동된 목록은 삭제함.
					break;
				}
			}
		}
	}



	for(i=6; i>=1;i--) {
		ZeroMemory( latestb[i], sizeof(latestb[i]) );
		strcpy(latestb[i], latestb[i-1] );
	}
	strcpy(latestb[0], namestr);	//맨위에다설정

}





//동료->친구    id의 이전,다음 리스트를 보여줌.
void HoParty_KTJ::chat_PageChange(int sw)
{
	switch(cN_MenuN) {
	case 0:				//최근목록은 교체 안함.
		break;
	case 1:				//1: 친구들목록 
		if(sw==0) {
			fNameS -= 7;
			if(fNameS <0)fNameS =0;
			fNameE = fNameS +7;
			if(fNameE  > 100) fNameE  = 100;
			return;
		}
		fNameS += 7;
		if(fNameb[ fNameS ][0]==0) fNameS -= 7;
		fNameE = fNameS + 7;
		if(fNameE  > 100) fNameE  = 100;
		break;
	case 2:				//2:거부자목록
		if(sw==0) {
			dNameS -= 7;
			if(dNameS <0)dNameS =0;
			dNameE = dNameS +7;

			if(dNameE > 100) dNameE = 100;
			return;
		}
		dNameS += 7;
		if(dNameb[ dNameS ][0]==0) dNameS -= 7;
		dNameE = dNameS + 7;
		if(dNameE > 100) dNameE = 100;

		break;
	}
}

//동료->친구    id 삭제시 필요함.
void HoParty_KTJ::chat_IDdelete()
{
	int i;
	switch(cN_MenuN) {
	case 0:				//최근목록은 교체 안함.
		break;
	case 1:				//1: 친구들목록 
		if(cN_Pressf==0) return;		//선택안됐음.
		i = cN_Pressf-3;
		i += fNameS;

		chatDATA_delete(cN_MenuN, i);
		chatINI_save(cN_MenuN);
		break;
	case 2:				//2:거부자목록
		if(cN_Pressf==0) return;
		i = cN_Pressf-3;
		i += dNameS;

		chatDATA_delete(cN_MenuN, i);
		chatINI_save(cN_MenuN);

		break;
	}
}


//동료->친구->최근목록->친구등록버튼, 거부등록버튼눌렸을시에 사용
void HoParty_KTJ::chat_IDinsert(int sw, char *id)
{
	int i;
	//

	if(sw==1)	{							//1: 친구들목록에 등록하기
		if(cN_MenuN != 0) return;			//메뉴가 최근목록일때만 사용됨

		if(cN_Pressf==0) return;			//선택안됐음.
		i = cN_Pressf-3;
		
		chatDATA_Insert(1, latestb[i]);		//최근목록에서 거부자목록으로 옮긴다.
		chatINI_save(1);					//거부자목록 세이브

		chatDATA_delete(0, i);				//최근목록중에서 이동된 목록은 삭제함.
		return;
	}

	if(sw==2)	{							//2: 친구들목록에 등록하기
		if(cN_MenuN != 0) return;			//메뉴가 최근목록일때만 사용됨

		if(cN_Pressf==0) return;
		i = cN_Pressf-3;

		chatDATA_Insert(2, latestb[i]);		//최근목록에서 거부자목록으로 옮긴다.
		chatINI_save(2);					//거부자목록 세이브

		chatDATA_delete(0, i);				//최근목록중에서 이동된 목록은 삭제함.
	}



	//이부분은 서버에서 직접 친구목록으로 id를 넣을때 사용함.
	if(sw==3) {
		chatDATA_Insert(1, id);		//최근목록에서 거부자목록으로 옮긴다.
		chatINI_save(1);					//거부자목록 세이브
		return;
	}

}







//로긴되있는지의 여부를 알아냄.
void HoParty_KTJ::chat_isLOG(char *name, int flag)
{
	int i;
	int len = strlen(name);
	for(i=0; i<100; i++) {
		if( ( len == strlen(fNameb[i]) ) || 
			( strcmp(name, fNameb[i])==0 ) 
			 ) 
		{
			fName_LOG[i] = flag;		//on off될수있음.
			return;
		}
	}


	for(i=0; i<100; i++) {
		if( ( len == strlen(dNameb[i]) ) || 
			( strcmp(name, dNameb[i])==0 ) 
			 ) 
		{
			dName_LOG[i] = flag;
			return;
		}
	}
}


void HoParty_KTJ::chat_cN_Pressf(int press)	
{
	switch(cN_MenuN) {
	case 0:
		press -= 3;
		if(latestb[ press ][0] == 0) {
			cN_Pressf = 0;
			return;
		}

		cN_Pressf = press + 3;
		break;

	case 1:
		press = press-3;
		press += fNameS;

		if(fNameb[ press ][0] == 0) {
			cN_Pressf = 0;
			return;
		}
		cN_Pressf = (press-fNameS) + 3;
		break;
	case 2:
		press = press-3;
		press += dNameS;

		if(dNameb[ press ][0] == 0) {
			cN_Pressf = 0;
			return;
		}

		cN_Pressf = (press-dNameS) + 3;
		break;
	}
}


int HoParty_KTJ::chat_IDs(int dataN, int cnt, char *id)
{
	switch(dataN) {
	case 0:	
		if(cnt>6) return 0;
		
		if(latestb[ cnt ][0] == 0) {
			return 0;
		}
		strcpy(id, latestb[ cnt ]);
		break;

	case 1:
		if(cnt>(100-1)) return 0;
		
		if(fNameb[ cnt ][0] == 0) {
			return 0;
		}
		strcpy(id, fNameb[ cnt ]);
		
		break;
	case 2:
		if(cnt>(100-1)) return 0;
		
		if(dNameb[ cnt ][0] == 0) {
			return 0;
		}
		strcpy(id, dNameb[ cnt ]);

		break;
	}

	return 1;
}


void HoParty_KTJ::chat_WhisperPartyPlayer(int num)
{
	int i;
	switch(cN_MenuN) {
	case 0:
		cN_PressfDBL_Where = 0;


		if(num==-1) {
			if(cN_PressfDBL == cN_Pressf) {
				cN_PressfDBL = 0;
				break;
			}
			cN_PressfDBL = cN_Pressf;
			cN_Pressf = 0;

			i = cN_PressfDBL - 3;
		}
		else {
			if(cN_Pressf != num){
				cN_Pressf = num;
				return;
			}
			cN_PressfDBL = num;
			i = cN_PressfDBL - 3;
		}

		
		
		WhisperPartyPlayer( latestb[ i ] );
		break;

	case 1:

		if(num==-1) {

			if(cN_PressfDBL == ( cN_Pressf+fNameS)  ) {
				cN_PressfDBL = 0;
				cN_PressfDBL_Where = 0;
				break;
			}
			i = cN_Pressf - 3;
			i += fNameS;
			cN_Pressf = 0;
		}
		else {
			if(cN_Pressf != num){

				cN_Pressf = num;
				return;
			}
			i = num - 3;
			i += fNameS;
		}

		
		WhisperPartyPlayer( fNameb[ i ] );
		cN_PressfDBL_Where = 1;
		cN_PressfDBL = i+3;
		break;

	case 2:
		break;
	}
}

void HoParty_KTJ::chat_WhisperPartyPlayer_close()
{
	cN_PressfDBL =0;
}

void HoParty_KTJ::chat_changeMENU(int sw)
{
	cN_MenuN = sw;
}

void HoParty_KTJ::menuSet(int sw)
{
	int i;
	int menuButPosx;
	int menuButPosy;
	int qNamePosx;
	int qNamePosy;

	if(sw==3) {
		void CLANmenu_ChatList(int sw);
		CLANmenu_ChatList(1);
		return ;
	}
	int isClanWon = 0;
	

#ifdef  __CLANSUBCHIP__
	if(cldata.myPosition==100 || cldata.myPosition==101 || cldata.myPosition==104) {
#else
	if(cldata.myPosition==100 || cldata.myPosition==101 ) {
#endif
		isClanWon = 1;
	}

	if(sw==0) {

		chat_WhisperPartyPlayer_close();

		void chatlistSPEAKERflagChg(int flag);
		chatlistSPEAKERflagChg(0);


		menuPos[0].left  = BackStartPos.x+140;
		menuPos[0].top   = (BackStartPos.y-8)+10;
		menuPos[0].right = BackStartPos.x+(140+105);	
		menuPos[0].bottom= (BackStartPos.y-8)+30;

		menuPos[1].left  = -1;
		menuPos[1].top   = -1;
		menuPos[1].right = -1;
		menuPos[1].bottom= -1;	
		menuPos[2].left  = -1;
		menuPos[2].top   = -1;
		menuPos[2].right = -1;
		menuPos[2].bottom= -1;
	}
	else {

		chat_WhisperPartyPlayer_close();

		void chatlistSPEAKERflagChg(int flag);
		chatlistSPEAKERflagChg(0);


		menuPos[0].left  = -1;	//left
		menuPos[0].top   = -1;	//top
		menuPos[0].right = -1;	//right
		menuPos[0].bottom= -1;	//bottom

		savePmenuN = sw;

		menuPos[1].left  = BackStartPos.x+60;
		menuPos[1].top   = (BackStartPos.y-8)+10;
		menuPos[1].right = BackStartPos.x+120;	
		menuPos[1].bottom= (BackStartPos.y-8)+30;	


		menuPos[2].left  = BackStartPos.x+120;
		menuPos[2].top   = (BackStartPos.y-8)+10;
		menuPos[2].right = BackStartPos.x+180;
		menuPos[2].bottom= (BackStartPos.y-8)+30;
	}


	switch(sw) {
	case 0:

		menuSu = 3;

		menuButPosx =BackStartPos.x+100;
		menuButPosy =BackStartPos.y+220;
		for(i=0+3; i<5+3; i++) {
			menuPos[i].left  = menuButPosx+( (i-3)*23);		//left
			menuPos[i].top   = menuButPosy ;			//top
			menuPos[i].right = menuPos[i].left+20;		//right
			menuPos[i].bottom= menuPos[i].top +20;		//bottom
		}
		menuSu = 3+5;

		qNameS=0, qNameE=5;
		qNameSelN=0;
		
		qNameS_ENDs=0, qNameE_ENDs=5;
		

		qNamePosx =BackStartPos.x+110;
		qNamePosy =BackStartPos.y+40;

		for(i=0; i<5; i++) {
			menuPos[menuSu ].left  = qNamePosx;
			menuPos[menuSu ].top   = qNamePosy;
			menuPos[menuSu ].right = menuPos[menuSu ].left+(12*10);
			menuPos[menuSu ].bottom= menuPos[menuSu ].top +20;
			menuSu ++;
			qNamePosy += 30;
		}
		menuButPosx =BackStartPos.x-80;
		menuButPosy =BackStartPos.y+220;
		i=0;
		menuPos[menuSu ].left  = menuButPosx+( i*23);		
		menuPos[menuSu ].top   = menuButPosy ;
		menuPos[menuSu ].right = menuPos[menuSu ].left+34;
		menuPos[menuSu ].bottom= menuPos[menuSu ].top +20;
		menuSu ++;

		i=2;
		menuPos[menuSu ].left  = menuButPosx+( i*23);		
		menuPos[menuSu ].top   = menuButPosy ;
		menuPos[menuSu ].right = menuPos[menuSu ].left+20;
		menuPos[menuSu ].bottom= menuPos[menuSu ].top +20;
		menuSu ++;

		i=3;
		menuPos[menuSu ].left  = menuButPosx+( i*23);		
		menuPos[menuSu ].top   = menuButPosy ;
		menuPos[menuSu ].right = menuPos[menuSu ].left+20;
		menuPos[menuSu ].bottom= menuPos[menuSu ].top +20;
		menuSu ++;

		i=5;
		menuPos[menuSu ].left  = menuButPosx+( i*23) - 14;	
		menuPos[menuSu ].top   = menuButPosy ;			
		menuPos[menuSu ].right = menuPos[menuSu ].left+20;
		menuPos[menuSu ].bottom= menuPos[menuSu ].top +20;		//bottom
		menuSu ++;

		break;

	case 1:
		menuSu = 3;
		menuButPosx =BackStartPos.x+100;
		menuButPosy =BackStartPos.y+220;
		for(i=0+3; i<5+3; i++) {
			menuPos[i].left  = menuButPosx+( (i-3)*23);		//left
			menuPos[i].top   = menuButPosy ;			//top
			menuPos[i].right = menuPos[i].left+20;		//right
			menuPos[i].bottom= menuPos[i].top +20;		//bottom
		}
		menuSu = 3+5;


		if(isClanWon==1) {
			menuPos[menuSu ].left  = BackStartPos.x+180;
			menuPos[menuSu ].top   = (BackStartPos.y-8)+10;
			menuPos[menuSu ].right = BackStartPos.x+240;
			menuPos[menuSu ].bottom= (BackStartPos.y-8)+30;
			menuSu = 3+5+1;
		}
		
		break;


	case 2:
		menuSu = 3;
		menuButPosx =BackStartPos.x+100;
		menuButPosy =BackStartPos.y+55;
		for(i= 0; i < 7; i++) {
			menuPos[menuSu ].left  = menuButPosx;				//left
			menuPos[menuSu ].top   = menuButPosy;				//top
			menuPos[menuSu ].right = menuPos[menuSu ].left+(12*10);		//right
			menuPos[menuSu ].bottom= menuPos[menuSu ].top +18;		//bottom

			menuButPosy += 20;
			menuSu ++;
		}

		menuSu = 3+7;
		menuButPosx =BackStartPos.x+80;
		menuButPosy =BackStartPos.y+55;
		for(i= 0; i < 7; i++) {
			menuPos[menuSu ].left  = menuButPosx;				//left
			menuPos[menuSu ].top   = menuButPosy;				//top
			menuPos[menuSu ].right = menuPos[menuSu ].left+20;	//right
			menuPos[menuSu ].bottom= menuPos[menuSu ].top +18;		//bottom

			menuButPosy += 20;
			menuSu ++;
		}
	
		menuSu = 3+7+7;	
		menuButPosx =BackStartPos.x+85;
		menuButPosy =BackStartPos.y+220;

		for(i=0; i<3+ 4; i++) {	
			
			menuPos[menuSu ].left  = menuButPosx+( i*20);

			if(i>=3) menuPos[menuSu ].left +=5;

			menuPos[menuSu ].top   = menuButPosy ;				//top
			menuPos[menuSu ].right = menuPos[menuSu ].left+20;	//right
			menuPos[menuSu ].bottom= menuPos[menuSu ].top +20;	//bottom

			menuSu ++;
		}

		menuSu = 3+7+7 + 7;	

		if(isClanWon==1) {
			menuPos[menuSu ].left  = BackStartPos.x+180;
			menuPos[menuSu ].top   = (BackStartPos.y-8)+10;
			menuPos[menuSu ].right = BackStartPos.x+240;
			menuPos[menuSu ].bottom= (BackStartPos.y-8)+30;
			menuSu = 3+7+7 + 7 + 1;	
		}

		break;
	}
}


void HoParty_KTJ::Init()
{
	Load();
	
	memset(PartyMember, 0, sizeof(PartyMember));
	PartyMemberCount = 0;
	q_explationS = 0;
}



void HoParty_KTJ::Close()
{
	int i, index;
	for(i= 0; i < PARTY_BINFOsu; i++)
	{
		if(binfo_Bmp[i]) {
			binfo_Bmp[i]->Release();
			binfo_Bmp[i] = NULL;
		}
	}

	for(i= 0; i < PARTY_BUTTONsu; i++)
	{
		if(butt_Bmp[i]) {
			butt_Bmp[i]->Release();
			butt_Bmp[i] = NULL;
		}
	}
	
	if(BmpLifeBlue)
	{
		BmpLifeBlue->Release();
		BmpLifeBlue = NULL;
	}

	if(BmpLifeRed)
	{
		BmpLifeRed->Release();
		BmpLifeRed = NULL;
	}


	for(index = 0; index < 5; index++)
	{
		if(ToolTip[index] != NULL)
		{
			ToolTip[index]->Release();
			ToolTip[index] = NULL;
		}
	}

	for(index = 0; index < 4; index++)
	{
		if(Button[index] != NULL)
		{
			Button[index]->Release();
			Button[index] = NULL;
		}
	}

	for(index = 0; index < 2; index++)
	{
		if(BmpMessageOk[index] != NULL)
		{
			BmpMessageOk[index]->Release();
			BmpMessageOk[index] = NULL;
		}

		if(BmpMessageCancel[index] != NULL)
		{
			BmpMessageCancel[index]->Release();
			BmpMessageCancel[index] = NULL;
		}
	}
	

	for(index = 0; index < 3; index++)
	{
		if(BmpMan[index] != NULL)
		{
			BmpMan[index]->Release();
			BmpMan[index] = NULL;
		}
	}

}

void HoParty_KTJ::Load()
{
	int i, index;
	MatPartyBackGround = //CreateTextureMaterial( "Effect\\Party\\PartyBackGround.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
		CreateTextureMaterial( "image\\party\\party_win.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );



	MatTitle[0] =
		CreateTextureMaterial( "image\\party\\title-quest.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatTitle[1] =
		CreateTextureMaterial( "image\\party\\title-Party.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatTitle[2] =
		CreateTextureMaterial( "image\\party\\title-chatting.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatTitle[3] =
		CreateTextureMaterial( "image\\party\\quest_win.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	

	MatTitle[4] =
		CreateTextureMaterial( "image\\party\\title-NoClan_Party.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatTitle[5] =
		CreateTextureMaterial( "image\\party\\title-NoClan_chatting.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );

	char *binfoName[] = {	
	"ButtonInfo-1.bmp",	//0 이임
	"ButtonInfo-2.bmp",	//1 해산
	"ButtonInfo-3.bmp",	//2	추방
	"ButtonInfo-4.bmp",	//3	탈퇴
	"ButtonInfo-5.bmp",	//4	닫기

	"ButtonInfo-c0.bmp",	//5	채팅
	"ButtonInfo-c1.bmp",	//6	동료
	"ButtonInfo-c2.bmp",	//7 친구목록
	"ButtonInfo-c3.bmp",	//8	거부자
	"ButtonInfo-c4.bmp",	//9	최근목록
	"ButtonInfo-c5.bmp",	//10	길드	

	"ButtonInfo-q0.bmp",	//11	퀘스트
	"ButtonInfo-q1.bmp",	//12	아래
	"ButtonInfo-q2.bmp",	//13	위
	"ButtonInfo-q3.bmp",	//14	진행
	"ButtonInfo-q4.bmp",	//15	완수

	"ButtonInfo-qc.bmp",	//16	퀘스트취소
	"ButtonInfo-qc-nxt.bmp",//17	이전페이지
	"ButtonInfo-qc-pre.bmp",//18	다음페이지


	"ButtonInfo-c6.bmp",	//19	삭제(채팅에 관련된게 추가됨)
	"ButtonInfo-c7.bmp",	//20	거부(거부자등록)
	"ButtonInfo-c8.bmp",	//21	친구(친구등록)
	};	

	char buffer[256];		
	for(i= 0; i < PARTY_BINFOsu; i++)
	{
		memset(buffer, 0, sizeof(buffer));
		wsprintf(buffer, "image\\party\\%s", binfoName[i]);
		binfo_Bmp[i] = LoadDibSurfaceOffscreen(buffer);
	}

	char *iconName[] = {
	"ButtonCancel.bmp",			//0
	"ButtonCancel_0.bmp",		//1

	"chatting-icon_b.bmp",		//2
	"chattng-icon_off.bmp",		//3
	"chattng-icon_on.bmp",		//4
	"ButtonExit.bmp",			//5	
	"close_.bmp",				//6

	"icon-1.bmp",				//7
	"icon-1_.bmp",				//8
	"icon-2.bmp",				//9
	"icon-2_.bmp",				//10
	"icon-3.bmp",				//11
	"icon-3_.bmp",				//12
	"icon-4.bmp",				//13
	"icon-4_.bmp",				//14

	"icon-c1.bmp",				//15
	"icon-c1_.bmp",				//16
	"icon-c2.bmp",				//17
	"icon-c2_.bmp",				//18
	"icon-c3.bmp",				//19
	"icon-c3_.bmp",				//20
	"icon-c4.bmp",				//21
	"icon-c4_.bmp",				//22
	"icon-c5.bmp",				//23
	"icon-c5_.bmp",				//24

	"icon-q1.bmp",				//25
	"icon-q1_.bmp",				//26
	"icon-q2.bmp",				//27
	"icon-q2_.bmp",				//28
	"icon-q3.bmp",				//29
	"icon-q3_.bmp",				//30
	"icon-q4.bmp",				//31
	"icon-q4_.bmp",				//32

	"icon-qc.bmp",				//33
	"icon-qc_.bmp",				//34

	"party_icon.bmp",			//35
	"party_icon_0.bmp",			//36
	"party_man_0.bmp",			//37
	"party_man_1.bmp",			//38
	"party_man_jang.bmp",		//39

	"quest_icon-end.bmp",		//40
	"quest_icon-ing.bmp",		//41


	"LeftArrow.bmp",		//42
	"LeftArrow_.bmp",		//43
	"RightArrow.bmp",		//44
	"RightArrow_.bmp",		//45



	"icon-c6.bmp",
	"icon-c6_.bmp",

	"icon-7.bmp",
	"icon-7_.bmp",
	"icon-8.bmp",
	"icon-8_.bmp",
	"last_man.bmp",

	"chattng-icon-c.bmp",


	};
	for(i= 0; i < PARTY_BUTTONsu; i++)
	{
		if(i==48)
			i=48;
		memset(buffer, 0, sizeof(buffer));
		wsprintf(buffer, "image\\party\\%s", iconName[i]);
		butt_Bmp[i] = LoadDibSurfaceOffscreen(buffer);
	}

	BmpLifeBlue = LoadDibSurfaceOffscreen( "Effect\\Party\\LifeBlue.bmp");
	BmpLifeRed  = LoadDibSurfaceOffscreen( "Effect\\Party\\LifeRed.bmp");
		
	for(index = 0; index < 3; index++)
	{
		memset(buffer, 0, sizeof(buffer));
		wsprintf(buffer, "%s%d%s", "Effect\\Party\\Man", index+1, ".bmp");
		BmpMan[index] = LoadDibSurfaceOffscreen(buffer);
	}


	chatINI_read();
}

extern int	WhisperPartyPlayer( char *szName );
extern int	PartyButton( int Num , int Button );

int b_Nis[] = {25, 27, 29, 31, 5};
int bi_Nis[] = {18, 17, 14, 15, 4};
int b_Nflag[] = {0, 0, 0, 0, 0, 0, 0,0,0,0, 0,0,0,0,0,0,0,0,0};
int b2_Nflag[] = {0, 0, 0, 0, 0, 0, 0,0,0,0, 0,0,0,0,0,0,0,0,0};
int qN_flag[] = {0, 0, 0, 0, 0, 0, 0,0,0,0, 0,0,0,0,0,0,0,0,0};
int qN_Pressf=0;
int db_Nis[] = {7, 9, 11, 13, 5};
int dbi_Nis[] = {0, 1, 2, 3, 4};

int cb_Nis[] = {21, 17, 46,                    42, 50, 48, 5};		
int cbi_Nis[] = {9, 7,  8,                    17, 21, 20, 4};
int cb_Nis1[] = {21, 17, 46,                    42, 44, 19, 5};		
int cbi_Nis1[] = {9, 7,  8,                    17, 18, 19, 4};
int cb_Nis2[] = {21, 17, 46,                    42, 44, 19, 5};
int cbi_Nis2[] = {9, 7,  8,                    17, 18, 19, 4};

int QuestnotCharMove = 0;
int QuestnotCharMove2 = 0;

int HoParty_KTJ::Draw()
{
	int i, j, j2;
	if(GRAPHICDEVICE == NULL)
		return FALSE;
	int qNamePosx ;
	int qNamePosy ;
	int mSu ;

	if( PmenuN==1) goto menu_dongru;
	if( PmenuN==2) goto menu_frList;
	if( PmenuN==3) goto menu_clanList;
	
	if(qN_Pressf &&  MatTitle[3]!= -1)
		dsDrawTexImage( MatTitle[3], 
			BackStartPos.x-195, BackStartPos.y +15, 256, 256, 255);

	if(MatPartyBackGround != -1)
		dsDrawTexImage( MatPartyBackGround, 
			BackStartPos.x, BackStartPos.y, 256, 256, 255);
	
	if(MatTitle[PmenuN]!= -1)
		dsDrawTexImage( MatTitle[PmenuN], 
			BackStartPos.x, BackStartPos.y - 8,256, 32, 255);

	

	if(PartyPosState != PARTY_PROCESS) return 1;
	
	mSu = 3;
	for(i= 0; i < 5; i++)
	{
		if(b_Nflag[i])
		{
			DrawSprite(menuPos[mSu  ].left-30,  menuPos[mSu  ].top-30, binfo_Bmp[ bi_Nis[i] ],  0, 0, 76, 27,1);
			DrawSprite(menuPos[mSu  ].left, menuPos[mSu  ].top, butt_Bmp[  b_Nis[i]  ],  0, 0, 20, 20,1);
		}
		else
		{
			///if(i != 2 )
//			DrawSprite(menuPos[mSu  ].left,  menuPos[mSu  ].top, butt_Bmp[  28  ],  0, 0, 20, 20,1);
			DrawSprite(menuPos[mSu  ].left,  menuPos[mSu  ].top, butt_Bmp[  b_Nis[i]+1  ],  0, 0, 20, 20,1);
		}

		if((questSelN-3) == i)
		{
			DrawSprite(menuPos[mSu  ].left/*-30*/, menuPos[mSu  ].top, butt_Bmp[  b_Nis[i]  ],  0, 0, 20, 20,1);
		}
		mSu++;
	}

	qNamePosx =BackStartPos.x+116;
	qNamePosy =BackStartPos.y+40;
	HDC hdc = NULL;
	HFONT oldFont = NULL;

	for(i= qNameS; i < qNameE; i++) {

		if(qN_flag[i]) {
			SetFontTextColor( RGB(255, 255, 255));
		}
		else {
			SetFontTextColor( RGB(255, 190, 107));
			SetFontTextColor( RGB(255, 255, 255));
		}

		if(qN_Pressf &&  (((qN_Pressf-8)+qNameS)== i) )
			SetFontTextColor( RGB(255, 0, 0));
			



		if(questSelN == 5) {
#ifdef _LANGUAGE_ARGENTINA
			dsTextLineOut(hdc, qNamePosx-30, qNamePosy, qNameb[i], lstrlen(qNameb[i]));
#else
			dsTextLineOut(hdc, qNamePosx, qNamePosy, qNameb[i], lstrlen(qNameb[i]));
#endif
		}
		else {
#ifdef _LANGUAGE_ARGENTINA
			dsTextLineOut(hdc, qNamePosx-30, qNamePosy, qNameb_E[i], lstrlen(qNameb_E[i]));
#else
			dsTextLineOut(hdc, qNamePosx, qNamePosy, qNameb_E[i], lstrlen(qNameb_E[i]));
#endif
		}

		qNamePosy += 30;
	}
	if(oldFont)
		SelectObject( hdc, oldFont);

	qNamePosx =BackStartPos.x+110;
	qNamePosy =BackStartPos.y+40;
	for(i= qNameS; i < qNameE; i++) {


		if(questSelN == 5) {
			if(qNameb[i][0]==0) break;
		}
		else {
			if(qNameb_E[i][0]==0) break;
		}


		if(qN_flag[i]) {
#ifdef _LANGUAGE_ARGENTINA
			DrawSprite(qNamePosx-40, qNamePosy, butt_Bmp[ 40 ],  0, 0, 14, 15,1);
#else
			DrawSprite(qNamePosx-15, qNamePosy, butt_Bmp[ 40 ],  0, 0, 14, 15,1);
#endif
		}
		else {
#ifdef _LANGUAGE_ARGENTINA
			DrawSprite(qNamePosx-40, qNamePosy, butt_Bmp[ 41 ],  0, 0, 14, 15,1);
#else
			DrawSprite(qNamePosx-15, qNamePosy, butt_Bmp[ 41 ],  0, 0, 14, 15,1);
#endif
		}
		qNamePosy += 30;
	}

	qNamePosx =BackStartPos.x-80;
	qNamePosy =BackStartPos.y+50;

	if(qN_Pressf==0) {
		QuestnotCharMove = 0;
		QuestnotCharMove2= 0;
		return 1;
	}
#ifdef _LANGUAGE_ARGENTINA
	Draw_questExplane(BackStartPos.x-120, BackStartPos.y+55);
#else
	Draw_questExplane(BackStartPos.x-105, BackStartPos.y+55);
#endif

	QuestnotCharMove = 1;
	QuestnotCharMove2= 1;

	static int bN[] ={33, 42, 44, 5};
	static int bSize[] ={34, 20, 20, 20};

	static int bi_Nis2[] = {0, 18, 17, 4};


    mSu = 3+5+5;
	
	for(i= 0; i < 4; i++)
	{
		if(i==0) {
			goto jmp111;
		}

		if(b2_Nflag[i]) 
		{
			DrawSprite(menuPos[mSu  ].left-30,  menuPos[mSu  ].top-30,
				binfo_Bmp[ bi_Nis2[i] ],  0, 0, 76, 27,1);

			DrawSprite(menuPos[mSu ].left, menuPos[mSu ].top, 
				butt_Bmp[  bN[i]  ],  0, 0, bSize[i], 20,1);

		}
		else {
				DrawSprite(menuPos[mSu ].left, menuPos[mSu ].top, 
					butt_Bmp[  bN[i]+1  ],  0, 0, bSize[i], 20,1);
		}

jmp111:
		mSu ++;
	}
	
	return 1;

menu_dongru:
	if(MatPartyBackGround != -1)
		dsDrawTexImage( MatPartyBackGround, 
			BackStartPos.x, BackStartPos.y, 
			256, 256, 255);
#ifdef  __CLANSUBCHIP__
	if(cldata.myPosition==100 || cldata.myPosition==101 || cldata.myPosition==104) 
#else
	if(cldata.myPosition==100 || cldata.myPosition==101) 
#endif

	{

		if(MatTitle[PmenuN]!= -1)
			dsDrawTexImage( MatTitle[PmenuN],    BackStartPos.x, BackStartPos.y - 8,   256, 32, 255);
		
	}
	else {
		if(MatTitle[4]!= -1)
			dsDrawTexImage( MatTitle[4],   BackStartPos.x, BackStartPos.y - 8,    256, 32, 255);
	}

	if(PartyPosState != PARTY_PROCESS) return 1;
	
	TextStartPos.x = BackStartPos.x+80;
	TextStartPos.y = BackStartPos.y+38;

	for(int index = 0; index < PartyMemberCount; index++)
	{
		if(BmpLifeBlue != NULL && BmpLifeRed != NULL)
		{
			DrawSprite(TextStartPos.x-3+14, TextStartPos.y+30*index-2+15, BmpLifeRed,  0, 0, 94, 4, 1);
			DrawSprite(TextStartPos.x-3+14, TextStartPos.y+30*index-2+15, BmpLifeBlue, 0, 0, int((94.f/100.f)*(float)PartyMember[index].Life), 4, 1);
		}
			
			
		if(pCursorPos.x > TextStartPos.x && pCursorPos.y > TextStartPos.y+30*index-10 &&
		   pCursorPos.x < TextStartPos.x +110 && pCursorPos.y < TextStartPos.y+30*index+18)
		{
			
			if(BmpMan[1] != NULL && PartyMember[index].JangFlag == FALSE)
				DrawSprite(TextStartPos.x-4, TextStartPos.y+30*index-2, BmpMan[1],  0, 0, 13, 17,1);
				
			if(BmpMan[3] != NULL && PartyMember[index].JangFlag)
				DrawSprite(TextStartPos.x-4, TextStartPos.y+30*index-2, BmpMan[2],  0, 0, 13, 17,1);
			
			if(MouseButton[0] && PartyPosState == PARTY_PROCESS)
			{
				MouseDownPartyMember = index;
				
				if(strlen(PartyMember[MouseDownPartyMember].Name) != 0 &&
				   strlen(PartyMember[MouseDownPartyMember].ModelName1) != 0 &&
				   strlen(PartyMember[MouseDownPartyMember].ModelName2) != 0 && faceFlag == FALSE)
				{
					if(faceFlag == FALSE)
					{
						faceFlag = TRUE;
						OpenPartyPlayer(PartyMember[MouseDownPartyMember].Name, PartyMember[MouseDownPartyMember].ModelName1, PartyMember[MouseDownPartyMember].ModelName2);
					}
				}
			}
				
			if(MouseDblClick && whisperFlag == FALSE)
			{
				if(lpCurPlayer->dwObjectSerial != PartyMember[index].ChrCode )
				{
					whisperFlag = TRUE;
					WhisperPartyPlayer(PartyMember[index].Name);
				}
			}
			
		}
		else
		{
			
			if(BmpMan[0] != NULL && PartyMember[index].JangFlag == FALSE)
				DrawSprite(TextStartPos.x-4, TextStartPos.y+30*index-2, BmpMan[0],  0, 0, 13, 17,1);
			
			if(BmpMan[3] != NULL && PartyMember[index].JangFlag)
				DrawSprite(TextStartPos.x-4, TextStartPos.y+30*index-2, BmpMan[2],  0, 0, 13, 17,1);
		}
	}


		if(PartyPosState == PARTY_PROCESS)
		{
			if(MouseDownPartyMember != -1 && PartyMember[MouseDownPartyMember].JangFlag == FALSE)
				DrawSprite(TextStartPos.x-4, TextStartPos.y+30*MouseDownPartyMember-2, BmpMan[1],  0, 0, 13, 17,1);
			if(MouseDownPartyMember != -1 && PartyMember[MouseDownPartyMember].JangFlag == TRUE)
				DrawSprite(TextStartPos.x-4, TextStartPos.y+30*MouseDownPartyMember-2, BmpMan[2],  0, 0, 13, 17,1);
		}

		if (hFont != NULL)
		{
			hdc = NULL;
			oldFont = (HFONT)SelectObject(hdc, hFont);
		}

		char levelBuffer[256];
		for ( int index = 0; index < PartyMemberCount; index++ )
		{
			SetFontTextColor( RGB(0, 0, 0));
			dsTextLineOut(hdc, TextStartPos.x+11, TextStartPos.y+30*index+1, PartyMember[index].Name, lstrlen(PartyMember[index].Name));
			memset(levelBuffer, 0, sizeof(levelBuffer));

			if(PartyMember[index].Level > 10)
				wsprintf(levelBuffer, "%s%d%s", "L",PartyMember[index].Level/10,"x");
			else
				wsprintf(levelBuffer, "%s%d", "L", PartyMember[index].Level);
			dsTextLineOut(hdc, TextStartPos.x+110, TextStartPos.y+5+30*index+1, levelBuffer, lstrlen(levelBuffer));
		}
		
	
		for ( int index = 0; index < PartyMemberCount; index++ )
		{
			if( MouseDownPartyMember != index)
				SetFontTextColor( RGB(209, 190, 154));		
			else
				SetFontTextColor( RGB(255, 150, 0));
			
			if(pCursorPos.x > TextStartPos.x && pCursorPos.y > TextStartPos.y+30*index-10 &&
			   pCursorPos.x < TextStartPos.x +110 && pCursorPos.y < TextStartPos.y+30*index+18)
				SetFontTextColor( RGB(255, 150, 0));
			dsTextLineOut(hdc,TextStartPos.x+10, TextStartPos.y+30*index, PartyMember[index].Name, lstrlen(PartyMember[index].Name));

			memset(levelBuffer, 0, sizeof(levelBuffer));
			
			if(PartyMember[index].Level > 10)
				wsprintf(levelBuffer, "%s%d%s", "L",PartyMember[index].Level/10,"x");
			else
				wsprintf(levelBuffer, "%s%d", "L", PartyMember[index].Level);

			SetFontTextColor( RGB(255, 190, 107));
			dsTextLineOut(hdc, TextStartPos.x+110, TextStartPos.y+5+30*index, levelBuffer, lstrlen(levelBuffer));
		}
		if(oldFont)
			SelectObject( hdc, oldFont);
		//lpDDSBack->ReleaseDC(hdc);
		

		mSu = 3;
		for(i= 0; i < 5; i++)
		{
			if(b_Nflag[i])	{
				if(lpCurPlayer->dwObjectSerial != PartyMember[0].ChrCode )
				{
					if(i == 0 || i == 1 || i == 2)
						goto jump0;
				}

				if(PartyMemberCount == 0)
				{
					if(i == 0 || i == 1 || i == 2 || i == 3)
						goto jump0;
				}

				DrawSprite(menuPos[mSu  ].left-30, menuPos[mSu  ].top-30, 
					binfo_Bmp[ dbi_Nis[i] ],  0, 0, 76, 27,1);

				DrawSprite(menuPos[mSu  ].left, menuPos[mSu  ].top, butt_Bmp[  db_Nis[i]  ],  0, 0, 20, 20,1);
			}
			else {
	jump0:
				DrawSprite(menuPos[mSu  ].left, menuPos[mSu  ].top, butt_Bmp[  db_Nis[i]+1  ],  0, 0, 20, 20,1);
			}


			mSu++;
		}

		for ( int index = 0; index < 5; index++ )
		{
			if(menuPos[3+index].left <= pCursorPos.x  && pCursorPos.x <=  menuPos[3+index].right &&
				menuPos[3+index].top <= pCursorPos.y  && pCursorPos.y <=  menuPos[3+index].bottom)
			{
				if(lpCurPlayer->dwObjectSerial != PartyMember[0].ChrCode)
				{
					if(index == 0 || index == 1 || index == 2)
						continue;
				}
				
				if(PartyMemberCount == 0 && index != 4)
					continue;
				DrawSprite(BackStartPos.x+110+index*23, BackStartPos.y+220, Button[index],  0, 0, 20, 20,1);
				
				if(MouseButton[0] && PartyPosState == PARTY_PROCESS)
				{
					
					if(buttonMessageFlag == FALSE)
					{
						MouseDownButton = index;
						buttonMessageFlag = TRUE;
						PartyButton(MouseDownPartyMember, index);
						MouseDownPartyMember = -1;
					}
					
					if(MouseDownButton == 4)
						PartyPosState = PARTY_END;
				}
			}
		}

		
	return 1;


menu_frList:

	if(MatPartyBackGround != -1)
		dsDrawTexImage( MatPartyBackGround, 
			BackStartPos.x, BackStartPos.y, 
			256, 256, 255);

#ifdef  __CLANSUBCHIP__
	if(cldata.myPosition==100 || cldata.myPosition==101 || cldata.myPosition==104) 
#else
	if(cldata.myPosition==100 || cldata.myPosition==101 ) 
#endif

	{
		if(MatTitle[PmenuN]!= -1)
			dsDrawTexImage( MatTitle[PmenuN],   BackStartPos.x, BackStartPos.y - 8,   256, 32, 255);
		
	}
	else {
		if(MatTitle[5]!= -1)
			dsDrawTexImage( MatTitle[5],    BackStartPos.x, BackStartPos.y - 8,   256, 32, 255);
	}

	mSu = 3;
	if(PartyPosState != PARTY_PROCESS) return 1;


	int *intp_N, *intp_Ni;
	if(cN_MenuN == 0) {
		intp_N = cb_Nis;
		intp_Ni = cbi_Nis;
	}
	else if(cN_MenuN == 1) {
		intp_N = cb_Nis1;
		intp_Ni = cbi_Nis1;
	}
	else {
		intp_N = cb_Nis2;
		intp_Ni = cbi_Nis2;
	}

	mSu = 3+7+7;
	for(i= 0; i < 3+4; i++)
	{
		if(cN_MenuN == 0 && i==3)
				goto jmp1;



		if(cN_MenuN == 0 && i==0) goto jmp1_1;
		if(cN_MenuN == 1 && i==1) goto jmp1_1;
		if(cN_MenuN == 2 && i==2) goto jmp1_1;


		if(b_Nflag[i])	{

			DrawSprite(menuPos[mSu  ].left-30, menuPos[mSu  ].top-26,
				binfo_Bmp[ intp_Ni[i] ],  0, 0, 76, 27,1);
jmp1_1:
			DrawSprite(menuPos[mSu  ].left, menuPos[mSu  ].top,
				butt_Bmp[  intp_N[i]  ],  0, 0, 20, 20,1);
		}
		else {
			DrawSprite(menuPos[mSu  ].left, menuPos[mSu  ].top,
				butt_Bmp[  intp_N[i]+1  ],  0, 0, 20, 20,1);
		}

jmp1:
		mSu++;
	}
	
	mSu = 3;

	//if(hFont != NULL)
		//oldFont = (HFONT)SelectObject( hdc , hFont );

	SetFontTextColor( RGB(255, 255, 255));
	if(cN_MenuN == 0) {	
	
		SetFontTextColor( RGB(198, 184, 171));
		hdc = NULL;
		dsTextLineOut(hdc, menuPos[mSu  ].left+26, menuPos[mSu  ].top-24, HoTextFriendNear, lstrlen(HoTextFriendNear) );

		j2=-1;
		for(i= 0; i < 7; i++) {

			if(cN_Pressf==0)
				j=-1;
			else
				j = cN_Pressf-3;

			if(cN_PressfDBL && cN_PressfDBL_Where== 0)
				j2 = cN_PressfDBL-3;


			
			if(j2 == i){
				SetFontTextColor( RGB(255, 0, 0));
			}
			else if(j == i){
				SetFontTextColor( RGB(255, 200, 100));
			}
			else {
				SetFontTextColor( RGB(255, 255, 255));
			}

			dsTextLineOut(hdc, menuPos[mSu  ].left, menuPos[mSu  ].top, latestb[i], lstrlen(latestb[i]));


			mSu++;
		}



	}
	else if(cN_MenuN == 1) {

		int max = (fNameSu/7);
		if(fNameSu%7) max++;

		int min = (fNameS/7);
		if(fNameSu) min++;

		char buf[128];
		wsprintf(buf, HoTextFriendList , min, max  );

		SetFontTextColor(RGB(198, 184, 171));
		hdc = NULL;
		dsTextLineOut(hdc, menuPos[mSu  ].left+21, menuPos[mSu  ].top-24, buf, strlen(buf) );

		
		for(i= fNameS; i < fNameE; i++) {
			j=-1;
			if(cN_Pressf) {
				j = cN_Pressf-3;
				j += fNameS;
			}
			j2=-1;
			if(cN_PressfDBL && cN_PressfDBL_Where== 1) {
				j2 = cN_PressfDBL-3;
			}

			if(j2 == i){
				SetFontTextColor( RGB(255, 0, 0));
			}
			else if(j == i){
				SetFontTextColor( RGB(255, 200, 100));
			}
			else 
				SetFontTextColor( RGB(255, 255, 255));
			dsTextLineOut(hdc, menuPos[mSu  ].left, menuPos[mSu  ].top, fNameb[i], lstrlen(fNameb[i]));

			mSu++;
		}


	}
	else {
		char buf[128];
		int max = (dNameSu/7);
		if(dNameSu%7) max++;

		int min = (dNameS/7);
		if(dNameSu) min++;
		wsprintf(buf, HoTextFriendRefuse , min, max  );

		hdc = NULL;
		SetFontTextColor(RGB(198, 184, 171));
		dsTextLineOut(hdc, menuPos[mSu  ].left+16, menuPos[mSu  ].top-24, buf, strlen(buf) );

		if(dNameS>13){
			dNameS = dNameS;
		}

		for(i= dNameS; i < dNameE; i++) {

			j = -1;
			if(cN_Pressf) {
				j = cN_Pressf-3;
			}

			if(j == i){
				SetFontTextColor( RGB(255, 200, 100));
			}
			else 
				SetFontTextColor( RGB(255, 255, 255));
			dsTextLineOut(hdc, menuPos[mSu  ].left, menuPos[mSu  ].top, dNameb[i], lstrlen(dNameb[i]));

			mSu++;
		}

	}

	mSu = 3;

	if(cN_MenuN == 0) {	
		
		j = cN_PressfDBL-3;
		for(i= 0; i < 7; i++) {
	
			if( latestb[ i ][0]==0) goto jmp10;


			if(cN_PressfDBL && cN_PressfDBL_Where== 0) {
				if(j==i) {
					DrawSprite(	menuPos[mSu  ].left - 23, menuPos[mSu  ].top-3, 
						butt_Bmp[ 53 ],  0, 0, 16, 16,1);
					goto jmp10;
				}
			}

			DrawSprite(	menuPos[mSu  ].left - 23, menuPos[mSu  ].top-5, 
					butt_Bmp[ 52 ],  0, 0, 20, 20,1);
	jmp10:

			mSu++;
		}
	}



	if(cN_MenuN == 1) {	
		
		for(i= fNameS; i < fNameE; i++) {
	
			if( fNameb[ i ][0]==0) goto jmp11;


			if(cN_PressfDBL && cN_PressfDBL_Where== 1) {
				j = cN_PressfDBL-3;
				if(j==i) {
					DrawSprite(	menuPos[mSu  ].left - 23, menuPos[mSu  ].top-3, 
						butt_Bmp[ 53 ],  0, 0, 16, 16,1);
					goto jmp11;
				}
			}
				DrawSprite(	menuPos[mSu  ].left - 23, menuPos[mSu  ].top-2, 
							butt_Bmp[ 4 ],  0, 0, 16, 16,1);

	jmp11:		

			mSu++;
		}
	}

	if(cN_MenuN == 2) {	

		for(i= dNameS; i < dNameE; i++) {
			if( dNameb[ i ][0]==0) goto jmp12;

				DrawSprite(	menuPos[mSu  ].left - 23, menuPos[mSu  ].top-2, 
							butt_Bmp[ 2 ],  0, 0, 16, 16,1);

	jmp12:
			mSu++;
		}
	}

	return TRUE;

menu_clanList:

	void CLANmenu_ChatList_Draw();
	CLANmenu_ChatList_Draw();

	return TRUE;

}

int HoParty_KTJ::Main_menuSet(int sw)
{
	PmenuN = sw;
	BackStartPos.x = 539;
	PartyPosState = PARTY_PROCESS;
	menuSet(PmenuN);
	return 1;
}



void but_exit_chk()
{
	extern int ret_menuN;
	ret_menuN  = 0;
	InterfaceParty_KTJ.PmenuN=2;
}




int HoParty_KTJ::Main(int sw)
{
	int i;
	char namebuf[256];



	PartyPosState = sw;


	if(PartyPosState == PARTY_START)
	{
		

		//동료 메뉴열경우 파티에 사람이 없으면 친구메뉴로 바꾼다.
		if(PmenuN==1 && PartyMember[0].Name[0]==0)
			PmenuN  = 2;


		//BackStartPos.x-= 15;
		BackStartPos.x-= 20;
		//if(BackStartPos.x <= 549)
		if(BackStartPos.x <= 539)
		//if(BackStartPos.x <= xLeft)
		{
			//BackStartPos.x = 549;
			BackStartPos.x = 539;
			//BackStartPos.x = xLeft;
			PartyPosState = PARTY_PROCESS;
		}
		menuSet(PmenuN);




		for(i=0; i<8; i++) {
			b_Nflag[i] = 0;	//마우스가 위에 있는지의 여부를 0으로 한다.
			b2_Nflag[i] = 0;	//마우스가 위에 있는지의 여부를 0으로 한다.

			if(i<5)
				qN_flag[i] = 0;
		}



		//TextStartPos.x = BackStartPos.x+102;		
	}
	else if(PartyPosState == PARTY_END)
	{
		
		//BackStartPos.x += 15;
		BackStartPos.x += 20;
		if(BackStartPos.x >= 800)
		//if(BackStartPos.x >= xRight)
		{
			MouseDownPartyMember = -1;
			MouseDownButton = -1;
			BackStartPos.x = 800;
			//BackStartPos.x = xRight;
			PartyPosState = PARTY_NONE;


			qN_Pressf_CLEAR();
		}
		//TextStartPos.x = BackStartPos.x+102;
		//menuSet(PmenuN);
		
	}
	


	switch(sw) {
	case PARTY_START:
		break;

	case PARTY_PROCESS:

		for(i=0; i<8; i++) {
			b_Nflag[i] = 0;	//마우스가 위에 있는지의 여부를 0으로 한다.
			b2_Nflag[i] = 0;	//마우스가 위에 있는지의 여부를 0으로 한다.

			if(i<5)
				qN_flag[i] = 0;
		}


		i = chkeckMenuN();

		//파티메뉴번호
		if(PmenuN==0) {			//0:퀘스트메뉴
			switch(i) {
			case 3:				//퀘스트메뉴의 아래버튼들
			case 4:
			case 5:
			case 6:
			case 7:
				b_Nflag[i-3] = 1;	//마우스가 위에 있는지의 여부를 0으로 한다.
				break;

			case 8:				//퀘스트의 이름들(1차전업시험, 스킬마스터하기등의 퀘스트이름임)
			case 9:
			case 10:
			case 11:
			case 12:
				qN_flag[i-8] = 0;	//마우스가 위에 있는지의 여부를 0으로 한다.
				break;

			//============================================= 퀘스트의 왼쪽메뉴의 버튼들 === start
			case 13:
				//b2_Nflag[ (i-13) ] = 1;	//마우스가 위에 있는지의 여부를 0으로 한다.
				break;
			case 14:	//left 버튼임.
				b2_Nflag[ (i-13) ] = 1;	
				break;
			case 15:	//right 버튼임.
				b2_Nflag[ (i-13) ] = 1;	
				break;
			case 16:
				b2_Nflag[ (i-13) ] = 1;	
				break;
			//============================================= 퀘스트의 왼쪽메뉴의 버튼들 === end
			}

			if(MouseButton[0])			//눌렀을경우
			{		
				if(i!= -1)
            		MouseButton[0]=0;
				switch(i) {
				case -1: break;			//제대로 안눌른것임.
				case 0:	break;			//헤드부분의 퀘스트, 동료, 채팅 누름
				case 1:
				case 2:
					PmenuN=i;
					menuSet(PmenuN);		//메뉴가 틀려서 다시 셋팅함.
					break;


				case 3:					//퀘스트메뉴의 아래버튼들

					if(questSelN == 5) {
						qNameS -= 8;
						if(qNameS<0)qNameS=0;
						qNameE = qNameS+5;
					}
					else {
						qNameS_ENDs -= 8;
						if(qNameS_ENDs<0)qNameS_ENDs=0;
						qNameE_ENDs = qNameS_ENDs+5;
					}

					break;
				case 4:		//right 버튼임.
					if(questSelN == 5) {
						qNameS += 8;
						if(qNameb[qNameS ][0] == 0){		//공백이면 원래대로 돌린다.
							qNameS-=8;
						}
						qNameE = qNameS+5;
					}
					else {

						qNameS_ENDs += 8;
						if(qNameb_E[qNameS ][0] == 0){		//공백이면 원래대로 돌린다.
							qNameS_ENDs-=8;
						}
						qNameE_ENDs = qNameS_ENDs+5;

					}

					break;





					//=======진행하는곳
				case 5:		//진행
					if(questSelN == 5) break;		//원래 눌려있으면 안함.
					questSelN = 5;
					qN_Pressf = 0;

					break;


				case 6:		//완수
					if(questSelN == 6) break;		//원래 눌려있으면 안함.
					//if(qN_Pressf==0) 
						questSelN = 6;
						qN_Pressf = 0;
					break;

				case 7:
					//나가기버튼누른경우임.
					qN_Pressf=0;				//옆에 나온창 없앤다.
					return PARTY_END;
					
				//==== 퀘스트의 이름들(1차전업시험, 스킬마스터하기등의 퀘스트이름임) start
				case 8:					
				case 9:
				case 10:
				case 11:
				case 12:
					if(questSelN == 5) 
					{				
						if(qNameb[ qNameS + (i-8) ][0]==0)	break;	//이름이 없으면 안한다
						if(qN_Pressf==i) {
							qN_Pressf=0;	break;
						}
						qN_Pressf = i;

						//퀘스트의 이름을 세이브해 왼쪽해설메뉴에 넣는다.
						strcpy( q_explationTitle, qNameb[ qNameS + (i-8) ]);
						
						//퀘스트 해설파일을 메모리에 넣는다.
						wsprintf(namebuf, ".\\image\\party\\party_DATA\\%s", qFnameb[   qNameS + (i-8)  ]);
						if(ReadFileData(namebuf) == 0) {
							ReadFileData(".\\image\\party\\party_DATA\\quest_imsi.txt");	//파일이 없을때..
						}
						ZeroMemory((void *)q_explation, sizeof(q_explation));
						ReadFileData_moveBuf( (char *)q_explation, 128 );


					}
					else {

						if(qNameb_E[ qNameS_ENDs  + (i-8) ][0]==0)	break;	//이름이 없으면 안한다
						if(qN_Pressf==i) {
							qN_Pressf=0;	break;
						}
						qN_Pressf = i;

						//퀘스트의 이름을 세이브해 왼쪽해설메뉴에 넣는다.
						strcpy( q_explationTitle, qNameb_E[ qNameS_ENDs  + (i-8) ]);
						
						//퀘스트 해설파일을 메모리에 넣는다.
						wsprintf(namebuf, ".\\image\\party\\party_DATA\\%s", qFnameb_E[   qNameS_ENDs  + (i-8)  ]);
						if(ReadFileData(namebuf) == 0) {
							ReadFileData(".\\image\\party\\party_DATA\\quest_imsi.txt");	//파일이 없을때..
						}
						ZeroMemory((void *)q_explation, sizeof(q_explation));
						ReadFileData_moveBuf( (char *)q_explation, 128 );
					}



					break;
				//==== 퀘스트의 이름들(1차전업시험, 스킬마스터하기등의 퀘스트이름임) end


				//============================================= 퀘스트의 왼쪽메뉴의 버튼들 === start
				case 13:
					break;

				case 14:	//left 버튼임.
					if(qN_Pressf==0) break;
					q_explationS -= 8;
					if(q_explationS<0)q_explationS=0;
					break;
				case 15:	//right 버튼임.
					if(qN_Pressf==0) break;

					q_explationS += 8;
					if(q_explation[ q_explationS ][0] == 0){		//공백이면 원래대로 돌린다.
						q_explationS-=8;
					}
					break;
				case 16:
					b2_Nflag[ (i-13) ] = 1;

					qN_Pressf=0;//왼쪽창에서 빠져나간다.
					break;
				//============================================= 퀘스트의 왼쪽메뉴의 버튼들 === end
				}
			}

		}
		else if(PmenuN==1)		//1:동료
		{
			switch(i) {
			case 3:				//파티메뉴의 아래버튼들
			case 4:
			case 5:
			case 6:
			case 7:
				b_Nflag[i-3] = 1;	//마우스가 위에 있는지의 여부를 0으로 한다.
				break;
			}

			if(MouseButton[0]) {		//눌렀을경우
				//if(i!= -1)
            	//	MouseButton[0]=0;
				
				switch(i) {
				case -1: break;		//제대로 안눌른것임.
				case 1:	break;
				case 0:				//헤드부분의 퀘스트, 동료, 채팅 누름
				case 2:
				case 8:
					if(i==8) i=3;	//ktj : 마지막에 넣은거라 3으로 고침
					PmenuN=i;
					menuSet(PmenuN);	//메뉴가 틀려서 다시 셋팅함.

					
					break;


				case 3:				//파티메뉴의 아래버튼들
				case 4:
				case 5:
				case 6:	//탈퇴버튼
					//MouseButton[0]=0;
					break;
				case 7:	//나가기버튼누른경우임.
					MouseButton[0]=0;
					return PARTY_END;
				}
			}


		}
		else if(PmenuN==2)		//2:친구
		{
			//친구 메뉴 부분 ================ start
			switch(i) {
			case 3:				//친구의 id들
			case 4:

			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				break;




			case 17:
			case 18:				//채팅메뉴의 아래버튼들
			case 19:

			case 20:
			case 21:
			case 22:
			case 23:
				b_Nflag[i-17] = 1;	//마우스가 위에 있는지의 여부를 0으로 한다.
				break;
			}









			if(MouseButton[0]) {		//눌렀을경우
				if(i!= -1)
            		MouseButton[0]=0;
				switch(i) {
				case -1: break;		//제대로 안눌른것임.
				case 2:  break;
				case 0:				//헤드부분의 퀘스트, 동료, 채팅 누름
				case 1:
				case 24:
					if(i==24) i=3;	//ktj : 마지막에 넣은거라 3으로 고침
					if(cN_PressfDBL != 0) break;

					PmenuN=i;
					menuSet(PmenuN);	//메뉴가 틀려서 다시 셋팅함.
					break;


				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:	//채팅..눌린거 id부분임.
					//cN_Pressf = i;
					chat_cN_Pressf(i);	//cN_Pressf 눌린번호 셋팅(빈 부분은 셋팅이 안되기 때문임
					break;

				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:	//id앞의 표시
					chat_WhisperPartyPlayer( (i-10)+3);
					break;




				case 17:			//최근목록
					cN_MenuN = 0;
					cN_Pressf = 0;
					break;
				case 18:			//친구 목록
					cN_MenuN = 1;
					cN_Pressf = 0;
					
					break;
				case 19:
					cN_MenuN = 2;	//거부자목록
					cN_Pressf = 0;
					break;
			
				case 20:			//채팅의 페이지바꿈버튼 left
					//if(cN_PressfDBL != 0) break;
					chat_PageChange(0);
					cN_Pressf = 0;
					break;
				case 21:			//채팅의 페이지바꿈버튼 right     , 친구목록등록용의 2가지용도
					//if(cN_PressfDBL != 0) break;
					chat_PageChange(1);

					chat_IDinsert(1, NULL);	//cN_MenuN==0 일때만 친구들목록에 등록용으로 사용됨
					cN_Pressf = 0;
					break;

				case 22:	//삭제			, 거부자목록등록용의 2가지용도
					chat_IDdelete();

					chat_IDinsert(2, NULL);	//cN_MenuN==0 일때만 친구들목록에 등록용으로 사용됨
					//chat_IDinsert(3, "옴마냐3");	//cN_MenuN==0 일때만 친구들목록에 등록용으로 사용됨
					cN_Pressf = 0;
					cN_PressfDBL = 0;
					break;
				case 23:	//닫기
					cN_Pressf = 
					cN_PressfDBL = 0;
					return PARTY_END;
				}
			}


			
			
			//귀말 보내기...
			if(MouseDblClick)
			{
				switch(i) {
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:	//채팅..눌린거
					//cN_Pressf = i;
					//chat_cN_Pressf(i);	//cN_Pressf 눌린번호 셋팅(빈 부분은 셋팅이 안되기 때문임
					//WhisperPartyPlayer("옴마니아");
					chat_WhisperPartyPlayer(-1);
					MouseDblClick=0;					
					break;



				}
			}
			//친구 메뉴 부분 ================ end
		}
		else {	//if(PmenuN==3)		//클랜원리스트

			void CLANmenu_ChatList_Main();
			CLANmenu_ChatList_Main();
			extern int ret_menuN;
			switch(ret_menuN ) 
			{
			case 1:		//친구
			case 2:		//동료
				PmenuN=ret_menuN ;
				menuSet(PmenuN);		//메뉴가 틀려서 다시 셋팅함.
				break;
			case 3:						//본 메뉴를 닫는다.

				ret_menuN  = 0;
				PmenuN=2;
				//나가기버튼누른경우임.
				MouseButton[0]=0;

				void cN_PressfDBL_clear();	//ce_chatlist.cpp의 변수클리어
				cN_PressfDBL_clear();
				return PARTY_END;
			}
		}

		//여기는 마우스가 메뉴의 헤드를 눌러 퀘, 동, 채 선택하는 부분 ========== end
		
		break;

	case PARTY_END:
		break;
	}

	return TRUE;
}



//ktj : 주의 : 이함수는 사용안함.
int HoParty_KTJ::AddMember(char *memberName,DWORD chrCode, int life, int level)
{
	if(memberName == NULL)
		return FALSE;
	
	if(PartyMemberCount >= MAX_PARTY_MEMBER)
		return FALSE;
	memcpy(PartyMember[PartyMemberCount].Name, memberName, strlen(memberName));
	PartyMember[PartyMemberCount].ChrCode = chrCode;
	PartyMember[PartyMemberCount].Life = life;
	PartyMember[PartyMemberCount].Level = 100;
	PartyMemberCount++;
	return TRUE;
}


//ktj : 새로 만듦. ============== start
int HoParty_KTJ::AddMember(hoPartyMember *member,int cnt)
{
	PartyMember[cnt].ChrCode = member->ChrCode;
	PartyMember[cnt].Life  = member->Life;
	PartyMember[cnt].Level = member->Level;
	PartyMember[cnt].CharacterPos.x = member->CharacterPos.x;
	PartyMember[cnt].CharacterPos.z = member->CharacterPos.z;
	PartyMember[cnt].CharacterPos.y = member->CharacterPos.y;
	lstrcpy( PartyMember[cnt].Name , member->Name );
	lstrcpy( PartyMember[cnt].ModelName1 , member->ModelName1 );
	lstrcpy( PartyMember[cnt].ModelName2 , member->ModelName2 );
	PartyMember[cnt].JangFlag = member->JangFlag ;
				
	return TRUE;
}

void HoParty_KTJ::AddMember_memberSu(int memberSu)
{
    PartyMemberCount = memberSu;
}

int HoParty_KTJ::PARTY_PLAYUPDATE(hoPartyMember *member,int cnt)
{
	if ( PartyMember[cnt].ChrCode==member->ChrCode) {
		PartyMember[cnt].Life  = member->Life;
		PartyMember[cnt].Level = member->Level;
		PartyMember[cnt].CharacterPos.x = member->CharacterPos.x;
		PartyMember[cnt].CharacterPos.z = member->CharacterPos.z;
		return 1;
	}
	return 0;
}


int HoParty_KTJ::PARTY_RELEASE()
{
	PartyMemberCount = 0;
	MouseDownPartyMember = -1;

	if(PmenuN==0) return PmenuN;


	//ktj : 클랜의 휠부분클리어
	void TJBscrollWheelClear_ChatL();
	TJBscrollWheelClear_ChatL();


	PartyPosState = PARTY_END;

	return PmenuN;
}
//ktj : 새로 만듦. ============== end




//부수적 함수들 ( 마우스로 누른 메뉴 번호 첵크)
int HoParty_KTJ::chkeckMenuN()
{
	int i;

	for(i=0; i<menuSu; i++) {
		if(  chkMenu(pCursorPos.x, pCursorPos.y, &menuPos[i])  ) {
			return i;
		}
	}
	return -1;
}









//퀘스트의 설명글을 넣는다.
void HoParty_KTJ::Draw_questExplane(int x, int y)	//퀘스트의 설명글 그리기
{
	int i;
	HDC hdc = NULL;
	HFONT oldFont;

	//wsprintf(q_explationTitle, "타이틀이야요");
	//lpDDSBack->GetDC(&hdc);
	//SetBkMode( hdc, TRANSPARENT );

	if(hFont != NULL)
		oldFont = (HFONT)SelectObject( hdc , hFont );

	
	SetFontTextColor( RGB(255, 190, 107));		//헤더이름
	dsTextLineOut(hdc, x+30, y-20, q_explationTitle, lstrlen(q_explationTitle));

	for(i= q_explationS; i < q_explationS +8; i++) {
		SetFontTextColor( RGB(255, 190, 107));
		dsTextLineOut(hdc, x, y, q_explation[i], lstrlen(q_explation[i]));
		y += 20;
	}
	if(oldFont)
		SelectObject( hdc, oldFont);
	//lpDDSBack->ReleaseDC(hdc);
}


void HoParty_KTJ::Init_questExplane(char *string, int sw)	//퀘스트의 설명글 그리기
{
	if(sw==-1) {	//title셋팅
		wsprintf(q_explationTitle, string);
		return;
	}
	wsprintf(q_explation[sw], string);
}















/*///////////////////////////////////////////////////////////////////////////
: 함수  명 :
: 하는  일 :  파일을 이용한 디버깅용
///////////////////////////////////////////////////////////////////////////*/

char QuestReadBuf[64*50];
//퀘스트의 설명글이 있는 버퍼에서 데이타를 읽어서 각 줄용 데이타로 다시 옮긴다.
void HoParty_KTJ::ReadFileData_moveBuf(char *savebuf, int lineLen)	
{
	int i, bufC=0,cnt=0;
	char c;
	for(i=0; ; i++) {

		while(1) {
			c = QuestReadBuf[cnt++];
			
			if(c==0) break;	
			if(c==13) {
				cnt++;		//13, 10이 캐리지리턴으로 같이 있음.
				break;
			}
			savebuf[i*lineLen + bufC] = c;
			bufC++;
		}
		//fd2(&savebuf[i*lineLen]);

		bufC = 0;
		if(c==0) break;					//끝이면 끝
	}
}



//퀘스트의 설명글의 데이타셋팅함수들 ================= start
int HoParty_KTJ::ReadFileData(char *fname)	//퀘스트의 설명글 그리기
{

	HANDLE hFile;
    DWORD dwAcess =0;
	hFile = CreateFile( fname, //"D:\\프소스2\\image\\party\\quest_imsi.txt", 
				GENERIC_READ , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );


	ZeroMemory((void*)QuestReadBuf, sizeof(QuestReadBuf));

    if( hFile != INVALID_HANDLE_VALUE ) {
		DWORD rbyte;
		int chk = ReadFile(hFile, (LPVOID)QuestReadBuf, sizeof(QuestReadBuf ), &rbyte, NULL);

        //파일 닫기
        CloseHandle( hFile );
		return 1;
    }

	return 0;		//파일없음.
}
//퀘스트의 설명글의 데이타셋팅함수들 ================= end




/*
//동료의 데이타를 관련 함수 ================= start
int HoParty_KTJ::Read_FriendData(char *fname)
{
	int i;
	HANDLE hFile;
    DWORD dwAcess =0;

	hFile = CreateFile( fname, 
				GENERIC_READ , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );

	ZeroMemory((void*)QuestReadBuf, sizeof(QuestReadBuf));

    if( hFile != INVALID_HANDLE_VALUE ) {
		DWORD rbyte;
		//int chk = ReadFile(hFile, (LPVOID)QuestReadBuf, sizeof(QuestReadBuf ), &rbyte, NULL);

		DWORD fileH;
		DWORD fsize = GetFileSize(hFile, &fileH);
		if(fsize==0) {
			CloseHandle( hFile );
			return 0;
		}


		PartyMemberCount = fsize/sizeof(hoPartyMember);
		for(i=0; i<PartyMemberCount ; i++) {
			ReadFile(hFile, (LPVOID)&PartyMember[i], sizeof(hoPartyMember), &rbyte, NULL);
		}
		//hoPartyMember	PartyMember[MAX_PARTY_MEMBER];	//파티맴버 정보 구조체..
		//int				PartyMemberCount;				//현재 인원수...

		//파일 닫기
        CloseHandle( hFile );
		return 1;
    }

	return 0;		//파일없음.
}

int HoParty_KTJ::Write_FriendData(char *fname)
{
	int i;
	HANDLE hFile;
    DWORD dwAcess =0;


	if(PartyMemberCount ==0){	
		CreateFile( fname, 
				GENERIC_WRITE , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, 
				TRUNCATE_EXISTING, //넣을 데이타가 없으므로 파일이 있으면 무조건 0바이트로 만든다.
				FILE_ATTRIBUTE_NORMAL , NULL );
		return 0;
	}



	hFile = CreateFile( fname, 
				GENERIC_WRITE , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, 
				CREATE_ALWAYS, //파일이 있어도 무조건 덮어쓰기
				FILE_ATTRIBUTE_NORMAL , NULL );

	ZeroMemory((void*)QuestReadBuf, sizeof(QuestReadBuf));

    if( hFile != INVALID_HANDLE_VALUE ) {
		DWORD rbyte;
		//int chk = ReadFile(hFile, (LPVOID)QuestReadBuf, sizeof(QuestReadBuf ), &rbyte, NULL);

		for(i=0; i<PartyMemberCount ; i++) {
			WriteFile(hFile, (LPVOID)&PartyMember[i], sizeof(hoPartyMember), &rbyte, NULL);
		}


		//파일 닫기
        CloseHandle( hFile );
		return 1;
    }

	return 0;		//파일없음.
}
//동료의 데이타를 관련 함수 ================= end
*/


/*==================================================================================
//
// ktj  : 새로 만든것.													end
//
==================================================================================*/
































//ktj : 아래 이건 머다냐????? 몰르것다. ========================================

HoEffectHitBoard EffectHitBoard;

HoEffectHitBoard::HoEffectHitBoard()
{
	MatHitBackBoard = -1;
	MatHitCount = -1;
	
	memset(BufferNum, 0, sizeof(BufferNum));
}

HoEffectHitBoard::~HoEffectHitBoard()
{
	
}

void HoEffectHitBoard::Load()
{
	MatHitBackBoard  = CreateTextureMaterial( "Effect\\Etc\\HitBackBoard.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatHitCount		 = CreateTextureMaterial( "Effect\\Etc\\HitCount.tga"     , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	ReadTextures();
}


POINT BackSize[4]  = { {32, 32}, {32, 32}, {50, 50}, {80, 80}};
float BackSizeStepX = 0.f;
float BackSizeStepY = 0.f;
float BackAlphaStep = 0.f;
int   BackAlpha[4] = { 200, 200, 80, 0};
int	  Delay[4]     = { 5, 5, 5, 10};
int	  BackFrameCount = 0;
int   BackTimeCount;

POINT TextSize[4]  = { {10, 10}, {10, 10}, {20, 20}, {30, 30}};   
float TextSizeStepX = 0.f;
float TextSizeStepY = 0.f;
float TextAlphaStep = 0.f;
int   TextAlpha[4] = { 255, 255, 100, 0};
int	  TextDelay[4]     = { 5, 5, 5, 10};
int	  TextFrameCount = 0;
int   TextTimeCount;


POINT CriticalSize[4]  = { {70, 20}, {70, 20}, {70+70, 20+20}, {70+70*2, 20+20*2}};   
float CriticalSizeStepX = 0.f;
float CriticalSizeStepY = 0.f;
float CriticalAlphaStep = 0.f;
int   CriticalAlpha[4] = { 255, 255, 200, 0};
int	  CriticalDelay[4]     = { 30, 30, 8, 20};
int	  CriticalFrameCount = 0;
int   CriticalTimeCount;

int HoEffectHitBoard::Main()
{
	if(StartFlag)
	{
		if(CriticalFlag)
		{
			CriticalPutSizeX += CriticalSizeStepX;
			CriticalPutSizeY += CriticalSizeStepY;
			CriticalPutAlpha += CriticalAlphaStep;
			CriticalTimeCount++;

			if(CriticalTimeCount > CriticalDelay[BackFrameCount])
			{
				if(CriticalFrameCount == 3)
					CriticalFlag = FALSE;
				
				if(CriticalFrameCount != 3)
				{
					CriticalSizeStepX = (float(CriticalSize[CriticalFrameCount+1].x)-float(CriticalSize[CriticalFrameCount].x))/(float)CriticalDelay[CriticalFrameCount];
					CriticalSizeStepY = (float(CriticalSize[CriticalFrameCount+1].y)-float(CriticalSize[CriticalFrameCount].y))/(float)CriticalDelay[CriticalFrameCount];
					CriticalAlphaStep = (float(CriticalAlpha[CriticalFrameCount+1]-CriticalPutAlpha))/(float)CriticalDelay[CriticalFrameCount];
					CriticalFrameCount++;
				}
				CriticalTimeCount = 0;
			}
		}
		else
		{
			BackPutSizeX  += BackSizeStepX;
			BackPutSizeY  += BackSizeStepY;
			BackPutAlpha  += BackAlphaStep;
			BackTimeCount++;
			if(BackTimeCount >= Delay[BackFrameCount])
			{
				if(BackFrameCount != 3)
				{
					BackSizeStepX = (float(BackSize[BackFrameCount+1].x)-float(BackSize[BackFrameCount].x))/(float)Delay[BackFrameCount];
					BackSizeStepY = (float(BackSize[BackFrameCount+1].y)-float(BackSize[BackFrameCount].y))/(float)Delay[BackFrameCount];
					BackAlphaStep = (float(BackAlpha[BackFrameCount+1]-(float)BackAlpha[BackFrameCount]))/(float)Delay[BackFrameCount];
					BackFrameCount++;
				}
				BackTimeCount = 0;
			}

			TextPutSizeX  += TextSizeStepX;
			TextPutSizeY  += TextSizeStepY;
			TextPutAlpha  += TextAlphaStep;
			TextTimeCount++;
			if(TextTimeCount > TextDelay[TextFrameCount])
			{
				if(TextFrameCount == 3)
				{
					StartFlag = FALSE;
				}					
				
				if(TextFrameCount != 3)
				{
					TextSizeStepX = (float(TextSize[TextFrameCount+1].x)-float(TextSize[TextFrameCount].x))/(float)TextDelay[TextFrameCount];
					TextSizeStepY = (float(TextSize[TextFrameCount+1].y)-float(TextSize[TextFrameCount].y))/(float)TextDelay[TextFrameCount];
					TextAlphaStep = (float(TextAlpha[TextFrameCount+1]-(float)TextAlpha[TextFrameCount]))/(float)TextDelay[TextFrameCount];

					TextFrameCount++;
				}
				TextTimeCount = 0;
			}
		}
	}
	return TRUE;
}

int HoEffectHitBoard::Start(char *num, BOOL criticalFlag)
{
	BackFrameCount = 0;
	BackTimeCount  = 0;

	CriticalFrameCount = 0;
	CriticalTimeCount = 0;
	
	TextFrameCount = 0;
	TextTimeCount = 0;
	StartFlag = TRUE;
	
	BackPutSizeX = 64;
	BackPutSizeY = 64;
	BackPutAlpha = 255;

	TextPutSizeX = 10;
	TextPutSizeY = 10;
	TextPutAlpha = 255;
	
	CriticalPutSizeX = 70;
	CriticalPutSizeY = 20;
	CriticalPutAlpha = 255;

	BackSizeStepX = float((BackSize[BackFrameCount+1].x-BackSize[BackFrameCount].x)/Delay[BackFrameCount]);
	BackSizeStepY = float((BackSize[BackFrameCount+1].y-BackSize[BackFrameCount].y)/Delay[BackFrameCount]);
	BackAlphaStep = float((BackAlpha[BackFrameCount+1]-BackAlpha[BackFrameCount])/Delay[BackFrameCount]);
	
	TextSizeStepX = float((TextSize[BackFrameCount+1].x-TextSize[BackFrameCount].x)/TextDelay[TextFrameCount]);
	TextSizeStepY = float((TextSize[BackFrameCount+1].y-TextSize[BackFrameCount].y)/TextDelay[TextFrameCount]);
	TextAlphaStep = float((TextAlpha[BackFrameCount+1] -TextAlpha[BackFrameCount])/TextDelay[TextFrameCount]);
	
	if(criticalFlag)
	{
		CriticalSizeStepX = float((CriticalSize[CriticalFrameCount+1].x-CriticalSize[BackFrameCount].x)/CriticalDelay[CriticalFrameCount]);
		CriticalSizeStepY = float((CriticalSize[CriticalFrameCount+1].y-CriticalSize[BackFrameCount].y)/CriticalDelay[CriticalFrameCount]);
		CriticalAlphaStep = float((CriticalAlpha[CriticalFrameCount+1] -CriticalAlpha[BackFrameCount])/CriticalDelay[CriticalFrameCount]);
	}
	memset(BufferNum, 0, sizeof(BufferNum));
	wsprintf(BufferNum, "%s", num);

	CriticalFlag = criticalFlag;
	return TRUE;
}

int HoEffectHitBoard::Draw()
{
	int x = 725;
	int y = 50;
	

	if(StartFlag)
	{
		

		if(CriticalFlag == FALSE)
		{
			hoDrawTexImageFloat(MatHitBackBoard, x-BackPutSizeX/2+10, y-BackPutSizeY/2, BackPutSizeX, BackPutSizeY, 0, 0, 32.f, 32.f, 32.f, 32.f, (int)BackPutAlpha);
			
			int count;
			for(int index = 0; index < (int)strlen(BufferNum); index++)
			{
				count = 0;
				if(BufferNum[index] == '0')
					count = 9;
				if(BufferNum[index] == '1')
					count = 0;
				if(BufferNum[index] == '2')
					count = 1;
				if(BufferNum[index] == '3')
					count = 2;
				if(BufferNum[index] == '4')
					count = 3;
				if(BufferNum[index] == '5')
					count = 4;
				if(BufferNum[index] == '6')
					count = 5;
				if(BufferNum[index] == '7')
					count = 6;
				if(BufferNum[index] == '8')
					count = 7;
				if(BufferNum[index] == '9')
					count = 8;
				
				int size = (int)strlen(BufferNum);
				//점수 출력하는곳..
				hoDrawTexImageFloat(MatHitCount, float(float(x+index*TextPutSizeX)-((float)size/2.f)*TextPutSizeX+10.f),
					-TextPutSizeY/2+y,
					TextPutSizeX, TextPutSizeY, 
					float(count*10), 0.f, 11.f, 14.f, 128.f, 64.f, (int)TextPutAlpha);
			}
		}
		else
		{
			//hoDrawTexImageFloat(MatHitCount, float(x-16), float(y-16), 70.f, 20.f, 0.f, 14.f, 128.f, 17.f, 128.f, 32.f, (int)BackPutAlpha);
			hoDrawTexImageFloat(MatHitCount, x-CriticalPutSizeX/2+20, float(y-16), CriticalPutSizeX, CriticalPutSizeY, 0.f, 14.f, 128.f, 17.f, 128.f, 64.f, (int)CriticalPutAlpha);
		}

		
	}
	return TRUE;
}


