// WiFiManager.h
// Description: WiFiManager class header file
#ifndef WiFiManager_h
#define WiFiManager_h

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>


class WiFiManager {
public:
  WiFiManager(const char* ssid, const char* password);
  void connect();
  void syncTime();
  unsigned long getCurrentTime();
  void printWiFiStatus(); // Add a public member function to print the WiFi status


private:
  const char* ssid;
  const char* password;
  WiFiClient wifiClient;
  NTPClient timeClient;
  WiFiUDP wifiUDP; // Add a private member variable of type WiFiUDP to the WiFiManager class
};

#endif