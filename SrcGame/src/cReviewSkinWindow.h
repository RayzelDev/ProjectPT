#pragma once

#include "UI\\UILinker.h"
struct PacketReviewSkin
{
	int size, code;
	int Count;
	int Coins;
	bool bFirst;
	int  Sections;
	struct Info
	{
		sITEMINFO ResultItem;
		int Section;
		int Hand;

	} InfoReview[10];


};


enum ReviewObjects
{
	Review_Window = 0,
	Review_GetItem,
	Review_ReviewItens,
	Review_DrawItens,
	Review_NameItem,
	Review_Button,
	Review_BgItem,
	Review_Scroll,
	Review_Close,
	Review_Menu,
};

struct ReviewSkin
{
	int ID;
	int Section;
	sITEM Item;
};

class cReviewSkinWindow
{
public:
	static cReviewSkinWindow* GetInstance() { static cReviewSkinWindow instance; return &instance; };
	~cReviewSkinWindow();

	void Init();
	void Draw();
	void BuildImages();

	void Open() {
		pWindow1->show();
		pOpen = true;
	};
	void Close();

	void Resize();
	bool isOpen() { if (pWindow1) return pWindow1->isVisible(); else return false; }
	bool OnMouseScroll(int zDelta);
	bool OnMouseClick(bool Type);

	void OnMouseMove();

	void HandlePacket(PacketReviewSkin* psPacket);

private:
	UINewWindow* pWindow1;
	vector<ReviewSkin*> vResultSkins;

	int SelectItem;
	bool pOpen;
	void OnButtonGetClick(sITEM pItem, int Hand);
	void ClearItems();
};

#define REVIEWSKINWINDOW cReviewSkinWindow::GetInstance()
