#pragma once

#include "smPacket.h"
#include "smLib3d\smObj3d.h"
#include "smLib3d/smStage3d.h"

#define PLAYBUFF_SIZE		256
#define PLAYBUFF_MASK		255

#define PLAYCOUNT_PER_SECOND		70

#define DIST_TRANSLEVEL_NEAR	0x12000				//((64*3)^2)*2	
#define DIST_TRANSLEVEL_HIGH	0x48000				//((64*6)^2)*2	
#define DIST_TRANSLEVEL_MHIGH	0xC8000				//((64*10)^2)*2	
#define DIST_TRANSLEVEL_MID		0x120000			//((64*12)^2)*2		
#define DIST_TRANSLEVEL_LOW		0x320000			//((64*20)^2)*2	

#define DIST_TRANSLEVEL_DISCONNECT	0x320000			//((64*20)^2)*2	
#define DIST_TRANSLEVEL_CONNECT		0x120000			//((64*12)^2)*2	

#define	PARTY_GETTING_DIST		(18*64*18*64)
#define	PARTY_GETTING_DIST2		(17*64*17*64)

#define PLAYSERVER_SENDCOUNTER		(70*1)

#define	ICE_FOOT_COUNT_MAX		20
#define	ICE_FOOT_LIFE_TIME		5000
#define	ICE_FOOT_ALPHA_STEP		(200.f / float(ICE_FOOT_LIFE_TIME))

struct SIceFootInfo
{
	int     Life;
	int     TypeNum;
	DWORD   dwPlayTime;
	POINT3D FootPos;
	POINT3D FootAngle;
};

#define DPAT_MAX		2048

#include <list>

namespace Delta3D::Graphics
{
	class Mesh;
};

struct ModelGroup
{
	std::list<Delta3D::Graphics::Mesh*> meshes;
};

struct smDPAT 
{
	smDPAT* lpTalkLink;

	smDPAT* smDinaLink;
	smPAT3D* Pat;

	char	szPatName[64];
	int		UseCount;
	DWORD	dwSpeedFindSum;

	smMODELINFO* lpModelInfo;

	struct ModelsGroup
	{
		ModelGroup sHighModel;
		ModelGroup sDefaultModel;
		ModelGroup sLowModel;
	};
	ModelsGroup* sModelsGroup = nullptr;

	DWORD	LastUsedTime;
};


class smPATTERN 
{
public:

	smPATTERN *BipPattern;

	smDPAT	DinaPat[ DPAT_MAX ];

	smPATTERN(void);
	~smPATTERN(void);

	void Init();
	int	Close();

	int GetNew();
	int FindFromName( char *szName );
	int FindFromCode( DWORD dwCode );
	smDPAT *LoadCharactor( char *szFileName ) ;
	smDPAT *LoadBipPattern( char *szFileName ) ;

};

#define	DISP_MODE_PATMAIN	1
#define	DISP_MODE_PATSUB	2
#define	DISP_MODE_PATTALK	4
#define CHAR_FRAME_MASK		0x00FFFFFF
#define CHAR_FRAME_SELSHIFT	24	

namespace Delta3D::Graphics
{
	class Material;
};

struct smCHARTOOL
{
	smOBJ3D* ObjBip;
	smPAT3D* PatTool;
	DWORD	dwItemCode;

	int	SizeMax, SizeMin;

	int		ColorBlink;
	union
	{
		short	sColors[4];

		struct
		{
			int iAgeLevel;
			int iMixID;
		};
	};
	DWORD	DispEffect;
	int		BlinkScale;
	int		EffectKind;

	union
	{
		Delta3D::Graphics::Material* blendingMaterial = nullptr;
		int		TexMixCode;
	};
	int TexScroll;
};



//////////// 메카니션 스킬 //////////////
#define SKILL_PLAY_EXTREME_SHIELD	0x01
#define SKILL_PLAY_MECHANIC_BOMB	0x02
#define	SKILL_PLAY_PHYSICAL_ABSORB	0x03
#define	SKILL_PLAY_POISON_ATTRIBUTE	0x04

#define	SKILL_PLAY_GREAT_SMASH		0x05
#define	SKILL_PLAY_MAXIMIZE			0x06
#define	SKILL_PLAY_AUTOMATION		0x07
#define	SKILL_PLAY_SPARK			0x08

#define	SKILL_PLAY_METAL_ARMOR		0x09
#define	SKILL_PLAY_GRAND_SMASH		0x0A
#define	SKILL_PLAY_SPARK_SHIELD		0x0B

#define	SKILL_PLAY_IMPULSION		0x0C
#define	SKILL_PLAY_COMPULSION		0x0D
#define	SKILL_PLAY_MAGNETIC_SPHERE	0x0E
#define	SKILL_PLAY_METAL_GOLEM		0x0F


//////////// 파이크맨 스킬 //////////////
#define SKILL_PLAY_PIKEWIND			0x12	// 박재원 - 매직 포스 사용시 1차스킬로 인식 못하여서 할당된 코드 수정(0x10 -> 0x12 로 수정함)
#define SKILL_PLAY_CRITICAL_HIT		0x13
#define SKILL_PLAY_JUMPING_CRASH	0x14

