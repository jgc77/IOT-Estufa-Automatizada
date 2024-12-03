#include "MQTTPub.h"

MQTTPub::MQTTPub(const char* ssid, const char* password, const char* mqtt_server, int mqtt_port, const char* mqtt_topic)
  : _ssid(ssid), _password(password), _mqtt_server(mqtt_server), _mqtt_port(mqtt_port), _mqtt_topic(mqtt_topic), mqttClient(espClient) {
  lastPublishTime = 0;  // Inicializa o tempo de última publicação
  publishInterval = 1000;  // Define o intervalo de 1 segundo para as publicações
}

void MQTTPub::conectarWiFi() {
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(_ssid, _password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado. IP: " + WiFi.localIP().toString());
}

void MQTTPub::reconectarMQTT() {
  while (!mqttClient.connected()) {
    Serial.println("Conectando ao MQTT...");
    if (mqttClient.connect("ESP32_ClientID")) {
      Serial.println("Conectado ao MQTT.");
    } else {
      Serial.print("Falha ao conectar, rc=");
      Serial.print(mqttClient.state());
      Serial.println(". Tentando novamente em 5 segundos.");
      delay(5000);
    }
  }
}

void MQTTPub::iniciar() {
  conectarWiFi();
  mqttClient.setServer(_mqtt_server, _mqtt_port);
  
  // Configurar relógio com NTP
  configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("Aguardando sincronização com o servidor NTP...");
  while (time(nullptr) < 100000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nHora sincronizada.");
}

void MQTTPub::publicar(int modo, float temperatura, int umidade, int luminosidade, int umisolo, int valor_led, String estado_motor, String estado_rele) {
  unsigned long currentMillis = millis();

  // Verifica se já passou o tempo de intervalo
  if (currentMillis - lastPublishTime >= publishInterval) {
    if (WiFi.status() != WL_CONNECTED) {
      conectarWiFi();
    }
    if (!mqttClient.connected()) {
      reconectarMQTT();
    }
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
   

    // Serializar JSON
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);

    // Publicar no MQTT
    mqttClient.publish(_mqtt_topic, jsonBuffer);

    // Exibir no Serial Monitor
    //Serial.println("Dados publicados:");
    //Serial.println(jsonBuffer);

    // Atualiza o tempo da última publicação
    lastPublishTime = currentMillis;
  }
}
