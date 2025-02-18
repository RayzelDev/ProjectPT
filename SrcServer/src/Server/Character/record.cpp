
#include "smlib3d\\smd3d.h"

#include "smwsock.h"
#include "character.h"
#include "playmain.h"

#include "fileread.h"
#include "Caravana\Caravana.h"
#include "Shop/NewShop.h"
#include "netplay.h"
#include "sinbaram\\sinlinkheader.h"
#include "record.h"
#include "playsub.h"

#include "field.h"
#include "language.h"

#include "TextMessage.h"
#include "srcserver\\onserver.h"


char	*szRecordHeader = "RC 1.50";			//저장 구조체
DWORD	dwRecordVersion = 150;					//기록 구조체 버전

char	*szRecordUserDataDir = "userdata";
char	*szRecordUserBackupDataDir = "userdata_backup";
char	*szRecordUserInfoDir = "userinfo";
char	*szRecordWareHouseDir = "warehouse";
char* szRecordCaravanDir = "caravan";
char	*szRecordDeleteDir = "deleted";
char	*szPostBoxDir = "Data\\PostBox";

sLAST_QUEST_INFO	RecordLastQuestInfo;			//지난 퀘스트 정보

//#define	CHAR_NAME_MAXLEN	(18+6)
#define	CHAR_NAME_MAXLEN	(16+2)

int	Permit_CheckMoney = TOTAL_CHECK_MONEY_MAX;
int	Permit_CheckExp = TOTAL_CHECK_EXP_MAX;

extern rsSERVER_CONFIG		rsServerConfig;				//서버 설정 구조
extern DWORD				dwPlayServTime;				//서버 시간
extern time_t				tServerTime;				//서버의 시간 ( 빌링 처리용 )
extern int					Server_DebugCount;						//디버그 처리 카운터

//캐릭터 고유 코드를 생성
DWORD GetNewObjectSerial();
//해킹 시도 기록 파일로 남김
int RecordHackLogFile( rsPLAYINFO *lpPlayInfo , void *lpTransCommand );
//물약 거래정보 확인
int	rsGetTradePotionInfo( rsPLAYINFO *lpPlayInfo , DWORD dwCode );
//포인트 티켓 아이템 특정 유저의 인벤토리보냄
int rsPutItem_PointTicket( rsPLAYINFO *lpPlayInfo , int Price );


/*
//아이템 비교
#define ITEM_CODER_MAX		1000000
#define COPY_ITEM_MAX		1000

struct	sITEM_CODE_CMP {
	DWORD	dwCode;
	DWORD	dwKey;
	DWORD	dwSum;

	DWORD	OpenCounter;
};

struct sITEMCODER {
	sITEM_CODE_CMP	sItemCode[ITEM_CODER_MAX];
	int sItemCodeCount;
	int CopyItemCount;
};

struct sCOPYITEM {
	int sItemCodeCount;
	sITEM_CODE_CMP sItemCode[COPY_ITEM_MAX];
};
*/
#include "Resource/resource.h"
#include "English/e_sinMsg.h"

static char LastAcessID[64];



//////////////////////////////////////////////////////////////////////////////////////////////
struct sREC_DATABUFF {
	rsPLAYINFO	*lpPlayInfo;
	DWORD		dwConnectCount;
	char		szName[32];
	char		szFileName[128];
	char		szBackupFileName[128];
	TRANS_RECORD_DATA	TransRecData;
};

#define	REC_DATABUFF_MAX		128
#define	REC_DATABUFF_MASK		127
#define	REC_DATABUFF_LIMIT		64

sREC_DATABUFF	*sRecDataBuff =0;
int				sRecDataBuffCount;
HANDLE			hRecThread =0;
DWORD			dwRecThreadId;
DWORD			dwLastRecDataTime = 0;

//크리티컬 섹션
CRITICAL_SECTION	cRecDataSection;				//저장 동기용 크리티컬섹션
CRITICAL_SECTION	cSaveDataSection;				//저장중인 크리티컬섹션

//서버DB에 데이타 저장요구
int rsSaveRecData( TRANS_RECORD_DATA *lpTransRecordData , rsPLAYINFO *lpPlayInfo , 
				  char *szFileName , char *szBackupFileName );
//저장 대기중인 데이타 있는지 확인
int CheckRecWaitData( char *szName );

//별 포인트 이벤트 티켓 발생 설정
int	OpenStarPointEvent( rsPLAYINFO *lpPlayInfo , smCHAR_INFO *lpCharInfo );
//별 포인트 이벤트 티켓 발생 저장
int	CloseStarPointEvent( rsPLAYINFO *lpPlayInfo , smCHAR_INFO *lpCharInfo );
//별 포인트 이벤트 티켓 발생 설정
int	OpenStarPointTicket( rsPLAYINFO *lpPlayInfo );


////////////////////////////////////////////////////////////////////////////////////////////


int	EncodeFileName( char *szName , char *szDecodeName )
{
	int	len;
	int cnt;
	int cnt2;
	BYTE ch1,ch2;
	DWORD ch;

	len = lstrlen(szName);

	cnt2 = 0;

	for( cnt=0;cnt<len;cnt++ ) {
		ch = (DWORD)szName[cnt];
		ch1 = (BYTE)(ch>>4)&0xF;
		ch2 = (BYTE)ch&0xF;
		ch1 += 'a';
		ch2 += 'a';
		szDecodeName[ cnt2++ ] = (char)ch1;
		szDecodeName[ cnt2++ ] = (char)ch2;
	}
	szDecodeName[cnt2] = 0;

	return TRUE;
}


static int GetUserCode( char *szName )
{
	int cnt,len;
	BYTE ch;
	BYTE *lpData = (BYTE *)szName;

	len = lstrlen( szName );

	ch = 0;
	/*
		if ( ch>='a' && ch<='z' ) {//대문자 소문자로
			Sum2 += (ch-0x20)*(cnt+1);
	*/

	for(cnt=0;cnt<len;cnt++) {
		if ( lpData[cnt]>='a' && lpData[cnt]<='z' ) {
			ch += ( lpData[cnt]-0x20 );				//소문자 대문자로 계산
		}
		else
			ch += lpData[cnt];
	}

	return ch;
}


//우편함 파일 경로 구하기
int GetPostBoxFile( char *szID , char *szFileName )
{

	if ( szID[4]==0 && szID[3]>='0' && szID[3]<='9' && (
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='m' || szID[2]=='M')) || 
		((szID[0]=='l' || szID[0]=='L') && (szID[1]=='p' || szID[1]=='P') && (szID[2]=='t' || szID[2]=='T')) ) 
		) {
 		wsprintf( szFileName , "%s\\%d\\＃%s.dat" , szPostBoxDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	if ( szID[3]==0 && 
		((szID[0]=='p' || szID[0]=='P') && (szID[1]=='r' || szID[1]=='R') && (szID[2]=='n' || szID[2]=='N')) ||
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='n' || szID[2]=='N')) 
		) {
 		wsprintf( szFileName , "%s\\%d\\＃%s.dat" , szPostBoxDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	wsprintf( szFileName , "%s\\%d\\%s.dat" , szPostBoxDir , GetUserCode(szID) , szID );
	
	return TRUE;
}


int GetUserInfoFile( char *szID , char *szFileName )
{
	//wsprintf( szFileName , "%s\\%s.dat" , szRecordUserInfoDir , szID );

	if ( szID[4]==0 && szID[3]>='0' && szID[3]<='9' && (
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='m' || szID[2]=='M')) || 
		((szID[0]=='l' || szID[0]=='L') && (szID[1]=='p' || szID[1]=='P') && (szID[2]=='t' || szID[2]=='T')) ) 
		) {
 		wsprintf( szFileName , "Data\\DataServer\\%s\\%d\\＃%s.dat" , szRecordUserInfoDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	if ( szID[3]==0 && 
		((szID[0]=='p' || szID[0]=='P') && (szID[1]=='r' || szID[1]=='R') && (szID[2]=='n' || szID[2]=='N')) ||
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='n' || szID[2]=='N')) 
		) {
 		wsprintf( szFileName , "Data\\DataServer\\%s\\%d\\＃%s.dat" , szRecordUserInfoDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	wsprintf( szFileName , "Data\\DataServer\\%s\\%d\\%s.dat" , szRecordUserInfoDir , GetUserCode(szID) , szID );
	
	return TRUE;
}

int GetUserDataFile( char *szName , char *szFileName )
{
	//wsprintf( szFileName , "%s\\%s.dat" , szRecordUserDataDir , szName );
	wsprintf( szFileName , "Data\\DataServer\\%s\\%d\\%s.dat" , szRecordUserDataDir , GetUserCode(szName) , szName );

	return TRUE;
}

int GetTT_ServerPath( char *szServerID , char *szTTServerPath, size_t size)
{
	int cnt;

	szTTServerPath[0] = 0;

	for(cnt=0;cnt<rsServerConfig.TT_DataServer_Count;cnt++) {
		if ( lstrcmpi( szServerID , rsServerConfig.TT_DataServer[cnt].szServerID )==0 ) {
			strcpy_s( szTTServerPath, size, rsServerConfig.TT_DataServer[cnt].szServerPath );
			return TRUE;
		}
	}

	return FALSE;
}


int GetRealID( char *szID , char *szRealID )
{

	int cnt,len;

	len = lstrlen( szID );
	lstrcpy( szRealID , szID );
	for(cnt=len-1;cnt>=0;cnt--) {
		if ( szRealID[cnt]=='@' ) {
			szRealID[cnt] = 0;
			return TRUE;
		}
	}


	return FALSE;
}

//서버ID 구하기
int SetServerID( char *szID , char *szServerID )
{
	char szFile[64];
	int cnt,len;

	if ( rsServerConfig.TT_DataServer_Count ) {
		lstrcpy( szFile , szID );
		len = lstrlen(szFile);
		for(cnt=len-1;cnt>=0;cnt--) {
			if ( szFile[cnt]=='@' ) {
				lstrcpy( szServerID , szFile+cnt+1 );
				szServerID[4] = 0;
				return TRUE;
			}
		}
	}

	return FALSE;
}


static int GetUserInfoFile2( char *szID , char *szFileName , char *szServerID )
{
	//wsprintf( szFileName , "%s\\%s.dat" , szRecordUserInfoDir , szID );
	char szTTServerPath[128];
	GetTT_ServerPath( szServerID , szTTServerPath, sizeof(szTTServerPath));

	char szRealID[64];

	GetRealID( szID , szRealID );

	if ( szID[4]==0 && szID[3]>='0' && szID[3]<='9' && (
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='m' || szID[2]=='M')) || 
		((szID[0]=='l' || szID[0]=='L') && (szID[1]=='p' || szID[1]=='P') && (szID[2]=='t' || szID[2]=='T')) ) 
		) {
 		wsprintf( szFileName , "%s\\Data\\DataServer\\%s\\%d\\＃%s.dat" , szTTServerPath, szRecordUserInfoDir , GetUserCode(szRealID) , szRealID );
		return TRUE;
	}

	if ( szID[3]==0 && 
		((szID[0]=='p' || szID[0]=='P') && (szID[1]=='r' || szID[1]=='R') && (szID[2]=='n' || szID[2]=='N')) ||
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='n' || szID[2]=='N')) 
		) {
 		wsprintf( szFileName , "%s\\Data\\DataServer\\%s\\%d\\＃%s.dat" , szTTServerPath,szRecordUserInfoDir , GetUserCode(szRealID) , szRealID );
		return TRUE;
	}

	wsprintf( szFileName , "%s\\Data\\DataServer\\%s\\%d\\%s.dat" , szTTServerPath , szRecordUserInfoDir , GetUserCode(szRealID) , szRealID );
	
	return TRUE;
}

int GetUserDataFile2( char *szName , char *szFileName , char *szServerID )
{
	char szTTServerPath[128];
	GetTT_ServerPath( szServerID , szTTServerPath, sizeof(szTTServerPath));
/*
	char szRealID[64];
	int cnt,len;

	len = lstrlen( szID );
	lstrcpy( szRealID , szID );
	for(cnt=len-1;cnt>=0;cnt--) {
		if ( szRealID[cnt]=='@' ) {
			szRealID[cnt] = 0;
			break;
		}
	}
*/
	wsprintf( szFileName , "%s\\Data\\DataServer\\%s\\%d\\%s.dat" , szTTServerPath, szRecordUserDataDir , GetUserCode(szName) , szName );

	return TRUE;
}


static int GetDeleteDataFile( char *szName , char *szFileName )
{
	wsprintf( szFileName , "Data\\DataServer\\%s\\%s.dat" , szRecordDeleteDir , szName );

	return TRUE;
}


static int GetUserDataBackupFile( char *szName , char *szFileName )
{
	//wsprintf( szFileName , "%s\\%s.dat" , szRecordUserBackupDataDir , szName );
	wsprintf( szFileName , "Data\\DataServer\\%s\\%d\\%s.dat" , szRecordUserBackupDataDir , GetUserCode(szName) , szName );

	return TRUE;
}

