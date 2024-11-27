#ifndef ControleTemperatura_h
#define ControleTemperatura_h

#include "Arduino.h"
#include "DHT.h"

class ControleTemperatura {
  public:
    ControleTemperatura(int pino_dht, int pino_saida1, int pino_saida2, int pino_pwm, int _pino_rele);  // Adicione o pino PWM
    void ajustarModo(String comando_serial);  // Função para ajustar o modo
    void atualizar();  // Função para atualizar e verificar a temperatura
    int lerTemp();  // Função para retornar a leitura da temp
    int lerUmi();
    void ligarMotor();
  private:
    int _pino_dht;
    uint8_t _pino_saida1;
    uint8_t _pino_saida2;
    int _pino_pwm;  
    int _pwm_atual;
    int _pino_rele;
    int _modo;  // 0 = Automático, 1 = Manual
    int _manual_motor;  // Estado manual do motor
    DHT* _dht;
};

#endif
