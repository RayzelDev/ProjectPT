#include "UILinker.h"

UINewButton::UINewButton(int ID, char* path, char* path_, int x, int y, bool hover, bool selected, std::function<void()> callback)
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
	Activation = callback;

	setHover(hover);
	setSelected(selected);

	Caption = 0;

	if (getSelected() && Activation)
		Activation();
}

void UINewButton::Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
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

		if (((isMousePos(iRenderX, iRenderY - iStartY, iRenderWidth, iRenderHeight) && getHover()) || getSelected()) && Image_)
		{
			Image_->Render(iRenderX, iRenderY - iStartY, iRenderWidth, iRenderHeight, iStartX, iStartY);
			if (isMousePos(iRenderX, iRenderY - iStartY, iRenderWidth, iRenderHeight) && fHover)
				fHover();
		}
		else
		{
			Image->Render(iRenderX, iRenderY - iStartY, iRenderWidth, iRenderHeight, iStartX, iStartY);
		}
	}
}

bool UINewButton::OnMouseClick(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
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

		if (isMousePos(iRenderX, iRenderY - iStartY, iRenderWidth, iRenderHeight))
		{
			if (Activation)
				Activation();
			return true;
		}
	}
	return false;
}

void UINewButton::setDisabledImage(char * path)
{
	_Image = new UINewImage(0, path, 0, 0);
}

void UINewButton::setCaption(UINewText * pCaption)
{
	Caption = pCaption;
}
