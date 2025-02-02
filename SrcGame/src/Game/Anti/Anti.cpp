#include "StdAfx.h"
//#include "md5.h"
#include <string.h>
#include <algorithm>
extern "C" { int _afxForceUSRDLL; }
CONFIG config;
HANDLE m_DllModule;

extern  "C"  __declspec(dllexport) void __cdecl initConfig() {
	//config.DHide_occours		= 14000;
	//config.DDump_occours		= 5000;
	//config.DWindow_occours		= 20000;
	//config.DClassName_occours	= 11000;
	config.DSpeed_occours		= 12000;
	config.DAntiInject_occours = 12000;
}

/*void checkmd5() {
	MD5 md5 ;
	char *thatthis = md5.digestFile("PTV.dll");
	if(strcmp(thatthis, "0ea55a1dfc694100e47ed5cd5e385d3a") != 0) {
		free(thatthis);
		ExitProcess(1);
	}
	free(thatthis);
	thatthis = md5.digestFile("game.lix");
	if(strcmp(thatthis, "a89dac220486be962ea94cba661e150b") != 0) {
		free(thatthis);
		ExitProcess(1);
	}
	free(thatthis);
}*/
BOOL startAnti( ) {

	//Log("Main", "Started");
	//m_DllModule = hModule;
	//checkmd5();


	//initConfig(); //Init config
	//Hide_T(); //Detect hide
	//ProtectionMain(); //dump
	//DwProtection(); //scan title
	//CNProtection();	//scan class
	//DetectSpeed(); //hackspeed
	//AntiInject();
	return TRUE;
}