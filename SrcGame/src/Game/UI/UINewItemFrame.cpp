#include "UILinker.h"

UINewItemFrame::UINewItemFrame(int ID, int x, int y, int w, int h)
{
	ZeroMemory(&Item, sizeof(sITEM));
	setID(ID);
	setX(x);
	setY(y);
	setWidth(w);
	setHeight(h);
}

void UINewItemFrame::Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
{
	if (isVisible())
	{
		if (iWidth <= 0 || iHeight <= 0)
			return;

		int cX = getX();
		int cY = getY();
		int cWidth = getWidth();
		int cHeight = getHeight();
		int iStartX = cX < iSourceX ? abs(cX - iSourceX) : 0;
		int iStartY = cY < iSourceY ? abs(cY - iSourceY) : 0;

		int iRenderX = cX + iX + iStartX;
		int iRenderY = cY + iY + iStartY;
		int iRenderWidth = low(cWidth, iWidth);
		int iRenderHeight = low(cHeight, iHeight);

		if (cX + iRenderWidth > iWidth)
			iRenderWidth -= (cX + iRenderWidth) - iWidth;

		if (cY + iRenderHeight > iHeight)
			iRenderHeight -= (cY + iRenderHeight) - iHeight;

		if (isMousePos(iRenderX, iRenderY - iStartY, iRenderWidth, iRenderHeight))
		{
			if (MouseItem.Flag)
			{
				if (getPutItem())
					dsDrawColorBox(D3DCOLOR_RGBA(160, 235, 256, 128), iRenderX, iRenderY, iRenderWidth, iRenderHeight);
				else
					dsDrawColorBox(D3DCOLOR_RGBA(255, 0, 0, 128), iRenderX, iRenderY, iRenderWidth, iRenderHeight);
			}

			if (Item.Flag)
			{
				Item.x = pCursorPos.x;
				Item.y = pCursorPos.y;
				sinShowItemInfoFlag = 1;
				GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&Item, FALSE, FALSE, FALSE);

				
			}
		}

		if (Item.Flag)
			DrawSprite(iRenderX + ((getWidth() - Item.w) / 2), iRenderY + ((getHeight() - Item.h) / 2), Item.lpItem, 0, 0, Item.w, Item.h);
	}
}

bool UINewItemFrame::OnMouseClick(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
{
	if (isVisible() && Type != Up)
	{
		if (iWidth <= 0 || iHeight <= 0)
			return false;

		int cX = getX();
		int cY = getY();
		int cWidth = getWidth();
		int cHeight = getHeight();

		int iStartX = cX < iSourceX ? abs(cX - iSourceX) : 0;
		int iStartY = cY < iSourceY ? abs(cY - iSourceY) : 0;
		int iRenderX = cX + iX + iStartX;
		int iRenderY = cY + iY + iStartY;
		int iRenderWidth = low(cWidth, iWidth);
		int iRenderHeight = low(cHeight, iHeight);

		if (cX + iRenderWidth > iWidth)
			iRenderWidth -= (cX + iRenderWidth) - iWidth;

		if (cY + iRenderHeight > iHeight)
			iRenderHeight -= (cY + iRenderHeight) - iHeight;

		if (isMousePos(iRenderX, iRenderY - iStartY, iRenderWidth, iRenderHeight) && getPutItem())
		{
			if (MouseItem.Flag)
				setItem(&MouseItem);
			else
				pickItem(&MouseItem);
			return true;
		}
	}

	return false;
}

bool UINewItemFrame::OnMouseMove(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
{
	if (isVisible())
	{
		if (iWidth <= 0 || iHeight <= 0)
			return false;

		int cX = getX();
		int cY = getY();
		int cWidth = getWidth();
		int cHeight = getHeight();

		int iStartX = cX < iSourceX ? abs(cX - iSourceX) : 0;
		int iStartY = cY < iSourceY ? abs(cY - iSourceY) : 0;
		int iRenderX = cX + iX + iStartX;
		int iRenderY = cY + iY + iStartY;
		int iRenderWidth = low(cWidth, iWidth);
		int iRenderHeight = low(cHeight, iHeight);

		if (cX + iRenderWidth > iWidth)
			iRenderWidth -= (cX + iRenderWidth) - iWidth;

		if (cY + iRenderHeight > iHeight)
			iRenderHeight -= (cY + iRenderHeight) - iHeight;

		if (isMousePos(iRenderX, iRenderY - iStartY, iRenderWidth, iRenderHeight))
		{
			if (fHover)
				fHover();
			return true;
		}
	}

	return false;
}

void UINewItemFrame::setItem(sITEM * pItem)
{
	if (!pItem || !pItem->Flag)
		return;

	if (Item.Flag)
	{
		sITEM bkp;
		memcpy(&bkp, &Item, sizeof(sITEM));
		memcpy(&Item, pItem, sizeof(sITEM));
		memcpy(pItem, &bkp, sizeof(sITEM));
	}
	else
	{
		memcpy(&Item, pItem, sizeof(sITEM));
		ZeroMemory(pItem, sizeof(sITEM));
	}

	if (!Item.lpItem)
	{
		Item.lpItem = LoadDibSurfaceOffscreen((char*)FormatString("image\\Sinimage\\Items\\%s\\it%s.bmp", Item.ItemFilePath, Item.LastCategory));
	}

	if (fPutItem)
		fPutItem();

	ResetInvenItemCode();
}

void UINewItemFrame::pickItem(sITEM * pItem)
{
	memcpy(pItem, &Item, sizeof(sITEM));
	ZeroMemory(&Item, sizeof(sITEM));

	if (fPickItem)
		fPickItem();

	ResetInvenItemCode();
}
