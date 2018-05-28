#include "ambiente.h"

void setup()
{
	Serial.begin(9600);

	// le_EEPROM ();
	inicia_menu();
	inicia_motor();
	inicia_sensor();
	inicia_entradas();
	inicia_odometro();
}

conta_tempo t(2000);

void loop()
{
	// put your main code here, to run repeatedly:
	atualiza();
	menu();
	movimentos();

	if (t.fim())
	{
		t.print();
		t.reset();
		Serial.print("sinal D = ");
		Serial.println(encoder_D.read());
		Serial.print("sinal E = ");
		Serial.println(encoder_E.read());
	}

}
