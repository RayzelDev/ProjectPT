#include "Utils\common.h"

#include "smlib3d\\smd3d.h"
#include "smwsock.h"
#include "character.h"
#include "fileread.h"
#include "playmain.h"
#include "netplay.h"
#include "field.h"
#include "sinbaram\\sinlinkheader.h"
#include "effectsnd.h"
#include "playsub.h"
#include "skillsub.h"
#include "language.h"
#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\cE_CViewClanInfo.h"

#include "damage.h"
#include "srcserver\\svr_Damage.h"
#include "srcserver\\onserver.h"
#include "Chat/ChatServer.h"

//추가 회피 데미지 출력 옵션
#define DISP_DAMAGE

#ifdef	DISP_DAMAGE
int rsDisplayDamgeToClient(rsPLAYINFO* lpPlayInfo, char* szDmgMsg);
#endif

#define		SUCCUBUS_CURSE_TIME		10

//PK시 40% 흡수력 적용
#define		PKMODE_ABSORB			30

extern BYTE VRKeyBuff[256];
extern DWORD dwPlayServTime;					//서버모드일때 시간

static int MatMBlur;							//메트리얼 번호

static smTEXLINK smTexLinkMotionBlur[128];		//텍스쳐 좌표
static smVERTEX MbVertex[64];
static smFACE MbFace[64];

static smTEXLINK smTexLinkMotionBlur2[64];		//텍스쳐 좌표
static smVERTEX MbVertex2[32];
static smFACE MbFace2[32];

#define	AGING_SUB_LEVEL	10

int	SkillChargingFlag = 0;			//스킬 차징 플랙
int	SkillEventDamageCount;			//스킬 데미지 카운터

TRANS_SKIL_ATTACKDATA	Trans_SplashCharList;		//팅겨서 맞는 공격 데이타 임시저장

//크리티컬 섹션
CRITICAL_SECTION	cLoadSection;

int DrawMotionBlur();

/////////////////////// 서버측 함수 불러다 쓴다 //////////////////////
//이벤트 지역인지 첵크
int rsCheckHardCoreEventArea(int x, int y, int z);
extern int	rsHardCoreEventInterLineZ;
extern int	rsHardCoreEventInterLineZ2;
extern int	rsHardCoreEventPosY;
extern int	rsHardCoreEventOutLineZ;
extern rsSERVER_CONFIG		rsServerConfig;

//특정위치에 보조몬스터 강제 출현 시킴
smCHAR* rsOpenSubMonster(smCHAR* lpMainChar, DWORD dwCharSoundCode, int x, int y, int z, int MaxFlag);
//해당 몬스터 출신 깃발 의 활성화 몬스터 수
int rsGetMonCountFlag(smCHAR* lpChar);


//상대 캐릭터에 기술 공격 가함 
int rsSendTransSkillAttack(smCHAR* lpChar, rsPLAYINFO* lpPlayInfo);
//서버 - 홀리 리플렉션 공격 처리
int rsProcessAttack_SkillHolyReflection(rsPLAYINFO* lpPlayInfo, smCHAR* lpChar);
//주변 몬스터를 찾는다
smCHAR* rsFindAroundMonster(smCHAR* lpChar, int RoundDist);
//주변 몬스터를 찾아 데미지 공격
smCHAR* rsSkillDamage_AroundMonster(smCHAR* lpChar);
//주변 몬스터를 찾아 스킬 ( 라투 주변 몬스터 스턴제거 )
smCHAR* rsSkillMonster_AroundMonster(smCHAR* lpChar, int RoundDist);

//////////////////////////////////////////////////////////////////////

int GetProjPosi(int& x3d, int& y3d, int& z3d, POINT3D* CameraPosi, POINT3D* CameraAngle);
//상대 캐릭터에 기술 공격 가함 
int rsSendTransSkillAttack(smCHAR* lpChar, rsPLAYINFO* lpPlayInfo);

extern int rsRegist_ItemSecCode(rsPLAYINFO* lpPlayInfo, TRANS_ITEMINFO* lpTransItemInfo, int NewItem);
extern int	rsRecord_ItemLog(rsPLAYINFO* lpPlayInfo, DWORD dwItemCode, DWORD	dwHead, DWORD dwChkSum, int Flag, char itemName[50]);

extern int rsPutItem2(rsPLAYINFO* lpPlayInfo, sITEMINFO* lpItem);

int	SkillShadowMasterEvent[9][7] = {
	{ 0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0 },

	{ 1,3,5,0,0,0,0 },		//3 번 때림
	{ 1,3,5,6,0,0,0 },
	{ 1,2,3,4,6,0,0 },
	{ 1,2,3,4,6,7,0 },
	{ 1,2,3,4,5,6,7 },		//7 번 때림

	{ 1,2,3,4,5,6,7 }
};

//공성전 필드 좌표 정보
int CastleBattleZone_LineZ = -24043 * fONE;
int	CastleBattleZone_DoorLineZ = -25545 * fONE;								//성문 입구
int	CastleBattleZone_DoorLineX[2] = { 34407 * fONE , 34695 * fONE };			//성문 입구
int	CastleBattleZone_BridgeZ = -25258 * fONE;									//다리 입구
int CastleSoulFountain_Area[4] = { 37040 * fONE , 37158 * fONE , -30286 * fONE, -30102 * fONE };	//영혼의샘터



//저항 속성 계산
int GetResistancePower(int power, int ResCode)
{
	int rs, result;

	rs = lpCurPlayer->smCharInfo.Resistance[ResCode];

	if (rs < 0) rs = 0;
	if (rs >= 100) rs = 100;
	rs = 100 - rs;

	result = (power * rs) / 100;

	if (result <= 0) result = 4;

	return result;
}


//######################################################################################
//작 성 자 : 오 영 석
int			Debug_TalkZoomMode;
int			Debug_TalkRepeatMode;
int			Debug_TalkRepeatModeType;

void DrawTalkZoom(smCHAR* lpChar, smPAT3D* lpPattern, float fx, float fy) {}
//######################################################################################


//도구 위치 포인트 구하기
int smCHAR::GetToolBipPoint(smCHARTOOL* ChrTool, int* nX, int* nY, int* nZ)
{

	smMATRIX* mWorld;
	POINT3D angle;
	int tx, ty, tz;
	smOBJ3D* lpObj;
	int pframe;
	int rx, ry, rz;

	if (!DisplayFlag) return FALSE;

	tx = 0;
	ty = 0;
	tz = 0;

	memcpy(&angle, &Angle, sizeof(POINT3D));
	angle.y = (-angle.y + ANGLE_180) & ANGCLIP;
	lpObj = ChrTool->ObjBip;

	if (lpObj) {
		mWorld = &lpObj->mWorld;
		pframe = frame;

		AnimObjectTree(lpObj, pframe, angle.x, angle.y, angle.z);

		rx = tx * mWorld->_11 +
			ty * mWorld->_21 +
			tz * mWorld->_31;

		ry = tx * mWorld->_12 +
			ty * mWorld->_22 +
			tz * mWorld->_32;

		rz = tx * mWorld->_13 +
			ty * mWorld->_23 +
			tz * mWorld->_33;

		*nX = pX + (rx >> FLOATNS) + mWorld->_41;
		*nZ = pZ + (ry >> FLOATNS) + mWorld->_42;
		*nY = pY + (rz >> FLOATNS) + mWorld->_43;

		return TRUE;
	}

	return FALSE;
}



//공격 포인트 좌표를 구한다
int smCHAR::GetAttackPoint(int* nX, int* nY, int* nZ)
{

	smMATRIX* mWorld;
	POINT3D angle;
	int tx, ty, tz;
	smCHARTOOL* ChrTool = 0;
	smOBJ3D* lpObj;
	int pframe;
	int rx, ry, rz;
	/*
		if ( HvLeftHand.PatTool )
			ChrTool = &HvLeftHand;

		if ( HvRightHand.PatTool )
	*/
	ChrTool = &HvRightHand;

	if (!DisplayFlag) return FALSE;

	tx = 0;
	ty = 0;

	if (ChrTool->PatTool)
		tz = ChrTool->SizeMax / 2;
	else
		tz = 0;

	memcpy(&angle, &Angle, sizeof(POINT3D));
	angle.y = (-angle.y + ANGLE_180) & ANGCLIP;

	if (AttackObjBip && !tz && !ShootingMode)
		lpObj = AttackObjBip;
	else
		lpObj = ChrTool->ObjBip;

	if (lpObj) {
		mWorld = &lpObj->mWorld;
		pframe = frame;

		AnimObjectTree(lpObj, pframe, angle.x, angle.y, angle.z);

		rx = tx * mWorld->_11 +
			ty * mWorld->_21 +
			tz * mWorld->_31;

		ry = tx * mWorld->_12 +
			ty * mWorld->_22 +
			tz * mWorld->_32;

		rz = tx * mWorld->_13 +
			ty * mWorld->_23 +
			tz * mWorld->_33;

		*nX = pX + (rx >> FLOATNS) + mWorld->_41;
		*nZ = pZ + (ry >> FLOATNS) + mWorld->_42;
		*nY = pY + (rz >> FLOATNS) + mWorld->_43;

		return TRUE;
	}

	return FALSE;
}



//아이템 번호 찾기 코드로
int GetSinItemNumFromCode(DWORD CODE)
{
	for (int j = 0; j < MAX_ITEM; j++) {
		if (sItem[j].CODE == CODE) {
			return j;
		}

	}
	return -1;
}

//2점의 거리를 구한다 ( 반환은 거리의 제곱값 )
int GetDistanceDbl(int x1, int z1, int x2, int z2)
{
	int ax, az;

	ax = (x1 - x2) >> FLOATNS;
	az = (z1 - z2) >> FLOATNS;
	ax *= ax;
	az *= az;
	return ax + az;
}

//위치를 검사하여 일정크기 박스크기로 확인
int CheckLocateBox(int x1, int y1, int z1, int x2, int y2, int z2, int size)
{
	if (abs(x1 - x2) < size && abs(y1 - y2) < size && abs(z1 - z2) < size)
		return TRUE;

	return FALSE;
}


//특정 오브젝트의 트리를 추적하여 애니메이션 시킴
int AnimObjectTree(smOBJ3D* tObj, int frame, int ax, int ay, int az)
{
	smOBJ3D* ObjList[32];
	smOBJ3D* obj;
	int ObjCnt;
	int cnt;

	ObjCnt = 0;
	obj = tObj;

	while (1) {
		ObjList[ObjCnt++] = obj;
		if (!obj->pParent) break;
		obj = obj->pParent;
	}

	for (cnt = ObjCnt - 1; cnt >= 0; cnt--) {
		ObjList[cnt]->TmAnimation(frame, ax, ay, az);
	}

	return TRUE;
}


char* szBipName_RightHand = "Bip weapon01";//"Bip01 R Hand";//
char* szBipName_LeftHand = "Bip01 L Hand";
char* szBipName_Shield = "Bip01 L Forearm";
char* szBipName_Shield2 = "Bip-shield";
char* szBipName_BackSpine = "Bip in01";
char* szBipName_BackSpineCross = "Bip in-bow";
char* szBipName_BackSpineBow = "Bip in-cro";
char* szBipName_RFoot = "Bip01 R Foot";
char* szBipName_LFoot = "Bip01 L Foot";
char* szBipName_Head = "Bip01 Head";


//char *szBipName_BackSpine = "Bip01 Neck";

int smCHAR_CameraX;
int smCHAR_CameraY;
int smCHAR_CameraZ;

int smCHAR_CameraAngX;
int smCHAR_CameraAngY;
int smCHAR_CameraAngZ;


// 캐릭터에 사용할 카메라 위치를 설정한다
int smCHAR_SetCameraPosi(int x, int y, int z, int angX, int angY, int angZ)
{
	smCHAR_CameraX = x;
	smCHAR_CameraY = y;
	smCHAR_CameraZ = z;

	smCHAR_CameraAngX = angX;
	smCHAR_CameraAngY = angY;
	smCHAR_CameraAngZ = angZ;

	return TRUE;
}

////////////////////////////////////////////////////////////////////


//고속 첵크용 코드 제작
DWORD GetSpeedSum(char* szName)
{
	DWORD cnt;
	DWORD Sum1, Sum2;
	BYTE ch;
	DWORD  dwSum;

	Sum2 = 0;
	dwSum = 0;

	cnt = 0;

	while (1) {
		ch = (BYTE)szName[cnt];
		if (ch == 0) break;
		if (ch >= 'a' && ch <= 'z') {//대문자 소문자로
			Sum2 += (ch - 0x20) * (cnt + 1);
			dwSum += (ch - 0x20) * (cnt * cnt);
		}
		else {
			Sum2 += (ch * (cnt + 1));
			dwSum += ch * (cnt * cnt);
		}
		cnt++;
	}

	Sum1 = cnt;

	return (dwSum << 24) | (Sum1 << 16) | Sum2;
}

//######################################################################################
// 작 성 자 : 오 영 석	
smPATTERN::smPATTERN(void)
{
	Init();
}


smPATTERN::~smPATTERN(void)
{
	Close();
}
//######################################################################################


//초기화
void smPATTERN::Init()
{
	int cnt;

	for (cnt = 0; cnt < DPAT_MAX; cnt++)
		ZeroMemory(&DinaPat[cnt], sizeof(smDPAT));

	BipPattern = 0;
}

//말기화
int smPATTERN::Close()
{
	int cnt;

	//smMODELINFO

	for (cnt = 0; cnt < DPAT_MAX; cnt++) {
		if (DinaPat[cnt].Pat) {
			delete DinaPat[cnt].Pat;
			DinaPat[cnt].Pat = 0;
			if (DinaPat[cnt].lpModelInfo) {
				delete DinaPat[cnt].lpModelInfo;
				DinaPat[cnt].lpModelInfo = 0;
			}
		}
	}

	return TRUE;
}


//이릉으로 패턴번호를 찾음
int smPATTERN::FindFromName(char* szName)
{
	int cnt;

	for (cnt = 0; cnt < DPAT_MAX; cnt++) {
		if (DinaPat[cnt].Pat) {
			if (lstrcmpi(szName, DinaPat[cnt].szPatName) == 0) {
				return cnt;
			}
		}
	}

	return -1;
}

//이릉으로 패턴번호를 찾음
int smPATTERN::FindFromCode(DWORD dwCode)
{
	int cnt;

	for (cnt = 0; cnt < DPAT_MAX; cnt++) {
		if (DinaPat[cnt].Pat && dwCode == DinaPat[cnt].dwSpeedFindSum)
			return cnt;
	}

	return -1;
}

//새로운 패턴 위치를 얻음
int smPATTERN::GetNew()
{
	int cnt;

	for (cnt = 0; cnt < DPAT_MAX; cnt++) {
		if (DinaPat[cnt].Pat == 0)
			return cnt;
	}

	return -1;
}

//뼉따귀 모션 데이타를 불러와 등록
smDPAT* smPATTERN::LoadBipPattern(char* szFileName)
{
	int Num;
	DWORD dwCode;


	//Num = FindFromName( szFileName );
	dwCode = GetSpeedSum(szFileName);
	Num = FindFromCode(dwCode);

	if (Num < 0) {
		//AddChatBuff( szFileName );

		Num = GetNew();
		if (Num >= 0) {
			DinaPat[Num].Pat = smASE_ReadBone(szFileName);		// 애니메이션 읽기
			if (DinaPat[Num].Pat) {
				DinaPat[Num].smDinaLink = 0;
				lstrcpy(DinaPat[Num].szPatName, szFileName);
				DinaPat[Num].dwSpeedFindSum = dwCode;
				DinaPat[Num].UseCount = 1;
				return &DinaPat[Num];
			}
		}
		else
			return NULL;
	}

	DinaPat[Num].UseCount++;

	return &DinaPat[Num];
}

//껍데기 데이타를 불러와 등록
smDPAT* smPATTERN::LoadCharactor(char* szFileName)
{
	int Num, sframe;
	DWORD cnt;
	smDPAT* dMotionPat;
	smPAT3D* AnimPat;
	DWORD	dwCode;

	smMODELINFO	modelInfo;


	//Num = FindFromName( szFileName );
	dwCode = GetSpeedSum(szFileName);
	Num = FindFromCode(dwCode);//GetSpeedSum(szFileName) );

	if (Num < 0) {

		if (smModelDecode(szFileName, &modelInfo) == NULL)
			return NULL;

		//AddChatBuff( szFileName );

		smASE_SetPhysique(0);

		Num = GetNew();

		if (Num < 0) return NULL;

		if (BipPattern && modelInfo.szMotionFile[0]) {
			dMotionPat = BipPattern->LoadBipPattern(modelInfo.szMotionFile);
			if (dMotionPat) {
				smASE_SetPhysique(dMotionPat->Pat);
				DinaPat[Num].smDinaLink = dMotionPat;
			}
			else return NULL;
		}
		else
			DinaPat[Num].smDinaLink = 0;

		//######################################################################################
		//작 성 자 : 오 영 석
		if (BipPattern && modelInfo.szTalkMotionFile[0])
		{
			smDPAT* lpTalkLink = BipPattern->LoadBipPattern(modelInfo.szTalkMotionFile);
			if (lpTalkLink)
				DinaPat[Num].lpTalkLink = lpTalkLink;
			else
				return NULL;
		}
		else
			DinaPat[Num].lpTalkLink = NULL;
		//######################################################################################

		DinaPat[Num].Pat = smASE_Read(modelInfo.szModelFile);// , modelInfo.szModelName );
		if (!DinaPat[Num].Pat) {
			if (smConfig.DebugMode) {
				//로딩 에러 메세지 출력
				char	szErrorMsg[128];
				wsprintf(szErrorMsg, "Loading Error , ( %s )", modelInfo.szModelFile);
				AddChatBuff(szErrorMsg);
			}
			return NULL;
		}

		if (DinaPat[Num].smDinaLink)
			AnimPat = DinaPat[Num].smDinaLink->Pat;
		else
			AnimPat = DinaPat[Num].Pat;

		//에니메이션 프레임 보정
		if (AnimPat && AnimPat->TmFrameCnt > 0) {
			for (cnt = 0; cnt < modelInfo.MotionCount; cnt++) {
				sframe = AnimPat->TmFrame[modelInfo.MotionInfo[cnt].MotionFrame - 1].StartFrame / 160;
				modelInfo.MotionInfo[cnt].StartFrame += sframe;
				modelInfo.MotionInfo[cnt].EndFrame += sframe;
			}
		}

		DinaPat[Num].lpModelInfo = new smMODELINFO;
		memcpy(DinaPat[Num].lpModelInfo, &modelInfo, sizeof(smMODELINFO));
		lstrcpy(DinaPat[Num].szPatName, szFileName);
		DinaPat[Num].dwSpeedFindSum = dwCode;
		DinaPat[Num].UseCount = 1;
		return &DinaPat[Num];
	}

	DinaPat[Num].UseCount++;
	return &DinaPat[Num];
}


extern smPAT3D* PatKnife;
char* szDefaultCharName = "char\\woman.ini";

//모델 로더 초기화
int InitLoader();
//모델 로딩을 위한 쓰레드 생성
int CreateLoaderThread();
DWORD WINAPI smLoaderThreadProc(void* pInfo);

struct smDATALOADER {
	smSTAGE3D* lpStage;
	smCHAR* lpChar;
	scITEM* lpItem;


	char		szLoadName[64];
	char		szLoadName2[64];

	int			Result;
	int			Cancel;				//취소 요구
};

#define DATALOADER_MAX		64
#define DATALOADER_MASK		63

smDATALOADER	smLoader[DATALOADER_MAX];
int	smLoaderCnt;
int smLoaderEnd;
int	ThreadLoaderStats;				//쓰레드 상태

smPATTERN smBipPattern;
smPATTERN smPattern;

//배경을 새 쓰레드에 통하여 로드 한다
int AddLoaderStage(smSTAGE3D* lpStage, char* szName)
{
	int cnt;

	cnt = smLoaderEnd & DATALOADER_MASK;

	smLoaderEnd++;

	smLoader[cnt].lpStage = lpStage;
	smLoader[cnt].lpChar = 0;
	smLoader[cnt].lpItem = 0;
	smLoader[cnt].Cancel = 0;
	smLoader[cnt].Result = 0;
	lstrcpy(smLoader[cnt].szLoadName, szName);

	//모델 로딩을 위한 쓰레드 생성
	CreateLoaderThread();

	return smLoaderEnd;
}

//아이템을 새 쓰레드에 통하여 로드 한다
int	AddLoaderPattern(smCHAR* lpChar, char* szName, char* szName2)
{
	int cnt;

	cnt = smLoaderEnd & DATALOADER_MASK;

	smLoaderEnd++;

	smLoader[cnt].lpStage = 0;
	smLoader[cnt].lpChar = lpChar;
	smLoader[cnt].lpItem = 0;
	smLoader[cnt].Cancel = 0;
	smLoader[cnt].Result = 0;
	lstrcpy(smLoader[cnt].szLoadName, szName);

	if (szName2)
		lstrcpy(smLoader[cnt].szLoadName2, szName2);
	else
		smLoader[cnt].szLoadName2[0] = 0;

	smLoader[cnt].lpChar->PatLoading = TRUE;

	//모델 로딩을 위한 쓰레드 생성
	CreateLoaderThread();

	return TRUE;
}




//캐릭터를 새 쓰레드에 통하여 로드 한다
int	AddLoaderItem(scITEM* lpItem, char* szName)
{
	int cnt;

	cnt = smLoaderEnd & DATALOADER_MASK;

	smLoaderEnd++;

	smLoader[cnt].lpStage = 0;
	smLoader[cnt].lpChar = 0;
	smLoader[cnt].lpItem = lpItem;
	smLoader[cnt].Cancel = 0;
	smLoader[cnt].Result = 0;
	lstrcpy(smLoader[cnt].szLoadName, szName);

	smLoader[cnt].lpItem->PatLoading = TRUE;

	//모델 로딩을 위한 쓰레드 생성
	CreateLoaderThread();

	return TRUE;
}


DWORD WINAPI smLoaderThreadProc(void* pInfo)
{
	//	HANDLE	hThread;

	int	cnt;
	smDPAT* smDPat;
	smDPAT* smDPat2;
	//	int Result;

	smDPAT* lpPatOld;
	smDPAT* lpPatOld2;

	//	char szDebugPrint[256];

		//크리티칼 섹션 선언
	EnterCriticalSection(&cLoadSection);

	//	while( smLoaderCnt<smLoaderEnd ) {
	cnt = smLoaderCnt & DATALOADER_MASK;

	//배경 로드
	if (smLoader[cnt].lpStage) {
		//로딩중 화면 표시
		OpenDispLoading();

		if (smSTAGE3D_ReadASE(smLoader[cnt].szLoadName, smLoader[cnt].lpStage))
		{

			smLoader[cnt].Result = TRUE;
			smLoader[cnt].lpStage->Head = TRUE;
		}
		//로딩 표시종료
		CloseDispLoading();
	}

	//아이템 로드
	if (smLoader[cnt].lpItem) {
		smLoader[cnt].lpItem->PatLoading = TRUE;
		//AddChatBuff( smLoader[cnt].szLoadName );

		lpPatOld = smLoader[cnt].lpItem->lpDinaPattern;
		smDPat = smPattern.LoadCharactor(smLoader[cnt].szLoadName);

		if (smDPat)
		{


			//캐릭터 스킨 확인 초기화
			//smLoader[cnt].lpChar->Init();

			smLoader[cnt].lpItem->DisplayFlag = 0;

			smLoader[cnt].lpItem->SetPattern(smDPat->Pat);
			smLoader[cnt].lpItem->lpDinaPattern = smDPat;
			lstrcpy(smLoader[cnt].lpItem->szModelName, smDPat->szPatName);
			smLoader[cnt].lpItem->DisplayFlag = TRUE;

		}
		smLoader[cnt].Result = TRUE;

		if (smDPat && lpPatOld)
			smDPAT_Delete(lpPatOld);

		smLoader[cnt].lpItem->PatLoading = FALSE;
	}



	//캐릭터 로드
	if (smLoader[cnt].lpChar) {
		smDPat = 0;
		smDPat2 = 0;

		smLoader[cnt].lpChar->PatLoading = TRUE;
		//AddChatBuff( smLoader[cnt].szLoadName );

		lpPatOld = smLoader[cnt].lpChar->lpDinaPattern;
		lpPatOld2 = smLoader[cnt].lpChar->lpDinaPattern2;

		smDPat = smPattern.LoadCharactor(smLoader[cnt].szLoadName);

		if (smDPat) {
			//캐릭터 스킨 확인 초기화
			//smLoader[cnt].lpChar->Init();

			smLoader[cnt].lpChar->DisplayFlag = 0;

			smLoader[cnt].lpChar->SetDinaPattern(smDPat);

			smLoader[cnt].lpChar->SetMotionFromCode(CHRMOTION_STATE_STAND);

			//				smLoader[cnt].lpChar->SetTool( PatKnife ,  hvPOSI_RHAND );
							//smLoader[cnt].lpChar->ActionPattern = 99;
			smLoader[cnt].lpChar->Flag = 1;
			lstrcpy(smLoader[cnt].lpChar->smCharInfo.szModelName, smDPat->szPatName);


			smLoader[cnt].lpChar->smCharInfo.szModelName2[0] = 0;

			if (smDPat->lpModelInfo->szSubModelFile[0]) {
				//보조 모델 가져오기 ( 메인모델에 보조 모델 파일이 존재할경우 )
				smDPat2 = smPattern.LoadCharactor(smDPat->lpModelInfo->szSubModelFile);
			}
			else {
				//보조 모델 로딩이 설정되 있는 경우 
				if (smLoader[cnt].szLoadName2[0]) {
					smDPat2 = smPattern.LoadCharactor(smLoader[cnt].szLoadName2);
					lstrcpy(smLoader[cnt].lpChar->smCharInfo.szModelName2, smDPat2->szPatName);
				}
			}

			smLoader[cnt].lpChar->SetDinaPattern2(smDPat2);
			smLoader[cnt].lpChar->DisplayFlag = TRUE;

		}
		else {
			/*
			//현재 하드에 없는 캐릭터 스킨
			smDPat = smPattern.LoadCharactor( szDefaultCharName );

			if ( lpD3DDevice && smDPat->UseCount==1 ) {
				//wsprintf( szDebugPrint , "TEX_READ:%s",szDefaultCharName );
				//AddChatBuff( szDebugPrint );
				ReadTextures();
				//스왑 텍스쳐 확인
				if ( smDPat && smDPat->Pat ) smDPat->Pat->smMaterialGroup->CheckMatreialTextureSwap();
			}

			smLoader[cnt].lpChar->DisplayFlag = 0;

			//캐릭터 스킨 확인 초기화
			//smLoader[cnt].lpChar->Init();
			smLoader[cnt].lpChar->SetDinaPattern( smDPat );
			smLoader[cnt].lpChar->SetMotionFromCode( CHRMOTION_STATE_STAND );

//				smLoader[cnt].lpChar->SetTool( PatKnife ,  hvPOSI_RHAND );
				//smLoader[cnt].lpChar->ActionPattern = 99;
				smLoader[cnt].lpChar->Flag = 1;
				lstrcpy( smLoader[cnt].lpChar->smCharInfo.szModelName , smDPat->szPatName );
				smLoader[cnt].lpChar->DisplayFlag = TRUE;
				*/
			Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 7");
			DisconnectFlag = dwPlayTime;
		}
		smLoader[cnt].Result = TRUE;

		if (lpPatOld) smDPAT_Delete(lpPatOld);
		if (lpPatOld2) smDPAT_Delete(lpPatOld2);
		/*
					if ( smDPat && lpPatOld ) {
						if ( smDPat2 && lpPatOld2 ) smDPAT_Delete( lpPatOld2 );
						smDPAT_Delete( lpPatOld );
					}
		*/

		smLoader[cnt].lpChar->PatLoading = FALSE;
	}
	smLoaderCnt++;
	//	}

		//크리티칼 섹션 해제
	LeaveCriticalSection(&cLoadSection);

	ThreadLoaderStats = FALSE;				//쓰레드 상태
	//ExitThread( TRUE );


	return TRUE;
}

//모델 로딩을 위한 쓰레드 생성
int CreateLoaderThread()
{
	//	DWORD dwMsgId;
	//	HANDLE	handle;
	/*
	//	if ( ThreadLoaderStats==FALSE ) {
			handle = CreateThread( NULL , 0, smLoaderThreadProc , 0 , 0, &dwMsgId );
	//	}

		SetThreadPriority( handle , THREAD_PRIORITY_HIGHEST );		//클래스를 높게
	*/

	smLoaderThreadProc(0);

	return FALSE;
}
//모델 로더 초기화
int InitLoader()
{
	smLoaderCnt = 0;
	smLoaderEnd = 0;

	return TRUE;
}

//패턴버퍼를 초기화
int InitPatterns()
{
	smBipPattern.Init();
	smPattern.Init();

	smPattern.BipPattern = &smBipPattern;

	//모델 로더 초기화
	InitLoader();


	//크리티컬 섹션 초기화
	InitializeCriticalSection(&cLoadSection);

	//	CRITICAL_SECTION	cPatSection;


	return TRUE;
}

//패턴버퍼를 말기화
int ClosePatterns()
{
	smBipPattern.Close();
	smPattern.Close();

	return TRUE;
}

//제거
int smDPAT_Delete(smDPAT* dPat)
{
	if (dPat->UseCount > 0) {
		dPat->UseCount--;

		//######################################################################################
		//작 성 자 : 오 영 석
		if (dPat->lpTalkLink && dPat->UseCount == 0)
			smDPAT_Delete(dPat->lpTalkLink);
		//######################################################################################

		if (dPat->smDinaLink && dPat->UseCount == 0)
			smDPAT_Delete(dPat->smDinaLink);

		if (dPat->UseCount <= 0) {
			delete dPat->Pat;
			dPat->Pat = 0;
			if (dPat->lpModelInfo) {
				delete dPat->lpModelInfo;
				dPat->lpModelInfo = 0;
			}
		}
	}
	return NULL;
}


//캐릭터에 패턴을 설정
int SetLoadPattern(smCHAR* smChar, char* szName, char* szName2, POINT3D* Posi, POINT3D* Angle)
{
	smDPAT* smDPat;
	smDPAT* smDPat2;

	smDPat = 0;
	smDPat2 = 0;

	smDPat = smPattern.LoadCharactor(szName);

	if (smDPat) {
		smChar->Init();
		if (Posi && Angle)
			smChar->SetPosi(Posi->x, Posi->y, Posi->z, Angle->x, Angle->y, Angle->z);

		smChar->SetDinaPattern(smDPat);
		smChar->ChangeMotion(0);
		smChar->DisplayFlag = TRUE;
		lstrcpy(smChar->smCharInfo.szModelName, smDPat->szPatName);

		smChar->smCharInfo.szModelName2[0] = 0;

		if (smDPat->lpModelInfo->szSubModelFile[0]) {
			//보조 모델 가져오기 ( 메인모델에 보조 모델 파일이 존재할경우 )
			smDPat2 = smPattern.LoadCharactor(smDPat->lpModelInfo->szSubModelFile);
		}
		else {
			//보조 모델 로딩이 설정되 있는 경우 
			if (szName2 && szName2[0]) {
				smDPat2 = smPattern.LoadCharactor(szName2);
				lstrcpy(smChar->smCharInfo.szModelName2, smDPat2->szPatName);
			}
		}

		//if ( smDPat2 ) {
		smChar->SetDinaPattern2(smDPat2);
		//}

		return TRUE;
	}

	return FALSE;
}

//캐릭터에 패턴을 설정
int SetLoadPattern(smCHAR* smChar, char* szName, POINT3D* Posi, POINT3D* Angle)
{

	return SetLoadPattern(smChar, szName, 0, Posi, Angle);
}




////////////////////////// smCHAR 함수 /////////////////////////////
smCHAR::smCHAR()
{
	Pattern = 0;
	Pattern2 = 0;

	Init();
}

smCHAR::~smCHAR()
{

}

void smCHAR::Init()
{

	//######################################################################################
	//작 성 자 : 오 영 석
	TalkPattern = NULL;
	TalkFrame = TalkSoundSum = 0;
	lpTalkModelParent = NULL;
	lpTalkMotionInfo = NULL;

	::ZeroMemory(&FrameInfo, sizeof(SMotionStEndInfo));
	//######################################################################################

	frame = 0;
	action = 0;
	Flag = 0;
	pX = pY = pZ = 0;
	Angle.x = Angle.y = Angle.z = 0;
	MoveFlag = FALSE;
	FrameCnt = 0;
	Counter = 0;

	frame = 0;

	ZeroMemory(PlayBuff, sizeof(smPLAYBUFF) * PLAYBUFF_SIZE);
	PlayBuffCnt = 0;
	PlayBuffPosi_End = 0;

	TransMinCnt = 8;
	TransLevel = 0;
	TransDelayMax = PLAYCOUNT_PER_SECOND * 5;

	lpDinaPattern = 0;
	lpDinaPattern2 = 0;
	PatLoading = 0;

	lpDinaLeftPattern = 0;
	lpDinaRightPattern = 0;

	HvLeftHand.PatTool = 0;
	HvRightHand.PatTool = 0;
	HvLeftHand.dwItemCode = 0;
	HvRightHand.dwItemCode = 0;

	HvLeftHand.ColorBlink = 0;
	HvRightHand.ColorBlink = 0;

	//	int		ColorBlink;
	//	short	sColors[4];
		/*
		HvRightHand.sColors[SMC_R] = 0;
		HvRightHand.sColors[SMC_G] = 0;
		HvRightHand.sColors[SMC_B] = 190;
		HvRightHand.sColors[SMC_A] = 96;

		HvLeftHand.sColors[SMC_R] = 0;
		HvLeftHand.sColors[SMC_G] = 0;
		HvLeftHand.sColors[SMC_B] = 0;
		HvLeftHand.sColors[SMC_A] = -180;
		*/


	wStickItems[0] = -1;
	wStickItems[1] = -1;
	wStickItems[2] = -1;
	wStickItems[3] = -1;

	DisplayFlag = 0;
	AnimDispMode = 0;
	PatDispMode = DISP_MODE_PATMAIN;
	MotionSelectFrame = 0;

	smMotionInfo = 0;

	//######################################################################################
	//작 성 자 : 오 영 석
	smMotionInfo2 = 0;
	//######################################################################################


	DisplayTools = TRUE;

	dwTarget = FALSE;

	if (smStage) lpStage = smStage;

	//움직일 다음 목적 좌표
	ptNextTarget.x = 0;
	ptNextTarget.y = 0;
	ptNextTarget.z = 0;

	//목적 캐릭터 ~~ 
	lpTargetPlayInfo = 0;		//목표 캐릭터 (서버용)
	lpTargetChar = 0;		//목표 캐릭터 (클라이언트용)

	lpMasterPlayInfo = 0;		//주인 캐릭터 ( 서버용 )

	//체력 임시 설정
	smCharInfo.Life[0] = 100;
	smCharInfo.Life[1] = 100;
	smCharInfo.State = 0;
	//smCharInfo.Brood = 0;

	dwLastTransTime = dwPlayTime;

	FrameCounter = 0;

	PatWidth = 0;
	PatHeight = 0;
	PatSizeLevel = 0;

	FrameStep = 80;
	MoveSpeed = 250;		//이동 움직임 값
	AttackSpeed = fONE;		//공격 속도

	lpExt1 = 0;
	lpExt2 = 0;
	lpExt3 = 0;
	lpExt4 = 0;

	//채팅 문자열 초기화
	dwChatMessageTimer = dwPlayTime;
	//전송 버퍼 초기화
	srTransBuffSize = 0;

	dwActionItemCode = 0;		//기본 액셕 아이템 

	ShootingMode = FALSE;
	ShootingFlag = FALSE;
	chrAttackTarget = 0;

	AttackToolRange = 0;
	AttackAnger = 0;
	AttackIce = 0;
	OnStageField = -1;
	OnStageFieldState = 0;
	OldMotionState = 0;

	OverLapPosi = 0;
	OverLapPosi2 = 0;
	lpCharOverLap = 0;			//겹친 캐릭터의 포인트

	TargetMoveCount = 0;		//목표 이동좌표 카운터

	AttackObjBip = 0;

	NearPlayCount = 256;		//근처에 유저 확인 카운터 ( 서버에서 사용 )
	AttackCritcal = 0;			//크리티컬 힛 설정

	AutoMoveStep = 0;

	LevelLog = 0;

	dwNextMotionCode = 0;
	PosBeginCount = 0;

	EnableStateBar = 0;
	DisplayAlpha = 0;			//반투명도
	RendAlpha = 0;				//반투명 캐릭
	AttackSkil = 0;				//스킬공격
	AttackEffect = 0;			//공격용 효과 초기화

	dwDispAppSkill = 0;
	dwDispAppSkillMask = 0;

	HoSkillCode = 0;
	PartyFlag = 0;
	dwAttackPlayTime = 0;

	PlaySlowCount = 0;
	PlaySlowSpeed = 0;
	PlayStunCount = 0;
	PlayStopCount = 0;
	ServerCode = 0;

	PlayHolyMind[1] = 0;			//데미지 약화 기능 ( [0] 감소된 데미지% [1] 유지시간 )
	PlayHolyPower[1] = 0;		//데미지 강화 기능 ( [0] 강화된 데미지% [1] 유지시간 )
	PlayPoison[1] = 0;			//독 공격 받음
	DispPoison = 0;				//독 표시
	PlayHolyIncantation[1] = 0;	//몬스터 현혹
	PlayVanish = 0;				//배니쉬 스킬 적용상태
	PlayCurseQuest = 0;			//저주 퀘스트
	PlayVague = 0;
	PlayDistortion = 0;
	PlayInvincible = 0;
	//	Bless_HealLimit = 0;

	dwForceOfNatureTime = 0;	//포스오브 네이쳐 유지시간
	dwHallOfValhallaTime = 0;	//홀오브발할라 유지시간


	AttackSkillRange = 0;

	RecvExp = 0;				//이캐릭으로 부터 받은 경험치 

	sAddColorEffect[0] = 0;
	sAddColorEffect[1] = 0;
	sAddColorEffect[2] = 0;
	sAddColorEffect[3] = 0;

	((DWORD*)smCharInfo.wPlayClass)[0] = 0;

	RenderLatter = FALSE;
	dwUpdateCharInfoTime = 0;

	dwTradeMsgCode = 0;
	EventAttackCount = 0;

	dwClanManageBit = 0;

	lpAttackDamageList = 0;
	lpAttackDamageList_BlessCastle = 0;

	dwForceOrbCode = 0;		//포스오브 적용 코드
	dwForceOrbTime = 0;		//포스오브 유지시간

	// 박재원 - 데미지 부스터(생명력)
	dwLifeBoosterCode = 0; // 박재원 - 부스터 아이템(생명력) 코드
	dwLifeBoosterTime = 0; // 박재원 - 부스터 아이템(생명력) 유지시간
	// 박재원 - 데미지 부스터(기력)
	dwManaBoosterCode = 0; // 박재원 - 부스터 아이템(기력) 코드
	dwManaBoosterTime = 0; // 박재원 - 부스터 아이템(기력) 유지시간
	// 박재원 - 데미지 부스터(근력)
	dwStaminaBoosterCode = 0; // 박재원 - 부스터 아이템(근력) 코드
	dwStaminaBoosterTime = 0; // 박재원 - 부스터 아이템(근력) 유지시간

	// 장별 - 스킬 딜레이
	dwSkillDelayCode = 0;
	dwSkillDelayTime = 0;

	dwLinkObjectCode = 0;
	lpLinkPlayInfo = 0;
	lpLinkChar = 0;

	PkMode_CharState = 0;
	dwClanCode = 0;
	UseObject_VirtualLife = 0;


#ifndef _W_SERVER
	//( 클라이언트 에서만 사용 )
	//무기용 이펙트 
	WeaponEffect = 0;
	dwWeaponEffectTime = 0;

	ClanInfoNum = -1;					//클랜 정보 초기화
	Clan_CastleMasterFlag = 0;				//성주 클랜
	dwClanInfoTime = 0;
	TryAttackCount = 0;

	((DWORD*)sBlessCastle_Damage)[0] = 0;	//공성 개인별 데미지 추가치


#else
	dwTargetLockTime = 0;

	lpCompulsionPlayInfo = 0;	//시선끌기 캐릭터
	dwCompulsionTime = 0;		//시선끌기 시간

	dwAssassinEyeTime = 0;		//어세신아이 적용 몬스터
	AssassinEyeParam = 0;		//어세신아이 적용 몬스터

	//몬스터 이동 스텝 구성
	sMoveStepCount[0] = 0;
	sMoveStepCount[1] = 0;

	DontMoveFlag = 0;

	Clan_CastleMasterFlag = 0;				//빌링 몬스터 플랙 ( 서버에서 서로 공격안하도록 하기위한 구분용 )


#endif

}


