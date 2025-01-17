#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "SPIFFS.h"

// Configuração WiFi
#define WIFI_SSID "SUA REDE WIFI"
#define WIFI_PASS "SUA SENAH DO WIFI"

// Configuração Adafruit IO
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "Seu_Username"
#define AIO_KEY "Sua_AIO_Key"

// Configuração do feed
#define FEED_NAME "sensor_ultrassonico"

// Conexão WiFi
WiFiClient client;

// Cliente MQTT
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Feed MQTT
Adafruit_MQTT_Publish sensorFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/" FEED_NAME);

#define TRIG_PIN 4
#define ECHO_PIN 5
#define BUZZER_PIN 14 // Pino do buzzer

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT); // Configura o pino do buzzer como saída

  // Inicialização do SPIFFS para armazenamento de dados
  if (!SPIFFS.begin(true)) {
    Serial.println("Falha ao montar o sistema de arquivos SPIFFS.");
    return;
  }

  Serial.print("Conectando ao WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi!");

  connectToMQTT();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
  }

  if (!mqtt.connected()) {
    connectToMQTT();
  }

  mqtt.processPackets(10);
  mqtt.ping();

  long duration;
  float distance;

  // Gera um pulso no TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Lê o tempo do pulso no ECHO
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration * 0.034) / 2;

  Serial.print("Distância: ");
  Serial.println(distance);

  if (distance <= 120 && distance > 0) {
    Serial.println("Alerta! Alguém proximo ao sensor.");
    
    // Ativa o buzzer por 10 segundos
    tone(BUZZER_PIN, 1000); // Frequência de 1000 Hz
    delay(3000); // Buzzer ligado por 10 segundos
    noTone(BUZZER_PIN); // Desliga o buzzer

    // Publica alerta no feed
    if (!sensorFeed.publish(distance)) {
      Serial.println("Erro ao publicar no feed.");
    }

    // Armazena o evento no SPIFFS
    saveEventToSPIFFS(distance);
  }

  delay(500);
}

void connectToMQTT() {
  Serial.print("Conectando ao Adafruit IO... ");
  while (mqtt.connect() != 0) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Conectado!");
}

void saveEventToSPIFFS(float distance) {
  File file = SPIFFS.open("/log.txt", FILE_APPEND);
  if (!file) {
    Serial.println("Falha ao abrir o arquivo para escrita.");
    return;
  }

  String logEntry = "Distância crítica: " + String(distance) + " cm - Evento registrado\n";
  file.print(logEntry);
  file.close();

  Serial.println("Evento salvo no SPIFFS: " + logEntry);
}
