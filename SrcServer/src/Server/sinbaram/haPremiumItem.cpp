#include "sinLinkHeader.h"
#include "field.h"
#include "AreaServer.h"
#include "Character/playmain.h"
#include "Character/playsub.h"
#include "netplay.h"
#include "Character/character.h"

#include <string>

using namespace std;

cHaPremiumItem  chaPremiumitem;


#define  TELEPORT_FIELDMAX 64
int TelePort_FieldNum[TELEPORT_FIELDMAX][3] = {
	// EXPLICANDO A PRIMEIRA LINHA, { 0, 20, 0 }, 0 = Posi��o na lista do Core M�gico, 20 = Field 20, 0 = Level para entrar.
	{ 0, 20, 0 }, // Estrada dos Ventos
	{ 1, 19, 0 }, // Vale Tranquilo
	{ 2, 17, 0 }, // Floresta dos Esp�ritos
	{ 3, 0, 0 }, // Mata das Ac�cias
	{ 4, 2, 0 }, // Jardim da Liberdade
	{ 5, 4, 0 }, // Refugio dos Ancioes
	{ 6, 5, 0 }, // Castelo dos Perdidos
	{ 7, 7, 0 },  // Terra Maldita
	{ 8, 8, 0 }, // Terra Esquecida
	{ 9, 10, 0 }, // Oasis
	{ 10, 11, 55 }, // Campo de Batalha
	{ 11, 12, 55 }, // Terra Proibida
	{ 12, 25, 55 }, // Caverna das Abelhas
	{ 13, 24, 55 }, // Caverna dos Cogumelos
	{ 14, 26, 65 }, // Santu�rio Sombrio 
	{ 15, 13, 40 }, // Calabou�o Antigo 1� Andar 
	{ 16, 14, 50 }, // Calabou�o Antigo 2� Andar
	{ 17, 15, 60 }, // Calabou�o Antigo 3� Andar
	{ 18, 22, 70 }, // Templo Maldito 1� Andar 
	{ 19, 23, 75 }, // Templo Maldito 2� Andar 
	{ 20, 42, 80 }, // Templo Maldito 3� Andar
	{ 21, 34, 70 }, // Lago da Gan�ncia
	{ 22, 27, 80 }, // Estrada de Ferro do Caos
	{ 23, 28, 90 }, // Cora��o de Perum
	{ 24, 29, 70 }, // Eura
	{ 25, 31, 70 }, // Vale Gallubia
	{ 26, 35, 90 }, // Santu�rio Congelado
	{ 27, 36,90},   // Covil de Kelvezu 
	{ 28, 37, 95 }, // Ilha Perdida
	{ 29, 38, 100 }, // Templo Perdido
	{ 30, 40, 102 }, // Torre Sem Fim 1o Andar
	{ 31, 41, 105 }, // Torre Sem Fim 2� Andar 
	{ 32, 43, 108 }, // Torre Sem Fim 3� Andar
	{ 33, 44, 110 }, // Mina de Gelo 1� Andar 
	{ 34, 46, 118 }, // Laborat�rio Secreto
	/*
	{ 35, 47, 120 }, // Arma Secreta
	{ 36, 48, 130 }, // Abismo do Mar 1� Andar
	{ 37, 49, 135 }, // Abismo do Mar 2� andar
	{ 38, 51, 132 }, // Ilha da Morte 3
	{ 39, 52, 132 }, // Lar do Eragon
	{ 42, 53, 132 }, // Floresta Misteriosa 3�
	{ 41, 54, 132 }, // Floresta Misteriosa 2�
	{ 40, 55, 132 }, // Floresta Misteriosa 1�
	{ 43, 56, 132 }, // Atlantis
	{ 44, 57, 132 }, // Calabou�o Perdido 1�
	{ 45, 58, 132 }, // Calabou�o Perdido 2�
	{ 46, 59, 132 }, // Calabou�o Perdido 3�
	{ 47, 60, 132 }, // Templo Esquecido da Morte 1�
	{ 48, 61, 132 }, // Templo Esquecido da Morte 2�
	{ 49, 62, 132 }, // Arena de Batalha
	{ 50, 63, 132 }, // Nucleo de Ferro
	{ 51, 64, 132 }  // Cora��o de Fogo
	*/
};

