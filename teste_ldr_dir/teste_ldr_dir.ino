// Programa : Teste LCD 16x2 com Keypad

#include <LiquidCrystal.h>



void setup()
{
 	Serial.begin(9600);
}

void loop()
{
 Serial.println(analogRead(0));
}
