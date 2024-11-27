#include "MenuLCD.h"

/////////////////////////////////////////////////////
// Logomarca em matriz 
byte SIMB1[8] = {
  B00000,
  B00000,
  B00001,
  B00001,
  B00010,
  B00010,
  B00110,
  B00100
};

byte SIMB2[8] = {
  B01110,
  B10001,
  B10001,
  B00100,
  B00100,
  B01110,
  B01110,
  B00100
};

byte SIMB3[8] = {
  B00000,
  B00000,
  B10000,
  B10000,
  B01000,
  B01000,
  B01100,
  B00100
};

byte SIMB4[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};


byte SIMB5[8] = {
  B00100,
  B00100,
  B00100,
  B00110,
  B00011,
  B00001,
  B00000,
  B00000
};

byte SIMB6[8] = {
  B10101,
  B11101,
  B01110,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100
};

byte SIMB7[8] = {
  B00100,
  B00100,
  B00100,
  B01100,
  B11000,
  B10000,
  B00000,
  B00000
};

byte SIMB8[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
/////////////////////////////////////////////////////


MenuLCD::MenuLCD(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t btn1_pin, uint8_t btn2_pin)
  : lcd(lcd_addr, lcd_cols, lcd_rows), botao_1(btn1_pin), botao_2(btn2_pin) {
  menu_atual = 4;
  dentro_submenu = false;
  estado_anterior_botao_1 = HIGH;
  estado_anterior_botao_2 = HIGH;
  temperatura = 0;
  umidade = 0;
  luminosidade = 0;
  umisolo = 0;
}

// Função para iniciar o LCD e definir botões
void MenuLCD::iniciar() {
  lcd.init();  // Iniciar o LCD
  lcd.backlight();  // Ligar o backlight
  pinMode(botao_1, INPUT_PULLUP);  // Configurar botão 1 como entrada com pull-up
  pinMode(botao_2, INPUT_PULLUP);  // Configurar botão 2 como entrada com pull-up

//Alocando cada matrix como um símbolo 
  lcd.createChar(1, SIMB1);
  lcd.createChar(2, SIMB2);
  lcd.createChar(3, SIMB3);
  lcd.createChar(4, SIMB4);
  lcd.createChar(5, SIMB5);
  lcd.createChar(6, SIMB6);
  lcd.createChar(7, SIMB7);
  lcd.createChar(8, SIMB8);
 
  mostrarMenu();
}

void MenuLCD::atualizar() {
  int estado_botao_1 = digitalRead(botao_1);
  int estado_botao_2 = digitalRead(botao_2);

  // Verificar se o botão 2 foi pressionado (navegar entre os menus)
  if (estado_botao_2 == LOW && estado_anterior_botao_2 == HIGH) {
    if (dentro_submenu) {
      dentro_submenu = false;  // Voltar para o menu principal
      mostrarMenu();
    } else {
      menu_atual = (menu_atual + 1) % 5;  // Navegar pelos menus (Limite de 4 menus)
      mostrarMenu();
    }
    delay(200);  // Pequeno delay para evitar múltiplos cliques
  }

  // Verificar se o botão 1 foi pressionado (entrar no submenu ou voltar)
  if (estado_botao_1 == LOW && estado_anterior_botao_1 == HIGH) {
    if (dentro_submenu) {
      dentro_submenu = false;  // Sair do submenu e voltar ao menu principal
      mostrarMenu();
    } else {
      dentro_submenu = true;  // Entrar no submenu
      mostrarSubmenu();
    }
    delay(200);  // Pequeno delay para evitar múltiplos cliques
  }

  // Atualizar o estado anterior dos botões
  estado_anterior_botao_1 = estado_botao_1;
  estado_anterior_botao_2 = estado_botao_2;
}

void MenuLCD::atualizarLuminosidade() {
  if (dentro_submenu && menu_atual == 2) {  // Se estamos no submenu de luminosidade
    lcd.setCursor(0, 1);  // Mover o cursor para a linha 2
    lcd.print("            "); // Limpar a linha anterior
    lcd.setCursor(0, 1);
    lcd.print(luminosidade);
    lcd.print("Lux");
  }
}

void MenuLCD::atualizarTemperatura() {
  if (dentro_submenu && menu_atual == 0) {  // Se estamos no submenu de temperatura
    lcd.setCursor(0, 1);  // Mover o cursor para a linha 2
    lcd.print("            "); // Limpar a linha anterior
    lcd.setCursor(0, 1);
    lcd.print(temperatura);
    lcd.print("C");
    lcd.print((char)223);
  }
}

void MenuLCD::atualizarUmidade() {
  if (dentro_submenu && menu_atual == 1) {  // Se estamos no submenu de umidade
    lcd.setCursor(0, 1);  // Mover o cursor para a linha 2
    lcd.print("            "); // Limpar a linha anterior
    lcd.setCursor(0, 1);
    lcd.print(umidade);
    lcd.print("%");
  }
}

void MenuLCD::atualizarUmisolo() {
  if (dentro_submenu && menu_atual == 3) {  // Se estamos no submenu de umidade
    lcd.setCursor(0, 1);  // Mover o cursor para a linha 2
    lcd.print("            "); // Limpar a linha anterior
    lcd.setCursor(0, 1);
    lcd.print(umisolo);
    lcd.print("%");
  }
}

void MenuLCD::setTemperatura(int temp) {
  temperatura = temp;
}

void MenuLCD::setUmidade(int umid) {
  umidade = umid;
}

void MenuLCD::setLuminosidade(int lumi) {
  luminosidade = lumi;
}

void MenuLCD::setUmisolo(int umidso) {
  umisolo = umidso;
}

void MenuLCD::mostrarMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (menu_atual) {
    case 0:
      lcd.print("1. Temperatura");
      break;
    case 1:
      lcd.print("2. Umidade");
      break;
    case 2:
      lcd.print("3. Luminosidade");
      break;
    case 3:
      lcd.print("4.  Umidade do");
      lcd.setCursor(6, 1);
      lcd.print("solo");
      break;
    case 4:
      byte Count = 1;
      for (byte y = 0; y < 2; y++) {
        for (byte x = 0; x < 4; x++) {
          lcd.setCursor(x, y);
          lcd.write(Count);
           Count++;
        }
      }
      lcd.setCursor(4, 0);
      lcd.print("   ESTUFA");
      lcd.setCursor(4, 1);
      lcd.print("AUTOMATIZADA"); 
      break;

  }
}

void MenuLCD::mostrarSubmenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (menu_atual) {
    case 0:
      lcd.print("Temperatura:");
      lcd.setCursor(0, 1);
      lcd.print(temperatura);
      lcd.print("C");
      lcd.print((char)223);
      break;
    case 1:
      lcd.print("Umidade:");
      lcd.setCursor(0, 1);
      lcd.print(umidade);
      lcd.print("%");
      break;
    case 2:
      lcd.print("Luminosidade:");
      lcd.setCursor(0, 1);
      lcd.print(luminosidade);
      lcd.print("Lux");
      break;
    case 3:
      lcd.print("Umidade solo:");
      lcd.setCursor(0, 1);
      lcd.print(umisolo);
      lcd.print("%");
      break;
  }
}
