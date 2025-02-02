#include "UILinker.h"

UINewListBox::UINewListBox(int ID, int x, int y, int w, int h)
{
	setID(ID);
	setX(x);
	setY(y);
	setWidth(w);
	setHeight(h);
}

UINewListBox::~UINewListBox()
{
	for (auto pListItem : vListItems)
		if (pListItem)
			delete pListItem;
}

void UINewListBox::Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
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
		int iRenderX = cX + iX;
		int iRenderY = cY + iY;
		int iRenderWidth = low(cWidth, iWidth);
		int iRenderHeight = low(cHeight, iHeight);

		if (cX + iRenderWidth > iWidth)
			iRenderWidth -= (cX + iRenderWidth) - iWidth;

		if (cY + iRenderHeight > iHeight)
			iRenderHeight -= (cY + iRenderHeight) - iHeight;

		for (auto pListItem : vListItems)
		{
			pListItem->Render(iRenderX, iRenderY, iRenderWidth, iRenderHeight, iStartX, iStartY);
			pListItem->RenderWindows(iX, iY, iWidth, iHeight, iStartX, iStartY);
		}
	}
}

bool UINewListBox::OnMouseClick(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
{
	bool bRet = false;
	if (isVisible())
	{
		if (iWidth <= 0 || iHeight <= 0)
			return false;

		int cX = getX();
		int cY = getY();
		int cWidth = getWidth();
		int cHeight = getHeight();

		int iScroll = pVerticalScroll && pVerticalScroll->isVisible() ? pVerticalScroll->getValue() : 0;

		int iStartX = cX < iSourceX ? abs(cX - iSourceX) : 0;
		int iStartY = cY < iSourceY ? abs(cY - iSourceY) : 0;
		int iRenderX = cX + iX + iStartX;
		int iRenderY = cY + iY + iStartY;
		int iRenderWidth = low(cWidth, iWidth);
		int iRenderHeight = low(cHeight, iHeight);

		if(cX + iRenderWidth > iWidth)
			iRenderWidth -= (cX+ iRenderWidth) - iWidth;

		if (cY + iRenderHeight > iHeight)
			iRenderHeight -= (cY + iRenderHeight) - iHeight;

		if (isMousePos(iRenderX, iRenderY - iStartY, iRenderWidth, iRenderHeight))
		{
			auto pLastSelectedItem = pSelectedItem;
			for (auto pListItem : vListItems)
			{
				if (pListItem->OnMouseClick(Type, iRenderX, iRenderY - iScroll, iRenderWidth, iRenderHeight + iScroll, 0, iScroll))
				{
					if(Type == Down)
						pSelectedItem = pListItem;
					break;
				}
			}

			if (pLastSelectedItem != pSelectedItem)
			{
				for (auto pListItem : vListItems)
				{
					if (pListItem == pSelectedItem)
						pListItem->OnSelect();
					else
						pListItem->OnUnselect();
				}
			}
			bRet = true;
		}

		if (pVerticalScroll)
			if (pVerticalScroll->OnMouseClick(Type, iRenderX, iRenderY, iRenderWidth, iRenderHeight, iStartX, iStartY))
				bRet = true;

		for (auto pListItem : vListItems)
		{
			if (pListItem->OnMouseClickWindows(Type, iX, iY, iWidth, iHeight, 0, 0))
				bRet = true;
		}
	}
	return bRet;
}

bool UINewListBox::OnMouseMove(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
{
	if (isVisible())
	{
		if (iWidth <= 0 || iHeight <= 0)
			return false;

		int cX = getX();
		int cY = getY();
		int cWidth = getWidth();
		int cHeight = getHeight();

		int iScroll = pVerticalScroll && pVerticalScroll->isVisible() ? pVerticalScroll->getValue() : 0;

		int iStartX = cX < iSourceX ? abs(cX - iSourceX) : 0;
		int iStartY = cY < iSourceY ? abs(cY - iSourceY) : 0;
		int iRenderX = cX + iX + iStartX;
		int iRenderY = cY + iY + iStartY;
		int iRenderWidth = low(cWidth, iWidth);
		int iRenderHeight = low(cHeight, iHeight);

		if(cX + iRenderWidth > iWidth)
			iRenderWidth -= (cX+ iRenderWidth) - iWidth;

		if (cY + iRenderHeight > iHeight)
			iRenderHeight -= (cY + iRenderHeight) - iHeight;

		if (pVerticalScroll)
			pVerticalScroll->OnMouseMove(iRenderX, iRenderY, iRenderWidth, iRenderHeight, iStartX, iStartY);

		if (isMousePos(iRenderX, iRenderY, iRenderWidth, iRenderHeight))
		{
			auto pLastSelectedItem = pSelectedItem;
			for (auto pListItem : vListItems)
			{
				pListItem->OnMouseMove(iRenderX, iRenderY - iScroll, iRenderWidth, iRenderHeight + iScroll, 0, iScroll);
			}
			return true;
		}

		for (auto pListItem : vListItems)
		{
			pListItem->OnMouseMoveWindows(iX, iY, iWidth, iHeight, 0, 0);
		}
	}
	return false;
}

bool UINewListBox::OnMouseScroll(int zDelta, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
{
	if (isVisible())
	{
		if (iWidth <= 0 || iHeight <= 0)
			return false;

		int cX = getX();
		int cY = getY();
		int cWidth = getWidth();
		int cHeight = getHeight();

		int iScroll = pVerticalScroll && pVerticalScroll->isVisible() ? pVerticalScroll->getValue() : 0;

		int iStartX = cX < iSourceX ? abs(cX - iSourceX) : 0;
		int iStartY = cY < iSourceY ? abs(cY - iSourceY) : 0;
		int iRenderX = cX + iX + iStartX;
		int iRenderY = cY + iY + iStartY;
		int iRenderWidth = low(cWidth, iWidth);
		int iRenderHeight = low(cHeight, iHeight);

		if(cX + iRenderWidth > iWidth)
			iRenderWidth -= (cX+ iRenderWidth) - iWidth;

		if (cY + iRenderHeight > iHeight)
			iRenderHeight -= (cY + iRenderHeight) - iHeight;

		if (isMousePos(iRenderX, iRenderY, iRenderWidth, iRenderHeight))
		{
			auto pLastSelectedItem = pSelectedItem;
			for (auto pListItem : vListItems)
			{
				pListItem->OnMouseScroll(zDelta, iRenderX, iRenderY - iScroll, iRenderWidth, iRenderHeight + iScroll, 0, iScroll);
			}

			if (pVerticalScroll)
				pVerticalScroll->OnMouseScroll(zDelta, iRenderX, iRenderY, iRenderWidth, iRenderHeight, iStartX, iStartY);
			return true;
		}

		for (auto pListItem : vListItems)
		{
			pListItem->OnMouseScrollWindows(zDelta, iX, iY, iWidth, iHeight, 0, 0);
		}
	}
	return false;
}

bool UINewListBox::OnKeyDown(char Code)
{
	bool bRet = false;
	for (auto pListItem : vListItems)
		if (pListItem->OnKeyDown(Code))
			bRet = true;
	return bRet;
}

bool UINewListBox::OnKeyChar(char Code)
{
	bool bRet = false;
	for (auto pListItem : vListItems)
		if (pListItem->OnKeyChar(Code))
			bRet = true;
	return bRet;
}

void UINewListBox::selectItem(int iID)
{
	for (auto pListItem : vListItems)
	{
		if (pListItem->getID() == iID)
		{
			pSelectedItem = pListItem;
			pSelectedItem->OnSelect();
		}
		else
			pListItem->OnUnselect();
	}
}

void UINewListBox::SetScrollBar(UINewVerticalScroll * pScroll)
{
	pVerticalScroll = pScroll;
}

void UINewListBox::AddListItem(UINewListItem* pListItem)
{
	if (pListItem->getY() + pListItem->getHeight() > iMaxY)
	{
		iMaxY = pListItem->getY() + pListItem->getHeight();

		if (iMaxY > getHeight())
		{
			if (pVerticalScroll)
			{
				pVerticalScroll->setMax(iMaxY - getHeight());
				pVerticalScroll->updateButtonPosByValue();
				pVerticalScroll->show();
			}
		}
	}

	vListItems.push_back(pListItem);
}

void UINewListBox::ClearList()
{
	for (auto listItem : vListItems)
	{
		if (listItem)
			delete listItem;
	}

	vListItems.clear();
	pSelectedItem = 0;
}
