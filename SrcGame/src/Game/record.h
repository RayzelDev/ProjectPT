#pragma once

#define RECORD_ITEM_MAX		200
#define	TRANS_RECORD_LEN	(smSOCKBUFF_SIZE-32)


#define	PRIME_ITEM_PACKAGE_NONE			0
#define	PRIME_ITEM_PACKAGE_BRONZE		1
#define	PRIME_ITEM_PACKAGE_SILVER		2
#define	PRIME_ITEM_PACKAGE_GOLD			3
#define	PRIME_ITEM_PACKAGE_ULTRA		4
// pluto 패키지2
#define	PRIME_ITEM_PACKAGE_BRONZE2		5
#define	PRIME_ITEM_PACKAGE_SILVER2		6
#define	PRIME_ITEM_PACKAGE_GOLD2		7
#define	PRIME_ITEM_PACKAGE_ULTRA2		8
#define	PRIME_ITEM_PACKAGE_NONE_50_EXPUP		 9 // 박재원 - 경험치증가 포션(50%) 아이템 전용 
#define	PRIME_ITEM_PACKAGE_NONE_100_EXPUP		10 // 박재원 - 경험치증가 포션(100%) 아이템 전용 

//저장될 아이템 구조
struct	sRECORD_ITEM {
	int	ItemCount;
	int x,y;			        //아이템이 그려질 좌표 
	int	ItemPosition;			//코드가 저장

	sITEMINFO sItemInfo;		//아이템 정보
};

#define	_SIN_MAX_USE_SKILL	16

//저장될 스킬 구조
struct	RECORD_SKILL {
	BYTE	bSkillPoint[_SIN_MAX_USE_SKILL];			//스킬 포인트
	WORD	wSkillMastery[_SIN_MAX_USE_SKILL];		//스킬 숙련도
	BYTE	bShortKey[_SIN_MAX_USE_SKILL];			//펑션 키
	WORD	wSelectSkill[2];						//선택된 스킬
	//int		RemainPoint;
};

//퀘스트 저장 정보
struct	sQUEST_INFO	{
	WORD	wQuestCode[2];

	DWORD	Data[7];
};

#define	LAST_QUEST_MAX		32
#define	LAST_QUEST_MASK		31

struct	sLAST_QUEST_INFO {
	WORD wLastQuest[LAST_QUEST_MAX];
	int	LastQuestCount;
};

extern sLAST_QUEST_INFO	RecordLastQuestInfo;			//지난 퀘스트 정보


//게임 저장될 데이타 구조체 ( 캐릭터 정보 이외에 퀘스트 나 파티 정보 )
struct	sGAME_SAVE_INFO {
	DWORD	Head;

	int		PlayStageNum;			//플레이중인 위치
	int		CameraMode;				//카메라 설정값
	int		PosX,PosZ;				//로그아웃 한 좌표
	int		LastMoeny;				//마지막 저장안된 돈

	DWORD	dwChkSum_CharInfo;		//캐릭터 정보 첵크섬 ( 소켓 후킹을 통한 조작 방지 )

	RECORD_SKILL	SkillInfo;		//스킬 정보	[ 크기 17*sizeof( DWORD ) ]

	int		SaveTime;				//저장한 시간 ( Time_T )

	int		PCRNo;					//PC방 정보
	short	EventPlay[2];			//이벤트정보

	short	sPetInfo[2];			//펫정보 저장

	short	sPotionUpdate[2];		//물약 업데이트 정보
	short	sPotionCount[3][4];		//실제 물약 갯수
	WORD	wForceOrbUsing[2];		//포스오브 사용 [0]->FO1(XX) [1]->Time(Sec)

	DWORD	dwLevelQuestLog;		//레벨 퀘스트 수행 여부

	int		ShortKey_NormalAttack;	//기본 공격 단축키 정보

	int		BlessCastleTax;			//블레스 캐슬 현재 세율
	DWORD	dwBlessCastleMaster;	//블레스 캐슬 지배자 (클랜코드)

	DWORD	dwElementaryQuestLog;	//초보 퀘스트 수행 여부

	DWORD	dwTime_PrimeItem_X2;			//아이템 2배 드롭 // 써드아이즈
	DWORD	dwTime_PrimeItem_ExpUp;			//경험치업 포션
	DWORD	dwTime_PrimeItem_VampCuspid;	//피채워주는 아이템유효시간
	DWORD	dwTime_PrimeItem_ManaRecharg;	//마나채워주는 아이템 유효시간
	DWORD	dwPrimeItem_PackageCode;		//패키지아이템코드
	DWORD	dwTime_PrimeItem_MightofAwell;	// pluto 마이트 오브 아웰	소지량 +300
	DWORD	dwTime_PrimeItem_MightofAwell2;	// pluto 마이트 오브 아웰	소지량 +500
	DWORD	dwTime_PrimeItem_ManaReduce;	// pluto 마나 리듀스 포션
	DWORD	dwTime_PrimeItem_PhenixPet;		// pluto 펫(해외)
	DWORD	dwTime_PrimeItem_HelpPet;		// 박재원 - 빌링 도우미 펫 추가
	DWORD	dwTime_PrimeItem_VampCuspid_EX;	// 장별 - 뱀피릭 커스핏 EX
	DWORD	dwTime_PrimeItem_StaminaReduce;	// 박재원 - 근력 리듀스 포션

	int		TotalSubExp;			//그냥 쳐서 얻은 경험치
	int		TotalMoney;				//클라이언트에서 집계한 획득한 돈
	int		TotalExp;				//클라이언트에서 집계한 획득한 경험치

	char	szMasterID[32];			//캐릭터 만든 ID

