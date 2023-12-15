/**************************************************************************/
/*!
    @file     NfcWrapper.cpp
    @author   Steef
    @license  BSD
*/
/**************************************************************************/

#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  while (!Serial) {};

  Serial.println(__FILE__);
  Serial.println("Compiled: " __DATE__ ", " __TIME__);
}

void loop() {

}
