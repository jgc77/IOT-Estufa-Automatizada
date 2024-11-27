#include "Arduino.h"
#include "ControleIrrigacao.h"
#include "Servo.h"

//Função inicial
ControleIrrigacao::ControleIrrigacao(int pino_sens, int iservo) {

  _pino_sens = pino_sens;
  this->_iservo = iservo;
  _modo = 0; // Modo automático por padrão
  ultimoTempoAtualizacao = 0; // Inicializa a última atualização
  umidadeAtual = 0; // Valor inicial da umidade
  pinMode(iservo, OUTPUT);
}

//Função para iniciar o servo
void ControleIrrigacao::iniciar() {
  servo_motor.attach(_iservo); // Inicia o servo
  servo_motor.write(0);
}

//Função para definir o modo manual e automatico, além de controlar o servo no modo manual
void ControleIrrigacao::ajustarModo(String comando_serial) {
  comando_serial.trim(); // Remove espaços em branco ou novas linhas
  if (comando_serial == "automatico") {
    _modo = 0;
  } 
  else if (comando_serial == "manual") {
    _modo = 1;
  }
  else if (_modo == 1) {
     if (comando_serial == "servo on") {
       servo_motor.write(90);
       Serial.println("Servo ligado manualmente.");

      } else if (comando_serial == "servo off") {
        servo_motor.write(0);
        Serial.println("Servo desligado manualmente.");
      }
  }
}

//Função para controlar o servo no modo automatico
void ControleIrrigacao::atualizar() {
  if (_modo == 0) {
    int valorumidade = analogRead(_pino_sens); // Lê o sensor (0 a 1023)
    //Remover comentario para testes:
    //Serial.print("Valor do sensor de umidade: ");
    //Serial.println(valorumidade);
    if (valorumidade > 511) {
      servo_motor.write(90);
    } else {
      servo_motor.write(0);
    }
  }
}

//Função para leitura do sensor para variavel umidadeAtual
int ControleIrrigacao::lersolo() {
  if (millis() - ultimoTempoAtualizacao >= intervaloAtualizacao) {
    ultimoTempoAtualizacao = millis();
    int leituraAnalogica = analogRead(_pino_sens); // Atualiza a leitura
    umidadeAtual = map(leituraAnalogica, 1023, 0, 0, 100); // Converte para porcentagem
  }
  return umidadeAtual; // Retorna o valor em porcentagem
}