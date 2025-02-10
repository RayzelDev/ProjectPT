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



// Mix 9 cel Armaduras
//{ 4, {sinDA1, sinDA2, 0, 0, 0, 0, 0, 0},
//{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9 },
//{ SIN_ADD_DEFENCE, SIN_ADD_BLOCK_RATE, SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
//{ 250, 7, 100, 0, 0, 0, 0, 0 },
//{ SIN_ADD_NUM, SIN_ADD_PERCENT, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
//"+250 de Defesa\r+7% de Bloqueio\r+100 de Vida\r" },

// Mix 5 cel Escudos
{ 5, {sinDS1, 0, 0, 0, 0, 0, 0, 0},
{ 3, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0 },
{ SIN_ADD_ABSORB, SIN_ADD_BLOCK_RATE, SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
{ 1.0f, 2, 50, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_PERCENT, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+1.0 de Absorção\r+2% de Bloqueio\r+50 de Vida\r" },

// Mix 9 cel Escudos
{ 6, {sinDS1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9 },
{ SIN_ADD_ABSORB, SIN_ADD_BLOCK_RATE, SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
{ 3.0f, 5, 150, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_PERCENT, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+3.0 de Absorção\r+5% de Bloqueio\r+150 de Vida\r" },

// Mix 5 cel Orbs
{ 7, {sinOM1, 0, 0, 0, 0, 0, 0, 0},
{ 3, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0 },
{ SIN_ADD_ABSORB, SIN_ADD_BLOCK_RATE, SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
{ 1.0f, 2, 50, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_PERCENT, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+1.0 de Absorção\r+2% de Bloqueio\r+50 de Vida\r" },

// Mix 9 cel Orbs
{ 8, {sinOM1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9 },
{ SIN_ADD_ABSORB, SIN_ADD_BLOCK_RATE, SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
{ 3.0f, 5, 150, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_PERCENT, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+3.0 de Absorção\r+5% de Bloqueio\r+150 de Vida\r" },

// Mix 5 cel Botas
{ 9, {sinDB1, 0, 0, 0, 0, 0, 0, 0},
{ 3, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_MOVE_SPEED, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0 },
{ 80, 1.5f, 2.0f, 20, 20, 20, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0 },
"+80 de Defesa\r+1.5 de Absorção\r+2.0 de Velocidade\r+20 de Vida\r+20 de Mana\r+20 de Res.\r" },

// Mix 9 cel Botas
{ 10, {sinDB1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_MOVE_SPEED, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0 },
{ 140, 4.0f, 3.0f, 50, 50, 50, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0 },
"+140 de Defesa\r+4.0 de Absorção\r+3.0 de Velocidade\r+20 de Vida\r+20 de Mana\r+20 de Res.\r" },

// Mix 5 cel Luvas
{ 11, {sinDG1, 0, 0, 0, 0, 0, 0, 0},
{ 3, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0,0 },
{ 80, 1.5f, 20, 20, 20, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0,0 },
"+80 de Defesa\r+1.5 de Absorção\r+20 de Vida\r+20 de Mana\r+20 de Res.\r" },

// Mix 9 cel Luvas
{ 12, {sinDG1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0,0 },
{ 140, 2.5f, 50, 50, 50, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0,0 },
"+140 de Defesa\r+2.5 de Absorção\r+50 de Vida\r+50 de Mana\r+50 de Res.\r" },

// Mix 5 cel Bracelete
{ 13, {sinOA2, 0, 0, 0, 0, 0, 0, 0},
{ 3, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ATTACK_RATE, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0, 0 },
{ 80, 80, 20, 10, 20, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+80 de Defesa\r+80 de Taxa de Ataque\r+20 de Vida\r+20 de Mana\r+20 de Res.\r" },

// Mix 9 cel Bracelete
{ 14, {sinOA2, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9 },
{ SIN_ADD_DEFENCE, SIN_ADD_ATTACK_RATE, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0, 0 },
{ 140, 140, 50, 50, 50, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+140 de Defesa\r+140 de Taxa de Ataque\r+50 de Vida\r+50 de Mana\r+50 de Res.\r" },

// Mix 5 cel Amuletos
{ 15, {sinOA1, 0, 0, 0, 0, 0, 0, 0},
{ 3, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0 },
{ SIN_ADD_ATTACK_RATE, SIN_ADD_LIFEREGEN, SIN_ADD_MANAREGEN, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0 },
{ 80, 0.4f, 0.8f, 20, 20, 20, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0 },
"+80 de Taxa de Ataque\r+0.4 de Reg. de Vida\r+0.8 de Reg. de Mana\r+20 de Vida\r+20 de Mana\r+20 de Res.\r" },

// Mix 9 cel Amuletos
{ 16, {sinOA1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9 },
{ SIN_ADD_ATTACK_RATE, SIN_ADD_LIFEREGEN, SIN_ADD_MANAREGEN, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0 },
{ 150, 1.2f, 1.4f, 75, 75, 75, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0 },
"+150 de Taxa de Ataque\r+1.2 de Reg. de Vida\r+1.5 de Reg. de Mana\r+75 de Vida\r+75 de Mana\r+75 de Res.\r" },

// Mix 5 cel Aneis
{ 17, {sinOR1, 0, 0, 0, 0, 0, 0, 0},
{ 3, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0 },
{ SIN_ADD_ATTACK_RATE, SIN_ADD_LIFEREGEN, SIN_ADD_MANAREGEN, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0 },
{ 80, 0.4f, 0.8f, 20, 20, 20, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0 },
"+80 de Taxa de Ataque\r+0.4 de Reg. de Vida\r+0.8 de Reg. de Mana\r+20 de Vida\r+20 de Mana\r+20 de Res.\r" },

// Mix 5 cel Aneis
{ 18, {sinOR1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9 },
{ SIN_ADD_ATTACK_RATE, SIN_ADD_LIFEREGEN, SIN_ADD_MANAREGEN, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0 },
{ 150, 1.2f, 1.4f, 75, 75, 75, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0 },
"+150 de Taxa de Ataque\r+1.2 de Reg. de Vida\r+1.5 de Reg. de Mana\r+75 de Vida\r+75 de Mana\r+75 de Res.\r" },