// WifiManager.cpp
#include <Arduino.h>
#include <my_config.h>
#include "WiFiManager.h"
#include <WiFiUdp.h> // Add the missing include statement
#include "WiFiManager.h"

WiFiManager::WiFiManager(const char *ssid, const char *password)
    : ssid(ssid), password(password), wifiUDP(), timeClient(wifiUDP) {}

void WiFiManager::connect()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("(WiFiManager) Connecting to WiFi...");
  }
  Serial.println("(WiFiManager) Connected to WiFi");

  // Initialize NTP client
  Serial.println("(WiFiManager) Starting connection to NTP server...");
  timeClient.begin();
  //timeClient.setTimeOffset(TIME_ZONE_OFFSET); // Adjust as needed for your time zone
  timeClient.setUpdateInterval(60000);        // Update the time every hour
  timeClient.getFormattedTime();

  syncTime();
}

void WiFiManager::syncTime()
{
  unsigned long startTime = millis(); // Get the current time
  while (!timeClient.update())
  {
    Serial.println("(WiFiManager) Updating timeClient to NTP server...");
    timeClient.forceUpdate();
    timeClient.getEpochTime(); // this is ntpUnixTime

    // Check if the timeout has exceeded 10 seconds
    if (millis() - startTime > 10000)
    {
      Serial.println("(WiFiManager) Failed to synchronize time");
      break; // Exit the loop if timeout exceeded
    }
    else {
      Serial.println("(WiFiManager) Time synchronized");}
  } //end while
}

unsigned long WiFiManager::getCurrentTime()
{
  return timeClient.getEpochTime();// this is ntpUnixTime
}

void WiFiManager::printWiFiStatus()
{
  Serial.print("(WiFiManager) WiFi status: ");
  switch (WiFi.status())
  {
  case WL_CONNECTED:
    Serial.println("(WiFiManager) Connected to WiFi");
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    break;
  case WL_NO_SHIELD:
    Serial.println("No Shield");
    break;
  case WL_IDLE_STATUS:
    Serial.println("Idle");
    break;
  case WL_NO_SSID_AVAIL:
    Serial.println("No SSID Available");
    break;
  case WL_SCAN_COMPLETED:
    Serial.println("Scan Completed");
    break;
  case WL_CONNECT_FAILED:
    Serial.println("Connect Failed");
    break;
  case WL_CONNECTION_LOST:
    Serial.println("Connection Lost");
    break;
  case WL_DISCONNECTED:
    Serial.println("Disconnected");
    break;
  default:
    Serial.println("Unknown");
    break;
  }
}
