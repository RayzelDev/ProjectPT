#pragma once

struct sDamagePlayerToGame
{
	int size;
	int code;
	double Damage;
};

class INTERFACEDAMAGE
{
public:
	void Init();
	void Draw();
	void Open() { fOpen = true; };
	void Close() { fOpen = false; };
	bool IsOpen() { return fOpen; };
	BOOL IsMousePos(int x, int y, int w, int h);
	void Button();
	void DrawImage(int Image, int x, int y, int w, int h);
	void AtualizarDamage();

	int InterfaceDamageImg[10];
	int fOpen;
	int BaseX, BaseY;
	double damage;
};
extern INTERFACEDAMAGE InterfaceDamage;