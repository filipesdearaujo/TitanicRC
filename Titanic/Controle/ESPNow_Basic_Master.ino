#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h> // only for esp_wifi_set_channel()
#define CHANNEL 1
const int buttonPins[] = {15, 19, 26, 25};
unsigned long lastButtonPressTime = 0;

esp_now_peer_info_t slave;

// The all important data!
uint8_t data = 0;

// Init ESP Now with fallback
void InitESPNow() {
  esp_now_init();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  InitESPNow();
  esp_now_register_send_cb(OnDataSent);
  ScanForSlave();
  esp_now_add_peer(&slave);
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLDOWN);
  }
}

void loop() {
  sendData();
  delay(200);
}

// Scan for slaves in AP mode and ad as peer if found
void ScanForSlave() {
  int16_t scanResults = WiFi.scanNetworks(false, false, false, 300, CHANNEL); // Scan only on one channel

    for (int i = 0; i < scanResults; ++i) {
      String SSID = WiFi.SSID(i);
      String BSSIDstr = WiFi.BSSIDstr(i);

      if (SSID.indexOf("Slave") == 0) {

        int mac[6];
        if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
          for (int ii = 0; ii < 6; ++ii ) {
            slave.peer_addr[ii] = (uint8_t) mac[ii];
          }
        }
        slave.channel = CHANNEL; // pick a channel
        slave.encrypt = 0; // no encryption
        break;
      }
    }
}

// callback when data is sent from Master to Slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("I sent my data ->");
  Serial.println(data);
}

// pair the slave with master
bool manageSlave() {
      esp_err_t addStatus = esp_now_add_peer(&slave);
      return true;
       
}

// send data
void sendData() {
  for (int i = 0; i < 4; i++) {
    if (digitalRead(buttonPins[i]) == HIGH) {
      switch (buttonPins[i]) {
        case 15:
          data = 1; // Envia 'E' via ESPNOW
          Serial.println("E");
          break;
        case 19:
          data = 2; // Envia 'D' via ESPNOW
          Serial.println("D");
          break;
        case 26:
          data = 3; // Envia 'R' via ESPNOW
          Serial.println("R");
          break;
        case 25:
          data = 4; // Envia 'F' via ESPNOW
          Serial.println("F");
          break;
      }
      lastButtonPressTime = millis(); // Atualiza o tempo do último pressionamento do botão
      delay(200); // Pequena pausa para evitar múltiplas detecções do mesmo pressionamento
    }
  }
  if (millis() - lastButtonPressTime >= 10000) {
    data = 5; // Envia 'S' via ESPNOW
    Serial.println("S");
    lastButtonPressTime = millis(); // Reinicia o tempo do último pressionamento do botão
  }
  const uint8_t *peer_addr = slave.peer_addr;
  esp_err_t result = esp_now_send(peer_addr, &data, sizeof(data));
}
