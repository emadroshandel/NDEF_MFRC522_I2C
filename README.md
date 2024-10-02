# NDEF Library for Arduino/Particle

Read and Write NDEF messages on Mifare Ultralight NFC Tags with Arduino connected to MFRC522 RFID card.

NFC Data Exchange Format (NDEF) is a common data format that operates across all NFC devices, regardless of the underlying tag or device technology.

Originally forked from NDEF library that exclusively worked with NFC Shield. The library has been later adjusted by Don Coleman and Aaron Roller to work with the [MFRC522 Arduino](https://github.com/miguelbalboa/rfid) and [MFRC522 Particle](https://github.com/pkourany/MFRC522_RFID_Library) and limited to Mifare Ultralight NFC tags. The previous versions have issues for communcation through I2C. This version have been updated to address the I2C communication issue with a proper example. Note that the [MFRC522_I2C_Lib](https://github.com/emadroshandel/MFRC522_I2C_Lib) is required for utilization of this library. 

### Supports

- Reading from Mifare Ultralight tags.
- Writing to Mifare Ultralight tags.
- Works on Arduino and Particle (Gen 3 xenon/argon/boron)

### Requires

- [MFRC522_I2C_Lib](https://github.com/emadroshandel/MFRC522_I2C_Lib).

### Read/Write using Android and Iphone apps
- Understanding the Capability Container (CC): The Capability Container (CC) is a critical component of an NFC tag, typically located in the fourth sector. It defines the memory layout and access conditions of the tag, ensuring that the tag can be read and written correctly by NFC-enabled devices.
- Structure of the Capability Container: The CC is usually 4 bytes long and contains the following information:
- Magic Number (E1): Indicates that the tag is NFC Forum Type 2 compliant.
- Version Number (10): Specifies the version of the NFC Forum Type 2 Tag specification.
- Memory Size (6F): Indicates the total memory size of the tag in 8-byte blocks.
- Access Conditions (00): Defines the read/write access conditions for the tag.

Based on the explanations above, I have included a code example in the example folder to adjust the CC sector of NFC tags, enabling read and write permissions while interfacing the tag with both Android and iPhone devices.

## Hello Github

This will write this Github URL to your tag which will allow your NFC-Enabled phone to read the URL and open a browser to this page.
See [WriteTag.ino](examples/WriteTag/WriteTag.ino)

```
#include <Wire.h>
#include <MFRC522_I2C.h>
#include "MifareUltralight.h"

#define RST_PIN PA8 // Arduino Pin
// 0x2D is i2c address on SDA. Check your address with i2cscanner if not match.
MFRC522 mfrc522(0x2D, RST_PIN);   // Create MFRC522 instance.

using namespace ndef_mfrc522;

void setup()
{
  Serial2.begin(115200);
  Wire.setSDA(PB9);
  Wire.setSCL(PB6);
  Wire.begin();                   // Initialize I2C
  mfrc522.PCD_Init(); // Init MFRC522 card
}

void loop()
{
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
```

Read function is as follows:

```
void readfromNTAG(){
  MifareUltralight reader = MifareUltralight(mfrc522);
  NfcTag tag = reader.read();
  if (tag.hasNdefMessage()) // every tag won't have a message
  {
      NdefMessage mess = tag.getNdefMessage();
      Serial2.print("\nThis NFC Tag contains an NDEF Message with ");
      Serial2.print(mess.getRecordCount());
      Serial2.print(" NDEF Record");
      if (mess.getRecordCount() != 1) {
        Serial.print("s");
      }
      Serial.println(".");

            // cycle through the records, printing some info from each
      int recordCount = mess.getRecordCount();
      for (int i = 0; i < recordCount; i++)
      {
        Serial.print("\nNDEF Record ");Serial.println(i+1);
        NdefRecord record = mess.getRecord(i);
        // NdefRecord record = message[i]; // alternate syntax

        Serial.print("  TNF: ");Serial.println(record.getTnf());
        Serial.print("  Type: ");Serial.println(record.getType()); // will be "" for TNF_EMPTY

        // The TNF and Type should be used to determine how your application processes the payload
        // There's no generic processing for the payload, it's returned as a byte[]
        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);

        // Force the data into a String (might work depending on the content)
        // Real code should use smarter processing
        String payloadAsString = "";
        for (int c = 0; c < payloadLength; c++) {
          payloadAsString += (char)payload[c];
        }
        Serial.print("  Payload (as String): ");
        Serial.println(payloadAsString);

        // id is probably blank and will return ""
        String uid = record.getId();
        if (uid != "") {
          Serial.print("  ID: ");Serial.println(uid);
        }
      }
  }
}
```

... Write function is as f

```
void WriteNDEF()
{
NdefMessage message = NdefMessage();
        message.addTextRecord("Hello World!");
        message.addUriRecord("http://www.practiceNDEF.test");
        message.addTextRecord("The Second Text");
        message.addUriRecord("http://www.SecondWeb.i2c");
        MifareUltralight writer = MifareUltralight(mfrc522);
        bool success = writer.write(message);
        if (success) {
            Serial.println("Success. Try reading this tag with your phone.");
        } else {
            Serial.println("Write failed");
        }
}
```

- Type 55 U -> Indicates URL
- Decode the payload from ASCII and it will spell out your URL
- See the url written

### MifareUltralight

The user interacts with the MifareUltralight to read and write NFC tags using the MFRC522.

Read a message from a tag

    MifareUltralight reader = MifareUltralight(mfrc522);
    NfcTag tag = reader.read();

Write a message to a tag

    NdefMessage message = NdefMessage();
    MifareUltralight writer = MifareUltralight(mfrc522);
    bool success = writer.write(message);

Clean a tag. Cleaning resets a tag back to a factory-like state. For Mifare Ultralight, the tag is zeroed and left empty.

    MifareUltralight writer = MifareUltralight(mfrc522);
    bool success = writer.clean();

### NfcTag

Reading a tag with the shield, returns a NfcTag object. The NfcTag object contains meta data about the tag UID, technology, size. When an NDEF tag is read, the NfcTag object contains a NdefMessage.

### NdefMessage

A NdefMessage consist of one or more NdefRecords.

The NdefMessage object has helper methods for adding records.

    ndefMessage.addTextRecord("hello, world");
    ndefMessage.addUriRecord("http://arduino.cc");

The NdefMessage object is responsible for encoding NdefMessage into bytes so it can be written to a tag. The NdefMessage also decodes bytes read from a tag back into a NdefMessage object.

### NdefRecord

A NdefRecord carries a payload and info about the payload within a NdefMessage.

### Specifications

This code is based on the "NFC Data Exchange Format (NDEF) Technical Specification" and the "Record Type Definition Technical Specifications" that can be downloaded from the [NFC Forum](http://www.nfc-forum.org/specs/spec_license).

### Tests

- Unit tests from original repo work. Load them to arduino and look for success.
- The example and library have been successfully tested and validated on both the Arduino UNO and STM32F407G. Please note that the available example and defined pin numbers are based on the tested STM32F. You may need to adjust the pins for a different processor.
- It has been succesfully read from and write on ST25DV02K-W1R8S3 and NT3H1101 tags.
- The interfacing of the programmed tag with the Android and iPhone apps developed using Microsoft Power Apps has been validated.

### Usage

#### Arduino Usage

The library is not yet published in the Library Manager so you must treat it as a private library.
* Read [Arduino Libraries](https://www.arduino.cc/en/hacking/libraries) for how to use private library.

#### Particle Usage

The library is published and can easily install, but unfortunately there is a conflict between constants in `Arduino.h` and `spark_wiring_arduino_constants.h` which requires a little extra effort.

```
In file included from ./inc/Arduino.h:27:0,
                 from .../lib/NDEF-MFRC522/src/Ndef.h:9,
                 from .../lib/NDEF-MFRC522/src/MifareUltralight.h:4,
                 from .../src/school-tag-station-particle.ino:6:
../wiring/inc/spark_wiring_arduino_constants.h:152:18: error: conflicting declaration 'typedef uint32_t word'
```

* Open your project in Particle Workbench
* Install `NDEF_MFRC522_I2C`
* Open `spark_wiring_arduino_constants.h` in your particle library installed
    * MacOS: `/Users/{you}/.particle/toolchains/deviceOS/{version}/firmware-{version}/wiring/inc/`
* comment out `typedef uint32_t word;`
* Should compile and install without an error.

## Book

Need more info? Check out my book <a href="http://www.anrdoezrs.net/click-7521423-11260198-1430755877000?url=http%3A%2F%2Fshop.oreilly.com%2Fproduct%2F0636920021193.do%3Fcmp%3Daf-prog-books-videos-product_cj_9781449372064_%2525zp&cjsku=0636920021193" target="_top">
Beginning NFC: Near Field Communication with Arduino, Android, and PhoneGap</a><img src="http://www.lduhtrp.net/image-7521423-11260198-1430755877000" width="1" height="1" border="0"/>.

<a href="http://www.tkqlhce.com/click-7521423-11260198-1430755877000?url=http%3A%2F%2Fshop.oreilly.com%2Fproduct%2F0636920021193.do%3Fcmp%3Daf-prog-books-videos-product_cj_9781449372064_%2525zp&cjsku=0636920021193" target="_top"><img src="http://akamaicovers.oreilly.com/images/0636920021193/cat.gif" border="0" alt="Beginning NFC"/></a><img src="http://www.awltovhc.com/image-7521423-11260198-1430755877000" width="1" height="1" border="0"/>

## License

[BSD License](https://github.com/don/Ndef/blob/master/LICENSE.txt) (c) 2013-2014, Don Coleman
[BSD License](https://github.com/aroller/NDEF-MRFC522/blob/master/LICENSE.txt) (c) 2019, Aaron Roller
