#include "ambiente.h"

void setup()
{
	Serial.begin(9600);

    // le_EEPROM ();
	inicia_menu();
	inicia_motor();
	inicia_sensor();
	inicia_entradas();
}

void loop()
{
	// put your main code here, to run repeatedly:
	atualiza();
	menu();
	movimentos();
}
