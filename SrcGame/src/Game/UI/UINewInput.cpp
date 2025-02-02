#include "UILinker.h"
#include <regex>

UINewInput::UINewInput(int iID, char* path, int x, int y, int w, char* active_path)
{
	setID(iID);
	setX(x);
	setY(y);

	Background = new UINewImage(0, path, 0, 0, w);
	if (Background)
	{
		if (active_path)
			Background_ = new UINewImage(0, active_path, 0, 0, w);

		pWindow = new UINewWindow(0, 30, 0, Background->getWidth() - 30, Background->getHeight());

		InputText = new UINewText(0, "", 0, 0);
		InputText->setOrientation(EORIENT_Right);
		pWindow->AddElement(InputText);

		Cursor = new UINewText(0, "|", InputText->getWidth(), 0);
		pWindow->AddElement(Cursor);

		setWidth(Background->getWidth());
		setHeight(Background->getHeight());
	}
}

void UINewInput::Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
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
		int iRenderWidth = low(cWidth, iWidth);
		int iRenderHeight = low(cHeight, iHeight);
		int iRenderX = cX + iX + iStartX;
		int iRenderY = cY + iY + iStartY;

		if (cX + iRenderWidth > iWidth)
			iRenderWidth -= (cX + iRenderWidth) - iWidth;

		if (cY + iRenderHeight > iHeight)
			iRenderHeight -= (cY + iRenderHeight) - iHeight;

		if (Background_ && isActive())
			Background_->Render(iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY);
		else if (Background)
			Background->Render(iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY);

		if (isActive() && Cursor && getCursor())
		{
			if (GetCurrentTime() - frametime > 200)
			{
				if (Cursor->isVisible())
					Cursor->hide();
				else
					Cursor->show();

				frametime = GetCurrentTime();
			}
		}
		else
		{
			frametime = 0;
			if (Cursor)
				Cursor->hide();
		}

		if (pWindow)
			pWindow->Render(iRenderX, iRenderY, iRenderWidth, iRenderHeight, 0, 0);
	}
}

