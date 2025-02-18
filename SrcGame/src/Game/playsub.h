#pragma once

#define	CHAR_LEVEL_MAX	145 // N�vel maximo aqui

#define sinNPC_SHOP				0x00000001
#define sinNPC_MIX				0x00000002
#define sinNPC_AGING			0x00000004
#define sinNPC_WARE				0x00000008
#define sinNPC_SKILL			0x00000010
#define sinNPC_FORCE			0x00000020
#define sinNPC_SMELTING			0x00000030
#define sinNPC_MANUFACTURE		0x00000040
#define sinNPC_MIXTURE_RESET	0x00000080


struct MiniMapData;
extern DWORD	dwFuncChkTime;

DWORD Check_CodeSafe( DWORD dwStartFunc );
DWORD Check_CodeSafe2();
DWORD	GetSafeCode();
DWORD	GetSafeCode2();
int Code_VRamBuffOpen();
int	Mix_CodeVram();

extern char	szLastWhisperName[32];
extern int	LastWhisperLen;
extern int	PlayerArmor;
extern int	PlayUsed_Resurrection;


int SetPlayerArmor( int armor );
char *GetFaceHairSkin( smCHAR *lpChar );

//�κ��丮�� �ŷ�â���� ������ ã�� ���� ���
int GetInvenPotionCount( sCHK_POTION_COUNT *ChkPotion );
//���� ���� ���� ����
int	ResetPotion();
//���� ���� ���� ����
int	ResetPotion2();
//���హ�� ��
int	ComparePotion();
//���హ�� ��� ����
int	XorCheckPotion( sCHK_POTION_COUNT *ChkPotion );
///////////////////////////////////////////////////////////////////////

//��Ƽ�� �Ӹ� ������
int	WhisperPartyPlayer( char *szName );
//Ŭ���� ä�� ������
int	SetClanChatting();
//Ŭ�� ä�� ��� ����
int IsClanChatting();

void setexp();
//����ġ �߰�
int AddExp( INT64 Exp );
//���� ����ġ ���ϱ�
INT64 GetNextExp( int Level );
//������ ����ġ�� �´��� Ȯ��
int	CheckLevelExp( int Level , INT64 Exp );
//����ġ�� ���� �߻�
int	GetLevelFromExp( INT64 Exp );
//64��Ʈ ����ġ ����
int	SetExp64( smCHAR_INFO *smCharInfo , INT64 Exp64 );
//64��Ʈ ����ġ ��ȯ
INT64 GetExp64( smCHAR_INFO *smCharInfo );
//64��Ʈ ����ġ ��ȯ
INT64 GetXorExp64( smCHAR_INFO *smCharInfo );
//����ġ ����Ÿ�� ����
INT64 CodeXorCharInfo_Exp();

//�ʵ� ����� �������� Ȯ��
int CheckFieldContExp();

int funcLuckyBox(int BoxType, DWORD ItemCode, DWORD ItemHead, DWORD ItemChksum);
//��ȯ������ ���
int	ActionEtherCore( sITEM	*lpItem );

//�׾ ����ġ ����
int DeadPlayerExp( smCHAR *lpChar );
//�ʵ忡�� ����� ĳ���� ����ġ �߰� ����
int ContGameExp( smCHAR *lpChar );
//����� ���
int GetPlayDefence( smCHAR_INFO *lpAttackCharInfo , smCHAR_INFO *lpDefenceCharInfo );

//����
int DrawWeather( int ax );
int InitWeatherSystem();
//�ε��� ȭ�� ǥ��
int OpenDispLoading();
//�ε� ǥ������
int CloseDispLoading();
//���̳��� ���� ���� �߰�
int AddRemoveDynPattern( smDPAT *lpDinaPattern );

//�ε� ���� ����
int CloseLoadingLamp();
//�ε�����
int InitLoadingLamp( HWND hWnd );

//���ֻ���ϴ� �𵨵� �ε�
int LoadOftenModels();

