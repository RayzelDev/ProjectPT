#pragma once

class UINewCheckbox : public UINewListItem
{
public:
	UINewCheckbox(int ID, int x, int y, char* path, char* path_, char* szText, bool free = true, bool selected = false, bool hover = false, bool bold = false);
	UINewCheckbox(int ID, int x, int y, char* path, char* path_, bool free = true, bool selected = false, bool hover = false);

	void Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	bool OnMouseClick(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);

	void setActivation(std::function<void()> callback) { Activation = callback; }

	void setText(UINewText* text);
	

	UINewText* GetText() { return pText; }
	UINewImage* GetImage() { return Image; }
	UINewImage* GetImage_() { return Image_; }

private:
	UINewText* pText = nullptr;
	std::function<void()> Activation;
};