	sQUEST_INFO			QuestInfo;		//퀘스트 진행정보 ( 일단 1개만 만들었다 )	- 크기 DWORD *8
	sLAST_QUEST_INFO	LastQuestInfo;	//진행 완료된 퀘스트

	DWORD	TT_ServerID;				//토너먼트 서버ID

	WORD	wLifeBoosterUsing[2];		// 박재원 - 부스터 아이템(생명력)
	WORD	wManaBoosterUsing[2];		// 박재원 - 부스터 아이템(기력)
	WORD	wStaminaBoosterUsing[2];	// 박재원 - 부스터 아이템(근력)

	WORD	wSkillDelayUsing[2];	// 장별 - 스킬 딜레이

	DWORD	dwTime_Hat;
	DWORD	HatKind;
	DWORD	dwTime_PrimeItem_Mount;
	DWORD	dwPrimeItem_MountParam;

	DWORD	dwTemp2[30];
};


//저장될 전체 구조 
struct	TRANS_RECORD_DATA {
	int size,code;

	char	szHeader[8];

	smCHAR_INFO smCharInfo;

	sGAME_SAVE_INFO	GameSaveInfo;

	sTHROW_ITEM_INFO	ThrowItemInfo[THROW_ITEM_INFO_MAX];		//버려진 아이템 정보
	int		ThrowItemCount;										//버려진 아이템 카운터

	int		ItemCount;
	int		ItemSubStart;

//	sRECORD_ITEM	RecordItems[RECORD_ITEM_MAX];
	int		DataSize;
	BYTE	Data[ sizeof(sRECORD_ITEM)*RECORD_ITEM_MAX ];
};

//저장될 구조 파트
struct	TRANS_RECORD_DATAS {
	int	size,code;
	int	Count , Total;
	int	TransSize;

	char	szData[TRANS_RECORD_LEN];
};
#define sPLAY_CHAR_MAX		6

//이름 , 레벨 , 종족 , 직업 


struct	sPLAY_USER_DATA {
	char	szHeader[16];						//구분 헤더
	char	szID[32];							//ID

	char	szCharName[sPLAY_CHAR_MAX][32];		//캐릭터 이름
};



struct	_TRANS_CHAR_INFO {
	char	szName[32];				//이름
	char	szModelName[64];		//모델 이름
	char	szModelName2[64];		//모델 이름2
	DWORD	JOB_CODE;				//직업 코드 
	int		Level;					//레벨 
	DWORD	Brood;					//종족
	DWORD	dwArmorCode;			//갑옷 코드
	int		StartField;				//시작할 게임 필드
	int		PosX,PosZ;				//로그아웃 좌표
	DWORD	dwTemp[13];				//예비 버퍼
};

struct	TRANS_USERCHAR_INFO {
	int		size,code;

	char	szID[32];
	int		PlayUserCount;						//캐릭터 수만큼 카운트

	_TRANS_CHAR_INFO	CharInfo[sPLAY_CHAR_MAX];
};


class	rsRECORD_DBASE {
public:
	TRANS_RECORD_DATA	TransRecordData;
	int	TransLastPartCount;
	BYTE	TransDataBlock[64];
	DWORD	dwRecordTime;				//저장시도 시간
	DWORD	dwNextRecordTime;			//다음 저장 시간

	//저장할 데이타를 정리하여 제작
	int MakeRecordData( smCHAR_INFO *lpCharInfo , sITEM *lpItems , sITEM *lpItems2 , sITEM *lpMouseItem=0 );
	//불러온 데이타 정보를 해당 위치에 설정
	int	ResotrRecordData( smCHAR_INFO *lpCharInfo , sITEM *lpItems , sITEM *lpItems2 , sITEM *lpMouseItem=0 );

	//클라이언트 측
	//저장할 데이타를 분할하여 서버로 전송
	int SendRecordDataToServer( smWINSOCK *lpsmSock );
	//저장할 데이타를 분할하여 받음 ( 저장 완료되면 TRUE 반환 )
	int RecvRecordDataFromServer( TRANS_RECORD_DATAS *lpTransRecord );



	int IsData( char *szName );

};

int GetUserDataFile( char *szName , char *szFileName );




//데이타 저장서버 디렉토리 생성
int CreateDataServerDirectory();

//종료된 퀘스트 기록 추가
int Record_LastQuest( WORD	wQuestCode );
//지난 퀘스트 검사
int FindLastQuestCode( WORD wQuestCode );

//스킬 첵크
int	CheckSkillPoint( int Level , RECORD_SKILL *lpRecordSkill , int *spTotal , DWORD dwLevelQuestLog );






int SetServerID( char *szID , char *szServerID );

//////////////////////////////////////////////////
//시간 확인하여 저장시도
int rsTimeRecData();




int GetSkillPoint_LevelQuest( int Level , DWORD dwLevelQuestLog );
//퀘스트로 획득한 스텟 포인트
int GetStatePoint_LevelQuest( int Level , DWORD dwLevelQuestLog );
//캐릭터 스텟 포인트 확인 및 재설정
int ReformCharStatePoint( smCHAR_INFO *lpCharInfo , DWORD dwLevelQuestLog );


//로그인 서버 보안키 확인
int	rsCheck_LoginServerSafeKey( smCHAR_INFO *lpCharInfo , DWORD dwSpdSumCode=0 );
//로그인 서버 보안키 설정
int rsSet_LoginServerSafeKey( smCHAR_INFO *lpCharInfo );
//로그인 서버 보안 키 생성
DWORD	rsGetLoginSafeKey( smCHAR_INFO *lpCharInfo , DWORD dwSpdSumCode=0 );



extern int	Permit_CheckMoney ;
extern int	Permit_CheckExp ;


struct	sBILLING_INFO {
	int	PCRNo;
	int	EventPlay[2];
};

extern sBILLING_INFO	*sBiInfo;