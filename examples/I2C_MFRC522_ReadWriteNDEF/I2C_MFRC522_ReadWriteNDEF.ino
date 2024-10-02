#include <Wire.h>
#include "MFRC522_I2C.h"
#include "MifareUltralight.h"

#define RST_PIN PA8 // Arduino UNO Pin
// #define RST_PIN 14 // D5 Pin on NodeMCU
#include <SoftwareSerial.h>
#define     RXpin      PB11                     // MOD
#define     TXpin      PD8                     // MOD

using namespace ndef_mfrc522;

#define PIN_SERIAL_RX         PA3
#define PIN_SERIAL_TX         PA2

HardwareSerial Serial3(PB11,PD8);  // PB10 as TX, PB11 as RX for USART3

// 0x28 is i2c address on SDA. Check your address with i2cscanner if not match.
MFRC522 mfrc522(0x2D, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;          //create a MIFARE_Key struct named 'key', which will hold the card information
MFRC522::StatusCode status; //variable to get card status

//NfcAdapter nfc = NfcAdapter(&mfrc522);
////////////////////////////////////////////////////////////////////////////
int bufCleansize = 66 ;
byte bufClean[64]= {0};  //data transfer buffer (16+2 bytes data+CRC)
int TotalPageReqClean = bufCleansize/4   ;

void setup() {
    // reassign pin numbers for Serial1
  Serial2.setTx(PIN_SERIAL_TX);
  Serial2.setRx(PIN_SERIAL_RX);
  Serial3.setTx(TXpin);
  Serial3.setRx(RXpin);
  Serial3.begin(115200);         // Initialize serial communications with the PC
  Serial2.begin(115200);
  Wire.setSDA(PB9);
  Wire.setSCL(PB6);
  Wire.begin();                   // Initialize I2C

    mfrc522.PCD_Init(); // Init MFRC522 card

    // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    Serial2.println(F("BEWARE: Data will be written to the PICC, in sector #1"));
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  Serial.println();
 
  // Clean all pages
   clean64Buf();
  // Write data ***********************************************
   WriteNDEF();
  // Read data ***************************************************
   readfromNTAG();
  Serial2.println();
	// Dump debug info about the card; PICC_HaltA() is automatically called
	//mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  mfrc522.PICC_HaltA();
}