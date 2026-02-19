Real-time IoT Temperature Monitoring System
A full-stack IoT application that captures temperature data from an M5CoreS3 device, transmits it via MQTT to a DigitalOcean cloud server, and visualizes the data on a native Android application.

🚀 Features
Hardware Integration: Real-time data acquisition using M5CoreS3 (ESP32-S3)
Cloud Connectivity: Secure MQTT communication via Mosquitto broker hosted on DigitalOcean.
Live Visualization: Dynamic line charts on Android using the MPAndroidChart library.
Professional Android Standards: Implementation of String Resources and Version Catalogs for clean, scalable code.

🛠️ Tech Stack
Hardware: M5CoreS3, M5Unified Library (C++/Arduino).
Cloud: DigitalOcean Droplet, Ubuntu, Mosquitto MQTT Broker.
Mobile: Android Studio (Java), MPAndroidChart, Eclipse Paho MQTT.\

📂 Project Structure
m5stack/: Arduino code for M5CoreS3 sensor data publishing.
android_app/: Native Java Android application source code.

🔧 Setup & Installation
Hardware: Flash the M5CoreS3 using Arduino IDE. Ensure WiFi and MQTT credentials match your server.
Server: Deploy Mosquitto on your DigitalOcean Droplet and open port 1883.
Android: Open the project in Android Studio, Sync Gradle, and update the serverUri in MainActivity.java with your Droplet IP.


!!!in the code I made random temp if you want real temp need to modify code at temp section!!!

# Temperature MQTT with M5Stack
This project is designed to monitor and transmit temperature and humidity data using an M5Stack device via the MQTT protocol.

# (Prerequisites)  
Before getting started, ensure you have the following:
**Hardware:**
   -M5Stack Controller (e.g., Core, Core2, or M5StickC)  
   -Temperature & Humidity Sensor (e.g., ENV Unit)
**Software:**
   - [Arduino IDE](https://www.arduino.cc/en/software)
   - M5Stack Board support installed via Board Manager
**Libraries  Install via Library Manager)**
   -M5Stack or M5Unified
   -PubSubClient (by Nick O'Leary) for MQTT functionality
   -Adafruit BMP280 or the specific sensor library you are using

# (Setup Instructions)
**Clone Repository:**
   ```bash
   git clone [https://github.com/PetchTueksuban/tempmqttandm5.git](https://github.com/PetchTueksuban/tempmqttandm5.git)
