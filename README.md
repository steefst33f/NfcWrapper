# NFC Wrapper 

The Nfc Wrapper is a wrapper around the pn532 nfc module library code to read out NDEF data from different types of Nfc cards.
It implements the scanning for nfc ISO1443a tags and reading and returning the NDEF record(s) through callbacks.
This wrapper handles the reading flow so that you only need to implement the handlers for the callback to act on the nfc events.

## Supports 
- Interfacing with the PN532 Module with SPI (recommended)
- Interfacing with the Pn532 Module with I2C (Not recommendend because of possible clockstretching issues)
- Scanning for ISO1443a tags (Mifare Classic, Mifare Ultralight, NTAG21x, NTag424)
- Reading Type 2 (NTAG 21x) tags
- Reading Type 4 (NTAG 424 DNA) tags
- callbacks for events: 
    - on connected to nfc module (failed and success)
    - on foundTag (failed and success)
    - on readTag (failed and success)
    - on other detected errors

## Example 

In the Examples you can find an example of the implementation of the nfc wrappper that scans for a ISO1443a tag and will read and return its record if found.

