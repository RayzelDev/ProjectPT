#pragma once

#define   SIN_ADD_FIRE			0x00000001	//Fogo
#define   SIN_ADD_ICE			0x00000002	//Gelo
#define   SIN_ADD_LIGHTNING			0x00000004	//Raio
#define   SIN_ADD_POISON			0x00000008	//Veneno
#define   SIN_ADD_BIO			0x00000010	//Orgânico
#define   SIN_ADD_Critico			0x00000020	//Crítico
#define   SIN_ADD_ATTACK_RATE		0x00000040	//Taxa de Ataque
#define   SIN_ADD_DAMAGE_MIN		0x00000080	//Poder de Ataque Min.
#define   SIN_ADD_DAMAGE_MAX		0x00000100	//Poder de Ataque Máx.
#define   SIN_ADD_ATTACK_SPEED		0x00000200	//Vel. de Ataque
#define   SIN_ADD_ABSORB			0x00000400	//Absorção
#define   SIN_ADD_DEFENCE			0x00000800	//Taxa de Defesa
#define   SIN_ADD_BLOCK_RATE		0x00001000	//Bloqueio
#define   SIN_ADD_MOVE_SPEED		0x00002000	//Velocidade
#define   SIN_ADD_LIFE			0x00004000	//HP Máx.
#define   SIN_ADD_MANA			0x00008000	//MP Máx.
#define   SIN_ADD_STAMINA			0x00010000	//RES>
#define   SIN_ADD_LIFEREGEN			0x00020000 	//Regen de HP.
#define   SIN_ADD_MANAREGEN		0x00040000  	//Regen de Mana.
#define   SIN_ADD_STAMINAREGEN		0x00080000  	//Regen de RES.

#define   SIN_ADD_NUM				1
#define   SIN_ADD_PERCENT			2


