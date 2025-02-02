#include <windows.h>
#include <winnt.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <string>
#include <algorithm>

//#include "md5.h"

#pragma comment(lib, "shlwapi.lib")

// Defines
#define MAX_CLOAKED_DLLS 10
#define UNLINK(x) (x).Blink->Flink = (x).Flink; (x).Flink->Blink = (x).Blink;
#define RELINK(x,p) (x).Blink->Flink = (p);       (x).Flink->Blink = (p);

// Types
#pragma pack(push, 1)

typedef struct _UNICODE_STRING {
  USHORT  Length;
  USHORT  MaximumLength;
  PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _ModuleInfoNode
{
    LIST_ENTRY LoadOrder;
    LIST_ENTRY InitOrder;
    LIST_ENTRY MemoryOrder;
    HMODULE baseAddress;        //  Base address AKA module handle
    unsigned long entryPoint;
    unsigned int size;          //  Size of the modules image
    UNICODE_STRING fullPath;
    UNICODE_STRING name;
    unsigned long flags;
    unsigned short LoadCount;
    unsigned short TlsIndex;
    LIST_ENTRY HashTable;   //  A linked list of any other modules that have the same first letter
    unsigned long timestamp;
} ModuleInfoNode, *pModuleInfoNode;

typedef struct _ProcessModuleInfo
{
    unsigned int size;          //  Size of a ModuleInfo node?
    unsigned int initialized;
    HANDLE SsHandle;
    LIST_ENTRY LoadOrder;
    LIST_ENTRY InitOrder;
    LIST_ENTRY MemoryOrder;
} ProcessModuleInfo, *pProcessModuleInfo;

/*typedef struct BlockInfo {
  const char *name;

  const char *hash;
};*/


#pragma pack(pop)

//extern void LogMessage (const char* format, ...) ;

void CloakDll_stub();
void CD_stubend();

bool CloakDll(char *, char *);
unsigned long GetProcessIdFromProcname(char *);
HMODULE GetRemoteModuleHandle(unsigned long, char *);

/*BlockInfo Blocklist[] = {
  // EXAMPLE:
  // { "uxtheme.dll", ALL_VERSIONS },
  // { "uxtheme.dll", 0x0000123400000000ULL },
  // The DLL name must be in lowercase!

  // NPFFAddon - Known malware
  //{ "game.dll", "8af8dcd3b0f0f8888dae56b6dc1c1471"},
  //{ "handes.dll", "ea4b691f4d26806343a2c24251b0e9a7"},
  //{ "pt.dll", "7acbb7b16ef3ce00881409210e6b24e2"},
 // { "ptv.dll", "xxx"},
  //{ "ptv.lix", "xxx"},
  { "game.exe", "xxx"},


  { NULL, 0 }
};*/

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}


bool CloakDll(char *process, char *dllName)
{
    return true;
}

void UnloadDLL(HMODULE hModule)
	{
		LPVOID ExitThreadAddress = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "ExitThread");
		
		_asm
		{
			push hModule
			push ExitThreadAddress
			jmp dword ptr [FreeLibrary]
		}
	}

void CloakDll_stub()
{
    ProcessModuleInfo *pmInfo;
    ModuleInfoNode *module;
	/*
    asm("movl %fs:0x18, %eax\n\t"       // TEB
        "movl 0x30(%eax), %eax\n\t"     // PEB
        "movl 0x0C(%eax), %eax\n\t");   // PROCESS_MODULE_INFO
    asm("movl %%eax, %0" : "=r" (pmInfo) : :);
	*/
    _asm
    {
        mov eax, fs:[18h]       // TEB
        mov eax, [eax + 30h]    // PEB
        mov eax, [eax + 0Ch]    // PROCESS_MODULE_INFO
        mov pmInfo, eax
    }

    module = (ModuleInfoNode *)(pmInfo->LoadOrder.Flink);

    std::string gameDir;
    bool first = true;

	do {

		if (!module->fullPath.Buffer) continue;
		char *szTo = new char[module->fullPath.Length + 1];
		szTo[module->fullPath.Length] = '\0';
		WideCharToMultiByte(CP_ACP, 0, module->fullPath.Buffer, -1, szTo, (int)module->fullPath.Length, NULL, NULL);

		szTo = trimwhitespace(szTo);

		std::string full_path(szTo);
		std::transform(full_path.begin(), full_path.end(), full_path.begin(), ::tolower);

		//LogMessage("DLL: %s", full_path.c_str());
		delete[] szTo;

		if (full_path.rfind('\\') == std::string::npos)
		{
			//LogMessage("....");
			continue;
		}
		if (gameDir.empty()) {


			gameDir = full_path.substr(0, full_path.rfind('\\'));

			//LogMessage("Path: %s", gameDir.c_str());
		}

		if (first) { first = false; continue; }

		if (full_path.empty()) continue;

		if (full_path.find(gameDir) != std::string::npos) {

			/* BlockInfo *info = &Blocklist[0];
			 while (info->name) {
				 if (full_path.find(info->name) != std::string::npos)
					 break;
				 info++;
			 }
			 */
			 //if (info->name) {
				 //MD5 md5;
				 //char *tmp = md5.digestFile(full_path.c_str());
				 //OutputDebugString(tmp);
				 //if(strcmp(info->hash, tmp) == 0 || strcmp(info->hash, "xxx") == 0)
					 //continue;
				 //else {
					 //ExitProcess(1);
			//UnloadDLL(module->baseAddress);
			//}
			//free(tmp);
		//}
		//else {
			//ExitProcess(1);
		} else {
            if(full_path.find(":\\windows") != std::string::npos || full_path.find(":\\program files") != std::string::npos) {
                continue;
            } else {
                //ExitProcess(1);
				UnloadDLL(module->baseAddress);
            }
        }
    } while(module->baseAddress && (module = (ModuleInfoNode *)(module->LoadOrder.Flink)));
}

__declspec(naked) void CD_stubend() { }

unsigned long GetProcessIdFromProcname(char *procName)
{
   PROCESSENTRY32 pe;
   HANDLE thSnapshot;
   BOOL retval, ProcFound = false;

   thSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

   if(thSnapshot == INVALID_HANDLE_VALUE)
   {
      MessageBox(NULL, "Error: unable to create toolhelp snapshot", "Loader", NULL);
      return false;
   }

   pe.dwSize = sizeof(PROCESSENTRY32);

    retval = Process32First(thSnapshot, &pe);

   while(retval)
   {
      if(StrStrI(pe.szExeFile, procName) )
      {
         ProcFound = true;
         break;
      }

      retval    = Process32Next(thSnapshot,&pe);
      pe.dwSize = sizeof(PROCESSENTRY32);
   }

   return pe.th32ProcessID;
}

HMODULE GetRemoteModuleHandle(unsigned long pId, char *module)
{
    MODULEENTRY32 modEntry;
    HANDLE tlh = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pId);

    modEntry.dwSize = sizeof(MODULEENTRY32);
    Module32First(tlh, &modEntry);

    do
    {
        if(!stricmp(modEntry.szModule, module))
            return modEntry.hModule;
        modEntry.dwSize = sizeof(MODULEENTRY32);
    }
    while(Module32Next(tlh, &modEntry));

    return NULL;
}

void __cdecl DllAnti() {
	while(1) {
		CloakDll_stub();
		Sleep(5000);
	}
}