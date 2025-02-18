#include <windows.h>
#include <io.h>
#include <stdio.h>         /* for sprintf                           */
#include <string.h>        /* for strlen                            */
#include <memory.h>
#include <process.h>       /* for _beginthread                      */
#include <unordered_map>
#include "Utils/common.h"

#include "smwsock.h"
#include "smlib3d\\smd3d.h"
#include "Character/character.h"
#include "fileread.h"
#include "Character/playmain.h"
#include "field.h"
#include "sinbaram\\sinlinkheader.h"
#include "effectsnd.h"

#include "Character/record.h"
#include "Character/playsub.h"
#include "language.h"
#include "Craft/CManufactureHandler.h"

#include "onserver.h"
#include "svr_Damage.h"
#include "Character/damage.h"
#include "..//cSkinChanger.h"
#include "../Shop/NewShop.h"
#include "../Shop/NewShopTime.h"
#include <Quest/Quest.h>
#include <Chat/ChatServer.h>
#include "..\cReviewSkinHandler.h"

#define	PK_POWER_DIVIDE	5
#define	PK_SCORE_DIVIDE	10

//공성전 필드 좌표 정보 ( 나중에 헤더파일로 이전예정 )
extern int CastleBattleZone_LineZ;


//기본 공격 데미지
WORD	rsDefaultDamage[10][2] = {
	{	7,	14	},
	{	17,	40	},
	{	25,	50	},
	{	45,	74	},
	{	50,	80	},
	{	55,	86	},
	{	60,	90	},
	{	65,	100	},
	{	70,	110	},
	{	70,	110	}
};

int	dm_Critical_Temp;			//크리티컬 값 임시 보관
int dm_SkillCode_Temp;			//스킬코드 임시보관
//실제 크리티컬 
int GetCritical(rsPLAYINFO* lpPlayInfo, smCHAR* lpChar, int Critical_Hit);
int GetCritical2(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, int Critical_Hit);

//캐릭터 데미지 기록
int	rsRecordCharDamage(rsPLAYINFO* lpPlayInfo, smCHAR* lpChar, int Damage);
//캐릭터 PK 기록
int	rsRecord_PKDamage(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, int Damage);


//다른 유저 공격 패킷 보내기 ( 유저 공격 )
int rsSendAttackUser(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, int Power, int AttackState, short sParam1 = 0, short sParam2 = 0);

//빌링아이템스킬적용
int	rsBillingItemSkill(rsPLAYINFO* lpPlayInfo, smCHAR* lpChar, int HitMonsters, int Power, int SkillCode);


#define	DISP_DAMAGE

#ifdef DISP_DAMAGE
static char szDispDamage[128];
static char szDispDamage2[128];	//해외

//데미지 정보 출력
int rsDisplayDamgeToClient(rsPLAYINFO* lpPlayInfo, char* szDmgMsg)
{
	TRANS_CHATMESSAGE	TransChatMessage;

	if (lpPlayInfo->AdminMode <= 2) return FALSE;

	lstrcpy(TransChatMessage.szMessage, szDmgMsg);
	TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
	TransChatMessage.size = 32 + lstrlen(TransChatMessage.szMessage);
	TransChatMessage.dwIP = 0;
	TransChatMessage.dwObjectSerial = 0;
	lpPlayInfo->lpsmSock->Send((char*)&TransChatMessage, TransChatMessage.size, TRUE);

	return TRUE;
}
#endif

#define TRANS_VIEW_LIMIT		(64*64)

int SendShowDmg(smCHAR* lpChar, int Dmg, int Type, rsPLAYINFO* lpPlayInfo)
{
	smTRANS_COMMAND smTransCommand;
	smTransCommand.code = SMTRANSCODE_DISPLAY_DAMAGE;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = lpChar->dwObjectSerial;
	smTransCommand.LParam = Dmg;
	smTransCommand.SParam = Type;

	lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

	return TRUE;
}


int rsRecvAttackData_Old(rsPLAYINFO* lpPlayInfo, TRANS_ATTACKDATA* lpTransAttackData, smCHAR* lpTargetChar, int Type)
{
#ifdef	_W_SERVER

	DWORD	dwTime;
	TRANS_CHATMESSAGE	TransChatMessage;
	smCHAR* lpChar;
	smTRANS_COMMAND		smTransCommand;
	int		x, y, z;
	int		cnt, len;
	int		ang, ang2;
	int		regs;
	smWINSOCK* lpsmSock;
	rsPLAYINFO* lpPlayInfo2;

	int	Dmg1, Dmg2, Dmg3, Dmg4;

	Dmg1 = 0;	Dmg2 = 0;	Dmg3 = 0; Dmg4 = 0;

	lpsmSock = lpPlayInfo->lpsmSock;
	dwTime = dwPlayServTime;

	//공격 데이타 기록
	lpPlayInfo->Recv_AttackCount++;
	lpPlayInfo->Recv_AttackDamage += lpTransAttackData->Power;

	if (lpPlayInfo->BadPlayer == TRUE) return FALSE;
	if (lpPlayInfo->HideMode && lpPlayInfo->AdminMode < 4) return FALSE;

	if (lpTargetChar) lpChar = lpTargetChar;
	else lpChar = srFindCharFromSerial(lpTransAttackData->dwTarObjectSerial);

	if (!lpChar || lpChar->smCharInfo.State != smCHAR_STATE_NPC) {
		lpPlayInfo->AttackCount++;
	}

	if ((dwPlayServTime - lpPlayInfo->dwAttackTime) > 5000) {
		//공격 타이밍 계산
		lpPlayInfo->dwAttackTime = dwPlayServTime;
		if (lpPlayInfo->AttackCount >= 40) {
			//5초동안 10회 이상 공격 들어옴

			smTransCommand.WParam = 1823;
			smTransCommand.SParam = lpPlayInfo->AttackCount;
			smTransCommand.LParam = lpPlayInfo->AttackCount;

			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
			rsSendDataServer(lpPlayInfo->lpsmSock, &smTransCommand);

			if (lpPlayInfo->AttackCount >= 80)
				lpPlayInfo->BadPlayer = TRUE;
			//DisconnectUser( lpsmSock );			//30회 이상 들어온 경우 연결 종료

			return FALSE;
		}
		lpPlayInfo->AttackCount = 0;
	}

	if (Type != 33) {
		if (!Type && lpTransAttackData->dwChkSum != (DWORD)(lpTransAttackData->Power * 2002 + (lpTransAttackData->x * lpTransAttackData->y * lpTransAttackData->z))) {
			//첵크섬 확인 오류
			smTransCommand.WParam = 1820;
			smTransCommand.SParam = lpTransAttackData->Power;
			smTransCommand.LParam = lpTransAttackData->Power;
			//RecordHackLogFile( lpPlayInfo , &smTransCommand );

			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
			rsSendDataServer(lpPlayInfo->lpsmSock, &smTransCommand);

			//DisconnectUser( lpsmSock );
			lpPlayInfo->BadPlayer = TRUE;
			return FALSE;
		}

		if (lpTransAttackData->Power >= 20000) {
			//공격력 크기 터무니 없음
			//smTransCommand.WParam = 1821;
			//smTransCommand.SParam = lpTransAttackData->Power;
			//smTransCommand.LParam = lpTransAttackData->Power;
			//RecordHackLogFile( lpPlayInfo , &smTransCommand );

			//smTransCommand.size = sizeof(smTRANS_COMMAND);
			//smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
			//rsSendDataServer(lpPlayInfo->lpsmSock, &smTransCommand);

			//DisconnectUser( lpsmSock );
			//lpPlayInfo->BadPlayer = TRUE;
			return FALSE;
		}
	}
	if (abs(((long)(dwTime - lpPlayInfo->dwRecvTimePlayBuff))) > 7 * 1000) {			// pluto 형변환
		//7초 이상 모션 데이타 수신이 없었던 경우 ( 공격인정하지 않음 )
		return FALSE;
	}

	//하드코어 이벤트용 ( 이벤트 몬스터를 라인 밖에서 공격하면 무효 )
	if (lpChar && lpChar->PartyFlag == rsHARDCORE_EVENT_FLAG) {
		if (lpPlayInfo->EventMode != rsHARDCORE_EVENT_FLAG) {
			return FALSE;
		}
		/*
		if ( lpPlayInfo->EventScore>100000 ) {
			lpTransAttackData->Power>>=1;			//10만 이상 점수 획득한 경우 공격력을 반으로 감소 ( 너무 잘해서 의심 )
		}
		*/
	}

	if (lpChar) {
		lpChar->dwLastTransTime = dwPlayServTime;		//최근 활성화된 시간

		x = (lpPlayInfo->Position.x - lpChar->pX) >> FLOATNS;
		y = (lpPlayInfo->Position.y - lpChar->pY) >> FLOATNS;
		z = (lpPlayInfo->Position.z - lpChar->pZ) >> FLOATNS;
		if (Type != 33) {
			if (abs(y) > 100 || (x * x + z * z) > (800 * 800)) return FALSE;
		}

		x = (lpPlayInfo->Position.x - lpTransAttackData->x) >> FLOATNS;
		y = (lpPlayInfo->Position.y - lpTransAttackData->y) >> FLOATNS;
		z = (lpPlayInfo->Position.z - lpTransAttackData->z) >> FLOATNS;
		//if ( (x*x+y*y+z*z)>(64*8)*(64*8) ) break;					

		if (lpChar->MotionInfo->State != CHRMOTION_STATE_DEAD && lpChar->MotionInfo->State != CHRMOTION_STATE_WARP) {

			Dmg1 = lpTransAttackData->Power;

			//일단 적일 경우만..
			if (lpChar->smCharInfo.State) {

#ifndef	_LANGUAGE_KOREAN
				//캐릭터와 몬스터와의 레벨차가 큰경우 강제 보정
				cnt = lpChar->smCharInfo.Level - lpPlayInfo->smCharInfo.Level;
				if (cnt > 20) {
					cnt += (rand() % (cnt - 20));
					if (cnt > 25) {
						if (lpPlayInfo->smCharInfo.Level < 10) cnt = 20;
						else cnt = lpPlayInfo->smCharInfo.Level * 2;

						if (cnt < lpTransAttackData->Power) {
							lpTransAttackData->Power = GetRandomPos(cnt / 2, cnt);
						}
						if ((lpTransAttackData->AttackState & 0xFFFF) > 1) {
							lpTransAttackData->AttackState &= 0xFFFF0000;
						}
					}
				}
#endif

				if (lpPlayInfo->BadPlayer == 2 || lpPlayInfo->BadPlayer == 4) {
					//해킹 유저 공격력 조정
					cnt = lpPlayInfo->smCharInfo.Level / 10;
					if (cnt < 1 || cnt>8) cnt = 1;

					cnt = GetRandomPos(rsDefaultDamage[cnt][0], rsDefaultDamage[cnt][1]);
					if (cnt < lpTransAttackData->Power) {
						lpTransAttackData->Power = cnt;
					}
				}

				//최근 목표 몬스터 기록
				lpPlayInfo->lpChrTarget = lpChar;
				lpPlayInfo->dwTarObjSerial = lpChar->dwObjectSerial;

				lpChar->PlayStunCount = 0;			//중립모드 해제

				//인첸트 워폰 스킬 적용
				if (lpPlayInfo->dwSkill_EnchantWeapon_Time) {
					if (lpPlayInfo->dwSkill_EnchantWeapon_Time > dwPlayServTime) {
						if (((lpTransAttackData->AttackState >> 16) & 0xF) == 0) {
							len = lpPlayInfo->dwSkill_EnchantWeapon_Param & 0xFF;		//포인트
							switch (lpPlayInfo->dwSkill_EnchantWeapon_Param >> 8) {
							case 0:		//얼음 ( 범위공격이 따로 들어오므로 생략)
								/*
								lpTransAttackData->Power += GetRandomPos(
									Enchant_Weapon_Damage_Ice[len-1][0] , Enchant_Weapon_Damage_Ice[len-1][1] );
									*/

									// 장별 - 스킬 수정
								lpTransAttackData->Power += GetRandomPos(
									Enchant_Weapon_Damage_Ice[len - 1][0], Enchant_Weapon_Damage_Ice[len - 1][1]);
								lpTransAttackData->AttackState |= (sITEMINFO_ICE + 1) << 16;
								lpTransAttackData->AttackState |= (Enchant_Weapon_Damage_Ice[len - 1][1]) << (16 + 4);

								//	lpTransAttackData->AttackState |= (sITEMINFO_ICE+1)<<16;
								//	lpTransAttackData->AttackState |= (1)<<(16+4);
								break;
							case 1:		//번개
								lpTransAttackData->Power += GetRandomPos(
									Enchant_Weapon_Damage_Lightning[len - 1][0], Enchant_Weapon_Damage_Lightning[len - 1][1]);
								lpTransAttackData->AttackState |= (sITEMINFO_LIGHTING + 1) << 16;
								lpTransAttackData->AttackState |= (Enchant_Weapon_Damage_Lightning[len - 1][1]) << (16 + 4);
								break;

							case 2:		//불
								lpTransAttackData->Power += GetRandomPos(
									Enchant_Weapon_Damage_Fire[len - 1][0], Enchant_Weapon_Damage_Fire[len - 1][1]);
								lpTransAttackData->AttackState |= (sITEMINFO_FIRE + 1) << 16;
								lpTransAttackData->AttackState |= (Enchant_Weapon_Damage_Fire[len - 1][1]) << (16 + 4);
								break;
							}
						}
					}
					else {	//종료 ( 시간초과 )
						lpPlayInfo->dwSkill_EnchantWeapon_Time = 0;
						lpPlayInfo->dwSkill_EnchantWeapon_Param = 0;
					}
				}

				//속성 공격일 경우
				regs = (lpTransAttackData->AttackState >> 16) & 0xF;
				if (regs && regs < sITEMINFO_NONE) {
					cnt = lpTransAttackData->AttackState >> (16 + 4);		//속성 값 ( 상위 비트에 전체 데미지중 속성 데미지만 저장됨 )
					if (!cnt)
						cnt = lpTransAttackData->Power;

					//속성에 따른 저항력 적용
					len = lpChar->smCharInfo.Resistance[regs - 1];
					if (len) {					//속성 저항력 계산
						if (len >= 100) len = 100;
						if (len <= -100) len = -100;
						lpTransAttackData->Power -= ((cnt * len) / 100);

						if (lpTransAttackData->Power < 0) lpTransAttackData->Power = 0;
					}

					switch (regs - 1) { //(lpTransAttackData->AttackState>>16)-1) {
					case sITEMINFO_ICE:
						if (lpPlayInfo->dwSkill_EnchantWeapon_Time > dwPlayServTime && (lpPlayInfo->dwSkill_EnchantWeapon_Param >> 8) == 0) {
							//얼음 공격 속도 저하 ( 인체트 일때 적용 )
							lpChar->PlaySlowCount = 16 * 3;									//속도 느려짐 ( 3초 )
							lpChar->PlaySlowCount -= (lpChar->PlaySlowCount * len) / 100;
							lpChar->PlaySlowSpeed = 230 - 10 * (lpPlayInfo->dwSkill_EnchantWeapon_Param & 0xFF);	//레벨에 따른 속도
							lpChar->PlayDistortion = 0;			//디스토션 해제
							break;
						}
						if (lpPlayInfo->dwSkill_DancingSword_Time > dwPlayServTime && (lpPlayInfo->dwSkill_DancingSword_Param & 0xFF) == 0) {
							//댄싱소드 얼음 공격
							lpChar->PlaySlowCount = 16 * 5;					//속도 느려짐 ( 5초에서 상쇄 )
							lpChar->PlaySlowCount -= (lpChar->PlaySlowCount * len) / 100;
							lpChar->PlaySlowSpeed = 230 - 10 * (lpPlayInfo->dwSkill_DancingSword_Param >> 16);	//레벨에 따른 속도
							lpChar->PlayDistortion = 0;			//디스토션 해제
							break;
						}
						if (dm_SkillCode_Temp == SKILL_PLAY_PET_ATTACK) {
							//펫 얼음공격
							lpChar->PlaySlowCount = 16 * 10;					//속도 느려짐 ( 10초에서 상쇄 )
							lpChar->PlaySlowCount -= (lpChar->PlaySlowCount * len) / 100;
							lpChar->PlaySlowSpeed = 180;
							lpChar->PlayDistortion = 0;			//디스토션 해제
							break;
						}
						if (lpPlayInfo->dwSkill_FrostJavelin_Time > dwPlayServTime) {
							//프로스트재블린 얼음공격
							//속도 느려짐
							lpChar->PlaySlowCount = 16 * Frost_Javelin_IceTime[lpPlayInfo->dwSkill_FrostJavelin_Param];
							lpChar->PlaySlowCount -= (lpChar->PlaySlowCount * len) / 100;
							lpChar->PlaySlowSpeed = 240 - (240 * Frost_Javelin_SpeedSubPercent[lpPlayInfo->dwSkill_FrostJavelin_Param]) / 100;
							lpChar->PlayDistortion = 0;			//디스토션 해제
							break;
						}
						break;

					}

				}

				cnt = (lpTransAttackData->Power * lpChar->smCharInfo.Absorption) / 100;
				cnt = lpTransAttackData->Power - cnt;

				Dmg2 = lpTransAttackData->Power;
				Dmg3 = cnt;
				Dmg4 = cnt;

				//자기보다 레벨이 큰 몬스터를 한방에 죽이는 경우
				if ((lpChar->smCharInfo.Life[1] / 4) < cnt &&
					lpChar->smCharInfo.Level > lpPlayInfo->smCharInfo.Level) {

					cnt = GetRandomPos(lpChar->smCharInfo.Life[1] / 8, lpChar->smCharInfo.Life[1] / 4);
				}

				if (cnt > 0) {
					//선 공격자 기록시켜 경험치 우선 획득
					if (lpChar->smCharInfo.Life[0] == lpChar->smCharInfo.Life[1]) {
						if (!lpChar->lpExpAttackPlayInfo) {
							//선공격 등록
							lpChar->lpExpAttackPlayInfo = lpPlayInfo;
							lpChar->ExpAttackLife = 0;
							lpChar->dwExpAttackTime = dwTime;
						}
					}
					if (lpChar->lpExpAttackPlayInfo) {
						if (lpChar->lpExpAttackPlayInfo == lpPlayInfo) {
							//선공격자 공격 기록
							lpChar->ExpAttackLife += cnt;
							lpChar->dwExpAttackTime = dwTime;
						}
						else {
							if ((dwTime - lpChar->dwExpAttackTime) > 15000) {
								//15초 이상 공격없음/ 선공격자 취소
								lpChar->lpExpAttackPlayInfo = 0;
							}
						}
					}

					/////////////////// 스킬 처리 /////////////////////
					//HOLY_VALOR ( 공격력 % 가중 )
					if (lpPlayInfo->dwSkill_HolyValor_Time) {
						if (lpPlayInfo->dwSkill_HolyValor_Time > dwPlayServTime) {
							if (lpChar->smCharInfo.Brood == smCHAR_MONSTER_UNDEAD)
								cnt += (cnt * lpPlayInfo->dwSkill_HolyValor_Param) / 100;	//언데드 공격력 가중
						}
						else {	//종료 ( 시간초과 )
							lpPlayInfo->dwSkill_HolyValor_Time = 0;
							lpPlayInfo->dwSkill_HolyValor_Param = 0;
						}
					}

					Dmg4 = cnt;

					///////////////////////////////////////////////////
					//명중 ( 공격 성공 )
					lpChar->smCharInfo.Life[0] -= cnt;

					if (lpChar->smCharInfo.Life[0] > 0)
						SendShowDmg(lpChar, cnt, 6, lpPlayInfo);
					else
						SendShowDmg(lpChar, cnt, 7, lpPlayInfo);

					if (lpChar->lpAttackDamageList || lpChar->lpAttackDamageList_BlessCastle || 1) rsRecordCharDamage(lpPlayInfo, lpChar, cnt);	//받은공격 기억하는 몹

					if (lpChar->smCharInfo.Level > 14) {
						//레벨 14이상 20%이상 타격시에 멈찟
						cnt = (cnt * 5) / lpChar->smCharInfo.Life[1];
					}
					else {
						//레벨 10이하 10%이상 타격시에 멈찟
						cnt = (cnt * 10) / lpChar->smCharInfo.Life[1];
					}

					if (lpChar->smCharInfo.Life[0] > 0 && (cnt >= 1 || (lpTransAttackData->AttackState & 0xFFFF) > 1) &&
						(rand() % 100) < lpChar->smMonsterInfo.DamageStunPers) {
						//공격파워가 세거나 크리티컬 판정일 경우
						//맞으면 뒤로 물러나게 함
						ang2 = GetRadian2D(lpChar->pX, lpChar->pZ, lpPlayInfo->Position.x, lpPlayInfo->Position.z);
						ang = (ang2 + ANGLE_180) & ANGCLIP;
						lpChar->Angle.y = ang;
						lpChar->MoveAngle(10);
						lpChar->Angle.y = ang2;

						if (lpChar->smCharInfo.Life[0] &&
							lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
							lpChar->MotionInfo->State != CHRMOTION_STATE_EAT &&
							lpChar->MotionInfo->State != CHRMOTION_STATE_WARP &&
							lpChar->MotionInfo->State != CHRMOTION_STATE_SKILL) {

							lpChar->SetMotionFromCode(CHRMOTION_STATE_DAMAGE);
						}
					}
				}
				lpPlayInfo->dwLastAttackTime = dwTime;


#ifdef DISP_DAMAGE
				if (lpPlayInfo->AdminMode > 2)
				{
					wsprintf(szDispDamage, ">%s Damaged ( %d %d %d %d )", lpChar->smCharInfo.szName, Dmg1, Dmg2, Dmg3, Dmg4);
					rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
				}
#endif
			}
			else
			{
				// NPC = TRUE
				if ((abs((long)lpPlayInfo->dwLastAttackTime - (long)dwTime) > 1500) || Type == 33)
				{
					if (Type != 33)
						lpPlayInfo->dwLastAttackTime = dwTime;

					if (lpChar->smMonsterInfo.QuestCode && lpChar->smMonsterInfo.QuestCode == (lpTransAttackData->AttackState >> 16)) {
						if (rsProcessQuest(lpPlayInfo, lpChar, lpTransAttackData->AttackState >> 16, lpTransAttackData->AttackState & 0xFFFF) == TRUE)
							return TRUE;
					}

					if (lpChar->szChatMessage[0] && (!lpChar->smMonsterInfo.EventNPC || lpChar->smMonsterInfo.EventNPC == 5))
					{
						if (lpChar->smMonsterInfo.NpcMsgCount > 1)
						{
							SERVERCHAT->SendUserBoxChat(lpPlayInfo, lpChar->dwObjectSerial, lpChar->smMonsterInfo.lpNpcMessage[rand() % lpChar->smMonsterInfo.NpcMsgCount]);
						}
						else
						{
							SERVERCHAT->SendUserBoxChat(lpPlayInfo, lpChar->dwObjectSerial, lpChar->szChatMessage);
						}
					}

					if (lpChar->smMonsterInfo.SellAttackItemCount ||
						lpChar->smMonsterInfo.SellDefenceItemCount ||
						lpChar->smMonsterInfo.SellEtcItemCount) {

						// ABRE NPC DE VENDAS AQUI
						SendShopItemList(lpsmSock, lpChar);
					}

					
					if (lpChar->smMonsterInfo.SkillMaster) {
						if (lpPlayInfo->AdminMode) {
							SendSkillMenu(lpsmSock, lpChar, 2);
						}
						else
							SendSkillMenu(lpsmSock, lpChar, lpChar->smMonsterInfo.SkillChangeJob);

					}

					if (lpChar->smMonsterInfo.WareHouseMaster)
					{
						SendOpenWareHouse(lpsmSock);
					}

					if (lpChar->smMonsterInfo.Caravana)
					{
						Caravana::GetInstance()->OpenCaravan(lpsmSock, lpChar);
					}

					if (lpChar->smMonsterInfo.ItemMix)
					{
						SendOpenMixItem(lpsmSock, lpChar->smMonsterInfo.ItemMix);
					}

					if (lpChar->smMonsterInfo.Smelting)
					{
						SendOpenSmelting(lpsmSock);
					}
					if (lpChar->smMonsterInfo.Manufacture)
					{
						SendOpenManufacture(lpsmSock);
					}
					if (lpChar->smMonsterInfo.CollectMoney)
					{
						SendOpenCollectMoney(lpsmSock);
					}

					if (lpChar->smMonsterInfo.EventGift)
					{
						SendOpenEventGift(lpsmSock);
					}

					if (lpChar->smMonsterInfo.SkinChange)
						cSkinChanger.OpenSkinChange(lpPlayInfo);				

					if (lpChar->smMonsterInfo.GiftExpress)
					{
						smTransCommand.size = sizeof(smTRANS_COMMAND);
						smTransCommand.code = smTRANSCODE_ITEM_EXPRESS;
						smTransCommand.WParam = 0;
						smTransCommand.LParam = 0;
						smTransCommand.SParam = 0;
						rsSendDataServer(lpsmSock, &smTransCommand);
					}

					if (lpChar->smMonsterInfo.WingQuestNpc) {
						smTransCommand.size = sizeof(smTRANS_COMMAND);
						smTransCommand.code = smTRANSCODE_WING_NPC;
						smTransCommand.WParam = lpChar->smMonsterInfo.WingQuestNpc;
						smTransCommand.LParam = 0;
						smTransCommand.SParam = 0;
						lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
					}

					if (lpChar->smMonsterInfo.TelePortNpc) {
						int temp = 0;
						if (lpChar->smMonsterInfo.TelePortNpc == 3)
						{
							temp = -1;
						}
						smTransCommand.size = sizeof(smTRANS_COMMAND);
						smTransCommand.code = smTRANSCODE_WING_NPC;
						smTransCommand.WParam = temp;
						smTransCommand.LParam = lpChar->smMonsterInfo.TelePortNpc;
						smTransCommand.SParam = 0;
						lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
					}


					if (lpChar->smMonsterInfo.StarPointNpc && lpPlayInfo->smCharInfo.Level >= lpChar->smMonsterInfo.StarPointNpc)
					{
						smTransCommand.size = sizeof(smTRANS_COMMAND);
						smTransCommand.code = smTRANSCODE_OPEN_STARPOINT;
						smTransCommand.WParam = 0;
						smTransCommand.LParam = 0;
						smTransCommand.SParam = 0;
						lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
					}

					if (lpChar->smMonsterInfo.GiveMoneyNpc) {
						//돈 기부함
						smTransCommand.size = sizeof(smTRANS_COMMAND);
						smTransCommand.code = smTRANSCODE_OPEN_GIVEMONEY;
						smTransCommand.WParam = 0;
						smTransCommand.LParam = 0;
						smTransCommand.SParam = 0;
						lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
					}



					if (lpChar->smMonsterInfo.EventNPC) {
						if (lpChar->smMonsterInfo.EventNPC == 5) {
							//SOD 회계사
							smTransCommand.size = sizeof(smTRANS_COMMAND);
							smTransCommand.code = smTRANSCODE_OPEN_COLLECT;
							smTransCommand.WParam = 5;
							smTransCommand.LParam = 0;
							smTransCommand.SParam = 0;
							lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
						}
						else {
							//SOD 입장도우미
							SendOpenEvent(lpChar, lpPlayInfo, lpChar->smMonsterInfo.EventNPC);
						}

						if (lpChar->MotionInfo->State < 0x100) {
							lpChar->SetMotionFromCode(CHRMOTION_STATE_ATTACK);
						}
					}


					if (lpChar->smMonsterInfo.ItemAging) {
						//아이템 에이징 NPC
						SendOpenAgingItem(lpsmSock);
					}

					if (lpChar->smMonsterInfo.ClanNPC) {
						//클랜 NPC
						SendOpenClanMenu(lpsmSock);
					}

					if (lpChar->smMonsterInfo.BlessCastleNPC) {
						//블래스캐슬 설정정보 송신
						rsSendBlessCastInfo(lpPlayInfo, lpChar->smMonsterInfo.BlessCastleNPC);
					}

					if (lpChar->smMonsterInfo.PollingNpc) {
						//설문조사 NPC
						smTransCommand.size = sizeof(smTRANS_COMMAND);
						smTransCommand.code = smTRANSCODE_PUBLIC_POLLING;
						smTransCommand.WParam = lpChar->smMonsterInfo.PollingNpc;
						smTransCommand.LParam = 0;
						smTransCommand.SParam = 0;
						smTransCommand.EParam = 0;
						rsSendDataServer(lpsmSock, &smTransCommand);
					}


					if (lpChar->smMonsterInfo.szMediaPlayNPC_Title && lpChar->smMonsterInfo.szMediaPlayNPC_Path) {
						//동영상 재생 NPC

						lstrcpy(TransChatMessage.szMessage, lpChar->smMonsterInfo.szMediaPlayNPC_Path);
						len = lstrlen(TransChatMessage.szMessage);
						lstrcpy(TransChatMessage.szMessage + len + 1, lpChar->smMonsterInfo.szMediaPlayNPC_Title);

						TransChatMessage.code = smTRANSCODE_MEDIA_NPC;
						TransChatMessage.size = 34 + len + lstrlen(lpChar->smMonsterInfo.szMediaPlayNPC_Title);
						TransChatMessage.dwIP = 0;
						TransChatMessage.dwObjectSerial = lpChar->dwObjectSerial;
						lpsmSock->Send((char*)&TransChatMessage, TransChatMessage.size, TRUE);
					}

					// 석지용 - 믹스쳐 리셋 ( 믹스쳐 리셋 npc를 선택하면 창을 띄우라는 메시지를 보내는 함수 호출 )
					if (lpChar->smMonsterInfo.MixtureReset)
					{
						SendOpenMixtureItemReset(lpsmSock);
					}
					//ABRE NPC AQUI
					if (lpChar->smMonsterInfo.ItemShop)
					{
						NewShop::GetInstance()->SendItems(lpPlayInfo);
						NewShop::GetInstance()->SendCoinToGame(lpPlayInfo);
					}

					if (lpChar->smMonsterInfo.ItemShopTime)
					{
						NewShopTime::GetInstance()->SendItems(lpPlayInfo);
						NewShopTime::GetInstance()->SendTimeToGame(lpPlayInfo);
					}

					if (lpChar->smMonsterInfo.NpcQuest)
					{
						Quest::GetInstance()->getStatus(lpPlayInfo, true);
						GameMasters::getInstance()->Packet(lpPlayInfo, smTRANSCODE_OPEN_NPC);
					}

					if (lpChar->smMonsterInfo.NpcRanking)
					{
						TopRanking::GetInstance()->sendTopPlayers(lpPlayInfo);
					}

					if (lpChar->smMonsterInfo.NpcListaMix)
					{
						GameMasters::getInstance()->Packet(lpPlayInfo, 0x50700009);
					}
						
					if (lpChar->smMonsterInfo.NpcSodRanking)
					{
						SodRanking::GetInstance()->SendSodRanking(lpPlayInfo);
					}

					if (lpChar->smMonsterInfo.NpcCraftGold)
					{
						CRAFTGOLDHANDLER->SendOpenManufactureWindow(lpPlayInfo);
					}

					if (lpChar->smMonsterInfo.NpcCraftCoin)
					{
						CRAFTHANDLER->SendOpenManufactureWindow(lpPlayInfo);
					}

					if (lpChar->smMonsterInfo.NpcReview)
					{
						REVIEWSKINHANDLER->SendOpenReviewSkin(lpPlayInfo);
		
					}

					if (lpChar->smMonsterInfo.NpcArena)
					{
						time_t now = time(0);
						struct tm* Data = localtime(&now);

						GameMasters::getInstance()->Packet(lpPlayInfo, 0x50600023, Data->tm_hour, Data->tm_min, Data->tm_sec, Data->tm_wday);
					}
				}
			}

			if (lpChar->smCharInfo.Life[0] <= 0) {

				if (lpChar->UseObject_VirtualLife && lpChar->sObject_VirtualLife[0] > 0)
					lpChar->smCharInfo.Life[0] = (lpChar->smCharInfo.Life[1] * 1) / 100;
				else
				{
					lpChar->SetMotionFromCode(CHRMOTION_STATE_DEAD);
					lpChar->lpExt2 = (void*)lpPlayInfo;
				}

				lpChar->SetMotionFromCode(CHRMOTION_STATE_DEAD);

				lpChar->lpExt2 = (void*)lpPlayInfo;


			}
		}
	}
	else {
		//현재 PK 불가 모드로 설정
		//캐릭터를 찾는다 ( 나중에 빨리 칮을수 있게 코드화 시켜 바꿔 줘야 함 )
		lpPlayInfo2 = srFindUserFromSerial(lpTransAttackData->dwTarObjectSerial);
		if (lpPlayInfo2) {
			if (rsServerConfig.Enable_PK || lpPlayInfo->smCharInfo.State != lpPlayInfo2->smCharInfo.State) {

				if (lpPlayInfo->AdminMode || (lpPlayInfo->smCharInfo.Level > LIMIT_PK_LEVEL && lpPlayInfo2->smCharInfo.Level > LIMIT_PK_LEVEL)) {
					//각각 레벨 10 이하는 PK금지
					//lpPlayInfo2->lpsmSock->Send( (char *)lpTransAttackData , lpTransAttackData->size , TRUE );

					//다른 유저 공격 패킷 보내기 ( 유저 공격 )
					rsSendAttackUser(lpPlayInfo, lpPlayInfo2, lpTransAttackData->Power / PK_POWER_DIVIDE, 0x80);
				}
			}
		}
	}
#endif		
	return TRUE;

}


