#include "UILinker.h"

char* FormatString(char* fmt, ...)
{
	static char buff[4096];

	ZeroMemory(buff, sizeof(buff));

	va_list args;

	va_start(args, fmt);
	vsprintf(buff, fmt, args);
	va_end(args);

	return buff;
}

UINewText::UINewText(int iID, char* Text, int x, int y, bool bold, DWORD dwColor)
{
	setID(iID);
	setX(x);
	setY(y);
	setHover(false);
	TextColor = dwColor;
	HoverColor = dwColor;

	pFont = CFONTHANDLER->GetFontDefault(bold);

	ZeroMemory(szText, sizeof(szText));
	lstrcpy(szText, Text);

	update();
	setOrientation(EORIENT_Left);
}

UINewText::UINewText(int iID, char* FontName, int FontSize, bool bold, char* Text, int x, int y, DWORD dwColor, DWORD dwHoverColor)
{
	setID(iID);
	setX(x);
	setY(y);
	setHover(false);
	TextColor = dwColor;
	HoverColor = dwHoverColor;

	ZeroMemory(szText, sizeof(szText));
	lstrcpy(szText, Text);

	pFont = CFONTHANDLER->GetFont(FontName, FontSize, bold);

	update();
	setOrientation(EORIENT_Left);
}

void UINewText::Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
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

		if (iStartY < iRenderHeight && iStartX < iRenderWidth)
		{
			UINT uFormat = 0;

			if (iStartX > 0)
			{
				uFormat |= DT_RIGHT;
			}
			else if (iRenderWidth < cWidth)
			{
				if (eOrientation == EORIENT_Left)
					uFormat |= DT_LEFT;
				else if (eOrientation == EORIENT_Right)
					uFormat |= DT_RIGHT;
			}

			if (iStartY > 0)
			{
				uFormat |= DT_BOTTOM;
			}

			if (!iMaxWidth)
			{
				if (eOrientation == EORIENT_Wrap)
				{
					uFormat |= DT_CENTER;
					uFormat |= DT_WORDBREAK;
				}
				else if (eOrientation == EORIENT_WrapLeft)
					uFormat |= DT_WORDBREAK;
			}

			if (eOrientation == EORIENT_Center)
				uFormat |= DT_CENTER;

			auto pszText = Text.c_str();
			int length = Text.length();

			if (Outline > 0)
			{
				pFont->SetFontColor(dwOutlineColor);
				pFont->DrawRect1(RECT({ iRenderX - Outline, iRenderY, iRenderX - Outline + iRenderWidth - iStartX, iRenderY + iRenderHeight - iStartY }), pszText, length, uFormat);
				pFont->DrawRect1(RECT({ iRenderX, iRenderY - Outline, iRenderX + iRenderWidth - iStartX, iRenderY - Outline + iRenderHeight - iStartY }), pszText, length, uFormat);
				pFont->DrawRect1(RECT({ iRenderX + Outline, iRenderY, iRenderX + Outline + iRenderWidth - iStartX, iRenderY + iRenderHeight - iStartY }), pszText, length, uFormat);
				pFont->DrawRect1(RECT({ iRenderX, iRenderY + Outline, iRenderX + iRenderWidth - iStartX, iRenderY + Outline + iRenderHeight - iStartY }), pszText, length, uFormat);
			}

			pFont->SetFontColor(getHover() || getSelected() ? HoverColor : TextColor);
			pFont->DrawRect1(RECT({ iRenderX, iRenderY, iRenderX + iRenderWidth - iStartX, iRenderY + iRenderHeight - iStartY }), pszText, length, uFormat);

			for (auto pText : vHighlight)
				pText->Render(iX, iY, iWidth, iHeight, iSourceX, iSourceY);

			if (fHover && isMousePos(iRenderX, iRenderY, iRenderWidth - iStartX, iRenderHeight - iStartY))
				fHover();
		}
	}
}

void UINewText::AddChar(char Code)
{
	if (lstrlen(szText) < sizeof(szText) - 1)
	{
		szText[lstrlen(szText)] = Code;
		szText[lstrlen(szText)] = 0;

		update();
	}
}

void UINewText::RemoveChar(int count)
{
	if (lstrlen(szText) >= count && count > 0)
	{
		szText[lstrlen(szText) - count] = 0;

		update();
	}
}

void UINewText::setText(string text)
{
	lstrcpy(szText, text.c_str());

	update();
}

void UINewText::append(string text)
{
	lstrcat(szText, text.c_str());

	update();
}