char *haTeleport_Field[] = {
	"image\\Sinimage\\help\\haTeleport_Field[1].sin",
	"image\\Sinimage\\help\\haTeleport_Field[2].sin",
	"image\\Sinimage\\help\\haTeleport_Field[3].sin",
	"image\\Sinimage\\help\\haTeleport_Field[4].sin",
};

cHaPremiumItem::cHaPremiumItem()
{
	TelePortCore.PageButtonFlag[0] = 0;
	TelePortCore.PageButtonFlag[1] = 0;
	TelePortCore.FieldIndex[0]     = 0;
	TelePortCore.FieldIndex[1]     = 0;
	TelePortCore.PageIndex         = 0;
	Using_ItemKind                 = 0;
	Using_SelectInvenItemIndex	   = 0;
	int i;
	for( i = 0 ; i < SINUPKEEPITEM_MAX ; i++ ){
		UpKeepItem[i].PosiX   = 0;
		UpKeepItem[i].PosiY   = 0;
		UpKeepItem[i].Visible = false;
		UpKeepItem[i].IconTime[0] = 0;   
		UpKeepItem[i].IconTime[1] = 0;
		UpKeepItem[i].Alpha       = 255;
		UpKeepItem[i].Alphakind   = 0;
		UpKeepItem[i].Per		  = 0;
		memset(UpKeepItem[i].Id,NULL,sizeof(UpKeepItem[i].Id));
	}

	UseClickPremiumItemFlag = false;

	//��� ���� �ʱ�ȭ
	m_ThirdEyesTime   = 0;
	m_ExpUpPotionTime = 0;
	m_VampiricCuspidTime  = 0;
	m_ManaRechargingPTime = 0;
	m_ManaReducePotiontime = 0; // pluto ���� ���ེ ����
	m_ManaReducePotionValue = 0;		// pluto ���� ���ེ ���� ���� %��
	m_MightofAwellTime = 0;		// pluto ����Ʈ ���� ����
	m_MightofAwellWeight = 0;	// pluto ����Ʈ ���� ���� ������
	m_PhenixPetTime = 0;		// pluto ��(�ؿ�)
	m_HelpPetTime = 0; // ����� - ���� ����� �� �߰�
	m_VampiricCuspidEXTime  = 0;		// �庰 - ���Ǹ� Ŀ���� EX
	m_StaminaReducePotiontime = 0;	// ����� - �ٷ� ���ེ ����
	m_StaminaReducePotionValue = 0; // ����� - �ٷ� ���ེ ���� ���� %��
	
}
/*----------------------------------------------------------------------------*
*					  �Ҹ���		
*-----------------------------------------------------------------------------*/
cHaPremiumItem::~cHaPremiumItem()
{
	//int i;
	//for( i = 0 ; i < SINUPKEEPITEM_MAX ; i++ ){
	//	if(UpKeepItem[i].lpImage){
	//		UpKeepItem[i].lpImage->Release();
	//		UpKeepItem[i].lpImage = 0;
	//	}
	//}
}

