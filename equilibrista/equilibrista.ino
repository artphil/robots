#include "ambiente.h"

void setup()
{
	Serial.begin(9600);

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
		t.reset();
		Serial.print("otico D = ");
		Serial.println(otico_direita);
		Serial.print("otico E = ");
		Serial.println(otico_esquerda);
		Serial.println();
		Serial.print("encoder D = ");
		Serial.println(encdr_d_valor-encdr_d_ultimo);
		Serial.print("encoder E = ");
		Serial.println(encdr_e_valor-encdr_e_ultimo);
		Serial.println();
		Serial.print("pot D = ");
		Serial.println(pot_motor_D);
		Serial.print("pot E = ");
		Serial.println(pot_motor_E);
		Serial.println();
		Serial.print("ldr = ");
		Serial.println(ldr_valor);
		Serial.println();
	}
}


void atualiza () // Atualiza as constantes utilizadas
{
	int sensorValue = analogRead(A0);
	carga = sensorValue * (5.0 / 1023.0);

	ldr_valor = analogRead(LDR_PIN);
	ldr_direita = analogRead(LDR_D_PIN);
	ldr_esquerda = analogRead(LDR_E_PIN);

	otico_direita = analogRead(OTICO_D_PIN);
	otico_esquerda = analogRead(OTICO_E_PIN);


	encdr_e_ultimo = encoder_E.read();
	encdr_d_ultimo = encoder_D.read();
	encdr_e_valor += encdr_e_ultimo;
	encdr_d_valor += encdr_d_ultimo;
	reset_encoders();

	objeto = ve_objeto();

	botao = 0;
}
