#include <Wire.h>
#include <Adafruit_PN532.h>
#include <RTClib.h>

// ---- Pin setup ----
#define PN532_IRQ   2
#define PN532_RESET 3
#define LED_PIN 25
#define ACTIVE_BUZZER_PIN 26
#define PASSIVE_BUZZER_PIN 27

// ---- Alarm time (24-hour format) ----
#define ALARM_HOUR 7
#define ALARM_MINUTE 0

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
RTC_DS3231 rtc;

// ---- Authorized UIDs ----
uint8_t authorizedUID1[] = {0xE7, 0xB9, 0xC8, 0x62}; // tag
uint8_t authorizedUID2[] = {0x8D, 0xFC, 0x50, 0x07}; // card

bool alarmTriggeredToday = false;
bool alarmActive = false;

bool checkUID(uint8_t *scannedUID, uint8_t scannedLength, uint8_t *knownUID, uint8_t knownLength) {
  if (scannedLength != knownLength) return false;
  for (uint8_t i = 0; i < scannedLength; i++) {
    if (scannedUID[i] != knownUID[i]) return false;
  }
  return true;
}

void setup() {
  Serial.begin(115200);

  Wire.begin();
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("PN532 not found!");
    while (1);
  }
  nfc.SAMConfig();

  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    while (1);
  }

  pinMode(LED_PIN, OUTPUT);
  pinMode(ACTIVE_BUZZER_PIN, OUTPUT);
  pinMode(PASSIVE_BUZZER_PIN, OUTPUT);

  Serial.println("Alarm clock ready.");
}

void loop() {
  DateTime now = rtc.now();

  // Reset the "already triggered today" flag right after midnight
  if (now.hour() == 0 && now.minute() == 0) {
    alarmTriggeredToday = false;
  }

  // Trigger the alarm at the set time (only once per day)
  if (!alarmActive && !alarmTriggeredToday &&
      now.hour() == ALARM_HOUR && now.minute() == ALARM_MINUTE) {
    alarmActive = true;
    alarmTriggeredToday = true;
    Serial.println("ALARM! Scan your card or tag to stop it.");
  }

  if (alarmActive) {
    // Flash LED + sound both buzzers while alarm is active
    digitalWrite(LED_PIN, (millis() / 300) % 2); // blinks every 300ms
    digitalWrite(ACTIVE_BUZZER_PIN, HIGH);
    tone(PASSIVE_BUZZER_PIN, 1000);

    // Check for a card scan
    uint8_t uid[7];
    uint8_t uidLength;
    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
      if (checkUID(uid, uidLength, authorizedUID1, sizeof(authorizedUID1)) ||
          checkUID(uid, uidLength, authorizedUID2, sizeof(authorizedUID2))) {
        Serial.println("Authorized scan - alarm off!");
        alarmActive = false;
        digitalWrite(LED_PIN, LOW);
        digitalWrite(ACTIVE_BUZZER_PIN, LOW);
        noTone(PASSIVE_BUZZER_PIN);
      } else {
        Serial.println("Unrecognized card - keep trying!");
      }
    }
  } else {
    // Idle state - everything off
    digitalWrite(LED_PIN, LOW);
    digitalWrite(ACTIVE_BUZZER_PIN, LOW);
    noTone(PASSIVE_BUZZER_PIN);
  }
}