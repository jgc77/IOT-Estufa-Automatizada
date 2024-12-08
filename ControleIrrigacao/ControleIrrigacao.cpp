#include "Arduino.h"
#include "ControleIrrigacao.h"
#include "ESP32Servo.h"
#include "BluetoothSerial.h"

extern BluetoothSerial SerialBT;  //inicia BLE pelo farm.io

ControleIrrigacao::ControleIrrigacao(int pino_sens, int iservo) {

  _pino_sens = pino_sens;
  this->_iservo = iservo;
  _modo = 0; // Modo automático por padrão
  ultimoTempoAtualizacao = 0; // Inicializa a última atualização
  umidadeAtual = 0; // Valor inicial da umidade
  pinMode(iservo, OUTPUT);
  _estado_servo = LOW;
}

void ControleIrrigacao::iniciar() {
  servo_motor.attach(_iservo); // Inicia o servo
  servo_motor.write(0);
}

void ControleIrrigacao::ajustarModo(String comando_serial) {
  comando_serial.trim(); // Remove espaços em branco ou novas linhas
  if (comando_serial == "automatico") {
    _modo = 0;
  } 
  else if (comando_serial == "manual") {
    _modo = 1;
    Serial.println("Controle a irrigação com 'servo on' e 'servo off'");
    SerialBT.println("Controle a irrigação com 'servo on' e 'servo off'");

  }
  else if (_modo == 1) {
     if (comando_serial == "servo on") {
       servo_motor.write(90);
       Serial.println("Servo ligado manualmente.");
       SerialBT.println("Servo ligado manualmente.");
       _estado_servo = true;

      } else if (comando_serial == "servo off") {
        servo_motor.write(0);
        Serial.println("Servo desligado manualmente.");
        SerialBT.println("Servo desligado manualmente.");
        _estado_servo = false;
      }
  }
}

void ControleIrrigacao::atualizar() {
  if (_modo == 0) {
    int valorumidade = analogRead(_pino_sens); // Lê o sensor (0 a 4095)
    if (valorumidade > 2047) {
      servo_motor.write(90);
      _estado_servo = true;
    } else {
      servo_motor.write(0);
      _estado_servo = false;
    }
  }
}

int ControleIrrigacao::lersolo() {
  if (millis() - ultimoTempoAtualizacao >= intervaloAtualizacao) {
    ultimoTempoAtualizacao = millis();
    int leituraAnalogica = analogRead(_pino_sens); // Atualiza a leitura
    umidadeAtual = map(leituraAnalogica, 4095, 0, 0, 100); // Converte para porcentagem
  }
  return umidadeAtual; // Retorna o valor em porcentagem
}

String ControleIrrigacao::EstadoServo() {
    return _estado_servo? "HIGH" : "LOW";
}