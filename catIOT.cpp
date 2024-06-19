#include "catIOT.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include "RTClib.h"

catIOT::catIOT(String id) : id_project(id) {}

float catIOT::sendToMongoDB(String payload, String type) {
    return sendToMongoDB(payload, type, "");
}

float catIOT::sendToMongoDB(String payload, String type, String Pin) {
    StaticJsonDocument<1024> doc;
    float data = -1.0; // Initialize with an invalid value
    HTTPClient http;
    http.begin("https://ap-southeast-1.aws.data.mongodb-api.com/app/data-oyybq/endpoint/data/v1/action/" + type);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Access-Control-Request-Headers", "*");
    http.addHeader("api-key", "CfmUYatHhEfinoDfKxckJ5ClcEZIdx5Ijd5xX7hN2yu6WA0C8GCEz6xCKiezdVp8");
    http.addHeader("Accept", "application/json");

    int httpCode = http.POST(payload);
    Serial.println(payload);
    Serial.println(httpCode);
    if (httpCode == HTTP_CODE_OK || httpCode == 200) {
        if (type == "findOne") {
            String responseBody = http.getString();
            DeserializationError error = deserializeJson(doc, responseBody);
            if (error) {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
                return -1;
            }
            data = doc["document"][Pin]["data"][1];
            Serial.println("Data body:");
            Serial.println(responseBody);
        }
        Serial.println("Data updated successfully");
    } else {
        Serial.println("Failed to update data in MongoDB");
    }

    http.end();
    return data;
}

float catIOT::readOne(String Pin) {
    String payload = "{\"dataSource\":\"Cluster0\",\"database\":\"ptudiot\",\"collection\":\"dulieus\",\"filter\":{\"id_project\":\"" + id_project + "\"}}";
    return sendToMongoDB(payload, "findOne", Pin); // Assuming "V5" is the Pin used in readOne
}

void catIOT::WriteArr(String Pin, float data) {
    for (int i = MAX_DATA_POINTS - 1; i > 0; i--) {
        arrayData[i] = arrayData[i - 1];
    }
    arrayData[0] = data;
    String payload = "{\"dataSource\":\"Cluster0\",\"database\":\"ptudiot\",\"collection\":\"dulieus\",\"filter\":{\"id_project\":\"" + id_project + "\"},\"update\":{\"$set\":{\"" + Pin + "\":{\"data\":[";
    for (int i = 1; i < MAX_DATA_POINTS; i++) {
        payload += arrayData[i];
        if (i < MAX_DATA_POINTS - 1) {
            payload += ",";
        }
    }
    payload += "]}}}}";
    sendToMongoDB(payload, "updateMany");
}