/*-------------------------------------------
SUMARIO do MENU
- Mascara
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

#define NOME_ROBO 	" MEGAZORD  v2.0 "

#define DIREITA 	1
#define ESQUERDA 	2
#define CIMA 		3
#define BAIXO 		4
#define SELECIONA 	5
#define FRENTE 		6
#define TRAS 		7

#define RED 		0
#define GREEN	 	1
#define BLUE 		2

#define LED_RED_PIN 	23
#define LED_GRN_PIN	 	25
#define LED_BLU_PIN 	27
#define LDR_PIN 		8

#define T_MAX_MENU 		180
#define T_GIRO_30 		300
#define T_GIRO_90 		900
#define T_ANDA_30		1000
#define T_ANDA_RE		100
#define T_MAX_ANDAR		10000

/* variaveis globais */

// Objeto Display
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Objeto Motor
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *motor_m1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor_m2 = AFMS.getMotor(2);

int 	estado_menu;	// Posicao na pilha do menu
int 	estado_liga;	// Posicao na pilha do menu 11
int 	estado_motor;	// Posicao na pilha de acoes do motor
int 	pot_motor_m1;	// Forca de Trabalho (aplicada no motor)
int 	pot_motor_m2;	// Forca de Trabalho (aplicada no motor)
int 	tdt;			// Tempo de Trabalho (unidade de tempo para cada acao do robo)
int 	txt;			// Texto a ser escrito;
int 	led;			// Led aceso;
int 	ldr_valor;		// Intencidade de lus captada;
int 	cores[3];		// Taxa de reflexao de cor (Red Green Blue)
bool	ligado;			// Informa se o robo esta funcionando;
float 	carga;			// Tensão real da fonte
unsigned long tempo;	// Contador do tempo corrente
unsigned long t_menu;	// Contador de tempo do menu
unsigned long t_liga;	// Contador de tempo do menu 11
unsigned long t_motor;	// Contador de tempo do motor

/* lista de funcoes utilizadas */

void 	inicia_menu();
void 	inicia_motor();
void 	inicia_sensor();
// Inicializa as vatiaveis
void 	atualiza ();
// Atualiza as constantes utilizadas
int 	verifica_botao ();
// Identifica qual botao foi acionado
void 	menu();
// Gerenciador do menu e suas opcoes
void 	aciona_motor (int, int);
// Contola o acionamento dos motores
int 	anda(int, int);
// move o robo em linha reta
int 	gira(int, int);
// gira o robo em uma direcao
void 	aciona_luz(bool, bool, bool);
// Controla o acionamento das luzes
void 	ve_objeto();
// Percebe um obstaculo
void 	ve_cor();
// Percebe uma cor


/* funcoes utilizadas pelo arduino */

void setup()
{
	// put your setup code here, to run once:
	inicia_menu();
	inicia_motor();
	inicia_sensor();
}

void loop()
{
	// put your main code here, to run repeatedly:
	atualiza();
	menu();
	// aciona_luz(true,true,true);
}

/* implementação das funcoes */

