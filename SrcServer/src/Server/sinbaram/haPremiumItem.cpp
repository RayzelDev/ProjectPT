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
	// EXPLICANDO A PRIMEIRA LINHA, { 0, 20, 0 }, 0 = Posição na lista do Core Mágico, 20 = Field 20, 0 = Level para entrar.
	{ 0, 20, 0 }, // Estrada dos Ventos
	{ 1, 19, 0 }, // Vale Tranquilo
	{ 2, 17, 0 }, // Floresta dos Espíritos
	{ 3, 0, 0 }, // Mata das Acácias
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
	{ 14, 26, 65 }, // Santuário Sombrio 
	{ 15, 13, 40 }, // Calabouço Antigo 1º Andar 
	{ 16, 14, 50 }, // Calabouço Antigo 2º Andar
	{ 17, 15, 60 }, // Calabouço Antigo 3º Andar
	{ 18, 22, 70 }, // Templo Maldito 1º Andar 
	{ 19, 23, 75 }, // Templo Maldito 2º Andar 
	{ 20, 42, 80 }, // Templo Maldito 3º Andar
	{ 21, 34, 70 }, // Lago da Ganância
	{ 22, 27, 80 }, // Estrada de Ferro do Caos
	{ 23, 28, 90 }, // Coração de Perum
	{ 24, 29, 70 }, // Eura
	{ 25, 31, 70 }, // Vale Gallubia
	{ 26, 35, 90 }, // Santuário Congelado
	{ 27, 36,90},   // Covil de Kelvezu 
	{ 28, 37, 95 }, // Ilha Perdida
	{ 29, 38, 100 }, // Templo Perdido
	{ 30, 40, 102 }, // Torre Sem Fim 1o Andar
	{ 31, 41, 105 }, // Torre Sem Fim 2º Andar 
	{ 32, 43, 108 }, // Torre Sem Fim 3º Andar
	{ 33, 44, 110 }, // Mina de Gelo 1º Andar 
	{ 34, 46, 118 }, // Laboratório Secreto
	/*
	{ 35, 47, 120 }, // Arma Secreta
	{ 36, 48, 130 }, // Abismo do Mar 1º Andar
	{ 37, 49, 135 }, // Abismo do Mar 2º andar
	{ 38, 51, 132 }, // Ilha da Morte 3
	{ 39, 52, 132 }, // Lar do Eragon
	{ 42, 53, 132 }, // Floresta Misteriosa 3º
	{ 41, 54, 132 }, // Floresta Misteriosa 2º
	{ 40, 55, 132 }, // Floresta Misteriosa 1º
	{ 43, 56, 132 }, // Atlantis
	{ 44, 57, 132 }, // Calabouço Perdido 1º
	{ 45, 58, 132 }, // Calabouço Perdido 2º
	{ 46, 59, 132 }, // Calabouço Perdido 3º
	{ 47, 60, 132 }, // Templo Esquecido da Morte 1º
	{ 48, 61, 132 }, // Templo Esquecido da Morte 2º
	{ 49, 62, 132 }, // Arena de Batalha
	{ 50, 63, 132 }, // Nucleo de Ferro
	{ 51, 64, 132 }  // Coração de Fogo
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

	//¸â¹ö º¯¼ö ÃÊ±âÈ­
	m_ThirdEyesTime   = 0;
	m_ExpUpPotionTime = 0;
	m_VampiricCuspidTime  = 0;
	m_ManaRechargingPTime = 0;
	m_ManaReducePotiontime = 0; // pluto ¸¶³ª ¸®µà½º Æ÷¼Ç
	m_ManaReducePotionValue = 0;		// pluto ¸¶³ª ¸®µà½º Æ÷¼Ç °¨¼Ò %°ª
	m_MightofAwellTime = 0;		// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£
	m_MightofAwellWeight = 0;	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£ ¼ÒÁö·®
	m_PhenixPetTime = 0;		// pluto Æê(ÇØ¿Ü)
	m_HelpPetTime = 0; // ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
	m_VampiricCuspidEXTime  = 0;		// Àåº° - ¹ìÇÇ¸¯ Ä¿½ºÇÍ EX
	m_StaminaReducePotiontime = 0;	// ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç
	m_StaminaReducePotionValue = 0; // ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç °¨¼Ò %°ª
	
}
/*----------------------------------------------------------------------------*
*					  ¼Ò¸êÀÚ		
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
//¿ÞÂÊ¿¡ ÂïÀÏ Æ÷Áö¼Ç ÁÂÇ¥°ª
int ItemUseLeftPosi[SINUPKEEPITEM_MAX][2]={
	{8,80},{8,116},{8,152},{8,188},{8,224},{8,260},{8,296}, {8, 332} 
};		
DWORD dwDelayTime = 0; 
void cHaPremiumItem::Main()
{
	//¸ó½ºÅÍ Å©¸®½ºÅ» ¾ÆÀÌÅÛ »ç¿ë½Ã µô·¹ÀÌ¸¦ ÁØ´Ù,
	if( chaPremiumitem.haItemUseDelayFlag == true){
		dwDelayTime++;
		if( dwDelayTime > 210){  //3ÃÊ·Î Á¦ÇÑÇÑ´Ù.{
			chaPremiumitem.haItemUseDelayFlag = false;
			dwDelayTime = 0;
		}
	}


	//À¯ÁöÇüÀ¸·Î »ç¿ëµÅ´Â ¾ÆÀÌÄÜÀÇ À§Ä¡Á¤º¸
	int i,cnt=0;	
	UseClickPremiumItemFlag = false;
	for( i = 0 ; i < SINUPKEEPITEM_MAX ; i++ ){
		////½Ã°£ Ã¼Å©
		if(UpKeepItem[i].IconTime[0] < UpKeepItem[i].IconTime[1]){
			if(UpKeepItem[i].Visible == true ){
				if(UpKeepItem[i].IconTime[1]-UpKeepItem[i].IconTime[0] <= 0 ){
					UpKeepItem[i].Visible = false;
					CheckUpKeepItem();  //Á¾·á½Ã ÇÑ¹ø¾¿ È£ÃâÇØÁØ´Ù.
				}
				cnt++;
				//¼­¹ö ½Ã°£À» ¹Þ¾Æ ¿Í¼­ Âï¾îÁØ´Ù.
				DWORD NowTime = GetPlayTime_T();			
				switch(UpKeepItem[i].TGAImageNumber){
					case nsPremiumItem::THIRD_EYES:   //½áµå ¾ÆÀÌÁî
						if( m_ThirdEyesTime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}
					break;
					case nsPremiumItem::EXPUP_POTION:  //°æÇèÄ¡ Áõ°¡ Æ÷¼Ç
						if( m_ExpUpPotionTime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}
					break;
					case nsPremiumItem::VAMPIRIC_CUS:  //¹ìÇÇ¸¯ Ä¿½ºÇÍ
						if( m_VampiricCuspidTime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}						
					break;

					
					case nsPremiumItem::VAMPIRIC_CUS_EX:  // Àåº° - ¹ìÇÇ¸¯ Ä¿½ºÇÍ EX
						if( m_VampiricCuspidEXTime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}						
					break;
					

					case nsPremiumItem::MANA_RECHAR_P:  //¸¶³ª ¸®Â÷Â¡  Æ÷¼Ç
						if( m_ManaRechargingPTime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}						
					break;
					case nsPremiumItem::MIGHT_OF_AWELL:	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£
						if( m_MightofAwellTime > 0 )
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::MANA_REDUCE_P:	// pluto ¸¶³ª ¸®µà½º Æ÷¼Ç
						if( m_ManaReducePotiontime > 0 )
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::PHENIX_PET:	// pluto Æê(ÇØ¿Ü)
						if( m_PhenixPetTime > 0 )
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::HELP_PET_TERRY: // ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
						if(m_HelpPetTime > 0 ) 
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::HELP_PET_NEPSYS: // ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
						if(m_HelpPetTime > 0 ) 
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::HELP_PET_IO: // ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
						if(m_HelpPetTime > 0 ) 
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::HELP_PET_MUTE: // ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
						if(m_HelpPetTime > 0 ) 
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::STAMINA_REDUCE_P:  // ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç 
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
			//À§Ä¡ º¯°æ
			if(UpKeepItem[i].PosiY > ItemUseLeftPosi[cnt-1][1])
				UpKeepItem[i].PosiY-=1;
			else{
				UpKeepItem[i].PosiY = ItemUseLeftPosi[cnt-1][1];
				UpKeepItem[i].PosiX = ItemUseLeftPosi[cnt-1][0];
			}
			//¸¶¿ì½º À§Ä¡ Ã¼Å©

			UpKeepItem[i].Infoflag = false;
			UseClickPremiumItemFlag = false;
			
		}
	}
}
/*----------------------------------------------------------------------------*
*					  Draw		
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::Draw() {}

DWORD dwKeepItemTime[7] = { 0, SINITEM_TIME_1HOURS, SINITEM_TIME_3HOURS, SINITEM_TIME_ONEDAY, SINITEM_TIME_SEVENDAY, SINITEM_TIME_30DAY, SINITEM_TIME_3DAY };		//½Ã°£Å×ÀÌºí

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
*					  ½ºÅ³ À¯ÁöÇü ¾ÆÀÌÄÜ Draw		
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
*Desc: ÇÁ¸®¹Ì¾ö ¾ÆÀÌÅÛ »ç¿ë½Ã  È£Ãâ		
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::UsePremiumItem(int kind) { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: ½ºÅ³ °ÔÀÌÁö°¡ »ý¼ºµÅ´Â  ¾ÆÀÌÅÛÀÏ°æ¿ì È£Ãâ	
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::SetPremiumItemSkill(int kind) { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: ÅÚ·¹Æ÷Æ® ÄÚ¾î »ç¿ë½Ã È£Ãâ						
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::UseTeleportScrollItem(short ItemIndex) { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: ´Ù¸¥ ÀÎÅÍÆäÀÌ½ºÃ¢ÀÌ ¶°ÀÕÀ»°æ¿ì ¸®ÅÏÇÑ´Ù.		
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::CheckWindowOpen() { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: ºó¹è¿­À» ´ó°ÜÁØ´Ù.		
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::CheckUpKeepItem()
{
	int i;
	//¾ÆÀÌÅÛÃ¼Å©
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
*Desc: À¯ÁöÇü ¼Ò¸ð¼º ¾ÆÀÌÅÛÀ» »ç¿ëÇÒ °æ¿ì µð½ºÇÃ·¹ÀÌ  	
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::SetUpKeepItem(int kind, DWORD dwTime, bool visible, char* id, int Percent, short ItemType) { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: À¯ÁöÇü¾ÆÀÌÄÜ ¼³¸í¹Ú½º 		
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::InfoBox(int PoisX, int PosiY, int Width, int Height) {}
/*----------------------------------------------------------------------------*
*Desc: À¯ÁöÇü¾ÆÀÌÄÜ ¼³¸í(ÀÌ¸§/À¯Áö½Ã°£ ) 		
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::DrawText() { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: Çì¾îÆ¾Æ®Æ÷¼Ç »ç¿ë 		
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::UseHairtintPotion(int ItemKind)
{
	switch(ItemKind){
		case 1:  //AÇü
			if( ChangeHairModel(0) )
				return TRUE;
		break;
		case 2:  //BÇü
			if( ChangeHairModel(1) )
				return TRUE;
		break;
		case 3:  //CÇü
			if( ChangeHairModel(2) )
				return TRUE;
		break;
		case 4:  //DÇü
			if( ChangeHairModel(3) )
				return TRUE;
		break;
		case 5:  //EÇü
			if( ChangeHairModel(4) )
				return TRUE;
		break;
		case 6:  //FÇü
			if( ChangeHairModel(5) )
				return TRUE;
		break;
		case 7:  //GÇü
			if( ChangeHairModel(6) )
				return TRUE;
		break;
		case 8:  //HÇü
			if( ChangeHairModel(7) )
				return TRUE;
		break;
			
		case 9:  //IÇü
			if( ChangeHairModel(8) )
				return TRUE;
		break;
		case 10:  //JÇü
			if( ChangeHairModel(9) )
				return TRUE;
		break;
		case 11:  //KÇü
			if( ChangeHairModel(10) )
				return TRUE;
		break;
		case 12:  //LÇü
			if( ChangeHairModel(11) )
				return TRUE;
		break;
		case 13:  //MÇü
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

// ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ ¾ÆÀÌÅÛ »ç¿ë
int cHaPremiumItem::UseAgingMaster(int ItemKind)
{
	switch(ItemKind)
	{
	case 0:
		sinCheckAgingLevel(SIN_AGING_ATTACK, true);
		sinCheckAgingLevel(SIN_AGING_CRITICAL, true);
		ReformCharForm(); //ÀçÀÎÁõ 
		break;
	case 1:
		if((cInvenTory.InvenItem[sInven[1].ItemIndex-1].CODE &sinITEM_MASK2) == sinDS1)
		{
			sinCheckAgingLevel(SIN_AGING_BLOCK, true);
			ReformCharForm(); //ÀçÀÎÁõ 
		}
		else if((cInvenTory.InvenItem[sInven[1].ItemIndex-1].CODE &sinITEM_MASK2) == sinOM1)
		{
			sinCheckAgingLevel(SIN_AGING_DEFENSE_ORB, true);
			ReformCharForm(); //ÀçÀÎÁõ 

		}
		break;
	case 2:
		sinCheckAgingLevel(SIN_AGING_DEFENSE_ARMOR, true);
		ReformCharForm(); //ÀçÀÎÁõ 

		break;
	case 3: // ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ(2Â÷) - ¾Ï¸´
		sinCheckAgingLevel(SIN_AGING_DEFENSE_ARMLETS, true);
		ReformCharForm(); //ÀçÀÎÁõ 

		break;
	case 4: // ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ(2Â÷) - Àå°©
		sinCheckAgingLevel(SIN_AGING_DEFENSE_GLOVES, true);
		ReformCharForm(); //ÀçÀÎÁõ 

		break;
	case 5: // ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ(2Â÷) - ºÎÃ÷
		sinCheckAgingLevel(SIN_AGING_DEFENSE_BOOTS, true);
		ReformCharForm(); //ÀçÀÎÁõ 
		break;
	}

	return TRUE;
}

// ¹ÚÀç¿ø - ½ºÅ³ ¸¶½ºÅÍ ¾ÆÀÌÅÛ »ç¿ë
int cHaPremiumItem::UseSkillMaster(int ItemKind)
{
	switch(ItemKind)
	{
	case 1:
		for(int j=1 ; j<5 ; j++) // 1Â÷ ÀüÁ÷ÈÄ »ý±â´Â ½ºÅ³
		{
			if(sinSkill.UseSkill[j].UseSkillCount<10000)
			{
				if(sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point)
				{
					sinSkill.UseSkill[j].UseSkillCount += 10000;
					cSkill.ReformSkillMasteryForm(0,j); //µ¥ÀÌÅ¸¸¦ °»½ÅÇÑ´Ù
				}
			}
		}
		break;
	case 2:
		for(int j=5 ; j<9 ; j++) // 2Â÷ ÀüÁ÷ÈÄ »ý±â´Â ½ºÅ³
		{
			if(sinSkill.UseSkill[j].UseSkillCount<10000)
			{
				if(sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point)
				{
					sinSkill.UseSkill[j].UseSkillCount += 10000;
					cSkill.ReformSkillMasteryForm(0,j); //µ¥ÀÌÅ¸¸¦ °»½ÅÇÑ´Ù
				}
			}
		}
		break;
	case 3:
		for(int j=9 ; j<13 ; j++) // 3Â÷ ÀüÁ÷ÈÄ »ý±â´Â ½ºÅ³
		{
			if(sinSkill.UseSkill[j].UseSkillCount<10000)
			{
				if(sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point)
				{
					sinSkill.UseSkill[j].UseSkillCount += 10000;
					cSkill.ReformSkillMasteryForm(0,j); //µ¥ÀÌÅ¸¸¦ °»½ÅÇÑ´Ù
				}
			}
		}
		break;
	}

	return TRUE;
}

// ¹ÚÀç¿ø - ÀÌµ¿ »óÁ¡ ¾ÆÀÌÅÛ
int cHaPremiumItem::UseMoveShop()
{
	return TRUE;
}