#define SKILL_PLAY_GROUND_PIKE		0x15
#define SKILL_PLAY_TORNADO			0x16
#define SKILL_PLAY_EXPANSION		0x18

#define SKILL_PLAY_VENOM_SPEAR		0x19

#define SKILL_PLAY_BLADE_OF_BLAZE	0x1A		//(구)
#define SKILL_PLAY_VANISH			0x1A		//SKILL_PLAY_BLADE_OF_BLAZE 랑 같은 코드

#define SKILL_PLAY_CHAIN_LANCE		0x1B

#define SKILL_PLAY_ASSASSIN_EYE		0x1C
#define SKILL_PLAY_CHARGING_STRIKE	0x1D
#define SKILL_PLAY_VAGUE			0x1E
#define SKILL_PLAY_SHADOW_MASTER	0x1F


//////////// 파이터 스킬 //////////////
#define SKILL_PLAY_RAVING			0x23
#define SKILL_PLAY_IMPACT			0x24

#define SKILL_PLAY_TRIPLE_IMPACT	0x25
#define SKILL_PLAY_BRUTAL_SWING		0x26
#define SKILL_PLAY_ROAR				0x27
#define SKILL_PLAY_RAGEOF_ZECRAM	0x28

#define SKILL_PLAY_CONCENTRATION	0x29
#define SKILL_PLAY_AVANGING_CRASH	0x2A
#define SKILL_PLAY_SWIFT_AXE		0x2B
#define SKILL_PLAY_BONE_SMASH		0x2C

#define SKILL_PLAY_DESTROYER		0x2D
#define SKILL_PLAY_BERSERKER		0x2E
#define SKILL_PLAY_CYCLONE_STRIKE	0x2F

//////////// 아쳐 스킬 //////////////

#define SKILL_PLAY_SCOUT_HAWK		0x41
#define SKILL_PLAY_WIND_ARROW		0x43
#define SKILL_PLAY_PERFECT_AIM		0x44

#define SKILL_PLAY_FALCON			0x46
#define SKILL_PLAY_ARROWOF_RAGE		0x47
#define SKILL_PLAY_AVALANCHE		0x48

#define SKILL_PLAY_ELEMENTAL_SHOT	0x49
#define SKILL_PLAY_GOLDEN_FALCON	0x4A
#define SKILL_PLAY_BOMB_SHOT		0x4B
#define SKILL_PLAY_PERFORATION		0x4C

#define SKILL_PLAY_RECALL_WOLVERIN	0x4D
#define SKILL_PLAY_PHOENIX_SHOT		0x4E
#define SKILL_PLAY_FORCE_OF_NATURE	0x4F


////////////// 나이트 스킬 //////////////
#define SKILL_PLAY_SWORD_BLAST		0x51
#define SKILL_PLAY_HOLY_BODY		0x52
#define SKILL_PLAY_DOUBLE_CRASH		0x54

#define SKILL_PLAY_HOLY_VALOR		0x55
#define SKILL_PLAY_BRANDISH			0x56
#define SKILL_PLAY_PIERCING			0x57
#define SKILL_PLAY_DRASTIC_SPIRIT	0x58

#define SKILL_PLAY_FLAME_BRANDISH		0x59
#define SKILL_PLAY_DIVINE_INHALATION	0x5A
#define SKILL_PLAY_HOLY_INCANTATION		0x5B
#define SKILL_PLAY_GRAND_CROSS			0x5C

#define SKILL_PLAY_DIVINE_PIERCING		0x5D
#define SKILL_PLAY_GODLY_SHIELD			0x5E
#define SKILL_PLAY_GODS_BLESS			0x5F
#define SKILL_PLAY_SWORD_OF_JUSTICE		0x50


////////////// 아탈란타 스킬 ///////////////
#define SKILL_PLAY_SHIELD_STRIKE	0x61
#define SKILL_PLAY_FARINA			0x62
#define SKILL_PLAY_VIGOR_SPEAR		0x64

#define SKILL_PLAY_WINDY			0x65
#define SKILL_PLAY_TWIST_JAVELIN	0x66
#define SKILL_PLAY_SOUL_SUCKER		0x67
#define SKILL_PLAY_FIRE_JAVELIN		0x68

#define SKILL_PLAY_SPLIT_JAVELIN		0x69
#define SKILL_PLAY_TRIUMPH_OF_VALHALLA	0x6A
#define SKILL_PLAY_LIGHTNING_JAVELIN	0x6B
#define SKILL_PLAY_STORM_JAVELIN		0x6C

#define SKILL_PLAY_HALL_OF_VALHALLA		0x6D
#define SKILL_PLAY_X_RAGE				0x6E
#define SKILL_PLAY_FROST_JAVELIN		0x6F
#define SKILL_PLAY_VENGEANCE			0x60


////////////// 프리스티스 스킬 ///////////////
#define SKILL_PLAY_HEALING			0x71
#define SKILL_PLAY_HOLY_BOLT		0x72
#define SKILL_PLAY_MULTI_SPARK		0x73
#define SKILL_PLAY_HOLY_MIND		0x74

