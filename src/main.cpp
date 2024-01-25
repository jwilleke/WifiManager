#include <Arduino.h>
#include "WiFiManager.h"
#include <arduino_secrets.h>
#include <RTC.h>
#include <TimeLib.h>
#include <Timezone.h>
#include <my_config.h>

extern WiFiManager wifiManager;

// used for  millis();
unsigned long lastTenMinutesExecutionTime = 0;
unsigned long lastHourlyExecutionTime = 0;
const unsigned long hourInterval = 3600000; // 1 hour in milliseconds

// used for NTP and time in general
// Define time zones and rules for Knox County, Ohio, United States
TimeChangeRule myDST = {"EDT", Second, Sun, Mar, 2, -240}; // Eastern Daylight Time
TimeChangeRule mySTD = {"EST", First, Sun, Nov, 2, -300};  // Eastern Standard Time
Timezone myTZ(myDST, mySTD);
TimeChangeRule *tcr; // pointer to the time change rule, use to get TZ abbrev
unsigned long ntpTime = 0;
unsigned long currentTime = 0;
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

WiFiManager wifiManager(ssid, pass);

// RTC.h https://docs.arduino.cc/tutorials/uno-r4-wifi/rtc/

/**
 * Convert a RTCTime object to a time_t object
 */
    time_t RTCTimeToTimeElements(RTCTime inTime) {
      RTCTime _intime = inTime;
      tmElements_t te;
      
      te.Second = _intime.getSeconds();
      te.Minute = _intime.getMinutes();
      te.Hour = _intime.getHour();
      te.Wday = static_cast<uint8_t>(_intime.getDayOfWeek());
      te.Day = _intime.getDayOfMonth();
      te.Month = static_cast<uint8_t>(_intime.getMonth());
      te.Year = _intime.getYear() - 1970;
      
      time_t myTimeElements = makeTime(te);

      return myTimeElements;
  }

// RTC.h https://docs.arduino.cc/tutorials/uno-r4-wifi/rtc/
RTCTime currentRTCTime;
/**
 * Print the given time to the serial monitor in a user-friendly format
 */
void printTime(time_t t)
{
  Serial.print(year(t));
  Serial.print("-");
  Serial.print(month(t));
  Serial.print("-");
  Serial.print(day(t));
  Serial.print("T");
  Serial.print(hour(t));
  Serial.print(":");
  Serial.print(minute(t));
  Serial.print(":");
  Serial.print(second(t));
}

void setRTC()
{
  if (RTC.isRunning())
  {
    Serial.println("(setRTC) RTC is running");
    // Set the RTC using time from the NTP server
    unsigned long rtcTime = 0;
    rtcTime = wifiManager.getCurrentTime(); // ntpUnixTime
    RTCTime timeToSet(rtcTime);             // Convert the time to a RTCTime object
    RTC.setTime(timeToSet);                 // Set the RTC to the time received from the NTP server
    // Retrieve the date and time from the RTC and print them
    RTCTime nowTime;      // Create a RTCTime object to store the current time in UnixTime
    RTC.getTime(nowTime); // Get the current time from the RTC UnixTime

    Serial.print("(setRTC) The RTC was just set to: (rtcUnixTime) ");
    Serial.println(nowTime); // Print the new time
  }
  else
  {
    Serial.println("(setRTC) RTC is not running! Trying to Start it now...");
    RTC.begin();
  }
}

void setup()
{
  Serial.begin(115200);

  // wi-Fi items
  wifiManager.connect();
  wifiManager.printWiFiStatus();

  ntpTime = wifiManager.getCurrentTime();
  Serial.print("(Setup)Current NTP TimeStamp: ");
  Serial.println(ntpTime);

  // RTC items
  Serial.print("(Setup) Setting RTC Time: ");
  RTC.begin();
  RTCTime mytime(24, Month::JANUARY, 2024, 15, 00, 00, DayOfWeek::WEDNESDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(mytime);
  // Get current time from RTC
  RTC.getTime(currentRTCTime);

  // Unix timestamp
  Serial.print("(Setup) Unix RTC timestamp: ");
  Serial.println(currentRTCTime.getUnixTime());
  delay(2000);
  setRTC();
  // Your setup code
}

void hourlyFunctions()
{
  Serial.println("1 Hour has passed");
  setRTC();
}

// Check if an hour has passed since the last execution
void oneMinutesFunctions()
{
  // Your code for the hourly function
  Serial.println("1 minute has passed");
  if (RTC.isRunning())
  {
    Serial.println("(oneMinutesFunctions) RTC is running");
  }
  RTCTime rtcNowTime;
  RTC.getTime(rtcNowTime);
  Serial.print("(oneMinutesFunctions) Current UNIX-RTC Time:(UTC) ");
  Serial.println(rtcNowTime);

  ntpTime = wifiManager.getCurrentTime();
  Serial.print("(oneMinutesFunctions) Current UNIX-NTP Time:(UTC) ");
  Serial.println(ntpTime);
  Serial.print("(oneMinutesFunctions) Current RTC Time: ");
  time_t t_time = RTCTimeToTimeElements(rtcNowTime);
  printTime(t_time);
  Serial.println();
  Serial.print("(oneMinutesFunctions) Current NTP Time: ");
  t_time = ntpTime;
  printTime(t_time);
  Serial.println();

  // Convert UTC time to local time in Knox County, Ohio
  // time_t local = myTZ.toLocal(utc);

  // Print both UTC and local time in a user-friendly format
  // Serial.print("UTC: ");
  // printTime(utc);

  // Serial.print("Local Time (Knox County, OH): ");
  // printTime(local);

  Serial.println("EXIT oneMinutesFunctions()");
}

// Check if an hour has passed since the last execution
void tenMinutesFunctions()
{
  // Your code for the hourly function
  Serial.println("10 minutes has passed");
  RTCTime nowTime;
  RTC.getTime(nowTime);
  Serial.println("The Time is: " + String(nowTime));

  RTCTime currentTime;
  RTC.getTime(currentTime);
  Serial.println("The RTC was just set to: " + String(currentTime));

  // Convert UTC time to local time in Knox County, Ohio
  // time_t local = myTZ.toLocal(utc);

  // Print both UTC and local time in a user-friendly format
  // Serial.print("UTC: ");
  // printTime(utc);

  // Serial.print("Local Time (Knox County, OH): ");
  // printTime(local);

  Serial.println();
}
void loop()
{
  // Your main loop code

  unsigned long currentTime = millis();
  if (currentTime - lastHourlyExecutionTime >= hourInterval) // 1 hour
  {
    // Execute the function
    hourlyFunctions();
    // Update the last execution time
    lastHourlyExecutionTime = currentTime; // Declare the variable before using it
  }
  if (currentTime - lastTenMinutesExecutionTime >= hourInterval / 6) // 10 minutes
  {                                                                  // 10 minutes
    // Execute the function
    tenMinutesFunctions();
    // Update the last execution time
    lastTenMinutesExecutionTime = currentTime;
  }
  if (currentTime - lastTenMinutesExecutionTime >= hourInterval / 60) // 1 minute
  {                                                                   // 10 minutes
    // Execute the function
    oneMinutesFunctions();
    // Update the last execution time
    lastTenMinutesExecutionTime = currentTime;
  }
  // Your remaining loop code
}
