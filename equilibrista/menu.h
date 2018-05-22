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
-----|- anda30 (tempo para o robor andar 30cm)
-----|- giro90 (tempo para o robo girar 90 graus)
--|- Visao
-----|- Balanco branco
-----|- Balanco preto
-----|- LDR Lmt (fator de corte para encontrar objetos)
--|- GRAVAR (grava dados no EEPROM)
--|- LE SALVO (le dados do EEPROM)
- Informacoes
--|- RGB (Intencidade refletida por cor(0-255))
--|- LDR (Intencidade refletida atual)
--|- Objeto (Existe objeto na frente do robo)
--|- Bateria
--|- Tempo (tempo de fincionam. do arduino em ms)
- Testes
--|- Motores auto (Movimentos pre estabelecidos)
--|- Anda F/T (acionamento manual)
--|- Gira D/E (acionamento manual)
--|- RGB (acionamento das luzes)
- Pre Moves
--|- Vai Vem
--|- Triangulo
--|- Quadrado
- Creditos
--|- Arthur P Silva
--|- Bernardo Sanches
--|- Gustavo Campos
--|- Matheus Santos
--------------------------------------------*/

//  Funcoes

// Gerenciador do menu e suas opcoes
void 	menu();