void inicia_menu() // Inicializa as vatiaveis
{
	// Parametros iniciais
	estado_menu = 0;
	t_menu = 0;
	lcd.begin(16, 2);
}
void inicia_motor() // Inicializa as vatiaveis
{
	// Parametros iniciais
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

void atualiza () // Atualiza as constantes utilizadas
{
	int sensorValue = analogRead(A0);
	carga = sensorValue * (5.0 / 1023.0);
	ldr_valor = analogRead(LDR_PIN);
	tempo = millis();
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
	if ((tempo - t_menu) > T_MAX_MENU)
	{
		t_menu = tempo;
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
		else if (estado_menu == 9)
		{
			lcd.print ("Creditos        ");

			if (botao == CIMA)
			{
				estado_menu = 4;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 0;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 91;
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

			tempo = millis();
			if ((tempo - t_liga) > 1000)
			{
				t_liga = tempo;
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
					txt 	= 1;
					estado_motor = -1;
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
				estado_menu = 29;
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
		else if (estado_menu == 29)
		{
			lcd.print ("Voltar          ");

			if (botao == CIMA)
			{
				estado_menu = 23;
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
		else if (estado_menu == 34)
		{
			lcd.print ("Tempo          s");
			lcd.setCursor(7,1);
			lcd.print (millis());

			if (botao == CIMA)
			{
				estado_menu = 33;
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
				if (led < 3) led++;
			}
			else if (botao == ESQUERDA)
			{
				if (led > 0) led--;
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
	else if (estado_menu < 100) // Creditos - nivel 2
	{
		lcd.setCursor(0,0);
		lcd.print ("    CREDITOS    ");
		lcd.setCursor(0,1);

		if (estado_menu == 91)
		{
			lcd.print ("Arthur P Silva  ");

			if (botao == CIMA)
			{
				estado_menu = 99;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 92;
			}

		}
		else if (estado_menu == 92)
		{
			lcd.print ("Bernardo Sanches");

			if (botao == CIMA)
			{
				estado_menu = 91;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 93;
			}
		}
		else if (estado_menu == 93)
		{
			lcd.print ("Gustavo Campos  ");

			if (botao == CIMA)
			{
				estado_menu = 92;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 94;
			}
		}
		else if (estado_menu == 94)
		{
			lcd.print ("Matheus Santos  ");

			if (botao == CIMA)
			{
				estado_menu = 93;
			}
			else if (botao == BAIXO)
			{
				estado_menu = 99;
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
			if (!ligado) {
				ligado 	= true;
				estado_motor = -1;
			}
			estado_menu = 0;
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
	if (ligado && (tempo-t_motor < tmp_acao))
	{
		if (direcao == FRENTE)
		{
			aciona_motor(1,1);
		}
		else if (direcao == TRAS)
		{
			aciona_motor(-1,-1);
		}
		return 0;
	}
	else
	{
		aciona_motor(0,0);
		ligado = false;
		return 1;
	}
}

int gira(int direcao, int tmp_acao)
{
	if (ligado && (tempo-t_motor < tmp_acao))
	{
		if (direcao == DIREITA)
		{
			aciona_motor(1,-1);
		}
		else if (direcao == ESQUERDA)
		{
			aciona_motor(-1,1);
		}
		return 0;
	}
	else
	{
		aciona_motor(0,0);
		ligado = false;
		return 1;
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

void ve_objeto()
{
	// liga led
	// sensorValue
	// desliga led
}

void ve_cor()
{
	float r, g, b, total;

	aciona_luz(true, false, false);
	delay(200);
	r = analogRead(LDR_PIN); // sensorValue
	r += analogRead(LDR_PIN); // sensorValue
	r += analogRead(LDR_PIN); // sensorValue
	r += analogRead(LDR_PIN); // sensorValue
	r /= 4; // sensorValue
	aciona_luz(false, false, false);
	r -= analogRead(LDR_PIN); // sensorValue

	aciona_luz(false, true, false);
	delay(200);
	g = analogRead(LDR_PIN); // sensorValue
	g += analogRead(LDR_PIN); // sensorValue
	g += analogRead(LDR_PIN); // sensorValue
	g += analogRead(LDR_PIN); // sensorValue
	g /= 4; // sensorValue
	aciona_luz(false, false, false);
	g -= analogRead(LDR_PIN); // sensorValue

	aciona_luz(false, false, true);
	delay(200);
	b = analogRead(LDR_PIN); // sensorValue
	b += analogRead(LDR_PIN); // sensorValue
	b += analogRead(LDR_PIN); // sensorValue
	b += analogRead(LDR_PIN); // sensorValue
	b /= 4; // sensorValue
	aciona_luz(false, false, false);
	b -= analogRead(LDR_PIN); // sensorValue

	total = r + g + b;

	cores[RED] 		= r * 100 / total;
	// cores[RED] 		= r / 10;
	cores[GREEN] 	= g * 100 / total;
	// cores[GREEN] 	= g / 10;
	cores[BLUE] 	= b * 100 / total;
	// cores[BLUE] 	= b / 10;
}
