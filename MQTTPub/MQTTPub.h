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
    void publicar(String modo, float temperatura, int umidade, int luminosidade, int umisolo, int valor_led, String estado_motor, String estado_rele, String estado_servo);

  private:
    const char* _ssid;
    const char* _password;
    const char* _mqtt_server;
    int _mqtt_port;
    const char* _mqtt_topic;
    
    WiFiClient espClient;
    PubSubClient mqttClient;

    unsigned long lastPublishTime;    // Controle de tempo para publicações
    unsigned long publishInterval;    // Intervalo entre publicações (ms)
    unsigned long lastWifiAttempt;    // Controle de tempo para tentativa de reconexão ao Wi-Fi
    unsigned long lastMqttAttempt;    // Controle de tempo para tentativa de reconexão ao MQTT
    unsigned long wifiRetryInterval;  // Intervalo para tentar reconectar ao Wi-Fi (ms)
    unsigned long mqttRetryInterval;  // Intervalo para tentar reconectar ao MQTT (ms)

    void conectarWiFi();              // Função para conectar ao Wi-Fi
    void reconectarMQTT();            // Função para reconectar ao MQTT
    void sincronizarRelogio();        // Função para sincronizar o relógio via NTP
};

#endif