static int GetWareHouseFile( char *szName , char *szFileName )
{
	char *szID = szName;

	if ( szID[4]==0 && szID[3]>='0' && szID[3]<='9' && (
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='m' || szID[2]=='M')) || 
		((szID[0]=='l' || szID[0]=='L') && (szID[1]=='p' || szID[1]=='P') && (szID[2]=='t' || szID[2]=='T')) ) 
		) {
 		wsprintf( szFileName , "Data\\DataServer\\%s\\%d\\＃%s.dat" , szRecordWareHouseDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	if ( szID[3]==0 && 
		((szID[0]=='p' || szID[0]=='P') && (szID[1]=='r' || szID[1]=='R') && (szID[2]=='n' || szID[2]=='N')) ||
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='n' || szID[2]=='N')) 
		) {
 		wsprintf( szFileName , "Data\\DataServer\\%s\\%d\\＃%s.dat" , szRecordWareHouseDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	wsprintf( szFileName , "Data\\DataServer\\%s\\%d\\%s.war" , szRecordWareHouseDir , GetUserCode(szName) , szName );

	return TRUE;
}

static int GetCaravanFile(char* szName, char* szFileName)
{
	char* szID = szName;

	if (szID[4] == 0 && szID[3] >= '0' && szID[3] <= '9' && (
		((szID[0] == 'c' || szID[0] == 'C') && (szID[1] == 'o' || szID[1] == 'O') && (szID[2] == 'm' || szID[2] == 'M')) ||
		((szID[0] == 'l' || szID[0] == 'L') && (szID[1] == 'p' || szID[1] == 'P') && (szID[2] == 't' || szID[2] == 'T')))
		) {
		wsprintf(szFileName, "Data\\DataServer\\%s\\%d\\＃%s.dat", szRecordCaravanDir, GetUserCode(szID), szID);
		return TRUE;
	}

	if (szID[3] == 0 &&
		((szID[0] == 'p' || szID[0] == 'P') && (szID[1] == 'r' || szID[1] == 'R') && (szID[2] == 'n' || szID[2] == 'N')) ||
		((szID[0] == 'c' || szID[0] == 'C') && (szID[1] == 'o' || szID[1] == 'O') && (szID[2] == 'n' || szID[2] == 'N'))
		) {
		wsprintf(szFileName, "Data\\DataServer\\%s\\%d\\＃%s.dat", szRecordCaravanDir, GetUserCode(szID), szID);
		return TRUE;
	}

	wsprintf(szFileName, "Data\\DataServer\\%s\\%d\\%s.war", szRecordCaravanDir, GetUserCode(szName), szName);

	return TRUE;
}

static int GetUserDataFile_BackupDay( char *szName , char *szFileName , int Day )
{
	wsprintf( szFileName , "%s\\%d\\%s\\%d\\%s.dat" , 
		rsServerConfig.szBackupPath , Day ,
		szRecordUserDataDir , GetUserCode(szName) , szName );

	return TRUE;
}

static int GetWareHouseFile_Backup( char *szName , char *szFileName , int Day )
{

	char *szID = szName;
	char szBuff[256];

	wsprintf( szBuff , "%s\\%d" , rsServerConfig.szBackupPath , Day ); 

	if ( szID[4]==0 && szID[3]>='0' && szID[3]<='9' && (
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='m' || szID[2]=='M')) || 
		((szID[0]=='l' || szID[0]=='L') && (szID[1]=='p' || szID[1]=='P') && (szID[2]=='t' || szID[2]=='T')) ) 
		) {
 		wsprintf( szFileName , "%s\\%s\\%d\\＃%s.dat" , szBuff, szRecordWareHouseDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	if ( szID[3]==0 && 
		((szID[0]=='p' || szID[0]=='P') && (szID[1]=='r' || szID[1]=='R') && (szID[2]=='n' || szID[2]=='N')) ||
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='n' || szID[2]=='N')) 
		) {
 		wsprintf( szFileName , "%s\\%s\\%d\\＃%s.dat" , szBuff, szRecordWareHouseDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	wsprintf( szFileName , "%s\\%s\\%d\\%s.war" , szBuff, szRecordWareHouseDir , GetUserCode(szName) , szName );

	return TRUE;
}

static int GetCaravanFile_Backup(char* szName, char* szFileName, int Day)
{

	char* szID = szName;
	char szBuff[256];

	wsprintf(szBuff, "%s\\%d", rsServerConfig.szBackupPath, Day);

	if (szID[4] == 0 && szID[3] >= '0' && szID[3] <= '9' && (
		((szID[0] == 'c' || szID[0] == 'C') && (szID[1] == 'o' || szID[1] == 'O') && (szID[2] == 'm' || szID[2] == 'M')) ||
		((szID[0] == 'l' || szID[0] == 'L') && (szID[1] == 'p' || szID[1] == 'P') && (szID[2] == 't' || szID[2] == 'T')))
		) {
		wsprintf(szFileName, "%s\\%s\\%d\\＃%s.dat", szBuff, szRecordCaravanDir, GetUserCode(szID), szID);
		return TRUE;
	}

	if (szID[3] == 0 &&
		((szID[0] == 'p' || szID[0] == 'P') && (szID[1] == 'r' || szID[1] == 'R') && (szID[2] == 'n' || szID[2] == 'N')) ||
		((szID[0] == 'c' || szID[0] == 'C') && (szID[1] == 'o' || szID[1] == 'O') && (szID[2] == 'n' || szID[2] == 'N'))
		) {
		wsprintf(szFileName, "%s\\%s\\%d\\＃%s.dat", szBuff, szRecordCaravanDir, GetUserCode(szID), szID);
		return TRUE;
	}

	wsprintf(szFileName, "%s\\%s\\%d\\%s.war", szBuff, szRecordCaravanDir, GetUserCode(szName), szName);

	return TRUE;
}


//데이타 저장서버 디렉토리 생성
int CreateDataServerDirectory()
{
	int cnt;
	char szBuff[256];

	CreateDirectory( "Data\\DataServer" , NULL );				//디렉토리 생성
	
	wsprintf( szBuff , "Data\\DataServer\\%s" , szRecordUserInfoDir );
	CreateDirectory( szBuff , NULL );				//디렉토리 생성
	wsprintf( szBuff , "Data\\DataServer\\%s" , szRecordUserDataDir );
	CreateDirectory( szBuff , NULL );				//디렉토리 생성
	wsprintf( szBuff , "Data\\DataServer\\%s" , szRecordWareHouseDir );
	CreateDirectory( szBuff , NULL );				//디렉토리 생성
	wsprintf( szBuff , "Data\\DataServer\\%s" , szRecordUserBackupDataDir );
	CreateDirectory( szBuff , NULL );				//디렉토리 생성
	wsprintf( szBuff , "Data\\DataServer\\%s" , szRecordDeleteDir );
	CreateDirectory( szBuff , NULL );				//디렉토리 생성


	for( cnt=0;cnt<256;cnt++ ) {
		wsprintf( szBuff , "Data\\DataServer\\%s\\%d" , szRecordUserInfoDir , cnt );
		CreateDirectory( szBuff , NULL );			//디렉토리 생성
	}
	for( cnt=0;cnt<256;cnt++ ) {
		wsprintf( szBuff , "Data\\DataServer\\%s\\%d" , szRecordUserDataDir , cnt );
		CreateDirectory( szBuff , NULL );			//디렉토리 생성
	}
	for( cnt=0;cnt<256;cnt++ ) {
		wsprintf( szBuff , "Data\\DataServer\\%s\\%d" , szRecordWareHouseDir , cnt );
		CreateDirectory( szBuff , NULL );			//디렉토리 생성
	}
	for( cnt=0;cnt<256;cnt++ ) {
		wsprintf( szBuff , "Data\\DataServer\\%s\\%d" , szRecordUserBackupDataDir , cnt );
		CreateDirectory( szBuff , NULL );			//디렉토리 생성
	}

	//우편함 디렉토리 만들기
	if ( CreateDirectory( szPostBoxDir , NULL ) ) {			//디렉토리 생성
		for( cnt=0;cnt<256;cnt++ ) {
			wsprintf( szBuff , "%s\\%d" , szPostBoxDir , cnt );
			CreateDirectory( szBuff , NULL );				//디렉토리 생성
		}
	}

	return TRUE;
}



//캐릭터 정보 저장 코드 구하기
DWORD	GetCharInfoCode( smCHAR_INFO *lpCharInfo )
{
	int cnt;
	int size;
	BYTE *lpCharBuff;
	DWORD	dwKey;
	DWORD	dwCode;

	size = sizeof(smCHAR_INFO);
	lpCharBuff = (BYTE *)lpCharInfo;

	dwKey = 0;

	for( cnt=0;cnt<size;cnt++ ) {
		dwKey += lpCharBuff[cnt];
	}

	dwCode = 0;

	for( cnt=0;cnt<size;cnt++ ) {
		dwCode += (dwKey+cnt)*lpCharBuff[cnt];
	}

	return dwCode;
}

//sUSESKILL sinSkill;
/*
/////////////스킬 테이블 구조체 
struct sSKILL{  
	char	sSkillName[32];			//스킬 이름 
	DWORD	CODE;					//스킬 코드 
	char    FileName[32];			//스킬 파일 이름 
	int     Flag;					//사용가능 플랙 
	int     Use;					//포인트 할당가능 플랙 
	int     Point;					//스킬에 할당된 포인트 
	int     ShortKey;				//펑션 키 
	int     MousePosi;				//스킬 마우스 포지션 
	int     Position;				
	int     UseTime;				//사용 시간 
	int     CheckTime;				//사용시간을 체크한다 
	int     GageLength;				//마스터리 게이지 길이 
	float     GageLength2;			//마스터리 게이지 길이 
	float   Mastery;				//마스터리 증가 수치 
	int     UseSkillCount;			//스킬사용 수치 
	float	UseSkillMastery;		//사용시 올라가는 수치 
	int     UseSkillMasteryGage;	//사용시 올라가는 게이지 
	int     UseSkillFlag;			//1은 사용 0의 사용불가 
	int     PlusState[5];			//플러스 
	sSKILL_INFO Skill_Info;
};

/////////////현재 사용되고있는 스킬 구조체 
struct sUSESKILL{
	sSKILL UseSkill[SIN_MAX_USE_SKILL]; //사용할수있는 스킬 
	sSKILLBOX SkillBox[SIN_MAX_USE_SKILL];
	sSKILL *pLeftSkill;		//마우스 왼쪽 버튼에 할당된 스킬				
	sSKILL *pRightSkill;	//마우스 오른쪽에 할당된 스킬 
	int   SkillPoint;

};
//저장될 스킬 구조
struct	RECORD_SKILL {
	BYTE	bSkillPoint[SIN_MAX_USE_SKILL];			//스킬 포인트
	WORD	wSkillMastery[SIN_MAX_USE_SKILL];		//스킬 숙련도
	BYTE	bShortKey[SIN_MAX_USE_SKILL];			//펑션 키
	WORD	wSelectSkill[2];						//선택된 스킬
	//int		RemainPoint;
};

*/

//스킬 저장
int	RecordSkill( RECORD_SKILL *lpRecordSkill )
{
	int cnt;
	int mcnt;

	for(cnt=0;cnt<SIN_MAX_USE_SKILL;cnt++) {
		mcnt = cnt&15;							//스킬 저장 영역 강제로 보정 ( 배열을 16개만 잡아놔서 16번 배열은 0에 저장 )

		lpRecordSkill->bSkillPoint[mcnt] = sinSkill.UseSkill[cnt].Point;
		if ( sinSkill.UseSkill[cnt].Point>255 ) lpRecordSkill->bSkillPoint[mcnt]=255;

		if ( sinSkill.UseSkill[cnt].UseSkillCount<10000 )		//스킬숙련도 최고치
			lpRecordSkill->wSkillMastery[mcnt] = sinSkill.UseSkill[cnt].UseSkillCount;
		else
			lpRecordSkill->wSkillMastery[mcnt] = 10000;

		lpRecordSkill->bShortKey[mcnt] = sinSkill.UseSkill[cnt].ShortKey|(sinSkill.UseSkill[cnt].MousePosi<<4);
	}

	lpRecordSkill->wSelectSkill[0] = 0;
	lpRecordSkill->wSelectSkill[1] = 0;

	if ( sinSkill.pLeftSkill && sinSkill.pLeftSkill->CODE!=SKILL_NORMAL_ATTACK ) {
		lpRecordSkill->wSelectSkill[0] = sinSkill.pLeftSkill->Skill_Info.SkillNum+1;
	}
	if ( sinSkill.pRightSkill && sinSkill.pRightSkill->CODE!=SKILL_NORMAL_ATTACK ) {
		lpRecordSkill->wSelectSkill[1] = sinSkill.pRightSkill->Skill_Info.SkillNum+1;
	}

	return TRUE;
}



//퀘스트로 획득한 스킬 포인트
int GetSkillPoint_LevelQuest( int Level , DWORD dwLevelQuestLog )
{
	int Point = 0;

	if ( dwLevelQuestLog&QUESTBIT_LEVEL_30 && Level>=30 ) {

	}
	if ( dwLevelQuestLog&QUESTBIT_LEVEL_55 && Level>=55 ) {
		Point++;
	}
	if ( dwLevelQuestLog&QUESTBIT_LEVEL_70 && Level>=70 ) {
		Point++;
	}
	if ( dwLevelQuestLog&QUESTBIT_LEVEL_80 && Level>=80 ) {
		Point+=2;
	}

	return Point;
}

//퀘스트로 획득한 스텟 포인트
int GetStatePoint_LevelQuest( int Level , DWORD dwLevelQuestLog )
{
	int Point = 0;

	if ( dwLevelQuestLog&QUESTBIT_LEVEL_30 && Level>=30 ) {
		Point += 5;
	}
	if ( dwLevelQuestLog&QUESTBIT_LEVEL_55 && Level>=55 ) {

	}
	if ( dwLevelQuestLog&QUESTBIT_LEVEL_70 && Level>=70 ) {
		Point += 5;
	}
	if ( dwLevelQuestLog&QUESTBIT_LEVEL_80 && Level>=80 ) {
		Point += 5;
	}

	if ( dwLevelQuestLog&QUESTBIT_LEVEL_80_2 && Level>=80 ) {		//80렙 이상 스텟7씩 준다 (+2)
		Point += (Level-79)*2;
	}

	if ( dwLevelQuestLog&QUESTBIT_LEVEL_90_2 && Level>=90 ) {		//90렙 이상 스텟10씩 준다 (+2+3)
		Point += (Level-89)*3;
	}

	return Point;
}


//스킬 복구
int	RestoreSkill( RECORD_SKILL *lpRecordSkill , DWORD dwLevelQuestLog )
{
	int cnt,mcnt;
	int	Point,EPoint;

	Point = 0;
	EPoint = 0;

	if ( lpRecordSkill->bSkillPoint[0]>0 && lpRecordSkill->bSkillPoint[15]==0 ) {
		lpRecordSkill->bSkillPoint[0] = 0;
		lpRecordSkill->wSkillMastery[0] = 0;
		lpRecordSkill->bShortKey[0] = 0;
	}

	for(cnt=0;cnt<SIN_MAX_USE_SKILL;cnt++) {
		mcnt = cnt&15;							//스킬 저장 영역 강제로 보정 ( 배열을 16개만 잡아놔서 16번 배열은 0에 저장 )

		if ( cnt>0 ) {	//주먹은 제외
			sinSkill.UseSkill[cnt].Point = lpRecordSkill->bSkillPoint[mcnt];
			sinSkill.UseSkill[cnt].UseSkillCount = lpRecordSkill->wSkillMastery[mcnt];
			sinSkill.UseSkill[cnt].ShortKey = lpRecordSkill->bShortKey[mcnt]&0xF;
			sinSkill.UseSkill[cnt].MousePosi = lpRecordSkill->bShortKey[mcnt]>>4;
			sinSkill.UseSkill[cnt].Position = cnt+1;
			//Point += sinSkill.UseSkill[cnt].Point;
		}
	}

	for(cnt=1;cnt<13;cnt++) {
		Point += sinSkill.UseSkill[cnt].Point;
	}
	for(cnt=13;cnt<SIN_MAX_USE_SKILL;cnt++) {
		EPoint += sinSkill.UseSkill[cnt].Point;
	}


	sinSkill.UseSkill[0].Point = 1;
	sinSkill.UseSkill[0].UseSkillFlag = 1;
	sinSkill.UseSkill[0].Position = 1;


	sinSkill.pLeftSkill = &sinSkill.UseSkill[ lpRecordSkill->wSelectSkill[0] ];
	sinSkill.pRightSkill = &sinSkill.UseSkill[ lpRecordSkill->wSelectSkill[1] ];

	if ( lpCurPlayer->smCharInfo.Level>=10 ) {
		sinSkill.SkillPoint = ((lpCurPlayer->smCharInfo.Level-8)/2)-Point;
		sinSkill.SkillPoint += GetSkillPoint_LevelQuest(lpCurPlayer->smCharInfo.Level,dwLevelQuestLog);		//퀘스트로 얻은 포인트

		if ( sinSkill.SkillPoint<0 ) {
			//스킬 포인트 오류 ( 스킬 초기화 )
			for(cnt=0;cnt<13;cnt++) {
				sinSkill.UseSkill[cnt].Point = 0;
				sinSkill.UseSkill[cnt].UseSkillCount = 0;
			}
			sinSkill.SkillPoint = 0;
		}
	}
	else {
		sinSkill.SkillPoint = 0;
	}


	if ( lpCurPlayer->smCharInfo.Level>=60 ) {
		sinSkill.SkillPoint4 = ((lpCurPlayer->smCharInfo.Level-58)/2)-EPoint;

		if ( sinSkill.SkillPoint4<0 ) {
			//스킬 포인트 오류 ( 스킬 초기화 )
			for(cnt=13;cnt<SIN_MAX_USE_SKILL;cnt++) {
				sinSkill.UseSkill[cnt].Point = 0;
				sinSkill.UseSkill[cnt].UseSkillCount = 0;
			}
			sinSkill.SkillPoint4 = 0;
		}
	}
	else {
		sinSkill.SkillPoint4 = 0;
	}


	cInvenTory.SetItemToChar();
	cSkill.InitReformSkillPointForm();

	return TRUE;
}


//스킬 첵크
int	CheckSkillPoint( int Level , RECORD_SKILL *lpRecordSkill , int *spTotal , DWORD dwLevelQuestLog )
{
	int cnt,mcnt;
	int	Point,EPoint;
	int SkillPoint;
	int ExtraPoint;

	Point = 0;
	EPoint = 0;

	//for(cnt=1;cnt<SIN_MAX_USE_SKILL;cnt++) {
	for(cnt=1;cnt<13;cnt++) {
		if ( lpRecordSkill->bSkillPoint[cnt]>10 ) return FALSE;
		Point += abs(lpRecordSkill->bSkillPoint[cnt]);
	}

	for(cnt=13;cnt<SIN_MAX_USE_SKILL;cnt++) {
		mcnt = cnt&15;
		if ( lpRecordSkill->bSkillPoint[mcnt]>10 ) return FALSE;
		EPoint += abs(lpRecordSkill->bSkillPoint[mcnt]);
	}

	if ( spTotal ) *spTotal = (Point+(EPoint<<16));

	if ( Level>=10 ) {
		SkillPoint = ((Level-8)/2)-Point;
		SkillPoint += GetSkillPoint_LevelQuest( Level , dwLevelQuestLog );		//퀘스트로 획득한 스킬 포인트

		if ( SkillPoint<0 ) {
			//스킬 포인트 오류 ( 스킬 초기화 )
			return FALSE;
		}
	}
	else {
		if ( Point>0 ) return FALSE;
	}

	if ( Level>=60 ) {
		ExtraPoint = ((Level-58)/2)-EPoint;
		if ( ExtraPoint<0 ) {
			//확장스킬 포인트 오류 ( 스킬 초기화 )
			return FALSE;
		}
	}
	else {
		if ( EPoint>0 ) return FALSE;
	}

	return TRUE;
}


//99
//캐릭터 스테이트 값을 새롭게 보정한다
int ReformCharStatePoint( smCHAR_INFO *lpCharInfo , DWORD dwLevelQuestLog )
{
	int Total;
	int NewState;

	Total = abs(lpCharInfo->Strength) + 
			abs(lpCharInfo->Spirit) + 
			abs(lpCharInfo->Dexterity) +
			abs(lpCharInfo->Health) +
			abs(lpCharInfo->Talent) +
			abs(lpCharInfo->StatePoint);

	//99가 기본치
	NewState = 99+((lpCharInfo->Level-1)*5);
	NewState += GetStatePoint_LevelQuest( lpCharInfo->Level , dwLevelQuestLog );		//퀘스트로 획득한 스텟 포인트

	lpCharInfo->StatePoint += (NewState-Total);

	if ( lpCharInfo->StatePoint<=-10 ) {
		//캐릭터 능력치 문제 발생
		lpCharInfo->Strength = 1; 
		lpCharInfo->Spirit = 8;
		lpCharInfo->Dexterity = 1;
		lpCharInfo->Health = 8;
		lpCharInfo->Talent = 1;
		if ( lpCharInfo->StatePoint<0 ) lpCharInfo->StatePoint = 0;

		return FALSE;
	}
	if ( lpCharInfo->StatePoint<0 ) lpCharInfo->StatePoint = 0;

	return TRUE;
}


//게임 진행 데이타 기록
int RecordGameData(sGAME_SAVE_INFO* lpGameSaveInfo) { return TRUE; }

//게임 진행 데이타 기록
int RestoreGameData( sGAME_SAVE_INFO *lpGameSaveInfo ) { return TRUE; }

//종료된 퀘스트 기록 추가
int Record_LastQuest( WORD	wQuestCode )
{
	int cnt;

	cnt = RecordLastQuestInfo.LastQuestCount&LAST_QUEST_MASK;
	RecordLastQuestInfo.wLastQuest[cnt] = wQuestCode;
	RecordLastQuestInfo.LastQuestCount++;

	return TRUE;
}

//지난 퀘스트 검사
int FindLastQuestCode( WORD wQuestCode )
{
	int cnt,mcnt,start;

	if ( RecordLastQuestInfo.LastQuestCount>LAST_QUEST_MASK )
		start = RecordLastQuestInfo.LastQuestCount-LAST_QUEST_MASK;
	else
		start = 0;

	for(cnt=start;cnt<RecordLastQuestInfo.LastQuestCount;cnt++) {
		mcnt = cnt&LAST_QUEST_MASK;
		if ( RecordLastQuestInfo.wLastQuest[mcnt]==wQuestCode ) {
			return TRUE;		//퀘스트 찾았다
		}
	}
	return FALSE;
}

/*
#define	LAST_QUEST_MAX		32
#define	LAST_QUEST_MASK		31

struct	sLAST_QUEST_INFO {
	WORD wLastQuest[LAST_QUEST_MAX];
	int	LastQuestCount;
}
*/
//기록 데이타 아이템 이상 유무 확인
int CheckRecordDataItem( TRANS_RECORD_DATA *lpRecordData )
{
	int DataSize , size;
	int ItemCount;
	int cnt;
	int SizeCount;

	BYTE *lpRecordItem;

	DataSize = lpRecordData->DataSize;
	ItemCount = lpRecordData->ItemCount;
	lpRecordItem = lpRecordData->Data;

	SizeCount = 0;

	for( cnt=0;cnt<ItemCount;cnt++ ) {

		size = ((int *)lpRecordItem)[0];
		SizeCount += size;
		lpRecordItem += size;

		if ( SizeCount>DataSize || SizeCount<0 )
			return FALSE;
	}

	if ( SizeCount==DataSize ) return TRUE;

	return FALSE;
}
/*
//저장된 아이템 데이타를 150 포맷으로 인증변환
int	ConvertItem_Server150(TRANS_RECORD_DATA *lpTransRecordData )
{
	BYTE	*lpRecItem;
	sRECORD_ITEM	sRecordItem[INVENTORY_MAXITEM*2];

	int cnt;
	int CompSize;

	lpRecItem = (BYTE *)lpTransRecordData->Data;

	for( cnt=0;cnt<lpTransRecordData->ItemCount;cnt++ ) {
		//압축 데이타 해독 ( Z/NZ 방식 )
		DecodeCompress( (BYTE *)lpRecItem , (BYTE *)&sRecordItem[cnt] );
		rsReformItem_Server( &sRecordItem[cnt].sItemInfo );					//아이템 신규 인증
		lpRecItem += ((int *)lpRecItem)[0];
	}

	lpTransRecordData->DataSize = 0;
	lpRecItem = (BYTE *)lpTransRecordData->Data;

	for( cnt=0;cnt<lpTransRecordData->ItemCount;cnt++ ) {
		//데이타 압축 ( Z/NZ 방식 )
		CompSize = EecodeCompress( (BYTE *)&sRecordItem[cnt] , (BYTE *)lpRecItem , sizeof(sRECORD_ITEM) );
		lpRecItem += CompSize;
		lpTransRecordData->DataSize += CompSize;
	}

	lpTransRecordData->size = ((DWORD)lpRecItem)-((DWORD)lpTransRecordData);

	return TRUE;
}
*/



//저장할 데이타를 정리하여 제작
int rsRECORD_DBASE::MakeRecordData( smCHAR_INFO *lpCharInfo , sITEM *lpItems , sITEM *lpItems2 , sITEM *lpMouseItem )
{
	int cnt;

	BYTE	*lpRecItem;
	sRECORD_ITEM	sRecordItem;
	int		CompSize;

	lstrcpy( TransRecordData.szHeader , szRecordHeader );
	memcpy( &TransRecordData.smCharInfo , lpCharInfo , sizeof( smCHAR_INFO ) );

	TransRecordData.smCharInfo.wVersion[0] = Version_CharInfo;
	//TransRecordData.smCharInfo.wVersion[1] = 0;

	ZeroMemory( &TransRecordData.GameSaveInfo , sizeof(sGAME_SAVE_INFO) );

	TransRecordData.ThrowItemCount = 0;

	RecordGameData( &TransRecordData.GameSaveInfo );
	//캐릭터 정보 코드 저장
	TransRecordData.GameSaveInfo.dwChkSum_CharInfo = GetCharInfoCode( lpCharInfo );

	TransRecordData.ItemCount = 0; 
	lpRecItem = TransRecordData.Data;
	TransRecordData.DataSize = 0;

	for( cnt=0;cnt<INVENTORY_MAXITEM;cnt++ ) {
		if ( lpItems[cnt].Flag ) {
/*
			if ( !lpItems[cnt].sItemInfo.ItemName[0] || 
				(lpItems[cnt].CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ||
				lpItems[cnt].sItemInfo.ItemHeader.dwChkSum==(lpItems[cnt].sItemInfo.Temp0-lpItems[cnt].sItemInfo.CODE) ) {
*/
				sRecordItem.ItemCount=cnt;
				sRecordItem.x = lpItems[cnt].x;
				sRecordItem.y = lpItems[cnt].y;
				sRecordItem.ItemPosition = lpItems[cnt].ItemPosition;
				memcpy( &sRecordItem.sItemInfo , &lpItems[cnt].sItemInfo , sizeof( sITEMINFO ) );

				//데이타 압축 ( Z/NZ 방식 )
				CompSize = EecodeCompress( (BYTE *)&sRecordItem , (BYTE *)lpRecItem , sizeof(sRECORD_ITEM) );

				lpRecItem += CompSize;
				TransRecordData.DataSize += CompSize;

				TransRecordData.ItemCount++;
/*
			}
			else {
				cnt = cnt;
			}
*/
		}
	}

	TransRecordData.ItemSubStart = TransRecordData.ItemCount;

	for( cnt=0;cnt<INVENTORY_MAXITEM;cnt++ ) {
		if ( lpItems2[cnt].Flag ) {
			sRecordItem.ItemCount=cnt;
			sRecordItem.x = lpItems2[cnt].x;
			sRecordItem.y = lpItems2[cnt].y;
			sRecordItem.ItemPosition = lpItems2[cnt].ItemPosition;
			memcpy( &sRecordItem.sItemInfo , &lpItems2[cnt].sItemInfo , sizeof( sITEMINFO ) );

			//데이타 압축 ( Z/NZ 방식 )
			CompSize = EecodeCompress( (BYTE *)&sRecordItem , (BYTE *)lpRecItem , sizeof(sRECORD_ITEM) );

			lpRecItem += CompSize;
			TransRecordData.DataSize += CompSize;

			TransRecordData.ItemCount++;
		}
	}

	if ( lpMouseItem && lpMouseItem->Flag ) {
		//마우스에 아이템일 잡고 있는 경우
		sRecordItem.ItemCount=0;
		sRecordItem.x = 0;
		sRecordItem.y = 0;
		sRecordItem.ItemPosition = -1;
		memcpy( &sRecordItem.sItemInfo , &lpMouseItem->sItemInfo , sizeof( sITEMINFO ) );

		//데이타 압축 ( Z/NZ 방식 )
		CompSize = EecodeCompress( (BYTE *)&sRecordItem , (BYTE *)lpRecItem , sizeof(sRECORD_ITEM) );

		lpRecItem += CompSize;
		TransRecordData.DataSize += CompSize;

		TransRecordData.ItemCount++;
	}


	TransRecordData.code = smTRANSCODE_RECORDDATA;
	TransRecordData.size = ((DWORD)lpRecItem)-((DWORD)&TransRecordData);

	return TRUE;
}


int ClientRecordPotion[3][4];
int ClientRecordPotionFlag = 0;

int ResetClientRecordPotion( sGAME_SAVE_INFO *lpGameSaveInfo )
{
	int cnt,cnt2;

	if ( !lpGameSaveInfo->sPotionUpdate[0] || lpGameSaveInfo->sPotionUpdate[0]!=lpGameSaveInfo->sPotionUpdate[1] ) 
		return FALSE;

	ClientRecordPotionFlag = TRUE;

	for(cnt=0;cnt<3;cnt++)
		for(cnt2=0;cnt2<4;cnt2++)
			ClientRecordPotion[cnt][cnt2] = lpGameSaveInfo->sPotionCount[cnt][cnt2];


	return TRUE;
}

//물약 검사용
int AddRecordPotion( DWORD dwPotionCode , int PotionCount )
{
	int Count=PotionCount;
	int x=-1;
	int y=-1;

	if ( ClientRecordPotionFlag==0 ) return 0;

	switch( dwPotionCode ) {
		case (sinPL1|sin01):		//생명(소)
			x=0;y=0;
			break;
		case (sinPL1|sin02):		//생명(중)
			x=0;y=1;
			break;
		case (sinPL1|sin03):		//생명(대)
			x=0;y=2;
			break;
		case (sinPL1|sin04):		//생명(신)
			x=0;y=3;
			break;

		case (sinPM1|sin01):		//기력(소)
			x=1;y=0;
			break;
		case (sinPM1|sin02):		//기력(중)
			x=1;y=1;
			break;
		case (sinPM1|sin03):		//기력(대)
			x=1;y=2;
			break;
		case (sinPM1|sin04):		//기력(신)
			x=1;y=3;
			break;

		case (sinPS1|sin01):		//근력(소)
			x=2;y=0;
			break;
		case (sinPS1|sin02):		//근력(중)
			x=2;y=1;
			break;
		case (sinPS1|sin03):		//근력(대)
			x=2;y=2;
			break;
		case (sinPS1|sin04):		//근력(신)
			x=2;y=3;
			break;
	}

	if ( x>=0 && y>=0 ) {
		ClientRecordPotion[x][y] += Count;
		return ClientRecordPotion[x][y];
	}

	return 0;
}



//불러온 데이타 정보를 해당 위치에 설정
int rsRECORD_DBASE::ResotrRecordData( smCHAR_INFO *lpCharInfo , sITEM *lpItems , sITEM *lpItems2 , sITEM *lpMouseItem )
{
	int cnt;
	int cnt2,cnt3;
	sITEM *lpChkItem[512];
	int		ChkCnt;
	int		CopyItemCount;
	int		BadItemCount;
	int		SetFlag;
	DWORD	dwItemCode;
	int		WeightError = 0;
	int		PostionError = 0;
	int		SvrPotionError = 0;
	int		ReformItemCount = 0;
	DWORD	dwCode;
	sITEMINFO	*lpsItemInfo;

	BYTE	*lpRecItem;
	sITEM	*lpSaveItem;
	sRECORD_ITEM	sRecordItem;
	sITEM	sItem_Buff;
	char	szMsgBuff[128];
	int		PotionCnt;

	//char	szTestBuff[4096];


	//HANDLE	hResource;
	//HANDLE	hLoadRes;
	//sCOPYITEM	*lpCopyItems=0;

	//캐릭터 정보 인증 확인
	CheckCharForm();

	memcpy( lpCharInfo , &TransRecordData.smCharInfo , sizeof( smCHAR_INFO ) );
	lpCharInfo->bUpdateInfo[0] = 0;

	//게임 포션 기록
	ResetClientRecordPotion( &TransRecordData.GameSaveInfo );

	//캐릭터 정보 인증 확인
	CheckCharForm();
	//크랙 첵크
	CheckCracker();
/*
	hResource = FindResource( hinst , MAKEINTRESOURCE(IDR_GOODITEM1), "GOODITEM" );
	hLoadRes = LoadResource( hinst , (HRSRC)hResource );
	lpCopyItems = (sCOPYITEM *)LockResource( hLoadRes );
*/
	if ( smConfig.DebugMode && smConfig.szFile_Player[0] ) {
		//운영자는 스킨 바꾸기 가능
		lstrcpy( lpCharInfo->szModelName ,  smConfig.szFile_Player );
		lpCharInfo->szModelName[0] = 0;
	}

	ZeroMemory( lpItems , sizeof( sITEM ) * INVENTORY_MAXITEM );
	ZeroMemory( lpItems2 , sizeof( sITEM ) * INVENTORY_MAXITEM );

	lpRecItem = (BYTE *)&TransRecordData.Data;

	ChkCnt = 0;
	CopyItemCount = 0;
	BadItemCount = 0;

/*
	if ( lpCharInfo->Weight[0]<0 || lpCharInfo->Weight[0]>lpCharInfo->Weight[1] ) {
		WeightError = TRUE;
		//해킹 시도한 유저 자동 신고
		SendSetHackUser2( 1900 , lpCharInfo->Weight[0] );
	}
*/
	// pluto 마이트 오브 아웰  캐쉬템 소지량 증가
	if( TransRecordData.GameSaveInfo.dwTime_PrimeItem_MightofAwell )
	{
		if( lpCharInfo->Weight[0]<0 || lpCharInfo->Weight[0]>lpCharInfo->Weight[1] + 300 ) {
			WeightError = TRUE;
			//해킹 시도한 유저 자동 신고
			SendSetHackUser2( 1900 , lpCharInfo->Weight[0] );
		}
	}
	else if( TransRecordData.GameSaveInfo.dwTime_PrimeItem_MightofAwell2 )
	{
		if( lpCharInfo->Weight[0]<0 || lpCharInfo->Weight[0]>lpCharInfo->Weight[1] + 500 ) {
			WeightError = TRUE;
			//해킹 시도한 유저 자동 신고
			SendSetHackUser2( 1900 , lpCharInfo->Weight[0] );
		}
	}
    else
	{
		if( lpCharInfo->Weight[0]<0 || lpCharInfo->Weight[0]>lpCharInfo->Weight[1] ) {
		WeightError = TRUE;
		//해킹 시도한 유저 자동 신고
		SendSetHackUser2( 1900 , lpCharInfo->Weight[0] );
		}
	}

	for( cnt=0;cnt<TransRecordData.ItemCount;cnt++ ) {

		lpsItemInfo = 0;
		//압축 데이타 해독 ( Z/NZ 방식 )
		DecodeCompress( (BYTE *)lpRecItem , (BYTE *)&sRecordItem );
/*
		//데이타 압축 ( Z/NZ 방식 )
		EecodeCompress( (BYTE *)&sRecordItem , (BYTE *)szTestBuff , sizeof(sRECORD_ITEM) );

		if ( ((int *)szTestBuff)[0]!=((int *)lpRecItem)[0] ) {
			SetFlag = SetFlag;
		}
*/

		SetFlag = TRUE;
		dwItemCode = sRecordItem.sItemInfo.CODE&sinITEM_MASK2;

		//버려진 아이템 제거
		for( cnt2=0;cnt2<TransRecordData.ThrowItemCount;cnt2++) {
			if ( TransRecordData.ThrowItemInfo[cnt2].dwCode==sRecordItem.sItemInfo.CODE &&
				TransRecordData.ThrowItemInfo[cnt2].dwKey==sRecordItem.sItemInfo.ItemHeader.Head &&
				TransRecordData.ThrowItemInfo[cnt2].dwSum==sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {
				SetFlag = FALSE;
				break;
			}
		}

/*
#define sinWA1			0x01010000	//Axes
#define sinWC1			0x01020000	//Claws
#define sinWH1			0x01030000	//Hammer & So On
#define sinWM1			0x01040000	//Magicial Stuffs
#define sinWP1			0x01050000	//Poles & Spears
#define sinWS1			0x01060000	//Shooters
#define sinWS2			0x01070000	//Swords
#define sinWT1			0x01080000	//Throwing Arms

#define sinDA1			0x02010000	//Armor
#define sinDB1			0x02020000	//Boots
#define sinDG1			0x02030000	//Gloves
#define sinDS1			0x02040000	//Shields
*/
		//PostionError
		if ( sRecordItem.ItemPosition>3 ) {
			dwCode = sRecordItem.sItemInfo.CODE>>24;
			if ( dwCode==1 ) {
				PostionError++;				//무기 있음
			}
			dwCode = sRecordItem.sItemInfo.CODE>>16;
			if ( dwCode==0x0201 || dwCode==0x0204 ) {
				PostionError++;				//갑옷 , 방패
			}
		}


/*
		///////////////// 소마의 링아머 제거 ////////////////////////
		if ( sRecordItem.sItemInfo.CODE==33622016 && 
			sRecordItem.sItemInfo.ItemHeader.Head==2242593061 && 
			sRecordItem.sItemInfo.ItemHeader.dwChkSum==4294914487 ) {

			//복사 아이템 제거
			SetFlag = FALSE;
			//해킹 시도한 유저 자동 신고
			SendSetHackUser2( 1960 , sRecordItem.sItemInfo.CODE );
		}
*/

		if ( sRecordItem.ItemPosition!=2 && CheckItemForm( &sRecordItem.sItemInfo )==FALSE ) {
			//잘못된 아이템 제거
			SetFlag = FALSE;
			//해킹 시도한 유저 자동 신고
			SendSetHackUser2( 1950 , 0 );
		}

		//아이템 인증번호가 0 인경우 제거 ( 껍데기 아이템 제외 )
		if ( !sRecordItem.sItemInfo.ItemHeader.Head || !sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {
			if ( sRecordItem.sItemInfo.ItemName[0] ) {
				SetFlag = FALSE;
				//해킹 시도한 유저 자동 신고
				SendSetHackUser2( 1950 , 0 );
			}
		}

		if ( TransRecordData.GameSaveInfo.TT_ServerID ) {
			if ( (sRecordItem.sItemInfo.CODE&sinITEM_MASK2) == sinBC1 ) {	
				SetFlag = FALSE;
			}
		}


		//포션 0 짜리 제거
		if ( (sRecordItem.sItemInfo.CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ) {
			if ( sRecordItem.sItemInfo.PotionCount<=0 ) {
				SetFlag = FALSE;
			}
			else {
				//물약 검사용
				PotionCnt = AddRecordPotion( sRecordItem.sItemInfo.CODE , -sRecordItem.sItemInfo.PotionCount );
				if ( PotionCnt<0 ) {
					sRecordItem.sItemInfo.PotionCount+=PotionCnt;
					SvrPotionError -= PotionCnt;		//오류난 갯수 기록

					if ( sRecordItem.sItemInfo.PotionCount<=0 ) {
						SetFlag = FALSE;
					}
				}
			}
		}

#ifdef _DELETE_FUCKIN_ITEM
		if ( sRecordItem.sItemInfo.CODE==(sinOR1|sin21) ||
				sRecordItem.sItemInfo.CODE==(sinOA1|sin21)

			/*sRecordItem.sItemInfo.CODE==(sinOR1|sin22) ||
				sRecordItem.sItemInfo.CODE==(sinOR1|sin23) ||
				sRecordItem.sItemInfo.CODE==(sinOR1|sin24)*/
			)
			{
				memcpy( &sItem_Buff.sItemInfo , &sRecordItem.sItemInfo , sizeof( sITEMINFO ) );
				OverDay_Item_Delete( &sItem_Buff );
			}
#endif

		if ( DeleteEventItem_TimeOut( &sRecordItem.sItemInfo )==TRUE ) {
			SetFlag = FALSE;

			wsprintf( szMsgBuff , mgItemTimeOut , sRecordItem.sItemInfo.ItemName );
			AddChatBuff( szMsgBuff , 0 );
		
			if ( sRecordItem.sItemInfo.CODE==(sinOR2|sin01) ||
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin06) || sRecordItem.sItemInfo.CODE==(sinOR2|sin07) || //클랜치프링
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin08) || sRecordItem.sItemInfo.CODE==(sinOR2|sin09) || //클랜치프링
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin10) || sRecordItem.sItemInfo.CODE==(sinOR2|sin11) || //클랜치프링
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin12) || sRecordItem.sItemInfo.CODE==(sinOR2|sin13) || //클랜치프링
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin14) || sRecordItem.sItemInfo.CODE==(sinOR2|sin15) || //클랜치프링
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin16) || sRecordItem.sItemInfo.CODE==(sinOR2|sin17) || //클랜치프링
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin18) || sRecordItem.sItemInfo.CODE==(sinOR2|sin19) || //클랜치프링
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin20) || sRecordItem.sItemInfo.CODE==(sinOR2|sin21) || //클랜치프링
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin22) || sRecordItem.sItemInfo.CODE==(sinOR2|sin23) || //클랜치프링 
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin24) || sRecordItem.sItemInfo.CODE==(sinOR2|sin25) || //클랜치프링
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin31) || sRecordItem.sItemInfo.CODE==(sinOR2|sin32) || // 박재원 - 보스 몬스터 링 추가(바벨, 퓨리)
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin27) || sRecordItem.sItemInfo.CODE==(sinOA1|sin32) || //박재원 - 산타링, 산타아뮬렛 추가
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin28) || sRecordItem.sItemInfo.CODE==(sinOA1|sin33) || //박재원 - 이벤트 링, 이벤트 아뮬렛 추가(7일)
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin29) || sRecordItem.sItemInfo.CODE==(sinOA1|sin34) || //박재원 - 이벤트 링, 이벤트 아뮬렛 추가(1시간)
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin30) || sRecordItem.sItemInfo.CODE==(sinOA1|sin35) || //박재원 - 이벤트 링, 이벤트 아뮬렛 추가(1일)
				 sRecordItem.sItemInfo.CODE==(sinOA1|sin36) || sRecordItem.sItemInfo.CODE==(sinOA1|sin37) || // 장별 - 눈꽃 목걸이, 하트 아뮬렛
				  sRecordItem.sItemInfo.CODE==(sinOR2|sin33) ||																		// 장별 - 하트링

				 
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin32) || sRecordItem.sItemInfo.CODE==(sinDA2|sin32) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin33) || sRecordItem.sItemInfo.CODE==(sinDA2|sin33) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin34) || sRecordItem.sItemInfo.CODE==(sinDA2|sin34) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin35) || sRecordItem.sItemInfo.CODE==(sinDA2|sin35) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin36) || sRecordItem.sItemInfo.CODE==(sinDA2|sin36) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin37) || sRecordItem.sItemInfo.CODE==(sinDA2|sin37) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin38) || sRecordItem.sItemInfo.CODE==(sinDA2|sin38) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin39) || sRecordItem.sItemInfo.CODE==(sinDA2|sin39) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin40) || sRecordItem.sItemInfo.CODE==(sinDA2|sin40) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin41) || sRecordItem.sItemInfo.CODE==(sinDA2|sin41) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin42) || sRecordItem.sItemInfo.CODE==(sinDA2|sin42) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin43) || sRecordItem.sItemInfo.CODE==(sinDA2|sin46) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin44) || sRecordItem.sItemInfo.CODE==(sinDA2|sin44) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin45) || sRecordItem.sItemInfo.CODE==(sinDA2|sin45) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin46) || sRecordItem.sItemInfo.CODE==(sinDA2|sin46) ||
				 sRecordItem.sItemInfo.CODE==(sinSP1|sin34) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin54) || sRecordItem.sItemInfo.CODE==(sinDA1|sin55) ||
				 sRecordItem.sItemInfo.CODE==(sinDA2|sin54) || sRecordItem.sItemInfo.CODE==(sinDA2|sin55) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin60) || sRecordItem.sItemInfo.CODE==(sinDA1|sin61) ||
				 sRecordItem.sItemInfo.CODE==(sinDA2|sin60) || sRecordItem.sItemInfo.CODE==(sinDA2|sin61) ||

				sRecordItem.sItemInfo.CODE == (sinOR2|sin36)	||
				sRecordItem.sItemInfo.CODE == (sinOR2|sin37)	||
				sRecordItem.sItemInfo.CODE == (sinOR2|sin38)	||
				sRecordItem.sItemInfo.CODE == (sinOR2|sin39)	||
				sRecordItem.sItemInfo.CODE == (sinOR2|sin40)	||
				sRecordItem.sItemInfo.CODE == (sinOA1|sin39)	||
				sRecordItem.sItemInfo.CODE == (sinOA1|sin40)	||
				sRecordItem.sItemInfo.CODE == (sinOA1|sin41)	||
				sRecordItem.sItemInfo.CODE == (sinOA1|sin42)	||

				sRecordItem.sItemInfo.CODE == (sinOA1|sin38)	||
				sRecordItem.sItemInfo.CODE == (sinOR2|sin34)	||
				sRecordItem.sItemInfo.CODE == (sinOR2|sin35)  || 

				sRecordItem.sItemInfo.CODE == (sinDS1|sin31)	||
				sRecordItem.sItemInfo.CODE == (sinDS1|sin32)	||
				sRecordItem.sItemInfo.CODE == (sinDS1|sin33)    ||
				
				
				sRecordItem.sItemInfo.expireTime > 0
				)

			{
				memcpy( &sItem_Buff.sItemInfo , &sRecordItem.sItemInfo , sizeof( sITEMINFO ) );
			}
		}

		if ( SetFlag ) {

			if ( dwItemCode!=sinPM1 && dwItemCode!=sinPL1 && dwItemCode!=sinPS1 ) {
				for( cnt3=0;cnt3<ChkCnt;cnt3++ ) {
					if ( sRecordItem.sItemInfo.ItemName[0] && CompareItems( &sRecordItem.sItemInfo , &lpChkItem[cnt3]->sItemInfo )==TRUE ) {
						CopyItemCount++;
						break;
					}
				}
			}
			else {
				cnt3 = ChkCnt;
			}

			//2차 보안 값
			if ( !sRecordItem.sItemInfo.Temp0 )
				sRecordItem.sItemInfo.Temp0 = sRecordItem.sItemInfo.ItemHeader.dwChkSum+sRecordItem.sItemInfo.CODE;

			if ( cnt3>=ChkCnt ) {

				if ( cnt<TransRecordData.ItemSubStart ) 
					lpSaveItem = &lpItems[sRecordItem.ItemCount];
				else
					lpSaveItem = &lpItems2[sRecordItem.ItemCount];


				if( sRecordItem.ItemPosition==-1 ) {
					//마우스에 들고 있는 아이템
					if ( lpMouseItem ) 
						lpSaveItem = lpMouseItem;
					else
						lpSaveItem = &lpItems2[INVENTORY_MAXITEM-1];

					sRecordItem.ItemPosition = 0;
				}

				lpSaveItem->x = sRecordItem.x;
				lpSaveItem->y = sRecordItem.y;
				lpSaveItem->ItemPosition = sRecordItem.ItemPosition;
				lpSaveItem->Flag = TRUE;
				memcpy( &lpSaveItem->sItemInfo, &sRecordItem.sItemInfo , sizeof( sITEMINFO ) );

				if ( dwItemCode!=sinPM1 && dwItemCode!=sinPL1 && dwItemCode!=sinPS1 ) 
					lpChkItem[ChkCnt++] = lpSaveItem;

			}
/*
			if ( ReformItemCount<5 && lpsItemInfo ) {
			//if ( ReformItemCount<100 && lpsItemInfo ) {
				//아이템을 서버에 보내서 확인
				if ( lpsItemInfo->ItemName[0] ) {
					dwLastCheckItemTime = 0;
					SendCheckItemToServer( lpsItemInfo );
				}
			}
*/
		}
		lpRecItem += ((int *)lpRecItem)[0];
	}

	//게임 진행 데이타 기록
	RestoreGameData( &TransRecordData.GameSaveInfo );

	//저장 안된 돈 복구
	if ( TransRecordData.GameSaveInfo.LastMoeny>0 ) {
		CheckCharForm();	//캐릭터 정보 인증 확인
		lpCharInfo->Money = TransRecordData.GameSaveInfo.LastMoeny-1;
		ReformCharForm();	//캐릭터 정보 인증 받기
	}
