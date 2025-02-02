#include "sinbaram\\sinlinkheader.h"
#include "FontImage.h"
#include "Alerta.h"


void Alerta::Init() {
	Imagens[0] = CreateTextureMaterial("game\\images\\Global\\Global.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

}

bool global;
char AlertaDrop[256];
void Alerta::DrawImage(int Mat, int x, int y, int w, int h)
{
	HoDrawTexImage(Mat, x, y, w, h, w, h, 255);
}
int i = 0;
int Transp = 255;
int j = 0;
void Alerta::Draw() {


	if (global) {
		if (j < 45 && i < 1800) {
			j++;
		}
		if (i >= 1800) {
			j--;
		}
		i++;

		SIZE size;
		size = {};

		AlertaDrop[strlen(AlertaDrop)];
		GetFontTextExtentPoint(AlertaDrop, lstrlen(AlertaDrop), &size);
		DrawImage(Imagens[0], smScreenWidth / 2 - 300, j + 35, 600, 43);
		DrawFontText((smScreenWidth / 2) - (size.cx / 2), j + 48, AlertaDrop, RGB(255, 255, 255), 0); //font msm tamanho do chat

	}
	if (i >= 1845) {
		global = false;
		i = 0;


	}
}
