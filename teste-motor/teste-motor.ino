#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
//Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
//Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
//Adafruit_DCMotor *motor4 = AFMS.getMotor(4);
   
void setup()
{
  AFMS.begin(); // create with the default frequency 1.6KHz
}
 
void loop()
{
  motor1->setSpeed(150);
  motor1->run(FORWARD);

  delay(1000);
  
  motor1->setSpeed(150);
  motor1->run(BACKWARD);
    
  delay(1000);
}
