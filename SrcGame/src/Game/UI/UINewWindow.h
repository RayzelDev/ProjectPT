#pragma once

class UINewWindow : public NewElement
{
public:
	UINewWindow() {}
	UINewWindow(int ID, char* path, int x, int y, bool fixed = true);
	UINewWindow(int ID, char* top, char* mid, char* bot, int x, int y, bool fixed = true);
	UINewWindow(int ID, int x, int y, int w, int h, bool fixed = true);
	~UINewWindow();

	void Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	bool OnMouseClick(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	bool OnMouseMove(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	bool OnMouseScroll(int zDelta, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	bool OnKeyDown(char Code);
	bool OnKeyChar(char Code);

	void setMidHeight(int height);
	int getMidHeight() { if (getType() == 2) return bgSplit[1]->getHeight(); else return 0; }
	void setDefaultMidHeight(int height) { iDefaultMidHeight = height; }
	void setExpandLimit(int limit) { iExpandLimit = limit; }
	void setExpandRate(int rate) { iExpandRate = rate; }
	void ExpandMidWindow();
	void AdjustScrollBar();
	void setElementMax(int val) { iElementMax = val; }
	int getMaxY() { return iMaxY; }
	void setMaxY(int maxY) { iMaxY = maxY; }
	void SetScrollBar(UINewVerticalScroll* pScroll);
	UINewVerticalScroll* GetScrollBar() { return pVerticalScroll; }

	void AddElement(NewElement* pElement);
	void RemoveElementByID(int ID);
	void RemoveAllElements();
	vector<NewElement*> GetAllElements() { return vElements; }

	template<typename T>
	inline T* GetElement(int iID)
	{
		for (auto pElement : vElements)
			if (pElement->getID() == iID)
				return static_cast<T*>(pElement);
		return nullptr;
	}

private:
	UINewImage* Background;
	UINewImage* bgSplit[3];

	vector<NewElement*> vElements;
	UINewVerticalScroll* pVerticalScroll = nullptr;

	int iMaxY = 0;
	bool bMouseToggle = false;
	int iPaddingX = 0;
	int iPaddingY = 0;
	int iDefaultMidHeight = 0;
	int iExpandCount = 0;
	int iExpandLimit = 0;
	int iExpandRate = 0;

	int iElementMax = 0;
};
