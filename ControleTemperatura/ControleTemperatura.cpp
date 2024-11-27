#include "Arduino.h"
#include "ControleTemperatura.h"
#include "DHT.h"

#define DHTTYPE DHT11  // Definindo o tipo de sensor DHT11

//Função inicial
ControleTemperatura::ControleTemperatura(int pino_dht, int pino_saida1, int pino_saida2, int pino_pwm, int pino_rele) {
  _pino_dht = pino_dht;
  _pino_saida1 = pino_saida1;
  _pino_saida2 = pino_saida2;
  _pino_pwm = pino_pwm;
  _pino_rele = pino_rele; 
  _modo = 0;
  _manual_motor = LOW;
  _pwm_atual = 0;
  _dht = new DHT(_pino_dht, DHTTYPE);
  _dht->begin();  // Inicializa o sensor DHT
  
  pinMode(_pino_saida1, OUTPUT);
  pinMode(_pino_saida2, OUTPUT);
  pinMode(_pino_pwm, OUTPUT);
  pinMode(_pino_rele, OUTPUT); 
}

//Função para definir o modo manual e automatico, além de controlar o motor e o relé no modo manual
void ControleTemperatura::ajustarModo(String comando_serial) {
  comando_serial.trim();
  if (comando_serial == "automatico") {
    _modo = 0;
  } 
  else if (comando_serial == "manual") {
    _modo = 1;
    Serial.println("Controle a ventilação com 'motor on' e 'motor off'.");
    Serial.println("Controle a umidade do ar com 'umi on' e 'umi off'.");
  } 

  else if (_modo == 1) {
    if (comando_serial == "motor on") {
      _manual_motor = HIGH;
      Serial.println("Motor ligado manualmente.");
    } else if (comando_serial == "motor off") {
      _manual_motor = LOW;
      Serial.println("Motor desligado manualmente.");
    } else if (comando_serial == "umi on") {
      digitalWrite(_pino_rele, LOW);
      Serial.println("Umidificador ligado manualmente.");
    } else if (comando_serial == "umi off") {
      digitalWrite(_pino_rele, HIGH);
      Serial.println("Umidificador desligado manualmente.");
    }
  }
}

//Função para partida do motor com rampa de subida (pwm)
void ControleTemperatura::ligarMotor() {
    if (_pwm_atual == 255) {
    return;
  }
  // Pino 6 low e pino 7 high ligam o motor
  digitalWrite(_pino_saida1, LOW);
  digitalWrite(_pino_saida2, HIGH);

  // Rampa de subida (aumenta o PWM de 0 a 255)
  for (int pwmValor = _pwm_atual; pwmValor <= 255; pwmValor += 5) {
    analogWrite(_pino_pwm, pwmValor);
    _pwm_atual = pwmValor;  // Atualiza o valor atual do PWM
    delay(10);  // Tempo entre os incrementos para suavizar a aceleração
  }
}

//Função para controlar o led no modo automatico
void ControleTemperatura::atualizar() {
  if (_modo == 0) {
    float temperatura = _dht->readTemperature();  // Lê a temperatura em Celsius
    float umidade = _dht->readHumidity(); // Lê a umidade em %

    if (isnan(temperatura) || isnan(umidade)) {
      Serial.println("Falha ao ler do sensor DHT");
      return;
    }

    if (umidade < 50) {
      digitalWrite(_pino_rele, LOW); 
    } else {
      digitalWrite(_pino_rele, HIGH); 
    }

    if (temperatura > 50) {
      ligarMotor();

    } else {
      digitalWrite(_pino_saida1, LOW);  
      digitalWrite(_pino_saida2, LOW);  
      analogWrite(_pino_pwm, 0);
      _pwm_atual = 0; //redefine o a variavel do pwm para zero, possibilitando iniciar a rampa novamente
    }
  } else if (_modo == 1) {
    if (_manual_motor == HIGH) {
      ligarMotor();
      
    } else {
      digitalWrite(_pino_saida1, LOW);  
      digitalWrite(_pino_saida2, LOW);   
      analogWrite(_pino_pwm, 0);
      _pwm_atual = 0; //redefine o a variavel do pwm para zero, possibilitando iniciar a rampa novamente
    }
  }
}

//Função para leitura do sensor para variavel temperatura
int ControleTemperatura::lerTemp() {
  float temperatura = _dht->readTemperature(); 
  return temperatura; 
}

//Função para leitura do sensor para variavel umidade
int ControleTemperatura::lerUmi() {
  float umidade = _dht->readHumidity();
  return umidade; 
}