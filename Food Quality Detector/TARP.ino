#include <ESP8266WiFi.h>
int GLED = A3; //Green LED to A3
int RLED = A4; //Red LED to A4
int gassensor =A1; //MQ2 Gas sensor to A1
int gassensorValue;
int buzzer=10;//Buzzer to pin 10
int tempval,humval,ldrsense;
int ldrsensor =A2; //LDR Sensor to A2
int tempsensor = A0; //DHT11 Temp part to A0
int humsensor = A5; //DHT11 Humidity part to A5
int ldrsensorval;
String foods[] = {"Tomato","Potato","Spinach","Onion","Cluster beans","Watermelon","Apple","Pear"};
int tempmin[] = {55,38, 32, 32, 45, 50,30,29};
int tempmax[] = {60,40,32,32,50,55,35,31};
int hummin[] = {85,85,90,70,85,85,90,90};
int hummax[] = {90,90,95,75,90,90,95,90};
int lightthresh[] = {1,2,0,2,0,2,2,0} ;// 1 is neutral , 2 is high and 0 is low
int chk;
String foodname; 
int i,j;

void setup() {
  // put your setup code here, to run once:
pinMode(buzzer,OUTPUT);
pinMode(A3,OUTPUT);
pinMode(A4,OUTPUT);
  Serial.begin(9600);
  setupESP8266();
}

void loop() {
  // put your main code here, to run repeatedly:
  //Blynk.run();
//timer.run();
  j=-1;
  Serial.println("Enter the food");
  while (Serial.available() == 0) {}
  foodname= Serial.read();
  for(i=0;i<8;i++)
  {
    if(foodname.toUpperCase()==foods[i].toUpperCase())
    {
       j=i;
    }
  }
  tempval =map(analogRead(A0),20,358,-40,125);
  humval =map(analogRead(A0),20,358,20,90);
  gassensorValue=analogRead(A1);
  ldrsensorval =analogRead(A2);
  if(ldrsensorval <= 333)
    ldrsense =  0;
  if(ldrsensorval > 333 && ldrsensorval<=700)
    ldrsense =  1;
  if(ldrsensorval > 700)
    ldrsense =  2;
  if(j==-1){
  Serial.println("Food not in database");
  }
  else
{
  if(gassensorValue<100 && tempval>=tempmin[j] && tempval<=tempmax[j] && ldrsense==lightthresh[j]&& humval>=hummin[j] && humval<=hummax[j] )
  {
    digitalWrite(buzzer,LOW); 
    Serial.println("Food conditions are satisfactory");  
  }
  else
  {
    digitalWrite(buzzer,HIGH);
    Serial.println("Food conditions are unsatisfactory"); 
}
}
Serial.println(" Gas value: ");
Serial.print(gassensorValue);
Serial.println(" Light_int: ");
Serial.print(ldrsensorval);
Serial.println(" Temp: ");
Serial.print(tempval);
Serial.println(" Humidity: ");
Serial.print(humval);
delay(1000);
}