int rsRecvRangeAttackData_Old(rsPLAYINFO* lpPlayInfo, TRANS_SKIL_ATTACKDATA* lpTransSkilAttackData, int Type)
{

	smTRANS_COMMAND		smTransCommand;

	lpPlayInfo->Recv_AttackCount++;
	lpPlayInfo->Recv_AttackDamage += lpTransSkilAttackData->Power;

	if (lpPlayInfo->HideMode && lpPlayInfo->AdminMode < 4) return FALSE;

	RecvRangeAttack(lpPlayInfo, lpTransSkilAttackData);

	if (lpTransSkilAttackData->Power >= 1000) {
		smTransCommand.WParam = 1830;
		smTransCommand.SParam = lpTransSkilAttackData->Power;
		smTransCommand.LParam = smTransCommand.SParam;
		RecordHackLogFile(lpPlayInfo, &smTransCommand);

		DisconnectUser(lpPlayInfo->lpsmSock);
	}

	return TRUE;
}

//공격 범위형 유저 수신
int rsRecvRangeAttackUserData_Old(rsPLAYINFO* lpPlayInfo, TRANS_SKIL_ATTACKDATA* lpTransSkilAttackData)
{

	smTRANS_COMMAND		smTransCommand;

	//범위형 스킬 공격 ( 유저 공격 )
	//공격 데이타 기록
	lpPlayInfo->Recv_AttackCount++;
	lpPlayInfo->Recv_AttackDamage += lpTransSkilAttackData->Power;

	if (lpPlayInfo->HideMode && lpPlayInfo->AdminMode < 4) return FALSE;

	RecvRangeAttackUser(lpPlayInfo, lpTransSkilAttackData);

	if (lpTransSkilAttackData->Power >= 1000) {
		smTransCommand.WParam = 1830;
		smTransCommand.SParam = lpTransSkilAttackData->Power;
		smTransCommand.LParam = smTransCommand.SParam;
		RecordHackLogFile(lpPlayInfo, &smTransCommand);

		DisconnectUser(lpPlayInfo->lpsmSock);
	}

	return TRUE;
}

int	rsCheckAttackRange(int dx, int dy, int dz, smCHAR* lpChar, int dAttackSize)
{
	int x, y, z;

	x = (dx - lpChar->pX) >> FLOATNS;
	y = (dy - lpChar->pY) >> FLOATNS;
	z = (dz - lpChar->pZ) >> FLOATNS;

	if (abs(y) > 100 || (x * x + z * z) > dAttackSize)
		return FALSE;

	return TRUE;
}

int	rsCheckAttackRange(int dx, int dy, int dz, rsPLAYINFO* lpPlayInfo)
{
	int x, y, z;

	x = (dx - lpPlayInfo->Position.x) >> FLOATNS;
	y = (dy - lpPlayInfo->Position.y) >> FLOATNS;
	z = (dz - lpPlayInfo->Position.z) >> FLOATNS;

	if (abs(y) > 100 || (x * x + z * z) > (300 * 300))
		return FALSE;

	return TRUE;
}

int rsSendAttackUser(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, int Power, int AttackState, short sParam1, short sParam2)
{

	TRANS_ATTACKDATA	TransAttackData;
	smTRANS_COMMAND		smTransCommand;
	int	temp;

	// pluto PK허용 필드 전부
	if (!lpPlayInfo->AdminMode)
	{
		if (rsServerConfig.Enable_PKField_All)
		{
			// pluto PK허용 필드 전부 수정
			if (!rsServerConfig.Enable_PK || lpPlayInfo->Position.Area < 11 && lpPlayInfo->Position.Area > 34)
				return FALSE;			//현재 일반유저는 PK금지

			if (rsServerConfig.BlessCastleMode == 2 && !lpPlayInfo->AdminMode) return FALSE;
		}
		else
		{
			if (!rsServerConfig.Enable_PK && !lpPlayInfo->PkMode && !lpPlayInfo2->PkMode &&
				(sField[lpPlayInfo->Position.Area].State != FIELD_STATE_CASTLE || lpPlayInfo->Position.z > CastleBattleZone_LineZ) &&
				lpPlayInfo->Position.Area != FIELD_ARENA && lpPlayInfo->Position.Area != 48)
				return FALSE;

			if (lpPlayInfo->Position.Area == FIELD_ARENA && EventoArena::GetInstance()->arenaStage != 2) return FALSE;
			if (lpPlayInfo->Position.Area == FIELD_ARENA && EventoArena::GetInstance()->arenaStage == 2 && (lpPlayInfo->nEquipeArena == lpPlayInfo2->nEquipeArena)) return FALSE;

			if (rsServerConfig.BlessCastleMode == 2 && !lpPlayInfo->PkMode && !lpPlayInfo2->PkMode && !lpPlayInfo->AdminMode && lpPlayInfo->Position.Area != FIELD_ARENA && lpPlayInfo->Position.Area != 42 && lpPlayInfo->Position.Area != 36) return FALSE;
		}
	}


	TransAttackData.code = smTRANSCODE_ATTACKDATA;
	TransAttackData.size = sizeof(TRANS_ATTACKDATA);

	TransAttackData.x = lpPlayInfo2->Position.x;
	TransAttackData.y = lpPlayInfo2->Position.y;
	TransAttackData.z = lpPlayInfo2->Position.z;


	TransAttackData.AttackState = AttackState;
	TransAttackData.AttackSize = 32 * fONE;
	TransAttackData.Power = Power;

	//종성 아이템 시간종료 확인
	if (lpPlayInfo->dwSiegeItem_Scroll_Time && lpPlayInfo->dwSiegeItem_Scroll_Time < dwPlayServTime) {
		lpPlayInfo->dwSiegeItem_Scroll_Code = 0;
		lpPlayInfo->dwSiegeItem_Scroll_Time = 0;
	}
	if (lpPlayInfo->dwSiegeItem_Stone1_Time && lpPlayInfo->dwSiegeItem_Stone1_Time < dwPlayServTime) {
		lpPlayInfo->dwSiegeItem_Stone1_Code = 0;
		lpPlayInfo->dwSiegeItem_Stone1_Time = 0;
	}
	if (lpPlayInfo->dwSiegeItem_Stone2_Time && lpPlayInfo->dwSiegeItem_Stone2_Time < dwPlayServTime) {
		lpPlayInfo->dwSiegeItem_Stone2_Code = 0;
		lpPlayInfo->dwSiegeItem_Stone2_Time = 0;
	}

	//종성 아이템 시간종료 확인
	if (lpPlayInfo2->dwSiegeItem_Scroll_Time && lpPlayInfo2->dwSiegeItem_Scroll_Time < dwPlayServTime) {
		lpPlayInfo2->dwSiegeItem_Scroll_Code = 0;
		lpPlayInfo2->dwSiegeItem_Scroll_Time = 0;
	}
	if (lpPlayInfo2->dwSiegeItem_Stone1_Time && lpPlayInfo2->dwSiegeItem_Stone1_Time < dwPlayServTime) {
		lpPlayInfo2->dwSiegeItem_Stone1_Code = 0;
		lpPlayInfo2->dwSiegeItem_Stone1_Time = 0;
	}
	if (lpPlayInfo->dwSiegeItem_Stone2_Time && lpPlayInfo->dwSiegeItem_Stone2_Time < dwPlayServTime) {
		lpPlayInfo->dwSiegeItem_Stone2_Code = 0;
		lpPlayInfo->dwSiegeItem_Stone2_Time = 0;
	}

	if (lpPlayInfo2->dwSiegeItem_Scroll_Code == (sinBC1 | sin01)) {	//무적스크롤
		if (lpPlayInfo2->Position.Area == rsCASTLE_FIELD) return FALSE;
	}
	if (lpPlayInfo2->dwSiegeItem_Scroll_Code == (sinBI1 | sin05))		//무적스크롤
		return FALSE;

	if (lpPlayInfo2->dwSiegeItem_Scroll_Code == (sinBC1 | sin03)) {	//추가 회피
		if (lpPlayInfo2->Position.Area == rsCASTLE_FIELD) {
			if ((rand() % 100) < (int)lpPlayInfo2->dwSiegeItem_Scroll_Param) {
#ifdef DISP_DAMAGE
				if (lpPlayInfo2->AdminMode > 2) {
					wsprintf(szDispDamage, "> %s Esquiva Adicional ( Pergaminho de Evas�o[%d] )", lpPlayInfo->smCharInfo.szName, lpPlayInfo2->dwSiegeItem_Scroll_Param);
					rsDisplayDamgeToClient(lpPlayInfo2, szDispDamage);
				}
#endif
				//추가회피 정보 클라리언트에 출력
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.WParam = 0;
				smTransCommand.LParam = lpPlayInfo2->dwSiegeItem_Scroll_Code;
				smTransCommand.SParam = lpPlayInfo->dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo2->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
				return FALSE;
			}
		}
	}
	if (lpPlayInfo2->dwSiegeItem_Scroll_Code == (sinBI1 | sin07)) {	//추가 회피
		if ((rand() % 100) < (int)lpPlayInfo2->dwSiegeItem_Scroll_Param) {
#ifdef DISP_DAMAGE
			if (lpPlayInfo2->AdminMode > 2) {
				wsprintf(szDispDamage, "> %s Esquiva adicional ( Pergaminho de Evas�o [%d] )", lpPlayInfo->smCharInfo.szName, lpPlayInfo2->dwSiegeItem_Scroll_Param);
				rsDisplayDamgeToClient(lpPlayInfo2, szDispDamage);
			}
#endif
			//추가회피 정보 클라리언트에 출력
			smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = 0;
			smTransCommand.LParam = lpPlayInfo2->dwSiegeItem_Scroll_Code;
			smTransCommand.SParam = lpPlayInfo->dwObjectSerial;
			smTransCommand.EParam = 0;
			lpPlayInfo2->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
			return FALSE;
		}
	}

	if (lpPlayInfo->dwSiegeItem_Stone2_Code && lpPlayInfo2->smCharInfo.JOB_CODE == lpPlayInfo->dwSiegeItem_Stone2_Code) {
		//직업별 강화 스크롤
		TransAttackData.Power += (TransAttackData.Power * lpPlayInfo->dwSiegeItem_Stone2_Param) / 100;

#ifdef DISP_DAMAGE
		///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
		if (lpPlayInfo->AdminMode > 2) {
			wsprintf(szDispDamage, ">%s 공격력 강화석 ( 직업:%d )( %d )", lpPlayInfo2->smCharInfo.szName, lpPlayInfo->dwSiegeItem_Stone2_Code, TransAttackData.Power);
			rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
		}
#endif
	}


	if (lpPlayInfo2->dwSkill_PhysicalAbsorb_Time) {	//공격력 감소 (흡수력 미리적용)- 피지컬 사용
		if (lpPlayInfo2->dwSkill_PhysicalAbsorb_Time > dwPlayServTime) {
			TransAttackData.Power -= lpPlayInfo2->dwSkill_PhysicalAbsorb_Param;
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo2->dwSkill_PhysicalAbsorb_Time = 0;
			lpPlayInfo2->dwSkill_PhysicalAbsorb_Param = 0;
		}
	}
	if (lpPlayInfo2->dwSkill_ExtreamShield_Time) {	//블럭율증가 - 익스트림 쉴드
		if (lpPlayInfo2->dwSkill_ExtreamShield_Time > dwPlayServTime) {
			TransAttackData.AttackState |= (lpPlayInfo2->dwSkill_ExtreamShield_Param << 16);
		}
		else {
			lpPlayInfo2->dwSkill_ExtreamShield_Time = 0;
			lpPlayInfo2->dwSkill_ExtreamShield_Param = 0;
		}
	}

	/////////////////////////////////////////////////////////////////////


	if (lpPlayInfo2->dwSkill_MetalArmor_Time) {	//공격력 감소 (흡수력 미리적용)- 메탈아머 사용
		if (lpPlayInfo2->dwSkill_MetalArmor_Time > dwPlayServTime) {
			TransAttackData.Power -= lpPlayInfo2->dwSkill_MetalArmor_Param;
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo2->dwSkill_MetalArmor_Time = 0;
			lpPlayInfo2->dwSkill_MetalArmor_Param = 0;
		}
	}


	if (lpPlayInfo2->dwSkill_VirtualLife_Time) {	//가상 생명력을 높여서 공격력 감소
		if (lpPlayInfo2->dwSkill_VirtualLife_Time > dwPlayServTime) {
			TransAttackData.Power -= (TransAttackData.Power * lpPlayInfo2->dwSkill_VirtualLife_Param) / 100;
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo2->dwSkill_VirtualLife_Time = 0;
			lpPlayInfo2->dwSkill_VirtualLife_Param = 0;
		}
	}

	if (lpPlayInfo2->dwSkill_EnergyShield_Time) {	//에너지쉴드 공격력 감소
		if (lpPlayInfo2->dwSkill_EnergyShield_Time > dwPlayServTime) {
			temp = (TransAttackData.Power * lpPlayInfo2->dwSkill_EnergyShield_Param) / 100;
			TransAttackData.Power -= temp;
			TransAttackData.AttackState |= (temp << 16);
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo2->dwSkill_EnergyShield_Time = 0;
			lpPlayInfo2->dwSkill_EnergyShield_Param = 0;
		}
	}

	if (lpPlayInfo2->dwSkill_Compulsion_Time) {	//Compulsion 흡수력 추가
		if (lpPlayInfo2->dwSkill_Compulsion_Time > dwPlayServTime) {
			TransAttackData.Power -= lpPlayInfo2->dwSkill_Compulsion_Param;
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo2->dwSkill_Compulsion_Time = 0;
			lpPlayInfo2->dwSkill_Compulsion_Param = 0;
		}
	}

	if (lpPlayInfo2->dwSkill_Berserker_Time) {	//버서커 흡수력 감소
		if (lpPlayInfo2->dwSkill_Berserker_Time > dwPlayServTime) {
			TransAttackData.Power -= Berserker_SubAbsorb[lpPlayInfo2->dwSkill_Berserker_Param];
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo2->dwSkill_Berserker_Time = 0;
			lpPlayInfo2->dwSkill_Berserker_Param = 0;
		}
	}


	//베이그 추가회피
	if (lpPlayInfo2->dwSkill_Vague_Time) {
		if (lpPlayInfo2->dwSkill_Vague_Time > dwPlayServTime) {
			if ((rand() % 100) < Vague_EvasionPercent[lpPlayInfo2->dwSkill_Vague_Param]) {

#ifdef DISP_DAMAGE
				if (lpPlayInfo2->AdminMode > 2) {
					wsprintf(szDispDamage, "> %s Evas�o Adicional ( Vague[%d] )", lpPlayInfo->smCharInfo.szName, Vague_EvasionPercent[lpPlayInfo2->dwSkill_Vague_Param]);
					rsDisplayDamgeToClient(lpPlayInfo2, szDispDamage);
				}
#endif

				//추가회피 정보 클라리언트에 출력
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.WParam = 0;
				smTransCommand.LParam = SKILL_PLAY_VAGUE;
				smTransCommand.SParam = lpPlayInfo->dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo2->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

				return FALSE;		//추가 회피 판정
			}
		}
		else {
			lpPlayInfo2->dwSkill_Vague_Time = 0;
			lpPlayInfo2->dwSkill_Vague_Param = 0;
		}
	}

	if (lpPlayInfo2->dwSkill_EvasionMastery_Time) {
		if (lpPlayInfo2->dwSkill_EvasionMastery_Time > dwPlayServTime) {
			if ((rand() % 100) < Evasion_Mastery_AddPercent[lpPlayInfo2->dwSkill_EvasionMastery_Param]) {

#ifdef DISP_DAMAGE
				if (lpPlayInfo2->AdminMode > 2) {
					wsprintf(szDispDamage, "> %s Evas�o Adicional ( Vague Phoenix[%d] )", lpPlayInfo->smCharInfo.szName, Evasion_Mastery_AddPercent[lpPlayInfo2->dwSkill_EvasionMastery_Param]);
					rsDisplayDamgeToClient(lpPlayInfo2, szDispDamage);
				}
#endif

				//추가회피 정보 클라리언트에 출력
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.WParam = 0;
				smTransCommand.LParam = SKILL_EVASION_MASTERY;
				smTransCommand.SParam = lpPlayInfo->dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo2->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

				return FALSE;		//추가 회피 판정
			}
		}
		else {
			lpPlayInfo2->dwSkill_EvasionMastery_Time = 0;
			lpPlayInfo2->dwSkill_EvasionMastery_Param = 0;
		}
	}

	if (lpPlayInfo2->dwSkill_GodlyShied_Time) {	//공격력 감소 (흡수력 미리적용)- 가들리쉴드사용
		if (lpPlayInfo2->dwSkill_GodlyShied_Time > dwPlayServTime) {
			TransAttackData.Power -= (TransAttackData.Power * Godly_Shield_AbsorbPercent[lpPlayInfo2->dwSkill_GodlyShied_Param]) / 100;
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo2->dwSkill_GodlyShied_Time = 0;
			lpPlayInfo2->dwSkill_GodlyShied_Param = 0;
		}
	}


	//홀오브발할라 추가회피
	if (lpPlayInfo2->dwSkill_HallOfValhalla_Time) {
		if (lpPlayInfo2->dwSkill_HallOfValhalla_Time > dwPlayServTime) {
			if ((rand() % 100) < (int)lpPlayInfo2->wSkill_HallOfValhalla_Param[0]) {
#ifdef DISP_DAMAGE
				if (lpPlayInfo2->AdminMode > 2) {
					wsprintf(szDispDamage, "> %s Evas�o Adicional ( Hall of Valhalla [%d] )", lpPlayInfo->smCharInfo.szName, lpPlayInfo2->wSkill_HallOfValhalla_Param[0]);
					rsDisplayDamgeToClient(lpPlayInfo2, szDispDamage);
				}
#endif
				//추가회피 정보 클라리언트에 출력
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.WParam = 0;
				smTransCommand.LParam = SKILL_PLAY_HALL_OF_VALHALLA;
				smTransCommand.SParam = lpPlayInfo->dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo2->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

				return FALSE;		//추가 회피 판정
			}
		}
		else {
			lpPlayInfo2->dwSkill_HallOfValhalla_Time = 0;
		}
	}

	if (lpPlayInfo2->dwSkill_SummonMuspell_Time) {	//서몬 무스펠
		if (lpPlayInfo2->dwSkill_SummonMuspell_Time > dwPlayServTime) {

			if ((rand() % 100) < Summon_Muspell_BlockPercent[lpPlayInfo2->dwSkill_SummonMuspell_Param]) {
#ifdef DISP_DAMAGE
				if (lpPlayInfo2->AdminMode > 2) {
					wsprintf(szDispDamage, "> %s Evas�o Adicional ( Summon Muspell [%d] )", lpPlayInfo->smCharInfo.szName, Summon_Muspell_BlockPercent[lpPlayInfo2->dwSkill_SummonMuspell_Param]);
					rsDisplayDamgeToClient(lpPlayInfo2, szDispDamage);
				}
#endif
				//추가회피 정보 클라리언트에 출력
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.WParam = 0;
				smTransCommand.LParam = SKILL_PLAY_SUMMON_MUSPELL;
				smTransCommand.SParam = lpPlayInfo->dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo2->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

				return FALSE;
			}
		}
		else {
			lpPlayInfo2->dwSkill_SummonMuspell_Time = 0;
			lpPlayInfo2->dwSkill_SummonMuspell_Param = 0;
		}
	}


	TransAttackData.sRating[0] = sParam1;
	TransAttackData.sRating[1] = sParam2;

	TransAttackData.dwDestObjectSerial = lpPlayInfo->dwObjectSerial;
	TransAttackData.dwTarObjectSerial = lpPlayInfo2->dwObjectSerial;
	TransAttackData.dwChkSum = TransAttackData.Power * 2002 +
		(TransAttackData.x * TransAttackData.y * TransAttackData.z);// 조작 방지 코드



	if (lpPlayInfo2->lpsmSock) {

		//공격 데이타 기록
		lpPlayInfo2->Send_AttackCount++;
		lpPlayInfo2->Send_AttackDamage += TransAttackData.Power;


		//캐릭터 PK 기록
		rsRecord_PKDamage(lpPlayInfo, lpPlayInfo2, TransAttackData.Power);

		if (rsServerConfig.CrazyPacketMode && lpPlayInfo2->dwDecPacketCode &&
			lpPlayInfo2->dwDecPacketTime2 < dwPlayServTime) {					//크레이지 패킷 모드

			//S2C 공격 첵크섬 ( 크레이지 패킷 )
			TransAttackData.code = smTRANSCODE_ATTACKDATA2;
			TransAttackData.dwDamageChkSum = dm_GetDamgeChkSum_S2V(&TransAttackData);		//S2C 공격 첵크섬

			if (lpPlayInfo2)
				rsEncodeDamagePacket(lpPlayInfo2, &TransAttackData);
		}
		else
			TransAttackData.dwDamageChkSum = dm_GetDamgeChkSum_S2V(&TransAttackData);		//S2C 공격 첵크섬

		lpPlayInfo2->lpsmSock->Send((char*)&TransAttackData, TransAttackData.size, TRUE);
	}

	return TRUE;
}


