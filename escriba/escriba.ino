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

int estado_menu;
int volume;
float carga;

int DIREITA = 1;
int ESQUERDA = 2;
int CIMA = 3;
int BAIXO = 4;
int SELECIONA = 5;

/* funcoes */

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

void init_menu() // Inicializa as vatiaveis do menu
{
	estado_menu = 0;
	volume = 5;
	lcd.begin(16, 2);
}

void menu() // Gerenciador do menu e suas opcoes
{
	int botao = verifica_botao();

	if (estado_menu < 10)	// Nivel 1
	{
		if (estado_menu == 0)	// Mascara
		{
			lcd.setCursor(0,0);
			lcd.print ("    MEGAZORD    ");
			lcd.setCursor(0,1);
			lcd.print ("                ");

			if (botao == BAIXO)
			{
				estado_menu = 1;
			}
			else if (botao == CIMA)
			{
				estado_menu = 3;
			}
			delay(200);
		}
		else if (estado_menu == 1)
		{
			lcd.setCursor(0,0);
			lcd.print ("    MEGAZORD    ");
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
				lcd.setCursor(1,1);
				lcd.print ("Iniciando em");
				for (int tempo=3; tempo>1; tempo--) {
					lcd.setCursor(13,1);
					lcd.print (tempo);
				}
				lcd.setCursor(0,1);
				lcd.print ("Morfando ...    ");
				estado_menu = 0;

				// delay(5000); // Aqui entra a função que vai desenhar a palavra
				motor_braco->setSpeed(150);
				motor_braco->run(FORWARD);

				delay(3000);

				motor_braco->setSpeed(150);
				motor_braco->run(BACKWARD);

				delay(3000);

				motor_braco->setSpeed(0);
			}
			delay(200);
		}
		else if (estado_menu == 2)
		{
			lcd.setCursor(0,0);
			lcd.print ("    MEGAZORD    ");
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
			delay(200);
		}
		else if (estado_menu == 3)
		{
			lcd.setCursor(0,0);
			lcd.print ("    MEGAZORD    ");
			lcd.setCursor(0,1);
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
			delay(200);
		}
		else if (estado_menu == 4)
		{
			lcd.setCursor(0,0);
			lcd.print ("    MEGAZORD    ");
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
				estado_menu = 41;
			}
			delay(200);
		}
	}
	else if (estado_menu < 30) // Opcao 2 - nivel 2
	{
		if (estado_menu == 21)
		{
			lcd.setCursor(0,0);
			lcd.print ("   CONFIGURAR   ");
			lcd.setCursor(0,1);
			lcd.print ("Volume  -     +");
			lcd.setCursor(11,1);
			lcd.print (volume);

			if (botao == BAIXO)
			{
				estado_menu = 22;
			}
			else if (botao == CIMA)
			{
				estado_menu = 22;
			}
			else if (botao == DIREITA)
			{
				if (volume < 9) volume++;
			}
			else if (botao == ESQUERDA)
			{
				if (volume > 0) volume--;
			}
			delay(200);
		}
		else if (estado_menu == 22)
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
				estado_menu = 21;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 0;
			}
			delay(200);
		}
	}
	else if (estado_menu < 40) // Opcao 3 - nivel 2
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
				estado_menu = 32;
			}
			else if (botao == DIREITA)
			{
				// if (volume < 9) volume++;
			}
			else if (botao == ESQUERDA)
			{
				// if (volume > 0) volume--;
			}
			delay(200);
		}
		else if (estado_menu == 32)
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
			delay(200);
		}
	}
	else if (estado_menu < 50) // Opcao 4 - nivel 2
	{
		if (estado_menu == 41)
		{
			lcd.setCursor(0,0);
			lcd.print ("    CREDITOS    ");
			lcd.setCursor(0,1);
			lcd.print ("Arthur P Silva  ");

			if (botao == BAIXO)
			{
				estado_menu = 42;
			}
			else if (botao == CIMA)
			{
				estado_menu = 45;
			}

			delay(200);
		}
		else if (estado_menu == 42)
		{
			lcd.setCursor(0,0);
			lcd.print ("    CREDITOS    ");
			lcd.setCursor(0,1);
			lcd.print ("Bernardo Sanches");

			if (botao == BAIXO)
			{
				estado_menu = 43;
			}
			else if (botao == CIMA)
			{
				estado_menu = 41;
			}

			delay(200);
		}
		else if (estado_menu == 43)
		{
			lcd.setCursor(0,0);
			lcd.print ("    CREDITOS    ");
			lcd.setCursor(0,1);
			lcd.print ("Gustavo Campos  ");

			if (botao == BAIXO)
			{
				estado_menu = 44;
			}
			else if (botao == CIMA)
			{
				estado_menu = 42;
			}

			delay(200);
		}
		else if (estado_menu == 44)
		{
			lcd.setCursor(0,0);
			lcd.print ("    CREDITOS    ");
			lcd.setCursor(0,1);
			lcd.print ("Matheus Santos  ");

			if (botao == BAIXO)
			{
				estado_menu = 45;
			}
			else if (botao == CIMA)
			{
				estado_menu = 43;
			}

			delay(200);
		}
		else if (estado_menu == 45)
		{
			lcd.setCursor(0,0);
			lcd.print ("    CREDITOS    ");
			lcd.setCursor(0,1);
			lcd.print ("Voltar          ");

			if (botao == BAIXO)
			{
				estado_menu = 41;
			}
			else if (botao == CIMA)
			{
				estado_menu = 44;
			}
			else if (botao == SELECIONA)
			{
				estado_menu = 0;
			}
			delay(200);
		}
	}
}

int escritor ()
{
	return 0;
}

/* funcoes utilizadas pelo arduino */

void setup()
{
	// put your setup code here, to run once:
	init_menu();
	AFMS.begin();
}

void loop()
{
	// put your main code here, to run repeatedly:
	menu();
}
