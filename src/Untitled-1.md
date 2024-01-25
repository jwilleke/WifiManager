















/*********************************************************************
 * WaterMonitor.ino
 *
 * Copyright (C)    2017   [DFRobot](http://www.dfrobot.com)
 * GitHub Link :https://github.com/DFRobot/watermonitor
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *
 * SD card attached to SPI bus as follows:
 * Mega:  MOSI - pin 51, MISO - pin 50, CLK - pin 52, CS - pin 53
 * and pin #53 (SS) must be an output
 * M0:   Onboard SPI pin,CS - pin 4 (CS pin can be changed)
 *
 * author  :  Jason(jason.ling@dfrobot.com)
 * version :  V1.0
 * date    :  2017-04-06
 **********************************************************************/
#include <Arduino.h>
#include <OneWire.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiUdp.h> // works with the Ardiuno R4 WiFi
#include <RTC.h>
#include <NTPClient.h> //Include the NTP library
// #include <DFRobot_SHT3x.h>
#include <arduino_secrets.h> // contains ssid and password are in local /Users/jim/.platformio/lib/specials/arduino_secrets.h
#include "GravitySensorHub.h"
// #include "../../src/GravityRtc.h"
#include "GravityEc.h"
#include "GravityPh.h"
// #include "../../src/GravityDo.h"
#include "SdService.h"
#include "Debug.h"
#include <WiFiServer.h>
#include <PushOverNTPClient.h>

// wi-Fi items
char ssid[] = SECRET_SSID; // your network SSID (name)
char pass[] = SECRET_PASS; // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;

// RTC Items
WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP
NTPClient timeClient(Udp);

RTCTime currentTime;
/// #define TIME_HEADER "T " // Header tag for serial time sync message
unsigned long updateTime = 0;

int led = LED_BUILTIN;

// Alias sensor logic as sensorHub
GravitySensorHub sensorHub;

// Alias SD logic as sdService applied to sensors
SdService sdService = SdService(sensorHub.sensors);

//PushOverNTPClient pushoverClient;

/**
 * common function to print wifi status
 */
void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

/**
 * get the sensor list and associated data
 */

void getSensorList()
{

  Serial.print("  TDS= ");
  Serial.println(sensorHub.getValueBySensorNumber(0));

  Serial.print("  EC= ");
  Serial.println(sensorHub.getValueBySensorNumber(1));

  Serial.print("  ph= ");
  Serial.println(sensorHub.getValueBySensorNumber(2));

  Serial.print("  Temp= ");
  Serial.println(sensorHub.getValueBySensorNumber(3));

  Serial.print("  ORB= ");
  Serial.println(sensorHub.getValueBySensorNumber(4));
}

/**
 * Get the sensor name as a string
 */
std::string getSensorName(SensorNumber sensor)
{
  switch (sensor)
  {
  case phSensor:
    return "phSensor";
  case tdsSensor:
    return "tdsSensor";
  case ecSensor:
    return "ecSensor";
  case temperatureSensor:
    return "temperatureSensor";
  case orpSensor:
    return "orpSensor";
  case liquidSensor:
    return "liquidSensor";
  default:
    return "Unknown";
  }
}

void printSensorConfiguration()
{
  for (size_t i = 0; i < SENSORCOUNT; i++)
  {
    if (sensorHub.sensors[i])
    {
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.print(" is ");
      Serial.println(getSensorName((SensorNumber)i).c_str());
    }
  }
}

/*

*/
void setup()
{
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Debug::println("Serial begin");

  // initialize the LED pin as an output:
  pinMode(led, OUTPUT); // set the LED pin mode

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println("Please upgrade the firmware");
  }

  // by default the local IP address of will be
  WiFi.setHostname(HOSTNAME);
  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to Wi-Fi named: ");
    Serial.println(ssid); // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  // server.begin();    // start the web server on port 80
  printWifiStatus(); // you're connected now, so print out the status

  // initialize RTC module with computer time
  RTC.begin();
  // RTC.setHourMode(CLOCK_H24);
  Serial.println("Starting connection to NTP server...");
  timeClient.begin();
  timeClient.update();

  // Get the current date and time from an NTP server and convert it to UTC TIME_ZONE_OFFSET (config.h) by passing the time zone offset in hours.
  auto timeZoneOffsetHours = TIME_ZONE_OFFSET;
  auto unixTime = timeClient.getEpochTime() + (timeZoneOffsetHours * 3600); // get unixTime with TIME_ZONE_OFFSET
  Serial.print("Unix time = ");
  Serial.println(unixTime);
  RTCTime timeToSet = RTCTime(unixTime);
  RTC.setTime(timeToSet);
  // Retrieve the date and time from the RTC and print them
  RTCTime currentTime;
  RTC.getTime(currentTime);
  Serial.println("The RTC was just set to: " + String(currentTime));

  // Reset and initialize sensors
  Debug::println("sensorHub setup");
  sensorHub.setup();

  getSensorList();

  // Apply calibration offsets
  // Calibrate pH
  ((GravityPh *)(sensorHub.sensors[phSensor]))->setOffset(PHOFFSET);
  Debug::print("pH offset: ");
  Debug::println(PHOFFSET);

  // Calibrate EC if present
  ((GravityEc *)(sensorHub.sensors[ecSensor]))->setKValue(ECKVALUE);
  Debug::print("EC K Value: ");
  Debug::println(ECKVALUE);

  // Check for SD card and configure datafile
  Debug::println("Setting up Sd Card...");
  sdService.setup();
} // edn setup

void loop()
{
  //pushoverClient.sendPushNotification("Hello from Arduino!");

 //pushoverClient.getTimeFromNTP();
  
  // Update time from clock module
  RTC.getTime(currentTime);

  // Collect sensor readings
  sensorHub.update();

  // Write data to SD card
  sdService.update();
}
