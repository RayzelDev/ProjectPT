#pragma once

class UINewCombobox : public NewElement
{
public:
	UINewCombobox(int iID);

	int iSelectedID = 0;

	void createButton(int iID, int x, int y, char* path, char* path_);
	UINewWindow* createWindow(int iID, int x, int y, char* top, char* mid, char* bot);
	void addOption(int iID, char* path, char* label, bool line = true);
	void selectOption(int iID);
	void setOnSelect(std::function<void()> callback) { OnSelect = callback; }

	void Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	bool OnMouseClick(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	void OnSelectOption(int iID);

	void setY(int y);

private:
	UINewButton* pButton = nullptr;
	UINewWindow* pWindow = nullptr;

	vector<std::pair<int, UINewImage*>> vLabels;

	std::function<void()> OnSelect = 0;

	void OnClickToggleWindow();
};