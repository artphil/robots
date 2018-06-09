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


	while (true)
	{
		a = (analogRead(LDR_PIN) < 200);

		if (a){
			aciona_motor(1,1);
		}
		else
		{
			if (lado)
			aciona_motor(0,1);
			else
			aciona_motor(1,0);

		}

	}
}