void UINewText::setOutline(int outline, DWORD color)
{
	Outline = outline;
	dwOutlineColor = color;
}

void UINewText::update()
{
	SIZE sz = {};

	if (iMaxWidth)
	{
		Text.assign(szText);

		int iTextWidth = 0;
		const char* pszText = Text.c_str();
		SIZE sz = {};

		for (UINT i = 0; i < Text.size(); i++)
		{
			char c = pszText[i];
			if (c == ' ')
				c = '-';
			pFont->GetTextSize1(&c, &sz, 0, 1);

			iTextWidth += sz.cx;

			if (iTextWidth >= iMaxWidth)
			{
				if (Text[i] == ' ')
					Text[i] = '\n';
				else
					Text.insert(Text.begin() + i, '\n');
				iTextWidth = 0;
			}
		}

		pFont->GetTextSize(Text.c_str(), &sz);
		iWidth = sz.cx;
		iHeight = sz.cy;

		pFont->GetTextSize(szText, &sz);
		iTrueWidth = sz.cx;
	}
	else if (eOrientation == EORIENT_Wrap || eOrientation == EORIENT_WrapLeft)
	{
		pFont->GetTextSize(szText, &sz, getWidth());
		Text.assign(szText);
		iTrueWidth = sz.cx;
		iHeight = sz.cy;
	}
	else
	{
		Text.assign(szText);
		pFont->GetTextSize(szText, &sz);
		iTrueWidth = iWidth = sz.cx;
		iHeight = sz.cy;
	}
}

bool UINewText::HighlightText(string hlText, DWORD dwColor, DWORD dwHoverColor, function<void()> hover)
{
	int iStartPos = 0;
	int iLen = 0;
	bool bRet = false;

	for (int i = 0; i < (int)Text.length(); i++)
	{
		if (Text[i] == hlText[0])
		{
			int iLines = 0;
			bool bFound = false;

			for (int j = 0; j < (int)Text.length() - i; j++)
			{
				int t = i + j;
				int x = j - iLines;

				if (x > (int)hlText.length())
					break;

				if (Text[t] == '\n')
				{
					if (hlText[x] != ' ')
						iLines++;
					continue;
				}

				if (Text[t] != hlText[x])
				{
					iStartPos = 0;
					iLen = 0;
					iLines = 0;
					break;
				}

				if (x + 1 == hlText.length())
				{
					iStartPos = i;
					iLen = j + 1;
					bFound = true;
					break;
				}
			}

			if (bFound)
				break;
		}
	}

	if (iLen > 0)
	{
		int iTextWidth = 0;
		int iTextHeight = 0;
		int iTextX = 0;
		int iTextY = 0;
		const char* pszText = Text.c_str();
		SIZE sz = {};
		string text("");

		for (int i = 0; i < (int)Text.length() + 1; i++)
		{
			char c = pszText[i];
			if (c == ' ')
				c = '-';
			pFont->GetTextSize1(&c, &sz, 0, 1);

			if (text.empty())
				iTextX = iTextWidth;

			iTextWidth += sz.cx;
			if (sz.cy >= iTextHeight)
				iTextHeight = sz.cy;

			if (c == '\n')
			{
				if (text.length())
				{
					AddHighlight(text, iTextX, iTextY, dwColor, dwHoverColor, hover);
					bRet = true;
					text.clear();
				}

				iTextWidth = 0;
				iTextY += iTextHeight;
				iTextHeight = 0;
			}
			else if (i >= iStartPos && i < iStartPos + iLen)
				text += pszText[i];
			else if (i >= iStartPos + iLen)
			{
				AddHighlight(text, iTextX, iTextY, dwColor, dwHoverColor, hover);
				bRet = true;
				break;
			}
		}
	}

	return bRet;
}

void UINewText::AddHighlight(string hlText, int x, int y, DWORD dwColor, DWORD dwHoverColor, function<void()> hover)
{
	auto pText = new UINewText(0, (char*)hlText.c_str(), getX() + x, getY() + y, false, dwColor);

	if (dwHoverColor && dwHoverColor != dwColor)
		pText->setHoverColor(dwHoverColor);

	if (hover)
		pText->setHoverFunc(hover);
	pText->setOutline(1);
	vHighlight.push_back(pText);
}

void UINewText::setY(int val)
{
	for (auto pText : vHighlight)
		pText->setY((pText->getY() - getY()) + val);

	iY = val;
}

void UINewText::setX(int val)
{
	for (auto pText : vHighlight)
		pText->setX((pText->getX() - getX()) + val);

	iX = val;
}
