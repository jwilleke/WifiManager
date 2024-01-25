# WifiManager

A demonatration of a custom class which that could be used across mutiple projects.

Also shows some examples of not using delay within loop().

[WifiManager GitHub](https://github.com/jwilleke/WifiManager)

[Arduino UNO R4 WiFi Real-Time Clock](https://docs.arduino.cc/tutorials/uno-r4-wifi/rtc/)

## UNO R4 WiFi Real-Time Clock

When connected to our iMac via usb the ti,me of the RTC drifts by at least a second every minute, which seem excessive.

06:19:55:747 -> 1 minute has passed
06:19:55:747 -> (oneMinutesFunctions) RTC is running
06:19:55:747 -> (oneMinutesFunctions) Current UNIX-RTC Time:(UTC) 1706181602
06:19:55:757 -> (oneMinutesFunctions) Current UNIX-NTP Time:(UTC) 1706181595
06:19:55:757 -> (oneMinutesFunctions) RTC is ahead of NTP by: 7 seconds
06:19:55:757 -> (oneMinutesFunctions) RTC and NTP are in sync - Sync RTC to NTP
06:19:55:767 -> (setRTC) RTC is running
06:19:55:767 -> (setRTC) Current NTP Time: (setRTC) Current RTC Time: 2024-01-25T11:20:02
06:19:55:782 -> (setRTC) The RTC was just set to: 1706181602
06:19:55:782 ->
06:19:55:790 -> EXIT oneMinutesFunctions()
06:20:55:769 -> 1 minute has passed
06:20:55:769 -> (oneMinutesFunctions) RTC is running
06:20:55:769 -> (oneMinutesFunctions) Current UNIX-RTC Time:(UTC) 1706181656
06:20:55:779 -> (oneMinutesFunctions) Current UNIX-NTP Time:(UTC) 1706181655
06:20:55:779 -> (oneMinutesFunctions) RTC is ahead of NTP by: 1 seconds
06:20:55:779 -> (oneMinutesFunctions) Current RTC Time: 2024-01-25T11:20:56
06:20:55:790 -> (oneMinutesFunctions) Current NTP Time: 2024-01-25T11:20:55
06:20:55:792 -> EXIT oneMinutesFunctions()