//���־��� �޽� �ε�
int	LoadOftenMeshPattern( char *szFileName );
//���־��� ���� �̸� �ε�
int LoadOftenArmor();
//���־��� ������ �̸� �ε�
int LoadOftenItem();
//NPC �̸� �ε�
int LoadOftenNpc();
//������ �̸� �ε�
int LoadOftenFace();
//���־� �޽� �ؽ��� ����ýũ
int CheckOftenMeshTextureSwap();
//�� ��Ų ����
int	ChangeModelSkin( smCHAR *lpChar , int ComicArmor , int ComicHeadNum , char *szArmor , char *szHead );
//�Ӹ���� �ٲ������
int ChangeHairModel( int HairCode );

//�ʵ� �� �ʱ�ȭ
int InitFieldMap();

//�ʵ� �� �׸���
int MainFieldMap();
//�ʵ� �� �׸���
int DrawFieldMap();
int DrawFieldMap(MiniMapData* lpCompactMap, int px, int py, int size);

int GetFieldMapCursor();		//�̴ϸ� ��ġ�� ���콺 ����

int psSetDefaultRenderState();


//����Ÿ ���� ( Z/NZ ��� )
int EecodeCompress( BYTE *lpSrcData , BYTE *lpDestData , int size );
//���൥��Ÿ �ص� ( Z/NZ ��� )
int DecodeCompress( BYTE *lpSrcData , BYTE *lpDestData );
//���൥��Ÿ �ص� ( Z/NZ ��� )
int DecodeCompress( BYTE *lpSrcData , BYTE *lpDestData , int dCount );
//����Ÿ ���� ( Z/NZ ��� )
int EecodeCompress( BYTE *lpSrcData , BYTE *lpDestData , int size , int DestSize );



//�ٸ� ĳ���Ϳ��� ��ġ ��ġ���� Ȯ��
smCHAR *CheckOtherPlayPosi( int x, int y, int z );

//������ ���� �ޱ�
int ReformItem( sITEMINFO *lpItemInfo );
//������ ���� �ޱ� ( �����ð� ���� )
int ReformItem_AdjustTime( sITEMINFO *lpItemInfo , int TimeAdjust );

//������ ���� Ȯ��
int CheckItemForm( sITEMINFO *lpItemInfo );
//������ 2���� ���Ѵ�
int CompareItems( sITEMINFO *lpItemInfo1 , sITEMINFO *lpItemInfo2 );
//������ ���� ������ �ޱ�
int	rsReformItem_Server( sITEMINFO *lpItemInfo );

//ĳ���� ���� ���� Ȯ��
int CheckCharForm();
//ĳ���� ���� ���� �ޱ�
int ReformCharForm();
//ĳ���� ���� ���� �ޱ�
int ReformCharForm( smCHAR_INFO *lpCharInfo );
//ĳ���� ���� ���� Ȯ��
int CheckCharForm( smCHAR_INFO *lpCharInfo );
//Ʈ���̵� ���� ���� �ޱ�
int ReformTrade( void *lpTrade );
//Ʈ���̵��� ���� ���� Ȯ��
int CheckTrade( void *lpTrade );

//������ ����¡ ���� �ޱ�
int ReformMixItem( sITEMINFO *lpItemInfo );
//������ ����¡ ���� Ȯ��
int CheckMixItem( sITEMINFO *lpItemInfo );

//������ �����Ͽ� ���ϱ�
float GetItemAbsorb( sITEMINFO *lpItemInfo );

int psSetDefaultRenderState();

//ĳ���� ������ ĳ�õ���Ÿ�� ���� ��´�
smTRNAS_PLAYERINFO	*GetCharInfoCache( char *lpData );
//ĳ���� ������ ĳ�õ���Ÿ�� ����
int SaveCharInfoCache( smTRNAS_PLAYERINFO *lpTransPlayInfo );

extern smCHAR	chrPartyPlayer;
//��Ƽ �÷��̾� �� ����
int OpenPartyPlayer( char *szName , char *szModelFile1 , char *szModelFile2 );


//���� ���� ýũ
int	CheckPlayAttackCount();
//���� ���� ��� ī����
int AttackMotionCountUp();
//���� ���� ��Ŷ ī����
int	AttackRecvCountUp();
//��ϵ� �ҷ� ������ �˻�
int	CheckCopyItem( sITEMINFO *lpItem );
//���� ���� �� ��ȭ
int ChangeJobFace();


