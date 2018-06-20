#include "ambiente.h"

/* implementação das funcoes */

void inicia_motor() // Inicializa as vatiaveis
{
	// Parametros iniciais
	t_teste 	= 2000;
	pot_motor_D = 150;
	pot_motor_E = 150;
	pot_motor_F = 100;
	anda_cm  	= 30;
	anda_fat  	= 100;
	t_anda  	= anda_cm*anda_fat; // 3000
	t_anda_raiz = sqrt(t_anda);
	t_anda_re	= 1000;
	t_giro_90  	=1500;
	t_giro_45   = t_giro_90/2;
	ligado = false;
	AFMS.begin(); // create with the default frequency 1.6KHz
}

void aciona_motor (int m1, int m2)
{
	// Se os motores estao em direções diferentes o movimento
	// eh um giro
	// mov_giro = (m1 == m2)? false : true;


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

void aciona_motor_F (int m1)
{

	if (m1 > 0)
	{
		motor_F->setSpeed(pot_motor_D);
		motor_F->run(FORWARD);
	}
	else if (m1 < 0)
	{
		motor_F->setSpeed(pot_motor_D);
		motor_F->run(BACKWARD);
	}
	else
	{
		motor_F->run(RELEASE);
	}
}

int anda(int direcao, unsigned long tmp_acao)
{
	// Serial.print("Direcao: ");
	// Serial.println(direcao);
	// Serial.print("Tempo: ");
	// Serial.println(tmp_acao);

	// t_motor.set_max(tmp_acao);

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

		// lcd.setCursor(12,1);
		// lcd.print (t_motor.get_seg());
	}
	else
	{
		aciona_motor(0,0);
		return 1;
	}
	return 0;
}

