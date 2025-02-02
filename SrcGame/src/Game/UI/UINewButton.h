#pragma once

class UINewButton : public NewElement
{
public:
	UINewButton(int ID, char* path, char* path_, int x, int y, bool hover = true, bool selected = false, std::function<void()> callback = 0);

	void Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	bool OnMouseClick(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);

	void setHoverFunc(std::function<void()> func) { fHover = func; }

	void setDisabledImage(char* path);
	void enable() { bEnabled = true; }
	void disable() { bEnabled = false; }

	void setCaption(UINewText* pCaption);

private:
	UINewImage* Image;
	UINewImage* Image_;
	UINewImage* _Image;
	UINewText* Caption;
	bool bEnabled = true;

	std::function<void()> Activation;
	std::function<void()> fHover;
};