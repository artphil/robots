/*-------------------------------------------
SUMARIO do MENU
- Mascara (Sublinha em branco)
- Iniciar
|- Iniciar(aciona funcionando)
- Configurar
|- TDT (tempo de trabalho)
|- pot m1 (potencia do motor 1)
|- pot m2 (potencia do motor 2)
- Informacoes
|- Bateria
|- RGB (Intencidade refletida por cor(%))
|- LDR (Intencidade refletida atual)
|- Tempo (tempo de fincionam. do arduino em ms)
- Testes
|- Motores auto (Movimentos pre estabelecidos)
|- Motor 1 (acionamento manual)
|- Motor 2 (acionamento manual)
|- RGB (acionamento das luzes)
- Pre Moves
|- Vai Vem
|- Triangulo
|- Quadrado
- Creditos
|- Arthur P Silva
|- Bernardo Sanches
|- Gustavo Campos
|- Matheus Santos
--------------------------------------------*/

/* bibliotecas */

#include <LiquidCrystal.h>
#include <Adafruit_MotorShield.h>
using namespace std;

/* definicoes de auxilio */

// Nome que aparece no menu
#define NOME_ROBO 	" MEGAZORD  v2.0 "

// Direcoes para fins diversos
#define DIREITA 	1
#define ESQUERDA 	2
#define CIMA 		3
#define BAIXO 		4
#define SELECIONA 	5
#define FRENTE 		6
#define TRAS 		7
#define PARA 		8

// Identificacao das cores dos leds
#define RED 		0
#define GREEN	 	1
#define BLUE 		2

// Identificacao dos pinos utilizados
#define LED_RED_PIN 	23
#define LED_GRN_PIN	 	25
#define LED_BLU_PIN 	27
#define LDR_PIN 		8

// Definicoes das constantes de tempo
#define T_MAX_MENU 		180
#define T_TESTE 		300
#define T_GIRO_30 		300
#define T_GIRO_90 		900
#define T_ANDA_30		1000
#define T_ANDA_RE		100
#define T_MAX_ANDAR		10000
#define T_NOME  		500


/* classes */

class entrada
{
	int tam = 50;
	int i;
	int direcao[50];
	int tempo[50];
public:
	void set_tam(int n);
	int  get_tam();
	void reset();
	void set(int n, int m);
	int  get_dir(int n);
	int  get_tmp(int n);
};
void entrada::set_tam(int n)
{ tam = n; }
int entrada::get_tam()
{ return tam; }
void entrada::reset()
{ i = 0; }
void entrada::set(int n, int m)
{
	if (i < 50)
	{
		direcao[i] = n;
		tempo[i] = m;
		i++;
	}
}
int entrada::get_dir(int n)
{
	if (n >= tam) return -1;
	return direcao[n];
}
int entrada::get_tmp(int n)
{
	if (n >= tam) return -1;
	return tempo[n];
}

/* variaveis globais */

// Objeto Display
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Objeto Motor
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *motor_m1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor_m2 = AFMS.getMotor(2);

// Interacao do menu Geral
int 	estado_menu;	// Posicao na pilha do menu
int 	estado_liga;	// Posicao na pilha do menu 11
unsigned long t_menu;	// Contador de tempo do menu
unsigned long t_liga;	// Contador de tempo do menu 11
// Interacao do menu CREDITOS
int letra = 0;
bool desliza = true;
char arthur[]   = {" Arthur Phillip Silva - Hacker Verde - Sistemas de Informacao     "};
char bernardo[] = {" Bernardo Sanches - Lego Builder Azul - Engenharia Aeroespacial     "};
char gustavo[]  = {" Gustavo Duarte - Eletrician Vermelho - Engenharia de Controle e Automacao     "};
char matheus[]  = {" Matheus Santos - Gear Master Preto - Engenharia de Controle e Automacao     "};