/*
	//보유할수 있는돈
	cnt = lpCharInfo->Level*5-40;
	if ( cnt<10 ) cnt=10;
	cnt *= 10000;
	if ( lpCharInfo->Money>cnt ) {
		SendSetHackUser2( 1960, lpCharInfo->Money );			//돈 한도 초과
		CheckCharForm();	//캐릭터 정보 인증 확인
		lpCharInfo->Money = cnt;
		ReformCharForm();	//캐릭터 정보 인증 받기
		SendSaveMoney();
	}
*/

	if ( CopyItemCount ) {
		//복사 아이템을 소지한 사람 신고 
		SendCopyItemUser( CopyItemCount );
	}
	if ( BadItemCount ) {
		//금지된 아이템을 소지한 사람 신고
		SendCopyItemUser( BadItemCount+10000 );
	}
	if ( PostionError ) {
		PostionError += 10000;
		SendSetHackUser2( 1900,PostionError );		//무계오류에 값을 삽입
		SendSetHackUser2( 99,0 );					//무계오류에 값을 삽입
	}

	if ( SvrPotionError ) {
		SvrPotionError += 20000;
		SendSetHackUser2( 1900,SvrPotionError );	//무계오류에 값을 삽입
		SendSetHackUser2( 99,0 );					//무계오류에 값을 삽입
	}



	//UnlockResource( hResource );
	CheckCharForm();
	cInvenTory.LoadItemInfo();
	CheckCharForm();

	//스킬 복구
	RestoreSkill( &TransRecordData.GameSaveInfo.SkillInfo , TransRecordData.GameSaveInfo.dwLevelQuestLog );
	//기본공격 단축기 복구
	sinSkill.UseSkill[0].ShortKey = TransRecordData.GameSaveInfo.ShortKey_NormalAttack&0xF;
	sinSkill.UseSkill[0].MousePosi = TransRecordData.GameSaveInfo.ShortKey_NormalAttack>>4;
	
	// 박재원 - 부스터 아이템(생명력) 복구
	if ( TransRecordData.GameSaveInfo.wLifeBoosterUsing[0] && TransRecordData.GameSaveInfo.wLifeBoosterUsing[1] ) {
		cSkill.SetBoosterItem( sinBC1+TransRecordData.GameSaveInfo.wLifeBoosterUsing[0] , TransRecordData.GameSaveInfo.wLifeBoosterUsing[1]*60 );

		lpCurPlayer->dwLifeBoosterCode = sinBC1+TransRecordData.GameSaveInfo.wLifeBoosterUsing[0];	// 부스터 적용 코드
		lpCurPlayer->dwLifeBoosterTime = dwPlayTime + (DWORD)TransRecordData.GameSaveInfo.wLifeBoosterUsing[1]*1000; // 부스터 사용 후 남은 시간 복구
	}
	// 박재원 - 부스터 아이템(기력) 복구
	if ( TransRecordData.GameSaveInfo.wManaBoosterUsing[0] && TransRecordData.GameSaveInfo.wManaBoosterUsing[1] ) {
		cSkill.SetBoosterItem( sinBC1+TransRecordData.GameSaveInfo.wManaBoosterUsing[0] , TransRecordData.GameSaveInfo.wManaBoosterUsing[1]*60 );

		lpCurPlayer->dwManaBoosterCode = sinBC1+TransRecordData.GameSaveInfo.wManaBoosterUsing[0];	// 부스터 적용 코드
		lpCurPlayer->dwManaBoosterTime = dwPlayTime + (DWORD)TransRecordData.GameSaveInfo.wManaBoosterUsing[1]*1000; // 부스터 사용 후 남은 시간 복구
	}
	// 박재원 - 부스터 아이템(근력) 복구
	if ( TransRecordData.GameSaveInfo.wStaminaBoosterUsing[0] && TransRecordData.GameSaveInfo.wStaminaBoosterUsing[1] ) {
		cSkill.SetBoosterItem( sinBC1+TransRecordData.GameSaveInfo.wStaminaBoosterUsing[0] , TransRecordData.GameSaveInfo.wStaminaBoosterUsing[1]*60 );

		lpCurPlayer->dwStaminaBoosterCode = sinBC1+TransRecordData.GameSaveInfo.wStaminaBoosterUsing[0];	// 부스터 적용 코드
		lpCurPlayer->dwStaminaBoosterTime = dwPlayTime + (DWORD)TransRecordData.GameSaveInfo.wStaminaBoosterUsing[1]*1000; // 부스터 사용 후 남은 시간 복구
	}

	// 장별 - 스킬 딜레이
	if ( TransRecordData.GameSaveInfo.wSkillDelayUsing[0] && TransRecordData.GameSaveInfo.wSkillDelayUsing[1] ) {
		cSkill.SetSkillDelayItem( sinBC1+TransRecordData.GameSaveInfo.wSkillDelayUsing[0] , TransRecordData.GameSaveInfo.wSkillDelayUsing[1]*60 );

		lpCurPlayer->dwSkillDelayCode = sinBC1+TransRecordData.GameSaveInfo.wSkillDelayUsing[0];	
		lpCurPlayer->dwSkillDelayTime = dwPlayTime + (DWORD)TransRecordData.GameSaveInfo.wSkillDelayUsing[1]*1000;
	}

	//포스 오브 복구
	if ( TransRecordData.GameSaveInfo.wForceOrbUsing[0] && TransRecordData.GameSaveInfo.wForceOrbUsing[1] ) {
		// 박재원 - 빌링 매직 포스 추가 
		if( TransRecordData.GameSaveInfo.wForceOrbUsing[0]>=sin01 && TransRecordData.GameSaveInfo.wForceOrbUsing[0]<=sin16 ) // 일반 포스
		{
			cInvenTory.SetForceOrb( sinFO1+TransRecordData.GameSaveInfo.wForceOrbUsing[0] , TransRecordData.GameSaveInfo.wForceOrbUsing[1] );
		}
		else if( TransRecordData.GameSaveInfo.wForceOrbUsing[0]>=sin21 && TransRecordData.GameSaveInfo.wForceOrbUsing[0]<=sin32 ) // 매직 포스
		{
			cInvenTory.SetMagicForceOrb( sinFO1+TransRecordData.GameSaveInfo.wForceOrbUsing[0] , TransRecordData.GameSaveInfo.wForceOrbUsing[1] );
		}
		else if( TransRecordData.GameSaveInfo.wForceOrbUsing[0]>=sin35 && TransRecordData.GameSaveInfo.wForceOrbUsing[0]<=sin37 ) // 빌링 매직 포스
		{
			cInvenTory.SetBillingMagicForceOrb( sinFO1+TransRecordData.GameSaveInfo.wForceOrbUsing[0] , TransRecordData.GameSaveInfo.wForceOrbUsing[1] );
		}

		lpCurPlayer->dwForceOrbCode = sinFO1+TransRecordData.GameSaveInfo.wForceOrbUsing[0];		//포스오브 적용 코드
		lpCurPlayer->dwForceOrbTime = dwPlayTime + (DWORD)TransRecordData.GameSaveInfo.wForceOrbUsing[1]*1000;
	}

	//프리미엄 아이템 사용시간 표시
	int	PrimeItem_Time;
	if ( TransRecordData.GameSaveInfo.dwTime_PrimeItem_X2 ) {
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_X2;
		if ( PrimeItem_Time>0 ){
			chaPremiumitem.SetThirdEyesTime( PrimeItem_Time );
			switch( TransRecordData.GameSaveInfo.dwPrimeItem_PackageCode){
				case PRIME_ITEM_PACKAGE_NONE:
					chaPremiumitem.SetUpKeepItem(nsPremiumItem::THIRD_EYES,chaPremiumitem.m_ThirdEyesTime,true,UpKeepItemName[0],50);
				break;
				case PRIME_ITEM_PACKAGE_BRONZE:
					chaPremiumitem.SetUpKeepItem(nsPremiumItem::THIRD_EYES,chaPremiumitem.m_ThirdEyesTime,true,UpKeepItemName[0],20);
				break;
				case PRIME_ITEM_PACKAGE_SILVER:	
					chaPremiumitem.SetUpKeepItem(nsPremiumItem::THIRD_EYES,chaPremiumitem.m_ThirdEyesTime,true,UpKeepItemName[0],30);
				break;
				case PRIME_ITEM_PACKAGE_GOLD:
					chaPremiumitem.SetUpKeepItem(nsPremiumItem::THIRD_EYES,chaPremiumitem.m_ThirdEyesTime,true,UpKeepItemName[0],40);
				break;
				case PRIME_ITEM_PACKAGE_ULTRA:	// pluto 슈페리어 패키지
					chaPremiumitem.SetUpKeepItem(nsPremiumItem::THIRD_EYES,chaPremiumitem.m_ThirdEyesTime,true,UpKeepItemName[0],50);
				break;
				default:	// 박재원 - 경험치증가 포션(50%)아이템과 중복 사용할 경우(PRIME_ITEM_PACKAGE_NONE_50_EXPUP , PRIME_ITEM_PACKAGE_NONE_100_EXPUP)
					chaPremiumitem.SetUpKeepItem(nsPremiumItem::THIRD_EYES,chaPremiumitem.m_ThirdEyesTime,true,UpKeepItemName[0],50);
				break;
			}
		}
	}
	if ( TransRecordData.GameSaveInfo.dwTime_PrimeItem_ExpUp ) {
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_ExpUp;
		if ( PrimeItem_Time>0 ){
			chaPremiumitem.SetExpUpPotionTime( PrimeItem_Time );
			switch( TransRecordData.GameSaveInfo.dwPrimeItem_PackageCode){
			case PRIME_ITEM_PACKAGE_NONE:
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION,chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],30);
				break;
			case PRIME_ITEM_PACKAGE_BRONZE:
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION,chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],10);
				break;
			case PRIME_ITEM_PACKAGE_SILVER:	
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION,chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],20);
				break;
			case PRIME_ITEM_PACKAGE_GOLD:
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION,chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],30);
				break;
#ifdef _LANGUAGE_VEITNAM
			case PRIME_ITEM_PACKAGE_ULTRA:	//베트남요청
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION, chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],50);
				break;
#else
			case PRIME_ITEM_PACKAGE_ULTRA:
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION, chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],40);
				break;
#endif

			case PRIME_ITEM_PACKAGE_BRONZE2:	// pluto 브론즈 패키지2
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION,chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],10);
				break;
			case PRIME_ITEM_PACKAGE_SILVER2:	// pluto 실버 패키지2
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION,chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],20);
				break;
			case PRIME_ITEM_PACKAGE_GOLD2:	// pluto 골드 패키지2
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION,chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],30);
				break;
			case PRIME_ITEM_PACKAGE_ULTRA2: // pluto 슈페리어 패키지2
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION, chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],40);
				break;
			case PRIME_ITEM_PACKAGE_NONE_50_EXPUP: // 박재원 - 경험치증가 포션(50%) 아이템 전용 
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION, chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],50);
				break;
			case PRIME_ITEM_PACKAGE_NONE_100_EXPUP: // 박재원 - 경험치증가 포션(100%) 아이템 전용 
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION, chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],100);
				break;
			}
		}
	}

	if ( TransRecordData.GameSaveInfo.dwTime_PrimeItem_VampCuspid ) {
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_VampCuspid;
		if ( PrimeItem_Time>0 ){
			chaPremiumitem.SetVampiricCuspidTime( PrimeItem_Time );
			chaPremiumitem.SetUpKeepItem(nsPremiumItem::VAMPIRIC_CUS,chaPremiumitem.m_VampiricCuspidTime,true,UpKeepItemName[2]);
		}
	}
	if ( TransRecordData.GameSaveInfo.dwTime_PrimeItem_ManaRecharg ) {
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_ManaRecharg;
		if ( PrimeItem_Time>0 ){
			chaPremiumitem.SetManaRechargingPTime( PrimeItem_Time );
			chaPremiumitem.SetUpKeepItem(nsPremiumItem::MANA_RECHAR_P,chaPremiumitem.m_ManaRechargingPTime,true,UpKeepItemName[3]);
		}
	}

	// 장별 - 뱀피릭 커스핏 EX
	if ( TransRecordData.GameSaveInfo.dwTime_PrimeItem_VampCuspid_EX ) {
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_VampCuspid_EX;
		if ( PrimeItem_Time>0 ){
			chaPremiumitem.SetVampiricCuspidEXTime( PrimeItem_Time );
			chaPremiumitem.SetUpKeepItem(nsPremiumItem::VAMPIRIC_CUS_EX,chaPremiumitem.m_VampiricCuspidEXTime,true,UpKeepItemName[11]);
		}
	}

	/*
	if (TransRecordData.GameSaveInfo.dwPrimeItem_PackageCode ) {
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwPrimeItem_PackageCode - GetPlayTime_T();
		if( PrimeItem_Time < 0 ){
			
			chaPremiumitem.SetThirdEyesTime( PrimeItem_Time );
			chaPremiumitem.SetUpKeepItem(nsPremiumItem::THIRD_EYES,chaPremiumitem.m_ThirdEyesTime,true,UpKeepItemName[0]);
			
			chaPremiumitem.SetExpUpPotionTime( PrimeItem_Time );
			chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION,chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1]);
		}
	}
	*/

	// pluto 마나 리듀스 포션
	if( TransRecordData.GameSaveInfo.dwTime_PrimeItem_ManaReduce )
	{
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_ManaReduce;
		if( PrimeItem_Time > 0 )
		{
			chaPremiumitem.SetManaReducePotionTime( PrimeItem_Time );
			switch( TransRecordData.GameSaveInfo.dwPrimeItem_PackageCode )
			{
				case PRIME_ITEM_PACKAGE_NONE:
					chaPremiumitem.SetManaReducePotionValue( 30 );	// pluto 마나 리듀스 포션 30% 감소
					chaPremiumitem.SetUpKeepItem( nsPremiumItem::MANA_REDUCE_P, chaPremiumitem.m_ManaReducePotiontime, true, UpKeepItemName[5], 30);
					break;
				case PRIME_ITEM_PACKAGE_BRONZE2:
					chaPremiumitem.SetManaReducePotionValue( 10 );	// pluto 마나 리듀스 포션 10% 감소
					chaPremiumitem.SetUpKeepItem( nsPremiumItem::MANA_REDUCE_P, chaPremiumitem.m_ManaReducePotiontime, true, UpKeepItemName[5], 10);
					break;
				case PRIME_ITEM_PACKAGE_SILVER2:
					chaPremiumitem.SetManaReducePotionValue( 20 );	// pluto 마나 리듀스 포션 20% 감소
					chaPremiumitem.SetUpKeepItem( nsPremiumItem::MANA_REDUCE_P, chaPremiumitem.m_ManaReducePotiontime, true, UpKeepItemName[5], 20);
					break;
				case PRIME_ITEM_PACKAGE_GOLD2:
					chaPremiumitem.SetManaReducePotionValue( 30 );	// pluto 마나 리듀스 포션 30% 감소
					chaPremiumitem.SetUpKeepItem( nsPremiumItem::MANA_REDUCE_P, chaPremiumitem.m_ManaReducePotiontime, true, UpKeepItemName[5], 30);
					break;
				case PRIME_ITEM_PACKAGE_ULTRA2:
					chaPremiumitem.SetManaReducePotionValue( 40 );	// pluto 마나 리듀스 포션 40% 감소
					chaPremiumitem.SetUpKeepItem( nsPremiumItem::MANA_REDUCE_P, chaPremiumitem.m_ManaReducePotiontime, true, UpKeepItemName[5], 40);
					break;
				default:	// 땜빵
					chaPremiumitem.SetManaReducePotionValue( 30 );	// pluto 마나 리듀스 포션 30% 감소
					chaPremiumitem.SetUpKeepItem( nsPremiumItem::MANA_REDUCE_P, chaPremiumitem.m_ManaReducePotiontime, true, UpKeepItemName[5], 30);
					break;
			}
		}
	}
	// pluto 마이트 오브 아웰
	if( TransRecordData.GameSaveInfo.dwTime_PrimeItem_MightofAwell )
	{
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_MightofAwell;
		if( PrimeItem_Time > 0 )
		{
			chaPremiumitem.SetMightOfAwellTime( PrimeItem_Time );
			chaPremiumitem.SetMightOfAwellWeight( 300 );
			chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 300);
			//switch( TransRecordData.GameSaveInfo.dwPrimeItem_PackageCode )
			//{
			//	case PRIME_ITEM_PACKAGE_NONE:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 20);
			//		break;
			//	case PRIME_ITEM_PACKAGE_BRONZE2:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 10);
			//		break;
			//	case PRIME_ITEM_PACKAGE_SILVER2:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 20);
			//		break;
			//	case PRIME_ITEM_PACKAGE_GOLD2:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 30);
			//		break;
			//	case PRIME_ITEM_PACKAGE_ULTRA2:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 40);
			//		break;
			//}
		}
	}
	// pluto 마이트 오브 아웰2
	if( TransRecordData.GameSaveInfo.dwTime_PrimeItem_MightofAwell2 )
	{
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_MightofAwell2;
		if( PrimeItem_Time > 0 )
		{
			chaPremiumitem.SetMightOfAwellTime( PrimeItem_Time );
			chaPremiumitem.SetMightOfAwellWeight( 500 );
			chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 500);
			//switch( TransRecordData.GameSaveInfo.dwPrimeItem_PackageCode )
			//{
			//	case PRIME_ITEM_PACKAGE_NONE:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 20);
			//		break;
			//	case PRIME_ITEM_PACKAGE_BRONZE2:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 10);
			//		break;
			//	case PRIME_ITEM_PACKAGE_SILVER2:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 20);
			//		break;
			//	case PRIME_ITEM_PACKAGE_GOLD2:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 30);
			//		break;
			//	case PRIME_ITEM_PACKAGE_ULTRA2:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 40);
			//		break;
			//}
		}
	}

	// pluto 펫(해외)
	if( TransRecordData.GameSaveInfo.dwTime_PrimeItem_PhenixPet )
	{
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_PhenixPet;
		if( PrimeItem_Time > 0 )
		{
			chaPremiumitem.SetPhenixPetTime( PrimeItem_Time );
			chaPremiumitem.SetUpKeepItem( nsPremiumItem::PHENIX_PET, chaPremiumitem.m_PhenixPetTime, true, UpKeepItemName[6], 500);
		}
		else if( PrimeItem_Time < 0 ) 
		{
			lpCharInfo->GravityScroolCheck[1] = 0;
		}
	}

	// 박재원 - 빌링 도우미 펫 추가
	if( TransRecordData.GameSaveInfo.dwTime_PrimeItem_HelpPet )
	{
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_HelpPet;
		if( PrimeItem_Time > 0 )
		{
			chaPremiumitem.SetHelpPetTimeTime( PrimeItem_Time );
			//	chaPremiumitem.SetUpKeepItem( nsPremiumItem::HELP_PET, chaPremiumitem.m_HelpPetTime, true, UpKeepItemName[6], 500);

			 for( int m = 0 ; m < SINUPKEEPITEM_MAX ; m++ )
			{
				if(chaPremiumitem.UpKeepItem[m].TGAImageNumber == nsPremiumItem::HELP_PET_TERRY)
				{
					lpCharInfo->GravityScroolCheck[1] = 1;
				}
			}

		}
		else if( PrimeItem_Time < 0 ) 
		{
			lpCharInfo->GravityScroolCheck[1] = 0;
		}
	}

	else 
	{
		lpCharInfo->GravityScroolCheck[1] = 0;
	}

	// 박재원 - 근력 리듀스 포션
	if( TransRecordData.GameSaveInfo.dwTime_PrimeItem_StaminaReduce )
	{
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_StaminaReduce;
		if( PrimeItem_Time > 0 )
		{
			chaPremiumitem.SetStaminaReducePotionTime( PrimeItem_Time );
			chaPremiumitem.SetStaminaReducePotionValue( 30 );	// 박재원 - 근력 리듀스 포션 30% 감소
			chaPremiumitem.SetUpKeepItem( nsPremiumItem::STAMINA_REDUCE_P, chaPremiumitem.m_StaminaReducePotiontime, true, UpKeepItemName[12], 30);

		}
	}

	return TRUE;
}


//저장할 데이타를 분할하여 서버로 전송
int rsRECORD_DBASE::SendRecordDataToServer( smWINSOCK *lpsmSock  )
{
	int cnt;
	int PartTotal;
	char *lpData;
	int TotalLen;
	int len;

	TRANS_RECORD_DATAS TransRecord;
	TRANS_RECORD_DATA *lpTransRecord;

	lpTransRecord = &TransRecordData;
	lpData = (char *)lpTransRecord;

	cnt = 0;
	TotalLen = 0;
	PartTotal = lpTransRecord->size / TRANS_RECORD_LEN;
	if ( (lpTransRecord->size%TRANS_RECORD_LEN)!=0 ) {
		if ( lpTransRecord->size>TRANS_RECORD_LEN )
			PartTotal++;
	}

	while(1) {

		len = lpTransRecord->size - TotalLen;
		if ( len>TRANS_RECORD_LEN ) len = TRANS_RECORD_LEN;

		TransRecord.code = smTRANSCODE_RECORDDATA;
		TransRecord.size = len+32;
		TransRecord.Count = cnt;
		TransRecord.Total = PartTotal;
		TransRecord.TransSize = len;

		memcpy( TransRecord.szData , &lpData[TotalLen] , len );

		lpsmSock->Send( (char *)&TransRecord , TransRecord.size , TRUE );
		cnt++;

		TotalLen+=len;
		if ( TotalLen>=lpTransRecord->size ) break;
	}

	return TRUE;
}

extern int RecordHackLogMoney( smCHAR_INFO *lpCharInfo );

