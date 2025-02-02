#include "../StdAfx.h"

#define WINNT 1 // Define for using EndTask API

void HT(){
	ExitProcess(0);
}

struct ProcInfo {
    HWND hWnd;
    DWORD dwProcId;
    CHAR szTitle[255];
    CHAR szClass[255];
};
 
void Hide_Toolz()
{
    DWORD dwProcId;
    HWND hWnd;
    ProcInfo mProc[255];
    int mIdList[255];
    int nCount = 0;
    int nPID = 0;
    int i = 0;
    hWnd = FindWindow(0,0); 
    while( hWnd > 0)
    {
        if (GetParent(hWnd) == 0) 
        {
            GetWindowThreadProcessId(hWnd, &dwProcId);
            if (!OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcId))
            {
                mProc[nCount].hWnd = hWnd;
                mProc[nCount].dwProcId = dwProcId ;
 
                GetWindowTextA(hWnd, mProc[nCount].szTitle, 255);
                GetClassNameA(hWnd, mProc[nCount].szClass, 255);
 
//printf("%2d. ProcessId: %d\n   - Class Name: %s\n   - Window Title: %s\n",  nCount+1, dwProcId, mProc[nCount].szClass, mProc[nCount].szTitle);
//printf("   - Window Handle: 0x%X\n   - Window State: %s\n\n", hWnd, IsWindowVisible(hWnd) ? "Shown" : "Hidden");
                nCount++; 
                for (i = 0; i < nPID; i++)
                    if (dwProcId == mIdList[i]) 
                        break; 
                if (i == nPID)
                     mIdList[nPID++] = dwProcId; 
            }
        }
        hWnd = GetWindow(hWnd, GW_HWNDNEXT); 
    }   
	if (nCount && nPID > 1){
		HT();
	//printf("\n=> Detected %d windows of %d hidden process.\n", nCount, nPID); 	
	}
  	else
    {
		Sleep(100);           
	}
}

void Hide_Tz(){
	again:
    Hide_Toolz();
    Sleep(config.DHide_occours);
    goto again;
}

void Hide_T(){
	CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(Hide_Tz),NULL,0,0);
}