// Controle do motor
int 	estado_motor;	// Posicao na pilha de acoes do motor
int 	pot_motor_m1;	// Forca de Trabalho (aplicada no motor)
int 	pot_motor_m2;	// Forca de Trabalho (aplicada no motor)
int 	tdt;			// Tempo de Trabalho (unidade de tempo para cada acao do robo)
int 	mov;			// Movimento a ser efetuado;
bool	ligado;			// Informa se o robo esta funcionando;
// Sequenciass de movimentos
entrada teste;
entrada vai_vem;
entrada triangulo;
entrada quadrado;

// Leds e sensores
int 	led;			// Led aceso;
int 	ldr_valor;		// Intencidade de luz captada;
int 	cores[3];		// Taxa de reflexao de cor (Red Green Blue)
int 	branco[3];		// Taxa de reflexao de cor (Red Green Blue)
int 	preto[3];		// Taxa de reflexao de cor (Red Green Blue)
bool	objeto;			// Informa se tem algo na frente do robo;
float 	carga;			// Tensão real da fonte
unsigned long t_motor;	// Contador de tempo do motor


/* lista de funcoes utilizadas */

// Genericas
// Atualiza as constantes utilizadas
void 	atualiza ();

//  Menu
// Inicializa as vatiaveis
void 	inicia_menu();
// Identifica qual botao foi acionado
int 	verifica_botao ();
// Gerenciador do menu e suas opcoes
void 	menu();

// Motor
// Inicializa as vatiaveis
void 	inicia_motor();
void 	inicia_entradas();
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

// Leds e sensor
// Inicializa as vatiaveis
void 	inicia_sensor();
// Controla o acionamento das luzes
void 	aciona_luz(bool, bool, bool);
// Retorna a media de leituras do sensor
int 	media_sensor();
// Percebe um obstaculo
bool 	ve_objeto();
// Percebe uma cor
void 	ve_cor();
// Calibra o receptor de luz
void balanco_branco();
void balanco_preto();

/* funcoes utilizadas pelo arduino */

void setup()
{
	// put your setup code here, to run once:
	inicia_menu();
	inicia_motor();
	inicia_sensor();
	inicia_entradas();
}

void loop()
{
	// put your main code here, to run repeatedly:
	atualiza();
	menu();
	movimentos();
	// aciona_luz(true,true,true);
}

/* implementação das funcoes */

void inicia_menu() // Inicializa as vatiaveis
{
	// Parametros iniciais
	estado_menu = 0;
	estado_menu = 0;
	t_menu = 0;
	lcd.begin(16, 2);
}
void inicia_motor() // Inicializa as vatiaveis
{
	// Parametros iniciais
	estado_motor = -1;
	pot_motor_m1 = 100;
	pot_motor_m2 = 100;
	tdt = 10000;
	ligado = false;
	AFMS.begin(); // create with the default frequency 1.6KHz
}

void inicia_sensor() // Inicializa as vatiaveis
{
	// Parametros iniciais
	cores[RED]	= 0.0;
	cores[GREEN]= 0.0;
	cores[BLUE]	= 0.0;
	led = 0;
	pinMode(RED,OUTPUT);
	pinMode(GREEN,OUTPUT);
	pinMode(BLUE,OUTPUT);
}

