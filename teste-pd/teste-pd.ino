/*ref:http://www.andrix.com.br/robo-seguidor-de-linha-utilizando-um-controlador-proporcional-derivativo-pd-com-arduino/*/
#include <PololuQTRSensors.h>
int motorEsquerda = 11;
int motorDireita = 10;
int setpoint = 3500; // como são 8 sensores, variam de 0 a 7.000, onde 3.500 é a metade ou seja, robô simétrico sobre a linha
float KP = 0.3;
float KD = 7;
int erroAnterior = 0;
int velocidadeMotor1 = 255;
int velocidadeMotor2 = 255;
PololuQTRSensorsRC matrizSensores((unsigned char[]) {2,3,4,5,6,7,8,9}, 8);  // 0-1-2-3-4-5-6-7
// OITO SENSORES DE LUZ PARA SEGUIR A LINHA

void setup()
{
  pinMode(motorEsquerda,OUTPUT);
  pinMode(motorDireita,OUTPUT);
// MOTOR PARADO: 127;
// VELOCIDADE MÁXIMA FRENTE: 255
// VELOCIDADE MÁXIMA TRÁS: 0

  analogWrite(motorEsquerda,127);
  analogWrite(motorDireita,127);
  delay(2000);

// INICIA GIRO HORÁRIO PARA CALIBRAR SOBRE A LINHA

  analogWrite(motorEsquerda,175);
  analogWrite(motorDireita,50);

  // CALIBRAÇÃO DOS SENSORES - 5 SEGUNDOS
  int i;
  for (i = 0; i < 250; i++)
  {
    matrizSensores.calibrate();
    delay(20);
  }
  analogWrite(motorEsquerda,127);
  analogWrite(motorDireita,127);
  delay(2000);
}
///////////////// FIM DO SETUP //////////////////////
void loop()
{
  // LEITURA DOS SENSORES
  unsigned int arraySensores[8];
  int leituraSensores = matrizSensores.readLine(arraySensores);

  // CÁLCULO PD
  int erro = leituraSensores - setpoint;
  int sinalControlador = KP * erro + KD * (erro - erroAnterior);
  erroAnterior = erro;
  int novaVelocidadeMotor1 = velocidadeMotor1 + sinalControlador;
  int novaVelocidadeMotor2 = velocidadeMotor2 - sinalControlador;

//// CORREÇÃO DOS VALORES EXTREMOS DOS MOTORES
  if (novaVelocidadeMotor1 < 127)
    novaVelocidadeMotor1 = 127;
  if (novaVelocidadeMotor2 < 127)
    novaVelocidadeMotor2 = 127;

  if (novaVelocidadeMotor1 > 255)
    novaVelocidadeMotor1 = 255;
  if (novaVelocidadeMotor2 > 255)
    novaVelocidadeMotor2 = 255;

  // APLICAÇÃO DAS NOVAS VELOCIDADES DOS MOTORES VIA PWM
  analogWrite(motorEsquerda,novaVelocidadeMotor1);
  analogWrite(motorDireita,novaVelocidadeMotor2);

} // FIM DO VOID LOOP
// FIM DO PROGRAMA
