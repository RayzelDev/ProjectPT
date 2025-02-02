#pragma once

#pragma warning( disable : 4244 )

#include "sinbaram/sinLinkHeader.h"

enum TYPES
{
	WINDOW = 1,
	OBJECT,
};

enum CLICKS
{
	Click,
	Down,
	Up
};

enum ALIGN
{
	EALIGN_TOP,
	EALIGN_BOT,
};

class NewElement
{
public:
	NewElement() { iType = -1; }

	int low(int a, int b)
	{
		if (a < b)
			return a;

		return b;
	}

	void						setID(int id) { iID = id; }
	int							getID() { return iID; }

	void						setType(int type) { iType = type; }
	int							getType() { return iType; }

	virtual void				setSelected(bool selected) { bSelected = selected; }
	virtual bool				getSelected() { return bSelected; }

	void						setHover(bool hover) { bHover = hover; }
	bool						getHover() { return bHover; }

	void						show() { bVisible = true; }
	void						hide() { bVisible = false; }
	bool						isVisible() { return bVisible; }

	void						setWidth(int Width) { iWidth = Width; }
	int							getWidth() { return iWidth; }
	void						setHeight(int Height) { iHeight = Height; }
	int							getHeight() { return iHeight; }

	virtual void				setX(int x) { iX = x; }
	virtual int					getX() { return iX; }
	virtual void				setY(int y) { iY = y; }
	virtual int					getY() { return iY; }

	void setHoverFunc(std::function<void()> func) { fHover = func; }

	void						setFree(bool free) { bFree = free; }
	bool						getFree() { return bFree; }

	void						setFixed(bool fixed) { bFixed = fixed; }
	bool						getFixed() { return bFixed; }

	void						setAlign(ALIGN align) { eAlign = align; }
	ALIGN						getAlign() { return eAlign; }

	virtual void				Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY, int iSourceWidth, int iSourceHeight) {};
	virtual void				Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY) {}
	virtual bool				OnMouseClick(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY) { return false; };
	virtual bool				OnMouseMove(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY) { return false; };
	virtual bool				OnMouseScroll(int zDelta, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY) { return false; };
	virtual bool				OnKeyDown(char Code) { return false; };
	virtual bool				OnKeyChar(char Code) { return false; };

private:
	int							iType;
	int							iID = -1;
	bool						bVisible = true;
	bool						bFree = true;
	std::function<void()> fHover;

protected:
	bool						bSelected = false;
	bool						bHover = true;
	bool						bFixed = true;
	ALIGN						eAlign = EALIGN_TOP;
	int							iX, iY, iWidth, iHeight;
};