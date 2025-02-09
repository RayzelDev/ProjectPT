#include "MixWindow.h"
#include "imGui/imgui.h"
#include <algorithm>
#include <d3d9.h>

#define MAX_MIX_COUNT 2

static int selected = 1;

std::vector<MixInfo> allMix;

char* SheltonsName[] =
{
	"null",
	"Lucidy",
	"Sereno",
	"Fadeo",
	"Sparky",
	"Raident",
	"Transparo",
	"Murki",
	"Devine",
	"Celesto",
	"Mirage",
	"Inferna",
	"Enigma",
	"Bellum",
	"Ordo"
};

PDIRECT3DTEXTURE9 my_texture2[15];
LPDIRECT3DTEXTURE9 Image2[15];

extern LPDIRECT3DTEXTURE9 LoadDibSurfaceOffscreen(char* Filename);
extern bool LoadTextureFromFile2(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height);

char Path2[64] = { 0 };
bool first = true;

void readTextures()
{
	int w = 0;
	int h = 0;

	for (int x = 1; x <= 14; x++)
	{
		sprintf_s(Path2, sizeof(Path2), "game\\images\\Mix\\%d.png", x);
		bool ret = LoadTextureFromFile2(Path2, &my_texture2[x], &w, &h);
		IM_ASSERT(ret);

		Image2[x] = LoadDibSurfaceOffscreen(Path2);
	}

	first = false;
}

