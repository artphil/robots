/*-------------------------------------------
SUMARIO do MENU
- Mascara (Sublinha em branco)
- Iniciar
--|- Iniciar(aciona funcionanento principal)
- Configurar
--|- LDR Lmt (fator de corte para encontrar objetos)
--|- pot ME (potencia do motor da roda esquerda)
--|- pot MD (potencia do motor da roda direita)
- Informacoes
--|- Bateria
--|- RGB (Intencidade refletida por cor(%))
--|- LDR (Intencidade refletida atual)
--|- Tempo (tempo de fincionam. do arduino em ms)
- Testes
--|- Motores auto (Movimentos pre estabelecidos)
--|- Anda F/T (acionamento manual)
--|- Gira D/E (acionamento manual)
--|- RGB (acionamento das luzes)
- Pre Moves
--|- Vai Vem
--|- Triangulo
--|- Quadrado
- Creditos
--|- Arthur P Silva
--|- Bernardo Sanches
--|- Gustavo Campos
--|- Matheus Santos
--------------------------------------------*/

/* Configuracao de ambiente */

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

// Definicoes das constantes de tempo
#define T_MAX_MENU 		180
#define T_TESTE 		2000
#define T_GIRO_30 		700
#define T_GIRO_90 		2100
#define T_ANDA_30		5000
#define T_ANDA_RE		500
#define T_MAX_ANDAR		10000
#define T_NOME  		400


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
	void print();
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
void entrada::print()
{
	for (int i = 0; i < tam; i++) {
		Serial.print("( ");
		Serial.print(direcao[i]);
		Serial.print(" , ");
		Serial.print(tempo[i]);
		Serial.println(" )");
	}
	Serial.println();
}

/* variaveis globais */

// Objeto Display
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Objeto Motor
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *motor_m1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor_m2 = AFMS.getMotor(3);

// Interacao do menu Geral
int 	botao;			// Botao pressionado no momento
int 	estado_menu;	// Posicao na pilha do menu
int 	estado_liga;	// Posicao na pilha do menu 11
unsigned long t_menu;	// Contador de tempo do menu
unsigned long t_liga;	// Contador de tempo do menu 11
// Interacao do menu CREDITOS
int letra = 0;
bool desliza = true;

// Controle do motor
int 	estado_motor;	// Posicao na pilha de acoes do motor
int 	pot_motor_m1;	// Forca de Trabalho (aplicada no motor)
int 	pot_motor_m2;	// Forca de Trabalho (aplicada no motor)
int 	mov;			// Movimento a ser efetuado;
bool	ligado;			// Informa se o robo esta funcionando;
// Sequenciass de movimentos
entrada teste;
entrada vai_vem;
entrada triangulo;
entrada quadrado;

// Leds e sensores
int 	led;			// Led aceso
int 	cor;			// Cor encontrada
int 	ldr_valor;		// Intencidade de luz captada
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
int 	media_sensor(int);
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
	Serial.begin(9600);

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
	pot_motor_m1 = 150;
	pot_motor_m2 = 150;
	ligado = false;
	AFMS.begin(); // create with the default frequency 1.6KHz
}

void inicia_sensor() // Inicializa as vatiaveis
{
	// Parametros iniciais
	ldr_cor[RED]	= 0.0;
	ldr_cor[GREEN]  = 0.0;
	ldr_cor[BLUE]	= 0.0;
	led = 0;
	cor = WHITE;
	ldr_limiar = 100;
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
	teste.print();

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
	quadrado.set( DIREITA, T_GIRO_90 );
	quadrado.set( FRENTE,  T_ANDA_30 );
	quadrado.set( DIREITA, T_GIRO_90 );
	quadrado.set( FRENTE,  T_ANDA_30 );
	quadrado.set( DIREITA, T_GIRO_90 );
	quadrado.set( FRENTE,  T_ANDA_30 );
	quadrado.set( DIREITA, T_GIRO_90 );
}

void atualiza () // Atualiza as constantes utilizadas
{
	int sensorValue = analogRead(A0);
	carga = sensorValue * (5.0 / 1023.0);
	ldr_valor = analogRead(LDR_PIN);
	objeto = ve_objeto();
	botao = 0;
}

int verifica_botao () // Identifica qual botao foi acionado
{
	int botao = analogRead (0); //Leitura do valor da porta analógica A0

	if (botao < 50) 		return DIREITA;
	else if (botao < 100) 	return CIMA;
	else if (botao < 300) 	return BAIXO;
	else if (botao < 500) 	return ESQUERDA;
	else if (botao < 700) 	return SELECIONA;
	else					return 0;

}

