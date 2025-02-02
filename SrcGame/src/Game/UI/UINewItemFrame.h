#pragma once

class UINewItemFrame : public NewElement
{
public:
	UINewItemFrame(int ID, int x, int y, int w, int h);

	void Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	bool OnMouseClick(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	bool OnMouseMove(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);

	void setItem(sITEM* pItem);
	sITEM* getItem() { return &Item; }
	void pickItem(sITEM* pItem);

	void setPutItem(bool value) { bCanPutItem = value; }
	bool getPutItem() { return bCanPutItem; }

	void setEnabled(bool value) { bEnabled = value; }
	bool getEnabled() { return bEnabled; }

	void setHover(std::function<void()> func) { fHover = func; }
	void setPutFunc(std::function<void()> func) { fPutItem = func; }
	void setPickFunc(std::function<void()> func) { fPickItem = func; }

private:
	sITEM Item;
	bool bCanPutItem = false;
	bool bEnabled = true;

	std::function<void()> fHover = {};
	std::function<void()> fPutItem = {};
	std::function<void()> fPickItem = {};
};