/* TX Code Example */

/*  CONNECTION OF NRF24L01 
      with arduino uno
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 9
   4 - CSN to Arduino pin 10
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED
*/

// NOTE>>>>>>> NRF24 will only transmit password if you push the button.
//if Rx authenticate it the the action 'll be done ( we take a led as an example ) 

//Import needed libraries 
#include <string.h>
#include <Wire.h>
#include <virtuabotixRTC.h>    // Creation of the Real Time Clock Object  
//SCLK -> 6, I/O -> 7, CE -> 8
 virtuabotixRTC myRTC(6, 7, 8); 

#include <SPI.h> // Call SPI Library 
//#include <nRF24L01.h> // Call NRF Version Library
#include <RF24.h> // RF24 Header File

//Declare Constants and Pin Numbers
#define CE_PIN   9
#define CSN_PIN 10

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;
RF24 radio(CE_PIN, CSN_PIN); // Set up nRF24L01 radio on SPI bus plus pins 9 & 10
int const sw=3;
bool tx_flag=false;
void setup(void)
{

//Hint .. if u 'll use next line for time setting so the code of Tx and Rx must be uploaded at the same time
// so that the diff. in time 'll be 1 or 2 sec.

  myRTC.setDS1302Time(00, 27, 3, 00, 2, 2, 2016);
   Wire.begin();
   Serial.begin(9600);
   radio.begin();
  // We will be using the Ack Payload feature,
  //so please enable it
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  //radio.setRetries(15,15);
  // Open pipes to other nodes for communication
  // This simple sketch opens a single pipes for these two nodes to communicate
  // back and forth.  One listens on it, the other talks to it.
  radio.openWritingPipe(pipe);
 
}

void loop(void)
{   
myRTC.updateTime();   

String check =String (myRTC.hours)+String(myRTC.minutes);
String data = "xxyy";
String sendd = data + check ;

 char buf[8];
 sendd.toCharArray(buf,8);
 tx_flag=true;
   
   if(tx_flag)
   {

       if(digitalRead(sw)==HIGH){
     bool ok = radio.write(buf, sizeof(buf));
       }
   }
  
}