int smCHAR::Close()
{
	Flag = 0;

	if (lpDinaPattern2) {
		AddRemoveDynPattern(lpDinaPattern2);
		//smDPAT_Delete( lpDinaPattern2 );
		lpDinaPattern2 = 0;
	}



	if (lpDinaPattern) {
		AddRemoveDynPattern(lpDinaPattern);
		//smDPAT_Delete( lpDinaPattern );
		lpDinaPattern = 0;
	}

	if (lpDinaLeftPattern) {
		AddRemoveDynPattern(lpDinaLeftPattern);
		//smDPAT_Delete( lpDinaLeftPattern );
		lpDinaLeftPattern = 0;
	}

	if (lpDinaRightPattern) {
		AddRemoveDynPattern(lpDinaRightPattern);
		//smDPAT_Delete( lpDinaRightPattern );
		lpDinaRightPattern = 0;
	}


	Pattern = 0;
	Pattern2 = 0;

	HvLeftHand.PatTool = 0;
	HvRightHand.PatTool = 0;

	smMotionInfo = 0;
	smMotionInfo2 = 0;

	dwDispAppSkill = 0;

	if (lpAttackDamageList) {
		delete	lpAttackDamageList;
		lpAttackDamageList = 0;
	}

	return TRUE;
}


//패턴설정
int smCHAR::SetPattern(smPAT3D* pat)
{
	smPAT3D* AnimPat;

	Pattern = pat;

	if (pat->TmParent) AnimPat = pat->TmParent;
	else AnimPat = pat;

	HvRightHand.ObjBip = AnimPat->GetObjectFromName(szBipName_RightHand);

	//왼쪽 도구
	if ((HvLeftHand.dwItemCode & sinITEM_MASK2) == sinDS1)
		HvLeftHand.ObjBip = AnimPat->GetObjectFromName(szBipName_Shield);		//방패
	else
		HvLeftHand.ObjBip = AnimPat->GetObjectFromName(szBipName_LeftHand);	//기타 활등


	BackObjBip[0] = AnimPat->GetObjectFromName(szBipName_BackSpine);
	BackObjBip[1] = AnimPat->GetObjectFromName(szBipName_BackSpineBow);
	BackObjBip[2] = AnimPat->GetObjectFromName(szBipName_BackSpineCross);
	ShieldObjBip = AnimPat->GetObjectFromName(szBipName_Shield2);

	//	if ( !BackObjBip[1] ) BackObjBip[1]=BackObjBip[0];
	//	if ( !BackObjBip[2] ) BackObjBip[2]=BackObjBip[0];

	AttackObjBip = 0;

	//여자 기본 공격 발차기 ( 아쳐 , 아탈란타 )
	if (smCharInfo.JOB_CODE == 3 || smCharInfo.JOB_CODE == 5)
		AttackObjBip = AnimPat->GetObjectFromName(szBipName_LFoot);

	if (smCharInfo.JOB_CODE == 7) //메지션 박치기
		AttackObjBip = AnimPat->GetObjectFromName(szBipName_Head);

	AnimPattern = AnimPat;

	PatHeight = pat->SizeHeight;
	PatWidth = (pat->SizeWidth * 160) >> FLOATNS;

	//	PatSizeLevel

	return TRUE;
}

//패턴머리 설정
int smCHAR::SetPattern2(smPAT3D* pat)
{
	//	smPAT3D *AnimPat;

	Pattern2 = pat;

	return TRUE;
}

// Posi豫o das Garras
DWORD	dwItemCodeFromVillage[] = {
	sinWC1 | sin01 , sinWC1 | sin02 , sinWC1 | sin03 , sinWC1 | sin04 , sinWC1 | sin05 ,
	sinWC1 | sin06 , sinWC1 | sin07 , sinWC1 | sin08 , sinWC1 | sin09 , sinWC1 | sin10 ,
	sinWC1 | sin11 , sinWC1 | sin12 , sinWC1 | sin13 , sinWC1 | sin14 , sinWC1 | sin15 ,
	sinWC1 | sin16 , sinWC1 | sin17 , sinWC1 | sin18 , sinWC1 | sin19 , sinWC1 | sin20 ,
	sinWC1 | sin21 , sinWC1 | sin22 , sinWC1 | sin23 , sinWC1 | sin24 , sinWC1 | sin25 ,
	sinWC1 | sin26 , sinWC1 | sin27 , sinWC1 | sin28 , sinWC1 | sin29 , sinWC1 | sin30 ,

	sinWM1 | sin01 , sinWM1 | sin02 , sinWM1 | sin03 , sinWM1 | sin06 ,

	sinWS2 | sin01 , sinWS2 | sin02 , sinWS2 | sin03 ,

	sinWS1 | sin02, sinWS1 | sin03,
	sinWS1 | sin09, sinWS1 | sin29,
	0
};

// Posi豫o dos Arcos (Besta)
DWORD	dwItemCodeFromVillage_Bow[] = {
	sinWS1 | sin04, sinWS1 | sin08, sinWS1 | sin10, sinWS1 | sin13, sinWS1 | sin17, sinWS1 | sin20,
	0
};

// Posi豫o dos Arcos (Normais)
DWORD	dwItemCodeFromVillage_Cross[] = {
	sinWS1 | sin01, sinWS1 | sin05, sinWS1 | sin06, sinWS1 | sin07,
	sinWS1 | sin11, sinWS1 | sin12, sinWS1 | sin14, sinWS1 | sin15,
	sinWS1 | sin16, sinWS1 | sin18, sinWS1 | sin19,
	sinWS1 | sin21, sinWS1 | sin22, sinWS1 | sin23, sinWS1 | sin24,
	sinWS1 | sin25, sinWS1 | sin26, sinWS1 | sin27, sinWS1 | sin28,
	sinWS1 | sin30, sinWS1 | sin31, sinWS1 | sin32,

	0
};



int smCHAR::SetTool(DWORD dwItemCode, int hvPosi)
{
	char szToolMeshName[64];
	int cnt, szX, szN;

	szX = szN = 0;

	if (smCharInfo.State != smCHAR_STATE_USER && PkMode_CharState != smCHAR_STATE_USER) return FALSE;

	if (dwItemCode)
		sinGetItemInfo(dwItemCode, szToolMeshName);

	switch (hvPosi) {
	case hvPOSI_RHAND:
		if (HvRightHand.ObjBip) {
			if (lpDinaRightPattern) {
				AddRemoveDynPattern(lpDinaRightPattern);
				//smDPAT_Delete( lpDinaRightPattern );
				lpDinaRightPattern = 0;
			}
			if (dwItemCode) {
				lpDinaRightPattern = smPattern.LoadCharactor(szToolMeshName);
				if (lpDinaRightPattern) {
					HvRightHand.PatTool = lpDinaRightPattern->Pat;
					for (cnt = 0; cnt < lpDinaRightPattern->Pat->nObj3d; cnt++) {
						if (szX < lpDinaRightPattern->Pat->obj3d[cnt]->maxY)
							szX = lpDinaRightPattern->Pat->obj3d[cnt]->maxY;
						if (szN < lpDinaRightPattern->Pat->obj3d[cnt]->minY)
							szN = lpDinaRightPattern->Pat->obj3d[cnt]->minY;
					}
					HvRightHand.SizeMax = szX;
					HvRightHand.SizeMin = szN;
					HvRightHand.dwItemCode = dwItemCode;
					//오른쪽 도구는 손에 붙임
					HvRightHand.ObjBip = AnimPattern->GetObjectFromName(szBipName_RightHand);
					dwActionItemCode = dwItemCode;

					//wStickItems[smPLAYBUFF_ITEM_ACTION] = GetSinItemNumFromCode( dwActionItemCode );
					wStickItems[smPLAYBUFF_ITEM_RIGHT] = GetSinItemNumFromCode(dwItemCode);
				}
				else {
					HvRightHand.PatTool = 0;
					HvRightHand.dwItemCode = 0;
					wStickItems[smPLAYBUFF_ITEM_RIGHT] = -1;
				}
			}
			else {
				HvRightHand.PatTool = 0;
				HvRightHand.dwItemCode = 0;
				wStickItems[smPLAYBUFF_ITEM_RIGHT] = -1;
			}
		}
		else
			return FALSE;
		break;

	case hvPOSI_LHAND:
		if (HvLeftHand.ObjBip) {
			if (lpDinaLeftPattern) {
				AddRemoveDynPattern(lpDinaLeftPattern);
				//smDPAT_Delete( lpDinaLeftPattern );
				lpDinaLeftPattern = 0;
			}
			if (dwItemCode) {
				//				lpDinaLeftPattern = smPattern.LoadCharactor( "Shield\\Ds101.ASE" );
				lpDinaLeftPattern = smPattern.LoadCharactor(szToolMeshName);
				if (lpDinaLeftPattern) {
					HvLeftHand.PatTool = lpDinaLeftPattern->Pat;
					for (cnt = 0; cnt < lpDinaLeftPattern->Pat->nObj3d; cnt++) {
						if (szX < lpDinaLeftPattern->Pat->obj3d[cnt]->maxY)
							szX = lpDinaLeftPattern->Pat->obj3d[cnt]->maxY;
						if (szN < lpDinaLeftPattern->Pat->obj3d[cnt]->minY)
							szN = lpDinaLeftPattern->Pat->obj3d[cnt]->minY;
					}
					HvLeftHand.SizeMax = szX;
					HvLeftHand.SizeMin = szN;
					HvLeftHand.dwItemCode = dwItemCode;
					wStickItems[smPLAYBUFF_ITEM_LEFT] = GetSinItemNumFromCode(dwItemCode);

					//왼쪽 도구
					if ((dwItemCode & sinITEM_MASK2) == sinDS1 || (dwItemCode & sinITEM_MASK2) == sinOM1) {
						if ((dwItemCode & sinITEM_MASK2) == sinDS1) {
							//방패
							HvLeftHand.ObjBip = AnimPattern->GetObjectFromName(szBipName_Shield);		//방패
						}
						else {
							//메지컬 스터프
							HvLeftHand.PatTool = 0;
							HvLeftHand.dwItemCode = 0;
							wStickItems[smPLAYBUFF_ITEM_LEFT] = -1;
						}
					}
					else {
						HvLeftHand.ObjBip = AnimPattern->GetObjectFromName(szBipName_LeftHand);	//기타 활등
						dwActionItemCode = dwItemCode;
						//wStickItems[smPLAYBUFF_ITEM_ACTION] = GetSinItemNumFromCode( dwActionItemCode );
					}
				}
				else {
					HvLeftHand.PatTool = 0;
					HvLeftHand.dwItemCode = 0;
					wStickItems[smPLAYBUFF_ITEM_LEFT] = -1;
				}
			}
			else {
				HvLeftHand.PatTool = 0;
				HvLeftHand.dwItemCode = 0;
				wStickItems[smPLAYBUFF_ITEM_LEFT] = -1;
			}

		}
		else
			return FALSE;
		break;
	}

	if (dwActionItemCode) {
		if (dwActionItemCode != HvLeftHand.dwItemCode && dwActionItemCode != HvRightHand.dwItemCode) {
			dwActionItemCode = 0;
			//wStickItems[smPLAYBUFF_ITEM_ACTION] = -1;
		}

		//공격 사정거리 설정
		if (dwActionItemCode == HvLeftHand.dwItemCode) {
			AttackToolRange = (HvLeftHand.SizeMax * 200) >> 8;
		}
		if (dwActionItemCode == HvRightHand.dwItemCode) {
			AttackToolRange = (HvRightHand.SizeMax * 200) >> 8;
		}

		cnt = 0;
		dwItemSetting = 1;

		while (1) {
			if (dwItemCodeFromVillage[cnt] == 0) break;
			if (dwItemCodeFromVillage[cnt] == dwActionItemCode) {
				dwItemSetting = 0;
				break;
			}
			cnt++;
		}

		cnt = 0;
		while (1) {
			if (dwItemCodeFromVillage_Bow[cnt] == 0) break;
			if (dwItemCodeFromVillage_Bow[cnt] == dwActionItemCode) {
				dwItemSetting = 2;
				break;
			}
			cnt++;
		}

		cnt = 0;
		while (1) {
			if (dwItemCodeFromVillage_Cross[cnt] == 0) break;
			if (dwItemCodeFromVillage_Cross[cnt] == dwActionItemCode) {
				dwItemSetting = 3;
				break;
			}
			cnt++;
		}

	}
	else
		AttackToolRange = 0;


	if ((dwActionItemCode & sinITEM_MASK2) == sinWS1 || (dwActionItemCode & sinITEM_MASK2) == sinWT1 ||
		((dwActionItemCode & sinITEM_MASK2) == sinWM1 && (smCharInfo.JOB_CODE == 7 || smCharInfo.JOB_CODE == 8))) {
		//활 , 마법 지팡이
		ShootingMode = TRUE;
	}
	else
		ShootingMode = FALSE;

	if ((smCharInfo.State == smCHAR_STATE_USER || PkMode_CharState == smCHAR_STATE_USER) && dwItemCode == dwActionItemCode) {
		if (MotionInfo->State != CHRMOTION_STATE_DEAD && MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
			MotionInfo->State != CHRMOTION_STATE_RESTART) {
			SetMotionFromCode(MotionInfo->State);
			AttackCritcal = -1;
		}

		if (dwItemCode) {
			//양손 무기 첵크
			for (cnt = 0; cnt < MAX_ITEM; cnt++) {
				if (sItem[cnt].CODE == dwItemCode) {
					if (sItem[cnt].Class == ITEM_CLASS_WEAPON_TWO && !ShootingMode)
						dwActionItemTwoHand = TRUE;
					else
						dwActionItemTwoHand = FALSE;

					break;
				}
			}
		}
	}

	/*
		if ( lpD3DDevice ) {
			ReadTextures();
			if ( HvRightHand.PatTool ) HvRightHand.PatTool->smMaterialGroup->CheckMatreialTextureSwap();
			if ( HvLeftHand.PatTool ) HvLeftHand.PatTool->smMaterialGroup->CheckMatreialTextureSwap();
		}
	*/
	DisplayTools = TRUE;
	return TRUE;
}



int smCHAR::SetPosi(int x, int y, int z, int angX, int angY, int angZ)
{
	int sy;

	pX = x;
	pZ = z;
	pY = y;

	if (y == 0 && lpStage)
		pY = lpStage->GetHeight(pX, pZ);

	if (pY == 0) {

		if (smGameStage[0])
			pY = smGameStage[0]->GetHeight(pX, pZ);

		if (smGameStage[1]) {
			sy = smGameStage[1]->GetHeight(pX, pZ);
			if (pY < sy) pY = sy;
		}
	}


	pY -= 2 * fONE;

	PHeight = pY;

	Angle.x = angX;
	Angle.y = angY;
	Angle.z = angZ;

	return TRUE;
}

int smCHAR::SetTargetPosi(int x, int z)
{

	tx = x << FLOATNS;
	tz = z << FLOATNS;
	MoveFlag = TRUE;

	Angle.y = GetRadian2D(pX, pZ, tx, tz);

	if (ActionPattern == 0) {
		if (MoveMode)
			SetMotionFromCode(CHRMOTION_STATE_RUN);
		else
			SetMotionFromCode(CHRMOTION_STATE_WALK);
	}

	return Angle.y;
}

int smCHAR::SetTargetPosi2(int x, int z)
{

	tx = x << FLOATNS;
	tz = z << FLOATNS;

	Angle.y = GetRadian2D(pX, pZ, tx, tz);

	return Angle.y;
}


//다음 목적 좌표 설정
int smCHAR::SetNextTarget(int x, int y, int z)
{
	ptNextTarget.x = x;
	ptNextTarget.y = y;
	ptNextTarget.z = z;

	return TRUE;
}

int smCHAR::SetAction(int nAction)
{
	//			if ( CheckLocateBox( pX, pZ , tx, tz , 16*fONE )==TRUE )

	if (nAction == 0) MoveFlag = FALSE;

	return TRUE;

}

//먼지 생성
int	smCHAR::SetSmoking(int level)
{
	return TRUE;
}

//이동 ( 정밀 )
int smCHAR::MoveAngle2(int step)
{
	int x, z, y;
	int he, hy;
	int OldStageField;
	POINT3D MovePosi;
	int MovLevel[2];

	WaterHeight = CLIP_OUT;

#ifndef _W_SERVER

	if (smConfig.DebugMode && VRKeyBuff['N']) {

		x = pX + (((GetSin[Angle.y & ANGCLIP] >> FLOATNS) * step) >> FLOATNS);
		z = pZ + (((GetCos[Angle.y & ANGCLIP] >> FLOATNS) * step) >> FLOATNS);
		y = pY;

		//y = smStage->GetHeight( x , z );
		//if ( abs( pY-y )>(16*fONE) ) return FALSE;

		pY = y;
		pX = x;
		pZ = z;

		return TRUE;
	}
#else
	if (DontMoveFlag) return FALSE;
#endif

	POINT3D Posi;

	he = 0;

	Posi.x = x = pX;
	Posi.y = y = pY;
	Posi.z = z = pZ;

	OldStageField = OnStageField;
	OnStageField = -1;

	if (step == 0 || PHeight < y) {
		if (lpStage) {
			he = lpStage->GetFloorHeight(x, y, z, Pattern->SizeHeight);
		}
		else {
			if (smGameStage[0]) {
				he = smGameStage[0]->GetFloorHeight(x, y, z, Pattern->SizeHeight);
				OnStageField = 0;
			}

			if (smGameStage[1]) {
				hy = smGameStage[1]->GetFloorHeight(x, y, z, Pattern->SizeHeight);
				if (he < hy) {
					he = hy;
					OnStageField = 1;
				}
			}
		}
		if (he != CLIP_OUT)
			y = he;
		else
			y = -80 * fONE;

		if ((pY - y) > 8 * fONE) {
			if (MotionInfo->State != CHRMOTION_STATE_FALLDOWN && (pY - y) > 32 * fONE) {
				FallHeight = pY - y;
				SetMotionFromCode(CHRMOTION_STATE_FALLDOWN);
			}
			PHeight = y;
			pY -= 8 * fONE;
		}
		else {
			pY = y;
			PHeight = y;
			if (MotionInfo->State == CHRMOTION_STATE_FALLDOWN) {
				if (FallHeight > 200 * fONE)
					SetMotionFromCode(CHRMOTION_STATE_FALLDAMAGE);
				else
					SetMotionFromCode(CHRMOTION_STATE_FALLSTAND);

				SetSmoking(0);

			}
		}
		return TRUE;
	}
	else {
		if (lpStage && Pattern && lpStage->CheckNextMove(&Posi, &Angle, &MovePosi, step, Pattern->SizeWidth, Pattern->SizeHeight, OverLapPosi2) > 0) {
			x = MovePosi.x;
			y = MovePosi.y;
			z = MovePosi.z;
		}
		else {
			MovLevel[0] = 0;
			MovLevel[1] = 0;

			if (smGameStage[0] && (MovLevel[0] = smGameStage[0]->CheckNextMove(&Posi, &Angle, &MovePosi, step, Pattern->SizeWidth, Pattern->SizeHeight, OverLapPosi2)) > 0) {

				if (smStage_WaterChk != CLIP_OUT)
					WaterHeight = smStage_WaterChk;


				if (StageField[0]->LimitLevel <= smCharInfo.Level) {

					OnStageField = 0;

					he = MovePosi.y;
					x = MovePosi.x;
					y = MovePosi.y;
					z = MovePosi.z;
				}

			}
			if (smGameStage[1] && (MovLevel[1] = smGameStage[1]->CheckNextMove(&Posi, &Angle, &MovePosi, step, Pattern->SizeWidth, Pattern->SizeHeight, OverLapPosi2) > 0)) {

				if (smStage_WaterChk != CLIP_OUT)
					WaterHeight = smStage_WaterChk;

				if (StageField[1]->LimitLevel <= smCharInfo.Level) {
					if (MovLevel[0] && abs(he - MovePosi.y) < fONE * 8) {
						if (MovLevel[0] > MovLevel[1]) {
							x = MovePosi.x;
							y = MovePosi.y;
							z = MovePosi.z;
							OnStageField = 1;
						}
					}
					else {
						if (he < MovePosi.y) {//Posi.y ) {
							x = MovePosi.x;
							y = MovePosi.y;
							z = MovePosi.z;
							OnStageField = 1;
						}
					}
				}
			}
		}
	}

	pX = x;
	pZ = z;

	if ((pY - y) > 8 * fONE) {

		if (MotionInfo->State != CHRMOTION_STATE_FALLDOWN && (pY - y) > 32 * fONE) {
			FallHeight = pY - y;
			SetMotionFromCode(CHRMOTION_STATE_FALLDOWN);
		}

		PHeight = y;
		pY -= 8 * fONE;
	}
	else {
		pY = y;
		PHeight = y;
		if (MotionInfo->State == CHRMOTION_STATE_FALLDOWN) {
			if (FallHeight > 200 * fONE)
				SetMotionFromCode(CHRMOTION_STATE_FALLDAMAGE);
			else
				SetMotionFromCode(CHRMOTION_STATE_FALLSTAND);

			SetSmoking(0);

		}
	}

	if (OnStageField >= 0 && OldStageField != OnStageField) {
		OnStageFieldState = StageField[OnStageField]->State;
		if (OldStageField < 0 || OnStageFieldState != StageField[OldStageField]->State) {
			SetMotionFromCode(MotionInfo->State);
		}
	}
	if (OnStageField < 0)
		OnStageField = OldStageField;

	return TRUE;
}


//이동
int smCHAR::MoveAngle(int step)
{
	return	MoveAngle2(step * fONE);
}

int smCHAR::SetDinaPattern(smDPAT* lpDPat)
{

	//######################################################################################
	//작 성 자 : 오 영 석
	TalkPattern = NULL;
	TalkFrame = TalkSoundSum = 0;
	lpTalkModelParent = NULL;
	lpTalkMotionInfo = NULL;

	if (smConfig.DebugMode)
	{
		Debug_TalkZoomMode = 0;
		Debug_TalkRepeatMode = 0;
		Debug_TalkRepeatModeType = -1;
	}
	//######################################################################################

	lpDinaPattern = lpDPat;
	smMotionInfo = lpDinaPattern->lpModelInfo;
	SetPattern(lpDPat->Pat);

	SetMotionFromCode(CHRMOTION_STATE_STAND);

	return TRUE;
}

int smCHAR::SetDinaPattern2(smDPAT* lpDPat)
{
	lpDinaPattern2 = lpDPat;

	// Corre豫o CRASH
	if (lpDinaPattern2 == nullptr || !lpDPat)
		return FALSE;
	// CORRE플O CRASH

	if (lpDinaPattern2) {
		smMotionInfo2 = lpDinaPattern2->lpModelInfo;
		SetPattern2(lpDPat->Pat);

		if (Pattern->TmParent == Pattern2->TmParent) {
			AnimDispMode = 0;				//같이 출력
			PatDispMode = DISP_MODE_PATMAIN | DISP_MODE_PATSUB;
		}
		else {
			AnimDispMode = 1;				//하나씩 출력
			PatDispMode = DISP_MODE_PATMAIN;
		}
	}
	else {
		AnimDispMode = 0;				//같이 출력
		PatDispMode = DISP_MODE_PATMAIN;
		SetPattern2(0);
	}

	//######################################################################################
	//작 성 자 : 오 영 석
	if (lpDinaPattern->lpTalkLink)
	{
		PatDispMode |= DISP_MODE_PATTALK;
		TalkPattern = lpDinaPattern->lpTalkLink->Pat;
	}
	else if (lpDinaPattern2 && lpDinaPattern2->lpTalkLink)
	{
		PatDispMode |= DISP_MODE_PATTALK;
		TalkPattern = lpDinaPattern2->lpTalkLink->Pat;
	}

	if (TalkPattern && (PatDispMode & DISP_MODE_PATTALK))
	{
		if (smMotionInfo->szTalkLinkFile[0])
			lpTalkModelParent = smMotionInfo;
		else if (smMotionInfo2 && smMotionInfo2->szTalkLinkFile[0])
			lpTalkModelParent = smMotionInfo2;
		else
		{
			// 에러 처리. 작동되지 않게끔.
			PatDispMode &= (~DISP_MODE_PATTALK);
			TalkPattern = NULL;
		}
	}
	//######################################################################################

	return TRUE;
}

//움직임 동작 설정
int smCHAR::ChangeMotion(int Motion, int DpMode)
{

	if (DpMode) {
		//다중 모델일 경우 움직임 선택
		if (MotionSelectFrame) {
			MotionInfo = &smMotionInfo2->MotionInfo[Motion];
			PatDispMode = DISP_MODE_PATSUB;
		}
		else {
			MotionInfo = &smMotionInfo->MotionInfo[Motion];
			PatDispMode = DISP_MODE_PATMAIN;
		}
	}
	else {
		//기본 모델 움직임으로 설정
		if (AnimDispMode)
			PatDispMode = DISP_MODE_PATMAIN;

		MotionInfo = &smMotionInfo->MotionInfo[Motion];
		MotionSelectFrame = 0;
	}

	frame = MotionInfo->StartFrame * 160;
	action = Motion;
	FrameCounter = 0;

	AttackSkil = 0;
	HideWeapon = 0;
	MotionLoop = 0;

	dwEventFrameExt[0] = 0;
	dwEventFrameExt[1] = 0;
	dwEventFrameExt[2] = 0;
	dwEventFrameExt[3] = 0;

	//######################################################################################
	//작 성 자 : 오 영 석
	FrameInfo.StartFrame = frame;
	FrameInfo.EndFrame = MotionInfo->EndFrame * 160;
	//######################################################################################

	return TRUE;
}

int smCHAR::AutoChangeTalkMotion(int TalkModeType)
{
	if (TalkPattern == NULL)
		return 0;

	int TalkCnt = 0;
	if (lpTalkModelParent->TalkMotionRate[CHRMOTION_EXT])
	{
		TalkCnt = rand() % 100;
		TalkCnt = lpTalkModelParent->TalkMotionRateCnt[TalkModeType][TalkCnt];
	}
	else
		TalkCnt = rand() % lpTalkModelParent->TalkMotionCount;

	lpTalkMotionInfo = &lpTalkModelParent->TalkMotionInfo[TalkCnt];

	if (lpTalkMotionInfo)
		TalkFrame = lpTalkMotionInfo->StartFrame * 160;

	return 1;
}

int smCHAR::FindMotionCountFromCode(DWORD MotionCode)
{
	int cnt;
	int FindCnt = 0;


	for (cnt = 0; cnt < (int)smMotionInfo->MotionCount; cnt++) {
		if (smMotionInfo->MotionInfo[cnt].State == MotionCode) {
			FindCnt++;
		}
	}


	return FindCnt;
}

int smCHAR::SetMotionFromCode(DWORD MotionCode)
{
	int cnt, cnt2, itcnt;
	int FoundNum[64];
	int FindCnt;

	int FoundNum2[64];
	int FindCnt2;
	int StageVillage;
	int	dwActItemCode;
	int	atSkill = 0;
	int	skillCode;
	BYTE	bSkill;

	DWORD	dwJobCodeMask;

	DWORD dwFindCode;

	smMODELINFO* lpMotionInfo;

	if (smCharInfo.JOB_CODE && smCharInfo.JOB_CODE < 16)
		dwJobCodeMask = BitMaskJobNames[smCharInfo.JOB_CODE - 1].dwBitMask;
	else
		dwJobCodeMask = 0;


	FindCnt = 0;
	FindCnt2 = 0;
	MotionSelectFrame = 0;

	dwActItemCode = dwActionItemCode;

	if (OnStageField >= 0) {
		if (StageField[OnStageField]->State == FIELD_STATE_VILLAGE) {
			StageVillage = 1;
			dwActItemCode = 0;
		}
		else
			StageVillage = 2;
	}
	else {
#ifdef _W_SERVER
		StageVillage = 3;
#else
		if (GameMode == 1) StageVillage = 1;
		else StageVillage = 3;
#endif
	}

	// Corre豫o crash motion aqui
	if (!smMotionInfo || smMotionInfo == NULL)
		return FALSE;

	lpMotionInfo = smMotionInfo;

	if (MotionCode == CHRMOTION_STATE_SKILL && AttackSkil) {
		atSkill = AttackSkil;
	}

	for (cnt = 0; cnt < (int)smMotionInfo->MotionCount; cnt++) {
		//각종 조건에 맞는 모션을 찾는다 ( 동작/맵위치/직업 등을 고려 )
		if (smMotionInfo->MotionInfo[cnt].State == MotionCode &&
			(!smMotionInfo->MotionInfo[cnt].MapPosition || (smMotionInfo->MotionInfo[cnt].MapPosition & StageVillage)) &&
			(!smMotionInfo->MotionInfo[cnt].dwJobCodeBit || !dwJobCodeMask ||
				(smMotionInfo->MotionInfo[cnt].dwJobCodeBit & dwJobCodeMask))) {

			if (atSkill) {
				//스킬 모션일 경우
				cnt2 = 0;
				skillCode = atSkill & 0xFF;
				while (1) {
					//스킬 코드 검색
					bSkill = smMotionInfo->MotionInfo[cnt].SkillCodeList[cnt2];
					if (bSkill == 0) break;
					if (SkillDataCode[bSkill].dwPlaySkillCode == skillCode) {
						break;
					}
					cnt2++;
				}
				if (bSkill) {
					//스킬 검색 성공
					FoundNum[FindCnt++] = cnt;
					if (FindCnt >= 32) break;
				}
			}
			else {
				FoundNum[FindCnt++] = cnt;
				if (FindCnt >= 32) break;
			}
		}
	}


	if (FindCnt == 0 && AnimDispMode && lpDinaPattern2) {
		//2차 모델에서 정보를 찾음
		for (cnt = 0; cnt < (int)smMotionInfo2->MotionCount; cnt++) {
			if (smMotionInfo2->MotionInfo[cnt].State == MotionCode &&
				(!smMotionInfo2->MotionInfo[cnt].MapPosition || (smMotionInfo2->MotionInfo[cnt].MapPosition & StageVillage))) {
				FoundNum[FindCnt++] = cnt;
				if (FindCnt >= 32) break;
			}
		}

		if (FindCnt) {
			MotionSelectFrame = 1;
			lpMotionInfo = smMotionInfo2;
		}
	}

	for (cnt = 0; cnt < FindCnt; cnt++) {
		itcnt = lpMotionInfo->MotionInfo[FoundNum[cnt]].ItemCodeCount;
		if (itcnt == 0) {
			FoundNum2[FindCnt2++] = FoundNum[cnt];
		}
		else {
			if (dwActItemCode) {
				for (cnt2 = 0; cnt2 < itcnt; cnt2++) {
					dwFindCode = lpMotionInfo->MotionInfo[FoundNum[cnt]].ItemCodeList[cnt2];
					if (dwActItemCode == (int)sItem[dwFindCode].CODE) {
						FoundNum2[FindCnt2++] = FoundNum[cnt];
						break;
					}
				}
			}
			else {
				for (cnt2 = 0; cnt2 < itcnt; cnt2++) {
					dwFindCode = lpMotionInfo->MotionInfo[FoundNum[cnt]].ItemCodeList[cnt2];
					if (dwFindCode == 0xFF) {
						FoundNum2[FindCnt2++] = FoundNum[cnt];
						break;
					}
				}
			}
		}
	}

	/*
	#define MOTION_LIST_MAX			32
	#define MOTION_INFO_MAX			512
	#define	MOTION_TOOL_MAX			20
	#define	MOTION_JOB_MAX			12


	struct smMOTIONINFO {
		DWORD	State;							//상태 TRUE면 유효
		DWORD	StartFrame;						//시작 프레임
		DWORD	EndFrame;						//종료 프레임
		DWORD	EventFrame[4];					//이벤트 생성 프레임

		int		ItemCodeCount;					//해당 아이템 리스트 카운터 ( 0 없음  -1 전체 해당 )
		DWORD	ItemCodeList[MOTION_TOOL_MAX];	//해당 아이템 코드 리스트
		BYTE	JobCodeList[MOTION_JOB_MAX];	//해당 직업 코드 리스트

		int		MapPosition;					//해당 맵 적용 ( 0-관계없음 마을 1 - 필드 2 )

		DWORD	Repeat;							//반복 여부
		CHAR	KeyCode;						//동작 시작 키
		int		MotionFrame;					//모션 연결 파일 번호
	};
	*/

	if (FindCnt2 == 0) {
		if (FindCnt == 1)
			ChangeMotion(FoundNum[0], AnimDispMode);
		else
			if (FindCnt) ChangeMotion(FoundNum[(rand() % FindCnt)], AnimDispMode);

		if (atSkill) AttackSkil = atSkill;

		return NULL;
	}

	//FoundNum2[0] = 115;FindCnt2=1;

	if (FindCnt2 == 1) ChangeMotion(FoundNum2[0], AnimDispMode);
	else
		if (FindCnt2) ChangeMotion(FoundNum2[(rand() % FindCnt2)], AnimDispMode);


	if (atSkill) AttackSkil = atSkill;

	return FindCnt;
}


//프레임 번호로 동작을 찾는다
int smCHAR::FindActionFromFrame(int sframe)
{
	DWORD cnt;
	DWORD compFrame;

	compFrame = sframe / 160;

	for (cnt = 0; cnt < smMotionInfo->MotionCount; cnt++) {
		if (smMotionInfo->MotionInfo[cnt].StartFrame<compFrame &&
			smMotionInfo->MotionInfo[cnt].EndFrame>compFrame)
			return cnt;
	}

	return -1;
}

int smCHAR::SavePlayBuff()
{

	PlayBuffCnt = (PlayBuffCnt + 1) & PLAYBUFF_MASK;

	PlayBuff[PlayBuffCnt].angle[0] = Angle.x;
	PlayBuff[PlayBuffCnt].angle[1] = Angle.y;
	PlayBuff[PlayBuffCnt].angle[2] = Angle.z;
	PlayBuff[PlayBuffCnt].angle[3] = action;

	PlayBuff[PlayBuffCnt].x = pX;
	PlayBuff[PlayBuffCnt].y = pY;
	PlayBuff[PlayBuffCnt].z = pZ;

	PlayBuff[PlayBuffCnt].frame = (MotionSelectFrame << CHAR_FRAME_SELSHIFT) | frame;

	if (chrAttackTarget)
		PlayBuff[PlayBuffCnt].dwTargetSerial = chrAttackTarget->dwObjectSerial;
	else
		PlayBuff[PlayBuffCnt].dwTargetSerial = NULL;

	memcpy(PlayBuff[PlayBuffCnt].wStickItems, wStickItems, sizeof(short) * 4);

	return PlayBuffCnt;
}

//몬스터 및 자동캐릭터용 프레임 저장 (서버용)
int smCHAR::SavePlayBuff2()
{

	PlayBuffCnt = (PlayBuffCnt + 1) & PLAYBUFF_MASK;

	PlayBuff[PlayBuffCnt].angle[0] = Angle.x;
	PlayBuff[PlayBuffCnt].angle[1] = Angle.y;
	PlayBuff[PlayBuffCnt].angle[2] = Angle.z;
	PlayBuff[PlayBuffCnt].angle[3] = action;

	PlayBuff[PlayBuffCnt].x = pX;
	PlayBuff[PlayBuffCnt].y = pY;
	PlayBuff[PlayBuffCnt].z = pZ;

	PlayBuff[PlayBuffCnt].frame = (MotionSelectFrame << CHAR_FRAME_SELSHIFT) | frame;

	PlayBuff[PlayBuffCnt].dwTargetSerial = NULL;

	if (lpTargetChar)
		PlayBuff[PlayBuffCnt].dwTargetSerial = lpTargetChar->dwObjectSerial;

	if (lpTargetPlayInfo)
		PlayBuff[PlayBuffCnt].dwTargetSerial = lpTargetPlayInfo->dwObjectSerial;

	memcpy(PlayBuff[PlayBuffCnt].wStickItems, wStickItems, sizeof(short) * 4);

	return PlayBuffCnt;
}



//이동 모드 변경
int smCHAR::ChangeMoveMode()
{
	//CHRMOTION_STATE_RUN
	/*
								if ( !SetMotionFromCode( CHRMOTION_STATE_SOMETIME ) ) {
									Angle.y = (cnt<<4)&ANGCLIP;
									ChangeMotion( CHRMOTION_WALK );
								}
							}
							else {
								if ( !SetMotionFromCode( TRUE ) ) {
									Angle.y = (cnt<<4)&ANGCLIP;
									ChangeMotion( CHRMOTION_WALK );
	*/
	if (MoveMode == 0) MoveMode = 1;
	else MoveMode = 0;

	if (MoveFlag) {
		if (MoveMode)
			SetMotionFromCode(CHRMOTION_STATE_RUN);
		else
			SetMotionFromCode(CHRMOTION_STATE_WALK);
	}

	return MoveMode;
}

//공격받은 시간 검사하여 원거리 공격 탐지
int smCHAR::CheckShootingTest_LastAttackTime(smCHAR* lpChar)
{
	if (dwLastRecvAttackTime<dwPlayTime && (dwLastRecvAttackTime + 2000)>dwPlayTime)
		return TRUE;

	return CheckShootingTest(lpChar);
}

//발사형 무기 메인함수
int smCHAR::CheckShootingTest(smCHAR* lpChrTarget)
{
	//	POINT3D Posi;
	int x, y, z;
	int he;//,hy;
	int step;
	int cnt;

	int angX, angY;
	int	SucessCnt;

	if (lpChrTarget->smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_TOWER_B) 	//발할라타워 제외
		return TRUE;

	GetRadian3D(pX, pY, pZ, lpChrTarget->pX, lpChrTarget->pY, lpChrTarget->pZ);
	angX = GeoResult_X;
	angY = GeoResult_Y;

	ShootingPosi.x = pX;
	ShootingPosi.y = pY + 34 * fONE;
	ShootingPosi.z = pZ;

	ShootingAngle.x = angX;
	ShootingAngle.y = angY;
	ShootingAngle.z = 0;

	SucessCnt = 0;

	step = 16 * fONE;

	GetMoveLocation(0, 0, step, angX, angY, 0);

	for (cnt = 0; cnt < 40; cnt++) {
		ShootingPosi.x += GeoResult_X;
		ShootingPosi.y += GeoResult_Y;
		ShootingPosi.z += GeoResult_Z;

		he = TRUE;

		//지정된 방향의 거리를 잇는 선분에 폴리건이 충돌하는지 확인 

		if (lpStage) {
			he = lpStage->CheckVecImpact(&ShootingPosi, &ShootingAngle, step);
		}
		else {
			if (smGameStage[0]) {
				he = smGameStage[0]->CheckVecImpact(&ShootingPosi, &ShootingAngle, step);
			}

			if (he == TRUE && smGameStage[1]) {
				he = smGameStage[1]->CheckVecImpact(&ShootingPosi, &ShootingAngle, step);
			}
		}

		if (chrAttackTarget) {

			x = abs(ShootingPosi.x - lpChrTarget->pX);
			z = abs(ShootingPosi.z - lpChrTarget->pZ);
			y = abs(ShootingPosi.y - lpChrTarget->pY);

			if (x < 48 * fONE && z < 48 * fONE && y < 64 * fONE && !SucessCnt) {
				//return TRUE;
				SucessCnt = cnt + 2;
			}
		}

		if (SucessCnt && SucessCnt < cnt)
			return TRUE;


		if (he == FALSE)
			return FALSE;


	}

	return TRUE;


}