int RecvRangeAttack_User(rsPLAYINFO* lpPlayInfo, TRANS_SKIL_ATTACKDATA* lpTransSkilAttackData, int AreaNum, int PlaySkillCode)
{
	rsPLAYINFO* lpCharInfo;
	int cnt, cnt2, pow;
	int	rs, time;
	int	StunFlag;
	int	dDist;
	int	SkillCode, SkillPoint;
	int	StnPers;
	int	Dmg1, Dmg2, Dmg3, Dmg4;
	int SlowSpeed;

	SkillCode = PlaySkillCode & 0xFF;
	SkillPoint = PlaySkillCode >> 8;
	if (SkillPoint) {
		SkillPoint--;
		if (SkillPoint < 0 || SkillPoint >= 10) SkillPoint = 0;
	}


	STG_AREA* lpStgArea;

	if (AreaNum < STAGE_AREA_MAX && StageArea[AreaNum].lpStage) lpStgArea = &StageArea[AreaNum];
	else {
		return FALSE;
	}

	if (lpPlayInfo->HideMode && lpPlayInfo->AdminMode < 4) return FALSE;

	if (rsCheckAttackRange(lpTransSkilAttackData->x, lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpPlayInfo) == FALSE)
		return FALSE;

	dDist = lpTransSkilAttackData->AttackSize + 60;
	dDist *= dDist;


	switch (lpTransSkilAttackData->AttackState) {
	case 0:		//일반 범위형
	case 100:	//레이지 오브 제크람 ( 불 공격 )
	case 101:	//스파크 ( 뇌 공격 )
	case 103:	//디바인 라이팅 ( 언데드 50% 가중 )
	case 104:	//얼음 ( 공격 )
	case 105:	//인첸트 전기
	case 106:	//독 공격

		for (cnt2 = 0; cnt2 < CONNECTMAX; cnt2++) {

			lpCharInfo = &rsPlayInfo[cnt2];

			if (lpCharInfo && lpCharInfo->lpsmSock && lpCharInfo->smCharInfo.State) {
				for (cnt = 0; cnt < lpTransSkilAttackData->TargetCount; cnt++) {

					if (lpCharInfo->dwObjectSerial == lpTransSkilAttackData->dwTarObjectSerial[cnt]) {

						//거리검사
						//if ( rsCheckAttackRange( lpTransSkilAttackData->x,lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar , dDist )==FALSE ) break;

						pow = lpTransSkilAttackData->Power;
						StunFlag = TRUE;

						Dmg1 = pow;

						if (lpTransSkilAttackData->AttackState == 100) {
							//불 공격 ( 불 저항력 적용 )
							rs = lpCharInfo->smCharInfo.Resistance[sITEMINFO_FIRE];
							if (SkillCode == SKILL_PLAY_METEO) {
								rs /= 2;		//미티어 불속성 50% 적용
							}
							if (rs) {
								if (rs >= 100) rs = 100;
								if (rs <= -100) rs = -100;
								pow -= ((pow * rs) / 100);
							}
						}
						if (lpTransSkilAttackData->AttackState == 101) {
							//뇌 공격 ( 전기 저항력 적용 )
							rs = lpCharInfo->smCharInfo.Resistance[sITEMINFO_LIGHTING];
							if (rs) {
								if (rs >= 100) rs = 100;
								if (rs <= -100) rs = -100;
								pow -= ((pow * rs) / 100);
							}
						}
						if (lpTransSkilAttackData->AttackState == 105) {
							//뇌 공격 ( 전기 저항력 적용 ) - 인첸트
							rs = lpCharInfo->smCharInfo.Resistance[sITEMINFO_LIGHTING];
							if (rs) {
								if (rs >= 100) rs = 100;
								if (rs <= -100) rs = -100;
								pow -= ((pow * rs) / 100);
							}
							StunFlag = FALSE;
						}
						if (lpTransSkilAttackData->AttackState == 104) {
							//얼음 공격 ( 얼음 저항력 적용 )
							rs = lpCharInfo->smCharInfo.Resistance[sITEMINFO_ICE];
							if (rs) {
								if (rs >= 100) rs = 100;
								if (rs <= -100) rs = -100;
								pow -= ((pow * rs) / 100);
							}
						}

						if (lpTransSkilAttackData->AttackState == 106) {
							//독 공격 ( 베놈 스피어 )
							rs = lpCharInfo->smCharInfo.Resistance[sITEMINFO_POISON];
							if (rs) {
								if (rs >= 100) rs = 100;
								if (rs <= -100) rs = -100;
								pow -= ((pow * rs) / 100);
							}
						}

						StnPers = rand() % 100;

						Dmg2 = pow;

						if (lpTransSkilAttackData->AttackState == 103) {
							//디바인 라이팅 ( 언데드 50% 가중 )
							if (lpCharInfo->smCharInfo.Brood == smCHAR_MONSTER_UNDEAD) {
								pow += pow / 2;
							}
							//뇌 공격 ( 전기 저항력 적용 )
							rs = lpCharInfo->smCharInfo.Resistance[sITEMINFO_LIGHTING] / 10;
							if (rs) {
								if (rs >= 100) rs = 100;
								if (rs <= -100) rs = -100;
								pow -= ((pow * rs) / 100);
							}
						}


						switch (SkillCode) {
						case SKILL_PLAY_MECHANIC_BOMB:
							//메카닉범 ( 뮤턴트 메카닉 몬스터에게 50%추가 )
							if (lpCharInfo->smCharInfo.Brood == smCHAR_MONSTER_MUTANT ||
								lpCharInfo->smCharInfo.Brood == smCHAR_MONSTER_MECHANIC) {
								pow += pow / 2;
							}
							break;

						case SKILL_PLAY_SPARK:
							//스파크 ( 메카닉 몬스터에게 50% 추가 )
							if (lpCharInfo->smCharInfo.Brood == smCHAR_MONSTER_MECHANIC) {
								pow += pow / 2;
							}
							break;

						case SKILL_PLAY_EXPANSION:
							//익스팬션 ( 뮤턴트 몬스터에게 30% 추가 )
							if (lpCharInfo->smCharInfo.Brood == smCHAR_MONSTER_MUTANT) {
								pow += (pow * 30) / 100;
							}
							break;

						case SKILL_PLAY_SWORD_BLAST:
							//소드 블래스트 ( 노멀 , 뮤턴트 몬스터 30% 추가 )
							if (lpCharInfo->smCharInfo.Brood == smCHAR_MONSTER_NORMAL ||
								lpCharInfo->smCharInfo.Brood == smCHAR_MONSTER_MUTANT) {
								pow += (pow * 30) / 100;
							}
							break;

						case SKILL_PLAY_PIERCING:
							//피어싱 ( 뮤턴트 몬스터에게 30% 추가 )
							if (lpCharInfo->smCharInfo.Brood == smCHAR_MONSTER_MUTANT) {
								pow += (pow * 30) / 100;
							}
							break;

						case SKILL_PLAY_VENOM_SPEAR:
							//베놈 스피어 독적용
							//lpChar->PlayPoison[1] = VenomSpear_Time[SkillPoint]*17;
							//lpChar->PlayPoison[0] = GetRandomPos( VenomSpear_Damage[SkillPoint][0] , VenomSpear_Damage[SkillPoint][1] );
							break;

						case SKILL_PLAY_PERFORATION:			//퍼포레이션
							if ((rand() % 100) < GetCritical2(lpPlayInfo, lpCharInfo, dm_Critical_Temp)) {
								pow = (pow * 170) / 100;						//크리티컬 공격가중
#ifdef DISP_DAMAGE
///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
								if (lpPlayInfo->AdminMode > 2) {
									wsprintf(szDispDamage, ">%s 범위공격받음 ( Critical )", lpCharInfo->smCharInfo.szName);
									rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
								}
#endif

							}
							break;

						case SKILL_PLAY_LIGHTNING_JAVELIN:
							if (lpCharInfo->smCharInfo.Brood == smCHAR_MONSTER_UNDEAD) {
								pow += pow / 2;
							}
							break;

						case SKILL_PLAY_FLAME_BRANDISH:
							//StnPers = 100;
							break;

						case SKILL_PLAY_BOMB_SHOT:
							if (lpCharInfo->smCharInfo.Brood == smCHAR_MONSTER_DEMON) {
								pow += (pow * BombShot_DemonDamage[SkillPoint]) / 100;
							}
							break;

						case SKILL_PLAY_DIASTROPHISM:
							if (lpCharInfo->smCharInfo.wPlayClass[0] != MONSTER_CLASS_BOSS)
								StnPers = 0;
							break;

						case SKILL_PLAY_IMPULSION:
							// 메카닉 몬스터에게 30% 추가 
							if (lpCharInfo->smCharInfo.Brood == smCHAR_MONSTER_MECHANIC) {
								pow += (pow * 30) / 100;
							}
							break;

						case SKILL_PLAY_CYCLONE_STRIKE:
							if ((rand() % 100) < GetCritical2(lpPlayInfo, lpCharInfo, dm_Critical_Temp)) {
								pow = (pow * 170) / 100;						//크리티컬 공격가중
#ifdef DISP_DAMAGE
///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
								if (lpPlayInfo->AdminMode > 2) {
									wsprintf(szDispDamage, "> %s Ataques de estrelas ( Critical )", lpCharInfo->smCharInfo.szName);
									rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
								}
#endif
							}
							break;

							/*case SKILL_PLAY_CHAIN_LIGHTNING:
								if (lpCharInfo->smCharInfo.wPlayClass[0] != MONSTER_CLASS_BOSS &&
									lpCharInfo->smCharInfo.Resistance[sITEMINFO_LIGHTING] <= 20)
									StnPers = 0;
								break;*/


						}



						Dmg3 = pow;

						if (pow > 0) {
							rsSendAttackUser(lpPlayInfo, lpCharInfo, pow / PK_POWER_DIVIDE, 0x80);

							Dmg4 = pow;


#ifdef DISP_DAMAGE
							///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
							if (lpPlayInfo->AdminMode > 2) {
								wsprintf(szDispDamage, "> %s Ataques de estrelas ( %d %d %d %d )", lpCharInfo->smCharInfo.szName, Dmg1, Dmg2, Dmg3, Dmg4);
								rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
							}
#endif
						}
						break;
					}
				}
			}
		}



	case 2:	//로어
	case 5:	//쉴드스트라이크
		for (cnt2 = 0; cnt2 < CONNECTMAX; cnt2++) {

			lpCharInfo = &rsPlayInfo[cnt2];

			if (lpCharInfo && lpCharInfo->lpsmSock && lpCharInfo->smCharInfo.State) {
				for (cnt = 0; cnt < lpTransSkilAttackData->TargetCount; cnt++) {

					if (lpCharInfo->dwObjectSerial == lpTransSkilAttackData->dwTarObjectSerial[cnt]) {

						//거리검사
						//if ( rsCheckAttackRange( lpTransSkilAttackData->x,lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar , dDist )==FALSE ) break;

						//생속성 에 따르 중립상태 유지 
						rs = lpCharInfo->smCharInfo.Resistance[sITEMINFO_BIONIC];
						if (rs < 0) rs = 0;
						if (rs >= 100) rs = 100;
						rs = 100 - rs;

						if (lpTransSkilAttackData->AttackState == 2) {
							//로어
							//if ( (rand()%100)>rs ) {
								//lpChar->PlayStunCount = lpTransSkilAttackData->Power<<4;		//중립상태 설정
								//lpChar->PlayStunCount = ( lpChar->PlayStunCount * rs )/100;
							//}
							rsSendAttackUser(lpPlayInfo, lpCharInfo, 1, 0x80, -SKILL_PLAY_ROAR, (lpTransSkilAttackData->Power * rs) / 100);

						}
						else if (lpTransSkilAttackData->AttackState == 5) {
							//쉴드 스트라이크
							/*
							pow = ( lpTransSkilAttackData->Power * lpChar->smCharInfo.Absorption )/100;
							pow = lpTransSkilAttackData->Power-pow;
							if ( pow>0 ) {
								//명중 ( 공격 성공 )
								lpChar->smCharInfo.Life[0] -= pow;
								if ( lpChar->lpAttackDamageList ) rsRecordCharDamage( lpPlayInfo , lpChar , pow );	//받은공격 기억하는 몹

							}
							lpChar->PlayStunCount = 16*10;			//중립상태 10초
							lpChar->PlayStunCount = ( lpChar->PlayStunCount * rs )/100;
							*/
							rsSendAttackUser(lpPlayInfo, lpCharInfo, lpTransSkilAttackData->Power / PK_POWER_DIVIDE, 0x80, -SKILL_PLAY_SHIELD_STRIKE, (10 * rs) / 100);


#ifdef DISP_DAMAGE
							///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
							if (lpPlayInfo->AdminMode > 2) {
								wsprintf(szDispDamage, ">%s 범위공격받음 ( %d %d )", lpCharInfo->smCharInfo.szName, lpTransSkilAttackData->Power, pow);
								rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
							}
#endif
						}
						break;
					}
				}
			}
		}
		break;

	case 3:
		if (SkillCode == SKILL_PLAY_GLACIAL_SPIKE) {
			//글래셜 스파이크
			time = 8;
			SlowSpeed = 200;
		}
		else if (SkillCode == SKILL_PLAY_CHAIN_LIGHTNING)
		{
			time = 1;
			SlowSpeed = 80;
		}
		else {
			//그라운드 파이크
			time = 10;
			SlowSpeed = 128;
			cnt2 = lpTransSkilAttackData->AttackSize >> FLOATNS;
			for (cnt = 0; cnt < 10; cnt++) {
				//그라운드 파이크 시간을 찾는다
				if (G_Pike_Range[cnt] == cnt2) {
					time = G_Pike_Time[cnt];
					break;
				}
			}
		}

		for (cnt2 = 0; cnt2 < CONNECTMAX; cnt2++) {
			lpCharInfo = &rsPlayInfo[cnt2];

			if (lpCharInfo && lpCharInfo->lpsmSock && lpCharInfo->smCharInfo.State) {
				for (cnt = 0; cnt < lpTransSkilAttackData->TargetCount; cnt++) {

					if (lpCharInfo->dwObjectSerial == lpTransSkilAttackData->dwTarObjectSerial[cnt]) {

						//거리검사
						//if ( rsCheckAttackRange( lpTransSkilAttackData->x,lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar , dDist )==FALSE ) break;

						pow = lpTransSkilAttackData->Power;
						Dmg1 = pow;
						rs = lpCharInfo->smCharInfo.Resistance[sITEMINFO_ICE];
						if (rs) {
							if (rs >= 100) rs = 100;
							if (rs <= -100) rs = -100;
							pow -= ((pow * rs) / 100);
						}
						Dmg2 = pow;

						/*
													lpChar->PlaySlowCount = time;		//속도 느려짐
													lpChar->PlaySlowSpeed = SlowSpeed;

													lpChar->PlayStunCount = 0;			//중립모드 해제

													if ( rs ) {
														if ( rs<0 ) rs = 0;
														lpChar->PlaySlowCount -= (rs*time)/100;
														if ( lpChar->PlaySlowCount<0 ) lpChar->PlaySlowCount = 0;
													}

													//if ( (10-rs/10)
													//10-(몬스터속성/10) 초
						*/
						if (pow > 0) {
							//명중 ( 공격 성공 )
							rsSendAttackUser(lpPlayInfo, lpCharInfo, pow / PK_POWER_DIVIDE, 0x80, -SkillCode, time);

#ifdef DISP_DAMAGE
							///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
							if (lpPlayInfo->AdminMode > 2) {
								wsprintf(szDispDamage, ">%s 범위공격받음 ( %d %d )", lpCharInfo->smCharInfo.szName, Dmg1, Dmg2);
								rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
							}
#endif
						}
						//}

						break;
					}
				}
			}
		}
		break;

	case 7:
		//디스토션
//	int	rs,time;
		time = Distortion_Time[SkillPoint];
		rs = 240 - (240 * Distortion_SpeedSubPercent[SkillPoint]) / 100;

		for (cnt2 = 0; cnt2 < CONNECTMAX; cnt2++) {
			lpCharInfo = &rsPlayInfo[cnt2];

			if (lpCharInfo && lpCharInfo->lpsmSock && lpCharInfo->smCharInfo.State) {
				for (cnt = 0; cnt < lpTransSkilAttackData->TargetCount; cnt++) {

					if (lpCharInfo->dwObjectSerial == lpTransSkilAttackData->dwTarObjectSerial[cnt]) {
						/*
													lpChar->PlaySlowCount = time;		//속도 느려짐
													lpChar->PlaySlowSpeed = rs;
													lpChar->PlayDistortion = time;

													lpChar->PlayStunCount = 0;			//중립모드 해제
						*/
						rsSendAttackUser(lpPlayInfo, lpCharInfo, 1, 0x80, -SkillCode, time);

						break;
					}
				}
			}
		}

		break;

	}

	return TRUE;
}


int RecvRangeAttack(rsPLAYINFO* lpPlayInfo, TRANS_SKIL_ATTACKDATA* lpTransSkilAttackData, int AreaNum, int PlaySkillCode)
{

	smCHAR* lpChar;
	int cnt, cnt2, pow;
	int ang, ang2;
	int x, y, z, dist;
	int	rs, time;
	int life, TotalLife;
	int	StunFlag;
	smTRANS_COMMAND	smTransCommand;
	int	dDist;
	int	SkillCode, SkillPoint;
	int	StnPers;
	int	Dmg1, Dmg2, Dmg3, Dmg4;
	int SlowSpeed;

	TRANS_PARTY_SKILL		TransPartySkill;

	SkillCode = PlaySkillCode & 0xFF;
	SkillPoint = PlaySkillCode >> 8;
	if (SkillPoint) {
		SkillPoint--;
		if (SkillPoint < 0 || SkillPoint >= 10) SkillPoint = 0;
	}


	STG_AREA* lpStgArea;

	if (AreaNum < STAGE_AREA_MAX && StageArea[AreaNum].lpStage) lpStgArea = &StageArea[AreaNum];
	else {
		return FALSE;
	}

	if (lpPlayInfo->HideMode && lpPlayInfo->AdminMode < 4) return FALSE;

	//거리 검사 ( 플레이어와 목표지점 거리 )
	if (rsCheckAttackRange(lpTransSkilAttackData->x, lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpPlayInfo) == FALSE)
		return FALSE;

	dDist = lpTransSkilAttackData->AttackSize + 60;
	dDist *= dDist;



	switch (lpTransSkilAttackData->AttackState) {
	case 0:		//일반 범위형
	case 100:	//레이지 오브 제크람 ( 불 공격 )
	case 101:	//스파크 ( 뇌 공격 )
	case 103:	//디바인 라이팅 ( 언데드 50% 가중 )
	case 104:	//얼음 ( 공격 )
	case 105:	//인첸트 전기
	case 106:	//독 공격

		for (cnt2 = 0; cnt2 < STG_MONSTER_MAX; cnt2++) {
			lpChar = lpStgArea->lpCharMonster[cnt2];
			if (lpChar && lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State != CHRMOTION_STATE_DEAD) {
				for (cnt = 0; cnt < lpTransSkilAttackData->TargetCount; cnt++) {

					if (lpChar->dwObjectSerial == lpTransSkilAttackData->dwTarObjectSerial[cnt]) {

						if (!lpPlayInfo->lpChrTarget) lpPlayInfo->lpChrTarget = lpChar;		//우선 목표 캐릭으로 설정

						if (lpChar->smCharInfo.Brood == smCHAR_MONSTER_USER &&
							lpChar->HoSkillCode &&
							lpChar->lpMasterPlayInfo == lpPlayInfo) {
							continue;	//본인 소환몹은 데미지 없음
						}

						//거리검사
						if (rsCheckAttackRange(lpTransSkilAttackData->x, lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar, dDist) == FALSE) break;

						pow = lpTransSkilAttackData->Power;
						StunFlag = TRUE;

						Dmg1 = pow;

						if (lpTransSkilAttackData->AttackState == 100) {
							//불 공격 ( 불 저항력 적용 )
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_FIRE];
							if (SkillCode == SKILL_PLAY_METEO) {
								rs /= 2;		//미티어 불속성 50% 적용
							}
							if (rs) {
								if (rs >= 100) rs = 100;
								if (rs <= -100) rs = -100;
								pow -= ((pow * rs) / 100);
							}
						}
						if (lpTransSkilAttackData->AttackState == 101) {
							//뇌 공격 ( 전기 저항력 적용 )
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_LIGHTING];
							if (rs) {
								if (rs >= 100) rs = 100;
								if (rs <= -100) rs = -100;
								pow -= ((pow * rs) / 100);
							}
						}
						if (lpTransSkilAttackData->AttackState == 105) {
							//뇌 공격 ( 전기 저항력 적용 ) - 인첸트
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_LIGHTING];
							if (rs) {
								if (rs >= 100) rs = 100;
								if (rs <= -100) rs = -100;
								pow -= ((pow * rs) / 100);
							}
							StunFlag = FALSE;
						}
						if (lpTransSkilAttackData->AttackState == 104) {
							//얼음 공격 ( 얼음 저항력 적용 )
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_ICE];
							if (rs) {
								if (rs >= 100) rs = 100;
								if (rs <= -100) rs = -100;
								pow -= ((pow * rs) / 100);
							}
						}

						if (lpTransSkilAttackData->AttackState == 106) {
							//독 공격 ( 베놈 스피어 )
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_POISON];
							if (rs) {
								if (rs >= 100) rs = 100;
								if (rs <= -100) rs = -100;
								pow -= ((pow * rs) / 100);
							}
						}

						StnPers = rand() % 100;

						Dmg2 = pow;

						if (lpTransSkilAttackData->AttackState == 103) {
							//디바인 라이팅 ( 언데드 50% 가중 )
							if (lpChar->smCharInfo.Brood == smCHAR_MONSTER_UNDEAD) {
								pow += pow / 2;
							}
							//뇌 공격 ( 전기 저항력 적용 )
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_LIGHTING] / 10;
							if (rs) {
								if (rs >= 100) rs = 100;
								if (rs <= -100) rs = -100;
								pow -= ((pow * rs) / 100);
							}
						}


						switch (SkillCode) {
						case SKILL_PLAY_MECHANIC_BOMB:
							//메카닉범 ( 뮤턴트 메카닉 몬스터에게 50%추가 )
							if (lpChar->smCharInfo.Brood == smCHAR_MONSTER_MUTANT ||
								lpChar->smCharInfo.Brood == smCHAR_MONSTER_MECHANIC) {
								pow += pow / 2;
							}
							break;

						case SKILL_PLAY_SPARK:
							//스파크 ( 메카닉 몬스터에게 50% 추가 )
							if (lpChar->smCharInfo.Brood == smCHAR_MONSTER_MECHANIC) {
								pow += pow / 2;
							}
							break;

						case SKILL_PLAY_EXPANSION:
							//익스팬션 ( 뮤턴트 몬스터에게 30% 추가 )
							if (lpChar->smCharInfo.Brood == smCHAR_MONSTER_MUTANT) {
								pow += (pow * 30) / 100;
							}
							break;

						case SKILL_PLAY_SWORD_BLAST:
							//소드 블래스트 ( 노멀 , 뮤턴트 몬스터 30% 추가 )
							if (lpChar->smCharInfo.Brood == smCHAR_MONSTER_NORMAL ||
								lpChar->smCharInfo.Brood == smCHAR_MONSTER_MUTANT) {
								pow += (pow * 30) / 100;
							}
							break;

						case SKILL_PLAY_PIERCING:
							//피어싱 ( 뮤턴트 몬스터에게 30% 추가 )
							if (lpChar->smCharInfo.Brood == smCHAR_MONSTER_MUTANT) {
								pow += (pow * 30) / 100;
							}
							break;

						case SKILL_PLAY_VENOM_SPEAR:
							//베놈 스피어 독적용
							lpChar->PlayPoison[1] = VenomSpear_Time[SkillPoint] * 17;
							lpChar->PlayPoison[0] = GetRandomPos(VenomSpear_Damage[SkillPoint][0], VenomSpear_Damage[SkillPoint][1]);
							break;

						case SKILL_PLAY_PERFORATION:			//퍼포레이션
							if ((rand() % 100) < GetCritical(lpPlayInfo, lpChar, dm_Critical_Temp)) {
								pow = (pow * 170) / 100;						//크리티컬 공격가중
#ifdef DISP_DAMAGE
///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
								if (lpPlayInfo->AdminMode > 2) {
									wsprintf(szDispDamage, ">%s 범위공격받음 ( Critical )", lpChar->smCharInfo.szName);
									rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
								}
#endif

							}
							break;

						case SKILL_PLAY_LIGHTNING_JAVELIN:
							if (lpChar->smCharInfo.Brood == smCHAR_MONSTER_UNDEAD) {
								pow += pow / 2;
							}
							break;

						case SKILL_PLAY_FLAME_BRANDISH:
							//StnPers = 100;
							break;

						case SKILL_PLAY_BOMB_SHOT:
							if (lpChar->smCharInfo.Brood == smCHAR_MONSTER_DEMON) {
								pow += (pow * BombShot_DemonDamage[SkillPoint]) / 100;
							}
							break;

						case SKILL_PLAY_DIASTROPHISM:
							//lpChar->lpTargetPlayInfo = 0;
							//lpChar->dwNextMotionCode = CHRMOTION_STATE_STAND;
							if (lpChar->smCharInfo.wPlayClass[0] != MONSTER_CLASS_BOSS)
								StnPers = 0;

							if (lpChar->smCharInfo.dwCharSoundCode == snCHAR_SOUND_DEATHKNIGHT || lpChar->smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL || lpChar->smCharInfo.dwCharSoundCode == snCHAR_SOUND_KELVEZU || lpChar->smCharInfo.dwCharSoundCode == snCHAR_SOUND_MOKOVA)
								StnPers = 100;
							break;

						case SKILL_PLAY_IMPULSION:
							// 메카닉 몬스터에게 30% 추가 
							if (lpChar->smCharInfo.Brood == smCHAR_MONSTER_MECHANIC) {
								pow += (pow * 30) / 100;
							}
							break;

						case SKILL_PLAY_CYCLONE_STRIKE:
							if ((rand() % 100) < GetCritical(lpPlayInfo, lpChar, dm_Critical_Temp)) {
								pow = (pow * 170) / 100;						//크리티컬 공격가중
#ifdef DISP_DAMAGE
///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
								if (lpPlayInfo->AdminMode > 2) {
									wsprintf(szDispDamage, ">%s 범위공격받음 ( Critical )", lpChar->smCharInfo.szName);
									rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
								}
#endif
							}
							break;

							//case SKILL_PLAY_CHAIN_LIGHTNING:
							//	//lpChar->lpTargetPlayInfo = 0;
							//	//lpChar->dwNextMotionCode = CHRMOTION_STATE_STAND;
							//	if (lpChar->smCharInfo.wPlayClass[0] != MONSTER_CLASS_BOSS &&
							//		lpChar->smCharInfo.Resistance[sITEMINFO_LIGHTING] <= 20)
							//		StnPers = 0;
							//	break;


						}

						lpChar->PlayStunCount = 0;			//중립모드 해제

						Dmg3 = pow;

						pow -= (pow * lpChar->smCharInfo.Absorption) / 100;
						//pow = lpTransSkilAttackData->Power-pow;

						Dmg4 = pow;

						if (pow > 0) {
							//명중 ( 공격 성공 )
							lpChar->smCharInfo.Life[0] -= pow;
							SendShowDmg(lpChar, pow, 6, lpPlayInfo);
							if (lpChar->lpAttackDamageList || lpChar->lpAttackDamageList_BlessCastle || 1) rsRecordCharDamage(lpPlayInfo, lpChar, pow);	//받은공격 기억하는 몹

							if (StunFlag && lpChar->smCharInfo.Life[0] && StnPers < lpChar->smMonsterInfo.DamageStunPers) {
								//맞으면 뒤로 물러나게 함

								if (lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
									lpChar->MotionInfo->State != CHRMOTION_STATE_EAT) {

									ang2 = GetRadian2D(lpChar->pX, lpChar->pZ, lpPlayInfo->Position.x, lpPlayInfo->Position.z);
									ang = (ang2 + ANGLE_180) & ANGCLIP;
									lpChar->Angle.y = ang;
									lpChar->MoveAngle(10);
									lpChar->Angle.y = ang2;

									//4% 이상
									if (((lpChar->smCharInfo.Life[1] << 2) / 100) < lpTransSkilAttackData->Power || StnPers == 0) {
										lpChar->SetMotionFromCode(CHRMOTION_STATE_DAMAGE);
									}
								}
							}

#ifdef DISP_DAMAGE
							///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
							if (lpPlayInfo->AdminMode > 2) {
								wsprintf(szDispDamage, ">%s 범위공격받음 ( %d %d %d %d )", lpChar->smCharInfo.szName, Dmg1, Dmg2, Dmg3, Dmg4);
								rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
							}
#endif


						}


						if (lpChar->smCharInfo.Life[0] <= 0) {
							lpChar->SetMotionFromCode(CHRMOTION_STATE_DEAD);
							lpChar->lpExt2 = (void*)lpPlayInfo;
						}

						break;
					}
				}
			}
		}
		break;
	case 1:
		//파이크 윈드
		for (cnt2 = 0; cnt2 < STG_MONSTER_MAX; cnt2++) {
			lpChar = lpStgArea->lpCharMonster[cnt2];
			if (lpChar && lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State != CHRMOTION_STATE_DEAD) {
				for (cnt = 0; cnt < lpTransSkilAttackData->TargetCount; cnt++) {

					if (lpChar->dwObjectSerial == lpTransSkilAttackData->dwTarObjectSerial[cnt]) {

						//거리검사
						if (rsCheckAttackRange(lpTransSkilAttackData->x, lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar, dDist) == FALSE) break;

						x = (lpPlayInfo->Position.x - lpChar->pX) >> FLOATNS;
						y = (lpPlayInfo->Position.y - lpChar->pY) >> FLOATNS;
						z = (lpPlayInfo->Position.z - lpChar->pZ) >> FLOATNS;
						if (abs(y) > 100 || (x * x + z * z) > (800 * 800)) break;		//거리가 먼경우 실패

						dist = (int)sqrt(x * x + z * z);
						dist = lpTransSkilAttackData->AttackSize - dist;



						if (lpChar->smCharInfo.dwCharSoundCode != snCHAR_SOUND_DEATHKNIGHT && lpChar->smCharInfo.dwCharSoundCode != snCHAR_SOUND_BABEL && lpChar->smCharInfo.dwCharSoundCode != snCHAR_SOUND_KELVEZU && lpChar->smCharInfo.dwCharSoundCode != snCHAR_SOUND_MOKOVA)
						{
							if (lpChar->smCharInfo.Life[0]) {
								//뒤로 물러나게 함
								ang2 = GetRadian2D(lpChar->pX, lpChar->pZ, lpPlayInfo->Position.x, lpPlayInfo->Position.z);
								ang = (ang2 + ANGLE_180) & ANGCLIP;
								lpChar->Angle.y = ang;
								lpChar->OverLapPosi2 = FALSE;
								lpChar->MoveAngle(dist);
								lpChar->Angle.y = ang2;

								if (lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
									lpChar->MotionInfo->State != CHRMOTION_STATE_EAT) {
									//4% 이상
									if (((lpChar->smCharInfo.Life[1] << 2) / 100) < lpTransSkilAttackData->Power) {
										lpChar->SetMotionFromCode(CHRMOTION_STATE_DAMAGE);
									}
								}
								lpChar->lpTargetPlayInfo = 0;
								lpChar->dwNextMotionCode = CHRMOTION_STATE_STAND;
							}
						}

						pow = (lpTransSkilAttackData->Power * lpChar->smCharInfo.Absorption) / 100;
						pow = lpTransSkilAttackData->Power - pow;

						lpChar->PlayStunCount = 0;			//중립모드 해제

						if (pow > 0) {
							//명중 ( 공격 성공 )
							lpChar->smCharInfo.Life[0] -= pow;
							SendShowDmg(lpChar, pow, 6, lpPlayInfo);
						}

						if (lpChar->smCharInfo.Life[0] <= 0) {
							lpChar->SetMotionFromCode(CHRMOTION_STATE_DEAD);
							lpChar->lpExt2 = (void*)lpPlayInfo;
						}

						break;

					}
				}
			}
		}
		break;
	case 2:	//로어
	case 5:	//쉴드스트라이크
	case 201:	//Compulsion
		for (cnt2 = 0; cnt2 < STG_MONSTER_MAX; cnt2++) {
			lpChar = lpStgArea->lpCharMonster[cnt2];
			if (lpChar && lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State != CHRMOTION_STATE_DEAD) {
				for (cnt = 0; cnt < lpTransSkilAttackData->TargetCount; cnt++) {

					if (lpChar->dwObjectSerial == lpTransSkilAttackData->dwTarObjectSerial[cnt]) {

						//거리검사
						if (rsCheckAttackRange(lpTransSkilAttackData->x, lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar, dDist) == FALSE) break;

						//생속성 에 따르 중립상태 유지 
						rs = lpChar->smCharInfo.Resistance[sITEMINFO_BIONIC];
						if (rs < 0) rs = 0;
						if (rs >= 100) rs = 100;
						rs = 100 - rs;

						if (lpTransSkilAttackData->AttackState == 2) {
							//로어
							//if ( (rand()%100)>rs ) {
							lpChar->PlayStunCount = lpTransSkilAttackData->Power << 4;		//중립상태 설정
							lpChar->PlayStunCount = (lpChar->PlayStunCount * rs) / 100;
							//}
						}
						else if (lpTransSkilAttackData->AttackState == 5) {
							//쉴드 스트라이크
							pow = (lpTransSkilAttackData->Power * lpChar->smCharInfo.Absorption) / 100;
							pow = lpTransSkilAttackData->Power - pow;
							if (pow > 0) {
								//명중 ( 공격 성공 )
								lpChar->smCharInfo.Life[0] -= pow;
								if (lpChar->lpAttackDamageList || lpChar->lpAttackDamageList_BlessCastle || 1) rsRecordCharDamage(lpPlayInfo, lpChar, pow);	//받은공격 기억하는 몹

							}
							lpChar->PlayStunCount = 16 * 10;			//중립상태 10초
							lpChar->PlayStunCount = (lpChar->PlayStunCount * rs) / 100;

#ifdef DISP_DAMAGE
							///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
							if (lpPlayInfo->AdminMode > 2) {
								wsprintf(szDispDamage, ">%s 범위공격받음 ( %d %d )", lpChar->smCharInfo.szName, lpTransSkilAttackData->Power, pow);
								rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
							}
#endif

						}
						else if (lpTransSkilAttackData->AttackState == 201) {	//Compulsion

							if (lpChar->smCharInfo.dwCharSoundCode != snCHAR_SOUND_DEATHKNIGHT && lpChar->smCharInfo.dwCharSoundCode != snCHAR_SOUND_BABEL && lpChar->smCharInfo.dwCharSoundCode != snCHAR_SOUND_KELVEZU && lpChar->smCharInfo.dwCharSoundCode != snCHAR_SOUND_MOKOVA)
							{
								lpChar->lpTargetPlayInfo = 0;
								lpChar->lpTargetChar = 0;

								cnt = Compulsion_Time[SkillPoint] * 1000;
								//cnt = 12*1000;

								lpChar->lpCompulsionPlayInfo = lpPlayInfo;
								lpChar->dwCompulsionTime = dwPlayServTime + cnt;

#ifdef DISP_DAMAGE
								///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
								if (lpPlayInfo->AdminMode > 2) {
									wsprintf(szDispDamage, ">%s 시선끌기 ( %d 초 )", lpChar->smCharInfo.szName, cnt / 1000);
									rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
								}
#endif
							}

						}

						if (lpChar->smCharInfo.Life[0] &&
							lpChar->smCharInfo.dwCharSoundCode != snCHAR_SOUND_BABEL &&
							lpChar->smCharInfo.dwCharSoundCode != snCHAR_SOUND_DEATHKNIGHT &&
							lpChar->smCharInfo.dwCharSoundCode != snCHAR_SOUND_KELVEZU &&
							lpChar->smCharInfo.dwCharSoundCode != snCHAR_SOUND_MOKOVA)
						{
							//맞으면 뒤로 물러나게 함
							ang2 = GetRadian2D(lpChar->pX, lpChar->pZ, lpPlayInfo->Position.x, lpPlayInfo->Position.z);
							ang = (ang2 + ANGLE_180) & ANGCLIP;
							lpChar->Angle.y = ang;
							lpChar->MoveAngle(10);
							lpChar->Angle.y = ang2;

							if (lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
								lpChar->MotionInfo->State != CHRMOTION_STATE_EAT) {
								lpChar->SetMotionFromCode(CHRMOTION_STATE_DAMAGE);
							}
						}

						if (lpChar->smCharInfo.Life[0] <= 0) {
							lpChar->SetMotionFromCode(CHRMOTION_STATE_DEAD);
							lpChar->lpExt2 = (void*)lpPlayInfo;
						}

						break;
					}
				}
			}
		}
		break;
	case 3:
		if (SkillCode == SKILL_PLAY_GLACIAL_SPIKE) {
			//글래셜 스파이크
			time = 8 * 17;
			SlowSpeed = 200;
		}
		else if (SkillCode == SKILL_PLAY_CHAIN_LIGHTNING) {
			time = 100;
			SlowSpeed = 80;
		}
		else {
			//그라운드 파이크
			time = 175;
			SlowSpeed = 0;
			cnt2 = lpTransSkilAttackData->AttackSize >> FLOATNS;
			for (cnt = 0; cnt < 10; cnt++) {
				//그라운드 파이크 시간을 찾는다
				if (G_Pike_Range[cnt] == cnt2) {
					time = G_Pike_Time[cnt] * 17;
					break;
				}
			}
		}

		for (cnt2 = 0; cnt2 < STG_MONSTER_MAX; cnt2++) {
			lpChar = lpStgArea->lpCharMonster[cnt2];
			if (lpChar && lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State != CHRMOTION_STATE_DEAD) {
				for (cnt = 0; cnt < lpTransSkilAttackData->TargetCount; cnt++) {

					if (lpChar->dwObjectSerial == lpTransSkilAttackData->dwTarObjectSerial[cnt]) {

						//거리검사
						if (rsCheckAttackRange(lpTransSkilAttackData->x, lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar, dDist) == FALSE) break;

						pow = lpTransSkilAttackData->Power;
						Dmg1 = pow;
						rs = lpChar->smCharInfo.Resistance[sITEMINFO_ICE];
						if (rs) {
							if (rs >= 100) rs = 100;
							if (rs <= -100) rs = -100;
							pow -= ((pow * rs) / 100);
						}
						Dmg2 = pow;
						pow -= (pow * lpChar->smCharInfo.Absorption) / 100;
						//pow = lpTransSkilAttackData->Power-pow;
						Dmg3 = pow;

						//int G_Pike_Time[10]	     = {3,4,5,6,7,8,9,10,11,12};


						lpChar->PlaySlowCount = time;		//속도 느려짐
						lpChar->PlaySlowSpeed = SlowSpeed;

						lpChar->PlayStunCount = 0;			//중립모드 해제
						lpChar->PlayDistortion = 0;			//디스토션 해제

						if (rs) {
							if (rs < 0) rs = 0;
							lpChar->PlaySlowCount -= (rs * time) / 100;
							if (lpChar->PlaySlowCount < 0) lpChar->PlaySlowCount = 0;
						}

						//if ( (10-rs/10)
						//10-(몬스터속성/10) 초

						if (pow > 0) {
							//명중 ( 공격 성공 )
							lpChar->smCharInfo.Life[0] -= pow;
							SendShowDmg(lpChar, pow, 6, lpPlayInfo);
							if (lpChar->lpAttackDamageList || lpChar->lpAttackDamageList_BlessCastle || 1) rsRecordCharDamage(lpPlayInfo, lpChar, pow);	//받은공격 기억하는 몹

							if (lpChar->smCharInfo.Life[0]) {
								//맞으면 뒤로 물러나게 함
								ang2 = GetRadian2D(lpChar->pX, lpChar->pZ, lpPlayInfo->Position.x, lpPlayInfo->Position.z);
								ang = (ang2 + ANGLE_180) & ANGCLIP;
								lpChar->Angle.y = ang;
								lpChar->MoveAngle(10);
								lpChar->Angle.y = ang2;

								if (lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
									lpChar->MotionInfo->State != CHRMOTION_STATE_EAT) {
									//4% 이상
									if (((lpChar->smCharInfo.Life[1] << 2) / 100) < lpTransSkilAttackData->Power) {
										lpChar->SetMotionFromCode(CHRMOTION_STATE_DAMAGE);
									}
								}
							}

#ifdef DISP_DAMAGE
							///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
							if (lpPlayInfo->AdminMode > 2) {
								wsprintf(szDispDamage, ">%s 범위공격받음 ( %d %d %d )", lpChar->smCharInfo.szName, Dmg1, Dmg2, Dmg3);
								rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
							}
#endif

						}

						if (lpChar->smCharInfo.Life[0] <= 0) {
							lpChar->SetMotionFromCode(CHRMOTION_STATE_DEAD);
							lpChar->lpExt2 = (void*)lpPlayInfo;
						}

						break;
					}
				}
			}
		}
		break;
	case 4:	//소울서커
		if (lpTransSkilAttackData->Power < 1 || lpTransSkilAttackData->Power>10) break;
		TotalLife = 0;

		for (cnt2 = 0; cnt2 < STG_MONSTER_MAX; cnt2++) {
			lpChar = lpStgArea->lpCharMonster[cnt2];
			if (lpChar && lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State != CHRMOTION_STATE_DEAD) {
				for (cnt = 0; cnt < lpTransSkilAttackData->TargetCount; cnt++) {

					if (lpChar->dwObjectSerial == lpTransSkilAttackData->dwTarObjectSerial[cnt] &&
						((lpChar->smCharInfo.Life[1] * 50) / 100) < lpChar->smCharInfo.Life[0] &&
						lpChar->smCharInfo.Brood != smCHAR_MONSTER_UNDEAD) {	//언데드 제외

						//거리검사
						if (rsCheckAttackRange(lpTransSkilAttackData->x, lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar, dDist) == FALSE) break;

						pow = Soul_Sucker_Absorb[lpTransSkilAttackData->Power - 1];

						rs = lpChar->smCharInfo.Resistance[sITEMINFO_BIONIC];
						if (rs) {
							if (rs >= 100) rs = 100;
							if (rs <= -100) rs = -100;
							pow -= ((pow * rs) / 100);
						}


						lpChar->PlayStunCount = 0;			//중립모드 해제

						if (pow > 0) {
							//명중 ( 공격 성공 )
							life = (lpChar->smCharInfo.Life[1] * pow) / 100;
							lpChar->smCharInfo.Life[0] -= life;
							SendShowDmg(lpChar, pow, 6, lpPlayInfo);
							if (lpChar->lpAttackDamageList || lpChar->lpAttackDamageList_BlessCastle || 1) rsRecordCharDamage(lpPlayInfo, lpChar, life);	//받은공격 기억하는 몹
							if (lpChar->smCharInfo.Life[0] < 0) life += lpChar->smCharInfo.Life[0];
							TotalLife += life;

							if (lpChar->smCharInfo.Life[0] && (rand() % 100) < lpChar->smMonsterInfo.DamageStunPers) {
								//맞으면 뒤로 물러나게 함
								ang2 = GetRadian2D(lpChar->pX, lpChar->pZ, lpPlayInfo->Position.x, lpPlayInfo->Position.z);
								ang = (ang2 + ANGLE_180) & ANGCLIP;
								lpChar->Angle.y = ang;
								lpChar->MoveAngle(10);
								lpChar->Angle.y = ang2;

								if (lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
									lpChar->MotionInfo->State != CHRMOTION_STATE_EAT) {

									lpChar->SetMotionFromCode(CHRMOTION_STATE_DAMAGE);
								}
							}
						}

						if (lpChar->smCharInfo.Life[0] <= 0) {
							lpChar->SetMotionFromCode(CHRMOTION_STATE_DEAD);
							lpChar->lpExt2 = (void*)lpPlayInfo;
						}

						break;
					}
				}
			}
		}
		if (TotalLife > 0) {
			//빼앗은 생명력을 보내준다
			smTransCommand.code = smTRANSCODE_VIRTURAL_POTION;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = TotalLife;
			smTransCommand.LParam = 0;
			smTransCommand.SParam = 0;
			smTransCommand.EParam = 0;
			if (lpPlayInfo->lpsmSock)
				lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
		}
		break;

	case 6:	// Extin豫o da PRS aqui
		if (lpTransSkilAttackData->Power < 1 || lpTransSkilAttackData->Power>10) break;
		TotalLife = 0;

		ZeroMemory(&TransPartySkill, sizeof(TRANS_PARTY_SKILL));

		for (cnt2 = 0; cnt2 < STG_MONSTER_MAX; cnt2++)
		{
			lpChar = lpStgArea->lpCharMonster[cnt2];

			if (lpChar && lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State != CHRMOTION_STATE_DEAD)
			{
				for (cnt = 0; cnt < lpTransSkilAttackData->TargetCount; cnt++)
				{
					if (lpChar->dwObjectSerial == lpTransSkilAttackData->dwTarObjectSerial[cnt] &&
						lpChar->smCharInfo.Brood == smCHAR_MONSTER_UNDEAD &&
						lpChar->smCharInfo.szName[0] &&
						!STRINGCOMPARE(lpChar->smCharInfo.szName, "Valento")) // N�o usa no valento
					{

						if (rsCheckAttackRange(lpTransSkilAttackData->x, lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar, dDist) == FALSE) break;

						pow = Extinction_Percent[lpTransSkilAttackData->Power - 1];
						rs = lpChar->smCharInfo.Resistance[sITEMINFO_BIONIC];
						if (rs) {
							if (rs >= 100)
								rs = 100;
							if (rs <= -100)
								rs = -100;
						}

						lpChar->PlayStunCount = 0;

						if ((rand() % 100) < pow) {

							if (TransPartySkill.PartyCount < 8)
								TransPartySkill.dwPartyUser[TransPartySkill.PartyCount++] = lpChar->dwObjectSerial;

							//명중 ( 공격 성공 )
							life = (lpChar->smCharInfo.Life[0] * Extinction_Amount[lpTransSkilAttackData->Power - 1]) / 100;
							lpChar->smCharInfo.Life[0] -= life;
							if (lpChar->lpAttackDamageList || lpChar->lpAttackDamageList_BlessCastle || 1) rsRecordCharDamage(lpPlayInfo, lpChar, life);	//받은공격 기억하는 몹
							if (lpChar->smCharInfo.Life[0] < 0) life += lpChar->smCharInfo.Life[0];
							TotalLife += life;

							if (lpChar->smCharInfo.Life[0] && (rand() % 100) < lpChar->smMonsterInfo.DamageStunPers) {
								//맞으면 뒤로 물러나게 함
								ang2 = GetRadian2D(lpChar->pX, lpChar->pZ, lpPlayInfo->Position.x, lpPlayInfo->Position.z);
								ang = (ang2 + ANGLE_180) & ANGCLIP;
								lpChar->Angle.y = ang;
								lpChar->MoveAngle(10);
								lpChar->Angle.y = ang2;

								if (lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
									lpChar->MotionInfo->State != CHRMOTION_STATE_EAT) {

									lpChar->SetMotionFromCode(CHRMOTION_STATE_DAMAGE);
								}
							}
						}

						if (lpChar->smCharInfo.Life[0] <= 0) {
							lpChar->SetMotionFromCode(CHRMOTION_STATE_DEAD);
							lpChar->lpExt2 = (void*)lpPlayInfo;
						}

						break;
					}
				}
			}
		}

		if (TransPartySkill.PartyCount > 0 && lpPlayInfo && lpPlayInfo->lpsmSock) {
			TransPartySkill.code = smTRANSCODE_PARTY_SKILL;
			TransPartySkill.size = sizeof(TRANS_PARTY_SKILL);
			TransPartySkill.dwSkillCode = SKILL_PLAY_EXTINCTION;
			rsSendCommandUser2(lpPlayInfo, (smTRANS_COMMAND*)&TransPartySkill);
		}

		break;
		/*
		int Distortion_SpeedSubPercent[10]  = {5,6,7,8,9,10,11,12,13,14};         //속도 저하
		int Distortion_DamageSubPercent[10] = {5,6,7,8,9,10,11,12,13,14};         //공격력 저하
		int Distortion_Area[10]             = {120,125,130,135,140,145,150,155,160,165};
		int Distortion_Time[10]             = {4,5,6,7,8,9,10,11,12,13};
		int Distortion_UseMana[10]          = {105,110,115,120,125,130,135,140,145,150};
		*/
	case 7:
		//디스토션