//#define	SKILL_PLAY_MEDITATION		0x75
#define	SKILL_PLAY_DIVINE_LIGHTNING	0x76
#define	SKILL_PLAY_HOLY_REFLECTION	0x77
#define	SKILL_PLAY_GREAT_HEALING	0x78

#define	SKILL_PLAY_VIGOR_BALL		0x79
#define	SKILL_PLAY_RESURRECTION		0x7A
#define	SKILL_PLAY_EXTINCTION		0x7B
#define	SKILL_PLAY_VIRTUAL_LIFE		0x7C

#define	SKILL_PLAY_GLACIAL_SPIKE	0x7D
#define	SKILL_PLAY_REGENERATION_FIELD	0x7E
#define	SKILL_PLAY_CHAIN_LIGHTNING	0x7F
#define	SKILL_PLAY_SUMMON_MUSPELL	0x70


////////////// 매지션 스킬 ///////////////
#define SKILL_PLAY_AGONY			0x81
#define SKILL_PLAY_FIRE_BOLT		0x82
#define SKILL_PLAY_ZENITH			0x83
#define SKILL_PLAY_FIRE_BALL		0x84

//#define SKILL_PLAY_MENTAL_MASTERY	0x85
#define SKILL_PLAY_COLUMN_OF_WATER	0x86
#define SKILL_PLAY_ENCHANT_WEAPON	0x87
#define SKILL_PLAY_DEAD_RAY			0x88

#define SKILL_PLAY_ENERGY_SHIELD	0x89
#define SKILL_PLAY_DIASTROPHISM		0x8A
#define SKILL_PLAY_SPIRIT_ELEMENTAL	0x8B
#define SKILL_PLAY_DANCING_SWORD	0x8C

#define SKILL_PLAY_FIRE_ELEMENTAL	0x8D
#define SKILL_PLAY_FLAME_WAVE		0x8E
#define SKILL_PLAY_DISTORTION		0x8F
#define SKILL_PLAY_METEO			0x80

///////////// 모션 확장 //////////////////
#define SKILL_PLAY_DIVINE_PIERCING2		0x90
#define SKILL_PLAY_DIVINE_PIERCING3		0x91

#define SKILL_TOPLVL					0x92
#define SKILL_PLAY_EVASION_MASTERY		0x93
////////////////////////////////////////////

#define SKILL_PLAY_BLESS_ABSORB			0xA0
#define SKILL_PLAY_BLESS_DAMAGE			0xA1
#define SKILL_PLAY_BLESS_EVADE			0xA2
#define SKILL_PLAY_BLESS_SIEGE_ITEM		0xA8

#define SKILL_PLAY_FIRE_CRYSTAL			0xB0
#define SKILL_PLAY_LIGHTNING_CRYSTAL	0xB1
#define SKILL_PLAY_ICE_CRYSTAL			0xB2
#define SKILL_PLAY_CHAOSCARA_VAMP		0xB8

#define SKILL_PLAY_PET_ATTACK		0xC0
#define SKILL_PLAY_PET_ATTACK2		0xC1

#define SKILL_PLAY_SOD_ITEM			0xD0
//#define SKILL_PLAY_METEO			0xE0
#define SKILL_PLAY_SPEACIAL			0xF0
#define	SKILL_PLAY_LOVELY_LIFE		0xF1

#define	SKILL_EFFECT_FIREFLOWER		0x12000010


//스킬 적용 마스크
#define	SKILL_APPMASK_EXTREME_SHIELD		0x0001
#define	SKILL_APPMASK_PHYSICAL_ABSORB		0x0002
#define	SKILL_APPMASK_AUTOMATION			0x0004
#define	SKILL_APPMASK_ANGER					0x0008
#define	SKILL_APPMASK_SPARK_SHIELD			0x0010
#define	SKILL_APPMASK_GODLY_SHIELD			0x0020


#define	SKILL_APPMASK_HOLY_BODY				0x00010000


//스킬 적용 마스크 ( 몬스터 속성 )
//#define	SKILL_APPMASK_ICE					0x0010


//데미지를 준 유저를 기억시킴
struct	ATTACK_DAMAGE_LIST	{
	DWORD	dwUserCode;
	int		DamageCount;
	int		Count;
};

#define	ATTACK_DAMAGE_LIST_MAX			100


class smCHAR {

	DWORD	Head;

public:
	
	int		DisplayFlag;		//화면 출력 여부
	//int		DisplayHead;		//머리 츨력 여부
	int		DisplayTools;		//무기 출력 여부

	smSTAGE3D	*lpStage;		//해당 배경 포인트
	DWORD		dwObjectSerial;	//객체 고유 번호
	int			AutoPlayer;		//자동 플레이어 ( TRUE 일 경우 서버로 부터 움직임을 받는다 )

	//Equipe na arena
	int nEquipeArena = 0;

	//N�vel do vip
	int vipLevel = 0;

