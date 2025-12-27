# Robot Line Follower Fire Fighter (Kelompok 10A)
Sistem Pengikut Garis Pemadam Api Otomatis Berbasis Computer Vision & IoT (MQTT)
Dibuat Oleh:
Edgard Altamerano Ferdinand
Fathir Raihan Muhammad
Khairana Mazaya
Muthmainnah Indriani Mulyana
Ripan


## 📝 Deskripsi Proyek
Proyek ini dikembangkan oleh Kelompok 10A untuk menciptakan sistem pemadam api otomatis yang cerdas dan responsif. Robot ini menggabungkan kecanggihan Computer Vision melalui kamera eksternal (Laptop/PC) dengan mobilitas robot otonom berbasis ESP32.
Berbeda dengan robot pemadam api konvensional yang hanya mengandalkan sensor jarak, robot ini menggunakan kamera untuk deteksi dini api dari jarak jauh sebelum mulai bergerak menuju titik api menggunakan navigasi garis (line following) yang presisi.

## 🛠️ Daftar Komponen
Perangkat Keras (Hardware)
Kontroler Utama: ESP32 
Driver Motor: L298N 
Sensor Navigasi: TCRT5000 5-Channel Infrared Sensor 
Sensor Api: Flame Sensor Analog 
Output Pemadam: Relay Module & Kipas DC 
Komunikasi: WiFi Module (Built-in ESP32) 
Perangkat Lunak (Software)
Bahasa Pemrograman: C++ (Arduino IDE) dan Python
Library Utama: WiFi.h, PubSubClient (MQTT), OpenCV (Python), NumPy
Protokol Komunikasi: MQTT (Message Queuing Telemetry Transport)

## Logika & Cara Kerja
Sistem ini bekerja dengan membagi tugas antara "Otak Deteksi" (Python) dan "Eksekutor" (ESP32).
1. Deteksi Api (Computer Vision)
Skrip Python menjalankan pemrosesan gambar secara real-time menggunakan kamera.
Logika deteksi menggunakan ruang warna HSV (Hue, Saturation, Value) untuk mengisolasi warna api.
Jika kontur api terdeteksi melebihi ambang batas luas yang ditentukan, skrip mengirim perintah "START" melalui protokol MQTT.
2. Navigasi & PID Control
Robot menerima perintah melalui MQTT dan mulai bergerak mengikuti garis hitam.
Navigasi menggunakan algoritma PID (Proportional-Derivative):
Proportional (Kp): Mengatur ketajaman belokan berdasarkan posisi sensor di garis.
Derivative (Kd): Meredam gerakan robot agar tidak goyang atau tidak keluar garis saat kecepatan tinggi.
Kecepatan motor dibatasi minimal pada angka 80 PWM untuk mencegah motor mendengung.
3. Eksekusi Pemadaman & Home Base
Saat mendekati api, sensor api tengah pada robot akan mendeteksi intensitas cahaya api.
Jika nilai sensor di bawah ambang batas (api dekat), robot berhenti dan menyalakan kipas melalui relay selama 3 detik.
Setelah api padam, robot mencari Home Base (kotak hitam pekat).
Logika deteksi Home Base: Jika kelima sensor mendeteksi warna hitam secara bersamaan selama 0,3 detik, robot akan berhenti.
Robot melakukan manuver Putar Balik 180° (Pivot) dan masuk ke kondisi diam selamanya hingga misi di-reset.

## Konfigurasi Pin ESP32
- **Motor Driver L298N**: ENA(19), ENB(21), Motor Kiri(18, 5), Motor Kanan(17, 16).
- **Sensor Garis (TCRT5000)**: IR1-5 (32, 33, 25, 26, 27).
- **Sensor Api & Output**: Api Tengah(35), Relay Kipas(4)

## Cara Penggunaan
1. **Persiapan Broker**: Jalankan MQTT Broker (seperti Mosquitto) di komputer Anda.
2. **Konfigurasi ESP32**:
  - Buka file .ino di folder arduino/.
  - Sesuaikan ssid, password, dan mqtt_server (Alamat IP Laptop).
  - Unggah kode ke papan ESP32.
3. **Konfigurasi Python**:
  - Instal dependensi: pip install opencv-python numpy paho-mqtt.
  - Jalankan skrip trigger.py.
4. **Uji Coba**: Letakkan robot di lintasan, nyalakan api di depan kamera, dan robot akan memulai misi secara otomatis.