void 	inicia_entradas()
{
	teste.set_tam(8);
	teste.reset();
	teste.set( FRENTE,   T_TESTE );
	teste.set( PARA, 	 T_TESTE );
	teste.set( TRAS,     T_TESTE );
	teste.set( PARA,   	 T_TESTE );
	teste.set( DIREITA,  T_TESTE );
	teste.set( PARA,   	 T_TESTE );
	teste.set( ESQUERDA, T_TESTE );
	teste.set( PARA,   	 T_TESTE );

	vai_vem.set_tam(6);
	vai_vem.reset();
	vai_vem.set( FRENTE,  T_ANDA_30 );
	vai_vem.set( DIREITA, T_GIRO_90 );
	vai_vem.set( DIREITA, T_GIRO_90 );
	vai_vem.set( FRENTE,  T_ANDA_30 );
	vai_vem.set( DIREITA, T_GIRO_90 );
	vai_vem.set( DIREITA, T_GIRO_90 );

	triangulo.set_tam(9);
	triangulo.reset();
	triangulo.set( FRENTE,  T_ANDA_30 );
	triangulo.set( DIREITA, T_GIRO_90 );
	triangulo.set( DIREITA, T_GIRO_30 );
	triangulo.set( FRENTE,  T_ANDA_30 );
	triangulo.set( DIREITA, T_GIRO_90 );
	triangulo.set( DIREITA, T_GIRO_30 );
	triangulo.set( FRENTE,  T_ANDA_30 );
	triangulo.set( DIREITA, T_GIRO_90 );
	triangulo.set( DIREITA, T_GIRO_30 );

	quadrado.set_tam(8);
	quadrado.reset();
	quadrado.set( FRENTE,  T_ANDA_30 );
	triangulo.set( DIREITA, T_GIRO_90 );
	quadrado.set( FRENTE,  T_ANDA_30 );
	triangulo.set( DIREITA, T_GIRO_90 );
	quadrado.set( FRENTE,  T_ANDA_30 );
	triangulo.set( DIREITA, T_GIRO_90 );
	quadrado.set( FRENTE,  T_ANDA_30 );
	triangulo.set( DIREITA, T_GIRO_90 );
}

void atualiza () // Atualiza as constantes utilizadas
{
	int sensorValue = analogRead(A0);
	carga = sensorValue * (5.0 / 1023.0);
	ldr_valor = media_sensor(5);
	if (ligado) objeto = ve_objeto();
}

int verifica_botao () // Identifica qual botao foi acionado
{
	int botao = analogRead (0); //Leitura do valor da porta analógica A0

	if (botao < 50) // DIREITA
	{
		return DIREITA;
	}
	else if (botao < 100) // CIMA
	{
		return CIMA;
	}
	else if (botao < 300) // BAIXO
	{
		return BAIXO;
	}
	else if (botao < 500) // ESQUERDA
	{
		return ESQUERDA;
	}
	else if (botao < 700) // SELECIONA
	{
		return SELECIONA;
	}
	else
	{
		return 0;
	}
}

