// Video tutorial: http://www.youtube.com/user/vertexbrasil


#ifndef ANTIHACK_ANTIHACK_H
#define  ANTIHACK_ANTIHACK_H


#define  MAX_DUMP_OFFSETS 165
#define  MAX_DUMP_SIZE 16
#define  MAX_PROCESS_DUMP 165




typedef struct ANITHACK_PROCDUMP {
    unsigned int m_aOffset;
    unsigned char m_aMemDump[MAX_DUMP_SIZE];
} *PANITHACK_PROCDUMP;


extern ANITHACK_PROCDUMP g_ProcessesDumps[MAX_PROCESS_DUMP];


void SystemProcessesScan();
bool ScanProcessMemory(HANDLE hProcess);




#endif //ANTIHACK_ANTIHACK_H