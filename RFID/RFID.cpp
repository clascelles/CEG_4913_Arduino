#include "RFID.h"

RFID::RFID(String name, int pin_RESET, int pin_RX, int pin_TX){
  pinMode(pin_RESET, OUTPUT);
  pinMode(pin_RX, INPUT);
  pinMode(pin_TX, OUTPUT); 
  
  RESET_PIN = pin_RESET;
  rfidName = name; 
  rfidSerial = new SoftwareSerial(pin_RX, pin_TX);
  rfidSerial->begin(9600);
}

void RFID::resetRFID(boolean reset)
{
  if(reset == false)
    digitalWrite(RESET_PIN, HIGH);  
  else
    digitalWrite(RESET_PIN, LOW);
}

boolean RFID::equals(byte *tag1, byte *tag2)
{
  boolean result = false;
  byte j; 
  for (j=0; j < RFID_TAG_LENGTH ;j++) 
  {
    if(tag1[j] != tag2[j]) break;
    else if (j == RFID_TAG_LENGTH-1) result = true;
  }    
  return result;
}

/**
 * read data from rfid reader
 * @return rfid tag number
 *
 * Based on code by BARRAGAN, HC Gilje, djmatic, Martijn
 * http://www.arduino.cc/playground/Code/ID12 
 */
 
boolean RFID::readID12(byte *code)
{
  boolean result = false;
  byte val = 0;
  byte bytesIn = 0;
  byte tempbyte = 0;
  byte checksum = 0;
  
  // read 10 digit code + 2 digit checksum
  while (bytesIn < RFID_TAG_INPUT) 
  {                        
    if( rfidSerial->available() > 0) 
    { 
      val = rfidSerial->read();

      // if CR, LF, ETX or STX before the 10 digit reading -> stop reading
      if((val == 0x0D)||(val == 0x0A)||(val == 0x03)||(val == 0x02)) break;
      
      // Do Ascii/Hex conversion:
      if ((val >= '0') && (val <= '9')) 
        val = val - '0';
      else if ((val >= 'A') && (val <= 'F'))
        val = 10 + val - 'A';

      // Every two hex-digits, add byte to code:
      if ((bytesIn & 1) == 1) 
      {
        // make some space for this hex-digit by
        // shifting the previous hex-digit with 4 bits to the left:
        code[bytesIn >> 1] = (val | (tempbyte << 4));
        
        // If we're at the checksum byte, Calculate the checksum... (XOR)
        if (bytesIn >> 1 != RFID_TAG_LENGTH) checksum ^= code[bytesIn >> 1]; 
      } 
      else 
      {
        // Store the first hex digit first...
        tempbyte = val;                           
      }

      // ready to read next digit
      bytesIn++;                                
    } 
  }

  // read complete
  if (bytesIn == RFID_TAG_INPUT) 
  { 
    // valid tag
    if(code[5] == checksum) result = true; 
  }
 
  return result;
}

void RFID::readRFID(byte *returnTag)
{
  byte startChar = 0;
  int delayCount = 0;
  
    rfidSerial->listen(); //THIS IS TO SWITCH SOFTSERIAL FROM ONE RFID TO THE OTHER!!!
    resetRFID(false); //Put RFID out of reset
	  
	while(!rfidSerial->available() && delayCount < 200) //Wait for serial to be available (up to 200ms)
	{
		delayCount += 10;
		delay(10);
	}
	  // serial connection with rfid reader
      if (rfidSerial->available() > 0) 
      {
        // wait for the next STX byte
        while(rfidSerial->available() && startChar != 0x02)
          startChar = rfidSerial->read();
        
        // STX byte found -> RFID tag available
        if(startChar == 0x02)
        {
          if(readID12(rfidTagTemp))
          {
			  byte j; 
			for (j=0; j < RFID_TAG_LENGTH ;j++) 
			{
				returnTag[j] = rfidTagTemp[j];
			} 
          }
        }
      }
    resetRFID(true);  
}