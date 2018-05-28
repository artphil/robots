/* implementação das funcoes */

void reset_encoders()
{
	encoder_D.write(0);
	encoder_E.write(0);
}
void calcula_PD ()
{
	int erro = encoder_D.read() - encoder_E.read();
    int sinalControlador = KP * erro + KD * (erro - ultimo_erro);
    ultimo_erro = erro;
    pot_motor_D = pot_motor_D + sinalControlador;
    pot_motor_E = pot_motor_E - sinalControlador;

	if (pot_motor_D > 255) pot_motor_D = 255;
	if (pot_motor_D < 0) pot_motor_D = 0;

	if (pot_motor_E > 255) pot_motor_E = 255;
	if (pot_motor_E < 0) pot_motor_E = 0;
	
}

void calibra_motor ()
{
	conta_tempo t_calib(1000);

	do {
		t_calib.reset();
		reset_encoders();
		while (!t_calib.fim()) aciona_motor(1,1);
		calcula_PD();
	} while(ultimo_erro != 0);
}
