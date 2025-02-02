#pragma once

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

class CManufactureHandler
{
public:
	static CManufactureHandler* getInstance() { static CManufactureHandler instance; return &instance; };

	void CManufactureHandler::SendOpenManufactureWindow(rsPLAYINFO* lpPlayInfo);
	void CManufactureHandler::HandlePacket(rsPLAYINFO* lpPlayInfo, PacketGetRecipe* psPacket);
	void CManufactureHandler::HandlePacket(rsPLAYINFO* lpPlayInfo, PacketManufactureRecipe* psPacket);
};

#define CRAFTHANDLER CManufactureHandler::getInstance()