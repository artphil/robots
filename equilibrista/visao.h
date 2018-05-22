#include "ambiente.h"
// Leds e sensor

// Controla o acionamento das luzes
int 	aciona_luz(bool, bool, bool);
// Retorna a media de leituras do sensor
int 	media_sensor(int);
// Percebe um obstaculo
bool 	ve_objeto();
// Percebe uma cor
void 	ve_cor();
// Calibra o receptor de luz
void balanco_branco();
void balanco_preto();
