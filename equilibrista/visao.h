#include "ambiente.h"
// Leds e sensor
int 	led;			// Led aceso
bool 	led_atual[3];	// Verifica se a mudou a os leds
int 	cor;			// Cor encontrada
String	nome_cor[] =
{
	"Vermelho",
	"Verde",
	"Azul",
	"Amarelo",
	"Branco",
	"Preto"
};						// Nome das cores

int 	ldr_cor[3];		// Taxa de reflexao de cor (Red Green Blue)
int 	ldr_branco[3];	// Taxa de reflexao de cor (Red Green Blue)
int 	ldr_preto[3];	// Taxa de reflexao de cor (Red Green Blue)
int 	ldr_valor;		// Intencidade de luz captada
int 	ldr_limiar;		// Valor limite para ver objeto
bool	objeto;			// Informa se tem algo na frente do robo
bool	vazio = true;			// Informa se tem algo na frente do robo

int 	ldr_start;		// Intencidade de luz captada

int 	ldr_difer;	// Intencidade de luz captada no ldr direito
int 	ldr_direcao;	// Direcao da maior intencidade de luz
int 	ldr_direcao_base;	// Direcao da maior intencidade de luz
int 	ldr_dif_max;	// Direcao da maior intencidade de luz
int 	ldr_dif_max_base;	// Direcao da maior intencidade de luz
int 	luz=0;

int 	otico_direita;	// Intencidade de luz captada no otico direito
int 	otico_esquerda;	// Intencidade de luz captada no otico esquerdo
int 	otico_direcao;	// Direcao da maior intencidade de luz
int 	otico_cor;		// Cor Identificada
int 	otico_cor_ultima;// Ultima cor Identificada

float 	carga;			// Tensão real da fonte

// Funcoes
void 	inicia_sensor();
// Controla o acionamento das luzes
int 	aciona_luz(bool, bool, bool);
// Retorna a media de leituras do sensor
int 	media_sensor(int);
// Percebe um obstaculo
bool 	ve_objeto();
// Percebe uma cor
void 	ve_cor();
// Calibra o receptor de luz
void 	balanco_branco();
void 	balanco_preto();
