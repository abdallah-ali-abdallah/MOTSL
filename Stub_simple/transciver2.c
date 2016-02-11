// Simple Transciver stub using Arduino
// Transceiver 2

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <avr/wdt.h> //for Watchdog timer ( to avoid loop hanging ) 
                     //... bt here for restart for arduino
//writing pipes .. reading pipes
const uint64_t pipe[2]={0x1212121234LL, 0x1212121256LL};

RF24 radio(9,10); // Set up nRF24L01 radio on SPI bus plus pins 9 & 10

int i=0;
char re=0;
char income[32];
int sender_id;
byte retx=0;
uint8_t pipe_num;
void setup()
{
  Serial.begin(9600);
  //delay(1000);
  radio.begin();
  print_welcome_message();
  radio.openReadingPipe(1,pipe[0]);//For Receiving Operation
  radio.openWritingPipe(pipe[1]);//For Transmitting Operation
  radio.startListening();

}

//watchdog timer fn
//as timeout signal
void software_Reboot()
{
  wdt_enable(WDTO_15MS);
}

void print_welcome_message()
{
    
     Serial.print("if u want to send a Message to host1 press 1\n");
     Serial.print("or Wait for Receiving");
     Serial.println(); 
}


void select(int rec)
{
  Serial.print("host1  ");
  Serial.print(rec);
  Serial.println();
}

void trans()
{
    Serial.println("Enter a msg (max 32 Characters)");
    while(!Serial.available());
      delay(32); // for collection
      char tx[32];
      for(int j=0;j<32;j++)
      {
       tx[j]=Serial.read();
       if(tx[j]==(char)-1)    // to avoid writing unkown char in spaces
       {
         tx[j]='\0';
         goto loop1;
       }
      }
      loop1:
     loop2: 
     bool done = radio.write(tx, sizeof(tx));  
     if(done==false)
     { 
       retx++;
       Serial.println("Tx Failed");
       if(retx<10)
       {
       goto loop2;
       }else retx=0;
     }
     radio.startListening();
     print_welcome_message();
}
void loop()
{
  if(Serial.available())
  {
    re=Serial.read();
    if(re=='1')
    {
      radio.stopListening();
      radio.openWritingPipe(pipe[0]);
      select(2);
      trans();
    }else if(re=='R')
    {
      software_Reboot();   //where the system restarts without the need
                            //to interrupt the power
    }else
    {
      Serial.println("Error Selection");
    }
  }else if(radio.available(&pipe_num)) 
  {
    bool done=false;
      while(!done)
      {
        done = radio.read( income, sizeof(income) );
      }
       Serial.print("Received = ");
      Serial.print(income);
      Serial.println();
      
      sender_id = pipe_num;
      if(sender_id==1) // selection which pipe u 'll send via 
      {
      Serial.print("host1");
      Serial.println();
      }
      
  }
  
}