void menu() // Gerenciador do menu e suas opcoes
{
	int botao;
	if ((millis() - t_menu) > T_MAX_MENU)
	{
		t_menu = millis();
		botao = verifica_botao();
	}

	if (estado_menu < 10)	// Nivel 1
	{
		lcd.setCursor(0,0);
		lcd.print (NOME_ROBO);
		lcd.setCursor(0,1);

		if (estado_menu == 0)	// Mascara
		{
			lcd.print ("                ");

			if (botao == CIMA)
			{
				estado_menu = 9;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 1;
			}
		}
		else if (estado_menu == 1)
		{
			lcd.print ("Iniciar         ");

			if (botao == CIMA)
			{
				estado_menu = 0;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 2;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 11;
				t_liga = millis();
				estado_liga = 0;
			}
		}
		else if (estado_menu == 2)
		{
			lcd.print ("Configurar      ");

			if (botao == CIMA)
			{
				estado_menu = 1;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 3;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 21;
			}
		}
		else if (estado_menu == 3)
		{
			lcd.print ("Informacoes     ");

			if (botao == CIMA)
			{
				estado_menu = 2;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 4;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 31;
			}
		}
		else if (estado_menu == 4)
		{
			lcd.print ("Testes          ");

			if (botao == CIMA)
			{
				estado_menu = 3;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 9;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 41;
			}
		}
		else if (estado_menu == 5)
		{
			lcd.print ("Pre moves       ");

			if (botao == CIMA)
			{
				estado_menu = 4;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 9;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 51;
			}
		}
		else if (estado_menu == 9)
		{
			lcd.print ("Creditos        ");

			if (botao == CIMA)
			{
				estado_menu = 5;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 0;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 91;
				t_liga = millis();
			}
		}
	}
	else if (estado_menu < 20) // Iniciar - nivel 2
	{
		if (estado_menu == 11)
		{
			lcd.setCursor(0,0);
			lcd.print (NOME_ROBO);
			lcd.setCursor(0,1);

			if ((millis() - t_liga) > 1000)
			{
				t_liga = millis();
				estado_liga += 1;
			}

			if (estado_liga < 4)
			{
				lcd.print ("Iniciando em    ");
				lcd.setCursor(13,1);
				lcd.print (3 - estado_liga);
			}
			else if (estado_liga < 5)
			{
				if (!ligado)
				{
					lcd.print ("Morfando        ");
					ligado 	= true;
					mov 	= 1;
					estado_motor = 0;
					t_motor = millis();
					estado_liga += 1;
				}
				else
				{
					estado_menu = 0;
				}
			}
			else if (estado_liga < 10)
			{
				lcd.setCursor((estado_liga+4),1);
				lcd.print (".");
			}
			else estado_menu = 0;
		}
	}
	else if (estado_menu < 30) // Configurar - nivel 2
	{
		lcd.setCursor(0,0);
		lcd.print ("   CONFIGURAR   ");
		lcd.setCursor(0,1);

		if (estado_menu == 21)
		{
			lcd.print ("  TDT  -       +");
			lcd.setCursor(10,1);
			lcd.print (tdt);

			if (botao == CIMA)
			{
				estado_menu = 29;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 22;
			}
			else if (botao == DIREITA)
			{
				// if (tdt < 10000)
				tdt += 10;
			}
			else if (botao == ESQUERDA)
			{
				if (tdt > 0) tdt -= 10;
			}
		}
		else if (estado_menu == 22)
		{
			lcd.print ("pot m1 -       +");
			lcd.setCursor(10,1);
			lcd.print (pot_motor_m1);

			if (botao == CIMA)
			{
				estado_menu = 21;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 23;
			}
			else if (botao == DIREITA)
			{
				if (pot_motor_m1 < 255) pot_motor_m1++;
			}
			else if (botao == ESQUERDA)
			{
				if (pot_motor_m1 > 0) pot_motor_m1--;
			}
		}
		else if (estado_menu == 23)
		{
			lcd.print ("pot m2 -       +");
			lcd.setCursor(10,1);
			lcd.print (pot_motor_m2);

			if (botao == CIMA)
			{
				estado_menu = 22;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 24;
			}
			else if (botao == DIREITA)
			{
				if (pot_motor_m2 < 255) pot_motor_m2++;
			}
			else if (botao == ESQUERDA)
			{
				if (pot_motor_m2 > 0) pot_motor_m2--;
			}
		}
		else if (estado_menu == 24)
		{
			lcd.print ("Balanco branco  ");

			if (botao == CIMA)
			{
				estado_menu = 23;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 25;
			}
			else if (botao == SELECIONA)
			{
				balanco_branco();
				estado_menu = 2;
			}
		}
		else if (estado_menu == 25)
		{
			lcd.print ("Balanco preto   ");

			if (botao == CIMA)
			{
				estado_menu = 24;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 29;
			}
			else if (botao == SELECIONA)
			{
				balanco_preto();
				estado_menu = 2;
			}
		}
		else if (estado_menu == 29)
		{
			lcd.print ("Voltar          ");

			if (botao == CIMA)
			{
				estado_menu = 25;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 21;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 2;
			}
		}
	}
	else if (estado_menu < 40) // Informacoes - nivel 2
	{
		lcd.setCursor(0,0);
		lcd.print ("  INFORMACOES  ");
		lcd.setCursor(0,1);
		if (estado_menu == 31)
		{
			lcd.print ("Bateria        v");
			lcd.setCursor(11,1);
			lcd.print (carga);

			if (botao == CIMA)
			{
				estado_menu = 39;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 32;
			}

		}
		else if (estado_menu == 32)
		{
			lcd.print ("R    G    B     ");
			lcd.setCursor(2,1);
			lcd.print (cores[RED]);
			lcd.setCursor(7,1);
			lcd.print (cores[GREEN]);
			lcd.setCursor(12,1);
			lcd.print (cores[BLUE]);

			if (botao == CIMA)
			{
				estado_menu = 31;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 33;
			}
			else if (botao == SELECIONA)
			{
				ve_cor();
			}

		}

		if (estado_menu == 33)
		{
			lcd.print ("LDR             ");
			lcd.setCursor(5,1);
			lcd.print (ldr_valor);

			if (botao == CIMA)
			{
				estado_menu = 32;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 34;
			}

		}
		if (estado_menu == 34)
		{
			lcd.print ("Objeto          ");
			lcd.setCursor(7,1);
			lcd.print (objeto);

			if (botao == CIMA)
			{
				estado_menu = 33;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 35;
			}

		}
		else if (estado_menu == 35)
		{
			lcd.print ("Tempo         s");
			lcd.setCursor(7,1);
			lcd.print (millis());

			if (botao == CIMA)
			{
				estado_menu = 34;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 39;
			}

		}
		else if (estado_menu == 39)
		{
			lcd.print ("Voltar          ");

			if (botao == CIMA)
			{
				estado_menu = 34;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 31;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 3;
			}
		}
	}
	else if (estado_menu < 50) // Testes - nivel 2
	{
		lcd.setCursor(0,0);
		lcd.print ("     TESTES     ");
		lcd.setCursor(0,1);

		if (estado_menu == 41)
		{
			lcd.print ("Motores auto     ");

			if (botao == CIMA)
			{
				estado_menu = 49;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 42;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 411;
				t_liga = millis();
				estado_liga = 0;
			}
		}
		else if (estado_menu == 42)
		{
			lcd.print ("F   Motor  1   T");
			lcd.setCursor(10,1);

			if (botao == CIMA)
			{
				estado_menu = 41;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 43;
			}
			else if (botao == DIREITA)
			{
				aciona_motor(1,0);
				delay(T_MAX_MENU);
			}
			else if (botao == ESQUERDA)
			{
				aciona_motor(-1,0);
				delay(T_MAX_MENU);
			}
			else
			{
				aciona_motor(0,0);
			}
		}
		else if (estado_menu == 43)
		{
			lcd.print ("F   Motor  2   T");
			lcd.setCursor(10,1);

			if (botao == CIMA)
			{
				estado_menu = 42;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 44;
			}
			else if (botao == DIREITA)
			{
				aciona_motor(0,1);
				delay(T_MAX_MENU);
			}
			else if (botao == ESQUERDA)
			{
				aciona_motor(0,-1);
				delay(T_MAX_MENU);
			}
			else
			{
				aciona_motor(0,0);
			}
		}
		else if (estado_menu == 44)
		{
			if (led == 0)
			{
				lcd.print ("<   R  G  B    >");
				aciona_luz(false, false, false);
			}
			else if (led == 1)
			{
				lcd.print ("<  *R* G  B    >");
				aciona_luz(true, false, false);
			}
			else if (led == 2)
			{
				lcd.print ("<   R *G* B    >");
				aciona_luz(false, true, false);
			}
			else if (led == 3)
			{
				lcd.print ("<   R  G *B*   >");
				aciona_luz(false, false, true);
			}
			else if (led == 4)
			{
				lcd.print ("<  *R**G**B*   >");
				aciona_luz(true, true, true);
			}

			if (botao == CIMA)
			{
				estado_menu = 43;
				aciona_luz(false, false, false);
			}
			else if (botao == BAIXO)
			{
				estado_menu = 49;
				aciona_luz(false, false, false);
			}
			else if (botao == DIREITA)
			{
				if (led < 4) led++;
				else led = 0;
			}
			else if (botao == ESQUERDA)
			{
				if (led > 0) led--;
				else led = 4;
			}
		}
		else if (estado_menu == 49)
		{
			lcd.print ("Voltar          ");

			if (botao == CIMA)
			{
				estado_menu = 44;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 41;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 4;
			}
		}
	}
	else if (estado_menu < 60) // Pre moves - nivel 2
	{
		lcd.setCursor(0,0);
		lcd.print ("   PRE  MOVES   ");
		lcd.setCursor(0,1);

		if (estado_menu == 51)
		{
			lcd.print ("Vai Vem         ");

			if (botao == CIMA)
			{
				estado_menu = 59;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 52;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 511;
				t_liga = millis();
				estado_liga = 0;
			}
		}
		else if (estado_menu == 52)
		{
			lcd.print ("Triangulo       ");

			if (botao == CIMA)
			{
				estado_menu = 51;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 53;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 521;
				t_liga = millis();
				estado_liga = 0;
			}
		}
		else if (estado_menu == 53)
		{
			lcd.print ("Quadrado         ");

			if (botao == CIMA)
			{
				estado_menu = 52;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 59;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 531;
				t_liga = millis();
				estado_liga = 0;
			}
		}
		else if (estado_menu == 59)
		{
			lcd.print ("Voltar          ");

			if (botao == CIMA)
			{
				estado_menu = 53;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 51;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 5;
			}
		}
	}
	else if (estado_menu < 100) // Creditos - nivel 2
	{
		lcd.setCursor(0,0);
		lcd.print ("    CREDITOS    ");
		lcd.setCursor(0,1);

		if (estado_menu == 91)
		{
			int tam = strlen(arthur);
			if (desliza)
			{
				if ((millis() - t_liga) > T_NOME)
				{
					t_liga = millis();
					letra += 1;
					if (letra >= tam) letra = 0;
				}
			}
			for (int l=0; l<16; l++)
			{
				lcd.setCursor(l,1);
				lcd.print (arthur[(letra+l)%tam]);
			}

			if (botao == CIMA)
			{
				estado_menu = 99;
				letra = 0;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 92;
				letra = 0;
			}
			else if (botao == SELECIONA)
			{
				desliza = !desliza;
			}
		}
		else if (estado_menu == 92)
		{
			int tam = strlen(bernardo);
			if (desliza)
			{
				if ((millis() - t_liga) > T_NOME)
				{
					t_liga = millis();
					letra += 1;
					if (letra >= tam) letra = 0;
				}
			}
			for (int l=0; l<16; l++)
			{
				lcd.setCursor(l,1);
				lcd.print (bernardo[(letra+l)%tam]);
			}

			if (botao == CIMA)
			{
				estado_menu = 91;
				letra = 0;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 93;
				letra = 0;
			}
			else if (botao == SELECIONA)
			{
				desliza = !desliza;
			}
		}
		else if (estado_menu == 93)
		{
			int tam = strlen(gustavo);
			if (desliza)
			{
				if ((millis() - t_liga) > T_NOME)
				{
					t_liga = millis();
					letra += 1;
					if (letra >= tam) letra = 0;
				}
			}
			for (int l=0; l<16; l++)
			{
				lcd.setCursor(l,1);
				lcd.print (gustavo[(letra+l)%tam]);
			}

			if (botao == CIMA)
			{
				estado_menu = 92;
				letra = 0;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 94;
				letra = 0;
			}
			else if (botao == SELECIONA)
			{
				desliza = !desliza;
			}
		}
		else if (estado_menu == 94)
		{
			int tam = strlen(matheus);
			if (desliza)
			{
				if ((millis() - t_liga) > T_NOME)
				{
					t_liga = millis();
					letra += 1;
					if (letra >= tam) letra = 0;
				}
			}
			for (int l=0; l<16; l++)
			{
				lcd.setCursor(l,1);
				lcd.print (matheus[(letra+l)%tam]);
			}

			if (botao == CIMA)
			{
				estado_menu = 93;
				letra = 0;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 99;
				letra = 0;
			}
			else if (botao == SELECIONA)
			{
				desliza = !desliza;
			}
		}
		else if (estado_menu == 99)
		{
			lcd.print ("Voltar          ");

			if (botao == CIMA)
			{
				estado_menu = 94;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 91;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 9;
			}
		}
	}
	else if (estado_menu < 500) // Teste - Motor - nivel 3
	{
		if (estado_menu == 411)
		{
			lcd.setCursor(0,1);
			lcd.print ("Iniciando em    ");

			if ((millis() - t_liga) > 1000)
			{
				t_liga = millis();
				estado_liga += 1;
			}

			if (estado_liga < 4)
			{
				lcd.setCursor(13,1);
				lcd.print (3 - estado_liga);
			}
			else if (estado_liga < 5)
			{
				if (!ligado)
				{
					ligado 	= true;
					mov 	= 2;
					estado_motor = 0;
					t_motor = millis();
					estado_liga += 1;
				}
				else
				{
					estado_menu = 0;
				}
			}
			else estado_menu = 0;
		}


	}
	else if (estado_menu < 600) // Pre move - nivel 3
	{
		if (estado_menu == 511) // Vai-vem
		{
			lcd.setCursor(0,1);
			lcd.print ("Iniciando em    ");

			if ((millis() - t_liga) > 1000)
			{
				t_liga = millis();
				estado_liga += 1;
			}

			if (estado_liga < 4)
			{
				lcd.setCursor(13,1);
				lcd.print (3 - estado_liga);
			}
			else if (estado_liga < 5)
			{
				if (!ligado)
				{
					ligado 	= true;
					mov 	= 3;
					estado_motor = 0;
					t_motor = millis();
					estado_liga += 1;
				}
				else
				{
					estado_menu = 0;
				}
			}
			else estado_menu = 0;
		}
		if (estado_menu == 521) // Triangulo
		{
			lcd.setCursor(0,1);
			lcd.print ("Iniciando em    ");

			if ((millis() - t_liga) > 1000)
			{
				t_liga = millis();
				estado_liga += 1;
			}

			if (estado_liga < 4)
			{
				lcd.setCursor(13,1);
				lcd.print (3 - estado_liga);
			}
			else if (estado_liga < 5)
			{
				if (!ligado)
				{
					ligado 	= true;
					mov 	= 4;
					estado_motor = 0;
					t_motor = millis();
					estado_liga += 1;
				}
				else
				{
					estado_menu = 0;
				}
			}
			else estado_menu = 0;
		}
		if (estado_menu == 531) // Quadrado
		{
			lcd.setCursor(0,1);
			lcd.print ("Iniciando em    ");

			if ((millis() - t_liga) > 1000)
			{
				t_liga = millis();
				estado_liga += 1;
			}

			if (estado_liga < 4)
			{
				lcd.setCursor(13,1);
				lcd.print (3 - estado_liga);
			}
			else if (estado_liga < 5)
			{
				if (!ligado)
				{
					ligado 	= true;
					mov 	= 5;
					estado_motor = 0;
					t_motor = millis();
					estado_liga += 1;
				}
				else
				{
					estado_menu = 0;
				}
			}
			else estado_menu = 0;
		}
	}
}