//발사형 무기 메인함수
int smCHAR::ShootingMain()
{

	POINT3D Posi;
	int x, y, z;
	int he;//,hy;
	int ShootSize;
	int step;
	int r, g, b;
	int EffectFlag;

	he = CLIP_OUT;
	EffectFlag = 0;

	ShootSize = 512;

	//	step = 1*fONE;
	if ((dwActionItemCode & sinITEM_MASK2) == sinWS1)
		step = 8 * fONE;				//화살
	else
		step = 6 * fONE;				//창이나 기타 던지는 무기

	Posi.x = x = ShootingPosi.x;
	Posi.y = y = ShootingPosi.y;
	Posi.z = z = ShootingPosi.z;

	GetMoveLocation(0, 0, step, ShootingAngle.x, ShootingAngle.y, ShootingAngle.z);
	ShootingPosi.x += GeoResult_X;
	ShootingPosi.y += GeoResult_Y;
	ShootingPosi.z += GeoResult_Z;

	if (smCharInfo.State == smCHAR_STATE_USER) {

		if (dwActionItemCode == HvRightHand.dwItemCode) {
			if (HvRightHand.PatTool && HvRightHand.ColorBlink && !HvRightHand.EffectKind) {

				r = (HvRightHand.sColors[SMC_R] >> 1) + 128;
				g = (HvRightHand.sColors[SMC_G] >> 1) + 128;
				b = (HvRightHand.sColors[SMC_B] >> 1) + 128;
				EffectFlag++;
			}
		}
		if (dwActionItemCode == HvLeftHand.dwItemCode) {
			if (HvLeftHand.PatTool && HvLeftHand.ColorBlink && !HvLeftHand.EffectKind) {

				r = (HvLeftHand.sColors[SMC_R] >> 1) + 128;
				g = (HvLeftHand.sColors[SMC_G] >> 1) + 128;
				b = (HvLeftHand.sColors[SMC_B] >> 1) + 128;
				EffectFlag++;
			}
		}
	}

	//	y += fONE* ;
	/*
			he = TRUE;

			//지정된 방향의 거리를 잇는 선분에 폴리건이 충돌하는지 확인

			if ( lpStage ) {
				he = lpStage->CheckVecImpact( &ShootingPosi, &ShootingAngle , step );
			}
			else {
				if ( smGameStage[0] ) {
					he = smGameStage[0]->CheckVecImpact( &ShootingPosi, &ShootingAngle , step );
				}

				if ( he==TRUE && smGameStage[1] ) {
					he = smGameStage[1]->CheckVecImpact( &ShootingPosi, &ShootingAngle , step );
				}
			}
	*/
	if (ShootingCount > 60 * 2) { //|| he==FALSE  ) {
		ShootingFlag = FALSE;
	}

	ShootingCount++;

	if (ShootingFlag > 1 && chrAttackTarget) {
		ShootingFlag++;
		if (ShootingFlag > 8) ShootingFlag = FALSE;
		return TRUE;
	}

	if (ShootingFlag == 1 && chrAttackTarget && AttackCritcal >= 0) {

		x = abs(ShootingPosi.x - chrAttackTarget->pX);
		z = abs(ShootingPosi.z - chrAttackTarget->pZ);
		y = abs(ShootingPosi.y - chrAttackTarget->pY);

		if (x < 64 * fONE && z < 64 * fONE && y < 80 * fONE) {
			ShootingFlag++;


			if (ShootingKind == SKILL_PLAY_FARINA) {
				//파리나 이팩트
				Posi.x = chrAttackTarget->pX;
				Posi.y = chrAttackTarget->pY + 22 * fONE;
				Posi.z = chrAttackTarget->pZ;

			}
			//공격소리
			//PlayWav( 9 , 0 , 300 , 2205 );

			//경험치 일부 상승
			if (this == lpCurPlayer && AttackExp > 0) {
				//AddExp( AttackExp );
				//SetTotalAddExp( AttackExp );			//경험치 증가 기록
				AttackExp = 0;
			}

		}

	}

	return TRUE;
}

//스킬 차징
int smCHAR::ChargingSkill()
{
	DWORD dwFrame;

	if (this != lpCurPlayer) return FALSE;

	dwFrame = frame - MotionInfo->StartFrame * 160;

	if (SkillChargingFlag) SkillChargingFlag = 0;

	switch (AttackSkil & 0xFF) {
	case SKILL_PLAY_PHOENIX_SHOT:
		if (dwFrame >= MotionInfo->EventFrame[0] && dwFrame < MotionInfo->EventFrame[1]) {
			if (SkillChargingFlag == 0) {
				frame = MotionInfo->StartFrame * 160 + MotionInfo->EventFrame[1];
				ChargingFlag = 1;
			}
		}
		break;
	case SKILL_PLAY_CHARGING_STRIKE:
		if (dwFrame >= MotionInfo->EventFrame[0] && dwFrame < MotionInfo->EventFrame[1]) {
			if (SkillChargingFlag == 0) {
				frame = MotionInfo->StartFrame * 160 + MotionInfo->EventFrame[1];
				ChargingFlag = 1;
			}
		}
		break;
	}

	return TRUE;
}

//공격 이벤트
int smCHAR::EventAttack(int Flag)
{

	int shootFlag;
	DWORD compFrame;
	int x, y, z;
	int wp;
	DWORD NearFrame;
	POINT3D	pos, pos2;

	int	cnt, ChrCnt;
	smCHAR* lpCharList[32];

	//	if ( MotionInfo->State<0x100 ) return FALSE;

	ChargingSkill();

	if (AttackSkil > 0 || (smCharInfo.State == smCHAR_STATE_ENEMY && MotionInfo->State == CHRMOTION_STATE_SKILL)) {
		if (Flag || ((MotionInfo->State == CHRMOTION_STATE_ATTACK || MotionInfo->State == CHRMOTION_STATE_SKILL) && MotionInfo->EventFrame[0])) {
			compFrame = frame - (MotionInfo->StartFrame * 160);
			//if ( MotionInfo->EventFrame[0]>(compFrame-FrameStep) && MotionInfo->EventFrame[0]<=compFrame ) {
			if ((MotionInfo->EventFrame[0] > (compFrame - FrameStep) && MotionInfo->EventFrame[0] <= compFrame) ||
				(MotionInfo->EventFrame[1] && MotionInfo->EventFrame[1] > (compFrame - FrameStep) && MotionInfo->EventFrame[1] <= compFrame) ||
				(MotionInfo->EventFrame[2] && MotionInfo->EventFrame[2] > (compFrame - FrameStep) && MotionInfo->EventFrame[2] <= compFrame) ||
				(MotionInfo->EventFrame[3] && MotionInfo->EventFrame[3] > (compFrame - FrameStep) && MotionInfo->EventFrame[3] <= compFrame) ||

				(dwEventFrameExt[0] && dwEventFrameExt[0] > (frame - FrameStep) && dwEventFrameExt[0] <= frame) ||
				(dwEventFrameExt[1] && dwEventFrameExt[1] > (frame - FrameStep) && dwEventFrameExt[1] <= frame) ||
				(dwEventFrameExt[2] && dwEventFrameExt[2] > (frame - FrameStep) && dwEventFrameExt[2] <= frame) ||
				(dwEventFrameExt[3] && dwEventFrameExt[3] > (frame - FrameStep) && dwEventFrameExt[3] <= frame) ||

				(MotionEvent == 0 && MotionInfo->EventFrame[0] <= compFrame)) {

				//스킬이벤트
				MotionEvent++;

				//경험치 일부 상승 ( 주인공 만 )
				if (this == lpCurPlayer) {//&& AttackExp>0 ) { 
					//AddExp( AttackExp );
					//SetTotalAddExp( AttackExp );			//경험치 증가 기록
					AttackExp = 0;

					if (chrAttackTarget && abs(chrAttackTarget->smCharInfo.Level - smCharInfo.Level) <= AGING_SUB_LEVEL) {
						//레벨 7차이 이하는 에이징 적용
						if (AttackCritcal == TRUE)
							sinCheckAgingLevel(SIN_AGING_CRITICAL, false);
						else
							sinCheckAgingLevel(SIN_AGING_ATTACK, false);
					}
				}

				if (EventSkill() == TRUE)
					return TRUE;
			}
		}
	}


	if (MotionInfo->State != CHRMOTION_STATE_ATTACK && MotionInfo->State != CHRMOTION_STATE_SKILL) return FALSE;

	if (ActionPattern)
		shootFlag = 0;
	else {
		shootFlag = ShootingFlag;
	}

	if (MotionInfo->EventFrame[0]) {
		compFrame = frame - (MotionInfo->StartFrame * 160);
		NearFrame = compFrame - FrameStep;

		if (Flag || (MotionInfo->EventFrame[0] > NearFrame && MotionInfo->EventFrame[0] <= compFrame) ||
			(MotionInfo->EventFrame[1] && MotionInfo->EventFrame[1] > NearFrame && MotionInfo->EventFrame[1] <= compFrame)) {

			MotionEvent++;

			//경험치 일부 상승 ( 주인공 만 )
			if (this == lpCurPlayer && AttackExp > 0) {

				//AttackExp = 500000;

				//AddExp( AttackExp );
				//SetTotalAddExp( AttackExp );			//경험치 증가 기록
				AttackExp = 0;

				//SaveGameData();					

				if (chrAttackTarget && abs(chrAttackTarget->smCharInfo.Level - smCharInfo.Level) <= AGING_SUB_LEVEL) {
					//레벨 7차이 이하는 에이징 적용
					if (AttackCritcal == TRUE)
						sinCheckAgingLevel(SIN_AGING_CRITICAL, false);
					else
						sinCheckAgingLevel(SIN_AGING_ATTACK, false);
				}

			}

			//주인공을 향한 공격
			if (chrAttackTarget == lpCurPlayer) {
				//공격 받은 모션 카운터
				AttackMotionCountUp();
			}

			switch (smCharInfo.dwCharSoundCode) {
			case snCHAR_SOUND_MUSHROOM:
				//버섯 독가스 발사
				break;

			case snCHAR_SOUND_IMP:
				//임프 매핏
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget) ||
						CheckLocateBox(pX, pY, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 * fONE)) {

						GetMoveLocation(0, 0, 10 * fONE, 0, Angle.y, 0);
						x = pX + GeoResult_X;
						y = pY + 8 * fONE;
						z = pZ + GeoResult_Z;

					}
					else {
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_SHADOW:
				//쉐도우 공격
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget) ||
						CheckLocateBox(pX, pY, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 * fONE)) {

					}
					else {
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_FIGON:
				//파이곤 공격
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget) ||
						CheckLocateBox(pX, pY, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 * fONE)) {

					}
					else {
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_WEB:
				//눈튀어나온 거미
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget) ||
						CheckLocateBox(pX, pY, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 * fONE)) {

						GetMoveLocation(0, 0, 32 * fONE, 0, Angle.y, 0);
					}
					else {
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_SKELETONRANGE:
				//활쏘는 해골
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget)) {
						ShootingPosi.x = pX;
						ShootingPosi.y = pY + 28 * fONE;
						ShootingPosi.z = pZ;
						GetRadian3D(pX, pY + 28 * fONE, pZ, chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ);
						ShootingAngle.x = GeoResult_X;
						ShootingAngle.y = GeoResult_Y;
						ShootingAngle.z = Angle.z;
						ShootingFlag = TRUE;
						ShootingCount = 0;
						dwActionItemCode = sinWS1;
					}
					else {
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_WOLVERLIN:
				//을버린
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 50 * fONE, 0, Angle.y, 0);
					x = pX + GeoResult_X;
					z = pZ + GeoResult_Z;
					y = pY + 18 * fONE;

				}
				break;

			case snCHAR_SOUND_EGAN:
				//이간
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget)) {
						GetMoveLocation(0, 0, 10 * fONE, 0, Angle.y, 0);
						x = pX + GeoResult_X;
						y = pY + 8 * fONE;
						z = pZ + GeoResult_Z;

					}
					else {
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_MUTANTTREE:
				//뮤턴트 트리
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget)) {
						GetMoveLocation(0, 0, 24 * fONE, 0, Angle.y, 0);
						x = pX + GeoResult_X;
						y = pY + 8 * fONE;
						z = pZ + GeoResult_Z;


					}
					else {
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_HULK:
				//헐크
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 54 * fONE, 0, Angle.y, 0);
				}
				break;


			case snCHAR_SOUND_DMACHINE:
				//디머신
				if (chrAttackTarget) {
					GetMoveLocation(0, 30 * fONE, 10 * fONE, 0, Angle.y, 0);

				}
				break;

			case snCHAR_SOUND_SUCCUBUS:
				//써큐버스
				if (chrAttackTarget)
				{

					GetMoveLocation(0, 20 * fONE, 20 * fONE, 0, Angle.y, 0);

				}
				break;

			case snCHAR_SOUND_STYGIANLORD:
				//스티지언로드
				if (chrAttackTarget) {
					//GetMoveLocation( 0 , 20*fONE, 20*fONE , 0, Angle.y , 0 );
					//StartEffectMonsterDest(pX+GeoResult_X, pY+GeoResult_Y,pZ+GeoResult_Z, 

				}
				break;

			case snCHAR_SOUND_MOKOVA:				//모코바
			case snCHAR_SOUND_OMICRON:
				//헐크 꺼랑 동일
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 68 * fONE, 0, Angle.y, 0);
				}
				break;

			case snCHAR_SOUND_GUARDIAN_SAINT:
				//가디안 - 세인트
				break;

			case snCHAR_SOUND_HYPERMACHINE:		//하이퍼 머신
				if (chrAttackTarget) {
					pos.x = chrAttackTarget->pX;
					pos.y = chrAttackTarget->pY + 20 * fONE;
					pos.z = chrAttackTarget->pZ;
				}
				break;

			case snCHAR_SOUND_MOUNTAIN:			//마운틴
				break;

			case snCHAR_SOUND_TOWERGOLEM:
				break;

			case snCHAR_SOUND_OMU:
				break;

			case snCHAR_SOUND_CHAINGOLEM:
			case snCHAR_SOUND_DEADZONE:
			case snCHAR_SOUND_IRONFIST:
			case snCHAR_SOUND_MORGON:
			case snCHAR_SOUND_RAMPAGE:
			case snCHAR_SOUND_VAMPRICBEE:
			case snCHAR_SOUND_IRONGUARD:
			case snCHAR_SOUND_GORGON:	// pluto 추가 몬스터 고르곤
			case snCHAR_SOUND_HOBOGOLEM: // pluto 추가 몬스터 호보고렘

				GetMoveLocation(0, 0, 54 * fONE, 0, Angle.y, 0);
				pos.x = pX + GeoResult_X;
				pos.y = pY + GeoResult_Y + 32 * fONE;
				pos.z = pZ + GeoResult_Z;
				break;

			case snCHAR_SOUND_GOBLINSHAMAN: // 박재원 - 저주받은 신전 3층(신규필드 몬스터) - 고블린샤먼

				break;

			case snCHAR_SOUND_GROTESQUE:
				GetMoveLocation(0, 0, 75 * fONE, 0, Angle.y, 0);
				pos.x = pX + GeoResult_X;
				pos.y = pY + GeoResult_Y + 32 * fONE;
				pos.z = pZ + GeoResult_Z;
				break;


			case snCHAR_SOUND_AVELIN_ARCHER:

				break;

			case snCHAR_SOUND_AVELIN_QUEEN:

				break;

			case snCHAR_SOUND_RUNICGUARDIAN:

				break;

			case snCHAR_SOUND_BABEL:				//바벨 (루닉가디안 보스)

				if (chrAttackTarget) {
					if (Trans_SplashCharList.code == smTRANSCODE_ATTACK_SLASH_LIST && Trans_SplashCharList.TargetCount > 1) {
						ChrCnt = 0;
						for (cnt = 0; cnt < Trans_SplashCharList.TargetCount; cnt++) {
							if (cnt >= 32) break;
							lpCharList[ChrCnt] = FindChrPlayer(Trans_SplashCharList.dwTarObjectSerial[cnt]);
							if (lpCharList[ChrCnt]) ChrCnt++;
						}
						Trans_SplashCharList.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_SADNESS:			//새드니스

				break;

				// 장별 - 수박몬스터 일반공격동작
			case snCHAR_SOUND_WATERMELON:
				if (chrAttackTarget)
				{
					GetMoveLocation(0, 0, 54 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 32 * fONE;
					pos.z = pZ + GeoResult_Z;
				}
				break;


				////////////////// 아이스 몬스터 /////////////////////
			case snCHAR_SOUND_COLDEYE:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 24 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + 64 * fONE;
					pos.z = pZ + GeoResult_Z;
					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 64 * fONE;
					pos2.z = chrAttackTarget->pZ;
				}
				break;

			case snCHAR_SOUND_FROZEN:

				break;

			case snCHAR_SOUND_FROST:

				break;

			case snCHAR_SOUND_CHAOSCARA:

				break;

			case snCHAR_SOUND_DEATHKNIGHT:			//발렌토

				break;


				////////////////// 그리디호수 몬스터 /////////////////////
			case snCHAR_SOUND_GREATE_GREVEN:
			case snCHAR_SOUND_LIZARDFOLK:
			case snCHAR_SOUND_SPIDER:
			case snCHAR_SOUND_STRIDER:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 68 * fONE, 0, Angle.y, 0);
				}
				break;

			case snCHAR_SOUND_M_LORD:

				break;


				////////////////// // 박재원 - 끝없는 탑 3층(신규필드 몬스터) /////////////////////
			case snCHAR_SOUND_IGOLATION: // 이골라티온 : 일반 공격
				if (chrAttackTarget && MotionInfo->KeyCode == 'H')
				{
					GetMoveLocation(0, 0, 68 * fONE, 0, Angle.y, 0);
				}
				break;
			case snCHAR_SOUND_NIKEN:  // 니켄 : 일반 공격
				if (chrAttackTarget)
				{
					pos.x = chrAttackTarget->pX;
					pos.y = chrAttackTarget->pY;
					pos.z = chrAttackTarget->pZ;

				}
				break;
			case snCHAR_SOUND_XETAN: // 제탄 : 일반 공격
				if (chrAttackTarget && MotionInfo->KeyCode == 'G')
				{
					GetMoveLocation(0, 40 * fONE, 20 * fONE, 0, Angle.y, 0);

				}
				break;
			case snCHAR_SOUND_SPRIN: // 스프린 : 일반 공격
				if (chrAttackTarget && (MotionInfo->KeyCode == 'G' || MotionInfo->KeyCode == 'H'))
				{
					if (CheckShootingTest_LastAttackTime(chrAttackTarget) ||
						CheckLocateBox(pX, pY, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 * fONE)) {

						GetMoveLocation(0, 0, 32 * fONE, 0, Angle.y, 0);

					}
					else {
						ResetAttackTrans();
					}
				}
				break;


				////////////////// // 박재원 - 복날 이벤트 몬스터 추가 /////////////////////

			case snCHAR_SOUND_BEBECHICK:	// 아기 꼬꼬
				if (chrAttackTarget && (MotionInfo->KeyCode == 'H' || MotionInfo->KeyCode == 'J'))
				{
					//  이펙트 정의 안함.(모션만 설정)
				}
				break;
			case snCHAR_SOUND_PAPACHICK:	// 아빠 꼬꼬
				if (chrAttackTarget && MotionInfo->KeyCode == 'H')
				{
					//  이펙트 정의 안함.(모션만 설정)
				}
				break;

				/////////////////////////////////////////////////////////////////////////////////////

			case snCHAR_SOUND_KELVEZU:

				break;

				////////////////////////////////////////////////////
