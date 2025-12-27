import cv2
import numpy as np
import paho.mqtt.client as mqtt
import time

# --- KONFIGURASI MQTT (GANTI IP BROKER) ---
BROKER = "YOUR_BROKER_IP"
PORT = 1883
TOPIC = "robot/kelompok10A/trigger"

client = mqtt.Client()
client.connect(BROKER, PORT, 60)

lower_fire = np.array([10, 45, 180])  
upper_fire = np.array([35, 255, 255])
cap = cv2.VideoCapture(0) 
trigger_sent = False
last_fire_time = 0 

while True:
    ret, frame = cap.read()
    if not ret: break
    hsv = cv2.cvtColor(cv2.GaussianBlur(frame, (21, 21), 0), cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, lower_fire, upper_fire)
    contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    
    fire_detected = any(cv2.contourArea(c) > 500 for c in contours)

    if fire_detected and not trigger_sent:
        client.publish(TOPIC, "START")
        trigger_sent = True
        last_fire_time = time.time()
    elif trigger_sent and (time.time() - last_fire_time > 10):
        trigger_sent = False

    cv2.imshow("Fire Monitoring", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'): break

cap.release()
cv2.destroyAllWindows()
client.disconnect()