//--------------------------------------------------- INICIO ARMAS ---------------------------------------------------
// Mix Inicial Armas 12 Celestos
{ 1, { sinWA1, sinWC1, sinWH1, sinWM1, sinWP1, sinWS1, sinWS2, sinWT1 },
// Lucidi, Sereno, Fadeo, Spark, Raidente, Transparo, Murki, Devine, Celesto, Mirage, Inferna, Enigma, Bellum
	{ 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0 },
	{ SIN_ADD_DAMAGE_MIN, SIN_ADD_DAMAGE_MAX, SIN_ADD_ATTACK_RATE, SIN_ADD_CRITICAL, SIN_ADD_MANA,0 , 0, 0 },
	{ 10, 10, 50, 3, 15, 0, 0, 0 },
	{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
	"+10 de Poder de Ataque Min\r+10 de Poder de Ataque Max\r+50 de Taxa de Ataque\r+3 de Crítico\r+15 de Mana\r"},

	// Mix Veterano 12 Mirages Armas
{ 2, { sinWA1, sinWC1, sinWH1, sinWM1, sinWP1, sinWS1, sinWS2, sinWT1 },
// Lucidi, Sereno, Fadeo, Spark, Raidente, Transparo, Murki, Devine, Celesto, Mirage, Inferna, Enigma, Bellum, Ordo
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0 },
	{ SIN_ADD_DAMAGE_MIN, SIN_ADD_DAMAGE_MAX, SIN_ADD_ATTACK_RATE, SIN_ADD_CRITICAL, SIN_ADD_MANA,0 , 0, 0 },
	{ 20, 20, 200, 5, 50, 0, 0, 0 },
	{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
	"+20 de Poder de Ataque Min\r+20 de Poder de Ataque Max\r+200 de Taxa de Ataque\r+5 de Crítico\r+50 de Mana\r" },

	// Mix Senhor da Guerra 12 Infernas Armas
{ 3, { sinWA1, sinWC1, sinWH1, sinWM1, sinWP1, sinWS1, sinWS2, sinWT1 },
// Lucidi, Sereno, Fadeo, Spark, Raidente, Transparo, Murki, Devine, Celesto, Mirage, Inferna, Enigma, Bellum, Ordo
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0 },
	{ SIN_ADD_DAMAGE_MIN, SIN_ADD_DAMAGE_MAX, SIN_ADD_ATTACK_RATE, SIN_ADD_CRITICAL, SIN_ADD_MANA,0 , 0, 0 },
	{ 40, 40, 300, 10, 150, 0, 0, 0 },
	{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
	"+40 de Poder de Ataque Min\r+40 de Poder de Ataque Max\r+300 de Taxa de Ataque\r+10 de Crítico\r+150 de Mana\r" },

	// Mix dos Deuses 12 Enigmas Armas
{ 4, { sinWA1, sinWC1, sinWH1, sinWM1, sinWP1, sinWS1, sinWS2, sinWT1 },
// Lucidi, Sereno, Fadeo, Spark, Raidente, Transparo, Murki, Devine, Celesto, Mirage, Inferna, Enigma, Bellum, Ordo
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0 },
	{ SIN_ADD_DAMAGE_MIN, SIN_ADD_DAMAGE_MAX, SIN_ADD_ATTACK_RATE, SIN_ADD_CRITICAL, SIN_ADD_MANA,0 , 0, 0 },
	{ 80, 80, 500, 20, 200, 0, 0, 0 },
	{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
	"+80 de Poder de Ataque Min\r+80 de Poder de Ataque Max\r+500 de Taxa de Ataque\r+20 de Crítico\r+200 de Mana\r" },
	//--------------------------------------------------- FIM ARMAS ---------------------------------------------------

	//--------------------------------------------------- INICIO ARMADURAS --------------------------------------------
	// Mix Iniciante 12 Celestos Armaduras & Roupões
{ 5, {sinDA1, sinDA2, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
{ 150, 5, 50, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+150 de Defesa\r+5 de ABS\r+50 de Vida\r" },

// Mix Veterano 12 Mirages Armaduras & Roupões
{ 6, {sinDA1, sinDA2, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
{ 350, 9, 150, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+350 de Defesa\r+9 de ABS\r+150 de Vida\r" },

// Mix Senhor da Guerra 12 Infernas Armaduras & Roupões
{ 7, {sinDA1, sinDA2, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
{ 750, 11, 250, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+750 de Defesa\r+11 de ABS\r+250 de Vida\r" },

// Mix dos Deuses 12 Enigmas Armaduras & Roupões
{ 8, {sinDA1, sinDA2, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
{ 1550, 20, 350, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+1550 de Defesa\r+20 de ABS\r+350 de Vida\r" },
//--------------------------------------------------- FIM ARMADURAS --------------------------------------------

//--------------------------------------------------- INICIO ESCUDOS -------------------------------------------
// Mix Inicial 12 Celestos Escudos
{ 9, {sinDS1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0 },
{ SIN_ADD_BLOCK_RATE,SIN_ADD_ABSORB , SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
{ 5, 3, 50, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+5%.0 de Bloqueio\r+3 de Absorção\r+50 de Vida\r" },

// Mix Veterano 12 Mirages Escudos
{ 10, {sinDS1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0 },
{ SIN_ADD_BLOCK_RATE,SIN_ADD_ABSORB , SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
{ 10, 5, 150, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+10%.0 de Bloqueio\r+5 de Absorção\r+150 de Vida\r" },

// Mix Senhor da Guerra 12 Infernas Escudos
{ 11, {sinDS1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0 },
{ SIN_ADD_BLOCK_RATE,SIN_ADD_ABSORB , SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
{ 15, 7, 250, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+15%.0 de Bloqueio\r+7 de Absorção\r+250 de Vida\r" },

// Mix dos Deuses 12 Enigmas Escudos
{ 12, {sinDS1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0 },
{ SIN_ADD_BLOCK_RATE,SIN_ADD_ABSORB , SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
{ 25, 10, 350, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+25%.0 de Bloqueio\r+10 de Absorção\r+350 de Vida\r" },


//--------------------------------------------------- FIM ESCUDOS ----------------------------------------------

//--------------------------------------------------- INICIO ORBS ----------------------------------------------

// Mix Inicial 12 Celestos Orbs
{ 13, {sinOM1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, SIN_ADD_STAMINA, 0, 0, 0, 0 },
{ 155, 3, 50, 100, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+155 de Defesa\r+3 de Absorção\r+50 de Vida\r+100 de Res" },

// Mix Veterano 12 Mirages Orbs
{ 14, {sinOM1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, SIN_ADD_STAMINA, 0, 0, 0, 0 },
{ 255, 5, 150, 200, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+255 de Defesa\r+5 de Absorção\r+150 de Vida\r+200 de Res" },

// Mix Senhor da Guerra 12 Infernas Orbs
{ 15, {sinOM1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, SIN_ADD_STAMINA, 0, 0, 0, 0 },
{ 455, 7, 250, 300, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+455 de Defesa\r+7 de Absorção\r+250 de Vida\r+300 de Res" },

// Mix dos Deuses 12 Enigmas Orbs
{ 16, {sinOM1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, SIN_ADD_STAMINA, 0, 0, 0, 0 },
{ 755, 15, 350, 500, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+755 de Defesa\r+15 de Absorção\r+350 de Vida\r+500 de Res" },


//--------------------------------------------------- FIM ORBS -------------------------------------------------

//--------------------------------------------------- INICIO BOTAS -------------------------------------------------

// Mix Inicial 12 Celestos Botas
{ 17, {sinDB1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_MOVE_SPEED, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0 },
{ 115, 5, 3, 50, 45, 50, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0 },
"+115 de Defesa\r+5 de Absorção\r+3.0 de Velocidade\r+50 de Vida\r+45 de Mana\r+50 de Res.\r" },

// Mix Veterano 12 Mirages Botas
{ 18, {sinDB1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_MOVE_SPEED, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0 },
{ 255, 7, 4, 150, 145, 150, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0 },
"+255 de Defesa\r+7 de Absorção\r+4.0 de Velocidade\r+150 de Vida\r+145 de Mana\r+150 de Res.\r" },

// Mix Senhor da Guerra 12 Infernas Botas
{ 19, {sinDB1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_MOVE_SPEED, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0 },
{ 355, 10, 5, 250, 250, 250, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0 },
"+355 de Defesa\r+10 de Absorção\r+5.0 de Velocidade\r+250 de Vida\r+250 de Mana\r+250 de Res.\r" },

// Mix dos Deuses 12 Enigmas Botas
{ 20, {sinDB1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_MOVE_SPEED, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0 },
{ 555, 15, 7, 350, 350, 350, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0 },
"+555 de Defesa\r+15 de Absorção\r+7.0 de Velocidade\r+350 de Vida\r+350 de Mana\r+350 de Res.\r" },

//--------------------------------------------------- FIM BOTAS -------------------------------------------------


//--------------------------------------------------- INICIO LUVAS -------------------------------------------------

// Mix Inicial 12 Celestos Luvas
{ 21, {sinDG1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0,0 },
{ 130, 4, 50, 50, 50, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0,0 },
"+130 de Defesa\r+4 de Absorção\r+50 de Vida\r+50 de Mana\r+50 de Res.\r" },

// Mix Veterano 12 Mirages Luvas
{ 22, {sinDG1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0,0 },
{ 230, 6, 120, 150, 150, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0,0 },
"+230 de Defesa\r+6 de Absorção\r+120 de Vida\r+150 de Mana\r+150 de Res.\r" },

// Mix Senhor da Guerra 12 Infernas Luvas
{ 23, {sinDG1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0,0 },
{ 330, 8, 250, 250, 250, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0,0 },
"+330 de Defesa\r+8 de Absorção\r+250 de Vida\r+250 de Mana\r+250 de Res.\r" },

// Mix dos Deuses 12 Enigmas Luvas
{ 24, {sinDG1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0,0 },
{ 530, 12, 350, 350, 350, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0,0 },
"+530 de Defesa\r+12 de Absorção\r+350 de Vida\r+350 de Mana\r+350 de Res.\r" },

//--------------------------------------------------- FIM LUVAS -------------------------------------------------

//--------------------------------------------------- INICIO BRACELETES -----------------------------------------


// Mix Inicial 12 Celestos Bracelete
{ 25, {sinOA2, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ATTACK_RATE, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0, 0 },
{ 125, 140, 50, 55, 50, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+125 de Defesa\r+140 de Taxa de Ataque\r+50 de Vida\r+55 de Mana\r+50 de Res.\r" },

// Mix Veterano 12 Mirages Bracelete
{ 26, {sinOA2, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ATTACK_RATE, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0, 0 },
{ 250, 240, 150, 155, 150, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+250 de Defesa\r+240 de Taxa de Ataque\r+150 de Vida\r+155 de Mana\r+150 de Res.\r" },

// Mix Senhor da Guerra 12 Infernas Bracelete
{ 27, {sinOA2, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ATTACK_RATE, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0, 0 },
{ 350, 350, 250, 255, 250, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+350 de Defesa\r+350 de Taxa de Ataque\r+250 de Vida\r+255 de Mana\r+250 de Res.\r" },

// Mix dos Deuses 12 Enigmas Bracelete
{ 28, {sinOA2, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ATTACK_RATE, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0, 0 },
{ 550, 550, 350, 355, 350, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+550 de Defesa\r+550 de Taxa de Ataque\r+350 de Vida\r+355 de Mana\r+350 de Res.\r" },

//--------------------------------------------------- FIM BRACELETES --------------------------------------------


//--------------------------------------------------- INICIO AMULETOS --------------------------------------------

// Mix Inicial 12 Celestos Amuletos
{ 29, {sinOA1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0 },
{ SIN_ADD_LIFEREGEN, SIN_ADD_MANAREGEN, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0,0 },
{ 2.0f, 2.0f, 50, 50, 50, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+2.0 de Reg. de Vida\r+2.0 de Reg. de Mana\r+50 de Vida\r+50 de Mana\r+50 de Res.\r" },

// Mix Veterano 12 Mirages Amuletos
{ 30, {sinOA1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0 },
{ SIN_ADD_LIFEREGEN, SIN_ADD_MANAREGEN, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0,0 },
{ 3.0f, 4.0f, 150, 150, 150, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+3.0 de Reg. de Vida\r+4.0 de Reg. de Mana\r+150 de Vida\r+150 de Mana\r+150 de Res.\r" },

// Mix Senhor da Guerra 12 Infernas Amuletos
{ 31, {sinOA1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0 },
{ SIN_ADD_LIFEREGEN, SIN_ADD_MANAREGEN, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0,0 },
{ 5.0f, 5.0f, 250, 250, 250, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+5.0 de Reg. de Vida\r+5.0 de Reg. de Mana\r+250 de Vida\r+250 de Mana\r+250 de Res.\r" },

// Mix dos Deuses 12 Enigmas Amuletos
{ 32, {sinOA1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0 },
{ SIN_ADD_LIFEREGEN, SIN_ADD_MANAREGEN, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0,0 },
{ 7.0f, 7.0f, 350, 350, 350, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+7.0 de Reg. de Vida\r+7.0 de Reg. de Mana\r+350 de Vida\r+350 de Mana\r+350 de Res.\r" },

//--------------------------------------------------- FIM AMULETOS --------------------------------------------

//--------------------------------------------------- INICIO ANEIS --------------------------------------------

// Mix Inicial 12 Celestos Aneis
{ 33, {sinOR1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0 },
{ SIN_ADD_LIFEREGEN, SIN_ADD_MANAREGEN, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0, 0 },
{ 2.0f, 2.0f, 75, 75, 75, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+2.0 de Reg. de Vida\r+2.0 de Reg. de Mana\r+75 de Vida\r+75 de Mana\r+75 de Res.\r" },

// Mix Veterano 12 Mirages Aneis
{ 34, {sinOR1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0 },
{ SIN_ADD_LIFEREGEN, SIN_ADD_MANAREGEN, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0, 0 },
{ 3.0f, 3.0f, 155, 175, 175, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+3.0 de Reg. de Vida\r+3.0 de Reg. de Mana\r+155 de Vida\r+175 de Mana\r+175 de Res.\r" },

// Mix Senhor da Guerra 12 Infernas Aneis
{ 35, {sinOR1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0 },
{ SIN_ADD_LIFEREGEN, SIN_ADD_MANAREGEN, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0, 0 },
{ 4.0f, 4.0f, 255, 275, 275, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+4.0 de Reg. de Vida\r+4.0 de Reg. de Mana\r+255 de Vida\r+275 de Mana\r+275 de Res.\r" },

// Mix dos Deuses 12 Enigmas Aneis
{ 36, {sinOR1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0 },
{ SIN_ADD_LIFEREGEN, SIN_ADD_MANAREGEN, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0, 0 },
{ 5.0f, 5.0f, 355, 375, 375, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+5.0 de Reg. de Vida\r+5.0 de Reg. de Mana\r+355 de Vida\r+375 de Mana\r+375 de Res.\r" },


//--------------------------------------------------- FIM ANEIS --------------------------------------------