void aciona_motor (int m1, int m2)
{
	if (m1 > 0)
	{
		motor_m1->setSpeed(pot_motor_m1);
		motor_m1->run(FORWARD);
	}
	else if (m1 < 0)
	{
		motor_m1->setSpeed(pot_motor_m1);
		motor_m1->run(BACKWARD);
	}
	else
	{
		motor_m1->run(RELEASE);
	}

	if (m2 > 0)
	{
		motor_m2->setSpeed(pot_motor_m2);
		motor_m2->run(FORWARD);
	}
	else if (m2 < 0)
	{
		motor_m2->setSpeed(pot_motor_m2);
		motor_m2->run(BACKWARD);
	}
	else
	{
		motor_m2->run(RELEASE);
	}
}

int anda(int direcao, int tmp_acao)
{
	if (ligado && (millis()-t_motor < tmp_acao))
	{
		if (direcao == FRENTE)
		{
			aciona_motor(1,1);
		}
		else if (direcao == TRAS)
		{
			aciona_motor(-1,-1);
		}
		else if (direcao == DIREITA)
		{
			aciona_motor(1,-1);
		}
		else if (direcao == ESQUERDA)
		{
			aciona_motor(-1,1);
		}
		else
		{
			aciona_motor(0,0);
		}
		return 0;
	}
	else
	{
		aciona_motor(0,0);
		return 1;
	}
}

