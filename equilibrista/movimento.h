#include "ambiente.h"
// Motor
// Objeto Motor
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Variaveis
Adafruit_DCMotor *motor_E = AFMS.getMotor(MOTOR_E_PIN);
Adafruit_DCMotor *motor_D = AFMS.getMotor(MOTOR_D_PIN);

int 	estado_motor;	// Posicao na pilha de acoes do motor
int 	estado_move;	// Posicao na pilha de acoes do motor
int 	pot_motor_D;	// Forca de Trabalho (aplicada no motor)
int 	pot_motor_E;	// Forca de Trabalho (aplicada no motor)
int 	mov;			// Movimento a ser efetuado;
bool	ligado;			// Informa se o robo esta funcionando;
int 	t_giro_45;
int 	t_giro_90;
int 	t_anda;
int 	anda_cm;
int 	anda_fat;
int 	t_anda_raiz;
int 	t_teste;
int 	t_anda_re;

conta_tempo t_motor(1000);
conta_tempo t_partida(T_MAX_ANDAR);

entrada seq_mov;

// Funcoes
void 	inicia_motor();
// Contola o acionamento dos motores
void 	aciona_motor (int, int);
// move o robo em linha reta
int 	anda(int, int);
// gira o robo em uma direcao
int 	gira(int, int);
// Controla os movimentos pre-estabelecidos
int 	move(entrada);
// Controla movimentos automaticos
int 	move_auto();
// Seleciona movimento a ser efetuado
int 	movimentos ();
// Movimentos preprogramados
void 	e_teste();
void 	e_vai_vem();
void 	e_triangulo();
void 	e_quadrado();
void 	e_giro90();
