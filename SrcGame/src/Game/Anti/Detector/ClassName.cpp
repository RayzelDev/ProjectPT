#include "../StdAfx.h"

void ClasseWindow(LPCSTR WindowClasse){
	DWORD procId; 
    HANDLE hProc;
	HWND WinClasse = FindWindowExA(NULL,NULL,WindowClasse,NULL);
	if( WinClasse > 0 && GetWindowThreadProcessId(WinClasse, &procId))
	{
		//Log("C", (char *)WindowClasse);
		if ( hProc = OpenProcess (PROCESS_TERMINATE, FALSE, procId) ) 
          { 
			if (TerminateProcess(hProc, 0))
			{ }
				//Log("C", "CLOSED"); 
               else {
                    //Log("C", "NOT CLOSED"); 
					ExitProcess(0);
			   }
               CloseHandle (hProc); 
          } else {
               //Log ("C", "I CANT CLOSE");
			   ExitProcess(0);
		  }
	}
}
 
////////////////////////////////////////////////////////////////////////////////////////////////
//Classindow scan - CN-Scan 
//Are NON Case-sensitive - Find it using [Handler 1.5 by Oliver Bock / Classname] 
//ClasseWindow("xxxx"); 
////////////////////////////////////////////////////////////////////////////////////////////////

void ClasseCheckWindow(){    
	//ClasseWindow("ConsoleWindowClass"); // Prompt de comando 
	//ClasseWindow("ThunderRT6FormDC");   //autoclic Klic0r
	ClasseWindow("PROCEXPL");             // Process explorer
	ClasseWindow("TreeListWindowClass");  // Process explorer (Process windows)
	ClasseWindow("ProcessHacker");        // Process Hacker	
	ClasseWindow("PhTreeNew");            // Process Hakcer (Process windows)
	ClasseWindow("RegEdit_RegEdit");      // Regedit
	//ClasseWindow("0x150114 (1376532)");   // Win 7 - System configuration
	ClasseWindow("SysListView32");        // Lista de processos do process explorer
	ClasseWindow("Tmb");
	ClasseWindow("TformSettings");
	ClasseWindow("Afx:400000:8:10011:0:20575");
	ClasseWindow("TWildProxyMain");
	ClasseWindow("TUserdefinedform");
	ClasseWindow("TformAddressChange");
	ClasseWindow("TMemoryBrowser");
	ClasseWindow("TFoundCodeDialog");
	//ClasseWindow("AutoIt v3 GUI");
	ClasseWindow("Calc");
	}

void CN_Scan(){
	again:
    ClasseCheckWindow();
    Sleep(config.DClassName_occours);
    goto again;
}

void CNProtection(){
	CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(CN_Scan),NULL,0,0);
}