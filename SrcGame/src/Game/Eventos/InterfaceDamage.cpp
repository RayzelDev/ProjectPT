#include "sinbaram/sinLinkHeader.h"
#include "InterfaceDamage.h"
#include <TitleBox/TitleBox.h>

INTERFACEDAMAGE InterfaceDamage;

void INTERFACEDAMAGE::Init()
{
	InterfaceDamageImg[0] = CreateTextureMaterial("game\\images\\damageHUD\\window.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	InterfaceDamageImg[1] = CreateTextureMaterial("game\\images\\damageHUD\\Buy.bmp", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	fOpen = true;
}

BOOL INTERFACEDAMAGE::IsMousePos(int x, int y, int w, int h)
{
	if (pRealCursorPos.x > x && pRealCursorPos.x < x + w && pRealCursorPos.y > y && pRealCursorPos.y < y + h)
		return TRUE;

	return FALSE;

}

void INTERFACEDAMAGE::AtualizarDamage()
{
	char szBuff[256];
	SIZE size = {};

	DrawImage(InterfaceDamageImg[0], BaseX + 15, BaseY +52, 181, 32);
	DrawImage(InterfaceDamageImg[1], BaseX + 155, BaseY + 80, 28, 28); 
	
	/*wsprintf(szBuff, "Jogador: %s", lpCurPlayer->smCharInfo.szName);
	GetFontTextExtentPoint(szBuff, strlen(szBuff), &size);
	dsTextLineOut(BaseX + (131 - size.cx / 2), BaseY + 125, szBuff, strlen(szBuff));*/

	char szTemp[32] = { 0 };
	NumLineComa64(damage, szTemp);

	wsprintf(szBuff, "%s", szTemp);
	GetFontTextExtentPoint(szBuff, strlen(szBuff), &size);
	dsTextLineOut(BaseX + (105 - size.cx / 2), BaseY + 60, szBuff, strlen(szBuff));
}

void INTERFACEDAMAGE::DrawImage(int Image, int x, int y, int w, int h)
{
	dsDrawTexImage(Image, x, y, w - 1, h - 1, 255);
}

void INTERFACEDAMAGE::Draw()
{
	if (fOpen)
	{
		char szBuff[256];
		SIZE size = {};

		DrawImage(InterfaceDamageImg[0], BaseX + 15, BaseY + 52, 181, 32);
		DrawImage(InterfaceDamageImg[1], BaseX + 155, BaseY + 80, 28, 28);

		/*wsprintf(szBuff, "Jogador: %s", lpCurPlayer->smCharInfo.szName);
		GetFontTextExtentPoint(szBuff, strlen(szBuff), &size);
		dsTextLineOut(BaseX + (131 - size.cx / 2), BaseY + 125, szBuff, strlen(szBuff));*/

		wsprintf(szBuff, "%d", damage);
		GetFontTextExtentPoint(szBuff, strlen(szBuff), &size);
		dsTextLineOut(BaseX + (105 - size.cx / 2), BaseY + 60, szBuff, strlen(szBuff));
	}
}

void INTERFACEDAMAGE::Button()
{
	
}