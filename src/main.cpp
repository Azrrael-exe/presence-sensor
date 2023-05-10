#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "sensors/dwarf_sensor.h"
#include "actuators/lighthouse_display.h"
#include "config/constants.h"
#include "com/wifi.h"
#include "com/mqtt.h"
#include "parking/parking.h"

String input_buffer;
bool input_queue = false;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  input_buffer = String((char*) payload, length);
  Serial.println(input_buffer);
  input_queue = true;
}

Parking parking = Parking(0x0ABC, 20);
LighthouseDisplay display;
DwarfSensor sensor = DwarfSensor(SENSOR_PIN1, SENSOR_PIN2, 5000);
WiFiClient espClient;
WiFiManager manager;
PubSubClient mqtt_client(MQTT_BROKER, 1883, callback, espClient);
MqttManager mqtt_manager = MqttManager(MQTT_CLIENT_ID, MQTT_IN_TOPIC, MQTT_OUT_TOPIC);


void setup() {
  sensor.config();
  display.config();
  Serial.begin(115200);
  WiFi.begin(SSID, SSID_PASSWORD);
  manager.connect(WiFi, Serial);
  mqtt_manager.connect(mqtt_client, Serial);
}

void loop() {
  sensor.loop();
  mqtt_client.loop();
  manager.checkStatus(WiFi, Serial);
  parking.loop(
    sensor, 
    mqtt_client, 
    input_buffer, 
    input_queue, 
    display
  );
}
