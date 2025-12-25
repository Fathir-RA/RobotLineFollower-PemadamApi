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

// lanjutkan dari bagian void calculatePID()  ya... Setalahnya hapus komen ini 