/*
					if (chrAttackTarget) {
						GetMoveLocation( 0 , 0, 54*fONE , 0, Angle.y , 0 );
						StartEffectMonster( pX+GeoResult_X, pY+48*fONE,pZ+GeoResult_Z, MONSTER_HULK_HIT1);
					}
*/
			case snCHAR_SOUND_DARKPHALANX:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 70 * fONE, 0, Angle.y, 0);
				}
				break;
			case snCHAR_SOUND_BLOODYKNIGHT:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 70 * fONE, 0, Angle.y, 0);
				}
				break;

			case snCHAR_SOUND_CHIMERA:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 70 * fONE, 0, Angle.y, 0);
				}
				break;

			case snCHAR_SOUND_FIREWORM:

				break;
			case snCHAR_SOUND_HELLHOUND:
				if (chrAttackTarget) {

					GetMoveLocation(0, 0, 70 * fONE, 0, Angle.y, 0);
				}
				break;
			case snCHAR_SOUND_DARKMAGE:

				break;
			case snCHAR_SOUND_DARKGUARD:
				//활쏘는 다크가드
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget)) {
						ShootingPosi.x = pX;
						ShootingPosi.y = pY + 38 * fONE;
						ShootingPosi.z = pZ;
						GetRadian3D(pX, pY + 38 * fONE, pZ, chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ);
						ShootingAngle.x = GeoResult_X;
						ShootingAngle.y = GeoResult_Y;
						ShootingAngle.z = Angle.z;
						ShootingFlag = TRUE;
						ShootingCount = 0;
						dwActionItemCode = sinWS1;
					}
					else {
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_TEMPLEGUARD:
				if (MotionInfo->KeyCode != 'I') {
				}
				else {
					GetMoveLocation(0, 0, 54 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 32 * fONE;
					pos.z = pZ + GeoResult_Z;
				}
				break;

			case snCHAR_SOUND_SETO:
				if (chrAttackTarget) {
					GetMoveLocation(0, 24 * fONE, 24 * fONE, 0, Angle.y, 0);
				}
				break;

			case snCHAR_SOUND_KINGSPIDER:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 70 * fONE, 0, Angle.y, 0);
				}
				break;

				////////////////////////////////////////////////////

			case snCHAR_SOUND_NPC_MORIF:		//믹스장인 모리프
				if (chrAttackTarget) {

					GetMoveLocation(0, 0, 25 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 32 * fONE;
					pos.z = pZ + GeoResult_Z;

					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;

				}
				break;

			case snCHAR_SOUND_NPC_MOLLYWOLF:		//송편훔친늑대
				if (chrAttackTarget) {

					GetMoveLocation(0, 0, 25 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 32 * fONE;
					pos.z = pZ + GeoResult_Z;

					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;

				}
				break;

			case snCHAR_SOUND_WORLDCUP:				//월드컵 몬스터

				break;

			case snCHAR_SOUND_S_F_ELEMENTAL:		//소환몹 화이어엘리멘탈
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 25 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 32 * fONE;
					pos.z = pZ + GeoResult_Z;

					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;

				}
				break;

			case snCHAR_SOUND_S_METALGOLEM:		//소환몹 메탈고렘
				if (chrAttackTarget) {
					if (!smCharInfo.wPlayClass[1])
						GetMoveLocation(0, 0, 54 * fONE, 0, Angle.y, 0);
					else
						GetMoveLocation(0, 0, 54 * smCharInfo.wPlayClass[1], 0, Angle.y, 0);

				}
				break;

			case snCHAR_SOUND_S_WOLVERLIN:		//소환몹 울버린
				if (chrAttackTarget) {
					if (!smCharInfo.wPlayClass[1])
						GetMoveLocation(0, 0, 50 * fONE, 0, Angle.y, 0);
					else
						GetMoveLocation(0, 0, 50 * smCharInfo.wPlayClass[1], 0, Angle.y, 0);

					x = pX + GeoResult_X;
					z = pZ + GeoResult_Z;
					y = pY + 18 * fONE;
				}
				break;

			case snCHAR_SOUND_NPC_SKILLMASTER:
				break;

			case snCHAR_SOUND_NPC_MAGICMASTER:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 25 * fONE, 0, Angle.y, 0);

				}
				break;

			case snCHAR_SOUND_CASTLE_CRYSTAL_R:	//빨간 크리스탈 타워
				if (chrAttackTarget) {
					y = GetRadian2D(pX, pZ, chrAttackTarget->pX, chrAttackTarget->pZ);
					GetMoveLocation(0, 0, 16 * fONE, 0, y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 136 * fONE;
					pos.z = pZ + GeoResult_Z;
					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;


				}
				break;

			case snCHAR_SOUND_CASTLE_CRYSTAL_G:	//그린 크리스탈 타워
				if (chrAttackTarget) {
					y = GetRadian2D(pX, pZ, chrAttackTarget->pX, chrAttackTarget->pZ);
					GetMoveLocation(0, 0, 16 * fONE, 0, y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 136 * fONE;
					pos.z = pZ + GeoResult_Z;
					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;
				}
				break;

			case snCHAR_SOUND_CASTLE_CRYSTAL_B:	//파랑 크리스탈 타워
				if (chrAttackTarget) {
					y = GetRadian2D(pX, pZ, chrAttackTarget->pX, chrAttackTarget->pZ);
					GetMoveLocation(0, 0, 16 * fONE, 0, y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 136 * fONE;
					pos.z = pZ + GeoResult_Z;
					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;

				}
				break;

			case snCHAR_SOUND_CASTLE_SOLDER_A:
			case snCHAR_SOUND_CASTLE_SOLDER_B:
			case snCHAR_SOUND_CASTLE_SOLDER_C:

				break;
			case snCHAR_SOUND_REVIVED_ARCHER:
				if (chrAttackTarget)
				{
					if (CheckShootingTest_LastAttackTime(chrAttackTarget))
					{
						ShootingPosi.x = pX;
						ShootingPosi.y = pY + 38 * fONE;
						ShootingPosi.z = pZ;
						GetRadian3D(pX, pY + 38 * fONE, pZ, chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ);
						ShootingAngle.x = GeoResult_X;
						ShootingAngle.y = GeoResult_Y;
						ShootingAngle.z = Angle.z;
						ShootingFlag = TRUE;
						ShootingCount = 0;
						dwActionItemCode = sinWS1;
					}
					else
					{
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			}

			if (this != lpCurPlayer) {
				PlayAttack();									//공격 적용
			}
			if (smCharInfo.State != smCHAR_STATE_USER && PkMode_CharState != smCHAR_STATE_USER) return TRUE;

			if (!ShootingMode) {
				if (GetAttackPoint(&x, &y, &z) == TRUE) {
					if (WeaponEffect) {
						pos.x = x;
						pos.y = y;
						pos.z = z;
					}
					else
					{



						if (dwForceOrbTime > dwPlayTime)
						{		//포스오브 공격 이펙트
							pos.x = x;
							pos.y = y;
							pos.z = z;

						}
					}
				}
			}
			else {
				//발사형 공격
				wp = (dwActionItemCode & sinITEM_MASK2);
				if (!shootFlag && chrAttackTarget) {
					GetAttackPoint(&x, &y, &z);

					if (wp == sinWS1 || wp == sinWT1) {


						if (smCharInfo.JOB_CODE == JOBCODE_ATALANTA &&
							(dwDispAppSkill & SKILL_APPMASK_GODLY_SHIELD) && wp == sinWT1) {
							//프로스트 제블린 스킬 이펙트 (아탈란타)
							pos.x = x;
							pos.y = y;
							pos.z = z;
							pos2.x = chrAttackTarget->pX;
							pos2.y = chrAttackTarget->pY + 24 * fONE;
							pos2.z = chrAttackTarget->pZ;



							return TRUE;
						}

						ShootingPosi.x = x;
						ShootingPosi.y = y;
						ShootingPosi.z = z;

						if (chrAttackTarget) {
							GetRadian3D(x, y, z, chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ);
							ShootingAngle.x = GeoResult_X;
							ShootingAngle.y = GeoResult_Y;
							ShootingAngle.z = Angle.z;
						}
						else {
							ShootingAngle.x = Angle.x;
							ShootingAngle.y = Angle.y;
							ShootingAngle.z = Angle.z;
						}

						ShootingCount = 0;
						ShootingFlag = TRUE;

						if (AttackSkil)
							ShootingKind = AttackSkil & 0xFF;
						else
							ShootingKind = TRUE;

					}

					if (wp == sinWM1) {
						//마법 지팡이

						if (WeaponEffect) {
							switch (WeaponEffect) {
							case (sITEMINFO_LIGHTING + 1):
								pos.x = x;
								pos.y = y;
								pos.z = z;
								pos2.x = chrAttackTarget->pX;
								pos2.y = chrAttackTarget->pY + 24 * fONE;
								pos2.z = chrAttackTarget->pZ;
								break;
							case (sITEMINFO_FIRE + 1):
								pos.x = x;
								pos.y = y;
								pos.z = z;
								pos2.x = chrAttackTarget->pX;
								pos2.y = chrAttackTarget->pY + 24 * fONE;
								pos2.z = chrAttackTarget->pZ;
								break;
							case (sITEMINFO_ICE + 1):
								pos.x = x;
								pos.y = y;
								pos.z = z;
								pos2.x = chrAttackTarget->pX;
								pos2.y = chrAttackTarget->pY + 24 * fONE;
								pos2.z = chrAttackTarget->pZ;

								break;
							}
						}
					}
				}
			}
		}
	}


	return TRUE;

}

extern int MatLight[2];

//손에 효과 주기 ( 파이곤 용 )
int smCHAR::StartHandEffect(int Mode) { return TRUE; }

#define SERVER_CHAR_FRAME_STEP			320
#define SERVER_CHAR_SKIP				4

int smCHAR::Main()
{

	int x, z, y;
	int cnt;
	DWORD compFrame;
	smPLAYBUFF* playBuff;
	int OldAction;
	int dist;
	//	int FallMode;
	POINT3D OldPos;
	//	int step1,step2;
	int	frameStep, moveStep;
	smTRANS_COMMAND	smTransCommand;
	smCHAR* lpChar;
	int EvtFrame;
	int dm1, dm2;
	int UserSlowSpeed = 128;

	int countCaravan = 0;

	if (!Pattern || PatLoading || !MotionInfo) {
		//PlayBuffCnt++;
		return FALSE;
	}

	//모델데이타 삭제된 오류
	if (lpDinaPattern->UseCount <= 0) {
		Close();
		if (smConfig.DebugMode) AddChatBuff("모델제거 오류");
		return FALSE;
	}


	switch (ActionPattern) {
	case 0:
	case 1:	//펭귄
		break;

	case 5:	//몬스터

		if (PlaySlowCount) {
			if (PlaySlowSpeed) {
				moveStep = (SERVER_CHAR_SKIP * PlaySlowSpeed) >> 8;
				frameStep = (SERVER_CHAR_FRAME_STEP * PlaySlowSpeed) >> 8;
			}
			else {
				moveStep = SERVER_CHAR_SKIP >> 1;
				frameStep = SERVER_CHAR_FRAME_STEP >> 1;
			}
		}
		else {
			moveStep = SERVER_CHAR_SKIP;
			frameStep = SERVER_CHAR_FRAME_STEP;
		}

		if (MotionInfo->State == CHRMOTION_STATE_WARP) {
			//워프 동작
			frame += frameStep;

			if (frame > MotionInfo->EndFrame * 160) {
				Close();
				return TRUE;
			}
			SavePlayBuff2();
			FrameCounter += 3;
			break;
		}

		//뱀피릭 머신 움직이지 않도록
		//if ( smCharInfo.dwCharSoundCode==snCHAR_SOUND_VAMPRICMACHINE || smCharInfo.dwCharSoundCode==snCHAR_SOUND_CASTLE_DOOR )
		if (DontMoveFlag)	moveStep = 0;


		//바벨 출현 유지중
		if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL) {
			if (smCharInfo.Life[0] > 0)	dwBabelPlayTime = dwPlayServTime + 5000;
		}


		if (MotionInfo->State && MotionInfo->State < 0x100 && moveStep)
		{
			if ((MotionInfo->State == CHRMOTION_STATE_WALK || MotionInfo->State == CHRMOTION_STATE_RUN)) //&& ((!smCharInfo.lpMonInfo->Caravana) || smCharInfo.lpMonInfo->Caravana && smCharInfo.lpMonInfo->CaravanMapNum != 33))
			{
				OldPos.x = pX;
				OldPos.y = pY;
				OldPos.z = pZ;

				if (sMoveStepCount[0]) {
					//이동 스텝 수동 설정
					if (sMoveStepCount[1]) moveStep *= sMoveStepCount[1];

					for (cnt = 0; cnt < sMoveStepCount[0]; cnt++) {
						MoveAngle(moveStep);
						if (MotionInfo->State == CHRMOTION_STATE_RUN)
							MoveAngle(moveStep);
					}
				}
				else {

					if (smCharInfo.wPlayClass[0] == MONSTER_CLASS_BOSS && smCharInfo.dwCharSoundCode == snCHAR_SOUND_DARKKNIGHT) {
						//둠로드
						MoveAngle(moveStep * 2);
					}
					else
						MoveAngle(moveStep);

					if (MotionInfo->State == CHRMOTION_STATE_RUN) {
						if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_GREVEN) {
							//공룡 달리기 무지 빨리
							if (lpTargetPlayInfo && TragetTraceMode) {
								SetTargetPosi2(lpTargetPlayInfo->Position.x >> FLOATNS,
									lpTargetPlayInfo->Position.z >> FLOATNS);
							}
							MoveAngle(moveStep * 2);
							MoveAngle(moveStep * 2);
						}
						else {
							if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_WOLVERLIN) {
								//울버린 달리기
								if (lpTargetPlayInfo && TragetTraceMode) {
									SetTargetPosi2(lpTargetPlayInfo->Position.x >> FLOATNS,
										lpTargetPlayInfo->Position.z >> FLOATNS);
								}
								MoveAngle(moveStep * 2);
							}
							else
								MoveAngle(moveStep);
						}
					}
				}

				if (MotionInfo->State != CHRMOTION_STATE_WALK && MotionInfo->State != CHRMOTION_STATE_RUN)
					SetMotionFromCode(CHRMOTION_STATE_WALK);



				if (OverLapPosi || pX != OldPos.x || pZ != OldPos.z) {
					lpCharOverLap = CheckOtherPlayPosi(pX, pY, pZ);
					if (lpCharOverLap) {
						if (!OverLapPosi) {
							pX = OldPos.x;
							pZ = OldPos.z;
							lpCharOverLap = CheckOtherPlayPosi(pX, pY, pZ);
						}

						if (lpCharOverLap) {
							OverLapPosi = TRUE;
							OverLapPosi2 = FALSE;
						}
						else {
							OverLapPosi = FALSE;
							OverLapPosi2 = TRUE;
						}

					}
					else {
						OverLapPosi = FALSE;
						OverLapPosi2 = FALSE;
					}
				}
				if (!OverLapPosi) lpCharOverLap = 0;
			}
			else {
				if (MotionInfo->State != CHRMOTION_STATE_STAND) SetMotionFromCode(CHRMOTION_STATE_STAND);
			}
		}

		frame += frameStep;

		if (frame > MotionInfo->EndFrame * 160) {

			if (MotionInfo->State == CHRMOTION_STATE_DEAD) {
				frame = MotionInfo->EndFrame * 160;
			}
			else {

				if (MotionInfo->State == CHRMOTION_STATE_EAT && PotionLog == 1) {
					//물약 사용 성공
					smCharInfo.Life[0] += (smCharInfo.Life[1] >> 1);
					if (smCharInfo.Life[0] > smCharInfo.Life[1])
						smCharInfo.Life[0] = smCharInfo.Life[1];
				}

				if (smMonsterInfo.SkillCurse && smCharInfo.Brood != smCHAR_MONSTER_USER) {
					if (MotionInfo->State == CHRMOTION_STATE_ATTACK) {
						lpTargetPlayInfo = 0;
					}

					if (lpTargetChar && MotionInfo->State == CHRMOTION_STATE_SKILL) {
						lpTargetChar->PlayHolyPower[0] = GetRandomPos(10, 20);
						lpTargetChar->PlayHolyPower[1] = 16 * 20;
						lpTargetChar = 0;
					}
				}

				if ((lpTargetPlayInfo || TargetMoveCount) && MotionInfo->State < 0x100)
					PosBeginCount++;

				if (MotionInfo->Repeat == TRUE) {
					frame = (MotionInfo->StartFrame * 160) + (frame - MotionInfo->EndFrame * 160);
				}
				else {
					if (MotionInfo->State == CHRMOTION_STATE_DAMAGE && !PlayStunCount && !PlayStopCount &&
						SetMotionFromCode(CHRMOTION_STATE_WALK))
					{
						TargetMoveCount = 2;
					}
					else
						SetMotionFromCode(CHRMOTION_STATE_STAND);
					AutoMoveStep = 1;
				}

				if (PosBeginCount > 4) {
					x = (pX - PosBeginMove.x) >> FLOATNS;
					z = (pZ - PosBeginMove.z) >> FLOATNS;
					dist = x * x + z * z;

					PosBeginMove.x = pX;
					PosBeginMove.z = pZ;

					PosBeginCount = 0;

					if (dist < (32 * 32)) {
						SetTargetPosi(PosiAround.x >> FLOATNS, PosiAround.z >> FLOATNS);
						TragetTraceMode = 0;

						y = (rand() % ANGLE_90) - ANGLE_45;
						Angle.y = (Angle.y + y) & ANGCLIP;

						TargetMoveCount = 10;
						OverLapPosi2 = FALSE;

						ptNextTarget.x = 0;
						ptNextTarget.y = 0;
						ptNextTarget.z = 0;

						if (!SetMotionFromCode(CHRMOTION_STATE_RUN)) {
							SetMotionFromCode(CHRMOTION_STATE_WALK);
						}

						//lpTargetPlayInfo = 0;
						goto Skip_5;
					}
				}

				if (TargetMoveCount > 0) {
					TargetMoveCount--;
					if (TargetMoveCount <= 0) SetMotionFromCode(CHRMOTION_STATE_STAND);
				}

				if (DistAroundDbl && (MotionInfo->State == CHRMOTION_STATE_WALK ||
					MotionInfo->State == CHRMOTION_STATE_RUN)) {
					//거리를 벗어난경우 처리
					x = (pX - PosiAround.x) >> FLOATNS;
					z = (pZ - PosiAround.z) >> FLOATNS;
					dist = x * x + z * z;
					if (dist > DistAroundDbl) {
						SetTargetPosi(PosiAround.x >> FLOATNS, PosiAround.z >> FLOATNS);
						ptNextTarget.x = ptNextTarget.z = 0;
						TargetMoveCount = 6;
						TragetTraceMode = 0;
					}
				}

				if (!TargetMoveCount && ptNextTarget.x && ptNextTarget.z) {

					SetTargetPosi(ptNextTarget.x >> FLOATNS, ptNextTarget.z >> FLOATNS);
					TragetTraceMode = TRUE;

					if (OverLapPosi && lpCharOverLap) {
						//캐릭터가 겹쳐 있음 ( 떨어지도록 노력 함 )
						Angle.y = GetRadian2D(lpCharOverLap->pX >> FLOATNS, lpCharOverLap->pZ >> FLOATNS,
							pX >> FLOATNS, pZ >> FLOATNS);

						SetMotionFromCode(CHRMOTION_STATE_WALK);
						OverLapPosi2 = FALSE;
						AutoMoveStep = 0;
					}
					else {
						if (AutoMoveStep <= 0) {
							if (MotionInfo->State == CHRMOTION_STATE_STAND) {
								//지능에 비례한 이동
								AutoMoveStep = smMonsterInfo.IQ - 5;
								if (AutoMoveStep < 0) AutoMoveStep = 0;

								x = (pX - ptNextTarget.x) >> FLOATNS;
								z = (pZ - ptNextTarget.z) >> FLOATNS;
								dist = x * x + z * z;

								if (smMonsterInfo.IQ >= 6 && dist < (64 * 8 * 64 * 8)) {
									if (SetMotionFromCode(CHRMOTION_STATE_RUN)) {
										AutoMoveStep <<= 2;			//달리기는 좀더 오래
									}
									else
										SetMotionFromCode(CHRMOTION_STATE_WALK);
								}
								else
									SetMotionFromCode(CHRMOTION_STATE_WALK);
							}
							else
								SetMotionFromCode(CHRMOTION_STATE_STAND);
						}
						else {
							SetTargetPosi2(ptNextTarget.x >> FLOATNS, ptNextTarget.z >> FLOATNS);
							AutoMoveStep--;
						}
					}

					ptNextTarget.x = 0;
					ptNextTarget.y = 0;
					ptNextTarget.z = 0;
				}

				//몬스터 물약 사용
				if (smMonsterInfo.PotionCount) {
					cnt = (smCharInfo.Life[0] * 100) / smCharInfo.Life[1];
					if (cnt < 30) {
						if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_NPC_MAGICMASTER) {
							//매직마스터는 물약대신 힐링
							smTransCommand.code = smTRANSCODE_PROCESS_SKILL2;
							smTransCommand.size = sizeof(smTRANS_COMMAND);
							smTransCommand.WParam = SKILL_PLAY_HEALING;
							smTransCommand.LParam = dwObjectSerial;
							smTransCommand.SParam = 0;
							smTransCommand.EParam = 0;
							if (lpLinkPlayInfo && lpLinkPlayInfo->lpsmSock)
								lpLinkPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

							smCharInfo.Life[0] += (smCharInfo.Life[1] >> 2);
							if (smCharInfo.Life[0] > smCharInfo.Life[1])
								smCharInfo.Life[0] = smCharInfo.Life[1];

						}
						else {	//일반 몬스터는 물약 사용
							SetMotionFromCode(CHRMOTION_STATE_EAT);
							PotionLog = 1;
						}

						smMonsterInfo.PotionCount--;
					}
				}

				if (dwNextMotionCode) {
					//다음 동작 예약 실행
					if (SetMotionFromCode(dwNextMotionCode) == FALSE) {
						SetMotionFromCode(CHRMOTION_STATE_STAND);
					}
					dwNextMotionCode = 0;
				}
				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_HUNGKY) {
					//변신
					cnt = (smCharInfo.Life[0] * 100) / smCharInfo.Life[1];
					if (cnt < 30 || AttackAnger>80) {
						SetMotionFromCode(CHRMOTION_STATE_WARP);
						lpTargetPlayInfo = 0;
					}
				}

				// 박재원 - 복날 이벤트
				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BEBECHICK) // 아기 꼬꼬 -> 아빠 꼬꼬 로 변신
				{
					//변신
					cnt = (smCharInfo.Life[0] * 100) / smCharInfo.Life[1];
					if (cnt < 30 || AttackAnger>80) {
						SetMotionFromCode(CHRMOTION_STATE_WARP);
						lpTargetPlayInfo = 0;
					}
				}

				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_DEADHOPT)	// pluto 언데드 킹호피 변신
				{
					cnt = (smCharInfo.Life[0] * 100) / smCharInfo.Life[1];
					if (cnt < 20 || AttackAnger > 80)
					{
						SetMotionFromCode(CHRMOTION_STATE_WARP);
						lpTargetPlayInfo = 0;
					}
				}

			}
		}
	Skip_5:
		if (DontMoveFlag == 2)
		{
			Angle.x = 0;
			Angle.y = 0;
			Angle.z = 0;
		}
		SavePlayBuff2();

		if (lpMasterPlayInfo && HoSkillCode && lpMasterPlayInfo->lpsmSock && smCharInfo.Life[0] > 0 && !smCharInfo.lpMonInfo->Caravana)
		{
			x = (lpMasterPlayInfo->Position.x - pX) >> FLOATNS;
			z = (lpMasterPlayInfo->Position.z - pZ) >> FLOATNS;
			dist = x * x + z * z;

			STG_AREA* lpStgArea;

			lpStgArea = FindStageField(lpMasterPlayInfo->Position.x, lpMasterPlayInfo->Position.z);

			if (lpStgArea)
			{
				int iMapID = lpMasterPlayInfo->Position.Area;

				// Mapas que os cristais n�o podem ir
				if (iMapID == 3 || iMapID == 21 || iMapID == 33 || iMapID == 49)
				{
					smCharInfo.Life[0] = 0;
					SetMotionFromCode(CHRMOTION_STATE_DEAD);
					return FALSE;
				}

				if (dist >= (500 * 500) || lpStgArea->lpStage != lpStage)
				{
					y = lpStgArea->lpStage->GetHeight(lpMasterPlayInfo->Position.x, lpMasterPlayInfo->Position.z);

					lpStage = lpStgArea->lpStage;

					if (y < 0)
					{
						smCharInfo.Life[0] = 0;
						SetMotionFromCode(CHRMOTION_STATE_DEAD);
					}

					if (smCharInfo.Life[0] > 0)
					{
						pX = lpMasterPlayInfo->Position.x;
						pY = lpMasterPlayInfo->Position.y;
						pZ = lpMasterPlayInfo->Position.z;

						ptNextTarget.x = ptNextTarget.z = 0;
						TargetMoveCount = 0;
						TragetTraceMode = 0;

						lpTargetChar = 0;
					}
				}
				else if (dist >= (300 * 300) || (!lpTargetChar && dist >= (150 * 150)))
				{
					SetTargetPosi(lpMasterPlayInfo->Position.x >> FLOATNS, lpMasterPlayInfo->Position.z >> FLOATNS);

					TargetMoveCount = 2;
					TragetTraceMode = 0;
					lpTargetChar = 0;
					if (MotionInfo->State != CHRMOTION_STATE_DEAD && (MotionInfo->State >= 0x100 || MotionInfo->State == CHRMOTION_STATE_STAND)) {
						if (!SetMotionFromCode(CHRMOTION_STATE_RUN))
							SetMotionFromCode(CHRMOTION_STATE_WALK);
					}
				}
			}
		}

		// Caravana seguindo pelos mapas aqui
		if (lpMasterPlayInfo && smCharInfo.lpMonInfo && smCharInfo.lpMonInfo->Caravana)
		{
			x = (lpMasterPlayInfo->Position.x - pX) >> FLOATNS;
			z = (lpMasterPlayInfo->Position.z - pZ) >> FLOATNS;
			dist = x * x + z * z;

			STG_AREA* lpStgArea;

			lpStgArea = FindStageField(lpMasterPlayInfo->Position.x, lpMasterPlayInfo->Position.z);

			if (lpStgArea)
			{
				// Caravana pegando ouro sozinha aqui
				if (!smCharInfo.lpMonInfo->GoldTimeIn && dist <= (350 * 350)) // Dist�ncia do player at� a caravana
				{
					auto lpStgItem = lpStgArea->FindNearGold(pX, pY, pZ); // Dist�ncia do ouro at� a caravana

					if (lpStgItem)
					{
						smCharInfo.lpMonInfo->GoldTimeIn = GetTickCount();
						smCharInfo.lpMonInfo->GoldToCollet = lpStgItem;
					}
				}
				else if (smCharInfo.lpMonInfo->GoldTimeIn)
				{
					// Tempo limite para tentar pegar o gold
					if ((GetTickCount() - smCharInfo.lpMonInfo->GoldTimeIn) > 3000) // 3sgs = tempo m�ximo de coleta de um ouro
					{
						smCharInfo.lpMonInfo->GoldToCollet = nullptr;
						smCharInfo.lpMonInfo->GoldTimeIn = 0;
					}
					else
					{
						if (smCharInfo.lpMonInfo->GoldToCollet)
						{
							if (rsPutItem2(lpMasterPlayInfo, &smCharInfo.lpMonInfo->GoldToCollet->Item.ItemInfo))
							{
								lpStgArea->DeleteItem(smCharInfo.lpMonInfo->GoldToCollet);
								smCharInfo.lpMonInfo->GoldToCollet = nullptr;
								smCharInfo.lpMonInfo->GoldTimeIn = 0;
							}
						}
					}
				}

				// Caravana seguindo o player aqui
				if ((dist >= (500 * 500) || lpStgArea->lpStage != lpStage) && smCharInfo.lpMonInfo->CaravanState == CaravanStates::FOLLOW)
				{
					int iMapID = lpMasterPlayInfo->Position.Area;

					if (iMapID != START_FIELD_CASTLE &&
						iMapID != rsSOD_FIELD &&
						iMapID != FIELD_ARENA)
					{
						lpStage = lpStgArea->lpStage;

						pX = lpMasterPlayInfo->Position.x;
						pY = lpMasterPlayInfo->Position.y;
						pZ = lpMasterPlayInfo->Position.z;

						ptNextTarget.x = 0;
						ptNextTarget.z = 0;
						TargetMoveCount = 0;
						TragetTraceMode = 0;

						chrAttackTarget = 0;
					}
				}
				else if ((dist >= (300 * 300) || dist >= (150 * 150)) && smCharInfo.lpMonInfo->CaravanState == CaravanStates::FOLLOW)
				{
					int iMapID = lpMasterPlayInfo->Position.Area;

					if (iMapID != START_FIELD_CASTLE &&
						iMapID != rsSOD_FIELD &&
						iMapID != FIELD_ARENA)
					{
						SetTargetPosi(lpMasterPlayInfo->Position.x >> FLOATNS, lpMasterPlayInfo->Position.z >> FLOATNS);
						TargetMoveCount = 2;
						TragetTraceMode = 0;
						chrAttackTarget = 0;

						if (MotionInfo->State != CHRMOTION_STATE_DEAD && (MotionInfo->State >= 0x100 || MotionInfo->State == CHRMOTION_STATE_STAND)) {
							if (!SetMotionFromCode(CHRMOTION_STATE_RUN))
								SetMotionFromCode(CHRMOTION_STATE_WALK);
						}
					}
				}
			}
		}


		if (dwLinkObjectCode) {
			switch (smCharInfo.dwCharSoundCode) {
			case snCHAR_SOUND_NPC_SKILLMASTER:
			case snCHAR_SOUND_NPC_MAGICMASTER:

				if (smCharInfo.Life[0] <= (smCharInfo.Life[1] >> 4)) {
					//스킬마스터를 이겼다!
					smCharInfo.Life[0] = (smCharInfo.Life[1] >> 4);
					lpTargetPlayInfo = 0;
					lpTargetChar = 0;
					SetMotionFromCode(CHRMOTION_STATE_STAND);
					//smCharInfo.State=smCHAR_STATE_NPC;
					ActionPattern = 12;
					PosiAround.x = pX;
					PosiAround.y = pY;
					PosiAround.z = pZ;
					DistAroundDbl = 256 * 256;
					if (lpLinkPlayInfo && lpLinkPlayInfo->dwObjectSerial == dwLinkObjectCode && lpLinkPlayInfo->lpsmSock) {
						rsProcessQuest(lpLinkPlayInfo, this, SIN_QUEST_CODE_CHANGEJOB4, dwObjectSerial);
					}
					lpChar = rsGetLinkChar(this);		//연결캐릭 삭제
					if (lpChar) {
						lpChar->SetMotionFromCode(CHRMOTION_STATE_DEAD);
						lpChar->smCharInfo.Life[0] = 0;
					}
					/*
					else {
						chrAutoPlayer[cnt].Close();
						((STG_AREA *)chrAutoPlayer[cnt].lpExt1)->MonsterCount--;
						((STG_AREA *)chrAutoPlayer[cnt].lpExt1)->DeleteMonTable( &chrAutoPlayer[cnt] );
					}
					*/
				}
				else {
					if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_NPC_MAGICMASTER) {
						//레이몬 순간이동 회피
						if ((rand() % 100) < 3 && MotionInfo->State != CHRMOTION_STATE_SKILL && MotionInfo->State != CHRMOTION_STATE_EAT) {
							if (lpLinkPlayInfo) {
								x = (pX - lpLinkPlayInfo->Position.x) >> FLOATNS;
								z = (pZ - lpLinkPlayInfo->Position.z) >> FLOATNS;
								dist = x * x + z * z;
								if (dist < (300 * 300)) {
									pX = QUEST_ARENA_NPC_POS_X + ((rand() % 600) - 300) * fONE;
									pZ = QUEST_ARENA_NPC_POS_Z + ((rand() % 600) - 300) * fONE;
								}
							}
						}
					}
				}
				break;

			case snCHAR_SOUND_FURY:

				if (smCharInfo.Life[0] <= 0 && !smMonsterInfo.PotionCount && smCharInfo.Life[1] > 1) {
					//스킬마스터를 이겼다!
					if (lpLinkPlayInfo && lpLinkPlayInfo->dwObjectSerial == dwLinkObjectCode && lpLinkPlayInfo->lpsmSock) {
						rsProcessQuest(lpLinkPlayInfo, this, HAQUEST_CODE_FURYOFPHANTOM, dwObjectSerial);
						smCharInfo.Life[1] = 1;
					}
				}
				break;
			}
		}

		if (lpTargetChar) {
			if (smMonsterInfo.SkillCurse && smCharInfo.Brood != smCHAR_MONSTER_USER && lpTargetChar->smCharInfo.Brood != smCHAR_MONSTER_USER) {
				//몬스터 공격력 강화용
				if (MotionInfo->State < 0x100) {
					TargetMoveCount = 1;
					AutoMoveStep = 1;
					lpTargetChar = 0;
				}
			}
			else if (lpTargetChar->Flag && !PlayStunCount && !PlayStopCount) {

				dist = smCharInfo.Shooting_Range + 32;			//슈팅 거리가 몬스터의 공격 거리
				if (dist < 80) dist = 80;						//거리 보정

				if (MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
					MotionInfo->State != CHRMOTION_STATE_DEAD &&
					MotionInfo->State != CHRMOTION_STATE_EAT) {

					if (MotionInfo->State != CHRMOTION_STATE_ATTACK &&
						MotionInfo->State != CHRMOTION_STATE_SKILL && MotionInfo->State != CHRMOTION_STATE_DEAD) {

						cnt = GetDistanceDbl(pX, pZ, lpTargetChar->pX, lpTargetChar->pZ);

						dist = dist >> FLOATNS;
						dist *= dist;

						if (cnt < dist && abs(pY - lpTargetChar->pY) < 64 * fONE) {

							PosBeginMove.x = 0;
							PosBeginMove.z = 0;
							PosBeginCount = 0;

							dwLastTransTime = dwPlayServTime;		//최근 활성화된 시간

							SetMotionFromCode(CHRMOTION_STATE_ATTACK);

							if (smMonsterInfo.SkillRating > 0 && MotionInfo->State == CHRMOTION_STATE_ATTACK) {
								//몬스터 스킬 공격
								if ((rand() % 100) < smMonsterInfo.SkillRating)
									SetMotionFromCode(CHRMOTION_STATE_SKILL);
							}

							Angle.y = GetRadian2D(pX, pZ, lpTargetChar->pX, lpTargetChar->pZ);
							AttackX = lpTargetChar->pX;
							AttackY = lpTargetChar->pY;
							AttackZ = lpTargetChar->pZ;

						}
						else {
							if (cnt > DIST_TRANSLEVEL_LOW) {
								lpTargetChar = 0;
							}
						}
					}
				}

				if (smCharInfo.Brood != smCHAR_MONSTER_USER) {
					if (FrameCounter == 80 && smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL) {		//보스 바벨
						AttackSkillRange = smMonsterInfo.SkillRange * smMonsterInfo.SkillRange;

						//몬스터 생명력 재생치 구하는 함수
						smCharInfo.Life[0] += rsGetMonsterLifeRegen(this);

						if (smCharInfo.Life[0] > smCharInfo.Life[1] || smCharInfo.Life[0] < 0)
							smCharInfo.Life[0] = smCharInfo.Life[1];
					}
				}


				if (FrameCounter == 40 && lpTargetChar) {

					if (MotionInfo->State == CHRMOTION_STATE_ATTACK || MotionInfo->State == CHRMOTION_STATE_SKILL) {
						//SendTransAttack( 0 , lpTargetPlayInfo->lpsmSock , 0);
						AttackUserFlag = TRUE;
						AttackAnger++;				//공격 횟수 카운터
						cnt = sinGetPVPAccuracy(smCharInfo.Level, smCharInfo.Attack_Rating, lpTargetChar->smCharInfo.Level, lpTargetChar->smCharInfo.Defence);
						if ((rand() % 100) < cnt && lpTargetChar->smCharInfo.Life[0] > 0) {
							//명중
							if (!lpTargetChar->lpTargetPlayInfo) {
								lpTargetChar->lpTargetChar = this;
							}
							else {
								dist = GetDistanceDbl(pX, pZ, lpTargetChar->pX, lpTargetChar->pZ);
								if (GetDistanceDbl(pX, pZ, lpTargetChar->lpTargetPlayInfo->Position.x, lpTargetChar->lpTargetPlayInfo->Position.z) > dist) {
									lpTargetChar->lpTargetChar = this;
									lpTargetChar->lpTargetPlayInfo = 0;
								}
							}

							// smCharInfo.Attack_Damage[0]  미스틱 처리

							if (MotionInfo->State == CHRMOTION_STATE_SKILL && smCharInfo.Attack_Damage[0]) {
								cnt = GetRandomPos(smMonsterInfo.SkillDamage[0], smMonsterInfo.SkillDamage[1]);

								switch (smCharInfo.dwCharSoundCode) {
								case snCHAR_SOUND_GUARDIAN_SAINT:
									//가디언 세인트 ( 범위 공격 )
									rsSkillDamage_AroundMonster(this);
									cnt = 0;
									break;
								}
							}
							else {
								cnt = GetRandomPos(smCharInfo.Attack_Damage[0], smCharInfo.Attack_Damage[1]);
							}


							if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL ||
								smCharInfo.dwCharSoundCode == snCHAR_SOUND_CHAOSCARA ||
								smCharInfo.dwCharSoundCode == snCHAR_SOUND_DEATHKNIGHT ||
								smCharInfo.dwCharSoundCode == snCHAR_SOUND_MOKOVA ||
								smCharInfo.dwCharSoundCode == snCHAR_SOUND_KELVEZU ||
								smCharInfo.dwCharSoundCode == snCHAR_SOUND_MOKOVA)
							{
								cnt *= 8;
								if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_KELVEZU)
								{
									lpTargetChar->smCharInfo.Life[0] = 0;
									cnt = 1;
								}
							}


							if (lpTargetChar)
								cnt -= (cnt * lpTargetChar->smCharInfo.Absorption) / 100;
							else
								cnt = 0;

							if (cnt > 0)
							{
								lpTargetChar->smCharInfo.Life[0] -= cnt;

								if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_WEB)
								{
									if (lpTargetChar->smCharInfo.dwCharSoundCode != snCHAR_SOUND_KELVEZU)
										lpTargetChar->PlaySlowCount = -36;
								}


								if (lpTargetChar->smCharInfo.Life[0] <= 0)
								{
									lpTargetChar->smCharInfo.Life[0] = 0;
									lpTargetChar->SetMotionFromCode(CHRMOTION_STATE_DEAD);
									if (lpMasterPlayInfo && lpMasterPlayInfo->lpsmSock && lpMasterPlayInfo->dwObjectSerial && lpMasterPlayInfo->dwObjectSerial == smCharInfo.Next_Exp)
									{
										lpTargetChar->lpExt2 = lpMasterPlayInfo;

										lpTargetChar->lpExt3 = this;
									}
									else
									{
										if (lpMasterPlayInfo)
										{

											smCharInfo.Life[0] = 0;
											SetMotionFromCode(CHRMOTION_STATE_DEAD);
										}
										else
										{
											lpTargetChar = 0;
										}
									}
								}
								else
								{
									if (lpTargetChar->MotionInfo->State < 0x100)
									{
										lpTargetChar->SetMotionFromCode(CHRMOTION_STATE_DAMAGE);
									}
								}
							}
						}
					}
				}
			}
			else
			{
				SetNextTarget(0, 0, 0);
			}


			if (lpTargetChar && (!lpTargetChar->Flag || lpTargetChar->smCharInfo.Life[0] <= 0)) {
				lpTargetChar = 0;
				if (MotionInfo->State < 0x100) {
					TargetMoveCount = 1;
					AutoMoveStep = 1;
				}
			}

			//if ( (smCharInfo.dwCharSoundCode==snCHAR_SOUND_DEATHKNIGHT || smCharInfo.dwCharSoundCode==snCHAR_SOUND_KELVEZU ) && lpTargetChar ) 
			if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_DEATHKNIGHT && lpTargetChar)
			{		//발렌토 얍삽이 상대 검사
				if (GetDistanceDbl(pX, pZ, lpTargetChar->pX, lpTargetChar->pZ) > 120 * 120) {
					lpTargetChar = 0;			//압삽이 상대 안함
					SetNextTarget(0, 0, 0);
				}
			}

		}


		cnt = TRUE;

		switch (smCharInfo.dwCharSoundCode) {		//뱀피릭 머신 공격 제한 (깃발내 몬스터수 확인)
		case snCHAR_SOUND_VAMPRICMACHINE:
			//해당 몬스터 출신 깃발 의 활성화 몬스터 수
			if (rsGetMonCountFlag(this) > 3) cnt = FALSE;
			break;

		case snCHAR_SOUND_MYSTIC:				//죽을때 미스틱 자폭
			if (MotionInfo->State == CHRMOTION_STATE_DEAD && FrameCounter < 16) {
				AttackSkillRange = smMonsterInfo.SkillRange * smMonsterInfo.SkillRange;
				AttackUserFlag = TRUE;
				FrameCounter = 16;
			}
			if (MotionInfo->State != CHRMOTION_STATE_DEAD) {
				if (Counter > 16 * 60 * 2) {
					smCharInfo.Life[0] = 0;
					SetMotionFromCode(CHRMOTION_STATE_DEAD);
					AttackSkillRange = smMonsterInfo.SkillRange * smMonsterInfo.SkillRange;
					AttackUserFlag = TRUE;
					FrameCounter = 16;
				}
			}
			if (lpTargetChar && MotionInfo->State == CHRMOTION_STATE_DEAD) {
				if (FrameCounter == 16 && lpTargetChar->MotionInfo->State != CHRMOTION_STATE_DEAD && lpTargetChar->smCharInfo.Life[0] > 0) {
					lpTargetChar->smCharInfo.Life[0] -= GetRandomPos(smMonsterInfo.SkillDamage[0], smMonsterInfo.SkillDamage[1]) * 2;
					if (lpTargetChar->smCharInfo.Life[0] <= 0) {
						lpTargetChar->smCharInfo.Life[0] = 0;
						lpTargetChar->SetMotionFromCode(CHRMOTION_STATE_DEAD);
					}
				}
				lpTargetChar = 0;
			}
			break;

		case snCHAR_SOUND_CHAOSCARA:
			if (MotionInfo->State == CHRMOTION_STATE_SKILL && MotionInfo->KeyCode != 'J') {
				smMonsterInfo.SkillRangeRect.left = 0;
				smMonsterInfo.SkillRangeRect.right = 0;
				smMonsterInfo.SkillRangeRect.top = 0;
				smMonsterInfo.SkillRangeRect.bottom = 0;
			}
			break;
		}




		//서버용 처리 ( 목표 캐릭을 추적하여 공격 하도록 설정 )
		if (!lpTargetChar && lpTargetPlayInfo && cnt) {
			if (lpTargetPlayInfo->lpsmSock && !PlayStunCount && !PlayStopCount) {

				dist = smCharInfo.Shooting_Range;			//슈팅 거리가 몬스터의 공격 거리

				if (MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
					MotionInfo->State != CHRMOTION_STATE_DEAD &&
					MotionInfo->State != CHRMOTION_STATE_EAT && dist >= 0) {
					if (PlayAttackFromPosi(
						lpTargetPlayInfo->Position.x,
						lpTargetPlayInfo->Position.y,
						lpTargetPlayInfo->Position.z,
						dist) == TRUE) {

						PosBeginMove.x = 0;
						PosBeginMove.z = 0;
						PosBeginCount = 0;

						dwLastTransTime = dwPlayServTime;		//최근 활성화된 시간

						if (smMonsterInfo.SkillRating > 0 && MotionInfo->State == CHRMOTION_STATE_ATTACK) {
							//몬스터 스킬 공격
							if ((rand() % 100) < smMonsterInfo.SkillRating) {
								if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_NPC_MAGICMASTER && (rand() % 100) < 20) {
									//매직 마스터 기력 물약먹기
									SetMotionFromCode(CHRMOTION_STATE_EAT);
									PotionLog = 2;
								}
								else if (SetMotionFromCode(CHRMOTION_STATE_SKILL) > 0) {
									if (smMonsterInfo.SkillCurse) {
										//저주 걸기 기술
										switch (smMonsterInfo.SkillCurse) {
										case 102:	//다른 몬스터 공격력 강화 기술
											lpChar = rsFindAroundMonster(this, 200);
											if (lpChar) {
												lpTargetChar = lpChar;
												lpTargetPlayInfo = 0;
												Angle.y = GetRadian2D(pX, pZ, lpTargetChar->pX, lpTargetChar->pZ);
											}
											else {
												//저주 기술 실패
												if (SetMotionFromCode(CHRMOTION_STATE_ATTACK) == FALSE)
													SetMotionFromCode(CHRMOTION_STATE_STAND);
											}
											break;

										case 103:	//유저에게 저주걸기 ( 공격력 약화 )
											lpTargetPlayInfo->dwCurse_Attack_Time = dwPlayServTime + SUCCUBUS_CURSE_TIME * 1000;
											lpTargetPlayInfo->dwCurse_Attack_Param = GetRandomPos(10, 15);
											break;

										case 101:	//유저에게 저주걸기 ( 흡수력 약화 )
											lpTargetPlayInfo->dwCurse_Defence_Time = dwPlayServTime + SUCCUBUS_CURSE_TIME * 1000;
											lpTargetPlayInfo->dwCurse_Defence_Param = GetRandomPos(2, 6);

											if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_MYSTIC)
												lpTargetPlayInfo->dwCurse_Defence_Param = GetRandomPos(10, 16);

											break;

										}
									}


									if (smMonsterInfo.SkillRange > 0) {
										//범위형
										//AttackSkillRange = smMonsterInfo.SkillRange*smMonsterInfo.SkillRange;
										AttackSkillRange = 0;

										if (smMonsterInfo.SkillDistance) {

											GetMoveLocation(0, 0, smMonsterInfo.SkillDistance * fONE, 0, Angle.y, 0);
											x = pX + GeoResult_X;
											z = pZ + GeoResult_Z;

											dist = smMonsterInfo.SkillRange << FLOATNS;

											smMonsterInfo.SkillRangeRect.left = x - dist;
											smMonsterInfo.SkillRangeRect.right = x + dist;
											smMonsterInfo.SkillRangeRect.top = z - dist;
											smMonsterInfo.SkillRangeRect.bottom = z + dist;
										}
									}

									switch (smCharInfo.dwCharSoundCode) {
									case snCHAR_SOUND_RATOO:
										//주변 몬스터를 찾아 스킬 ( 라투 주변 몬스터 스턴제거 )
										rsSkillMonster_AroundMonster(this, 200);
										break;
									case snCHAR_SOUND_NPC_SKILLMASTER:
										cnt = rand() % 100;
										if (cnt < 3 && !rsGetLinkChar(this) && lpLinkPlayInfo) {
											if ((cnt & 1) == 0) {
												lpChar = OpenMonsterFromSkill(SKILL_PLAY_METAL_GOLEM, pX, pY, pZ, lpLinkPlayInfo, this);
												if (lpChar) {
													lpChar->smCharInfo.wPlayClass[1] = fONE + 5 * 15;		//크기 확대
													lpChar->sMoveStepCount[0] = 1;		//이동 속도 2x2
													lpChar->sMoveStepCount[1] = 2;
													//lpChar->smCharInfo.Attack_Damage[0]/=2;
													//lpChar->smCharInfo.Attack_Damage[1]/=2;
													//lpChar->smMonsterInfo.SkillDamage[0]/=2;
													//lpChar->smMonsterInfo.SkillDamage[1]/=2;
												}
											}
											else {
												lpChar = OpenMonsterFromSkill(SKILL_PLAY_RECALL_WOLVERIN, pX, pY, pZ, lpLinkPlayInfo, this);
												if (lpChar) {
													lpChar->smCharInfo.wPlayClass[1] = (fONE / 2) + 5 * 12;		//크기 확대
													lpChar->sMoveStepCount[0] = 1;		//이동 속도 2x2
													lpChar->sMoveStepCount[1] = 2;
													//lpChar->smCharInfo.Attack_Damage[0]/=2;
													//lpChar->smCharInfo.Attack_Damage[1]/=2;
													//lpChar->smMonsterInfo.SkillDamage[0]/=2;
													//lpChar->smMonsterInfo.SkillDamage[1]/=2;
												}
											}
										}
										break;
									case snCHAR_SOUND_NPC_MAGICMASTER:
										cnt = rand() % 100;
										if (cnt < 3 && !rsGetLinkChar(this) && lpLinkPlayInfo) {
											lpChar = OpenMonsterFromSkill(SKILL_PLAY_FIRE_ELEMENTAL, pX, pY, pZ, lpLinkPlayInfo, this);
											if (lpChar) {
												lpChar->sMoveStepCount[0] = 1;		//이동 속도 2x2
												lpChar->sMoveStepCount[1] = 2;
												//lpChar->smCharInfo.Attack_Damage[0]/=2;
												//lpChar->smCharInfo.Attack_Damage[1]/=2;
												//lpChar->smMonsterInfo.SkillDamage[0]/=2;
												//lpChar->smMonsterInfo.SkillDamage[1]/=2;
											}
										}
										break;
									}
								}
							}
						}
					}
				}

				EvtFrame = 0;
				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL) {		//보스 바벨
					if (FrameCounter == 40 || FrameCounter == 80)
						EvtFrame = 1;
				}


				if ((!EvtFrame && FrameCounter == 40) || EvtFrame) {
					if (MotionInfo->State == CHRMOTION_STATE_ATTACK &&
						rsProcessAttack_SkillHolyReflection(lpTargetPlayInfo, this) == TRUE) {

						if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_VAMPRICMACHINE) {		//뱀피릭 머신 벌발사
							cnt = 0;
							//특정위치에 보조몬스터 강제 출현 시킴
							rsOpenSubMonster(this, snCHAR_SOUND_VAMPRICBEE, pX, pY, pZ, 4);
						}
						else {
							if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL && (rand() % 100) < 60) {	//60%확률
								//바벨 일반공격시 스킬도 같이 적용 (범위지정)
								rsSendRangeDamage(this, lpTargetPlayInfo, 100, fONE / 2);

								x = (lpTargetPlayInfo->Position.x - pX) >> FLOATNS;
								z = (lpTargetPlayInfo->Position.z - pZ) >> FLOATNS;
								dist = x * x + z * z;
								if (dist > (200 * 200)) {
									//원거리 70%데미지
									dm1 = smCharInfo.Attack_Damage[0];
									dm2 = smCharInfo.Attack_Damage[1];

									smCharInfo.Attack_Damage[0] = (smCharInfo.Attack_Damage[0] * 70) / 100;
									smCharInfo.Attack_Damage[1] = (smCharInfo.Attack_Damage[1] * 70) / 100;
									cnt = SendTransAttack(0, lpTargetPlayInfo->lpsmSock, 0);

									smCharInfo.Attack_Damage[0] = dm1;
									smCharInfo.Attack_Damage[1] = dm2;
								}
								else {
									//근거리 100% 데미지
									cnt = SendTransAttack(0, lpTargetPlayInfo->lpsmSock, 0);
								}

							}
							else {
								cnt = SendTransAttack(0, lpTargetPlayInfo->lpsmSock, 0);
							}
						}

						if (cnt) {
							//공격 데이타 기록
							lpTargetPlayInfo->Send_AttackCount++;
							lpTargetPlayInfo->Send_AttackDamage += cnt;

							//해외//////////////////////////////////////////////////////////////////////////////////////////////////
							/*							if ( rsServerConfig.Event_Child==1 && PartyFlag!=rsHARDCORE_EVENT_FLAG )
														{
															//어린이 이벤트
															if ( smCharInfo.dwCharSoundCode==snCHAR_SOUND_BUMA ||
																smCharInfo.dwCharSoundCode==snCHAR_SOUND_SEN ||
																smCharInfo.dwCharSoundCode==snCHAR_SOUND_BEEDOG ||
																smCharInfo.dwCharSoundCode==snCHAR_SOUND_MINIG ||
																smCharInfo.dwCharSoundCode==snCHAR_SOUND_DECOY ||
																smCharInfo.dwCharSoundCode==snCHAR_SOUND_MUMMY ||
																smCharInfo.dwCharSoundCode==snCHAR_SOUND_HULK ||
																smCharInfo.dwCharSoundCode==snCHAR_SOUND_SANDLEM ) {

																if ( (rand()%70)==1 ) {
																	smTransCommand.size = sizeof(smTRANS_COMMAND);
																	smTransCommand.code = smTRANSCODE_COMIC_SKIN;
																	smTransCommand.WParam = (rand()%2)+1;
																	smTransCommand.LParam = 60*10;
																	smTransCommand.SParam = 0;
																	smTransCommand.EParam = 0;
																	lpTargetPlayInfo->lpsmSock->Send( (char *)&smTransCommand , smTransCommand.size , TRUE );
																}
															}
														}
							*/   /////////////////////////////////////////////////////////////////////////////////////////////////////
							//뽕망치 이벤트
							if (smCharInfo.wPlayClass[0] == MONSTER_CLASS_HAMMER &&
								lpTargetPlayInfo->smCharInfo.SizeLevel <= 0x1000 || lpTargetPlayInfo->smCharInfo.SizeLevel > 0x1002) {

								if (dwEventAttackParam != lpTargetPlayInfo->dwObjectSerial) {

									EventAttackCount = 0;
									dwEventAttackParam = lpTargetPlayInfo->dwObjectSerial;
								}

								EventAttackCount++;

								if (EventAttackCount >= 10) {

									smTransCommand.size = sizeof(smTRANS_COMMAND);
									smTransCommand.code = smTRANSCODE_COMIC_SKIN;
									smTransCommand.WParam = (rand() % 2) + 1;
									smTransCommand.LParam = 60 * 10;
									smTransCommand.SParam = 0;
									smTransCommand.EParam = 0;
									lpTargetPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

									if (smMonsterInfo.UseEventModel && smCharInfo.szModelName2[1]) {
										//이벤트 모델 사용중

										Close();

										lstrcpy(smCharInfo.szModelName, smCharInfo.szModelName2 + 1);
										SetLoadPattern(smCharInfo.szModelName);
										smCharInfo.bUpdateInfo[0]++;
										SetMotionFromCode(CHRMOTION_STATE_STAND);

									}

									smCharInfo.wPlayClass[0] = MONSTER_CLASS_NORMAL;
									smMonsterInfo.UseEventModel = 0;
								}

								TRANS_CHATMESSAGE	TransChatMessage;
								wsprintf(TransChatMessage.szMessage, "%s: %d ", smCharInfo.szName, EventAttackCount);
								TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
								TransChatMessage.size = 32 + lstrlen(TransChatMessage.szMessage);
								TransChatMessage.dwIP = 0;
								TransChatMessage.dwObjectSerial = dwObjectSerial;

								rsSendCommandUser2(lpTargetPlayInfo, (smTRANS_COMMAND*)&TransChatMessage);
							}
						}

						AttackUserFlag = TRUE;
						AttackAnger++;				//공격 횟수 카운터

						if (lpTargetPlayInfo->BadPlayer == 4 && lpTargetPlayInfo->lpsmSock) {
							if ((smCharInfo.Level + 6) > lpTargetPlayInfo->smCharInfo.Level && (rand() % 100) > 92) {
								//배드 플레이어 확인 강제로 죽이기
								smTransCommand.code = smTRANSCODE_DEAD_PLAYER;
								smTransCommand.size = sizeof(smTRANS_COMMAND);
								smTransCommand.WParam = 0;
								smTransCommand.LParam = 0;
								smTransCommand.SParam = 0;
								smTransCommand.EParam = 0;
								lpTargetPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
								lpTargetPlayInfo->BadPlayer = 2;		//죽인후 해제
							}
						}
					}
				}

				EvtFrame = 32;

				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL || smCharInfo.dwCharSoundCode == snCHAR_SOUND_DEATHKNIGHT) {		//보스 바벨
					EvtFrame = 120;
					if (FrameCounter == EvtFrame) {							//피 채워준다
						//몬스터 생명력 재생치 구하는 함수
						smCharInfo.Life[0] += rsGetMonsterLifeRegen(this);

						if (smCharInfo.Life[0] > smCharInfo.Life[1] || smCharInfo.Life[0] < 0)
							smCharInfo.Life[0] = smCharInfo.Life[1];

					}
				}

				if (FrameCounter == EvtFrame) {
					if (MotionInfo->State == CHRMOTION_STATE_SKILL) {
						if (smCharInfo.dwCharSoundCode != snCHAR_SOUND_MYSTIC) {				//미스틱 저주 데미지 없음
							AttackSkillRange = smMonsterInfo.SkillRange * smMonsterInfo.SkillRange;
							if (!AttackSkillRange && lpTargetPlayInfo) {
								//상대 캐릭터에 기술 공격 가함 
								rsSendTransSkillAttack(this, lpTargetPlayInfo);
							}
							AttackUserFlag = TRUE;
						}
					}
				}

			}
			else {
				SetNextTarget(0, 0, 0);
			}
		}


		if (PlaySlowCount > 0) 		//몬스터는 저속 상태 유지 모드로 사용 
			PlaySlowCount--;

		if (PlaySlowCount < 0) 		//몬스터는 저속 상태 유지 모드로 사용 
			PlaySlowCount++;

		if (PlayStunCount > 0)		//몬스터는 중립 상태 유지 모드로 사용 (로어 공격 해당 )
			PlayStunCount--;

		if (PlayStopCount > 0)		//몬스터는 움직임이 멈춰 있다
			PlayStopCount--;

		if (PlayHolyMind[1] > 0)	//데미지 약화
			PlayHolyMind[1]--;

		if (PlayHolyPower[1] > 0)	//데미지 강화
			PlayHolyPower[1]--;

		if (PlayPoison[1] > 0) {
			PlayPoison[1]--;			//독 감염
			if ((Counter & 0xF) == 0) {
				if (smCharInfo.Life[0] > 0 && MotionInfo->State != CHRMOTION_STATE_DEAD) {
					smCharInfo.Life[0] -= PlayPoison[0];			//데미지 감소
					if (smCharInfo.Life[0] < 1) {				//죽음
						smCharInfo.Life[0] = 1;
						//SetMotionFromCode( CHRMOTION_STATE_DEAD );
						PlayPoison[1] = 0;
					}
				}
			}
		}

		if (PlayHolyIncantation[1] > 0) {
			PlayHolyIncantation[1]--;
			if (PlayHolyIncantation[1] == 0 && smCharInfo.Brood == smCHAR_MONSTER_USER) {
				smCharInfo.Brood = PlayHolyIncantation[0];	//smCHAR_MONSTER_USER
				smCharInfo.bUpdateInfo[0]++;
				lpMasterPlayInfo = 0;
			}
		}

		if (UseObject_VirtualLife) {
			//가상의 생명력
			if (sObject_VirtualLife[0] > 0) {
				cnt = (smCharInfo.Life[0] * 100) / smCharInfo.Life[1];
				if (cnt <= 10) { //10%이하 재생
					sObject_VirtualLife[0] --;
					smCharInfo.Life[0] = smCharInfo.Life[1];

					if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_KELVEZU && (sObject_VirtualLife[0] & 0x1) == 0) {
						//켈베쥬 (카오스 호출)
						rsOpenSubMonster(this, snCHAR_SOUND_CHAOSCARA, pX + ((rand() % (200 * fONE)) - 100 * fONE), 0, pZ + ((rand() % (200 * fONE)) - 100 * fONE), 8);
						rsOpenSubMonster(this, snCHAR_SOUND_CHAOSCARA, pX + ((rand() % (200 * fONE)) - 100 * fONE), 0, pZ + ((rand() % (200 * fONE)) - 100 * fONE), 8);
					}
					else if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_MOKOVA && (sObject_VirtualLife[0] & 0x1) == 0) {
						//모코바 (다크가드 호출)
						rsOpenSubMonster(this, snCHAR_SOUND_DARKGUARD, pX + ((rand() % (200 * fONE)) - 100 * fONE), 0, pZ + ((rand() % (200 * fONE)) - 100 * fONE), 8);
						rsOpenSubMonster(this, snCHAR_SOUND_DARKGUARD, pX + ((rand() % (200 * fONE)) - 100 * fONE), 0, pZ + ((rand() % (200 * fONE)) - 100 * fONE), 8);
					}

				}
			}

			cnt = (smCharInfo.Life[0] * 100) / smCharInfo.Life[1];

			sObject_DisplayLife[0] = sObject_VirtualLife[0] * 100;
			sObject_DisplayLife[0] += cnt;
			sObject_DisplayLife[1] = (sObject_VirtualLife[1] + 1) * 100;

			if (sObject_DisplayLife[0] <= 0 && smCharInfo.Life[0] > 0) {
				sObject_DisplayLife[0] = 1;
			}

			if (sObject_DisplayLife[0] > sObject_DisplayLife[1])
				sObject_DisplayLife[0] = sObject_DisplayLife[1];

		}

		FrameCounter += 3;
		break;


	case 10:	//동물
	case 11:	//NPC
		if (MotionInfo->State && MotionInfo->State < 0x100) {
			if (MotionInfo->State == CHRMOTION_STATE_WALK) {
				MoveAngle(SERVER_CHAR_SKIP);
				if (MotionInfo->State != CHRMOTION_STATE_WALK) SetMotionFromCode(CHRMOTION_STATE_WALK);
			}
			else {
				if (MotionInfo->State != CHRMOTION_STATE_STAND) SetMotionFromCode(CHRMOTION_STATE_STAND);
			}

		}

		frame += SERVER_CHAR_FRAME_STEP;

		if (frame > MotionInfo->EndFrame * 160) {

			if (MotionInfo->State == CHRMOTION_STATE_DEAD) {
				frame = MotionInfo->EndFrame * 160;
			}
			else {

				if (MotionInfo->Repeat == TRUE) {
					frame = (MotionInfo->StartFrame * 160) + (frame - MotionInfo->EndFrame * 160);
				}
				else SetMotionFromCode(CHRMOTION_STATE_STAND);



				if (MotionInfo->State == CHRMOTION_STATE_STAND) {
					cnt = rand() % ANGCLIP;
					if (((cnt >> 2) & 3) == 0) {
						//때때로 하는 동작
						if (ActionPattern == 10) {
							if (!SetMotionFromCode(CHRMOTION_STATE_SOMETIME)) {
								Angle.y = cnt & ANGCLIP;
								SetMotionFromCode(CHRMOTION_STATE_WALK);
							}
						}
						else {
							if (!SetMotionFromCode(TRUE)) {
								Angle.y = cnt & ANGCLIP;
								SetMotionFromCode(CHRMOTION_STATE_WALK);
							}
						}
					}
					else {
						Angle.y = cnt & ANGCLIP;
						SetMotionFromCode(CHRMOTION_STATE_WALK);
					}

					if (MotionInfo->State == CHRMOTION_STATE_WALK) {
						//거리를 벗어난경우 처리
						x = (pX - PosiAround.x) >> FLOATNS;
						z = (pZ - PosiAround.z) >> FLOATNS;
						dist = x * x + z * z;
						if (dist > DistAroundDbl) {
							SetTargetPosi(PosiAround.x >> FLOATNS, PosiAround.z >> FLOATNS);
						}
					}
				}
				else SetMotionFromCode(CHRMOTION_STATE_STAND);

				if (OverLapPosi && lpCharOverLap) {
					//캐릭터가 겹쳐 있음 ( 떨어지도록 노력 함 )
					Angle.y = GetRadian2D(lpCharOverLap->pX >> FLOATNS, lpCharOverLap->pZ >> FLOATNS,
						pX >> FLOATNS, pZ >> FLOATNS);

					SetMotionFromCode(CHRMOTION_STATE_WALK);
					OverLapPosi2 = FALSE;
				}
			}
		}
		SavePlayBuff();
		FrameCounter += 3;
		break;

	case 12:	//NPC
		//######################################################################################
		// 작 성 자 : 오 영 석	 ( 서버 코드에 추가 )
		// NPC 는 smMotionInfo2 사용치 않음. NPC 는 고정이다.		
		if (smMotionInfo->NpcMotionRate[CHRMOTION_EXT])
		{
			///////////////////////////////////////////////////////////////////
			if (MotionInfo->State == CHRMOTION_STATE_WARP)
			{
				//워프 동작
				frame += SERVER_CHAR_FRAME_STEP;
				if (frame > MotionInfo->EndFrame * 160)
				{
					Close();
					return TRUE;
				}
				SavePlayBuff();
				FrameCounter += 3;
				break;
			}
			///////////////////////////////////////////////////////////////////

			int TalkCnt = 0;
			if (MotionInfo->State == CHRMOTION_STATE_WALK)
			{
				x = pX; z = pZ;
				MoveAngle(SERVER_CHAR_SKIP);

				if (x == pX && z == pZ)
				{
					TalkCnt = rand() % 100;
					TalkCnt = smMotionInfo->NpcMotionRateCnt[TalkCnt];

					if (TalkCnt == action && smMotionInfo->MotionInfo[TalkCnt].State == CHRMOTION_STATE_WALK)
					{
						//거리를 벗어난경우 처리
						x = (pX - PosiAround.x) >> FLOATNS;
						z = (pZ - PosiAround.z) >> FLOATNS;
						dist = x * x + z * z;
						if (dist > DistAroundDbl)
							SetTargetPosi(PosiAround.x >> FLOATNS, PosiAround.z >> FLOATNS);
					}
					else
					{
						frame = MotionInfo->EndFrame * 160;
						MotionInfo->State = CHRMOTION_STATE_STAND;
					}
				}
			}

			frame += SERVER_CHAR_FRAME_STEP;
			if (frame > MotionInfo->EndFrame * 160)
			{
				if (MotionInfo->State == CHRMOTION_STATE_WALK)
					frame = (MotionInfo->StartFrame * 160) + (frame - MotionInfo->EndFrame * 160);
				else if (smCharInfo.dwCharSoundCode != snCHAR_SOUND_FURY)	// 퓨리는 걷지 않게
				{
					if (TalkCnt == 0)
						TalkCnt = rand() % 100;

					ChangeMotion(smMotionInfo->NpcMotionRateCnt[TalkCnt]);
					if (MotionInfo->State == CHRMOTION_STATE_WALK)
					{
						//거리를 벗어난경우 처리
						x = (pX - PosiAround.x) >> FLOATNS;
						z = (pZ - PosiAround.z) >> FLOATNS;
						dist = x * x + z * z;
						if (dist > DistAroundDbl)
							SetTargetPosi(PosiAround.x >> FLOATNS, PosiAround.z >> FLOATNS);
					}
					else
						Angle.y = rand() % ANGCLIP;
				}
				else // 퓨리 전용.
					SetMotionFromCode(CHRMOTION_STATE_STAND);
			}
		}
		//######################################################################################
		else // NpcMotionRate 값이 없다. (기존 NPC )
		{
			if (MotionInfo->State == CHRMOTION_STATE_WARP) {
				//워프 동작
				frame += SERVER_CHAR_FRAME_STEP;

				if (frame > MotionInfo->EndFrame * 160) {
					Close();
					return TRUE;
				}
				SavePlayBuff();
				FrameCounter += 3;
				break;
			}


			if (MotionInfo->State && MotionInfo->State < 0x100) {
				if (MotionInfo->State == CHRMOTION_STATE_WALK) {
					x = pX; z = pZ;

					MoveAngle(SERVER_CHAR_SKIP);

					if (x == pX && z == pZ) {
						Angle.y = rand() % ANGCLIP;
					}

					if (MotionInfo->State != CHRMOTION_STATE_WALK) SetMotionFromCode(CHRMOTION_STATE_WALK);
				}
				else {
					if (MotionInfo->State != CHRMOTION_STATE_STAND) SetMotionFromCode(CHRMOTION_STATE_STAND);
				}
			}

			frame += SERVER_CHAR_FRAME_STEP;

			if (frame > MotionInfo->EndFrame * 160) {

				if (MotionInfo->State == CHRMOTION_STATE_DEAD) {
					frame = MotionInfo->EndFrame * 160;
				}
				else {

					if (MotionInfo->Repeat == TRUE) {
						frame = (MotionInfo->StartFrame * 160) + (frame - MotionInfo->EndFrame * 160);
					}
					else SetMotionFromCode(CHRMOTION_STATE_STAND);



					if (MotionInfo->State == CHRMOTION_STATE_STAND && smCharInfo.dwCharSoundCode != snCHAR_SOUND_FURY) {	//퓨리는 걷지 않게
						cnt = rand() % ANGCLIP;
						if (((cnt >> 2) & 3) == 0) {
							//때때로 하는 동작
							if (!SetMotionFromCode(TRUE)) {
								if (MoveSpeed) Angle.y = cnt & ANGCLIP;

								if (DistAroundDbl)
									SetMotionFromCode(CHRMOTION_STATE_WALK);
								else
									SetMotionFromCode(CHRMOTION_STATE_STAND);
							}
						}
						else {
							SetMotionFromCode(CHRMOTION_STATE_STAND);
						}

						if (MotionInfo->State == CHRMOTION_STATE_WALK) {
							//거리를 벗어난경우 처리
							x = (pX - PosiAround.x) >> FLOATNS;
							z = (pZ - PosiAround.z) >> FLOATNS;
							dist = x * x + z * z;
							if (dist > DistAroundDbl) {
								SetTargetPosi(PosiAround.x >> FLOATNS, PosiAround.z >> FLOATNS);
							}
						}
					}
					else SetMotionFromCode(CHRMOTION_STATE_STAND);
					/*
									if ( OverLapPosi && lpCharOverLap ) {
										//캐릭터가 겹쳐 있음 ( 떨어지도록 노력 함 )
										Angle.y = GetRadian2D( lpCharOverLap->pX>>FLOATNS , lpCharOverLap->pZ>>FLOATNS ,
											pX>>FLOATNS , pZ>>FLOATNS );

										SetMotionFromCode( CHRMOTION_STATE_WALK );
										OverLapPosi2 = FALSE;
									}
					*/
				}
			}
		}

		SavePlayBuff();
		FrameCounter += 3;
		break;

		/*
	case 12:	//NPC
		if ( MotionInfo->State == CHRMOTION_STATE_WARP ) {
			//워프 동작
			frame += SERVER_CHAR_FRAME_STEP;

			if ( frame>MotionInfo->EndFrame*160 ) {
				Close();
				return TRUE;
			}
			SavePlayBuff();
			FrameCounter+=3;
			break;
		}


		if ( MotionInfo->State && MotionInfo->State<0x100 ) {
			if ( MotionInfo->State==CHRMOTION_STATE_WALK ) {
				x=pX; z=pZ;

				MoveAngle( SERVER_CHAR_SKIP );

				if ( x==pX && z==pZ ) {
					Angle.y = rand()%ANGCLIP;
				}

				if ( MotionInfo->State!=CHRMOTION_STATE_WALK ) SetMotionFromCode( CHRMOTION_STATE_WALK );
			}
			else {
				if ( MotionInfo->State!=CHRMOTION_STATE_STAND ) SetMotionFromCode( CHRMOTION_STATE_STAND );
			}
		}

		frame += SERVER_CHAR_FRAME_STEP;

		if ( frame>MotionInfo->EndFrame*160 ) {

			if ( MotionInfo->State == CHRMOTION_STATE_DEAD ) {
				frame = MotionInfo->EndFrame*160;
			}
			else {

				if ( MotionInfo->Repeat == TRUE ) {
					frame = (MotionInfo->StartFrame*160)+(frame - MotionInfo->EndFrame*160);
				}
				else SetMotionFromCode( CHRMOTION_STATE_STAND );



				if ( MotionInfo->State==CHRMOTION_STATE_STAND && smCharInfo.dwCharSoundCode!=snCHAR_SOUND_FURY ) {	//퓨리는 걷지 않게
					cnt = rand()%ANGCLIP;
					if ( ((cnt>>2)&3)==0 ) {
						//때때로 하는 동작
							if ( !SetMotionFromCode( TRUE ) ) {
								if ( MoveSpeed ) Angle.y = cnt&ANGCLIP;

								if ( DistAroundDbl )
									SetMotionFromCode( CHRMOTION_STATE_WALK );
								else
									SetMotionFromCode( CHRMOTION_STATE_STAND );
							}
					}
					else {
						SetMotionFromCode( CHRMOTION_STATE_STAND );
					}

					if ( MotionInfo->State==CHRMOTION_STATE_WALK ) {
						//거리를 벗어난경우 처리
						x = (pX-PosiAround.x)>>FLOATNS;
						z = (pZ-PosiAround.z)>>FLOATNS;
						dist = x*x+z*z;
						if ( dist>DistAroundDbl ) {
							SetTargetPosi( PosiAround.x>>FLOATNS , PosiAround.z>>FLOATNS );
						}
					}
				}
				else SetMotionFromCode( CHRMOTION_STATE_STAND );
			}
		}
		SavePlayBuff();
		FrameCounter+=3;
		break;
		*/



	case 99:

		if (PlayBuffCnt == PlayBuffPosi_End) {

			if (MotionInfo->State != CHRMOTION_STATE_STAND && MotionInfo->State != CHRMOTION_STATE_DEAD && MotionInfo->State != CHRMOTION_STATE_WARP &&
				MotionInfo->State != CHRMOTION_STATE_ATTACK && MotionInfo->State != CHRMOTION_STATE_SKILL)
				SetMotionFromCode(CHRMOTION_STATE_STAND);

			frame += 80;
			compFrame = frame / 160;

			if (compFrame >= MotionInfo->EndFrame) {
				if (MotionInfo->Repeat) {
					ChangeMotion(action, AnimDispMode);
				}
				else
					frame = (MotionInfo->EndFrame) * 160;

				if (MotionInfo->State == CHRMOTION_STATE_ATTACK || MotionInfo->State == CHRMOTION_STATE_SKILL)
					SetMotionFromCode(CHRMOTION_STATE_STAND);

			}
			TransDelayCheck();

		}
		else {
			playBuff = &PlayBuff[PlayBuffCnt & PLAYBUFF_MASK];
			OldAction = action;

			Angle.x = playBuff->angle[0];
			Angle.y = playBuff->angle[1];
			Angle.z = playBuff->angle[2];
			action = playBuff->angle[3];

			if (!action) {
				action = OldAction;

				x = abs(pX - playBuff->x);
				y = abs(pY - playBuff->y);
				z = abs(pZ - playBuff->z);

				cnt = x + y + z;

				if (cnt > 32) {
					if (MotionInfo->State != CHRMOTION_STATE_WALK && MotionInfo->State != CHRMOTION_STATE_RUN) {

						if (cnt < 512) {
							//걷기
							SetMotionFromCode(CHRMOTION_STATE_WALK);
						}
						else {
							//뛰기
							if (!SetMotionFromCode(CHRMOTION_STATE_RUN))
								SetMotionFromCode(CHRMOTION_STATE_WALK);
						}
					}
				}
				else {
					if (MotionInfo->State != CHRMOTION_STATE_STAND) {
						SetMotionFromCode(CHRMOTION_STATE_STAND);
					}
				}
			}

			pX = playBuff->x;
			pY = playBuff->y;
			pZ = playBuff->z;

			PHeight = pY;


			if (!playBuff->dwTargetSerial) chrAttackTarget = 0;
			else {
				if (chrAttackTarget && chrAttackTarget->dwObjectSerial == playBuff->dwTargetSerial) {

				}
				else
					chrAttackTarget = FindChrPlayer(playBuff->dwTargetSerial);
			}

			if (playBuff->frame < smPLAYBUFF_FRAME_NONE) {
				//				if ( OldAction!=action ) {// || !MotionInfo->State || MotionInfo->State>=0x100 ) {

								//프레임 설정
				frame = playBuff->frame & CHAR_FRAME_MASK;

				//해당 프레임의 모델선택
				MotionSelectFrame = playBuff->frame >> CHAR_FRAME_SELSHIFT;
				if (AnimDispMode) {
					if (MotionSelectFrame)
						PatDispMode = DISP_MODE_PATSUB;			//보조 모델
					else {
						PatDispMode = DISP_MODE_PATMAIN;		//주 모델
					}
				}

				if (action) {

					if (PatDispMode == DISP_MODE_PATSUB)
						MotionInfo = &smMotionInfo2->MotionInfo[action];
					else
						MotionInfo = &smMotionInfo->MotionInfo[action];

					//######################################################################################
					//작 성 자 : 오 영 석
					FrameInfo.StartFrame = MotionInfo->StartFrame * 160;
					FrameInfo.EndFrame = MotionInfo->EndFrame * 160;
					//######################################################################################
				}

				if (OldMotionState != MotionInfo->State) {

					if (MotionInfo->State >= 0x100 && MotionInfo->State != CHRMOTION_STATE_ATTACK && MotionInfo->State != CHRMOTION_STATE_SKILL) {
						//if ( MotionInfo->State!=CHRMOTION_STATE_DEAD || (OnStageField>=0 && StageField[OnStageField]->State!=FIELD_STATE_VILLAGE) ) {
							//캐릭터 사운드
						//}
					}
					/*
										//다시 시작 이펙트
										if ( MotionInfo->State==CHRMOTION_STATE_DAMAGE )
											//피튀기는 파티클
											StartEffect( pX, pY+18*fONE, pZ, 50,50 , EFFECT_DAMAGE1 );
					*/
					//if ( AttackSkil && !MotionEvent && (OldMotionState==CHRMOTION_STATE_ATTACK || OldMotionState==CHRMOTION_STATE_SKILL) ) {
					if (!MotionEvent && (OldMotionState == CHRMOTION_STATE_ATTACK || OldMotionState == CHRMOTION_STATE_SKILL)) {
						if (MotionInfo->EventFrame[0]) {
							EventAttack();				//스킵된 이벤트 실행
							//PlayAttack(TRUE);									//공격 적용
							dwAttackPlayTime = 0;		//원거리 캐릭의 공격은 제외
							ResetAttackTrans();		//AttackTrans.code = 0;
						}
						PlayAttack();									//공격 적용
					}
					MotionEvent = 0;

					if (playBuff->wStickItems[smPLAYBUFF_ITEM_SKIL] >= 0) {
						//스킬시작
						BeginSkill(playBuff->wStickItems[smPLAYBUFF_ITEM_SKIL], 0, chrAttackTarget, 0, 0, 0);
						//AttackSkil = playBuff->wStickItems[smPLAYBUFF_ITEM_SKIL];
					}

					if (smCharInfo.State == smCHAR_STATE_ENEMY) {
						if (MotionInfo->State == CHRMOTION_STATE_SKILL)
							BeginSkill(0, 0, chrAttackTarget, 0, 0, 0);

						if (MotionInfo->State == CHRMOTION_STATE_ATTACK)
							BeginAttack_Monster();

						if (OldMotionState == CHRMOTION_STATE_SKILL) {
							PlayAttack();									//공격 적용
						}

					}

					switch (MotionInfo->State) {
					case CHRMOTION_STATE_RESTART:
						break;
					case CHRMOTION_STATE_WARP:
						break;

					case CHRMOTION_STATE_DEAD:			//캐릭 죽음동작 시작
						FrameCounter = 0;
						break;
					}
				}
			}
			else {
				frame += playBuff->frame & 0xFFFF;
				//frame += 80;
			}

			/*
						if ( PatDispMode==DISP_MODE_PATSUB && MotionInfo->State<0x100  ) {

							compFrame = frame / 160;

						}
			*/

			AttackSkil = playBuff->wStickItems[smPLAYBUFF_ITEM_SKIL];
			EventAttack();			//공격 이벤트

			if (smCharInfo.State != smCHAR_STATE_USER &&
				(MotionInfo->State == CHRMOTION_STATE_WALK || MotionInfo->State == CHRMOTION_STATE_RUN)) {
				//발자국 소리 ( 바곤 및 거대 몬스터 )
				if (MotionInfo->EventFrame[0]) {
					compFrame = frame - (MotionInfo->StartFrame * 160);
					if (MotionInfo->EventFrame[0] > (compFrame - 80) && MotionInfo->EventFrame[0] <= compFrame) {
					}
				}

				if (MotionInfo->EventFrame[1]) {
					compFrame = frame - (MotionInfo->StartFrame * 160);
					if (MotionInfo->EventFrame[1] > (compFrame - 80) && MotionInfo->EventFrame[1] <= compFrame) {
					}
				}
			}

			FrameStep = 80;

			if (MotionInfo->State == CHRMOTION_STATE_EAT) {
				//물약 먹기 동작
				if (MotionInfo->EventFrame[0]) {
					compFrame = frame - (MotionInfo->StartFrame * 160);
					if (MotionInfo->EventFrame[0] > (compFrame - FrameStep) && MotionInfo->EventFrame[0] <= compFrame) {
						if (smCharInfo.State == smCHAR_STATE_USER)
							cnt = pY + 48 * fONE;
						else
							cnt = pY + PatHeight;


					}
				}
			}

			compFrame = frame / 160;

			if (compFrame < MotionInfo->StartFrame)
				frame = MotionInfo->StartFrame * 160;


			if (compFrame >= MotionInfo->EndFrame) {
				if (MotionInfo->Repeat) {
					ChangeMotion(action, AnimDispMode);
				}
				else
					frame = (MotionInfo->EndFrame) * 160;

				AttackEffect = 0;
			}

			/*
						if ( compFrame>=smMotionInfo->MotionInfo[action].EndFrame ) {
							if ( smMotionInfo->MotionInfo[action].Repeat ) {
								ChangeMotion( action );
							}
							else
								frame = (smMotionInfo->MotionInfo[action].EndFrame)*160;
						}
			*/

			////// 장착한 아이템 코드 확인하여 변경 사항 적용 ////////////
			if ((smCharInfo.State == smCHAR_STATE_USER || PkMode_CharState == smCHAR_STATE_USER) && DisplayFlag) {

				// 왼쪽 장착 아이템
				if (playBuff->wStickItems[smPLAYBUFF_ITEM_LEFT] != wStickItems[smPLAYBUFF_ITEM_LEFT]) {
					if (playBuff->wStickItems[smPLAYBUFF_ITEM_LEFT] >= 0) {
						SetTool(sItem[playBuff->wStickItems[smPLAYBUFF_ITEM_LEFT]].CODE, hvPOSI_LHAND);
						HvLeftHand.ColorBlink = 0;
						HvLeftHand.DispEffect = 0;
					}
					else
						SetTool(0, hvPOSI_LHAND);
				}
				// 오른쪽 장착 아이템
				if (playBuff->wStickItems[smPLAYBUFF_ITEM_RIGHT] != wStickItems[smPLAYBUFF_ITEM_RIGHT]) {
					if (playBuff->wStickItems[smPLAYBUFF_ITEM_RIGHT] >= 0) {
						SetTool(sItem[playBuff->wStickItems[smPLAYBUFF_ITEM_RIGHT]].CODE, hvPOSI_RHAND);
						HvRightHand.ColorBlink = 0;
						HvRightHand.DispEffect = 0;
					}
					else
						SetTool(0, hvPOSI_RHAND);
				}
				// 몸 장착 아이템( 갑옷 등 )
				if (playBuff->wStickItems[smPLAYBUFF_ITEM_BODY] != wStickItems[smPLAYBUFF_ITEM_BODY]) {
					SetChrArmor(this, playBuff->wStickItems[smPLAYBUFF_ITEM_BODY]);
					wStickItems[smPLAYBUFF_ITEM_BODY] = playBuff->wStickItems[smPLAYBUFF_ITEM_BODY];
				}
			}



			PlayBuffCnt++;
			MoveCnt = 0;

			//분노치 변화
			//if ( (( MotionInfo->State==CHRMOTION_STATE_ATTACK || MotionInfo->State==CHRMOTION_STATE_DAMAGE )&& 
			//	PatDispMode==DISP_MODE_PATSUB) ) {
			if (((MotionInfo->State == CHRMOTION_STATE_ATTACK || MotionInfo->State == CHRMOTION_STATE_DAMAGE) &&
				smCharInfo.dwCharSoundCode == snCHAR_SOUND_MINIG)) {
				AttackAnger += 2;
				if (AttackAnger > 100) AttackAnger = 100;
			}
			else {
				AttackAnger -= 1;
				if (AttackAnger <= 0) AttackAnger = 0;
			}

			if (AttackIce != 0) {
				//얼음 공격
				if (AttackIce > 0 && AttackIce < 160) AttackIce++;
				if (AttackIce < 0) AttackIce++;
				if (MotionInfo->State == CHRMOTION_STATE_DEAD) AttackIce = 0;
			}

			if (DispPoison != 0) {
				//독에 감염
				if (DispPoison > 0 && DispPoison < 100) DispPoison++;
				if (DispPoison < 0) DispPoison++;
				if (MotionInfo->State == CHRMOTION_STATE_DEAD) DispPoison = 0;
			}

			if (PlayDistortion > 0) {
				//왜곡 기능
				PlayDistortion--;
			}

			//바닥 확인용
			if ((PlayBuffCnt & 0x3F) == 0 && smCharInfo.State != TRUE) {
				OnStageField = -1;
				if (smGameStage[0]) {
					cnt = smGameStage[0]->GetFloorHeight(pX, pY, pZ, Pattern->SizeHeight);
					OnStageField = 0;
				}
				if (cnt == CLIP_OUT && smGameStage[1]) {
					OnStageField = 1;
				}
			}

			if (dwAttackPlayTime && dwAttackPlayTime < dwPlayTime) {
				//공격 시간예약 적용
				PlayAttack(TRUE);
				dwAttackPlayTime = 0;
			}


			OldMotionState = MotionInfo->State;


			//게임 데이타 전송이 처음 되었을때 캐릭터 화면에 표시
			DisplayFlag = TRUE;
		}

		if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_WOLVERLIN && smCharInfo.State == smCHAR_STATE_ENEMY) {
			if ((Counter & 0x1) == 0 && MotionInfo->State != CHRMOTION_STATE_DEAD && MotionInfo->State == CHRMOTION_STATE_RUN) {
				GetMoveLocation(0, 0, ((rand() % 48) - 24) * fONE, 0, Angle.y, 0);
			}
			if (MotionInfo->State == CHRMOTION_STATE_DEAD) {
				if (sAddColorEffect[SMC_R] > 0) sAddColorEffect[SMC_R]--;
				if (sAddColorEffect[SMC_G] > 0) sAddColorEffect[SMC_G]--;
				if (sAddColorEffect[SMC_B] > 0) sAddColorEffect[SMC_B]--;
			}
		}

		if (smCharInfo.wPlayClass[0] == MONSTER_CLASS_BOSS) {
			//보스몬스터 죽음
			if (MotionInfo->State == CHRMOTION_STATE_DEAD) {
				if (sAddColorEffect[SMC_R] > 0) sAddColorEffect[SMC_R]--;
				if (sAddColorEffect[SMC_G] > 0) sAddColorEffect[SMC_G]--;
				if (sAddColorEffect[SMC_B] > 0) sAddColorEffect[SMC_B]--;
			}
			/*
			else {
				if ( RendSucess && RendPoint.z>0 && RendPoint.z<64*14*fONE && (Counter&1)==0 ) {
					sinSetEffect( SIN_PARTICLE_ROTATE_RISE ,  pX,pY ,pZ , 1000 , 0 , 1500+(smCharInfo.SizeLevel*500) );
				}
			}
			*/
		}

		if (smCharInfo.wPlayClass[1]) {
			if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_HOBGOBLIN && smCharInfo.Level == 60) {
				//풍선같은 산타 고블린 (맞을수록 커진다)
				cnt = ((smCharInfo.Life[1] - smCharInfo.Life[0]) * 460) / smCharInfo.Life[1];
				cnt += 120;
				smCharInfo.wPlayClass[1] = cnt;
			}
		}

		// 박재원 - 미리 모리프 이벤트(모리프를 사냥하면 모리프가 점점 커지고 미니 모리프가 나온다)
		if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_NPC_MORIF && smCharInfo.Level == 60) {
			//풍선같이 점점 커지는 모리프 (맞을수록 커진다)
			cnt = ((smCharInfo.Life[1] - smCharInfo.Life[0]) * 700) / smCharInfo.Life[1];
			cnt += 300;
			smCharInfo.wPlayClass[1] = cnt;
		}

		if (smCharInfo.Life[0] != DispLifeBar) {
			if (DispLifeBar < smCharInfo.Life[0]) DispLifeBar++;
			else DispLifeBar--;
		}

		if (MotionInfo->State == CHRMOTION_STATE_WARP) {
			if (FrameCounter > 10 && smCharInfo.dwCharSoundCode == snCHAR_SOUND_WOLVERLIN) {
				if (sAddColorEffect[SMC_A] > -256) sAddColorEffect[SMC_A] -= 8;
				if (sAddColorEffect[SMC_R] > 0) sAddColorEffect[SMC_R] -= 8;
				if (sAddColorEffect[SMC_G] > 0) sAddColorEffect[SMC_G] -= 8;
				if (sAddColorEffect[SMC_B] > 0) sAddColorEffect[SMC_B] -= 8;
				DisplayAlpha = -16;
			}
		}

		//무기 속성 이펙트
		if (dwWeaponEffectTime) {
			if (dwWeaponEffectTime < dwPlayTime) {
				dwWeaponEffectTime = 0;
				WeaponEffect = 0;
			}
		}

		if (PlayVanish > 0) PlayVanish--;
		if (PlayInvincible > 0)	PlayInvincible--;		//무적 아이템 사용중

		//클랜 정보 업데이트 확인
		if (smCharInfo.ClassClan && ClanInfoNum < 0 && dwClanInfoTime && (dwClanInfoTime + 60 * 1000) < dwPlayTime) {
			//클랜원인데 클랜정보가 업데이트 되지 않은경우 1 분 마다 재 시도
			ClanInfoNum = ReadClanInfo(smCharInfo.ClassClan);
			dwClanInfoTime = dwPlayTime;
		}
		break;

	case 101:		//확대 캐릭터

		frame += 80;
		compFrame = frame / 160;

		if (compFrame >= MotionInfo->EndFrame)
		{
			SetMotionFromCode(CHRMOTION_STATE_STAND);
		}

		if (TalkFrame && lpTalkMotionInfo)
		{
			if (TalkFrame >= lpTalkMotionInfo->EndFrame * 160)
				TalkFrame = 0;
			else
				TalkFrame += 80;
		}
		//######################################################################################
		break;

	}

	if (ShootingFlag) {
		//발사형 무기 메인함수
		ShootingMain();
	}

	if (DisplayAlpha < 0) {
		DisplayAlpha += 4;
		if (DisplayAlpha > 0) DisplayAlpha = 0;
	}
	if (DisplayAlpha > 0) {
		DisplayAlpha -= 4;
		if (DisplayAlpha < 0) DisplayAlpha = 0;
	}


	Counter++;
	FrameCounter++;

	return TRUE;
}

