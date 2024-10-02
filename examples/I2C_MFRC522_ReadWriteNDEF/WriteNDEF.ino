#define MIFARE_ULTRALIGHT_DEBUG
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

// 3   E1 10 6F 00