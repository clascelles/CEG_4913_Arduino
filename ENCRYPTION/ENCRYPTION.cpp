#include "ENCRYPTION.h"

//Constructor
//Set the Encryption Key
ENCRYPTION::ENCRYPTION(String& keyString)
{
	byte key[KEY_LENGTH];

	//Convert KEY String to byte array
	for(int i=0; i<KEY_LENGTH; i++){
		key[i] = (byte) keyString[i];
	}

	//Set the encryption key in AES library
	aes.set_key (key, 128) ;

}


byte ENCRYPTION::encrypt(byte * message, int msgLength, byte * encrypted)
{ 
	//Reset Input Vector. Starts by default with all zeros.
	byte iv[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} ;

	//Clean encrypted byte array
	for(int i=0; i<MAX_MESSAGE_LENGTH; i++){
		encrypted[i] = 0;
	}

	//Add PHCS5 Padding to message
	//Get the number of blocks to encrypt.
	byte nblocks = insertPKCS5Padding(message, msgLength);

	//Encrypt message
	aes.cbc_encrypt (message, encrypted, nblocks, iv) ;

	//Return the encrypted message length
	return nblocks*BLOCK_SIZE;
}

byte ENCRYPTION::decrypt(byte * message, int msgLength, byte * encrypted)
{
	//Reset Input Vector. Starts by default with all zeros.
	byte iv[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} ;

	//Clean message byte array
	for(int i=0; i<MAX_MESSAGE_LENGTH; i++){
		message[i] = 0;
	}

	//Decrypt message
	aes.cbc_decrypt (encrypted, message, msgLength/BLOCK_SIZE, iv) ;

	//Remove the PKCD5Padding from the message
	msgLength = removePKCS5Padding(message, msgLength);

	//Return the message length
	return msgLength;
}

byte ENCRYPTION::insertPKCS5Padding(byte * message, int msgLength){

	int modPad = msgLength % BLOCK_SIZE;  
	int paddingBytes = BLOCK_SIZE - modPad;
	int nblocks = msgLength / BLOCK_SIZE;

	if(modPad > 0){
		nblocks++;

		for(int i=msgLength; i<msgLength+paddingBytes; i++){
			message[i] = paddingBytes;
		}
	}

	return nblocks;
}

byte ENCRYPTION::removePKCS5Padding(byte * message, int msgLength){

	int paddingValue = message[msgLength];

	bool paddingCheck = true;
	for(int i=msgLength; i<msgLength-paddingValue; i--){
		if(message[i] != paddingValue){
			paddingCheck = false;
		}
	}

	if(paddingCheck){
		for(int i=msgLength; i<msgLength-paddingValue; i--){
			message[i] = 0;
		}
	}

	return msgLength-paddingValue;
}

