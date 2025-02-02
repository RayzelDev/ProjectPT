#pragma once


struct UINewDropitem
{
	UINewText* pText;
	int Value;
};

class UINewDropdown : public NewElement
{
public:
	UINewDropdown(int ID, int x, int y, char* base, char* top, char* mid, char* bot);
	~UINewDropdown();

	void Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	bool OnMouseClick(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);

	void open();
	void close();

	void setOpen(function<void()> func) { fOpen = func; }
	void setClose(function<void()> func) { fClose = func; }
	void setSelectItem(function<void(int)> func) { fSelectItem = func; }

	void AddOption(char* Text, int Value);

	void SelectItem(int index);
	void SelectItemByValue(int value);
	void RemoveAllItems();

	void setBackgroundItem(char* normal, char* hover);
	void setAlignTextCenter(bool value) { bAlignTextCenter = value; }
	void setPaddingBackgroundLeft(int value) { paddingBackgroundLeft = value; }
	void setPaddingBackgroundTop(int value) { paddingBackgroundTop = value; }
	void setPaddingLeftItemText(int value) { paddingLeftItemText = value; }
	void setShowSelectedValue(bool value) { bShowSelectedValue = value; }
	void setFontItem(char* font, int size, bool Bold) { FontName = font; FontSize = size; bold = Bold; }
	void setCloseWhenClickOutside(bool value) { bCloseWhenClickOutside = value; }

	int GetSelectedValue();

	int returnSelected()
	{
		return vItems[iSelectedIndex].Value;
	};

private:
	UINewImage* Base;
	UINewImage* Top;
	UINewImage* Mid;
	UINewImage* Bot;

	UINewImage* BackgroundItem[2] = { 0 };

	UINewText* SelectedText;

	vector<UINewDropitem> vItems;

	int paddingBackgroundLeft = 0;
	int paddingBackgroundTop = 0;
	int paddingLeftItemText = 0;

	bool bCloseWhenClickOutside = false;
	bool bShowSelectedValue = true;
	bool bAlignTextCenter = false;
	int iSelectedIndex = 0;

	char* FontName = "Arial";
	int FontSize = 16;
	bool bold = false;

	function<void()> fOpen;
	function<void(int)> fSelectItem;
	function<void()> fClose;
};
