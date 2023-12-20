/**************************************************************************/
/*!
    @file     main.cpp
    @author   Steef
    @license  BSD
*/
/**************************************************************************/

#include <Arduino.h>
#include <NfcWrapper.h>

//NFC module
#if NFC_SPI
#define PN532_SCK  (25)
#define PN532_MISO (27)
#define PN532_MOSI (26)
#define PN532_SS   (33)
Adafruit_PN532 *nfcModule = new Adafruit_PN532(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);
#elif NFC_I2C
#define PN532_IRQ     (2)
#define PN532_RESET   (3)
Adafruit_PN532 *nfcModule = new Adafruit_PN532(PN532_IRQ, PN532_RESET);
#else
#error "Need NFC interface defined! Define NFC_SPI=1 or NFC_I2C=1!"
#endif

NfcWrapper nfc = NfcWrapper(nfcModule);

// Nfc callback handlers
void onNfcModuleConnected();
void onStartScanningTag();
void onReadingTag(/*ISO14443aTag tag*/);
void onReadTagRecord(String stringRecord);
void onFailure(NfcWrapper::Error error);

void scanningForNfc();

void setup()
{
  Serial.begin(115200);
  while (!Serial) {};

  Serial.println(__FILE__);
  Serial.println("Compiled: " __DATE__ ", " __TIME__);
  
  //setup nfc callback handlers
  nfc.setOnNfcModuleConnected(onNfcModuleConnected);
  nfc.setOnStartScanningForTag(onStartScanningTag);
  nfc.setOnReadMessageRecord(onReadTagRecord);
  nfc.setOnStartReadingTag(onReadingTag);
  nfc.setOnFailure(onFailure);

  //connect to nfc module
  nfc.begin();
}

void loop() {
  scanningForNfc();
  delay(1000);
}

//-----------------------------------------------------------------------//

void scanningForNfc() {
  Serial.println("scanningForNfc..");
  if (nfc.isNfcModuleAvailable()) {
    nfc.scanForTag();
  }
}

//NFC callback handlers
void onNfcModuleConnected() {
  Serial.println("NFC module Connected!");
}

void onStartScanningTag() {
  Serial.println(String("Waiting for NFC tag.."));
}

void onReadingTag(/*ISO14443aTag tag*/) {
  Serial.println("Reading NFC tag.. \nDon't remove the tag untill done");
  // tag.print();
}

void onReadTagRecord(String stringRecord) {
  Serial.println("Read NFC record: " + stringRecord);
}

void onFailure(NfcWrapper::Error error) {
  if (error == NfcWrapper::Error::scanFailed) {
     Serial.println("No NFC tag detected yet..");
      return;
  }

  String errorString;
  switch (error) {
    case NfcWrapper::Error::connectionModuleFailed:
      errorString = "Not connected to NFC module";
      break;
    case NfcWrapper::Error::scanFailed:
      errorString = "Failed ";
      break;
    case NfcWrapper::Error::readFailed:
      errorString = "Failed to read tag";
      break;
    case NfcWrapper::Error::identifyFailed:
      errorString = "Couldn't identify tag";
      break;
    case NfcWrapper::Error::releaseFailed:
      errorString = "Failed to release tag properly";
      break;
    default:
      errorString = "unknow error";
      break;
  }
  Serial.println("NFC Error" + errorString);

  if (error == NfcWrapper::Error::connectionModuleFailed) {
    Serial.println("\n-----------------------------------------------------------------------");
    Serial.println("Error! nfc Module not available:");
    Serial.println("[ ] Check if nfc module is properly connected.");
    Serial.println("[ ] Check if DIP switch is set for selected interface.");
    Serial.println("[ ] Check if correct interface is set in platformio.ini flags");
    Serial.println("-----------------------------------------------------------------------\n");

    delay(2000);
    nfc.powerDownMode();
    nfc.begin();
  }
}
