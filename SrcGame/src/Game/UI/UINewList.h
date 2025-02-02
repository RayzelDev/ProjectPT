#pragma once

class UINewListBox : public NewElement
{
public:
	UINewListBox(int ID, int x, int y, int w, int h);
	~UINewListBox();

	void Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	bool OnMouseClick(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	bool OnMouseMove(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	bool OnMouseScroll(int zDelta, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	bool OnKeyDown(char Code);
	bool OnKeyChar(char Code);

	UINewListItem* getSelectedItem() { return pSelectedItem; }
	void selectItem(int iID);

	UINewVerticalScroll* GetScrollBar() { return pVerticalScroll; }
	void SetScrollBar(UINewVerticalScroll* pScroll);
	int getMaxY() { return iMaxY; }

	void AddListItem(UINewListItem* pListItem);

	void ClearList();

	template<typename T>
	inline T* GetListItem(int iID)
	{
		for (auto pListItem : vListItems)
			if (pListItem->getID() == iID)
				return static_cast<T*>(pListItem);
		return nullptr;
	}

private:
	vector<UINewListItem*> vListItems;
	UINewListItem* pSelectedItem;
	UINewVerticalScroll* pVerticalScroll = nullptr;
	int iMaxY = 0;
};