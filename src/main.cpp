#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

#include "sensors/pir_sensor.h"
#include "config/constants.h"
#include "com/wifi.h"
#include "com/mqtt.h"


String input_buffer;
bool input_queue = false;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  input_buffer = String((char*) payload, length);
  Serial.println(input_buffer);
}

Adafruit_NeoPixel pixels(LIGHTS_COUNT, LIGHTS_PIN, NEO_GRB + NEO_KHZ800);

PirSensor sensor = PirSensor(SENSOR_PIN);
WiFiClient espClient;
WiFiManager manager;
PubSubClient mqtt_client(MQTT_BROKER, 1883, callback, espClient);
MqttManager mqtt_manager = MqttManager(MQTT_CLIENT_ID, MQTT_IN_TOPIC, MQTT_OUT_TOPIC);


void setup() {
  sensor.config();
  Serial.begin(115200);
  pixels.begin();
  pixels.fill(pixels.Color(0, 0, 0), 0, LIGHTS_COUNT);
  pixels.clear();
  WiFi.begin(SSID, SSID_PASSWORD);
  pinMode(LIGHTS_PIN, OUTPUT);
  manager.connect(WiFi, Serial);
  mqtt_manager.connect(mqtt_client, Serial);
}

void loop() {
  sensor.loop();
  mqtt_client.loop();
  manager.checkStatus(WiFi, Serial);

  if(input_queue) {
    DynamicJsonDocument input(200);
    DeserializationError error = deserializeJson(input, input_buffer);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
    }
    
    pixels.fill(pixels.Color(input["red"], input["green"], input["blue"]), 0, LIGHTS_COUNT);
    pixels.show();
    input_queue = false;
  }

  if(sensor.hasChanged()){
    Serial.println(sensor.getValue());
    DynamicJsonDocument  output(20);
    output["sensor"] = sensor.getValue();
    String output_str;
    serializeJson(output, output_str);
    mqtt_client.publish(MQTT_OUT_TOPIC, output_str.c_str());
  }
}