	smPAT3D	*Pattern;			//일차 패턴
	smPAT3D	*Pattern2;			//머리 패턴
	smPAT3D *AnimPattern;

	//######################################################################################
	//작 성 자 : 오 영 석
	smPAT3D *TalkPattern;		// 표정 패턴. ( 다이나믹 패턴은 ini 에 따라서 lpDinaPattern 또는 lpDinaPattern2 )
	//######################################################################################

	smDPAT  *lpDinaPattern;		//다이나믹 패턴 포인터 (패턴의 관리자)
	smDPAT  *lpDinaPattern2;	//다이나믹 패턴 포인터 머리(패턴의 관리자)

	smDPAT  *lpDinaLeftPattern;		//왼쪽 무기
	smDPAT  *lpDinaRightPattern;	//오른쪽 무기

	smCHARTOOL	HvLeftHand;		//왼손에 착용 도구
	smCHARTOOL	HvRightHand;	//오른손에 착용 도구

	smMATRIX	HvLeftHand_Matrix;	//왼손 무기 행렬 복사
	smMATRIX	HvRightHand_Matrix; //오른손 무기 행렬 복사

	int		Rend_HvLeftHand;		//무기 렌더링 유무
	int		Rend_HvRightHand;		//무기 렌더링 유무


	smOBJ3D	*BackObjBip[3];		//등쪽 무기 착용
	smOBJ3D	*AttackObjBip;		//공격 패드 오브젝트
	smOBJ3D	*ShieldObjBip;		//방패 패드 오브젝트

	int		AttackToolRange;	//공격 사정거리
	int		AttackAnger;		//공격시 분도치 ( 그냥 보기 좋게 하기 위함 )
	int		AttackIce;			//얼음 공격 받음

	BOOL	bActive = TRUE;
	BOOL	bPartyLeader = FALSE;
	union
	{
		struct
		{
			int pX;
			int pY;
			int pZ;
		};

		Point3D sPosition;
	};
	
	int		Flag;
	int		AttackX,AttackY,AttackZ;		// 좌표
	int		PHeight;		// 바닥 높이 좌표
	int		FallHeight;		// 떨어진 높이 
	int		OnStageField;	// 밟고 서있는 바닥 필드 번호
	int		OnStageFieldState;	//바닥 필드의 속성값

	int		PatLoading;		//현재 로딩 중인지...

	int		PatWidth;		//캐릭터 넓이
	int		PatHeight;		//캐릭터 높이

	int		PatSizeLevel;	//캐릭터 크기 단계 ( 0 ~ 3 )

	int		OverLapPosi;	//다른 캐릭터와 위치가 겹침
	int		OverLapPosi2;	//다른 캐릭터와 위치가 겹침
	smCHAR	*lpCharOverLap;	//겹친 캐릭터의 포인트

	POINT3D Posi;

	union
	{
		struct
		{
			POINT3D Angle;
		};

		Point3D sAngle;
	};

	int		MoveMode;	//이동 모드 ( 0 - 걷기 1-달리기 )
	int		MoveFlag;
	int		MoveCnt;	// 통신시 받은 데이타가 없을경우 자동 이동할때 거리 제한 카운터
	int		tx,tz;		// 목표 이동 좌표
	int		TargetMoveCount;	//목표 이동 좌표 카운터
	int		WaterHeight;		//물 높이

	DWORD	dwActionItemCode;	//주 움직임에 관련된 코드
	DWORD	dwItemSetting;		//아이템 세팅 방법( 0-구분 없음 1-마을필드 구분 )
	DWORD	dwActionItemTwoHand;	//양손 무기 첵크

	short	wStickItems[4];		//장착 아이템 번호

	//화살 및 던지기 모드
	int		ShootingMode;	//발사형 공격 모드 ( TRUE )
	int		ShootingFlag;	//현재 발사됬는지 여부
	int		ShootingKind;	//발사 구분
	POINT3D	ShootingPosi;	//발사 위치

	POINT3D	ShootingAngle;	//발사 방향
	int		ShootingCount;	//발사 카운터

	smCHAR	*chrAttackTarget;	//공격 목표 캐릭터
	int		AttackCritcal;		//크리티컬 공격 유무
	int		AttackCriticalCount;//크리티컬 공격 카운터 ( 연속 공격시 )
	int		AttackExp;			//공격시 경험치
	int		AttackSkil;			//스킬 공격 코드
	int		AttackEffect;		//공격시 특수 이펙트 표시
	int		LastSkillParam;		//최근 스킬공격 구분 코드 ( 카오스카라 땜에 땜빵용으로 추가 ) 

	int		RecvExp;			//경험치 부여 받음

	POINT3D	ptNextTarget;	//다음 이동 목표
	POINT3D	PosiAround;		// 주변 위치 이곳을 멀리 벗어나지 못하게 함 (근처에서만..)
	int		DistAroundDbl;	// 주변 경계 거리 한계치
	int		TragetTraceMode;	//추적 모드 값 (서버사용 )

	POINT3D	PosBeginMove;		//이동 시작시의 좌표
	DWORD	PosBeginCount;

