#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID   "TMPLZoaq0wr5"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define DHTPIN D7    
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);

#define relay D3
#define relay2 D4
char auth[] = "I04e1qpec7RHZJvOi0HAhF6bYdzNYCMs";

char ssid[] = "Aekies2.4";
char pass[] = "0866102216";

byte auto_SW =0;

float Temp_auto=0,Humidity_auto=0 ;

BlynkTimer timer;
WidgetLED led1(V20);
WidgetLED led2(V21);



BLYNK_WRITE(V2)
{
if(param.asInt()==1&&auto_SW==0){
  digitalWrite(relay,!digitalRead(relay));
  if(digitalRead(relay) == LOW){
    led1.on();
  } 
  }
else if(param.asInt()==0&&auto_SW==0){
  digitalWrite(relay,HIGH);
  led1.off();
  }
}

BLYNK_WRITE(V3)
{
if(param.asInt()==1&&auto_SW==0){
  digitalWrite(relay2,LOW);
  if(digitalRead(relay2)==LOW){
    led2.on();
    }
}
else if(param.asInt()==0&&auto_SW==0){
  digitalWrite(relay2,HIGH);
  led2.off();
  }
}

BLYNK_WRITE(V4)
{
Temp_auto = param.asFloat();  
}

BLYNK_WRITE(V5)
{
Humidity_auto = param.asFloat();  
  
}

BLYNK_WRITE(V9)
{
  auto_SW = param.asInt();
  digitalWrite(relay, HIGH);
  digitalWrite(relay2,HIGH);
  led1.off();
  led2.off();
}

void AUTO() {
//lcd.clear();
  float h = dht.readHumidity();             //รับค่าความชื้น
  float t = dht.readTemperature();          //รับค่าอุณหภูมิ
  lcd.setCursor(0, 0); 
  lcd.print("Temp = " + String(t) + " C"); //แสดงค่าอุณหภูมิ
  lcd.setCursor(0, 1);
  lcd.print("Humi = " + String(h) + " %"); //แสดงค่าความซื้น*/
  Blynk.virtualWrite(V1, String(t,1));    
  Blynk.virtualWrite(V0, String(h,1));    
  if (auto_SW == 1) {
    if(t < Temp_auto){
      delay(3000);
      digitalWrite(relay, LOW);
      digitalWrite(relay2,LOW); 
      led1.on();//โหมด auto หากอุณหภูมิน้อยกว่าค่าที่ตั้งไว้ relay ch1 จะไม่ทำงาน 
      led2.on();
    }
  else  {
     delay(3000);
     digitalWrite(relay, LOW); 
     digitalWrite(relay2,HIGH);
     led1.on();//โหมด auto หากอุณหภูมิมากกว่าค่าที่ตั้งไว้ relay ch1 จะทำงาน
     led2.off(); 
     
    }
  
  }
  
 
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  /*
  
  if(auto_SW == 1&&h > Humidity_auto){
    digitalWrite(relay2 ,LOW);
    led2.on(); 
    
   }
  else if(auto_SW == 1&&h < Humidity_auto){
    digitalWrite(relay2,HIGH);
    led2.off(); 
    delay(3000); 
   
  }*/
}





void setup() {
  Serial.begin(9600);
  dht.begin();
  Blynk.begin(auth,ssid,pass);
  lcd.begin();
  lcd.backlight();
  pinMode(relay,OUTPUT);
  pinMode(relay2,OUTPUT);
  digitalWrite(relay, HIGH);
  digitalWrite(relay2, HIGH);
  led1.off();
  led2.off();
}

void loop() {
  // Wait a few seconds between measurements.
 AUTO();
delay(200);
 Blynk.run();
 //timer.run();                     

/*float t = dht.readTemperature();
float h = dht.readHumidity();

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.println(t);
  
  

 Blynk.virtualWrite(V1, t);
 Blynk.virtualWrite(V0, h);*/                  
}
