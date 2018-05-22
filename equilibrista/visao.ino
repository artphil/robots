#include "ambiente.h"

/* implementação das funcoes */

int aciona_luz(bool r, bool g, bool b)
{
	if (led_atual[0] == r)
	if (led_atual[1] == g)
	if (led_atual[2] == b)
	return 1;

	led_atual[0] = r;
	led_atual[1] = g;
	led_atual[2] = b;

	if (r) digitalWrite(LED_RED_PIN, HIGH);
	else digitalWrite(LED_RED_PIN, LOW);

	if (g) digitalWrite(LED_GRN_PIN, HIGH);
	else digitalWrite(LED_GRN_PIN, LOW);

	if (b) digitalWrite(LED_BLU_PIN, HIGH);
	else digitalWrite(LED_BLU_PIN, LOW);


}

int media_sensor(int n)
{
	int total = 0;
	for (int i = 0;i < n;i++)
	{
		delay(200);
		total += analogRead(LDR_PIN);
	}
	return total/n;
}

bool ve_objeto()
{
	if (ldr_valor > ldr_limiar) return false;
	else return true;
}

void ve_cor()
{
	int r, g, b;
	float cinza;

	lcd.setCursor(0,1);	lcd.print ("Analizando      ");

	aciona_luz(true, false, false);
	r = media_sensor(5);
	aciona_luz(false, false, false);

	lcd.setCursor(11,1);	lcd.print (".");

	aciona_luz(false, true, false);
	g = media_sensor(5);
	aciona_luz(false, false, false);

	lcd.setCursor(12,1);	lcd.print (".");

	aciona_luz(false, false, true);
	b = media_sensor(5);
	aciona_luz(false, false, false);

	lcd.setCursor(13,1);	lcd.print (".");
	delay(200);


	cinza = ldr_branco[RED]-ldr_preto[RED];
	ldr_cor[RED] 	= (r - ldr_preto[RED])*(255.0/cinza);

	Serial.print("calculo red = ");
	Serial.print(r);
	Serial.print("-");
	Serial.print(ldr_preto[RED]);
	Serial.print("*255 /");
	Serial.println(cinza);

	cinza = ldr_branco[GREEN]-ldr_preto[GREEN];
	ldr_cor[GREEN] 	= (g - ldr_preto[GREEN])*(255.0/cinza);

	Serial.print("calculo green = ");
	Serial.print(g);
	Serial.print("-");
	Serial.print(ldr_preto[GREEN]);
	Serial.print("*255 /");
	Serial.println(cinza);

	cinza = ldr_branco[BLUE]-ldr_preto[BLUE];
	ldr_cor[BLUE] 	= (b - ldr_preto[BLUE])*(255.0/cinza);

	Serial.print("calculo blue = ");
	Serial.print(b);
	Serial.print("-");
	Serial.print(ldr_preto[BLUE]);
	Serial.print("*255 /");
	Serial.println(cinza);

	Serial.print("bruto =  ");
	Serial.print(r);
	Serial.print(", ");
	Serial.print(g);
	Serial.print(", ");
	Serial.println(b);

	if ((ldr_cor[RED] + ldr_cor[GREEN] + ldr_cor[BLUE]) < 50)
	{
		cor = BLACK;
	}
	else if ((ldr_cor[RED] + ldr_cor[GREEN] + ldr_cor[BLUE]) > 700)
	{
		cor = WHITE;
	}
	else
	{
		cinza = (ldr_cor[RED] + ldr_cor[GREEN] + ldr_cor[BLUE]);
		r = (100.0 * ldr_cor[RED]) / cinza;
		g = (100.0 * ldr_cor[GREEN]) / cinza;
		b = (100.0 * ldr_cor[BLUE]) / cinza;

		if (ldr_cor[RED] > ldr_cor[GREEN] && ldr_cor[RED] > ldr_cor[BLUE])
		{
			if ((r-g) < 20) cor = YELLOW;
			else cor = RED;
		}
		else if (ldr_cor[GREEN] > ldr_cor[RED] && ldr_cor[GREEN] > ldr_cor[BLUE])
		{
			cor = GREEN;
		}
		else cor = BLUE;
	}
	Serial.print("calculo =  ");
	Serial.print(ldr_cor[RED]);
	Serial.print(", ");
	Serial.print(ldr_cor[GREEN]);
	Serial.print(", ");
	Serial.println(ldr_cor[BLUE]);

	Serial.print("percent =  ");
	Serial.print(r);
	Serial.print(", ");
	Serial.print(g);
	Serial.print(", ");
	Serial.println(b);

	lcd.setCursor(0,1);
	lcd.print ("                ");
	lcd.setCursor(0,1);
	lcd.print (nome_cor[cor]);
	Serial.println(nome_cor[cor]);

	delay(1000);
}

void balanco_branco()
{
	lcd.setCursor(0,1);	lcd.print ("Analizando      ");

	aciona_luz(true, false, false);
	ldr_branco[RED] = media_sensor(5);
	aciona_luz(false, false, false);

	lcd.setCursor(11,1);	lcd.print (".");

	aciona_luz(false, true, false);
	ldr_branco[GREEN] = media_sensor(5);
	aciona_luz(false, false, false);

	lcd.setCursor(12,1);	lcd.print (".");

	aciona_luz(false, false, true);
	ldr_branco[BLUE] = media_sensor(5);
	aciona_luz(false, false, false);

	Serial.print("branco( ");
	Serial.print(ldr_branco[RED]);
	Serial.print(", ");
	Serial.print(ldr_branco[GREEN]);
	Serial.print(", ");
	Serial.print(ldr_branco[BLUE]);
	Serial.println(" )");


	lcd.setCursor(13,1);	lcd.print (".");
	delay(200);
	lcd.setCursor(14,1);	lcd.print (".");
	delay(200);
	lcd.setCursor(15,1);	lcd.print (".");
	delay(200);
}

void balanco_preto()
{
	lcd.setCursor(0,1);	lcd.print ("Analizando      ");

	aciona_luz(true, false, false);
	ldr_preto[RED] = media_sensor(5);
	aciona_luz(false, false, false);

	lcd.setCursor(11,1);	lcd.print (".");

	aciona_luz(false, true, false);
	ldr_preto[GREEN] = media_sensor(5);
	aciona_luz(false, false, false);

	lcd.setCursor(12,1);	lcd.print (".");

	aciona_luz(false, false, true);
	ldr_preto[BLUE] = media_sensor(5);
	aciona_luz(false, false, false);

	Serial.print("preto( ");
	Serial.print(ldr_preto[RED]);
	Serial.print(", ");
	Serial.print(ldr_preto[GREEN]);
	Serial.print(", ");
	Serial.print(ldr_preto[BLUE]);
	Serial.println(" )");

	lcd.setCursor(13,1);	lcd.print (".");
	delay(200);
	lcd.setCursor(14,1);	lcd.print (".");
	delay(200);
	lcd.setCursor(15,1);	lcd.print (".");
	delay(200);
}

int diferenca_ldr()
{
	Serial.print("Difernça ldr =");
	Serial.println(abs(ldr_esquerda-ldr_direita));
	Serial.print("Direção =");
	if (ldr_esquerda>ldr_direita)
	{
		ldr_direcao = ESQUERDA;
		Serial.println("Esquerda");
	}
	else if (ldr_esquerda<ldr_direita)
	{
		ldr_direcao = DIREITA;
		Serial.println("Direita");
	}
	else
	{
		ldr_direcao = 0;
		Serial.println("Centro");
	}
	return abs(ldr_esquerda-ldr_direita);
}