/*----------------------------------------------------------------------------*
*					  init		
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::init() {}
/*----------------------------------------------------------------------------*
*					  Main		
*-----------------------------------------------------------------------------*/
//���ʿ� ���� ������ ��ǥ��
int ItemUseLeftPosi[SINUPKEEPITEM_MAX][2]={
	{8,80},{8,116},{8,152},{8,188},{8,224},{8,260},{8,296}, {8, 332} 
};		
DWORD dwDelayTime = 0; 
void cHaPremiumItem::Main()
{
	//���� ũ����Ż ������ ���� �����̸� �ش�,
	if( chaPremiumitem.haItemUseDelayFlag == true){
		dwDelayTime++;
		if( dwDelayTime > 210){  //3�ʷ� �����Ѵ�.{
			chaPremiumitem.haItemUseDelayFlag = false;
			dwDelayTime = 0;
		}
	}


	//���������� ���Ŵ� �������� ��ġ����
	int i,cnt=0;	
	UseClickPremiumItemFlag = false;
	for( i = 0 ; i < SINUPKEEPITEM_MAX ; i++ ){
		////�ð� üũ
		if(UpKeepItem[i].IconTime[0] < UpKeepItem[i].IconTime[1]){
			if(UpKeepItem[i].Visible == true ){
				if(UpKeepItem[i].IconTime[1]-UpKeepItem[i].IconTime[0] <= 0 ){
					UpKeepItem[i].Visible = false;
					CheckUpKeepItem();  //����� �ѹ��� ȣ�����ش�.
				}
				cnt++;
				//���� �ð��� �޾� �ͼ� ����ش�.
				DWORD NowTime = GetPlayTime_T();			
				switch(UpKeepItem[i].TGAImageNumber){
					case nsPremiumItem::THIRD_EYES:   //��� ������
						if( m_ThirdEyesTime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}
					break;
					case nsPremiumItem::EXPUP_POTION:  //����ġ ���� ����
						if( m_ExpUpPotionTime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}
					break;
					case nsPremiumItem::VAMPIRIC_CUS:  //���Ǹ� Ŀ����
						if( m_VampiricCuspidTime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}						
					break;

					
					case nsPremiumItem::VAMPIRIC_CUS_EX:  // �庰 - ���Ǹ� Ŀ���� EX
						if( m_VampiricCuspidEXTime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}						
					break;
					

					case nsPremiumItem::MANA_RECHAR_P:  //���� ����¡  ����
						if( m_ManaRechargingPTime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}						
					break;
					case nsPremiumItem::MIGHT_OF_AWELL:	// pluto ����Ʈ ���� ����
						if( m_MightofAwellTime > 0 )
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::MANA_REDUCE_P:	// pluto ���� ���ེ ����
						if( m_ManaReducePotiontime > 0 )
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::PHENIX_PET:	// pluto ��(�ؿ�)
						if( m_PhenixPetTime > 0 )
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::HELP_PET_TERRY: // ����� - ���� ����� �� �߰�
						if(m_HelpPetTime > 0 ) 
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::HELP_PET_NEPSYS: // ����� - ���� ����� �� �߰�
						if(m_HelpPetTime > 0 ) 
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::HELP_PET_IO: // ����� - ���� ����� �� �߰�
						if(m_HelpPetTime > 0 ) 
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::HELP_PET_MUTE: // ����� - ���� ����� �� �߰�
						if(m_HelpPetTime > 0 ) 
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::STAMINA_REDUCE_P:  // ����� - �ٷ� ���ེ ���� 
						if( m_StaminaReducePotiontime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}						
						break;
				}
			}
		}
		else
			UpKeepItem[i].Visible = false;
		if(UpKeepItem[i].Visible == true){
			//��ġ ����
			if(UpKeepItem[i].PosiY > ItemUseLeftPosi[cnt-1][1])
				UpKeepItem[i].PosiY-=1;
			else{
				UpKeepItem[i].PosiY = ItemUseLeftPosi[cnt-1][1];
				UpKeepItem[i].PosiX = ItemUseLeftPosi[cnt-1][0];
			}
			//���콺 ��ġ üũ

			UpKeepItem[i].Infoflag = false;
			UseClickPremiumItemFlag = false;
			
		}
	}
}
/*----------------------------------------------------------------------------*
*					  Draw		
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::Draw() {}

DWORD dwKeepItemTime[7] = { 0, SINITEM_TIME_1HOURS, SINITEM_TIME_3HOURS, SINITEM_TIME_ONEDAY, SINITEM_TIME_SEVENDAY, SINITEM_TIME_30DAY, SINITEM_TIME_3DAY };		//�ð����̺�

int  cHaPremiumItem::CheckTime(DWORD CurrentTime)
{

	for (int i = 0; i < 8; i++) {
		if (dwKeepItemTime[i] < CurrentTime && CurrentTime < dwKeepItemTime[i + 1]) {
			return i + 1;
		}
	}

	return false;
}
/*----------------------------------------------------------------------------*
*					  ��ų ������ ������ Draw		
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::DrawUp() {}
/*----------------------------------------------------------------------------*
*					  LButtonDown		
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::LButtonDown(int x, int y) {}
/*----------------------------------------------------------------------------*
*					  LButtonUp		
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::LButtonUp(int x,int y)
{

}
DWORD dwPremiumItemCode[]={(sinBI1|sin01),(sinBI1|sin02),(sinBI1|sin03),(sinBI1|sin04),(sinBI1|sin05),(sinBI1|sin06),(sinBI1|sin07)};
/*----------------------------------------------------------------------------*
*Desc: �����̾� ������ ����  ȣ��		
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::UsePremiumItem(int kind) { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: ��ų �������� �����Ŵ�  �������ϰ�� ȣ��	
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::SetPremiumItemSkill(int kind) { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: �ڷ���Ʈ �ھ� ���� ȣ��						
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::UseTeleportScrollItem(short ItemIndex) { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: �ٸ� �������̽�â�� ��������� �����Ѵ�.		
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::CheckWindowOpen() { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: ��迭�� ����ش�.		
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::CheckUpKeepItem()
{
	int i;
	//������üũ
	for( i = 0 ; i < SINUPKEEPITEM_MAX ; i++ ){
		if(UpKeepItem[i].Visible == false){
			if(UpKeepItem[i+1].Visible == true){
				memcpy(&UpKeepItem[i],&UpKeepItem[i+1],sizeof(sUpKeepItem));
				UpKeepItem[i+1].Visible = false;
				break;
			}			
		}
	}
	return true;
}
/*----------------------------------------------------------------------------*
*Desc: ������ �Ҹ� �������� ����� ��� ���÷���  	
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::SetUpKeepItem(int kind, DWORD dwTime, bool visible, char* id, int Percent, short ItemType) { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: ������������ �����ڽ� 		
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::InfoBox(int PoisX, int PosiY, int Width, int Height) {}
/*----------------------------------------------------------------------------*
*Desc: ������������ ����(�̸�/�����ð� ) 		
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::DrawText() { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: ���ƾƮ���� ��� 		
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::UseHairtintPotion(int ItemKind)
{
	switch(ItemKind){
		case 1:  //A��
			if( ChangeHairModel(0) )
				return TRUE;
		break;
		case 2:  //B��
			if( ChangeHairModel(1) )
				return TRUE;
		break;
		case 3:  //C��
			if( ChangeHairModel(2) )
				return TRUE;
		break;
		case 4:  //D��
			if( ChangeHairModel(3) )
				return TRUE;
		break;
		case 5:  //E��
			if( ChangeHairModel(4) )
				return TRUE;
		break;
		case 6:  //F��
			if( ChangeHairModel(5) )
				return TRUE;
		break;
		case 7:  //G��
			if( ChangeHairModel(6) )
				return TRUE;
		break;
		case 8:  //H��
			if( ChangeHairModel(7) )
				return TRUE;
		break;
			
		case 9:  //I��
			if( ChangeHairModel(8) )
				return TRUE;
		break;
		case 10:  //J��
			if( ChangeHairModel(9) )
				return TRUE;
		break;
		case 11:  //K��
			if( ChangeHairModel(10) )
				return TRUE;
		break;
		case 12:  //L��
			if( ChangeHairModel(11) )
				return TRUE;
		break;
		case 13:  //M��
			if( ChangeHairModel(12) )
				return TRUE;
		break;
		case 14: //novos cabelos xxstr
			if (ChangeHairModel(14))
				return TRUE;
		break;
		case 15:
			if (ChangeHairModel(18))
				return TRUE;
		break;
		case 16:
			if (ChangeHairModel(19))
				return TRUE;
		break;
		case 17:
			if (ChangeHairModel(20))
				return TRUE;
		break;
		case 18:
			if (ChangeHairModel(21))
				return TRUE;
		break;
		case 19:
			if (ChangeHairModel(22))
				return TRUE;
		break;
		case 20:
			if (ChangeHairModel(23))
				return TRUE;
		break;
		case 21:
			if (ChangeHairModel(24))
				return TRUE;
		break;
		

	}	

	return FALSE; 
}

// ����� - ����¡ ������ ������ ���
int cHaPremiumItem::UseAgingMaster(int ItemKind)
{
	switch(ItemKind)
	{
	case 0:
		sinCheckAgingLevel(SIN_AGING_ATTACK, true);
		sinCheckAgingLevel(SIN_AGING_CRITICAL, true);
		ReformCharForm(); //������ 
		break;
	case 1:
		if((cInvenTory.InvenItem[sInven[1].ItemIndex-1].CODE &sinITEM_MASK2) == sinDS1)
		{
			sinCheckAgingLevel(SIN_AGING_BLOCK, true);
			ReformCharForm(); //������ 
		}
		else if((cInvenTory.InvenItem[sInven[1].ItemIndex-1].CODE &sinITEM_MASK2) == sinOM1)
		{
			sinCheckAgingLevel(SIN_AGING_DEFENSE_ORB, true);
			ReformCharForm(); //������ 

		}
		break;
	case 2:
		sinCheckAgingLevel(SIN_AGING_DEFENSE_ARMOR, true);
		ReformCharForm(); //������ 

		break;
	case 3: // ����� - ����¡ ������(2��) - �ϸ�
		sinCheckAgingLevel(SIN_AGING_DEFENSE_ARMLETS, true);
		ReformCharForm(); //������ 

		break;
	case 4: // ����� - ����¡ ������(2��) - �尩
		sinCheckAgingLevel(SIN_AGING_DEFENSE_GLOVES, true);
		ReformCharForm(); //������ 

		break;
	case 5: // ����� - ����¡ ������(2��) - ����
		sinCheckAgingLevel(SIN_AGING_DEFENSE_BOOTS, true);
		ReformCharForm(); //������ 
		break;
	}

	return TRUE;
}

// ����� - ��ų ������ ������ ���
int cHaPremiumItem::UseSkillMaster(int ItemKind)
{
	switch(ItemKind)
	{
	case 1:
		for(int j=1 ; j<5 ; j++) // 1�� ������ ����� ��ų
		{
			if(sinSkill.UseSkill[j].UseSkillCount<10000)
			{
				if(sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point)
				{
					sinSkill.UseSkill[j].UseSkillCount += 10000;
					cSkill.ReformSkillMasteryForm(0,j); //����Ÿ�� �����Ѵ�
				}
			}
		}
		break;
	case 2:
		for(int j=5 ; j<9 ; j++) // 2�� ������ ����� ��ų
		{
			if(sinSkill.UseSkill[j].UseSkillCount<10000)
			{
				if(sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point)
				{
					sinSkill.UseSkill[j].UseSkillCount += 10000;
					cSkill.ReformSkillMasteryForm(0,j); //����Ÿ�� �����Ѵ�
				}
			}
		}
		break;
	case 3:
		for(int j=9 ; j<13 ; j++) // 3�� ������ ����� ��ų
		{
			if(sinSkill.UseSkill[j].UseSkillCount<10000)
			{
				if(sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point)
				{
					sinSkill.UseSkill[j].UseSkillCount += 10000;
					cSkill.ReformSkillMasteryForm(0,j); //����Ÿ�� �����Ѵ�
				}
			}
		}
		break;
	}

	return TRUE;
}

// ����� - �̵� ���� ������
int cHaPremiumItem::UseMoveShop()
{
	return TRUE;
}
