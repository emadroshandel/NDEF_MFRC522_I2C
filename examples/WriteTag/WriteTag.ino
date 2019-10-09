
/**
 * Example Arduino code that writes a URL to the tag in NDEF format
 * allowing a phone to read the tag and open to a browser.
 * Success if your phone opens to this Github repo.
 */
#include "MifareUltralight.h"
#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 6
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  Serial.begin(115200); // Initialize serial communications with the PC
  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522 card
  Serial.println(F("Hold a tag to the MFRC522 and look for success."));
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
    return;

  NdefMessage message = NdefMessage();
  String url = String("https://github.com/aroller/NDEF-MFRC522");
  message.addUriRecord(url);
  MifareUltralight writer = MifareUltralight(mfrc522);
  bool success = writer.write(message);
  if (success)
    Serial.println(F("Success. Now read the tag with your phone."));
  else
    Serial.println(F("Failure. See output above?"));
  delay(5000); // avoids duplicate scans
}