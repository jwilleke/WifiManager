# WifiManager

A demonatration of a custom class which that could be used across mutiple projects.

Also shows some examples of not using delay within loop().

[WifiManager GitHub](https://github.com/jwilleke/WifiManager)

[Arduino UNO R4 WiFi Real-Time Clock](https://docs.arduino.cc/tutorials/uno-r4-wifi/rtc/)

## WifiManager.h

This is the header file for the `WiFiManager` class. It includes necessary libraries and declares the `WiFiManager` class along with its methods and member variables.

### Libraries

- `WiFi.h`: The main WiFi library for Arduino.
- `WiFiUdp.h`: A library that provides UDP protocol functionality over WiFi.
- `NTPClient.h`: A library for connecting to an NTP server and getting the current time.

### WiFiManager Class

The `WiFiManager` class is designed to manage a WiFi connection and synchronize time using a Network Time Protocol (NTP) client.

#### Member Variables

- `ssid`: A string that stores the SSID (network name) of the WiFi network.
- `password`: A string that stores the password of the WiFi network.
- `wifiUDP`: An instance of the `WiFiUDP` class, used for UDP functionality over WiFi.
- `timeClient`: An instance of the `NTPClient` class, used for getting the current time from an NTP server.

#### Methods

- `WiFiManager(String ssid, String password)`: The constructor of the `WiFiManager` class. It takes two parameters: `ssid` and `password`, which represent the SSID and password of the WiFi network. The constructor initializes the `ssid` and `password` member variables with the provided values. It also initializes `wifiUDP` and `timeClient` without parameters.
- `void connect()`: This method is responsible for establishing a WiFi connection and initializing the NTP client. It starts by calling `WiFi.begin(ssid, password)`, which initiates a connection to the WiFi network with the provided `ssid` and `password`. It then enters a while loop that continues until the WiFi connection is established. After establishing the WiFi connection, the method initializes the NTP client. It starts the NTP client with `timeClient.begin()`, sets the update interval to 60000 milliseconds (or 1 minute) with `timeClient.setUpdateInterval(60000)`, and retrieves the current time in a human-readable format with `timeClient.getFormattedTime()`.
- `void syncTime()`: This method is not shown in the provided code, but based on its name, it likely synchronizes the system time with the time from the NTP server.

## WifiManager.cpp

This is the implementation file for the `WiFiManager` class. It includes the `WifiManager.h` header file and defines the methods declared in the `WiFiManager` class.

### Libraries WifiManager.cpp

- `WifiManager.h`: The header file for the `WiFiManager` class.

## WiFiManager Class Implementation

The `WiFiManager` class is designed to manage a WiFi connection and synchronize time using a Network Time Protocol (NTP) client. This file provides the implementation for the methods declared in the `WiFiManager` class.

### Methods WifiManager.cpp

- `WiFiManager(String ssid, String password)`: The constructor of the `WiFiManager` class. It takes two parameters: `ssid` and `password`, which represent the SSID and password of the WiFi network. The constructor initializes the `ssid` and `password` member variables with the provided values. It also initializes `wifiUDP` and `timeClient` without parameters.
- `void connect()`: This method is responsible for establishing a WiFi connection and initializing the NTP client. It starts by calling `WiFi.begin(ssid, password)`, which initiates a connection to the WiFi network with the provided `ssid` and `password`. It then enters a while loop that continues until the WiFi connection is established. After establishing the WiFi connection, the method initializes the NTP client. It starts the NTP client with `timeClient.begin()`, sets the update interval to 60000 milliseconds (or 1 minute) with `timeClient.setUpdateInterval(60000)`, and retrieves the current time in a human-readable format with `timeClient.getFormattedTime()`.
- `void syncTime()`: This method is not shown in the provided code, but based on its name, it likely synchronizes the system time with the time from the NTP server.

## UNO R4 WiFi Real-Time Clock

[RTC.h](https://docs.arduino.cc/tutorials/uno-r4-wifi/rtc/) is a specific header for use with the real-time clock (RTC) on an Arduino UNO R4 WiFi board. The RTC is embedded in the UNO R4 WiFi's microcontroller (RA4M1).

The RTC.h provides a RTCTime Object for time which, a bit surpzingly, is different thant the Object provided from <TimeLib.h>. The main.cpp file provides a function ```time_t RTCTimeToTimeElements(RTCTime inTime)``` for converting RTCTime to a time_t Object.

When connected to our iMac via usb the time of the RTC drifts by at least a second every minute, which seem excessive.

We have left extensive console loging on for demonstation purposes but highly reccomend disabling all printing for production use.

## main.cpp

This is the main file of the Arduino project. It includes several libraries and declares global variables and objects that are used throughout the project.

## Libraries main.cpp

- `Arduino.h`: The main Arduino library.
- `WiFiManager.h`: A custom library for managing WiFi connections.
- `arduino_secrets.h`: A file where sensitive data like WiFi credentials are stored.
- `RTC.h`: A library for interfacing with the Real-Time Clock (RTC).
- `TimeLib.h`: A library for manipulating date and time.
- `Timezone.h`: A library for handling time zones.
- `my_config.h`: A custom configuration file.
- `LibPrintf.h`: A library for formatted printing.

### Global Variables and Objects

- `wifiManager`: An instance of the `WiFiManager` class, used to manage WiFi connections.
- `lastOneMinutesExecutionTime`, `lastTenMinutesExecutionTime`, `lastHourlyExecutionTime`: Variables to store the last execution times of certain tasks.
- `uptimeMinutes`: A variable to store the uptime in minutes.
- `hourInterval`: A constant representing one hour in milliseconds.
- `myDST`, `mySTD`: Objects representing the daylight saving time and standard time rules for a specific location.
- `myTZ`: An object representing the timezone for a specific location.
- `tcr`: A pointer to the current time change rule.
- `ssid`, `pass`: Variables to store the WiFi SSID and password.

### Console output

```bash
13:49:04:440 -> 439 minutes has passed(oneMinutesFunctions) RTC is running
13:49:04:440 -> (oneMinutesFunctions) Current UNIX-RTC Time:(UTC) 1706208540
13:49:04:450 -> (oneMinutesFunctions) Current UNIX-NTP Time:(UTC) 1706208536
13:49:04:450 -> (oneMinutesFunctions) RTC is ahead of NTP by: 4 seconds
13:49:04:461 -> (oneMinutesFunctions) Current RTC Time: 2024-01-25T18:49:00
13:49:04:461 -> (oneMinutesFunctions) Current NTP Time: 2024-01-25T18:48:56
13:49:04:463 -> EXIT oneMinutesFunctions()
13:50:04:456 -> 10 minutes has passed
13:50:04:456 -> The Time is: 2024-01-25T18:50:02
13:50:04:456 -> The RTC was just set to: 2024-01-25T18:50:02
13:50:04:456 -> 
13:50:04:456 -> 440 minutes has passed(oneMinutesFunctions) RTC is running
13:50:04:466 -> (oneMinutesFunctions) Current UNIX-RTC Time:(UTC) 1706208602
13:50:04:466 -> (oneMinutesFunctions) Current UNIX-NTP Time:(UTC) 1706208596
13:50:04:477 -> (oneMinutesFunctions) RTC is ahead of NTP by: 6 seconds
13:50:04:477 -> (oneMinutesFunctions) RTC and NTP are in sync - Sync RTC to NTP
13:50:04:487 -> (setRTC) RTC is running
13:50:04:487 -> (setRTC) Current NTP Time: (setRTC) Current RTC Time: 2024-01-25T18:50:02
13:50:04:498 -> (setRTC) The RTC was just set to: 1706208602
13:50:04:498 -> 
```
