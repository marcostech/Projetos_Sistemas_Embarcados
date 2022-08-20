//Bibliotecas
#include <avr\io.h> //necessario para o uso dos registradores
//#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include <SoftwareSerial.h>
// --
const int led_A = 0;
const int led_B = 1;
const int led_C = 5;
const int led_D = 6;
const int led_E = 7;
const int led_F = 8;
const int led_G = 9;
const int led_DP = 10;
const int DSP1 = 2;
const int DSP2 = 3;
const int DSP3 = 4;

void setup() {
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  digitalWrite(2,LOW);  
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
};

void loop() {
  digitalWrite(2,LOW); 
  delay(1000);
  digitalWrite(2,HIGH);
  delay(1000);
  digitalWrite(3,LOW); 
  delay(1000);
  digitalWrite(3,HIGH); 
  delay(1000);
  digitalWrite(4,LOW); 
  delay(1000);
  digitalWrite(4,HIGH);
  delay(1000);
  digitalWrite(5,LOW); 
  delay(1000);
  digitalWrite(5,HIGH); 
  delay(1000);
};