void m_iniciar(int n)	// Inicia processo de movimentos
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
			estado_motor = 0;
			t_motor = millis();
			estado_liga += 1;
		}
		else
		{
			lcd.print ("Ja em operacao  ");
			estado_menu = 9;
		}
	}
	else if (estado_liga < 10)
	{
		lcd.setCursor((estado_liga+5),1);
		lcd.print (".");
	}
	else estado_menu = 0;
}

void m_inicio(int n) // MENU - Nivel 1
{
	int ns = 7;
	String titulo = NOME_ROBO;
	String subtitulo[ns] =
	{
		"                ",
		"Iniciar         ",
		"Configurar      ",
		"Informacoes     ",
		"Testes          ",
		"Pre moves       ",
		"Creditos        "
	};

	if ((millis() - t_menu) > T_MAX_MENU)
	{
		t_menu = millis();
		botao = verifica_botao();
	}

	lcd.setCursor(0,0);
	lcd.print (titulo);
	lcd.setCursor(0,1);
	lcd.print (subtitulo[estado_menu%10]);

	switch (botao)
	{
		case CIMA:
		estado_menu--;
		if (estado_menu < n) estado_menu = (n+ns)-1;
		break;

		case BAIXO:
		estado_menu++;
		if (estado_menu >= (n+ns)) estado_menu = n;
		break;

		case DIREITA:
		break;

		case ESQUERDA:
		break;

		case SELECIONA:
		if (estado_menu == 1)
		{
			mov = 1;
			t_liga = millis();
			estado_liga = 0;
		}
		estado_menu = (estado_menu*10)+1;
		break;
	}
}

void m_configurar(int n) // Configurar - nivel 2
{
	String titulo = "   CONFIGURAR   ";
	int ns = 6;
	String subtitulo[ns] = {
		"Voltar          ",
		"ldrlmt -       +",
		"pot ME -       +",
		"pot MD -       +",
		"Balanco branco  ",
		"Balanco preto   ",
	};

	if ((millis() - t_menu) > T_MAX_MENU)
	{
		t_menu = millis();
		botao = verifica_botao();
	}

	lcd.setCursor(0,0);
	lcd.print (titulo);
	lcd.setCursor(0,1);
	lcd.print (subtitulo[estado_menu%10]);
	lcd.setCursor(10,1);

	if (estado_menu == n+1)      lcd.print (ldr_limiar);
	else if (estado_menu == n+2) lcd.print (pot_motor_m1);
	else if (estado_menu == n+3) lcd.print (pot_motor_m2);

	switch (botao)
	{
		case CIMA:
		estado_menu--;
		if (estado_menu < n) estado_menu = (n+ns)-1;
		break;

		case BAIXO:
		estado_menu++;
		if (estado_menu >= (n+ns)) estado_menu = n;
		break;

		case DIREITA:
		if (estado_menu == n+1)
		{	if (ldr_limiar < 10000) ldr_limiar += 10;
		}
		else if (estado_menu == n+2)
		{	if (pot_motor_m1 < 255) pot_motor_m1++;
		}
		else if (estado_menu == n+3)
		{	if (pot_motor_m2 < 255) pot_motor_m2++;
		}
		break;

		case ESQUERDA:
		if (estado_menu == n+1)
		{	if (ldr_limiar > 0) ldr_limiar -= 10;
		}
		else if (estado_menu == n+2)
		{	if (pot_motor_m1 > 0) pot_motor_m1--;
		}
		else if (estado_menu == n+3)
		{	if (pot_motor_m2 > 0) pot_motor_m2--;
		}
		break;

		case SELECIONA:
		if (estado_menu == n)
		{	estado_menu = (estado_menu/10);
		}
		else if (estado_menu == n+4)
		{	balanco_branco();
			estado_menu = (estado_menu/10);
		}
		else if (estado_menu == n+5)
		{	balanco_preto();
			estado_menu = (estado_menu/10);
		}
		break;
	}
}