//	int	rs,time;
		time = Distortion_Time[SkillPoint] * 17;
		rs = 240 - (240 * Distortion_SpeedSubPercent[SkillPoint]) / 100;

		for (cnt2 = 0; cnt2 < STG_MONSTER_MAX; cnt2++) {
			lpChar = lpStgArea->lpCharMonster[cnt2];
			if (lpChar && lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State != CHRMOTION_STATE_DEAD) {
				for (cnt = 0; cnt < lpTransSkilAttackData->TargetCount; cnt++) {

					if (lpChar->dwObjectSerial == lpTransSkilAttackData->dwTarObjectSerial[cnt]) {

						//거리검사
						if (rsCheckAttackRange(lpTransSkilAttackData->x, lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar, dDist) == FALSE) break;


						lpChar->PlaySlowCount = time;		//속도 느려짐
						lpChar->PlaySlowSpeed = rs;
						lpChar->PlayDistortion = time;

						lpChar->PlayStunCount = 0;			//중립모드 해제


						if (lpChar->smCharInfo.Life[0]) {
							//맞으면 뒤로 물러나게 함
							ang2 = GetRadian2D(lpChar->pX, lpChar->pZ, lpPlayInfo->Position.x, lpPlayInfo->Position.z);
							ang = (ang2 + ANGLE_180) & ANGCLIP;
							lpChar->Angle.y = ang;
							lpChar->MoveAngle(10);
							lpChar->Angle.y = ang2;

							if (lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
								lpChar->MotionInfo->State != CHRMOTION_STATE_EAT) {
								//4% 이상
								if (((lpChar->smCharInfo.Life[1] << 2) / 100) < lpTransSkilAttackData->Power) {
									lpChar->SetMotionFromCode(CHRMOTION_STATE_DAMAGE);
								}
							}
						}

						if (lpChar->smCharInfo.Life[0] <= 0) {
							lpChar->SetMotionFromCode(CHRMOTION_STATE_DEAD);
							lpChar->lpExt2 = (void*)lpPlayInfo;
						}

						break;
					}
				}
			}
		}
		break;


	}

	return TRUE;
}


int RecvRangeAttack(rsPLAYINFO* lpPlayInfo, TRANS_SKIL_ATTACKDATA* lpTransSkilAttackData)
{
	/*
		smCHAR *lpChar;
		int cnt,cnt2,pow;
		int ang,ang2;
		int x,y,z,dist;
		int	rs,time;
		int life,TotalLife;
		int	StunFlag;
		smTRANS_COMMAND	smTransCommand;

		switch( lpTransSkilAttackData->AttackState ) {
		case 0:		//일반 범위형
		case 100:	//레이지 오브 제크람 ( 불 공격 )
		case 101:	//스파크 ( 뇌 공격 )
		case 103:	//디바인 라이팅 ( 언데드 50% 가중 )
		case 104:	//얼음 ( 공격 )
		case 105:	//인첸트 전기
			for(cnt2=0;cnt2<AUTOPLAYER_MAX;cnt2++) {
				lpChar = &chrAutoPlayer[cnt2];
				if ( lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
					for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {

						if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {

						x = (lpPlayInfo->Position.x-lpChar->pX)>>FLOATNS;
						y = (lpPlayInfo->Position.y-lpChar->pY)>>FLOATNS;
						z = (lpPlayInfo->Position.z-lpChar->pZ)>>FLOATNS;
						if ( abs(y)>100 || (x*x+z*z)>(800*800) ) break;		//거리가 먼경우 실패

							pow = lpTransSkilAttackData->Power;
							StunFlag = TRUE;

							if ( lpTransSkilAttackData->AttackState==100 ) {
								//불 공격 ( 불 저항력 적용 )
								rs = lpChar->smCharInfo.Resistance[sITEMINFO_FIRE];
								if ( rs ) {
									if ( rs>=100 ) rs=100;
									if ( rs<=-100 ) rs=-100;
									pow -= ((pow*rs)/100);
								}
							}
							if ( lpTransSkilAttackData->AttackState==101 ) {
								//뇌 공격 ( 전기 저항력 적용 )
								rs = lpChar->smCharInfo.Resistance[sITEMINFO_LIGHTING];
								if ( rs ) {
									if ( rs>=100 ) rs=100;
									if ( rs<=-100 ) rs=-100;
									pow -= ((pow*rs)/100);
								}
							}
							if ( lpTransSkilAttackData->AttackState==105 ) {
								//뇌 공격 ( 전기 저항력 적용 ) - 인첸트
								rs = lpChar->smCharInfo.Resistance[sITEMINFO_LIGHTING];
								if ( rs ) {
									if ( rs>=100 ) rs=100;
									if ( rs<=-100 ) rs=-100;
									pow -= ((pow*rs)/100);
								}
								StunFlag = FALSE;
							}

							if ( lpTransSkilAttackData->AttackState==104 ) {
								//얼음 공격 ( 얼음 저항력 적용 )
								rs = lpChar->smCharInfo.Resistance[sITEMINFO_ICE];
								if ( rs ) {
									if ( rs>=100 ) rs=100;
									if ( rs<=-100 ) rs=-100;
									pow -= ((pow*rs)/100);
								}
							}
							if ( lpTransSkilAttackData->AttackState==103 ) {
								//디바인 라이팅 ( 언데드 50% 가중 )
								if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_UNDEAD ) {
									pow += pow/2;
								}
								//뇌 공격 ( 전기 저항력 적용 )
								rs = lpChar->smCharInfo.Resistance[sITEMINFO_LIGHTING]/10;
								if ( rs ) {
									if ( rs>=100 ) rs=100;
									if ( rs<=-100 ) rs=-100;
									pow -= ((pow*rs)/100);
								}
							}


							pow -= ( pow * lpChar->smCharInfo.Absorption )/100;
							//pow = lpTransSkilAttackData->Power-pow;

							if ( pow>0 ) {
								//명중 ( 공격 성공 )
								lpChar->smCharInfo.Life[0] -= pow;

								if ( StunFlag && lpChar->smCharInfo.Life[0] && (rand()%100)<lpChar->smMonsterInfo.DamageStunPers ) {
									//맞으면 뒤로 물러나게 함
									//ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpTransSkilAttackData->x, lpTransSkilAttackData->z );
									ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
									ang = (ang2+ANGLE_180) & ANGCLIP;
									lpChar->Angle.y = ang;
									lpChar->MoveAngle( 10 );
									lpChar->Angle.y = ang2;

									if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
										lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {
											//4% 이상
											if ( ((lpChar->smCharInfo.Life[1]<<2)/100)<lpTransSkilAttackData->Power ) {
												lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
											}
									}
								}
							}


							if ( lpChar->smCharInfo.Life[0]<=0 ) {
								lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
								lpChar->lpExt2 = (void *)lpPlayInfo;
							}

							break;
						}
					}
				}
			}
			break;
		case 1:
			//파이크 윈드
			for(cnt2=0;cnt2<AUTOPLAYER_MAX;cnt2++) {
				lpChar = &chrAutoPlayer[cnt2];
				if ( lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
					for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {

						if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {

							x = (lpPlayInfo->Position.x-lpChar->pX)>>FLOATNS;
							y = (lpPlayInfo->Position.y-lpChar->pY)>>FLOATNS;
							z = (lpPlayInfo->Position.z-lpChar->pZ)>>FLOATNS;
							if ( abs(y)>100 || (x*x+z*z)>(800*800) ) break;		//거리가 먼경우 실패

							//x = ( lpChar->pX-lpPlayInfo->Position.x )>>FLOATNS;
							//z = ( lpChar->pZ-lpPlayInfo->Position.z )>>FLOATNS;

							dist = (int)sqrt(x*x+z*z);
							dist = (lpTransSkilAttackData->AttackSize>>FLOATNS) - dist;


								if ( lpChar->smCharInfo.Life[0] ) {
									//뒤로 물러나게 함
									ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
									ang = (ang2+ANGLE_180) & ANGCLIP;
									lpChar->Angle.y = ang;
									lpChar->OverLapPosi2 = FALSE;
									lpChar->MoveAngle( dist );
									lpChar->Angle.y = ang2;

									if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
										lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {
											//4% 이상
											if ( ((lpChar->smCharInfo.Life[1]<<2)/100)<lpTransSkilAttackData->Power ) {
												lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
											}
									}
									lpChar->lpTargetPlayInfo = 0;
									lpChar->dwNextMotionCode = CHRMOTION_STATE_STAND;
								}

							pow = ( lpTransSkilAttackData->Power * lpChar->smCharInfo.Absorption )/100;
							pow = lpTransSkilAttackData->Power-pow;

							if ( pow>0 ) {
								//명중 ( 공격 성공 )
								lpChar->smCharInfo.Life[0] -= pow;
							}

							if ( lpChar->smCharInfo.Life[0]<=0 ) {
								lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
								lpChar->lpExt2 = (void *)lpPlayInfo;
							}

							break;

						}
					}
				}
			}
			break;
		case 2:	//로어
		case 5:	//쉴드스트라이크
			for(cnt2=0;cnt2<AUTOPLAYER_MAX;cnt2++) {
				lpChar = &chrAutoPlayer[cnt2];
				if ( lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
					for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {

						if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {

							x = (lpPlayInfo->Position.x-lpChar->pX)>>FLOATNS;
							y = (lpPlayInfo->Position.y-lpChar->pY)>>FLOATNS;
							z = (lpPlayInfo->Position.z-lpChar->pZ)>>FLOATNS;
							if ( abs(y)>100 || (x*x+z*z)>(800*800) ) break;		//거리가 먼경우 실패

							//생속성 에 따르 중립상태 유지
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_BIONIC];
							if ( rs<0 ) rs = 0;
							if ( rs>=100 ) rs=100;
							rs = 100-rs;

							if ( lpTransSkilAttackData->AttackState==2 ) {
								//로어
								//if ( (rand()%100)>rs ) {
									lpChar->PlayStunCount = lpTransSkilAttackData->Power<<4;		//중립상태 설정
									lpChar->PlayStunCount = ( lpChar->PlayStunCount * rs )/100;
								//}
							}
							else if ( lpTransSkilAttackData->AttackState==5 ) {
								//쉴드 스트라이크
								pow = ( lpTransSkilAttackData->Power * lpChar->smCharInfo.Absorption )/100;
								pow = lpTransSkilAttackData->Power-pow;
								if ( pow>0 ) {
									//명중 ( 공격 성공 )
									lpChar->smCharInfo.Life[0] -= pow;
								}
								lpChar->PlayStunCount = 16*10;			//중립상태 10초
								lpChar->PlayStunCount = ( lpChar->PlayStunCount * rs )/100;
							}


							if ( lpChar->smCharInfo.Life[0] ) {
								//맞으면 뒤로 물러나게 함
								ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
								ang = (ang2+ANGLE_180) & ANGCLIP;
								lpChar->Angle.y = ang;
								lpChar->MoveAngle( 10 );
								lpChar->Angle.y = ang2;

								if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
									lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {
									lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
								}
							}

							if ( lpChar->smCharInfo.Life[0]<=0 ) {
								lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
								lpChar->lpExt2 = (void *)lpPlayInfo;
							}

							break;
						}
					}
				}
			}
			break;
		case 3:
			//그라운드 파이크
			time = 175;
			cnt2 = lpTransSkilAttackData->AttackSize>>FLOATNS;
			for(cnt=0;cnt<10;cnt++) {
				//그라운드 파이크 시간을 찾는다
				if ( G_Pike_Range[cnt]==cnt2 ) {
					time = G_Pike_Time[cnt]*17;
					break;
				}
			}

			for(cnt2=0;cnt2<AUTOPLAYER_MAX;cnt2++) {
				lpChar = &chrAutoPlayer[cnt2];
				if ( lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
					for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {

						if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {

							x = (lpPlayInfo->Position.x-lpChar->pX)>>FLOATNS;
							y = (lpPlayInfo->Position.y-lpChar->pY)>>FLOATNS;
							z = (lpPlayInfo->Position.z-lpChar->pZ)>>FLOATNS;
							if ( abs(y)>100 || (x*x+z*z)>(800*800) ) break;		//거리가 먼경우 실패

								pow = lpTransSkilAttackData->Power;
								rs = lpChar->smCharInfo.Resistance[sITEMINFO_ICE];
								if ( rs ) {
									if ( rs>=100 ) rs=100;
									if ( rs<=-100 ) rs=-100;
									pow -= ((pow*rs)/100);
								}
								pow -= ( pow * lpChar->smCharInfo.Absorption )/100;
								//pow = lpTransSkilAttackData->Power-pow;



								//int G_Pike_Time[10]	     = {3,4,5,6,7,8,9,10,11,12};


								lpChar->PlaySlowCount = time;		//속도 느려짐
								lpChar->PlaySlowSpeed = 0;

								if ( rs ) {
									if ( rs<0 ) rs = 0;
									lpChar->PlaySlowCount -= (rs*time)/100;
									if ( lpChar->PlaySlowCount<0 ) lpChar->PlaySlowCount = 0;
								}

								//if ( (10-rs/10)
								//10-(몬스터속성/10) 초

								if ( pow>0 ) {
									//명중 ( 공격 성공 )
									lpChar->smCharInfo.Life[0] -= pow;

									if ( lpChar->smCharInfo.Life[0] ) {
										//맞으면 뒤로 물러나게 함
										ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
										ang = (ang2+ANGLE_180) & ANGCLIP;
										lpChar->Angle.y = ang;
										lpChar->MoveAngle( 10 );
										lpChar->Angle.y = ang2;

										if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
											lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {
												//4% 이상
												if ( ((lpChar->smCharInfo.Life[1]<<2)/100)<lpTransSkilAttackData->Power ) {
													lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
												}
										}
									}

							}

							if ( lpChar->smCharInfo.Life[0]<=0 ) {
								lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
								lpChar->lpExt2 = (void *)lpPlayInfo;
							}

							break;
						}
					}
				}
			}
			break;
		case 4:	//소울서커
			if ( lpTransSkilAttackData->Power<1 || lpTransSkilAttackData->Power>10 ) break;
			TotalLife = 0;

			for(cnt2=0;cnt2<AUTOPLAYER_MAX;cnt2++) {
				lpChar = &chrAutoPlayer[cnt2];
				if ( lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
					for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {

						if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] &&
							((lpChar->smCharInfo.Life[1]*60)/100)<lpChar->smCharInfo.Life[0] ) {

							x = (lpPlayInfo->Position.x-lpChar->pX)>>FLOATNS;
							y = (lpPlayInfo->Position.y-lpChar->pY)>>FLOATNS;
							z = (lpPlayInfo->Position.z-lpChar->pZ)>>FLOATNS;
							if ( abs(y)>100 || (x*x+z*z)>(800*800) ) break;		//거리가 먼경우 실패

							pow = Soul_Sucker_Absorb[ lpTransSkilAttackData->Power-1 ];

							rs = lpChar->smCharInfo.Resistance[sITEMINFO_BIONIC];
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}

							if ( pow>0 ) {
								//명중 ( 공격 성공 )
								life = (lpChar->smCharInfo.Life[1]*pow)/100;
								lpChar->smCharInfo.Life[0] -= life;
								if ( lpChar->smCharInfo.Life[0]<0 ) life+=lpChar->smCharInfo.Life[0];
								TotalLife += life;

								if ( lpChar->smCharInfo.Life[0] && (rand()%100)<lpChar->smMonsterInfo.DamageStunPers ) {
									//맞으면 뒤로 물러나게 함
									ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
									ang = (ang2+ANGLE_180) & ANGCLIP;
									lpChar->Angle.y = ang;
									lpChar->MoveAngle( 10 );
									lpChar->Angle.y = ang2;

									if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
										lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {

											lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
									}
								}
							}

							if ( lpChar->smCharInfo.Life[0]<=0 ) {
								lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
								lpChar->lpExt2 = (void *)lpPlayInfo;
							}

							break;
						}
					}
				}
			}
			if ( TotalLife>0 ) {
				//빼앗은 생명력을 보내준다
				smTransCommand.code = smTRANSCODE_VIRTURAL_POTION;
				smTransCommand.size = sizeof( smTRANS_COMMAND );
				smTransCommand.WParam = TotalLife;
				smTransCommand.LParam = 0;
				smTransCommand.SParam = 0;
				smTransCommand.EParam = 0;
				if ( lpPlayInfo->lpsmSock )
					lpPlayInfo->lpsmSock->Send( (char *)&smTransCommand , smTransCommand.size , TRUE );
			}
			break;


		}
	*/
	return TRUE;
}


//범위형 공격 받음 ( 유저 공격 )
int RecvRangeAttackUser(rsPLAYINFO* lpPlayInfo, TRANS_SKIL_ATTACKDATA* lpTransSkilAttackData)
{

	int cnt, cnt2;
	TRANS_ATTACKDATA	TransAttackData;

	if (!lpPlayInfo->AdminMode) {
		if (!rsServerConfig.Enable_PK) return FALSE;			//현재 일반유저는 PK금지
		if (lpPlayInfo->smCharInfo.Level <= LIMIT_PK_LEVEL) return FALSE;	//레벨 10이하 PK금지
	}

	if ((lpTransSkilAttackData->AttackState & 0xFFFF) == 2) //로어 PK공격 해당 없음
		return FALSE;


	for (cnt2 = 0; cnt2 < CONNECTMAX; cnt2++) {
		if (rsPlayInfo[cnt2].lpsmSock && rsPlayInfo[cnt2].dwObjectSerial) {
			for (cnt = 0; cnt < lpTransSkilAttackData->TargetCount; cnt++) {
				if (rsPlayInfo[cnt2].dwObjectSerial == lpTransSkilAttackData->dwTarObjectSerial[cnt] &&
					rsPlayInfo[cnt2].smCharInfo.Level > LIMIT_PK_LEVEL) {


					TransAttackData.code = smTRANSCODE_ATTACKDATA;
					TransAttackData.size = sizeof(TRANS_ATTACKDATA);

					TransAttackData.x = rsPlayInfo[cnt2].Position.x;
					TransAttackData.y = rsPlayInfo[cnt2].Position.y;
					TransAttackData.z = rsPlayInfo[cnt2].Position.z;


					TransAttackData.AttackState = 1;
					TransAttackData.AttackSize = 32 * fONE;
					TransAttackData.Power = lpTransSkilAttackData->Power;

					TransAttackData.dwDestObjectSerial = lpPlayInfo->dwObjectSerial;
					TransAttackData.dwTarObjectSerial = rsPlayInfo[cnt2].dwObjectSerial;
					TransAttackData.dwChkSum = TransAttackData.Power * 2002 +
						(TransAttackData.x * TransAttackData.y * TransAttackData.z);// 조작 방지 코드

					//공격 기록
					rsPlayInfo[cnt2].Send_AttackCount++;
					rsPlayInfo[cnt2].Send_AttackDamage += lpTransSkilAttackData->Power;

					rsPlayInfo[cnt2].lpsmSock->Send((char*)&TransAttackData, TransAttackData.size, TRUE);

					break;
				}
			}
		}
	}

	return TRUE;
}


///////////////////////////// 새로운 공격 패킷 //////////////////////////////////


