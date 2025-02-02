#pragma once

#include "UI/UILinker.h"

struct RecipeItem
{
	char ItemCode[32];
	DWORD Code, Head, ChkSum;
};

struct PacketOpenManufactureWindow
{
	int size, code;
};

struct PacketGetRecipe
{
	int size, code;

	char RecipeCode[32];
};

struct PacketRecipeInfo
{
	int size, code;

	int Chance;
	int ChanceF;
	char StoneCode[3][32];
	sITEMINFO ResultItem[11];
	int Count;
	int Price;
};

struct PacketManufactureRecipe
{
	int size, code;

	RecipeItem Recipe;
	RecipeItem Runes[3];
};

enum CraftObjects
{
	Craft_Receita = 1,
	Craft_ReviewItens = 2,
	Craft_ChanceCraftar = 3,
	Craft_Preco = 4,

	Craft_Slots = 0x100,
};

class CManufactureWindow
{
public:
	static CManufactureWindow* getInstance();

	void Init();
	void Render();

	void Resize();

	bool isOpen() { if (pWindow1) return pWindow1->isVisible(); else return false; }

	bool OnMouseClick(bool Type);
	void OnMouseMove();

	void HandlePacket(PacketOpenManufactureWindow* psPacket);
	void HandlePacket(PacketRecipeInfo* psPacket);

protected:
	void BuildWindow1();

	void Close();
	void Help();

	void OnCraftButtonClick();
	void OnFrameHover(UINewItemFrame* pItemFrame, sITEM* pItem);
	void OnMainFrameHover(UINewItemFrame* pItemFrame, sITEM* pItem);
	void OnPutRecipe(UINewItemFrame* pItemFrame);
	void OnPreviewHover(sITEM* pItem);
	void OnPickRecipe();

	void ResetRuneFrames();

private:
	UINewWindow* pWindow1;
	char StoneCode[3][32];
	vector<sITEM> vResultItems;
	int Price;
};

#define	CRAFTWINDOW CManufactureWindow::getInstance()
