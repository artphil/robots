/* bibliotecas */
#include <LiquidCrystal.h>  

/* variaveis globais */
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int estado_menu;
int volume;

int DIREITA = 1;
int ESQUERDA = 2;
int CIMA = 3;
int BAIXO = 4;
int SELECIONA = 5;


int verifica_botao () {
  int botao;  
  botao = analogRead (0);  //Leitura do valor da porta analógica A0
 if (botao < 50) {       // DIREITA
    return DIREITA;
  }  
  else if (botao < 100) { // CIMA
    return CIMA;
  }  
  else if (botao < 300){  // BAIXO 
    return BAIXO;
  }  
  else if (botao < 500){  // ESQUERDA 
    return ESQUERDA;  
  }  
  else if (botao < 700){  // SELECIONA 
    return SELECIONA;
  }
  else {
      return 0;  
  }
}

void init_menu() {
  estado_menu = 0;
  volume = 5;
  lcd.begin(16, 2);  
}

void menu() {
  int botao = verifica_botao();

  if (estado_menu < 10) {
    if (estado_menu == 0){
      lcd.setCursor(0,0);
      lcd.print ("    MEGAZORD    ");
      lcd.setCursor(0,1);
      lcd.print ("                ");

      if (botao == BAIXO) {
        estado_menu = 1;
      }
      else if (botao == CIMA) {
        estado_menu = 2;
      }
      delay(200);
    }
    else if (estado_menu == 1){
      lcd.setCursor(0,0);
      lcd.print ("    MEGAZORD    ");
      lcd.setCursor(0,1);
      lcd.print ("Iniciar         ");

      if (botao == BAIXO) {
        estado_menu = 2;
      }
      else if (botao == CIMA) {
        estado_menu = 0;
      }
      else if (botao == SELECIONA) {
        lcd.setCursor(1,1);
        lcd.print ("Iniciando em");
        for (int tempo=3; tempo>1; tempo--){
          lcd.setCursor(13,1);
          lcd.print (tempo);
        }
        lcd.setCursor(0,1);
        lcd.print ("Morfando ...    ");
        estado_menu = 0;

        delay(5000); // Aqui entra a função que vai desenhar a palavra
      }
      delay(200);
    }
    else if (estado_menu == 2){
      lcd.setCursor(0,0);
      lcd.print ("    MEGAZORD    ");
      lcd.setCursor(0,1);
      lcd.print ("Configurar      ");

      if (botao == BAIXO) {
        estado_menu = 0;
      }
      else if (botao == CIMA) {
        estado_menu = 1;
      }
      else if (botao == SELECIONA) {
        estado_menu = 21;
      }
      delay(200);
    }
  }
  else  if (estado_menu < 30) {
    if (estado_menu == 21){
      lcd.setCursor(0,0);
      lcd.print ("   CONFIGURAR   ");
      lcd.setCursor(0,1);
      lcd.print ("Volume  -     +");
      lcd.setCursor(11,1);
      lcd.print (volume);

      if (botao == BAIXO) {
        estado_menu = 22;
      }
      else if (botao == CIMA) {
        estado_menu = 21;
      }
      else if (botao == DIREITA) {
        volume++;      
      }
      else if (botao == ESQUERDA) {
        volume--;
      }
      delay(200);
    }
    else if (estado_menu == 22){
      lcd.setCursor(0,0);
      lcd.print ("   CONFIGURAR   ");
      lcd.setCursor(0,1);
      lcd.print ("Voltar          ");

      if (botao == BAIXO) {
        estado_menu = 22;
      }
      else if (botao == CIMA) {
        estado_menu = 21;
      }
      else if (botao == SELECIONA) {
        estado_menu = 0;      
      }
      delay(200);
    }
  }
  
}
void setup() {
  // put your setup code here, to run once:
  init_menu();
}

void loop() {
  // put your main code here, to run repeatedly:
  menu();
}
