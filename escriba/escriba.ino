/* bibliotecas */

#include <LiquidCrystal.h>
#include <Adafruit_MotorShield.h>
using namespace std;

/* classes */

class entrada
{
	int tam = 50;
	int i;
	int valor[50][2];
public:
	void set_tam(int n);
	int  get_tam();
	void reset();
	void set(int n, int m);
	int  get_v1(int n);
	int  get_v2(int n);
};
void entrada::set_tam(int n)
{ tam = n; }
int entrada::get_tam()
{ return tam; }
void entrada::reset()
{ i = 0; }
void entrada::set(int n, int m)
{	valor[i][0] = n;	valor[i][1] = m;	i++; }
int entrada::get_v1(int n)
{ return valor[n][0]; }
int entrada::get_v2(int n)
{ return valor[n][1]; }

/* variaveis globais */

// Objeto Display
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Objeto Motor
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *motor_base = AFMS.getMotor(3);
Adafruit_DCMotor *motor_braco = AFMS.getMotor(4);

#define NOME_ROBO 	" MEGAZORD  v1.0 "

#define DIREITA 	1
#define ESQUERDA 	2
#define CIMA 		3
#define BAIXO 		4
#define SELECIONA 	5

#define T_MAX_MENU 	180

int 	estado_menu;	// Posicao na pilha do menu
int 	estado_liga;	// Posicao na pilha do menu 11
int 	estado_motor;	// Posicao na pilha de acoes do motor
int 	tdt;			// Tempo de Trabalho (unidade de tempo para cada acao do robo)
int 	pot_motor_base;	// Forca de Trabalho (aplicada no motor)
int 	pot_motor_braco;// Forca de Trabalho (aplicada no motor)
int 	txt;			// Texto a ser escrito;
bool	ligado;			// Informa se o robo esta funcionando;
float 	carga;			// Tensão real da fonte
unsigned long tempo;	// Contador do tempo corrente
unsigned long t_menu;	// Contador de tempo do menu
unsigned long t_liga;	// Contador de tempo do menu 11
unsigned long t_motor;	// Contador de tempo do motor

/* sequencia de movimentos doa Motores */

entrada ufmg;
entrada ufmg2;
entrada teste;
entrada miss;
entrada coracao;

/* lista de funcoes utilizadas */

void 	inicia();
// Inicializa as vatiaveis
void 	cria_entradas();
// Inicializa as entradas
void 	atualiza ();
// Atualiza as constantes utilizadas
int 	verifica_botao ();
// Identifica qual botao foi acionado
void 	menu();
// Gerenciador do menu e suas opcoes
void 	aciona_motor (int m1, int m2);
// Contola o acionamento dos motores
int 	escritor ();
// Gerencia os movimentos da escrita
int 	escreve (entrada a);
// Desenha o vetor

/* funcoes utilizadas pelo arduino */

void setup()
{
	// put your setup code here, to run once:
	inicia();
}

void loop()
{
	// put your main code here, to run repeatedly:
	atualiza();
	menu();
	if (ligado)
	{
		escritor();
	}
}

/* implementação das funcoes */

void inicia() // Inicializa as vatiaveis
{
	// Parametros iniciais
	estado_menu = 0;
	txt = 0;
	t_menu = 0;
	pot_motor_base = 200;
	pot_motor_braco = 50;
	tdt = 1500;
	ligado = false;
	lcd.begin(16, 2);
	AFMS.begin(); // create with the default frequency 1.6KHz
	init_entradas ();
}

