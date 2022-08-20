/*
 *
 * Marcos Vinicius - GBNM²
 * 28/06/2021
 * R00
 * Sistema de leitura de REDE AC
 * Controle de tensão para sistema Bivolt
 * Emprego de transformador de controle para medição da tensão 
 * Ligado em 220V / saída 12+12 
 * Usando retificação completa para chegar com 24V
 * Quando em 110V teremos 12vac na saida
 * Quando em 220V teremos 24vac na saida
 * 
*/
//Bibliotecas
#include <avr\io.h> //necessario para o uso dos registradores
//#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include <SoftwareSerial.h>
// --

//Definição dos pinos e lcd
#define BAT_IN A6 // Tensão de BAT
#define AC_IN A1 // Tensão de rede 
#define EXT_IN A0 // Tensão de externa 
#define RELE_110 4
#define RELE_220 5
#define BUZZER 2
#define Q2 3
#define RX A3
#define TX A4
//LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
SoftwareSerial mySerial(A3, A4);
// --

//Definição das funções de leitura AD
float leitura_rms(int ANALOG_PIN);
float volt_rms();
// --

//Definição das Constantes
const int MEDIA_RMS = 5;
const int PERIODO = 600; // periodo de leituras n*10uS = periodo
/*const float FATOR_CONV = 9.3; // r1 390k2 r2 45k3 // formula da constante Vin = Vout × (R1 + R2) ÷ R2 --6.8
const float FATOR_CONV_TRAFO_1 = 8.60; // multiplica relação do primario x secundario para pegar Vac da rede
const float FATOR_CONV_TRAFO_2 = 8.70; // adicionada queda de tensão com consumo dos relés
const float FATOR_RMS = 0.7;
*/
// --

// Variavel Global de erro
  int erro = 0;  
// --

//Main
void setup() {
  mySerial.begin(9600);
  mySerial.println("Luffe, IND. BRASIL");
  //lcd.init();
  //lcd.backlight(); 
  //lcd.clear();    
  pinMode(A3,INPUT); //especifico para PCB R00
  //zera reles
  pinMode(RELE_110,OUTPUT);
  pinMode(RELE_220,OUTPUT);  
  pinMode(BUZZER,OUTPUT);
  digitalWrite(RELE_110,LOW);  
  digitalWrite(RELE_220,LOW);
  digitalWrite(BUZZER,LOW);
}

