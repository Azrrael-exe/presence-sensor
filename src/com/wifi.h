#ifndef WIFI_CON_H
#define WIFI_CON_H

#include <Arduino.h>
#include <WiFi.h>

class WiFiManager {
    public:
        void connect(WiFiClass &client, Stream &uart);
        void checkStatus(WiFiClass &client, Stream &uart);
    private:
};

void WiFiManager::connect(WiFiClass &client, Stream &uart) {
    while (client.status() != WL_CONNECTED) {
        delay(500);
        uart.print("-");
  }
  uart.println("-");
  uart.print("Conected with IP: ");
  uart.println(client.localIP());
}

void WiFiManager::checkStatus(WiFiClass &client, Stream &uart){
    if (client.status() != WL_CONNECTED) {
        this->connect(client, uart);
    }
}

#endif