void init_entradas ()
{
		ufmg.set_tam(44);
		ufmg.reset();
        // U
		ufmg.set( 0, 1 );		ufmg.set( 0, 1 );		ufmg.set( 0, 1 );
		ufmg.set( 1, 0 );		ufmg.set( 1, 0 );		ufmg.set( 0,-1 );
		ufmg.set( 0,-1 );		ufmg.set( 0,-1 );
        //
		ufmg.set( 1, 0 );
        // F
		ufmg.set( 0, 1 );		ufmg.set( 0, 1 );		ufmg.set( 0, 1 );
		ufmg.set( 0,-1 );		ufmg.set( 0,-1 );		ufmg.set( 1, 0 );
		ufmg.set(-1, 0 );		ufmg.set( 0,-1 );		ufmg.set( 1, 0 );
		ufmg.set( 1, 0 );
        // M
		ufmg.set( 0, 1 );		ufmg.set( 0, 1 );		ufmg.set( 0, 1 );
		ufmg.set( 0,-1 );		ufmg.set( 0,-1 );		ufmg.set( 0,-1 );
		ufmg.set( 1, 1 );		ufmg.set( 1,-1 );		ufmg.set( 0, 1 );
		ufmg.set( 0, 1 );		ufmg.set( 0, 1 );
        //
		ufmg.set( 1, 0 );
        // G
		ufmg.set( 1, 0 );		ufmg.set( 1, 0 );		ufmg.set( 0,-1 );
		ufmg.set(-1, 0 );		ufmg.set( 1, 0 );		ufmg.set( 0, 1 );
		ufmg.set(-1, 0 );		ufmg.set(-1, 0 );		ufmg.set( 0,-1 );
		ufmg.set( 0,-1 );		ufmg.set( 0,-1 );		ufmg.set( 1, 0 );
		ufmg.set( 1, 0 );

		teste.set_tam(4);
		teste.reset();
		teste.set( 0, 1 );		teste.set( 1, 0 );		teste.set( 0,-1 );
		teste.set(-1, 0 );   	teste.set( 1, 1 );		teste.set(-1,-1 );

		miss.set_tam(40);
		miss.reset();
		// M
		miss.set( 0, 1 );		miss.set( 0, 1 );		miss.set( 0, 1 );
		miss.set( 0,-1 );		miss.set( 0,-1 );		miss.set( 0,-1 );
		miss.set( 1, 1 ); 		miss.set( 1,-1 );      	miss.set( 0, 1 );
		miss.set( 0, 1 );		miss.set( 0, 1 );
        //
		miss.set( 1, 0 );
        // I
		miss.set( 0,-1 );		miss.set( 0,-1 );		miss.set( 0,-1 );
		miss.set( 0, 1 );		miss.set( 0, 1 );		miss.set( 0, 1 );
		//
		miss.set( 1, 0 );
        // S
		miss.set( 1, 0 );		miss.set( 1, 0 );		miss.set( 0,-1 );
		miss.set(-1, 0 );		miss.set(-1, 0 );		miss.set( 0,-1 );
		miss.set( 0,-1 );		miss.set( 1, 0 );		miss.set( 1, 0 );
        //
		miss.set( 1, 0 );
        // S
		miss.set( 1, 0 );		miss.set( 1, 0 );		miss.set(-1, 0 );
		miss.set(-1, 0 );		miss.set( 0, 1 );		miss.set( 0, 1 );
		miss.set( 1, 0 );		miss.set( 1, 0 );		miss.set( 0, 1 );
		miss.set(-1, 0 );		miss.set(-1, 0 );

		coracao.set_tam(41);
		coracao.reset();
		coracao.set( 1, 0 );		coracao.set( 1, 0 );		coracao.set( 1, 0 );
		coracao.set( 1, 0 );		coracao.set( 0, 1 );		coracao.set( 1, 0 );
		coracao.set( 0, 1 );		coracao.set( 1, 0 );		coracao.set( 0,-1 );
		coracao.set( 1, 0 );		coracao.set( 0,-1 );		coracao.set( 1, 0 );
		coracao.set( 0, 1 );		coracao.set( 1, 0 );		coracao.set( 0, 1 );
		coracao.set( 1, 0 );		coracao.set( 0, 1 );		coracao.set( 0, 1 );
		coracao.set(-1, 0 );		coracao.set( 0, 1 );		coracao.set(-1, 0 );
		coracao.set( 0, 1 );		coracao.set(-1, 0 );		coracao.set( 0, 1 );
		coracao.set(-1, 0 );		coracao.set( 0, 1 );		coracao.set(-1, 0 );
		coracao.set( 0,-1 );		coracao.set(-1, 0 );		coracao.set( 0,-1 );
		coracao.set(-1, 0 );		coracao.set( 0,-1 );		coracao.set(-1, 0 );
		coracao.set( 0,-1 );		coracao.set(-1, 0 );		coracao.set( 0,-1 );
		coracao.set( 0,-1 );		coracao.set( 1, 0 );		coracao.set( 0,-1 );
		coracao.set( 1, 0 );		coracao.set( 0,-1 );

		ufmg2.set_tam(42);
		ufmg2.reset();
		// u
		ufmg2.set( 1, 0 );		ufmg2.set( 0, 1 );		ufmg2.set( 0, 1 );
		ufmg2.set( 1, 0 );		ufmg2.set( 1, 0 );		ufmg2.set( 0,-1 );
		ufmg2.set( 0,-1 );		ufmg2.set( 0, 1 );		ufmg2.set( 1, 1 );
        // f
		ufmg2.set( 0,-1 );		ufmg2.set( 1,-1 );		ufmg2.set( 1, 0 );
		ufmg2.set(-1, 0 );		ufmg2.set(-1, 1 );		ufmg2.set( 0, 1 );
		ufmg2.set( 0, 1 );		ufmg2.set(-1, 1 );		ufmg2.set( 1,-1 );
		ufmg2.set( 0,-1 );		ufmg2.set( 1, 0 );		ufmg2.set( 1, 0 );
		// m
		ufmg2.set( 0,-1 );		ufmg2.set( 1,-1 );		ufmg2.set( 1, 1 );
		ufmg2.set( 0, 1 );		ufmg2.set( 0,-1 );		ufmg2.set( 1,-1 );
		ufmg2.set( 1, 1 );		ufmg2.set( 0, 1 );		ufmg2.set( 1, 0 );
        // g
		ufmg2.set( 1, 0 );		ufmg2.set( 1, 0 );		ufmg2.set(-1, 0 );
		ufmg2.set(-1, 0 );		ufmg2.set( 0,-1 );		ufmg2.set( 1,-1 );
		ufmg2.set( 1, 0 );		ufmg2.set( 0, 1 );		ufmg2.set( 0, 1 );
		ufmg2.set( 0, 1 );		ufmg2.set(-1, 1 );		ufmg2.set(-1, 0 );

}

