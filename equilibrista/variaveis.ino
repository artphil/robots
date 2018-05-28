#include "ambiente.h"

/* implementação das funcoes */

void atualiza () // Atualiza as constantes utilizadas
{
	int sensorValue = analogRead(A0);
	carga = sensorValue * (5.0 / 1023.0);

	ldr_valor = analogRead(LDR_PIN);
	ldr_direita = analogRead(LDR_D_PIN);
	ldr_esquerda = analogRead(LDR_E_PIN);

	encdr_d_valor = encoder_E.read();
	encdr_e_valor = encoder_D.read();

	objeto = ve_objeto();

	botao = 0;
}

void grava_EEPROM ()
{
	lcd.setCursor(0,1);
	lcd.print ("Gravando        ");

	EEPROM.write(0, pot_motor_D);
	EEPROM.write(1, pot_motor_E);

	EEPROM.write(2, ldr_limiar);

	EEPROM.write(3, ldr_branco[0]/4);
	EEPROM.write(4, ldr_branco[1]/4);
	EEPROM.write(5, ldr_branco[2]/4);

	EEPROM.write(6, ldr_preto[0]/4);
	EEPROM.write(7, ldr_preto[1]/4);
	EEPROM.write(8, ldr_preto[2]/4);

	EEPROM.write(9, t_anda/100);
	EEPROM.write(10, t_giro_90/100);

	for (size_t i = 0; i < 4; i++) {
		lcd.setCursor(9+i,1);
		lcd.print (".");
		delay(200);
	}
}

void le_EEPROM ()
{
	lcd.setCursor(0,1);
	lcd.print ("Lendo dados     ");

	pot_motor_D = EEPROM.read(0);
	pot_motor_E = EEPROM.read(1);

	ldr_limiar   = EEPROM.read(2);

	ldr_branco[0]= EEPROM.read(3)*4;
	ldr_branco[1]= EEPROM.read(4)*4;
	ldr_branco[2]= EEPROM.read(5)*4;

	ldr_preto[0]= EEPROM.read(6)*4;
	ldr_preto[1]= EEPROM.read(7)*4;
	ldr_preto[2]= EEPROM.read(8)*4;

	t_anda  	= EEPROM.read(9)*100;
	t_anda_raiz = sqrt(t_anda);
	t_giro_90  	= EEPROM.read(10)*100;
	t_giro_45   = t_giro_90/2;

	for (size_t i = 0; i < 4; i++) {
		lcd.setCursor(12+i,1);
		lcd.print (".");
		delay(200);
	}
}


void inicia_menu() // Inicializa as vatiaveis
{
	// Parametros iniciais
	estado_menu = 0;
	estado_menu = 0;
	t_menu = 0;
	lcd.begin(16, 2);
}

void inicia_motor() // Inicializa as vatiaveis
{
	// Parametros iniciais
	t_teste = 2000;
	pot_motor_D = 160;
	pot_motor_E = 140;
	t_anda  	= 5000;
	t_anda_raiz = sqrt(t_anda);
	t_giro_90  	= 2400;
	t_giro_45   = t_giro_90/2;
	ligado = false;
	AFMS.begin(); // create with the default frequency 1.6KHz
}

void inicia_sensor() // Inicializa as vatiaveis
{
	// Parametros iniciais
	ldr_limiar   = 170;
	ldr_cor[RED]	= 0.0;
	ldr_cor[GREEN]  = 0.0;
	ldr_cor[BLUE]	= 0.0;
	led = 0;
	cor = WHITE;
	led_atual[0] = false;
	led_atual[1] = false;
	led_atual[2] = false;
	pinMode(LED_RED_PIN,OUTPUT);
	pinMode(LED_GRN_PIN,OUTPUT);
	pinMode(LED_BLU_PIN,OUTPUT);
}

void inicia_odometro()
{
	KP = 0.3;
	KD = 7;
	esperado = 10;
	ultimo_erro = 0;
}

void 	inicia_entradas()
{
	teste.set_tam(8);
	teste.reset();
	teste.set( FRENTE,   t_teste );
	teste.set( PARA, 	 t_teste );
	teste.set( TRAS,     t_teste );
	teste.set( PARA,   	 t_teste );
	teste.set( DIREITA,  t_teste );
	teste.set( PARA,   	 t_teste );
	teste.set( ESQUERDA, t_teste );
	teste.set( PARA,   	 t_teste );
	teste.print();

	vai_vem.set_tam(6);
	vai_vem.reset();
	vai_vem.set( FRENTE,  	t_anda );
	vai_vem.set( ESQUERDA, 	t_giro_90 );
	vai_vem.set( ESQUERDA, 	t_giro_90 );
	vai_vem.set( FRENTE,  	t_anda );
	vai_vem.set( DIREITA, 	t_giro_90 );
	vai_vem.set( DIREITA, 	t_giro_90 );

	triangulo.set_tam(8);
	triangulo.reset();
	triangulo.set( FRENTE,  t_anda );
	triangulo.set( DIREITA, t_giro_90 );
	triangulo.set( FRENTE,  t_anda );
	triangulo.set( DIREITA, t_giro_90 );
	triangulo.set( DIREITA, t_giro_45 );
	triangulo.set( FRENTE,  t_anda_raiz);
	triangulo.set( DIREITA, t_giro_90 );
	triangulo.set( DIREITA, t_giro_45 );

	quadrado.set_tam(8);
	quadrado.reset();
	quadrado.set( FRENTE,  t_anda );
	quadrado.set( DIREITA, t_giro_90 );
	quadrado.set( FRENTE,  t_anda );
	quadrado.set( DIREITA, t_giro_90 );
	quadrado.set( FRENTE,  t_anda );
	quadrado.set( DIREITA, t_giro_90 );
	quadrado.set( FRENTE,  t_anda );
	quadrado.set( DIREITA, t_giro_90 );

	giro45.set_tam(3);
	giro45.reset();
	giro45.set( DIREITA,  t_giro_45 );
	giro45.set( PARA,  	  t_giro_45 );
	giro45.set( ESQUERDA, t_giro_45 );

	giro90.set_tam(3);
	giro90.reset();
	giro90.set( DIREITA,  t_giro_90 );
	giro90.set( PARA,     t_giro_90 );
	giro90.set( ESQUERDA, t_giro_90 );
}
