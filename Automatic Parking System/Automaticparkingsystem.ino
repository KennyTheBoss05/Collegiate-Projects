#include "SPI.h"
#include "MFRC522.h"
#include <Servo.h>
//#include <ESP8266WiFi.h>
//#include
#define DEBUG true
#define RX 3
#define TX 4

String HOST = "api.thingspeak.com";
String PORT = "80";
String AP = "BERACAH";
String PASS = "Prisuj2818";


String API = "K5M7K5O873NY1X8R";
String field = "field1";

int countTrueCommand;
int countTimeCommand;
boolean found = false;
int valSensor = 1;

SoftwareSerial esp8266(RX,TX);

#define SS_PIN 10
#define RST_PIN 9
#define trigger A0
#define echo A1

float tim=0,distance=0;
int count = 10;
Servo myservo;
// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7) 
int green_led = A3;
int red_led = A2;
char incomingByte = 0;
int slot=0;
boolean boo = false;
int k=-1;

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(8, OUTPUT);
  myservo.attach(8);
  myservo.write(0);
  Serial.println("waiting for card...");
  Serial.print("SMART PARKING");
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(5,OUTPUT);
  esp8266.begin(115200);

sendCommand("AT",5,"OK");
sendCommand("AT+CWMODE=1",5,"OK");
sendCommand("AT+CWJAP=""+ AP +"",""+ PASS +""",15,"OK");
countTrueCommand = 0;
}
void loop() {
  // put your main code here, to run repeatedly:
  String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(valSensor);
switch(countTrueCommand) {

case 0: sendCommand("AT",5,"OK");break;
case 1: sendCommand("AT+RST",10,"invalid");break;
case 2: sendCommand("AT+CIPMUX=1",5,"OK"); break;
case 3: sendCommand("AT+CIPSTART=0,"TCP","+ HOST +","+ PORT,15,"OK"); break;
case 4: sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">"); break;
case 5: esp8266.println(getData);delay(1500);countTrueCommand++; break;
case 6: sendCommand("AT+CIPCLOSE=0",5,"OK"); break;
case 7:

Serial.println(valSensor);
Serial.print(getData);
Serial.print(",");
Serial.println(getData.length());
valSensor = random(100000); // random value, change with sensor value if using sensor
countTrueCommand = 0;
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }
  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i != 3 ? ":" : "");
  }
  strID.toUpperCase();
  Serial.print("Tap card key: ");
  Serial.println(strID);
  delay(500);
 
  /*digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  tim=pulseIn(echo,HIGH);
  distance=tim*340/20000;
  Serial.println(distance);*/
  if(true)//distance<6
 { digitalWrite(green_led,HIGH);
  digitalWrite(red_led,LOW);
  if (strID.indexOf("E1:7B:C4:02") >= 0&&count>0) {  
//put your own tap card key;   put your own rfid code of the tag in "D3:08:1B:06"
  
    Serial.println("Authorised access");
  analogWrite(red_led,0);
  analogWrite(green_led,1023);
  count=count-1;
    //digitalWrite(8, LOW);
    myservo.write(90); //motor moves 90 degree
    delay(2000);
    myservo.write(0);
    Serial.print("Slots remaining = ");
    Serial.println(count);
    if (count==0)
    {
      Serial.println("OUT OF SLOTS");
      //return;
    }
    Serial.println("Enter slot");
    slot =0;
    //Serial. flush();
while(slot==0)
{
if(Serial.available()>0)
  {
     incomingByte = Serial.read();
     slot = incomingByte - '0';
    //Serial.println(choice);
    if(slot<0)
    slot = 0;
    switch(slot)
   { 

   }
   k=1;

  }
}
  if(slot==1)
  {
    Serial.println("\nYou may enter \n\nParking assistant is now online...");
    boo=true;
    distance = 50;
    while(boo)
    {
        digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  tim=pulseIn(echo,HIGH);
  distance=tim*340/20000;
  Serial.println(distance);
  if(distance>50)
  {
  distance = 50;
  }
      digitalWrite(5,HIGH);
      delay(distance*30);
      digitalWrite(5,LOW);
      delay(distance*10);
      //digitalWrite(5,HIGH);
      if(distance<4)
      {
      boo = false;
      Serial.println("You have parked your car");
      }
    }
    digitalWrite(5,LOW);
  }
  else
  {
    Serial.println("Do not enter");
    count = count+1;
   analogWrite(red_led,1023);
  analogWrite(green_led,0);
  }
  slot =0;
  }
   else 
  {
  analogWrite(red_led,1023);
  analogWrite(green_led,0);
    Serial.println("Access denied");
    myservo.write(0);
    //digitalWrite(8, HIGH);
    //delay(2000);
    //digitalWrite(8, LOW);
    Serial.print("Slots remaining = ");
    Serial.println(count);

  }
 }
 delay(3000);

}

void sendCommand(String command, int maxTime, char readReplay[]) {

Serial.print(countTrueCommand);
Serial.print(". at command => ");
Serial.print(command);
Serial.print(" ");
while(countTimeCommand < (maxTime*1))

{

esp8266.println(command);//at+cipsend
if(esp8266.find(readReplay))//ok
{

found = true;
break;

}
countTimeCommand++;

}
if(found == true)
{

Serial.println("OYI");

countTrueCommand++;
countTimeCommand = 0;

}
if(found == false)
{

Serial.println("Fail");
countTrueCommand = 0;
countTimeCommand = 0;

}
found = false;

}
