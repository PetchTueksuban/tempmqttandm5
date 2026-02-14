#include <M5Unified.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "";//wifi
const char* password = "";//pass
const char* mqtt_server = "ip";//ip droplet
const char* mqtt_user = "";// User ที่คุณสร้างใน Mosquitto
const char* mqtt_pass = "";// รหัสผ่านที่คุณสร้างใน Mosquitto

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);
    
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(WHITE);
    M5.Display.println("=== M5CoreS3 Status ===");

    // 1. ขั้นตอนเชื่อมต่อ WiFi
    M5.Display.print("WiFi: Connecting");
    WiFi.begin(ssid, password);
    
    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        M5.Display.print(".");
        attempt++;
        if(attempt > 20) { // ถ้าต่อไม่ได้เกิน 10 วินาทีให้ Restart
            M5.Display.println("\nWiFi Failed! Restarting...");
            delay(2000);
            ESP.restart();
        }
    }
    M5.Display.println("\n[OK] WiFi Connected!");
    M5.Display.print("IP: ");
    M5.Display.println(WiFi.localIP());

    // 2. ตั้งค่า MQTT
    client.setServer(mqtt_server, 1883);
    M5.Display.println("MQTT: Initialized");
    delay(1000);
}

void reconnect() {
    while (!client.connected()) {
        M5.Display.setTextColor(YELLOW);
        M5.Display.print("MQTT: Attempting...");
        
        // สร้าง Client ID แบบสุ่มเล็กน้อยเพื่อไม่ให้ซ้ำ
        String clientId = "M5CoreS3-" + String(random(0, 999));
        
        if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
            M5.Display.setTextColor(GREEN);
            M5.Display.println("Connected!");
            M5.Display.setTextColor(WHITE);
        } else {
            M5.Display.setTextColor(RED);
            M5.Display.print("Failed (rc=");
            M5.Display.print(client.state());
            M5.Display.println(") Try again in 5s");
            delay(5000);
        }
    }
}

void loop() {
    M5.update();
    
    // ตรวจสอบการเชื่อมต่อ MQTT เสมอ
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    // 3. อ่านค่าและส่งข้อมูล
    float temp = 25.0 + (random(0, 50) / 10.0); 
    char msg[50];
    snprintf(msg, 50, "%.2f", temp);
    
    // เคลียร์พื้นที่แสดงเฉพาะส่วนอุณหภูมิ (ไม่ให้หน้าจอกระพริบทั้งจอ)
    M5.Display.fillRect(0, 150, 320, 90, BLACK); 
    M5.Display.setCursor(0, 160);
    
    if(client.publish("m5stack/temp", msg)) {
        M5.Display.setTextColor(CYAN);
        M5.Display.setTextSize(3);
        M5.Display.printf("Temp: %s C\n", msg);
        M5.Display.setTextSize(2);
        M5.Display.setTextColor(GREEN);
        M5.Display.println("Status: Data Sent OK");
    } else {
        M5.Display.setTextColor(RED);
        M5.Display.println("Status: Publish Failed!");
    }

    delay(5000); // ส่งทุก 5 วินาที
}