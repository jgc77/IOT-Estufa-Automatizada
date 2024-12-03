#include <ControleIrrigacao.h>
#include <ControleTemperatura.h>
#include <ControleLuminosidade.h>
#include <MenuLCD.h>
#include <MQTTPub.h>

// Definir os pinos dos botões
const uint8_t botao1 = 32;
const uint8_t botao2 = 33;

// Definir os pinos do LDR e LED
const int led_uv = 13;
const int ldr = 36;

// Definir os pinos de temperatura
const int dht11 = 4;
const uint8_t motor1 = 18;
const uint8_t motor2 = 19;
const int motor_pwm = 23;
const int rele = 25;

int modo = 0;

const char* ssid = "PISCINA ";              // Nome do ponto de acesso
const char* password = "38473144";          // Senha do ponto de acesso
const char* mqtt_server = "192.168.30.19";  // Usando um broker público como exemplo
const int mqtt_port = 1885;                 // Porta padrão MQTT
const char* mqtt_topic = "dados";

//Definir tempo do led power
unsigned long previousMillis = 0;
const long interval = 300;  // Intervalo de piscar (ms)

// Criar instâncias das bibliotecas
MenuLCD menu(0x27, 16, 2, botao1, botao2);                             // Menu para o LCD 16x2
ControleLuminosidade controleLuminosidade(led_uv, ldr);                // Controle de luminosidade
ControleTemperatura controle(dht11, motor1, motor2, motor_pwm, rele);  // Controle de Temperatura
ControleIrrigacao controleIrrigacao(34, 27);
MQTTPub mqttPub(ssid, password, mqtt_server, mqtt_port, mqtt_topic);

void setup() {
  // Iniciar os módulos de menu e controle serial
  mqttPub.iniciar();
  menu.iniciar();
  controleIrrigacao.iniciar();
  Serial.begin(9600);
  pinMode(26, OUTPUT);  //LED de Power
  Serial.println("ESTUFA AUTOMATIZADA");
  Serial.println("Bem vindo, escolha entre o modo manual e automatico.");
}

void loop() {

  //LED power
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    digitalWrite(26, !digitalRead(26));  // Alterna o estado do LED
  }

  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    if (comando == "automatico") {
      Serial.println("Modo automatico ativado.");
      modo = 0;

    } else if (comando == "manual") {
      Serial.println("Modo manual ativado");
      modo = 1;
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
  int valorled = controleLuminosidade.lerLED();
  //float tensao = random(400, 501) / 100.0;  // 4.00 a 5.00 V

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

  mqttPub.publicar(modo, temperatura, umidade, luminosidade, umisolo, valorled);

  // delay para aliviar o loop
  delay(100);
}