	DWORD	dwNextMotionCode;	//다음 동작 예약

	int		ChargingFlag;		//차징 플랙

	//######################################################################################
	//작 성 자 : 오 영 석
	DWORD	TalkFrame;		// 표정 프레임
	DWORD	TalkSoundSum;

	SMotionStEndInfo	FrameInfo;
	//######################################################################################

	int		action;
	DWORD	frame;
	int		FrameCnt;
	int		FrameStep;		//프레임 바뀜 값
	int		MoveSpeed;		//이동 움직임 값
	int		AttackSpeed;	//공격 속도

	int		ActionPattern;	// 움직임 패턴

	DWORD	dwEventFrameExt[4];	//이벤트 프레임 확장

	int		NearPlayCount;	//근처에 다른 유저가 존재 하는지 확인 ( NPC/몬스터 )
	int		ReopenCount;	//몬스터 시간이 지나 재출현 시킬 카운터
	int		AutoMoveStep;	//자동 이동시 스텝 카운터
	short	sMoveStepCount[2];		//이동 스텝 루프 가운터

	int		Counter;
	int		FrameCounter;			//모션 시작부터 끝날때 까지 사용되는 카운터

	int		RendSucess;				//화면상의 렌더링 성공 여부
	RECT	RendRect2D;				//렌더링된 2D 좌표 영역
	POINT3D	RendPoint;				//렌더링된 2D 좌표 ( x,y,z )
	int		FlagShow;				//화면표시 플랙 ( 0 - 화면 출력 불허 )

	//채팅
	char	szChatMessage[256];		//채팅 문자열
	DWORD	dwChatMessageTimer;		//채팅 문자를 표시 종료시간 기록

	//상점
	char	szTradeMessage[128];	//상점 메세지 문자열
	DWORD	dwTradeMsgCode;			//상점 메세지 코드


	char	srTransBuff[TRANS_BUFF_SIZE];	//전송할 데이타 대기용 버퍼
	int		srTransBuffSize;				//전송할 데이타 대기용 버퍼 크기

	//통신 관련
	int		ServerCode;				//해당 캐릭터 서버 코드

	smWINSOCK *TransSock;

	int		TransSendWait;			//통신 포트 기다림 
	int		TransLastSendCnt;		//통신 마지막 전송 시간
	int		TransLastSendTime;		//통신 마지막 전송의 시간차 ( 속도 첵크용 )
	int		TransMinCnt;			//최소 교신 카운터
	int		TransLevel;				//통신의 중요 단계
	int		TransDelayMax;			//통신 지연 최대치 ( 이시간을 넘으면 재시도 )

	smPLAYBUFF	PlayBuff[PLAYBUFF_SIZE];	//플레이 진행 상황을 버퍼에 저장하여 통신시 버퍼링함
	int		PlayBuffCnt;					//PlayBuff 의 카운터
	int		PlayBuffPosi_End;				//PlayBuff 버퍼링 끝 위치

	DWORD	dwLastTransTime;				//마지막 교신 시간
	int		LastPlayDataType;				//마지막 교신한 데이타 타입

	smCHAR_INFO		smCharInfo;				//유저 정보
	smCHAR_MONSTER_INFO	smMonsterInfo;		//몬스터 관련 구조

	int		AnimDispMode;			//동작 표시 모드 ( 0-모두 표시 1-한개씩 표시)
	int		PatDispMode;			//패턴 표시 모드 ( 0-비표시 1-메인 2-서브만 3-모두 )

	int		MotionSelectFrame;		//현재의 프레임의 모션의 번호 ( 0 , 1 ) 

	smMODELINFO		*smMotionInfo;	//모델의 동작별 프레임과 정보
	smMODELINFO		*smMotionInfo2;	//모델이 2개일 경우 1차모델에서 데이타가 없는경우 2차모델의 데이타를 출력 
	smMOTIONINFO	*MotionInfo;

	//######################################################################################
	//작 성 자 : 오 영 석
	smMODELINFO		*lpTalkModelParent;
	smMOTIONINFO	*lpTalkMotionInfo;
	//######################################################################################


	DWORD			OldMotionState;			//이전의 모션 속성 정보
	DWORD			dwTarget;				//상대방이 주인공을 겨냥한 액션일경우 

	//소환된 캐릭터 ( 마스터 유저가 존재 )
	rsPLAYINFO		*lpMasterPlayInfo;		//주인 캐릭터 (서버용)

	int						nCheckChar;		//내가 불러낸 공격 캐릭터 구분 (서버용) // 장별 - 소울스톤

	//목적 캐릭터 ~~ 
	rsPLAYINFO		*lpTargetPlayInfo;		//목표 캐릭터 (서버용)
	smCHAR			*lpTargetChar;			//목표 캐릭터 (클라이언트용)
	int				AttackUserFlag;			//몬스터가 유저를 공격함
	DWORD			dwTargetLockTime;		//목표 변경 금지

	DWORD			dwLinkObjectCode;		//연결된 오브젝트 코드
	rsPLAYINFO		*lpLinkPlayInfo;		//연결된 플레이어
	smCHAR			*lpLinkChar;			//연결된 캐랙터

