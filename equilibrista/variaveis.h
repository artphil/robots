#include "ambiente.h"

/* variaveis globais */

// Objeto Display
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Objeto Motor
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Encoder encoder_D(ENCDR_D1_PIN, ENCDR_D2_PIN);
Encoder encoder_E(ENCDR_E1_PIN, ENCDR_E2_PIN);

Adafruit_DCMotor *motor_D = AFMS.getMotor(1);
Adafruit_DCMotor *motor_E = AFMS.getMotor(3);

// Interacao do menu Geral
int 	botao;			// Botao pressionado no momento
int 	tst;			// intica qual teste esta sendo realizado
int 	estado_menu;	// Posicao na pilha do menu
int 	estado_antigo;	// Posicao anterior na pilha do menu
int 	estado_liga;	// Posicao na pilha do menu 11
conta_tempo t_menu(180); 	// Contador de tempo do menu
conta_tempo t_liga(1000);	// Contador de tempo do menu 11
// Interacao do menu CREDITOS
int letra = 0;
bool desliza = true;
conta_tempo t_nome(400);	// Contador de tempo do menu 11

// Controle do motor
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

// Sequencias de movimentos
entrada seq_mov;
entrada teste;
entrada vai_vem;
entrada triangulo;
entrada quadrado;
entrada giro45;
entrada giro90;

// Leds e sensores
int 	led;			// Led aceso
bool 	led_atual[3];	// Verifica se a mudou a os leds
int 	cor;			// Cor encontrada
int 	ldr_valor;		// Intencidade de luz captada
int 	ldr_direita;	// Intencidade de luz captada no ldr direito
int 	ldr_esquerda;	// Intencidade de luz captada no ldr esquerdo
int 	otico_direita;	// Intencidade de luz captada no otico direito
int 	otico_esquerda;	// Intencidade de luz captada no otico esquerdo
int 	otico_direcao;	// Direcao da maior intencidade de luz
int 	ldr_direcao;	// Direcao da maior intencidade de luz
int 	ldr_dif_max;	// Direcao da maior intencidade de luz
int 	ldr_limiar;		// Valor limite para ver objeto
int 	ldr_cor[3];		// Taxa de reflexao de cor (Red Green Blue)
int 	ldr_branco[3];	// Taxa de reflexao de cor (Red Green Blue)
int 	ldr_preto[3];	// Taxa de reflexao de cor (Red Green Blue)
bool	objeto;			// Informa se tem algo na frente do robo
float 	carga;			// Tensão real da fonte
unsigned long t_motor;	// Contador de tempo do motor
String	nome_cor[] =
{
	"Vermelho",
	"Verde",
	"Azul",
	"Amarelo",
	"Branco",
	"Preto"
};						// Nome das cores
int dir_atual=DIREITA;
int dir_ultima;

// odometria
bool mov_giro;
bool mov_anterior;
int encdr_e_valor;
int encdr_d_valor;
int encdr_e_ultimo;
int encdr_d_ultimo;
int esperado;
int ultimo_erro;
float KP;
float KD;
conta_tempo t_pd(50);
// Atualiza as constantes utilizadas
void 	atualiza ();
// Inicializa as vatiaveis
void 	inicia_menu();
void 	inicia_motor();
void 	inicia_entradas();
void 	inicia_sensor();

// Manipula a memoria EEPROM
/* Variveis:
pot_motor_m*
ldr_limiar
ldr_branco[]
ldr_preto[]
t_anda
t_giro_90
*/
void 	grava_EEPROM ();
void 	le_EEPROM ();

// Entradas
void 	e_teste();
void 	e_vai_vem();
void 	e_triangulo();
void 	e_quadrado();
void 	e_giro90();
