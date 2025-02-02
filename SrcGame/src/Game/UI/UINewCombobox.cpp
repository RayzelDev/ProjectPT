#include "UILinker.h"

UINewCombobox::UINewCombobox(int iID)
{
	setID(iID);
}

void UINewCombobox::createButton(int iID, int x, int y, char * path, char * path_)
{
	pButton = new UINewButton(iID, path, path_, x, y, true, false, std::bind(&UINewCombobox::OnClickToggleWindow, this));

	setX(x);
	setY(y);
	setWidth(pButton->getWidth());
	setHeight(pButton->getHeight());
}

UINewWindow * UINewCombobox::createWindow(int iID, int x, int y, char * top, char * mid, char * bot)
{
	pWindow = new UINewWindow(iID, top, mid, bot, x, y);
	pWindow->hide();
	return pWindow;
}

void UINewCombobox::addOption(int iID, char * path, char * label, bool line)
{
	pWindow->ExpandMidWindow();
	pWindow->AddElement(new UINewButton(iID, path, path, 8, (pWindow->getMaxY() ? pWindow->getMaxY() : 5) + 3, true, false, std::bind(&UINewCombobox::OnSelectOption, this, iID)));

	if(line)
		pWindow->AddElement(new UINewImage(0, "game\\GUI\\Chat\\chat_combo_line.png", 5, pWindow->getMaxY() + 2));

	if(pButton)
		vLabels.push_back(std::pair<int, UINewImage*>(iID, new UINewImage(iID, label, pButton->getX(), pButton->getY())));
}

void UINewCombobox::selectOption(int iID)
{
	for (auto pLabel : vLabels)
	{
		if (pLabel.first == iID)
			pLabel.second->show();
		else
			pLabel.second->hide();
	}

	iSelectedID = iID;
	pWindow->hide();
}

void UINewCombobox::Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
{
	if (isVisible())
	{
		if (pButton)
		{
			pButton->Render(iX, iY, iWidth, iHeight, iSourceX, iSourceY);
			for (auto pLabel : vLabels)
				pLabel.second->Render(iX, iY, iWidth, iHeight, iSourceX, iSourceY);
		}

		if (pWindow)
			pWindow->Render(iX, iY, iWidth, iHeight, iSourceX, iSourceY);
	}
}

bool UINewCombobox::OnMouseClick(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
{
	bool bRet = false;
	if (isVisible())
	{
		if (pButton)
			if (pButton->OnMouseClick(Type, iX, iY, iWidth, iHeight, iSourceX, iSourceY))
				bRet = true;

		if (pWindow)
			if (pWindow->OnMouseClick(Type, iX, iY, iWidth, iHeight, iSourceX, iSourceY))
				bRet = true;
	}
	return bRet;
}

void UINewCombobox::setY(int y)
{
	if (pButton && pWindow)
	{
		pButton->setY(y + (iY - pButton->getY()));
		pWindow->setY(y - (iY - pWindow->getY()));

		for (auto pLabel : vLabels)
			pLabel.second->setY(pButton->getY());
	}

	iY = y;
}

void UINewCombobox::OnSelectOption(int iID)
{
	selectOption(iID);
	if (OnSelect)
		OnSelect();
}

void UINewCombobox::OnClickToggleWindow()
{
	if (pWindow)
	{
		if (pWindow->isVisible())
			pWindow->hide();
		else
			pWindow->show();
	}
}