int rsRECORD_DBASE::SendRecordDataToClient( rsPLAYINFO *lpPlayInfo , char *szName , smWINSOCK *lpsmSock , int Mode )
{
	int cnt;
	int PartTotal;
	char *lpData;
	int TotalLen;
	int len;
	char szFile[256];
	FILE *fp;
	INT64	exp64;
	smTRANS_COMMAND	smTransCommand;

	TRANS_RECORD_DATAS TransRecord;
	TRANS_RECORD_DATA *lpTransRecord;

	lpData = (char *)&TransRecordData;
	lpTransRecord = &TransRecordData;

	switch( Mode ) {
		case 0:
			GetUserDataFile( szName , szFile );
			break;

		case 1:
			GetDeleteDataFile( szName , szFile );
			break;

		case 2:
			GetUserDataBackupFile( szName , szFile );
			break;
	}

	if ( Mode>1000 ) {
		GetUserDataFile_BackupDay( szName , szFile , Mode );

		WIN32_FIND_DATA		ffd;
		HANDLE				hFind;
 		hFind = FindFirstFile( szFile , &ffd );
		if ( hFind==INVALID_HANDLE_VALUE ) return FALSE;
		FindClose( hFind );
	}

/*
	if ( Mode==TRUE ) {
		//지운파일 불러오기
		GetDeleteDataFile( szName , szFile );
	}
	else {
		//파일불러오기
		GetUserDataFile( szName , szFile );
	}
*/
	if ( sRecDataBuff ) EnterCriticalSection( &cSaveDataSection );

	//저장 대기중인 데이타 있는지 확인
	if ( CheckRecWaitData( szName )==TRUE ) return FALSE;

	lpTransRecord->code = 0;
	lpTransRecord->size = 0;

	fp = fopen( szFile , "rb" );
	if ( fp ) {
		fread( lpTransRecord , sizeof(TRANS_RECORD_DATA) , 1, fp );
		fclose( fp );
	}
	else {
		if ( sRecDataBuff ) LeaveCriticalSection( &cSaveDataSection );
		return FALSE;
	}
	if ( sRecDataBuff ) LeaveCriticalSection( &cSaveDataSection );


	if ( lpTransRecord->code==0 && lpTransRecord->size==0 ) 
		return FALSE;			//임시 저장 데이타 이므로 실패 ( 신규 캐릭을 만든경우임 )

	if ( lpTransRecord->size<0 || lpTransRecord->size>sizeof(TRANS_RECORD_DATA) )
		return FALSE;			//오류 데이타

/*
	//상위 32비트 경험치 초기화
	if ( lpTransRecord->smCharInfo.Level<80 && lpTransRecord->smCharInfo.Exp_High ) {
		lpTransRecord->smCharInfo.Exp_High = 0;
	}
*/

	exp64 = GetExp64( &lpTransRecord->smCharInfo );
	if ( exp64<0 ) {	//경험치 오류 0보다 작다
		exp64 = 0;
		SetExp64( &lpTransRecord->smCharInfo , exp64 );
	}

	//레벨과 경험치가 맞는지 확인
	if ( CheckLevelExp( lpTransRecord->smCharInfo.Level , exp64 )==FALSE ) {
		//경험치로 레벨 추산
		lpTransRecord->smCharInfo.Level = GetLevelFromExp( exp64 );
	}

#ifndef _MODE_EXP64
	if ( lpTransRecord->smCharInfo.Level>=80 || GetExp64( &lpTransRecord->smCharInfo )>ExpLevelTable[99] ) {
		smTransCommand.WParam = 8560;
		smTransCommand.LParam = lpTransRecord->smCharInfo.Level;
		smTransCommand.SParam = lpTransRecord->smCharInfo.Exp;
		smTransCommand.EParam = 0;
		RecordHackLogFile( lpPlayInfo , &smTransCommand );

		//레벨 1 로 다운
		lpTransRecord->smCharInfo.Level = 1;
		lpTransRecord->smCharInfo.Exp = 0;
		lpTransRecord->smCharInfo.Exp_High = 0;
		fp = fopen( szFile , "wb" );
		if ( fp ) {
			fwrite( &TransRecordData , sizeof(TRANS_RECORD_DATA) , 1, fp );
			fclose( fp );
		}
		return FALSE;
	}
#endif

	lpTransRecord->smCharInfo.Next_Exp = (int)GetNextExp( lpTransRecord->smCharInfo.Level );

	ReformCharStatePoint( &lpTransRecord->smCharInfo , lpTransRecord->GameSaveInfo.dwLevelQuestLog );

	lpTransRecord->smCharInfo.ClassClan = 0;

	if ( lpTransRecord->smCharInfo.Level<20 && lpTransRecord->smCharInfo.ChangeJob!=0 ) {
		lpTransRecord->smCharInfo.ChangeJob = 0;
	}

	lpTransRecord->GameSaveInfo.PCRNo			= lpPlayInfo->Bl_RNo;
	lpTransRecord->GameSaveInfo.EventPlay[0]	= lpPlayInfo->Bl_Meter;

	lpTransRecord->GameSaveInfo.sPotionUpdate[1] = rsServerConfig.PotionMonitor;

	lpTransRecord->GameSaveInfo.BlessCastleTax = rsBlessCastle.Tax;
	lpTransRecord->GameSaveInfo.dwBlessCastleMaster = rsBlessCastle.dwMasterClan;

	lpPlayInfo->sLowLevel_PresentItem[0] = lpTransRecord->smCharInfo.sPresentItem[0];
	lpPlayInfo->sLowLevel_PresentItem[1] = lpTransRecord->smCharInfo.sPresentItem[1];
	rsUpdateServerParam( lpPlayInfo , smUPDATE_PARAM_LOWUSER_PRESENT , lpPlayInfo->sLowLevel_PresentItem[0] , lpPlayInfo->sLowLevel_PresentItem[1] , FALSE );


	if ( lpPlayInfo->AdminMode && lpPlayInfo->sLowLevel_PresentItem[0]==0 )
		lpPlayInfo->sLowLevel_PresentItem[0] = 1;

	if ( lpPlayInfo->szServerID[0] )
		lpTransRecord->GameSaveInfo.TT_ServerID = ((DWORD *)lpPlayInfo->szServerID)[0];
	else
		lpTransRecord->GameSaveInfo.TT_ServerID = 0;

	if ( lpTransRecord->GameSaveInfo.PlayStageNum==rsSOD_FIELD ) lpTransRecord->GameSaveInfo.PlayStageNum = rsSOD_VILLAGE;
	if ( lpTransRecord->GameSaveInfo.PlayStageNum==QUEST_ARENA_FIELD ) {
		if ( lpTransRecord->smCharInfo.JOB_CODE<=4 )
			lpTransRecord->GameSaveInfo.PlayStageNum = START_FIELD_NUM;
		else
			lpTransRecord->GameSaveInfo.PlayStageNum = START_FIELD_MORYON;
	}

	if ( !lpPlayInfo->dwObjectSerial )
		lpTransRecord->smCharInfo.dwObjectSerial = GetNewObjectSerial();
	else
		lpTransRecord->smCharInfo.dwObjectSerial = lpPlayInfo->dwObjectSerial;

	if ( rsServerConfig.Event_ComicBirthDay==1 && (lpPlayInfo->Bl_Meter&BIMASK_BIRTHDAY_USER)!=0 ) {
		lpTransRecord->smCharInfo.SizeLevel = (rand()%1)+0x1001;
		lpTransRecord->smCharInfo.dwEventTime_T = ( DWORD )tServerTime + ( 60 * 24 * 6 );
	}

	if ( (lpPlayInfo->Bl_Meter&BIMASK_VIP_USER)!=0 && rsServerConfig.FreeLevel && lpTransRecord->smCharInfo.Level<rsServerConfig.FreeLevel ) {
		lpTransRecord->GameSaveInfo.EventPlay[0] -=BIMASK_VIP_USER;
	}

	if ( rsServerConfig.EventPlay_BitMask )
		lpTransRecord->GameSaveInfo.EventPlay[0] |= (short)rsServerConfig.EventPlay_BitMask;		


	if ( !lpPlayInfo->AdminMode ) {
		if ( lpTransRecord->smCharInfo.dwCharSoundCode!=0 || lpTransRecord->smCharInfo.State!=smCHAR_STATE_USER ) {

			smTransCommand.WParam = 8730;
			smTransCommand.LParam = lpTransRecord->smCharInfo.State;
			smTransCommand.SParam = lpTransRecord->smCharInfo.dwCharSoundCode;
			smTransCommand.EParam = 0;
			RecordHackLogFile( lpPlayInfo , &smTransCommand );
			lpTransRecord->smCharInfo.dwCharSoundCode = 0;
			lpTransRecord->smCharInfo.State = smCHAR_STATE_USER;
		}
	}

	ReformCharForm( &lpTransRecord->smCharInfo );

	if ( rsServerConfig.Event_StarPointTicket ) 
		OpenStarPointEvent( lpPlayInfo , &lpTransRecord->smCharInfo );


	if ( lpTransRecord->GameSaveInfo.Head<140 &&
		lstrcmp( lpTransRecord->szHeader , szRecordHeader )!=0 ) {
		lpTransRecord->GameSaveInfo.dwChkSum_CharInfo = GetCharInfoCode( &lpTransRecord->smCharInfo );
	}

		lpTransRecord->smCharInfo.wVersion[1] = 0;


	if ( rsServerConfig.FixedStartField )
		lpTransRecord->GameSaveInfo.PlayStageNum = rsServerConfig.FixedStartField;

	rsSet_LoginServerSafeKey( &lpTransRecord->smCharInfo );


	cnt = 0;
	TotalLen = 0;
	PartTotal = lpTransRecord->size / TRANS_RECORD_LEN;
	if ( (lpTransRecord->size%TRANS_RECORD_LEN)!=0 ) {
		if ( lpTransRecord->size>TRANS_RECORD_LEN )
			PartTotal++;
	}

	while(1) {

		len = lpTransRecord->size - TotalLen;
		if ( len>TRANS_RECORD_LEN ) len = TRANS_RECORD_LEN;

		TransRecord.code = smTRANSCODE_RECORDDATA;
		TransRecord.size = len+32;
		TransRecord.Count = cnt;
		TransRecord.Total = PartTotal;
		TransRecord.TransSize = len;

		memcpy( TransRecord.szData , &lpData[TotalLen] , len );

		lpsmSock->Send( (char *)&TransRecord , TransRecord.size , TRUE );
		cnt++;

		TotalLen+=len;
		if ( TotalLen>=lpTransRecord->size ) break;
	}

	return TRUE;
}

#define	RECORD_ITEM_INFO_HEAD_SIZE		44

