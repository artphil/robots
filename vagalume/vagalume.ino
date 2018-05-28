/*-------------------------------------------
SUMARIO do MENU
- Mascara (Sublinha em branco)
- Iniciar
--|- Iniciar(aciona funcionanento principal)
- Configurar
--|- Balanco branco
--|- Balanco preto
--|- LDR Lmt (fator de corte para encontrar objetos)
--|- pot ME (potencia do motor da roda esquerda)
--|- pot MD (potencia do motor da roda direita)
--|- anda30 (tempo para o robor andar 30cm)
--|- giro90 (tempo para o robo girar 90 graus)
--|- GRAVAR (grava dados no EEPROM)
--|- LE SALVO (le dados do EEPROM)
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
#include <EEPROM.h>
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
#define t_teste 		2000
#define t_anda_re		600
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

Adafruit_DCMotor *motor_D = AFMS.getMotor(1);
Adafruit_DCMotor *motor_E = AFMS.getMotor(3);

// Interacao do menu Geral
int 	botao;			// Botao pressionado no momento
int 	tst;			// intica qual teste esta sendo realizado
int 	estado_menu;	// Posicao na pilha do menu
int 	estado_antigo;	// Posicao anterior na pilha do menu
int 	estado_liga;	// Posicao na pilha do menu 11
unsigned long t_menu;	// Contador de tempo do menu
unsigned long t_liga;	// Contador de tempo do menu 11
// Interacao do menu CREDITOS
int letra = 0;
bool desliza = true;

// Controle do motor
int 	estado_motor;	// Posicao na pilha de acoes do motor
int 	estado_move;	// Posicao na pilha de acoes do motor
int 	pot_motor_D;	// Forca de Trabalho (aplicada no motor)
int 	pot_motor_E;	// Forca de Trabalho (aplicada no motor)
int 	mov;			// Movimento a ser efetuado;
bool	ligado;			// Informa se o robo esta funcionando;
int t_giro_45;
int t_giro_90;
int t_anda;
// Sequenciass de movimentos
entrada teste;
entrada vai_vem;
entrada triangulo;
entrada quadrado;
entrada giro45;
entrada giro90;

// Leds e sensores
int 	led;			// Led aceso
bool 	led_atual[3];		// Verifica se a mudou a os leds
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
int 	aciona_luz(bool, bool, bool);
// Retorna a media de leituras do sensor
int 	media_sensor(int);
// Percebe um obstaculo
bool 	ve_objeto();
// Percebe uma cor
void 	ve_cor();
// Calibra o receptor de luz
void balanco_branco();
void balanco_preto();
// Manipula a memoria EEPROM
// Variveis: pot_motor_m*, ldr_limiar, ldr_branco e ldr_preto.
void grava_EEPROM ();
void le_EEPROM ();


/* funcoes proprietarias do arduino */

void setup()
{
	Serial.begin(9600);

    le_EEPROM ();
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

// void inicia_eeprom() // Inicializa as vatiaveis
// {
	// pot_motor_D = 140;
	// pot_motor_E = 160;
	// t_giro_45 = 1100;
	// t_giro_90 = 2100;
	// t_anda = 5000;
	// ldr_limiar = 170;
	// le_EEPROM();
// }

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
	led_atual[0] = false;
	led_atual[1] = false;
	led_atual[2] = false;
	pinMode(LED_RED_PIN,OUTPUT);
	pinMode(LED_GRN_PIN,OUTPUT);
	pinMode(LED_BLU_PIN,OUTPUT);
}

