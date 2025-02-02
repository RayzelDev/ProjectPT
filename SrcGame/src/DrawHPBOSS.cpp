#include "sinbaram\\sinLinkHeader.h"
#include "DrawHPBOSS.h"



void cDrawHPBOSS::Init()//EXTREMEZ HUD DOS BOSS
{

    Images[5] = CreateTextureMaterial("game\\images\\UI\\target\\life.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
    Images[6] = CreateTextureMaterial("game\\images\\UI\\target\\life_.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
}


void cDrawHPBOSS::DrawHP(smCHAR* pcUnitData)
{
    char		szBuff[255];
    int x = (WinSizeX - 474) / 2;
    int y = 25;

         dsDrawTexImage(Images[5], x, y, 474, 58, 255);
         dsDrawTexImage(Images[6], x + 43, y + 22, (386 * (float)((float)pcUnitData->smCharInfo.Life[0] / (float)pcUnitData->smCharInfo.Life[1])), 16, 255);
       

         float fPercent = (float)((float)pcUnitData->smCharInfo.Life[0] / (float)pcUnitData->smCharInfo.Life[1]);

         sprintf(szBuff, "%.2f%%", fPercent * 100);
         SetTextColor(0, RGB(255, 255, 255));
         dsTextLineOut(x + 50, y + 21, pcUnitData->smCharInfo.szName, lstrlen(pcUnitData->smCharInfo.szName));
         dsTextLineOut(x + 367, y + 21, szBuff, lstrlen(szBuff));
           
}

