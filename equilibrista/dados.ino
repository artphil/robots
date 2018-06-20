#include "ambiente.h"

/* implementação das funcoes */

void grava_EEPROM ()
{
	lcd.setCursor(0,1);
	lcd.print ("Gravando        ");

	EEPROM.write(0, pot_motor_D);
	EEPROM.write(1, pot_motor_E);

	EEPROM.write(2, ldr_limiar);

	EEPROM.write(3, ldr_branco[0]/4);
	EEPROM.write(4, ldr_branco[1]/4);
	EEPROM.write(5, ldr_branco[2]/4);

	EEPROM.write(6, ldr_preto[0]/4);
	EEPROM.write(7, ldr_preto[1]/4);
	EEPROM.write(8, ldr_preto[2]/4);

	EEPROM.write(9, t_anda);
	EEPROM.write(10, t_giro_90);

	for (size_t i = 0; i < 4; i++) {
		lcd.setCursor(9+i,1);
		lcd.print (".");
		delay(200);
	}
}

void le_EEPROM ()
{
	lcd.setCursor(0,1);
	lcd.print ("Lendo dados     ");

	pot_motor_D = EEPROM.read(0);
	pot_motor_E = EEPROM.read(1);

	ldr_limiar   = EEPROM.read(2);

	ldr_branco[0]= EEPROM.read(3)*4;
	ldr_branco[1]= EEPROM.read(4)*4;
	ldr_branco[2]= EEPROM.read(5)*4;

	ldr_preto[0]= EEPROM.read(6)*4;
	ldr_preto[1]= EEPROM.read(7)*4;
	ldr_preto[2]= EEPROM.read(8)*4;

	t_anda  	= EEPROM.read(9);
	t_anda_raiz = sqrt(t_anda);
	t_giro_90  	= EEPROM.read(10);
	t_giro_45   = t_giro_90/2;

	for (size_t i = 0; i < 4; i++) {
		lcd.setCursor(12+i,1);
		lcd.print (".");
		delay(200);
	}
}

void atualiza() // Atualiza as constantes utilizadas
{
	int sensorValue = analogRead(A0);
	carga = sensorValue * (5.0 / 1023.0);

	ldr_valor = analogRead(LDR_PIN);
	ldr_start = analogRead(LDR_S_PIN);
	ldr_difer = analogRead(LDR_DIF_PIN);

	otico_direita = digitalRead(OTICO_D_PIN);
	otico_esquerda = digitalRead(OTICO_E_PIN);

	// encoder_E.tick();
	// encoder_D.tick();
	// encdr_e_ultimo = encoder_E.getPosition();
	// encdr_d_ultimo = encoder_D.getPosition();
	// // encdr_e_valor += encdr_e_ultimo;
	// encdr_d_valor += encdr_d_ultimo;
	// reset_encoders();

	objeto = ve_objeto();

	botao = 0;
}
