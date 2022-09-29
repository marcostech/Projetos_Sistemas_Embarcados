/* Project Name: Sistema de Controle Automatico
   Author: Marcos Vinicius Pereira da Silva
   Date: 09/09/22
   Description: Firmware to use in LUF800R00, this programm aims to give a easy
   setup and installation for the user, it can configure its main charge characteristics
   such as end voltage and maximum charge time. The main goal of this programm
   is to replace and be the standard for all automatic chargers of the company.
   To use this system is advised to install the Oled display on the i2c port, without
   it will be very difficult to caliber the system, it can be used with only the serial port
   but we recommend otherwise.
   Any changes on this code is not permited, under lincesed use only
*/
//Library
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//------------------------
//Display CFG
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//------------------------
//ADC CFG
#define ADC_CH  A0
//------------------------
//Digital CFG
#define buttonCursorDown 2
#define buttonCursorUp 3
#define outputRelay 5
#define buttonEnter 8
#define outputExtra 10

void setup() {
  pinMode (LED_BUILTIN, OUTPUT);
  //button cfg
  pinMode(buttonCursorUp, INPUT_PULLUP);
  pinMode(buttonCursorDown, INPUT_PULLUP);
  pinMode(buttonEnter, INPUT_PULLUP);
  pinMode(outputRelay, OUTPUT);
  pinMode(outputExtra, OUTPUT);
  //
  digitalWrite(outputRelay, LOW);
  digitalWrite(outputExtra, LOW);
  //Display verifier
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    while (true) { //Lock program if display fail
      digitalWrite(LED_BUILTIN, HIGH);
      delay(300);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
  }  
}

void loop() {  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(1,1);
  display.print("eeprom_clear_v1");
  display.setCursor(1,12);
  display.print("Operando EEPROM");
  display.setCursor(1,22);
  display.print("N desligue, aguarde...");
  display.display();
  delay(2000);
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0xFF);
    delay(1);
  }
  display.clearDisplay(); 
  display.setCursor(1,1);
  display.print("Finalizado!");
  display.setCursor(1,12);
  display.print("EEPROM zerada -> 0xFF");   
  display.setCursor(1,22);
  display.print("Seguro para desligar");
  display.display();

  while(true) {
    delay(500);
    //Led system on blink
    !digitalRead(LED_BUILTIN) ? digitalWrite(LED_BUILTIN, HIGH) : digitalWrite(LED_BUILTIN, LOW) ;
  }
}