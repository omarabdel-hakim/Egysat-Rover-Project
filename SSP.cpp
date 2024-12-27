#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crc16.h"
#include <Arduino.h>
#include "Rover_F1.h"

#define ACK 0x01

extern unsigned char TLM[7];
unsigned char *  SSP_P_Con (unsigned char src, unsigned char * Data , unsigned char Dest ,unsigned char type,unsigned char  Datasize)
{
unsigned char * packet_withou_crc =(unsigned char *) malloc (4+Datasize);
memcpy (packet_withou_crc + 4 , Data , Datasize);

packet_withou_crc [0] = Dest ;
packet_withou_crc [1] = src ;
packet_withou_crc [2] = type ;
packet_withou_crc [3] = Datasize ;
Serial.print("Dest=");
Serial.println(Dest,HEX);
memcpy (packet_withou_crc+4,Data,Datasize);

unsigned short crc=crc16_calc(packet_withou_crc,4+Datasize) ;
unsigned char * ssppacket=(unsigned char *)malloc(Datasize+8);
memcpy ((ssppacket+1),packet_withou_crc,Datasize+4);
ssppacket[0]=0xc0;
ssppacket[Datasize+6]=(unsigned char)(crc&0xff);
ssppacket[Datasize+5]=(unsigned char)(crc>>8);
ssppacket[Datasize+7]=0xc0;
return ssppacket;
}


unsigned char Exetreaction (unsigned char * ReceivedData, unsigned char  ReceivedDataLen,unsigned char * Received_ssp_frame)
{
char i;
for(i=0;i<ReceivedDataLen;i++)
{
 if (ReceivedData[i]==0xc0)
    {
        int Len=ReceivedData[i+4];

        if(Len<=ReceivedDataLen-i-7)
        {
            if(ReceivedData[Len+5+2]==0xc0)
            {
                unsigned short crccheck=crc16_calc(ReceivedData+i+1,6+Len);
                if(crccheck==0)
                {
                    memcpy(Received_ssp_frame,(ReceivedData+1+i),(Len+4));
                    return (Len+4);
                }
                else
                {
                    return -1;
                }
            }
        }
    }
}
}

unsigned char ssp_CMD_Response(unsigned char *ssp_received_packet,  unsigned char *response)
{
extern bool start_scanFlag;
unsigned char t[128];
unsigned char result;
unsigned char ti;

for(ti=0;ti<128;ti++)
{
    t[ti]=58;
}
   

    unsigned char ping            =0x00;
    unsigned char get_online_TLM  =0x0f;
    unsigned char cap_image       =0x0c;
    unsigned char get_image       =0x08;
    unsigned char start_scan      =0x16;
    unsigned char stop_scan       =0x1c;


    unsigned char mysrc                 =ssp_received_packet[2];
    unsigned char dest_to               =ssp_received_packet[1];
    unsigned char type                  =ssp_received_packet[3];

    Serial.print("type=");
    Serial.println(type,HEX);
    if(type==ping)
    {
    unsigned char * a=malloc(8);
    a=SSP_P_Con(mysrc,0, dest_to, ACK, 0);
    memcpy(response,a,8);
    for(char i=0;i<8;i++)
    {
    Serial.print("response_ssp=");
    Serial.println(response[i],HEX);
    }
Serial1.write(a,8);
    result=8;
    }

    else if(type==get_online_TLM)
    {
  
   
     unsigned char  *b=malloc(7) ;
     
    memcpy(b,&TLM[0],7);
   unsigned char  * a=SSP_P_Con(mysrc ,b, dest_to, get_online_TLM, 7);
    char arr[15]={0xc0,0x0b,0x01,0x0f,0x07,0x00,0x87,0x00,0x46,46,0x08,0x16,0xd9,0x80,0xc0};
    Serial.println("tlm");
    for(int i=0;i<7;i++)
    Serial.println(TLM[i],HEX);
    Serial1.write(arr,15);
     Serial.println("tlm end");
    result=14;
    }
  
    else if(type==cap_image)
    {
    unsigned char * a=malloc(8);
    a=SSP_P_Con(mysrc ,0, dest_to, cap_image, 0);
     memcpy(response,a,8);
      Serial1.write(a,8);
     
    result=8;
    }

    else if(type==get_image)  
    {
       unsigned char * a=malloc(8);
    //response=SSP_P_Con(mysrc ,0, dest_to, get_image, 0);
    a=SSP_P_Con(mysrc,0, dest_to, get_image, 0);
    memcpy(response,a,8);
     Serial1.write(a,8);
    result=8;
    }

    else if(type==start_scan) 
    {
       unsigned char * a=malloc(8);
     a=SSP_P_Con(mysrc,0, dest_to, start_scan, 0);
    memcpy(response,a,8);
   // response=SSP_P_Con(mysrc ,0, dest_to, start_scan, 0);
    start_scanFlag=true;
     Serial1.write(a,8);
    Serial.println("rover start scan");
    result=8;
    }

    else if(type==stop_scan)
    {
       unsigned char * a=malloc(8);
     a=SSP_P_Con(mysrc,0, dest_to, stop_scan, 0);
    memcpy(response,a,8);
   // response=SSP_P_Con(mysrc ,0, dest_to, stop_scan, 0);
    start_scanFlag=false;
    Serial1.write(a,8);
    Serial.println("rover stop scan");
    result=8;
    }
    
    else
    {
    result=-1;
    }


      return result;



}
