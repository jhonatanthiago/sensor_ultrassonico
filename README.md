# Projeto de Monitoramento de Idosa com Sensor Ultrassônico e Buzzer

## Descrição

Este projeto foi desenvolvido para monitorar a proximidade de uma idosa em relação à borda da cama, utilizando um sensor ultrassônico e um buzzer. Caso a idosa se aproxime demais da borda da cama (distância de 3 cm ou menos), um alerta sonoro será emitido por 10 segundos através do buzzer. Além disso, o evento será registrado na memória do ESP32 e enviado para a plataforma IoT Adafruit IO para monitoramento remoto, e através do MQTT Alert Andorid, uma notificação é disparada quando acionada.

## Funcionalidades

- **Sensor Ultrassônico**: Mede a distância entre o sensor e a pessoa.
- **Buzzer**: Emite um alerta sonoro quando a distância é atingida.
- **Armazenamento Local**: Armazena eventos de proximidade no sistema de arquivos do ESP32 (SPIFFS).
- **Comunicação IoT**: Envia um alerta para a plataforma Adafruit IO via MQTT.
- **Visualização de Dados**: Envia eventos para a plataforma Adafruit IO para visualização e monitoramento.

## Requisitos

### Hardware

- **ESP32**
- **Sensor Ultrassônico** (HC-SR04)
- **Buzzer** (ativo ou passivo)
- **Cabo USB** para conectar o ESP32 ao computador

### Software

- **Arduino IDE**
- **Biblioteca Adafruit MQTT** para comunicação com Adafruit IO
- **Biblioteca WiFi** para conexão com a rede

### Conexões

- **Sensor Ultrassônico**:
  - `VCC` -> 3V (ou 5V)
  - `GND` -> GND
  - `TRIG` -> GPIO 4
  - `ECHO` -> GPIO 5

- **Buzzer**:
  - Conecte o pino positivo do buzzer ao **GPIO 14** e o pino negativo ao **GND**.

## Configuração

### 1. Configuração Wi-Fi

Altere as seguintes variáveis no código para corresponder à sua rede Wi-Fi:

```cpp
#define WIFI_SSID "SUA REDE WIFI"
#define WIFI_PASS "SUA SENHA DO WIFI"
```

### 2. Configuração Adafruit IO

Crie uma conta no [Adafruit IO](https://io.adafruit.com/) e crie um **Feed** chamado `sensor_ultrassonico`. Substitua os seguintes valores no código pelo seu **AIO Username** e **AIO Key**:

```cpp
#define AIO_USERNAME "Seu_Username"
#define AIO_KEY "Sua_AIO_Key"
```

### 3. Carregar o Código no ESP32

1. Abra o Arduino IDE.
2. Selecione a placa **ESP32**.
3. Conecte o ESP32 via USB e selecione a porta correta.
4. Carregue o código para o ESP32.

## Como Funciona

1. O código mede a distância entre o sensor ultrassônico e a pessoa.
2. Se a distância for igual ou menor que 3 cm, o **buzzer** é ativado por 10 segundos.
3. Um alerta é enviado para a plataforma **Adafruit IO** via MQTT.
4. O evento é registrado no sistema de arquivos **SPIFFS** para futura referência.
5. O código se conecta automaticamente à sua rede Wi-Fi e à plataforma Adafruit IO para envio de dados.

## Estrutura do Código

- **Configuração de pinos**:
  - O pino TRIG do sensor ultrassônico está configurado como **GPIO 4**.
  - O pino ECHO do sensor está configurado como **GPIO 5**.
  - O buzzer está conectado ao pino **GPIO 14**.
  
- **Wi-Fi e MQTT**:
  - O código se conecta à rede Wi-Fi usando as credenciais fornecidas.
  - Após a conexão Wi-Fi, o código tenta se conectar ao servidor MQTT Adafruit IO e publica alertas quando a distância for crítica.

- **Armazenamento de eventos**:
  - Quando o buzzer é acionado, o evento é armazenado no arquivo **log.txt** na memória interna do ESP32 (SPIFFS).

## Melhorias Futuras

- **Desligamento remoto do buzzer**: Implementar a possibilidade de desligar o buzzer via controle remoto ou aplicativo.
- **Dashboard na plataforma IoT**: Criar um dashboard no Adafruit IO para visualização dos dados em tempo real.

## Referências

- [Adafruit IO](https://io.adafruit.com/)
- LINK DA PLAY STORE PARA MQTT ALERT ANDROID: https//play.google.com/store/apps/details?id=gigiosoft.MQTTAlert
