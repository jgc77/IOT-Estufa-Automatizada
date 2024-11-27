#include <ControleIrrigacao.h>
#include <ControleTemperatura.h>
#include <ControleLuminosidade.h>
#include <MenuLCD.h>

// Definir os pinos dos botões
const uint8_t botao1 = 2;
const uint8_t botao2 = 3;

// Definir os pinos do LDR e LED
const int led_uv = 6;
const int ldr = A0;

// Definir os pinos de temperatura
const int dht11 = 4;
const uint8_t motor1 = 9;
const uint8_t motor2 = 7;
const int motor_pwm = 10;
const int rele = 8;

//Definir tempo do led power 
unsigned long previousMillis = 0; 
const long interval = 300; // Intervalo de piscar (ms)

// Criar instâncias das bibliotecas
MenuLCD menu(0x27, 16, 2, botao1, botao2);               // Menu para o LCD 16x2
ControleLuminosidade controleLuminosidade(led_uv, ldr);  // Controle de luminosidade
ControleTemperatura controle(dht11, motor1, motor2, motor_pwm, rele);     // Controle de Temperatura
ControleIrrigacao controleIrrigacao(A1, 11);

void setup() {
  // Iniciar os módulos de menu e controle serial
  menu.iniciar();
  controleIrrigacao.iniciar();
  Serial.begin(9600);
  pinMode(5, OUTPUT);  //LED de Power
  Serial.println("ESTUFA AUTOMATIZADA");
  Serial.println("Bem vindo, escolha entre o modo manual e automatico.");
}

void loop() {

  //LED power
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    digitalWrite(5, !digitalRead(5)); // Alterna o estado do LED
  }

  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    if (comando == "automatico") {
      Serial.println("Modo automatico ativado.");

    } else if (comando == "manual") {
      Serial.println("Modo manual ativado");
    }

    controleLuminosidade.ajustarModo(comando);  //luminosidade
    controle.ajustarModo(comando);              //temperatura
    controleIrrigacao.ajustarModo(comando);
  }
  controleLuminosidade.atualizar();
  controle.atualizar();  //temperatura
  controleIrrigacao.atualizar();

  // Obter o valor da luminosidade lida pelos sensores
  int luminosidade = controleLuminosidade.lerLDR();
  int temperatura = controle.lerTemp();
  int umidade = controle.lerUmi();
  int umisolo = controleIrrigacao.lersolo();

  // Atualizar o valor dos sensores na biblioteca de menu
  menu.setLuminosidade(luminosidade);
  menu.setTemperatura(temperatura);
  menu.setUmidade(umidade);
  menu.setUmisolo(umisolo);

  // Atualizar o menu (navegação e exibição)
  menu.atualizar();

  // Atualizar a luminosidade na tela
  menu.atualizarLuminosidade();
  menu.atualizarTemperatura();
  menu.atualizarUmidade();
  menu.atualizarUmisolo();

  // delay para aliviar o loop
  delay(100);
}