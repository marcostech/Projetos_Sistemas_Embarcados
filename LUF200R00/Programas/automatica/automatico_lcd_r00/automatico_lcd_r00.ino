/*
 *
 * Marcos Vinicius - GBNM²
 * 05/010/2021
 * R00
 * Sistema de leitura de Bateria DC
 * Acionando ou não saida de corrente
 * Controle via trimpot RV1=A6 / RV2 = A0
 * Tensão de Bateria A1
 * 
*/
//Bibliotecas
#include <avr\io.h> //necessario para o uso dos registradores
//#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include <SoftwareSerial.h>
// --

//Definição dos pinos e lcd
#define BAT_MAX A6 // Tensão de ALTA
#define BAT_MIN A0 // Tensão de BAIXA
#define BAT_IN A1 // Tensão de BATERIA
#define RL1 5 // Usado para controle do transformador
#define RL2 4 // Não usado
#define SAIDA_1 2 // LED em carga
#define SAIDA_2 3 // LED aviso de evento
#define RX A3
#define TX A4
//LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
SoftwareSerial mySerial(A3, A4);
// --

//Definição das funções de leitura AD
float leitura_rms(int ANALOG_PIN);
float volt_rms(int AN_SEL);
// --

//Definição das Constantes
const int MEDIA_RMS = 5;
const int PERIODO = 600; 
//--

// Variavel Global de erro
int erro = 0;  
// --

//Main
void setup() {
  
  mySerial.begin(9600);
  mySerial.println("Luffe, IND. BRASIL");
    
  pinMode(A3,INPUT); //especifico para PCB R00  
  pinMode(RL1,OUTPUT);
  pinMode(RL2,OUTPUT);    
  pinMode(SAIDA_1,OUTPUT);
  pinMode(SAIDA_2,OUTPUT);
  digitalWrite(RL1,LOW);  
  digitalWrite(RL2,LOW);
  digitalWrite(SAIDA_1,LOW);
  digitalWrite(SAIDA_2,LOW);
  delay(1);
}

void loop() {
  int bat_atual = 0;  
  int bat_max = 0;
  int bat_min = 0;
  int estado = 0; // estado 0 = em carga --- estado 1 = carregada --- estado 3 = falha geral
  int verificador =  5;
  delay(1);
  
//Leituras ADC
  bat_atual = analogRead(A1); //leitura de bateria  
  bat_max = analogRead(A6); //leitura de bateria - Vref alta
  bat_min = analogRead(A0); //leitura de bateria - Vref baixa
  
  mySerial.println(bat_atual);   
  mySerial.println(bat_max);
  mySerial.println(bat_min);
//---
pisca_led(SAIDA_2);
//----------Modo em carga
  while(estado == 0){
    if(bat_atual > bat_max){
      verificador--;      
      pisca_led(SAIDA_1);        
    };
    if(bat_atual < bat_max){
      verificador = 5;
    };
    if(verificador == 0){
      estado = 1;      
      digitalWrite(RL1,HIGH);      
      digitalWrite(SAIDA_1,LOW);
    }
//Leituras ADC
    bat_atual = analogRead(A1); //leitura de bateria  
    bat_max = analogRead(A6); //leitura de bateria - Vref alta
    bat_min = analogRead(A0); //leitura de bateria - Vref baixa
    
    mySerial.println(bat_atual);   
    mySerial.println(bat_max);
    mySerial.println(bat_min);
  }
//---

//reset verificador 
  verificador = 5;
  
//----------Modo carregada
  while(estado == 1){
    if(bat_atual < bat_min){
      verificador--;      
      pisca_led(SAIDA_1);        
    };
    if(bat_atual > bat_min){
      verificador = 5;
    };
    if(verificador == 0){
      estado = 0;
      digitalWrite(RL1,LOW);
    }
//Leituras ADC
    bat_atual = analogRead(A1); //leitura de bateria  
    bat_max = analogRead(A6); //leitura de bateria - Vref alta
    bat_min = analogRead(A0); //leitura de bateria - Vref baixa
    
    mySerial.println(bat_atual);   
    mySerial.println(bat_max);
    mySerial.println(bat_min);
  }  
//--

}

// -------------------
// Funções auxiliares 
// -------------------
int pisca_led(int pin){  // oscila 5 vezes porta D2
  digitalWrite(pin,HIGH);
  delay(300);
  digitalWrite(pin,LOW);
  delay(300);
  /*digitalWrite(pin,HIGH);
  delay(300);
  digitalWrite(pin,LOW);
  delay(300);
  digitalWrite(pin,HIGH);
  delay(300);
  digitalWrite(pin,LOW);
  delay(300);
  digitalWrite(pin,HIGH);
  delay(300);  
  digitalWrite(pin,LOW);
  delay(300);
  digitalWrite(pin,HIGH);
  delay(300);  
  digitalWrite(pin,LOW);
  */
}
