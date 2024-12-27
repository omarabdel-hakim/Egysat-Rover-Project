//*****
#include "SSP.h"
#include "crc16.h"
#include "Rover_F1.h"
extern unsigned char distance1,distance2;
extern unsigned char TLM[7];

unsigned char * response=(unsigned char * )malloc(15);

bool start_scanFlag=false;

const int BUFFER_SIZE = 8;
unsigned char buf[BUFFER_SIZE];

unsigned char * ssp_=(unsigned char * )malloc(8);
const unsigned char * Resiveddata=(unsigned char * )malloc(4);

void setup() 
{  
  Serial.begin(9600);
  Serial1.begin(9600);
  Rover_init();
  
}

void loop() 
{ 
  UlTRASONIC1_start();
   
if (Serial1.available()>5) 
  {
      //Collect_TEM();
      delay(100);
      Serial1.readBytes(buf,BUFFER_SIZE);

       unsigned char Receiveddata = Exetreaction(buf,8,Resiveddata);  
      Serial.print("buf[0]=");
      Serial.println(buf[0],HEX);
      Serial.print("buf[3]=");
      Serial.println(buf[3],HEX);
         
      
     ssp_CMD_Response(buf,response);
      for(char i=0;i<8;i++)
      {
        Serial.print("response_RF="); 
        Serial.println(response[i],HEX);
      }
      
     // Serial1.write(response,8);

        
 
  
   Serial1.begin(9600);
   Serial.println("\n");
  }
if(start_scanFlag)
        {
         
          Start_scan();
        }
        else
        {
          MOTORS_stop();
         }

}
