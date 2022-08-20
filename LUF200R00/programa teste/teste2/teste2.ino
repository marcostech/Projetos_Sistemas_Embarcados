//Bibliotecas
//apoio da biblioteca stivseg!!
#include <avr\io.h> //necessario para o uso dos registradores
#include <Wire.h> 
#include <SoftwareSerial.h>
//#include <TimerOne.h>
// --
#define led_A 0
#define led_B 1
#define led_C 5
#define led_D 6
#define led_E 7
#define led_F 8
#define led_G 9
#define led_DP 10
#define DSP1 2
#define DSP2 3
#define DSP3 4
//

/*
void display(int adc_valor){   
    int long dig1 = (adc_valor/1000)%10; //preserva digito 1 do valor AD
    dsp1 = 1;
    escreve_display(dig1,0);
    __delay_ms(3);
    dsp1 = 0;
    int long dig2 = (adc_valor/100)%10;//preserva digito 2 do valor AD
    dsp2 = 1;
    escreve_display(dig2,1);
    __delay_ms(3);
    dsp2 = 0;
    int long dig3 = (adc_valor/10)%10; //preserva digito 3 do valor AD
    dsp3 = 1;
    escreve_display(dig3,0);
    __delay_ms(3);
    dsp3 = 0;
    int long dig4 = adc_valor%10; //preserva digito 4 do valor AD
    dsp4 = 1;
    escreve_display(dig4,0);
    __delay_ms(3);
    dsp4 = 0;
} //chama display para exibição 
*/
//Variavel de Offset do sensor de corrente
long leitura_offset = 0;
bool leitura_offset_inicial = true;
//Definição dos pinos e displays
int pinos_segmentos[8] {led_A, led_B, led_C, led_D, led_E, led_F, led_G, led_DP};
int pinos_displays[3] {DSP1, DSP2, DSP3};
int total_displays = 3;
int buffer_leitura[3];
bool escala_alta = false;

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
  if((tempo_atual - tempo) > 1000) {
    atualiza_display();
    tempo = millis();
  };
  
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
  //Leitura do Offset do sensor
  while(leitura_offset_inicial){
    delay(200);//estabilização
    for(int i = 0; i < 64; i++) {
      leitura_offset += analogRead(A0);
      };
    leitura_offset = leitura_offset / 64;
    delay(1);
    leitura_offset_inicial = false;
  }
};

//função de atualização do display
void atualiza_display() {
  int media_leituras = 0;
  long leitura_adc = 0;
  //desabilita perifericos redução ruido
  
  //media das leituras 10x

  for(int i = 0; i < 8; i++) { 
    media_leituras += analogRead(A0);
  };
  leitura_adc = media_leituras / 8;
   
//leitura do sensor hall HGX 150A
  //leitura_adc = leitura_adc - leitura_offset; //definição do offset  
  if(leitura_adc < 0 ) {leitura_adc = 0;}; //medição negativa = 0 
  if(leitura_adc > 1 ) {leitura_adc += 2;}; //remoção do erro offset
  leitura_adc *= 100; //multiplica para preservar Ponto decimal
  leitura_adc = leitura_adc * 0.00488; //escala 5v1
  ////leitura_adc = leitura_adc * 0.00105; //escala 1v1
  leitura_adc = leitura_adc / 0.0227; //escala do sensor mv/A - Tipo A
  ////leitura_adc = leitura_adc / 0.00295; //escala do sensor mv/A - Tipo B  
  ////leitura_adc = leitura_adc / 0.00145; //escala do sensor mv/A - Tipo C 
  //Fator de correção - %
  //leitura_adc = leitura_adc * 0.94; //6% SOMENTE PARA HALL HGX
  leitura_adc /= 10;
/* 
   * //função de conversão da leitura - voltimetro
   * leitura_adc = leitura_adc * 10; //multiplica para preservar ponto decimal mais adiante
   * leitura_adc = leitura_adc * 0.08; // 85/1023
*/
  //troca do fundo de escala em 99.9V para 999V
  if (leitura_adc >= 999) { 
    leitura_adc /= 10;
    escala_alta = true;
  } else {
    escala_alta = false;
        };  
  //laço de atualização do buffer global do displays     
  for(int i = 0; i < 3; i++) { 
    buffer_leitura[i] = leitura_adc % 10; //preserva somente o digito
    leitura_adc /= 10;
  };
};
