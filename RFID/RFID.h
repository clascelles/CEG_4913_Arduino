#ifndef RFID_h
#define RFID_h

#include "Arduino.h"
#include <SoftwareSerial.h>

// settings
#define RFID_TAG_LENGTH   5 // 5 Bytes
#define RFID_TAG_INPUT    12 // DATA (10 ASCII) + CHECK SUM (2 ASCII)

class RFID{
  public:
    RFID(String name, int pin_RESET, int pin_RX, int pin_TX); 
    boolean equals(byte *tag1, byte *tag2);
	void readRFID(byte *returnTag);
   
  private:    
    int RESET_PIN;
	String rfidName;
	SoftwareSerial *rfidSerial;
	byte rfidTagTemp[6]; // temp tag (1-5 is tag, 6 is checksum)

	boolean readID12(byte *code); //used for parsing
	void resetRFID(boolean reset);
};

#endif