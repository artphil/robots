#include "ambiente.h"

/* classes */

// Entrada
void entrada::set_tam(int n)
{ tam = n; }

int entrada::get_tam()
{ return tam; }

void entrada::reset()
{ i = 0; }

void entrada::set(int d, int t)
{
	if (i < 50)
	{
		direcao[i] = d;
		tempo[i] = t;
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
	return tempo[n];
}

void entrada::print()
{
	for (int i = 0; i < tam; i++) {
		Serial.print("( ");
		Serial.print(direcao[i]);
		Serial.print(" , ");
		Serial.print(tempo[i]);
		Serial.println(" )");
	}
	Serial.println();
}

// Conta Tempo
conta_tempo::conta_tempo(int max)
{
	t_max = max;
	tmp = 0;
}

bool conta_tempo::fim()
{
	return (get_milis() > t_max);
}

void conta_tempo::reset()
{
	tmp = millis();
}

void conta_tempo::set_max(long t)
{
	t_max = t;
}

int conta_tempo::get_milis()
{
	return (int)(millis()-tmp);
}

int conta_tempo::get_seg()
{
	return (int)((millis()-tmp)/1000);
}

void conta_tempo::print()
{
	Serial.print("tempo = ");
	Serial.print(get_milis()/1000.0);
	Serial.println(" s");
	Serial.print("tempo max = ");
	Serial.print(t_max/1000.0);
	Serial.println(" s");
}
