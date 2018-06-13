#include "ambiente.h"

// Variveis
RotaryEncoder encoder_D(ENCDR_D1_PIN, ENCDR_D2_PIN);
RotaryEncoder encoder_E(ENCDR_E1_PIN, ENCDR_E2_PIN);

bool mov_giro;
bool mov_anterior;
int encdr_e_valor;
int encdr_d_valor;
int encdr_e_ultimo;
int encdr_d_ultimo;
int esperado;
int ultimo_erro;
float KP;
float KD;
conta_tempo t_pd(50);

// Funcoes
void 	inicia_odometro();
// calibra as velocidades usando PD
void calibra_motor ();
void corretor();