#define VIEW_HIGH_DIST	36864			//(64*3)		
#define VIEW_MID_DIST	102400			//(64*5)		


int bb_r, bb_g, bb_b, bb_a;

int SetRenderBlinkColor(smCHARTOOL* smCharTool) { return TRUE; }

int RestoreRenderBlinkColor() { return TRUE; }

//가상으로 그린다 ( 실제로는 그리지 않고 값만 세팅 )
int smCHAR::VirtualDraw() { return TRUE; }

int smCHAR::Draw() { return TRUE; }

//그림자 그리기
int smCHAR::DrawShadow() { return TRUE; }


//######################################################################################
//작 성 자 : 오 영 석
void smCHAR::SetIceFoot(int TypeNum, DWORD dwPlayTime, int x, int z)
{
	if (smGameStage[OnStageField] == NULL)
		return;
	int loop = 0;
	for (loop = 0; loop < ICE_FOOT_COUNT_MAX; loop++)
	{
		if (m_IceFootInfo[loop].Life == 0)
			break;
	}

	if (loop >= ICE_FOOT_COUNT_MAX)
		return;

	POINT3D FootPos;
	FootPos.x = pX + x;
	//FootPos.y = PHeight;
	FootPos.y = pY + (10 * fONE);
	FootPos.z = pZ + z;

	POINT3D FaceAngle;
	if (smGameStage[OnStageField]->CheckFaceIceFoot(&FootPos, &FaceAngle, 32 * fONE) == 0)
		return;

	m_IceFootInfo[loop].Life = 1;
	m_IceFootInfo[loop].dwPlayTime = dwPlayTime;
	m_IceFootInfo[loop].TypeNum = TypeNum;
	m_IceFootInfo[loop].FootPos.x = FootPos.x;
	m_IceFootInfo[loop].FootPos.y = FootPos.y;
	m_IceFootInfo[loop].FootPos.z = FootPos.z;
	m_IceFootInfo[loop].FootAngle.x = -(FaceAngle.x - ANGLE_180) & ANGCLIP;
	m_IceFootInfo[loop].FootAngle.y = (-Angle.y) & ANGCLIP;
	m_IceFootInfo[loop].FootAngle.z = (FaceAngle.z - ANGLE_180) & ANGCLIP;
}

//
int smCHAR::DrawIceFoot(void) { return TRUE; }



#define STATE_BAR_WIDTH		100
#define STATE_BAR_HEIGHT	16
#define STATE_BAR_SIZE		92


// 체력바 그리기
int smCHAR::DrawStateBar(int x, int y) { return TRUE; }


// 체력바 그리기
int smCHAR::DrawStateBar2(int x, int y) { return TRUE; }


// 차징 게이지 그리기
int smCHAR::DrawChargingBar(int x, int y, int Charging, int ChargingMax) { return TRUE; }

//움직임에 잔상 남기기
int smCHAR::DrawMotionBlur() { return TRUE; }



//무기 움직임에 잔상 남기기
int smCHAR::DrawMotionBlurTool(smCHARTOOL* ChrTool) { return TRUE; }





//캐릭터에 패턴을 설정
int smCHAR::SetLoadPattern(char* szName)
{
	Pattern = 0;
	Flag = 1;
	return AddLoaderPattern((smCHAR*)&Head, szName);

}

//받은 데미지 암화화 코드
DWORD smCHAR::GetAttackTrans_XorCode()
{
	return dwObjectSerial ^ lpCurPlayer->smCharInfo.dwObjectSerial ^ (((DWORD)this) >> 8);
}

//받은데미지 수치 초기화
int smCHAR::ResetAttackTrans()
{

	AttackTrans.code = GetAttackTrans_XorCode();
	return TRUE;
}

//받은데미지 수치 암호화 반전
int smCHAR::XorAttackTrans()
{
	int cnt, len;
	DWORD dwCode;

	len = (sizeof(TRANS_ATTACKDATA) / sizeof(DWORD));//+2;
	DWORD* lpdwBuff = (DWORD*)&AttackTrans;

	dwCode = GetAttackTrans_XorCode();

	lpdwBuff[0] ^= dwCode;
	lpdwBuff[1] ^= dwCode;

	for (cnt = 2; cnt < len; cnt++) {
		lpdwBuff[cnt] ^= dwCode;
		dwCode += dwCode;
	}

	return TRUE;
}


