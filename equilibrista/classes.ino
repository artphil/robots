#include "ambiente.h"

/* classes */

void entrada::set_tam(int n)
{ tam = n; }

int entrada::get_tam()
{ return tam; }

void entrada::reset()
{ i = 0; }

void entrada::set(int n, int *m)
{
	if (i < 50)
	{
		direcao[i] = n;
		tempo[i] = m;
		i++;
	}
}

int entrada::get_dir(int n)
{
	if (n >= tam) return -1;
	return direcao[n];
}

int entrada::get_tmp(int n)
{
	if (n >= tam) return -1;
	return *tempo[n];
}

void entrada::print()
{
	for (int i = 0; i < tam; i++) {
		Serial.print("( ");
		Serial.print(direcao[i]);
		Serial.print(" , ");
		Serial.print(*tempo[i]);
		Serial.println(" )");
	}
	Serial.println();
}

conta_tempo::conta_tempo(int max)
{
	t_max = max;
}

void conta_tempo::reset()
{
	tmp = millis();
}

int conta_tempo::get_tempo()
{
	return (int)(millis-tmp);
}

void conta_tempo::print()
{
	Serial.print("tempo = ");
	Serial.print(get_tempo()/1000.0);
	Serial.println(" s");
}