void m_informacoes(int n) // Informacoes - nivel 2
{
	String titulo = "  INFORMACOES  ";
	int ns = 6;
	String subtitulo[ns] =
	{
		"Voltar          ",
		"Bateria        v",
		"R    G    B     ",
		"LDR             ",
		"Objeto          ",
		"Tempo         s",
	};

	if ((millis() - t_menu) > T_MAX_MENU)
	{
		t_menu = millis();
		botao = verifica_botao();
	}

	lcd.setCursor(0,0);
	lcd.print (titulo);
	lcd.setCursor(0,1);
	lcd.print (subtitulo[estado_menu%10]);
	lcd.setCursor(10,1);

	if (estado_menu == n+1) lcd.print (carga);
	else if (estado_menu == n+2)
	{
		lcd.setCursor(2,1);
		lcd.print (ldr_cor[RED]);
		lcd.setCursor(7,1);
		lcd.print (ldr_cor[GREEN]);
		lcd.setCursor(12,1);
		lcd.print (ldr_cor[BLUE]);
	}
	else if (estado_menu == n+3) lcd.print (ldr_valor);
	else if (estado_menu == n+4) objeto ? lcd.print ("SIM") : lcd.print ("NAO");
	else if (estado_menu == n+5) lcd.print (millis());

	switch (botao)
	{
		case CIMA:
		estado_menu--;
		if (estado_menu < n) estado_menu = (n+ns)-1;
		break;

		case BAIXO:
		estado_menu++;
		if (estado_menu >= (n+ns)) estado_menu = n;
		break;

		case DIREITA:
		break;

		case ESQUERDA:
		break;

		case SELECIONA:
		if (estado_menu == n) estado_menu = (estado_menu/10);
		else if (estado_menu == n+2) ve_cor();
		break;
	}
}

void m_testes(int n) // Testes - nivel 2
{
	String titulo = "     TESTES     ";
	int ns = 5;
	String subtitulo[ns] =
	{
		"Voltar          ",
		"Motores auto    ",
		"F     Anda     T",
		"E     Gira     D",
		"<  R   G   B   >",
	};

	if ((millis() - t_menu) > T_MAX_MENU)
	{
		t_menu = millis();
		botao = verifica_botao();
	}

	lcd.setCursor(0,0);
	lcd.print (titulo);
	lcd.setCursor(0,1);
	lcd.print (subtitulo[estado_menu%10]);

	if (estado_menu == n+4 && led != 0)
	{
		switch (led)
		{
			case 0:
			aciona_luz(false, false, false);
			break;

			case 1:
			lcd.setCursor(2,1);
			lcd.print ("*");
			lcd.setCursor(4,1);
			lcd.print ("*");
			aciona_luz(true, false, false);
			break;

			case 2:
			lcd.setCursor(6,1);
			lcd.print ("*");
			lcd.setCursor(8,1);
			lcd.print ("*");
			aciona_luz(false, true, false);
			break;

			case 3:
			lcd.setCursor(10,1);
			lcd.print ("*");
			lcd.setCursor(12,1);
			lcd.print ("*");
			aciona_luz(false, false, true);
			break;

			case 4:
			for (int i = 2; i < 13; i+=2)
			{
				lcd.setCursor(i,1);
				lcd.print ("*");
			}
			aciona_luz(true, true, true);
			break;
		}
	}

	switch (botao)
	{
		case CIMA:
		estado_menu--;
		if (estado_menu < n) estado_menu = (n+ns)-1;
		break;

		case BAIXO:
		estado_menu++;
		if (estado_menu >= (n+ns)) estado_menu = n;
		break;

		case DIREITA:
		if (estado_menu == n+2)			anda(TRAS, T_MAX_MENU);
		else if (estado_menu == n+3)	anda(DIREITA, T_MAX_MENU);
		else if (estado_menu == n+4)
		{
			if (led < 4) led++;
			else led = 0;
		}
		break;

		case ESQUERDA:
		if (estado_menu == n+2)			anda(FRENTE, T_MAX_MENU);
		else if (estado_menu == n+3)	anda(ESQUERDA, T_MAX_MENU);
		else if (estado_menu == n+4)
		{
			if (led > 0) led--;
			else led = 4;
		}
		break;

		case SELECIONA:
		if (estado_menu == n) estado_menu = (estado_menu/10);
		else if (estado_menu == n+1)
		{
			estado_menu = 10;
			mov 	= 2;
			t_liga = millis();
			estado_liga = 0;
		}
		break;
	}
}

