#include "ambiente.h"

/* implementação das funcoes */

void inicia_motor() // Inicializa as vatiaveis
{
	// Parametros iniciais
	t_teste 	= 2000;
	pot_motor_D = 200;
	pot_motor_E = 200;
	anda_cm  	= 30;
	anda_fat  	= 1670;
	t_anda  	= 5000;
	t_anda_raiz = sqrt(t_anda);
	t_giro_90  	= 2400;
	t_giro_45   = t_giro_90/2;
	ligado = false;
	AFMS.begin(); // create with the default frequency 1.6KHz
}

void aciona_motor (int m1, int m2)
{
	// Se os motores estao em direções diferentes o movimento
	// e um giro
	mov_giro = (m1 == m2)? false : true;

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
	// Serial.print("Direcao: ");
	// Serial.println(direcao);
	// Serial.print("Tempo: ");
	// Serial.println(tmp_acao);

	t_motor.set_max(tmp_acao);

	if (t_motor.get_milis() < tmp_acao)
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
		else  if (direcao == PARA)
		{
			aciona_motor(0,0);
			lcd.print ("PARA           ");
			return 1;
		}
		else
		{
			aciona_motor(0,0);
			return 1;
		}

		lcd.setCursor(12,1);
		lcd.print (t_motor.get_seg());
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
		t_motor.reset();
	}
	// Serial.print("Estado: ");
	// Serial.println(estado_motor);

	return 0;
}

int move_auto()
{
	if (estado_motor > 1 && t_partida.fim())
	{
		return 1;
	}

	if (objeto) estado_motor = 3;

	switch (estado_motor) {
		case 0:	// Espera o inicio
		if (analogRead(LDR_S_PIN) < 100)
		{
			estado_motor = 1;
			t_motor.reset();
			t_partida.reset();
			luz = 0;
		}
		break;

		case 1: // Procura Posicao inicial
		if (busca_luz(luz) == 1)
		{
			while (anda(DIREITA,t_giro_90) != 1); 	// gira 90 graus
			estado_motor = 2;
		}
		break;

		case 2: // Analiza cor do objeto

		estado_motor = 3;
		break;
/*
		case 1: // Analiza cor do objeto
		if (anda(PARA,T_MAX_ANDAR) == 1) return 1; 	// Acaba o tempo
		ve_cor();
		if (cor == RED)
		{
			estado_motor = 2;
			t_motor.reset();
		}
		if (cor == GREEN)
		{
			estado_motor = 3;
			t_motor.reset();
		}
		if (cor == BLUE)
		{
			estado_motor = 4;
			t_motor.reset();
		}
		if (cor == YELLOW)
		{
			estado_motor = 5;
			t_motor.reset();
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
		t_motor.reset();
		while (anda(DIREITA,t_giro_90) != 1); 	// gira 90 graus
		t_motor.reset();
		while (anda(DIREITA,t_giro_90) != 1); 	// gira 90 graus
		t_motor.reset();
		while (anda(DIREITA,t_giro_90) != 1); 	// gira 90 graus
		t_motor.reset();
		while (anda(DIREITA,t_giro_90) != 1); 	// gira 90 graus
		t_motor.reset();
		return 1;
		break;

		case 3:
		while (anda(TRAS,t_anda_re) != 1); 		// anda para tras
		t_motor.reset();
		while (anda(ESQUERDA,t_giro_90) != 1); 	// gira 90 graus
		estado_motor = 0;
		t_motor.reset();
		break;

		case 4:
		while (anda(TRAS,t_anda_re) != 1); 		// anda para tras
		t_motor.reset();
		while (anda(DIREITA,t_giro_90) != 1); 	// gira 90 graus
		estado_motor = 0;
		t_motor.reset();
		break;

		case 5:
		while (anda(TRAS,t_anda_re) != 1); 		// anda para tras
		t_motor.reset();
		while (anda(ESQUERDA,t_giro_90) != 1); 	// gira 90 graus
		t_motor.reset();
		while (anda(ESQUERDA,t_giro_90) != 1); 	// gira 90 graus
		estado_motor = 0;
		t_motor.reset();
		break;
		/*/
	}
	return 0;
}

