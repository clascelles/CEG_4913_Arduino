#ifndef ENCRYPTION_h
#define ENCRYPTION_h

//#include "Arduino.h"
#include <AES.h>
#include "Arduino.h"

typedef unsigned char byte;

// settings
#define KEY_LENGTH   16				// 16 Bytes
#define BLOCK_SIZE	 16				// 16 Bytes
#define MAX_MESSAGE_LENGTH    128	// Maximum message length

#define SUCCESS 0
#define FAILURE 1

class ENCRYPTION{
public:
	ENCRYPTION(String& keyString); 
	byte encrypt(byte * message, int msgLength, byte * encrypted);
	byte decrypt(byte * message, int msgLength, byte * encrypted);	

private:  
	AES aes;
	byte insertPKCS5Padding(byte * message, int msgLength);
	byte removePKCS5Padding(byte * message, int msgLength);


};

#endif