void atualiza () // Atualiza as constantes utilizadas
{
	carga = 5.0;
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
	tempo = millis();
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

			if (botao == BAIXO)
			{
				estado_menu = 1;
			}
			else if (botao == CIMA)
			{
				estado_menu = 9;
			}
		}
		else if (estado_menu == 1)
		{
			lcd.print ("Iniciar         ");

			if (botao == BAIXO)
			{
				estado_menu = 2;
			}
			else if (botao == CIMA)
			{
				estado_menu = 0;
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

			if (botao == BAIXO)
			{
				estado_menu = 3;
			}
			else if (botao == CIMA)
			{
				estado_menu = 1;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 21;
			}
		}
		else if (estado_menu == 3)
		{
			lcd.print ("Informacoes     ");

			if (botao == BAIXO)
			{
				estado_menu = 4;
			}
			else if (botao == CIMA)
			{
				estado_menu = 2;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 31;
			}
		}
		else if (estado_menu == 4)
		{
			lcd.print ("Testes          ");

			if (botao == BAIXO)
			{
				estado_menu = 9;
			}
			else if (botao == CIMA)
			{
				estado_menu = 3;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 41;
			}
		}
		else if (estado_menu == 9)
		{
			lcd.print ("Creditos        ");

			if (botao == BAIXO)
			{
				estado_menu = 0;
			}
			else if (botao == CIMA)
			{
				estado_menu = 4;
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

			if (botao == BAIXO)
			{
				estado_menu = 22;
			}
			else if (botao == CIMA)
			{
				estado_menu = 29;
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
			lcd.print (pot_motor_base);

			if (botao == BAIXO)
			{
				estado_menu = 23;
			}
			else if (botao == CIMA)
			{
				estado_menu = 21;
			}
			else if (botao == DIREITA)
			{
				if (pot_motor_base < 255) pot_motor_base++;
			}
			else if (botao == ESQUERDA)
			{
				if (pot_motor_base > 0) pot_motor_base--;
			}
		}
		else if (estado_menu == 23)
		{
			lcd.print ("pot m2 -       +");
			lcd.setCursor(10,1);
			lcd.print (pot_motor_braco);

			if (botao == BAIXO)
			{
				estado_menu = 29;
			}
			else if (botao == CIMA)
			{
				estado_menu = 21;
			}
			else if (botao == DIREITA)
			{
				if (pot_motor_braco < 255) pot_motor_braco++;
			}
			else if (botao == ESQUERDA)
			{
				if (pot_motor_braco > 0) pot_motor_braco--;
			}
		}
		else if (estado_menu == 29)
		{
			lcd.print ("Voltar          ");

			if (botao == BAIXO)
			{
				estado_menu = 21;
			}
			else if (botao == CIMA)
			{
				estado_menu = 22;
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

			if (botao == BAIXO)
			{
				estado_menu = 32;
			}
			else if (botao == CIMA)
			{
				estado_menu = 39;
			}

		}
		else if (estado_menu == 32)
		{
			lcd.print ("Tempo          s");
			lcd.setCursor(7,1);
			lcd.print (millis());

			if (botao == BAIXO)
			{
				estado_menu = 39;
			}
			else if (botao == CIMA)
			{
				estado_menu = 31;
			}

		}
		else if (estado_menu == 39)
		{
			lcd.print ("Voltar          ");

			if (botao == BAIXO)
			{
				estado_menu = 31;
			}
			else if (botao == CIMA)
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

			if (botao == BAIXO)
			{
				estado_menu = 42;
			}
			else if (botao == CIMA)
			{
				estado_menu = 49;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 411;
			}
		}
		else if (estado_menu == 42)
		{
			lcd.print ("E  Motor Base  D");
			lcd.setCursor(10,1);

			if (botao == BAIXO)
			{
				estado_menu = 43;
			}
			else if (botao == CIMA)
			{
				estado_menu = 41;
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
			lcd.print ("C  Motor Braco B");
			lcd.setCursor(10,1);

			if (botao == BAIXO)
			{
				estado_menu = 44;
			}
			else if (botao == CIMA)
			{
				estado_menu = 42;
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
			lcd.print ("      MISS      ");

			if (botao == BAIXO)
			{
				estado_menu = 45;
			}
			else if (botao == CIMA)
			{
				estado_menu = 43;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 441;
			}
		}
		else if (estado_menu == 45)
		{
			lcd.print ("    CORACAO     ");

			if (botao == BAIXO)
			{
				estado_menu = 46;
			}
			else if (botao == CIMA)
			{
				estado_menu = 44;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 451;
			}
		}
		else if (estado_menu == 46)
		{
			lcd.print ("      ufmg      ");

			if (botao == BAIXO)
			{
				estado_menu = 49;
			}
			else if (botao == CIMA)
			{
				estado_menu = 45;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 461;
			}
		}
		else if (estado_menu == 49)
		{
			lcd.print ("Voltar          ");

			if (botao == BAIXO)
			{
				estado_menu = 41;
			}
			else if (botao == CIMA)
			{
				estado_menu = 45;
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

			if (botao == BAIXO)
			{
				estado_menu = 92;
			}
			else if (botao == CIMA)
			{
				estado_menu = 99;
			}

		}
		else if (estado_menu == 92)
		{
			lcd.print ("Bernardo Sanches");

			if (botao == BAIXO)
			{
				estado_menu = 93;
			}
			else if (botao == CIMA)
			{
				estado_menu = 91;
			}
		}
		else if (estado_menu == 93)
		{
			lcd.print ("Gustavo Campos  ");

			if (botao == BAIXO)
			{
				estado_menu = 94;
			}
			else if (botao == CIMA)
			{
				estado_menu = 92;
			}
		}
		else if (estado_menu == 94)
		{
			lcd.print ("Matheus Santos  ");

			if (botao == BAIXO)
			{
				estado_menu = 99;
			}
			else if (botao == CIMA)
			{
				estado_menu = 93;
			}
		}
		else if (estado_menu == 99)
		{
			lcd.print ("Voltar          ");

			if (botao == BAIXO)
			{
				estado_menu = 91;
			}
			else if (botao == CIMA)
			{
				estado_menu = 94;
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
				txt 	= 2;
				estado_motor = -1;
			}
			estado_menu = 0;
		}
		else if (estado_menu == 441)
		{
			if (!ligado) {
				ligado 	= true;
				txt 	= 3;
				estado_motor = -1;
			}
			estado_menu = 0;
		}
		else if (estado_menu == 451)
		{
			if (!ligado) {
				ligado 	= true;
				txt 	= 4;
				estado_motor = -1;
			}
			estado_menu = 0;
		}
		else if (estado_menu == 461)
		{
			if (!ligado) {
				ligado 	= true;
				txt 	= 5;
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
		motor_base->setSpeed(pot_motor_base);
		motor_base->run(FORWARD);
	}
	else if (m1 < 0)
	{
		motor_base->setSpeed(pot_motor_base);
		motor_base->run(BACKWARD);
	}
	else
	{
		motor_base->run(RELEASE);
	}

	if (m2 > 0)
	{
		motor_braco->setSpeed(pot_motor_braco);
		motor_braco->run(FORWARD);
	}
	else if (m2 < 0)
	{
		motor_braco->setSpeed(pot_motor_braco);
		motor_braco->run(BACKWARD);
	}
	else
	{
		motor_braco->run(RELEASE);
	}
}

int escritor ()
{
	if (txt == 0) return 0;
	if (txt == 1)
	{
		escreve(ufmg);
	}
	else if (txt == 2)
	{
		escreve(teste);
	}
	else if (txt == 3)
	{
		escreve(miss);
	}
	else if (txt == 4)
	{
		escreve(coracao);
	}
	else if (txt == 5)
	{
		escreve(ufmg2);
	}
}

int escreve (entrada a)
{
	tempo = millis();
	if ((tempo - t_motor) > tdt)
	{
		t_motor = tempo;
		estado_motor++;
	}

	if (estado_motor < a.get_tam()) {
		aciona_motor(a.get_v1(estado_motor),a.get_v2(estado_motor));
	}
	else
	{
		ligado 	= false;
		txt     = 0;
		aciona_motor(0,0);
	}
	return 0;
}
