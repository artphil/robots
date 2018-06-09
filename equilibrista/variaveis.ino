#include "ambiente.h"

/* implementação das funcoes */

void atualiza () // Atualiza as constantes utilizadas
{
	int sensorValue = analogRead(A0);
	carga = sensorValue * (5.0 / 1023.0);

	ldr_valor = analogRead(LDR_PIN);
	ldr_direita = analogRead(LDR_D_PIN);
	ldr_esquerda = analogRead(LDR_E_PIN);

	otico_direita = analogRead(OTICO_D_PIN);
	otico_esquerda = analogRead(OTICO_E_PIN);


	encdr_e_ultimo = encoder_E.read();
	encdr_d_ultimo = encoder_D.read();
	encdr_e_valor += encdr_e_ultimo;
	encdr_d_valor += encdr_d_ultimo;
	reset_encoders();

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

	EEPROM.write(9, t_anda);
	EEPROM.write(10, t_giro_90);

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

	t_anda  	= EEPROM.read(9);
	t_anda_raiz = sqrt(t_anda);
	t_giro_90  	= EEPROM.read(10);
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
	lcd.begin(16, 2);
}

void inicia_motor() // Inicializa as vatiaveis
{
	// Parametros iniciais
	t_teste = 2000;
	pot_motor_D = 200;
	pot_motor_E = 200;
	t_anda  	= 100;
	anda_cm  	= 30;
	anda_fat  	= 7;
	t_anda_raiz = sqrt(t_anda);
	t_giro_90  	= 36;
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
	otico_direcao = FRENTE;
	pinMode(LED_RED_PIN,OUTPUT);
	pinMode(LED_GRN_PIN,OUTPUT);
	pinMode(LED_BLU_PIN,OUTPUT);
}

void inicia_odometro()
{
	KP = 0.3;
	KD = 5.0;
	esperado = 10;
	ultimo_erro = 0;
	mov_giro = false;
	mov_anterior = false;
}


void 	e_teste()
{
	seq_mov.set_tam(8);
	seq_mov.reset();
	seq_mov.set( FRENTE,   t_teste );
	seq_mov.set( PARA, 	 t_teste );
	seq_mov.set( TRAS,     t_teste );
	seq_mov.set( PARA,   	 t_teste );
	seq_mov.set( DIREITA,  t_teste );
	seq_mov.set( PARA,   	 t_teste );
	seq_mov.set( ESQUERDA, t_teste );
	seq_mov.set( PARA,   	 t_teste );
	seq_mov.print();
}

void 	e_vai_vem()
{
	seq_mov.set_tam(6);
	seq_mov.reset();
	seq_mov.set( FRENTE,  	t_anda );
	seq_mov.set( ESQUERDA, 	t_giro_90 );
	seq_mov.set( ESQUERDA, 	t_giro_90 );
	seq_mov.set( FRENTE,  	t_anda );
	seq_mov.set( DIREITA, 	t_giro_90 );
	seq_mov.set( DIREITA, 	t_giro_90 );
	seq_mov.print();
}

void 	e_triangulo()
{
	seq_mov.set_tam(8);
	seq_mov.reset();
	seq_mov.set( FRENTE,  t_anda );
	seq_mov.set( DIREITA, t_giro_90 );
	seq_mov.set( FRENTE,  t_anda );
	seq_mov.set( DIREITA, t_giro_90 );
	seq_mov.set( DIREITA, t_giro_45 );
	seq_mov.set( FRENTE,  t_anda_raiz);
	seq_mov.set( DIREITA, t_giro_90 );
	seq_mov.set( DIREITA, t_giro_45 );
	seq_mov.print();
}

void 	e_quadrado()
{
	seq_mov.set_tam(8);
	seq_mov.reset();
	seq_mov.set( FRENTE,  t_anda );
	seq_mov.set( DIREITA, t_giro_90 );
	seq_mov.set( FRENTE,  t_anda );
	seq_mov.set( DIREITA, t_giro_90 );
	seq_mov.set( FRENTE,  t_anda );
	seq_mov.set( DIREITA, t_giro_90 );
	seq_mov.set( FRENTE,  t_anda );
	seq_mov.set( DIREITA, t_giro_90 );
	seq_mov.print();
}

void 	e_giro90()
{
	seq_mov.set_tam(3);
	seq_mov.reset();
	seq_mov.set( DIREITA,  t_giro_90 );
	seq_mov.set( PARA,     t_giro_90 );
	seq_mov.set( ESQUERDA, t_giro_90 );
	seq_mov.print();
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