//���� ����Ʈ �ʱ�ȭ
int InitTimeEffect();
//����Ʈ ���� �߰�
int AddTimeEffect( DWORD EffectCode , DWORD dwTime , int x, int y, int z );
int AddTimeEffect( DWORD EffectCode , DWORD dwTime , int x, int y, int z , smCHAR *lpChar );
//���� ����Ʈ ����
int TimeEffectMain();


//ȸ�� �÷��̾� ������ ����
smCHAR *SelectRotPlayer( smCHAR *lpCurrentChar );
//ȸ�� �÷��̾� ������ ����
int	CloseRotPlayer();
//ȸ�� �÷��̾� ������ �ʱ�ȭ
smCHAR *InitRotPlayer();

//PLAYSUB ��Ƿ�ƾ ýũ
int Check_PlaySubReleaseFunc();

//�� �����Ѱ� ����
int SetJobFace( char *szModelName );
//���� �´��� Ȯ��
int	CheckUserModel( smCHAR_INFO *lpCharInfo );

//�̺��丮 ������ �ڵ� �ʱ�ȭ
int	ResetInvenItemCode();
//�̺��丮 ������ �ڵ忡�� Ư�� ������ ����
int SubInvenItemCode( sITEMINFO *lpItem );
//�κ��丮 ������ �ڵ� Ȯ��
int	CheckInvenItemCode();

//��� ýũ 2
void CheckPlaySubFunc2();



//������ �׷��� ýũ �ʱ�ȭ
int	ResetEnergyGraph( int Num );
//������ �׷��� ���� ýũ
int CheckEnergyGraphError();
//������ �׷����� ���� ýũ
int CheckEnergyGraph();

//�κ��丮�� ������ �������� ã�� �˻��Ͽ� ã�´�
sITEMINFO *FindInvenItem( DWORD dwItemCode , DWORD dwHead , DWORD dwChkSum );
//�κ��丮 ������ ���� ����
int SendInvenItemError( DWORD dwKind , DWORD dwItemCode , DWORD dwHead , DWORD dwChkSum );
//�κ��丮�� ���� ������ ���� ������ ������
int SendUsingItemListToServer();
//�κ��丮�� ���� ������ ���� ������ ������
int SendUsingItemListToServer2();


//�̺��丮 �� �˻� �ʱ�ȭ
int	ResetInvenMoney();
//�κ��丮 �˻�뵷 �߰�
int	AddInvenMoney( int Money );
//�κ��丮 �� �˻�
int	CheckInvenMoney();


//���ݼӵ��� �����Ӽӵ��� ��ȯ
int GetAttackSpeedMainFrame( int AttackSpeed );
int	GetAttackSpeedFrame( int AttackSpeed , int AddSpeed );
int	GetAttackSpeedFrame( int AttackSpeed );


extern int DispLoading;			//�ε� ���� ǥ�� ����
extern int MessageLoading;		//�ε��� �޼��� ó��

//��Ƽ���ھ� ���� �α� �ڵ�
#define POTION_ETHERCORE		0x40

extern int	CodeXorExp;					//����ġ �����ڵ�
extern int	CodeXorExp_High;			//����ġ �����ڵ�
extern int	CodeXorLife;				//������ �����ڵ�
extern int	CodeXorMana;				//��� �����ڵ�
extern int	CodeXorStamina;				//�ٷ� �����ڵ�
extern int	CodeXorMoney;				//�� �����ڵ�


//�������� �������� ���¸� �Ǻ��Ѵ�
int GetItemKindFromBliankColor( smCHARTOOL	*lpCharTool );


extern char	szSOD_String[64];
extern BYTE	bSOD_StringColor[4];
extern int	SOD_StringCount;

extern sSOD_SCORE	sSodScore;
extern int		SoDGateFlag;

int	DisplaySodScore();	//Sod���� ǥ�� �Լ�
int	SodScoreMain();		//SoD ����
int	Sod_SodEffect( int Round );			//SoD ���� ����Ʈ


