
#include "MifareUltralight.h"
#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 6

using namespace ndef_mfrc522;

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  Serial.begin(115200); // Initialize serial communications with the PC
  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522 card
  Serial.println(
      F("Hold a tag with NDEF record to the MFRC522. Read the output."));
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
    return;

  MifareUltralight reader = MifareUltralight(mfrc522);
  NfcTag tag = reader.read();
  tag.print();

  delay(5000); // avoids duplicate scans
}