//레벨로 정상적인 스킬 코드인지 검사
int rsCheckSkillLevel(rsPLAYINFO* lpPlayInfo, DWORD dwSkillCode)
{
	DWORD	dwCode = dwSkillCode & 0xFF;
	DWORD	dwCodeByte = dwSkillCode & 0xF;
	int		lv = lpPlayInfo->smCharInfo.Level;

	if (dwCode >= 0xC0) return TRUE;

	if (dwSkillCode == SKILL_PLAY_PIKEWIND || dwSkillCode == SKILL_PLAY_ENCHANT_WEAPON) return TRUE;

	if (dwCodeByte == 0 || dwCodeByte >= 0xD) {
		//4차스킬
		if (lv < 59) return FALSE;
	}
	else if (dwCodeByte >= 9) {
		//3차 스킬
		if (lv < 39) return FALSE;
	}
	else if (dwCodeByte >= 5) {
		//2차 스킬
		if (lv < 19) return FALSE;
	}
	else if (dwCodeByte >= 1) {
		//1차 스킬
		if (lv < 9) return FALSE;
	}

	return TRUE;
}



int rsSetSkillAttack(rsPLAYINFO* lpPlayInfo, smCHAR* lpChar, TRANS_ATTACKDATA* lpTransAttackData, TRANS_ATTACKDATA2* lpTransAttackData2)
{
	DWORD	dwSkillCode;
	int		Point;
	int		Param;
	int		pow, pow1, pow2;
	int		cnt;
	smTRANS_COMMAND	smTransCommand;
	smTRANS_COMMAND_EX	TransCommandEx;


	dwSkillCode = lpTransAttackData2->SkillCode & 0xFF;
	Point = ((lpTransAttackData2->SkillCode >> 8) & 0xF) - 1;
	Param = (lpTransAttackData2->SkillCode >> 12) & 0xF;

	if (Point < 0 || Point >= 10) return FALSE;

	dm_SkillCode_Temp = dwSkillCode;


	if (rsCheckSkillLevel(lpPlayInfo, dwSkillCode) == FALSE)
	{
		if (lpPlayInfo->WarningCount < 5)
		{

			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
			smTransCommand.WParam = 8850;
			smTransCommand.LParam = lpPlayInfo->smCharInfo.Level;
			smTransCommand.SParam = dwSkillCode;
			smTransCommand.EParam = Point;
			rsSendDataServer(lpPlayInfo->lpsmSock, &smTransCommand);
			lpPlayInfo->WarningCount++;
		}
		return FALSE;
	}

	switch (dwSkillCode)
	{
	case SKILL_PLAY_RAVING:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_FIGHTER) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * Raving_Damage[Point]) / 100;
		lpTransAttackData->AttackState = (lpTransAttackData->AttackState & 0xFFFF0000) + 2;
		break;

	case SKILL_PLAY_IMPACT:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_FIGHTER) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * Impact_Damage[Point]) / 100;
		lpTransAttackData->AttackState = (lpTransAttackData->AttackState & 0xFFFF0000) + 2;

		break;

	case SKILL_PLAY_CRITICAL_HIT:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PIKEMAN) return FALSE;

		lpTransAttackData2->Critical[0] += Critical_Hit_Critical[Point];
		break;

	case SKILL_PLAY_JUMPING_CRASH:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PIKEMAN) return FALSE;

		lpTransAttackData->AttackState = (lpTransAttackData->AttackState & 0xFFFF0000) + 2;
		lpTransAttackData->Power += (lpTransAttackData->Power * Jumping_Crash_Damage[Point]) / 100;

		if (lpChar && lpChar->smCharInfo.Brood == smCHAR_MONSTER_DEMON)
		{

			lpTransAttackData->Power += (lpTransAttackData->Power * 30) / 100;
		}
		break;

	case SKILL_PLAY_WIND_ARROW:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ARCHER) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * Wind_Arrow_Damage[Point]) / 100;

		if (lpChar && lpChar->smCharInfo.Brood == smCHAR_MONSTER_NORMAL)
		{

			lpTransAttackData->Power += (lpTransAttackData->Power * 30) / 100;
		}
		break;

	case SKILL_PLAY_PERFECT_AIM:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ARCHER) return FALSE;

		lpTransAttackData->Power += Perfect_Aim_Damage[Point];
		lpTransAttackData->Power += lpPlayInfo->smCharInfo.Level / Perfect_Aim_Damage_LV[Point];

		if (lpChar && lpChar->smCharInfo.Brood == smCHAR_MONSTER_DEMON)
		{

			lpTransAttackData->Power += (lpTransAttackData->Power * 30) / 100;
		}
		break;

	case SKILL_PLAY_AVALANCHE:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ARCHER) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * Avalanche_Damage[Point]) / 100;
		lpTransAttackData2->Critical[0] >>= 1;
		break;

	case SKILL_PLAY_TRIPLE_IMPACT:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_FIGHTER) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * T_Impact_Damage[Point]) / 100;
		if (lpTransAttackData2->MotionCount[0] == 2)
		{
			lpTransAttackData->AttackState = (lpTransAttackData->AttackState & 0xFFFF0000) + 2;
			lpTransAttackData2->Critical[0] = 0;
		}
		break;

	case SKILL_PLAY_BRUTAL_SWING:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_FIGHTER) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * B_Swing_Damage[Point]) / 100;
		lpTransAttackData2->Critical[0] += (lpTransAttackData2->Critical[0] * B_Swing_Critical[Point]) / 100;

		if (lpChar && lpChar->smCharInfo.Brood == smCHAR_MONSTER_DEMON)
		{

			lpTransAttackData->Power += (lpTransAttackData->Power * 40) / 100;
		}
		break;

	case SKILL_PLAY_RAGEOF_ZECRAM:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_FIGHTER) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * R_Zecram_Damage[Point]) / 100;
		lpTransAttackData->AttackState |= (sITEMINFO_FIRE + 1) << 16;
		break;

	case SKILL_PLAY_DOUBLE_CRASH:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_KNIGHT) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * D_Crash_Damage[Point]) / 100;
		lpTransAttackData2->Critical[0] += D_Crash_Critical[Point];
		break;

	case SKILL_PLAY_FARINA:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ATALANTA) return FALSE;

		break;

	case SKILL_PLAY_VIGOR_SPEAR:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ATALANTA) return FALSE;

		lpTransAttackData->Power += GetRandomPos(V_Spear_Damage[Point][0], V_Spear_Damage[Point][1]);
		lpTransAttackData->Power += lpPlayInfo->smCharInfo.Level / 2;
		break;

	case SKILL_PLAY_TWIST_JAVELIN:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ATALANTA) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * Twist_Javelin_Damage[Point]) / 100;

		if (lpChar && lpChar->smCharInfo.Brood == smCHAR_MONSTER_DEMON)
		{

			lpTransAttackData->Power += (lpTransAttackData->Power * 30) / 100;
		}
		break;

	case SKILL_PLAY_FIRE_JAVELIN:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ATALANTA) return FALSE;

		pow = GetRandomPos(Fire_Javelin_Damage[Point][0], Fire_Javelin_Damage[Point][1]);
		if (lpChar && lpChar->smCharInfo.Brood == smCHAR_MONSTER_MUTANT)
		{

			pow += (pow * 50) / 100;
		}

		lpTransAttackData->AttackState |= (sITEMINFO_FIRE + 1) << 16;
		lpTransAttackData->AttackState |= pow << (16 + 4);
		lpTransAttackData->Power += pow;
		lpTransAttackData->Power += lpPlayInfo->smCharInfo.Level / 3;
		break;

	case SKILL_PLAY_HOLY_BOLT:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PRIESTESS || !lpChar) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * HolyBolt_Damage[Point]) / 100;
		lpTransAttackData2->Critical[0] = 0;
		break;

	case SKILL_PLAY_MULTI_SPARK:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PRIESTESS) return FALSE;
		lpTransAttackData->Power += (lpTransAttackData->Power * M_Spark_Damage[Point] * Param) / 100;
		lpTransAttackData2->Critical[0] = 0;
		break;

	case SKILL_PLAY_FIRE_BOLT:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_MAGICIAN) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * FireBolt_Damage[Point]) / 100;
		lpTransAttackData->AttackState |= (sITEMINFO_FIRE + 1) << 16;
		lpTransAttackData2->Critical[0] = 0;
		break;

	case SKILL_PLAY_DEAD_RAY:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_MAGICIAN) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * Dead_Ray_Damage[Point]) / 100;

		lpTransAttackData->AttackState |= (sITEMINFO_NONE) << 16;
		lpTransAttackData2->Critical[0] = 0;
		break;

	case SKILL_PLAY_FALCON:
		if (lpPlayInfo->smCharInfo.JOB_CODE == JOBCODE_ARCHER)
		{

			if (!lpPlayInfo->dwSKill_Falcon_Time || lpPlayInfo->dwSKill_Falcon_Time < dwPlayServTime)
			{
				lpPlayInfo->dwSKill_Falcon_Time = 0;
				return FALSE;
			}

			lpTransAttackData->Power = GetRandomPos(lpPlayInfo->dwSKill_Falcon_Damage[0], lpPlayInfo->dwSKill_Falcon_Damage[1]);
			lpTransAttackData2->Critical[0] = 0;


			if (lpPlayInfo->dwSkill_ForceOfNature_Time)
			{
				if (lpPlayInfo->dwSkill_ForceOfNature_Time < dwPlayServTime)
				{
					lpPlayInfo->dwSkill_ForceOfNature_Time = 0;
				}
				else
				{
					lpTransAttackData->Power += lpPlayInfo->wSkill_ForceOfNature_Param[1];
				}
			}
			break;
		}
		return FALSE;

	case SKILL_PLAY_DANCING_SWORD:
		if (lpPlayInfo->smCharInfo.JOB_CODE == JOBCODE_MAGICIAN)
		{

			if (!lpPlayInfo->dwSkill_DancingSword_Time || lpPlayInfo->dwSkill_DancingSword_Time < dwPlayServTime)
			{
				lpPlayInfo->dwSkill_DancingSword_Time = 0;
				lpPlayInfo->dwSkill_DancingSword_Param = 0;
				return FALSE;
			}

			pow1 = lpPlayInfo->dwSkill_DancingSword_Param >> 16;

			if (lpPlayInfo->dwSkill_DancingSword_DelayTime)
			{
				if (lpPlayInfo->dwSkill_DancingSword_DelayTime > (lpTransAttackData2->dwTime + 1000))
				{

					return FALSE;
				}
			}

			lpPlayInfo->dwSkill_DancingSword_DelayTime = lpTransAttackData2->dwTime + Dancing_Sword_AttackDelay[pow1] * 900;

			if (lpTransAttackData2->Power[0] < lpTransAttackData2->Power2[0] || lpTransAttackData2->Power[1] < lpTransAttackData2->Power2[1])
			{

				return FALSE;
			}

			if ((lpPlayInfo->dwSkill_DancingSword_Param & 0xFF) == 0)
			{

				lpTransAttackData->Power = GetRandomPos(
					Dancing_Sword_IceDamage[pow1][0] + (lpTransAttackData2->Power2[0] / 3) + (lpTransAttackData2->Critical[1] / 8),
					Dancing_Sword_IceDamage[pow1][1] + (lpTransAttackData2->Power2[1] / 3) + (lpTransAttackData2->Critical[1] / 6));

				lpTransAttackData2->Critical[0] = 0;
				lpTransAttackData->AttackState |= (sITEMINFO_ICE + 1) << 16;
			}
			else
			{

				lpTransAttackData->Power = GetRandomPos(
					Dancing_Sword_FireDamage[pow1][0] + (lpTransAttackData2->Power2[0] / 3) + (lpTransAttackData2->Critical[1] / 8),
					Dancing_Sword_FireDamage[pow1][1] + (lpTransAttackData2->Power2[1] / 3) + (lpTransAttackData2->Critical[1] / 6));

				lpTransAttackData2->Critical[0] = 0;
				lpTransAttackData->AttackState |= (sITEMINFO_FIRE + 1) << 16;
			}
			break;
		}
		return FALSE;


	case SKILL_PLAY_HEALING:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PRIESTESS) return FALSE;

		if (lpPlayInfo->sLimitDamage[0] < lpTransAttackData2->Power2[0] || lpPlayInfo->sLimitDamage[1] < lpTransAttackData2->Power2[1])
		{

			return FALSE;
		}

		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_HEALING;



		smTransCommand.WParam = GetRandomPos(
			Healing_Heal[Point][0] + (lpTransAttackData2->Power2[0] / 3) + (lpTransAttackData2->Critical[1] / 8),
			Healing_Heal[Point][1] + (lpTransAttackData2->Power2[1] / 3) + (lpTransAttackData2->Critical[1] / 6));

		smTransCommand.LParam = 0;
		smTransCommand.SParam = lpTransAttackData2->dwTarObjectSerial;
		smTransCommand.EParam = lpPlayInfo->dwObjectSerial;
		rsPlayHealing(lpPlayInfo, &smTransCommand);
		return FALSE;



	case SKILL_PLAY_GREAT_HEALING:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PRIESTESS) return FALSE;

		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_GRAND_HEALING;
		smTransCommand.WParam = Point + 1;


		smTransCommand.LParam = GetRandomPos(
			Grand_Healing[Point][0] + (lpTransAttackData2->Critical[1] / 8) + (lpTransAttackData2->Power2[0] / 3),
			Grand_Healing[Point][1] + (lpTransAttackData2->Critical[1] / 6) + (lpTransAttackData2->Power2[1] / 3));



		smTransCommand.SParam = Grand_Healing_Range[Point];
		smTransCommand.EParam = lpPlayInfo->dwObjectSerial;
		rsPlayGrandHealing(lpPlayInfo, &smTransCommand);
		return FALSE;


	case SKILL_PLAY_SPARK_SHIELD:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_MECHANICIAN) return FALSE;
		if (!lpChar || !lpPlayInfo->dwSkill_SparkShield_Time || lpPlayInfo->dwSkill_SparkShield_Time < dwPlayServTime)
		{

			lpPlayInfo->dwSkill_SparkShield_Time = 0;
			return FALSE;
		}

		pow1 = lpPlayInfo->dwSkill_SparkShield_Param >> 16;
		pow2 = lpPlayInfo->dwSkill_SparkShield_Param & 0xFFFF;
		pow = GetRandomPos(pow1, pow2);
		if (lpChar && lpChar->smCharInfo.Brood == smCHAR_MONSTER_MECHANIC)
		{

			pow += (pow * 50) / 100;
		}

		lpTransAttackData->AttackState |= (sITEMINFO_LIGHTING + 1) << 16;
		lpTransAttackData->Power = pow;
		lpTransAttackData2->Critical[0] = 0;

		smTransCommand.code = smTRANSCODE_PROCESS_SKILL;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = SKILL_PLAY_SPARK_SHIELD;
		smTransCommand.LParam = lpPlayInfo->dwObjectSerial;
		smTransCommand.SParam = lpChar->dwObjectSerial;
		rsSendCommandUser2(lpPlayInfo, &smTransCommand);
		break;

	case SKILL_PLAY_GRAND_SMASH:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_MECHANICIAN) return FALSE;
		lpTransAttackData->Power += (lpTransAttackData->Power * Grand_Smash_Damage[Point]) / 100;
		break;

	case SKILL_PLAY_AVANGING_CRASH:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_FIGHTER) return FALSE;
		lpTransAttackData->Power += (lpTransAttackData->Power * A_Crash_Damage[Point]) / 100;
		if (lpTransAttackData2->Power2[0] > 0 && lpTransAttackData2->Power2[0] <= 10)
		{
			lpTransAttackData2->Critical[0] += B_Swing_Critical[lpTransAttackData2->Power2[0]] / 2;
		}
		break;

	case SKILL_PLAY_BONE_SMASH:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_FIGHTER) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * B_Crash_Damage[Point]) / 100;

		if (lpChar && lpChar->smCharInfo.Brood == smCHAR_MONSTER_DEMON)
		{

			lpTransAttackData->Power += (lpTransAttackData->Power * B_Crash_DemonDamage[Point]) / 100;
		}
		break;

	case SKILL_PLAY_CHAIN_LANCE:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PIKEMAN) return FALSE;
		lpTransAttackData->Power += (lpTransAttackData->Power * Chain_Lance_Damage[Point]) / 100;
		break;


	case SKILL_PLAY_ELEMENTAL_SHOT:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ARCHER) return FALSE;
		if (Param == 0)
		{
			lpTransAttackData2->Critical[0] = 0;
			pow = GetRandomPos(Elemental_Shot_Fire[Point][0], Elemental_Shot_Fire[Point][1]);
			lpTransAttackData->Power += pow;
			lpTransAttackData->AttackState |= (sITEMINFO_FIRE + 1) << 16;
			lpTransAttackData->AttackState |= pow << (16 + 4);
		}
		break;


	case SKILL_PLAY_HOLY_INCANTATION:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_KNIGHT) return FALSE;

		if (lpChar && lpChar->smCharInfo.Brood == smCHAR_MONSTER_UNDEAD && lpChar->smCharInfo.wPlayClass[0] != MONSTER_CLASS_BOSS) // Ajuste n�o funcionar em boss
		{
			// Retirado resist�cnia do mob na chance
			//pow = 0; lpChar->smCharInfo.Resistance[sITEMINFO_BIONIC];
			//if (pow < 0) pow = 0;
			//if (pow > 100) pow = 100;
			//pow = 100 - pow;
			//pow = (H_Incantation_Success[Point] * pow) / 100;

			pow = H_Incantation_Success[Point];

			if ((rand() % 100) < pow)
			{
				lpChar->lpMasterPlayInfo = lpPlayInfo;
				lpChar->smMonsterInfo.dwAutoCharCode = 0;
				lpChar->smCharInfo.bUpdateInfo[0]++;
				lpChar->PlayHolyIncantation[0] = (short)lpChar->smCharInfo.Brood;
				lpChar->PlayHolyIncantation[1] = H_Incantation_Time[Point] * 17;

				lpChar->smCharInfo.Life[0] = lpChar->smCharInfo.Life[1] = (lpChar->smCharInfo.Life[0] * H_Incantation_AddLife[Point]) / 100;

				lpChar->smCharInfo.Next_Exp = lpPlayInfo->dwObjectSerial;
				lpChar->smCharInfo.Brood = smCHAR_MONSTER_USER;
				strcpy_s(lpChar->smCharInfo.szModelName2 + 1, sizeof(lpChar->smCharInfo.szModelName2) - 1, lpPlayInfo->smCharInfo.szName);
				lpChar->lpTargetChar = 0;
				lpChar->lpTargetPlayInfo = 0;


				ZeroMemory(&TransCommandEx, sizeof(smTRANS_COMMAND_EX));
				TransCommandEx.WParam = smCOMMNAD_USER_HOLY_INCANTATION;
				TransCommandEx.LParam = lpChar->dwObjectSerial;
				TransCommandEx.SParam = H_Incantation_Time[Point];
				TransCommandEx.EParam = 0;
				rsSendCommandUser(lpPlayInfo, &TransCommandEx);
			}
		}

		return FALSE;

	case SKILL_PLAY_GRAND_CROSS:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_KNIGHT) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * Grand_Cross_Damage[Point]) / 100;

		if (lpChar && lpChar->smCharInfo.Brood == smCHAR_MONSTER_UNDEAD)
		{

			lpTransAttackData->Power += (lpTransAttackData->Power * Grand_Cross_UndeadDamage[Point]) / 100;
		}
		break;

	case SKILL_PLAY_SPLIT_JAVELIN:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ATALANTA) return FALSE;
		if (lpTransAttackData2->Power2[0] > Split_Javelin_AttackNum[Point]) return FALSE;

		lpTransAttackData->Power = 0;


		for (cnt = 0; cnt < lpTransAttackData2->Power2[0]; cnt++)
		{
			pow = GetRandomPos(lpTransAttackData2->Power[0], lpTransAttackData2->Power[1]);
			pow += (pow * Split_Javelin_Damage[Point]) / 100;
			lpTransAttackData->Power += pow;
		}
		lpTransAttackData2->Critical[0] = 0;
		break;

	case SKILL_PLAY_VIGOR_BALL:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PRIESTESS || !lpChar) return FALSE;
		if (lpTransAttackData2->Power[0] > lpTransAttackData2->Power2[0] && lpTransAttackData2->Power[1] > lpTransAttackData2->Power2[1])
		{

			lpTransAttackData->Power += (lpTransAttackData->Power * Vigor_Ball_Damage[Point]) / 100;
			lpTransAttackData2->Critical[0] = 0;
			break;
		}
		return FALSE;

	case SKILL_PLAY_PET_ATTACK:





		pow1 = 0;
		pow2 = 0;

		switch (Point)
		{
		case PET_TERRY:



			pow1 = lpPlayInfo->smCharInfo.Level / 2;
			pow2 = (lpPlayInfo->smCharInfo.Level * 95) / 100;
			lpTransAttackData->AttackState |= (sITEMINFO_FIRE + 1) << 16;
			break;

		case PET_NEPSYS:



			pow1 = lpPlayInfo->smCharInfo.Level / 2;
			pow2 = (lpPlayInfo->smCharInfo.Level * 95) / 100;
			lpTransAttackData->AttackState |= (sITEMINFO_ICE + 1) << 16;
			break;

		case PET_IO:



			pow1 = (lpPlayInfo->smCharInfo.Level * 80) / 100 + 1;
			pow2 = lpPlayInfo->smCharInfo.Level + (lpPlayInfo->smCharInfo.Level * 80) / 100 + 2;
			break;


		case PET_MUTE:



			pow1 = (lpPlayInfo->smCharInfo.Level) / 2;
			pow2 = lpPlayInfo->smCharInfo.Level + (lpPlayInfo->smCharInfo.Level * 95) / 100;
			break;
		}





		lpTransAttackData->Power = GetRandomPos(pow1, pow2);
		lpTransAttackData2->Critical[0] = 0;
		break;

	case SKILL_PLAY_PET_ATTACK2:





		if (!lpPlayInfo->dwTime_PrimeItem_PhenixPet || (dwPlayServTime - lpPlayInfo->dwPCBang_Pet_AttackTime) < 1000) return FALSE;

		lpPlayInfo->dwPCBang_Pet_AttackTime = dwPlayServTime;

		pow1 = 0;
		pow2 = 0;

		switch (Point)
		{
		case PCBANG_PET_BURSTP:
		case PCBANG_PET_BURSTS:
		case PCBANG_PET_BURSTL:
		case PCBANG_PET_BURSTD:
			pow1 = lpPlayInfo->smCharInfo.Level / 2 + 1;
			pow2 = lpPlayInfo->smCharInfo.Level + lpPlayInfo->smCharInfo.Level / 2 + 1;
			break;
		}

		lpTransAttackData->Power = GetRandomPos(pow1, pow2);
		lpTransAttackData2->Critical[0] = 0;
		break;




	case SKILL_PLAY_IMPULSION:
		if (lpPlayInfo->smCharInfo.JOB_CODE == JOBCODE_MECHANICIAN)
		{
			pow1 = Impulsion_LightingDamage[Point][0];
			pow2 = Impulsion_LightingDamage[Point][1];

			lpTransAttackData->Power = GetRandomPos(pow1, pow2);


			if (lpChar && lpChar->smCharInfo.Brood == smCHAR_MONSTER_MECHANIC)
			{
				lpTransAttackData->Power += (lpTransAttackData->Power * 30) / 100;
			}

			lpTransAttackData2->Critical[0] = 0;
			break;
		}
		return FALSE;


	case SKILL_PLAY_MAGNETIC_SPHERE:
		if (lpPlayInfo->smCharInfo.JOB_CODE == JOBCODE_MECHANICIAN)
		{
			if (!lpPlayInfo->dwSkill_MagneticSphere_Time || lpPlayInfo->dwSkill_MagneticSphere_Time < dwPlayServTime)
			{
				lpPlayInfo->dwSkill_MagneticSphere_Time = 0;
				return FALSE;
			}

			pow1 = Magnetic_Sphere_AddDamage[lpPlayInfo->dwSkill_MagneticSphere_Param][0];
			pow2 = Magnetic_Sphere_AddDamage[lpPlayInfo->dwSkill_MagneticSphere_Param][1];

			lpTransAttackData->Power = GetRandomPos(pow1, pow2);
			lpTransAttackData2->Critical[0] = 0;
			lpTransAttackData->AttackState |= (sITEMINFO_LIGHTING + 1) << 16;


			if (lpChar && lpChar->smCharInfo.Brood == smCHAR_MONSTER_MECHANIC)
			{
				lpTransAttackData->Power += (lpTransAttackData->Power * 30) / 100;
			}
			break;
		}
		return FALSE;


	case SKILL_PLAY_DESTROYER:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_FIGHTER) return FALSE;

		lpTransAttackData->AttackState = (lpTransAttackData->AttackState & 0xFFFF0000) + 2;
		lpTransAttackData->Power += (lpTransAttackData->Power * Destoryer_DamagePercent[Point]) / 100;

		lpTransAttackData2->Critical[0] += Destoryer_AddCritical[Point];

		if (lpChar && lpChar->smCharInfo.Brood == smCHAR_MONSTER_DEMON)
		{

			lpTransAttackData->Power += (lpTransAttackData->Power * 30) / 100;
		}
		break;

	case SKILL_PLAY_ASSASSIN_EYE:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PIKEMAN) return FALSE;

		lpPlayInfo->dwSkill_AssassinEye_Time = dwPlayServTime + Assassin_Eye_Time[Point] * 1000;
		lpPlayInfo->dwSkill_AssassinEye_Param = Point;

		if (lpChar)
		{
			lpChar->dwAssassinEyeTime = lpPlayInfo->dwSkill_AssassinEye_Time;
			lpChar->AssassinEyeParam = Assassin_Eye_M_SubCritical[Point];
		}
		return FALSE;

	case SKILL_PLAY_CHARGING_STRIKE:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PIKEMAN) return FALSE;

		if (!lpTransAttackData2->Power2[0])
			lpTransAttackData->Power += (lpTransAttackData->Power * Charging_Strike_CharingDamagePercent[Point]) / 100;
		else
			lpTransAttackData->Power += (lpTransAttackData->Power * Charging_Strike_DamagePercent[Point]) / 100;
		break;

	case SKILL_PLAY_SHADOW_MASTER:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PIKEMAN) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * Shadow_Master_DamagePercent[Point]) / 100;
		break;

	case SKILL_PLAY_PHOENIX_SHOT:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ARCHER) return FALSE;
		lpTransAttackData->Power += (lpTransAttackData->Power * Phoenix_Shot_DamagePercent[Point]) / 100;
		break;

	case SKILL_PLAY_DIVINE_PIERCING:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_KNIGHT) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * Divine_Piercing_DamagePercent[Point]) / 100;
		break;

	case SKILL_PLAY_VENGEANCE:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ATALANTA) return FALSE;

		lpTransAttackData->Power += (lpTransAttackData->Power * Vengeance_DamagePercent[Point]) / 100;
		break;

	case SKILL_PLAY_SUMMON_MUSPELL:
		if (lpPlayInfo->smCharInfo.JOB_CODE == JOBCODE_PRIESTESS)
		{
			if (!lpPlayInfo->dwSkill_SummonMuspell_Time || lpPlayInfo->dwSkill_SummonMuspell_Time < dwPlayServTime)
			{
				lpPlayInfo->dwSkill_SummonMuspell_Time = 0;
				return FALSE;
			}

			pow1 = Summon_Muspell_Damage[lpPlayInfo->dwSkill_SummonMuspell_Param][0];
			pow2 = Summon_Muspell_Damage[lpPlayInfo->dwSkill_SummonMuspell_Param][1];

			lpTransAttackData->Power = GetRandomPos(pow1, pow2);
			lpTransAttackData2->Critical[0] = 0;
			break;
		}
	}

	return TRUE;
}