int move(entrada a)
{
	if (estado_motor < 0)
	{
		ligado = false;
		aciona_motor(0,0);
		return 1;
	}
	if (estado_motor >= a.get_tam())
	{
		ligado = false;
		estado_motor = -1;
		aciona_motor(0,0);
		return 1;
	}

	estado_motor += anda(a.get_dir(estado_motor), a.get_tmp(estado_motor));


	return 0;
}

int move_auto()
{
	int dir = FRENTE;
	int tmp = T_MAX_ANDAR;
	int estado = 0;

	anda(dir,tmp);

	if (estado == 0)
	{
		dir = FRENTE;
		tmp = T_MAX_ANDAR;
	}
	return 0;
}

int movimentos ()
{
	if (!ligado || mov == 0) return 0;
	if (mov == 1)
	{
		if (move_auto() == 1) 		ligado = false;
	}
	else if (mov == 2)
	{
		if (move(vai_vem) == 1) 	ligado = false;
	}
	else if (mov == 3)
	{
		if (move(vai_vem) == 1) 	ligado = false;
	}
	else if (mov == 4)
	{
		if (move(triangulo) == 1) 	ligado = false;
	}
	else if (mov == 5)
	{
		if (move(quadrado) == 1) 	ligado = false;
	}

}


void aciona_luz(bool r, bool g, bool b)
{
	if (r) digitalWrite(LED_RED_PIN, HIGH);
	else digitalWrite(LED_RED_PIN, LOW);

	if (g) digitalWrite(LED_GRN_PIN, HIGH);
	else digitalWrite(LED_GRN_PIN, LOW);

	if (b) digitalWrite(LED_BLU_PIN, HIGH);
	else digitalWrite(LED_BLU_PIN, LOW);
}

