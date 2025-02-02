#pragma once

#include "Engine/CFontHandler.h"

enum EOrientation : int
{
	EORIENT_Left = 1,
	EORIENT_Right = 2,
	EORIENT_Wrap = 3,
	EORIENT_WrapLeft = 4,
	EORIENT_Center = 5,
};

class UINewText : public NewElement
{
public:
	UINewText(int iID, char* Text, int x, int y, bool bold = false, DWORD dwColor = D3DCOLOR_ARGB(255, 255, 255, 255));
	UINewText(int iID, char* FontName, int FontSize, bool bold, char* Text, int x, int y, DWORD dwColor = D3DCOLOR_ARGB(255, 255, 255, 255), DWORD dwHoverColor = D3DCOLOR_ARGB(255, 255, 255, 255));
	~UINewText()
	{
		for (auto pText : vHighlight)
			delete pText;

		vHighlight.clear();
	}

	void Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	void AddChar(char Code);
	void RemoveChar(int count = 1);

	void setText(string text);
	string getText() { return string(szText); }

	void append(string text);

	int Length() { return lstrlen(szText); }
	void setOrientation(EOrientation orientation) { eOrientation = orientation; if (eOrientation == EORIENT_Wrap || eOrientation == EORIENT_WrapLeft) update(); }
	void setWidth(int width) { iWidth = width; if (eOrientation == EORIENT_Wrap || eOrientation == EORIENT_WrapLeft) update(); }
	void setHeight(int height) { iHeight = height; if (eOrientation == EORIENT_Wrap || eOrientation == EORIENT_WrapLeft) update(); }

	void setOutline(int outline, DWORD color = D3DCOLOR_ARGB(255, 0, 0, 0));
	void setColor(DWORD color) { TextColor = color; }
	void setHoverColor(DWORD color) { HoverColor = color; }

	CFont* GetFont() { return pFont; }

	int getTrueWidth() { return iTrueWidth; }
	void update();
	void setMaxWidth(int max) { iMaxWidth = max; update(); }
	void setHoverFunc(function<void()> func) { fHover = func; }

	bool HighlightText(string hlText, DWORD dwColor, DWORD dwHoverColor = 0, function<void()> hover = {});
	void AddHighlight(string hlText, int x, int y, DWORD dwColor, DWORD dwHoverColor = 0, function<void()> hover = {});

	void setX(int val);
	void setY(int val);

private:
	char szText[5000];
	string Text;
	DWORD TextColor;
	DWORD HoverColor;
	EOrientation eOrientation;
	CFont* pFont = nullptr;
	int Outline = 0;
	DWORD dwOutlineColor = D3DCOLOR_ARGB(255, 0, 0, 0);
	int iTrueWidth = 0;
	int iMaxWidth = 0;
	vector<UINewText*> vHighlight;
	function<void()> fHover;
};

extern char* FormatString(char* fmt, ...);