//스킬 공격 변환
int rsSetSkillPlus(rsPLAYINFO* lpPlayInfo, smCHAR* lpChar, TRANS_ATTACKDATA* lpTransAttackData, TRANS_ATTACKDATA2* lpTransAttackData2)
{
	int pow;
	DWORD dwWeaponCode;
	smTRANS_COMMAND	smTransCommand;
	DWORD 	dwSkillCode = lpTransAttackData2->SkillCode & 0xFF;

	DWORD	dwAttSkillCode = 0;

	if (!lpPlayInfo->AdminMode)
		dwAttSkillCode = lpTransAttackData2->SkillCode;


	if (lpPlayInfo->dwSkill_Maximize_Time && !dwAttSkillCode) {
		//맥시마이즈 적용 ( 최대 공격력 무기 최대치 만큼 추가 )
		if (lpPlayInfo->dwSkill_Maximize_Time < dwPlayServTime) {
			lpPlayInfo->dwSkill_Maximize_Time = 0;
			lpPlayInfo->dwSkill_Maximize_Param = 0;
			return TRUE;
		}

		pow = lpTransAttackData2->Power[1] - lpTransAttackData2->Power2[1];
		if (pow >= 0 && pow < lpTransAttackData2->Power[1]) {
			lpTransAttackData2->Power[1] += (pow * lpPlayInfo->dwSkill_Maximize_Param) / 100;
			lpTransAttackData->Power = GetRandomPos(lpTransAttackData2->Power[0], lpTransAttackData2->Power[1]);
		}
	}

	if (lpPlayInfo->dwSkill_AutoMation_Time && !dwAttSkillCode) {
		//오토매이션 적용 ( 공격력을 무기 데미지 의 비율로 추가 )
		if (lpPlayInfo->dwSkill_AutoMation_Time < dwPlayServTime) {
			lpPlayInfo->dwSkill_AutoMation_Time = 0;
			lpPlayInfo->dwSkill_AutoMation_Param = 0;
			return TRUE;
		}

		dwWeaponCode = lpTransAttackData2->dwWeaponCode & sinITEM_MASK2;
		if (dwWeaponCode == sinWS1 || dwWeaponCode == sinWT1) {		//무기 활 , 재블린 계열만 적용
			pow = lpTransAttackData2->Power[1] - lpTransAttackData2->Power2[1];
			if (pow >= 0 && pow < lpTransAttackData2->Power[1])
				lpTransAttackData2->Power[1] += (pow * lpPlayInfo->dwSkill_AutoMation_Param) / 100;

			pow = lpTransAttackData2->Power[0] - lpTransAttackData2->Power2[0];
			if (pow >= 0 && pow < lpTransAttackData2->Power[0])
				lpTransAttackData2->Power[0] += (pow * lpPlayInfo->dwSkill_AutoMation_Param) / 100;

			lpTransAttackData->Power = GetRandomPos(lpTransAttackData2->Power[0], lpTransAttackData2->Power[1]);
		}
	}

	if (lpPlayInfo->dwSkill_TriumphOfValhalla_Time && !dwAttSkillCode) {
		//트라이엄프 발할라
		if (lpPlayInfo->dwSkill_TriumphOfValhalla_Time < dwPlayServTime) {
			lpPlayInfo->dwSkill_TriumphOfValhalla_Time = 0;
			lpPlayInfo->dwSkill_TriumphOfValhalla_Param = 0;
			return TRUE;
		}

		pow = GetRandomPos(lpTransAttackData2->Power[0], lpTransAttackData2->Power[1] + (short)lpPlayInfo->dwSkill_TriumphOfValhalla_Param);
		lpTransAttackData->Power = pow;
	}

	if (lpPlayInfo->dwSkill_HallOfValhalla_Time && !dwAttSkillCode) {
		//홀오브 발할라
		if (lpPlayInfo->dwSkill_HallOfValhalla_Time < dwPlayServTime) {
			lpPlayInfo->dwSkill_HallOfValhalla_Time = 0;
			return TRUE;
		}

		pow = GetRandomPos(lpTransAttackData2->Power[0], lpTransAttackData2->Power[1] + (short)lpPlayInfo->wSkill_HallOfValhalla_Param[1]);
		lpTransAttackData->Power = pow;
	}

	if (lpPlayInfo->dwSkill_Vanish_Time) {
		//배니쉬
		if (lpPlayInfo->dwSkill_Vanish_Time < dwPlayServTime) {
			lpPlayInfo->dwSkill_Vanish_Time = 0;
			lpPlayInfo->dwSkill_Vanish_Param = 0;
			return TRUE;
		}

		lpTransAttackData->Power += (lpTransAttackData->Power * Vanish_Damage[lpPlayInfo->dwSkill_Vanish_Param]) / 100;

		lpPlayInfo->dwSkill_Vanish_Time = 0;
		lpPlayInfo->dwSkill_Vanish_Param = 0;

		//배니쉬 해제하기
		smTransCommand.code = smTRANSCODE_PROCESS_SKILL;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = SKILL_PLAY_VANISH;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;
		lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
	}

	if (lpPlayInfo->dwSkill_Berserker_Time) {
		//버서커
		if (lpPlayInfo->dwSkill_Berserker_Time < dwPlayServTime) {
			lpPlayInfo->dwSkill_Berserker_Time = 0;
			lpPlayInfo->dwSkill_Berserker_Param = 0;
			return TRUE;
		}

		lpTransAttackData->Power += Berserker_AddAttack[lpPlayInfo->dwSkill_Berserker_Param];
	}

	if (lpPlayInfo->dwSkill_SpiritElemental_Time)
	{
		if (lpPlayInfo->dwSkill_SpiritElemental_Time > dwPlayServTime)
		{
			lpTransAttackData->Power += (lpTransAttackData->Power * lpPlayInfo->dwSkill_SpiritElemental_Param) / 100;
		}
		else
		{
			lpPlayInfo->dwSkill_SpiritElemental_Time = 0;
			lpPlayInfo->dwSkill_SpiritElemental_Param = 0;
		}
	}

	if (lpPlayInfo->dwSkill_ForceOfNature_Time) {
		//포스오브 네이쳐
		if (lpPlayInfo->dwSkill_ForceOfNature_Time < dwPlayServTime) {
			lpPlayInfo->dwSkill_ForceOfNature_Time = 0;
			return TRUE;
		}

		if (dwSkillCode != SKILL_PLAY_FALCON)
			lpTransAttackData->Power += lpPlayInfo->wSkill_ForceOfNature_Param[0];
	}


	if (lpPlayInfo->dwSkill_GodsBless_Time) {
		//갓블레스
		if (lpPlayInfo->dwSkill_GodsBless_Time < dwPlayServTime) {
			lpPlayInfo->dwSkill_GodsBless_Time = 0;
			lpPlayInfo->dwSkill_GodsBless_Param = 0;
			return TRUE;
		}

		lpTransAttackData->Power += lpPlayInfo->dwSkill_GodsBless_Param;
	}

	if (lpPlayInfo->dwSkill_FrostJavelin_Time && !dwAttSkillCode) {
		//프로스트재블린
		if (lpPlayInfo->dwSkill_FrostJavelin_Time < dwPlayServTime) {
			lpPlayInfo->dwSkill_FrostJavelin_Time = 0;
			lpPlayInfo->dwSkill_FrostJavelin_Param = 0;
			return TRUE;
		}

		dwWeaponCode = lpTransAttackData2->dwWeaponCode & sinITEM_MASK2;
		if (dwWeaponCode == sinWT1) {		//무기 재블린 계열만 적용
			pow = GetRandomPos(Frost_Javelin_IceAddDamage[lpPlayInfo->dwSkill_FrostJavelin_Param][0],
				Frost_Javelin_IceAddDamage[lpPlayInfo->dwSkill_FrostJavelin_Param][1]);

			lpTransAttackData->AttackState |= (sITEMINFO_ICE + 1) << 16;
			lpTransAttackData->AttackState |= pow << (16 + 4);		//전체중 얼음속성 데미지
			lpTransAttackData->Power += pow;
		}
	}

	if (lpPlayInfo->dwBlessSkill_Code == SKILL_PLAY_BLESS_DAMAGE) {	//클랜스킬 (데미지추가)
		if (lpPlayInfo->dwBlessSkill_Time > dwPlayServTime) {
			lpTransAttackData->Power += (lpTransAttackData->Power * lpPlayInfo->dwBlessSkill_Param) / 100;
		}
		else {
			lpPlayInfo->dwBlessSkill_Code = 0;
			lpPlayInfo->dwBlessSkill_Time = 0;
			return TRUE;
		}
	}

	//공성 스크롤
	if (lpPlayInfo->dwSiegeItem_Scroll_Time) {
		if (lpPlayInfo->dwSiegeItem_Scroll_Time < dwPlayServTime) {
			lpPlayInfo->dwSiegeItem_Scroll_Code = 0;
			lpPlayInfo->dwSiegeItem_Scroll_Time = 0;
		}
		else {
			switch (lpPlayInfo->dwSiegeItem_Scroll_Code) {
			case (sinBI1 | sin05):					//무적 스크롤
				lpTransAttackData->Power /= lpPlayInfo->dwSiegeItem_Scroll_Param;			//공격력 감소
				break;
			case (sinBI1 | sin06):					//크리티컬 스크롤
				lpTransAttackData2->Critical[0] += (short)lpPlayInfo->dwSiegeItem_Scroll_Param;		//크리티컬 2배 증가
				break;
			}

			if (lpPlayInfo->Position.Area == rsCASTLE_FIELD) {
				switch (lpPlayInfo->dwSiegeItem_Scroll_Code) {
				case (sinBC1 | sin01):					//무적 스크롤
					lpTransAttackData->Power /= lpPlayInfo->dwSiegeItem_Scroll_Param;			//공격력 감소
					break;
				case (sinBC1 | sin02):					//크리티컬 스크롤
					lpTransAttackData2->Critical[0] += (short)lpPlayInfo->dwSiegeItem_Scroll_Param;		//크리티컬 2배 증가
					break;
				}
			}
		}
	}


	return TRUE;
}


//실제 크리티컬 
int GetCritical(rsPLAYINFO* lpPlayInfo, smCHAR* lpChar, int Critical_Hit)
{
	int Result;

	Result = Critical_Hit + (((lpPlayInfo->smCharInfo.Level - lpChar->smCharInfo.Level) * 25) / 100);

	//어세신아이 스킬 크리티컬 강화
	/*
	if ( lpPlayInfo->dwSkill_AssassinEye_Time && Critical_Hit>0 ) {

		if ( lpPlayInfo->dwSkill_AssassinEye_Time>dwPlayServTime ) {
			Result += Assassin_Eye_AddCritical[ lpPlayInfo->dwSkill_AssassinEye_Param ];
		}
		else {
			lpPlayInfo->dwSkill_AssassinEye_Time = 0;
			lpPlayInfo->dwSkill_AssassinEye_Param = 0;
		}
	}
	*/
	if (lpChar->dwAssassinEyeTime && Critical_Hit > 0) {
		if (lpChar->dwAssassinEyeTime > dwPlayServTime) {
			Result -= lpChar->AssassinEyeParam;
		}
		else {
			lpChar->dwAssassinEyeTime = 0;
			lpChar->AssassinEyeParam = 0;
		}
	}



	if (Result > 70)
		Result = 70;

	return Result;
}

//실제 크리티컬 
int GetCritical2(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, int Critical_Hit)
{
	int Result;

	Result = Critical_Hit + (((lpPlayInfo->smCharInfo.Level - lpPlayInfo2->smCharInfo.Level) * 25) / 100);
	if (Result > 70)
		Result = 70;

	return Result;
}

//포스오브 파워업
int AddForceOrbPower(rsPLAYINFO* lpPlayInfo, smCHAR* lpChar, TRANS_ATTACKDATA* lpTransAttackData, DWORD dwSkillCode, int MinDamage, int MaxDamage)
{
	int damage = 0;
	int	sJobLevel;
	int	damage2;

	// 박재원 - 매직 포스, 빌링 매직 포스는 일반스킬과 단타에는 적용이 되지 않으므로 return FALSE
	if ((lpPlayInfo->dwForceOrb_Code & sinITEM_MASK3) > sin14) return FALSE;
	if (!lpPlayInfo->dwForceOrb_Time) return FALSE;
	if (lpPlayInfo->dwForceOrb_Time < dwPlayServTime) {
		lpPlayInfo->dwForceOrb_Time = 0;
		lpPlayInfo->dwForceOrb_Code = 0;
		lpPlayInfo->dwForceOrb_Damage = 0;
		return FALSE;
	}

	//일부 스킬 적용 금지
	switch (dwSkillCode & 0xFF)
	{
	case SKILL_PLAY_RAGEOF_ZECRAM:
	case SKILL_PLAY_SPARK_SHIELD:
	case SKILL_PLAY_FALCON:
	case SKILL_PLAY_DANCING_SWORD:
	case SKILL_PLAY_SUMMON_MUSPELL:	//서먼무스펠 에 포스 공격력 추가 버그를 일본측 요구로 수정 (2005년7월14일)
	case SKILL_PLAY_PET_ATTACK:
	case SKILL_PLAY_PET_ATTACK2:


		return FALSE;
	}

	damage = lpPlayInfo->dwForceOrb_Damage;

	// 박재원 : 빌링 포스 추가
	if (lpPlayInfo->dwForceOrb_Code == (sinFO1 | sin07) || lpPlayInfo->dwForceOrb_Code == (sinFO1 | sin08) || lpPlayInfo->dwForceOrb_Code == (sinFO1 | sin09) || lpPlayInfo->dwForceOrb_Code == (sinFO1 | sin10))
	{
		//머키 포스, 데비네 포스, 쎌레스토 포스, 미라지 포스 공격력 10%가중
		damage2 = (MaxDamage + MinDamage) / 2;
		if (damage2 > 0 && damage2 < 1000)
			damage += (damage2 * 10) / 100;
	}
	else if (lpPlayInfo->dwForceOrb_Code == (sinFO1 | sin11))
	{
		//인페르나 포스 15%가중
		damage2 = (MaxDamage + MinDamage) / 2;
		if (damage2 > 0 && damage2 < 1000)
			damage += (damage2 * 15) / 100; // 공격력 15%가중
	}
	else if (lpPlayInfo->dwForceOrb_Code >= (sinFO1 | sin12))
	{
		//이니그마 포스 20%가중
		damage2 = (MaxDamage + MinDamage) / 2;
		if (damage2 > 0 && damage2 < 1000)
			damage += (damage2 * 20) / 100;
	}

	if (dwSkillCode) {
		sJobLevel = dwSkillCode & 0xF;
		sJobLevel--;
		if (sJobLevel >= 0 && sJobLevel < 0x10) {
			sJobLevel >>= 2;
			switch (sJobLevel) {
			case 0:		//1차스킬
				if ((dwSkillCode & 0xFF) == SKILL_PLAY_MULTI_SPARK)
					damage = (damage * 50) / 100;
				else
					damage = (damage * 80) / 100;

				break;
			case 1:		//2차스킬
				damage = (damage * 90) / 100;
				break;
			case 2:		//3차스킬

				break;
			case 3:		//4차스킬
				damage = (damage * 150) / 100;
				break;
			}

		}
	}

	lpTransAttackData->Power += damage;

#ifdef DISP_DAMAGE
	///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
	if (lpPlayInfo->AdminMode > 2) {
		if (lpChar) {					//해외
			wsprintf(szDispDamage, ">%s Attack ( ForceOrb + %d ) (%d)", lpChar->smCharInfo.szName, damage, lpTransAttackData->Power);
			rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
		}
		else {							//해외
			wsprintf(szDispDamage, "> Attack ( ForceOrb + %d ) (%d)", damage, lpTransAttackData->Power);
			rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
		}
	}
#endif
	return TRUE;
}

// 박재원 - 매직 포스 추가, 빌링 매직 포스 추가(범위공격용 포스오브 파워업)
int AddRangeForceOrbPower(rsPLAYINFO* lpPlayInfo, smCHAR* lpChar, TRANS_SKIL_ATTACKDATA* lpTransSkillAttackData, DWORD dwSkillCode, int MinDamage, int MaxDamage)
{
	int damage = 0;
	int	sJobLevel;
	int	damage2;

	if ((lpPlayInfo->dwForceOrb_Code & sinITEM_MASK2) == sinFO1) // 박재원 - 일반 포스 적용중에 범위형 스킬을 사용하면 리턴시킨다.
	{
		if ((lpPlayInfo->dwForceOrb_Code & sinITEM_MASK3) < sin21)
			return FALSE;
	}
	if (!lpPlayInfo->dwForceOrb_Time) return FALSE;
	if (lpPlayInfo->dwForceOrb_Time < dwPlayServTime) {
		lpPlayInfo->dwForceOrb_Time = 0;
		lpPlayInfo->dwForceOrb_Code = 0;
		lpPlayInfo->dwForceOrb_Damage = 0;
		return FALSE;
	}

	//일부 스킬 적용 금지
	switch (dwSkillCode & 0xFF)
	{
		//	case SKILL_PLAY_RAGEOF_ZECRAM: // 박재원 - 레이지 오브 제크람 스킬은 매직 포스 때 적용시킨다.
	case SKILL_PLAY_SPARK_SHIELD:
	case SKILL_PLAY_FALCON:
	case SKILL_PLAY_DANCING_SWORD:
	case SKILL_PLAY_SUMMON_MUSPELL:	//서먼무스펠 에 포스 공격력 추가 버그를 일본측 요구로 수정 (2005년7월14일)
	case SKILL_PLAY_PET_ATTACK:
	case SKILL_PLAY_PET_ATTACK2:
	case SKILL_PLAY_ELEMENTAL_SHOT: // 박재원 - 매직 포스 사용시 이 스킬의 뇌 속성은 제외시킨다.
	case SKILL_PLAY_SOUL_SUCKER: // 박재원 - 쏘울썩커도 매직 포스 적용시 제외시킨다.
	case SKILL_PLAY_X_RAGE: // 박재원 - 익스티림 레이지도 매직 포스 적용시 제외시킨다.
	case SKILL_PLAY_EXPANSION: // 박재원 - 익스팬션도 매직 포스 적용시 제외시킨다.

		return FALSE;
	}

	damage = lpPlayInfo->dwForceOrb_Damage;


	// 박재원 : 빌링 포스 추가
	if (lpPlayInfo->dwForceOrb_Code == (sinFO1 | sin27) || lpPlayInfo->dwForceOrb_Code == (sinFO1 | sin28) || lpPlayInfo->dwForceOrb_Code == (sinFO1 | sin29) || lpPlayInfo->dwForceOrb_Code == (sinFO1 | sin30))
	{
		//머키 매직포스, 데비네 매직포스, 쎌레스토 매직포스, 미라지 매직포스 공격력 10%가중
		damage2 = (MaxDamage + MinDamage) / 2;
		if (damage2 > 0 && damage2 < 1000)
			damage += (damage2 * 10) / 100;
	}
	else if (lpPlayInfo->dwForceOrb_Code == (sinFO1 | sin31) || lpPlayInfo->dwForceOrb_Code == (sinFO1 | sin35) ||
		lpPlayInfo->dwForceOrb_Code == (sinFO1 | sin36) || lpPlayInfo->dwForceOrb_Code == (sinFO1 | sin37))
	{
		//인페르나 매직포스, 빌링 매직포스(3종)도 15%가중
		damage2 = (MaxDamage + MinDamage) / 2;
		if (damage2 > 0 && damage2 < 1000)
			damage += (damage2 * 15) / 100; // 공격력 15%가중
	}
	else if (lpPlayInfo->dwForceOrb_Code == (sinFO1 | sin32))
	{
		//이니그마 매직포스 20%가중
		damage2 = (MaxDamage + MinDamage) / 2;
		if (damage2 > 0 && damage2 < 1000)
			damage += (damage2 * 20) / 100; //공격력 20%가중
	}

	if (dwSkillCode)
	{
		// 박재원 - 나이트 SKILL_PLAY_SWORD_OF_JUSTICE / 매지션 SKILL_PLAY_METEO 사용시 4차 스킬 인식 오류로 보완함.
		if (dwSkillCode == 336 || dwSkillCode == 384)
		{
			dwSkillCode += 15;
		}
		sJobLevel = dwSkillCode & 0xF;
		sJobLevel--;
		if (sJobLevel >= 0 && sJobLevel < 0x10) {
			sJobLevel >>= 2;
			switch (sJobLevel) {
			case 0:		//1차스킬
				if ((dwSkillCode & 0xFF) == SKILL_PLAY_MULTI_SPARK)
					damage = (damage * 50) / 100;
				else
					damage = (damage * 80) / 100;

				break;
			case 1:		//2차스킬
				damage = (damage * 90) / 100;
				break;
			case 2:		//3차스킬

				break;
			case 3:		//4차스킬
				damage = (damage * 150) / 100;
				break;
			}

		}
	}

	lpTransSkillAttackData->Power += damage;

#ifdef DISP_DAMAGE
	///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
	if (lpPlayInfo->AdminMode > 2) {
		if (lpChar) {
			wsprintf(szDispDamage, ">%s 범위공격 ( 매직포스오브 + %d ) (%d)", lpChar->smCharInfo.szName, damage, lpTransSkillAttackData->Power);
			rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
		}
		else {
			wsprintf(szDispDamage, "> 범위공격 ( 매직포스오브 + %d ) (%d)", damage, lpTransSkillAttackData->Power);
			rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
		}
	}
#endif
	return TRUE;
}

//공격을 유저한테 보냄 ( PK-PVP )
int rsSendAttackDataToUser(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, TRANS_ATTACKDATA* lpTransAttackData)
{
	int regs, cnt, len;
	int  Param1, Param2;

	if (!lpPlayInfo->AdminMode) {
		if (rsServerConfig.Enable_PKField_All)
		{
			// pluto PK허용 필드 전부 수정
			if (!rsServerConfig.Enable_PK || lpPlayInfo->Position.Area < 11 && lpPlayInfo->Position.Area > 34)
				return FALSE;			//현재 일반유저는 PK금지
		}
		else
		{
			if (lpPlayInfo->Position.Area == FIELD_ARENA && EventoArena::GetInstance()->arenaStage != 2) return FALSE;
			if (lpPlayInfo->Position.Area == FIELD_ARENA && EventoArena::GetInstance()->arenaStage == 2 && (lpPlayInfo->nEquipeArena == lpPlayInfo2->nEquipeArena)) return FALSE;
			
			if (!rsServerConfig.Enable_PK && !lpPlayInfo->PkMode && !lpPlayInfo2->PkMode &&
				sField[lpPlayInfo->Position.Area].State != FIELD_STATE_CASTLE &&
				lpPlayInfo->Position.Area != FIELD_ARENA && lpPlayInfo->Position.Area != 48)
				return FALSE;			//현재 일반유저는 PK금지
		}
	}

#ifdef DISP_DAMAGE
	///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
	if (lpPlayInfo->AdminMode > 2)
	{
		//해외
		wsprintf(szDispDamage, ">%s Damaged ( %d -> %d )", lpPlayInfo2->smCharInfo.szName, lpTransAttackData->Power, lpTransAttackData->Power / PK_POWER_DIVIDE);
		rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
	}
#endif

	Param1 = 0;
	Param2 = 0;

	//속성 공격일 경우
	regs = (lpTransAttackData->AttackState >> 16) & 0xF;
	if (regs && regs < sITEMINFO_NONE) {
		cnt = lpTransAttackData->AttackState >> (16 + 4);		//속성 값 ( 상위 비트에 전체 데미지중 속성 데미지만 저장됨 )
		if (!cnt)
			cnt = lpTransAttackData->Power;

		//속성에 따른 저항력 적용
		len = lpPlayInfo2->smCharInfo.Resistance[regs - 1];
		if (len) {					//속성 저항력 계산
			if (len >= 100) len = 100;
			if (len <= -100) len = -100;
			lpTransAttackData->Power -= ((cnt * len) / 100);

			if (lpTransAttackData->Power < 0) lpTransAttackData->Power = 0;
		}

		switch (regs - 1) { //(lpTransAttackData->AttackState>>16)-1) {
		case sITEMINFO_ICE:
			if (lpPlayInfo->dwSkill_EnchantWeapon_Time > dwPlayServTime && (lpPlayInfo->dwSkill_EnchantWeapon_Param >> 8) == 0) {
				//얼음 공격 속도 저하 ( 인체트 일때 적용 )
				Param1 = -SKILL_PLAY_ENCHANT_WEAPON;
				Param2 = 230 - 10 * (lpPlayInfo->dwSkill_EnchantWeapon_Param & 0xFF);	//레벨에 따른 속도
				break;
			}
			if (lpPlayInfo->dwSkill_DancingSword_Time > dwPlayServTime && (lpPlayInfo->dwSkill_DancingSword_Param & 0xFF) == 0) {
				//댄싱소드 얼음 공격
				Param1 = -SKILL_PLAY_DANCING_SWORD;
				Param2 = 230 - 10 * (lpPlayInfo->dwSkill_DancingSword_Param >> 16);
				break;
			}
			if (lpPlayInfo->dwSkill_FrostJavelin_Time > dwPlayServTime) {
				//프로스트재블린 얼음공격
				//속도 느려짐
				Param1 = -SKILL_PLAY_FROST_JAVELIN;
				Param2 = 240 - (240 * Frost_Javelin_SpeedSubPercent[lpPlayInfo->dwSkill_FrostJavelin_Param]) / 100;
				break;
			}
			break;

		}
	}


	return rsSendAttackUser(lpPlayInfo, lpPlayInfo2, lpTransAttackData->Power / PK_POWER_DIVIDE, 0x80, Param1, Param2);
}

//공격 수신 받음
int rsRecvAttackData(rsPLAYINFO* lpPlayInfo, TRANS_ATTACKDATA2* lpTransAttackData)
{
	TRANS_ATTACKDATA	TransAttackData;
	smCHAR* lpChar;
	smTRANS_COMMAND_EX	smTransCommand;
	rsPLAYINFO* lpPlayInfo2;
	char				szDispDamage[64];
	short				sDefAttack[2];
	int					pow;
	DWORD				dwCode;

	/*
	#ifdef DISP_DAMAGE
	///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
	if ( lpPlayInfo->AdminMode>2 ) {
	wsprintf( szDispDamage , "> 공격패킷 입수" );
	rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
	}
	#endif
	*/
#ifdef _USE_DYNAMIC_ENCODE
	rsDecodeDamagePacket(lpPlayInfo, lpTransAttackData);						//Decryption
#else
	dm_DecodePacket(lpPlayInfo->dwObjectSerial, lpTransAttackData);			//Decryption
#endif

	if (lpTransAttackData->dwChkSum != dm_GetDamgeChkSum(lpTransAttackData))	//패킷 검사
	{
		//데미지 데이타 오류를 군서버 로그에 기록
		smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = 8500;
		smTransCommand.LParam = 1;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;

		rsRecordDamageError(lpPlayInfo, (smTRANS_COMMAND*)&smTransCommand);
		return FALSE;
	}

	dwCode = lpTransAttackData->SkillCode & 0xFF;
	if (dwCode != SKILL_PLAY_FALCON && dwCode != SKILL_PLAY_MAGNETIC_SPHERE && dwCode != SKILL_PLAY_CHARGING_STRIKE &&
		dwCode != SKILL_PLAY_SHADOW_MASTER && dwCode != SKILL_PLAY_DIVINE_PIERCING) {
		//팰컨, 순서가 바뀔 가능성 땜에 제외
		if (lpTransAttackData->AttackCount <= lpPlayInfo->DamagePacketCount ||
			lpTransAttackData->dwTime <= lpPlayInfo->dwDamagePacketTime) {


#ifdef DISP_DAMAGE
			///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
			if (lpPlayInfo->AdminMode > 2) {
				wsprintf(szDispDamage, "> 데미지 클라이언트 카운팅 시간 오류");
				rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
			}
#endif

			//데미지 클라이언트 측 카운팅,시간 오류
			return FALSE;
		}

		lpPlayInfo->DamagePacketCount = lpTransAttackData->AttackCount;
		lpPlayInfo->dwDamagePacketTime = lpTransAttackData->dwTime;
	}

	if (lpTransAttackData->Power[0] > lpPlayInfo->sLimitDamage[0] ||
		lpTransAttackData->Power[1] > lpPlayInfo->sLimitDamage[1] ||
		lpTransAttackData->Critical[0] > lpPlayInfo->sLimitCritical[0]) {

		//데미지 데이타 오류를 군서버 로그에 기록
		smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
		smTransCommand.size = sizeof(smTRANS_COMMAND_EX);
		smTransCommand.WParam = 8510;
		smTransCommand.WxParam = 1;

		smTransCommand.LParam = lpTransAttackData->Power[0];
		smTransCommand.SParam = lpTransAttackData->Power[1];
		smTransCommand.EParam = lpTransAttackData->Critical[0];

		smTransCommand.LxParam = lpPlayInfo->sLimitDamage[0];
		smTransCommand.SxParam = lpPlayInfo->sLimitDamage[1];
		smTransCommand.ExParam = lpPlayInfo->sLimitCritical[0];

		rsRecordDamageError(lpPlayInfo, (smTRANS_COMMAND*)&smTransCommand);

#ifdef _LANGUAGE_KOREAN
		if (lpPlayInfo->AdminMode) {
			TRANS_CHATMESSAGE	TransChatMessage;

			wsprintf(TransChatMessage.szMessage, "공격 수치오류 ( %d %d %d ) ( %d %d %d )",
				lpTransAttackData->Power[0], lpTransAttackData->Power[1], lpTransAttackData->Critical[0],
				lpPlayInfo->sLimitDamage[0], lpPlayInfo->sLimitDamage[1], lpPlayInfo->sLimitCritical[0]);

			TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
			TransChatMessage.size = 32 + lstrlen(TransChatMessage.szMessage);
			TransChatMessage.dwIP = 0;
			TransChatMessage.dwObjectSerial = 0;
			lpPlayInfo->lpsmSock->Send((char*)&TransChatMessage, TransChatMessage.size, TRUE);
	}
#endif

		//제한 공격 수치 초과 보정
		lpTransAttackData->Power[0] = lpPlayInfo->sLimitDamage[0];
		lpTransAttackData->Power[1] = lpPlayInfo->sLimitDamage[1];
		lpTransAttackData->Critical[0] = lpPlayInfo->sLimitCritical[0];

}


	TransAttackData.x = lpTransAttackData->x;
	TransAttackData.y = lpTransAttackData->y;
	TransAttackData.z = lpTransAttackData->z;

	TransAttackData.AttackSize = lpTransAttackData->AttackSize;
	TransAttackData.dwTarObjectSerial = lpTransAttackData->dwTarObjectSerial;
	TransAttackData.dwDestObjectSerial = lpTransAttackData->dwDestObjectSerial;

	sDefAttack[0] = lpTransAttackData->Power[0];
	sDefAttack[1] = lpTransAttackData->Power[1];

	TransAttackData.Power = GetRandomPos(sDefAttack[0], sDefAttack[1]);
	TransAttackData.AttackState = lpTransAttackData->AttackState;

	dm_SkillCode_Temp = 0;

	if (lpTransAttackData->Area[0] >= 0)
		lpChar = srFindCharFromSerial(lpTransAttackData->dwTarObjectSerial, lpTransAttackData->Area[0]);
	else
		lpChar = srFindCharFromSerial(lpTransAttackData->dwTarObjectSerial);

	if (lpTransAttackData->SkillCode) {
		if (rsSetSkillAttack(lpPlayInfo, lpChar, &TransAttackData, lpTransAttackData) == FALSE)
			return FALSE;
	}

	if (rsSetSkillPlus(lpPlayInfo, lpChar, &TransAttackData, lpTransAttackData) == FALSE) return FALSE;

	if (!lpTransAttackData->SkillCode) {
		//일반 공격
		if (rsServerConfig.Event_Child) {
			//어린이 이벤트 ( 얼큰이 캐릭일때 공격가중 )
			if (lpPlayInfo->smCharInfo.SizeLevel > 0x1000 && lpPlayInfo->smCharInfo.SizeLevel <= 0x1002) {
				TransAttackData.Power += (TransAttackData.Power * 15) / 100;	//공격력 15% 가중
			}
		}
	}

	//공격에 저주가 걸렸다
	if (lpPlayInfo->dwCurse_Attack_Time) {
		if (lpPlayInfo->dwCurse_Attack_Time > dwPlayServTime)
			TransAttackData.Power -= (TransAttackData.Power * lpPlayInfo->dwCurse_Attack_Param) / 100;
		else
			lpPlayInfo->dwCurse_Attack_Time = 0;
	}


	// 장별 - 옐로우 드래곤
	if (lpPlayInfo->dwTime_PrimeItem_HelpPet > (DWORD)tServerTime && lpPlayInfo->smCharInfo.GravityScroolCheck[1] == 2)
	{
		TransAttackData.Power += (TransAttackData.Power * 10) / 100;
	}





	//	if ( !lpChar ) return FALSE;

	if (lpChar) {

		//포스오브 공격력 추가 
		AddForceOrbPower(lpPlayInfo, lpChar, &TransAttackData, lpTransAttackData->SkillCode, sDefAttack[0], sDefAttack[1]);

		if (lpTransAttackData->Critical[0]) {
			if ((rand() % 100) < GetCritical(lpPlayInfo, lpChar, lpTransAttackData->Critical[0])) {
				TransAttackData.AttackState = (TransAttackData.AttackState & 0xFFFF0000) + 2;		//크리티컬 처리
				pow = TransAttackData.Power;
				TransAttackData.Power = (pow * 170) / 100;						//크리티컬 공격가중

				//어세신 아이
				if (lpPlayInfo->dwSkill_AssassinEye_Time) {
					if (lpPlayInfo->dwSkill_AssassinEye_Time > dwPlayServTime) {
						TransAttackData.Power = (pow * (170 + Assassin_Eye_AddCritical[lpPlayInfo->dwSkill_AssassinEye_Param])) / 100;						//크리티컬 공격가중
					}
					else {
						lpPlayInfo->dwSkill_AssassinEye_Time = 0;
						lpPlayInfo->dwSkill_AssassinEye_Param = 0;
					}
				}

				smTransCommand.code = smTRANSCODE_ATTACK_RESULT;
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.WParam = 1;
				smTransCommand.LParam = 0;
				smTransCommand.SParam = 0;
				smTransCommand.EParam = 0;
				lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

#ifdef DISP_DAMAGE
				///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
				//if ( lpPlayInfo->AdminMode>2 ) {
				//wsprintf( szDispDamage , ">%s 공격시도 ( 크리티컬 )" , lpChar->smCharInfo.szName );
				//rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
				//}
#endif

			}
		}

		//빌링아이템스킬적용
		rsBillingItemSkill(lpPlayInfo, lpChar, 1, TransAttackData.Power, lpTransAttackData->SkillCode & 0xFF);


		rsRecvAttackData_Old(lpPlayInfo, &TransAttackData, lpChar, TRUE);

		if (lpChar->dwTargetLockTime == 0 && (
			lpChar->smCharInfo.dwCharSoundCode == snCHAR_SOUND_DEATHKNIGHT ||
			lpChar->smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL ||
			lpChar->smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_R ||
			lpChar->smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_G ||
			lpChar->smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_B))
		{
			//바벨 또는 크리스탈 원거리 공격한 유저로 타겟 변경
			int dist, x, y, z;

			x = (lpPlayInfo->Position.x - lpChar->pX) >> FLOATNS;
			y = (lpPlayInfo->Position.y - lpChar->pY) >> FLOATNS;
			z = (lpPlayInfo->Position.z - lpChar->pZ) >> FLOATNS;
			dist = x * x + y * y + z * z;

			if (dist <= (lpChar->smCharInfo.Shooting_Range * lpChar->smCharInfo.Shooting_Range)) {
				lpChar->lpTargetPlayInfo = lpPlayInfo;
				lpChar->dwTargetLockTime = dwPlayServTime + 4000;
			}
		}
	}
	else {
		//PK - PVP 모드용

		lpPlayInfo2 = srFindUserFromSerial(lpTransAttackData->dwTarObjectSerial);
		if (lpPlayInfo2) 
		{
			if (lpPlayInfo->PkMode && !lpPlayInfo2->PkMode && lpPlayInfo2->Position.Area != FIELD_ARENA && (sField[lpPlayInfo2->Position.Area].State != FIELD_STATE_CASTLE || lpPlayInfo2->Position.z > CastleBattleZone_LineZ))
				return FALSE;

			AddForceOrbPower(lpPlayInfo, 0, &TransAttackData, lpTransAttackData->SkillCode, sDefAttack[0], sDefAttack[1]);

			if (lpTransAttackData->Critical[0]) {
				if ((rand() % 100) < GetCritical2(lpPlayInfo, lpPlayInfo2, lpTransAttackData->Critical[0])) {
					TransAttackData.AttackState = (TransAttackData.AttackState & 0xFFFF0000) + 2;		//크리티컬 처리
					pow = TransAttackData.Power;
					TransAttackData.Power = (pow * 170) / 100;						//크리티컬 공격가중

					//어세신 아이
					if (lpPlayInfo->dwSkill_AssassinEye_Time) {
						if (lpPlayInfo->dwSkill_AssassinEye_Time > dwPlayServTime) {
							TransAttackData.Power = (pow * (170 + Assassin_Eye_AddCritical[lpPlayInfo->dwSkill_AssassinEye_Param])) / 100;						//크리티컬 공격가중
						}
						else {
							lpPlayInfo->dwSkill_AssassinEye_Time = 0;
							lpPlayInfo->dwSkill_AssassinEye_Param = 0;
						}
					}

					smTransCommand.code = smTRANSCODE_ATTACK_RESULT;
					smTransCommand.size = sizeof(smTRANS_COMMAND);
					smTransCommand.WParam = 1;
					smTransCommand.LParam = 0;
					smTransCommand.SParam = 0;
					smTransCommand.EParam = 0;
					lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

#ifdef DISP_DAMAGE
					///////////////////////// 디버그용 데미지 적용 출력 //////////////////////
					//if ( lpPlayInfo->AdminMode>2 ) {
					//wsprintf( szDispDamage , ">%s 공격시도 ( 크리티컬 )" , lpPlayInfo2->smCharInfo.szName );
					//rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
					//}
#endif
				}
			}
			rsSendAttackDataToUser(lpPlayInfo, lpPlayInfo2, &TransAttackData);
		}
	}


	if (lpPlayInfo->AdminMode) {
		smTransCommand.code = smTRANSCODE_DISP_DAMAGE;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = TransAttackData.Power;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;
		lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
	}

	return TRUE;
	}



