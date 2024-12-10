# Estufa Automatizada

Este repositório contém a 2ª Parte do Projeto de Sistemas Embarcados, expandindo a funcionalidade da estufa automatizada com o uso do ESP32 para envio de dados, integração com Node-RED e armazenamento em um banco de dados PostgreSQL. A visualização das informações em tempo real é feita via Metabase e o controle do sistema pode ser realizado através de comandos enviados por Bluetooth.

## Estrutura do Repositório

- **Farm**: Código principal que integra todas as funcionalidades da estufa automatizada.
- **MenuLCD**: Biblioteca para navegação e controle de menus em um display LCD.
- **ControleLuminosidade**: Biblioteca para controle da intensidade da iluminação via PWM.
- **ControleTemperatura**: Biblioteca para monitoramento e controle da temperatura e umidade do ar.
- **ControleIrrigacao**: Biblioteca para controle da umidade do solo, acionando um servo motor.
- **MQTTPub**: Biblioteca para conexão WI-FI e envio de dados MQTT.

## Instruções de Instalação

1. Copie as pastas **MenuLCD**, **ControleLuminosidade**, **ControleTemperatura**, **ControleIrrigacao** e **MQTTPub** para a pasta **libraries** do Arduino.
2. Carregue o código principal na pasta **Farm** para o Arduino.
3. Edite o Farm.ino com seus dados de wifi, senha e ip.

## Instruções de Utilização modo manual (via Serial e Serial Bluetooth)

- **Modos de Operação**: Escolha entre modo **manual** e **automático**.
- **Luminosidade**: Controle a intensidade do LED com o comando `led` seguido da porcentagem desejada. Exemplo: `led 65`.
- **Ventilador**: Controle o ventilador com `motor on` (ligar) e `motor off` (desligar).
- **Umidificador**: Use `umi on` para ligar e `umi off` para desligar o umidificador.
- **Irrigação**: Controle a irrigação com `servo on` (ativar) e `servo off` (desativar).

## Navegação em Modo Automático

- **Botão Esquerdo**: Confirma a seleção ou retorna ao menu anterior.
- **Botão Direito**: Navega para a direita no menu ou retorna à opção anterior.

## Portas Utilizadas
- **D04**: DHT11  
- **D13**: LED UV  
- **D18**: Motor 1  
- **D19**: Motor 2  
- **D21**: Display (I2C - SDA)  
- **D22**: Display (I2C - SCL)  
- **D23**: PWM  
- **D25**: Relé  
- **D26**: LED Power  
- **D27**: PWM Servo  
- **D32**: Botão 2 (B2)  
- **D33**: Botão 1 (B1)  
- **D34**: Umidade do Solo (Sensor Analógico)  
- **D36**: LDR (Sensor de Luminosidade - Analógico)  

## Links:
- https://github.com/jgc77/Estufa-Automatizada
- https://www.tinkercad.com/things/kj5nUbN28iH-estufa-automatizada?sharecode=ryG8-UEUu79qbrlhKUh0y8Mcmv00TgAfoeZEw4P74pI
