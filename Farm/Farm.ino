#include <ControleIrrigacao.h>
#include <ControleTemperatura.h>
#include <ControleLuminosidade.h>
#include <MenuLCD.h>
#include <MQTTPub.h>
#include "BluetoothSerial.h"

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

int mod = 0;

//const char* ssid = "Xiaomi 11 Lite 5G";
const char* ssid = "PISCINA ";  // Nome do ponto de acesso
//const char* password = "joaojoao123";
const char* password = "38473144";          // Senha do ponto de acesso
const char* mqtt_server = "192.168.30.19";  // Usando um broker público como exemplo
//const char* mqtt_server = "192.168.86.150";  // Usando um broker público como exemplo
const int mqtt_port = 1885;  // Porta padrão MQTT
const char* mqtt_topic = "dados";

//Definir tempo do led power
unsigned long previousMillis = 0;
const long interval = 300;  // Intervalo de piscar (ms)

// Criar instâncias das bibliotecas
MenuLCD menu(0x27, 16, 2, botao1, botao2);                             // Menu para o LCD 16x2
ControleLuminosidade controleLuminosidade(led_uv, ldr);                // Controle de luminosidade
ControleTemperatura controle(dht11, motor1, motor2, motor_pwm, rele);  // Controle de Temperatura
ControleIrrigacao controleIrrigacao(34, 27);
MQTTPub mqttPub(ssid, password, mqtt_server, mqtt_port, mqtt_topic);  //wifi e envio de dados mqtt

// Bluetooth Serial
BluetoothSerial SerialBT;

void setup() {
  // Iniciar os módulos de menu e controle serial
  mqttPub.iniciar();
  menu.iniciar();
  controleIrrigacao.iniciar();
  Serial.begin(115200);
  pinMode(26, OUTPUT);                    //LED de Power
  SerialBT.begin("Estufa Automatizada");  // Nome do dispositivo Bluetooth
  Serial.println("ESTUFA AUTOMATIZADA");
  SerialBT.println("ESTUFA AUTOMATIZADA");
  Serial.println("Bem vindo, escolha entre o modo manual e automatico.");
  SerialBT.println("Bem vindo, escolha entre o modo manual e automatico.");
}

void loop() {

  //LED power
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    digitalWrite(26, !digitalRead(26));  // Alterna o estado do LED
  }

  // Ler dados do Serial ou do Bluetooth
  if (Serial.available() > 0 || SerialBT.available() > 0) {
    String comando;
    if (Serial.available() > 0) {
      comando = Serial.readStringUntil('\n');
    } else if (SerialBT.available() > 0) {
      comando = SerialBT.readStringUntil('\n');
    }

    // Remover espaços e caracteres invisíveis
    comando.trim();  // Remove espaços em branco no início/fim e caracteres como \r\n

    // Processar comando
    if (comando == "automatico") {
      Serial.println("Modo automatico ativado.");
      mod = 0;
    } else if (comando == "manual") {
      Serial.println("Modo manual ativado.");
      mod = 1;
    }

    // Exibir o modo no Serial Bluetooth
    if (mod == 0) {
      SerialBT.println("Modo automatico ativado.");
    } else if (mod == 1) {
      SerialBT.println("Modo manual ativado.");
    }

    // Ajustar modos
    controleLuminosidade.ajustarModo(comando);
    controle.ajustarModo(comando);
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
  int valor_led = controleLuminosidade.lerLED();
  String estado_motor = controle.EstadoMotor();
  String estado_rele = controle.EstadoRele();
  String estado_servo = controleIrrigacao.EstadoServo();
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

  String modo = (mod == 1) ? "Manual" : "Automático";
  mqttPub.publicar(modo, temperatura, umidade, luminosidade, umisolo, valor_led, estado_motor, estado_rele, estado_servo);

  // delay para aliviar o loop
  delay(100);
}