//공격 실행
int smCHAR::PlayAttack(int EventFlag)
{
	int cnt, len, act;
	int block, ChanceBlock;
	smCHAR* lpChar;
	int	HolyBody_Undead = 0;
	int x, y, z;
	POINT3D	Pos1;//,Pos2;
	int temp;
	int Param1, Param2;
	int	bStunCount;


	TRANS_ATTACKDATA* lpTransAttackData;

	lpTransAttackData = &AttackTrans;

	XorAttackTrans();

	if (!lpTransAttackData->code || (RestartPlayCount > 0 && RestartPlayCount < 1000) ||
		(UseEtherCoreCode && lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_EAT)) {
		//ResetAttackTrans();//lpTransAttackData->code = 0;
		AttackTrans.code = GetAttackTrans_XorCode();
		if (RestartPlayCount) Ptect_IncAttackCount(0);
		return FALSE;
	}

	if (lpTransAttackData->dwDamageChkSum != dm_GetDamgeChkSum_S2V(lpTransAttackData)) {	//코드 검사 오류
		ResetAttackTrans();
		SendSetHackUser3(8580, lpTransAttackData->Power, 0);
		return FALSE;
	}

	Ptect_IncAttackCount(-1);

	if (lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD && DisplayFlag) {
		len = GetDistanceDbl(lpCurPlayer->pX, lpCurPlayer->pZ, pX, pZ);
		cnt = GetDistanceDbl(lpCurPlayer->pX, lpCurPlayer->pZ, lpTransAttackData->x, lpTransAttackData->z);

		if (smCharInfo.State == smCHAR_STATE_USER || PkMode_CharState == smCHAR_STATE_USER) {
			len = 0;			//PK 모드
			cnt = 0;
		}

		if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_FIGON || smCharInfo.dwCharSoundCode == snCHAR_SOUND_WEB ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_SKELETONRANGE || smCharInfo.dwCharSoundCode == snCHAR_SOUND_IMP ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_SCORPION ||

			smCharInfo.dwCharSoundCode == snCHAR_SOUND_NAZ ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_NAZSENIOR || // 박재원 - 끝없는 탑 3층(신규필드 몬스터)
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_SHADOW ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_BEEDOG ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_MUTANTTREE ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_SUCCUBUS ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_BERSERKER ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_STYGIANLORD ||

			smCharInfo.dwCharSoundCode == snCHAR_SOUND_AVELIN_ARCHER ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_AVELIN_QUEEN ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_DMACHINE ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_SADNESS ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_RUNICGUARDIAN ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_HYPERMACHINE ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_TURTLE_CANNON ||


			smCharInfo.dwCharSoundCode == snCHAR_SOUND_FROZEN ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_CHAOSCARA ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_COLDEYE ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_M_LORD ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_KELVEZU ||

			smCharInfo.dwCharSoundCode == snCHAR_SOUND_DARKGUARD ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_DARKMAGE ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_FIREWORM ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_SETO ||

			smCharInfo.dwCharSoundCode == snCHAR_SOUND_NPC_MAGICMASTER ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_NPC_SKILLMASTER ||

			smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_R ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_G ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_B ||

			smCharInfo.dwCharSoundCode == snCHAR_SOUND_GOBLINSHAMAN || // 박재원 - 저주받은 신전 3층(신규필드 몬스터) - 고블린샤먼

			lpTransAttackData->AttackState == 0x80						//스킬공격
			) {
			//원거리 공격
			if (len < (460 * 460)) len = 0;

			cnt = 0;

			if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_WEB) {
				lpCurPlayer->PlaySlowCount = 70 * 2;			//2초동안 느리게 움직임
				lpCurPlayer->PlaySlowSpeed = 180;
			}
			if (!EventFlag) { //&& !dwAttackPlayTime ) {	
				dwAttackPlayTime = dwPlayTime + 1100;
				XorAttackTrans();
				return TRUE;
			}
		}

		if (lpTransAttackData->sRating[0] < 0) {
			Param1 = -lpTransAttackData->sRating[0];
			Param2 = lpTransAttackData->sRating[1];
			lpTransAttackData->sRating[0] = 0;
			lpTransAttackData->sRating[1] = 0;
		}
		else {
			Param1 = 0;
			Param2 = 0;
		}


		//주인공 몬스터 스킨일 경우
		if (lpCurPlayer->smCharInfo.State == smCHAR_STATE_ENEMY) act = 1;
		else act = 0;

		if (lpTransAttackData->AttackState == 0x80) {
			x = (lpTransAttackData->x - lpCurPlayer->pX) >> FLOATNS;
			y = (lpTransAttackData->y - lpCurPlayer->pY) >> FLOATNS;
			z = (lpTransAttackData->z - lpCurPlayer->pZ) >> FLOATNS;
			if ((x * x + y * y + z * z) > (140 * 140)) {
				cnt = lpTransAttackData->AttackSize;
			}
			act = 1;			//몬스터의 스킬공격 ( 명중 100% )
		}

		Record_RecvDamageCount++;			//데미지 카운터

		if (cnt < lpTransAttackData->AttackSize && sinGetLife()>0 && (len < (330 * 330) || act) && DisplayAlpha == 0) {
			/*
							lpChar = 0;

							//공격한 상대방을 찾아 방향을 바꾼다
							for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
								if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial==lpTransAttackData->dwDestObjectSerial ) {
									lpChar = &chrOtherPlayer[cnt];
									break;
								}
							}
			*/

			lpChar = this;

			block = FALSE;

			if (lpChar) {

				if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_ARCHER) {			//아처
					cnt = cSkill.GetSkillPoint(SKILL_EVASION_MASTERY);			//추가 회피 마스터리
					if (cnt > 0 && cnt <= 10) {
						cnt = Evasion_Mastery_AddPercent[cnt - 1];
						if (cnt <= 26) { // 장별 - 스킬밸런스 조정(10.08.10)
							if ((rand() % 100) <= cnt) {
								//추가회피 성공
								block = 100;
								Record_BlockCount++;
							}
						}
					}
				}



				if (lpCurPlayer->smCharInfo.Chance_Block) {
					ChanceBlock = sinGetBlockRating(lpChar->smCharInfo.Level);

					if ((lpCurPlayer->HvLeftHand.dwItemCode & sinITEM_MASK2) == sinDS1) {
						if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_MECHANICIAN && (lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_EXTREME_SHIELD) != 0) {		//익스트림 쉴드
							ChanceBlock += (lpTransAttackData->AttackState >> 16);
						}
						if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_KNIGHT && (lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_SPARK_SHIELD) != 0) {		//익스트림 쉴드
							ChanceBlock += (lpTransAttackData->AttackState >> (16 + 8));
						}
						if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_KNIGHT && (lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_GODLY_SHIELD) != 0) {		//가들리 쉴드
							cnt = cSkill.GetSkillPoint(SKILL_DIVINE_INHALATION);		//디바인 쉴드 인계
							if (cnt > 0 && D_Inhalation_Block[cnt - 1] <= 10) {
								ChanceBlock += D_Inhalation_Block[cnt - 1];
							}
						}
						if (ChanceBlock > 50) ChanceBlock = 50;
					}
				}
				else
					ChanceBlock = 0;

				if ((rand() % 100) < ChanceBlock) {
					//블럭성공
					block = TRUE;
					Record_BlockCount++;

					if (abs(lpChar->smCharInfo.Level - lpCurPlayer->smCharInfo.Level) <= AGING_SUB_LEVEL) {
						//레벨 7차이 이하는 에이징 적용
						sinCheckAgingLevel(SIN_AGING_BLOCK, false);
					}

					if ((lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_SPARK_SHIELD) && lpChar) {

						if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_MECHANICIAN) {
							//스파크 쉴드 적용
							dm_SendTransDamage(lpChar, 0, 0, 0, 0, SKILL_PLAY_SPARK_SHIELD | 0x100, FALSE);
							/*
							//스파크 쉴드
							if ( lpCurPlayer->HvLeftHand.PatTool ) {
								if ( lpCurPlayer->GetToolBipPoint( &lpCurPlayer->HvLeftHand , &x,&y,&z ) ) {

									Pos1.x = x;//lpCurPlayer->pX;
									Pos1.y = y;//lpCurPlayer->pY;
									Pos1.z = z;//lpCurPlayer->pZ;

									Pos2.x = lpChar->pX;
									Pos2.y = lpChar->pY+24*fONE;
									Pos2.z = lpChar->pZ;

									AssaParticle_SparkShieldDefence( &Pos1 , &Pos2 );
									SkillPlaySound( SKILL_SOUND_SKILL_SPARK2	 , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ );
								}
							}
							*/
						}
						//디바인 인홀레이션
						if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_KNIGHT) {
							cnt = (lpTransAttackData->AttackState >> 16) & 0xFF;
							if (cnt > 0) {
								sinSetLife((sinGetLife() + cnt));
								ResetEnergyGraph(0);						//에너지 그래프 첵크 초기화

								if (lpCurPlayer->HvLeftHand.PatTool) {
									if (lpCurPlayer->GetToolBipPoint(&lpCurPlayer->HvLeftHand, &x, &y, &z)) {
										Pos1.x = x;
										Pos1.y = y;
										Pos1.z = z;

									}
								}
							}
						}
					}
				}
			}

			//블럭율 계산
			if (lpChar && (!block || act)) {

				//방어율 계산
				//cnt = GetPlayDefence( &smCharInfo , &lpCurPlayer->smCharInfo );
				if (sizeof(TRANS_ATTACKDATA) == lpTransAttackData->size)
					cnt = sinGetMonsterAccuracy(lpTransAttackData->sRating[0], lpTransAttackData->sRating[1]);
				else
					cnt = sinGetMonsterAccuracy(smCharInfo.Level, smCharInfo.Attack_Rating);

				if (dmAttackCount < 2) cnt <<= 2;			//공격을 안할시에는 방어률 절반

				//cnt = 100;
				if ((rand() % 100) <= cnt || act) {

					cnt = lpTransAttackData->Power;

					if ((lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_HOLY_BODY)) {
						if (smCharInfo.Brood == smCHAR_MONSTER_UNDEAD) {
							//홀리 바디 스킬 적용 ( 언데드만 적용 )
							//cnt -= (cnt*HolyBody_Absorb[DispApp_SkillPoint-1])/100;
							HolyBody_Undead = TRUE;
						}
					}

					if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_MAGICIAN) {
						//에너지 쉴드 적용 확인
						temp = lpTransAttackData->AttackState >> 16;
						if (temp > 0) {
							temp = sinGetMana() - temp;
							if (temp < 0) temp = 0;
							sinSetMana(temp);
							ResetEnergyGraph(1);						//에너지 그래프 첵크 초기화
						}
					}

					if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_PRIESTESS) {
						//서몬 무스펠
						if (lpChar->smCharInfo.Brood == smCHAR_MONSTER_UNDEAD && (lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_GODLY_SHIELD) != 0) {
							temp = lpTransAttackData->AttackState >> 16;
							if (temp > 0) {
								temp = sinGetLife() + temp;
								sinSetLife(temp);
								ResetEnergyGraph(0);						//에너지 그래프 첵크 초기화
							}
						}
					}

					bStunCount = lpCurPlayer->PlayStunCount;
					lpCurPlayer->PlayStunCount = 0;

					/////////////////// PKMODE ///////////////////
					switch (Param1) {
					case SKILL_PLAY_ROAR:
					case SKILL_PLAY_SHIELD_STRIKE:
						if (!bStunCount) {
							lpCurPlayer->PlayStunCount = GetResistancePower(Param2 * 70, sITEMINFO_BIONIC);
						}
						break;
					case SKILL_PLAY_GROUND_PIKE:
						lpCurPlayer->PlaySlowCount = GetResistancePower(Param2 * 70, sITEMINFO_ICE);
						lpCurPlayer->PlaySlowSpeed = 180;
						lpCurPlayer->AttackIce = 1;
						break;
					case SKILL_PLAY_GLACIAL_SPIKE:
					case SKILL_PLAY_ICE_CRYSTAL:		//공성 얼음탑
						lpCurPlayer->PlaySlowCount = GetResistancePower(Param2 * 70, sITEMINFO_ICE);
						lpCurPlayer->PlaySlowSpeed = 200;
						lpCurPlayer->AttackIce = 1;
						break;
					case SKILL_PLAY_DISTORTION:
						lpCurPlayer->PlaySlowCount = Param2 * 70;
						lpCurPlayer->PlaySlowSpeed = 210;
						lpCurPlayer->PlayDistortion = lpCurPlayer->PlaySlowCount;
						lpCurPlayer->AttackIce = 0;
						break;
					case SKILL_PLAY_ENCHANT_WEAPON:
						lpCurPlayer->PlaySlowCount = GetResistancePower(100, sITEMINFO_ICE);
						lpCurPlayer->PlaySlowSpeed = Param2 * 70;
						lpCurPlayer->AttackIce = 1;
						break;
					case SKILL_PLAY_DANCING_SWORD:
						lpCurPlayer->PlaySlowCount = GetResistancePower(140, sITEMINFO_ICE);
						lpCurPlayer->PlaySlowSpeed = Param2 * 70;
						lpCurPlayer->AttackIce = 1;
						break;
					case SKILL_PLAY_FROST_JAVELIN:
						lpCurPlayer->PlaySlowCount = GetResistancePower(210, sITEMINFO_ICE);
						lpCurPlayer->PlaySlowSpeed = Param2 * 70;
						lpCurPlayer->AttackIce = 1;
						break;
					case SKILL_PLAY_FIRE_CRYSTAL:		//공성 불탑
						cnt = GetResistancePower(cnt, sITEMINFO_FIRE);
						break;
					case SKILL_PLAY_LIGHTNING_CRYSTAL:	//공성 전기탑
						if (!bStunCount) {
							lpCurPlayer->PlayStunCount = GetResistancePower(Param2 * 70, sITEMINFO_LIGHTING);
						}
						break;
					}


					switch (smCharInfo.dwCharSoundCode) {
					case snCHAR_SOUND_COLDEYE:
					case snCHAR_SOUND_FROZEN:
						//case snCHAR_SOUND_ICEGOLEM:
						if ((lpTransAttackData->AttackState & 0xFF) == 0x80)						//스킬공격
							lpCurPlayer->PlaySlowCount = GetResistancePower(70 * 4, sITEMINFO_ICE);
						lpCurPlayer->PlaySlowSpeed = 200;
						lpCurPlayer->AttackIce = 1;
						break;
					case snCHAR_SOUND_FROST:
					case snCHAR_SOUND_DEATHKNIGHT:
						if ((lpTransAttackData->AttackState & 0xFF) == 0x80)						//스킬공격
							lpCurPlayer->PlaySlowCount = GetResistancePower(70 * 6, sITEMINFO_ICE);
						lpCurPlayer->PlaySlowSpeed = 200;
						lpCurPlayer->AttackIce = 1;
						break;

					case snCHAR_SOUND_CHAOSCARA:
						//카오스카라 스킬
						if (Param1 == SKILL_PLAY_CHAOSCARA_VAMP && LastSkillParam) {
							sinSetMana(sinGetMana() - cnt);
						}
						break;

					case snCHAR_SOUND_STINGRAY:
						if (chrAttackTarget && (lpTransAttackData->AttackState & 0xFF) == 0x80) {					//스킬공격
							if (chrAttackTarget == lpCurPlayer) {
								lpCurPlayer->PlayPoison[0] = 1;
								lpCurPlayer->PlayPoison[1] = len;
							}
						}
						break;

					case snCHAR_SOUND_KINGBAT: // 박재원 - 저주받은 신전 3층(신규필드 몬스터) - 킹뱃
						if (chrAttackTarget == lpCurPlayer) {
							lpCurPlayer->PlayPoison[0] = 1;
							lpCurPlayer->PlayPoison[1] = len;
						}
						break;

					case snCHAR_SOUND_DEVIL_BIRD:
						if (chrAttackTarget) {
						}
						break;

						/////////////////////////

					case snCHAR_SOUND_BLOODYKNIGHT:
						if (chrAttackTarget && (lpTransAttackData->AttackState & 0xFF) == 0x80) {					//스킬공격
							lpCurPlayer->PlayStunCount = 70 * 3;
						}
						break;
					case snCHAR_SOUND_HELLHOUND:
						if (chrAttackTarget && (lpTransAttackData->AttackState & 0xFF) == 0x80) {					//스킬공격
						}
						break;
					case snCHAR_SOUND_CHIMERA:
						if (chrAttackTarget && (lpTransAttackData->AttackState & 0xFF) == 0x80) {					//스킬공격
							lpCurPlayer->PlayStunCount = 70 * 3;
						}
						break;


					}


					//////////////////////////////////////////////

					//흡수율 계산
					if (PkMode_CharState == smCHAR_STATE_USER)
						cnt = (cnt - ((lpCurPlayer->smCharInfo.Absorption * PKMODE_ABSORB) / 100));
					else
						cnt = (cnt - lpCurPlayer->smCharInfo.Absorption);
					/*
											if ( (lpCurPlayer->dwDispAppSkill&SKILL_APPMASK_HOLY_BODY) ) {
												if ( smCharInfo.Brood==smCHAR_MONSTER_UNDEAD ) {
													//홀리 바디 스킬 적용 ( 언데드만 적용 )
													cnt -= HolyBody_Absorb[DispApp_SkillPoint-1];
													HolyBody_Undead = TRUE;
												}
											}
					*/
					//if ( smCharInfo.State==smCHAR_STATE_USER )	cnt = (cnt*80)/100;		//PK모드 생명감소 반으로 줄임

					if (cnt <= 0) cnt = 1;

					if (abs(lpChar->smCharInfo.Level - lpCurPlayer->smCharInfo.Level) <= AGING_SUB_LEVEL) {
						//레벨 7차이 이하는 에이징 적용
					//	sinCheckAgingLevel( SIN_AGING_DEFENSE , false);
						sinCheckAgingLevel(SIN_AGING_DEFENSE_ARMOR, false);
						sinCheckAgingLevel(SIN_AGING_DEFENSE_ORB, false);
						sinCheckAgingLevel(SIN_AGING_DEFENSE_BOOTS, false);   // 박재원 - 에이징 아이템 추가(부츠)
						sinCheckAgingLevel(SIN_AGING_DEFENSE_GLOVES, false);  // 박재원 - 에이징 아이템 추가(장갑)
						sinCheckAgingLevel(SIN_AGING_DEFENSE_ARMLETS, false); // 박재원 - 에이징 아이템 추가(암릿)
					}

					CheckCharForm();

					sinSetLife(sinGetLife() - cnt);
					//명중 ( 공격 성공 )
					//lpCurPlayer->smCharInfo.Life[0] -= cnt;

					if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_SUCCUBUS) {
						if (lpTransAttackData->sRating[0] > 75) {
							if (lpTransAttackData->sRating[0] < 80)	//서큐버스
								sinSetStamina(sinGetStamina() - ((lpCurPlayer->smCharInfo.Stamina[1] * 6) / 100));
							else 	//인큐버스
								sinSetMana(sinGetMana() - ((lpCurPlayer->smCharInfo.Mana[1] * 6) / 100));
						}
					}


					if (((lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_PHYSICAL_ABSORB) && (lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_HOLY_BODY) == 0) || HolyBody_Undead) {
						//피지클업솝 적용이펙트
					}

					if (lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DAMAGE && lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_EAT && lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_ATTACK &&
						lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_SKILL && cnt > 1) {

						lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_DAMAGE);
					}

					//임시 처리
					if (sinGetLife() <= 0) {

						if (sinQuest_ChangeJob4.CODE == SIN_QUEST_CODE_CHANGEJOB4 &&
							lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->FieldCode == QUEST_ARENA_FIELD) {
							//퀘스트 대전장
							OpenEachPlayer(this);
							sinBattleNpcResult(0);		//3차 전업 마스터 한테 졌다
							sinSetLife(0);
							lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_DEAD);
							ResetAttackTrans();
							return TRUE;
						}
						lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_DEAD);
						sinSetLife(0);

						SendCommand_AreaServer(smTRANSCODE_DEAD_USER, dwObjectSerial, 0, 0, 0);
						SendCommand_DataServer(smTRANSCODE_DEAD_USER, dwObjectSerial, 1, 0, 0);
						WeaponEffect = 0;
						dwWeaponEffectTime = 0;

#ifdef LOG_THE_PK
						if (smCharInfo.State != smCHAR_STATE_USER && PkMode_CharState != smCHAR_STATE_USER)
#else
						if (smCharInfo.State != smCHAR_STATE_USER)
#endif
						{//해외
							DeadPlayerExp(lpCurPlayer);				//죽어서 경험치 깍임 (PK당했을때는 제외)
						}

						sinCheckQuest85Die();		//퀘스트 수행중 죽었을 경우

						//게임을 저장
						SaveGameData();

#ifdef LOG_THE_PK
						if (smCharInfo.State == smCHAR_STATE_USER || PkMode_CharState == smCHAR_STATE_USER)
#else
						if (smCharInfo.State == smCHAR_STATE_USER)
#endif
						{//해외
							//PK당해서 죽음
							PlayerKilling(dwObjectSerial);
						}
						ResetAttackTrans();	//lpTransAttackData->code = 0;
						return TRUE;
					}
					ReformCharForm();					//캐릭터 정보 인증

				}
				else {
					//회피(방어) 성공
					Record_DefenceCount++;
				}
			}
			dwBattleTime = dwPlayTime;			//전투 시간 기록
		}
	}

	ResetAttackTrans();	//lpTransAttackData->code = 0;


	return TRUE;
}


//위치를 받아 예측데이타를 만들어 넣는다
int smCHAR::MakePlayBuffFromPosi(int px, int py, int pz, int anX, int anY, int anZ, int act)
{
	int len;
	int sx, sy, sz;
	int x, y, z;
	int mx, my, mz;
	int PlayBuffPosi;
	int cnt;
	int aY;
	int dist;
	//smMOTIONINFO	*lpMotionInfo;
	//int SelFrame;

	if (!smMotionInfo) return FALSE;

	len = 0x40;


	if (PlayBuffPosi_End <= 0) {
		sx = px;
		sy = py;
		sz = pz;
	}
	else {
		PlayBuffPosi = (PlayBuffPosi_End - 1) & PLAYBUFF_MASK;
		sx = PlayBuff[PlayBuffPosi].x;
		sy = PlayBuff[PlayBuffPosi].y;
		sz = PlayBuff[PlayBuffPosi].z;
	}

	if (sx == px && sy == py && sz == pz)
		aY = anY;
	else
		aY = GetRadian2D(sx, sz, px, pz);

	x = (px - sx) >> FLOATNS;
	z = (py - sy) >> FLOATNS;

	dist = x * x + z * z;
	if (dist > (110 * 110)) {
		//예측 데이타 간격이 너무 긴 경우

		PlayBuffPosi = PlayBuffPosi_End & PLAYBUFF_MASK;

		PlayBuff[PlayBuffPosi].x = px;
		PlayBuff[PlayBuffPosi].y = py;
		PlayBuff[PlayBuffPosi].z = pz;

		PlayBuff[PlayBuffPosi].angle[0] = 0;
		PlayBuff[PlayBuffPosi].angle[1] = aY;
		PlayBuff[PlayBuffPosi].angle[2] = 0;
		PlayBuff[PlayBuffPosi].angle[3] = 0;

		PlayBuff[PlayBuffPosi].frame = smPLAYBUFF_FRAME_NONE | 80;
		PlayBuff[PlayBuffPosi].dwTargetSerial = 0;

		memcpy(PlayBuff[PlayBuffPosi].wStickItems, wStickItems, sizeof(short) * 4);

		PlayBuffPosi_End++;
		return TRUE;
	}

	mx = ((px - sx) << 8) / len;
	my = ((py - sy) << 8) / len;
	mz = ((pz - sz) << 8) / len;

	x = 0;	y = 0;	z = 0;

	for (cnt = 0; cnt < len; cnt++) {
		PlayBuffPosi = (PlayBuffPosi_End + cnt) & PLAYBUFF_MASK;

		PlayBuff[PlayBuffPosi].x = sx + (x >> 8);
		PlayBuff[PlayBuffPosi].y = sy + (y >> 8);
		PlayBuff[PlayBuffPosi].z = sz + (z >> 8);

		PlayBuff[PlayBuffPosi].angle[0] = 0;
		PlayBuff[PlayBuffPosi].angle[1] = aY;
		PlayBuff[PlayBuffPosi].angle[2] = 0;
		PlayBuff[PlayBuffPosi].angle[3] = 0;

		PlayBuff[PlayBuffPosi].frame = smPLAYBUFF_FRAME_NONE | 80;
		PlayBuff[PlayBuffPosi].dwTargetSerial = 0;

		memcpy(PlayBuff[PlayBuffPosi].wStickItems, wStickItems, sizeof(short) * 4);

		x += mx;
		y += my;
		z += mz;
	}
	PlayBuffPosi_End = PlayBuffPosi_End + len;

	return TRUE;
}

//전송 받은 데이타로 PLAYBUFF에 예측 데이타를 만들어 넣는다
int smCHAR::MakePlayBuffFromRecvData(smPLAYBUFF* StartBuff, smPLAYBUFF* EndBuff, int len)
{
	int cnt;
	int x, y, z;
	int px, py, pz;
	int mx, my, mz;
	int PlayBuffPosi;
	int aX, aY, aZ;
	int saX, saY, saZ;
	int taX, taY, taZ;
	int frameState;
	int frameStep;

	if (!smMotionInfo || len <= 1) return FALSE;

	if (!EndBuff) {
		PlayBuffPosi = PlayBuffPosi_End & PLAYBUFF_MASK;

		PlayBuff[PlayBuffPosi].x = StartBuff->x;
		PlayBuff[PlayBuffPosi].y = StartBuff->y;
		PlayBuff[PlayBuffPosi].z = StartBuff->z;

		PlayBuff[PlayBuffPosi].angle[0] = StartBuff->angle[0] & ANGCLIP;
		PlayBuff[PlayBuffPosi].angle[1] = StartBuff->angle[1] & ANGCLIP;
		PlayBuff[PlayBuffPosi].angle[2] = StartBuff->angle[2] & ANGCLIP;
		PlayBuff[PlayBuffPosi].angle[3] = StartBuff->angle[3];

		PlayBuff[PlayBuffPosi].frame = StartBuff->frame;
		PlayBuff[PlayBuffPosi].dwTargetSerial = StartBuff->dwTargetSerial;

		memcpy(PlayBuff[PlayBuffPosi].wStickItems, StartBuff->wStickItems, sizeof(short) * 4);
		PlayBuffPosi_End++;
		return TRUE;
	}

	px = StartBuff->x;
	py = StartBuff->y;
	pz = StartBuff->z;

	mx = ((EndBuff->x - px) << 8) / len;
	my = ((EndBuff->y - py) << 8) / len;
	mz = ((EndBuff->z - pz) << 8) / len;

	aX = StartBuff->angle[0];
	aY = StartBuff->angle[1];
	aZ = StartBuff->angle[2];

	taX = ANGLE_180 - EndBuff->angle[0];
	taY = ANGLE_180 - EndBuff->angle[1];
	taZ = ANGLE_180 - EndBuff->angle[2];

	saX = (aX + taX) & ANGCLIP;
	saY = (aY + taY) & ANGCLIP;
	saZ = (aZ + taZ) & ANGCLIP;

	saX = ((saX - ANGLE_180) << 8) / len;
	saY = ((saY - ANGLE_180) << 8) / len;
	saZ = ((saZ - ANGLE_180) << 8) / len;

	if (StartBuff->frame < CHAR_FRAME_MASK) {
		frameState = smMotionInfo->MotionInfo[StartBuff->angle[3]].State;
		if (frameState == CHRMOTION_STATE_WALK || frameState == CHRMOTION_STATE_RUN) {
			aY = GetRadian2D(px, pz, EndBuff->x, EndBuff->z);
			saY = 0;
		}
	}


	x = 0;
	y = 0;
	z = 0;

	taX = 0;
	taY = 0;
	taZ = 0;

	cnt = len - 1;
	if (cnt <= 0) {
		frameStep = 80;
	}
	else {
		frameStep = (EndBuff->frame - StartBuff->frame) / cnt;
	}

	if (frameStep >= 4096 || frameStep < 20) {
		frameStep = 80;
	}

	for (cnt = 0; cnt < len; cnt++) {
		PlayBuffPosi = (PlayBuffPosi_End + cnt) & PLAYBUFF_MASK;

		PlayBuff[PlayBuffPosi].x = px + (x >> 8);
		PlayBuff[PlayBuffPosi].y = py + (y >> 8);
		PlayBuff[PlayBuffPosi].z = pz + (z >> 8);
		/*
				PlayBuff[ PlayBuffPosi ].angle[0] = (aX+(taX>>8)) & ANGCLIP;
				PlayBuff[ PlayBuffPosi ].angle[1] = (aY+(taY>>8)) & ANGCLIP;
				PlayBuff[ PlayBuffPosi ].angle[2] = (aZ+(taZ>>8)) & ANGCLIP;
		*/
		PlayBuff[PlayBuffPosi].angle[0] = aX & ANGCLIP;
		PlayBuff[PlayBuffPosi].angle[1] = aY & ANGCLIP;
		PlayBuff[PlayBuffPosi].angle[2] = aZ & ANGCLIP;

		PlayBuff[PlayBuffPosi].angle[3] = StartBuff->angle[3];

		PlayBuff[PlayBuffPosi].frame = smPLAYBUFF_FRAME_NONE | frameStep;

		PlayBuff[PlayBuffPosi].dwTargetSerial = StartBuff->dwTargetSerial;

		memcpy(PlayBuff[PlayBuffPosi].wStickItems, StartBuff->wStickItems, sizeof(short) * 4);

		x += mx;
		y += my;
		z += mz;

		taX += saX;
		taY += saY;
		taZ += saZ;
	}

	PlayBuff[PlayBuffPosi_End & PLAYBUFF_MASK].frame = StartBuff->frame;
	PlayBuff[(PlayBuffPosi_End + len - 1) & PLAYBUFF_MASK].frame = EndBuff->frame;

	PlayBuffPosi_End = PlayBuffPosi_End + len;

	return TRUE;
}


//통신 오류 체크하여 보정
int smCHAR::TransDelayCheck()
{
	int Delay;

	if (TransSock && TransSock->ConnectDirection) {
		Delay = PlayCounter - TransLastSendCnt;

		if (Delay > TransDelayMax) {
			TransSendWait = 0;
			TransLastSendCnt = PlayCounter - 16;
			return TRUE;
		}
	}

	return FALSE;
}

//거리별 전송 시간차를 조절 한다
int smCHAR::SetDistSendCnt(smCHAR* player)
{
	int x, y, z;
	int dist;

	x = (pX - player->pX) >> FLOATNS;
	y = (pY - player->pY) >> FLOATNS;
	z = (pZ - player->pZ) >> FLOATNS;

	dist = x * x + y * y + z * z;

	if (dist < DIST_TRANSLEVEL_HIGH || !DisplayFlag) {
		TransMinCnt = 8;
		TransLevel = 0;
		TransDelayMax = PLAYCOUNT_PER_SECOND * 4;		//4초
		return TRUE;
	}
	if (dist < DIST_TRANSLEVEL_MID) {
		TransMinCnt = PLAYCOUNT_PER_SECOND;
		TransLevel = 1;
		TransDelayMax = PLAYCOUNT_PER_SECOND * 6;		//8초
		return TRUE;
	}
	if (dist < DIST_TRANSLEVEL_LOW) {
		TransMinCnt = PLAYCOUNT_PER_SECOND * 5;
		TransLevel = 2;
		TransDelayMax = PLAYCOUNT_PER_SECOND * 10;		//5초
		return TRUE;
	}

	TransMinCnt = PLAYCOUNT_PER_SECOND * 15;		//15초
	TransLevel = 3;
	TransDelayMax = PLAYCOUNT_PER_SECOND * 25;		//25초

	return TRUE;
}

//상대 플레이어의 데이타 초기 설정
int smCHAR::FormSetPlayerInfo()
{

	int cnt = 0;

	if (!Pattern) {
		//패턴 오류 경고 - 로그에 기록
		char szBuff[128];

		wsprintf(szBuff, "Mesh Pattern Error [ %s ] ( %s %s )", smCharInfo.szName, smCharInfo.szModelName, smCharInfo.szModelName2);
		Record_ClinetLogFile(szBuff);
	}


	PHeight = pY;

	smCharInfo.bUpdateInfo[1] = 0xFF;			//스킬포인트 초기화

	if (this != lpCurPlayer && lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_STAND &&
		abs(lpCurPlayer->pX - pX) < 32 * fONE && abs(lpCurPlayer->pZ - pZ) < 32 * fONE) {

		if (lpCurPlayer->OnStageField >= 0) {
			if (StageField[lpCurPlayer->OnStageField]->CheckStartPoint(lpCurPlayer->pX, lpCurPlayer->pZ) == TRUE) {
				lpCurPlayer->MoveFlag = TRUE;
				lpCurPlayer->MoveCnt = 100;
			}
		}
	}

	//신규 입력 캐릭터 바닥 확인
	if (smCharInfo.State != smCHAR_STATE_ENEMY) {
		OnStageField = -1;
		if (smGameStage[0] && Pattern) {
			cnt = smGameStage[0]->GetFloorHeight(pX, pY, pZ, Pattern->SizeHeight);
			OnStageField = 0;
		}
		if (cnt == CLIP_OUT && smGameStage[1]) {
			OnStageField = 1;
		}
	}

	PatSizeLevel = smCharInfo.SizeLevel;
	if (PatSizeLevel >= 0x1000) PatSizeLevel = 1;

	if (TransSock && TransSock->ConnectDirection) 	TransSendWait = 0;
	else TransSendWait = 1;

	TransLastSendCnt = PlayCounter;

	TransMinCnt = 8;
	TransLevel = 0;
	TransDelayMax = PLAYCOUNT_PER_SECOND * 4;		//4초

	PlayBuffPosi_End = 0;
	PlayBuffCnt = 0;

	DisplayAlpha = -256;
	DispLifeBar = 0;

	switch (smCharInfo.dwCharSoundCode) {
	case snCHAR_SOUND_FIGON:
		//파이곤 손에 불 붙이기
		if (Pattern && Pattern->TmParent) {
			HvLeftHand.ObjBip = Pattern->TmParent->GetObjectFromName("Bip01 L Finger1");
			HvRightHand.ObjBip = Pattern->TmParent->GetObjectFromName("Bip01 R Finger1");
		}
		else {
			HvLeftHand.ObjBip = 0;
			HvRightHand.ObjBip = 0;
		}
		break;

	case snCHAR_SOUND_ILLUSIONKNIGHT:
		//일루전 나이트 - 반투명
		RendAlpha = -180;
		break;

	case snCHAR_SOUND_MUDY:
		//머디
		RenderLatter = TRUE;
		break;

	case snCHAR_SOUND_WOLVERLIN:
		//울버린 색상
		if (smCharInfo.State == smCHAR_STATE_ENEMY) {
			sAddColorEffect[SMC_R] = 100;
			sAddColorEffect[SMC_G] = 100;
		}
		break;

	case snCHAR_SOUND_SEN:
		//센 조명
		if (Pattern && Pattern->TmParent) {
			HvLeftHand.ObjBip = Pattern->TmParent->GetObjectFromName("Bip01");
		}
		else {
			HvLeftHand.ObjBip = 0;
			HvRightHand.ObjBip = 0;
		}
		break;

	case snCHAR_SOUND_MUTANTTREE:
		//뮤턴트 트리 연기
		if (Pattern && Pattern->TmParent) {
			HvLeftHand.ObjBip = Pattern->TmParent->GetObjectFromName("Bip05");
		}
		else {
			HvLeftHand.ObjBip = 0;
			HvRightHand.ObjBip = 0;
		}
		break;

	case snCHAR_SOUND_SHADOW:
		//쉐도우
		if (Pattern && Pattern->TmParent) {
			HvLeftHand.ObjBip = Pattern->TmParent->GetObjectFromName("Bip01 L UpperArm");
			HvRightHand.ObjBip = Pattern->TmParent->GetObjectFromName("Bip01 R UpperArm");
		}
		else {
			HvLeftHand.ObjBip = 0;
			HvRightHand.ObjBip = 0;
		}
		break;

	case snCHAR_SOUND_MRGHOST:
		RendAlpha = -80;
		break;

	case snCHAR_SOUND_FURY:

		switch (smCharInfo.Level)				//퀘스트용 퓨리에 색상 설정
		{
		case 118:
			sAddColorEffect[SMC_R] = 100;
			sAddColorEffect[SMC_G] = 100;
			RendAlpha = -80;
			break;
		case 119:
			sAddColorEffect[SMC_R] = 100;
			RendAlpha = -80;
			break;
		case 120:
			sAddColorEffect[SMC_B] = 100;
			RendAlpha = -80;
			break;
		}
		break;

	case snCHAR_SOUND_BEEVIL:							//벌
		RenderLatter = TRUE;
		break;

	case snCHAR_SOUND_GUARDIAN_SAINT:		//가디언-세인트 ( 몸에서 빛이 점멸 )
		sAddColorEffect[SMC_R] = 80;
		sAddColorEffect[SMC_G] = 80;
		sAddColorEffect[SMC_B] = 20;

		break;

	case  snCHAR_SOUND_CHAINGOLEM:			//체인골렘
		break;

	case snCHAR_SOUND_RUNICGUARDIAN:		//루닉 가디안
		break;

	case snCHAR_SOUND_BABEL:				//바벨 (루닉가디안 보스)
		break;

	case snCHAR_SOUND_CHAOSCARA:
		break;

	case snCHAR_SOUND_DEATHKNIGHT:			//발렌토
		break;

	case snCHAR_SOUND_S_F_ELEMENTAL:		//소환몹 화이어엘리멘탈
		break;

	case snCHAR_SOUND_NPC_SKILLMASTER:		//스킬마스터
		if (Pattern && Pattern->TmParent)
			AttackObjBip = Pattern->TmParent->GetObjectFromName("bip01 weapon");
		break;

	case snCHAR_SOUND_NPC_MAGICMASTER:		//매직마스터
		if (Pattern && Pattern->TmParent)
			AttackObjBip = Pattern->TmParent->GetObjectFromName("Bip01 staff02");
		break;

	case snCHAR_SOUND_CASTLE_SOLDER_A:
	case snCHAR_SOUND_CASTLE_SOLDER_B:
	case snCHAR_SOUND_CASTLE_SOLDER_C:
		if (Pattern && Pattern->TmParent)
			AttackObjBip = Pattern->TmParent->GetObjectFromName("Bip01 Effect");
		break;

	case snCHAR_SOUND_KELVEZU:
		break;
	case snCHAR_SOUND_REVIVED_KNIGHT:	// pluto 신규필드 몬스터 되살아난 나이트
		if (Pattern && Pattern->TmParent)
		{
			HvRightHand.ObjBip = Pattern->TmParent->GetObjectFromName("Bip01 R Hand");
		}
		break;
	case snCHAR_SOUND_REVIVED_PIKEMAN:
		if (Pattern && Pattern->TmParent)
		{
			AttackObjBip = Pattern->TmParent->GetObjectFromName("bip01 wea");
		}
		break;
	case snCHAR_SOUND_REVIVED_ARCHER:
		if (Pattern && Pattern->TmParent)
		{
			HvRightHand.ObjBip = Pattern->TmParent->GetObjectFromName("Bip01 R Hand");
		}
		break;
	}



	if (smCharInfo.wPlayClass[0] == MONSTER_CLASS_BOSS) {
		//보스 몬스터
		if (smCharInfo.State == smCHAR_STATE_ENEMY) {
			sAddColorEffect[SMC_R] = 130;
			sAddColorEffect[SMC_G] = 50;
			sAddColorEffect[SMC_B] = 70;
		}
	}

	return TRUE;
}

//수신 받은 상대 플레이어의 데이타 설정
int smCHAR::SetTransPlayerInfo(smTRNAS_PLAYERINFO* lpTransPlayerInfo)
{

	memcpy(&smCharInfo, &lpTransPlayerInfo->smCharInfo, sizeof(smCHAR_INFO));
	ResetAttackTrans();

	if (!PatLoading)
		AddLoaderPattern((smCHAR*)&Head, smCharInfo.szModelName, smCharInfo.szModelName2);

	pX = lpTransPlayerInfo->x;
	pY = lpTransPlayerInfo->y;
	pZ = lpTransPlayerInfo->z;

	FormSetPlayerInfo();

	if (smCharInfo.ClassClan) {
		//클랜 정보
		ClanInfoNum = ReadClanInfo(smCharInfo.ClassClan);
		dwClanInfoTime = dwPlayTime;

		if (GetClanCode(smCharInfo.ClassClan) == rsBlessCastle.dwMasterClan)	//캐슬 마스터 클랜
			Clan_CastleMasterFlag = TRUE;
	}

	PkMode_CharState = 0;

	return TRUE;
}

//수신 받은 상대 플레이어의 데이타 설정
int smCHAR::SetTransPlayerInfoQuick(smTRNAS_PLAYERINFO_QUICK* lpTransPlayerInfo)
{
	int len;

	len = sizeof(smTRNAS_PLAYERINFO_QUICK) - lpTransPlayerInfo->size;

	ZeroMemory(&smCharInfo, sizeof(smCHAR_INFO));
	smCharInfo.Life[0] = 10;
	smCharInfo.Life[1] = 50;

	memcpy(&smCharInfo, &lpTransPlayerInfo->smCharInfo, sizeof(smCHAR_INFO) - len);
	ResetAttackTrans();

	if (!PatLoading)
		AddLoaderPattern((smCHAR*)&Head, smCharInfo.szModelName, smCharInfo.szModelName2);

	pX = lpTransPlayerInfo->x;
	pY = lpTransPlayerInfo->y;
	pZ = lpTransPlayerInfo->z;

	FormSetPlayerInfo();

	if (smCharInfo.ClassClan) {
		//클랜 정보
		ClanInfoNum = ReadClanInfo(smCharInfo.ClassClan);
		dwClanInfoTime = dwPlayTime;

		if (GetClanCode(smCharInfo.ClassClan) == rsBlessCastle.dwMasterClan)	//캐슬 마스터 클랜
			Clan_CastleMasterFlag = TRUE;
	}

	PkMode_CharState = 0;

	return TRUE;
}