	//공격당한 캐릭터 정보 ( 몬스터 경험치 배분을 목적 - 서버용 )
	rsPLAYINFO		*lpExpAttackPlayInfo;
	int				ExpAttackLife;
	DWORD			dwExpAttackTime;
	TRANS_ATTACKDATA AttackTrans;			//공격 데이타 버퍼
	DWORD			dwAttackPlayTime;		//공격 적용 시간

	int				PotionLog;				//포션 사용 기록
	int				LevelLog;				//레벨 변화 기록
	int				CriticalLog;			//크리티컬 로그

	int				EnableStateBar;			//체력바 그리기 옵션
	int				MotionRecordCount;		//모션 정보 기록 카운터
	int				OpenStartPostion;		//캐릭터의 시작 출신깃발 번호 (서버용)

	int				DisplayAlpha;			//반투명도 ( 임시 반투명 출력 )
	int				RendAlpha;				//반투명도 ( 원래 반투명 캐릭터 )
	int				RenderLatter;			//렌더링 나중에

	DWORD			dwDispAppSkill;			//스킬 적용표시
	DWORD			dwDispAppSkillMask;		//스킬 적용표시 마스크 ( 동기를 맞추기 위함 )
	int				HideWeapon;				//무기 숨김
	int				MotionLoop;				//반복 모션 횟수
	int				MotionLoopSpeed;		//반복 모션용 프레임 속도
	int				MotionEvent;			//모션 이벤트 플랙
	int				WeaponEffect;			//무기용 속성 이펙트
	DWORD			dwWeaponEffectTime;		//무기용 속성 이펙트 카운터
	int				EnchantEffect_Point;	//스킬 EnchantWeapon 의 스킬 포인트

	rsPLAYINFO		*lpCompulsionPlayInfo;	//시선끌기 캐릭터
	DWORD			dwCompulsionTime;		//시선끌기 시간

	DWORD			dwAssassinEyeTime;		//어세신아이 적용 몬스터
	int 			AssassinEyeParam;		//어세신아이 적용 몬스터


	////////////////////// 클랜 //////////////////////////
	int				ClanInfoNum;			//클랜 정보 번호
	DWORD			dwClanInfoTime;			//클랜 정보 받은 시간
	DWORD			dwClanManageBit;		//클랜 관리 표시
	int				Clan_CastleMasterFlag;	//성주 클랜원 이다!

	//확장용
	void			*lpExt1;				//(서버) 필드 구조체 연결 포인터 
	void			*lpExt2;
	void			*lpExt3;
	void			*lpExt4;

	POINT3D			HoSkillPos;				//스킬 관련 좌표
	int				HoSkillCode;			//관련 스킬 코드
	int				HoSkillMode;			//스킬 소환수 행동모드

	int				PartyFlag;				//파티 플랙
	int				PartyParam;				//파티 파라메터

	int				PlaySlowCount;		//느려지는 공격 받음
	int				PlaySlowSpeed;		//느려지는 속도
	int				PlayStunCount;		//중립 상태 카운터
	int				PlayStopCount;		//움직임이 멈춤 카운터
	short			PlayHolyMind[2];	//데미지 약화 기능 ( [0] 감소된 데미지% [1] 유지시간 )
	short			PlayHolyPower[2];	//데미지 강화 기능 ( [0] 강화된 데미지% [1] 유지시간 )
	short			PlayPoison[2];		//독에 공격 ( [0] 감소될 데미지 [1] 해독시간 )
	short			PlayHolyIncantation[2];	//몬스터 현혹 스킬 ( [0] 임시 [1] 유지시간 )
	int				PlayVanish;			//배니쉬 스킬적용
	int				PlayCurseQuest;		//저주 퀘스트 모드
	int				PlayVague;			//보그 스킬적용
	int				PlayDistortion;		//디스토션 (몬스터 왜곡)
	int				PlayInvincible;		//무적 아이템
	int				PlayCursePvP;		// reipvp xxstr
	int			    PlayCurseTopLVL;	// toplvl xxstr
	int				PlayEvasionMastery;
	int				PlayerPvPMode;
	BOOL			CheckFlagIce;


	int				DispPoison;

	DWORD			dwForceOfNatureTime;
	DWORD			dwHallOfValhallaTime;


	int				AttackSkillRange;

	int				EventAttackCount;
	DWORD			dwEventAttackParam;

	short			sAddColorEffect[4];
	int				DispLifeBar;

	DWORD			dwUpdateCharInfoTime;
	DWORD			dwLastRecvAttackTime;

	int				TryAttackCount;

	DWORD			dwForceOrbCode;
	DWORD			dwForceOrbTime;

	DWORD			dwLifeBoosterCode;
	DWORD			dwLifeBoosterTime;
	DWORD			dwManaBoosterCode;
	DWORD			dwManaBoosterTime;
	DWORD			dwStaminaBoosterCode;
	DWORD			dwStaminaBoosterTime;

