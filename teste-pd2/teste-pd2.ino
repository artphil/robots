#include <RotaryEncoder.h>

RotaryEncoder encoder1(A8, A9);
RotaryEncoder encoder2(A10, A11);
int ent_um = A10;
int ent_dois = A11;

void setup()
{
	Serial.begin(9600);
	Serial.println("SimplePollRotator example for the RotaryEncoder library.");
}


void loop()
{
	encoder1.tick();
	encoder2.tick();

	Serial.println(encoder1.getPosition());
	Serial.println(encoder2.getPosition());
	Serial.println();
}
