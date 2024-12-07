#ifndef MQTTPub_h
#define MQTTPub_h

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>

class MQTTPub {
  public:
    MQTTPub(const char* ssid, const char* password, const char* mqtt_server, int mqtt_port, const char* mqtt_topic);
    void iniciar();
    void publicar(int modo, float temperatura, int umidade, int luminosidade, int umisolo, int valor_led, String estado_motor, String estado_rele, String estado_servo);
  
  private:
    const char* _ssid;
    const char* _password;
    const char* _mqtt_server;
    int _mqtt_port;
    const char* _mqtt_topic;
    
    WiFiClient espClient;
    PubSubClient mqttClient;

    unsigned long lastPublishTime;   // Variável para controlar o tempo de publicação
    unsigned long publishInterval;   // Intervalo de tempo entre publicações (em milissegundos)

    void conectarWiFi();
    void reconectarMQTT();
};

#endif
