# NFC Alarm Clock

An embedded alarm clock built on an ESP32 that requires scanning an NFC card or tag to turn off the alarm — built to solve the very real problem of sleeping through a snooze button.

## Overview

Instead of a snooze button you can hit half-asleep, this alarm clock forces you to get up and physically scan an authorized NFC card/tag to silence it. The alarm state is tracked with a real-time clock so it only fires once per day at the set time.

### Hardware

- **ESP32 Dev Module** — main microcontroller
- **PN532 NFC/RFID Reader** — wired in I2C mode, used to scan authorized cards/tags
- **DS3231 RTC Module** — real-time clock for accurate alarm timekeeping
- **OLED Display** — shows current time / menu state
- **Active + Passive Buzzers** — dual buzzer output for the alarm sound
- **LED** — visual alarm indicator (blinks while alarm is active)
- **Push Button(s)** — menu navigation for setting alarm and clock time

## Features

- NFC-based alarm dismissal with a whitelist of authorized UIDs
- RTC-based alarm triggering (fires once per day, resets after midnight)
- Dual buzzer + blinking LED alarm feedback
- Button-driven menu for setting alarm time and clock time
- OLED display for status/time feedback

## Firmware Notes

- Built in the Arduino IDE using the `Adafruit_PN532` and `RTClib` libraries
- Resolved a CP2102 USB driver issue during initial ESP32 setup
- NFC UID matching implemented with a simple byte-array comparison against a whitelist

## Roadmap

- Custom alarm melody (replacing the default tone)
- Optional WiFi / web dashboard for remote configuration

## Status

Actively in development — core alarm, NFC, RTC, and buzzer logic working. Menu system and OLED display integration in progress.