//불러온 아이템 목록을 유저정보에 저장
int rsRECORD_DBASE::MakeRecordItemList( rsPLAYINFO *lpPlayInfo )
{
	int cnt,cnt2;

	BYTE	*lpRecItem;
	sRECORD_ITEM	sRecordItem;
	int		size;
	int		BuffSize;
	smTRANS_COMMAND_EX	smTransCommand;

	sTHROW_ITEM_INFO	ThrowItemInfo[THROW_ITEM_INFO_MAX];			//버려진 아이템 정보
	int					ThrowItemCount;								//버려진 아이템 카운터

	lpRecItem = (BYTE *)&TransRecordData.Data;
	BuffSize = 0;
	ThrowItemCount = 0;

	ZeroMemory( lpPlayInfo->ServerPotion , sizeof(int)*3*4 );

	if ( TransRecordData.ItemCount>RECORD_ITEM_MAX )  TransRecordData.ItemCount = RECORD_ITEM_MAX; 
	if ( TransRecordData.ThrowItemCount>THROW_ITEM_INFO_MAX ) TransRecordData.ThrowItemCount = THROW_ITEM_INFO_MAX;

	for( cnt=0;cnt<TransRecordData.ItemCount;cnt++ ) {
		if ( cnt>=(INVENTORY_MAXITEM*2) ) break;

		if ( rsServerConfig.PotionMonitor ) {
			//압축데이타 서버 아이템 해독용 ( Z/NZ 방식 ) - 물약 계열 아이템을 확인하여 수량 파악
			DecodeCompress_ItemPotion( lpPlayInfo , (BYTE *)lpRecItem , (BYTE *)&sRecordItem , RECORD_ITEM_INFO_HEAD_SIZE , &TransRecordData );
		}
		else {
			//압축 데이타 해독 ( Z/NZ 방식 )
			DecodeCompress( (BYTE *)lpRecItem , (BYTE *)&sRecordItem , RECORD_ITEM_INFO_HEAD_SIZE );
		}

		if ( sRecordItem.sItemInfo.CODE && sRecordItem.sItemInfo.ItemHeader.Head && 
			sRecordItem.sItemInfo.ItemHeader.dwChkSum &&
			(sRecordItem.sItemInfo.CODE&sinITEM_MASK1)!=(sinPM1&sinITEM_MASK1) ) {

			//버려진 아이템 확인
			for( cnt2=0;cnt2<TransRecordData.ThrowItemCount;cnt2++ ) {
				if ( TransRecordData.ThrowItemInfo[cnt2].dwCode==sRecordItem.sItemInfo.CODE &&
					TransRecordData.ThrowItemInfo[cnt2].dwKey==sRecordItem.sItemInfo.ItemHeader.Head &&
					TransRecordData.ThrowItemInfo[cnt2].dwSum==sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {

						if ( ThrowItemCount<THROW_ITEM_INFO_MAX ) {
							//실제로 존재하는 아이템 확인 ( 던져진 아이템 목록 정리 )
							memcpy( &ThrowItemInfo[ThrowItemCount++] , &TransRecordData.ThrowItemInfo[cnt2] ,sizeof(sTHROW_ITEM_INFO) );
						}
						break;
					}
			}

			if ( cnt2>=TransRecordData.ThrowItemCount ) {
				for( cnt2=0;cnt2<cnt;cnt2++ ) {
					//복사아이템 검사
					if ( lpPlayInfo->InvenItemInfo[cnt2].dwCode && 
						lpPlayInfo->InvenItemInfo[cnt2].dwCode==sRecordItem.sItemInfo.CODE &&
						lpPlayInfo->InvenItemInfo[cnt2].dwKey==sRecordItem.sItemInfo.ItemHeader.Head &&
						lpPlayInfo->InvenItemInfo[cnt2].dwSum==sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {

							//로그에 기록
							smTransCommand.WParam = 8070;
							smTransCommand.WxParam = 1;
							smTransCommand.LxParam = (int)"*INVENTORY";
							smTransCommand.LParam = sRecordItem.sItemInfo.CODE;
							smTransCommand.SParam = sRecordItem.sItemInfo.ItemHeader.Head;
							smTransCommand.EParam = sRecordItem.sItemInfo.ItemHeader.dwChkSum;
							RecordHackLogFile( lpPlayInfo , &smTransCommand );
							break;
						}
				}
				if ( cnt2>=cnt ) {
					lpPlayInfo->InvenItemInfo[cnt].dwCode = sRecordItem.sItemInfo.CODE;
					lpPlayInfo->InvenItemInfo[cnt].dwKey  = sRecordItem.sItemInfo.ItemHeader.Head;
					lpPlayInfo->InvenItemInfo[cnt].dwSum  = sRecordItem.sItemInfo.ItemHeader.dwChkSum;
				}
			}
		}

		size = ((int *)lpRecItem)[0];
		BuffSize += size;
		lpRecItem += size;
		if ( BuffSize>=(sizeof(sRECORD_ITEM)*RECORD_ITEM_MAX) ) break;			//버퍼 크기 초과시
	}


	lpPlayInfo->ThrowItemCount = ThrowItemCount;
	memcpy( lpPlayInfo->ThrowItemInfo , ThrowItemInfo , sizeof(sTHROW_ITEM_INFO)*ThrowItemCount );


	return TRUE;
}


//저장할 데이타를 분할하여 받음
int rsRECORD_DBASE::RecvRecordDataFromServer( TRANS_RECORD_DATAS *lpTransRecord )
{

	char *lpData;

	lpData = (char *)&TransRecordData;

	memcpy( &lpData[lpTransRecord->Count*TRANS_RECORD_LEN] ,  lpTransRecord->szData , lpTransRecord->TransSize );
	TransDataBlock[lpTransRecord->Count] = 1;

	int cnt = 0;
	for(cnt=0;cnt<lpTransRecord->Total;cnt++)
		if ( !TransDataBlock[cnt] ) break;

	if ( cnt==lpTransRecord->Total && TransRecordData.code==smTRANSCODE_RECORDDATA ) {
		//수신 완료
		TransRecordData.code=0;
		return TRUE;
	}

/*
	if ( lpTransRecord->Count>0 ) {
		if ( TransLastPartCount!=lpTransRecord->Count-1 ) 
			return FALSE;		//수신 오류
	}

	if ( lpTransRecord->Count>=lpTransRecord->Total-1 && TransRecordData.code==smTRANSCODE_RECORDDATA ) {
		//수신 완료
		TransRecordData.code=0;
		return TRUE;
	}
*/

	TransLastPartCount = lpTransRecord->Count;

	return FALSE;
}

#ifdef _W_SERVER
int	CheckPlayExpTable[15] = {
	40000,			//0
	100000,			//1
	300000,			//2
	500000,			//3
	800000,			//4
	1200000,		//5
	1600000,		//6
	2000000,		//7
	3000000,		//8
	4000000,		//9
	4000000,		//10
	6000000,		//11
	6000000,		//12
	8000000,		//13
	8000000			//14
};
#else
int	CheckPlayExpTable[15] = {
	0,		//0
	0,		//1
	0,		//2
	0,		//3
	0,		//4
	0,		//5
	0,		//6
	0,		//7
	0,		//8
	0,		//9
	0,		//10
	0,		//11
	0,		//12
	0,		//13
	0		//14
};
#endif

int rsRECORD_DBASE::RecvRecordDataFromClient( TRANS_RECORD_DATAS *lpTransRecord , rsPLAYINFO *lpPlayInfo )
{

	char* lpData;

	char szFile[256];
	char szBackupFile[256];
	FILE* fp;
	int		size;
	int		cnt;
	int		CopyVal;
	char* lpBuff;

	int	Level = lpPlayInfo->smCharInfo.Level;
	int CrackUser = lpPlayInfo->CrackWarning;
	char* lpRecordMemBuff = lpPlayInfo->lpRecordDataBuff;

	lpData = (char*)&TransRecordData;

	if (lpTransRecord->TransSize >= sizeof(TransRecordData))
		return -20;

	if (lpTransRecord->Count * TRANS_RECORD_LEN + lpTransRecord->TransSize >= sizeof(TransRecordData))
		return -20;

	memcpy(&lpData[lpTransRecord->Count * TRANS_RECORD_LEN], lpTransRecord->szData, lpTransRecord->TransSize);

	if (lpTransRecord->Count > 0)
	{
		if (TransLastPartCount != lpTransRecord->Count - 1)
			return -4;
	}
	else
	{

		if (lpTransRecord->Count != TransLastPartCount) return -4;
	}

	if (lpTransRecord->Count >= lpTransRecord->Total - 1 && (TransRecordData.code == smTRANSCODE_RECORDDATA || TransRecordData.code == 5004))
	{
		TransRecordData.GameSaveInfo.SaveTime = (int)tServerTime;

		if (CheckCharForm(&TransRecordData.smCharInfo) == FALSE)
		{

			TransRecordData.code = 0;
			return -1;
		}

		if (lpPlayInfo->AdminMode < 2 && lstrcmp(TransRecordData.smCharInfo.szName, lpPlayInfo->szName) != 0)
		{

			TransRecordData.code = 0;
			return -15;
		}

		if (TransRecordData.size <= 0)
		{
			return -4;
		}

		if (TransRecordData.smCharInfo.Level < Level)
		{

			TransRecordData.code = 0;
			return -2;
		}

		if (TransRecordData.DataSize >= 0x7FFFFFFF || TransRecordData.ItemCount >= 0x7FFFFFFF || ((int*)TransRecordData.Data)[0] >= 0x7FFFFFFF)
			return -20;

		if (CheckRecordDataItem(&TransRecordData) == FALSE)
		{



			fp = fopen("Data\\DataServer\\LastError.dat", "wb");
			if (fp)
			{
				cnt = fwrite(&TransRecordData, TransRecordData.size, 1, fp);
				fclose(fp);
			}

			TransRecordData.code = 0;
			return -5;
		}

#ifndef _SERVER_MODE_OLD			

		if (lpPlayInfo->smCharInfo.Level > 0 &&
			TransRecordData.smCharInfo.Level >= 6 &&
			abs(lpPlayInfo->smCharInfo.Level - TransRecordData.smCharInfo.Level) >= 2 &&
			lpPlayInfo->AdminMode < 2)
		{

			return -12;
		}

		INT64	exp, exp2;
		int		money;


		exp = GetExp64(&TransRecordData.smCharInfo);
		if (exp < 0) return -6;

		exp = exp - lpPlayInfo->spExp_Start;
		money = TransRecordData.smCharInfo.Money - lpPlayInfo->spMoney_Start;




		if (ReformCharStatePoint(&TransRecordData.smCharInfo, TransRecordData.GameSaveInfo.dwLevelQuestLog) == FALSE)
		{

			TransRecordData.code = 0;
			return -8;
		}

		exp2 = 50000;
		if (TransRecordData.smCharInfo.Level >= 20 && TransRecordData.smCharInfo.Level < 40) exp2 = 200000;
		else if (TransRecordData.smCharInfo.Level >= 40 && TransRecordData.smCharInfo.Level < 60) exp2 = 400000;
		else if (TransRecordData.smCharInfo.Level >= 60 && TransRecordData.smCharInfo.Level < 100) exp2 = 800000;
		else if (TransRecordData.smCharInfo.Level >= 100 && TransRecordData.smCharInfo.Level < CHAR_LEVEL_MAX) exp2 = 1500000;

		INT64 tExp, sExp;

		tExp = lpPlayInfo->dwGameServerExp[0] + lpPlayInfo->dwGameServerExp[1] + lpPlayInfo->dwGameServerExp[2] + lpPlayInfo->dwGameServerExp[3] +
			lpPlayInfo->dwGameServerExp[4] + lpPlayInfo->dwGameServerExp[5] + lpPlayInfo->dwGameServerExp[6] + lpPlayInfo->dwGameServerExp[7];

		exp = GetExp64(&TransRecordData.smCharInfo) - lpPlayInfo->spExp_Start;

		if (exp > tExp)
		{
			sExp = exp - tExp;

			exp2 = CheckPlayExpTable[TransRecordData.smCharInfo.Level / 10];

			if (rsServerConfig.Event_ExpUp)
				exp2 = (exp * rsServerConfig.Event_ExpUp) / 100;

#ifdef _LANGUAGE_ARGENTINA		
			if (rsServerConfig.Event_ExpUp_latin)
				exp2 = (exp * rsServerConfig.Event_ExpUp_latin) / 100;
#endif
			if (rsServerConfig.ExpFixMode > 0)
			{
				exp2 = (exp2 * rsServerConfig.ExpFixMode) / 100;
			}
			else
			{
				exp2 /= 2;
			}


			if (sExp > exp2 && sExp > (tExp / 2))
			{



				if ((lpPlayInfo->dwLastSaveTime + 10 * 1000) < dwPlayServTime)
					lpPlayInfo->RecordWarningCount++;

				lpPlayInfo->RecordWarningExp = TransRecordData.smCharInfo.Exp;

				if (rsServerConfig.ExpFixMode)
				{
					SetExp64(&TransRecordData.smCharInfo, lpPlayInfo->spExp_Start + tExp + (tExp / 10));

				}

			}
			else
			{
				lpPlayInfo->RecordWarningCount = 0;
				lpPlayInfo->RecordWarningExp = 0;
			}
		}
		else
		{
			lpPlayInfo->RecordWarningCount = 0;
			lpPlayInfo->RecordWarningExp = 0;
		}



		if (CheckSkillPoint(TransRecordData.smCharInfo.Level, &TransRecordData.GameSaveInfo.SkillInfo, 0, TransRecordData.GameSaveInfo.dwLevelQuestLog) == FALSE)
		{

			return	-10;
		}
		else
		{
			if (rsServerConfig.Disable_DecSkillPoint)
			{


				for (cnt = 0; cnt < SKILL_POINT_COLUM_MAX; cnt++)
				{
					if (TransRecordData.GameSaveInfo.SkillInfo.bSkillPoint[cnt] < lpPlayInfo->bSkillPoint[cnt])
					{

						return	-10;
					}
				}
			}
		}


		memcpy(lpPlayInfo->bSkillPoint, TransRecordData.GameSaveInfo.SkillInfo.bSkillPoint, SKILL_POINT_COLUM_MAX);

		if (TransRecordData.smCharInfo.Weight[0]<0 ||
			TransRecordData.smCharInfo.Weight[0]>(TransRecordData.smCharInfo.Weight[1] + 1500))
		{

			if (TransRecordData.smCharInfo.Weight[1] != 0)
			{

				TransRecordData.code = 0;
				return -11;
			}
		}


		if (lpPlayInfo->smCharInfo.JOB_CODE == 0)
			lpPlayInfo->smCharInfo.JOB_CODE = TransRecordData.smCharInfo.JOB_CODE;

		if (lpPlayInfo->AdminMode < 2 && lpPlayInfo->smCharInfo.JOB_CODE != TransRecordData.smCharInfo.JOB_CODE)
		{
			TransRecordData.code = 0;
			return -14;
		}


#endif


		TransRecordData.smCharInfo.sPresentItem[0] = lpPlayInfo->sLowLevel_PresentItem[0];
		TransRecordData.smCharInfo.sPresentItem[1] = lpPlayInfo->sLowLevel_PresentItem[1];


		if (TransRecordData.smCharInfo.Life[0] == 0)
		{

			TransRecordData.smCharInfo.Life[0] = TransRecordData.smCharInfo.Life[1] / 2;

			if (TransRecordData.smCharInfo.JOB_CODE < 5)
			{

				TransRecordData.GameSaveInfo.PlayStageNum = START_FIELD_NUM;
				TransRecordData.GameSaveInfo.PosX = 746752;
				TransRecordData.GameSaveInfo.PosZ = -4464384;
			}
			else
			{
				TransRecordData.GameSaveInfo.PlayStageNum = START_FIELD_MORYON;
				TransRecordData.GameSaveInfo.PosX = 505344;
				TransRecordData.GameSaveInfo.PosZ = 18948864;
			}
		}

		lstrcpy(TransRecordData.GameSaveInfo.szMasterID, lpPlayInfo->szID);



		size = TransRecordData.size;
		if (size < srRECORD_DEFAULT_SIZE) size = srRECORD_DEFAULT_SIZE;

		if (lpRecordMemBuff && size <= srRECORD_MEMORY_SIZE)
		{
			memcpy(lpRecordMemBuff, &TransRecordData, TransRecordData.size);
		}
		else
		{
			CopyVal = CopyFile(szFile, szBackupFile, FALSE);

			lpBuff = lpPlayInfo->lpRecordDataBuff;
			lpPlayInfo->lpRecordDataBuff = (char*)&TransRecordData;

			rsSaveThrowData(lpPlayInfo);
			rsRecordMemoryBuff_CheckInvenItem(lpPlayInfo);

			fp = fopen(szFile, "wb");
			if (fp)
			{
				cnt = fwrite(&TransRecordData, size, 1, fp);
				fclose(fp);
			}

			lpPlayInfo->lpRecordDataBuff = lpBuff;

			if (lpRecordMemBuff)
				((TRANS_RECORD_DATA*)lpRecordMemBuff)->size = 0;

		}

		TransRecordData.code = 0;
		return TRUE;
	}

	TransLastPartCount = lpTransRecord->Count;

	return FALSE;
}

extern void Utils_Log( DWORD type, char* msg, ... );

//메모리 버퍼를 파일로 저장
int rsRecordMemoryBuffToFile( rsPLAYINFO *lpPlayInfo , char *szName ,  char *lpRecordMemBuff )
{
	char szFile[256];
	char szBackupFile[256];
	FILE	*fp;
	int		size;

	if ( !szName || !szName[0] ) return FALSE;
	if ( lpPlayInfo->szServerID[0] ) return TRUE;


	if ( lpRecordMemBuff && ((TRANS_RECORD_DATA *)lpRecordMemBuff)->size>0 ) {

		GetUserDataFile( szName , szFile );
		GetUserDataBackupFile( szName , szBackupFile );

		CopyFile( szFile, szBackupFile , FALSE );

		size = ((TRANS_RECORD_DATA *)lpRecordMemBuff)->size;
		if ( size<srRECORD_DEFAULT_SIZE ) size=srRECORD_DEFAULT_SIZE;

		if ( lpPlayInfo ) {
			rsSaveServerPotion( lpPlayInfo , &((TRANS_RECORD_DATA *)lpRecordMemBuff)->GameSaveInfo );
			rsSaveServerForce( lpPlayInfo , &((TRANS_RECORD_DATA *)lpRecordMemBuff)->GameSaveInfo );
		}

		fp = fopen( szFile , "wb" );
		if ( fp ) {
			fwrite( lpRecordMemBuff , size , 1, fp );
			fclose( fp );
			return TRUE;
		}
	}

	return FALSE;
}
 
int rsSaveThrowData(rsPLAYINFO* lpPlayInfo)
{
	TRANS_RECORD_DATA* lpTransRecordData;
	sTHROW_ITEM_INFO* lpThrowItemList;
	int cnt;

	if (!lpPlayInfo->lpRecordDataBuff) return FALSE;


	lpThrowItemList = lpPlayInfo->ThrowItemInfo;
	lpTransRecordData = (TRANS_RECORD_DATA*)lpPlayInfo->lpRecordDataBuff;

	cnt = lpPlayInfo->ThrowItemCount;
	if (cnt > THROW_ITEM_INFO_MAX)  cnt = THROW_ITEM_INFO_MAX;

	memcpy(&lpTransRecordData->ThrowItemInfo, lpThrowItemList, sizeof(sTHROW_ITEM_INFO) * cnt);
	lpTransRecordData->ThrowItemCount = cnt;

	if (lpPlayInfo->UnsaveMoney >= 0 && lpTransRecordData->smCharInfo.Money > lpPlayInfo->UnsaveMoney)
		lpTransRecordData->GameSaveInfo.LastMoeny = lpPlayInfo->UnsaveMoney + 1;			//돈 기록

	lpPlayInfo->ThrowItemCount = 0;
	lpPlayInfo->UnsaveMoney = -1;

	return TRUE;
}


//저장할 메모리버퍼의 아이템이 올바른지 전부 확인
int rsRecordMemoryBuff_CheckInvenItem( rsPLAYINFO *lpPlayInfo , int Mode )
{

	BYTE	*lpRecItem;
	TRANS_RECORD_DATA	*lpTransRecordData;
	sRECORD_ITEM	sRecordItem;
	int cnt,cnt2,cnt3;
	INT64 money;
	smTRANS_COMMAND_EX	smTransCommand;
	int	size,BuffSize;
	int	flag;

	sTHROW_ITEM_INFO	ThrowItemInfo[THROW_ITEM_INFO_MAX];			//버려진 아이템 정보
	int				ThrowItemCount =0;											//버려진 아이템 카운터


	if ( !lpPlayInfo->lpRecordDataBuff )	return FALSE;

//#ifdef _LANGUAGE_KOREAN
	if ( lpPlayInfo->AdminMode>=3 ) return FALSE;
//#endif

	lpTransRecordData = (TRANS_RECORD_DATA *)lpPlayInfo->lpRecordDataBuff;

	lpRecItem = (BYTE *)&lpTransRecordData->Data;
	BuffSize = 0;


	for( cnt=0;cnt<lpTransRecordData->ItemCount;cnt++ ) {
		if ( cnt>=(INVENTORY_MAXITEM*2) ) break;

		//압축 데이타 해독 ( Z/NZ 방식 )
		DecodeCompress( (BYTE *)lpRecItem , (BYTE *)&sRecordItem , RECORD_ITEM_INFO_HEAD_SIZE );


		if ( sRecordItem.sItemInfo.CODE && 
			sRecordItem.sItemInfo.ItemHeader.Head && sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {

			flag = 0;

			if ( (sRecordItem.sItemInfo.CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ) {
				//아이템 [ 물약 ]
				if ( lpPlayInfo->TradePotionInfoCount>0 ) {
					//거래 물약 제거 (거래직후 게임이 저장되지 않은 경우 , 거래했던 물약과 같은 종류 모두 제거 )
					if ( rsGetTradePotionInfo( lpPlayInfo , sRecordItem.sItemInfo.CODE )==TRUE ) {
						flag = 0;
					}
				}
				else {
					flag=-1;
				}
			}
			else {
				//일반 아이템의 경우

				//인벤토리 검사
				for( cnt2=0;cnt2<INVEN_ITEM_INFO_MAX;cnt2++ ) {
					if ( lpPlayInfo->InvenItemInfo[cnt2].dwCode &&
						lpPlayInfo->InvenItemInfo[cnt2].dwCode==sRecordItem.sItemInfo.CODE &&
						lpPlayInfo->InvenItemInfo[cnt2].dwKey==sRecordItem.sItemInfo.ItemHeader.Head &&
						lpPlayInfo->InvenItemInfo[cnt2].dwSum==sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {
							lpPlayInfo->InvenItemInfo[cnt2].dwCode = 0;
							flag++;
						}
				}

				//창고 검사
				if ( lpPlayInfo->OpenWarehouseInfoFlag ) 
				{
					for( cnt2=0;cnt2<100;cnt2++ ) {
						if ( lpPlayInfo->WareHouseItemInfo[cnt2].dwCode &&
							lpPlayInfo->WareHouseItemInfo[cnt2].dwCode==sRecordItem.sItemInfo.CODE &&
							lpPlayInfo->WareHouseItemInfo[cnt2].dwKey==sRecordItem.sItemInfo.ItemHeader.Head &&
							lpPlayInfo->WareHouseItemInfo[cnt2].dwSum==sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {

								lpPlayInfo->WareHouseItemInfo[cnt2].dwCode = 0;
								flag++;
							}
					}
				}

				if (lpPlayInfo->OpenCaravanInfoFlag)
				{
					for (cnt2 = 0; cnt2 < 100; cnt2++) {
						if (lpPlayInfo->CaravanItemInfo[cnt2].dwCode &&
							lpPlayInfo->CaravanItemInfo[cnt2].dwCode == sRecordItem.sItemInfo.CODE &&
							lpPlayInfo->CaravanItemInfo[cnt2].dwKey == sRecordItem.sItemInfo.ItemHeader.Head &&
							lpPlayInfo->CaravanItemInfo[cnt2].dwSum == sRecordItem.sItemInfo.ItemHeader.dwChkSum) {

							lpPlayInfo->CaravanItemInfo[cnt2].dwCode = 0;
							flag++;
						}
					}
				}
			}

			if ( flag==0 ) {
				for( cnt2=0;cnt2<lpTransRecordData->ThrowItemCount;cnt2++ ) {
					if ( lpTransRecordData->ThrowItemInfo[cnt2].dwCode==sRecordItem.sItemInfo.CODE &&
						lpTransRecordData->ThrowItemInfo[cnt2].dwKey==sRecordItem.sItemInfo.ItemHeader.Head &&
						lpTransRecordData->ThrowItemInfo[cnt2].dwSum==sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {
							break;
						}
				}

				if ( ThrowItemCount<THROW_ITEM_INFO_MAX ) {

					for(cnt3=0;cnt3<ThrowItemCount;cnt3++) {
						if ( ThrowItemInfo[cnt3].dwCode==sRecordItem.sItemInfo.CODE &&
							ThrowItemInfo[cnt3].dwKey==sRecordItem.sItemInfo.ItemHeader.Head &&
							ThrowItemInfo[cnt3].dwSum==sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {
								break;
							}
					}
					if ( cnt3>=ThrowItemCount ) {
						ThrowItemInfo[ThrowItemCount].dwCode	= sRecordItem.sItemInfo.CODE;
						ThrowItemInfo[ThrowItemCount].dwKey		= sRecordItem.sItemInfo.ItemHeader.Head;
						ThrowItemInfo[ThrowItemCount].dwSum		= sRecordItem.sItemInfo.ItemHeader.dwChkSum;
						ThrowItemCount++;
					}

				}

				if ( cnt2>=lpTransRecordData->ThrowItemCount ) {
					//로그에 기록
					smTransCommand.WParam = 8000;
					smTransCommand.WxParam = 1;
					smTransCommand.LxParam = (int)"*RECORD ITEM";
					smTransCommand.LParam = sRecordItem.sItemInfo.CODE;
					smTransCommand.SParam = sRecordItem.sItemInfo.ItemHeader.Head;
					smTransCommand.EParam = sRecordItem.sItemInfo.ItemHeader.dwChkSum;
					RecordHackLogFile( lpPlayInfo , &smTransCommand );
				}
			}
			else {
				if ( flag>0 ) {
					//아이템이 존재 하지만 버려진 코드에 있는지 재확인( 제거 시킴 )
					for( cnt2=0;cnt2<lpTransRecordData->ThrowItemCount;cnt2++ ) {
						if ( lpTransRecordData->ThrowItemInfo[cnt2].dwCode==sRecordItem.sItemInfo.CODE &&
							lpTransRecordData->ThrowItemInfo[cnt2].dwKey==sRecordItem.sItemInfo.ItemHeader.Head &&
							lpTransRecordData->ThrowItemInfo[cnt2].dwSum==sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {

								if ( ThrowItemCount<THROW_ITEM_INFO_MAX ) {
									ThrowItemInfo[ThrowItemCount].dwCode	= sRecordItem.sItemInfo.CODE;
									ThrowItemInfo[ThrowItemCount].dwKey		= sRecordItem.sItemInfo.ItemHeader.Head;
									ThrowItemInfo[ThrowItemCount].dwSum		= sRecordItem.sItemInfo.ItemHeader.dwChkSum;
									ThrowItemCount++;
								}
								break;
							}
					}
				}
			}

			if ( flag>1 ) {
				//복사아이템 발견 처리

				//로그에 기록
				smTransCommand.WParam = 8000;
				smTransCommand.WxParam = flag;
				smTransCommand.LxParam = (int)"*RECORD COPIED ITEM";
				smTransCommand.LParam = sRecordItem.sItemInfo.CODE;
				smTransCommand.SParam = sRecordItem.sItemInfo.ItemHeader.Head;
				smTransCommand.EParam = sRecordItem.sItemInfo.ItemHeader.dwChkSum;
				RecordHackLogFile( lpPlayInfo , &smTransCommand );
			}
		}

		size = ((int *)lpRecItem)[0];
		BuffSize += size;
		lpRecItem += size;
		if ( BuffSize>=(sizeof(sRECORD_ITEM)*RECORD_ITEM_MAX) ) break;			//버퍼 크기 초과시
	}
	if ( ThrowItemCount>0 ) {
		memcpy( lpTransRecordData->ThrowItemInfo , ThrowItemInfo, sizeof(sTHROW_ITEM_INFO)*ThrowItemCount );
		lpTransRecordData->ThrowItemCount = ThrowItemCount;
	}

	//창고를 연적이 있으면 창고도 검사

	sWAREHOUSE WareHouseCheck;
	TRANS_WAREHOUSE	TransWareHouse;
	int	WareHouseFixFlag = 0;
	char szFileName[128];
	char szItemName[64];
	FILE *fp;


	sCARAVAN CaravanCheck;
	TRANS_CARAVAN	TransCaravan;
	int	CaravanFixFlag = 0;


	if ( lpPlayInfo->OpenWarehouseInfoFlag && (lpPlayInfo->dwDataError&rsDATA_ERROR_WAREHOUSE)==0 ) {
		GetWareHouseFile( lpPlayInfo->szID , szFileName );

		fp = fopen( szFileName , "rb" );
		if ( fp ) {
			fread( &TransWareHouse , sizeof(TRANS_WAREHOUSE), 1 , fp );
			fclose(fp);
		}
		else {
			lpPlayInfo->OpenWarehouseInfoFlag = 0;
		}
	}

	if (lpPlayInfo->OpenCaravanInfoFlag && (lpPlayInfo->dwDataError & rsDATA_ERROR_WAREHOUSE) == 0) 
	{
		GetCaravanFile(lpPlayInfo->szID, szFileName);

		fp = fopen(szFileName, "rb");
		if (fp) {
			fread(&TransCaravan, sizeof(TRANS_CARAVAN), 1, fp);
			fclose(fp);
		}
		else {
			lpPlayInfo->OpenCaravanInfoFlag = 0;
		}
	}



skip_caravan:

	if (lpPlayInfo->OpenCaravanInfoFlag) 
	{
		DecodeCompress((BYTE*)TransCaravan.Data, (BYTE*)&CaravanCheck, sizeof(sCARAVAN));

		//오류난 창고 확인
		DWORD	dwChkSum = 0;
		char* szComp = (char*)&CaravanCheck;

		for (cnt = 0; cnt < sizeof(sCARAVAN); cnt++) {
			dwChkSum += szComp[cnt] * (cnt + 1);
		}
		if (dwChkSum != TransCaravan.dwChkSum) {
			lpPlayInfo->OpenCaravanInfoFlag = 0;
			goto skip_caravan;
		}

		for (cnt = 0; cnt < 100; cnt++) {
			if (CaravanCheck.CaravanItem[cnt].Flag) {

				flag = 0;
				for (cnt2 = 0; cnt2 < INVEN_ITEM_INFO_MAX; cnt2++) {
					if (lpPlayInfo->InvenItemInfo[cnt2].dwCode &&
						lpPlayInfo->InvenItemInfo[cnt2].dwCode == CaravanCheck.CaravanItem[cnt].sItemInfo.CODE &&
						lpPlayInfo->InvenItemInfo[cnt2].dwKey == CaravanCheck.CaravanItem[cnt].sItemInfo.ItemHeader.Head &&
						lpPlayInfo->InvenItemInfo[cnt2].dwSum == CaravanCheck.CaravanItem[cnt].sItemInfo.ItemHeader.dwChkSum) {
						lpPlayInfo->InvenItemInfo[cnt2].dwCode = 0;
						flag++;
					}
				}

				for (cnt2 = 0; cnt2 < 100; cnt2++) {
					if (lpPlayInfo->CaravanItemInfo[cnt2].dwCode &&
						lpPlayInfo->CaravanItemInfo[cnt2].dwCode == CaravanCheck.CaravanItem[cnt].sItemInfo.CODE &&
						lpPlayInfo->CaravanItemInfo[cnt2].dwKey == CaravanCheck.CaravanItem[cnt].sItemInfo.ItemHeader.Head &&
						lpPlayInfo->CaravanItemInfo[cnt2].dwSum == CaravanCheck.CaravanItem[cnt].sItemInfo.ItemHeader.dwChkSum) {

						lpPlayInfo->CaravanItemInfo[cnt2].dwCode = 0;
						flag++;
					}
				}


				if (!flag) 
				{
					CaravanCheck.CaravanItem[cnt].Flag = 0;
					CaravanFixFlag++;

					memcpy(szItemName, CaravanCheck.CaravanItem[cnt].sItemInfo.ItemName, 32);
					szItemName[31] = 0;

					smTransCommand.WParam = 8000;
					smTransCommand.WxParam = 3;
					smTransCommand.LxParam = (int)szItemName;
					smTransCommand.LParam = CaravanCheck.CaravanItem[cnt].sItemInfo.CODE;
					smTransCommand.SParam = CaravanCheck.CaravanItem[cnt].sItemInfo.ItemHeader.Head;
					smTransCommand.EParam = CaravanCheck.CaravanItem[cnt].sItemInfo.ItemHeader.dwChkSum;

					RecordHackLogFile(lpPlayInfo, &smTransCommand);
				}

				if (flag > 1) 
				{
					smTransCommand.WParam = 8000;
					smTransCommand.WxParam = flag;
					smTransCommand.LxParam = (int)"*RECORD COPIED ITEM IN CARAVAN";
					smTransCommand.LParam = CaravanCheck.CaravanItem[cnt].sItemInfo.CODE;
					smTransCommand.SParam = CaravanCheck.CaravanItem[cnt].sItemInfo.ItemHeader.Head;
					smTransCommand.EParam = CaravanCheck.CaravanItem[cnt].sItemInfo.ItemHeader.dwChkSum;
					RecordHackLogFile(lpPlayInfo, &smTransCommand);
				}
			}
		}

		if (CaravanFixFlag && !Mode) 
		{
			if (SaveCaravan2(&CaravanCheck, &TransCaravan) == TRUE) {

				rsSaveCaravanData(lpPlayInfo->szID, &TransCaravan);
			}
		}
	}




skip_Warehouse:

	if ( lpPlayInfo->OpenWarehouseInfoFlag ) {

		//창고 압축 풀어서 아이템 검사하여 설정
		DecodeCompress( (BYTE *)TransWareHouse.Data , (BYTE *)&WareHouseCheck , sizeof(sWAREHOUSE) );

			//오류난 창고 확인
			DWORD	dwChkSum = 0;
			char	*szComp = (char *)&WareHouseCheck;

			for( cnt=0;cnt<sizeof(sWAREHOUSE);cnt++ ) {
				dwChkSum += szComp[cnt]*(cnt+1);
			}
			if ( dwChkSum!=TransWareHouse.dwChkSum ) {
				lpPlayInfo->OpenWarehouseInfoFlag = 0;
				goto skip_Warehouse;
			}



		for(cnt=0;cnt<100;cnt++) {
			if ( WareHouseCheck.WareHouseItem[cnt].Flag ) {

				flag = 0;
				for( cnt2=0;cnt2<INVEN_ITEM_INFO_MAX;cnt2++ ) {
					if ( lpPlayInfo->InvenItemInfo[cnt2].dwCode &&
						lpPlayInfo->InvenItemInfo[cnt2].dwCode==WareHouseCheck.WareHouseItem[cnt].sItemInfo.CODE &&
						lpPlayInfo->InvenItemInfo[cnt2].dwKey==WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.Head &&
						lpPlayInfo->InvenItemInfo[cnt2].dwSum==WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum ) {
							lpPlayInfo->InvenItemInfo[cnt2].dwCode = 0;
							flag++;
						}
				}

				for( cnt2=0;cnt2<100;cnt2++ ) {
					if ( lpPlayInfo->WareHouseItemInfo[cnt2].dwCode &&
						lpPlayInfo->WareHouseItemInfo[cnt2].dwCode==WareHouseCheck.WareHouseItem[cnt].sItemInfo.CODE &&
						lpPlayInfo->WareHouseItemInfo[cnt2].dwKey==WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.Head &&
						lpPlayInfo->WareHouseItemInfo[cnt2].dwSum==WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum ) {

						lpPlayInfo->WareHouseItemInfo[cnt2].dwCode = 0;
						flag++;
					}
				}


				if ( !flag ) {
					WareHouseCheck.WareHouseItem[cnt].Flag = 0;
					WareHouseFixFlag ++;


					//데이타 오류날 수 있기 때문에 버퍼에 이동후 로그 기록
					memcpy( szItemName , WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemName , 32 );
					szItemName[31] = 0;

					//로그에 기록
					smTransCommand.WParam = 8000;
					smTransCommand.WxParam = 3;
					smTransCommand.LxParam = (int)szItemName;
					smTransCommand.LParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.CODE;
					smTransCommand.SParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.Head;
					smTransCommand.EParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum;

					RecordHackLogFile( lpPlayInfo , &smTransCommand );
				}

				if ( flag>1 ) {
					//로그에 기록
					smTransCommand.WParam = 8000;
					smTransCommand.WxParam = flag;
					smTransCommand.LxParam = (int)"*RECORD COPIED ITEM IN WAREHOUSE";
					smTransCommand.LParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.CODE;
					smTransCommand.SParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.Head;
					smTransCommand.EParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum;
					RecordHackLogFile( lpPlayInfo , &smTransCommand );
				}
			}
		}

		if ( WareHouseCheck.Money ) {
			lpPlayInfo->WareHouseMoney = (WareHouseCheck.Money-2023);
			money = lpTransRecordData->smCharInfo.Money;
			money += lpPlayInfo->WareHouseMoney;

			if( lpTransRecordData->smCharInfo.Money<0 ) {
				//캐릭 주머니의 돈이 - 인경우 창고의 돈으로 보정
				lpTransRecordData->smCharInfo.Money = 0;
				money = lpPlayInfo->WareHouseMoney;
			}
		}
		else {
			lpPlayInfo->WareHouseMoney = 0;
			money = lpTransRecordData->smCharInfo.Money;
		}



		if ( money>lpPlayInfo->ServerMoney ) {
			//로그에 기록
			smTransCommand.WParam = 8010;
			smTransCommand.LParam = 2;
			smTransCommand.SParam = lpPlayInfo->ServerMoney;
			smTransCommand.EParam = (int)money;
			RecordHackLogFile( lpPlayInfo , &smTransCommand );

			cnt2 = (int)(money-lpPlayInfo->ServerMoney);
			lpTransRecordData->smCharInfo.Money -= cnt2;
			lpPlayInfo->UnsaveMoney = -1;
			lpTransRecordData->GameSaveInfo.LastMoeny = -1;

			if ( lpTransRecordData->smCharInfo.Money<0 ) {
				WareHouseCheck.Money += lpTransRecordData->smCharInfo.Money;
				if ( WareHouseCheck.Money<2023 ) WareHouseCheck.Money = 2023;
				WareHouseFixFlag ++;

				lpTransRecordData->smCharInfo.Money=0;
			}

		}

		if ( WareHouseFixFlag && !Mode ) {

			//창고 저장
			if ( SaveWareHouse( &WareHouseCheck , &TransWareHouse )==TRUE ) {
				//창고 데이타 저장
				rsSaveWareHouseData( lpPlayInfo->szID , &TransWareHouse );
			}
		}
	}
	else {
	//if ( WareHouseCheck.Money ) 
	//			lpPlayInfo->ServerMoney += WareHouseCheck.Money-2023;

		//돈 수치 검사하여 보정
		if ( lpTransRecordData->smCharInfo.Money>lpPlayInfo->ServerMoney ) {

			//로그에 기록
			smTransCommand.WParam = 8010;
			smTransCommand.LParam = 1;
			smTransCommand.SParam = lpPlayInfo->ServerMoney;
			smTransCommand.EParam = lpTransRecordData->smCharInfo.Money;
			RecordHackLogFile( lpPlayInfo , &smTransCommand );

			lpTransRecordData->smCharInfo.Money = lpPlayInfo->ServerMoney;
			lpPlayInfo->UnsaveMoney = -1;
			if ( lpTransRecordData->smCharInfo.Money<0 ) lpTransRecordData->smCharInfo.Money=0;
		}

		money = lpTransRecordData->GameSaveInfo.LastMoeny;

		if ( money &&  (money-1)>lpPlayInfo->ServerMoney ) {

			//로그에 기록
			smTransCommand.WParam = 8010;
			smTransCommand.LParam = 3;
			smTransCommand.SParam = lpPlayInfo->ServerMoney;
			smTransCommand.EParam = lpTransRecordData->GameSaveInfo.LastMoeny-1;
			RecordHackLogFile( lpPlayInfo , &smTransCommand );

			lpTransRecordData->GameSaveInfo.LastMoeny = lpPlayInfo->ServerMoney+1;
			if ( lpTransRecordData->GameSaveInfo.LastMoeny<0 ) lpTransRecordData->GameSaveInfo.LastMoeny=0;
		}

	}



	return TRUE;
}


//캐릭터 제거 기록 파일로 남김
int RecordDeleteCharacterError( char *szID , char *szName );

//해당 ID에 해당하는 캐릭터 데이타를 클라이언트로 전송함
int rsRECORD_DBASE::SendUserDataToClient( char *szID , smWINSOCK *lpsmSock , char *szServerID )
{
	char	szFile[256];
	char	szFile2[256];
	char	szFileInfo[256];
	WIN32_FIND_DATA		ffd;
	HANDLE				hFind;
	sPLAY_USER_DATA		sPlayUserData;
	TRANS_USERCHAR_INFO	TransUserCharInfo;
	_TRANS_CHAR_INFO	*lpCharInfo;
	FILE	*fp;
	int		cnt;
	int		FindCnt;
	int		DeleteCnt;
	int		LevelMax = 0;
	int		CharNameMax = CHAR_NAME_MAXLEN;

	//파일불러오기
	//wsprintf( szFileInfo , "userInfo\\%s.dat" , szID );

	if ( rsServerConfig.TT_DataServer_Count )
		CharNameMax = CHAR_NAME_MAXLEN+6;

	GetUserInfoFile( szID , szFileInfo );

	lstrcpy( LastAcessID , szID );

 	hFind = FindFirstFile( szFileInfo , &ffd );
	FindClose( hFind );

	if ( hFind==INVALID_HANDLE_VALUE ) {

		//CreateDirectory( szRecordUserInfoDir , NULL );			//디렉토리 생성

		//파일이 없을때 신규 생성
		ZeroMemory( &sPlayUserData , sizeof( sPLAY_USER_DATA ) );
		lstrcpy( sPlayUserData.szID , szID );
		lstrcpy( sPlayUserData.szHeader , "PS_TAILID 1.10" );

		fp = fopen( szFileInfo , "wb" );
		if ( !fp ) return FALSE;
		fwrite( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
		fclose(fp);
	}
	else {
		ZeroMemory( &sPlayUserData , sizeof( sPLAY_USER_DATA ) );

		fp = fopen( szFileInfo , "rb" );
		if ( !fp ) return FALSE;
		fread( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
		fclose(fp);

		if ( lstrcmpi( sPlayUserData.szID , szID )!=0 ) {
			//파일이 없을때 신규 생성
			ZeroMemory( &sPlayUserData , sizeof( sPLAY_USER_DATA ) );
			lstrcpy( sPlayUserData.szID , szID );
			lstrcpy( sPlayUserData.szHeader , "PS_TAILID 1.10" );

			fp = fopen( szFileInfo , "wb" );
			if ( !fp ) return FALSE;
			fwrite( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
			fclose(fp);
		}
	}

	TransUserCharInfo.code = smTRANSCODE_ID_SETUSERINFO;
	TransUserCharInfo.size = sizeof(TRANS_USERCHAR_INFO);
	lstrcpy( TransUserCharInfo.szID , szID );

	FindCnt = 0;
	DeleteCnt = 0;

	for( cnt=0;cnt<sPLAY_CHAR_MAX;cnt++) {
		if ( sPlayUserData.szCharName[cnt][0] ) {
			//캐릭터 파일에서 데이타 입수 

			//wsprintf( szFile , "userdata\\%s.dat" , sPlayUserData.szCharName[cnt] );
			GetUserDataFile( sPlayUserData.szCharName[cnt] , szFile );

			ZeroMemory( &TransRecordData , sizeof(TRANS_RECORD_DATA) );

			fp = fopen( szFile , "rb" );
			if ( fp ) {
				fread( &TransRecordData , sizeof(TRANS_RECORD_DATA) , 1 , fp );
				fclose(fp);

				if ( lstrcmpi( TransRecordData.smCharInfo.szName , sPlayUserData.szCharName[cnt] )==0 &&
					lstrlen(sPlayUserData.szCharName[cnt])<CharNameMax ) {	//CHAR_NAME_MAXLEN  ) {

					lpCharInfo = (_TRANS_CHAR_INFO *)&TransUserCharInfo.CharInfo[FindCnt];

					lstrcpy( lpCharInfo->szName , TransRecordData.smCharInfo.szName );
					lstrcpy( lpCharInfo->szModelName , TransRecordData.smCharInfo.szModelName );
					lstrcpy( lpCharInfo->szModelName2 , TransRecordData.smCharInfo.szModelName2 );

					lpCharInfo->Brood = TransRecordData.smCharInfo.Brood;
					lpCharInfo->dwArmorCode = 0;
					//lpCharInfo->JOB_CODE = TransRecordData.smCharInfo.JOB_CODE |(TransRecordData.smCharInfo.ChangeJob<<24);
					lpCharInfo->JOB_CODE = TransRecordData.smCharInfo.JOB_CODE;
					lpCharInfo->Level = TransRecordData.smCharInfo.Level;

					if ( rsServerConfig.FixedStartField )
						lpCharInfo->StartField =rsServerConfig.FixedStartField;				//시작필드 강제 지정
					else
						lpCharInfo->StartField =TransRecordData.GameSaveInfo.PlayStageNum;

					if ( lpCharInfo->StartField==rsSOD_FIELD ) lpCharInfo->StartField = rsSOD_VILLAGE;
					if ( lpCharInfo->StartField==QUEST_ARENA_FIELD ) {
						if ( lpCharInfo->JOB_CODE<=4 )
							lpCharInfo->StartField = START_FIELD_NUM;
						else
							lpCharInfo->StartField = START_FIELD_MORYON;
					}

					//if ( TransRecordData.smCharInfo.wPlayerKilling[0]>0 ) {
					//	//감옥에 갇혀있는 상태
					//	lpCharInfo->PosX = PrisonX;
					//	lpCharInfo->PosZ = PrisonZ;
					//}
					else {
						lpCharInfo->PosX = TransRecordData.GameSaveInfo.PosX;
						lpCharInfo->PosZ = TransRecordData.GameSaveInfo.PosZ;
					}

					if ( LevelMax<lpCharInfo->Level ) LevelMax=lpCharInfo->Level;

					FindCnt++;
				}
				else {
					if ( TransRecordData.size ) {
						//오류캐릭파일 백업
						GetDeleteDataFile( sPlayUserData.szCharName[cnt] , szFile2 );
						CopyFile( szFile , szFile2 , FALSE );

						RecordDeleteCharacterError( szID , szFile );
					}

					DeleteFile( szFile );

					//오류난 이름 삭제
					sPlayUserData.szCharName[cnt][0] = 0;
					DeleteCnt++;
				}
			}
			else {
				//오류난 이름 삭제
				sPlayUserData.szCharName[cnt][0] = 0;
				DeleteCnt++;
			}
		}
	}

	//클라이언트로 전송
	TransUserCharInfo.PlayUserCount = FindCnt;
	lpsmSock->Send( (char *)&TransUserCharInfo , TransUserCharInfo.size , TRUE );


	//오류난 데이타 복구시 다시 저정
	if ( DeleteCnt ) {
		fp = fopen( szFileInfo , "wb" );
		if ( fp ) {
			fwrite( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
			fclose(fp);
		}
	}


	return LevelMax;
}

// Salva novo char criado aqui
int rsRECORD_DBASE::InsertCharData( char *szID , char *szName , int Mode )
{
	char	szFile[256];
	sPLAY_USER_DATA		sPlayUserData;
	FILE	*fp;
	int		cnt;

	if ( !Mode && IsData( szName )==TRUE ) return FALSE;

	GetUserInfoFile( szID , szFile );

	fp = fopen( szFile , "rb" );
	if ( !fp ) return FALSE;

	fread( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
	fclose(fp);

	for( cnt=0;cnt<sPLAY_CHAR_MAX;cnt++) {
		if ( !sPlayUserData.szCharName[cnt][0] ) {
			lstrcpy( sPlayUserData.szCharName[cnt] , szName );
			fp = fopen( szFile , "wb" );
			if ( fp ) {
				fwrite( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
				fclose(fp);
			}

			if ( !Mode ) {
				GetUserDataFile( sPlayUserData.szCharName[cnt] , szFile );
				ZeroMemory( &TransRecordData , sizeof(TRANS_RECORD_DATA) );

				fp = fopen( szFile , "wb" );
				if ( fp ) {
					fwrite( &TransRecordData , srRECORD_MEMORY_SIZE , 1, fp );
					fclose( fp );
				}	

			}

			return TRUE;
		}
	}

	return FALSE;
}


//캐릭터 데이타 삭제
int rsRECORD_DBASE::DeleteCharData( char *szID , char *szName )
{
	char szFile[256];
	char szDelFile[256];
	char szDelBackupFile[256];
	FILE	*fp;
	int		cnt;
	sPLAY_USER_DATA		sPlayUserData;

//	wsprintf( szFile , "userdata\\%s.dat" , szName );



	GetUserDataFile( szName , szDelFile );

	fp = fopen( szDelFile , "rb" );
	if ( fp ) {
		fread( &TransRecordData , sizeof(TRANS_RECORD_DATA) , 1, fp );
		fclose( fp );
	}

	//파일불러오기
//	wsprintf( szFile , "userInfo\\%s.dat" , szID );
	GetUserInfoFile( szID , szFile );

	fp = fopen( szFile , "rb" );
	if ( !fp ) return FALSE;

	fread( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
	fclose(fp);

	for( cnt=0;cnt<sPLAY_CHAR_MAX;cnt++) {
		if ( sPlayUserData.szCharName[cnt][0] &&
			lstrcmpi( sPlayUserData.szCharName[cnt] , szName )==0 ) {
			
			sPlayUserData.szCharName[cnt][0] = 0;

			fp = fopen( szFile , "wb" );
			if ( fp ) {
				fwrite( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
				fclose(fp);
				if ( TransRecordData.smCharInfo.Level>=10 ) {
					//레벨 10이상의 캐릭터는 백업 받음
					GetDeleteDataFile( szName , szDelBackupFile );
					if ( TransRecordData.smCharInfo.Level>=20 )
						CopyFile( szDelFile , szDelBackupFile , TRUE );
					else
						CopyFile( szDelFile , szDelBackupFile , FALSE );
				}

				DeleteFile( szDelFile );
			}
			return TRUE;
		}
	}

	return TRUE;
}

#include	"checkname.h"		//지병훈 제작

//캐릭터가 존재 하는지 파일을 검사하여 확인
int rsRECORD_DBASE::IsData( char *szName )
{
	char	szFile[256];
	WIN32_FIND_DATA		ffd;
	HANDLE				hFind;

	//파일불러오기
	//wsprintf( szFile , "userdata\\%s.dat" , szName );

	//CreateDirectory( szRecordUserDataDir , NULL );			//디렉토리 생성
	//CreateDirectory( szRecordUserBackupDataDir , NULL );			//디렉토리 생성

	if ( lstrlen(szName)>=CHAR_NAME_MAXLEN  ) return TRUE;

	if(!c_CheckName(".\\CharacterNameList",szName)) return TRUE;    // <--- 요거 추가 함 됨니다.(지병훈)

	GetUserDataFile( szName , szFile );

 	hFind = FindFirstFile( szFile , &ffd );
	FindClose( hFind );
	if ( hFind!=INVALID_HANDLE_VALUE ) {
		return TRUE;
	}

	return FALSE;
}





//버려진 아이템 데이타 저장 
int rsRECORD_DBASE::SaveThrowData( char *szName , sTHROW_ITEM_INFO *lpThrowItemList , int Count , int SaveMoney )
{
	char szFile[256];
	FILE	*fp;

	if ( !szName[0] ) return FALSE;

	//wsprintf( szFile , "userdata\\%s.dat" , szName );
	GetUserDataFile( szName , szFile );

	fp = fopen( szFile , "rb" );
	if ( !fp ) return FALSE;

	fread( &TransRecordData , sizeof(TRANS_RECORD_DATA) , 1 , fp );
	fclose(fp);

	memcpy( &TransRecordData.ThrowItemInfo , lpThrowItemList , sizeof(sTHROW_ITEM_INFO)*Count );
	TransRecordData.ThrowItemCount = Count;

	//버려진 돈기록 ( 원래 저장된값보다 작은경우만 저장 - 복사방지 )
	if ( SaveMoney>=0 && TransRecordData.smCharInfo.Money>SaveMoney ) 
		TransRecordData.GameSaveInfo.LastMoeny = SaveMoney+1;			//돈 기록


	fp = fopen( szFile , "wb" );
	if ( !fp ) return FALSE;

	fwrite( &TransRecordData , sizeof(TRANS_RECORD_DATA) , 1 , fp );
	fclose(fp);

	return TRUE;
}



int rsSaveWareHouseData( char *szID , TRANS_WAREHOUSE *lpTransWareHouse )
{
	char szFileName[128];
	FILE	*fp;

	GetWareHouseFile( szID , szFileName );

	fp = fopen( szFileName , "wb" );
	if ( !fp ) return FALSE;
	fwrite( lpTransWareHouse , lpTransWareHouse->size , 1 , fp );
	fclose(fp);

	return TRUE;
}

int rsSaveCaravanData(char* szID, TRANS_CARAVAN* lpTransWareHouse)
{
	char szFileName[128];
	FILE* fp;

	GetCaravanFile(szID, szFileName);

	fp = fopen(szFileName, "wb");
	if (!fp) return FALSE;
	fwrite(lpTransWareHouse, lpTransWareHouse->size, 1, fp);
	fclose(fp);

	return TRUE;
}

int rsLoadWareHouseData( rsPLAYINFO *lpPlayInfo )
{
	char szFileName[128];
	TRANS_WAREHOUSE TransWareHouse;
	sWAREHOUSE	WareHouseCheck;
	FILE	*fp;
	smTRANS_COMMAND_EX	smTransCommand;
	int	CopiedItemFlag;

	//int cnt;

	WIN32_FIND_DATA		ffd;
	HANDLE				hFind;
	int	Money;
	int cnt,cnt2;


	GetWareHouseFile( lpPlayInfo->szID , szFileName );

 	hFind = FindFirstFile( szFileName , &ffd );
	FindClose( hFind );
	if ( hFind!=INVALID_HANDLE_VALUE ) {
		fp = fopen( szFileName , "rb" );
		if ( fp ) {
			fread( &TransWareHouse , sizeof(TRANS_WAREHOUSE), 1 , fp );
			fclose(fp);
		}
	}
	else {
		TransWareHouse.code = smTRANSCODE_WAREHOUSE;
		TransWareHouse.size = sizeof(TRANS_WAREHOUSE)-sizeof(sWAREHOUSE);
		TransWareHouse.DataSize = 0;
		TransWareHouse.dwChkSum = 0;
		TransWareHouse.wVersion[0] = Version_WareHouse;
		TransWareHouse.wVersion[1] = 0;
		TransWareHouse.WareHouseMoney = 0;
		TransWareHouse.UserMoney = 0;
		TransWareHouse.dwTemp[0] = 0;
		TransWareHouse.dwTemp[1] = 0;
		TransWareHouse.dwTemp[2] = 0;
		TransWareHouse.dwTemp[3] = 0;
		TransWareHouse.dwTemp[4] = 0;

	}

	if ( TransWareHouse.size>=smSOCKBUFF_SIZE ) TransWareHouse.size = smSOCKBUFF_SIZE;

	Money = TransWareHouse.WareHouseMoney;

	TransWareHouse.WareHouseMoney = 0;
	TransWareHouse.UserMoney = 0;
	CopiedItemFlag = 0;


	Server_DebugCount = 500;


	if ( !lpPlayInfo->OpenWarehouseInfoFlag ) {

		Server_DebugCount = 510;

		lpPlayInfo->OpenWarehouseInfoFlag = TRUE;
		ZeroMemory( lpPlayInfo->WareHouseItemInfo , sizeof(sTHROW_ITEM_INFO)*100 );

		if ( TransWareHouse.DataSize ) {

			DecodeCompress( (BYTE *)TransWareHouse.Data , (BYTE *)&WareHouseCheck , sizeof(sWAREHOUSE) );

			DWORD	dwChkSum = 0;
			char	*szComp = (char *)&WareHouseCheck;

			for( cnt=0;cnt<sizeof(sWAREHOUSE);cnt++ ) {
				dwChkSum += szComp[cnt]*(cnt+1);
			}
			if ( dwChkSum!=TransWareHouse.dwChkSum ) {
				lpPlayInfo->OpenWarehouseInfoFlag = FALSE;
				lpPlayInfo->dwDataError |= rsDATA_ERROR_WAREHOUSE;
				return 0;
			}


			for( cnt=0;cnt<100;cnt++ ) {
				if ( WareHouseCheck.WareHouseItem[cnt].Flag ) {

					for(cnt2=0;cnt2<INVEN_ITEM_INFO_MAX;cnt2++) {
		
						if ( lpPlayInfo->InvenItemInfo[cnt2].dwCode &&
							lpPlayInfo->InvenItemInfo[cnt2].dwCode==WareHouseCheck.WareHouseItem[cnt].sItemInfo.CODE &&
							lpPlayInfo->InvenItemInfo[cnt2].dwKey==WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.Head &&
							lpPlayInfo->InvenItemInfo[cnt2].dwSum==WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum ) {

	
								smTransCommand.WParam = 8070;
								smTransCommand.WxParam = 2;
								smTransCommand.LxParam = (int)"*WAREHOUSE";
								smTransCommand.LParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.CODE;
								smTransCommand.SParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.Head;
								smTransCommand.EParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum;
								RecordHackLogFile( lpPlayInfo , &smTransCommand );
								CopiedItemFlag++;
								break;

							}
					}

					if ( cnt2>=INVEN_ITEM_INFO_MAX ) {
						lpPlayInfo->WareHouseItemInfo[cnt].dwCode = WareHouseCheck.WareHouseItem[cnt].sItemInfo.CODE;
						lpPlayInfo->WareHouseItemInfo[cnt].dwKey = WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.Head;
						lpPlayInfo->WareHouseItemInfo[cnt].dwSum = WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum;
					}
				}
			}

			if ( WareHouseCheck.Money ) 
				lpPlayInfo->AddServerMoney( WareHouseCheck.Money-2023 , WHERE_OPEN_WAREHOUES );
		}
		Server_DebugCount = 520;
	}

	if ( !CopiedItemFlag ) {
		if ( lpPlayInfo->lpsmSock )
			lpPlayInfo->lpsmSock->Send( (char *)&TransWareHouse , TransWareHouse.size , TRUE );
	}
	else {
		smTransCommand.code = smTRANSCODE_CLOSECLIENT;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = 0;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;

		if ( lpPlayInfo->lpsmSock )
			lpPlayInfo->lpsmSock->Send( (char *)&smTransCommand , smTransCommand.size , TRUE );
	}

	Server_DebugCount = 0;

	return Money;
}






int rsLoadCaravanData(rsPLAYINFO* lpPlayInfo)
{
	char szFileName[128];
	TRANS_CARAVAN TransWareHouse;
	sCARAVAN	WareHouseCheck;
	FILE* fp;
	smTRANS_COMMAND_EX	smTransCommand;
	int	CopiedItemFlag;

	WIN32_FIND_DATA		ffd;
	HANDLE				hFind;
	int	Money;
	int cnt, cnt2;

	GetCaravanFile(lpPlayInfo->szName, szFileName);

	hFind = FindFirstFile(szFileName, &ffd);
	FindClose(hFind);
	if (hFind != INVALID_HANDLE_VALUE) {
		fp = fopen(szFileName, "rb");
		if (fp) {
			fread(&TransWareHouse, sizeof(TRANS_CARAVAN), 1, fp);
			fclose(fp);
		}
	}
	else {
		TransWareHouse.code = smTRANSCODE_CARAVAN;
		TransWareHouse.size = sizeof(TRANS_CARAVAN) - sizeof(sCARAVAN);
		TransWareHouse.DataSize = 0;
		TransWareHouse.dwChkSum = 0;
		TransWareHouse.wVersion[0] = Version_WareHouse;
		TransWareHouse.wVersion[1] = 0;
		TransWareHouse.WareHouseMoney = 0;
		TransWareHouse.UserMoney = 0;
		TransWareHouse.dwTemp[0] = 0;
		TransWareHouse.dwTemp[1] = 0;
		TransWareHouse.dwTemp[2] = 0;
		TransWareHouse.dwTemp[3] = 0;
		TransWareHouse.dwTemp[4] = 0;

	}

	if (TransWareHouse.size >= smSOCKBUFF_SIZE) TransWareHouse.size = smSOCKBUFF_SIZE;

	Money = TransWareHouse.WareHouseMoney;

	TransWareHouse.WareHouseMoney = 0;
	TransWareHouse.UserMoney = 0;
	CopiedItemFlag = 0;


	Server_DebugCount = 500;


	if (!lpPlayInfo->OpenCaravanInfoFlag) {

		Server_DebugCount = 510;

		lpPlayInfo->OpenCaravanInfoFlag = TRUE;
		ZeroMemory(lpPlayInfo->CaravanItemInfo, sizeof(sTHROW_ITEM_INFO) * 100);

		if (TransWareHouse.DataSize) {

			DecodeCompress((BYTE*)TransWareHouse.Data, (BYTE*)&WareHouseCheck, sizeof(sCARAVAN));

			DWORD	dwChkSum = 0;
			char* szComp = (char*)&WareHouseCheck;

			for (cnt = 0; cnt < sizeof(sCARAVAN); cnt++) {
				dwChkSum += szComp[cnt] * (cnt + 1);
			}
			if (dwChkSum != TransWareHouse.dwChkSum) {
				lpPlayInfo->OpenCaravanInfoFlag = FALSE;
				lpPlayInfo->dwDataError |= rsDATA_ERROR_WAREHOUSE;
				return 0;
			}


			for (cnt = 0; cnt < 100; cnt++) {
				if (WareHouseCheck.CaravanItem[cnt].Flag) {

					for (cnt2 = 0; cnt2 < INVEN_ITEM_INFO_MAX; cnt2++) {

						if (lpPlayInfo->InvenItemInfo[cnt2].dwCode &&
							lpPlayInfo->InvenItemInfo[cnt2].dwCode == WareHouseCheck.CaravanItem[cnt].sItemInfo.CODE &&
							lpPlayInfo->InvenItemInfo[cnt2].dwKey == WareHouseCheck.CaravanItem[cnt].sItemInfo.ItemHeader.Head &&
							lpPlayInfo->InvenItemInfo[cnt2].dwSum == WareHouseCheck.CaravanItem[cnt].sItemInfo.ItemHeader.dwChkSum) {


							smTransCommand.WParam = 8070;
							smTransCommand.WxParam = 2;
							smTransCommand.LxParam = (int)"*CARAVAN";
							smTransCommand.LParam = WareHouseCheck.CaravanItem[cnt].sItemInfo.CODE;
							smTransCommand.SParam = WareHouseCheck.CaravanItem[cnt].sItemInfo.ItemHeader.Head;
							smTransCommand.EParam = WareHouseCheck.CaravanItem[cnt].sItemInfo.ItemHeader.dwChkSum;
							RecordHackLogFile(lpPlayInfo, &smTransCommand);
							CopiedItemFlag++;
							break;

						}
					}

					if (cnt2 >= INVEN_ITEM_INFO_MAX) {
						lpPlayInfo->CaravanItemInfo[cnt].dwCode = WareHouseCheck.CaravanItem[cnt].sItemInfo.CODE;
						lpPlayInfo->CaravanItemInfo[cnt].dwKey = WareHouseCheck.CaravanItem[cnt].sItemInfo.ItemHeader.Head;
						lpPlayInfo->CaravanItemInfo[cnt].dwSum = WareHouseCheck.CaravanItem[cnt].sItemInfo.ItemHeader.dwChkSum;
					}
				}
			}

			if (WareHouseCheck.Money)
				lpPlayInfo->AddServerMoney(WareHouseCheck.Money - 2023, WHERE_OPEN_WAREHOUES);
		}
		Server_DebugCount = 520;
	}

	if (!CopiedItemFlag) {
		if (lpPlayInfo->lpsmSock)
			lpPlayInfo->lpsmSock->Send((char*)&TransWareHouse, TransWareHouse.size, TRUE);
	}
	else {
		smTransCommand.code = smTRANSCODE_CLOSECLIENT;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = 0;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;

		if (lpPlayInfo->lpsmSock)
			lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
	}

	Server_DebugCount = 0;

	return Money;
}


int rsLoadWareHouseData_Admin( rsPLAYINFO *lpPlayInfo , char *szID , int Day )
{
	char szFileName[128];
	TRANS_WAREHOUSE TransWareHouse;
	FILE	*fp;

	WIN32_FIND_DATA		ffd;
	HANDLE				hFind;


	if ( Day==0 )
		GetWareHouseFile( szID , szFileName );
	else
		GetWareHouseFile_Backup( szID , szFileName , Day );

 	hFind = FindFirstFile( szFileName , &ffd );
	FindClose( hFind );
	if ( hFind!=INVALID_HANDLE_VALUE ) {
		fp = fopen( szFileName , "rb" );
		if ( fp ) {
			fread( &TransWareHouse , sizeof(TRANS_WAREHOUSE), 1 , fp );
			fclose(fp);
		}
	}
	else {
		return FALSE;
	}

	if ( TransWareHouse.size>=smSOCKBUFF_SIZE ) TransWareHouse.size = smSOCKBUFF_SIZE;

	lpPlayInfo->OpenWarehouseInfoFlag = 0;

	if ( lpPlayInfo->lpsmSock )
		lpPlayInfo->lpsmSock->Send( (char *)&TransWareHouse , TransWareHouse.size , TRUE );

	return TRUE;
}

int rsCaravanData_Admin(rsPLAYINFO* lpPlayInfo, char* szID, int Day)
{
	char szFileName[128];
	TRANS_CARAVAN TransWareHouse;
	FILE* fp;

	WIN32_FIND_DATA		ffd;
	HANDLE				hFind;

	if (Day == 0)
		GetCaravanFile(szID, szFileName);
	else
		GetCaravanFile_Backup(szID, szFileName, Day);

	hFind = FindFirstFile(szFileName, &ffd);
	FindClose(hFind);
	if (hFind != INVALID_HANDLE_VALUE) {
		fp = fopen(szFileName, "rb");
		if (fp) {
			fread(&TransWareHouse, sizeof(TRANS_CARAVAN), 1, fp);
			fclose(fp);
		}
	}
	else {
		return FALSE;
	}

	if (TransWareHouse.size >= smSOCKBUFF_SIZE) TransWareHouse.size = smSOCKBUFF_SIZE;

	lpPlayInfo->OpenCaravanInfoFlag = 0;

	if (lpPlayInfo->lpsmSock)
		lpPlayInfo->lpsmSock->Send((char*)&TransWareHouse, TransWareHouse.size, TRUE);

	return TRUE;
}


//해당 ID에 해당 캐릭터이 존재하는지 확인
int rsCheckAccountChar( char *szID , char *szName )
{
	char	szFileInfo[128];
	sPLAY_USER_DATA		sPlayUserData;
	FILE	*fp;
	int		cnt;

	GetUserInfoFile( szID , szFileInfo );

	ZeroMemory( &sPlayUserData , sizeof( sPLAY_USER_DATA ) );

	fp = fopen( szFileInfo , "rb" );
	if ( !fp ) return FALSE;
	fread( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
	fclose(fp);

	if ( lstrcmpi( sPlayUserData.szID , szID )!=0 ) return FALSE;

	for( cnt=0;cnt<sPLAY_CHAR_MAX;cnt++) {
		if ( sPlayUserData.szCharName[cnt][0] ) {
			if ( lstrcmpi( sPlayUserData.szCharName[cnt] , szName )==0 ) {
				return TRUE;
			}
		}
	}

	return FALSE;
}

//포스오브 사용 설정
int rsLoadServerForce( rsPLAYINFO *lpPlayInfo , sGAME_SAVE_INFO *lpGameSaveInfo )
{
	int cnt;
	int cnt2; // 박재원 - 부스터 아이템(생명력, 기력, 근력)

	if ( lpGameSaveInfo->wForceOrbUsing[0] && lpGameSaveInfo->wForceOrbUsing[1] ) {
		// 박재원 - 빌링 매직 포스 추가
		cnt = lpGameSaveInfo->wForceOrbUsing[0]>>8;
		if(lpGameSaveInfo->wForceOrbUsing[0]>=sin26)
		{
			cnt -= 16;
		}
		cnt --;
		if(cnt>=0 && cnt<16 )  // 박재원 : 빌링 포스 추가로 포스 갯수 12로 연장 // 일반 포스
		{
			if (lpGameSaveInfo->wForceOrbUsing[1]<=ForceOrbUseTime[cnt]) {
				lpPlayInfo->dwForceOrb_SaveCode = sinFO1+lpGameSaveInfo->wForceOrbUsing[0];
				lpPlayInfo->dwForceOrb_SaveTime = dwPlayServTime+lpGameSaveInfo->wForceOrbUsing[1]*1000;
				lpPlayInfo->dwForceOrb_SaveDamage = ForceOrbDamage[cnt];
			}
		}
		else if( cnt>=20 && cnt < 32) // 매직 포스
		{
			if (lpGameSaveInfo->wForceOrbUsing[1]<=MagicForceOrbUseTime[cnt-20]) {
				lpPlayInfo->dwForceOrb_SaveCode = sinFO1+lpGameSaveInfo->wForceOrbUsing[0];
				lpPlayInfo->dwForceOrb_SaveTime = dwPlayServTime+lpGameSaveInfo->wForceOrbUsing[1]*1000;
				lpPlayInfo->dwForceOrb_SaveDamage = MagicForceOrbDamage[cnt-20];
			}
		}
		else if( cnt>=34 && cnt < 37) // 빌링 매직 포스
		{
			if (lpGameSaveInfo->wForceOrbUsing[1]<=BillingMagicForceOrbUseTime[cnt-34]) {
				lpPlayInfo->dwForceOrb_SaveCode = sinFO1+lpGameSaveInfo->wForceOrbUsing[0];
				lpPlayInfo->dwForceOrb_SaveTime = dwPlayServTime+lpGameSaveInfo->wForceOrbUsing[1]*1000;
				lpPlayInfo->dwForceOrb_SaveDamage = BillingMagicForceOrbDamage[cnt-34];
			}
		}	
	}

	// 박재원 - 부스터 아이템(생명력) 사용 설정
	if ( lpGameSaveInfo->wLifeBoosterUsing[0] && lpGameSaveInfo->wLifeBoosterUsing[1] ) 
	{
		cnt2 = lpGameSaveInfo->wLifeBoosterUsing[0]>>8; // sin21 -> 21 / sin22 -> 22 / sin23 -> 23
		cnt2 -= 21;
		if (lpGameSaveInfo->wLifeBoosterUsing[1]<=BoosterItem_UseTime[cnt2]/60) 
		{ 
			lpPlayInfo->dwLifeBooster_SaveCode = sinBC1+lpGameSaveInfo->wLifeBoosterUsing[0]; // 데미지 부스터 코드
			lpPlayInfo->dwLifeBooster_SaveTime = dwPlayServTime+lpGameSaveInfo->wLifeBoosterUsing[1]*60*1000; // 데미지 부스터 사용후 남은 시간 복구
			lpPlayInfo->dwLifeBooster_SaveData = BoosterItem_DataPercent[0]; // 부스터 아이템 가중퍼센트
		}
	}
	// 박재원 - 부스터 아이템(기력) 사용 설정
	if ( lpGameSaveInfo->wManaBoosterUsing[0] && lpGameSaveInfo->wManaBoosterUsing[1] ) 
	{
		cnt2 = lpGameSaveInfo->wManaBoosterUsing[0]>>8; // sin24 -> 24 / sin25 -> 25 / sin26 -> 26
		if(lpGameSaveInfo->wManaBoosterUsing[0]>=sin26) // sin26부터는  -16을 빼줘야 옳바른 정수가 나온다.
		{
			cnt2 -= 16;
		}
		cnt2 -= 24;
		if (lpGameSaveInfo->wManaBoosterUsing[1]<=BoosterItem_UseTime[cnt2]/60) 
		{ 
			lpPlayInfo->dwManaBooster_SaveCode = sinBC1+lpGameSaveInfo->wManaBoosterUsing[0]; // 데미지 부스터 코드
			lpPlayInfo->dwManaBooster_SaveTime = dwPlayServTime+lpGameSaveInfo->wManaBoosterUsing[1]*60*1000; // 데미지 부스터 사용후 남은 시간 복구
			lpPlayInfo->dwManaBooster_SaveData = BoosterItem_DataPercent[1]; // 부스터 아이템 가중퍼센트
		}
	}
	// 박재원 - 부스터 아이템(근력) 사용 설정
	if ( lpGameSaveInfo->wStaminaBoosterUsing[0] && lpGameSaveInfo->wStaminaBoosterUsing[1] ) 
	{
		cnt2 = lpGameSaveInfo->wStaminaBoosterUsing[0]>>8; // sin27 -> 27 / sin28 -> 28 / sin29 -> 29
		if(lpGameSaveInfo->wStaminaBoosterUsing[0]>=sin26) // sin26부터는  -16을 빼줘야 옳바른 정수가 나온다.
		{
			cnt2 -= 16;
		}
		cnt2 -= 27;
		if (lpGameSaveInfo->wStaminaBoosterUsing[1]<=BoosterItem_UseTime[cnt2]/60) 
		{ 
			lpPlayInfo->dwStaminaBooster_SaveCode = sinBC1+lpGameSaveInfo->wStaminaBoosterUsing[0]; // 데미지 부스터 코드
			lpPlayInfo->dwStaminaBooster_SaveTime = dwPlayServTime+lpGameSaveInfo->wStaminaBoosterUsing[1]*60*1000; // 데미지 부스터 사용후 남은 시간 복구
			lpPlayInfo->dwStaminaBooster_SaveData = BoosterItem_DataPercent[2]; // 부스터 아이템 가중퍼센트
		}
	}

	// 장별 - 스킬 딜레이
	if ( lpGameSaveInfo->wSkillDelayUsing[0] && lpGameSaveInfo->wSkillDelayUsing[1] ) 
	{
		cnt2 = lpGameSaveInfo->wSkillDelayUsing[0]>>8; // sin27 -> 27 / sin28 -> 28 / sin29 -> 29
		if(lpGameSaveInfo->wSkillDelayUsing[0]>=sin26) // sin26부터는  -16을 빼줘야 옳바른 정수가 나온다.
		{
			cnt2 -= 16;
		}
		cnt2 -= 27;
		if (lpGameSaveInfo->wSkillDelayUsing[1]<=nSkillDelay_UseTime[cnt2-3]/60) 
		{ 
			lpPlayInfo->dwSkillDelay_SaveCode = sinBC1+lpGameSaveInfo->wSkillDelayUsing[0]; 
			lpPlayInfo->dwSkillDelay_SaveTime = dwPlayServTime+lpGameSaveInfo->wSkillDelayUsing[1]*60*1000; 
		//	lpPlayInfo->dwSkillDelay_SaveData = BoosterItem_DataPercent[2]; 
		}
	}

	// Retomar Tempo dos Premium
	if (lpGameSaveInfo->dwTime_PrimeItem_X2 > 0)
		lpPlayInfo->dwTime_PrimeItem_X2 = lpGameSaveInfo->dwTime_PrimeItem_X2;
	else
		lpPlayInfo->dwTime_PrimeItem_X2 = 0;

	if (lpGameSaveInfo->dwTime_PrimeItem_ExpUp > 0)
		lpPlayInfo->dwTime_PrimeItem_ExpUp = lpGameSaveInfo->dwTime_PrimeItem_ExpUp;
	else
		lpPlayInfo->dwTime_PrimeItem_ExpUp = 0;

	if (lpPlayInfo->dwTime_PrimeItem_X2 || lpGameSaveInfo->dwTime_PrimeItem_ExpUp)
	{
		lpPlayInfo->dwPrimeItem_PackageCode = lpGameSaveInfo->dwPrimeItem_PackageCode;
	}

	if (lpGameSaveInfo->dwTime_PrimeItem_VampCuspid > 0)
		lpPlayInfo->dwTime_PrimeItem_VampCuspid = lpGameSaveInfo->dwTime_PrimeItem_VampCuspid;
	else
		lpPlayInfo->dwTime_PrimeItem_VampCuspid = 0;


	if (lpGameSaveInfo->dwTime_PrimeItem_VampCuspid_EX > 0)
		lpPlayInfo->dwTime_PrimeItem_VampCuspid_EX = lpGameSaveInfo->dwTime_PrimeItem_VampCuspid_EX;
	else
		lpPlayInfo->dwTime_PrimeItem_VampCuspid_EX = 0;

	if (lpGameSaveInfo->dwTime_PrimeItem_ManaRecharg > 0)
		lpPlayInfo->dwTime_PrimeItem_ManaRecharg = lpGameSaveInfo->dwTime_PrimeItem_ManaRecharg;
	else
		lpPlayInfo->dwTime_PrimeItem_ManaRecharg = 0;

	if (lpGameSaveInfo->dwTime_PrimeItem_ManaReduce > 0)
	{
		lpPlayInfo->dwTime_PrimeItem_ManaReduce = lpGameSaveInfo->dwTime_PrimeItem_ManaReduce;
	}
	else
	{
		lpPlayInfo->dwTime_PrimeItem_ManaReduce = 0;
	}

	if (lpGameSaveInfo->dwTime_PrimeItem_MightofAwell > 0)
	{
		lpPlayInfo->dwTime_PrimeItem_MightofAwell = lpGameSaveInfo->dwTime_PrimeItem_MightofAwell;
	}
	else
	{
		lpPlayInfo->dwTime_PrimeItem_MightofAwell = 0;
	}

	if (lpGameSaveInfo->dwTime_PrimeItem_MightofAwell2 > 0)
	{
		lpPlayInfo->dwTime_PrimeItem_MightofAwell2 = lpGameSaveInfo->dwTime_PrimeItem_MightofAwell2;
	}
	else
	{
		lpPlayInfo->dwTime_PrimeItem_MightofAwell2 = 0;
	}

	if (lpGameSaveInfo->dwTime_PrimeItem_StaminaReduce > 0)
	{
		lpPlayInfo->dwTime_PrimeItem_StaminaReduce = lpGameSaveInfo->dwTime_PrimeItem_StaminaReduce;
	}
	else
	{
		lpPlayInfo->dwTime_PrimeItem_StaminaReduce = 0;
	}

	if (lpGameSaveInfo->dwTime_PrimeItem_PhenixPet > 0)
	{
		lpPlayInfo->dwTime_PrimeItem_PhenixPet = lpGameSaveInfo->dwTime_PrimeItem_PhenixPet;
	}
	else
	{
		lpPlayInfo->dwTime_PrimeItem_PhenixPet = 0;
	}


	if (lpGameSaveInfo->dwTime_PrimeItem_HelpPet > 0)
	{
		lpPlayInfo->dwTime_PrimeItem_HelpPet = lpGameSaveInfo->dwTime_PrimeItem_HelpPet;

	}
	else
	{
		lpPlayInfo->smCharInfo.GravityScroolCheck[1] = 0;
		lpPlayInfo->dwTime_PrimeItem_HelpPet = 0;
	}

	if (lpGameSaveInfo->dwTime_Hat > 0)
	{
		lpPlayInfo->dwTime_Hat = lpGameSaveInfo->dwTime_Hat;
		lpPlayInfo->HatKind = lpGameSaveInfo->HatKind;
	}
	else
	{
		lpPlayInfo->dwTime_Hat = 0;
		lpPlayInfo->HatKind = 0;
	}

	if (lpGameSaveInfo->dwTime_PrimeItem_Mount > 0)
	{
		lpPlayInfo->dwTime_PrimeItem_Mount = lpGameSaveInfo->dwTime_PrimeItem_Mount;
		lpPlayInfo->dwPrimeItem_MountParam = lpGameSaveInfo->dwPrimeItem_MountParam;
	}
	else
	{
		lpPlayInfo->dwTime_PrimeItem_Mount = 0;
		lpPlayInfo->dwPrimeItem_MountParam = 0;
	}


	return TRUE;

}

//포스오브 사용 저장
int rsSaveServerForce( rsPLAYINFO *lpPlayInfo , sGAME_SAVE_INFO *lpGameSaveInfo )
{
	int sec;
	int LifeBooster_sec; // 박재원 - 부스터 아이템(생명력)
	int ManaBooster_sec; // 박재원 - 부스터 아이템(기력)
	int StaminaBooster_sec; // 박재원 - 부스터 아이템(근력)
	int nSkillDelay_sec; // 장별 - 스킬 딜레이

	sec = (lpPlayInfo->dwForceOrb_SaveTime-dwPlayServTime)/1000;

	LifeBooster_sec = (lpPlayInfo->dwLifeBooster_SaveTime - dwPlayServTime)/1000/60; // 박재원 - 부스터 아이템(생명력)사용 후 남은 시간 저장 
	ManaBooster_sec = (lpPlayInfo->dwManaBooster_SaveTime - dwPlayServTime)/1000/60; // 박재원 - 부스터 아이템(기력)사용 후 남은 시간 저장 
	StaminaBooster_sec = (lpPlayInfo->dwStaminaBooster_SaveTime - dwPlayServTime)/1000/60; // 박재원 - 부스터 아이템(근력)사용 후 남은 시간 저장 
	nSkillDelay_sec = (lpPlayInfo->dwSkillDelay_SaveTime - dwPlayServTime)/1000/60; // 장별 - 스킬 딜레이

	//포스 저장
	if ( lpPlayInfo->dwForceOrb_SaveTime && lpPlayInfo->dwForceOrb_SaveTime>dwPlayServTime ) {
		lpGameSaveInfo->wForceOrbUsing[0] = (WORD)(lpPlayInfo->dwForceOrb_SaveCode&sinITEM_MASK3);
		lpGameSaveInfo->wForceOrbUsing[1] = (WORD)sec;
	}
	else {
		lpGameSaveInfo->wForceOrbUsing[0] = 0;
		lpGameSaveInfo->wForceOrbUsing[1] = 0;
	}

	// 박재원 - 부스터 아이템(생명력) 사용 후 남은 시간 저장
	if ( lpPlayInfo->dwLifeBooster_SaveTime && lpPlayInfo->dwLifeBooster_SaveTime>dwPlayServTime ) 
	{
		lpGameSaveInfo->wLifeBoosterUsing[0] = (WORD)(lpPlayInfo->dwLifeBooster_SaveCode&sinITEM_MASK3);
		lpGameSaveInfo->wLifeBoosterUsing[1] = (WORD)LifeBooster_sec;
	}
	else {
		lpGameSaveInfo->wLifeBoosterUsing[0] = 0;
		lpGameSaveInfo->wLifeBoosterUsing[1] = 0;
	}
	// 박재원 - 부스터 아이템(기력) 사용 후 남은 시간 저장
	if ( lpPlayInfo->dwManaBooster_SaveTime && lpPlayInfo->dwManaBooster_SaveTime>dwPlayServTime ) 
	{
		lpGameSaveInfo->wManaBoosterUsing[0] = (WORD)(lpPlayInfo->dwManaBooster_SaveCode&sinITEM_MASK3);
		lpGameSaveInfo->wManaBoosterUsing[1] = (WORD)ManaBooster_sec;
	}
	else {
		lpGameSaveInfo->wManaBoosterUsing[0] = 0;
		lpGameSaveInfo->wManaBoosterUsing[1] = 0;
	}
	// 박재원 - 부스터 아이템(근력) 사용 후 남은 시간 저장
	if ( lpPlayInfo->dwStaminaBooster_SaveTime && lpPlayInfo->dwStaminaBooster_SaveTime>dwPlayServTime ) 
	{
		lpGameSaveInfo->wStaminaBoosterUsing[0] = (WORD)(lpPlayInfo->dwStaminaBooster_SaveCode&sinITEM_MASK3);
		lpGameSaveInfo->wStaminaBoosterUsing[1] = (WORD)StaminaBooster_sec;
	}
	else {
		lpGameSaveInfo->wStaminaBoosterUsing[0] = 0;
		lpGameSaveInfo->wStaminaBoosterUsing[1] = 0;
	}

	// 장별 - 스킬 딜레이
	if ( lpPlayInfo->dwSkillDelay_SaveTime && lpPlayInfo->dwSkillDelay_SaveTime>dwPlayServTime ) 
	{
		lpGameSaveInfo->wSkillDelayUsing[0] = (WORD)(lpPlayInfo->dwSkillDelay_SaveCode&sinITEM_MASK3);
		lpGameSaveInfo->wSkillDelayUsing[1] = (WORD)nSkillDelay_sec;
	}
	else {
		lpGameSaveInfo->wSkillDelayUsing[0] = 0;
		lpGameSaveInfo->wSkillDelayUsing[1] = 0;
	}

	NewShop::GetInstance()->SendCoinToGame(lpPlayInfo);

	if (lpPlayInfo->dwTime_PrimeItem_X2 > 0)
		lpGameSaveInfo->dwTime_PrimeItem_X2 = lpPlayInfo->dwTime_PrimeItem_X2;
	else
		lpGameSaveInfo->dwTime_PrimeItem_X2 = 0;

	if (lpPlayInfo->dwTime_PrimeItem_ExpUp > 0)
		lpGameSaveInfo->dwTime_PrimeItem_ExpUp = lpPlayInfo->dwTime_PrimeItem_ExpUp;
	else
		lpGameSaveInfo->dwTime_PrimeItem_ExpUp = 0;

	if (lpPlayInfo->dwTime_PrimeItem_X2 || lpGameSaveInfo->dwTime_PrimeItem_ExpUp)
	{
		lpGameSaveInfo->dwPrimeItem_PackageCode = lpPlayInfo->dwPrimeItem_PackageCode;
	}

	if (lpPlayInfo->dwTime_PrimeItem_VampCuspid > 0)
		lpGameSaveInfo->dwTime_PrimeItem_VampCuspid = lpPlayInfo->dwTime_PrimeItem_VampCuspid;
	else
		lpGameSaveInfo->dwTime_PrimeItem_VampCuspid = 0;


	if (lpPlayInfo->dwTime_PrimeItem_VampCuspid_EX > 0)
		lpGameSaveInfo->dwTime_PrimeItem_VampCuspid_EX = lpPlayInfo->dwTime_PrimeItem_VampCuspid_EX;
	else
		lpGameSaveInfo->dwTime_PrimeItem_VampCuspid_EX = 0;

	if (lpPlayInfo->dwTime_PrimeItem_ManaRecharg > 0)
		lpGameSaveInfo->dwTime_PrimeItem_ManaRecharg = lpPlayInfo->dwTime_PrimeItem_ManaRecharg;
	else
		lpGameSaveInfo->dwTime_PrimeItem_ManaRecharg = 0;

	if (lpPlayInfo->dwTime_PrimeItem_ManaReduce > 0)
	{
		lpGameSaveInfo->dwTime_PrimeItem_ManaReduce = lpPlayInfo->dwTime_PrimeItem_ManaReduce;
	}
	else
	{
		lpGameSaveInfo->dwTime_PrimeItem_ManaReduce = 0;
	}

	if (lpPlayInfo->dwTime_PrimeItem_MightofAwell > 0)
	{
		lpGameSaveInfo->dwTime_PrimeItem_MightofAwell = lpPlayInfo->dwTime_PrimeItem_MightofAwell;
	}
	else
	{
		lpGameSaveInfo->dwTime_PrimeItem_MightofAwell = 0;
	}

	if (lpPlayInfo->dwTime_PrimeItem_MightofAwell2 > 0)
	{
		lpGameSaveInfo->dwTime_PrimeItem_MightofAwell2 = lpPlayInfo->dwTime_PrimeItem_MightofAwell2;
	}
	else
	{
		lpGameSaveInfo->dwTime_PrimeItem_MightofAwell2 = 0;
	}

	if (lpPlayInfo->dwTime_PrimeItem_PhenixPet > 0)
	{
		lpGameSaveInfo->dwTime_PrimeItem_PhenixPet = lpPlayInfo->dwTime_PrimeItem_PhenixPet;
	}
	else
	{
		lpGameSaveInfo->dwTime_PrimeItem_PhenixPet = 0;
	}


	if (lpPlayInfo->dwTime_PrimeItem_HelpPet > 0)
	{
		lpGameSaveInfo->dwTime_PrimeItem_HelpPet = lpPlayInfo->dwTime_PrimeItem_HelpPet;
	}
	else
	{
		lpPlayInfo->smCharInfo.GravityScroolCheck[1] = 0;
		lpGameSaveInfo->dwTime_PrimeItem_HelpPet = 0;
	}


	if (lpPlayInfo->dwTime_PrimeItem_StaminaReduce > 0)
	{
		lpGameSaveInfo->dwTime_PrimeItem_StaminaReduce = lpPlayInfo->dwTime_PrimeItem_StaminaReduce;
	}
	else
	{
		lpGameSaveInfo->dwTime_PrimeItem_StaminaReduce = 0;
	}

	if (lpPlayInfo->dwTime_Hat > 0 && lpPlayInfo->HatKind)
	{
		lpGameSaveInfo->dwTime_Hat = lpPlayInfo->dwTime_Hat;
		lpGameSaveInfo->HatKind = lpPlayInfo->HatKind;
	}
	else
	{
		lpGameSaveInfo->dwTime_Hat = 0;
		lpGameSaveInfo->HatKind = 0;
	}

	if (lpPlayInfo->dwTime_PrimeItem_Mount > 0 && lpPlayInfo->dwPrimeItem_MountParam)
	{
		lpGameSaveInfo->dwTime_PrimeItem_Mount = lpPlayInfo->dwTime_PrimeItem_Mount;
		lpGameSaveInfo->dwPrimeItem_MountParam = lpPlayInfo->dwPrimeItem_MountParam;
	}
	else
	{
		lpGameSaveInfo->dwTime_PrimeItem_Mount = 0;
		lpGameSaveInfo->dwPrimeItem_MountParam = 0;
	}

	return TRUE;

}
//서버 포션 저장
int rsSaveServerPotion( rsPLAYINFO *lpPlayInfo , sGAME_SAVE_INFO *lpGameSaveInfo )
{
	int cnt1,cnt2;

	if ( rsServerConfig.PotionMonitor && lpPlayInfo->AdminOperMode==0 ) {
		lpGameSaveInfo->sPotionUpdate[0] = rsServerConfig.PotionMonitor;
		lpGameSaveInfo->sPotionUpdate[1] = 0;

		for(cnt1=0;cnt1<3;cnt1++)
			for(cnt2=0;cnt2<4;cnt2++) {
				lpGameSaveInfo->sPotionCount[cnt1][cnt2] = (short)lpPlayInfo->ServerPotion[cnt1][cnt2];
			}

	}
	else {
		lpGameSaveInfo->sPotionUpdate[0] = 0;
		lpGameSaveInfo->sPotionUpdate[1] = 0;
	}

	return TRUE;
}


//물약 보유량 비교
int rsCompareServerPotion( rsPLAYINFO *lpPlayInfo , sGAME_SAVE_INFO *lpGameSaveInfo )
{
	int cnt1,cnt2,pot;
	int OverPotion[3][4];
	int OverPotion2[3] = { 0,0,0 };
	int ErrFlag;
	smTRANS_COMMAND	smTransCommand;

	if ( rsServerConfig.PotionMonitor && 
		lpGameSaveInfo->sPotionUpdate[0] && lpGameSaveInfo->sPotionUpdate[0]==lpGameSaveInfo->sPotionUpdate[1] ) {

		ErrFlag = 0;

		for(cnt1=0;cnt1<3;cnt1++) {
			for(cnt2=0;cnt2<4;cnt2++) {
				if ( ((int)lpGameSaveInfo->sPotionCount[cnt1][cnt2])<lpPlayInfo->ServerPotion[cnt1][cnt2]) {
					pot = lpPlayInfo->ServerPotion[cnt1][cnt2]-lpGameSaveInfo->sPotionCount[cnt1][cnt2];
					OverPotion[cnt1][cnt2] = pot;
					OverPotion2[cnt1]+=pot;
					ErrFlag++;
				}
				else
					OverPotion[cnt1][cnt2] = 0;
			}
		}

		if ( ErrFlag ) {
			//오류 처리
			smTransCommand.WParam = 8800;
			smTransCommand.LParam = OverPotion2[0];
			smTransCommand.SParam = OverPotion2[1];
			smTransCommand.EParam = OverPotion2[2];
			RecordHackLogFile( lpPlayInfo , &smTransCommand );

			//물약갯수 다시 보정 ( 복사방지 )
			for(cnt1=0;cnt1<3;cnt1++) {
				for(cnt2=0;cnt2<4;cnt2++) {
					lpPlayInfo->ServerPotion[cnt1][cnt2] = lpGameSaveInfo->sPotionCount[cnt1][cnt2];
					if ( lpPlayInfo->ServerPotion[cnt1][cnt2]<0 ) lpPlayInfo->ServerPotion[cnt1][cnt2] = 0;
				}
			}
		}
	}

	return TRUE;
}


//다른 서버에서 파일을 불러와 저장
int ImportTTServerUser( char *szID , char *szServerID )
{
	char szRealID[32];
	char szTTServerPath[128];
	sPLAY_USER_DATA		sPlayUserData;
	char szFile[256];
	char szFile2[256];
	TRANS_RECORD_DATA	TransRecordData;
	char szName[64];
	int	size;
	int cnt;
	FILE	*fp;

	GetTT_ServerPath( szServerID , szTTServerPath, sizeof(szTTServerPath));
	GetRealID( szID , szRealID );


	GetUserInfoFile2( szID , szFile , szServerID );

	ZeroMemory( &sPlayUserData , sizeof( sPLAY_USER_DATA ) );
	fp = fopen( szFile , "rb" );
	if ( !fp ) return FALSE;
	fread( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
	fclose(fp);

	for( cnt=0;cnt<sPLAY_CHAR_MAX;cnt++) {
		if ( sPlayUserData.szCharName[cnt][0] ) {
			//캐릭터 파일에서 데이타 입수 

			GetUserDataFile2( sPlayUserData.szCharName[cnt] , szFile2 , szServerID );

			wsprintf( szName , "%s@%s" , sPlayUserData.szCharName[cnt] , szServerID );
			strcpy_s( sPlayUserData.szCharName[cnt] , szName );

			ZeroMemory( &TransRecordData , sizeof(TRANS_RECORD_DATA) );
			fp = fopen( szFile2 , "rb" );
			if ( fp ) {
				fread( &TransRecordData , sizeof(TRANS_RECORD_DATA) , 1 , fp );
				fclose(fp);
			}
			if ( TransRecordData.size>0 ) {
				GetUserDataFile( sPlayUserData.szCharName[cnt] , szFile2 );
				strcpy_s( TransRecordData.smCharInfo.szName , szName );
				strcpy_s( TransRecordData.GameSaveInfo.szMasterID , szID );

				size = TransRecordData.size;
				if ( size<16384 ) size = 16384;

				fp = fopen( szFile2 , "wb" );
				if ( fp ) {
					fwrite( &TransRecordData , size , 1 , fp );
					fclose(fp);
				}
			}
		}
	}

	GetUserInfoFile( szID , szFile );

	lstrcpy( sPlayUserData.szID , szID );

	fp = fopen( szFile , "wb" );
	if ( fp ) {
		fwrite( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1 , fp );
		fclose(fp);
	}

	return TRUE;
}


////////////////////////// 쓰레드를 통한 저장 ////////////////////////


//게임 데이타를 저장시키는 쓰레드
DWORD WINAPI RecDataThreadProc( void *pInfo )
{
	HANDLE	hThread;
	int cnt;
	int size;
	FILE *fp;
	sREC_DATABUFF	*recDataBuff;
	int	recDataBuffCount;

	recDataBuff	= new sREC_DATABUFF[REC_DATABUFF_MAX];


	hThread = GetCurrentThread();
	dwLastRecDataTime = GetCurrentTime();

	while(1) {
		SuspendThread(hThread);

		if ( sRecDataBuffCount==0 ) {
			dwLastRecDataTime = GetCurrentTime();
			continue;
		}

		//저장할 데이타가 있는 메모리 블럭을 복사해 온다
		//크리티칼 섹션 선언
		EnterCriticalSection( &cRecDataSection );
		recDataBuffCount = sRecDataBuffCount;
		memcpy( recDataBuff , sRecDataBuff , sizeof(sREC_DATABUFF)*sRecDataBuffCount );
		sRecDataBuffCount = 0;
		dwLastRecDataTime = GetCurrentTime();
		//크리티칼 섹션 해제
		LeaveCriticalSection( &cRecDataSection );

		//복사된 데이타를 하드디스크에 저장한다
		//크리티칼 섹션 선언
		EnterCriticalSection( &cSaveDataSection );
		for( cnt=0;cnt<recDataBuffCount;cnt++ ) {
			//저장할 길이 보정
			size = recDataBuff[cnt].TransRecData.size;
			if ( size<16000 ) size=16000;

			CopyFile( recDataBuff[cnt].szFileName , recDataBuff[cnt].szBackupFileName , FALSE );		//파일백업
			fp = fopen( recDataBuff[cnt].szFileName , "wb" );
			if ( fp ) {
				fwrite( &recDataBuff[cnt].TransRecData , size , 1, fp );
				fclose( fp );
			}
		}
		//크리티칼 섹션 해제
		LeaveCriticalSection( &cSaveDataSection );
	}

	ExitThread( TRUE );
	return TRUE;
}

//서버DB함수 초기화
int rsInitDataBase()
{
	//크리티칼 섹션 초기화
	InitializeCriticalSection( &cRecDataSection );
	//크리티칼 섹션 초기화
	InitializeCriticalSection( &cSaveDataSection );

	if ( !sRecDataBuff ) {
		sRecDataBuff = new sREC_DATABUFF[REC_DATABUFF_MAX];
		if ( !sRecDataBuff ) return FALSE;
	}

	if ( !hRecThread ) {
		hRecThread = CreateThread( NULL , 0, RecDataThreadProc , 0 , 0, &dwRecThreadId );
		if ( !hRecThread ) {
			delete sRecDataBuff;
			sRecDataBuff = 0;
			return FALSE;
		}
	}
	sRecDataBuffCount = 0;

	return TRUE;
}

//서버DB함수 말기화
int rsCloseDataBase()
{
	if ( sRecDataBuff ) {
		delete sRecDataBuff;
		sRecDataBuff = 0;
	}

	return TRUE;
}

//서버DB에 데이타 저장요구
int rsSaveRecData( TRANS_RECORD_DATA *lpTransRecordData , rsPLAYINFO *lpPlayInfo , 
				  char *szFileName , char *szBackupFileName )
{
	if ( sRecDataBuff && hRecThread ) {

		if ( sRecDataBuffCount>=REC_DATABUFF_MAX ) return FALSE;
		if ( sRecDataBuffCount>=REC_DATABUFF_LIMIT ) ResumeThread( hRecThread );

		EnterCriticalSection( &cRecDataSection );				//크리티칼 섹션 선언

		sRecDataBuff[sRecDataBuffCount].lpPlayInfo = lpPlayInfo;
		if ( lpPlayInfo ) {
			sRecDataBuff[sRecDataBuffCount].dwConnectCount  = lpPlayInfo->dwConnectCount;
		}
		else {
			sRecDataBuff[sRecDataBuffCount].dwConnectCount  = 0;
		}

		lstrcpy( sRecDataBuff[sRecDataBuffCount].szName , lpTransRecordData->smCharInfo.szName );

		lstrcpy( sRecDataBuff[sRecDataBuffCount].szFileName , szFileName );
		lstrcpy( sRecDataBuff[sRecDataBuffCount].szBackupFileName , szBackupFileName );
		memcpy( &sRecDataBuff[sRecDataBuffCount].TransRecData , lpTransRecordData , lpTransRecordData->size );
		sRecDataBuffCount++;

		LeaveCriticalSection( &cRecDataSection );				//크리티칼 섹션 해제
		return TRUE;
	}

	return FALSE;
}

//저장 대기중인 데이타 있는지 확인
int CheckRecWaitData( char *szName )
{


	return FALSE;
}

//시간 확인하여 저장시도
int rsTimeRecData()
{
	DWORD dwTime;

	dwTime = GetCurrentTime();

	if ( hRecThread && (dwTime-dwLastRecDataTime)>5000 ) {			//저장시도한지 5초 지남
		ResumeThread( hRecThread );						//저장 쓰레드 활성화
		return TRUE;
	}

	return FALSE;
}



int	rsLoadPostBox( rsPLAYINFO	*lpPlayInfo )
{
	rsPOST_BOX_ITEM		*lpPostBox;
	_POST_BOX_ITEM		*lpPostItem;

	char	szFileName[64];
	FILE* fp = nullptr;
	char	*p;
	char	 *pb;
	char	szLine[512];
	char	strBuff[512];
	int		cnt;

	if ( !lpPlayInfo->szID[0] ) return FALSE;
	if (lpPlayInfo->lpPostBoxItem) return TRUE;

	GetPostBoxFile( lpPlayInfo->szID , szFileName );

	fopen_s(&fp, szFileName, "rb");
	if ( !fp ) return FALSE;

	lpPlayInfo->lpPostBoxItem = new rsPOST_BOX_ITEM;

	if (!lpPlayInfo->lpPostBoxItem)
	{
		fclose(fp);
		return FALSE;
	}

	ZeroMemory(lpPlayInfo->lpPostBoxItem, sizeof(rsPOST_BOX_ITEM));

	lpPostBox = lpPlayInfo->lpPostBoxItem;

	while( !feof( fp ) )//  feof: file end까지 읽어라 
	{
		if( fgets( szLine, 500, fp ) == NULL)	break;
		if ( lpPostBox->ItemCounter>=POST_ITEM_MAX ) break;

		szLine[500] = 0;

		lpPostItem = &lpPostBox->PostItem[ lpPostBox->ItemCounter ];

		if ( szLine[0] ) {
			p = szLine;

			pb=p;p=GetWord(strBuff,p);if(strBuff[0]==34)p=GetString(strBuff,pb);	//받을 캐릭터 이름
			if ( strBuff[0] ) {
				strBuff[31] = 0;
				lstrcpy( lpPostItem->szCharName , strBuff );
			}

			pb=p;p=GetWord(strBuff,p);if(strBuff[0]==34)p=GetString(strBuff,pb);	//아이템 코드
			if ( strBuff[0] ) {
				strBuff[31] = 0;
				lstrcpy( lpPostItem->szItemCode , strBuff );

				if ( lstrcmpi( strBuff , "MONEY" )==0 ) {		//돈
					lpPostItem->dwItemCode = sinGG1|sin01;
				}
				if ( lstrcmpi( strBuff , "EXP" )==0 ) {			//경험치
					lpPostItem->dwItemCode = sinGG1|sin02;
				}

				if ( !lpPostItem->dwItemCode ) {
					for(cnt=0;cnt<MAX_ITEM;cnt++) {
						if ( lstrcmpi( sItem[cnt].LastCategory , strBuff )==0 ) {
							lpPostItem->dwItemCode = sItem[cnt].CODE;
							break;
						}
					}
				}
			}

			pb=p;p=GetWord(strBuff,p);if(strBuff[0]==34)p=GetString(strBuff,pb);	//특화 코드 ( 직업 코드 )
			if ( strBuff[0] ) {
				strBuff[31] = 0;
				strcpy_s( lpPostItem->szSpeJob , strBuff );
				lpPostItem->dwJobCode = atoi( strBuff );
			}

			pb=p;p=GetWord(strBuff,p);if(strBuff[0]==34)p=GetString(strBuff,pb);	//설명
			if ( strBuff[0] ) {
				strBuff[127] = 0;
				strcpy_s( lpPostItem->szDoc , strBuff );
			}

			pb=p;p=GetWord(strBuff,p);if(strBuff[0]==34)p=GetString(strBuff,pb);	//인증코드
			if ( strBuff[0] ) {
				strBuff[63] = 0;
				strcpy_s( lpPostItem->szFormCode , strBuff );
				lpPostItem->dwFormCode = atoi(strBuff);
			}

			pb=p;p=GetWord(strBuff,p);if(strBuff[0]==34)p=GetString(strBuff,pb);	//암호코드
			if ( strBuff[0] ) {
				strBuff[16] = 0;
				strcpy_s( lpPostItem->szPassCode , strBuff );
				lpPostItem->dwPassCode = GetSpeedSum( strBuff );
				lpPostItem->dwParam[0] = TRUE;
			}


			lpPostItem->Flag ++;
			lpPostBox->ItemCounter++;
		}
	}
	fclose(fp);

	DeleteFile(szFileName);

	return TRUE;
}

int	rsSavePostBox( rsPLAYINFO	*lpPlayInfo )
{
	char	szFileName[64];
	char	strBuff[512];
	int		cnt;

	HANDLE	hFile;
	DWORD	dwAcess;
	DWORD	FileLength;


	if ( !lpPlayInfo->szID[0] ) return FALSE;
	if ( !lpPlayInfo->lpPostBoxItem ) return FALSE;

	GetPostBoxFile( lpPlayInfo->szID , szFileName );

	hFile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	FileLength = GetFileSize(hFile, NULL);
	SetFilePointer(hFile, FileLength, NULL, FILE_BEGIN);

	for (cnt = 0; cnt < lpPlayInfo->lpPostBoxItem->ItemCounter; cnt++)
	{

		if (lpPlayInfo->lpPostBoxItem->PostItem[cnt].Flag)
		{

			wsprintf(strBuff, "%s		%s		%s		\"%s\"	%s	%s\r\n",
				lpPlayInfo->lpPostBoxItem->PostItem[cnt].szCharName,
				lpPlayInfo->lpPostBoxItem->PostItem[cnt].szItemCode,
				lpPlayInfo->lpPostBoxItem->PostItem[cnt].szSpeJob,
				lpPlayInfo->lpPostBoxItem->PostItem[cnt].szDoc,
				lpPlayInfo->lpPostBoxItem->PostItem[cnt].szFormCode,
				lpPlayInfo->lpPostBoxItem->PostItem[cnt].szPassCode);

			WriteFile(hFile, strBuff, lstrlen(strBuff), &dwAcess, NULL);
		}
	}

	CloseHandle(hFile);

	return TRUE;
}

// adciciona item postbox logador premiado xxstr
int	rsAddPostBox_OnlineReward(rsPLAYINFO *lpPlayInfo, char * ItemName, int iQuantity)
{
	char	szFileName[64];
	char	strBuff[512];


	HANDLE	hFile;
	DWORD	dwAcess;
	DWORD	FileLength;
	// verifica userid
	if (!lpPlayInfo->szID[0]) return FALSE;
	// pega o postbox do player
	GetPostBoxFile(lpPlayInfo->szID, szFileName);
	// criar arquivo postbox
	hFile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}

	FileLength = GetFileSize(hFile, NULL);
	SetFilePointer(hFile, FileLength, NULL, FILE_BEGIN);

	// escreve o tanto de item por quantidade informado no sql
	for (int  i = 0; i < iQuantity; i++)
	{
		// ID + Item + quant + MSG
		wsprintf(strBuff, "%s %s %d \"Logado Premiado!\"\r\n", lpPlayInfo->smCharInfo.szName, ItemName, 0);

		WriteFile(hFile, strBuff, lstrlen(strBuff), &dwAcess, NULL);
	}

	CloseHandle(hFile);

	return TRUE;
}

int	rsAddPostBox_EventoInvasao(char* id, char* name, char* ItemName, int iQuantity)
{
	char	szFileName[64];
	char	strBuff[512];

	HANDLE	hFile;
	DWORD	dwAcess;

	GetPostBoxFile(id, szFileName);
	hFile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}

	int FileLength = GetFileSize(hFile, NULL);
	SetFilePointer(hFile, FileLength, NULL, FILE_BEGIN);

	// escreve o tanto de item por quantidade informado no sql
	for (int i = 0; i < iQuantity; i++)
	{
		// ID + Item + quant + MSG
		wsprintf(strBuff, "%s %s %d \"Arena Royale\"\r\n", name, ItemName, 0);

		WriteFile(hFile, strBuff, lstrlen(strBuff), &dwAcess, NULL);
	}

	CloseHandle(hFile);

	return TRUE;
}

//아이템선물 우편 저장
int	rsAddPostBox_Present( rsPLAYINFO *lpPlayInfo )
{
	char	szFileName[64];
	int		len;

	HANDLE	hFile;
	DWORD	dwAcess;
	DWORD	FileLength;

	char	strBuff[16384];

	if ( !lpPlayInfo->szID[0] ) return FALSE;

	hFile = CreateFile( "Present.dat" , GENERIC_READ , FILE_SHARE_READ, NULL ,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL , NULL );

	if ( hFile ==INVALID_HANDLE_VALUE ) return FALSE;

	len = GetFileSize( hFile,NULL );
	if ( len>16384 ) return FALSE;

	ReadFile( hFile , strBuff, len , &dwAcess , NULL );
	CloseHandle( hFile );

	GetPostBoxFile( lpPlayInfo->szID , szFileName );

	hFile = CreateFile( szFileName , GENERIC_WRITE , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );
	if ( hFile==INVALID_HANDLE_VALUE ) {
		return FALSE;
	}

	FileLength = GetFileSize( hFile , NULL );
	SetFilePointer( hFile , FileLength , NULL , FILE_BEGIN );

	WriteFile( hFile , strBuff , len , &dwAcess , NULL );

	CloseHandle( hFile );

	return TRUE;
}


//별 포인트 이벤트 티켓 발생 설정
int	OpenStarPointEvent( rsPLAYINFO *lpPlayInfo , smCHAR_INFO *lpCharInfo )
{
	int cnt;

	if ( !lpCharInfo ) {
		if ( lpPlayInfo->CharLevelMax<rsServerConfig.Event_StarPointTicket ) {

			if ( !lpPlayInfo->CharLevelMax ) {
				lpPlayInfo->Event_StarTicketLevel = 3+(rand()%6);
			}
			else {
				cnt = (lpPlayInfo->CharLevelMax/10)+1;
				cnt = cnt*10;
				lpPlayInfo->Event_StarTicketLevel = cnt+ ( rand()%10 );
			}
		}

		return TRUE;
	}


	//별 상품권 발생시키기
	if ( lpPlayInfo->CharLevelMax<rsServerConfig.Event_StarPointTicket &&
		lpCharInfo->Level>=lpPlayInfo->CharLevelMax ) {		//최대 레벨이 확인하여 적용

		if ( lpCharInfo->sEventParam[0]==CHAR_EVENT_STARPOINT ) {

		}
		else {
			lpCharInfo->sEventParam[0] = CHAR_EVENT_STARPOINT;
			lpCharInfo->sEventParam[1] = 0;
		}

		cnt = (lpCharInfo->Level/10)+1;
		cnt = cnt*10;

		if ( lpCharInfo->Level>lpCharInfo->sEventParam[1] ) {
			lpCharInfo->sEventParam[1] = cnt+ ( rand()%10 );
		}

		lpPlayInfo->Event_StarTicketLevel = lpCharInfo->sEventParam[1];
	}

	return TRUE;
}


int	CloseStarPointEvent( rsPLAYINFO *lpPlayInfo , smCHAR_INFO *lpCharInfo )
{

	if ( lpPlayInfo->Event_StarTicketLevel ) {
		lpCharInfo->sEventParam[0] = CHAR_EVENT_STARPOINT;
		lpCharInfo->sEventParam[1] = lpPlayInfo->Event_StarTicketLevel;
	}

	return TRUE;

}

int	OpenStarPointTicket( rsPLAYINFO *lpPlayInfo )
{
	int cnt;
	int Price;

	if ( lpPlayInfo->Event_StarTicketLevel && 
		lpPlayInfo->Event_StarTicketLevel<rsServerConfig.Event_StarPointTicket &&
		lpPlayInfo->smCharInfo.Level>=lpPlayInfo->Event_StarTicketLevel ) {

		cnt = (lpPlayInfo->smCharInfo.Level/10)+1;
		cnt = cnt*10;

		lpPlayInfo->Event_StarTicketLevel = cnt+ ( rand()%10 );

		Price = 100000;

		switch( cnt ) {
			case 10:
				Price = 200000;
				break;
			case 20:
				Price = 300000;
				break;
			case 30:
				Price = 500000;
				break;
			case 40:
				Price = 1000000;
				break;
		}
		rsPutItem_PointTicket( lpPlayInfo , Price );

	}

	return TRUE;
}

int SaveCloseUserRecord( char *szUserName , int Level , int Exp , int Money  )
{
	HANDLE	hFile;
	DWORD	dwAcess;
	DWORD	FileLength;
	char	szFileName[128];
	char	szBuff[256];
	int		TotalMoney = 0;


	wsprintf( szFileName , "UserRecord.txt"  );

	wsprintf( szBuff , "%s		레벨(%d)	경험치(%d)	돈(%d)\r\n",szUserName , Level , Exp , Money );

	hFile = CreateFile( szFileName , GENERIC_WRITE , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );
	if ( hFile==INVALID_HANDLE_VALUE ) return FALSE;

	FileLength = GetFileSize( hFile , NULL );
	SetFilePointer( hFile , FileLength , NULL , FILE_BEGIN );
	WriteFile( hFile , szBuff , lstrlen(szBuff) , &dwAcess , NULL );
	CloseHandle( hFile );

	return TRUE;
}

int SaveWareHouseRecord( char *szUserName , int Money , int Weight1,int Weight2 )
{
	HANDLE	hFile;
	DWORD	dwAcess;
	DWORD	FileLength;
	char	szFileName[128];
	char	szBuff[256];
	int		TotalMoney = 0;


	wsprintf( szFileName , "UserRecord.txt"  );

	wsprintf( szBuff , "ID( %s )	창고돈(%d)	무게( %d/%d )\r\n",szUserName , Money  , Weight1 ,Weight2 );

	hFile = CreateFile( szFileName , GENERIC_WRITE , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );
	if ( hFile==INVALID_HANDLE_VALUE ) return FALSE;

	FileLength = GetFileSize( hFile , NULL );
	SetFilePointer( hFile , FileLength , NULL , FILE_BEGIN );
	WriteFile( hFile , szBuff , lstrlen(szBuff) , &dwAcess , NULL );
	CloseHandle( hFile );

	return TRUE;
}

int PeekMonMsg();
int	MonQuit = 0;
HWND hDialog;
HWND hWndMon;
LONG APIENTRY DialogProc( HWND hWnd,UINT messg,WPARAM wParam,LPARAM lParam );
LONG APIENTRY MonitorProc( HWND hWnd,UINT messg,WPARAM wParam,LPARAM lParam );

#define RECORD_MONEY_LIMIT		100000


int CheckUserData( char *szName )
{
	char *lpData;
	char szFile[256];
	FILE *fp;
	int	recFlag = 0;

	TRANS_RECORD_DATA TransRecordData;
	TRANS_RECORD_DATA *lpTransRecord;

	lpData = (char *)&TransRecordData;
	lpTransRecord = &TransRecordData;

	GetUserDataFile( szName , szFile );

	fp = fopen( szFile , "rb" );
	if ( fp ) {
		fread( lpTransRecord , sizeof(TRANS_RECORD_DATA) , 1, fp );
		fclose( fp );
	}
	else
		return FALSE;

	if ( lpTransRecord->smCharInfo.Level>=70 ||
		lpTransRecord->smCharInfo.Money>=RECORD_MONEY_LIMIT ) {
		SaveCloseUserRecord( szName , lpTransRecord->smCharInfo.Level , lpTransRecord->smCharInfo.Exp , lpTransRecord->smCharInfo.Money  );
	}
	return TRUE;
}

int CheckWareHouseData( char *szFile )
{
	TRANS_WAREHOUSE	TransWareHouse;
	sWAREHOUSE	WareHouse;
	FILE *fp;
	int cnt,len;
	char szName[256];

	fp = fopen( szFile , "rb" );
	if ( fp ) {
		fread( &TransWareHouse , sizeof(TRANS_WAREHOUSE) , 1, fp );
		fclose( fp );
	}
	else
		return FALSE;
	
	DecodeCompress( (BYTE *)&TransWareHouse.Data , (BYTE *)&WareHouse );

	if ( WareHouse.Weight[0]>WareHouse.Weight[1] || WareHouse.Money>=RECORD_MONEY_LIMIT ) {

		lstrcpy( szName , szFile );
		len = lstrlen( szName );

		for( cnt=0;cnt<len;cnt++ ) {
			if ( szName[cnt]=='.' ) {
				szName[cnt] = 0;
				break;
			}
		}
		for( cnt=cnt;cnt>=0;cnt-- ) {
			if ( szName[cnt]=='\\' )  {
				cnt++;
				break;
			}
		}
		SaveWareHouseRecord( szName+cnt , WareHouse.Money , WareHouse.Weight[0],WareHouse.Weight[1]);
	}

	return TRUE;
}

int CheckUserDataFull()
{
	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;
	char	szFindPath[64];
	char	szUserName[64];
	char	szMsgBuff[256];
	int		cnt,len;
	int		UserCnt;
	int		CharCount = 0;

	for( UserCnt=0;UserCnt<256;UserCnt++ ) {

		wsprintf( szFindPath , "Data\\DataServer\\UserData\\%d\\*.dat" ,UserCnt );

		wsprintf( szMsgBuff , "캐릭터 확인중 ( %d/255 ) - %d", UserCnt,CharCount );
		SetWindowText(GetDlgItem( hDialog , IDC_EDIT1 ) , szMsgBuff );


		hFindHandle = FindFirstFile( szFindPath , &fd );
		if ( hFindHandle!=INVALID_HANDLE_VALUE ) {
			while(1) {
				lstrcpy( szUserName , fd.cFileName );
				len = lstrlen( szUserName );

				for( cnt=0;cnt<len;cnt++ ) {
					if ( szUserName[cnt]=='.' ) {
						szUserName[cnt] = 0;
						break;
					}
				}

				CheckUserData( szUserName );
				CharCount++;

				PeekMonMsg();		//메세지 중간 처리

				//다음 파일 찾음
				if ( FindNextFile( hFindHandle , &fd )==FALSE ) break;
				if ( MonQuit ) break;
			}
		}
		FindClose( hFindHandle );
	}

	return TRUE;
}

int CheckWareHouseDataFull()
{
	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;
	char	szFindPath[64];
	char	szUserName[64];
	char	szUserPath[64];
	char	szMsgBuff[256];

	int		cnt,len;
	int		UserCnt;
	int		CharCount = 0;

	for( UserCnt=0;UserCnt<256;UserCnt++ ) {

		wsprintf( szFindPath , "Data\\DataServer\\WareHouse\\%d\\*.war",UserCnt );

		wsprintf( szMsgBuff , "아이템보관소 확인중 ( %d/255 ) - %d", UserCnt , CharCount );
		SetWindowText(GetDlgItem( hDialog , IDC_EDIT1 ) , szMsgBuff );

		hFindHandle = FindFirstFile( szFindPath , &fd );
		if ( hFindHandle!=INVALID_HANDLE_VALUE ) {
			while(1) {
				lstrcpy( szUserName , fd.cFileName );
				len = lstrlen( szUserName );

				for( cnt=0;cnt<len;cnt++ ) {
					if ( szUserName[cnt]=='.' ) {
						szUserName[cnt] = 0;
						break;
					}
				}

				wsprintf( szUserPath , "Data\\DataServer\\WareHouse\\%d\\%s" ,UserCnt, fd.cFileName );
				CheckWareHouseData( szUserPath );

				CharCount++;
				PeekMonMsg();		//메세지 중간 처리

				//다음 파일 찾음
				if ( FindNextFile( hFindHandle , &fd )==FALSE ) break;
				if ( MonQuit ) break;
			}
		}
		FindClose( hFindHandle );
	}

	return TRUE;
}

// 파일 수신창 창 만듬
HWND CreateDialogWnd();

//서버에 기록된 전체 데이타를 확인하여 의심가는 유저를 찾는다
int	CheckServerRecordData()
{
	DeleteFile( "UserRecord.txt" );
	CreateDialogWnd();
	CheckUserDataFull();
	if ( MonQuit ) return FALSE;
	CheckWareHouseDataFull();
	if ( MonQuit ) return FALSE;

	return TRUE;
}


// 파일 수신창 창 만듬
HWND CreateDialogWnd()
{


	return NULL;
}

//메신저 업그레이드 파일 받는창 프로시저
LONG APIENTRY DialogProc( HWND hWnd,UINT messg,WPARAM wParam,LPARAM lParam )
{

	//char szTemp[128];

    switch (messg)
    {
        case WM_INITDIALOG:
			break;

		case WM_CLOSE:
			DestroyWindow( hWnd );
			break;

		case WM_PAINT:
			break;

        case WM_COMMAND:
			switch( LOWORD(wParam) ) {
			case IDOK:
				SendMessage( hWnd , WM_CLOSE , 0, 0 );
				MonQuit = TRUE;	
				break;
			}

			break;

		default:
			return FALSE;
			break;
    }
	return FALSE;
}

LONG APIENTRY MonitorProc( HWND hWnd,UINT messg,WPARAM wParam,LPARAM lParam )
{

	//char szTemp[128];

    switch (messg)
    {
		case WM_CREATE:
			break;

		default: 
			return DefWindowProc(hWnd,messg,wParam,lParam);
			break;

    }
	return FALSE;
}

int PeekMonMsg()
{
	MSG msg;

	while (1) {
	    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	    {
	      TranslateMessage(&msg);
	      DispatchMessage(&msg);
	    }
		else
			break;
	}
	return TRUE;
}








///////////////////// 캐릭터 백업 데이타로 복구 //////////////////////


static char decode[512];
static char line[512];

static char *GetWord(char *q , char *p)
{

		while ( (*p == 32) || (*p == 9) ) // SPACE or TAB or ':'는 제외 시킴
		{
			p++;
		}
	
		while ( (*p != 32) && (*p != 9) ) // SPACE or TAB
		{
			if ( *p == '\n' || *p=='\r' ) 
				break;



			*q++ = *p++;
		}

		*q++ = 0;	// end of one string, put Null character

	return p;
}






/*
static int GetUserInfoFile( char *szID , char *szFileName )
{
	//wsprintf( szFileName , "%s\\%s.dat" , szRecordUserInfoDir , szID );

	if ( szID[4]==0 && szID[3]>='0' && szID[3]<='9' && (
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='m' || szID[2]=='M')) || 
		((szID[0]=='l' || szID[0]=='L') && (szID[1]=='p' || szID[1]=='P') && (szID[2]=='t' || szID[2]=='T')) ) 
		) {
 		wsprintf( szFileName , "Data\\DataServer\\＃%s\\%d\\%s.dat" , szRecordUserInfoDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	if ( szID[3]==0 && 
		((szID[0]=='p' || szID[0]=='P') && (szID[1]=='r' || szID[1]=='R') && (szID[2]=='n' || szID[2]=='N')) ||
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='n' || szID[2]=='N')) 
		) {
 		wsprintf( szFileName , "Data\\DataServer\\＃%s\\%d\\%s.dat" , szRecordUserInfoDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	wsprintf( szFileName , "Data\\DataServer\\%s\\%d\\%s.dat" , szRecordUserInfoDir , GetUserCode(szID) , szID );
	
	return TRUE;
}

static int GetUserDataFile( char *szName , char *szFileName )
{
	//wsprintf( szFileName , "%s\\%s.dat" , szRecordUserDataDir , szName );
	wsprintf( szFileName , "Data\\DataServer\\%s\\%d\\%s.dat" , szRecordUserDataDir , GetUserCode(szName) , szName );

	return TRUE;
}
*/


//캐릭터 정보 파일에서 해독하여 설정한다
int RestoreBackupData( char *szListFile , char *BackupPath )
{

	FILE	*fp , *fp2 , *fp3;
	char strBuff[256];
	char *p;
	WIN32_FIND_DATA		ffd;
	HANDLE				hFind;
	sPLAY_USER_DATA		sPlayUserData;
	int cnt;
	int flag;

	char szInfoFile[256];
	char szSrcDataFile[256];
	char szDestDataFile[256];
	char szLogBuff[256];
	char szFile[256];

	fp = fopen( szListFile , "rb" );
	if ( fp==NULL ) return FALSE;

	fp3 = fopen( "BackupData.log" , "wb" );
	if ( fp3==NULL ) {
		fclose(fp);
		return FALSE;
	}

	CreateDirectory( "BackupUser" , 0 );

	while( !feof( fp ) )//  feof: file end까지 읽어라 
	{
		if( fgets( line, 255, fp ) == NULL)	break;

		p = GetWord( decode , line);

		///////////////// 이름 ///////////////////////
		if ( decode[0] ) {
			p=GetWord(strBuff,p);

			if ( strBuff[0] ) {

				///////////////// 경로 설정 ///////////////////
				GetUserInfoFile( decode , szInfoFile );
				GetUserDataFile( strBuff , szDestDataFile );
				lstrcpy( szSrcDataFile , BackupPath );
				lstrcat( szSrcDataFile , szDestDataFile );
				///////////////////////////////////////////////

				flag = TRUE;

			 	hFind = FindFirstFile( szSrcDataFile , &ffd );
				FindClose( hFind );
				if ( hFind!=INVALID_HANDLE_VALUE ) {
					//소스 데이타 파일 존재
					fp2 = fopen( szInfoFile , "rb" );
					if ( fp2 ) {
						fread( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp2 );
						fclose(fp2);

						for( cnt=0;cnt<sPLAY_CHAR_MAX;cnt++) {
							if ( sPlayUserData.szCharName[cnt][0] ) {
								if ( lstrcmpi( sPlayUserData.szCharName[cnt] , strBuff )==0 ) {
									break;
								}
							}
						}

						if ( cnt>=sPLAY_CHAR_MAX ) {
							//계정안에 캐릭이 없다
						 	hFind = FindFirstFile( szDestDataFile , &ffd );
							FindClose( hFind );
							if ( hFind==INVALID_HANDLE_VALUE ) {
								//만들 캐릭이 존재 하지 않는다 . 계정에 캐릭 삽입
								for( cnt=0;cnt<sPLAY_CHAR_MAX;cnt++) {
									if ( !sPlayUserData.szCharName[cnt][0] ) {
										lstrcpy( sPlayUserData.szCharName[cnt] , strBuff );
										fp2 = fopen( szInfoFile , "wb" );
										if ( fp2 ) {
											fwrite( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp2 );
											fclose(fp2);
										}
										break;
									}
								}
							}
							else {
								flag = FALSE;
								wsprintf( szLogBuff , "%s (%s) -> 실패 ( 신규캐릭이존재 )\r\n", decode , strBuff );
								fwrite( szLogBuff , lstrlen( szLogBuff ) , 1, fp3 );
							}
						}

						if ( flag ) {
							//파일 복사

							wsprintf( szFile , "BackupUser\\%s.dat" ,strBuff );
							CopyFile( szDestDataFile , szFile , TRUE );

							CopyFile( szSrcDataFile , szDestDataFile , FALSE );

							wsprintf( szLogBuff , "%s (%s) -> 성공\r\n", decode , strBuff );
							fwrite( szLogBuff , lstrlen( szLogBuff ) , 1, fp3 );
						}
					}
					else {
						wsprintf( szLogBuff , "%s (%s) -> 실패 ( 계정이없음 )\r\n", decode , strBuff );
						fwrite( szLogBuff , lstrlen( szLogBuff ) , 1, fp3 );
					}
				}
				else {
					wsprintf( szLogBuff , "%s (%s) -> 실패 ( 소스파일없음 )\r\n", decode , strBuff );
					fwrite( szLogBuff , lstrlen( szLogBuff ) , 1, fp3 );
				}
			}
		}
	}


	fclose(fp3);
	fclose(fp);

	return TRUE;
}

#ifdef _W_SERVER

#ifdef _LANGUAGE_ENGLISH
#ifndef _LANGUAGE_PHILIPIN
#define	LOGIN_SERVER_KEY		0x512234a5
#define	LOGIN_SERVER_SHIFT		6
#else 
#define	LOGIN_SERVER_KEY		0x542634c3
#define	LOGIN_SERVER_SHIFT		4
#endif
#endif

//중국
#ifdef _LANGUAGE_CHINESE
#define	LOGIN_SERVER_KEY		0x512234a5
#define	LOGIN_SERVER_SHIFT		6
#endif

#ifdef _LANGUAGE_BRAZIL
#define	LOGIN_SERVER_KEY		0x532254a5
#define	LOGIN_SERVER_SHIFT		5
#endif

#ifdef _LANGUAGE_ARGENTINA		//아르헨티나 (브라질 세팅값 임시 적용)
#define	LOGIN_SERVER_KEY		0x532254a5
#define	LOGIN_SERVER_SHIFT		5
#endif

#ifdef _LANGUAGE_VEITNAM
#define	LOGIN_SERVER_KEY		0x442934c3
#define	LOGIN_SERVER_SHIFT		3
#endif

#ifdef _LANGUAGE_JAPANESE
#define	LOGIN_SERVER_KEY		0x542634c5
#define	LOGIN_SERVER_SHIFT		6
/*
#define	LOGIN_SERVER_KEY		0x542634c3
#define	LOGIN_SERVER_SHIFT		4
*/
#endif

#ifdef _LANGUAGE_THAI
#define	LOGIN_SERVER_KEY		0x533734c3
#define	LOGIN_SERVER_SHIFT		7
#endif


DWORD	rsGetLoginSafeKey( smCHAR_INFO *lpCharInfo , DWORD dwSpdSumCode )
{
	DWORD	dwCode = dwSpdSumCode;
	DWORD	dwCode2 = lpCharInfo->dwObjectSerial ^ LOGIN_SERVER_KEY;

	if ( !dwCode ) 
		dwCode = GetSpeedSum( lpCharInfo->szName );

	dwCode = dwCode * ((dwCode2>>LOGIN_SERVER_SHIFT)|(dwCode2<<LOGIN_SERVER_SHIFT));

	return	dwCode;
}

int rsSet_LoginServerSafeKey( smCHAR_INFO *lpCharInfo )
{

	lpCharInfo->dwLoginServerSafeKey = rsGetLoginSafeKey( lpCharInfo );

	return TRUE;
}

int	rsCheck_LoginServerSafeKey( smCHAR_INFO *lpCharInfo , DWORD dwSpdSumCode )
{
	if ( lpCharInfo->dwLoginServerSafeKey==rsGetLoginSafeKey( lpCharInfo , dwSpdSumCode ) ) 
		return TRUE;

	return FALSE;
}

#else
DWORD	rsGetLoginSafeKey( smCHAR_INFO *lpCharInfo , DWORD dwSpdSumCode )
{
	return TRUE;
}
int rsSet_LoginServerSafeKey( smCHAR_INFO *lpCharInfo )
{
	return TRUE;
}
int	rsCheck_LoginServerSafeKey( smCHAR_INFO *lpCharInfo , DWORD dwSpdSumCode )
{
	return TRUE;
}
#endif







