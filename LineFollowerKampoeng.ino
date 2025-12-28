#include <WiFi.h>
#include <PubSubClient.h>

// --- KONFIGURASI JARINGAN (GANTI DENGAN DATA ANDA) ---
const char* ssid = "YOUR_WIFI_SSID";           
const char* password = "YOUR_WIFI_PASSWORD";       
const char* mqtt_server = "YOUR_BROKER_IP"; 
const char* mqtt_topic = "robot/kelompok10A/trigger";

WiFiClient espClient;
PubSubClient client(espClient);

// --- PIN DEFINISI ---
#define ENA_PIN         19  
#define MOTOR_KIRI_1    18
#define MOTOR_KIRI_2    5
#define ENB_PIN         21  
#define MOTOR_KANAN_1   17
#define MOTOR_KANAN_2   16
#define IR1 32
#define IR2 33
#define IR3 25 
#define IR4 26
#define IR5 27
#define API_TENGAH_PIN  35 
#define RELAY_KIPAS     4

// --- PARAMETER PID & STATE ---
float Kp = 30.0;  
float Kd = 15.0;  
int baseSpeed = 90; 
int maxSpeed = 120; 
int lastError = 0;
const int GARIS = 0; 
const int JARAK_PADAM = 300; 
unsigned long timerHome = 0;
bool missionActive = false;
bool misiSelesai = false;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqttCallback);

  pinMode(IR1, INPUT); pinMode(IR2, INPUT); pinMode(IR3, INPUT);
  pinMode(IR4, INPUT); pinMode(IR5, INPUT);
  pinMode(API_TENGAH_PIN, INPUT);
  pinMode(ENA_PIN, OUTPUT); pinMode(MOTOR_KIRI_1, OUTPUT); pinMode(MOTOR_KIRI_2, OUTPUT);
  pinMode(ENB_PIN, OUTPUT); pinMode(MOTOR_KANAN_1, OUTPUT); pinMode(MOTOR_KANAN_2, OUTPUT);
  pinMode(RELAY_KIPAS, OUTPUT); digitalWrite(RELAY_KIPAS, HIGH);
}

void loop() {
  if (!client.connected()) reconnectMQTT();
  client.loop();

  if (missionActive) {
    int apiCenter = analogRead(API_TENGAH_PIN);
    if (apiCenter < JARAK_PADAM && !misiSelesai) {
      motorControl(0, 0);
      digitalWrite(RELAY_KIPAS, LOW); 
      delay(3000);                      
      digitalWrite(RELAY_KIPAS, HIGH); 
      delay(500);
      misiSelesai = true;
    } else {
      calculatePID(); 
    }
  }
}

void calculatePID() {
  int s1 = digitalRead(IR1); int s2 = digitalRead(IR2);
  int s3 = digitalRead(IR3); int s4 = digitalRead(IR4); int s5 = digitalRead(IR5);

  // Logika Home Base (Kotak Hitam)
  if (s1 == GARIS && s2 == GARIS && s3 == GARIS && s4 == GARIS && s5 == GARIS) {
    if (timerHome == 0) timerHome = millis();
    if (millis() - timerHome > 300) {
      motorControl(0, 0); delay(1000);
      // Putar Balik 180 Derajat
      motorControl(100, 100); delay(500); 
      while(digitalRead(IR3) != GARIS) { motorControl(80, -80); }
      motorControl(0, 0); missionActive = false;
      return;
    }
  } else { timerHome = 0; }

  int error = 0;
  if      (s1 == GARIS) error = -3;
  else if (s2 == GARIS) error = -1;
  else if (s3 == GARIS) error = 0;
  else if (s4 == GARIS) error = 1;
  else if (s5 == GARIS) error = 3;
  else { error = (lastError > 0) ? 4 : -4; }

  float PIDValue = (Kp * error) + (Kd * (error - lastError));
  lastError = error;
  int speedKiri = constrain(baseSpeed + PIDValue, 80, maxSpeed);
  int speedKanan = constrain(baseSpeed - PIDValue, 80, maxSpeed);
  motorControl(speedKiri, speedKanan);
}

void motorControl(int kiri, int kanan) {
  if (kiri >= 0) { digitalWrite(MOTOR_KIRI_1, HIGH); digitalWrite(MOTOR_KIRI_2, LOW); }
  else { digitalWrite(MOTOR_KIRI_1, LOW); digitalWrite(MOTOR_KIRI_2, HIGH); }
  analogWrite(ENA_PIN, abs(kiri));
  if (kanan >= 0) { digitalWrite(MOTOR_KANAN_1, HIGH); digitalWrite(MOTOR_KANAN_2, LOW); }
  else { digitalWrite(MOTOR_KANAN_1, LOW); digitalWrite(MOTOR_KANAN_2, HIGH); }
  analogWrite(ENB_PIN, abs(kanan));
}

// lanjutkan dari bagian void setup_wifi() ya... Setalahnya hapus komen ini 
