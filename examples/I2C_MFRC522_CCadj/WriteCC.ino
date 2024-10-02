void clean256Buf(){
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
  Serial2.println(F("MIFARE_Ultralight_Write() Cleaned"));
  Serial2.println();
}

void writetoNTAG(uint8_t pagestart,int TotalPages,byte *bufinput){//( ){
    for (int i=0; i < TotalPages; i++) {
    //data is writen in blocks of 4 bytes (4 bytes per page)
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Ultralight_Write(pagestart+i, &bufinput[i*4], 4);
    if (status != MFRC522::STATUS_OK) {
      Serial2.print(F("MIFARE_Read() failed: "));
      Serial2.println(mfrc522.GetStatusCodeName(status));
      return;
    }
  }
  Serial2.println(F("MIFARE_Ultralight_Write() OK "));
  Serial2.println();
}