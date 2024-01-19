#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1

//variavel ESPNOW
uint8_t newData;

//variaveis motores
int enableMotor1 = 23;
int enableMotor2 = 32;
int pin1 = 21;
int pin2 = 19;
int pin3 = 33;
int pin4 = 22;


void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("Slave_1", "Slave_1_Password", CHANNEL, 0);

  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);

  //Configure the pins of the Motors
  pinMode(enableMotor1, OUTPUT);
  pinMode(enableMotor2, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);

  // Habilita a potencia dos motores 
  digitalWrite(enableMotor1, HIGH);
  digitalWrite(enableMotor2, HIGH);
}

void loop() {
  //colocar a logica aqui usando a variável newData.
  switch (newData) {
        case 1:
          motor1Frente();
          motor2Tras();
          break;
        case 2:
          motor1Tras();
          motor2Frente();
          break;
        case 3:
          motor1Frente();
          motor2Frente();
          break;
        case 4:
          motor1Tras();
          motor2Tras();
          break;
        case 5:
          motor2Parado();
          motor1Parado();
          break;
      }
  delay(100);
}

// callback when data is recv from Master
void OnDataRecv(const esp_now_recv_info_t * info, const uint8_t *data, int data_len) {
  Serial.print("Last Packet Recv Data: "); 
  Serial.println(*data);
  memcpy(&newData, data, sizeof(newData));
}

void motor1Frente() {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  return;
}

void motor2Frente(){
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, HIGH);
  return;
}
    
void motor1Tras() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
  return;
}

void motor2Tras() {
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, LOW);
  return;
}

void motor1Parado(){
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  return;
}

void motor2Parado() {
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, HIGH);
  return;
}

void parar() {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, HIGH);
  return;
}