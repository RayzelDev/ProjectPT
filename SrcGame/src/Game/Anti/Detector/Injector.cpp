#include "../stdafx.h"

//Show system list of system window names ///////////////////////////
#define CREATE_THREAD_ACCESS ( PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ )
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
int WINAPI WinMainChdeck(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int iCmdShow){
	EnumWindows(EnumWindowsProc, NULL);
	return 0;
}
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	char class_name[80];
	char title[80];
	GetClassName(hwnd,class_name, sizeof(class_name));
	GetWindowText(hwnd,title,sizeof(title));
    cout <<"Window title: "<<title<<endl;
    //cout <<"Class name: "<<class_name<<endl<<endl;
	return TRUE;
}
void winchedks(){
	CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(WinMainChdeck),NULL,0,0);
}


void DllProtec2(){
HWND hWnd = FindWindow( 0,  "PTV");  
if ( hWnd == 0 )
{
//cout << "Could not find window 2" << endl;
}
else
{
//cout << "Window 2 found" << endl;
}
DWORD proc_id;
GetWindowThreadProcessId( hWnd, &proc_id );
char DLL_LOCATION[ MAX_PATH ] = { 0 };
GetFullPathName( carrega.Inject_Dll_Value, MAX_PATH, DLL_LOCATION, NULL );
if ( !startInject( proc_id, DLL_LOCATION ) )
{
//cout << carrega.Injected_Window_2 << " = FAIL!" << endl;
}
else
{
//cout << carrega.Injected_Window_2 << " = Sucess!" << endl;
}
}


int DllProtect()
{
  ifstream ifile (carrega.Inject_Dll_Value); 
  if (!ifile){
  Dllf();
//cout <<"c.dll don't found!"<<endl;
  }
  else
  {
	Sleep (1500);
	DllProtec2();
	Sleep (1500);
	Sleep (1500);
  	return(0);
}
}


bool startInject( DWORD pID, const char* DLL_LOCATION )
{
HMODULE hLib;
char buf[ 50 ] = { 0 };
if ( !pID )
return false;
//cout << endl << endl << "pID: " << pID << endl;
HANDLE hProc = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pID );
if ( !hProc )
{
//sprintf( buf, "OpenProcess failed: %d", GetLastError( ) );
//printf( buf );
return false;
}
int injectionType = 0;
//cout << endl << endl << "Enter an integer to specify the injection type: ";
//cin >> injectionType;
switch( injectionType )
{
case 0:
{
return injectMethodA( hProc, DLL_LOCATION );
}
break;
default:
{
//cout << "Invalid choice...";
//cout << endl << endl << "Enter an integer to specify the injection type: ";
//cin >> injectionType;
}
break;
}

return true;
}
bool injectMethodA( HANDLE hProc, const char* DLL_LOCATION )
{
LPVOID RemoteString, LoadLibAdd;
LoadLibAdd = ( LPVOID )GetProcAddress( GetModuleHandle( "kernel32.dll" ), "LoadLibraryA" );
RemoteString = ( LPVOID )VirtualAllocEx( hProc, NULL, strlen( DLL_LOCATION ), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
WriteProcessMemory( hProc, ( LPVOID )RemoteString, DLL_LOCATION, strlen( DLL_LOCATION ), NULL );
CreateRemoteThread( hProc, NULL, NULL, ( LPTHREAD_START_ROUTINE )LoadLibAdd, ( LPVOID )RemoteString, NULL, NULL );
CloseHandle( hProc );
return true;
}
// DLL INJECT END /////////////////////////////////////////////////////////////


int Dll_Protection(){
    //Enabe Admin privileges on XP
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tkp;
    if(!OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ))
    {
		//cout << "1 false" <<endl;
        return FALSE;		
    }
    if(!LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &luid ))
    {
		//cout << "2 false" <<endl;
       return FALSE;	
    }
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = luid;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if(!AdjustTokenPrivileges( hToken, false, &tkp, sizeof( tkp ), NULL, NULL ))
    {
		//cout << "3 false" <<endl;
        return FALSE;	
    }
    if(!CloseHandle( hToken ))
    {
		//cout << "4 false" <<endl;
        return FALSE;		
    }
	//cout << " true" <<endl;
    CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(DllProtect),NULL,0,0);
	
    return TRUE;
}