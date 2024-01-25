#include <Arduino.h>
#include "WiFiManager.h"
#include <arduino_secrets.h>
#include <RTC.h>              // RTC.h https://docs.arduino.cc/tutorials/uno-r4-wifi/rtc/
#include <TimeLib.h>
#include <Timezone.h>
#include <my_config.h>
#include <LibPrintf.h>

extern WiFiManager wifiManager;

// used for  millis();
unsigned long lastOneMinutesExecutionTime = 0;
unsigned long lastTenMinutesExecutionTime = 0;
unsigned long lastHourlyExecutionTime = 0;
unsigned long uptimeMinutes = 0;
const unsigned long hourInterval = 3600000; // 1 hour in milliseconds

// used for NTP and time in general
// Define time zones and rules for Knox County, Ohio, United States
TimeChangeRule myDST = {"EDT", Second, Sun, Mar, 2, -240}; // Eastern Daylight Time
TimeChangeRule mySTD = {"EST", First, Sun, Nov, 2, -300};  // Eastern Standard Time
Timezone myTZ(myDST, mySTD);
TimeChangeRule *tcr; // pointer to the time change rule, use to get TZ abbrev

// used for wi-fi
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

/**
 * @class WiFiManager
 * @brief A class for managing WiFi connections.
 *
 * This class provides methods for connecting to WiFi networks and managing the connection settings.
 * It simplifies the process of connecting to a WiFi network by handling the configuration and connection process.
 * The class also provides methods for resetting the WiFi settings and managing the access point mode.
 */
WiFiManager wifiManager(ssid, pass);


/**
 * @brief Converts an RTCTime object to TimeElements.
 * 
 * This function takes an RTCTime object and converts it to TimeElements, which represents the individual
 * components of time such as year, month, day, hour, minute, and second.
 * 
 * @param inTime The RTCTime object to be converted.
 * @return TimeElements The converted TimeElements object.
 */