INT MixWindow::ReadMix()
{
	allMix.clear();

	if (first)
		readTextures();

	MixInfo Mix;

	// ARMAS

	Mix.ID = 1;
	Mix.mixName = "Mix Inicial";
	Mix.description = u8"ATK MIN +10 - MAX +10; TAXA DE ATK +50; CRÍT. +3; MANA +15";
	Mix.mixType = MixType::Weapons;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 12);

	allMix.push_back(Mix);


	Mix.ID = 2;
	Mix.mixName = "Mix Veterano";
	Mix.description = u8"ATK MIN +20 - MAX +20; TAXA DE ATK +200; CRÍT. +5; MANA +50";
	Mix.mixType = MixType::Weapons;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Mirage, 12);

	allMix.push_back(Mix);

	Mix.ID = 3;
	Mix.mixName = "Mix Senhor da Guerra";
	Mix.description = u8"ATK MIN +40 - MAX +40; TAXA DE ATK +300; CRÍT. +10; MANA +150";
	Mix.mixType = MixType::Weapons;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Inferna, 12);

	allMix.push_back(Mix);

	Mix.ID = 4;
	Mix.mixName = "Mix dos Deuses";
	Mix.description = u8"ATK MIN +80 - MAX +80; TAXA DE ATK +500; CRÍT. +20; MANA +200";
	Mix.mixType = MixType::Weapons;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Enigma, 12);

	allMix.push_back(Mix);


	// ARMADURAS E ROUPÕES

	Mix.ID = 5;
	Mix.mixName = "Mix Inicial";
	Mix.description = u8"DEF. +150; ABS. +5.0; HP +50";
	Mix.mixType = MixType::Armors;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 12);

	allMix.push_back(Mix);

	Mix.ID = 6;
	Mix.mixName = "Mix Veterano";
	Mix.description = u8"DEF. +350; ABS. +9.0; HP +150";
	Mix.mixType = MixType::Armors;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Mirage, 12);

	allMix.push_back(Mix);

	Mix.ID = 7;
	Mix.mixName = "Mix Senhor da Guerra";
	Mix.description = u8"DEF. +750; ABS. +11.0; HP +250";
	Mix.mixType = MixType::Armors;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Inferna, 12);

	allMix.push_back(Mix);

	Mix.ID = 8;
	Mix.mixName = "Mix dos Deuses";
	Mix.description = u8"DEF. +1550; ABS. +20.0; HP +350";
	Mix.mixType = MixType::Armors;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Enigma, 12);

	allMix.push_back(Mix);

	// ESCUDOS

	Mix.ID = 9;
	Mix.mixName = "Mix Inicial";
	Mix.description = u8"BLOQUEIO +5%; ABS. +3.0; HP +50";
	Mix.mixType = MixType::Shields;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 12);

	allMix.push_back(Mix);

	Mix.ID = 10;
	Mix.mixName = "Mix Veterano";
	Mix.description = u8"BLOQUEIO +10%; ABS. +5.0; HP +150";
	Mix.mixType = MixType::Shields;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Mirage, 12);

	allMix.push_back(Mix);

	Mix.ID = 11;
	Mix.mixName = "Mix Senhor da Guerra";
	Mix.description = u8"BLOQUEIO +15%; ABS. +7.0; HP +250";
	Mix.mixType = MixType::Shields;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Inferna, 12);

	allMix.push_back(Mix);

	Mix.ID = 12;
	Mix.mixName = "Mix dos Deuses";
	Mix.description = u8"BLOQUEIO +25%; ABS. +10.0; HP +350";
	Mix.mixType = MixType::Shields;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Enigma, 12);

	allMix.push_back(Mix);


	// ORBITAIS

	Mix.ID = 13;
	Mix.mixName = "Mix Inicial";
	Mix.description = u8"DEF. +155; ABS. +3.0; HP +50, RES +100";
	Mix.mixType = MixType::Orbs;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 12);

	allMix.push_back(Mix);

	Mix.ID = 14;
	Mix.mixName = "Mix Veterano";
	Mix.description = u8"DEF. +255; ABS. +5.0; HP +150, RES +200";
	Mix.mixType = MixType::Orbs;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Mirage, 12);

	allMix.push_back(Mix);

	Mix.ID = 15;
	Mix.mixName = "Mix Senhor da Guerra";
	Mix.description = u8"DEF. +455; ABS. +7.0; HP +250, RES +300";
	Mix.mixType = MixType::Orbs;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Inferna, 12);

	allMix.push_back(Mix);

	Mix.ID = 16;
	Mix.mixName = "Mix dos Deuses";
	Mix.description = u8"DEF. +755; ABS. +15.0; HP +350, RES +500";
	Mix.mixType = MixType::Orbs;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Enigma, 12);

	allMix.push_back(Mix);

	// BOTAS

	Mix.ID = 17;
	Mix.mixName = "Mix Inicial";
	Mix.description = u8"DEF. +115; ABS. +5.0; VELOCIDADE +3.0; HP +50; MANA +45; RES +50";
	Mix.mixType = MixType::Boots;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 12);

	allMix.push_back(Mix);

	Mix.ID = 18;
	Mix.mixName = "Mix Veterano";
	Mix.description = u8"DEF. +255; ABS. +7.0; VELOCIDADE +4.0; HP +150; MANA +145; RES +150";
	Mix.mixType = MixType::Boots;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Mirage, 12);

	allMix.push_back(Mix);

	Mix.ID = 19;
	Mix.mixName = "Mix Senhor da Guerra";
	Mix.description = u8"DEF. +355; ABS. +10.0; VELOCIDADE +5.0; HP +250; MANA +250; RES +250";
	Mix.mixType = MixType::Boots;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Inferna, 12);

	allMix.push_back(Mix);

	Mix.ID = 20;
	Mix.mixName = "Mix do Deuses";
	Mix.description = u8"DEF. +555; ABS. +15.0; VELOCIDADE +7.0; HP +350; MANA +350; RES +350";
	Mix.mixType = MixType::Boots;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Enigma, 12);

	allMix.push_back(Mix);


	// LUVAS

	Mix.ID = 21;
	Mix.mixName = "Mix Inicial";
	Mix.description = u8"DEF. +130; ABS. +4.0; HP +50; MANA +50; RES +50";
	Mix.mixType = MixType::Gloves;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 12);

	allMix.push_back(Mix);

	Mix.ID = 22;
	Mix.mixName = "Mix Veterano";
	Mix.description = u8"DEF. +230; ABS. +6.0; HP +120; MANA +150; RES +150";
	Mix.mixType = MixType::Gloves;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Mirage, 12);

	allMix.push_back(Mix);

	Mix.ID = 23;
	Mix.mixName = "Mix Senhor da Guerra";
	Mix.description = u8"DEF. +330; ABS. +8.0; HP +250; MANA +250; RES +250";
	Mix.mixType = MixType::Gloves;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Inferna, 12);

	allMix.push_back(Mix);

	Mix.ID = 24;
	Mix.mixName = "Mix dos Deuses";
	Mix.description = u8"DEF. +530; ABS. +12.0; HP +350; MANA +350; RES +350";
	Mix.mixType = MixType::Gloves;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Enigma, 12);

	allMix.push_back(Mix);

	// BRACELETES

	Mix.ID = 25;
	Mix.mixName = "Mix Inicial";
	Mix.description = u8"DEF. +125; TAXA DE ATK +140; HP +50; MANA +55; RES +50";
	Mix.mixType = MixType::Bracelets;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 12);

	allMix.push_back(Mix);

	Mix.ID = 26;
	Mix.mixName = "Mix Veterano";
	Mix.description = u8"DEF. +250; TAXA DE ATK +240; HP +150; MANA +155; RES +150";
	Mix.mixType = MixType::Bracelets;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Mirage, 12);

	allMix.push_back(Mix);

	Mix.ID = 27;
	Mix.mixName = "Mix Senhor da Guerra";
	Mix.description = u8"DEF. +350; TAXA DE ATK +350; HP +250; MANA +255; RES +250";
	Mix.mixType = MixType::Bracelets;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Inferna, 12);

	allMix.push_back(Mix);

	Mix.ID = 28;
	Mix.mixName = "Mix dos Deuses";
	Mix.description = u8"DEF. +550; TAXA DE ATK +550; HP +350; MANA +355; RES +350";
	Mix.mixType = MixType::Bracelets;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Enigma, 12);

	allMix.push_back(Mix);

	// AMULETOS

	Mix.ID = 29;
	Mix.mixName = "Mix Inicial";
	Mix.description = u8"REG. de HP +2.0; REG. de MANA +2.0; HP +50; MANA +50; RES +50";
	Mix.mixType = MixType::Amulets;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 12);

	allMix.push_back(Mix);

	Mix.ID = 30;
	Mix.mixName = "Mix Veterano";
	Mix.description = u8"REG. de HP +3.0; REG. de MANA +4.0; HP +150; MANA +150; RES +150";
	Mix.mixType = MixType::Amulets;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Mirage, 12);

	allMix.push_back(Mix);

	Mix.ID = 31;
	Mix.mixName = "Mix Senhor da Guerra";
	Mix.description = u8"REG. de HP +5.0; REG. de MANA +5.0; HP +250; MANA +250; RES +250";
	Mix.mixType = MixType::Amulets;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Inferna, 12);

	allMix.push_back(Mix);

	Mix.ID = 32;
	Mix.mixName = "Mix dos Deuses";
	Mix.description = u8"REG. de HP +7.0; REG. de MANA +7.0; HP +350; MANA +350; RES +350";
	Mix.mixType = MixType::Amulets;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Enigma, 12);

	allMix.push_back(Mix);

	// ANÉIS

	Mix.ID = 33;
	Mix.mixName = "Mix Inicial";
	Mix.description = u8"REG. de HP +2.0; REG. de MP +2.0; HP +75; MANA +75; RES +75; Tx de Atk +150";
	Mix.mixType = MixType::Rings;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 12);

	allMix.push_back(Mix);

	Mix.ID = 34;
	Mix.mixName = "Mix Veterano";
	Mix.description = u8"REG. de HP +3.0; REG. de MP +3.0; HP +155; MANA +175; RES +175; Tx de Atk +250";
	Mix.mixType = MixType::Rings;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Mirage, 12);

	allMix.push_back(Mix);

	Mix.ID = 35;
	Mix.mixName = "Mix Senhor da Guerra";
	Mix.description = u8"REG. de HP +4.0; REG. de MP +4.0; HP +255; MANA +275; RES +275; Tx de Atk +350";
	Mix.mixType = MixType::Rings;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Inferna, 12);

	allMix.push_back(Mix);

	Mix.ID = 36;
	Mix.mixName = "Mix dos Deuses";
	Mix.description = u8"REG. de HP +5.0; REG. de MP +5.0; HP +355; MANA +375; RES +375; Tx de Atk +450";
	Mix.mixType = MixType::Rings;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Enigma, 12);

	allMix.push_back(Mix);

	openFlag = true;

	return TRUE;
}

extern int smScreenWidth;
extern int smScreenHeight;

int ID = 0;

void MixWindow::OpenNpc(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(450, 500), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2((smScreenWidth / 2) - 250, (smScreenHeight / 2) - 260));
	ImGui::SetNextWindowBgAlpha(0.80f);

	ImGui::StyleColorArmageddon();


	if (ImGui::Begin("Lista de Mix - Project Priston Tale", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
	{
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

		if (ImGui::BeginTabBar("Listas", tab_bar_flags))
		{
			if (ImGui::BeginTabItem(u8"Armas"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Weapons)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);




						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::Separator();
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"Armaduras e Robes"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Armors)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);

						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Separator();
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTabItem();
			}


			if (ImGui::BeginTabItem(u8"Escudos"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Shields)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);

						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Separator();
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"Orbs"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Orbs)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);

						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Separator();
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"Botas"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Boots)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);

						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Separator();
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"Luvas"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Gloves)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);

						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Separator();
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"Braceletes"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Bracelets)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);

						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Separator();
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"Amuletos"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Amulets)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);

						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Separator();
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"Anéis"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Rings)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);

						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Separator();
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTabItem();
			}


			ImGui::EndTabBar();
		}

		ImGui::End();
	}

}
