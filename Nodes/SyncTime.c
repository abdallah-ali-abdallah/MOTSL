/* Time-Sync Code between 2 nodes */

// this code for setting time of RTC 
//that's to make 2 RTC sychronized .. that's solved by 
//connectting a common switch (push button) and common pins

#include <virtuabotixRTC.h> 
 
 // Creation of the Real Time Clock Object
  
//SCLK -> 6, I/O -> 7, CE -> 8
 
virtuabotixRTC myRTC(6, 7, 8);  
 
#include <Wire.h>

int const sw=3;


void setup() {

  //set up serial and SDA&SCL
  Serial.begin(115200);
  Wire.begin();  
  pinMode(sw,INPUT);
}

void loop() {

myRTC.updateTime();   
Serial.print("Current Date / Time: "); 
Serial.print(myRTC.dayofmonth); 
Serial.print("/"); 
Serial.print(myRTC.month); 
Serial.print("/");
Serial.print(myRTC.year); 
Serial.print(" ");
Serial.print(myRTC.hours);
Serial.print(":");  
Serial.print(myRTC.minutes);
Serial.print(":");
Serial.println(myRTC.seconds);

// Delay so the program doesn't print non-stop
delay( 5000); 

 if(digitalRead(sw)==HIGH){
    myRTC.setDS1302Time(00, 27, 10, 00, 31, 1, 2016);
  }
  
}
