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
//#include <ArduinoJson.h>
//------------------------
//Display CFG
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define LOGO_HEIGHT   32
#define LOGO_WIDTH    128
//LOGO IMAGE
static const unsigned char PROGMEM logo_bmp[] =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x70, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0e,
  0x00, 0x01, 0xfd, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x0e,
  0x00, 0x1b, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x1c,
  0x7f, 0xfe, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x1c,
  0x3f, 0xfc, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x1c,
  0x1f, 0xfe, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x38,
  0x07, 0xff, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x38,
  0x03, 0xff, 0x81, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x38,
  0x01, 0xff, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x70,
  0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x70,
  0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x70,
  0x01, 0xdf, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0,
  0x07, 0xcf, 0xf8, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0,
  0x1f, 0x83, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0,
  0x0f, 0x81, 0xfe, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x80, 0xfe, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x81, 0xff, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x80, 0xff, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x80, 0x3c, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x80, 0x1e, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x80, 0x0f, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0xc0, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xf0, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x3e, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x0f, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x03, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
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
uint8_t delayExtra = 1;
uint8_t cursorDelayTime = 0; //buffer variable to store input data
uint8_t cursorDelayTimeMenuEntry = 5; //"delay" to enter cfg menu
boolean  menuEnabled = false; //Start with menu disabled
//------------------------
//EEPROM CFG
const long OVF = 1000000;
int endVoltageADDR = 10;
int startVoltageADDR = 20;
int maxChargeTimeADDR = 30;
int R1ADDR = 40;
int R2ADDR = 50;
int adcFixADDR = 60;
//------------------------
//void sendSerialJson(float batteryVoltage, String cycleTime, int cycleCurrent, String cycleStatus);
void menuConfig();

class MenuCursor{
  private:
    uint8_t menuCursorPosition;
    bool menuEntryFlag;
    bool menuSetFlag;

  public:
    MenuCursor():menuCursorPosition(0),menuEntryFlag(false),menuSetFlag(false){};

    void clear() {
      menuCursorPosition = 0;
      menuEntryFlag = false;
      menuSetFlag = false;
    }

    uint8_t readPress(uint8_t detectWindow) {
      uint8_t pressCounterEnter = 0;
      uint8_t pressCounterUp = 0;      
      uint8_t pressCounterDown = 0;
      uint8_t keyPressed = 0;

      for (uint8_t i = 0; i < detectWindow; i++) {   
        //Read     
        if (digitalRead(buttonEnter) == LOW) {
          pressCounterEnter++;
        } 
         if (digitalRead(buttonCursorUp) == LOW) {
          pressCounterUp++;
        }
         if (digitalRead(buttonCursorDown) == LOW) {
          pressCounterDown++;
        }        
        //Set Condition  
        if (pressCounterEnter > (detectWindow / 2)) {
          keyPressed = 1;
          break;
        } 
        if (pressCounterUp > (detectWindow / 2)) {
          keyPressed = 2;          
          break;
        } 
        if (pressCounterDown > (detectWindow / 2)) {
          keyPressed = 3;          
          break;
        }
        delay(10);
      }
      return keyPressed;
    }

    void updateCursor(uint8_t keyPressed) {
      //3 = Down / 2 = Up / 1 = Menu
      if (keyPressed == 1) {        
        if(menuEntryFlag) {
          menuEntryFlag = false;
        } else {
          menuEntryFlag = true;
          }
      }  
      if(keyPressed == 2) {
        menuCursorPosition++;
      }  
      if(keyPressed == 3 && menuCursorPosition > 0) {
        menuCursorPosition--;          
      }
    }

    uint8_t getMenuCursorPosition() {
      return menuCursorPosition;
    }

    uint8_t setMenuCursorPosition(uint8_t newPosition){
      menuCursorPosition = newPosition;
    } 

    bool getMenuFlag() {
      return menuEntryFlag;
    }

    void clearMenuFlag(){
      menuEntryFlag = false;
    }

};

class ChargeCycle  {
  private:
    short currentTimeHours;
    byte currentCycle;
    unsigned long  offsetMillis;

  public:
    ChargeCycle():currentCycle(1), offsetMillis(0), currentTimeHours(0){};
    void Begin(){};

    String getCurrentTimeFormated () {
      unsigned long currentTimeSeconds = (millis() - offsetMillis ) / 1000; //Test different time scenarios here
      short currentTimeMinutes = 0;
      short currentTimeHoursFormated = 0;
      short currentTimeSecondsFormated = 0;
      short currentTimeMinutesFormated = 0;
      boolean hasTimeLeft = true;

      /*If the seconds variables has more than 60 then decrement 60 and add +1 to minutes
         Loop until there is seconds / 60 < 1
         Minutes loop will have the same logic
         Hours will only be incremented
      */

      //Seconds processing
      while (hasTimeLeft) {
        if ((currentTimeSeconds / 60) >= 1) {
          currentTimeSeconds -= 60;
          ++currentTimeMinutes;
        } else {
          hasTimeLeft = false;
          currentTimeSecondsFormated = currentTimeSeconds;
        }
      }
      //Minutes processing
      hasTimeLeft = true;
      while (hasTimeLeft) {
        if ((currentTimeMinutes / 60) >= 1) {
          currentTimeMinutes -= 60;
          ++currentTimeHoursFormated;
        } else {
          hasTimeLeft = false;
          currentTimeMinutesFormated = currentTimeMinutes;
        }
      }
      //String formater - get values and make a formated string to return
      setCurrentTimeHours(currentTimeHoursFormated);
      char stringBuffer[20];
      sprintf(stringBuffer, "%02d:%02d:%02d", currentTimeHoursFormated, currentTimeMinutesFormated, currentTimeSeconds);
      return stringBuffer;
    }