int busca_luz(int n)
{
	int luzeiro = diferenca_ldr();
	if (n == 0)
	{
		n += anda(ESQUERDA,4*t_giro_90); 		// gira 360 graus
		ldr_dif_max = (ldr_dif_max>luzeiro) ? ldr_dif_max : luzeiro;
	}
	else if (n == 1)
	{
		ldr_dif_max -= 10;
		n++;
	}
	else if (n == 2)
	{
		anda(DIREITA,4*t_giro_90);		// gira ate encontra maior luz
		if (ldr_dif_max < luzeiro) {
			aciona_motor(0,0);
			return 1;
		}
	}
	return 0;
}

int linha()
{
	if (verifica_botao() == SELECIONA)
	{
		aciona_motor(0,0);
		return 1;
	}
	diferenca_otico();
	anda(otico_direcao,t_anda);
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
		if (move(seq_mov) == 1) 		ligado = false;
	}
	else if (mov == 3)
	{
		if (move(seq_mov) == 1) 	ligado = false;
	}
	else if (mov == 4)
	{
		if (move(seq_mov) == 1) 	ligado = false;
	}
	else if (mov == 5)
	{
		if (move(seq_mov) == 1) 	ligado = false;
	}
	else if (mov == 6)
	{
		if (move(seq_mov) == 1) 		ligado = false;
	}
	else if (mov == 7)
	{
		if (busca_luz(luz) == 1) 		ligado = false;
	}
	else if (mov == 8)
	{
		if (linha() == 1) 		ligado = false;
	}
}

void 	e_teste()
{
	seq_mov.set_tam(8);
	seq_mov.reset();
	seq_mov.set( FRENTE,   t_teste );
	seq_mov.set( PARA, 	 t_teste );
	seq_mov.set( TRAS,     t_teste );
	seq_mov.set( PARA,   	 t_teste );
	seq_mov.set( DIREITA,  t_teste );
	seq_mov.set( PARA,   	 t_teste );
	seq_mov.set( ESQUERDA, t_teste );
	seq_mov.set( PARA,   	 t_teste );
	seq_mov.print();
}

void 	e_vai_vem()
{
	seq_mov.set_tam(6);
	seq_mov.reset();
	seq_mov.set( FRENTE,  	t_anda );
	seq_mov.set( ESQUERDA, 	t_giro_90 );
	seq_mov.set( ESQUERDA, 	t_giro_90 );
	seq_mov.set( FRENTE,  	t_anda );
	seq_mov.set( DIREITA, 	t_giro_90 );
	seq_mov.set( DIREITA, 	t_giro_90 );
	seq_mov.print();
}

void 	e_triangulo()
{
	seq_mov.set_tam(8);
	seq_mov.reset();
	seq_mov.set( FRENTE,  t_anda );
	seq_mov.set( DIREITA, t_giro_90 );
	seq_mov.set( FRENTE,  t_anda );
	seq_mov.set( DIREITA, t_giro_90 );
	seq_mov.set( DIREITA, t_giro_45 );
	seq_mov.set( FRENTE,  t_anda_raiz);
	seq_mov.set( DIREITA, t_giro_90 );
	seq_mov.set( DIREITA, t_giro_45 );
	seq_mov.print();
}

void 	e_quadrado()
{
	seq_mov.set_tam(8);
	seq_mov.reset();
	seq_mov.set( FRENTE,  t_anda );
	seq_mov.set( DIREITA, t_giro_90 );
	seq_mov.set( FRENTE,  t_anda );
	seq_mov.set( DIREITA, t_giro_90 );
	seq_mov.set( FRENTE,  t_anda );
	seq_mov.set( DIREITA, t_giro_90 );
	seq_mov.set( FRENTE,  t_anda );
	seq_mov.set( DIREITA, t_giro_90 );
	seq_mov.print();
}

void 	e_giro90()
{
	seq_mov.set_tam(3);
	seq_mov.reset();
	seq_mov.set( DIREITA,  t_giro_90 );
	seq_mov.set( PARA,     t_giro_90 );
	seq_mov.set( ESQUERDA, t_giro_90 );
	seq_mov.print();
}
