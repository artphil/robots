/* implementação das funcoes */

void inicia_odometro()
{
	KP = 0.3;
	KD = 5.0;
	mov_giro = false;
	mov_anterior = false;
}

void reset_encoders()
{
	encoder_D.setPosition(0);
	encoder_E.setPosition(0);
}
void calcula_PD (int tmpo)
{
	atualiza();
	int ref = tmpo/100;

	int erro_e = ref - encdr_e_ultimo;
	int sinal_e = KP * erro_e + KD * (erro_e - ultimo_erro_e);
	int new_pot_motor_E = pot_motor_E + sinal_e;

	ultimo_erro_e = erro_e;

	int erro_d = ref - encdr_d_ultimo;
	int sinal_d = KP * erro_d + KD * (erro_d - ultimo_erro_d);
	int new_pot_motor_D = pot_motor_D + sinal_d;

	ultimo_erro_d = erro_d;

	if (new_pot_motor_D > 255) new_pot_motor_D = 255;
	if (new_pot_motor_D < 0) new_pot_motor_D = 0;
	pot_motor_D = new_pot_motor_D;

	if (new_pot_motor_E > 255) new_pot_motor_E = 255;
	if (new_pot_motor_E < 0) new_pot_motor_E = 0;
	pot_motor_E = new_pot_motor_E;
}

void calibra_motor ()
{
	int temp = 1000;
	conta_tempo t_calib(temp);
	mov_giro = true;
	do {
		reset_encoders();
		aciona_motor(1,1);
		t_calib.reset();
		while (!t_calib.fim()) atualiza();
		aciona_motor(0,0);
		calcula_PD(temp);
		Serial.print("encoder D = ");
		Serial.println(encdr_d_ultimo);
		Serial.print("encoder E = ");
		Serial.println(encdr_e_ultimo);
		Serial.println();
		Serial.print("pot D = ");
		Serial.println(pot_motor_D);
		Serial.print("pot E = ");
		Serial.println(pot_motor_E);
		Serial.println();
	} while(ultimo_erro_e != 0 || ultimo_erro_d != 0);
	mov_giro = false;

}

void corretor()
{
	if (mov_giro != mov_anterior)
	{
		mov_anterior = mov_giro;
		encdr_e_valor = 0;
		encdr_d_valor = 0;
		reset_encoders();
		t_pd.reset();
	}

	if (!mov_giro && t_pd.fim())
	{
		t_pd.reset();
		calcula_PD(t_correc);
	}
}
