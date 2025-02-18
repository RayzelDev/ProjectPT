#include "UILinker.h"

UINewListItem::UINewListItem(int ID, char * path, char * path_, int x, int y, bool hover, bool selected)
{
	Image = new UINewImage(ID, path, 0, 0);

	if (!Image)
		return;

	Image_ = new UINewImage(ID, path_, 0, 0);
	setWidth(Image->getWidth());
	setHeight(Image->getHeight());

	setID(ID);
	setX(x);
	setY(y);

	setHover(hover);
	setSelected(selected);
}

void UINewListItem::Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
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

		if (((isMousePos(iRenderX, iRenderY, iRenderWidth, iRenderHeight) && getHover()) || getSelected()) && Image_)
		{
			Image_->Render(iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY);
		}
		else
		{
			Image->Render(iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY);
		}
	}
}

void UINewListItem::RenderWindows(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
{
	if (isVisible())
	{
		if (!getSelected())
			return;

		for (auto pElement : vElements)
			pElement->Render(iX, iY, iWidth, iHeight, iSourceX, iSourceY);
	}
}

bool UINewListItem::OnMouseClick(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
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

		if(cX + iRenderWidth > iWidth)
			iRenderWidth -= (cX+ iRenderWidth) - iWidth;

		if (cY + iRenderHeight > iHeight)
			iRenderHeight -= (cY + iRenderHeight) - iHeight;

		if (isMousePos(iRenderX, iRenderY - iStartY, iRenderWidth, iRenderHeight))
		{
			return true;
		}
	}
	return false;
}

bool UINewListItem::OnMouseClickWindows(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
{
	bool bRet = false;
	if (isVisible())
	{
		if (!getSelected())
			return false;

		for (auto pElement : vElements)
			if (pElement->OnMouseClick(Type, iX, iY, iWidth, iHeight, iSourceX, iSourceY))
				bRet = true;
	}

	return bRet;
}

bool UINewListItem::OnMouseMoveWindows(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
{
	if (isVisible())
	{
		if (!getSelected())
			return false;

		for (auto pElement : vElements)
			pElement->OnMouseMove(iX, iY, iWidth, iHeight, iSourceX, iSourceY);
	}
	return false;
}

bool UINewListItem::OnMouseScrollWindows(int zDelta, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
{
	if (isVisible())
	{
		if (!getSelected())
			return false;

		for (auto pElement : vElements)
			pElement->OnMouseScroll(zDelta, iX, iY, iWidth, iHeight, iSourceX, iSourceY);
	}
	return false;
}

bool UINewListItem::OnKeyDown(char Code)
{
	bool bRet = false;
	for (auto pElement : vElements)
		if (pElement->OnKeyDown(Code))
			bRet = true;
	return bRet;
}

bool UINewListItem::OnKeyChar(char Code)
{
	bool bRet = false;
	for (auto pElement : vElements)
		if (pElement->OnKeyChar(Code))
			bRet = true;
	return bRet;
}

void UINewListItem::RegisterEvent(ListNewEvents eEvent, std::function<void()> callback)
{
	switch (eEvent)
	{
	case LISTEVENT_Select:
		SelectCallback = callback;
		break;

	case LISTEVENT_Unselect:
		UnselectCallback = callback;
		break;
	}
}

void UINewListItem::OnSelect()
{
	if (getSelected())
		return;

	setSelected(true);

	for (auto pElement : vElements)
		pElement->show();

	if (SelectCallback)
		SelectCallback();
}

void UINewListItem::OnUnselect()
{
	if (!getSelected())
		return;

	setSelected(false);

	for (auto pElement : vElements)
		pElement->hide();

	if (UnselectCallback)
		UnselectCallback();
}

void UINewListItem::setSelectedImage(char * path)
{
	_Image = new UINewImage(0, path, 0, 0);
}

void UINewListItem::AddElement(NewElement * pElement)
{
	vElements.push_back(pElement);
}
