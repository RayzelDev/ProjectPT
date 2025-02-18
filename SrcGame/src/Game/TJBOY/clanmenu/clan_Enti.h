#pragma once
#ifndef __CLAN_ENTI_HPP__
#define __CLAN_ENTI_HPP__

//#define CLANHELPICON_NUM_ENIT		100

class CLANEntity
{
public:
	int				life;	
	CLANEntity      *parent;


public:
	CLANEntity();
	~CLANEntity();

	virtual int Init() ;
	virtual int Clear();

	//***************************************************************************************
	virtual void Main();
	virtual void RenderMain();

	//***************************************************************************************
	void	setParent(CLANEntity      *p){parent = p;}
	void			Kill()			{ life = 0; };
	int				IsDead()		{ return(life); };

	//virtual void main();
	virtual void Draw();
	//***************************************************************************************
	virtual void menuInit(int num);

	//***************************************************************************************
	void Draw_C_Text(char *str, int x, int y, int sw=0);
	void Draw_C_Text2(char *str, int x, int y, int sw=0); //클릭을 당했을때 붉은색(red)글자
	void Draw_C_Text3(char *str, int x, int y, int sw=0);  //채팅리스트에서 디폴트 녹색(green)글자

//박철호
//#ifdef PARKMENU
	void Draw_C_TextPark(char *str, int x, int y, int RedColor, int GreenColor, int BlueColor, int sw);

	


	//***************************************************************************************
	//int  menuSu;
	//RECT menuPos[30];		//메뉴셋팅.
	int  chkMenu(int x, int y, RECT *r);
	int  chkeckMenuN();



	//data 파싱 ========================================
	void ParsingIsClanMember(char* data);
	void ParsingIsClanMember2(char *data);
	int chk_readStrBuf(char *cmd, char *readbuf, char *retBuf);
	void ParsingData(char* cmd, char* data, char* save);
	void ParsingMakeClanMoney(char* data,int* money);


	void ParsigClanWon(char* data);
	
	void ParsingClanWonStrLen();

	void LoadToFile();
	

	//도움말 관련 함수들================
	void LoadHelp(char *help);
	void DestroyClanHelpIcon();
	void LoadHelpImage();
	BOOL ReadClanHelpScr(char *filename);
	void ReMoveClanWon(char *src);
	//void Updatecldata();
	void WriteMyClanInfo();
	int CheckMark(BOOL bFlag);

	void CheckClanExp(char* byte); //클랜 설명 버퍼 조절



	

	//아래 리소스 로드 클로즈 함수들
	void Load();
	void Load2();

	virtual void Close();
	void copy_LoadResource(CLANEntity *copyDest);
	void copy_LoadResource2(CLANEntity *copyDest);
	//마크 넘겨주기
	inline void SetBase(CLANEntity *copyDest){parent = copyDest;}

	#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
	#else
	inline HBITMAP GetClanMark(){return parent->hMark;}
	#endif

	//PartyInterface 리소스..
	#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
	
	int	MatPartyBackGround;		//배경
	int	hBoxCorner,hBoxLine;	//박스그리기
	int	hBox1,hBox2,hBox3,hBox4,hBox6,hBox7,hBox8,hBox9; //박스그리기
	int	hScl_Bar_tj;
	int	hScl_icon_tj;
	int   hLine;
	

	inline LPDIRECT3DTEXTURE9 GetClanMark(){return parent->hMark;}


	LPDIRECT3DTEXTURE9	hBt_Help;
	LPDIRECT3DTEXTURE9	hMark_box;
	LPDIRECT3DTEXTURE9	hOk_Gray,hOk_Yellow; //확인
	LPDIRECT3DTEXTURE9	hCancel_Gray,hCancel_Yellow; //취소
	LPDIRECT3DTEXTURE9	hYes_Gray,hYes_Yellow; // 예
	LPDIRECT3DTEXTURE9	hNo_Gray,hNo_Yellow; //아니오

