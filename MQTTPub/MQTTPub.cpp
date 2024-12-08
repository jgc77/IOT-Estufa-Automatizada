#include "MQTTPub.h"

MQTTPub::MQTTPub(const char* ssid, const char* password, const char* mqtt_server, int mqtt_port, const char* mqtt_topic)
  : _ssid(ssid), _password(password), _mqtt_server(mqtt_server), _mqtt_port(mqtt_port), _mqtt_topic(mqtt_topic), mqttClient(espClient) {
  lastPublishTime = 0;
  lastWifiAttempt = 0;
  lastMqttAttempt = 0;
  publishInterval = 1000;       // Intervalo para publicações (1 segundo)
  wifiRetryInterval = 10000;    // Intervalo para tentar reconectar ao Wi-Fi (10 segundos)
  mqttRetryInterval = 5000;     // Intervalo para tentar reconectar ao MQTT (5 segundos)
}

void MQTTPub::conectarWiFi() {
  if (WiFi.status() != WL_CONNECTED && millis() - lastWifiAttempt >= wifiRetryInterval) {
    lastWifiAttempt = millis();
    Serial.println("Conectando ao Wi-Fi...");
    WiFi.begin(_ssid, _password);
  }
}

void MQTTPub::reconectarMQTT() {
  if (!mqttClient.connected() && WiFi.status() == WL_CONNECTED && millis() - lastMqttAttempt >= mqttRetryInterval) {
    lastMqttAttempt = millis();
    Serial.println("Conectando ao MQTT...");
    if (mqttClient.connect("ESP32_ClientID")) {
      Serial.println("Conectado ao MQTT.");
    } else {
      Serial.print("Falha ao conectar ao MQTT, rc=");
      Serial.println(mqttClient.state());
    }
  }
}

void MQTTPub::iniciar() {
  conectarWiFi();
  mqttClient.setServer(_mqtt_server, _mqtt_port);

  // Configurar relógio com NTP de forma não bloqueante
  configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("Aguardando sincronização com o servidor NTP...");
}

void MQTTPub::sincronizarRelogio() {
  if (time(nullptr) < 100000) {
    Serial.println("Sincronizando relógio NTP...");
  }
}

void MQTTPub::publicar(int modo, float temperatura, int umidade, int luminosidade, int umisolo, int valor_led, String estado_motor, String estado_rele, String estado_servo) {
  unsigned long currentMillis = millis();

  // Verifica o intervalo
  if (currentMillis - lastPublishTime >= publishInterval) {
    lastPublishTime = currentMillis;

    // Reconectar Wi-Fi se necessário
    conectarWiFi();

    // Reconectar MQTT se necessário
    reconectarMQTT();

    // Verifica se está conectado antes de publicar
    if (WiFi.status() == WL_CONNECTED && mqttClient.connected()) {
      mqttClient.loop();

      // Obter datetime
      time_t now = time(nullptr);
      struct tm* timeinfo = localtime(&now);

      // Criar JSON
      StaticJsonDocument<256> doc;
      char datetime[30];
      strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", timeinfo);
      doc["datetime"] = datetime;
      doc["modo"] = modo;
      doc["temperatura"] = temperatura;
      doc["umidade"] = umidade;
      doc["luminosidade"] = luminosidade;
      doc["umisolo"] = umisolo;
      doc["valor_led"] = valor_led;
      doc["estado_motor"] = estado_motor;
      doc["estado_rele"] = estado_rele;
      doc["estado_servo"] = estado_servo;

      // Serializar JSON
      char jsonBuffer[512];
      serializeJson(doc, jsonBuffer);

      // Publicar no MQTT
      mqttClient.publish(_mqtt_topic, jsonBuffer);

      //Serial.println("Dados publicados:");
      //Serial.println(jsonBuffer);
    } //else {
      //Serial.println("Wi-Fi ou MQTT desconectados. Dados não enviados.");
    //}
  }
}
