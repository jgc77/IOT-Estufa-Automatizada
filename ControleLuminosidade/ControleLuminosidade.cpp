#include "Arduino.h"
#include "ControleLuminosidade.h"

ControleLuminosidade::ControleLuminosidade(int pino_led, int pino_ldr) {
  _pino_led = pino_led;
  _pino_ldr = pino_ldr;
  _modo = 0; // Modo automático por padrão
  _valor_led_manual = 0;
  _potencia = 0;
  pinMode(_pino_led, OUTPUT);
}

void ControleLuminosidade::ajustarModo(String comando_serial) {
  comando_serial.trim(); // Remove espaços em branco ou novas linhas
  if (comando_serial == "automatico") {
    _modo = 0;
  } 
  else if (comando_serial == "manual") {
    _modo = 1;
    Serial.println("Controle a luminosidade com valores de 0 a 100. Exemplo: 'led 33'.");
  }
  else if (_modo == 1 && comando_serial.startsWith("led")) {
    int porcentagem = comando_serial.substring(4).toInt(); // Extrai o valor após 'led '
    if (porcentagem >= 0 && porcentagem <= 100) {
      _valor_led_manual = map(porcentagem, 0, 100, 0, 255); // Converte para PWM (0-255)
      analogWrite(_pino_led, _valor_led_manual);
      _potencia = porcentagem; //Variavel para o banco de dados
      Serial.print("Luminosidade manual ajustada para: ");
      Serial.println(porcentagem);
    } else {
      Serial.println("Valor invalido! Use entre 0 e 100.");
    }
  }
}

void ControleLuminosidade::atualizar() {
  if (_modo == 0) {
    int valor_ldr = analogRead(_pino_ldr); // Lê o LDR (0 a 1023)
    int brilho_led;
    if (valor_ldr > 800) {
      brilho_led = 0; // Desliga o LED se a leitura for maior que 800
    } else {
      brilho_led = map(valor_ldr, 0, 800, 255, 0); // Mapeia para PWM (0-800)
    }
    analogWrite(_pino_led, brilho_led);
    _potencia = map(brilho_led, 0, 255, 0, 100); //Variavel para o banco de dados
  }
}

int ControleLuminosidade::lerLDR() {
  return analogRead(_pino_ldr); // Retorna o valor do LDR
}

int ControleLuminosidade::lerLED() {
  return _potencia; // Retorna a potência em porcentagem (banco de dados)
}