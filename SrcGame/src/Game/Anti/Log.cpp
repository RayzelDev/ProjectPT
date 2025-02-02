#include "StdAfx.h"

void Log(char* x, char *y) {
	ofstream out("game/Log.txt", ios::app);
	out << "\n " << x << ":  ", out << y;
	out.close();
}