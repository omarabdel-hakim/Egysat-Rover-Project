#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Arduino.h>
//********************************************* IR
#define IRSensor 53 
#define whiteLed 13
//********************************************* GAS
#define smokeA0 A0
#define sensorThres  160
//********************************************* UltraSonic
#define echo1 22
#define trig1 23
#define echo2 24
#define trig2 25
unsigned long duration1,duration2;
unsigned long distance1,distance2;
//********************************************* DHT
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 26
//********************************************* MOTORS
#define motor1pin1 2
#define motor1pin2  3
#define motor2pin1  4
#define motor2pin2  5
#define ENA 8
#define ENB 9
#define controll_speed 255

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

unsigned char TLM[7];
unsigned char u1,u2;

//********************************************* IR
void IR_init(void)
{
  pinMode (IRSensor, INPUT); // sensor pin INPUT
  pinMode (whiteLed, OUTPUT); // Led pin OUTPUT
}
void IR_start(void) 
{
  //char statusSensor = digitalRead(IRSensor);
  TLM[0]=digitalRead(IRSensor);
  digitalWrite(whiteLed,HIGH);
  if (TLM[0] == 1)
  {
  digitalWrite(whiteLed,LOW);
  }
  else
  {
  digitalWrite(whiteLed, HIGH);
  }
}

//********************************************* GAS
void GAS_init(void)
{ 
  pinMode(smokeA0, INPUT);
}

void GAS_start(void)
{ 
  short analogSensor = analogRead(smokeA0);
  TLM[1]=analogSensor>>8;
  TLM[2]=(analogSensor<<8)>>8;
}
//********************************************* UltraSonic 1
void UlTRASONIC1_init(void)
{
  pinMode(echo1,INPUT);
  pinMode(trig1,OUTPUT);
}


unsigned char UlTRASONIC1_start()
{
  digitalWrite(trig1,LOW);
  delayMicroseconds(5);
  digitalWrite(trig1,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1,LOW);
  duration1=pulseIn(echo1,HIGH);
  distance1=(unsigned char)(duration1*0.034) / 2;
  TLM[3]=distance1;
  TLM[4]=distance1;
  return TLM[3];
}
//******************************************* UltraSonic 2
void UlTRASONIC2_init(void)
{
  pinMode(echo2,INPUT);
  pinMode(trig2,OUTPUT);
}


unsigned char UlTRASONIC2_start()
{
  digitalWrite(trig2,LOW);
  delayMicroseconds(5);
  digitalWrite(trig2,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2,LOW);
  duration2=pulseIn(echo2,HIGH);
  distance2=(unsigned char)(duration2*0.034) / 2;
  TLM[4]=distance2;
  return TLM[4];
}
//******************************************* DHT
void DHT_start(void)
{
  DHT.read(DHT11_PIN);
  TLM[5]=DHT.temperature;
  TLM[6]=DHT.humidity;
  delay(100);
}
//******************************************* Motors
void MOTORS_init(void)
{
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(ENA, OUTPUT); 
  pinMode(ENB, OUTPUT);
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
}
void MOTORS_forward(void)
{
  analogWrite(ENA,controll_speed);
  analogWrite(ENB,controll_speed);
  digitalWrite(motor1pin1,HIGH);
  digitalWrite(motor1pin2,LOW);
  digitalWrite(motor2pin1,HIGH);
  digitalWrite(motor2pin2,LOW); 
}
void MOTORS_backward(void)
{
  analogWrite(ENA,controll_speed);
  analogWrite(ENB,controll_speed);
  digitalWrite(motor1pin1,LOW);
  digitalWrite(motor1pin2,HIGH);
  digitalWrite(motor2pin1,LOW);
  digitalWrite(motor2pin2,HIGH); 
}
void MOTORS_rotate_right(void)
{
  analogWrite(ENA,controll_speed);
  analogWrite(ENB,controll_speed);
  digitalWrite(motor2pin1,LOW);
  digitalWrite(motor2pin2,HIGH);
}
void MOTORS_rotate_left(void)
{
  analogWrite(ENA,controll_speed);
  analogWrite(ENB,controll_speed);
  digitalWrite(motor1pin1,LOW);
  digitalWrite(motor1pin2,HIGH); 
}
void MOTORS_stop(void)
{
  digitalWrite(motor1pin1,LOW);
  digitalWrite(motor1pin2,LOW); 
  digitalWrite(motor2pin1,LOW);
  digitalWrite(motor2pin2,LOW);
  analogWrite(ENA,LOW);
  analogWrite(ENB,LOW);
}
//********************************************************Collect TEM
void Collect_TEM( )
{
  IR_start();
  GAS_start();
  UlTRASONIC1_start();
 
  DHT_start();
  
  Serial.print("GAS=");
  Serial.println((((short)TLM[1])<<8)+(((short)TLM[2])));
  
  Serial.print("Ultra 1=");
  Serial.println(TLM[3]);
  
  Serial.print("Ultra 2=");
  Serial.println(TLM[4]);
  
  Serial.print("Temperature=");
  Serial.println(TLM[5]);
  
  Serial.print("Humidity=");
  Serial.println(TLM[6]);

  
 
}
//************************************************************Rover Inti
void Rover_init(void)
{
  IR_init();
  GAS_init();
  UlTRASONIC1_init();
  UlTRASONIC2_init();
  MOTORS_init();
}
//**************************************************** Start Scan
void Start_scan(void)
{
  if (TLM[3]>80)
    {
      MOTORS_forward();
    }
if ( TLM[3]<80)
    {
      MOTORS_rotate_right();
     
    }
}
//**************************************************************
