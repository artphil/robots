#include "ambiente.h"

/* implementação das funcoes */

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

int busca_luz(int n)
{
	int luz = diferenca_ldr();
	if (n == 0)
	{
		n += anda(ESQUERDA,4*t_giro_90);
		ldr_dif_max = (ldr_dif_max>luz) ? ldr_dif_max : luz;
	}
	else if (n == 1)
	{
		ldr_dif_max -= 10;
		n++;
	}
	else if (n == 2)
	{
		anda(DIREITA,4*t_giro_90);
		if (ldr_dif_max < luz) {
			aciona_motor(0,0);
			return 1;
		}
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
	else if (mov == 8)
	{
		if (busca_luz(0) == 1) 		ligado = false;
	}
}