    void addCurrentCycle () {
      ++currentCycle ;
    }

    int getCurrentCycle () {
      return currentCycle;
    }

    void setOffsetMillis (unsigned long newOffsetMillis) {
      offsetMillis = newOffsetMillis;
    }

    void setCurrentTimeHours (int timeValue) {
      currentTimeHours = timeValue;
    }

    int getCurrentTimeHours () {
      return currentTimeHours;
    }
};

class Battery {
  private:
    float voltage;
    float endVoltage;
    float startVoltage;
    byte maxChargeTime;

  public:
    Battery():endVoltage(0), startVoltage(0), maxChargeTime(0), voltage(0){};
    void Begin(){

      endVoltage = EEPROM.get(endVoltageADDR, endVoltage);
      delay(10);
      if (isnan(endVoltage) || endVoltage <= 0 || endVoltage > OVF) {
        endVoltage = 14.70;
      }

      startVoltage = EEPROM.get(startVoltageADDR, startVoltage);
      delay(10);
      if (isnan(startVoltage) || startVoltage <= 0 || startVoltage > OVF) {
        startVoltage = 11.80;
      }

      maxChargeTime = EEPROM.get(maxChargeTimeADDR, maxChargeTime);
      delay(10);
      if (isnan(maxChargeTime) || maxChargeTime <= 0 || maxChargeTime > 254) {
        maxChargeTime = 10;
      }
    }

    float getVoltage () {
      return voltage;
    }

    void setVoltage (float voltageRead) {
      voltage = voltageRead;
    }

    float getStartVoltage () {
      return startVoltage;
    }

    float getEndVoltage () {
      return endVoltage;
    }

    void  setStartVoltage (float voltage) {
      startVoltage = voltage;
      EEPROM.put(startVoltageADDR, startVoltage);
    }

    void  setEndVoltage (float voltage) {
      endVoltage = voltage;
      EEPROM.put(endVoltageADDR, endVoltage);
    }

    void setMaxChargeTime (byte newChargeTime) {
      maxChargeTime = newChargeTime;
      EEPROM.put(maxChargeTimeADDR, maxChargeTime);
    }

    byte getMaxChargeTime () {
      return maxChargeTime;
    }

};

class AdConverter {
  private:
    float Resistor1;
    float Resistor2;
    float adcFix;
    byte adcChannel = ADC_CH;
    float aref = 0.00488 ;

  public:
    AdConverter(): Resistor1(0), Resistor2(0), adcFix(0){};
    void Begin() {

      Resistor1 = EEPROM.get(R1ADDR, Resistor1);
      delay(10);
      if (isnan(Resistor1) || Resistor1 <= 0 || Resistor1 > OVF) {
        Resistor1 = 98400.00;
      }

      Resistor2 = EEPROM.get(R2ADDR, Resistor2);
      delay(10);
      if (isnan(Resistor2) || Resistor2 <= 0 || Resistor2 > OVF) {
        Resistor2 = 4550.00;
    }

      adcFix = EEPROM.get(adcFixADDR, adcFix);
      delay(10);
      if (isnan(adcFix) || adcFix <= 0 || adcFix > OVF) {
        adcFix = 0.002;
      }
    }

    float getReading (int iteratorLimit) {
      long  voltageBuffer = 0;
      long  voltageRead = 0;

      for ( int i = 0; i < iteratorLimit; i++) {
        voltageRead = analogRead(getAdcChannel());
        voltageBuffer = voltageBuffer + (voltageRead * voltageRead);
      }
      // Processing
      float adcRead = voltageBuffer / iteratorLimit;
      float adcReadRaw = 0;
      adcRead = sqrt(adcRead);
      adcRead *= aref;  // (adcRead * AREF) / 1024.00 - currently -> Aref = 3,500
      adcRead = adcRead / (Resistor2 / (Resistor1 + Resistor2)); // Vout / (R2/(R1+R2)) - currently -> 4550 / 102.950 = 0,04419
      adcReadRaw = (adcRead * getAdcFix()); // minor % fix on readings
      adcRead -= adcReadRaw;
      return adcRead;
    }

    void setAdcFix (float fixValue) {
      adcFix = fixValue;
      EEPROM.put(adcFixADDR, adcFix);
    }

    float getAdcFix () {
      return adcFix ;
    }

    void setR1 (float R1Value) {
      Resistor1 = R1Value;
      EEPROM.put(R1ADDR, Resistor1);
    }

    float getR1 () {
      return Resistor1 ;
    }