void m_pre_moves(int n) // Pre moves - nivel 2
{
	String titulo = "   PRE  MOVES   ";
	int ns = 4;
	String subtitulo[ns] =
	{
		"Voltar          ",
		"Vai Vem         ",
		"Triangulo       ",
		"Quadrado         ",
	};

	if ((millis() - t_menu) > T_MAX_MENU)
	{
		t_menu = millis();
		botao = verifica_botao();
	}

	lcd.setCursor(0,0);
	lcd.print (titulo);
	lcd.setCursor(0,1);
	lcd.print (subtitulo[estado_menu%10]);
	mov = (estado_menu%10)+2;

	switch (botao)
	{
		case CIMA:
		estado_menu--;
		if (estado_menu < n) estado_menu = (n+ns)-1;
		break;

		case BAIXO:
		estado_menu++;
		if (estado_menu >= (n+ns)) estado_menu = n;
		break;

		case DIREITA:
		break;

		case ESQUERDA:
		break;

		case SELECIONA:
		if (estado_menu == n) estado_menu = (estado_menu/10);
		else
		{
			estado_menu = 10;
			t_liga = millis();
			estado_liga = 0;
		}
		break;
	}
}

void m_creditos(int n) // Creditos - nivel 2
{
	String titulo = "    CREDITOS    ";
	int ns  = 6;
	int tam = 70;
	char subtitulo[ns][tam] =
	{
		"Voltar                                                                ",
		" Arthur Phillip Silva - Hacker Verde - Sistemas de Informacao         ",
		" Bernardo Sanches - Lego Builder Azul - Engenharia Aeroespacial       ",
		" Gustavo Duarte - Eletrician Vermelho - Eng. Controle e Automacao     ",
		" Matheus Santos - Gear Master Preto - Eng. Controle e Automacao       ",
		" Professor: Douglas G. Macharet - Introducao a Robotica - UFMG        "
	};

	if ((millis() - t_menu) > T_MAX_MENU)
	{
		t_menu = millis();
		botao = verifica_botao();
	}

	lcd.setCursor(0,0);
	lcd.print (titulo);

	if (desliza && estado_menu != n)
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
		lcd.print (subtitulo[estado_menu%10][(letra+l)%tam]);
	}

	switch (botao)
	{
		case CIMA:
		estado_menu--;
		if (estado_menu < n) estado_menu = (n+ns)-1;
		letra = 0;
		break;

		case BAIXO:
		estado_menu++;
		if (estado_menu >= (n+ns)) estado_menu = n;
		letra = 0;
		break;

		case DIREITA:
		break;

		case ESQUERDA:
		break;

		case SELECIONA:
		if (estado_menu == n) estado_menu = (estado_menu/10);
		else desliza = !desliza;
		break;
	}
}

void menu() // Gerenciador do menu e suas opcoes
{
	if (estado_menu < 10) m_inicio(0);
	else if (estado_menu < 20) m_iniciar(10);
	else if (estado_menu < 30) m_configurar(20);
	else if (estado_menu < 40) m_informacoes(30);
	else if (estado_menu < 50) m_testes(40);
	else if (estado_menu < 60) m_pre_moves(50);
	else if (estado_menu < 70) m_creditos(60);
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
	Serial.print("Direcao: ");
	Serial.println(direcao);
	Serial.print("Tempo: ");
	Serial.println(tmp_acao);

	if (millis()-t_motor < tmp_acao)
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
		else if (direcao == TRAS)
		{
			aciona_motor(0,0);
		}
		else
		{
			aciona_motor(0,0);
			return 1;
		}
	}
	else
	{
		aciona_motor(0,0);
		return 1;
	}
	return 0;
}

int move(entrada a)
{
	if (estado_motor >= a.get_tam())
	{
		ligado = false;
		aciona_motor(0,0);
		return 1;
	}


	if (anda(a.get_dir(estado_motor), a.get_tmp(estado_motor)) == 1)
	{
		estado_motor += 1;
		t_motor = millis();
	}
	Serial.print("Estado: ");
	Serial.println(estado_motor);

	return 0;
}

