#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);
bool liga = false;
bool garra = false;
int potencia = 150;
void setup()
{
	AFMS.begin(); // create with the default frequency 1.6KHz
}

void loop()
{

	if(verifica_botao() == 5) liga = !liga;
	if(verifica_botao() == 1) potencia++;
	if(verifica_botao() == 4) potencia--;
	if(verifica_botao() == 2) garra = !garra;

	motor1->setSpeed(potencia);
	motor2->setSpeed(potencia);
	motor3->setSpeed(potencia);
	motor4->setSpeed(100);

	if(liga)
	{
		if (garra) motor4->run(FORWARD);
		else motor4->run(BACKWARD);
		// motor1->run(FORWARD);
		// motor2->run(FORWARD);
		// motor3->run(FORWARD);
		// motor4->run(FORWARD);
	}
	else
	{
		motor1->run(RELEASE);
		motor2->run(RELEASE);
		motor4->run(RELEASE);
		motor3->run(RELEASE);
	}
}
int verifica_botao () // Identifica qual botao foi acionado
{
	int botao = analogRead (0); //Leitura do valor da porta analógica A0

	if (botao < 50) 		return 1;
	else if (botao < 100) 	return 2;
	else if (botao < 300) 	return 3;
	else if (botao < 500) 	return 4;
	else if (botao < 700) 	return 5;
	else					return 0;
}
