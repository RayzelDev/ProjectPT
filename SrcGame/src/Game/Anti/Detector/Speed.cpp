#include "../StdAfx.h"

void Speed_Scanner(){
    
	int time = ::GetTickCount();
	Sleep(5000);
	int timeElapsed = ::GetTickCount() - time; 
	if ("%d", timeElapsed > 5500){ // velocidade máxima (em milisegundos) que não deve ser ultrapassada
		//Log("S", "Speed Hack detected!");
		ExitProcess(0);
	}
}


void Speed_Scan(){
	again:
    Speed_Scanner();
    Sleep(config.DSpeed_occours); 
    goto again;
}

void DetectSpeed(){
	CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(Speed_Scan),NULL,0,0);	
}