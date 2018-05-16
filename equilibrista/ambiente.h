/* Configuracao de ambiente */

#ifndef _AMBIENTE_
#define _AMBIENTE_

#include <LiquidCrystal.h>
#include <Adafruit_MotorShield.h>
#include <EEPROM.h>

#include "classes.h"
#include "menu.h"
#include "movimento.h"
#include "visao.h"
#include "variaveis.h"

using namespace std;

#endif

/* definicoes de auxilio */

// Nome que aparece no menu
#define NOME_ROBO 	" MEGAZORD  v2.1 "

// Direcoes para fins diversos
#define DIREITA 	1
#define ESQUERDA 	2
#define CIMA 		3
#define BAIXO 		4
#define SELECIONA 	5
#define FRENTE 		6
#define TRAS 		7
#define PARA 		8

// Identificacao das cores
#define RED 		0
#define GREEN	 	1
#define BLUE 		2
#define YELLOW 		3
#define WHITE 		4
#define BLACK 		5

// Identificacao dos pinos utilizados
#define LED_RED_PIN 	23
#define LED_GRN_PIN	 	25
#define LED_BLU_PIN 	27
#define LDR_PIN 		8
#define LDR_E_PIN 		9
#define LDR_D_PIN 		10

// Definicoes das constantes de tempo
#define T_MAX_MENU 		180
#define T_MAX_ANDAR		10000
#define T_NOME  		400