void 	inicia_entradas()
{
	teste.set_tam(8);
	teste.reset();
	teste.set( FRENTE,   t_teste );
	teste.set( PARA, 	 t_teste );
	teste.set( TRAS,     t_teste );
	teste.set( PARA,   	 t_teste );
	teste.set( DIREITA,  t_teste );
	teste.set( PARA,   	 t_teste );
	teste.set( ESQUERDA, t_teste );
	teste.set( PARA,   	 t_teste );
	teste.print();

	vai_vem.set_tam(6);
	vai_vem.reset();
	vai_vem.set( FRENTE,  t_anda );
	vai_vem.set( ESQUERDA, t_giro_90 );
	vai_vem.set( ESQUERDA, t_giro_90 );
	vai_vem.set( FRENTE,  t_anda );
	vai_vem.set( DIREITA, t_giro_90 );
	vai_vem.set( DIREITA, t_giro_90 );

	triangulo.set_tam(8);
	triangulo.reset();
	triangulo.set( FRENTE,  t_anda );
	triangulo.set( DIREITA, t_giro_90 );
	triangulo.set( FRENTE,  t_anda );
	triangulo.set( DIREITA, t_giro_90 );
	triangulo.set( DIREITA, t_giro_45 );
	triangulo.set( FRENTE,  (int)(t_anda*sqrt(2)) );
	triangulo.set( DIREITA, t_giro_90 );
	triangulo.set( DIREITA, t_giro_45 );

	quadrado.set_tam(8);
	quadrado.reset();
	quadrado.set( FRENTE,  t_anda );
	quadrado.set( DIREITA, t_giro_90 );
	quadrado.set( FRENTE,  t_anda );
	quadrado.set( DIREITA, t_giro_90 );
	quadrado.set( FRENTE,  t_anda );
	quadrado.set( DIREITA, t_giro_90 );
	quadrado.set( FRENTE,  t_anda );
	quadrado.set( DIREITA, t_giro_90 );

	giro45.set_tam(3);
	giro45.reset();
	giro45.set( DIREITA,  t_giro_45 );
	giro45.set( PARA,  	  t_giro_45 );
	giro45.set( ESQUERDA, t_giro_45 );

	giro90.set_tam(3);
	giro90.reset();
	giro90.set( DIREITA,  t_giro_90 );
	giro90.set( PARA,     t_giro_90 );
	giro90.set( ESQUERDA, t_giro_90 );
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

void grava_EEPROM ()
{
	lcd.setCursor(0,1);
	lcd.print ("Gravando        ");

	EEPROM.write(0, pot_motor_D);
	EEPROM.write(1, pot_motor_E);

	EEPROM.write(2, ldr_limiar);

	EEPROM.write(3, ldr_branco[0]/4);
	EEPROM.write(4, ldr_branco[1]/4);
	EEPROM.write(5, ldr_branco[2]/4);

	EEPROM.write(6, ldr_preto[0]/4);
	EEPROM.write(7, ldr_preto[1]/4);
	EEPROM.write(8, ldr_preto[2]/4);

	EEPROM.write(9, t_anda/100);
	EEPROM.write(10, t_giro_90/100);

	for (size_t i = 0; i < 4; i++) {
		lcd.setCursor(9+i,1);
		lcd.print (".");
		delay(200);
	}
}

void le_EEPROM ()
{
	lcd.setCursor(0,1);
	lcd.print ("Lendo dados     ");

	pot_motor_D = EEPROM.read(0);
	pot_motor_E = EEPROM.read(1);

	ldr_limiar   = EEPROM.read(2);

	ldr_branco[0]= EEPROM.read(3)*4;
	ldr_branco[1]= EEPROM.read(4)*4;
	ldr_branco[2]= EEPROM.read(5)*4;

	ldr_preto[0]= EEPROM.read(6)*4;
	ldr_preto[1]= EEPROM.read(7)*4;
	ldr_preto[2]= EEPROM.read(8)*4;

	t_anda  	= EEPROM.read(9)*100;
	t_giro_90  	= EEPROM.read(10)*100;
	t_giro_45 = t_giro_90/2;

	for (size_t i = 0; i < 4; i++) {
		lcd.setCursor(12+i,1);
		lcd.print (".");
		delay(200);
	}
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
			ligado 	= true;
			estado_motor = 0;
			t_motor = millis();
			estado_liga += 1;
		}
		else
		{
			lcd.print ("Ja em operacao  ");
			estado_menu = 0;
		}
	}
	else if (estado_liga > 10) estado_menu = 0;
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
void m_conf_motor(int n)
{
	String titulo = "     MOTORES    ";
	int ns = 5;
	String subtitulo[ns] = {
		"Voltar          ",
		"pot ME -       +",
		"pot MD -       +",
		"anda30 -       +",
		"giro90 -       +"
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

	if (estado_menu == n+1) lcd.print (pot_motor_D);
	else if (estado_menu == n+2) lcd.print (pot_motor_E);
	else if (estado_menu == n+3) lcd.print (t_anda);
	else if (estado_menu == n+4) lcd.print (t_giro_90);

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
		{
			if (pot_motor_D < 255) pot_motor_D++;
		}
		else if (estado_menu == n+2)
		{
			if (pot_motor_E < 255) pot_motor_E++;
		}
		else if (estado_menu == n+3)
		{
			if (t_anda < 25500) t_anda+=100;
		}
		else if (estado_menu == n+4)
		{
			if (t_giro_90 < 25500)
			{
				t_giro_90+=100;
				t_giro_45 = t_giro_90/2;
			}
		}
		break;

		case ESQUERDA:
		if (estado_menu == n+1)
		{
			if (pot_motor_D > 0) pot_motor_D--;
		}
		else if (estado_menu == n+2)
		{
			if (pot_motor_E > 0) pot_motor_E--;
		}
		else if (estado_menu == n+3)
		{
			if (t_anda > 0) t_anda-=100;
		}
		else if (estado_menu == n+4)
		{
			if (t_giro_90 > 0)
			{
				t_giro_90-=100;
				t_giro_45 = t_giro_90/2;
			}
		}
		break;

		case SELECIONA:
		if (estado_menu == n)
		{
			estado_menu = (estado_menu/10);
		}
		break;
	}

}