//데이타 입수 ( 그룹 처리용 )
int smCHAR::RecvPlayData2(char* lpData)
{
	smPLAYDATA* lpPlayData;
	int RecvCode;
	int cnt;
	int len;
	int LastBuffPosi_End;
	int act;
	char* lpBuff;
	int	Code, Size;
	smCHAR* lpChar;

	RecvCode = ((int*)lpData)[1];

	switch (RecvCode) {
	case smTRANSCODE_PLAYDATA1:
		//게임 진행 수신
		lpPlayData = (smPLAYDATA*)lpData;

		act = lpPlayData->angle[3];
		//위치를 받아 예측데이타를 만들어 넣는다
		MakePlayBuffFromPosi(lpPlayData->x, lpPlayData->y, lpPlayData->z, lpPlayData->angle[0], lpPlayData->angle[1], lpPlayData->angle[2], act);
		MoveCnt = 0;

		/*
				if ( PlayBuffCnt==PlayBuffPosi_End ) {

					pX = lpPlayData->x;
					pY = lpPlayData->y;
					pZ = lpPlayData->z;

					Angle.x = lpPlayData->angle[0] & ANGCLIP;
					Angle.y = lpPlayData->angle[1] & ANGCLIP;
					Angle.z = lpPlayData->angle[2] & ANGCLIP;
					act = lpPlayData->angle[3];

					if ( act!=action ) {//|| ( act!=CHRMOTION_WALK && act!=CHRMOTION_STAND ) )
						if ( Pattern )
							ChangeMotion( act );
						else
							act = 0;
					}

					action = act;

					MoveCnt = 0;
					dwTarget = 0;
					//PlayBuffCnt = PlayBuffPosi_End;
				}
		*/
		LastPlayDataType = 1;			//원거리 교신
		smCharInfo.bUpdateInfo[1] = 0xFF;
		break;

	case smTRANSCODE_TRANSPLAYDATA:
		//게임 진행 수신

		smTRANS_PLAYDATA* lpTransPlayData;
		smTRANS_PLAYBUFF* lpTransPlayBuff;
		smTRANS_PLAYBUFF* lpTransPlayBuff2;

		LastPlayDataType = 0;			//근거리 교신

		lpTransPlayData = (smTRANS_PLAYDATA*)lpData;
		lpTransPlayBuff = (smTRANS_PLAYBUFF*)(lpData + sizeof(smTRANS_PLAYDATA));

		LastBuffPosi_End = PlayBuffPosi_End;

		smCharInfo.Life[0] = lpTransPlayData->Hp[0];		//체력
		smCharInfo.Life[1] = lpTransPlayData->Hp[1];	//체력최대치

		if (LevelLog) {
			if (smCharInfo.State == smCHAR_STATE_USER && lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_LEVEL] > LevelLog) {
				//상대방 레벨업
			}
		}

		LevelLog = lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_LEVEL];
		PotionLog = lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_POTION] & 0x0F;
		CriticalLog = lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_POTION] >> 4;
		dwDispAppSkill = lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_SKIL];

		/*
				if ( dwDispAppSkill&SKILL_APPMASK_ICE ) {
					//얼음 공격받음
					if ( AttackIce<=0 ) AttackIce = 1;
				}
				else {
					if ( AttackIce>0 ) AttackIce = -AttackIce;
				}
		*/

		if (CriticalLog >= 8) CriticalLog = -1;
		AttackCritcal = CriticalLog;

		if (lpTransPlayData->PlayBuffCnt > 1) {

			for (cnt = 0; cnt < lpTransPlayData->PlayBuffCnt - 1; cnt++) {
				lpTransPlayBuff2 = lpTransPlayBuff + 1;

				len = lpTransPlayBuff2->BuffCount - lpTransPlayBuff->BuffCount;
				MakePlayBuffFromRecvData(&lpTransPlayBuff->PlayBuff, &lpTransPlayBuff2->PlayBuff, len);

				lpTransPlayBuff++;
			}

			dwTarget = lpTransPlayData->dwTarget;
			/*
						if ( lpTransPlayData->bUpdateInfo[0]!=smCharInfo.bUpdateInfo[0]) {
							//내용 정보가 바뀜
							//Close();				//캐릭터 닫기
							//smCharInfo.bUpdateInfo[1] = lpTransPlayData->bUpdateInfo[0];


							//smCharInfo.bUpdateInfo[0] = lpTransPlayData->bUpdateInfo[0];

							if ( !dwUpdateCharInfoTime || (dwUpdateCharInfoTime+1000)<dwPlayTime ) {
								Send_GetCharInfo( dwObjectSerial , ServerCode );

								if ( smCharInfo.dwCharSoundCode==snCHAR_SOUND_HUNGKY && !dwUpdateCharInfoTime) {
									//이펙트 ( 헐크 변신 )
									StartEffect( pX,pY+48*fONE,pZ, EFFECT_RETURN1 );
									SetDynLight( pX,pY,pZ, 50, 100,70,0, 300 , 1 );

									//변신 소리
									cnt = MotionInfo->State;
									MotionInfo->State=CHRMOTION_STATE_WARP;
									CharPlaySound( this );
									MotionInfo->State = cnt;
								}
								dwUpdateCharInfoTime = dwPlayTime;
							}
						}
			*/
			if (smCharInfo.bUpdateInfo[1] == 0xFF) {
				//스킬포인터 오래되서 새로 갱신
				smCharInfo.bUpdateInfo[1] = lpTransPlayData->bUpdateInfo[1];
			}
			else {
				if (lpTransPlayData->bUpdateInfo[1] != smCharInfo.bUpdateInfo[1]) {
					//스킬 포인트 변화
					if (lpTransPlayData->bUpdateInfo[1] < smCharInfo.bUpdateInfo[1])
						LearnSkillEffect(this);			//스킬 습득 이펙트

					smCharInfo.bUpdateInfo[1] = lpTransPlayData->bUpdateInfo[1];
				}
			}


			lpTransPlayBuff++;
		}
		else {		//서있는 동작 연속
			MakePlayBuffFromRecvData(&lpTransPlayBuff->PlayBuff, 0, 0);
			if (PlayBuffCnt == PlayBuffPosi_End) {

				if (DisplayFlag) {

					pX = lpTransPlayBuff->PlayBuff.x;
					pY = lpTransPlayBuff->PlayBuff.y;
					pZ = lpTransPlayBuff->PlayBuff.z;

					// 왼쪽 장착 아이템
					if (lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_LEFT] != wStickItems[smPLAYBUFF_ITEM_LEFT]) {
						if (lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_LEFT] >= 0) {
							SetTool(sItem[lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_LEFT]].CODE, hvPOSI_LHAND);
							HvLeftHand.ColorBlink = 0;
							HvLeftHand.DispEffect = 0;
						}
						else
							SetTool(0, hvPOSI_LHAND);
					}

					// 오른쪽 장착 아이템
					if (lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_RIGHT] != wStickItems[smPLAYBUFF_ITEM_RIGHT]) {
						if (lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_RIGHT] >= 0) {
							SetTool(sItem[lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_RIGHT]].CODE, hvPOSI_RHAND);
							HvRightHand.ColorBlink = 0;
							HvRightHand.DispEffect = 0;
						}
						else
							SetTool(0, hvPOSI_RHAND);
					}

					// 몸 장착 아이템( 갑옷 등 )
					if (lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_BODY] != wStickItems[smPLAYBUFF_ITEM_BODY]) {
						SetChrArmor(this, lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_BODY]);
						wStickItems[smPLAYBUFF_ITEM_BODY] = lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_BODY];
					}
				}
			}

			lpTransPlayBuff++;
		}

		if (lpTransPlayData->bUpdateInfo[0] != smCharInfo.bUpdateInfo[0]) {

			if (!dwUpdateCharInfoTime || (dwUpdateCharInfoTime + 1000) < dwPlayTime) {
				Send_GetCharInfo(dwObjectSerial, ServerCode);

				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_HUNGKY && !dwUpdateCharInfoTime) {
					//이펙트 ( 헐크 변신 )

					//변신 소리
					if (MotionInfo) {
						cnt = MotionInfo->State;
						MotionInfo->State = CHRMOTION_STATE_WARP;
						MotionInfo->State = cnt;
					}
				}
				// pluto 언데드 킹호피 변신
				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_DEADHOPT && !dwUpdateCharInfoTime)
				{

					if (MotionInfo)
					{
						cnt = MotionInfo->State;
						MotionInfo->State = CHRMOTION_STATE_WARP;
						MotionInfo->State = cnt;
					}
				}

				// 박재원 - 복날 이벤트
				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BEBECHICK && !dwUpdateCharInfoTime)
				{

					if (MotionInfo)
					{
						cnt = MotionInfo->State;
						MotionInfo->State = CHRMOTION_STATE_WARP;
						MotionInfo->State = cnt;
					}
				}


				dwUpdateCharInfoTime = dwPlayTime;
			}
		}


		////////////////// 공성전 필드 확인 ////////////////////

		if (lpCurPlayer->OnStageField >= 0 &&
			(
				(StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_CASTLE && CastleBattleZone_LineZ > lpCurPlayer->pZ) ||
				(StageField[lpCurPlayer->OnStageField]->FieldCode == PK_FieldState.FieldCode && PK_FieldState.dwPlay_FieldTime > dwPlayTime && !PartyFlag)
				)) {

			//주인공 공성 모드
			if (PkMode_CharState == 0 && smCharInfo.State == smCHAR_STATE_USER &&
				(smCharInfo.ClassClan == 0 || smCharInfo.ClassClan != lpCurPlayer->smCharInfo.ClassClan))
			{

				if (StageField[lpCurPlayer->OnStageField]->FieldCode != PK_FieldState.FieldCode || !PartyFlag) {
					PkMode_CharState = smCharInfo.State;
					smCharInfo.State = smCHAR_STATE_ENEMY;
				}
			}
		}
		else {
			if (PkMode_CharState) {
				//공성모드 해제
				smCharInfo.State = PkMode_CharState;
				PkMode_CharState = 0;
			}
		}

		int ChkIceFlag = 0;
		int ChkPoisonFlag = 0;
		DWORD	TradeMsgCode = 0;
		//DWORD	dwMsgCode;

		PlayVanish = 0;
		TradeMsgCode = dwTradeMsgCode;
		dwTradeMsgCode = 0;
		PlayCurseQuest = 0;

		if (lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT]) {
			lpBuff = (char*)lpTransPlayBuff;

			for (cnt = 0; cnt < lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT]; cnt++) {

				Code = ((int*)lpBuff)[1];
				Size = ((int*)lpBuff)[0];

				switch (Code) {
				case smTRANSCODE_EFFECT_ITEM:
					//착용 무기 이펙트 적용
					SetTransEffectItems((smEFFECT_ITEM*)lpBuff);
					break;

				case smTRANSCODE_EFFECT_WEAPON:
					//수신된 데이타의 장착 무기에 속성 이펙트 적용 
					SetTransEffectWeapon((smEFFECT_ITEM*)lpBuff);
					break;

				case smTRANSCODE_EFFECT_STUN:
					//캐릭터가 스턴 상태
					if (smCharInfo.Life[0] > 0)
						break;

				case smTRANSCODE_EFFECT_CURSE:
					//써큐버스 저주 ( HolyPower 적용중 )
					break;

				case smTRANSCODE_EFFECT_ICE:
					//꽁꽁 얼었음 
					if (((smEFFECT_ITEM*)lpBuff)->ColorBlink) {
						PlayDistortion = 120;//((smEFFECT_ITEM *)lpBuff)->ColorBlink*4;
					}
					else {
						PlayDistortion = 0;	//디스토션후 얼음공격일때 디스토션 효과 유지 버그 수정( 2005년 7월14일 ) 
						if (AttackIce <= 0) AttackIce = 1;
						ChkIceFlag = TRUE;
					}
					break;

				case smTRANSCODE_EFFECT_POISION:
					if (DispPoison <= 0) DispPoison = 1;
					ChkPoisonFlag = TRUE;
					break;

				case smTRANSCODE_EFFECT_CURSE_QUEST:
					PlayCurseQuest = 1;
					break;

				case smTRANSCODE_EFFECT_VANISH:
					PlayVanish = ((smEFFECT_ITEM*)lpBuff)->dwItemCode;
					PlayVague = ((smEFFECT_ITEM*)lpBuff)->ColorBlink;
					break;

				case smTRANSCODE_EFFECT_KEEPSKILL:
					//스킬 이펙트 플랙 정보
					switch (((smEFFECT_ITEM*)lpBuff)->dwItemCode) {
					case SKILL_PLAY_FORCE_OF_NATURE:
						break;
					case SKILL_PLAY_HALL_OF_VALHALLA:
						break;
					case SKILL_PLAY_BLESS_SIEGE_ITEM:
						PlayInvincible = 70 * 5;					//무적 아이템
						break;
					}
					break;

				case smTRANSCODE_EFFECT_PERSHOP:		//개인상점 오픈
					dwTradeMsgCode = ((smEFFECT_ITEM*)lpBuff)->dwItemCode;

					if (TradeMsgCode != dwTradeMsgCode) {
						/*
						if ( szTradeMessage[0] )
							dwMsgCode = GetSpeedSum( szTradeMessage );
						else
							dwMsgCode = 0;
						*/

						//코드 변경
						wsprintf(szTradeMessage, "%s :", smCharInfo.szName);
						//지역서버에 명령 코드 전송
						SendCommand_AreaServer(smTRANSCODE_MYSHOP_MESSAGE, 0, dwObjectSerial, dwTradeMsgCode, 0);
					}
					break;

				case smTRANSCODE_EFFECT_LOWPET:		//저레벨 도움미 펫

					if (((smEFFECT_ITEM*)lpBuff)->ColorBlink)
						lpChar = FindChrPlayer(((smEFFECT_ITEM*)lpBuff)->ColorBlink);
					else
						lpChar = 0;

					break;

				case smTRANSCODE_EFFECT_PCBANGPET:		//PC방 펫

					if (((smEFFECT_ITEM*)lpBuff)->ColorBlink)
						lpChar = FindChrPlayer(((smEFFECT_ITEM*)lpBuff)->ColorBlink);
					else
						lpChar = 0;

					break;

				case smTRANSCODE_EFFECT_CLANMGR:		//순위권 클랜 표시
					dwClanManageBit = ((smEFFECT_ITEM*)lpBuff)->dwItemCode;
					break;

				case smTRANSCODE_EFFECT_FORCEORB:		//포스 오브 시전중
					dwForceOrbCode = ((smEFFECT_ITEM*)lpBuff)->dwItemCode;
					dwForceOrbTime = ((smEFFECT_ITEM*)lpBuff)->ColorBlink;
					dwForceOrbTime += dwPlayTime;

					break;

				case smTRANSCODE_EFFECT_BLESS_SCORE:
					//블래스 캐슬 득점 정보 입수
					sBlessCastle_Damage[0] = ((smEFFECT_ITEM*)lpBuff)->sColors[0];
					sBlessCastle_Damage[1] = ((smEFFECT_ITEM*)lpBuff)->sColors[1];
					break;
				}
				lpBuff += Size;
			}
		}
		if (!ChkIceFlag && AttackIce > 0) AttackIce = -AttackIce;
		if (!ChkPoisonFlag && DispPoison > 0) DispPoison = -DispPoison;

	}


	TransSendWait = 0;
	dwLastTransTime = dwPlayTime;

	return TRUE;
}

//데이타를 받음
int smCHAR::RecvPlayData(smTHREADSOCK* pData)
{
	int act;
	smPLAYDATA PlayData;
	smPLAYDATA2 PlayData2;
	smPLAYDATA3 PlayData3;
	smTRNAS_PLAYERINFO* lpTransPlayerInfo;
	int RecvCode;
	int cnt;
	int len;
	int LastBuffPosi_End;
	//smCHAR	*lpChar;

	RecvCode = ((int*)pData->Buff)[1];

	switch (RecvCode) {
	case smTRANSCODE_PLAYERINFO:
		//상대 플레이어의 데이타 수신 받음
		lpTransPlayerInfo = (smTRNAS_PLAYERINFO*)pData->Buff;
		SetTransPlayerInfo(lpTransPlayerInfo);
		return TRUE;

	case smTRANSCODE_PLAYERINFO_Q:
		//상대 플레이어의 데이타 수신 받음 ( 패킷양 적은 데이타 )
		SetTransPlayerInfoQuick((smTRNAS_PLAYERINFO_QUICK*)pData->Buff);
		break;

	case smTRANSCODE_PLAYDATA1:
		//게임 진행 수신
		memcpy(&PlayData, pData->Buff, sizeof(smPLAYDATA));

		pX = PlayData.x;
		pY = PlayData.y;
		pZ = PlayData.z;

		Angle.x = PlayData.angle[0] & ANGCLIP;
		Angle.y = PlayData.angle[1] & ANGCLIP;
		Angle.z = PlayData.angle[2] & ANGCLIP;
		act = PlayData.angle[3];

		if (act != action)//|| ( act!=CHRMOTION_WALK && act!=CHRMOTION_STAND ) )
			frame = PlayData.frame;

		action = act;

		PlayBuffCnt = PlayBuffPosi_End;
		MoveCnt = 0;

		dwTarget = 0;
		break;

	case smTRANSCODE_PLAYDATA2:
		//게임 진행 수신
		LastBuffPosi_End = PlayBuffPosi_End;
		memcpy(&PlayData2, pData->Buff, sizeof(smPLAYDATA2));
		len = PlayData2.BuffCount[1] - PlayData2.BuffCount[0];
		MakePlayBuffFromRecvData(&PlayData2.PlayBuff[0], &PlayData2.PlayBuff[1], len);
		if ((PlayBuffPosi_End - PlayBuffCnt) > 32) PlayBuffCnt = LastBuffPosi_End;

		dwTarget = PlayData2.dwTarget;
		break;

	case smTRANSCODE_PLAYDATA3:
		//게임 진행 수신
		LastBuffPosi_End = PlayBuffPosi_End;
		memcpy(&PlayData3, pData->Buff, sizeof(smPLAYDATA3));
		len = PlayData3.BuffCount[2] - PlayData3.BuffCount[0];
		MakePlayBuffFromRecvData(&PlayData3.PlayBuff[0], &PlayData3.PlayBuff[2], len);
		len = PlayData3.BuffCount[1] - PlayData3.BuffCount[2];
		MakePlayBuffFromRecvData(&PlayData3.PlayBuff[2], &PlayData3.PlayBuff[1], len);
		if ((PlayBuffPosi_End - PlayBuffCnt) > 32) PlayBuffCnt = LastBuffPosi_End;

		dwTarget = PlayData3.dwTarget;
		break;

	case smTRANSCODE_TRANSPLAYDATA:
		//게임 진행 수신

		smTRANS_PLAYDATA* lpTransPlayData;
		smTRANS_PLAYBUFF* lpTransPlayBuff;
		smTRANS_PLAYBUFF* lpTransPlayBuff2;

		lpTransPlayData = (smTRANS_PLAYDATA*)pData->Buff;
		lpTransPlayBuff = (smTRANS_PLAYBUFF*)(pData->Buff + sizeof(smTRANS_PLAYDATA));

		LastBuffPosi_End = PlayBuffPosi_End;

		smCharInfo.Life[0] = lpTransPlayData->Hp[0];		//체력

		for (cnt = 0; cnt < lpTransPlayData->PlayBuffCnt - 1; cnt++) {
			lpTransPlayBuff2 = lpTransPlayBuff + 1;

			len = lpTransPlayBuff2->BuffCount - lpTransPlayBuff->BuffCount;
			MakePlayBuffFromRecvData(&lpTransPlayBuff->PlayBuff, &lpTransPlayBuff2->PlayBuff, len);

			lpTransPlayBuff++;
		}

		dwTarget = lpTransPlayData->dwTarget;
		break;

	case smTRANSCODE_ATTACKDATA2:
	case smTRANSCODE_ATTACKDATA:
		//상대방한테 공격 함
		AttackRecvCountUp();

		Record_TotalRecvDamageCount++;

		if (((TRANS_ATTACKDATA*)pData->Buff)->AttackState == 0x80) {
			memcpy(&AttackTrans, (TRANS_ATTACKDATA*)pData->Buff, sizeof(TRANS_ATTACKDATA));
			XorAttackTrans();
			if (PkMode_CharState || (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->FieldCode == rsCASTLE_FIELD))
				PlayAttack(TRUE);								//밀린 공격 적용
			else
				PlayAttack();									//밀린 공격 적용
			break;
		}

		if (AttackTrans.code ^ GetAttackTrans_XorCode()) {
			PlayAttack(TRUE);									//밀린 공격 적용
		}

		memcpy(&AttackTrans, (TRANS_ATTACKDATA*)pData->Buff, sizeof(TRANS_ATTACKDATA));
		XorAttackTrans();

		/*
		#ifdef	_S2C_DAMAGE_PACKET
		#ifdef _LANGUAGE_CHINESE
				PlayAttack( TRUE );									//밀린 공격 적용
		#endif
		#endif
		*/
		/*
		TRANS_ATTACKDATA	*lpTransAttackData;

		lpTransAttackData = (TRANS_ATTACKDATA *)pData->Buff;

		if ( lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DAMAGE && lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD && DisplayFlag ) {

			len = GetDistanceDbl( lpCurPlayer->pX , lpCurPlayer->pZ , pX, pZ );
			cnt = GetDistanceDbl( lpCurPlayer->pX , lpCurPlayer->pZ , lpTransAttackData->x, lpTransAttackData->z );

			if ( smCharInfo.dwCharSoundCode==snCHAR_SOUND_FIGON || smCharInfo.dwCharSoundCode==snCHAR_SOUND_WEB ) {
				//원거리 공격
				len = 0;
				cnt = 0;
			}

			//주인공 몬스터 스킨일 경우
			if ( lpCurPlayer->smCharInfo.State==smCHAR_STATE_ENEMY ) act = 1;
			else act = 0;

			if ( cnt<lpTransAttackData->AttackSize && lpCurPlayer->smCharInfo.Life[0]>0 && ( len<(128*128) || act ) && DisplayAlpha==0 ) {

				lpChar = 0;

				//공격한 상대방을 찾아 방향을 바꾼다
				for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
					if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial==lpTransAttackData->dwDestObjectSerial ) {
						lpChar = &chrOtherPlayer[cnt];
						break;
					}
				}

				//블럭율 계산
				if ( lpChar && ( (rand()%100)>=sinGetBlockRating( lpChar->smCharInfo.Level ) || act ) ) {

					//방어율 계산
					//cnt = GetPlayDefence( &smCharInfo , &lpCurPlayer->smCharInfo );
					cnt = sinGetMonsterAccuracy( smCharInfo.Level , smCharInfo.Attack_Rating );
					//cnt = 100;
					if ( (rand()%100)<=cnt || act ) {

						//흡수율 계산
						cnt = (lpTransAttackData->Power - lpCurPlayer->smCharInfo.Absorption);

						if ( cnt<=0 ) cnt = 1;

							CheckCharForm();
							//명중 ( 공격 성공 )
							lpCurPlayer->smCharInfo.Life[0] -= cnt;

							//피튀기는 파티클
							StartEffect( lpCurPlayer->pX, lpCurPlayer->pY+16*fONE, lpCurPlayer->pZ, EFFECT_DAMAGE1 );
							SetDynLight( lpCurPlayer->pX, lpCurPlayer->pY+16*fONE, lpCurPlayer->pZ, 160, 0,0,0, 180 );

							if ( (lpCurPlayer->dwDispAppSkill&SKILL_APPMASK_PHYSICAL_ABSORB) ) {
								//피지클업솝 적용이펙트
								StartEffect( lpCurPlayer->pX, lpCurPlayer->pY+24*fONE , lpCurPlayer->pZ , EFFECT_PHYSICAL_ABSORB_DAMAGE );
							}

							if ( lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_EAT && lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_ATTACK &&
								lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_SKILL && cnt>1 ) {
								lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
								CharPlaySound( lpCurPlayer );
							}

							//임시 처리
							if ( lpCurPlayer->smCharInfo.Life[0]<=0 ) {
								lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_DEAD );
								lpCurPlayer->smCharInfo.Life[0] = 0;
								ReformCharForm();					//캐릭터 정보 인증
								CharPlaySound( lpCurPlayer );
								DeadPlayerExp( lpCurPlayer );
								//게임을 저장
								SaveGameData();
								break;
							}
							ReformCharForm();					//캐릭터 정보 인증

					}
				}
				dwBattleTime = dwPlayTime;			//전투 시간 기록
				//공격한 상대방을 찾아 방향을 바꾼다
				//lpCurPlayer->Angle.y = GetRadian2D( lpCurPlayer->pX , lpCurPlayer->pZ ,lpChar->pX, lpChar->pZ );
			}
		}
		*/
		break;
	}

	TransSendWait = 0;
	dwLastTransTime = dwPlayTime;

	//게임 데이타 전송이 처음 되었을때 캐릭터 화면에 표시
	//if ( Pattern ) DisplayFlag = TRUE;		

	return TRUE;
}

//데이타를 보냄
int smCHAR::SendPlayData(smCHAR* player)
{
	int SendTime;
	smPLAYDATA PlayData;
	smPLAYDATA2 PlayData2;
	smPLAYDATA3 PlayData3;
	int PlayerBuffCnt;
	int cnt;
	int StartPosi;
	int BuffPosi;
	int mAction;
	int SendMode;

	//if ( AutoPlayer ) return NULL;

	SendTime = PlayCounter - TransLastSendCnt;

	if (TransSendWait == 0 && SendTime >= TransMinCnt) {

		//목표를 겨냥한 동작일 경우~~ 목표점 표시한다
		if (lpCharMsTrace && lpCharMsTrace == this) {
			PlayData2.dwTarget = TRUE;
			PlayData3.dwTarget = TRUE;
		}
		else {
			PlayData2.dwTarget = FALSE;
			PlayData3.dwTarget = FALSE;
		}

		if (TransLevel > 1) {
			//버퍼링 없는 데이타 전송 ( 통신의 중요도가 낮음 )
			PlayData.size = sizeof(smPLAYDATA);
			PlayData.code = smTRANSCODE_PLAYDATA1;
			PlayData.x = player->pX;
			PlayData.y = player->pY;
			PlayData.z = player->pZ;

			PlayData.angle[0] = player->Angle.x;
			PlayData.angle[1] = player->Angle.y;
			PlayData.angle[2] = player->Angle.z;
			PlayData.angle[3] = player->action;

			PlayData.frame = player->frame;

			if (TransSock->Send((char*)&PlayData, sizeof(smPLAYDATA))) {
				TransSendWait = 1;
				TransLastSendCnt = PlayCounter;
				TransLastSendTime = SendTime;
				SetDistSendCnt(player);				//통신 시간차 조절
			}
		}
		else {
			//버퍼링 데이타 전송 ( 고품질의 데이타 전송 )
			SendMode = 0;
			PlayerBuffCnt = player->PlayBuffCnt & PLAYBUFF_MASK;
			StartPosi = (PlayerBuffCnt - SendTime) & PLAYBUFF_MASK;

			mAction = player->PlayBuff[PlayerBuffCnt].angle[3];

			for (cnt = SendTime; cnt >= 0; cnt--) {
				BuffPosi = (StartPosi + cnt) & PLAYBUFF_MASK;
				if (player->PlayBuff[BuffPosi].angle[3] != mAction) {
					SendMode = 1;
					memcpy(&PlayData3.PlayBuff[2], &player->PlayBuff[BuffPosi], sizeof(smPLAYBUFF));
					PlayData3.BuffCount[2] = cnt;
					break;
				}
			}

			if (!SendMode) {
				memcpy(&PlayData2.PlayBuff[0], &player->PlayBuff[StartPosi], sizeof(smPLAYBUFF));
				memcpy(&PlayData2.PlayBuff[1], &player->PlayBuff[PlayerBuffCnt], sizeof(smPLAYBUFF));
				PlayData2.code = smTRANSCODE_PLAYDATA2;
				PlayData2.size = sizeof(smPLAYDATA2);
				PlayData2.BuffCount[0] = 0;
				PlayData2.BuffCount[1] = SendTime;

				if (TransSock->Send((char*)&PlayData2, sizeof(smPLAYDATA2))) {
					TransSendWait = 1;
					TransLastSendCnt = PlayCounter;
					TransLastSendTime = SendTime;
					SetDistSendCnt(player);				//통신 시간차 조절
				}
			}
			else {
				memcpy(&PlayData3.PlayBuff[0], &player->PlayBuff[StartPosi], sizeof(smPLAYBUFF));
				memcpy(&PlayData3.PlayBuff[1], &player->PlayBuff[PlayerBuffCnt], sizeof(smPLAYBUFF));
				PlayData3.code = smTRANSCODE_PLAYDATA3;
				PlayData3.size = sizeof(smPLAYDATA3);
				PlayData3.BuffCount[0] = 0;
				PlayData3.BuffCount[1] = SendTime;

				if (TransSock->Send((char*)&PlayData3, sizeof(smPLAYDATA3))) {
					TransSendWait = 1;
					TransLastSendCnt = PlayCounter;
					TransLastSendTime = SendTime;
					SetDistSendCnt(player);				//통신 시간차 조절
				}
			}
		}
	}

	return TRUE;
}
//통신용 플레이 데이타를 지정한 버퍼에 작성 
int smCHAR::MakeTransPlayData(char* lpTargetBuff, int SendTime, int pBuffStep)
{
	smTRANS_PLAYDATA* lpTransPlayData;
	smTRANS_PLAYBUFF* lpTransPlayBuff;
	smEFFECT_ITEM* lpEffectItem;

	int StartPosi;
	int BuffPosi;
	int BuffPosi_Old;
	int mAction;
	int mFrameSelect;
	DWORD mFrame;
	int	Life;

	//	int PlayerBuffCnt;

	int	buffCnt;
	int cnt;
	int LastCnt;
	int	SameAction;

	lpTransPlayData = (smTRANS_PLAYDATA*)lpTargetBuff;
	lpTransPlayBuff = (smTRANS_PLAYBUFF*)(lpTargetBuff + sizeof(smTRANS_PLAYDATA));

	lpTransPlayData->code = smTRANSCODE_TRANSPLAYDATA;
	lpTransPlayData->dwObjectSerial = dwObjectSerial;
	lpTransPlayData->dwTarget = 0;
	lpTransPlayData->StartPosi = PlayBuffCnt - SendTime;
	if (this == lpCurPlayer) {
		if (MotionInfo && MotionInfo->State == CHRMOTION_STATE_DEAD)
			lpTransPlayData->Hp[0] = 0;
		else
			lpTransPlayData->Hp[0] = sinGetLife();
	}
	else {
		lpTransPlayData->Hp[0] = smCharInfo.Life[0];
	}
	lpTransPlayData->Hp[1] = smCharInfo.Life[1];

	if (UseObject_VirtualLife) {		//가상 생명력 사용 (확장)
		lpTransPlayData->Hp[0] = sObject_DisplayLife[0];
		lpTransPlayData->Hp[1] = sObject_DisplayLife[1];
	}



	if (smCharInfo.State == smCHAR_STATE_ENEMY)
		lpTransPlayData->dwAutoCharCode = smMonsterInfo.dwAutoCharCode;
	else
		lpTransPlayData->dwAutoCharCode = 0;

	smCharInfo.bUpdateInfo[1] = sinSkill.SkillPoint;				//스킬 포인트 저장

	lpTransPlayData->bUpdateInfo[0] = smCharInfo.bUpdateInfo[0];
	lpTransPlayData->bUpdateInfo[1] = smCharInfo.bUpdateInfo[1];
	lpTransPlayData->bUpdateInfo[2] = 0;
	lpTransPlayData->bUpdateInfo[3] = 0;
	lpTransPlayData->bPartyLeader = smCharInfo.bPartyLeader;

	int	MainArea, SubArea;

	if (lpCurPlayer && lpCurPlayer->OnStageField >= 0) {
		MainArea = (lpCurPlayer->OnStageField) & 1;
		SubArea = (lpCurPlayer->OnStageField + 1) & 1;
	}
	else {
		MainArea = 0;
		SubArea = 0;
	}

	//로딩된 해당 지역 코드 삽입
	if (StageField[0])
		lpTransPlayData->wArea[0] = StageField[MainArea]->FieldCode;
	else
		lpTransPlayData->wArea[0] = -1;

	if (StageField[1])
		lpTransPlayData->wArea[1] = StageField[SubArea]->FieldCode;
	else
		lpTransPlayData->wArea[1] = -1;

	lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_LEVEL] = smCharInfo.Level;
	lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_POTION] = (PotionLog & 0x0F) | ((AttackCritcal & 0xF) << 4);
	lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] = 0;
	lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_SKIL] = (BYTE)dwDispAppSkill;

	/*
		if ( smCharInfo.State!=smCHAR_STATE_USER ) {
			//몬스터
			if ( PlaySlowCount>0 ) lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_SKIL] |= SKILL_APPMASK_ICE;
		}
	*/
	//lpCurPlayer->dwDispAppSkill = 0;

