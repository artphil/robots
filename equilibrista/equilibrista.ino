#include "ambiente.h"

void setup()
{
	Serial.begin(9600);

	srand(millis());
	// le_EEPROM ();
	inicia_menu();
	inicia_motor();
	inicia_sensor();
	// inicia_entradas();
	inicia_odometro();
}

conta_tempo t(2000);
bool a = true;
bool lado = true;
void loop()
{
	// put your main code here, to run repeatedly:
	atualiza();
	menu();
	movimentos();
	corretor();

	if (t.fim())
	{
		// t.print();
		// (i_rand(3,7));//
		t.reset();
		Serial.print("otico D = ");
		Serial.println(otico_direita);
		Serial.print("otico E = ");
		Serial.println(otico_esquerda);
		Serial.println();
		/*		Serial.print("encoder D = ");
		Serial.println(encdr_d_valor-encdr_d_ultimo);
		Serial.print("encoder E = ");
		Serial.println(encdr_e_valor-encdr_e_ultimo);
		Serial.println();
		Serial.print("pot D = ");
		Serial.println(pot_motor_D);
		Serial.print("pot E = ");
		Serial.println(pot_motor_E);
		/*/		Serial.println();
		Serial.print("ldr = ");
		Serial.println(ldr_start);
		Serial.println();
	}
}

void atualiza () // Atualiza as constantes utilizadas
{
	int sensorValue = analogRead(A0);
	carga = sensorValue * (5.0 / 1023.0);

	ldr_valor = analogRead(LDR_PIN);
	ldr_start = analogRead(LDR_S_PIN);
	ldr_difer = analogRead(LDR_DIF_PIN);

	otico_direita = digitalRead(OTICO_D_PIN);
	otico_esquerda = digitalRead(OTICO_E_PIN);

	encoder_E.tick();
	encoder_D.tick();
	encdr_e_ultimo = encoder_E.getPosition();
	encdr_d_ultimo = encoder_D.getPosition();
	encdr_e_valor += encdr_e_ultimo;
	encdr_d_valor += encdr_d_ultimo;
	reset_encoders();

	objeto = ve_objeto();

	botao = 0;
}
