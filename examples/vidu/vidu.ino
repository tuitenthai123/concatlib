#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <catIOT.h>

#define DHTPIN 15
#define DHTTYPE DHT22

catIOT cat("your-id-project-here");

void connectWiFi() {
    Serial.print("Connecting to WiFi ");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" Connected!");
}

void connectMQTT() {
    Serial.print("Connecting to MQTT server... ");
    Serial.println("Connected!");
}

void setup() {
    Serial.begin(115200);
    connectWiFi();
    dht.begin(DHTPIN,DHTTYPE)
}

void loop() {
    float a = cat.readOne("V3");
    Serial.print("data: ");
    Serial.println(a);
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    if (!isnan(temp) && !isnan(humidity)) {
        cat.WriteArr("V3", humidity);
    }
    delay(5000); // Adjust the delay as needed
}
