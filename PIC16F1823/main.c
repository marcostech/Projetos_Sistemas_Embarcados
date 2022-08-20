/*
 * ESPECIAL KAME SEM RETARDO DE 5SEG
 * BLOQUEIO DIRETO 
 * File:   bloq_v2.2_eep_block_LUF.c
 * Author: Marcos Vinicius
 *
 * Created on 08 de Março de 2021, 10:00
 * Para Natalinda
 * 
 */

#include <xc.h>
#include <pic16f1823.h>
#define _XTAL_FREQ 1000000
// portas
#define led3_neg LATCbits.LATC5 //led eeprom status - neg
#define led3a LATCbits.LATC4 //led eeprom status - pos
#define ctrl LATAbits.LATA5 //rele
#define led2 LATAbits.LATA4 //led carregando
#define botao LATCbits.LATC2
//------
// PIC16F1825 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)
// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = HI        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)
//------ END CFG BITS
void pisca_led(){
    led2 = 0;
    __delay_ms(50);
    led2 = 1;
    __delay_ms(50);
    led2 = 0;
}
void pisca_led_eeprom(){
    led3_neg = 0;
    led3a = 1;
    __delay_ms(50);
    led3_neg = 1;
    led3a = 0;
    __delay_ms(50);
    led3_neg = 0;
    led3a = 1;
    __delay_ms(50);
    led3_neg = 1;
    led3a = 0;
    __delay_ms(50);
}
/*
int ler_adc_vref(){
    int adc_Reading;
    unsigned int adc_Average;
    unsigned int adc_Return;
    unsigned int adcValues [16];
    unsigned char adc_index = 0;
    CHS0 = 0; //AN2
    CHS1 = 1;
    CHS2 = 0;
    CHS3 = 0;
    CHS4 = 0;
    ADON = 1;
    while(adc_index<16){
    __delay_ms(1);
    GO_nDONE = 1;
    while (GO_nDONE==1);
    adc_Reading = (ADRESH<<8) + ADRESL ;
    adcValues[adc_index] = adc_Reading;
    adc_index++;
    };    
    adc_Average = 0;
    for (int i=0; i < 16; i++)
    {
        adc_Average += adcValues[i];
    }    
        adc_index= 0;
        adc_Return = adc_Average/16;
        return adc_Return;
}
int ler_adc_vref2(){
    int adc_Reading;
    unsigned int adc_Average;
    unsigned int adc_Return;
    unsigned int adcValues [16];
    unsigned char adc_index = 0;
    CHS0 = 1; //AN5
    CHS1 = 0;
    CHS2 = 1;
    CHS3 = 0;
    CHS4 = 0;
    ADON = 1;
    while(adc_index<16){
    __delay_ms(1);
    GO_nDONE = 1;
    while (GO_nDONE==1);
    adc_Reading = (ADRESH<<8) + ADRESL ;
    adcValues[adc_index] = adc_Reading;
    adc_index++;
    };    
    adc_Average = 0;
    for (int i=0; i < 16; i++)
    {
        adc_Average += adcValues[i];
    }    
        adc_index= 0;
        adc_Return = adc_Average/16;
        return adc_Return;
}
*/ //Tensões de referencia com trimpot da auto v0
int tbat_media(){
    int adc_Reading;
    unsigned int adc_Average;
    unsigned int adc_Return;
    unsigned int adcValues [32];
    unsigned char adc_index = 0;
    CHS0 = 1; //AN1
    CHS1 = 0;
    CHS2 = 0;
    CHS3 = 0;
    CHS4 = 0;
    ADON = 1;
    while(adc_index<32){
    __delay_ms(1);
    GO_nDONE = 1;
    while (GO_nDONE==1);
    adc_Reading = (ADRESH<<8) + ADRESL ;
    //high = ADRESH;
    //low = ADRESL;
    adcValues[adc_index] = adc_Reading;
   adc_index++;
    };    
    adc_Average = 0;
   for (int i=0; i < 32; i++)
    {
        adc_Average += adcValues[i];
   }    
        adc_index= 0;
        adc_Return = adc_Average/32;
    
    return (adc_Return);  
} //leitura da bateria
int botao_media(){
    int adc_Reading;
    unsigned int adc_Average;
    unsigned int adc_Return;
    unsigned int adcValues [32];
    unsigned char adc_index = 0;
    CHS0 = 0; //AN6
    CHS1 = 1;
    CHS2 = 1;
    CHS3 = 0;
    CHS4 = 0;
    ADON = 1;
    while(adc_index<32){
    __delay_ms(1);
    GO_nDONE = 1;
    while (GO_nDONE==1);
    adc_Reading = (ADRESH<<8) + ADRESL ;
    //high = ADRESH;
    //low = ADRESL;
    adcValues[adc_index] = adc_Reading;
   adc_index++;
    };    
    adc_Average = 0;
   for (int i=0; i < 32; i++)
    {
        adc_Average += adcValues[i];
   }    
        adc_index= 0;
        adc_Return = adc_Average/32;
    
    return (adc_Return);  
} //leitura do jumper JP3
//função de gravar e ler da eeprom
void eeprom_write_code(unsigned int ADRL, unsigned int DATA){
    unsigned int gie_Status;
    
    while(WR);            // check the WR bit to see if a previous Write operation is in progress
    EEADRL= ADRL;  // Write the address to EEADRL.
    EEDATL= DATA;    // load the 8-bit data value to be written in the EEDATA register.
    WREN = 1;               // Set the WREN bit to enable eeprom operation.
    gie_Status = GIE;     // Copy the current Interrupt state
    GIE = 0;              // Disable the interrupts
    EECON2 = 0x55;          // Execute the special instruction sequence
    EECON2 = 0xAA;          // Refer the datasheet for more info
    WR = 1;                 // Set the WR bit to trigger the eeprom write operation.
    GIE = gie_Status;     // Restore the interrupts
    WREN = 0;               // Disable the EepromWrite
}
int eeprom_read_code(unsigned int ADRL){
    while(RD || WR);           // check the WR&RD bit to see if a RD/WR is in progress
    EEADRL = ADRL;       // Write the address to EEADR.
    RD = 1;                    // Set the RD bit to trigger the eeprom read operation.
    return(EEDATL);            // Return the data read form eeprom.
}
//--------------------------------
int main() {
    // PORT A CFG
    TRISA = 0b00001010;
    PORTA = 0b00010000;
    LATA = 0b00010000;
    ANSELA = 0b00001010;
    //--------  
    // PORT C CFG
    TRISC = 0b00000100;
    PORTC = 0b00100000;
    LATC = 0b00100000;
    ANSELC = 0b00000100;      
    //--------
    //OSC CFG 1MHZ
    OSCCON = 0b01011011; 
    OSCTUNE = 0b00000000;
    //------
    // ADC CFG
    // Seleção dos canais AN1_bateria , AN2 VREF1 - baixa e AN5 VREF2 - alta via função
    ADFM = 1; //bits de conversão justificados a direita
    ADCS0 = 0; //clock da conversão TAD - FOSC/8 
    ADCS1 = 0; //------
    ADCS2 = 1; //------
    //ADNREF = 0; //referencia negativa = VSS
    ADPREF0 = 0; //referencia positiva = VDD
    ADPREF1 = 0; //------
    //--------    
    int b = 0; 
    int a = 0;  
    //Leitura dos valores calibrados daa EEPROM 
    __delay_ms(1);    
    unsigned int bit_low_adrsh_read = eeprom_read_code(0x01); //eeprom_read_code(ADRL, DATA)
    __delay_ms(1); //delay de segurança par eeprom
    unsigned int bit_low_adrsl_read = eeprom_read_code(0x00); // enredeço 0x00 e 0x01 = baixa  
    __delay_ms(1); //delay de segurança par eeprom
    bit_low_adrsh_read = (bit_low_adrsh_read<<8); //reorganiza os bits e salva valor
    unsigned int min = bit_low_adrsh_read+bit_low_adrsl_read; //define 'min'    
    //leitura da eeprom - Alta
    __delay_ms(1); //delay de segurança par eeprom
    unsigned int bit_high_adrsl_read = eeprom_read_code(0x02); //eeprom_read_code(ADRL, DATA)
    __delay_ms(1); //delay de segurança par eeprom
    unsigned int bit_high_adrsh_read = eeprom_read_code(0x03); //endereço 0x02 e 0x03 = alta   
    __delay_ms(1); //delay de segurança par eeprom
    bit_high_adrsh_read = (bit_high_adrsh_read<<8); //reorganiza os bits e salva valor
    unsigned int max = bit_high_adrsh_read+bit_high_adrsl_read; //define 'max'
    //------------------------
    inicio:    //caminho para goto   
    //----------------    
    __delay_ms(1);
    while( b < 2 ){ //função loop em monitoramento        
    led2 = 0; // LED Funcionamento
    //------------------- Programa REAL 'monitoramento'
    if (min>tbat_media()){
        __delay_ms(40);
        pisca_led();
        b++;
    };
    if (min<=tbat_media()){
        b=0;
    };
    //--------------------
   }; //while fim
   //---------------------   
    __delay_ms(1);
    while( a < 2 ){ //função loop 'bloqueada'
    led2 = 0; // LED Funcionamento
    //-------------------- Programa REAL 'bloqueada'
    if (max<=tbat_media()){
        pisca_led();        
        a++;
        __delay_ms(40);
    } else {
        a=0;
        ctrl = 1;
            };
    }; //fim do while
    //quando sair do while: reseta ctrl, zera contadores e volta ao loop 'monitoramento'
    ctrl = 0;
    a = 0;
    b = 0;
    goto inicio; // reseta codigo
    //codigo nunca acaba
    return 0;
}