void loop() {
  float volts = 0;  
  int rede = 0; //0 = erro / 110 = tensão de rede 110v / 220 = tensão de rede 220v
  int rede_atual = 0;
  int verificador = 3;
  delay(10);
  volts = volt_rms() ; 
  /*
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Luffe, Bivolt");  
  lcd.setCursor(0,1);
  lcd.print("Volt: ");
  lcd.setCursor(6,1);
  lcd.print(volts);  
  lcd.setCursor(13,1);
  lcd.print("E=");
  lcd.setCursor(15,1);
  lcd.print(erro);  
  */
  mySerial.println(volts); 
  mySerial.println(erro);
  digitalWrite(BUZZER,LOW);
  
  rede = rede_comp(volts);
  rede_atual = rede;
  
  while(verificador) { 
    volts = volt_rms();
    //lcd.setCursor(6,1);
    //lcd.print(volts);
    mySerial.println(rede);
    rede = rede_comp(volts);
    if (rede_atual == rede ) {
      verificador--;
      } else{
        verificador = 0;
        rede = 0;        
        }; 
  };
  
    
  while(rede) //??????
  switch (rede) {
    case 110:      
      digitalWrite(RELE_110,HIGH); //aciona rele 110
      /*lcd.clear();      
      lcd.setCursor(0,0);
      lcd.print("Luffe Ind.");
      lcd.setCursor(11,0);
      lcd.print(rede);
      lcd.setCursor(14,0);
      lcd.print("V");
      lcd.setCursor(0,1);
      lcd.print("Rede");
      lcd.setCursor(6,1);
      lcd.print((volts));
      lcd.setCursor(11,1);
      lcd.print("V");
      */
      mySerial.println("Sistema liberado para 110V");
      mySerial.println(volts);
      delay(1000);
            
      volts = volt_rms();
      rede = rede_comp(volts);
      if(rede_atual != rede) {
        rede = 0;        
        digitalWrite(RELE_110,LOW);    
        erro++;
        digitalWrite(BUZZER,HIGH);
       /* lcd.clear();        
        lcd.setCursor(0,0);
        lcd.print("Reiniciando...");
        lcd.setCursor(0,1);
        lcd.print("Rede instavel");
       */ 
       mySerial.println("Reiniciando - rede instavel");
       delay(500);
      };
    break;
    case 220:      
      digitalWrite(RELE_220,HIGH); //aciona rele 220
      /*lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Luffe Ind.");
      lcd.setCursor(11,0);
      lcd.print(rede);
      lcd.setCursor(14,0);
      lcd.print("V");
      lcd.setCursor(0,1);
      lcd.print("Rede:");
      lcd.setCursor(6,1);
      lcd.print(volts);
      lcd.setCursor(11,1);
      lcd.print("V");            
      */
      mySerial.println("Sistema liberado para 220V"); 
      mySerial.println(volts);
      delay(1000);
             
      volts = volt_rms();
      rede = rede_comp(volts);
      if(rede_atual != rede) {
        rede = 0;        
        digitalWrite(RELE_220,LOW);
        erro++;
        digitalWrite(BUZZER,HIGH);
        /*lcd.clear();        
        lcd.setCursor(0,0);
        lcd.print("Reiniciando...");
        lcd.setCursor(0,1);
        lcd.print("Rede instavel");
       */
       mySerial.println("Reiniciando - rede instavel");
       delay(500);
      };
    break;
    default:
      rede = 0;      
      digitalWrite(RELE_110,LOW);  
      digitalWrite(RELE_220,LOW);
      erro++;      
      digitalWrite(BUZZER, HIGH);
      mySerial.println("Reiniciando - rede não identificada");
    break;
    };
}
// -------------------
// Funções auxiliares 
// -------------------
int rede_comp(int volts) { //Janela correta 100-240vac +- 10%
  int estado_rede = 0;
  //Função de comparação da leitura de volts rms
  if(volts >= 80 && volts <= 140 ) { //Janela de 100v - 145v para 110 -- Leitura de tensão com erro de - 20V em 100V / -6 140V (somar 20 para valor real)
    estado_rede = 110;
    erro = 0;
  };
  if(volts >= 187 && volts <= 248) { //Janela de 190v - 245v para 220 -- Leitura de tensão com erro de - 3V em 220V (somar 3 para valor real)
    estado_rede = 220;
    erro = 0;
  };
  if(volts < 79 || volts > 249) { //Janela de erro abaixo e acima
    estado_rede = 0;    
    digitalWrite(BUZZER,HIGH);
    mySerial.println("Fora da faixa de tensão aceitavel de trabalho - Falha de Rede");
    mySerial.println(volts);
    delay(500);
  };
  if(volts > 141 && volts < 186) { //Janela de erro entre tensões
    estado_rede = 0;    
    digitalWrite(BUZZER,HIGH);
    mySerial.println("Flutuante entre tensões de trabalho - Falha de Rede");
    mySerial.println(volts);
    delay(500);
  };
return estado_rede;
  }

float volt_rms() { 
  //Função de leitura com media
  float media[MEDIA_RMS] = {0};
  float soma_media = 0;
  float volt_rms = 0;
  // Rotina de leitura;
  for(int i = 1; i < MEDIA_RMS ; i++) { 
    media[i] = leitura_rms(AC_IN); //função de leitura
  };
  for(int j = 1; j < MEDIA_RMS; j++) {
    soma_media += media[j]; //função da media
  };
  volt_rms = soma_media / MEDIA_RMS;
  return volt_rms;
}

float leitura_rms(int AN_PIN) { 
  /*    
  Função para onda não pura, somente onda completa
  onda pura = Vp x .707 = Vrms -/- Vrms = 1.11 x Vavg -/- 1.414 x Vrms= Vp -/- Vavg= .637 x Vp
  onda não pura = rms = sqrt(v1²+v2²+v3²+v4²+v5²+v6²+v7²+v8²+v9²+v10²/10)
  OBS: problemas de medição rms com onda não pura devido a imprecisão do inicio da leitura do semi-ciclo,
  não sendo possivel restringir a leitura a apenas um.
  */
  float leitura = 0;  
  float leitura_buffer = 0;
  for (int k = 0; k < PERIODO; k++) {
      leitura = analogRead(AN_PIN); //740    
      leitura = leitura * 305/1023; //Alterar para 5 original
     // leitura = leitura * FATOR_CONV; //Conversão para reduzir tamanho da variavel
      leitura *= leitura; //ao quadrado
      leitura_buffer += leitura; //salva no buffer para calcular depois
      delayMicroseconds(10);
  } 
  leitura_buffer = leitura_buffer / PERIODO; //divisão da equação  rms 
  leitura_buffer = sqrt(leitura_buffer);  //raiz quadrada da equação rms
  return leitura_buffer; //retorna media calculada
}