	DWORD			dwSiegeItem_ScrollCode_Eternal;
	DWORD			dwSiegeItem_ScrollTime_Eternal;			
	DWORD			dwSiegeItem_ScrollCode_Fatal;
	DWORD			dwSiegeItem_ScrollTime_Fatal;		
	DWORD			dwSiegeItem_ScrollCode_Avert;
	DWORD			dwSiegeItem_ScrollTime_Avert;	

	DWORD			dwSkillDelayCode;
	DWORD			dwSkillDelayTime;

	int				PkMode_CharState; //(smCHAR_STATE_NPC / ENEMY / USER)
	int				DontMoveFlag;
	DWORD			dwClanCode;

	int				UseObject_VirtualLife;
	short			sObject_VirtualLife[2];
	short			sObject_DisplayLife[2];

	short			sBlessCastle_Damage[2];		//0-Damage 1-Killing Count


	ATTACK_DAMAGE_LIST	*lpAttackDamageList;
	ATTACK_DAMAGE_LIST* lpAttackDamageList_BlessCastle;

	float DissolveAmount = 0.f;
	int iMountID = 0;
	class CMountHandler* pMount = nullptr;

	smCHAR();
	~smCHAR();
	void Init();
	int Close();

	int SetPosi( int x, int y, int z , int angX, int angY, int angZ );

	int SetTargetPosi( int x, int z );
	int SetTargetPosi2( int x, int z );

	int GetAttackPoint( int *nX, int *nY, int *nZ );
	int GetToolBipPoint( smCHARTOOL *ChrTool, int *nX, int *nY, int *nZ );
	int SetNextTarget( int x, int y, int z );

	int MoveAngle( int step );
	int MoveAngle2( int step );

	int ChangeMoveMode();

	int StartHandEffect(int Mode);

	int SetAction( int nAction );
	int ChangeMotion( int Motion , int DpMode=0 );

	int AutoChangeTalkMotion( int TalkModeType );

	int FindActionFromFrame( int sframe );

	int SetDinaPattern( smDPAT	*lpDPat );
	int SetDinaPattern2( smDPAT	*lpDPat );

	int SetTool( DWORD dwItemCode , int hvPosi  );

	int SetPattern( smPAT3D *pat );
	int SetPattern2( smPAT3D *pat );

	int SavePlayBuff();

	int SavePlayBuff2();

	int Main();
	int Draw(bool DrawCharacterTarget = false);
	int VirtualDraw();

	int ShootingMain();
	
	int	ChargingSkill();
	int EventAttack( int Flag=0 );

	int CheckShootingTest(smCHAR *lpChrTarget);

	int CheckShootingTest_LastAttackTime( smCHAR *lpChar );

	int	SetSmoking( int level );

	SIceFootInfo	m_IceFootInfo[ ICE_FOOT_COUNT_MAX ];

	void SetIceFoot( int TypeNum, DWORD dwPlayTime, int x, int z );
	int  DrawIceFoot(void);	

	int DrawShadow();
	int DrawStateBar(int x, int y, int w, int h);
	int DrawStateBar2( int x, int y );
	int DrawChargingBar( int x, int y , int Charging , int ChargingMax );

	int DrawMotionBlurTool( smCHARTOOL *ChrTool );
	int DrawMotionBlur();

	int SetLoadPattern( char *szName );

	int MakePlayBuffFromPosi( int px, int py, int pz , int anX,int anY, int anZ , int act );
	int MakePlayBuffFromRecvData( smPLAYBUFF *StartBuff , smPLAYBUFF *EndBuff , int len );

	int MakeTransPlayData( char *lpTargetBuff , int SendTime , int pBuffStep=1 );

	int SetTransEffectItems(smEFFECT_ITEM *lpEffectItem);
	int SetTransEffectWeapon(smEFFECT_ITEM *lpEffectItem);

	int TransDelayCheck();
	int SetDistSendCnt( smCHAR *player );

	int SetMotionFromCode( DWORD MotionCode );

	int FindMotionCountFromCode( DWORD MotionCode );

	int FormSetPlayerInfo();

	int SetTransPlayerInfo( smTRNAS_PLAYERINFO *lpTransPlayerInfo );

	int SetTransPlayerInfoQuick( smTRNAS_PLAYERINFO_QUICK *lpTransPlayerInfo );

	DWORD GetAttackTrans_XorCode();
	int ResetAttackTrans();
	int XorAttackTrans();

	int PlayAttack( int EventFlag=0 );

	int RecvPlayData2( char *lpData );
	int RecvPlayData( smTHREADSOCK *RecvData );
	int SendPlayData( smCHAR *player );


	int SendCharInfo( smWINSOCK *lpsmsock );

	int SendPlayDataSock( smWINSOCK *lpsmsock , char *lpTransBuff , int ex, int ey, int ez );

	int SendTransAttack( smCHAR *lpChar , smWINSOCK *lpsmsock , int AttackCode , int Add_Damage=0 , int Resistance=0 );

	int PlayAttackFromPosi( int ex, int ey, int ez, int Dist ,int attack=TRUE );

