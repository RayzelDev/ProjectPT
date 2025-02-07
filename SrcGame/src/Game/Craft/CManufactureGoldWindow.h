#pragma once

#include "UI/UILinker.h"


struct RecipeItemGold
{
	char ItemCode[32];
	DWORD Code, Head, ChkSum;
};

struct PacketOpenManufactureGoldWindow
{
	int size, code;
};

struct PacketGetRecipeGold
{
	int size, code;

	char RecipeCode[32];
};

struct PacketRecipeInfoGold
{
	int size, code;

	int Chance;
	int ChanceF;
	char StoneCode[3][32];
	sITEMINFO ResultItem[11];
	int Count;
	int Price;
};

struct PacketManufactureRecipeGold
{
	int size, code;

	RecipeItemGold Recipe;
	RecipeItemGold Runes[3];
};

enum CraftObjectsGold
{
	CraftGold_Receita = 1,
	CraftGold_ReviewItens = 2,
	CraftGold_ChanceCraftar = 3,
	CraftGold_Preco = 4,

	CraftGold_Slots = 0x100,
};

class CManufactureGoldWindow
{
public:
	static CManufactureGoldWindow* getInstance();

	void Init();
	void Render();

	void Resize();

	bool isOpen() { if (pWindow1) return pWindow1->isVisible(); else return false; }

	bool OnMouseClick(bool Type);
	void OnMouseMove();

	void HandlePacket(PacketOpenManufactureGoldWindow* psPacket);
	void HandlePacket(PacketRecipeInfoGold* psPacket);
	UINewWindow* pWindow1;
	void OnPutRecipe(UINewItemFrame* pItemFrame);
	char StoneCode[3][32];

protected:
	void BuildWindow1();

	void Close();
	void Help();

	void OnMainFrameHover(UINewItemFrame* pItemFrame, sITEM* pItem);
	void OnFrameHover(UINewItemFrame* pItemFrame, sITEM* pItem);
	void OnCraftButtonClick();
	void OnPreviewHover(sITEM* pItem);
	void OnPickRecipe();

	void ResetRuneFrames();

private:
	vector<sITEM> vResultItems;
	int Price;
};

#define	CRAFTGOLDWINDOW CManufactureGoldWindow::getInstance()
