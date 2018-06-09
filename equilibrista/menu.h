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
--|- Matheus Santos
--------------------------------------------*/

//  Funcoes

// Gerenciador do menu e suas opcoes
void 	menu();
