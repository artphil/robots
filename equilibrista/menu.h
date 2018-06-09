#include "ambiente.h"
/*-------------------------------------------
SUMARIO do MENU
- Mascara (Sublinha em branco)
- Iniciar
--|- Iniciar(aciona funcionanento principal)
- Configurar
--|- Movimento
-----|- pot ME (potencia do motor da roda esquerda)
-----|- pot MD (potencia do motor da roda direita)
-----|- anda (tempo para o robor andar Xcm)
-----|- giro90 (tempo para o robo girar 90 graus)
--|- Visao
-----|- Balanco branco
-----|- Balanco preto
-----|- LDR Lmt (fator de corte para encontrar objetos)
--|- Odometro
-----|- KP (fator de correcao de giro da roda)
-----|- KD (fator de correcao de giro da roda)
-----|- Calibra motor (funcao odometrica)
--|- GRAVAR (grava dados no EEPROM)
--|- LE SALVO (le dados do EEPROM)
- Informacoes
--|- LDR2 (Intencidade refletida do ldr duplo)
--|- LDR dir (lado do ldr duplo com mais luz)
--|- RGB (Intencidade refletida por cor(0-255))
--|- LDR (Intencidade refletida atual)
--|- Objeto (Existe objeto na frente do robo)
--|- Bateria
--|- Tempo (tempo de fincionam. do arduino em ms)
- Testes
--|- RGB (acionamento das luzes)
--|- Anda F/T (acionamento manual)
--|- Gira D/E (acionamento manual)
--|- Motores auto (Movimentos pre estabelecidos)
- Pre Moves
--|- Vai Vem
--|- Triangulo
--|- Quadrado
--|- Giro 90
--|- Busca Luz (Aponta para a direcao com maior intencidade de luz)
- Creditos
--|- Arthur P Silva
--|- Bernardo Sanches
--|- Gustavo Campos
// Variaveis
--|- Matheus Santos
--------------------------------------------*/

// Objeto Display
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int 	botao;			// Botao pressionado no momento
int 	tst;			// intica qual teste esta sendo realizado
int 	estado_menu;	// Posicao na pilha do menu
int 	estado_antigo;	// Posicao anterior na pilha do menu
int 	estado_liga;	// Posicao na pilha do menu 11
conta_tempo t_menu(180); 	// Contador de tempo do menu
conta_tempo t_liga(1000);	// Contador de tempo do menu 11
// Interacao do menu CREDITOS
int letra = 0;
bool desliza = true;
conta_tempo t_nome(400);	// Contador de tempo do menu 11

//  Funcoes
void 	menu();				// Gerenciador do menu e suas opcoes
int 	verifica_botao (); 	// Identifica qual botao foi acionado
void 	inicia_menu();