int rsSetSkillRangeAttack(rsPLAYINFO* lpPlayInfo, smCHAR* lpChar, TRANS_ATTACKDATA* lpTransAttackData, TRANS_SKIL_ATTACKDATA* lpTransSkillAttackData, TRANS_SKIL_ATTACKDATA2* lpTransRangeAttack)
{
	DWORD	dwSkillCode;
	int		Point;
	int		Param;
	int		pow, pow1, pow2;
	int		cnt;
	smTRANS_COMMAND	smTransCommand;

	dwSkillCode = lpTransRangeAttack->SkillCode & 0xFF;
	Point = ((lpTransRangeAttack->SkillCode >> 8) & 0xF) - 1;
	Param = (lpTransRangeAttack->SkillCode >> 12) & 0xF;

	if (Point < 0 || Point >= 10) return FALSE;

	dm_SkillCode_Temp = dwSkillCode;

	if (lpPlayInfo->dwSkill_Vanish_Time)
	{

		if (lpPlayInfo->dwSkill_Vanish_Time < dwPlayServTime)
		{
			lpPlayInfo->dwSkill_Vanish_Time = 0;
			lpPlayInfo->dwSkill_Vanish_Param = 0;
			return TRUE;
		}

		lpPlayInfo->dwSkill_Vanish_Time = 0;
		lpPlayInfo->dwSkill_Vanish_Param = 0;


		smTransCommand.code = smTRANSCODE_PROCESS_SKILL;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = SKILL_PLAY_VANISH;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;
		lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
	}


	if (rsCheckSkillLevel(lpPlayInfo, dwSkillCode) == FALSE)
	{
		if (lpPlayInfo->WarningCount < 5)
		{

			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
			smTransCommand.WParam = 8850;
			smTransCommand.LParam = lpPlayInfo->smCharInfo.Level;
			smTransCommand.SParam = dwSkillCode;
			smTransCommand.EParam = Point;
			rsSendDataServer(lpPlayInfo->lpsmSock, &smTransCommand);
			lpPlayInfo->WarningCount++;
		}
		return FALSE;
	}

	switch (dwSkillCode)
	{
	case SKILL_PLAY_PIKEWIND:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PIKEMAN) return FALSE;

		lpTransSkillAttackData->AttackSize = Pike_Wind_Push_Lenght[Point];
		lpTransSkillAttackData->Power = GetRandomPos(Pike_Wind_Damage[Point][0], Pike_Wind_Damage[Point][1]);
		lpTransSkillAttackData->AttackState = 1;
		return TRUE;

	case SKILL_PLAY_GROUND_PIKE:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PIKEMAN) return FALSE;

		lpTransSkillAttackData->AttackSize = G_Pike_Range[Point];
		lpTransSkillAttackData->Power = GetRandomPos(G_Pike_Damage[Point][0], G_Pike_Damage[Point][1]);
		lpTransSkillAttackData->AttackState = 3;
		return TRUE;

	case SKILL_PLAY_ROAR:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_FIGHTER) return FALSE;

		lpTransSkillAttackData->AttackSize = Roar_Range[Point];
		lpTransSkillAttackData->Power = Roar_Time[Point];
		lpTransSkillAttackData->AttackState = 2;
		return TRUE;

	case SKILL_PLAY_DIVINE_LIGHTNING:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PRIESTESS) return FALSE;
		if (lpTransSkillAttackData->TargetCount > Divine_Lightning_Num[Point]) return FALSE;

		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		pow += (pow * Divine_Lightning_Damage[Point]) / 100;
		lpTransSkillAttackData->Power = pow;
		lpTransSkillAttackData->AttackState = 103;

		return TRUE;

	case SKILL_PLAY_MECHANIC_BOMB:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_MECHANICIAN) return FALSE;

		lpTransSkillAttackData->AttackSize = Mechanic_Bomb_Attack_Range[Point];
		lpTransSkillAttackData->Power = GetRandomPos(Mechanic_Bomb_Damage[Point][0], Mechanic_Bomb_Damage[Point][1]);
		lpTransAttackData->Power = lpTransSkillAttackData->Power >> 1;
		lpTransSkillAttackData->Power >>= 1;
		lpTransSkillAttackData->AttackState = 0;
		return TRUE;

	case SKILL_PLAY_GREAT_SMASH:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_MECHANICIAN) return FALSE;

		lpTransSkillAttackData->AttackSize = 70;

		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1] + (lpTransRangeAttack->Critical[1] / 5));
		pow += G_Smash_Attack_Rage[Point];
		pow >>= 1;

		lpTransSkillAttackData->Power = pow;
		lpTransAttackData->Power = pow;
		lpTransSkillAttackData->AttackState = 0;
		return TRUE;

	case SKILL_PLAY_SPARK:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_MECHANICIAN) return FALSE;

		lpTransSkillAttackData->AttackSize = Spark_Area[Point];
		lpTransSkillAttackData->Power = GetRandomPos(Spark_Damage[Point][0], Spark_Damage[Point][1]);
		lpTransSkillAttackData->Power += lpPlayInfo->smCharInfo.Level / 5;
		lpTransSkillAttackData->AttackState = 101;


		return TRUE;

	case SKILL_PLAY_TORNADO:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PIKEMAN) return FALSE;

		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		pow += (pow * Tornado_Damage[Point]) / 100;
		lpTransSkillAttackData->Power = pow;
		lpTransSkillAttackData->AttackState = 0;
		return TRUE;

	case SKILL_PLAY_ARROWOF_RAGE:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ARCHER) return FALSE;

		lpTransSkillAttackData->AttackSize = A_Rage_Area[Point];
		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		pow += (lpPlayInfo->smCharInfo.Level / A_Rage_Damage[Point]);
		lpTransSkillAttackData->Power = pow;
		lpTransSkillAttackData->AttackState = 0;
		return TRUE;

	case SKILL_PLAY_RAGEOF_ZECRAM:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_FIGHTER) return FALSE;

		lpTransSkillAttackData->AttackSize = 70;
		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		pow += (pow * R_Zecram_Damage[Point]) / 100;
		pow >>= 1;
		lpTransSkillAttackData->Power = pow;
		lpTransSkillAttackData->AttackState = 100;
		lpTransAttackData->AttackState |= (sITEMINFO_FIRE + 1) << 16;
		return TRUE;

	case SKILL_PLAY_EXPANSION:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PIKEMAN) return FALSE;

		lpTransSkillAttackData->AttackSize = 180;
		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		pow += (pow * Expansion_Damage[Point]) / 100;


		pow += (pow * (5 * (Point + 1))) / 100;

		lpTransSkillAttackData->Power = pow;
		lpTransSkillAttackData->AttackState = 0;


		if (lpChar)
		{
			for (cnt = 0; cnt < lpTransRangeAttack->TargetCount; cnt++)
			{
				if (lpTransRangeAttack->dwTarObjectSerial[cnt] == lpChar->dwObjectSerial)
				{
					lpTransAttackData->Power = 0;
					break;
				}
			}
		}
		return TRUE;

	case SKILL_PLAY_SWORD_BLAST:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_KNIGHT) return FALSE;

		pow = GetRandomPos(Sword_Blast_Damage[Point][0], Sword_Blast_Damage[Point][1] + (lpPlayInfo->smCharInfo.Level / 2));
		lpTransSkillAttackData->Power = pow;
		lpTransSkillAttackData->AttackState = 0;
		return TRUE;

	case SKILL_PLAY_BRANDISH:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_KNIGHT) return FALSE;

		lpTransSkillAttackData->AttackSize = Brandish_Range[Point];
		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		pow += (pow * Brandish_Damage[Point]) / 100;
		lpTransSkillAttackData->Power = pow;
		lpTransSkillAttackData->AttackState = 0;
		return TRUE;

	case SKILL_PLAY_PIERCING:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_KNIGHT) return FALSE;

		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		pow += (pow * Piercing_Damage[Point]) / 100;
		lpTransSkillAttackData->Power = pow;
		lpTransSkillAttackData->AttackState = 0;
		return TRUE;

	case SKILL_PLAY_SHIELD_STRIKE:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ATALANTA) return FALSE;

		lpTransSkillAttackData->Power = GetRandomPos(S_Strike_Damage[Point][0], S_Strike_Damage[Point][1] + (lpPlayInfo->smCharInfo.Level / 5));
		lpTransSkillAttackData->AttackState = 5;
		return TRUE;

		// Corre豫o soul sucker funcionando
	case SKILL_PLAY_SOUL_SUCKER:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ATALANTA) return FALSE;


		if (lpChar)
		{
			for (cnt = 0; cnt < lpTransRangeAttack->TargetCount; cnt++)
			{
				if (lpTransRangeAttack->dwTarObjectSerial[cnt])
				{
					auto monster = srFindCharFromSerial(lpTransRangeAttack->dwTarObjectSerial[cnt]);

					if (monster)
					{
						if (monster->smCharInfo.wPlayClass[0] != MONSTER_CLASS_BOSS) // N�o funciona em boss
							monster->smCharInfo.Life[0] -= monster->smCharInfo.Life[0] * Soul_Sucker_Absorb[Point] / 100;
					}
				}
			}
		}

		lpTransSkillAttackData->Power = Point;
		lpTransSkillAttackData->AttackState = 4;

		return TRUE;

	case SKILL_PLAY_FIRE_BALL:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_MAGICIAN) return FALSE;

		lpTransSkillAttackData->AttackSize = FireBall_Area[Point];

		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		pow += (pow * FireBall_Damage[Point]) / 100;
		lpTransSkillAttackData->Power = pow;
		lpTransAttackData->Power += (lpTransAttackData->Power * FireBall_Damage[Point]) / 100;
		lpTransSkillAttackData->AttackState = 100;
		lpTransAttackData->AttackState |= (sITEMINFO_FIRE + 1) << 16;
		return TRUE;

	case SKILL_PLAY_COLUMN_OF_WATER:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_MAGICIAN) return FALSE;

		lpTransSkillAttackData->AttackSize = Watornado_Area[Point];

		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		pow += (pow * Watornado_Damage[Point]) / 100;
		lpTransSkillAttackData->Power = pow;

		lpTransAttackData->Power += (lpTransAttackData->Power * Watornado_Damage[Point]) / 100;
		lpTransSkillAttackData->AttackState = 104;
		lpTransAttackData->AttackState |= (sITEMINFO_NONE) << 16;
		return TRUE;

	case SKILL_PLAY_ENCHANT_WEAPON:
		if (!lpPlayInfo->dwSkill_EnchantWeapon_Time) return FALSE;

		lpTransSkillAttackData->AttackSize = 50;
		lpTransSkillAttackData->Power = GetRandomPos(Enchant_Weapon_Damage_Lightning[Point][0], Enchant_Weapon_Damage_Lightning[Point][1]);
		lpTransSkillAttackData->AttackState = 105;
		return TRUE;

	case SKILL_PLAY_VENOM_SPEAR:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PIKEMAN) return FALSE;
		if (lpTransSkillAttackData->TargetCount > VenomSpear_Num[Point]) return FALSE;


		if (lpTransRangeAttack->Power2[0] > lpPlayInfo->sLimitDamage[0])
			lpTransRangeAttack->Power2[0] = lpPlayInfo->sLimitDamage[0];
		if (lpTransRangeAttack->Power2[1] > lpPlayInfo->sLimitDamage[1])
			lpTransRangeAttack->Power2[1] = lpPlayInfo->sLimitDamage[1];

		pow1 = lpTransRangeAttack->Power2[0] >> 1;
		pow2 = lpTransRangeAttack->Power2[1] >> 1;

		lpTransSkillAttackData->Power = GetRandomPos(pow1, pow2);
		lpTransSkillAttackData->AttackState = 106;
		lpTransAttackData->AttackState |= (sITEMINFO_POISON + 1) << 16;

		return TRUE;


	case SKILL_PLAY_ELEMENTAL_SHOT:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ARCHER) return FALSE;
		if (Param != 1) return FALSE;
		lpTransSkillAttackData->AttackSize = 75;
		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		lpTransAttackData->Power = pow;
		lpTransSkillAttackData->Power = GetRandomPos(Elemental_Shot_Lightning[Point][0], Elemental_Shot_Lightning[Point][1]);
		lpTransSkillAttackData->AttackState = 105;
		lpTransAttackData->AttackState |= (sITEMINFO_LIGHTING + 1) << 16;
		return TRUE;

	case SKILL_PLAY_BOMB_SHOT:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ARCHER) return FALSE;

		if (lpChar)
		{
			pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
			pow += (pow * BombShot_Damage[Point]) / 100;

			if (lpChar->smCharInfo.Brood == smCHAR_MONSTER_DEMON)
				pow += (pow * BombShot_DemonDamage[Point]) / 100;

			lpTransAttackData->Power = pow;
		}
		lpTransSkillAttackData->Power = GetRandomPos(BombShot_AddDamage[Point][0], BombShot_AddDamage[Point][1]);
		lpTransSkillAttackData->AttackState = 100;
		lpTransAttackData->AttackState = 0;

		return TRUE;

	case SKILL_PLAY_PERFORATION:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ARCHER) return FALSE;

		lpTransSkillAttackData->AttackSize = 340 + Perforation_Attack_Range[Point];

		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		pow += (pow * Perforation_Damage[Point]) / 100;
		lpTransSkillAttackData->Power = pow;
		lpTransSkillAttackData->AttackState = 0;
		dm_Critical_Temp = lpTransRangeAttack->Critical[0];
		return TRUE;

	case SKILL_PLAY_LIGHTNING_JAVELIN:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ATALANTA) return FALSE;

		lpTransSkillAttackData->AttackSize = 100;
		pow = GetRandomPos(Lightning_Javelin_Damage[Point][0], Lightning_Javelin_Damage[Point][1] + (lpPlayInfo->smCharInfo.Level / 3));
		lpTransSkillAttackData->Power = pow;
		lpTransSkillAttackData->AttackState = 101;

		lpTransAttackData->Power = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1] + (lpPlayInfo->smCharInfo.Level / 3));
		lpTransAttackData->AttackState = 0;

		if (lpChar)
		{
			if (lpChar->smCharInfo.Brood == smCHAR_MONSTER_UNDEAD)
			{
				lpTransAttackData->Power += lpTransAttackData->Power / 2;
			}


			if (lpTransRangeAttack->Critical[0])
			{
				if ((rand() % 100) < GetCritical(lpPlayInfo, lpChar, lpTransRangeAttack->Critical[0]))
				{
					lpTransAttackData->Power = (lpTransAttackData->Power * 170) / 100;

				}
			}
		}
		return TRUE;

	case SKILL_PLAY_STORM_JAVELIN:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ATALANTA) return FALSE;
		lpTransSkillAttackData->AttackSize = 340;
		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		pow += (pow * Storm_Javelin_Damage[Point]) / 100;
		lpTransSkillAttackData->Power = pow;
		lpTransSkillAttackData->AttackState = 0;
		return TRUE;


	case SKILL_PLAY_EXTINCTION:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PRIESTESS) return FALSE;
		lpTransSkillAttackData->Power = Point + 1;
		lpTransSkillAttackData->AttackState = 6;
		return TRUE;

	case SKILL_PLAY_DIASTROPHISM:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_MAGICIAN) return FALSE;
		if (lpTransRangeAttack->Power[0] < lpTransRangeAttack->Power2[0] || lpTransRangeAttack->Power[1] < lpTransRangeAttack->Power2[1])
		{

			return FALSE;
		}

		lpTransSkillAttackData->AttackSize = Diastrophism_Area[Point];
		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		pow += (pow * Diastrophism_Damage[Point]) / 100;

		lpTransSkillAttackData->Power = pow;
		lpTransSkillAttackData->AttackState = 0;

		return TRUE;




	case SKILL_PLAY_IMPULSION:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_MECHANICIAN) return FALSE;


		lpTransSkillAttackData->AttackSize = Impulsion_Range[Point];
		lpTransSkillAttackData->Power = GetRandomPos(Impulsion_LightingDamage[Point][0], Impulsion_LightingDamage[Point][1]);
		lpTransSkillAttackData->AttackState = 101;
		return TRUE;

	case SKILL_PLAY_COMPULSION:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_MECHANICIAN) return FALSE;
		lpTransSkillAttackData->AttackState = 201;
		lpTransSkillAttackData->AttackSize = Compulsion_Area[Point];

		lpPlayInfo->dwSkill_Compulsion_Time = dwPlayServTime + Compulsion_Time[Point] * 1000;
		lpPlayInfo->dwSkill_Compulsion_Param = Compulsion_AddAbsorb[Point];

		return TRUE;

	case SKILL_PLAY_CYCLONE_STRIKE:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_FIGHTER) return FALSE;
		if (lpTransSkillAttackData->TargetCount > Cyclone_Strike_AttackNum[Point] + 1) return FALSE;

		lpTransAttackData->Power = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		lpTransAttackData->Power += (lpTransAttackData->Power * Cyclone_Strike_DamagePercent[Point]) / 100;
		lpTransSkillAttackData->AttackSize = Cyclone_Strike_Area[Point];
		lpTransSkillAttackData->Power += GetRandomPos(Cyclone_Strike_AreaDamage[Point][0], Cyclone_Strike_AreaDamage[Point][1]);
		lpTransSkillAttackData->AttackState = 0;
		dm_Critical_Temp = lpTransRangeAttack->Critical[0];
		return TRUE;

	case SKILL_PLAY_PHOENIX_SHOT:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ARCHER) return FALSE;

		lpTransSkillAttackData->Power = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		lpTransSkillAttackData->Power += (lpTransSkillAttackData->Power * Phoenix_Shot_DamagePercent[Point]) / 100;
		lpTransSkillAttackData->AttackState = 0;
		return TRUE;


	case SKILL_PLAY_SWORD_OF_JUSTICE:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_KNIGHT) return FALSE;

		lpTransSkillAttackData->Power = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		lpTransSkillAttackData->Power += (lpTransSkillAttackData->Power * Sword_Of_Justice_DamagePercent[Point]) / 100;
		lpTransSkillAttackData->AttackSize = Sword_Of_Justice_Area[Point];
		lpTransSkillAttackData->AttackState = 0;
		return TRUE;

	case SKILL_PLAY_X_RAGE:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_ATALANTA) return FALSE;

		lpTransSkillAttackData->Power = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		lpTransSkillAttackData->Power += (lpTransSkillAttackData->Power * X_Rage_DamagePercent[Point]) / 100;
		lpTransSkillAttackData->AttackSize = X_Rage_Area[Point];
		lpTransSkillAttackData->AttackState = 0;
		return TRUE;

	case SKILL_PLAY_GLACIAL_SPIKE:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PRIESTESS) return FALSE;

		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		pow += (pow * Glacial_Spike_Damage[Point]) / 100;
		lpTransSkillAttackData->Power = pow;
		lpTransSkillAttackData->AttackState = 3;
		return TRUE;

	case SKILL_PLAY_CHAIN_LIGHTNING:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_PRIESTESS) return FALSE;
		/*if (lpTransSkillAttackData->TargetCount > Chain_Lightning_Num[Point]) return FALSE;
		lpTransSkillAttackData->AttackSize = Chain_Lightning_Range[Point] * Chain_Lightning_Num[Point];*/

		lpTransSkillAttackData->AttackSize = Chain_Lightning_Range[Point];

		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		pow += (pow * Chain_Lightning_Damage[Point]) / 100;
		lpTransSkillAttackData->Power = pow;

		lpTransSkillAttackData->AttackState = 3;
		return TRUE;

	case SKILL_PLAY_FLAME_WAVE:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_MAGICIAN) return FALSE;


		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		pow += (pow * Flame_Wave_FireDamage[Point]) / 100;
		lpTransSkillAttackData->Power = pow;

		lpTransSkillAttackData->AttackState = 100;
		return TRUE;

	case SKILL_PLAY_DISTORTION:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_MAGICIAN) return FALSE;


		lpTransSkillAttackData->AttackState = 7;
		return TRUE;

	case SKILL_PLAY_METEO:
		if (lpPlayInfo->smCharInfo.JOB_CODE != JOBCODE_MAGICIAN) return FALSE;

		lpTransSkillAttackData->AttackSize = Meteo_Area[Point];


		pow = GetRandomPos(lpTransRangeAttack->Power[0], lpTransRangeAttack->Power[1]);
		pow += (pow * Meteo_Damage[Point]) / 100;
		lpTransSkillAttackData->Power = pow;

		lpTransSkillAttackData->AttackState = 100;
		return TRUE;

	}

	return FALSE;
}

int rsRecvRangeAttackData(rsPLAYINFO* lpPlayInfo, TRANS_SKIL_ATTACKDATA2* lpTransRangeAttackData)
{

	TRANS_ATTACKDATA		TransAttackData;
	TRANS_SKIL_ATTACKDATA	TransSkillAttackData;
	smCHAR* lpChar;
	smTRANS_COMMAND_EX	smTransCommand;

#ifdef _USE_DYNAMIC_ENCODE
	rsDecodeDamagePacket(lpPlayInfo, lpTransAttackData);						//Decryption
#else
	dm_DecodePacket(lpPlayInfo->dwObjectSerial, lpTransRangeAttackData);			//Decryption
#endif

	if (lpTransRangeAttackData->dwChkSum != dm_GetRangeDamgeChkSum(lpTransRangeAttackData)) {	//패킷검사

		//데미지 데이타 오류를 군서버 로그에 기록
		smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = 8500;
		smTransCommand.LParam = 2;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;

		rsRecordDamageError(lpPlayInfo, (smTRANS_COMMAND*)&smTransCommand);
		return FALSE;
	}


	if ((lpTransRangeAttackData->SkillCode & 0xFF) != SKILL_PLAY_ENCHANT_WEAPON) {		//인첸트 웨폰, 순서가 바뀔 가능성 땜에 제외
		if (lpTransRangeAttackData->AttackCount <= lpPlayInfo->DamagePacketCount ||
			lpTransRangeAttackData->dwTime <= lpPlayInfo->dwDamagePacketTime) {
			//데미지 클라이언트 측 카운팅,시간 오류
			return FALSE;
		}

		lpPlayInfo->DamagePacketCount = lpTransRangeAttackData->AttackCount;
		lpPlayInfo->dwDamagePacketTime = lpTransRangeAttackData->dwTime;
	}

	if (lpTransRangeAttackData->Power[0] > lpPlayInfo->sLimitDamage[0] ||
		lpTransRangeAttackData->Power[1] > lpPlayInfo->sLimitDamage[1] ||
		lpTransRangeAttackData->Critical[0] > lpPlayInfo->sLimitCritical[0]) {

		//데미지 데이타 오류를 군서버 로그에 기록
		smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
		smTransCommand.size = sizeof(smTRANS_COMMAND_EX);
		smTransCommand.WParam = 8510;
		smTransCommand.WxParam = 1;

		smTransCommand.LParam = lpTransRangeAttackData->Power[0];
		smTransCommand.SParam = lpTransRangeAttackData->Power[1];
		smTransCommand.EParam = lpTransRangeAttackData->Critical[0];

		smTransCommand.LxParam = lpPlayInfo->sLimitDamage[0];
		smTransCommand.SxParam = lpPlayInfo->sLimitDamage[1];
		smTransCommand.ExParam = lpPlayInfo->sLimitCritical[0];

		rsRecordDamageError(lpPlayInfo, (smTRANS_COMMAND*)&smTransCommand);

#ifdef _LANGUAGE_KOREAN
		if (lpPlayInfo->AdminMode) {
			TRANS_CHATMESSAGE	TransChatMessage;

			wsprintf(TransChatMessage.szMessage, "공격 수치오류 ( %d %d %d ) ( %d %d %d )",
				lpTransRangeAttackData->Power[0], lpTransRangeAttackData->Power[1], lpTransRangeAttackData->Critical[0],
				lpPlayInfo->sLimitDamage[0], lpPlayInfo->sLimitDamage[1], lpPlayInfo->sLimitCritical[0]);

			TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
			TransChatMessage.size = 32 + lstrlen(TransChatMessage.szMessage);
			TransChatMessage.dwIP = 0;
			TransChatMessage.dwObjectSerial = 0;
			lpPlayInfo->lpsmSock->Send((char*)&TransChatMessage, TransChatMessage.size, TRUE);
	}
#endif
		//제한 공격 수치 초과 보정
		lpTransRangeAttackData->Power[0] = lpPlayInfo->sLimitDamage[0];
		lpTransRangeAttackData->Power[1] = lpPlayInfo->sLimitDamage[1];
		lpTransRangeAttackData->Critical[0] = lpPlayInfo->sLimitCritical[0];
}


	TransAttackData.Power = 0;
	TransSkillAttackData.Power = 0;
	dm_SkillCode_Temp = 0;

	if (lpTransRangeAttackData->dwMainTargetObject)
		lpChar = srFindCharFromSerial(lpTransRangeAttackData->dwMainTargetObject, lpTransRangeAttackData->Area[0]);
	else
		lpChar = 0;


	if (lpTransRangeAttackData->dwMainTargetObject) {
		TransAttackData.AttackState = 0;
		TransAttackData.AttackSize = 64 * 64;
		TransAttackData.dwDestObjectSerial = lpTransRangeAttackData->dwDestObjectSerial;
		TransAttackData.x = lpTransRangeAttackData->x;
		TransAttackData.y = lpTransRangeAttackData->y;
		TransAttackData.z = lpTransRangeAttackData->z;
	}

	TransSkillAttackData.AttackSize = lpTransRangeAttackData->AttackSize;
	TransSkillAttackData.dwDestObjectSerial = lpTransRangeAttackData->dwDestObjectSerial;
	TransSkillAttackData.x = lpTransRangeAttackData->x;
	TransSkillAttackData.y = lpTransRangeAttackData->y;
	TransSkillAttackData.z = lpTransRangeAttackData->z;
	TransSkillAttackData.AttackSize = 200;

	if (lpTransRangeAttackData->TargetCount > 0 && lpTransRangeAttackData->TargetCount < SKIL_ATTACK_CHAR_MAX) {
		memcpy(TransSkillAttackData.dwTarObjectSerial, lpTransRangeAttackData->dwTarObjectSerial, sizeof(DWORD) * lpTransRangeAttackData->TargetCount);
		TransSkillAttackData.TargetCount = lpTransRangeAttackData->TargetCount;
	}
	else
		TransSkillAttackData.TargetCount = 0;


	if (rsSetSkillRangeAttack(lpPlayInfo, lpChar, &TransAttackData, &TransSkillAttackData, lpTransRangeAttackData) == FALSE)
		return FALSE;

	if (lpPlayInfo->dwSkill_SpiritElemental_Time)
	{
		if (lpPlayInfo->dwSkill_SpiritElemental_Time > dwPlayServTime)
		{
			TransSkillAttackData.Power += (TransSkillAttackData.Power * lpPlayInfo->dwSkill_SpiritElemental_Param) / 100;
		}
		else
		{
			lpPlayInfo->dwSkill_SpiritElemental_Time = 0;
			lpPlayInfo->dwSkill_SpiritElemental_Param = 0;
		}
	}

	// 박재원 - 매직 포스 추가, 빌링 매직 포스 추가(범위공격용 포스오브 파워업)
	AddRangeForceOrbPower(lpPlayInfo, lpChar, &TransSkillAttackData, lpTransRangeAttackData->SkillCode, lpTransRangeAttackData->Power[0], lpTransRangeAttackData->Power[1]);

	//빌링아이템스킬적용
	int Total = TransSkillAttackData.TargetCount;
	int AvgPow;

	if (lpChar) Total += 1;
	if (Total) AvgPow = (TransAttackData.Power + TransSkillAttackData.Power * TransSkillAttackData.TargetCount) / Total;
	else AvgPow = 0;

	rsBillingItemSkill(lpPlayInfo, lpChar, Total, AvgPow, lpTransRangeAttackData->SkillCode & 0xFF);

	if (lpChar && TransAttackData.Power) {
		//AddForceOrbPower(lpPlayInfo , lpChar ,  &TransAttackData , lpTransRangeAttackData->SkillCode );	//포스오브 공격력 추가 
		rsRecvAttackData_Old(lpPlayInfo, &TransAttackData, lpChar, TRUE);
	}

	if (TransSkillAttackData.TargetCount > 0) {
		RecvRangeAttack(lpPlayInfo, &TransSkillAttackData, lpTransRangeAttackData->Area[0], lpTransRangeAttackData->SkillCode);


		// pluto PK허용 필드 전부
		if (rsServerConfig.Enable_PKField_All)
		{
			// pluto PK허용 필드 전부 수정
			if (rsServerConfig.Enable_PK)
			{
				//현재  PK 테스트중
				RecvRangeAttack_User(lpPlayInfo, &TransSkillAttackData, lpTransRangeAttackData->Area[0], lpTransRangeAttackData->SkillCode);
			}
		}
		else
		{
			if (rsServerConfig.Enable_PK || 
				lpPlayInfo->PkMode || 
				lpPlayInfo->Position.Area == rsCASTLE_FIELD ||
				lpPlayInfo->Position.Area == 48 ||
				(lpPlayInfo->Position.Area == FIELD_ARENA && EventoArena::GetInstance()->arenaStage == 2))
			{
				RecvRangeAttack_User(lpPlayInfo, &TransSkillAttackData, lpTransRangeAttackData->Area[0], lpTransRangeAttackData->SkillCode);
			}
		}
	}

	if (lpPlayInfo->AdminMode) {
		smTRANS_COMMAND	smTransCommand;

		smTransCommand.code = smTRANSCODE_DISP_DAMAGE;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = TransSkillAttackData.Power + TransAttackData.Power;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;
		lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
	}


	return TRUE;
		}


