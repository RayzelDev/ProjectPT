#include "../stdafx.h"
#include "AntiInjection.h"
void Injetou()
{
	Sleep(2000);
	//Banir("Inject de dll");
	ExitProcess(0);
	ExitThread(0);
}
/*void Log1(const char* Log1)
{
	         FILE* file = NULL;
	         fopen_s(&file, "AntiInject.txt", "a");
			 fprintf_s(file,Log1);
	         fclose(file);
}*/

//Oo isso nem funciona o anti inject? '-' e funciona s '-' axo q n hein ele so hooka dps q o Knight Gunz abre se o cara injectar a dll ao abrir o Knight Gunz
DWORD g_dwLoadLibraryAJMP;

DWORD WINAPI jumphook( DWORD AddressToPerformJump, DWORD AddressOfMyFunction, DWORD LenghOfTheAreaToPerformTheJump	)
{
	if( LenghOfTheAreaToPerformTheJump < 5 )
		return 0;

	DWORD RelativeJump, 
		  NextInstructionAddress,
		  Flag;

	if ( ! VirtualProtect((LPVOID)AddressToPerformJump, LenghOfTheAreaToPerformTheJump, PAGE_EXECUTE_READWRITE, &Flag) )
		return 0;

	NextInstructionAddress = AddressToPerformJump + LenghOfTheAreaToPerformTheJump;

	*(BYTE*)AddressToPerformJump = 0xE9;//jmp

	for( DWORD i = 5; i < LenghOfTheAreaToPerformTheJump; i++)
		*(BYTE*)(AddressToPerformJump+i) = 0x90;//nop

	RelativeJump = AddressOfMyFunction - AddressToPerformJump - 0x5;

	*(DWORD*)(AddressToPerformJump + 0x1) = RelativeJump;

	VirtualProtect((LPVOID)AddressToPerformJump, LenghOfTheAreaToPerformTheJump, Flag, &Flag);

	return NextInstructionAddress; 
}


HMODULE WINAPI hLoadLibraryA( LPCSTR lpLibFileName )
{	
	__asm
	{
		mov eax, dword ptr ss:[esp + 0x18]//eax
		cmp dword ptr ds:[eax-0x12], 0x8B55FF8B//code cave principal goes here
		je erro// jump
	}
	

	if( lpLibFileName )
	{
		if( !strcmp( lpLibFileName, "twain_32.dll" ) )//hook library
			__asm jmp g_dwLoadLibraryAJMP
	}			

	return LoadLibraryExA( lpLibFileName, 0, 0 );

erro:


	                   CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Injetou, NULL, 0, NULL);
					   MessageBoxA(0, "[Anti-Hack] Injeção de DLL's detectado! Seu jogo será fechado.", "Brasil  Gunz Protect", MB_ICONSTOP);
					   Log1("Injeção de DLL's encontrado, seu jogo foi fechado.\n");
					   return 0;
}

void AntiInject()
{
	g_dwLoadLibraryAJMP = (DWORD)GetModuleHandle( "kernel32" ) + 0x6E2A1;

	jumphook( (DWORD)LoadLibraryA, (DWORD)&hLoadLibraryA, 57 );
}