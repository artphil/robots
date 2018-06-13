#include <RotaryEncoder.h>

RotaryEncoder encoder(A8, A9);
int ent_um = A8;
int ent_dois = A9;

void setup()
{
	Serial.begin(9600);
	Serial.println("SimplePollRotator example for the RotaryEncoder library.");
}


void loop()
{
	encoder.tick();

	Serial.print(encoder.getPosition());
	Serial.println();
}
