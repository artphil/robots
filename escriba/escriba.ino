/* bibliotecas */

#include <LiquidCrystal.h>
#include <Adafruit_MotorShield.h>

/* variaveis globais */

// Objeto Display
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Objeto Motor
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *motor_base = AFMS.getMotor(1);
Adafruit_DCMotor *motor_braco = AFMS.getMotor(2);

const String nome_robo = " MEGAZORD  v1.0 ";

const int DIREITA = 1;
const int ESQUERDA = 2;
const int CIMA = 3;
const int BAIXO = 4;
const int SELECIONA = 5;

const int T_MAX_MENU = 150;

const int MOVIMENTOS = 3;

int 	estado_menu;// Posicao na pilha do menu
int 	estado_liga;// Posicao na pilha do menu 11
int 	estado_motor;// Posicao na pilha de acoes do motor
int 	tdt;		// Tempo de Trabalho (unidade de tempo para cada acao do robo)
int 	potencia;	// Forca de Trabalho (aplicada no motor)
bool	ligado;		// Informa se o robo esta funcionando;
float 	carga;		// Tensão real da fonte
unsigned long tempo;// Contador do tempo corrente
unsigned long t_menu;// Contador de tempo do menu
unsigned long t_liga;// Contador de tempo do menu 11
unsigned long t_motor;// Contador de tempo do motor

int pilha_motor_base[MOVIMENTOS] = {0,0,0};
int pilha_motor_braco[MOVIMENTOS] = {0,1,-1};


/* funcoes */

void inicia() // Inicializa as vatiaveis
{
	estado_menu = 0;
	t_menu = 0;
	tdt = 3000;
	ligado = false;
	lcd.begin(16, 2);
	AFMS.begin(); // create with the default frequency 1.6KHz
}

void atualiza () // Atualiza as constantes utilizadas
{

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
		if (estado_menu == 0)	// Mascara
		{
			lcd.setCursor(0,0);
			lcd.print (nome_robo);
			lcd.setCursor(0,1);
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
			lcd.setCursor(0,0);
			lcd.print (nome_robo);
			lcd.setCursor(0,1);
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
			lcd.setCursor(0,0);
			lcd.print (nome_robo);
			lcd.setCursor(0,1);
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
			lcd.setCursor(0,0);
			lcd.print (nome_robo);
			lcd.setCursor(0,1);
			lcd.print ("Informacoes     ");

			if (botao == BAIXO)
			{
				estado_menu = 9;
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
		else if (estado_menu == 9)
		{
			lcd.setCursor(0,0);
			lcd.print (nome_robo);
			lcd.setCursor(0,1);
			lcd.print ("Creditos        ");

			if (botao == BAIXO)
			{
				estado_menu = 0;
			}
			else if (botao == CIMA)
			{
				estado_menu = 3;
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
			lcd.print (nome_robo);

			tempo = millis();
			if ((tempo - t_liga) > 1000)
			{
				t_liga = tempo;
				estado_liga += 1;
			}

			if (estado_liga < 4) {
				lcd.setCursor(0,1);
				lcd.print ("Iniciando em");
				lcd.setCursor(13,1);
				lcd.print (3 - estado_liga);
			}
			else {
				lcd.setCursor(0,1);
				lcd.print ("Morfando ...    ");
				ligado = true;
				estado_motor = -1;
			}
		}
	}
	else if (estado_menu < 30) // Configurar - nivel 2
	{
		if (estado_menu == 21)
		{
			lcd.setCursor(0,0);
			lcd.print ("   CONFIGURAR   ");
			lcd.setCursor(0,1);
			lcd.print ("  TDT  -       +");
			lcd.setCursor(9,1);
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
				if (tdt < 9) tdt += 10;
			}
			else if (botao == ESQUERDA)
			{
				if (tdt > 0) tdt -= 10;
			}
		}
		else if (estado_menu == 22)   //   IGNORADO
		{
			lcd.setCursor(0,0);
			lcd.print ("   CONFIGURAR   ");
			lcd.setCursor(0,1);
			lcd.print ("potenc -       +");
			lcd.setCursor(11,1);
			lcd.print (potencia);

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
				if (potencia < 255) potencia++;
			}
			else if (botao == ESQUERDA)
			{
				if (potencia > 0) potencia--;
			}
		}
		else if (estado_menu == 29)
		{
			lcd.setCursor(0,0);
			lcd.print ("   CONFIGURAR   ");
			lcd.setCursor(0,1);
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
				estado_menu = 0;
			}
		}
	}
	else if (estado_menu < 40) // Informacoes - nivel 2
	{
		if (estado_menu == 31)
		{
			lcd.setCursor(0,0);
			lcd.print ("  INFORMACOES  ");
			lcd.setCursor(0,1);
			lcd.print ("Bateria         ");
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
			lcd.setCursor(0,0);
			lcd.print ("  INFORMACOES  ");
			lcd.setCursor(0,1);
			lcd.print ("Clock         s");
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
			lcd.setCursor(0,0);
			lcd.print ("  INFORMACOES  ");
			lcd.setCursor(0,1);
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
				estado_menu = 0;
			}
		}
	}
	else if (estado_menu < 100) // Creditos - nivel 2
	{
		if (estado_menu == 91)
		{
			lcd.setCursor(0,0);
			lcd.print ("    CREDITOS    ");
			lcd.setCursor(0,1);
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
			lcd.setCursor(0,0);
			lcd.print ("    CREDITOS    ");
			lcd.setCursor(0,1);
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
			lcd.setCursor(0,0);
			lcd.print ("    CREDITOS    ");
			lcd.setCursor(0,1);
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
			lcd.setCursor(0,0);
			lcd.print ("    CREDITOS    ");
			lcd.setCursor(0,1);
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
			lcd.setCursor(0,0);
			lcd.print ("    CREDITOS    ");
			lcd.setCursor(0,1);
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
				estado_menu = 0;
			}
		}
	}
}

void aciona_motor (int m1, int m2)
{
	if (m1 > 0)
	{
		motor_base->setSpeed(potencia);
		motor_base->run(FORWARD);
	}
	else if (m1 < 0)
	{
		motor_base->setSpeed(potencia);
		motor_base->run(BACKWARD);
	}
	else
	{
		motor_base->setSpeed(0);
	}

	if (m2 > 0)
	{
		motor_braco->setSpeed(potencia);
		motor_braco->run(FORWARD);
	}
	else if (m2 < 0)
	{
		motor_braco->setSpeed(potencia);
		motor_braco->run(BACKWARD);
	}
	else
	{
		motor_braco->setSpeed(0);
	}
}

int escritor ()
{
	tempo = millis();
	if ((tempo - t_motor) > tdt)
	{
		t_motor = tempo;
		estado_motor++;
	}
	if (estado_motor < MOVIMENTOS) {
		aciona_motor(pilha_motor_base[estado_motor],pilha_motor_braco[estado_motor]);
	}
	else
	{
		ligado = false;
		estado_menu = 0;
		aciona_motor(0,0);
	}
	return 0;
}

/* funcoes utilizadas pelo arduino */

void setup()
{
	// put your setup code here, to run once:
	inicia();
	AFMS.begin();
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