int anda(int direcao)
{
	// Serial.print("Direcao: ");
	// Serial.println(direcao);


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
	else  if (direcao == PARA)
	{
		aciona_motor(0,0);
	}
	else
	{
		aciona_motor(0,0);
	}

	return (t_motor.get_seg());
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
	int dir_rand;

	lcd.setCursor(10,1);
	lcd.print (t_partida.get_seg());
	lcd.setCursor(15,1);
	lcd.print (t_motor.get_seg());

	if (verifica_botao() == SELECIONA)
	{
		aciona_motor(0,0);

		return 1;
	}
	int temp_max = 59;
	if (t_partida.get_seg()>temp_max) {
		aciona_motor(0,0);
		aciona_motor_F(0);
		return 1;
	}
	if (estado_motor > 1)
	{
		if (objeto) estado_motor = 3;

		int ld = analogRead(LDR_DIR_PIN);
		int le = analogRead(LDR_ESQ_PIN);

		if((ld < lim_parede) && (le < lim_parede))
		{
			t_motor.reset();
			while (t_motor.get_milis() < t_anda_re) aciona_motor(-1, -1);
			t_motor.reset();
			while (t_motor.get_milis() < 2*t_giro_90) aciona_motor(-1, 1); 	// gira 90 graus
			// while (anda(DIREITA,2*t_giro_90) != 1); 	// gira 180 graus
		}
		else if(ld < lim_parede)
		{
			t_motor.reset();
			while (t_motor.get_milis() < t_giro_90) aciona_motor(1, -1); 	// gira 90 graus
			// while (anda(ESQUERDA,t_giro_90) != 1); 	// gira 90 graus
		}
		else if(le < lim_parede)
		{
			t_motor.reset();
			while (t_motor.get_milis() < t_giro_90) aciona_motor(-1, 1); 	// gira 90 graus
			// while (anda(DIREITA,t_giro_90) != 1); 	// gira 90 graus
		}
	}


	switch (estado_motor) {
		case 0:	// Espera o inicio
		lcd.setCursor(0,1);
		lcd.print ("estado 0");

		if (analogRead(LDR_S_PIN) < 100)
		{
			t_partida.reset();
			t_motor.reset();
			luz = 0;
			estado_motor = 1;
		}
		break;

		case 1: // Procura Posicao inicial
		lcd.setCursor(0,1);
		lcd.print ("estado 1");

		if (busca_luz(luz) == 1)
		{
			ldr_direcao_base = ldr_direcao;
			ldr_dif_max_base = ldr_dif_max;
			t_motor.reset();
			while (t_motor.get_milis() < 2*t_giro_90) aciona_motor(1, -1); 	// gira 90 graus
			estado_motor = 2;
		}
		break;

		case 2: // Anda sobre a linha

		lcd.setCursor(0,1);
		lcd.print ("estado 2");
		// delay(1000);
		atualiza();

		segue_linha();

		if (otico_cor != otico_cor_ultima)
		{
			otico_cor_ultima = otico_cor;

			if (otico_cor == BLACK)
			{
				dir_rand = i_rand(1,2);
				// t_motor.reset();
				// while (anda(TRAS,t_anda_re) != 1); 		// anda para tras
				// delay(5);
				t_motor.reset();
				if (dir_rand = 1) while (t_motor.get_milis() < t_giro_90) aciona_motor(0, 1); 	// gira 90 graus
				else while (t_motor.get_milis() < t_giro_90) aciona_motor(0, 1); 	// gira 90 graus
			}
		}

		// estado_motor = 3;
		break;

		case 3: // Analiza cor do objeto
		lcd.setCursor(0,1);
		lcd.print ("estado 3");
		anda(PARA);
		ve_cor();
		if (cor == BLACK)
		{
			dir_rand = i_rand(1,2);
			t_motor.reset();
			while (t_motor.get_milis() < t_anda_re) aciona_motor(-1, -1);
			// while (anda(TRAS,t_anda_re) != 1); 		// anda para tras
			t_motor.reset();
			while (t_motor.get_milis() < t_giro_90) aciona_motor(-1, 1); 	// gira 90 graus
			// while (anda(dir_rand,t_giro_90) != 1); 	// gira 90 graus
		}
		// else if (cor == WHITE)
		// {
		// 	dir_rand = i_rand(1,2);
		// 	t_motor.reset();
		// 	while (anda(TRAS,t_anda_re) != 1); 		// anda para tras
		// 	t_motor.reset();
		// 	while (anda(dir_rand,2*t_giro_90) != 1); 	// gira 90 graus
		// }
		else
		{
			t_motor.reset();
			while (t_motor.get_milis() < t_anda_re) aciona_motor(1, 1); 	// gira 90 graus
			// anda(FRENTE, t_anda_re);
			aciona_motor_F(1);
			delay(10);
			aciona_motor_F(0);

			luz = 0;
			while (busca_luz(luz) != 1);
			if (ldr_direcao == ldr_direcao_base)
			{
				// while (diferenca_ldr() < ldr_dif_max_base ) aciona_motor(1, 1);  //anda(FRENTE);

			}
			else
			{
				t_motor.reset();
				while (t_motor.get_milis() < 2*t_giro_90) aciona_motor(-1, 1);//(anda(dir_rand,2*t_giro_90) != 1);

				// int difer = diferenca_ldr();
				// int dirr;
				//
				// while (diferenca_ldr() < ldr_dif_max_base ) {
				// 	atualiza();
				// 	if (diferenca_ldr() >= difer) difer = diferenca_ldr();
				// 	else
				// 	{
				// 		dirr = (dirr == 1)? 2:1;
				// 		t_motor.reset();
				// 		anda(dirr, t_anda_re);
				// 	}
				// 	aciona_motor(1, 1);//anda(FRENTE);
				// }
			}
			// aciona_motor_F(-1);
			// delay(10);
			// aciona_motor_F(0);

			t_motor.reset();
			while (t_motor.get_milis() < t_anda_re) aciona_motor(-1, 1);// (anda(TRAS,t_anda_re) != 1);
			t_motor.reset();
			while (t_motor.get_milis() < 2*t_giro_90) aciona_motor(-1, 1);//(anda(dir_rand,2*t_giro_90) != 1);
			// while (anda(dir_rand,2*t_giro_90) != 1); 	// gira 90 graus

		}
		estado_motor = 2;
		break;
		/*
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
	atualiza();
	// static int luz = 0;
	int luzeiro = diferenca_ldr();

	if (luz == 0)
	{
		luz = 1;
		ldr_dif_max = 0;
		t_move.reset();
	}
	else if (luz == 1)
	{
		if (t_move.get_milis() > 4*t_giro_90) luz = 2; 		// gira 360 graus
		// aciona_motor(1,-1);
		// if (anda(ESQUERDA,4*t_giro_90) == 1) n = 2; 		// gira 360 graus
		aciona_motor(1, -1); 		// gira 360 graus
		ldr_dif_max = (ldr_dif_max>luzeiro) ? ldr_dif_max : luzeiro;
	}
	else if (luz == 2)
	{
		t_move.reset();
		while (!t_move.fim()) anda(PARA);
		// aciona_motor(0,0);
		luz = 3;
	}
	else if (luz == 3)
	{
		ldr_dif_max -= 60;
		luz = 4;
	}
	else if (luz == 4)
	{
		aciona_motor(-1, 1); 		// gira 360 graus
		// anda(DIREITA);		// gira ate encontra maior luz
		if (ldr_dif_max < luzeiro);// || luzeiro > 300)
		// if (luzeiro > 300)
		{
			// aciona_motor(0,0);
			// gira ate encontra maior luz
			aciona_motor(0, 0);
			t_move.reset();
			while (!t_move.fim());
			luz = 0;
			return 1;
		}
	}
	return 0;
}

int segue_linha()
{
	int dir_rand;

	if (verifica_botao() == SELECIONA)
	{
		aciona_motor(0,0);
		return 1;
	}
	atualiza();
	diferenca_otico();
	// anda(otico_direcao);
	// int e=digitalRead(OTICO_E_PIN);
	// int d=digitalRead(OTICO_D_PIN);


	if (otico_direita == otico_esquerda)
	{
		aciona_motor(1,1);
	}
	else if (otico_esquerda == 1) {
		aciona_motor(1,0);
		// delay(5);
		// aciona_motor(1,1);
	}
	else if (otico_direita == 1) {
		aciona_motor(0,1);
		// delay(5);
		// aciona_motor(1,1);
	}
}

int movimentos ()
{
	if (!ligado || mov == 0)
	{
		aciona_motor(0,0);
		ligado = false;
		return 1;
	}

	if (verifica_botao() == SELECIONA)
	{
		aciona_motor(0,0);
		ligado = false;
		return 1;
	}

	if (mov == 1)		{if (move_auto() == 1) 		ligado = false;}
	else if (mov == 2)	{if (move(seq_mov) == 1)	ligado = false;} // Teste
	else if (mov == 3)	{if (move(seq_mov) == 1) 	ligado = false;} // Vai Vem
	else if (mov == 4)	{if (move(seq_mov) == 1) 	ligado = false;} // Triangulo
	else if (mov == 5)	{if (move(seq_mov) == 1) 	ligado = false;} // Quadrado
	else if (mov == 6)	{if (move(seq_mov) == 1)	ligado = false;} // Giro 90
	else if (mov == 7)	{if (busca_luz(0) == 1)	ligado = false;}
	else if (mov == 8)	{if (segue_linha() == 1) 	ligado = false;}

	return 0;
}

void 	e_teste()
{
	seq_mov.set_tam(8);
	seq_mov.reset();
	seq_mov.set( FRENTE,  t_teste );
	seq_mov.set( PARA, 	  t_teste );
	seq_mov.set( TRAS,    t_teste );
	seq_mov.set( PARA,    t_teste );
	seq_mov.set( DIREITA, t_teste );
	seq_mov.set( PARA,    t_teste );
	seq_mov.set( ESQUERDA,t_teste );
	seq_mov.set( PARA,    t_teste );
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
	seq_mov.set( FRENTE,  t_anda_raiz );
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
