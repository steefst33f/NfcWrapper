/**************************************************************************/
/*!
    @file     NfcWrapper.h
    @author   Steef
    @license  BSD
*/
/**************************************************************************/

#include <Arduino.h>

#pragma once

#ifndef Nfc_Wrapper_h
#define Nfc_Wrapper_h

#include <Adafruit_PN532.h>
#include <NfcAdapter.h>

class NfcWrapper {
    public:

    enum class State {
        notConnected,
        idle,
        scanning,
        inlisting, // identifying
        reading,
        releasing
    };

    enum class Error {
        readFailed,
        identifyFailed,
        releaseFailed,
        scanFailed,
        connectionModuleFailed,
        unknownError
    };

    NfcAdapter nfcAdapter;

    NfcWrapper(Adafruit_PN532 *nfcModule);
    void begin();
    void powerDownMode();
    void wakeUp();
    void scanForTag();

    // Actions
    void identifyTag();
    void readTagMessage();
    void releaseTag();

    // Handlers
    void setOnNfcModuleConnected(std::function<void(void)> setOnNfcModuleConnected);
    void setOnStartScanningForTag(std::function<void(void)> onStartScanningForTag);
    void setOnReadMessageRecord(std::function<void(String)> onReadMessageRecord);
    void setOnStartReadingTag(std::function<void(/*ISO14443aTag*/)> onStartReadingTag); // actualy on identifiedTag but it will also initiate Reading the NDEF message when called
    void setOnFailure(std::function<void(Error)> onFailure);

    bool isNfcModuleAvailable();

    String convertToStringFromBytes(uint8_t dataArray[], int sizeOfArray) {
        String stringOfData = "";
        for(int byteIndex = 0; byteIndex < sizeOfArray; byteIndex++) {
            stringOfData += (char)dataArray[byteIndex];
        }
        return stringOfData;
    }

    private:
    State state;
    bool _hasFoundNfcModule;

    //Events
    void connectedToNfcModule();
    void connectionNfcModuleFailed();
    void startScanningForTag();
    void tagFound();
    void noTagFound();
    void tagIdentifiedSuccess(/*ISO14443aTag tag*/);
    void tagIdentifyFailed();
    void readSuccess(String payloadString);
    void readFailed();
    void tagReleased();

    //Callbacks
    std::function<void(void)> _onNfcModuleConnected;
    std::function<void(void)> _onStartScanningTag;
    std::function<void(String)> _onReadMessageRecord;
    std::function<void(/*ISO14443aTag*/)> _onStartReadingTag;
    std::function<void(Error)> _onFailure;
};

#endif
