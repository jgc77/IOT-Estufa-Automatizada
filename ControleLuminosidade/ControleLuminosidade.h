#ifndef ControleLuminosidade_h
#define ControleLuminosidade_h

#include "Arduino.h"

class ControleLuminosidade {
  public:
    ControleLuminosidade(int pino_led, int pino_ldr);
    void ajustarModo(String comando_serial);
    void atualizar();
    int lerLDR();  // Função para retornar a leitura do LDR
  private:
    int _pino_led;
    int _pino_ldr;
    int _modo; // 0 = Automático, 1 = Manual
    int _valor_led_manual;
};

#endif
