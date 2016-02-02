/* RX Code Example */

//Import needed libraries 
#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <virtuabotixRTC.h>    // Creation of the Real Time Clock Object  
//SCLK -> 6, I/O -> 7, CE -> 8
virtuabotixRTC myRTC(6, 7, 8); 
 
//Declare Constants and Pin Numbers
#define CE_PIN   9
#define CSN_PIN 10

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
int tx=1;     //intialize variable tx
char rx[8]=""; //recieving data
boolean result;  // variable which will hold the compare result
int led = 3;


void setup(void)
{
/* for setting time .. from left side ( neglect 00 .. 22(minutes).. 12(hr)..00(sec) ..then day,month and weak */
//Hint .. if u 'll use next line for time setting so the code of Tx and Rx must be uploaded at the same time
// so that the diff. in time 'll be 1 or 2 sec.
// else ( for more accurate applications ) we already upload code for symch

myRTC.setDS1302Time(00, 27, 3, 00, 2, 2, 2016);



  Wire.begin();
  pinMode(led,OUTPUT);
  Serial.begin(9600);
  //delay(2000);
  radio.begin();
  // We will be using the Ack Payload feature, so please enable it
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setRetries(15,15);
  // Open pipes to other nodes for communication
  // This simple sketch opens a single pipes for these two nodes to communicate
  // back and forth.  One listens on it, the other talks to it.
  radio.openReadingPipe(1,pipe);
  radio.startListening();

}



void loop(void)
{

    // if there is data ready
    if ( radio.available() )
    {
      radio.writeAckPayload( 1, &tx, sizeof(unsigned int) );
      radio.read( rx, sizeof(rx) );
      
      Serial.print("REC: ");
      Serial.print(rx);
      Serial.print("    Tx: ");
      Serial.print(tx);
      Serial.println();
      tx++;
   delay(1000);
 
    }

   
// This allows for the update of variables for time or accessing the individual elements. 
myRTC.updateTime();   
 
String check =String (myRTC.hours)+String(myRTC.minutes);
String data = "xxyy";
String sendd = data + check ;

 char buf[8];
 sendd.toCharArray(buf,8);

 result = true; // set variable equal to one
 for (int x=0; x<8; x++){ 
  int test1 = rx[x];  // asign each index of arrays to test, one by one and compare
  int test2 = buf[x];
  
  if(test1 != test2){ // if any index comparison is not equal
                       //then the arrays are not equal and result will be 0.
   result = false; 
  }
  }
  if(result == true){ // if the arrays are equal, do something.
  digitalWrite(led,HIGH);}
 
}
