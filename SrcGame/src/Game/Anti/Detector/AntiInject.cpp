#include "../StdAfx.h"

BOOLEAN BlockAPI(HANDLE, CHAR *, CHAR *);
void AntiInject();

/****************/
main()
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)AntiInject, 0, 0, 0);
	while (TRUE); // loop forever... now to try and inject
}

void AntiInject() 
{ 
   HANDLE hProc = GetCurrentProcess(); 
   while (TRUE) { 
      BlockAPI(hProc, "NTDLL.DLL", "LdrLoadDll"); 
      Sleep (100); 
   } 
} 
 
BOOLEAN BlockAPI (HANDLE hProcess, CHAR *libName, CHAR *apiName) 
{ 
   CHAR pRet[]={0xC3}; 
   HINSTANCE hLib = NULL; 
   VOID *pAddr = NULL; 
   BOOL bRet = FALSE; 
   DWORD dwRet = 0; 
 
   hLib = LoadLibrary (libName); 
   if (hLib) { 
       pAddr = (VOID*)GetProcAddress (hLib, apiName); 
       if (pAddr) { 
           if (WriteProcessMemory (hProcess, 
                            (LPVOID)pAddr, 
                            (LPVOID)pRet, 
                            sizeof (pRet), 
                            &dwRet )) { 
              if (dwRet) { 
                 bRet = TRUE; 
              } 
           } 
       } 
       FreeLibrary (hLib); 
   } 
   return bRet; 
}