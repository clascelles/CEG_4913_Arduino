#include <AES.h>
#include <ENCRYPTION.h>

//Key and Message in String Format
String keyString = "Thisis16bitslong";
String message = "This is my message to send and encrypt with AES and this test string is long";

//Byte arrays that will be used
byte encrypted[128];
byte messageArray[128];

//Extra variable to mesure the time.
int t0, t1;

//Encryption object pointer
ENCRYPTION * encryption;

void setup(){
  
  //Initialize the Serial
  Serial.begin(9600);
  delay(500);
  
  Serial.println("");

  //Initialize the two message and encrypted arrays to ZEROs  
  for(int i=0; i<128; i++){
    messageArray[i] = 0;
    encrypted[i] = 0;
  }

  //Convert MESSAGE String to byte array
  message.getBytes(messageArray, message.length()+1);
  
  //Create the Encryption object
  t0 = micros();
  encryption = new ENCRYPTION(keyString);
  t1 = micros();
  
  Serial.print("Create Encryption object: ");
  Serial.print(t1-t0);
  Serial.println("us");
  
  //Encrypt
  t0 = micros();
  byte encryptLength = encryption->encrypt(messageArray, message.length(), encrypted);
  t1 = micros();
  
  Serial.print("Encrypt message: ");
  Serial.print(t1-t0);
  Serial.println("us");
  
  t0 = micros();
  byte messageLength = encryption->decrypt(messageArray, encryptLength, encrypted);
  t1 = micros();
  
  Serial.print("Decrypt message: ");
  Serial.print(t1-t0);
  Serial.println("us");
  
  //Print the decrypted string
  for(int i=0; i<messageLength; i++){
    Serial.print((char)messageArray[i]);
  }

}

void loop (){}