bool UINewInput::OnMouseClick(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
{
	if (isVisible())
	{
		if (iWidth <= 0 || iHeight <= 0)
			return true;

		int cX = getX();
		int cY = getY();
		int cWidth = getWidth();
		int cHeight = getHeight();

		int iStartX = cX < iSourceX ? abs(cX - iSourceX) : 0;
		int iStartY = cY < iSourceY ? abs(cY - iSourceY) : 0;
		int iRenderWidth = low(cWidth, iWidth);
		int iRenderHeight = low(cHeight, iHeight);
		int iRenderX = cX + iX + iStartX;
		int iRenderY = cY + iY + iStartY;

		if (cX + iRenderWidth > iWidth)
			iRenderWidth -= (cX + iRenderWidth) - iWidth;

		if (cY + iRenderHeight > iHeight)
			iRenderHeight -= (cY + iRenderHeight) - iHeight;

		if (eActivate == EACTIVATE_Click || eActivate == EACTIVATE_Both)
		{
			if (isMousePos(iRenderX, iRenderY, iRenderWidth, iRenderHeight))
			{
				if (Type != Up)
					setActive(true);
				frametime = 0;
				return true;
			}
			else
			{
				setActive(false);
				frametime = 0;
			}
		}
	}
	return false;
}

bool UINewInput::OnKeyDown(char Code)
{
	if (isVisible())
	{
		if (isActive() && Code != VK_TAB)
		{
			SendMessage(hFocusWnd, WM_KEYDOWN, (WPARAM)Code, (LPARAM)0);

			char buff[255];
			GetWindowText(hFocusWnd, buff, 255);
			TrueText.assign(buff);

			if (bFilter)
			{
				string str(TrueText);
				fill(str.begin(), str.end(), '*');
				InputText->setText(str);
			}
			else
			{
				InputText->setText(TrueText);
			}

			UpdateCaretPos();
		}

		auto it = vKeyBinds.find(Code);
		if (it != vKeyBinds.end())
			it->second();
	}
	return isActive();
}

bool UINewInput::OnKeyChar(char Code)
{
	if (isVisible())
	{
		if (isActive())
		{
			if (Code != VK_BACK)
			{
				if ((int)InputText->getText().length() >= iLength)
				{
					SendMessage(hFocusWnd, WM_CHAR, (WPARAM)VK_BACK, 0);
				}
				else
				{
					if (eMode == EMODE_Number)
					{
						if ((Code < '0' || Code > '9'))
							SendMessage(hFocusWnd, WM_CHAR, (WPARAM)VK_BACK, 0);
					}
					else if (eMode == EMODE_NoSpecial)
					{
						//J4 character name
						bool bAllow = false;
						char* nameFilter = "qwertyuiopasdfghjklzxcvbnm1234567890[]-";
						for (int j = 0; j < lstrlen(nameFilter); j++)
						{
							if (tolower(Code) == tolower(nameFilter[j]))
							{
								bAllow = true;
								break;
							}
						}
						if (!bAllow)
							SendMessage(hFocusWnd, WM_CHAR, (WPARAM)VK_BACK, 0);
					}
				}
			}


			char buff[255];
			GetWindowText(hFocusWnd, buff, 255);
			TrueText.assign(buff);

			if (bFilter)
			{
				string str(TrueText);
				fill(str.begin(), str.end(), '*');
				InputText->setText(str);
			}
			else
			{
				InputText->setText(TrueText);
			}

			UpdateCaretPos();
			return true;
		}
	}
	return false;
}

extern HWND hFocusWnd;
extern HWND hTextWnd;

void UINewInput::setActive(bool active)
{
	if (!getEnabled())
	{
		bActive = false;
		return;
	}

	if (bActive != active && !active)
	{
		hFocusWnd = 0;
		//iFocusID = -1;

		if (fOnUnfocus)
			fOnUnfocus();
	}

	bActive = active;
	if (isActive())
	{
		//iFocusID = getID();
		hFocusWnd = hTextWnd;
		SetWindowText(hFocusWnd, TrueText.c_str());
		SendMessage(hFocusWnd, EM_SETSEL, TrueText.length(), TrueText.length());
		UpdateCaretPos();

		if (fOnFocus)
			fOnFocus();
	}
}

void UINewInput::setText(string text)
{
	if (bFilter)
	{
		string str(text);
		fill(str.begin(), str.end(), '*');
		InputText->setText(str);
	}
	else
	{
		InputText->setText(text);
	}

	TrueText.assign(text);

	if (isActive())
	{
		SetWindowText(hFocusWnd, TrueText.c_str());
		SendMessage(hFocusWnd, EM_SETSEL, TrueText.length(), TrueText.length());
		UpdateCaretPos();
	}
}

void UINewInput::Bind(char Code, function<void()> func)
{
	vKeyBinds.emplace(Code, func);
}

void UINewInput::setTextArea(int x, int y, int w, int h)
{
	pWindow->setX(x);
	pWindow->setY(y);
	pWindow->setWidth(w);
	pWindow->setHeight(h);
}

void UINewInput::setOrient(EOrientation eOrient)
{
	if (eOrient == EORIENT_WrapLeft)
	{
		InputText->setMaxWidth(pWindow->getWidth());
		InputText->setOrientation(EORIENT_Left);
	}
	else
	{
		InputText->setOrientation(eOrient);

		if (eOrient == EORIENT_Wrap || eOrient == EORIENT_Center)
			InputText->setWidth(pWindow->getWidth());
	}
}

void UINewInput::UpdateCaretPos()
{
	int firstChar, lastChar;
	SendMessage(hFocusWnd, EM_GETSEL, (WPARAM)&firstChar, (LPARAM)&lastChar);
	if (firstChar > 0)
	{
		SIZE sz = {};
		string Text = InputText->getText().substr(0, firstChar);
		auto pFont = InputText->GetFont();

		int len = Text.length();
		if (len > 0 && Text[len - 1] == ' ')
			Text[len - 1] = '-';

		pFont->GetTextSize(Text.c_str(), &sz);
		Cursor->setX(InputText->getX() + sz.cx - 1);
	}
	else
		Cursor->setX(InputText->getX());
}
