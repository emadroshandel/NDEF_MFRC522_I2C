# NDEF Library for Arduino/Particle

Read and Write NDEF messages on Mifare Ultralight NFC Tags with Arduino connected to MFRC522 RFID card.

NFC Data Exchange Format (NDEF) is a common data format that operates across all NFC devices, regardless of the underlying tag or device technology.

Originally forked from NDEF library that exclusively worked with NFC Shield, but adapted to work with the [MFRC522 Arduino](https://github.com/miguelbalboa/rfid) and [MFRC522 Particle](https://github.com/pkourany/MFRC522_RFID_Library) and limited to Mifare Ultralight NFC tags.

### Supports

- Reading from Mifare Ultralight tags.
- Writing to Mifare Ultralight tags.

### Requires

- [MFRC522 Arduino](https://github.com/miguelbalboa/rfid) or [MFRC522 Particle](https://github.com/pkourany/MFRC522_RFID_Library)

## Hello Github

This will write this Github URL to your tag which will allow your NFC-Enabled phone to read the URL and open a browser to this page.
See [WriteTag.ino](examples/WriteTag/WriteTag.ino)

```
#include <SPI.h>
#include <MFRC522.h>
#include "MifareUltralight.h"

#define SS_PIN 10
#define RST_PIN 6

using namespace ndef_mfrc522;

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup()
{
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
}

void loop()
{
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
}
```

Now read the tag using similar main code as above, but with this read excerpt from [ReadTag example](examples/ReadTag/ReadTag.ino).

```
  MifareUltralight reader = MifareUltralight(mfrc522);
  NfcTag tag = reader.read();
  tag.print();
```

... expect something similar to...

```
NFC Tag - NFC Forum Type 2
UID 04 0D 89 32 F1 4A 80

NDEF Message 1 record, 44 bytes
  NDEF Record
    TNF 0x1 Well Known
    Type Length 0x1 1
    Payload Length 0x28 40
    Type 55  U
    Payload 00 68 74 74 70 73 3A 2F 2F 67 69 74 68 75 62 2E 63 6F 6D 2F 61 72 6F 6C 6C 65 72 2F 4E 44 45 46 2D 4D 46 52 43 35 32 32  .https://github.com/aroller/NDEF-MFRC522
    Record is 44 bytes
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

## Releases

See [Releases](releases) for the latest.

Steps to release:

1. Update library.properties with the correct semantic version
   1. Update both in the version and the URL reference for explicit src reference
1. Merge PR into master
1. Create a Release named with the version in library.properties
1. `particle library publish` to update particle
   1. Arduino users rely on the github repo

## Known Issues

This software is in development. It works for the happy path. Error handling could use improvement. It runs out of memory, especially on the Uno board. Use small messages with the Uno. The Due board can write larger messages. Please submit patches.

- Read and Write in the same session fails
- Consider breaking NDEF files (NFC*.h/Ndef*.h) out from I/O files (MifareUltralight.h)
- Not all examples are converted to MFRC522 yet.

## Book

Need more info? Check out my book <a href="http://www.anrdoezrs.net/click-7521423-11260198-1430755877000?url=http%3A%2F%2Fshop.oreilly.com%2Fproduct%2F0636920021193.do%3Fcmp%3Daf-prog-books-videos-product_cj_9781449372064_%2525zp&cjsku=0636920021193" target="_top">
Beginning NFC: Near Field Communication with Arduino, Android, and PhoneGap</a><img src="http://www.lduhtrp.net/image-7521423-11260198-1430755877000" width="1" height="1" border="0"/>.

<a href="http://www.tkqlhce.com/click-7521423-11260198-1430755877000?url=http%3A%2F%2Fshop.oreilly.com%2Fproduct%2F0636920021193.do%3Fcmp%3Daf-prog-books-videos-product_cj_9781449372064_%2525zp&cjsku=0636920021193" target="_top"><img src="http://akamaicovers.oreilly.com/images/0636920021193/cat.gif" border="0" alt="Beginning NFC"/></a><img src="http://www.awltovhc.com/image-7521423-11260198-1430755877000" width="1" height="1" border="0"/>

## License

[BSD License](https://github.com/don/Ndef/blob/master/LICENSE.txt) (c) 2013-2014, Don Coleman
[BSD License](https://github.com/aroller/NDEF-MRFC522/blob/master/LICENSE.txt) (c) 2019, Aaron Roller
