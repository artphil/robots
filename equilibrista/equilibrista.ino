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
	// inicia_odometro();
}
int num=1;
conta_tempo t(2000);
bool a = true;
bool lado = true;
void loop()
{
	// put your main code here, to run repeatedly:
	atualiza();
	menu();
	movimentos();
	// corretor();

	if (t.fim())
	{
		// t.print();
		// (i_rand(3,7));//
		t.reset();
		// Serial.print("otico D = ");
		// Serial.println(otico_direita);
		// Serial.print("otico E = ");
		// Serial.println(otico_esquerda);
		// Serial.println();
		/**/
		// Serial.print("encoder D = ");
		// Serial.println(encdr_d_ultimo);
		// Serial.print("encoder E = ");
		// Serial.println(encdr_e_ultimo);
		// Serial.println();
		// Serial.print("pot D = ");
		// Serial.println(pot_motor_D);
		// Serial.print("pot E = ");
		// Serial.println(pot_motor_E);
		// Serial.println();
		/**/
		Serial.print("ldr = ");
		Serial.println(ldr_difer);
		Serial.println(diferenca_ldr());
		Serial.println();
		Serial.print("anda = ");
		Serial.println((long) anda_fat*num);
		num++;
		Serial.println(num);
		Serial.println(anda_fat);
		/**/
		Serial.print("ldr D = ");
		Serial.println(analogRead(LDR_DIR_PIN));
		Serial.print("ldr E = ");
		Serial.println(analogRead(LDR_ESQ_PIN));
		Serial.println();
	}
}