	LPDIRECT3DTEXTURE9	hMark,hClanName;
	LPDIRECT3DTEXTURE9	hSecession[2];
	LPDIRECT3DTEXTURE9	hClose[2];
	LPDIRECT3DTEXTURE9	hIcon;
	LPDIRECT3DTEXTURE9	hIcon_Click;
	LPDIRECT3DTEXTURE9	hHelp1;
	LPDIRECT3DTEXTURE9	hClinetMark;
	LPDIRECT3DTEXTURE9	hServerMark;
	LPDIRECT3DTEXTURE9	hWareHouse[2]; //마우스가 위로 올라왔을때
	LPDIRECT3DTEXTURE9	hWHRight[2];
	LPDIRECT3DTEXTURE9	hClanMark[2];
	LPDIRECT3DTEXTURE9	hRemove[2];
	LPDIRECT3DTEXTURE9	hClanRelease[2];
	LPDIRECT3DTEXTURE9	hHelp[2];
	LPDIRECT3DTEXTURE9	hOut[2];

	LPDIRECT3DTEXTURE9	hButton_Box;
	LPDIRECT3DTEXTURE9	hClanTitle;
	LPDIRECT3DTEXTURE9	hRemoveToMenu[2];
	LPDIRECT3DTEXTURE9	hScl_Bar;
	LPDIRECT3DTEXTURE9	hScl_icon;
	LPDIRECT3DTEXTURE9	hClanHelpIcon[CLANHELPICON_NUM][4];
	LPDIRECT3DTEXTURE9    hClanNameInput;

	LPDIRECT3DTEXTURE9   hTxt_Remove[2]; //추방
	LPDIRECT3DTEXTURE9   hTxt_Release[2]; //해체
	LPDIRECT3DTEXTURE9   hTxt_Mark[2]; //마크
	LPDIRECT3DTEXTURE9   hTxt_Help[2]; //도움말
	LPDIRECT3DTEXTURE9   hTxt_Secession[2];//탈퇴
	LPDIRECT3DTEXTURE9   hTxt_LeaveClan[2];//이임
	LPDIRECT3DTEXTURE9   hTxt_SubChip[2];//부클랜칩
	LPDIRECT3DTEXTURE9   hSubChip;

	

	
	char		szHelpMsg1[HELPMSG_MAX];
	char		szErrorMsg[256];

	#else

	HBITMAP	hLine;

	HBITMAP	hBox1,hBox2,hBox3,hBox4,hBox6,hBox7,hBox8,hBox9;
	HBITMAP	MatPartyBackGround;
	HBITMAP	hMark_box;
	HBITMAP	hBt_Help;
	
	HBITMAP	hOk_Gray,hOk_Yellow; //확인
	HBITMAP	hCancel_Gray,hCancel_Yellow; //취소
	HBITMAP	hYes_Gray,hYes_Yellow; // 예
	HBITMAP	hNo_Gray,hNo_Yellow; //아니오

	HBITMAP	hMark,hClanName;
	HBITMAP	hSecession[2];
	HBITMAP	hClose[2];
	HBITMAP	hIcon;
	HBITMAP   hIcon_Click;
	HBITMAP	hHelp1;
	HBITMAP	hClinetMark;
	HBITMAP	hServerMark;
	HBITMAP	hWareHouse[2]; //마우스가 위로 올라왔을때
	HBITMAP	hWHRight[2];
	HBITMAP	hClanMark[2];
	HBITMAP	hRemove[2];
	HBITMAP	hClanRelease[2];
	HBITMAP	hHelp[2];
	HBITMAP	hOut[2];
	HBITMAP	hButton_Box;
	HBITMAP	hClanTitle;
	HBITMAP	hRemoveToMenu[2];
	HBITMAP	hScl_Bar;
	HBITMAP	hScl_icon;
	HBITMAP   hClanHelpIcon[CLANHELPICON_NUM][4]; //이차원 배열로 처리

	HBITMAP   hTxt_Remove[2]; //추방
	HBITMAP   hTxt_Release[2]; //해체
	HBITMAP   hTxt_Mark[2]; //마크
	HBITMAP   hTxt_Help[2]; //도움말
	HBITMAP   hTxt_Secession[2]; //탈퇴
	HBITMAP   hTxt_LeaveClan[2];//이임
	HBITMAP   hTxt_SubChip[2];//부클랜칩
	HBITMAP   hSubChip;

	HBITMAP    hClanNameInput;

	HWND		m_hClanName;	
	char		szErrorMsg[256];

	#endif
	BOOL bImageDown;
};

#endif //__CLAN_ENTI_HPP__





#if defined (CLAN_EITI_CPP)


	class CLANEntity  cBaseMenu;

#else

	extern class CLANEntity  cBaseMenu;


#endif




