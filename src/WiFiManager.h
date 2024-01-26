

/**
 * @file WiFiManager.h
 * @brief This file contains the declaration of the WiFiManager class.
 */

#ifndef WiFiManager_h
#define WiFiManager_h

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>

/**
 * @class WiFiManager
 * @brief The WiFiManager class provides functionality for managing WiFi connections and time synchronization.
 */
class WiFiManager {
public:
  /**
   * @brief Constructs a WiFiManager object with the specified SSID and password.
   * @param ssid The SSID of the WiFi network.
   * @param password The password of the WiFi network.
   */
  WiFiManager(const char* ssid, const char* password);

  /**
   * @brief Connects to the WiFi network using the specified SSID and password.
   */
  void connect();

  /**
   * @brief Synchronizes the device's time with an NTP server.
   */
  void syncTime();

  /**
   * @brief Gets the current time in milliseconds since the epoch.
   * @return The current time in milliseconds.
   */
  unsigned long getCurrentTime();

  /**
   * @brief Prints the WiFi connection status to the serial monitor.
   */
  void printWiFiStatus();

private:
  const char* ssid; /**< The SSID of the WiFi network. */
  const char* password; /**< The password of the WiFi network. */
  WiFiClient wifiClient; /**< The WiFi client object for managing the connection. */
  NTPClient timeClient; /**< The NTP client object for time synchronization. */
  WiFiUDP wifiUDP; /**< The UDP object for NTP communication. */
};

#endif