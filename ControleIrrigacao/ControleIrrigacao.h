#ifndef ControleIrrigacao_h
#define ControleIrrigacao_h

#include "Arduino.h"
#include "Servo.h"

class ControleIrrigacao {
  public:
    ControleIrrigacao(int pino_sens, int iservo);
    void ajustarModo(String comando_serial);
    void atualizar();
    void iniciar();
    int lersolo();  // Função para retornar a leitura do LDR
  private:
    //int _pino_servo;
    int _pino_sens;
    int _modo; // 0 = Automático, 1 = Manual
    int _iservo;
    //int _valor_servo_manual;
    Servo servo_motor;
    //int umidade_limite; 
    unsigned long ultimoTempoAtualizacao;
    const unsigned long intervaloAtualizacao = 1000; // Intervalo de 1 segundo
    int umidadeAtual; // Armazena o último valor lido da umidade
};

#endif