//	BYTE	bEventInfo[4];		//모션 이벤트 정보 ( 0-Level 1-Potion사용 2-스킬사용 3-예비 )


	StartPosi = (lpTransPlayData->StartPosi) & PLAYBUFF_MASK;

	buffCnt = 0;

	//시작 정보
	mAction = PlayBuff[StartPosi].angle[3];
	mFrame = PlayBuff[StartPosi].frame;
	memcpy(&lpTransPlayBuff->PlayBuff, &PlayBuff[StartPosi], sizeof(smPLAYBUFF));
	lpTransPlayBuff->BuffCount = 0;
	lpTransPlayBuff++;
	buffCnt++;
	LastCnt = 0;
	BuffPosi_Old = StartPosi;
	mFrameSelect = PlayBuff[StartPosi].frame >> CHAR_FRAME_SELSHIFT;

	SameAction = TRUE;

	for (cnt = 1; cnt < SendTime - 1; cnt++) {
		BuffPosi = (StartPosi + cnt) & PLAYBUFF_MASK;

		if (PlayBuff[BuffPosi].angle[3] != mAction) SameAction = FALSE;

		if (PlayBuff[BuffPosi].frame < mFrame || PlayBuff[BuffPosi].angle[3] != mAction || mFrameSelect != (int)(PlayBuff[BuffPosi].frame >> CHAR_FRAME_SELSHIFT)) {
			//if ( (cnt-LastCnt)>4 ) {
				//바뀐 모션 이벤트 기록
			mAction = PlayBuff[BuffPosi_Old].angle[3];
			memcpy(&lpTransPlayBuff->PlayBuff, &PlayBuff[BuffPosi_Old], sizeof(smPLAYBUFF));
			lpTransPlayBuff->BuffCount = (cnt * pBuffStep) - 1;
			lpTransPlayBuff++;
			buffCnt++;
			//}

			//바뀐 모션 이벤트 기록
			mAction = PlayBuff[BuffPosi].angle[3];
			mFrame = PlayBuff[BuffPosi].frame;
			memcpy(&lpTransPlayBuff->PlayBuff, &PlayBuff[BuffPosi], sizeof(smPLAYBUFF));
			lpTransPlayBuff->BuffCount = cnt * pBuffStep;
			lpTransPlayBuff++;
			buffCnt++;
			LastCnt = cnt;
			mFrameSelect = PlayBuff[BuffPosi].frame >> CHAR_FRAME_SELSHIFT;
		}

		BuffPosi_Old = BuffPosi;
	}

	BuffPosi = (StartPosi + SendTime - 1) & PLAYBUFF_MASK;

	//마지막 기록
	mAction = PlayBuff[BuffPosi].angle[3];
	memcpy(&lpTransPlayBuff->PlayBuff, &PlayBuff[BuffPosi], sizeof(smPLAYBUFF));
	lpTransPlayBuff->BuffCount = (SendTime * pBuffStep) - 1;
	lpTransPlayBuff++;
	buffCnt++;

	lpTransPlayData->StartPosi = (lpTransPlayData->StartPosi * pBuffStep) & PLAYBUFF_MASK;

	if (SameAction == TRUE && smCharInfo.State == smCHAR_STATE_USER && MotionInfo->State == CHRMOTION_STATE_STAND)
		buffCnt = 1;			//가만히 서있는 동작의 연속

	lpTransPlayData->PlayBuffCnt = buffCnt;
	lpTransPlayData->size = sizeof(smTRANS_PLAYDATA) + (sizeof(smTRANS_PLAYBUFF) * buffCnt);

	/////////////////// 추가 정보 뒤에 기록 /////////////////////


	MotionRecordCount++;


	//	if ( (MotionRecordCount&0x7)==0 ) {

			//추가 이펙트 정보가 있으면 주기적으로 전송
	lpEffectItem = (smEFFECT_ITEM*)(lpTargetBuff + lpTransPlayData->size);

	if (HvLeftHand.PatTool && HvLeftHand.dwItemCode && (HvLeftHand.ColorBlink || HvLeftHand.DispEffect)) {

		lpEffectItem->code = smTRANSCODE_EFFECT_ITEM;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = HvLeftHand.dwItemCode;
		lpEffectItem->ColorBlink = HvLeftHand.ColorBlink;
		memcpy(lpEffectItem->sColors, HvLeftHand.sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = HvLeftHand.DispEffect;
		lpEffectItem->BlinkScale = HvLeftHand.BlinkScale;


		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (HvRightHand.PatTool && HvRightHand.dwItemCode && (HvRightHand.ColorBlink || HvRightHand.DispEffect)) {

		lpEffectItem->code = smTRANSCODE_EFFECT_ITEM;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = HvRightHand.dwItemCode;
		lpEffectItem->ColorBlink = HvRightHand.ColorBlink;
		memcpy(lpEffectItem->sColors, HvRightHand.sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = HvRightHand.DispEffect;
		lpEffectItem->BlinkScale = HvRightHand.BlinkScale;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (dwWeaponEffectTime) {
		//속성 이펙트
		lpEffectItem->code = smTRANSCODE_EFFECT_WEAPON;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = WeaponEffect;
		lpEffectItem->ColorBlink = (dwWeaponEffectTime - dwPlayTime) + 500;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = EnchantEffect_Point;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (this == lpCurPlayer)
		Life = sinGetLife();
	else
		Life = smCharInfo.Life[0];

	if (PlayStunCount && Life > 0) {
		lpEffectItem->code = smTRANSCODE_EFFECT_STUN;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = PlayStunCount;
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (PlayHolyPower[1] && Life > 0) {
		lpEffectItem->code = smTRANSCODE_EFFECT_CURSE;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = PlayHolyPower[1];
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}
	/*
		if ( smCharInfo.State!=smCHAR_STATE_USER ) {
			//몬스터
			if ( PlaySlowCount>0 ) lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_SKIL] |= SKILL_APPMASK_ICE;
		}
	*/
	if (PlaySlowCount > 0 && Life > 0) {
		if (smCharInfo.State != smCHAR_STATE_USER || AttackIce || PlayDistortion) {
			lpEffectItem->code = smTRANSCODE_EFFECT_ICE;
			lpEffectItem->size = sizeof(smEFFECT_ITEM);
			lpEffectItem->dwItemCode = PlaySlowCount;
			lpEffectItem->ColorBlink = PlayDistortion;
			ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
			lpEffectItem->DispEffect = 0;
			lpEffectItem->BlinkScale = 0;

			lpTransPlayData->size += lpEffectItem->size;
			lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
			lpEffectItem++;
		}
	}

	if (PlayPoison[1] && Life > 0) {
		lpEffectItem->code = smTRANSCODE_EFFECT_POISION;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = PlayPoison[0];
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (PlayCurseQuest && Life > 0) {
		lpEffectItem->code = smTRANSCODE_EFFECT_CURSE_QUEST;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = PlayCurseQuest;
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (PlayVanish > 0 && Life > 0) {
		lpEffectItem->code = smTRANSCODE_EFFECT_VANISH;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = PlayVanish;
		lpEffectItem->ColorBlink = PlayVague;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (dwForceOfNatureTime > dwPlayTime) {
		lpEffectItem->code = smTRANSCODE_EFFECT_KEEPSKILL;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = SKILL_PLAY_FORCE_OF_NATURE;
		lpEffectItem->ColorBlink = dwForceOfNatureTime - dwPlayTime;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (dwHallOfValhallaTime > dwPlayTime) {
		lpEffectItem->code = smTRANSCODE_EFFECT_KEEPSKILL;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = SKILL_PLAY_HALL_OF_VALHALLA;
		lpEffectItem->ColorBlink = dwHallOfValhallaTime - dwPlayTime;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (PlayInvincible > 0) {
		lpEffectItem->code = smTRANSCODE_EFFECT_KEEPSKILL;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = SKILL_PLAY_BLESS_SIEGE_ITEM;
		lpEffectItem->ColorBlink = PlayInvincible;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}


	if (dwTradeMsgCode) {
		lpEffectItem->code = smTRANSCODE_EFFECT_PERSHOP;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = dwTradeMsgCode;
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (dwForceOrbTime && dwForceOrbTime > dwPlayTime) {
		lpEffectItem->code = smTRANSCODE_EFFECT_FORCEORB;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = dwForceOrbCode;
		lpEffectItem->ColorBlink = dwPlayTime - dwForceOrbTime;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}
	if (dwClanManageBit) {
		lpEffectItem->code = smTRANSCODE_EFFECT_CLANMGR;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = dwClanManageBit;
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

#ifndef _W_SERVER
	if (OnStageField >= 0 && StageField[OnStageField]->State == FIELD_STATE_CASTLE) {
		//블래스 캐슬 개인 정보
		if (sBlessCastle_Damage[0] || sBlessCastle_Damage[1]) {
			lpEffectItem->code = smTRANSCODE_EFFECT_BLESS_SCORE;
			lpEffectItem->size = sizeof(smEFFECT_ITEM);
			lpEffectItem->dwItemCode = 0;
			lpEffectItem->ColorBlink = 0;
			lpEffectItem->sColors[0] = sBlessCastle_Damage[0];
			lpEffectItem->sColors[1] = sBlessCastle_Damage[1];
			lpEffectItem->sColors[2] = 0;
			lpEffectItem->sColors[3] = 0;
			lpEffectItem->DispEffect = 0;
			lpEffectItem->BlinkScale = 0;

			lpTransPlayData->size += lpEffectItem->size;
			lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
			lpEffectItem++;
		}
	}

#endif

	srTransBuffSize = lpTransPlayData->size;

	return TRUE;
	}

//수신된 데이타의 장착 아이템에 이펙트 적용 
int smCHAR::SetTransEffectItems(smEFFECT_ITEM* lpEffectItem) { return TRUE; }

//수신된 데이타의 장착 무기에 속성 이펙트 적용 
int smCHAR::SetTransEffectWeapon(smEFFECT_ITEM* lpEffectItem) { return TRUE; }

//자신의 데이타를 소켓을 통하여 다른 곳으로 전송 ( 서버에서 클라이언트로 전송용 )
int smCHAR::SendPlayDataSock(smWINSOCK* lpsmsock, char* lpTransBuff, int ex, int ey, int ez)
{
	int SendTime;
	smPLAYDATA PlayData;
	int dist;
	int x, y, z;


	//	TransLastSendCnt++;

	if ((PlayBuffCnt & 0x3F) != 0) return FALSE;

	SendTime = 0x40;

	x = (pX - ex) >> FLOATNS;
	y = (pY - ey) >> FLOATNS;
	z = (pZ - ez) >> FLOATNS;

	dist = x * x + y * y + z * z;

	TransLevel = 3;

	if (dist < DIST_TRANSLEVEL_HIGH || !DisplayFlag) {
		TransLevel = 0;
	}
	else {
		if (dist < DIST_TRANSLEVEL_MID) {
			TransLevel = 1;
		}
		else {
			if (dist < DIST_TRANSLEVEL_LOW) {
				TransLevel = 2;
			}
		}
	}

	if (TransLevel > 1) {
		//버퍼링 없는 데이타 전송 ( 통신의 중요도가 낮음 )
		if ((PlayBuffCnt & 0xFF) != 0) return FALSE;

		PlayData.size = sizeof(smPLAYDATA);
		PlayData.code = smTRANSCODE_PLAYDATA1;
		PlayData.x = pX;
		PlayData.y = pY;
		PlayData.z = pZ;

		PlayData.angle[0] = Angle.x;
		PlayData.angle[1] = Angle.y;
		PlayData.angle[2] = Angle.z;
		PlayData.angle[3] = action;

		PlayData.frame = frame;
		PlayData.dwObjectSerial = dwObjectSerial;

		if (lpsmsock->Send((char*)&PlayData, sizeof(smPLAYDATA), TRUE)) {
			//SetDistSendCnt( player );				//통신 시간차 조절
		}
	}
	else {
		/*
		//버퍼링 데이타 전송 ( 고품질의 데이타 전송 )
		SendMode = 0;
		PlayerBuffCnt = PlayBuffCnt&PLAYBUFF_MASK;
		StartPosi = ( PlayerBuffCnt - SendTime) & PLAYBUFF_MASK;

		mAction = PlayBuff[ PlayerBuffCnt ].angle[3];


		for( cnt=SendTime; cnt>=0; cnt-- ) {
			BuffPosi = (StartPosi + cnt ) & PLAYBUFF_MASK;
			if ( PlayBuff[BuffPosi].angle[3]!=mAction ) {
				SendMode = 1;
				memcpy( &PlayData3.PlayBuff[2] , &PlayBuff[BuffPosi] , sizeof( smPLAYBUFF ) );
				PlayData3.BuffCount[2] = cnt;
				break;
			}
		}

		if ( !SendMode ) {
			memcpy( &PlayData2.PlayBuff[0] , &PlayBuff[ StartPosi ] , sizeof( smPLAYBUFF ) );
			memcpy( &PlayData2.PlayBuff[1] , &PlayBuff[ PlayerBuffCnt ] , sizeof( smPLAYBUFF ) );
			PlayData2.code = smTRANSCODE_PLAYDATA2;
			PlayData2.size = sizeof( smPLAYDATA2 );
			PlayData2.BuffCount[0] = 0;
			PlayData2.BuffCount[1] = SendTime;
			PlayData2.dwObjectSerial = dwObjectSerial;

			if ( lpsmsock->Send( (char *)&PlayData2 , sizeof( smPLAYDATA2 ) ) ) {
				//TransSendWait = 1;
				//TransLastSendCnt = PlayCounter;
				//TransLastSendTime = SendTime;
				//SetDistSendCnt( player );				//통신 시간차 조절
			}
		}
		else {
			memcpy( &PlayData3.PlayBuff[0] , &PlayBuff[ StartPosi ] , sizeof( smPLAYBUFF ) );
			memcpy( &PlayData3.PlayBuff[1] , &PlayBuff[ PlayerBuffCnt ] , sizeof( smPLAYBUFF ) );
			PlayData3.code = smTRANSCODE_PLAYDATA3;
			PlayData3.size = sizeof( smPLAYDATA3 );
			PlayData3.BuffCount[0] = 0;
			PlayData3.BuffCount[1] = SendTime;
			PlayData3.dwObjectSerial = dwObjectSerial;

			if ( lpsmsock->Send( (char *)&PlayData3 , sizeof( smPLAYDATA3 ) ) ) {
				//TransSendWait = 1;
				//TransLastSendCnt = PlayCounter;
				//TransLastSendTime = SendTime;
				//SetDistSendCnt( player );				//통신 시간차 조절
			}
		}
		*/

		//					lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_DEAD );
		//					if ( MotionInfo->State==CHRMOTION_STATE_ATTACK )


		if (lpsmsock->Send(lpTransBuff, ((smTRANS_PLAYDATA*)lpTransBuff)->size), TRUE) {
			//TransSendWait = 1;
			//TransLastSendCnt = PlayCounter;
			//TransLastSendTime = SendTime;
			//SetDistSendCnt( player );				//통신 시간차 조절
		}

	}
	//	}

	return TRUE;
}



//캐릭터의 포괄적 정보를 소켓으로 상대방에 전송
int smCHAR::SendCharInfo(smWINSOCK* lpsmsock)
{

	smTRNAS_PLAYERINFO	TransPlayerInfo;

	memcpy(&TransPlayerInfo.smCharInfo, &smCharInfo, sizeof(smCHAR_INFO));
	TransPlayerInfo.size = sizeof(smTRNAS_PLAYERINFO);
	TransPlayerInfo.code = smTRANSCODE_PLAYERINFO;
	TransPlayerInfo.dwObjectSerial = dwObjectSerial;

	TransPlayerInfo.x = pX;
	TransPlayerInfo.y = pY;
	TransPlayerInfo.z = pZ;

	return lpsmsock->Send((char*)&TransPlayerInfo, TransPlayerInfo.size, TRUE);
}

//스킬시작
int smCHAR::BeginSkill(int SkilCode, int Level, smCHAR* lpTarChar, int x, int y, int z) { return TRUE; }

//몬스터 일반 공격 시작
int smCHAR::BeginAttack_Monster() { return TRUE; }

//몬스터 스킬 시작
int smCHAR::BeginSkill_Monster() { return TRUE; }

//몬스터 스킬 이벤트
int smCHAR::EventSkill_Monster() { return TRUE; }

//스킬동작종료
int smCHAR::EndSkill() { return TRUE; }

//스킬이벤트
int smCHAR::EventSkill() { return TRUE; }


//특정 상대에게 공격을 가한다 ( 상대방의 소켓값 또는 캐릭터 포인터 [둘중 선택] )
int smCHAR::SendTransAttack(smCHAR* lpChar, smWINSOCK* lpsmsock, int AttackCode, int Add_Damage, int Resistance)
{
	TRANS_ATTACKDATA	TransAttackData;
	smWINSOCK* lpsmSendSock;
	rsPLAYINFO* lpPlayInfo = 0;
	char szDispDamage[128];
	smTRANS_COMMAND	smTransCommand;

	int	pow;

	int temp;
	int	accuracy;

#ifndef _W_SERVER
	//클라이언트 로그 파일 기록
	Record_ClinetLogFile("구버전 공격패킷 - ::SendTransAttack ");
	quit = 1;
	return TRUE;
#endif
	/*
		if ( this==lpCurPlayer &&
			(abs(dwPlayTime-dwLastMouseMoveTime)>1024*60*3 ||
			 abs(dwPlayTime-dwLastCharMoveTime)>1024*60*3 ||
			 abs(dwFuncChkTime-dwPlayTime)>14*60*1024 ) ||
			 RestartPlayCount!=0 ) {
			//2~3분 이상 마우스 움직임 없을땐 공격 무효 ( 보츠 플레이 자동 방어 )
			//2~3분 이상 캐릭터 제자리에 서있을때 공격 무효 ( 보츠 플레이 자동 방어 )
			return FALSE;
		}
	*/
	/*
		if ( smConfig.DebugMode ) {
			//스킬 테스트
			if ( lpChar && VRKeyBuff[ VK_CONTROL ] && MouseButton[1] ) {
				//범위형 공격
				//BeginSkill( SKILL_PLAY_METEO , 0 , lpChar ,0,0,0 );
				AddChatBuff( "메테오 사용불가~ 메롱" );
				return TRUE;
			}
		}
	*/

	if (lpsmsock) {
		lpPlayInfo = (rsPLAYINFO*)lpsmsock->ExtData1;
	}

	TransAttackData.code = smTRANSCODE_ATTACKDATA;
	TransAttackData.size = sizeof(TRANS_ATTACKDATA);

	TransAttackData.x = pX;
	TransAttackData.y = pY;
	TransAttackData.z = pZ;

	TransAttackData.AttackState = 1;
	TransAttackData.AttackSize = 48 * fONE;
	TransAttackData.Power = GetRandomPos(smCharInfo.Attack_Damage[0], smCharInfo.Attack_Damage[1]);

	TransAttackData.sRating[0] = smCharInfo.Level;
	TransAttackData.sRating[1] = smCharInfo.Attack_Rating;

	if (PlayHolyMind[1] > 0)		//공격력 감소 - 홀리마인드
		TransAttackData.Power -= ((TransAttackData.Power * PlayHolyMind[0]) / 100);

	if (!lpPlayInfo) return FALSE;


	//종성 아이템 시간종료 확인
	if (lpPlayInfo->dwSiegeItem_Scroll_Time && lpPlayInfo->dwSiegeItem_Scroll_Time < dwPlayServTime) {
		lpPlayInfo->dwSiegeItem_Scroll_Code = 0;
		lpPlayInfo->dwSiegeItem_Scroll_Time = 0;
	}
	if (lpPlayInfo->dwSiegeItem_Stone1_Time && lpPlayInfo->dwSiegeItem_Stone1_Time < dwPlayServTime) {
		lpPlayInfo->dwSiegeItem_Stone1_Code = 0;
		lpPlayInfo->dwSiegeItem_Stone1_Time = 0;
	}

	if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_B) {
		//공성 타워크리스탈 (아이스)
		TransAttackData.sRating[0] = -SKILL_PLAY_ICE_CRYSTAL;
		TransAttackData.sRating[1] = 3;
	}
	else if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_G)
	{
		//공성 타워크리스탈 (전기)
		TransAttackData.sRating[0] = -SKILL_PLAY_LIGHTNING_CRYSTAL;
		TransAttackData.sRating[1] = 2;
	}
	else if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_R) {
		//공성 타워크리스탈 (불)
		TransAttackData.sRating[0] = -SKILL_PLAY_FIRE_CRYSTAL;
		TransAttackData.sRating[1] = 1;
	}

	if (lpPlayInfo->dwSiegeItem_Stone1_Code && lpPlayInfo->dwSiegeItem_Stone1_Code == smCharInfo.dwCharSoundCode) {
		//크리스탈 저항석
		TransAttackData.Power /= 2;		//공격력 절반
		TransAttackData.sRating[0] = smCharInfo.Level;
		TransAttackData.sRating[1] = smCharInfo.Attack_Rating;
	}

	if (lpPlayInfo->dwSiegeItem_Scroll_Code == (sinBC1 | sin01)) {	//무적스크롤
		if (lpPlayInfo->Position.Area == rsCASTLE_FIELD) return FALSE;
	}
	if (lpPlayInfo->dwSiegeItem_Scroll_Code == (sinBI1 | sin05))		//무적스크롤
		return FALSE;


	if (lpPlayInfo->dwSiegeItem_Scroll_Code == (sinBC1 | sin03)) {	//추가 회피
		if (lpPlayInfo->Position.Area == rsCASTLE_FIELD) {
			if ((rand() % 100) < (int)lpPlayInfo->dwSiegeItem_Scroll_Param) {
#ifdef DISP_DAMAGE
				if (lpPlayInfo->AdminMode > 2) {
					wsprintf(szDispDamage, ">%s 공격 추가회피 ( Evade Scroll[%d] )", smCharInfo.szName, lpPlayInfo->dwSiegeItem_Scroll_Param);
					rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
				}
#endif
				//추가회피 정보 클라리언트에 출력
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.WParam = 0;
				smTransCommand.LParam = lpPlayInfo->dwSiegeItem_Scroll_Code;
				smTransCommand.SParam = dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
				return FALSE;
			}
		}
	}

	if (lpPlayInfo->dwSiegeItem_Scroll_Code == (sinBI1 | sin07)) {	//추가 회피
		if ((rand() % 100) < (int)lpPlayInfo->dwSiegeItem_Scroll_Param) {
#ifdef DISP_DAMAGE
			if (lpPlayInfo->AdminMode > 2) {
				wsprintf(szDispDamage, ">%s 공격 추가회피 ( Evade Scroll[%d] )", smCharInfo.szName, lpPlayInfo->dwSiegeItem_Scroll_Param);
				rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
			}
#endif
			//추가회피 정보 클라리언트에 출력
			smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = 0;
			smTransCommand.LParam = lpPlayInfo->dwSiegeItem_Scroll_Code;
			smTransCommand.SParam = dwObjectSerial;
			smTransCommand.EParam = 0;
			lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
			return FALSE;
		}
	}

	if (lpPlayInfo->dwSkill_PhysicalAbsorb_Time) {	//공격력 감소 (흡수력 미리적용)- 피지컬 사용
		if (lpPlayInfo->dwSkill_PhysicalAbsorb_Time > dwPlayServTime) {
			TransAttackData.Power -= lpPlayInfo->dwSkill_PhysicalAbsorb_Param;
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_PhysicalAbsorb_Time = 0;
			lpPlayInfo->dwSkill_PhysicalAbsorb_Param = 0;
		}
	}

	if (lpPlayInfo->dwSkill_MetalArmor_Time) {	//공격력 감소 (흡수력 미리적용)- 메탈아머 사용
		if (lpPlayInfo->dwSkill_MetalArmor_Time > dwPlayServTime) {
			TransAttackData.Power -= lpPlayInfo->dwSkill_MetalArmor_Param;
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_MetalArmor_Time = 0;
			lpPlayInfo->dwSkill_MetalArmor_Param = 0;
		}
	}


	if (lpPlayInfo->dwSkill_HolyBody_Time) {	//언데드 x% 공격력 감소 (흡수력 미리적용)- 홀리바디 사용
		if (lpPlayInfo->dwSkill_HolyBody_Time > dwPlayServTime) {
			if (smCharInfo.Brood == smCHAR_MONSTER_UNDEAD) {
				TransAttackData.Power -= (TransAttackData.Power * lpPlayInfo->dwSkill_HolyBody_Param) / 100;
				if (TransAttackData.Power < 0) return FALSE;
			}
		}
		else {
			lpPlayInfo->dwSkill_HolyBody_Time = 0;
			lpPlayInfo->dwSkill_HolyBody_Param = 0;
		}
	}

	if (lpPlayInfo->dwSkill_ExtreamShield_Time) {	//블럭율증가 - 익스트림 쉴드
		if (lpPlayInfo->dwSkill_ExtreamShield_Time > dwPlayServTime) {
			TransAttackData.AttackState |= (lpPlayInfo->dwSkill_ExtreamShield_Param << 16);
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_ExtreamShield_Time = 0;
			lpPlayInfo->dwSkill_ExtreamShield_Param = 0;
		}
	}


	if (lpPlayInfo->dwSkill_DivineInhalation_Time && smCharInfo.Brood == smCHAR_MONSTER_UNDEAD) {	//블럭시 생명력
		if (lpPlayInfo->dwSkill_DivineInhalation_Time > dwPlayServTime) {

			temp = smCharInfo.Resistance[sITEMINFO_BIONIC];
			if (temp) {
				if (temp >= 100) temp = 100;
				if (temp <= -100) temp = -100;
				temp = (D_Inhalation_Life[lpPlayInfo->dwSkill_DivineInhalation_Param] * temp) / 100;
				temp &= 0xFF;
				temp |= D_Inhalation_Block[lpPlayInfo->dwSkill_DivineInhalation_Param] << 8;
				if (temp) {
					TransAttackData.AttackState |= (temp << 16);
				}
			}
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_DivineInhalation_Time = 0;
			lpPlayInfo->dwSkill_DivineInhalation_Param = 0;
		}
	}

	if (lpPlayInfo->dwSkill_VirtualLife_Time) {	//가상 생명력을 높여서 공격력 감소
		if (lpPlayInfo->dwSkill_VirtualLife_Time > dwPlayServTime) {
			TransAttackData.Power -= (TransAttackData.Power * lpPlayInfo->dwSkill_VirtualLife_Param) / 100;
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_VirtualLife_Time = 0;
			lpPlayInfo->dwSkill_VirtualLife_Param = 0;
		}
	}

	if (lpPlayInfo->dwSkill_EnergyShield_Time) {	//에너지쉴드 공격력 감소
		if (lpPlayInfo->dwSkill_EnergyShield_Time > dwPlayServTime) {
			temp = (TransAttackData.Power * lpPlayInfo->dwSkill_EnergyShield_Param) / 100;
			TransAttackData.Power -= temp;
			TransAttackData.AttackState |= (temp << 16);
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_EnergyShield_Time = 0;
			lpPlayInfo->dwSkill_EnergyShield_Param = 0;
		}
	}

	if (lpPlayInfo->dwSkill_Compulsion_Time) {	//Compulsion 흡수력 추가
		if (lpPlayInfo->dwSkill_Compulsion_Time > dwPlayServTime) {
			TransAttackData.Power -= lpPlayInfo->dwSkill_Compulsion_Param;
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_Compulsion_Time = 0;
			lpPlayInfo->dwSkill_Compulsion_Param = 0;
		}
	}

	if (lpPlayInfo->dwSkill_Berserker_Time) {	//버서커 흡수력 감소
		if (lpPlayInfo->dwSkill_Berserker_Time > dwPlayServTime) {
			TransAttackData.Power -= Berserker_SubAbsorb[lpPlayInfo->dwSkill_Berserker_Param];
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_Berserker_Time = 0;
			lpPlayInfo->dwSkill_Berserker_Param = 0;
		}
	}


	//베이그 추가회피
	if (lpPlayInfo->dwSkill_Vague_Time) {
		if (lpPlayInfo->dwSkill_Vague_Time > dwPlayServTime) {
			if ((rand() % 100) < Vague_EvasionPercent[lpPlayInfo->dwSkill_Vague_Param]) {

#ifdef DISP_DAMAGE
				if (lpPlayInfo->AdminMode > 2) {
					wsprintf(szDispDamage, ">%s 공격 추가회피 ( Vague[%d] )", smCharInfo.szName, Vague_EvasionPercent[lpPlayInfo->dwSkill_Vague_Param]);
					rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
				}
#endif

				//추가회피 정보 클라리언트에 출력
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.WParam = 0;
				smTransCommand.LParam = SKILL_PLAY_VAGUE;
				smTransCommand.SParam = dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

				return FALSE;		//추가 회피 판정
			}
		}
		else {
			lpPlayInfo->dwSkill_Vague_Time = 0;
			lpPlayInfo->dwSkill_Vague_Param = 0;
		}
	}

	if (lpPlayInfo->dwSkill_GodlyShied_Time) {	//공격력 감소 (흡수력 미리적용)- 가들리쉴드사용
		if (lpPlayInfo->dwSkill_GodlyShied_Time > dwPlayServTime) {
			TransAttackData.Power -= (TransAttackData.Power * Godly_Shield_AbsorbPercent[lpPlayInfo->dwSkill_GodlyShied_Param]) / 100;
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_GodlyShied_Time = 0;
			lpPlayInfo->dwSkill_GodlyShied_Param = 0;
		}
	}


	//홀오브발할라 추가회피
	if (lpPlayInfo->dwSkill_HallOfValhalla_Time) {
		if (lpPlayInfo->dwSkill_HallOfValhalla_Time > dwPlayServTime) {
			if ((rand() % 100) < (int)lpPlayInfo->wSkill_HallOfValhalla_Param[0]) {
#ifdef DISP_DAMAGE
				if (lpPlayInfo->AdminMode > 2) {
					wsprintf(szDispDamage, ">%s 공격 추가회피 ( Hall of Valhalla [%d] )", smCharInfo.szName, lpPlayInfo->wSkill_HallOfValhalla_Param[0]);
					rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
				}
#endif
				//추가회피 정보 클라리언트에 출력
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.WParam = 0;
				smTransCommand.LParam = SKILL_PLAY_HALL_OF_VALHALLA;
				smTransCommand.SParam = dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

				return FALSE;		//추가 회피 판정
			}
		}
		else {
			lpPlayInfo->dwSkill_HallOfValhalla_Time = 0;
		}
	}

	if (lpPlayInfo->dwSkill_SummonMuspell_Time) {	//서몬 무스펠
		if (lpPlayInfo->dwSkill_SummonMuspell_Time > dwPlayServTime) {

			if ((rand() % 100) < Summon_Muspell_BlockPercent[lpPlayInfo->dwSkill_SummonMuspell_Param]) {
#ifdef DISP_DAMAGE
				if (lpPlayInfo->AdminMode > 2) {
					wsprintf(szDispDamage, ">%s 공격 추가 회피 ( Summon Muspell [%d] )", smCharInfo.szName, Summon_Muspell_BlockPercent[lpPlayInfo->dwSkill_SummonMuspell_Param]);
					rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
				}
#endif
				//추가회피 정보 클라리언트에 출력
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.WParam = 0;
				smTransCommand.LParam = SKILL_PLAY_SUMMON_MUSPELL;
				smTransCommand.SParam = dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

				return FALSE;
			}
			if (smCharInfo.Brood == smCHAR_MONSTER_UNDEAD) {
				//언데드 공격력 흡수
				temp = (TransAttackData.Power * Summon_Muspell_UndeadAbsorbPercent[lpPlayInfo->dwSkill_SummonMuspell_Param]) / 100;
				TransAttackData.AttackState |= (temp << 16);
				if (TransAttackData.Power < 0) return FALSE;
			}
		}
		else {
			lpPlayInfo->dwSkill_SummonMuspell_Time = 0;
			lpPlayInfo->dwSkill_SummonMuspell_Param = 0;
		}
	}


	if (lpPlayInfo->dwBlessSkill_Code) {
		if (lpPlayInfo->dwBlessSkill_Code == SKILL_PLAY_BLESS_ABSORB) {	//클랜스킬 (흡수)
			if (lpPlayInfo->dwBlessSkill_Time > dwPlayServTime) {
				TransAttackData.Power -= lpPlayInfo->dwBlessSkill_Param;
				if (TransAttackData.Power < 0) return FALSE;
			}
			else {
				lpPlayInfo->dwBlessSkill_Code = 0;
				lpPlayInfo->dwBlessSkill_Time = 0;
			}
		}
		else if (lpPlayInfo->dwBlessSkill_Code == SKILL_PLAY_BLESS_EVADE)
		{	//클랜스킬 (이베이드)
			if (lpPlayInfo->dwBlessSkill_Time > dwPlayServTime) {

				if ((rand() % 100) < (int)lpPlayInfo->dwBlessSkill_Param) {
#ifdef DISP_DAMAGE
					if (lpPlayInfo->AdminMode > 2) {
						wsprintf(szDispDamage, ">%s 공격 추가 회피 ( Clan Skill [%d] )", smCharInfo.szName, lpPlayInfo->dwBlessSkill_Param);
						rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
					}
#endif
					//추가회피 정보 클라리언트에 출력
					smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
					smTransCommand.size = sizeof(smTRANS_COMMAND);
					smTransCommand.WParam = 0;
					smTransCommand.LParam = SKILL_PLAY_BLESS_EVADE;
					smTransCommand.SParam = dwObjectSerial;
					smTransCommand.EParam = 0;
					lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

					return FALSE;
				}
			}
			else {
				lpPlayInfo->dwBlessSkill_Code = 0;
				lpPlayInfo->dwBlessSkill_Time = 0;
			}
		}
	}



	//해당 플레이어 흡수력 저주 걸렸음 ( 몬스터 공격력 증가 )
	if (lpPlayInfo->dwCurse_Defence_Time) {
		if (lpPlayInfo->dwCurse_Defence_Time > dwPlayServTime)
			TransAttackData.Power += lpPlayInfo->dwCurse_Defence_Param;
		else
			lpPlayInfo->dwCurse_Defence_Time = 0;
	}

	//공격력 강화
	if (PlayHolyPower[1]) {
		TransAttackData.Power += (TransAttackData.Power * PlayHolyPower[0]) / 100;
	}


	//추가 데미지
	if (Add_Damage) {
		TransAttackData.Power += (TransAttackData.Power * Add_Damage) / 100;
	}

	if (AttackCritcal) {
		//크리티컬 힛 ( 공격 파워를 1.5배로 )
		TransAttackData.Power = (TransAttackData.Power * 170) / 100;
		TransAttackData.AttackState = 2;			//크리티컬 데미지
	}

	//공격 효과
	if (AttackEffect)
		TransAttackData.AttackState = 2;

	if (TransAttackData.Power < 1) TransAttackData.Power = 1;

	if (this == lpCurPlayer && smConfig.DebugMode && smConfig.szFile_Player[0]) {
		//몬스터 스킬 공격력 업
		if (VRKeyBuff[VK_CONTROL])
			TransAttackData.Power = 200;
		else
			TransAttackData.Power = 30;
	}
	else {
		if (lpChar) {
			switch (lpChar->smCharInfo.State) {
			case smCHAR_STATE_ENEMY:
				//명중률을 리턴한다 
				dwBattleTime = dwPlayTime;						//전투 시간 기록


				temp = smCharInfo.Attack_Rating;
				//스카우트호크 ( 명중율 증가 )
				if (HoSkillCode) {
					switch (HoSkillCode) {
					case SKILL_SCOUT_HAWK:
						pow = GetSkillPower(this, lpChar, 0, 0, 0);
						if (pow) {
							smCharInfo.Attack_Rating += (smCharInfo.Attack_Rating * pow) / 100;
						}
						break;
					}
				}
				accuracy = sinGetAccuracy(lpChar->smCharInfo.Level, lpChar->smCharInfo.Defence);
				smCharInfo.Attack_Rating = temp;
				if ((rand() % 100) > accuracy)
					return FALSE;		//방어율 실패
				break;

			case smCHAR_STATE_NPC:
				TransAttackData.AttackState = Resistance;
				Resistance = 0;
				break;
			}
		}
	}

	if (Resistance) {
		TransAttackData.AttackState |= (Resistance << 16);
		pow = Resistance >> 4;
		if (pow > 0 && pow < 512) {
			TransAttackData.Power += pow;			//속성 공격력 추가 ( 서버에서 추가된 공격력 따로 계산 )
		}
	}

	LastAttackDamage = TransAttackData.Power;			//최신 공격 데미지 기록
	TransAttackData.dwDestObjectSerial = dwObjectSerial;
	TransAttackData.dwTarObjectSerial = 0;
	TransAttackData.dwChkSum = TransAttackData.Power * 2002 + (TransAttackData.x * TransAttackData.y * TransAttackData.z);// 조작 방지 코드
	lpsmSendSock = lpsmsock;


	if (lpChar) {
		if (lpChar->TransSock)
			lpsmSendSock = lpChar->TransSock;

		if (lpChar->ServerCode) {
			lpsmSendSock = GetServerSock(lpChar->ServerCode);		//서버 코드 구하기
		}

		TransAttackData.dwTarObjectSerial = lpChar->dwObjectSerial;

		//처음 공격하는 캐릭한테만 체력바 출력
		if (lpChar->smCharInfo.State == smCHAR_STATE_ENEMY && lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0] == lpChar->smCharInfo.Life[1]) {
			lpChar->EnableStateBar = TRUE;
		}
	}


	if (lpsmSendSock) {
		if (!ServerMode) {
			//보낸 공격력 기록
			Record_SendDamage(lpsmSendSock, TransAttackData.Power);
		}

		pow = TransAttackData.Power;

		if (lpsmsock) {
			if (rsServerConfig.CrazyPacketMode && lpPlayInfo && lpPlayInfo->dwDecPacketCode &&
				lpPlayInfo->dwDecPacketTime2 < dwPlayServTime) {					//크레이지 패킷 모드

				//S2C 공격 첵크섬 ( 크레이지 패킷 )
				TransAttackData.code = smTRANSCODE_ATTACKDATA2;
				TransAttackData.dwDamageChkSum = dm_GetDamgeChkSum_S2V(&TransAttackData);
				rsEncodeDamagePacket(lpPlayInfo, &TransAttackData);
			}
			else
				TransAttackData.dwDamageChkSum = dm_GetDamgeChkSum_S2V(&TransAttackData);
		}

		if (lpsmSendSock->Send((char*)&TransAttackData, TransAttackData.size, TRUE)) {
			return pow;
		}
	}


	return FALSE;
}



//공격 사정거리에 들어왔으면 공격 ( 좌표 와 사정거리 )
int smCHAR::PlayAttackFromPosi(int ex, int ey, int ez, int Dist, int attack)
{
	int AtDist;
	int	dDist;
	int ay;

	ay = abs(pY - ey);

	if (OnStageField >= 0 && StageField[OnStageField]->FieldCode == rsCASTLE_FIELD) {
		//공성전 필드 공격 설정 보정
		if (ShootingMode) {
			if (ay > 80 * fONE) return NULL;
		}
		else {
			if (ay > 52 * fONE) return NULL;
		}

		//성문 경유해서 공격 금지
		if ((pZ<CastleBattleZone_DoorLineZ && ez>CastleBattleZone_DoorLineZ) ||
			(pZ > CastleBattleZone_DoorLineZ && ez < CastleBattleZone_DoorLineZ))
		{
			if (rsBlessCastle.CastleMode && rsBlessCastle.DefenceLevel == 0) {
				return NULL;
			}
		}
	}
	else
		if (ay > 64 * fONE) return NULL;

	AtDist = GetDistanceDbl(pX, pZ, ex, ez);
	dDist = (Dist >> FLOATNS);
	dDist *= dDist;

	if (AtDist < dDist) {
		if (MotionInfo->State != CHRMOTION_STATE_ATTACK &&
			MotionInfo->State != CHRMOTION_STATE_SKILL && MotionInfo->State != CHRMOTION_STATE_DEAD) {

			if (attack) {
				//마을에선 공격 불가
				if (smCharInfo.State != smCHAR_STATE_USER || (OnStageField >= 0 && StageField[OnStageField]->State != FIELD_STATE_VILLAGE)) {
					SetMotionFromCode(CHRMOTION_STATE_ATTACK);
					Angle.y = GetRadian2D(pX, pZ, ex, ez);
					AttackX = ex;
					AttackY = ey;
					AttackZ = ez;
				}
			}

			return TRUE;
		}
	}

	return NULL;
}



//모션부려 초기화
int InitMotionBlur() { return TRUE; }








int GetProjPosi(int& x3d, int& y3d, int& z3d, POINT3D* CameraPosi, POINT3D* CameraAngle)
{

	int x2d, y2d;
	smMATRIX	trans, rX, rY, rZ, rm;
	//	smPOINT3D	*pV;
	int sx, sy, sz;
	int cx, cy, cz;

	cx = x3d - CameraPosi->x;
	cy = y3d - CameraPosi->y;
	cz = z3d - CameraPosi->z;

	smIdentityMatrix(rX);
	smIdentityMatrix(rY);
	smIdentityMatrix(rZ);
	smIdentityMatrix(trans);
	smZeroMatrix(rm);

	smRotateXMatrix(rX, (-CameraAngle->x) & ANGCLIP);
	smRotateYMatrix(rY, (-CameraAngle->y) & ANGCLIP);
	smRotateZMatrix(rZ, (-CameraAngle->z) & ANGCLIP);
	smTranslateMatrix(trans, cx, cy, cz);

	smMatrixMult(rm, trans, rY);
	smMatrixMult(rm, rm, rX);
	smMatrixMult(rm, rm, rZ);


	sx = x3d * rm._11 +
		y3d * rm._21 +
		z3d * rm._31;


	sy = x3d * rm._12 +
		y3d * rm._22 +
		z3d * rm._32;

	sz = x3d * rm._13 +
		y3d * rm._23 +
		z3d * rm._33;

	x3d = (sx >> FLOATNS) + rm._41;
	y3d = (sy >> FLOATNS) + rm._42;
	z3d = (sz >> FLOATNS) + rm._43;


	if (z3d > 0)
	{
		x3d = MidX + ((x3d * viewdistZ) / z3d);
		y3d = MidY - ((y3d * viewdistZ) / z3d);
		z3d >>= FLOATNS;
		return TRUE;

	}
	else
	{
		x2d = CLIP_OUT;
		y2d = CLIP_OUT;
		return FALSE;
	}

}




scITEM::scITEM()
{
	Init();
}

scITEM::~scITEM()
{
	Close();
}

void scITEM::Init()
{
	ZeroMemory(&Head, sizeof(scITEM));
}

int scITEM::Close()
{

	Flag = 0;

	if (lpDinaPattern) {
		AddRemoveDynPattern(lpDinaPattern);
		//smDPAT_Delete( lpDinaPattern );
		lpDinaPattern = 0;
	}

	Pattern = 0;

	return TRUE;
}

//패턴설정
int scITEM::SetPattern(smPAT3D* pat)
{
	Pattern = pat;
	return TRUE;
}


//위치설정
int scITEM::SetPosi(int x, int y, int z)
{
	//	int py1;
	int py2 = 0;

	pX = x;
	pZ = z;
	pY = y;

	if (pY == 0) {

		if (smGameStage[0])
			pY = smGameStage[0]->GetHeight(pX, pZ);

		if (smGameStage[1])
			py2 = smGameStage[1]->GetHeight(pX, pZ);

		if (py2 > pY) pY = py2;
	}

	return TRUE;
}


//수신된 아이템 설정
int scITEM::SetTransItem(TRANS_ITEM* lpTransItem)
{
	dwLastTransTime = dwPlayTime;

	//	state = lpTransItem->state

	Flag = TRUE;

	lstrcpy(szName, lpTransItem->szName);
	//lstrcpy( szModelName , lpTransItem->szModelName );
	SetPosi(lpTransItem->x, lpTransItem->y, lpTransItem->z);
	lpStgArea = lpTransItem->lpStgArea;

	ItemCode = lpTransItem->ItemCode;


	Angle.y = ((pX + pZ) >> 2) & ANGCLIP;

	if (sinGetItemInfo(ItemCode, szModelName) == FALSE || ItemCode == 0) {
		lstrcpy(szModelName, "char\\flag\\flag.ini");
	}

	/*
		switch ( ItemCode ) {
		case 0:
			lstrcpy( szModelName , "char\\flag\\flag.ini" );
			break;
		case 1:
			lstrcpy( szModelName , "DropItem\\DRcoin.ASE" );
			break;
		case 2:
			lstrcpy( szModelName , "weapons\\Weapon_low\\Iw07.ASE" );
			break;
		}
	*/

	if (!PatLoading)
		AddLoaderItem(this, szModelName);

	return TRUE;
}




#define ITEM_TRANS_LIMIT_TIME		10000

int scITEM::Draw() { return TRUE; }


//////////////////////////////// 무적모드 악용 대비 감시함수 ////////////////////////////

DWORD	Ptect_RestartCounter = 0;
DWORD	Ptect_RestartMask = 0;

//공격 마크 보안 카운터 증가
int	Ptect_IncAttackCount(int Flag)
{
	DWORD	rc;
	smTRANS_COMMAND	smTransCommand;

	rc = Ptect_RestartCounter ^ Ptect_RestartMask;
	Ptect_RestartMask = dwPlayTime;

	switch (Flag) {
	case -1:
		rc = 0;
		break;
	case 0:
		rc++;
		break;
	}

	if (rc > 160) {
		//서버에 경고 보냄
		smTransCommand.code = smTRANSCODE_WARNING_BLINK_ATT;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = rc;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;
		if (smWsockDataServer) smWsockDataServer->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
		rc = 0;
	}

	rc = rc ^ Ptect_RestartMask;
	Ptect_RestartCounter = rc;

	return TRUE;
}

//공격 마크 보안 카운터 얻기
DWORD Ptect_GetAttackCount()
{
	DWORD	rc;
	rc = Ptect_RestartCounter ^ Ptect_RestartMask;
	Ptect_RestartMask = dwPlayTime;
	Ptect_RestartCounter = rc ^ Ptect_RestartMask;

	return rc;
}



//캐슬 마스터 업데이트
int UpdateCastleMasterClan(DWORD dwMasterClan)
{
	int cnt;

	SetClanMaster_Player(dwMasterClan);

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial) {
			if (chrOtherPlayer[cnt].smCharInfo.ClassClan && GetClanCode(chrOtherPlayer[cnt].smCharInfo.ClassClan) == dwMasterClan)
				lpCurPlayer->Clan_CastleMasterFlag = TRUE;
			else
				lpCurPlayer->Clan_CastleMasterFlag = FALSE;
		}
	}

	return TRUE;
}

//플레이어 클랜 마스터 확인
int SetClanMaster_Player(DWORD dwMasterClan)
{
	if (lpCurPlayer->smCharInfo.ClassClan && GetClanCode(lpCurPlayer->smCharInfo.ClassClan) == dwMasterClan)
		lpCurPlayer->Clan_CastleMasterFlag = TRUE;
	else
		lpCurPlayer->Clan_CastleMasterFlag = FALSE;

	return lpCurPlayer->Clan_CastleMasterFlag;

}