void m_conf_sensor(int n)
{
	String titulo = "    SENSORES    ";
	int ns = 4;
	String subtitulo[ns] = {
		"Voltar          ",
		"Balanco branco  ",
		"Balanco preto   ",
		"LDRlmt -       +"
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

	if (estado_menu == n+3)      lcd.print (ldr_limiar);

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
		if (estado_menu == n+3)
		{
			if (ldr_limiar < 1023) ldr_limiar++;
		}
		break;

		case ESQUERDA:
		if (estado_menu == n+3)
		{
			if (ldr_limiar > 0) ldr_limiar--;
		}
		break;

		case SELECIONA:
		if (estado_menu == n)
		{
			estado_menu = (estado_menu/10);
		}
		else if (estado_menu == n+1)
		{
			balanco_branco();
			estado_menu = (estado_menu/10);
		}
		else if (estado_menu == n+2)
		{
			balanco_preto();
			estado_menu = (estado_menu/10);
		}
		break;
	}

}

void m_configurar2(int n) // Configurar - nivel 2
{
	String titulo = "   CONFIGURAR   ";
	int ns = 5;
	String subtitulo[ns] = {
		"Voltar          ",
		"Motores         ",
		"Sensores        ",
		"     GRAVAR     ",
		"    LE SALVO    "
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
		if (estado_menu == n)
		{
			estado_menu = (estado_menu/10);
		}
		else if (estado_menu == n+3)
		{
			grava_EEPROM();
			estado_menu = (estado_menu/10);
		}
		else if (estado_menu == n+4)
		{
			le_EEPROM();
			estado_menu = (estado_menu/10);
		}
		else
		{
			estado_menu = (estado_menu*10);
		}
		break;
	}
}
void m_configurar(int n) // Configurar - nivel 2
{
	String titulo = "   CONFIGURAR   ";
	int ns = 10;
	String subtitulo[ns] = {
		"Voltar          ",
		"Balanco branco  ",
		"Balanco preto   ",
		"LDRlmt -       +",
		"pot ME -       +",
		"pot MD -       +",
		"anda30 -       +",
		"giro90 -       +",
		"     GRAVAR     ",
		"    LE SALVO    "
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

	if (estado_menu == n+3)      lcd.print (ldr_limiar);
	else if (estado_menu == n+4) lcd.print (pot_motor_D);
	else if (estado_menu == n+5) lcd.print (pot_motor_E);
	else if (estado_menu == n+6) lcd.print (t_anda);
	else if (estado_menu == n+7) lcd.print (t_giro_90);

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
		if (estado_menu == n+3)
		{
			if (ldr_limiar < 1023) ldr_limiar++;
		}
		else if (estado_menu == n+4)
		{
			if (pot_motor_D < 255) pot_motor_D++;
		}
		else if (estado_menu == n+5)
		{
			if (pot_motor_E < 255) pot_motor_E++;
		}
		else if (estado_menu == n+6)
		{
			if (t_anda < 25500) t_anda+=100;
		}
		else if (estado_menu == n+7)
		{
			if (t_giro_90 < 25500)
			{
				t_giro_90+=100;
				t_giro_45 = t_giro_90/2;
			}
		}
		break;

		case ESQUERDA:
		if (estado_menu == n+3)
		{
			if (ldr_limiar > 0) ldr_limiar--;
		}
		else if (estado_menu == n+4)
		{
			if (pot_motor_D > 0) pot_motor_D--;
		}
		else if (estado_menu == n+5)
		{
			if (pot_motor_E > 0) pot_motor_E--;
		}
		else if (estado_menu == n+6)
		{
			if (t_anda > 0) t_anda-=100;
		}
		else if (estado_menu == n+7)
		{
			if (t_giro_90 > 0)
			{
				t_giro_90-=100;
				t_giro_45 = t_giro_90/2;
			}
		}
		break;

		case SELECIONA:
		if (estado_menu == n)
		{
			estado_menu = (estado_menu/10);
		}
		else if (estado_menu == n+1)
		{
			balanco_branco();
			estado_menu = (estado_menu/10);
		}
		else if (estado_menu == n+2)
		{
			balanco_preto();
			estado_menu = (estado_menu/10);
		}
		else if (estado_menu == n+8)
		{
			grava_EEPROM();
			estado_menu = (estado_menu/10);
		}
		else if (estado_menu == n+9)
		{
			le_EEPROM();
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
		"R    G    B     ",
		"LDR             ",
		"Objeto          ",
		"Bateria        v",
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

	if (estado_menu == n+1)
	{
		lcd.setCursor(2,1);
		lcd.print (ldr_cor[RED]);
		lcd.setCursor(7,1);
		lcd.print (ldr_cor[GREEN]);
		lcd.setCursor(12,1);
		lcd.print (ldr_cor[BLUE]);
	}
	else if (estado_menu == n+2) lcd.print (ldr_valor);
	else if (estado_menu == n+3) objeto ? lcd.print ("SIM") : lcd.print ("NAO");
	else if (estado_menu == n+4) lcd.print (carga);
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
		else if (estado_menu == n+1) ve_cor();
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
		"<  R   G   B   >",
		"F     Anda     T",
		"E     Gira     D",
		"Motores auto    ",
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

	if (estado_menu == n+2)
	{
		switch (tst)
		{
			case 0:
			aciona_motor(0, 0);
			break;

			case 1:
			aciona_motor(-1, -1);
			break;

			case 2:
			aciona_motor(1, 1);
			break;
		}
	}
	else if (estado_menu == n+3)
	{
		switch (tst)
		{
			case 0:
			aciona_motor(0, 0);
			break;

			case 1:
			aciona_motor(1, -1);
			break;

			case 2:
			aciona_motor(-1, 1);
			break;
		}
	}
	else if (estado_menu == n+1)
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
		led = tst = 0;
		break;

		case BAIXO:
		estado_menu++;
		if (estado_menu >= (n+ns)) estado_menu = n;
		led = tst = 0;
		break;

		case DIREITA:
		if (estado_menu == n+2)
		{
			tst = 1;
		}
		else if (estado_menu == n+3)
		{
			tst = 1;
		}
		else if (estado_menu == n+4)
		{
			if (led < 4) led++;
			else led = 0;
			Serial.print("led: ");
			Serial.println(led);
		}
		break;

		case ESQUERDA:
		if (estado_menu == n+2)
		{
			tst = 2;
		}
		else if (estado_menu == n+3)
		{
			tst = 2;
		}
		else if (estado_menu == n+1)
		{
			if (led > 0) led--;
			else led = 4;
			Serial.println("led: ");
			Serial.println(led);
		}
		break;

		case SELECIONA:
		if (estado_menu == n) estado_menu = (estado_menu/10);
		else if (estado_menu == n+2)
		{
			tst = 0;
		}
		else if (estado_menu == n+3)
		{
			tst = 0;
		}
		else if (estado_menu == n+4)
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
	int ns = 6;
	String subtitulo[ns] =
	{
		"Voltar          ",
		"Vai Vem         ",
		"Triangulo       ",
		"Quadrado        ",
		"Giro 45         ",
		"Giro 90         ",
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
	else if (estado_menu < 30) m_configurar(20);
	else if (estado_menu < 220) m_conf_motor(210);
	else if (estado_menu < 230) m_conf_sensor(220);
}

void aciona_motor (int m1, int m2)
{
	if (m1 > 0)
	{
		motor_D->setSpeed(pot_motor_D);
		motor_D->run(FORWARD);
	}
	else if (m1 < 0)
	{
		motor_D->setSpeed(pot_motor_D);
		motor_D->run(BACKWARD);
	}
	else
	{
		motor_D->run(RELEASE);
	}

	if (m2 > 0)
	{
		motor_E->setSpeed(pot_motor_E);
		motor_E->run(FORWARD);
	}
	else if (m2 < 0)
	{
		motor_E->setSpeed(pot_motor_E);
		motor_E->run(BACKWARD);
	}
	else
	{
		motor_E->run(RELEASE);
	}
}

int anda(int direcao, int tmp_acao)
{
	Serial.print("Direcao: ");
	Serial.println(direcao);
	Serial.print("Tempo: ");
	Serial.println(tmp_acao);

	int tmp_aux = millis()-t_motor;

	if (tmp_aux < tmp_acao)
	{
		lcd.setCursor(0,1);

		if (direcao == FRENTE)
		{
			aciona_motor(1,1);
			lcd.print ("FRENTE          ");
		}
		else if (direcao == TRAS)
		{
			aciona_motor(-1,-1);
			lcd.print ("TRAS            ");
		}
		else if (direcao == DIREITA)
		{
			aciona_motor(1,-1);
			lcd.print ("DIREITA         ");
		}
		else if (direcao == ESQUERDA)
		{
			aciona_motor(-1,1);
			lcd.print ("ESQUERDA        ");
		}
		else if (direcao == PARA)
		{
			aciona_motor(0,0);
			lcd.print ("PARA            ");
		}
		else
		{
			aciona_motor(0,0);
		}

		lcd.setCursor(12,1);
		lcd.print (tmp_aux / 1000);
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
		else if (objeto)  			// Encontra objeto
		{
			estado_motor = 1;
			estado_move = 0;
		}
		break;

		case 1: // Analiza cor do objeto
		if (anda(PARA,T_MAX_ANDAR) == 1) return 1; 	// Acaba o tempo
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
			estado_motor = 1;
		}
		if (cor == WHITE)
		{
			estado_motor = 1;
		}
		break;

		case 2:
		while (anda(TRAS,t_anda_re) != 1); 		// anda para tras
		t_motor = millis();
		while (anda(DIREITA,t_giro_90) != 1); 	// gira 90 graus
		t_motor = millis();
		while (anda(DIREITA,t_giro_90) != 1); 	// gira 90 graus
		t_motor = millis();
		while (anda(DIREITA,t_giro_90) != 1); 	// gira 90 graus
		t_motor = millis();
		while (anda(DIREITA,t_giro_90) != 1); 	// gira 90 graus
		t_motor = millis();
			// if (estado_move==0 && anda(TRAS,t_anda_re)==1)
		// {
		// 	estado_move+=1;
		// 	t_motor = millis();
		// }
		// else if (estado_move==1 && anda(DIREITA,t_giro_90)==1)
		// {
		// 	estado_move+=1;
		// 	t_motor = millis();
		// }
		// else if (estado_move==2 && anda(DIREITA,t_giro_90)==1)
		// {
		// 	estado_move+=1;
		// 	t_motor = millis();
		// }
		// else if (estado_move==3 && anda(DIREITA,t_giro_90)==1)
		// {
		// 	estado_move+=1;
		// 	t_motor = millis();
		// }
		// else if (estado_move==4 && anda(DIREITA,t_giro_90)==1)
		// {
		// 	estado_move+=1;
		// 	t_motor = millis();
		// }
		// else if (estado_move==5)
		return 1;
		break;

		case 3:
		while (anda(TRAS,t_anda_re) != 1); 		// anda para tras
		t_motor = millis();
		while (anda(ESQUERDA,t_giro_90) != 1); 	// gira 90 graus
		estado_motor = 0;
		t_motor = millis();
		break;

		case 4:
		while (anda(TRAS,t_anda_re) != 1); 		// anda para tras
		t_motor = millis();
		while (anda(DIREITA,t_giro_90) != 1); 	// gira 90 graus
		estado_motor = 0;
		t_motor = millis();
		break;

		case 5:
		while (anda(TRAS,t_anda_re) != 1); 		// anda para tras
		t_motor = millis();
		while (anda(ESQUERDA,t_giro_90) != 1); 	// gira 90 graus
		t_motor = millis();
		while (anda(ESQUERDA,t_giro_90) != 1); 	// gira 90 graus
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
	else if (mov == 6)
	{
		if (move(giro45) == 1) 		ligado = false;
	}
	else if (mov == 7)
	{
		if (move(giro90) == 1) 		ligado = false;
	}
}

int aciona_luz(bool r, bool g, bool b)
{
	if (led_atual[0] == r)
	if (led_atual[1] == g)
	if (led_atual[2] == b)
	return 1;

	led_atual[0] = r;
	led_atual[1] = g;
	led_atual[2] = b;

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
		delay(200);
		total += analogRead(LDR_PIN);
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
	int r, g, b;
	float cinza;

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
	ldr_cor[RED] 	= (r - ldr_preto[RED])*(255.0/cinza);

	Serial.print("calculo red = ");
	Serial.print(r);
	Serial.print("-");
	Serial.print(ldr_preto[RED]);
	Serial.print("*255 /");
	Serial.println(cinza);

	cinza = ldr_branco[GREEN]-ldr_preto[GREEN];
	ldr_cor[GREEN] 	= (g - ldr_preto[GREEN])*(255.0/cinza);

	Serial.print("calculo green = ");
	Serial.print(g);
	Serial.print("-");
	Serial.print(ldr_preto[GREEN]);
	Serial.print("*255 /");
	Serial.println(cinza);

	cinza = ldr_branco[BLUE]-ldr_preto[BLUE];
	ldr_cor[BLUE] 	= (b - ldr_preto[BLUE])*(255.0/cinza);

	Serial.print("calculo blue = ");
	Serial.print(b);
	Serial.print("-");
	Serial.print(ldr_preto[BLUE]);
	Serial.print("*255 /");
	Serial.println(cinza);

	Serial.print("bruto =  ");
	Serial.print(r);
	Serial.print(", ");
	Serial.print(g);
	Serial.print(", ");
	Serial.println(b);

	if ((ldr_cor[RED] + ldr_cor[GREEN] + ldr_cor[BLUE]) < 50)
	{
		cor = BLACK;
	}
	else if ((ldr_cor[RED] + ldr_cor[GREEN] + ldr_cor[BLUE]) > 700)
	{
		cor = WHITE;
	}
	else
	{
		cinza = (ldr_cor[RED] + ldr_cor[GREEN] + ldr_cor[BLUE]);
		r = (100.0 * ldr_cor[RED]) / cinza;
		g = (100.0 * ldr_cor[GREEN]) / cinza;
		b = (100.0 * ldr_cor[BLUE]) / cinza;

		if (ldr_cor[RED] > ldr_cor[GREEN] && ldr_cor[RED] > ldr_cor[BLUE])
		{
			if ((r-g) < 20) cor = YELLOW;
			else cor = RED;
		}
		else if (ldr_cor[GREEN] > ldr_cor[RED] && ldr_cor[GREEN] > ldr_cor[BLUE])
		{
			cor = GREEN;
		}
		else cor = BLUE;
	}
	Serial.print("calculo =  ");
	Serial.print(ldr_cor[RED]);
	Serial.print(", ");
	Serial.print(ldr_cor[GREEN]);
	Serial.print(", ");
	Serial.println(ldr_cor[BLUE]);

	Serial.print("percent =  ");
	Serial.print(r);
	Serial.print(", ");
	Serial.print(g);
	Serial.print(", ");
	Serial.println(b);

	lcd.setCursor(0,1);
	lcd.print ("                ");
	lcd.setCursor(0,1);
	lcd.print (nome_cor[cor]);
	Serial.println(nome_cor[cor]);

	delay(1000);
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

	Serial.print("branco( ");
	Serial.print(ldr_branco[RED]);
	Serial.print(", ");
	Serial.print(ldr_branco[GREEN]);
	Serial.print(", ");
	Serial.print(ldr_branco[BLUE]);
	Serial.println(" )");


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

	Serial.print("preto( ");
	Serial.print(ldr_preto[RED]);
	Serial.print(", ");
	Serial.print(ldr_preto[GREEN]);
	Serial.print(", ");
	Serial.print(ldr_preto[BLUE]);
	Serial.println(" )");

	lcd.setCursor(13,1);	lcd.print (".");
	delay(200);
	lcd.setCursor(14,1);	lcd.print (".");
	delay(200);
	lcd.setCursor(15,1);	lcd.print (".");
	delay(200);
}
