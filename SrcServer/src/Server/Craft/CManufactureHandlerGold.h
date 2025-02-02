#pragma once

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

class CManufactureHandlerGold
{
public:
	static CManufactureHandlerGold* getInstance() { static CManufactureHandlerGold instance; return &instance; };

	void CManufactureHandlerGold::SendOpenManufactureWindow(rsPLAYINFO* lpPlayInfo);
	void CManufactureHandlerGold::HandlePacket(rsPLAYINFO* lpPlayInfo, PacketGetRecipeGold* psPacket);
	void CManufactureHandlerGold::HandlePacket(rsPLAYINFO* lpPlayInfo, PacketManufactureRecipeGold* psPacket);
};

#define CRAFTGOLDHANDLER CManufactureHandlerGold::getInstance()