time_t RTCTimeToTimeElements(RTCTime inTime)
{
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

/**
 * @brief Prints the given time.
 * 
 * @param t The time to be printed.
 */
void printTime(time_t t)
{
  Serial.print(year(t));
  Serial.print("-");
  if (month(t) < 10) {
    Serial.print("0");
  }
  Serial.print(month(t));
  Serial.print("-");
  if (day(t) < 10) {
    Serial.print("0");
  }
  Serial.print(day(t));
  Serial.print("T");
  if (hour(t) < 10) {
    Serial.print("0");
  }
  Serial.print(hour(t));
  Serial.print(":");
  if (minute(t) < 10) {
    Serial.print("0");
  }
  Serial.print(minute(t));
  Serial.print(":");
  if (second(t) < 10) {
    Serial.print("0");
  }
  Serial.print(second(t));
}

/**
 * @brief Sets the Real-Time Clock (RTC).
 * 
 * This function is responsible for setting the Real-Time Clock (RTC) in the Arduino sketch.
 * It performs the necessary operations to configure the RTC according to the desired settings.
 * 
 * @note This function assumes that the necessary libraries and hardware components are properly set up.
 * 
 * @return void
 */
void setRTC()
{
  if (RTC.isRunning())
  {
    Serial.println("(setRTC) RTC is running");

    // Get NTP Time using time from the NTP server
    unsigned long ntpUnixTime = 0;
    ntpUnixTime = wifiManager.getCurrentTime(); // ntpUnixTime
    Serial.print("(setRTC) Current NTP Time: ");

    Serial.print("(setRTC) Current RTC Time: ");
    RTCTime rtcNowTime;
    RTC.getTime(rtcNowTime); // Get the current time from the RTC UnixTime
    time_t t_time = RTCTimeToTimeElements(rtcNowTime);
    printTime(t_time);
    Serial.println();
    RTCTime timeToSet(ntpUnixTime); // Convert the time to a RTCTime object
    RTC.setTime(timeToSet);         // Set the RTC to the time received from the NTP server
    // Retrieve the date and time from the RTC and print them
    RTCTime nowTime;      // Create a RTCTime object to store the current time in UnixTime
    RTC.getTime(nowTime); // Get the current time from the RTC UnixTime
    t_time = RTCTimeToTimeElements(rtcNowTime);
    Serial.print("(setRTC) The RTC was just set to: ");
    Serial.println(t_time); // Print the new time
    Serial.println();
  }
  else
  {
    Serial.println("(setRTC) RTC is not running! Trying to Start it now...");
    RTC.begin();
  }
}

/**
 * @brief Function called once when the program starts.
 *        It is used to initialize variables, pin modes, libraries, etc.
 */
void setup()
{
  Serial.begin(115200);

  // wi-Fi items
  wifiManager.connect();
  wifiManager.printWiFiStatus();

  unsigned long ntpTime = wifiManager.getCurrentTime();
  Serial.print("(Setup)Current NTP TimeStamp: ");
  Serial.println(ntpTime);

  // RTC items
  Serial.print("(Setup) Setting RTC Time: ");
  RTC.begin();
  RTCTime mytime(24, Month::JANUARY, 2024, 15, 00, 00, DayOfWeek::WEDNESDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(mytime);
  // Get current time from RTC
  RTCTime currentRTCTime;
  RTC.getTime(currentRTCTime);

  // Unix timestamp
  Serial.print("(Setup) Unix RTC timestamp: ");
  Serial.println(currentRTCTime.getUnixTime());
  delay(2000);
  setRTC();
  // Your setup code
}

/**
 * @brief Perform hourly functions.
 * 
 * This function is responsible for executing tasks that need to be performed on an hourly basis.
 * Add your code inside this function to perform the desired hourly tasks.
 */
void hourlyFunctions()
{
  Serial.println("1 Hour has passed");
  setRTC();
}

// Check if an hour has passed since the last execution
/**
 * @brief Performs the functions that need to be executed every minute.
 */
void oneMinutesFunctions()
{
  // Your code for the hourly function
  printf("%ld minutes has passed", uptimeMinutes);
  if (RTC.isRunning())
  {
    Serial.println("(oneMinutesFunctions) RTC is running");
  }
  RTCTime rtcNowTime;
  RTC.getTime(rtcNowTime);
  Serial.print("(oneMinutesFunctions) Current UNIX-RTC Time:(UTC) ");
  Serial.println(rtcNowTime.getUnixTime());
  // get NTP Time
  unsigned long ntpTime = wifiManager.getCurrentTime();
  Serial.print("(oneMinutesFunctions) Current UNIX-NTP Time:(UTC) ");
  Serial.println(ntpTime);
  long int diff = abs((long int)ntpTime - (long int)rtcNowTime.getUnixTime());
  if (((long int)ntpTime - (long int)rtcNowTime.getUnixTime()) > 0)
  {
    printf("(oneMinutesFunctions) NTP is ahead of RTC by: %ld seconds\n", ((long int)ntpTime - (long int)rtcNowTime.getUnixTime()));
  }
  else if (((long int)rtcNowTime.getUnixTime() - (long int)ntpTime) > 0)
  {
    printf("(oneMinutesFunctions) RTC is ahead of NTP by: %ld seconds\n", ((long int)rtcNowTime.getUnixTime() - (long int)ntpTime));
  } else {
    printf("(oneMinutesFunctions) RTC and NTP are in sync\n");
  }
  if (diff > 5) {
    printf("(oneMinutesFunctions) RTC and NTP are in sync - Sync RTC to NTP\n");
    setRTC();
  }

  Serial.print("(oneMinutesFunctions) Current RTC Time: ");
  time_t t_time = RTCTimeToTimeElements(rtcNowTime);
  printTime(t_time);
  Serial.println();
  Serial.print("(oneMinutesFunctions) Current NTP Time: ");
  t_time = ntpTime;
  printTime(t_time);
  Serial.println();

  Serial.println("EXIT oneMinutesFunctions()");
}

// Check if an hour has passed since the last execution
/**
 * @brief Performs the functions that need to be executed every ten minutes.
 */
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
  Serial.println();
}

/**
 * @brief The main loop function.
 * 
 * This function is called repeatedly in the Arduino sketch. It is responsible for executing the main logic of the program.
 * 
 * @return void
 */
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
  if (currentTime - lastOneMinutesExecutionTime >= hourInterval / 60) // 1 minute
  {                                                                   
    // Execute the function
    uptimeMinutes++;
    oneMinutesFunctions();
    // Update the last execution time
    lastOneMinutesExecutionTime = currentTime;
  }
  // Your remaining loop code
}