/////////////////////////////////// 암호화 모듈 ///////////////////////////////////////////
//////////// 보안 철저 요구 /////////////

/*
//모듈 내용 - 원래 소스
#define	DAMAGE_PACKET_KEY	0xBBAA3840
#define	DAMAGE_PACKET_MUL	0x11223344

int	dm_EncodePacket2( void *lpPacket )
{
	DWORD size,code;
	DWORD cnt,cnt2,cnt3;
	DWORD dwCode;
	DWORD *lpDword = (DWORD *)lpPacket;

	size = lpDword[0];
	code = lpDword[1];
	size /=4;


	dwCode = (code^(code<<7)) + size*DAMAGE_PACKET_MUL;
	cnt2 = (code+DAMAGE_PACKET_KEY)&0xF5ABCF;
	cnt3 = cnt2&0x3C;

	for( cnt=2;cnt<size;cnt++ ) {
		dwCode = dwCode^(cnt2<<6)^lpDword[cnt];
		lpDword[cnt] = dwCode;
		cnt2+=cnt2+cnt3;
	}

	return TRUE;
}

//데미지 암호 풀기 함수
int	dm_DecodeDamagePacket( rsPLAYINFO *lpPlayInfo , void *lpPacket )
{
	DWORD size,code;
	DWORD cnt,cnt2,cnt3;
	DWORD dwCode;
	DWORD dwCode2;
	DWORD *lpDword = (DWORD *)lpPacket;

	size = lpDword[0];
	code = lpDword[1];
	size /=4;

	dwCode = (code^(code<<5)) + size*0x00003434;
	cnt2 = (code+0x00008989)&0xBCAFF5;
	cnt3 = cnt2&0x5E;

	for( cnt=2;cnt<size;cnt++ ) {
		dwCode2 = lpDword[cnt];
		lpDword[cnt] = dwCode2^(cnt2<<7)^dwCode;
		dwCode = dwCode2;
		cnt2+=cnt2+cnt3;
	}

	return TRUE;
}
*/

#ifdef _W_SERVER

#define	DM_ENCODE_PACKET_SIZE	0x60
#define	DM_ENCODE_PACKET_KEY1	0x13
#define	DM_ENCODE_PACKET_KEY2	0x1F

BYTE dm_EncodePacket_AsmCode[DM_ENCODE_PACKET_SIZE] = {
	0x53, 0x56, 0x57, 0x8b, 0x7c, 0x24, 0x10, 0x8b, 0x37, 0x8b, 0x47, 0x04, 0xc1, 0xee, 0x02, 0x8b,
	0xd6, 0x69, 0xd2, 0x44, 0x33, 0x22, 0x11, 0x8b, 0xc8, 0xc1, 0xe1, 0x07, 0x33, 0xc8, 0x05, 0x40,
	0x38, 0xaa, 0xff, 0x25, 0xcf, 0xab, 0xf5, 0x00, 0x03, 0xca, 0x8b, 0xd8, 0xba, 0x02, 0x00, 0x00,
	0x00, 0x83, 0xe3, 0x3c, 0x3b, 0xf2, 0x76, 0x1e, 0x55, 0x8d, 0xa4, 0x24, 0x00, 0x00, 0x00, 0x00,
	0x8b, 0xe8, 0xc1, 0xe5, 0x06, 0x33, 0x2c, 0x97, 0x8d, 0x04, 0x43, 0x33, 0xcd, 0x89, 0x0c, 0x97,
	0x42, 0x3b, 0xd6, 0x72, 0xeb, 0x5d, 0x5f, 0x5e, 0xb8, 0x01, 0x00, 0x00, 0x00, 0x5b, 0xc3, 0x90
};

#define	DM_DECODE_PACKET_SIZE	0x70
#define	DM_DECODE_PACKET_KEY1	0x13
#define	DM_DECODE_PACKET_KEY2	0x1F

BYTE dm_DecodePacket_AsmCode[DM_DECODE_PACKET_SIZE] = {
	0x53,0x56,0x57,0x8b,0x7c,0x24,0x14,0x8b,0x37,0x8b,0x47,0x04,0xc1,0xee,0x02,0x8b,
	0xd6,0x69,0xd2,0x34,0x34,0x00,0x00,0x8b,0xc8,0xc1,0xe1,0x05,0x33,0xc8,0x05,0x89,
	0x89,0x00,0x00,0x25,0xf5,0xaf,0xbc,0x00,0x03,0xca,0x8b,0xd8,0xba,0x02,0x00,0x00,
	0x00,0x83,0xe3,0x5e,0x3b,0xf2,0x76,0x2a,0x55,0x8d,0xa4,0x24,0x00,0x00,0x00,0x00,
	0x8b,0x2c,0x97,0x89,0x6c,0x24,0x18,0x8b,0xe8,0xc1,0xe5,0x07,0x33,0x6c,0x24,0x18,
	0x8d,0x04,0x43,0x33,0xe9,0x8b,0x4c,0x24,0x18,0x89,0x2c,0x97,0x42,0x3b,0xd6,0x72,
	0xdf,0x5d,0x5f,0x5e,0xb8,0x01,0x00,0x00,0x00,0x5b,0xc3,0x90,0x90,0x90,0x90,0x90
};

//데미지 암호화 키설정
int rsSetDamagePacketKey(rsPLAYINFO* lpPlayInfo)
{
	DWORD	dwChkSum;
	DWORD	dwObjSum;

	if (lpPlayInfo->szName[0])
		dwChkSum = GetSpeedSum(lpPlayInfo->szName);
	else
		dwChkSum = GetSpeedSum(lpPlayInfo->smCharInfo.szName);

	dwObjSum = lpPlayInfo->dwObjectSerial;

	lpPlayInfo->dwDamagePacketKey[0] = (dwChkSum * dwObjSum / 7) + 0x13;
	lpPlayInfo->dwDamagePacketKey[1] = lpPlayInfo->dwDamagePacketKey[0] ^ ((dwObjSum >> 8) + (dwObjSum << 15)) + 0x1F;

	dwObjSum *= (lpPlayInfo->smCharInfo.JOB_CODE + 2);
	lpPlayInfo->dwDamagePacketKey2[0] = (dwChkSum * dwObjSum / 11) + 0x34;
	lpPlayInfo->dwDamagePacketKey2[1] = lpPlayInfo->dwDamagePacketKey2[0] ^ ((dwObjSum >> 7) + (dwObjSum << 16)) + 0x37;

	return TRUE;
}

//데미지 암호화 펑션을 통채로 클라이언트에 보낸다
int rsSendDamageFuncToClient(rsPLAYINFO* lpPlayInfo)
{
	TRANS_FUNC_MEMORY	TransFuncMemory;

	ZeroMemory(&TransFuncMemory, sizeof(TRANS_FUNC_MEMORY));

	TransFuncMemory.size = DM_ENCODE_PACKET_SIZE + 64 + (rand() % 3) * 4;
	TransFuncMemory.code = smTRANSCODE_DAMAGE_ENCODE_MEM;

	TransFuncMemory.Param[0] = DM_ENCODE_PACKET_SIZE;

	memcpy(TransFuncMemory.szData, dm_EncodePacket_AsmCode, DM_ENCODE_PACKET_SIZE);

	rsSetDamagePacketKey(lpPlayInfo);			//패킷 암호화용 키 설정

	((DWORD*)(TransFuncMemory.szData + DM_ENCODE_PACKET_KEY1))[0] = lpPlayInfo->dwDamagePacketKey[1];
	((DWORD*)(TransFuncMemory.szData + DM_ENCODE_PACKET_KEY2))[0] = lpPlayInfo->dwDamagePacketKey[0];

	if (lpPlayInfo && lpPlayInfo->lpsmSock)
	{
		return lpPlayInfo->lpsmSock->Send((char*)&TransFuncMemory, TransFuncMemory.size, TRUE);
	}

	return FALSE;
}

//데미지 암호화 펑션을 통채로 클라이언트에 보낸다
int rsSendDamageFuncToClient2(rsPLAYINFO* lpPlayInfo)
{
	TRANS_FUNC_MEMORY	TransFuncMemory;

	ZeroMemory(&TransFuncMemory, sizeof(TRANS_FUNC_MEMORY));

	TransFuncMemory.size = DM_DECODE_PACKET_SIZE + 64 + (rand() % 3) * 4;
	TransFuncMemory.code = smTRANSCODE_DAMAGE_ENCODE_MEM2;

	TransFuncMemory.Param[0] = DM_DECODE_PACKET_SIZE;

	memcpy(TransFuncMemory.szData, dm_DecodePacket_AsmCode, DM_DECODE_PACKET_SIZE);

	rsSetDamagePacketKey(lpPlayInfo);			//패킷 암호화용 키 설정

	((DWORD*)(TransFuncMemory.szData + DM_DECODE_PACKET_KEY1))[0] = lpPlayInfo->dwDamagePacketKey2[1];
	((DWORD*)(TransFuncMemory.szData + DM_DECODE_PACKET_KEY2))[0] = lpPlayInfo->dwDamagePacketKey2[0];

	if (lpPlayInfo && lpPlayInfo->lpsmSock) {
		return lpPlayInfo->lpsmSock->Send((char*)&TransFuncMemory, TransFuncMemory.size, TRUE);
	}

	return FALSE;
}

//소켓 암호화 펑션을 통채로 클라이언트에 보낸다
int rsSendPacketFuncToClient(rsPLAYINFO* lpPlayInfo, DWORD dwEncPacketCode, BYTE bEncXor, BYTE bDecXor)
{

	TRANS_FUNC_MEMORY	TransFuncMemory;

	ZeroMemory(&TransFuncMemory, sizeof(TRANS_FUNC_MEMORY));

	TransFuncMemory.size = ENC_PACKET_ASM_SIZE + 64 + (rand() % 3) * 4;
	TransFuncMemory.code = smTRANSCODE_PACKET_DYNFUNC;

	TransFuncMemory.Param[0] = ENC_PACKET_ASM_SIZE;
	TransFuncMemory.Param[1] = POS_ENC_START;
	TransFuncMemory.Param[2] = POS_DEC_START;

	memcpy(TransFuncMemory.szData, smwsock_EncodePacket_AsmCode, ENC_PACKET_ASM_SIZE);

	((DWORD*)(TransFuncMemory.szData + POS_ENC_PACCODE))[0] = dwEncPacketCode;
	((BYTE*)(TransFuncMemory.szData + POS_ENC_XOR))[0] = bEncXor;
	((BYTE*)(TransFuncMemory.szData + POS_DEC_XOR))[0] = bDecXor;

	if (lpPlayInfo && lpPlayInfo->lpsmSock) {
		return lpPlayInfo->lpsmSock->Send((char*)&TransFuncMemory, TransFuncMemory.size, TRUE);
	}

	return FALSE;
}

#define	PACKET_SAFE_CODE		704
#define	PACKET_SAFE_SHIFT1		2
#define	PACKET_SAFE_SHIFT2		3


//암호 모듈 시행
int	rsProcessSafePacket(rsPLAYINFO* lpPlayInfo)
{

	DWORD	dwChkSum;
	DWORD	dwObjSum;
	BYTE	bEncXor, bDecXor;
	DWORD	dwEncPacketCode;

	if (!rsServerConfig.CrazyPacketMode) return FALSE;

	if (lpPlayInfo->szName[0])
		dwChkSum = GetSpeedSum(lpPlayInfo->szName);
	else
		dwChkSum = GetSpeedSum(lpPlayInfo->smCharInfo.szName);

	dwObjSum = (lpPlayInfo->dwObjectSerial + PACKET_SAFE_CODE) * dwChkSum;

	dwEncPacketCode = (dwObjSum ^ dwChkSum) & 0x0FFF0000;
	dwEncPacketCode |= smTRANSCODE_ENCODE_PACKET2;
	bEncXor = (BYTE)(dwObjSum ^ (dwChkSum >> PACKET_SAFE_SHIFT1)) & 0x7F;
	bDecXor = (BYTE)((dwObjSum >> PACKET_SAFE_SHIFT2) ^ dwChkSum) & 0x7F;

	rsSendPacketFuncToClient(lpPlayInfo, dwEncPacketCode, bDecXor, bEncXor);

	lpPlayInfo->dwDecPacketCode = dwEncPacketCode;
	lpPlayInfo->bPacketXor[0] = bEncXor;
	lpPlayInfo->bPacketXor[1] = bDecXor;

	lpPlayInfo->lpsmSock->dwEncPacketCode = smTRANSCODE_ENCODE_PACKET2;
	lpPlayInfo->lpsmSock->bEncXor = bEncXor;
	lpPlayInfo->lpsmSock->bDecXor = bDecXor;

	lpPlayInfo->dwDecPacketTime = dwPlayServTime;
	lpPlayInfo->dwDecPacketTime2 = dwPlayServTime + 60000;		//1분 동안 암호화 사용 보류 - 공격 패킷의 경우

	return TRUE;
}

//암호 모듈인지 확인하여 적용
int	rsCompareSafePacket(rsPLAYINFO* lpPlayInfo, DWORD dwRcvPacketCode)
{
	DWORD	dwChkSum;
	DWORD	dwObjSum;
	BYTE	bEncXor, bDecXor;
	DWORD	dwEncPacketCode;

	if ((dwRcvPacketCode >> 28) != 9) return FALSE;

	if (lpPlayInfo->szName[0])
		dwChkSum = GetSpeedSum(lpPlayInfo->szName);
	else
		dwChkSum = GetSpeedSum(lpPlayInfo->smCharInfo.szName);

	dwObjSum = (lpPlayInfo->dwObjectSerial + PACKET_SAFE_CODE) * dwChkSum;

	dwEncPacketCode = (dwObjSum ^ dwChkSum) & 0x0FFF0000;
	dwEncPacketCode |= smTRANSCODE_ENCODE_PACKET2;
	bEncXor = (BYTE)(dwObjSum ^ (dwChkSum >> PACKET_SAFE_SHIFT1)) & 0x7F;
	bDecXor = (BYTE)((dwObjSum >> PACKET_SAFE_SHIFT2) ^ dwChkSum) & 0x7F;

	if (dwRcvPacketCode == dwEncPacketCode) {
		lpPlayInfo->dwDecPacketCode = dwEncPacketCode;
		lpPlayInfo->bPacketXor[0] = bEncXor;
		lpPlayInfo->bPacketXor[1] = bDecXor;

		lpPlayInfo->lpsmSock->dwEncPacketCode = smTRANSCODE_ENCODE_PACKET2;
		lpPlayInfo->lpsmSock->bEncXor = bEncXor;
		lpPlayInfo->lpsmSock->bDecXor = bDecXor;

		return TRUE;
	}

	return FALSE;
}

//데미지 암호 풀기 함수
int	rsDecodeDamagePacket(rsPLAYINFO* lpPlayInfo, void* lpPacket)
{
	DWORD size, code;
	DWORD cnt, cnt2, cnt3;
	DWORD dwCode;
	DWORD dwCode2;
	DWORD* lpDword = (DWORD*)lpPacket;

	size = lpDword[0];
	code = lpDword[1];
	size /= 4;

	dwCode = (code ^ (code << 7)) + size * lpPlayInfo->dwDamagePacketKey[1];
	cnt2 = (code + lpPlayInfo->dwDamagePacketKey[0]) & 0xF5ABCF;
	cnt3 = cnt2 & 0x3C;

	for (cnt = 2; cnt < size; cnt++)
	{
		dwCode2 = lpDword[cnt];
		lpDword[cnt] = dwCode2 ^ (cnt2 << 6) ^ dwCode;
		dwCode = dwCode2;
		cnt2 += cnt2 + cnt3;
	}

	return TRUE;
}

int	rsEncodeDamagePacket(rsPLAYINFO* lpPlayInfo, void* lpPacket)
{
	DWORD size, code;
	DWORD cnt, cnt2, cnt3;
	DWORD dwCode;
	DWORD* lpDword = (DWORD*)lpPacket;

	size = lpDword[0];
	code = lpDword[1];
	size /= 4;


	dwCode = (code ^ (code << 5)) + size * lpPlayInfo->dwDamagePacketKey2[1];
	cnt2 = (code + lpPlayInfo->dwDamagePacketKey2[0]) & 0xBCAFF5;
	cnt3 = cnt2 & 0x5E;

	for (cnt = 2; cnt < size; cnt++)
	{
		dwCode = dwCode ^ (cnt2 << 7) ^ lpDword[cnt];
		lpDword[cnt] = dwCode;
		cnt2 += cnt2 + cnt3;
	}

	return TRUE;
}


#else

//데미지 암호화 키설정
int rsSetDamagePacketKey(rsPLAYINFO* lpPlayInfo)
{
	return TRUE;
}

//데미지 암호화 펑션을 통채로 클라이언트에 보낸다
int rsSendDamageFuncToClient(rsPLAYINFO* lpPlayInfo)
{
	return TRUE;
}
//데미지 암호 풀기 함수
int	rsDecodeDamagePacket(rsPLAYINFO* lpPlayInfo, void* lpPacket)
{
	return TRUE;
}
int	rsEncodeDamagePacket(rsPLAYINFO* lpPlayInfo, void* lpPacket)
{
	return TRUE;
}

#endif



//데미지 데이타 오류를 군서버 로그에 기록
int rsRecordDamageError(rsPLAYINFO* lpPlayInfo, smTRANS_COMMAND* lpTransCommand)
{

	if (lpPlayInfo->WarningCount < 20) {
		rsSendDataServer(lpPlayInfo->lpsmSock, lpTransCommand);
		lpPlayInfo->WarningCount++;
	}

	return TRUE;
}


//ATTACK_DAMAGE_LIST_MAX
/*
//데미지를 준 유저를 기억시킴
struct	ATTACK_DAMAGE_LIST	{
	DWORD	dwUserCode;
	int		DamageCount;
};
*/

extern std::unordered_map<DWORD, std::unordered_map<rsPLAYINFO*, DWORD>> DamageListNew;

//캐릭터 데미지 기록
int	rsRecordCharDamage(rsPLAYINFO* lpPlayInfo, smCHAR* lpChar, int Damage)
{
	int cnt;
	int EmptyNum = -1;
	DWORD	dwClanCode;

	DamageListNew[lpChar->dwObjectSerial][lpPlayInfo] += Damage;

	if (lpChar->lpAttackDamageList_BlessCastle) {
		//블레스 캐슬 크리스탈 타워 ( 클랜순위 )
		if (lpPlayInfo->dwClanCode && lpPlayInfo->Position.Area == rsCASTLE_FIELD) {

			lpPlayInfo->sBlessCastle_Damage[0] += Damage;	//데미지 정보 추가

			for (cnt = 0; cnt < ATTACK_DAMAGE_LIST_MAX; cnt++) {
				if (lpChar->lpAttackDamageList_BlessCastle[cnt].dwUserCode) {
					dwClanCode = GetClanCode(lpChar->lpAttackDamageList_BlessCastle[cnt].dwUserCode);
					if (dwClanCode == lpPlayInfo->dwClanCode) {
						lpChar->lpAttackDamageList_BlessCastle[cnt].DamageCount += Damage;
						lpChar->lpAttackDamageList_BlessCastle[cnt].Count++;
						return TRUE;
					}
				}
				if (EmptyNum < 0 && lpChar->lpAttackDamageList_BlessCastle[cnt].dwUserCode == 0) {
					EmptyNum = cnt;
				}

			}

			if (EmptyNum >= 0) {
				lpChar->lpAttackDamageList_BlessCastle[EmptyNum].dwUserCode = lpPlayInfo->smCharInfo.ClassClan;
				lpChar->lpAttackDamageList_BlessCastle[EmptyNum].DamageCount = Damage;
				lpChar->lpAttackDamageList_BlessCastle[EmptyNum].Count++;

				return TRUE;
			}
		}
		return TRUE;
	}

	if (!lpChar->lpAttackDamageList) return FALSE;

	if (lpChar->smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_TOWER_B) {
		//발할라 탑 ( 클랜순위 )
		if (lpPlayInfo->dwClanCode && lpPlayInfo->Position.Area == rsCASTLE_FIELD) {

			lpPlayInfo->sBlessCastle_Damage[0] += Damage;	//데미지 정보 추가

			for (cnt = 0; cnt < ATTACK_DAMAGE_LIST_MAX; cnt++) {
				if (lpChar->lpAttackDamageList[cnt].dwUserCode) {
					dwClanCode = GetClanCode(lpChar->lpAttackDamageList[cnt].dwUserCode);
					if (dwClanCode == lpPlayInfo->dwClanCode) {
						lpChar->lpAttackDamageList[cnt].DamageCount += Damage;
						lpChar->lpAttackDamageList[cnt].Count++;
						return TRUE;
					}
				}
				if (EmptyNum < 0 && lpChar->lpAttackDamageList[cnt].dwUserCode == 0) {
					EmptyNum = cnt;
				}

			}

			if (EmptyNum >= 0) {
				lpChar->lpAttackDamageList[EmptyNum].dwUserCode = lpPlayInfo->smCharInfo.ClassClan;
				lpChar->lpAttackDamageList[EmptyNum].DamageCount = Damage;
				lpChar->lpAttackDamageList[EmptyNum].Count++;

				return TRUE;
			}
		}
	}
	else {
		//일반 순위 (개인)
		for (cnt = 0; cnt < ATTACK_DAMAGE_LIST_MAX; cnt++) {
			if (lpChar->lpAttackDamageList[cnt].dwUserCode == lpPlayInfo->dwObjectSerial) {
				lpChar->lpAttackDamageList[cnt].DamageCount += Damage;
				lpChar->lpAttackDamageList[cnt].Count++;
				return TRUE;
			}
			if (EmptyNum < 0 && lpChar->lpAttackDamageList[cnt].dwUserCode == 0) {
				EmptyNum = cnt;
			}
		}

		if (EmptyNum >= 0) {
			lpChar->lpAttackDamageList[EmptyNum].dwUserCode = lpPlayInfo->dwObjectSerial;
			lpChar->lpAttackDamageList[EmptyNum].DamageCount = Damage;
			lpChar->lpAttackDamageList[EmptyNum].Count++;

			return TRUE;
		}
	}

	return FALSE;
}

int	rsRecord_PKDamage(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, int Damage)
{
	int cnt;
	ATTACK_DAMAGE_LIST* lpAttackDamageList = 0;
	DWORD dwClanCode;
	int EmptyNum = -1;


	if (!lpPlayInfo2->dwClanCode || lpPlayInfo2->dwClanCode == lpPlayInfo->dwClanCode) {
		if (lpPlayInfo2->dwClanCode != rsBlessCastle.dwMasterClan && lpPlayInfo->dwClanCode != rsBlessCastle.dwMasterClan)
		{
			return FALSE;
		}
	}

	if (rsBlessCastle.lpAttackDamageList) {
		lpAttackDamageList = rsBlessCastle.lpAttackDamageList;
	}
	else if (rsBlessCastle.lpChar_ValhallaTower) {
		if (rsBlessCastle.lpChar_ValhallaTower->Flag &&
			rsBlessCastle.lpChar_ValhallaTower->smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_TOWER_B &&
			rsBlessCastle.lpChar_ValhallaTower->smCharInfo.Life[0] > 0)
		{
			lpAttackDamageList = rsBlessCastle.lpChar_ValhallaTower->lpAttackDamageList;
		}
	}

	if (!lpAttackDamageList) return FALSE;

	Damage -= lpPlayInfo->smCharInfo.Absorption;
	Damage /= PK_SCORE_DIVIDE;
	if (Damage <= 0) {
		Damage = 1;
	}

	lpPlayInfo->sBlessCastle_Damage[0] += Damage;

	if (lpPlayInfo->dwClanCode && lpPlayInfo->Position.Area == rsCASTLE_FIELD) {

		for (cnt = 0; cnt < ATTACK_DAMAGE_LIST_MAX; cnt++) {
			if (lpAttackDamageList[cnt].dwUserCode) {
				dwClanCode = GetClanCode(lpAttackDamageList[cnt].dwUserCode);
				if (dwClanCode == lpPlayInfo->dwClanCode) {
					lpAttackDamageList[cnt].DamageCount += Damage;
					lpAttackDamageList[cnt].Count++;
					return TRUE;
				}
			}
			if (EmptyNum < 0 && lpAttackDamageList[cnt].dwUserCode == 0) {
				EmptyNum = cnt;
			}
		}
		if (EmptyNum >= 0) {
			lpAttackDamageList[EmptyNum].dwUserCode = lpPlayInfo->smCharInfo.ClassClan;
			lpAttackDamageList[EmptyNum].DamageCount = Damage;
			lpAttackDamageList[EmptyNum].Count++;

			return TRUE;
		}
	}

	return TRUE;
}

int	rsBillingItemSkill(rsPLAYINFO* lpPlayInfo, smCHAR* lpChar, int HitMonsters, int Power, int SkillCode)
{
	float fps, fp1, fp2;
	smTRANS_COMMAND	smTransCommand;
	DWORD	dwServTime_T = (DWORD)tServerTime;

	if (SkillCode == SKILL_PLAY_ROAR || SkillCode == SKILL_PLAY_METAL_GOLEM || SkillCode == SKILL_PLAY_COMPULSION || SkillCode == SKILL_PLAY_EXTINCTION ||
		SkillCode == SKILL_PLAY_RECALL_WOLVERIN || SkillCode == SKILL_PLAY_FIRE_ELEMENTAL || SkillCode == SKILL_PLAY_DISTORTION ||
		SkillCode == SKILL_PLAY_PET_ATTACK || SkillCode == SKILL_PLAY_PET_ATTACK2 || lpPlayInfo->Position.Area == rsCASTLE_FIELD)
	{
		return FALSE;
	}

	if (!lpChar) return FALSE;

	if (lpChar) {
		if (lpChar->smCharInfo.State == smCHAR_STATE_NPC || lpChar->smCharInfo.Brood == smCHAR_MONSTER_USER)
			return FALSE;
	}

	// Efeito da presa vampirica aqui
	if (lpPlayInfo->dwTime_PrimeItem_VampCuspid > 0) {
		fp1 = float(lpPlayInfo->smCharInfo.Strength + (((float)Power) / 10.0f)) / 20.0f;
		fp2 = (1.0f + (((float)HitMonsters) / 5.0f));
		fps = fp1 * fp2 * HitMonsters + 1.0f;

		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_VAMPRIC_CUSPID;
		smTransCommand.WParam = (int)fps;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = lpPlayInfo->dwObjectSerial;

		if (smTransCommand.WParam)
			lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
	}

	// Efeito super presa vampirica aqui
	if (lpPlayInfo->dwTime_PrimeItem_VampCuspid_EX > 0)
	{
		fp1 = float(lpPlayInfo->smCharInfo.Strength + (((float)Power) / 10.0f)) / 15.0f;
		fp2 = (1.0f + (((float)HitMonsters) / 7.0f));
		fps = fp1 * fp2 * HitMonsters + 1.0f;

		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_VAMPRIC_CUSPID_EX;
		smTransCommand.WParam = (int)fps;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = lpPlayInfo->dwObjectSerial;

		if (smTransCommand.WParam)
			lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
	}


	// Efeito drena almas aqui
	if (lpPlayInfo->dwTime_PrimeItem_ManaRecharg > 0) {
		fp1 = float(lpPlayInfo->smCharInfo.Spirit + (((float)Power) / 10.0f)) / 13.0f;
		fp2 = (1.0f + (((float)HitMonsters) / 5.0f));
		fps = fp1 * fp2 * HitMonsters + 1.0f;

		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_MANA_RECHARGING;
		smTransCommand.WParam = (int)fps;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = lpPlayInfo->dwObjectSerial;

		if (smTransCommand.WParam)
			lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
	}

	return TRUE;
}

