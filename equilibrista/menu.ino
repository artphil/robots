#include "ambiente.h"

/* funcoes */
int verifica_botao (); 		// Identifica qual botao foi acionado
void m_iniciar(int);		// Inicia processo de movimentos
void m_inicio(int); 		// MENU 		- nivel 1
void m_configurar(int); 	// Configurar 	- nivel 2
void m_conf_movimento(int);	// Configurar 	- Movimento - nivel 3
void m_conf_visao(int); 	// Configurar 	- Visao 	- nivel 3
void m_conf_odometro(int); 	// Configurar 	- Odometro 	- nivel 3
void m_informacoes(int); 	// Informacoes 	- nivel 2
void m_testes(int); 		// Testes 		- nivel 2
void m_pre_moves(int); 		// Pre moves 	- nivel 2
void m_creditos(int);	 	// Creditos 	- nivel 2

/* implementação das funcoes */

void menu() // Gerenciador do menu e suas opcoes
{
	if (estado_menu < 10) 		m_inicio(0);
	else if (estado_menu < 20)  m_iniciar(10);
	else if (estado_menu < 30)  m_configurar(20);
	else if (estado_menu < 40)  m_informacoes(30);
	else if (estado_menu < 50)  m_testes(40);
	else if (estado_menu < 60)  m_pre_moves(50);
	else if (estado_menu < 70)  m_creditos(60);
	else if (estado_menu < 30)  m_configurar(20);
	else if (estado_menu < 220) m_conf_movimento(210);
	else if (estado_menu < 230) m_conf_visao(220);
	else if (estado_menu < 240) m_conf_odometro(230);
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

	if (t_menu.fim())
	{
		t_menu.reset();
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
			t_liga.reset();
			estado_liga = 0;
		}
		estado_menu = (estado_menu*10)+1;
		break;
	}
}

void m_iniciar(int n)	// Inicia processo de movimentos
{
	lcd.setCursor(0,0);
	lcd.print (NOME_ROBO);
	lcd.setCursor(0,1);

	if (t_liga.fim())
	{
		t_liga.reset();
		estado_liga += 1;
	}

	if (estado_liga == 0)
	{
		if (mov == 2) e_teste();
		else if (mov == 3) e_vai_vem();
		else if (mov == 4) e_triangulo();
		else if (mov == 5) e_quadrado();
		else if (mov == 6) e_giro90();
		estado_liga = 1;
	}
	else if (estado_liga < 5)
	{
		lcd.print ("Iniciando em    ");
		lcd.setCursor(13,1);
		lcd.print (4 - estado_liga);
	}
	else if (estado_liga < 6)
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
		}
	}
	else if (estado_liga > 10) estado_menu = 0;
}

void m_configurar(int n) // Configurar - nivel 2
{
	String titulo = "   CONFIGURAR   ";
	int ns = 6;
	String subtitulo[ns] = {
		"Voltar          ",
		"Movimento       ",
		"Visao           ",
		"Odometro        ",
		"     GRAVAR     ",
		"    LE SALVO    "
	};

	if (t_menu.fim())
	{
		t_menu.reset();
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
		else if (estado_menu == n+4)
		{
			grava_EEPROM();
			estado_menu = (estado_menu/10);
		}
		else if (estado_menu == n+5)
		{
			le_EEPROM();
			estado_menu = (estado_menu/10);
		}
		else
		{
			estado_menu = (estado_menu*10)+1;
		}
		break;
	}
}