int media_sensor(int n){
	int total = 0;
	for (int i = 0;i < n;i++)
	{
		total += analogRead(LDR_PIN);
	}
	return total/n;
}

bool ve_objeto()
{
	int sinal;
	aciona_luz(true, true, true);
	delay(200);
	sinal = media_sensor(5);
	aciona_luz(false, false, false);

	if (sinal > 100) return false;
	else return true;
}

void ve_cor()
{
	int r, g, b, cinza;

	aciona_luz(true, false, false);
	delay(200);
	r = media_sensor(5);
	aciona_luz(false, false, false);

	aciona_luz(false, true, false);
	delay(200);
	g = media_sensor(5);
	aciona_luz(false, false, false);

	aciona_luz(false, false, true);
	delay(200);
	b = media_sensor(5);
	aciona_luz(false, false, false);

	cinza = branco[RED]-preto[RED];
	cores[RED] 		= (r - preto[RED])/(cinza)*255;

	cinza = branco[GREEN]-preto[GREEN];
	cores[GREEN] 	= (r - preto[GREEN])/(cinza)*255;

	cinza = branco[BLUE]-preto[BLUE];
	cores[BLUE] 	= (r - preto[BLUE])/(cinza)*255;
}

void balanco_branco()
{
	aciona_luz(true, false, false);
	delay(100);
	branco[RED] = media_sensor(5);
	aciona_luz(false, false, false);
	delay(100);

	aciona_luz(false, true, false);
	delay(100);
	branco[GREEN] = media_sensor(5);
	aciona_luz(false, false, false);
	delay(100);

	aciona_luz(false, false, true);
	delay(100);
	branco[BLUE] = media_sensor(5);
	aciona_luz(false, false, false);
	delay(100);
}

void balanco_preto()
{
	aciona_luz(true, false, false);
	delay(100);
	preto[RED] = media_sensor(5);
	aciona_luz(false, false, false);
	delay(100);

	aciona_luz(false, true, false);
	delay(100);
	preto[GREEN] = media_sensor(5);
	aciona_luz(false, false, false);
	delay(100);

	aciona_luz(false, false, true);
	delay(100);
	preto[BLUE] = media_sensor(5);
	aciona_luz(false, false, false);
	delay(100);
}
