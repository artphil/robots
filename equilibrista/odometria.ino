/* implementação das funcoes */

void inicia_odometro()
{
	KP = 0.3;
	KD = 5.0;
	esperado = 10;
	ultimo_erro = 0;
	mov_giro = false;
	mov_anterior = false;
}

void reset_encoders()
{
	encoder_D.setPosition(0);
	encoder_E.setPosition(0);
}
void calcula_PD ()
{

	int erro = encdr_d_ultimo - encdr_e_ultimo;
    int sinalControlador = KP * erro + KD * (erro - ultimo_erro);
    int new_pot_motor_D = pot_motor_D - sinalControlador;
    int new_pot_motor_E = pot_motor_E + sinalControlador;

	ultimo_erro = erro;

	if (new_pot_motor_D > 255) new_pot_motor_D = 255;
	if (new_pot_motor_D < 0) new_pot_motor_D = 0;
	pot_motor_D = new_pot_motor_D;

	if (new_pot_motor_E > 255) new_pot_motor_E = 255;
	if (new_pot_motor_E < 0) new_pot_motor_E = 0;
	pot_motor_E = new_pot_motor_E;
}

void calibra_motor ()
{
	conta_tempo t_calib(1000);
	mov_giro = true;
	do {
		reset_encoders();
		aciona_motor(1,1);
		t_calib.reset();
		while (!t_calib.fim());
		aciona_motor(0,0);
		calcula_PD();
	} while(ultimo_erro != 0);
	mov_giro = false;

}

void corretor()
{
	if (mov_giro != mov_anterior)
	{
		mov_anterior = mov_giro;
		encdr_e_valor = 0;
		encdr_d_valor = 0;
	}

	if (!mov_giro)
	{
		// t_pd.reset();
		calcula_PD();
	}
}