    void setR2 (float R2Value) {
      Resistor2 = R2Value;
      EEPROM.put(R2ADDR, Resistor2);
    }

    float getR2 () {
      return Resistor2 ;
    }

    void setAdcChannel (byte NewAdcChannel) {
      adcChannel = NewAdcChannel;
    }

    byte getAdcChannel () {
      return adcChannel;
    }
};
//---------------------------------
AdConverter adc;
Battery battery;
ChargeCycle cycle;
MenuCursor menuCursor;

void setup() {  
  //3.5V external reference
  //analogReference(EXTERNAL);
  //Led PWR on cfg
  pinMode (LED_BUILTIN, OUTPUT);
  //button cfg
  pinMode(buttonCursorUp, INPUT_PULLUP);
  pinMode(buttonCursorDown, INPUT_PULLUP);
  pinMode(buttonEnter, INPUT_PULLUP);
  pinMode(outputRelay, OUTPUT);
  pinMode(outputExtra, OUTPUT);
  //
  digitalWrite(outputRelay, LOW);
  digitalWrite(outputExtra, HIGH);
  //Display verifier
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    while (true) { //Lock program if display fail
      digitalWrite(LED_BUILTIN, HIGH);
      delay(300);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
  }
  //Display Template
  display.clearDisplay(); //Clear display data
  display.drawBitmap(( display.width()  - LOGO_WIDTH ) / 2, (display.height() - LOGO_HEIGHT) / 2, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  display.setTextColor(SSD1306_WHITE);
  delay(100);
}
//
void loop() {  
  adc.Begin();
  battery.Begin();
  cycle.Begin();//does nothing
  digitalWrite(outputRelay, LOW);
  digitalWrite(outputExtra, HIGH);
  //Serial.begin(115200);

  //"Global" utils
  unsigned long previousMillis = 0;
  byte chargeStatus = 0; //Start System on Status - 0
  String lastTime;
  //System Lock
  while (true) { //Power On Routine -- TODO: change to State Machina -> switch case?
    //Button state checker - menu entry
    if (digitalRead(buttonEnter) == LOW) {
      cursorDelayTime++;
    } else {
      cursorDelayTime = 0;
    }
    if (cursorDelayTime > cursorDelayTimeMenuEntry || menuEnabled) {
      menuEnabled = false; //clear flag
      cursorDelayTime = 0; //set cursor verifier to 0
      chargeStatus = 0; //change if needed
      menuConfig(); //Enter Menu
    }

    //Charger - Startup
    while (chargeStatus == 0) {     // Self-Test Routine -- TODO: change to State Machine -> switch case?
      chargeStatus = 1;
      //load variables or selftest
    }
    //Charger - Charge
    while (chargeStatus == 1) {      // On Charge Routine -- TODO: change to State Machine -> switch case?      
      digitalWrite(outputRelay, LOW);
      digitalWrite(outputExtra, HIGH);
      //Button state checker - menu entry
      if (digitalRead(buttonEnter) == LOW) {
        cursorDelayTime++;
      } else {
        cursorDelayTime = 0;
      }
      if (cursorDelayTime > cursorDelayTimeMenuEntry) {
        menuEnabled = true;
        cursorDelayTime = 0;
        chargeStatus = 0;
      }
      //Led system on blink
      !digitalRead(LED_BUILTIN) ? digitalWrite(LED_BUILTIN, HIGH) : digitalWrite(LED_BUILTIN, LOW) ;
      //Voltage reading and battery setting
      battery.setVoltage(adc.getReading(8000));
      //Display routine
      display.drawBitmap(( display.width()  - LOGO_WIDTH ) / 2, (display.height() - LOGO_HEIGHT) / 2, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.setTextColor(SSD1306_WHITE);
      display.fillRect(34, 0, 80, 32, SSD1306_BLACK);
      display.setTextSize(1);
      display.setCursor(52, 16);
      display.println(F("Tempo atual"));
      display.setCursor(52, 24);
      display.println(cycle.getCurrentTimeFormated());
      display.setTextSize(2);
      display.setCursor(52, 1);
      display.println(battery.getVoltage(), 2);
      display.display();
      /*
      //Send Json
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= 200) {
        previousMillis = currentMillis;
        sendSerialJson(battery.getVoltage(), cycle.getCurrentTimeFormated(), cycle.getCurrentCycle(), F("Em Carga"));
        delay(50);
      }
      */
      //Charge complete checker
      if (battery.getEndVoltage() < battery.getVoltage() || cycle.getCurrentTimeHours() == battery.getMaxChargeTime ()) {
        lastTime = cycle.getCurrentTimeFormated();
        chargeStatus = 2;
        display.clearDisplay();
        previousMillis = millis();
      }
    }
    //Charger - Complete
    while (chargeStatus == 2) {      // Charge Complete Routine -- TODO: change to State Machine -> switch case ?
      digitalWrite(outputRelay, HIGH);
      digitalWrite(outputExtra, LOW);
      //Button state checker - menu entry
      if (digitalRead(buttonEnter) == LOW) {
        cursorDelayTime++;
      } else {
        cursorDelayTime = 0;
      }
      if (cursorDelayTime > cursorDelayTimeMenuEntry) {
        menuEnabled = true;
        cursorDelayTime = 0;
        chargeStatus = 0;
      }
      //Display Routine
      display.setTextSize(1);
      display.setCursor(1, 1);
      display.print(F("Completa: ciclo "));
      display.print(cycle.getCurrentCycle());
      display.setTextSize(2);
      display.setCursor(1, 15);
      display.println(lastTime);
      display.display();
      battery.setVoltage(adc.getReading(8000));
      /*
      //Send Json
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= 200) {
        previousMillis = currentMillis;
        sendSerialJson(battery.getVoltage(), lastTime, cycle.getCurrentCycle(), F("Completa"));
        delay(50);
      }
      */
      //Start Voltage checker
      if (battery.getStartVoltage() > battery.getVoltage()) {
        cycle.addCurrentCycle();
        chargeStatus = 1;
        display.clearDisplay();
        cycle.setOffsetMillis(millis());
      }
    }

  }
   
}//Loop here
// 

/*
void sendSerialJson(float batteryVoltage, String cycleTime, int cycleCurrent, String cycleStatus) {
  StaticJsonDocument<96> doc;
  char bufferArr[6];
  dtostrf(batteryVoltage, 4, 2, bufferArr);
  doc["batteryVoltage"] = bufferArr;
  doc["chargeTime"] = cycleTime;
  doc["currentCycle"] = cycleCurrent;
  doc["chargeStatus"] = cycleStatus;
  serializeJsonPretty(doc, Serial);
  Serial.println(); // make correct exibition in terminal - remove when sending to a programm
}
*/

void menuConfig(){
  //Menu Entry loop            // Menu CFG Routine -- TODO: change to State Machine -> switch case?
      // uint8_t menuUse = 0;
      // uint8_t menuCursorPosition = 0;
      // uint8_t menuCursorPositionInsideMenu = 0;
      uint8_t menuPage = 0;
      // uint8_t cursorDelayTimeUp = 0; //buffer variable to store input data
      // uint8_t cursorDelayTimeDown = 0; ////buffer variable to store input data
      // uint8_t cursorDelayTimeInterval = 1; //interval for accept the input
      // uint8_t cursorDelayTimeIntervalMenu = 2; //interval for accept the input

      display.clearDisplay();
      display.display();
      delay(1000); //make small delay for the user to release the Enter button
      while (true) {
        //Page 1
        while (menuPage == 0) {
          !digitalRead(LED_BUILTIN) ? digitalWrite(LED_BUILTIN, HIGH) : digitalWrite(LED_BUILTIN, LOW) ;
          //Menu display - necessary here for correct exhibition after the hover animation
          menuCursor.updateCursor(menuCursor.readPress(10));
          display.clearDisplay();
          display.setTextSize(1);
          display.setTextColor(SSD1306_WHITE, SSD1306_BLACK); //change back to original color, after hover animation has set the inverse
          display.setCursor(1, 1);
          display.print(F("Menu Prog. Sair"));
          display.setCursor(1, 12);
          display.print(F("1 - V de REcarga"));
          display.setCursor(1, 22);
          display.print(F("2 - V final de carga"));
          display.setCursor(120, 1);
          display.print(menuCursor.getMenuCursorPosition());
          //Button state checker
          //Menu Selection
          switch (menuCursor.getMenuCursorPosition()) {
            case 0: //Exit CFG menu
              //Hover animation
              display.setCursor(1, 1);
              display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //Animation setter, change background to white and font to black
              display.print(F("Menu Prog. Sair"));
              display.display();
              //Enter Menu
              while (menuCursor.getMenuFlag()) { 
                display.clearDisplay();
                menuCursor.clear();  
                return; //exit function              
              }
              break;

            case 1: //Set charger start voltage Value
              //Hover animation
                display.setCursor(1, 12);
                display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
                display.print(F("1 - V de REcarga"));
                display.display();
              //Enter Menu
              while (false) {
                display.clearDisplay();
                display.setCursor(1, 1);
                display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
                display.print(F("Valor atual:"));
                display.print(battery.getStartVoltage());
                display.display();
                //Button state checker
                if (menuCursor.readPress(10) == 1) {
                  menuCursor.clearMenuFlag();
                }                 
                //Up - set
                if (menuCursor.readPress(10) == 2) {
                  battery.setStartVoltage(battery.getStartVoltage() + 0.05);                  
                }                
                //Down - set
                if (menuCursor.readPress(10) == 3) {
                  battery.setStartVoltage(battery.getStartVoltage() - 0.05);                  
                }
              }
              break;

            case 2: //Set charger end voltage Value
              //Hover animation
                display.setCursor(1, 22);
                display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
                display.print(F("2 - V final de carga"));
                display.display();
              //Button state checker              
              // //Enter Menu
              // while (menuUse == 1) {
              //   display.clearDisplay();
              //   display.setCursor(1, 1);
              //   display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
              //   display.print(F("Valor Atual:"));
              //   display.print(battery.getEndVoltage());
              //   display.display();
              //   //Button state checker
              //   //Enter - checker
              //   if (digitalRead(buttonEnter) == LOW) {
              //     cursorDelayTime++;
              //   } else {
              //     cursorDelayTime = 0;
              //   }
              //   //Enter - set
              //   if (cursorDelayTime > cursorDelayTimeIntervalMenu + delayExtra) {
              //     menuUse = 0;
              //     menuPage = 0;
              //     cursorDelayTime = 0;
              //   }
              //   //Up - checker
              //   if (digitalRead(buttonCursorUp) == LOW) {
              //     cursorDelayTimeUp++;
              //   } else {
              //     cursorDelayTimeUp = 0;
              //   }
              //   //Up -set
              //   if (cursorDelayTimeUp > (cursorDelayTimeInterval + delayExtra)) {
              //     battery.setEndVoltage(battery.getEndVoltage() + 0.05);
              //     cursorDelayTimeUp = 0;
              //   }
              //   //Down - checker
              //   if (digitalRead(buttonCursorDown) == LOW) {
              //     cursorDelayTimeDown++;
              //   } else {
              //     cursorDelayTimeDown = 0;
              //   }
              //   //Down - set
              //   if (cursorDelayTimeDown > (cursorDelayTimeInterval + delayExtra)) {
              //     battery.setEndVoltage(battery.getEndVoltage() - 0.05);
              //     cursorDelayTimeDown = 0;
              //   }
              // }
              //
               break;

            case 3:
              menuCursor.setMenuCursorPosition(0);
              break;

            default:
              break;
          }
        }
      //   //Page 2
      //   while (menuPage == 1) {
      //     //Button state checker
      //     //Up - checker
      //     if (digitalRead(buttonCursorUp) == LOW) {
      //       cursorDelayTimeUp++;
      //     } else {
      //       cursorDelayTimeUp = 0;
      //     }
      //     //Up - set
      //     if (cursorDelayTimeUp > cursorDelayTimeInterval) {
      //       menuCursorPosition++;
      //       cursorDelayTimeUp = 0;
      //     }
      //     //Down - checker
      //     if (digitalRead(buttonCursorDown) == LOW) {
      //       cursorDelayTimeDown++;
      //     } else {
      //       cursorDelayTimeDown = 0;
      //     }
      //     //Down - set
      //     if (cursorDelayTimeDown > cursorDelayTimeInterval) {
      //       if (menuCursorPosition > 0) {
      //         menuCursorPosition--;
      //       } else {
      //         menuCursorPosition = 0;
      //       }
      //       cursorDelayTimeDown = 0;
      //     }
      //     //Menu display
      //     display.clearDisplay();
      //     display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
      //     display.setCursor(1, 1);
      //     display.print(F("3 - T maximo de carga"));
      //     display.setCursor(1, 12);
      //     display.print(F("4 - Corrigir ADC"));
      //     display.setCursor(1, 22);
      //     display.print(F("5 - Setar R1"));

      //     switch (menuCursorPosition) {
      //       case 2:
      //         menuPage = 0;
      //         break;

      //       case 3: //Set charging maximum time Value
      //         //Hover animation
      //         if (menuCursorPosition == 3) {
      //           display.setCursor(1, 1);
      //           display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      //           display.print(F("3 - T maximo de carga"));
      //           display.display();
      //         }
      //         //Button state checker
      //         //Enter - checker
      //         if (digitalRead(buttonEnter) == LOW) {
      //           cursorDelayTime++;
      //         } else {
      //           cursorDelayTime = 0;
      //         }
      //         //Enter - set
      //         if (cursorDelayTime > cursorDelayTimeIntervalMenu) {
      //           menuUse = 1;
      //           cursorDelayTime = 0;
      //         }
      //         //Enter Menu
      //         while (menuUse == 1) {
      //           display.clearDisplay();
      //           display.setCursor(1, 1);
      //           display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
      //           display.print(F("Valor atual em Horas:"));
      //           display.print(battery.getMaxChargeTime());
      //           display.display();
      //           //Button state checker
      //           //Enter - checker
      //           if (digitalRead(buttonEnter) == LOW) {
      //             cursorDelayTime++;
      //           } else {
      //             cursorDelayTime = 0;
      //           }
      //           //Enter - set
      //           if (cursorDelayTime > cursorDelayTimeIntervalMenu + delayExtra) {
      //             menuUse = 0;
      //             menuPage = 1;
      //             cursorDelayTime = 0;
      //           }
      //           //Up - checker
      //           if (digitalRead(buttonCursorUp) == LOW) {
      //             cursorDelayTimeUp++;
      //           } else {
      //             cursorDelayTimeUp = 0;
      //           }
      //           //Up - set
      //           if (cursorDelayTimeUp > (cursorDelayTimeInterval + delayExtra)) {
      //             battery.setMaxChargeTime(battery.getMaxChargeTime() + 1);
      //             cursorDelayTimeUp = 0;
      //           }
      //           //Down - checker
      //           if (digitalRead(buttonCursorDown) == LOW) {
      //             cursorDelayTimeDown++;
      //           } else {
      //             cursorDelayTimeDown = 0;
      //           }
      //           //Downd - set
      //           if (cursorDelayTimeDown > (cursorDelayTimeInterval + delayExtra)) {
      //             battery.setMaxChargeTime(battery.getMaxChargeTime() - 1);
      //             cursorDelayTimeDown = 0;
      //           }
      //         }
      //         break;

      //       case 4: //Set Adc fix Value
      //         //Hover animation
      //         if (menuCursorPosition == 4) {
      //           display.setCursor(1, 12);
      //           display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //Animation set
      //           display.print(F("4 - Corrigir ADC"));
      //           display.display();
      //         }
      //         //--------------
      //         //Button state checker
      //         //Enter - checker
      //         if (digitalRead(buttonEnter) == LOW) {
      //           cursorDelayTime++;
      //         } else {
      //           cursorDelayTime = 0;
      //         }
      //         //Enter - set
      //         if (cursorDelayTime > cursorDelayTimeIntervalMenu) {
      //           menuUse = 1;
      //           cursorDelayTime = 0;
      //         }
      //         //Enter Menu
      //         while (menuUse == 1) {
      //           display.clearDisplay();
      //           display.setCursor(1, 1);
      //           display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);  //Animation remove
      //           display.print(F("Valor em % multiplicado por 1000:"));
      //           display.setCursor(1, 22);
      //           display.print(adc.getAdcFix() * 1000);
      //           display.display();
      //           //Set value
      //           //Button state checker
      //           //Enter - checker
      //           if (digitalRead(buttonEnter) == LOW) {
      //             cursorDelayTime++;
      //           } else {
      //             cursorDelayTime = 0;
      //           }
      //           //Enter - set
      //           if (cursorDelayTime > cursorDelayTimeIntervalMenu + delayExtra) {
      //             menuUse = 0;
      //             menuPage = 1;
      //             cursorDelayTime = 0;
      //           }
      //           //Up - checker
      //           if (digitalRead(buttonCursorUp) == LOW) {
      //             cursorDelayTimeUp++;
      //           } else {
      //             cursorDelayTimeUp = 0;
      //           }
      //           //Up - set
      //           if (cursorDelayTimeUp > (cursorDelayTimeInterval + delayExtra)) {
      //             adc.setAdcFix(adc.getAdcFix() + 0.001);
      //             cursorDelayTimeUp = 0;
      //           }
      //           //Down - checker
      //           if (digitalRead(buttonCursorDown) == LOW) {
      //             cursorDelayTimeDown++;
      //           } else {
      //             cursorDelayTimeDown = 0;
      //           }
      //           //Down - set
      //           if (cursorDelayTimeDown > (cursorDelayTimeInterval + delayExtra)) {
      //             adc.setAdcFix(adc.getAdcFix() - 0.001);
      //             cursorDelayTimeDown = 0;
      //           }
      //         }
      //         break;

      //       case 5: //Set Adc R1 Value
      //         //Hover animation
      //         if (menuCursorPosition == 5) {
      //           display.setCursor(1, 22);
      //           display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      //           display.print(F("5 - Setar R1"));
      //           display.display();
      //         }
      //         //Button state checker
      //         //Enter - checker
      //         if (digitalRead(buttonEnter) == LOW) {
      //           cursorDelayTime++;
      //         } else {
      //           cursorDelayTime = 0;
      //         }
      //         //Enter - set
      //         if (cursorDelayTime > cursorDelayTimeInterval) {
      //           menuUse = 1;
      //           cursorDelayTime = 0;
      //         }
      //         //Enter Menu
      //         while (menuUse == 1) {
      //           display.clearDisplay();
      //           display.setCursor(1, 1);
      //           display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
      //           display.print(F("Valor Atual:"));
      //           display.print(adc.getR1());
      //           display.display();
      //           //Button state checker
      //           //Enter - checker
      //           if (digitalRead(buttonEnter) == LOW) {
      //             cursorDelayTime++;
      //           } else {
      //             cursorDelayTime = 0;
      //           }
      //           //Enter - set
      //           if (cursorDelayTime > cursorDelayTimeIntervalMenu + delayExtra) {
      //             menuUse = 0;
      //             menuPage = 1;
      //             cursorDelayTime = 0;
      //           }
      //           //Up - checker
      //           if (digitalRead(buttonCursorUp) == LOW) {
      //             cursorDelayTimeUp++;
      //           } else {
      //             cursorDelayTimeUp = 0;
      //           }
      //           //Up -set
      //           if (cursorDelayTimeUp > (cursorDelayTimeInterval + delayExtra)) {
      //             adc.setR1(adc.getR1() + 100);
      //             cursorDelayTimeUp = 0;
      //           }
      //           //Down - checker
      //           if (digitalRead(buttonCursorDown) == LOW) {
      //             cursorDelayTimeDown++;
      //           } else {
      //             cursorDelayTimeDown = 0;
      //           }
      //           //Down - set
      //           if (cursorDelayTimeDown > (cursorDelayTimeInterval + delayExtra)) {
      //             adc.setR1(adc.getR1() - 100);
      //             cursorDelayTimeDown = 0;
      //           }
      //         }
      //         break;

      //       case 6:
      //         menuPage = 2;
      //         break;

      //       default:
      //         break;
      //     }
      //   }
      //   //Page 3
      //   while (menuPage == 2) {
      //     //Button state checker
      //     //Up - checker
      //     if (digitalRead(buttonCursorUp) == LOW) {
      //       cursorDelayTimeUp++;
      //     } else {
      //       cursorDelayTimeUp = 0;
      //     }
      //     //Up - set
      //     if (cursorDelayTimeUp > cursorDelayTimeInterval + 2) {
      //       menuCursorPosition++;
      //       cursorDelayTimeUp = 0;
      //     }
      //     //Down - checker
      //     if (digitalRead(buttonCursorDown) == LOW) {
      //       cursorDelayTimeDown++;
      //     } else {
      //       cursorDelayTimeDown = 0;
      //     }
      //     //Down - set
      //     if (cursorDelayTimeDown > cursorDelayTimeInterval + 2) {
      //       if (menuCursorPosition > 0) {
      //         menuCursorPosition--;
      //       } else {
      //         menuCursorPosition = 0;
      //       }
      //       cursorDelayTimeDown = 0;
      //     }
      //     //Menu display - necessary for animations
      //     display.clearDisplay();
      //     display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
      //     display.setCursor(1, 1);
      //     display.print(F("6 - Setar R2"));
      //     display.setCursor(1, 12);
      //     display.print(F("7 - Pre-Cfg"));

      //     switch (menuCursorPosition) {
      //       case 5:
      //         menuPage = 1;
      //         break;

      //       case 6: //Set Adc R2 Value
      //         //Hover animation
      //         if (menuCursorPosition == 6) {
      //           display.setCursor(1, 1);
      //           display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      //           display.print(F("6 - Setar R2"));
      //           display.display();
      //         }
      //         //Button state checker
      //         //Enter - checker
      //         if (digitalRead(buttonEnter) == LOW) {
      //           cursorDelayTime++;
      //         } else {
      //           cursorDelayTime = 0;
      //         }
      //         //Enter - set
      //         if (cursorDelayTime > cursorDelayTimeInterval) {
      //           menuUse = 1;
      //           cursorDelayTime = 0;
      //         }
      //         //Enter Menu
      //         while (menuUse == 1) {
      //           display.clearDisplay();
      //           display.setCursor(1, 1);
      //           display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
      //           display.print(F("Valor Atual:"));
      //           display.print(adc.getR2());
      //           display.display();
      //           //Button state checker
      //           //Enter - checker
      //           if (digitalRead(buttonEnter) == LOW) {
      //             cursorDelayTime++;
      //           } else {
      //             cursorDelayTime = 0;
      //           }
      //           //Enter - set
      //           if (cursorDelayTime > cursorDelayTimeIntervalMenu + delayExtra) {
      //             menuUse = 0;
      //             menuPage = 2;
      //             cursorDelayTime = 0;
      //           }
      //           //Up - checker
      //           if (digitalRead(buttonCursorUp) == LOW) {
      //             cursorDelayTimeUp++;
      //           } else {
      //             cursorDelayTimeUp = 0;
      //           }
      //           //Up - set
      //           if (cursorDelayTimeUp > (cursorDelayTimeInterval + delayExtra)) {
      //             adc.setR2(adc.getR2() + 50);
      //             cursorDelayTimeUp = 0;
      //           }
      //           //Down - checker
      //           if (digitalRead(buttonCursorDown) == LOW) {
      //             cursorDelayTimeDown++;
      //           } else {
      //             cursorDelayTimeDown = 0;
      //           }
      //           //Down - set
      //           if (cursorDelayTimeDown > (cursorDelayTimeInterval + delayExtra)) {
      //             adc.setR2(adc.getR2() - 50);
      //             cursorDelayTimeDown = 0;
      //           }
      //         }
      //         break;

      //       case 7: //Set pre defined values for 12v -24v - 36v - 48v
      //         //Hover animation
      //         if (menuCursorPosition == 7) {
      //           display.setCursor(1, 12);
      //           display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      //           display.print(F("7 - Pre-Cfg"));
      //           display.display();
      //         }
      //         //Button state checker
      //         //Enter - checker
      //         if (digitalRead(buttonEnter) == LOW) {
      //           cursorDelayTime++;
      //         } else {
      //           cursorDelayTime = 0;
      //         }
      //         //Enter - set
      //         if (cursorDelayTime > cursorDelayTimeInterval) {
      //           menuUse = 1;
      //           cursorDelayTime = 0;
      //         }
      //         //Enter Menu
      //         while (menuUse == 1) {
      //           //Button State Checker
      //           //Up - checker
      //           if (digitalRead(buttonCursorUp) == LOW) {
      //             cursorDelayTimeUp++;
      //           } else {
      //             cursorDelayTimeUp = 0;
      //           }
      //           //Up - set
      //           if (cursorDelayTimeUp > cursorDelayTimeInterval + 2) {
      //             menuCursorPositionInsideMenu++;
      //             cursorDelayTimeUp = 0;
      //           }
      //           //Down - checker
      //           if (digitalRead(buttonCursorDown) == LOW) {
      //             cursorDelayTimeDown++;
      //           } else {
      //             cursorDelayTimeDown = 0;
      //           }
      //           //Down - set
      //           if (cursorDelayTimeDown > cursorDelayTimeInterval + 2) {
      //             if (menuCursorPositionInsideMenu > 0) {
      //               menuCursorPositionInsideMenu--;
      //             } else {
      //               menuCursorPositionInsideMenu = 0;
      //             }
      //             cursorDelayTimeDown = 0;
      //           }
      //           display.clearDisplay();
      //           display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
      //           display.setCursor(1, 1);
      //           display.print(F("24V"));
      //           display.setCursor(1, 12);
      //           display.print(F("36V"));
      //           display.setCursor(1, 22);
      //           display.print(F("48V"));
      //           //Button state checker
      //           switch (menuCursorPositionInsideMenu) {
      //             case 0: //Set pre defined 24v
      //               //Hover animation
      //               if (menuCursorPositionInsideMenu == 0) {
      //                 display.setCursor(1, 1);
      //                 display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      //                 display.print(F("24V"));
      //                 display.display();
      //               }
      //               //Button state checker
      //               //Enter - checker
      //               if (digitalRead(buttonEnter) == LOW) {
      //                 cursorDelayTime++;
      //               } else {
      //                 cursorDelayTime = 0;
      //               }
      //               //Enter - set
      //               if (cursorDelayTime > cursorDelayTimeInterval + 4) {
      //                 menuUse = 2;
      //                 cursorDelayTime = 0;
      //                 menuCursorPositionInsideMenu = 0;
      //               }
      //               //Enter Menu
      //               while (menuUse == 2) {
      //                 //Set 24V
      //                 battery.setStartVoltage(23.6);
      //                 battery.setEndVoltage(29.5);
      //                 battery.setMaxChargeTime(10);

      //                 menuUse = 0;
      //                 cursorDelayTime = 0;
      //                 menuPage = 0;
      //                 menuCursorPosition = 0;   
      //                 menuCursorPositionInsideMenu = 0;
      //               }
      //               break;

      //             case 1: //Set pre defined 36v
      //               //Hover animation
      //               if (menuCursorPositionInsideMenu == 1) {
      //                 display.setCursor(1, 12);
      //                 display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      //                 display.print(F("36V"));
      //                 display.display();
      //               }
      //               //Button state checker
      //               //Enter - checker
      //               if (digitalRead(buttonEnter) == LOW) {
      //                 cursorDelayTime++;
      //               } else {
      //                 cursorDelayTime = 0;
      //               }
      //               //Enter - set
      //               if (cursorDelayTime > cursorDelayTimeInterval + 4) {
      //                 menuUse = 2;
      //                 cursorDelayTime = 0;
      //                 menuCursorPositionInsideMenu = 0;
      //               }
      //               //Enter Menu
      //               while (menuUse == 2) {
      //                 //Set 36V
      //                 battery.setStartVoltage(35.4);
      //                 battery.setEndVoltage(44.1);
      //                 battery.setMaxChargeTime(10);

      //                 menuUse = 0;
      //                 cursorDelayTime = 0;
      //                 menuPage = 0;
      //                 menuCursorPosition = 0;   
      //                 menuCursorPositionInsideMenu = 0;
      //               }
      //               break;

      //             case 2: //Set pre defined 48v
      //               //Hover animation
      //               if (menuCursorPositionInsideMenu == 2) {
      //                 display.setCursor(1, 22);
      //                 display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      //                 display.print(F("48V"));
      //                 display.display();
      //               }
      //               //Button state checker
      //               //Enter - checker
      //               if (digitalRead(buttonEnter) == LOW) {
      //                 cursorDelayTime++;
      //               } else {
      //                 cursorDelayTime = 0;
      //               }
      //               //Enter - set
      //               if (cursorDelayTime > cursorDelayTimeInterval + 4) {
      //                 menuUse = 2;
      //                 cursorDelayTime = 0;
      //                 menuCursorPositionInsideMenu = 0;
      //               }
      //               //Enter Menu
      //               while (menuUse == 2) {
      //                 //Set 48V
      //                 battery.setStartVoltage(47.2);
      //                 battery.setEndVoltage(59.5);
      //                 battery.setMaxChargeTime(10);

      //                 menuUse = 0;
      //                 cursorDelayTime = 0;
      //                 menuPage = 0;
      //                 menuCursorPosition = 0;                         
      //                 menuCursorPositionInsideMenu = 0;
      //               }
      //               break;

      //             case 3:
      //               menuUse = 0;
      //               cursorDelayTime = 0;
      //               menuPage = 0;
      //               menuCursorPosition = 0;                    
      //               menuCursorPositionInsideMenu = 0;
      //               break;

      //             default:
      //               break;
      //           }
      //         }
      //         break;

      //       case 8:
      //         menuPage = 0;
      //         menuCursorPosition = 0;
      //         break;

      //       default:
      //         break;
      //     }
      //   }
      //
    }
}
//
