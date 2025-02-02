//#include "Pch.h"
#include "UILinker.h"


bool skipClick = false;

UINewDropdown::UINewDropdown(int ID, int x, int y, char* base, char* top, char* mid, char* bot)
{
	setID(ID);
	setX(x);
	setY(y);
	Base = 0;

	if (strlen(base) > 0)
	{
		Base = new UINewImage(0, base, 0, 0);
		if (Base)
		{
			setWidth(Base->getWidth());
			setHeight(Base->getHeight());
		}
	}

	Top = new UINewImage(0, top, 0, 0);
	Mid = new UINewImage(0, mid, 0, Top->getHeight());
	Mid->setHeight(0);
	Mid->setDrawStyle(EDRAW_Repeat);
	Bot = new UINewImage(0, bot, 0, Mid->getHeight() + Mid->getY());
	SelectedText = new UINewText(0, "", 0, 0);
}

UINewDropdown::~UINewDropdown()
{
	if (Base)
		delete Base;
	if (Top)
		delete Top;
	if (Mid)
		delete Mid;
	if (Bot)
		delete Bot;
	if (SelectedText)
		delete SelectedText;

	for (auto item : vItems)
		if (item.pText)
			delete item.pText;

	vItems.clear();
}

void UINewDropdown::Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
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

		if (!getSelected())
		{
			if (Base)
				Base->Render(iRenderX, iRenderY, iRenderWidth, iRenderHeight, iStartX, iStartY);
		}
		else
		{
			if (Top && Mid && Bot)
			{
				Top->Render(iRenderX + paddingBackgroundLeft, iRenderY + paddingBackgroundTop, iRenderWidth, iRenderHeight, iStartX, iStartY);
				Mid->Render(iRenderX + paddingBackgroundLeft, iRenderY + paddingBackgroundTop, iRenderWidth, iRenderHeight, iStartX, iStartY);
				Bot->Render(iRenderX + paddingBackgroundLeft, iRenderY + paddingBackgroundTop, iRenderWidth, iRenderHeight, iStartX, iStartY);

				int pos = 1;
				for (auto sDropitem : vItems)
				{
					int iY = iRenderY + (22 * (pos++));

					if (isMousePos(iRenderX, iY, Top->getWidth(), 22))
					{
						if (BackgroundItem[1])
							BackgroundItem[1]->Render(iRenderX, iY, iRenderWidth, iRenderHeight, 0, 0);
						else
							dsDrawColorBox(D3DCOLOR_ARGB(128, 125, 110, 73), iRenderX + 3, iY, Top->getWidth() - 5, 22);
					}
					else if (BackgroundItem[0])
						BackgroundItem[0]->Render(iRenderX, iY, iRenderWidth, iRenderHeight, 0, 0);

					sDropitem.pText->Render(iRenderX + paddingLeftItemText, iRenderY, iRenderWidth + paddingLeftItemText, iRenderHeight, iStartX, iStartY);
				}
			}
		}

		if (bShowSelectedValue)
			SelectedText->Render(iRenderX, iRenderY, iRenderWidth, iRenderHeight, iStartX, iStartY);
	}
}

bool UINewDropdown::OnMouseClick(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
{
	if (isVisible() && Type != Up)
	{
		if (iWidth <= 0 || iHeight <= 0)
			return false;

		if (skipClick)
		{
			skipClick = false;
			return false;
		}

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

		if (isMousePos(iRenderX, iRenderY, iRenderWidth, iRenderHeight) && !getSelected() && Base)
		{
			if (Top && Mid && Bot)
				setHeight(Top->getHeight() + Mid->getHeight() + Bot->getHeight());
			setSelected(true);

			if (fOpen)
				fOpen();
			return true;
		}
		else if (getSelected())
		{
			for (int i = 0; i < (int)vItems.size(); i++)
			{
				if (isMousePos(iRenderX, iRenderY + 22 + (i * 22), Base ? Base->getWidth() : getWidth(), 22))
				{
					SelectItem(i);
					break;
				}
			}

			setHeight(Base ? Base->getHeight() : getHeight());
			setSelected(false);

			if (fClose)
				fClose();
			return true;
		}
	}

	return false;
}

void UINewDropdown::open()
{
	if (!getSelected())
	{
		if (Top && Mid && Bot)
			setHeight(Top->getHeight() + Mid->getHeight() + Bot->getHeight());

		setSelected(true);

		if (fOpen)
			fOpen();

		skipClick = true;
	}
}

void UINewDropdown::close()
{
	if (getSelected())
	{
		setHeight(Base ? Base->getHeight() : 0);
		setSelected(false);

		if (fClose)
			fClose();
	}
}

void UINewDropdown::AddOption(char* Text, int Value)
{
	UINewDropitem sDropitem;
	sDropitem.pText = new UINewText(0, Text, 0, 22 + (22 * vItems.size()), bold);
	//	sDropitem.pText = new UIText(0, FontName, FontSize, bold, Text, ));
	sDropitem.pText->setY(sDropitem.pText->getY() + (22 - sDropitem.pText->getHeight()) / 2);
	sDropitem.pText->setX(Base ? (Base->getWidth() - sDropitem.pText->getWidth()) / 2 : 0);

	if (bAlignTextCenter)
	{
		sDropitem.pText->setWidth(Top->getWidth());
		sDropitem.pText->setHeight(22);
		sDropitem.pText->setOrientation(EORIENT_Center);
	}

	sDropitem.Value = Value;
	vItems.push_back(sDropitem);

	if (Mid && Bot)
	{
		Mid->setHeight(22 * vItems.size() - 7);
		Bot->setY(Mid->getY() + Mid->getHeight());
	}
}

void UINewDropdown::SelectItem(int index)
{
	if (index < 0 || index >(int)vItems.size())
		return;

	auto sDropitem = vItems[index];

	SelectedText->setText(sDropitem.pText->getText());
	SelectedText->setX(3 + (80 - SelectedText->getWidth()) / 2);
	SelectedText->setY((22 - SelectedText->getHeight()) / 2);
	iSelectedIndex = index;

	if (fSelectItem)
		fSelectItem(index);
}

void UINewDropdown::SelectItemByValue(int value)
{
	for (int i = 0; i < vItems.size(); i++)
		if (vItems[i].Value == value)
		{
			SelectItem(i);
			break;
		}
}

void UINewDropdown::RemoveAllItems()
{
	for (auto item : vItems)
		delete item.pText;
	vItems.clear();
}

void UINewDropdown::setBackgroundItem(char* normal, char* hover)
{
	BackgroundItem[0] = new UINewImage(0, normal, 0, 0);
	BackgroundItem[1] = new UINewImage(0, hover, 0, 0);
}

int UINewDropdown::GetSelectedValue()
{
	if (!vItems.size())
		return 0;

	return vItems[iSelectedIndex].Value;
}

