#pragma once

enum EActivate
{
	EACTIVATE_Click,
	EACTIVATE_Key,
	EACTIVATE_Both,
};

enum EMode
{
	EMODE_Normal,
	EMODE_Number,
	EMODE_NoSpecial,
};

class UINewInput : public NewElement
{
public:
	UINewInput(int iID, char* path, int x, int y, int w = 0, char* active_path = nullptr);

	~UINewInput()
	{
		if (pWindow)
			delete pWindow;
		if (Background)
			delete Background;
		if (Background_)
			delete Background_;
	}

	void Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	bool OnMouseClick(CLICKS Type, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);
	bool OnKeyDown(char Code);
	bool OnKeyChar(char Code);

	bool isActive() { return bActive; }
	void setActive(bool active);

	void setEnabled(bool enabled) { bEnabled = enabled; }
	bool getEnabled() { return bEnabled; }

	void setText(string text);
	string getText() { return TrueText; }

	void setActivationMethod(EActivate activate) { eActivate = activate; }

	void Bind(char Code, function<void()> func);

	void setTextArea(int x, int y, int w, int h);

	void setOrient(EOrientation eOrient);

	void setCursor(bool cursor) { bCursor = cursor; }
	bool getCursor() { return bCursor; }

	void setFilter(bool filter) { bFilter = filter; setText(TrueText); }

	void setMode(EMode mode) { eMode = mode; }

	void setLength(int length) { iLength = length; }

	void setOnFocus(function<void()> focus) { fOnFocus = focus; }
	void setOnUnfocus(function<void()> unfocus) { fOnUnfocus = unfocus; }

	int getMaxLength() { return iLength; }
private:
	bool bActive = false;
	bool bEnabled = true;
	UINewWindow* pWindow = 0;
	UINewImage* Background = 0;
	UINewImage* Background_ = 0;
	UINewText* Cursor = 0;
	UINewText* InputText = 0;
	DWORD frametime = 0;
	EActivate eActivate = EACTIVATE_Click;
	bool bCursor = true;
	bool bFilter = false;
	string TrueText;
	EMode eMode = EMODE_Normal;
	int iLength = 255;

	unordered_map<char, function<void()>> vKeyBinds;

	function<void()> fOnFocus;
	function<void()> fOnUnfocus;

	void UpdateCaretPos();
};