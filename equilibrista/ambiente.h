/* Configuracao de ambiente */

#ifndef _AMBIENTE_
#define _AMBIENTE_

#include <LiquidCrystal.h>
#include <Adafruit_MotorShield.h>
#include <RotaryEncoder.h>
#include <EEPROM.h>

#include "classes.h"
#include "menu.h"
#include "movimento.h"
#include "visao.h"
#include "dados.h"
#include "odometria.h"

using namespace std;

#endif

/* definicoes de auxilio */

// Nome que aparece no menu
#define NOME_ROBO 	" MEGAZORD  v2.1 "

// Direcoes para fins diversos
#define ESQUERDA 	1
#define DIREITA 	2
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
#define MOTOR_E_PIN 	1
#define MOTOR_D_PIN 	3
#define MOTOR_F_PIN 	4

#define LED_RED_PIN 	23
#define LED_GRN_PIN	 	25
#define LED_BLU_PIN 	27

#define OTICO_E_PIN		41
#define OTICO_D_PIN		45

#define LDR_PIN 		A12

#define LDR_S_PIN 		A14

#define LDR_DIF_PIN 	A15

#define LDR_DIR_PIN 	A10
#define LDR_ESQ_PIN 	A8

#define ENCDR_E1_PIN	A8
#define ENCDR_E2_PIN	A9

#define ENCDR_D1_PIN	A10
#define ENCDR_D2_PIN	A11

// Definicoes das constantes de tempo
#define T_MAX_MENU 		180
#define T_MAX_ANDAR		60000

// funcoes
#define i_rand(a,b)		a+(rand()%(b-a+1))

// typedef unsigned long unsigned long;
