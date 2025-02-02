#pragma once

using TextureID = int;

class cDrawHPBOSS
{
public:
    

    static cDrawHPBOSS* GetInstance() { static cDrawHPBOSS inst; return &inst; };

    void Init();
    void DrawHP(smCHAR* pcUnitData);

private:

    TextureID	m_LifeBar[2];

    char m_szBossName[32];
    int Images[7];
};

#define DrawHPBOSS cDrawHPBOSS::GetInstance()