	int BeginSkill( int SkilCode , int Level , smCHAR *lpTarChar , int x, int y, int z );
	int EventSkill();
	int BeginSkill_Monster();
	int EventSkill_Monster();
	int BeginAttack_Monster();
	int EndSkill();

};

/*
#define CHRMOTION_STAND		0
#define CHRMOTION_WALK		1
#define CHRMOTION_RUN		2
#define CHRMOTION_JUMP		3
#define CHRMOTION_ATTACK	4
*/

#define CHRMOTION_EXT		10

#define hvPOSI_RHAND	0x0001
#define hvPOSI_LHAND	0x0002


#define CHRMOTION_STATE_STAND		0x40
#define CHRMOTION_STATE_WALK		0x50
#define CHRMOTION_STATE_RUN			0x60
#define CHRMOTION_STATE_FALLDOWN	0x80

#define CHRMOTION_STATE_ATTACK		0x100
#define CHRMOTION_STATE_DAMAGE		0x110
#define CHRMOTION_STATE_DEAD		0x120
#define CHRMOTION_STATE_SOMETIME	0x130
#define CHRMOTION_STATE_EAT			0x140
#define CHRMOTION_STATE_SKILL		0x150

#define CHRMOTION_STATE_FALLSTAND	0x170
#define CHRMOTION_STATE_FALLDAMAGE	0x180

#define CHRMOTION_STATE_RESTART		0x200
#define CHRMOTION_STATE_WARP		0x210

#define CHRMOTION_STATE_YAHOO		0x220

#define CHRMOTION_STATE_HAMMER		0x300

#define CHRMOTION_STATE_TALK_AR			0x400
#define CHRMOTION_STATE_TALK_E			0x410
#define CHRMOTION_STATE_TALK_OH			0x420
#define CHRMOTION_STATE_TALK_EYE		0x430
#define CHRMOTION_STATE_TALK_BLANK		0
#define CHRMOTION_STATE_TALK_SMILE		0x440
#define CHRMOTION_STATE_TALK_GRUMBLE	0x450
#define CHRMOTION_STATE_TALK_SORROW		0x460
#define CHRMOTION_STATE_TALK_STARTLED	0x470
#define CHRMOTION_STATE_TALK_NATURE		0x480
#define CHRMOTION_STATE_TALK_SPECIAL	0x490

int smCHAR_SetCameraPosi( int x,int y,int z, int angX,int angY,int angZ );

extern CRITICAL_SECTION	cLoadSection;

int smDPAT_Delete( smDPAT *dPat );
int AnimObjectTree( smOBJ3D *tObj , int frame , int ax, int ay, int az );

int TestSetNewText( char *str ) ;

int InitMotionBlur();

int GetSinItemNumFromCode( DWORD CODE );

int InitPatterns();
int ClosePatterns();
int SetLoadPattern( smCHAR *smChar , char *szName , POINT3D *Posi , POINT3D *Angle );
int SetLoadPattern( smCHAR *smChar , char *szName , char *szName2 , POINT3D *Posi , POINT3D *Angle );

int	AddLoaderPattern( smCHAR *lpChar , char *szName , char *szName2=0 );
int AddLoaderStage( smSTAGE3D *lpStage , char *szName );
int AnimObjectTree( smOBJ3D *tObj , int frame , int ax, int ay, int az );

class scITEM {

public:
	DWORD	Head;

	int		DisplayFlag;

	int		Flag;
	int		pX,pY,pZ;

	char	szModelName[64];
	char	szName[64];

	DWORD   iClass;

	void	*lpStgArea;

	smPAT3D	*Pattern;
	smDPAT  *lpDinaPattern;

	int		PatLoading;

	int		ItemCode;

	DWORD	dwLastTransTime;

	int		ServerCode;	

	POINT3D	RendPoint;


	POINT3D Posi;
	POINT3D Angle;


	scITEM();
	~scITEM();

	void Init();
	int Close();

	int SetPosi( int x, int y, int z );
	int SetTransItem( TRANS_ITEM *lpTransItem );

	int SetPattern( smPAT3D *pat );
	int SetPosi( int x, int y, int z , int angX, int angY, int angZ );
	int Draw();

};

int	AddLoaderItem( scITEM *lpItem , char *szName );
DWORD GetSpeedSum( char *szName );

extern smPATTERN smBipPattern;
extern smPATTERN smPattern;


int	Ptect_IncAttackCount( int Flag );
DWORD Ptect_GetAttackCount();

extern int	SkillChargingFlag;

extern TRANS_SKIL_ATTACKDATA	Trans_SplashCharList;

extern BOOL isEnemy;

extern int smCHAR_CameraX;
extern int smCHAR_CameraY;
extern int smCHAR_CameraZ;

extern int CastleBattleZone_LineZ;
extern int CastleBattleZone_DoorLineZ;
extern int CastleBattleZone_DoorLineX[2];
extern int CastleBattleZone_BridgeZ;
extern int CastleSoulFountain_Area[4];

int UpdateCastleMasterClan( DWORD dwMasterClan );
int SetClanMaster_Player( DWORD dwMasterClan );