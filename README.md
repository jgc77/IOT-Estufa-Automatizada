# Estufa Automatizada

Este repositório contém o **1º Projeto de Sistemas Embarcados** focado no desenvolvimento de uma estufa automatizada usando a plataforma Arduino. O sistema monitora e controla variáveis críticas para o cultivo, como temperatura, umidade, luminosidade e irrigação, promovendo um ambiente otimizado para o crescimento das plantas.

## Estrutura do Repositório

- **Farm**: Código principal que integra todas as funcionalidades da estufa automatizada.
- **MenuLCD**: Biblioteca para navegação e controle de menus em um display LCD.
- **ControleLuminosidade**: Biblioteca para controle da intensidade da iluminação via PWM.
- **ControleTemperatura**: Biblioteca para monitoramento e controle da temperatura e umidade do ar.
- **ControleIrrigacao**: Biblioteca para controle da umidade do solo, acionando um servo motor.

## Instruções de Instalação

1. Copie as pastas **MenuLCD**, **ControleLuminosidade**, **ControleTemperatura** e **ControleIrrigacao** para a pasta **libraries** do Arduino.
2. Carregue o código principal na pasta **Farm** para o Arduino.

## Instruções de Utilização modo manual (via Serial)

- **Modos de Operação**: Escolha entre modo **manual** e **automático**.
- **Luminosidade**: Controle a intensidade do LED com o comando `led` seguido da porcentagem desejada. Exemplo: `led 65`.
- **Ventilador**: Controle o ventilador com `motor on` (ligar) e `motor off` (desligar).
- **Umidificador**: Use `umi on` para ligar e `umi off` para desligar o umidificador.
- **Irrigação**: Controle a irrigação com `servo on` (ativar) e `servo off` (desativar).

## Navegação em Modo Automático

- **Botão Esquerdo**: Confirma a seleção ou retorna ao menu anterior.
- **Botão Direito**: Navega para a direita no menu ou retorna à opção anterior.

## Links:
- https://github.com/jgc77/Estufa-Automatizada
- https://www.tinkercad.com/things/kj5nUbN28iH-estufa-automatizada?sharecode=ryG8-UEUu79qbrlhKUh0y8Mcmv00TgAfoeZEw4P74pI
