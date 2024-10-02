void clean64Buf(){
    // Write data ***********************************************
  for (int i=0; i < TotalPageReqClean; i++) {
    //data is writen in blocks of 4 bytes (4 bytes per page)
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Ultralight_Write(3+i, &bufClean[i*4], 4);
    if (status != MFRC522::STATUS_OK) {
      Serial2.print(F("MIFARE_Read() failed: "));
      Serial2.println(mfrc522.GetStatusCodeName(status));
      return;
    }
  }
  Serial2.println(F("MIFARE Ultralight: Cleaned"));
  Serial2.println();
}

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