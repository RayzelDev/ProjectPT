#include "../StdAfx.h"

bool TitleWindow(LPCSTR WindowTitle){
	HWND WinTitle = FindWindowA(NULL,WindowTitle);
	if( WinTitle > 0){
		Log("W", (char *)WindowTitle);
		ExitProcess(0);
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//Window name scan - w-scan 
//Are Case-sensitive - Find it just looking window name or using [Handler 1.5 by Oliver Bock] 
//TitleWindow("xxxx");
////////////////////////////////////////////////////////////////////////////////////////////////

void TitleCheckWindow(){
TitleWindow("!xSpeed 6.0");
TitleWindow("!xSpeed 6.0");
TitleWindow("!xSpeed.net 2");
TitleWindow("!xSpeed.net 3");
TitleWindow("!xSpeed.net 6");
TitleWindow("!xSpeed.net");
TitleWindow("!xSpeedPro");
TitleWindow("!xpeed.net 1.41");
TitleWindow("99QJ MU Bot");
TitleWindow("AE Bot v1.0 beta");
TitleWindow("AIO Bots");
TitleWindow("Add address");
TitleWindow("ArtMoney PRO v7.27");
TitleWindow("ArtMoney SE v7.31");
TitleWindow("ArtMoney SE v7.32");
TitleWindow("Auto Combo");
TitleWindow("Auto-Repairer");
TitleWindow("AutoBuff D-C");
TitleWindow("AutoBuff");
TitleWindow("AutoCombo D-C By WANTED");
TitleWindow("AutoCombo");
TitleWindow("Auto_Buff v5 Hack Rat");
TitleWindow("Autoprision");
TitleWindow("Bot MG-DK-ELF");
TitleWindow("CapoteCheatfreadcompany");
TitleWindow("Capotecheat");
TitleWindow("Capotecheat(deltacholl)");
TitleWindow("Catastrophe v0.1");
TitleWindow("Catastrophe v1.2");
TitleWindow("Catastrophe");
TitleWindow("Chaos Bot 2.1.0");
TitleWindow("CharBlaster");
TitleWindow("CharEditor (www.darkhacker.com.ar)");
TitleWindow("Cheat Engine 5.0");
TitleWindow("Cheat Engine 5.1");
TitleWindow("Cheat Engine 5.1.1");
TitleWindow("Cheat Engine 5.2");
TitleWindow("Cheat Engine 5.3");
TitleWindow("Cheat Engine 5.4");
TitleWindow("Cheat Engine 5.5");
TitleWindow("Cheat Engine 5.6");
TitleWindow("Cheat Engine 5.6.1");
TitleWindow("Cheat Engine 6.0");
TitleWindow("Cheat Engine 6.1");
TitleWindow("Cheat Engine 6.4");
TitleWindow("Cheat Engine 6.5");
TitleWindow("Cheat Engine");
TitleWindow("Cheat Happens v3.95b1/b2");
TitleWindow("Cheat Happens v3.95b3");
TitleWindow("Cheat Happens v3.96b2");
TitleWindow("Cheat Happens v3.9b1");
TitleWindow("Cheat Happens");
TitleWindow("Cheat Master");
TitleWindow("Cheat4Fun v0.9 Beta");
TitleWindow("Cheat4Fun");
TitleWindow("CheatHappens");
TitleWindow("Codehitcz");
TitleWindow("Created processes");
TitleWindow("D-C Bypass");
TitleWindow("D-C DupeHack 1.0");
TitleWindow("D-C Master Inject v1.0 by WANTED");
TitleWindow("DC Mu 1.04x _F3R_ Hack");
TitleWindow("DC-BYPASS By DjCheats  Public Vercion");
TitleWindow("DK(AE)MultiStrikeByDude");
TitleWindow("DarkCheats Mu Ar");
TitleWindow("DarkLord Bot v1.1");
TitleWindow("DarkyStats (www.darkhacker.com.ar)");
TitleWindow("Dizzys Auto Buff");
TitleWindow("Dupe-Full");
TitleWindow("Easy As MuPie");
TitleWindow("Esperando Mu Online");
TitleWindow("FunnyZhyper v5");
TitleWindow("FunnyZhyper");
TitleWindow("Game Speed Adjuster");
TitleWindow("Game Speed Changer");
TitleWindow("GodMode");
TitleWindow("Godlike");
TitleWindow("HiDeToolz");
TitleWindow("HideToolz");
TitleWindow("Hit Count");
TitleWindow("Hit Hack");
TitleWindow("Injector");
TitleWindow("Janopn Mini Multi Cheat v0.1");
TitleWindow("Jewel Drop Beta");
TitleWindow("JoyToKey");
TitleWindow("Kill");
TitleWindow("Lipsum v1 and v2");
TitleWindow("Lipsum_v2");
TitleWindow("List");
TitleWindow("Load File");
TitleWindow("Load");
TitleWindow("MJB Perfect DL Bot");
TitleWindow("MLEngine");
TitleWindow("MUSH");
TitleWindow("Minimize");
TitleWindow("ModzMu");
TitleWindow("MoonLight");
TitleWindow("Mugster Bot");
TitleWindow("Mupie Minimizer");
TitleWindow("Mush");
TitleWindow("NoNameMini");
TitleWindow("Nsauditor 1.9.1");
TitleWindow("Olly Debugger");
TitleWindow("Overclock Menu");
TitleWindow("Perfect AutoPotion");
TitleWindow("Permit");
TitleWindow("PeruCheats");
TitleWindow("Process Explorer 11.33");
TitleWindow("Process Explorer");
TitleWindow("ProxCheatsX 2.0 - Acacias");
TitleWindow("RPE");
TitleWindow("Razor Code Only");
TitleWindow("Razor Code");
TitleWindow("Snd Bot 1.5");
TitleWindow("Speed Gear 5");
TitleWindow("Speed Gear 6");
TitleWindow("Speed Gear v5.00");
TitleWindow("Speed Gear");
TitleWindow("Speed Hack 99.62t");
TitleWindow("Speed Hack Simplifier 1.0-1.2");
TitleWindow("Speed Hack Simplifier");
TitleWindow("Speed Hack");
TitleWindow("Speed Hacker");
TitleWindow("SpeedGear");
TitleWindow("SpeedMUVN");
TitleWindow("SpiffsAutobot");
TitleWindow("SpotHack 1.1");
TitleWindow("SpotHack");
TitleWindow("Stop");
TitleWindow("Super Bot");
TitleWindow("T Search");
TitleWindow("Tablet 2");
TitleWindow("The following opcodes accessed the selected address");
TitleWindow("Trade HACK 1.8");
TitleWindow("Ultimate Cheat");
TitleWindow("UoPilot  v2.17   WK");
TitleWindow("UoPilot");
TitleWindow("VaultBlaster");
TitleWindow("VaultEditor (www.darkhacker.com.ar)");
TitleWindow("WPE PRO");
TitleWindow("WPePro 0.9a");
TitleWindow("WPePro 1.3");
TitleWindow("Wall");
TitleWindow("WildProxy 1.0 Alpha");
TitleWindow("WildProxy v0.1");
TitleWindow("WildProxy v0.2");
TitleWindow("WildProxy v0.3");
TitleWindow("WildProxy v1.0 Public");
TitleWindow("WildProxy");
TitleWindow("Xelerator 1.4");
TitleWindow("Xelerator 2.0");
TitleWindow("Xelerator");
TitleWindow("ZhyperMu Packet Editor");
TitleWindow("[Dark-Cheats] MultiD-C");
TitleWindow("hacker");	
TitleWindow("rPE - rEdoX Packet Editor");
TitleWindow("razorcode");
TitleWindow("speed");
TitleWindow("speednet");
TitleWindow("speednet2");
TitleWindow("www.55xp.com");
TitleWindow("xSpeed.net 3.0");
TitleWindow("Windows Task Manager");
TitleWindow("BVKHEX");
TitleWindow("OllyDbg");
TitleWindow("HxD");
TitleWindow("hack");
TitleWindow("MHS");
TitleWindow("hanhtrinhamnhac");
}

void W_Scan(){
	again:
    TitleCheckWindow();
    Sleep(config.DWindow_occours);
    goto again;
}

void DwProtection(){
	CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(W_Scan),NULL,0,0);
}