int move_auto()
{
	switch (estado_motor) {
		case 0:	// Anda por 10 minutos
		if (anda(FRENTE,T_MAX_ANDAR) == 1) return 1; // Acaba o tempo
		else if (objeto) estado_motor = 1; // Encontra objeto
		break;

		case 1: // Analiza cor do objeto
		ve_cor();
		if (cor == RED)
		{
			estado_motor = 2;
			t_motor = millis();
		}
		if (cor == GREEN)
		{
			estado_motor = 3;
			t_motor = millis();
		}
		if (cor == BLUE)
		{
			estado_motor = 4;
			t_motor = millis();
		}
		if (cor == YELLOW)
		{
			estado_motor = 5;
			t_motor = millis();
		}
		if (cor == BLACK)
		{
			estado_motor = 6;
			t_motor = millis();
		}
		break;

		case 2:
		while (anda(TRAS,T_ANDA_RE) != 1); // anda para tras
		while (anda(DIREITA,T_GIRO_90) != 1); // gira 90 graus
		while (anda(DIREITA,T_GIRO_90) != 1); // gira 90 graus
		while (anda(DIREITA,T_GIRO_90) != 1); // gira 90 graus
		while (anda(DIREITA,T_GIRO_90) != 1); // gira 90 graus
		return 1;
		break;

		case 3:
		while (anda(TRAS,T_ANDA_RE) != 1); // anda para tras
		while (anda(ESQUERDA,T_GIRO_90) != 1); // gira 90 graus
		estado_motor = 0;
		t_motor = millis();
		break;

		case 4:
		while (anda(TRAS,T_ANDA_RE) != 1); // anda para tras
		while (anda(DIREITA,T_GIRO_90) != 1); // gira 90 graus
		estado_motor = 0;
		t_motor = millis();
		break;

		case 5:
		while (anda(TRAS,T_ANDA_RE) != 1); // anda para tras
		while (anda(ESQUERDA,T_GIRO_90) != 1); // gira 90 graus
		while (anda(ESQUERDA,T_GIRO_90) != 1); // gira 90 graus
		estado_motor = 0;
		t_motor = millis();
		break;
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
		if (move(teste) == 1) 		ligado = false;
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

int media_sensor(int n)
{
	int total = 0;
	for (int i = 0;i < n;i++)
	{
		total += analogRead(LDR_PIN);
		delay(100);
	}
	return total/n;
}

bool ve_objeto()
{
	if (ldr_valor > ldr_limiar) return false;
	else return true;
}

void ve_cor()
{
	int r, g, b, cinza;

	lcd.setCursor(0,1);	lcd.print ("Analizando      ");

	aciona_luz(true, false, false);
	r = media_sensor(5);
	aciona_luz(false, false, false);

	lcd.setCursor(11,1);	lcd.print (".");

	aciona_luz(false, true, false);
	g = media_sensor(5);
	aciona_luz(false, false, false);

	lcd.setCursor(12,1);	lcd.print (".");

	aciona_luz(false, false, true);
	b = media_sensor(5);
	aciona_luz(false, false, false);

	lcd.setCursor(13,1);	lcd.print (".");
	delay(200);

	cinza = ldr_branco[RED]-ldr_preto[RED];
	ldr_cor[RED] 		= (r - ldr_preto[RED])/(cinza)*100;

	lcd.setCursor(14,1);	lcd.print (".");
	delay(200);

	cinza = ldr_branco[GREEN]-ldr_preto[GREEN];
	ldr_cor[GREEN] 	= (g - ldr_preto[GREEN])/(cinza)*100;

	lcd.setCursor(15,1);	lcd.print (".");
	delay(200);

	cinza = ldr_branco[BLUE]-ldr_preto[BLUE];
	ldr_cor[BLUE] 	= (b - ldr_preto[BLUE])/(cinza)*100;
}

void balanco_branco()
{
	lcd.setCursor(0,1);	lcd.print ("Analizando      ");

	aciona_luz(true, false, false);
	ldr_branco[RED] = media_sensor(5);
	aciona_luz(false, false, false);

	lcd.setCursor(11,1);	lcd.print (".");

	aciona_luz(false, true, false);
	ldr_branco[GREEN] = media_sensor(5);
	aciona_luz(false, false, false);

	lcd.setCursor(12,1);	lcd.print (".");

	aciona_luz(false, false, true);
	ldr_branco[BLUE] = media_sensor(5);
	aciona_luz(false, false, false);

	lcd.setCursor(13,1);	lcd.print (".");
	delay(200);
	lcd.setCursor(14,1);	lcd.print (".");
	delay(200);
	lcd.setCursor(15,1);	lcd.print (".");
	delay(200);
}

void balanco_preto()
{
	lcd.setCursor(0,1);	lcd.print ("Analizando      ");

	aciona_luz(true, false, false);
	ldr_preto[RED] = media_sensor(5);
	aciona_luz(false, false, false);

	lcd.setCursor(11,1);	lcd.print (".");

	aciona_luz(false, true, false);
	ldr_preto[GREEN] = media_sensor(5);
	aciona_luz(false, false, false);

	lcd.setCursor(12,1);	lcd.print (".");

	aciona_luz(false, false, true);
	ldr_preto[BLUE] = media_sensor(5);
	aciona_luz(false, false, false);

	lcd.setCursor(13,1);	lcd.print (".");
	delay(200);
	lcd.setCursor(14,1);	lcd.print (".");
	delay(200);
	lcd.setCursor(15,1);	lcd.print (".");
	delay(200);
}