void m_conf_movimento(int n) // Configurar - Movimento - nivel 3
{
	String titulo = "    MOVIMENTO   ";
	int ns = 6;
	String subtitulo[ns] = {
		"Voltar          ",
		"pot ME -       +",
		"pot MD -       +",
		"anda   -     cm+",
		"f anda -       +",
		"giro90 -       +"
	};

	if (t_menu.fim())
	{
		t_menu.reset();
		botao = verifica_botao();
	}

	lcd.setCursor(0,0);
	lcd.print (titulo);
	lcd.setCursor(0,1);
	lcd.print (subtitulo[estado_menu%10]);
	lcd.setCursor(10,1);

	if (estado_menu == n+1) lcd.print (pot_motor_D);
	else if (estado_menu == n+2) lcd.print (pot_motor_E);
	else if (estado_menu == n+3) lcd.print (anda_cm);
	else if (estado_menu == n+4) lcd.print (anda_fat);
	else if (estado_menu == n+5) lcd.print (t_giro_90);

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
			if (anda_cm < 255) anda_cm++;
			t_anda = anda_cm * anda_fat;
		}
		else if (estado_menu == n+4)
		{
			if (anda_fat < 255) anda_fat++;
		}
		else if (estado_menu == n+5)
		{
			if (t_giro_90 < 255)
			{
				t_giro_90++;
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
			if (anda_cm < 255) anda_cm--;
			t_anda = anda_cm * anda_fat;
		}
		else if (estado_menu == n+4)
		{
			if (anda_fat < 255) anda_fat--;
		}
		else if (estado_menu == n+5)
		{
			if (t_giro_90 > 0)
			{
				t_giro_90--;
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

void m_conf_visao(int n) // Configurar - Visao - nivel 2
{
	String titulo = "      VISAO     ";
	int ns = 4;
	String subtitulo[ns] = {
		"Voltar          ",
		"Balanco branco  ",
		"Balanco preto   ",
		"LDRlmt -       +"
	};

	if (t_menu.fim())
	{
		t_menu.reset();
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

void m_conf_odometro(int n) // Configurar - Odometro - nivel 3
{
	String titulo = "    ODOMETRO    ";
	int ns = 4;
	String subtitulo[ns] = {
		"Voltar          ",
		"K P    -       +",
		"K D    -       +",
		"Calibra motor   "
	};

	if (t_menu.fim())
	{
		t_menu.reset();
		botao = verifica_botao();
	}

	lcd.setCursor(0,0);
	lcd.print (titulo);
	lcd.setCursor(0,1);
	lcd.print (subtitulo[estado_menu%10]);
	lcd.setCursor(10,1);

	if (estado_menu == n+1) lcd.print (KP);
	else if (estado_menu == n+2) lcd.print (KD);

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
			// if (KP < 255)
			KP += 0.1;
		}
		else if (estado_menu == n+2)
		{
			// if (KD < 255)
			KD += 0.1;
		}
		break;

		case ESQUERDA:
		if (estado_menu == n+1)
		{
			if (KP > 0) KP -= 0.1;
		}
		else if (estado_menu == n+2)
		{
			if (KD > 0) KD -= 0.1;
		}
		break;

		case SELECIONA:
		if (estado_menu == n)
		{
			estado_menu = (estado_menu/10);
		}
		else if (estado_menu == n+3)
		{
			calibra_motor();
			estado_menu = (estado_menu/10);
		}
		break;
	}

}

void m_informacoes(int n) // Informacoes - nivel 2
{
	String titulo = "  INFORMACOES  ";
	int ns = 8;
	String subtitulo[ns] =
	{
		"Voltar          ",
		"LDR2      |     ",
		"LDR dir         ",
		"R    G    B     ",
		"LDR             ",
		"Objeto          ",
		"Bateria        v",
		"Tempo          s",
	};

	if (t_menu.fim())
	{
		t_menu.reset();
		botao = verifica_botao();
	}

	lcd.setCursor(0,0);
	lcd.print (titulo);
	lcd.setCursor(0,1);
	lcd.print (subtitulo[estado_menu%10]);
	lcd.setCursor(10,1);

	if (estado_menu == n+1)
	{
		lcd.setCursor(5,1);
		lcd.print (ldr_esquerda);
		lcd.setCursor(11,1);
		lcd.print (ldr_direita);
	}
	else if (estado_menu == n+2)
	{
		switch (ldr_direcao)
		{
			case DIREITA:
			lcd.print ("Direita");
			break;

			case ESQUERDA:
			lcd.print ("Esquerda");
			break;

			default:
			lcd.print (ldr_direcao);
			break;
		}
	}
	else if (estado_menu == n+3)
	{
		lcd.setCursor(2,1);
		lcd.print (ldr_cor[RED]);
		lcd.setCursor(7,1);
		lcd.print (ldr_cor[GREEN]);
		lcd.setCursor(12,1);
		lcd.print (ldr_cor[BLUE]);
	}
	else if (estado_menu == n+4) lcd.print (ldr_valor);
	else if (estado_menu == n+5) objeto ? lcd.print ("SIM") : lcd.print ("NAO");
	else if (estado_menu == n+6) lcd.print (carga);
	else if (estado_menu == n+7) lcd.print (millis());

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
		else if (estado_menu == n+2) diferenca_ldr();
		else if (estado_menu == n+3) ve_cor();
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

	if (t_menu.fim())
	{
		t_menu.reset();
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
			t_liga.reset();
			estado_liga = 0;
		}
		break;
	}
}

void m_pre_moves(int n) // Pre moves - nivel 2
{
	String titulo = "   PRE  MOVES   ";
	int ns = 7;
	String subtitulo[ns] =
	{
		"Voltar          ",
		"Vai Vem         ",
		"Triangulo       ",
		"Quadrado        ",
		"Giro 90         ",
		"Busca Luz       ",
		"Linha           "
	};

	if (t_menu.fim())
	{
		t_menu.reset();
		botao = verifica_botao();
	}

	lcd.setCursor(0,0);
	lcd.print (titulo);
	lcd.setCursor(0,1);
	lcd.print (subtitulo[estado_menu%10]);
	mov = (estado_menu%10)+2;  // define a acao do robo

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
			t_liga.reset();
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

	if (t_menu.fim())
	{
		t_menu.reset();
		botao = verifica_botao();
	}

	lcd.setCursor(0,0);
	lcd.print (titulo);

	if (desliza && estado_menu != n)
	{
		if (t_nome.fim())
		{
			t_nome.reset();
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
