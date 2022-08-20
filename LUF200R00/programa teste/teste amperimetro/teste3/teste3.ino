//Codigo para teste
//de interrupções e
//uso da função sleep adc
//15/12/21


//Bibliotecas
//apoio da biblioteca stivseg!!
#include <avr\io.h> //necessario para o uso dos registradores
#include <Wire.h> 
#include <SoftwareSerial.h>

#define led_A 0
#define led_B 1
#define led_C 5
#define led_D 6
#define led_E 7
#define led_F 8
#define led_G 9
#define led_DP 10
#define DSP1 A3
#define DSP2 3
#define DSP3 4
//
//---------------------------------------------------------

//Definição dos pinos e displays
const byte adcPIN = A0;
int pinos_segmentos[8] {led_A, led_B, led_C, led_D, led_E, led_F, led_G, led_DP};
int pinos_displays[3] {DSP1, DSP2, DSP3};
int total_displays = 3;
int buffer_leitura[3];
bool escala_alta = false;

///

volatile long int adc_resultado = 0;
volatile boolean adc_done = false;
volatile int adc_leituras = 0;
volatile long int adc_processado = 0;
volatile long int adc_temp = 123;
boolean adc_started = false;
///

//variaveis de controle do tempo
int tempo = 0;
int tempo_atual = 0;

//Tabela de números
bool numeros[10][7]{
  // A    B    C    D    E    F    G 
  {true,true,true,true,true,true,false},     //0
  {false,true,true,false,false,false,false}, //1
  {true,true,false,true,true,false,true},    //2
  {true,true,true,true,false,false,true},    //3
  {false,true,true,false,false,true,true},   //4
  {true,false,true,true,false,true,true},    //5
  {true,false,true,true,true,true,true},     //6
  {true,true,true,false,false,false,false},  //7
  {true,true,true,true,true,true,true},      //8
  {true,true,true,true,false,true,true}      //9
  };


void setup() {
  //seletor de referencia Vref do ADC
  //analogReference(INTERNAL1V1); //INTERNAL1V1 - 1v1 Vref = 0,001mV/adc step

  //ADC CFG
  ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2)); // clear prescaler bits

  //ADCSRA |= bit (ADPS0);                               //   2  
  //ADCSRA |= bit (ADPS1);                               //   4  
  //ADCSRA |= bit (ADPS0) | bit (ADPS1);                 //   8  
  //DCSRA |= bit (ADPS2);                               //  16 
  //DCSRA |= bit (ADPS0) | bit (ADPS2);                 //  32 
  ADCSRA |= bit (ADPS1) | bit (ADPS2);                 //  64 
  //ADCSRA |= bit (ADPS0) | bit (ADPS1) | bit (ADPS2);   // 128

  
  ADCSRA |= bit(ADEN);  //habilita ADC
  ADMUX = bit(REFS0) | (A0 & 0x07); //Canal A0 - Vref = VCC

  //pinMode(2,INPUT_PULLUP);
  
  //iniciador do display, varredura dos pinos de controle comum
  for(int i = 0;i < total_displays; i++) {
    pinMode(pinos_displays[i],OUTPUT);
  };

  //varredura dos pinos de segmentos
  for(int i = 0;i <= 7; i++) {
    pinMode(pinos_segmentos[i],OUTPUT);
  };
  
  //"inicia" contador
  tempo = millis();
  delay(500);
};

void loop() {  
  //temporizador de atualização do display
  tempo_atual = millis();
  if((tempo_atual - tempo) > 500) {
    //ADCSRA |= bit (ADSC) | bit (ADIE); //Começa conversão e habilita interrupt    
    //atualiza_display();    
    tempo = millis();
  };
  
  //----------------
  
    
    if(!adc_started) {
    ADCSRA |= bit (ADSC) | bit (ADIE);
    adc_started = true;
    };
    if(adc_done){
      adc_started = false;
      atualiza_display();
      }
    
  //---------------  

    
  //varredura dos displays
  for (int i = 0; i < total_displays; i++) {
    
    //varredura dos segmentos e acionamento com base na tabela de numeros
    for (int j = 0; j <=6; j++) {
      digitalWrite(pinos_segmentos[j],numeros[buffer_leitura[i]][j]);//define os pinos de acordo com a tabela
      };
      
    //controle do ponto decimal, se em escala alta desativa
    if(i == 1 && !escala_alta) {
      digitalWrite(pinos_segmentos[7],HIGH);
    } else {
      digitalWrite(pinos_segmentos[7],LOW);
      }
      
    digitalWrite(pinos_displays[i],HIGH);//liga o display    
    delay(5);//delay para multiplexar - testado e ok
    digitalWrite(pinos_displays[i],LOW); //desliga display
  };  
};

//função de atualização do display
void atualiza_display() {  
  long int adc_buffer = adc_temp ;
  //adc_buffer *= 100;
  //adc_buffer = adc_buffer * 0.0048;
  //laço de atualização do buffer global do displays ()   
  for(int i = 0; i < 3; i++) {
    buffer_leitura[i] = adc_buffer % 10; //preserva somente o digito
    adc_buffer /= 10;
  };
};

ISR (ADC_vect)
  {
  int adcl = ADCL;
  int adch = ADCH * 256;
  adc_temp = adcl + adch;
  adc_done = true;
  };
