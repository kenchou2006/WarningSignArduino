#include "WiFiS3.h"
#include "WiFiSSLClient.h"
#include <LiquidCrystal_PCF8574.h>
#include "Arduino_LED_Matrix.h" // R4
#include <Servo.h>//伺服馬達
Servo myservo;

const char* ssid[11]={""};
const char* password[11]={""};
const char* server = "warningsign.pp.ua";
const int httpsPort = 443;
WiFiSSLClient client;
int WWW=8;
void setup() {
  Serial.begin(9600);
  pinMode(WWW, OUTPUT);
  myservo.attach(9);//伺服馬達腳位
  myservo.write(91);
  pinMode(A1, INPUT);//電池
  pinMode(A2, INPUT);//太陽能
  // put your setup code here, to run once:
  delay(10);
  wifiConnection();// 連接 Wi-Fi
}

void loop() {
  int open,off,Charge,power,SavePower,d;
  Charge=2;
  
  Serial.print("A1:");
  Serial.println((analogRead(A1) * 0.009775));
  power=((analogRead(A1)*0.009775)-2.7)/2.4*100;
  if(power>=100){power=100;}else if(power<=0){power=0;}
  if(power<=30){SavePower=1;}else{SavePower=2;}
  Serial.print("A2:");
  Serial.println((analogRead(A2) * 0.009775));
  if((analogRead(A2) * 0.009775)>1){Charge=1;}else{Charge=2;}
  server_transfer(power,2,Charge,SavePower);
  open=0;
  d=0;
  do {
    Serial.println(d);
    open=performRequest(0);
    digitalWrite(WWW, HIGH); 
    delay(500);
    digitalWrite(WWW, LOW);
    delay(500);
    if(open != 1){
    if(d<=10){
      if(SavePower==2){delay(0);}else{delay(7000);}
      d++;
    }else{
      Serial.print("A1:");
      Serial.println((analogRead(A1) * 0.009775));
      power=((analogRead(A1)*0.009775)-2.7)/2.4*100;
      if(power>=100){power=100;}else if(power<=0){power=0;}
      if(power<=30){SavePower=1;}else{SavePower=2;}
      Serial.print("A2:");
      Serial.println((analogRead(A2) * 0.009775));
      if((analogRead(A2) * 0.009775)>1){Charge=1;}else{Charge=2;}
      server_transfer(power,2,Charge,SavePower);
      d=0;
    }
    }//else{delay(2000);}
  }while(open!=1);
  myservo.write(-2);
  Serial.print("A1:");
  Serial.println((analogRead(A1) * 0.009775));
  power=((analogRead(A1)*0.009775)-2.7)/2.4*100;
  if(power>=100){power=100;}else if(power<=0){power=0;}
  if(power<=30){SavePower=1;}else{SavePower=2;}
  Serial.print("A2:");
  Serial.println((analogRead(A2) * 0.009775));
  if((analogRead(A2) * 0.009775)>1){Charge=1;}else{Charge=2;}
  server_transfer(power,1,Charge,SavePower);
  
  off=0;
  d=0;
  do {
    Serial.println(d);
    off=performRequest(1);
    digitalWrite(WWW, HIGH); 
    delay(500);
    digitalWrite(WWW, LOW);
    delay(500);
    if(off!=1){
    if(d<=10){
      if(SavePower==2){delay(0);}else{delay(7000);}
      d++;
    }else{
      Serial.print("A1:");
      Serial.println((analogRead(A1) * 0.009775));
      power=((analogRead(A1)*0.009775)-2.7)/2.4*100;
      if(power>=100){power=100;}else if(power<=0){power=0;}
      if(power<=30){SavePower=1;}else{SavePower=2;}
      Serial.print("A2:");
      Serial.println((analogRead(A2) * 0.009775));
      if((analogRead(A2) * 0.009775)>1){Charge=1;}else{Charge=2;}
      server_transfer(power,1,Charge,SavePower);
      d=0;
    }
    }//else{delay(2000);}
  }while(off!=1);
  myservo.write(91);
  delay(1000);
  client.stop(); // 關閉連線
  }
