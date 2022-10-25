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
//Logo image
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
/*
Menu Strings
[0] = "Menu Prog. Sair"
[1] = "1 - V de REcarga"
[2] = "2 - V final de carga"
[3] = "3 - T maximo de carga"
[4] = "4 - Corrigir ADC"
[5] = "5 - Setar R1"
[6] = "6 - Setar R2"
[7] = "7 - Pre-Cfg" 
[8] = "1 - 24V"
[9] = "2 - 36V"
[10] = "3 - 48V",
[11] = "Valor atual: "
[12] = "Valor atual em Horas:"
[13] = "Valor em % multiplicado por 1000:"
[14] = "Modo Operacao"
[15] = "Modo - Auto"
[16] = "Modo - Bloq"
[17] = "9 - Tempo Retardo"
[18] = "Reservado"
[19] = "Valor atual em s: "
[20] = "10 - Tempo EQ"
[21] = "Valor em min.: "
*/
const char *menuOptions[] = {
  "Menu Prog. Sair", "1 - V de REcarga","2 - V final de carga",
  "3 - T maximo de carga", "4 - Corrigir ADC", "5 - Setar R1",
  "6 - Setar R2","7 - Pre-Cfg", "1 - 24V", "2 - 36V", "3 - 48V",
  "Valor atual: ", "Valor atual em Horas:" , "Valor em % multiplicado por 1000:",
  "8 - Modo Operacao", "Modo - Auto", "Modo - Bloq", "9 - Tempo Retardo",
  "Reservado", "Valor em seg.: ", "10 - Tempo EQ", "Valor em min.: "
};
const char *timeOut = "Timeout";
//ADC CFG
#define ADC_CH  A0
//Digital CFG
#define buttonCursorDown 2
#define buttonCursorUp 3
#define outputRelay 5
#define buttonEnter 8
#define outputExtra 10
//Menu entry CFG
uint8_t cursorDelayTime = 0; //buffer variable to store input data
uint8_t cursorDelayTimeMenuEntry = 5; //"delay" to enter cfg menu
//EEPROM CFG
const long OVF = 1000000;
int systemModeADDR = 100;
int endVoltageADDR = 200;
int startVoltageADDR = 300;
int maxChargeTimeADDR = 400;
int R1ADDR = 500;
int R2ADDR = 600;
int adcFixADDR = 700;
uint16_t countdownValueADDR = 800;
uint8_t countdownFlagADDR = 900;
uint8_t eqCountdownValueADDR = 1000;

//------------------------
//void sendSerialJson(float batteryVoltage, String cycleTime, int cycleCurrent, String cycleStatus);
void menuConfig();
void displayCall();

class SystemMode{
  private:
  uint16_t systemCurrentMode;
  bool countdownFlag;

  public:
  SystemMode():systemCurrentMode(1){};
    
  void Begin() {
    systemCurrentMode = EEPROM.get(systemModeADDR, systemCurrentMode);
    delay(10);
    if (isnan(systemCurrentMode) || systemCurrentMode <= 0 || systemCurrentMode >= 4) {
      //TODO: not working on first programmimg
      setMode(1);
    }

    countdownFlag = EEPROM.get(countdownFlagADDR, countdownFlag);
    delay(10);
    if (countdownFlag < 0 || countdownFlag > 1) {      
      countdownFlag = true;
    }


  }

/*0 = Countdown Mode
1 = Charger Auto Mode
2 = Bloq Mode
*/
  void setMode(uint16_t newMode) {
    systemCurrentMode = newMode;
    EEPROM.put(systemModeADDR, systemCurrentMode);
  }

  uint16_t getCurrentMode(){
    return systemCurrentMode;
  }

  void setCountdownFlag(bool newCountdownFlag){
    if(newCountdownFlag) {
      countdownFlag = 1;
    } else {
      countdownFlag = 0;
    }
    EEPROM.put(countdownFlagADDR, countdownFlag);
  }

  bool getCountdownFlag(){    
    return (bool)countdownFlag;
  }
};

class MenuCursor{
  private:
    uint8_t menuCursorPosition;
    uint8_t menuCursorPositionInsideMenu;
    bool menuEntryFlag;
    bool menuSetFlag;
    bool subMenuEntryFlag;
    bool subMenuSetFlag;
    bool insideMenu;

  public:
    MenuCursor():
    menuCursorPosition(0),
    menuCursorPositionInsideMenu(0),
    insideMenu(false),
    menuEntryFlag(false),
    subMenuEntryFlag(false){};

    void clear() {      
      menuCursorPosition = 0;
      menuCursorPositionInsideMenu = 0;
      menuEntryFlag = false;
      insideMenu = false;      
      subMenuEntryFlag = false;
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

    void updateCursor(uint8_t keyPressed, bool insideMenu) {
      //3 = Down / 2 = Up / 1 = Menu      
      if (keyPressed == 1) {        
        if(!insideMenu) {
          if(menuEntryFlag) {
          menuEntryFlag = false;
        } else {
          menuEntryFlag = true;
          }
        } else {
          if(subMenuEntryFlag) {
          subMenuEntryFlag = false;
        } else {
          subMenuEntryFlag = true;
          }
        }
      }  
      if(keyPressed == 2) {
        if(!insideMenu) {
          menuCursorPosition++;
        } else {
          menuCursorPositionInsideMenu++;
        }
      }  
      if(keyPressed == 3 && menuCursorPosition > 0) {
        if(!insideMenu) {
          menuCursorPosition--;
        } else {
          menuCursorPositionInsideMenu--;
        }      
      }
    }

    uint8_t getMenuCursorPosition() {
      return menuCursorPosition;
    }

    void setMenuCursorPosition(uint8_t newPosition){
      menuCursorPosition = newPosition;
    } 

    uint8_t getCursorPositionInsideMenu(){
      return menuCursorPositionInsideMenu;
    }

    uint8_t setCursorPositionInsideMenu(uint8_t newPosition){
      menuCursorPositionInsideMenu = newPosition;
    }

    bool getMenuFlag() {
      return menuEntryFlag;
    }

    void clearMenuFlag(){
      menuEntryFlag = false;
    }

    bool getSubMenuFlag(){  
      return subMenuEntryFlag;
    }

    bool clearSubMenuFlag(){  
      subMenuEntryFlag = false;
    }

};

class ChargeCycle  {
  private:
    short currentTimeHours;
    byte currentCycle;
    unsigned long offsetMillis;
    uint64_t countdownOffsetMillis;
    uint64_t countdownMaxTime;
    bool countdownFlag;
    uint64_t countdownValue;
    uint64_t eqCountdownValue;

  public:
    ChargeCycle():currentCycle(1), offsetMillis(0), currentTimeHours(0),countdownValue(0),eqCountdownValue(0){};    

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

    void countdownBegin(uint64_t newCountdownTime){
      countdownOffsetMillis = millis();
      countdownMaxTime = newCountdownTime / 1000;
      countdownFlag = false;
    }

    void countdownReset(){
      countdownOffsetMillis = millis();
    }

    String getCountdownTime(){      
      int64_t currentTimeSeconds = (millis() - countdownOffsetMillis ) / 1000;
      int64_t currentCountdownTime = countdownMaxTime - currentTimeSeconds;
      uint16_t currentTimeMinutes = 0;
      uint16_t currentTimeHoursFormated = 0;
      uint16_t currentTimeSecondsFormated = 0;
      uint16_t currentTimeMinutesFormated = 0;
      bool hasTimeLeft = true;
      //Early return - Timeout
      if(currentCountdownTime <= 0){
        countdownFlag = true;
        return timeOut;
      }
      
      //Seconds processing
      while (hasTimeLeft) {
        if ((currentCountdownTime / 60) >= 1) {
          currentCountdownTime -= 60;
          ++currentTimeMinutes;
        } else {
          hasTimeLeft = false;
          currentTimeSecondsFormated = currentCountdownTime;
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
      char stringBuffer[20];
      sprintf(stringBuffer, "%02d:%02d:%02d", currentTimeHoursFormated, currentTimeMinutesFormated, currentCountdownTime);
      return stringBuffer;
    }

    void BeginCountdownCfg(){
      countdownValue = EEPROM.get(countdownValueADDR, countdownValue);
      delay(10);
      if (isnan(countdownValue) || countdownValue <= 0 || countdownValue >= OVF) {      
        setCountdownTimeCfg(5000);//5s
      }

      eqCountdownValue = EEPROM.get(eqCountdownValueADDR, eqCountdownValue);
      delay(10);
      if (isnan(eqCountdownValue) || eqCountdownValue <= 0 || eqCountdownValue > 500000000) {      
        setEqCountdownTimeCfg(3600000);//1Hr
      }

    }

    void setCountdownTimeCfg(uint64_t newDelay){
      countdownValue = newDelay;
      EEPROM.put(countdownValueADDR, countdownValue);
    }

    uint64_t getCountdownTimeCfg() {
      return countdownValue;
    }

    void setEqCountdownTimeCfg(uint64_t newDelay){
      eqCountdownValue = newDelay;
      EEPROM.put(eqCountdownValueADDR, eqCountdownValue);
    }

    uint64_t getEqCountdownTimeCfg() {
      return eqCountdownValue;
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
        Resistor1 = 198000.00;
      }

      Resistor2 = EEPROM.get(R2ADDR, Resistor2);
      delay(10);
      if (isnan(Resistor2) || Resistor2 <= 0 || Resistor2 > OVF) {
        Resistor2 = 4550.00;
    }

      adcFix = EEPROM.get(adcFixADDR, adcFix);
      delay(10);
      if (isnan(adcFix) || adcFix <= 0 || adcFix > OVF) {
        adcFix = 0.005;
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
SystemMode systemMode;
//TODO: Eeprom first read is hard coded, Change ?
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
  //Display Template
  display.clearDisplay(); //Clear display data
  display.drawBitmap(( display.width()  - LOGO_WIDTH ) / 2, (display.height() - LOGO_HEIGHT) / 2, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  display.setTextColor(SSD1306_WHITE);
  delay(100);
}
//
void loop() { 
  //Correct init of objects for use with arduino abstractions
  adc.Begin();
  battery.Begin();
  systemMode.Begin();
  cycle.BeginCountdownCfg();

  digitalWrite(outputRelay, LOW);
  digitalWrite(outputExtra, LOW);
  //Serial.begin(115200);

  //"Global" utils
  unsigned long previousMillis = 0;
  bool countdownFlagValue = systemMode.getCountdownFlag();
  String lastTime;
  //System Lock
  while (true) { //Power On Routine -- TODO: change to State Machina -> switch case?
    //Button state checker - menu entry
    if (menuCursor.readPress(10) == 1) {
      cursorDelayTime++;
    } else {
      cursorDelayTime = 0;
    }
    if (cursorDelayTime > cursorDelayTimeMenuEntry) {      
      cursorDelayTime = 0; //set cursor verifier to 0
      //chargeStatus = 0; //change if needed
      menuConfig(); //Enter Menu
    }

    //System - Startup
    while (countdownFlagValue) {             
      byte systemStatus = 1; //Start System on Status - 0
      cycle.countdownBegin(cycle.getCountdownTimeCfg());
      //System - locked
      while(systemStatus == 1) {
        //Button state checker - menu entry
        if (menuCursor.readPress(10) == 1) {
          cursorDelayTime++;
        } else {
          cursorDelayTime = 0;
        }
        if (cursorDelayTime > cursorDelayTimeMenuEntry) {
          cursorDelayTime = 0;
          systemStatus = 1;
          menuConfig();
        }
        //Led system on blink
        !digitalRead(LED_BUILTIN) ? digitalWrite(LED_BUILTIN, HIGH) : digitalWrite(LED_BUILTIN, LOW) ;
        //Voltage reading and battery setting
        battery.setVoltage(adc.getReading(8000));
        //Display routine
        display.drawBitmap(( display.width()  - LOGO_WIDTH ) / 2, (display.height() - LOGO_HEIGHT) / 2, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
        display.setTextColor(SSD1306_WHITE);
        display.fillRect(34, 0, 80, 32, SSD1306_BLACK);        
        display.setTextSize(2);
        display.setCursor(52, 1);
        display.println(battery.getVoltage(), 2);
        /*
        //Send Json
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= 200) {
          previousMillis = currentMillis;
          sendSerialJson(battery.getVoltage(), cycle.getCurrentTimeFormated(), cycle.getCurrentCycle(), F("Em Carga"));
          delay(50);
        }
        */
        //Battery Discharge checker
        if (battery.getVoltage() > (battery.getStartVoltage() * 0.5)) {
          //Let countdownTime go freely  
          display.setTextSize(1); 
          display.setCursor(52, 16);
          display.println(F("Inicio em:"));
          display.setCursor(52, 24);
          display.println(cycle.getCountdownTime());
          display.display();
        } else {
          //Reset if conditions are not met, countdownTime = countdownBegin          
          display.setTextSize(1);
          display.setCursor(52, 16);             
          display.println(F("erro bateria"));
          display.setCursor(52, 24);             
          display.println(F("Verifique!"));
          display.display();
          cycle.countdownReset();
        }
        if(cycle.getCountdownTime() == timeOut) {
          systemStatus = 2;
          display.clearDisplay();
        }
      }
    //System Start
      countdownFlagValue = false;
      cycle.setOffsetMillis(millis());
    }

    //System Mode - Auto
    while (systemMode.getCurrentMode() == 1) {      // On Charge Routine      
      byte systemStatus = 1; //Start System on Status - 0
      //Charger - Charge
      cycle.countdownBegin(cycle.getEqCountdownTimeCfg());
      bool eqFlag = false;
      digitalWrite(outputRelay, LOW);
      digitalWrite(outputExtra, HIGH);
      while(systemStatus == 1) {
        //Button state checker - menu entry
        if (menuCursor.readPress(10) == 1) {
          cursorDelayTime++;
        } else {
          cursorDelayTime = 0;
        }
        if (cursorDelayTime > cursorDelayTimeMenuEntry) {
          cursorDelayTime = 0;
          systemStatus = 1;
          menuConfig();
        }
        //Led system on blink
        !digitalRead(LED_BUILTIN) ? digitalWrite(LED_BUILTIN, HIGH) : digitalWrite(LED_BUILTIN, LOW) ;
        //Voltage reading and battery setting
        battery.setVoltage(adc.getReading(8000));
        //Display routine
        display.drawBitmap(( display.width()  - LOGO_WIDTH ) / 2, (display.height() - LOGO_HEIGHT) / 2, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
        display.setTextColor(SSD1306_WHITE);
        display.fillRect(34, 0, 80, 32, SSD1306_BLACK);
        display.setTextSize(2);
        display.setCursor(52, 1);
        display.println(battery.getVoltage(), 2);
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
        // if (battery.getEndVoltage() < battery.getVoltage() || cycle.getCurrentTimeHours() == battery.getMaxChargeTime ()) {
        //   lastTime = cycle.getCurrentTimeFormated();
        //   systemStatus = 2;
        //   display.clearDisplay();
        //   previousMillis = millis();
        // } 

        //Charge complete checker
        if(battery.getEndVoltage() < battery.getVoltage()) {                    
          eqFlag = true;
        }
        //Disable Equalization only if voltage goes below set point
        if(battery.getStartVoltage() > battery.getVoltage()) {
          eqFlag = false;
        }        
        if(eqFlag){
          display.setTextSize(1); 
          display.setCursor(52, 16);
          display.println(F("Equalizar:"));
          display.setCursor(52, 24);
          display.println(cycle.getCountdownTime());
          display.display();
          //Led status blinker
          digitalRead(outputExtra) ? digitalWrite(outputExtra, LOW) : digitalWrite(outputExtra, HIGH);
        } else {
          //Reset if conditions are not met, countdownTime = countdownBegin  
          display.setTextSize(1);
          display.setCursor(52, 16);
          display.println(F("Em carga:"));
          display.setCursor(52, 24);
          display.println(cycle.getCurrentTimeFormated());
          display.display();
          cycle.countdownReset();
          //Set Output default state
          digitalWrite(outputExtra, HIGH);
        }
        //Charge max hours and equalization countdown checker
        if(cycle.getCountdownTime() == timeOut || cycle.getCurrentTimeHours() == battery.getMaxChargeTime()) {
          lastTime = cycle.getCurrentTimeFormated();
          systemStatus = 2;
          display.clearDisplay();
          previousMillis = millis();
        }

      }
    
      //Charger - Complete
      while (systemStatus == 2) {      // Charge Complete Routine
        digitalWrite(outputRelay, HIGH);
        digitalWrite(outputExtra, LOW);
        //Button state checker - menu entry
        if (menuCursor.readPress(10) == 1) {
          cursorDelayTime++;
        } else {
          cursorDelayTime = 0;
        }
        if (cursorDelayTime > cursorDelayTimeMenuEntry) {
          cursorDelayTime = 0;
          systemStatus = 1;
          menuConfig();
        }
        
        battery.setVoltage(adc.getReading(4000));
        //Display Routine
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(1, 1);
        display.print(F("Completa: ciclo "));
        display.print(cycle.getCurrentCycle());
        display.setTextSize(2);
        display.setCursor(1, 15);
        display.println(lastTime);
        display.display();

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
        if (battery.getVoltage() < battery.getStartVoltage()) { 
          cycle.addCurrentCycle();
          systemStatus = 1;
          display.clearDisplay();
          cycle.setOffsetMillis(millis());
        }
      }
    }

    //System Mode - Bloq
    while (systemMode.getCurrentMode() == 2) {      // Use Bloq Routine      
      byte systemStatus = 1; //Start System on Status - 0
      cycle.countdownBegin(cycle.getCountdownTimeCfg());
      //Bloq - Unlocked
      while(systemStatus == 1) {
        digitalWrite(outputRelay, LOW);
        digitalWrite(outputExtra, HIGH);
        //Button state checker - menu entry
        if (menuCursor.readPress(10) == 1) {
          cursorDelayTime++;
        } else {
          cursorDelayTime = 0;
        }
        if (cursorDelayTime > cursorDelayTimeMenuEntry) {
          cursorDelayTime = 0;
          systemStatus = 1;
          menuConfig();
        }
        //Led system on blink
        !digitalRead(LED_BUILTIN) ? digitalWrite(LED_BUILTIN, HIGH) : digitalWrite(LED_BUILTIN, LOW) ;
        //Voltage reading and battery setting
        battery.setVoltage(adc.getReading(8000));
        //Display routine
        display.drawBitmap(( display.width()  - LOGO_WIDTH ) / 2, (display.height() - LOGO_HEIGHT) / 2, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
        display.setTextColor(SSD1306_WHITE);
        display.fillRect(34, 0, 80, 32, SSD1306_BLACK);        
        display.setTextSize(2);
        display.setCursor(52, 1);
        display.println(battery.getVoltage(), 2);
        /*
        //Send Json
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= 200) {
          previousMillis = currentMillis;
          sendSerialJson(battery.getVoltage(), cycle.getCurrentTimeFormated(), cycle.getCurrentCycle(), F("Em Carga"));
          delay(50);
        }
        */
        //Battery Discharge checker
        if (battery.getEndVoltage() > battery.getVoltage()) {
          //Let countdownTime go freely  
          display.setTextSize(1); 
          display.setCursor(52, 16);
          display.println(F("Ativar em:"));
          display.setCursor(52, 24);
          display.println(cycle.getCountdownTime());
          display.display();
        } else {
          //Reset if conditions are not met, countdownTime = countdownBegin          
          display.setCursor(52, 16);             
          display.println(F("livre"));
          display.display();
          cycle.countdownReset();
        }
        if(cycle.getCountdownTime() == timeOut) {
          lastTime = cycle.getCurrentTimeFormated();
          systemStatus = 2;
          display.clearDisplay();
          previousMillis = millis();
        }
      }
    
      //Bloq - Locked
      while (systemStatus == 2) {      // Charge Complete Routine 
        digitalWrite(outputRelay, HIGH);
        digitalWrite(outputExtra, LOW);
        //Button state checker - menu entry
        if (menuCursor.readPress(10) == 1) {
          cursorDelayTime++;
        } else {
          cursorDelayTime = 0;
        }
        if (cursorDelayTime > cursorDelayTimeMenuEntry) {
          cursorDelayTime = 0;
          systemStatus = 1;
          menuConfig();
        }
        
        battery.setVoltage(adc.getReading(4000));
        //Display Routine
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(1, 1);
        display.print(F("T. em uso: ciclo "));
        display.print(cycle.getCurrentCycle());
        display.setTextSize(2);
        display.setCursor(1, 15);
        display.println(lastTime);
        display.display();

        /*
        //Send Json
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= 200) {
          previousMillis = currentMillis;
          sendSerialJson(battery.getVoltage(), lastTime, cycle.getCurrentCycle(), F("Completa"));
          delay(50);
        }
        */
        //Battery charged checker
        if (battery.getVoltage() > battery.getStartVoltage()) { 
          cycle.addCurrentCycle();
          systemStatus = 1;
          display.clearDisplay();
          cycle.setOffsetMillis(millis());
        }
      }
    }
  }
   
}

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
//Parameter 1 set a display.clearDisplay() command - boolean
//Parameter 2 set the X screen position cursor - byte
//Parameter 3 set the Y screen position cursor - byte
//Parameter 4 invert the font color - boolean -> white on background and black on font
//Parameter 5 set de font size -> byte
void displayCall(
  bool clearDisplay = false,
  uint8_t x = 1,
  uint8_t y = 1,
  bool invertPixels = false,
  uint8_t size = 1
     ) {
  //Color definition, default = white font and black screen
  uint8_t colorOnPixels = SSD1306_WHITE;
  uint8_t colorOffPixels = SSD1306_BLACK;
  if(invertPixels) {
    colorOnPixels = SSD1306_BLACK;
    colorOffPixels = SSD1306_WHITE;
  }
  if(clearDisplay) {
    display.clearDisplay();
  }
  display.setTextSize(size);
  display.setTextColor(colorOnPixels,colorOffPixels);
  display.setCursor(x,y);  
}

void menuConfig(){
  uint8_t menuPage = 0;
  display.clearDisplay();
  display.display();
  delay(1000); //make small delay for the user to release the Enter button
  while (true) {
    //Page 1
    while (menuPage == 0) {
      //Menu display - necessary here for correct exhibition after the hover animation
      displayCall(true, 1, 1);
      display.print(menuOptions[0]);
      displayCall(false, 1, 12);
      display.print(menuOptions[1]);
      displayCall(false, 1, 22); 
      display.print(menuOptions[2]);     
      displayCall(false, 115, 12);
      display.print(menuCursor.getMenuCursorPosition());
      //Button state checker
      menuCursor.updateCursor(menuCursor.readPress(20), false);
      //Menu Selection
      switch (menuCursor.getMenuCursorPosition()) {
        case 0: //Exit CFG menu
          //Hover animation          
          displayCall(false, 1, 1, true);
          display.print(menuOptions[0]);
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
          displayCall(false, 1, 12, true);
          display.print(menuOptions[1]);
          display.display();
          //Enter Menu
          while (menuCursor.getMenuFlag()) {
            displayCall(true, 1, 1, false);
            display.print(menuOptions[11]);
            display.print(battery.getStartVoltage());
            display.display();
            //Button state checker
            if (menuCursor.readPress(30) == 1) {
              menuCursor.clearMenuFlag();
              display.clearDisplay();
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
          displayCall(false, 1, 22, true);
          display.print(menuOptions[2]);
          display.display();             
          //Enter Menu
          while (menuCursor.getMenuFlag()) {
            displayCall(true, 1, 1, false);
            display.print(menuOptions[11]);
            display.print(battery.getEndVoltage());
            display.display();
            //Button state checker
            if (menuCursor.readPress(30) == 1) {
              menuCursor.clearMenuFlag();
              display.clearDisplay();
            }                 
            //Up - set
            if (menuCursor.readPress(10) == 2) {
              battery.setEndVoltage(battery.getEndVoltage() + 0.05);                  
            }                
            //Down - set
            if (menuCursor.readPress(10) == 3) {
              battery.setEndVoltage(battery.getEndVoltage() - 0.05);                  
            }
          }              
          break;

        case 3:
          menuPage = 1;
          break;

        default:
          break;
      }      
    }
    //Page 2
    while (menuPage == 1) {
      //Menu display
      displayCall(true, 1, 1);
      display.print(menuOptions[3]);
      displayCall(false, 1, 12);
      display.print(menuOptions[4]);
      displayCall(false, 1, 22); 
      display.print(menuOptions[5]);     
      displayCall(false, 115, 12);
      display.print(menuCursor.getMenuCursorPosition());
      //Button Checker
      menuCursor.updateCursor(menuCursor.readPress(20), false);
      //Menu Selection
      switch (menuCursor.getMenuCursorPosition()) {
        case 2:
          menuPage = 0;
          break;

        case 3: //Set charging maximum time Value
          //Hover animation               
          displayCall(false, 1, 1, true);
          display.print(menuOptions[3]);
          display.display();
          //Enter Menu
          while (menuCursor.getMenuFlag()) {
            displayCall(true, 1, 1, false);
            display.print(menuOptions[12]);
            display.print(battery.getMaxChargeTime());
            display.display();
            //Button state checker
            if (menuCursor.readPress(30) == 1) {
              menuCursor.clearMenuFlag();
              display.clearDisplay();
            }                 
            //Up - set
            if (menuCursor.readPress(10) == 2) {
              battery.setMaxChargeTime(battery.getMaxChargeTime() + 1);                  
            }                
            //Down - set
            if (menuCursor.readPress(10) == 3) {
              battery.setMaxChargeTime(battery.getMaxChargeTime() - 1);                  
            }
          }
          break;

        case 4: //Set Adc fix Value
          //Hover animation     
          displayCall(false, 1, 12, true);
          display.print(menuOptions[4]);
          display.display();
          //Enter Menu
          while (menuCursor.getMenuFlag()) {
            displayCall(true, 1, 1, false);  //Animation remove
            display.print(menuOptions[13]);
            display.setCursor(1, 22);
            display.print(adc.getAdcFix() * 1000);            
            display.setCursor(28, 22);
            display.print(adc.getReading(800), 2);
            display.display();
            //Button state checker
            if (menuCursor.readPress(30) == 1) {
              menuCursor.clearMenuFlag();
              display.clearDisplay();
            }                 
            //Up - set
            if (menuCursor.readPress(10) == 2) {
              adc.setAdcFix(adc.getAdcFix() + 0.001);                  
            }                
            //Down - set
            if (menuCursor.readPress(10) == 3) {
              adc.setAdcFix(adc.getAdcFix() - 0.001);                  
            }
          }
          break;

        case 5: //Set Adc R1 Value
          //Hover animation
               
          displayCall(false, 1, 22, true);
          display.print(menuOptions[5]);
          display.display(); 
          //Enter Menu
          while (menuCursor.getMenuFlag()) {
            displayCall(true, 1, 1, false);            
            display.print(menuOptions[11]);
            display.print(adc.getR1());            
            display.setCursor(28, 22);
            display.print(adc.getReading(800), 2);
            display.display();
            //Button state checker
            if (menuCursor.readPress(30) == 1) {
              menuCursor.clearMenuFlag();
              display.clearDisplay();
            }                 
            //Up - set
            if (menuCursor.readPress(10) == 2) {
              adc.setR1(adc.getR1() + 100);                  
            }                
            //Down - set
            if (menuCursor.readPress(10) == 3) {
              adc.setR1(adc.getR1() - 100);                  
            }
          }
          break;

        case 6:
          menuPage = 2;
          break;

        default:
          break;
      }
    }
    //Page 3
    while (menuPage == 2) {
      //Menu display
      displayCall(true, 1, 1);
      display.print(menuOptions[6]);
      displayCall(false, 1, 12);
      display.print(menuOptions[7]);       
      displayCall(false, 1, 22);
      display.print(menuOptions[14]);   
      displayCall(false, 115, 12);
      display.print(menuCursor.getMenuCursorPosition());
      //Button Checker
      menuCursor.updateCursor(menuCursor.readPress(20), false);
      //Menu Selection
      switch (menuCursor.getMenuCursorPosition()) {
        case 5:
          menuPage = 1;
          break;

        case 6: //Set Adc R2 Value
          //Hover animation               
          displayCall(false, 1, 1, true);
          display.print(menuOptions[6]);
          display.display();
          //Enter Menu
          while (menuCursor.getMenuFlag()) {                
            displayCall(true, 1, 1, false);
            display.print(menuOptions[11]);
            display.print(adc.getR2());            
            display.setCursor(28, 22);
            display.print(adc.getReading(800), 2);
            display.display();
            //Button state checker
            if (menuCursor.readPress(30) == 1) {
              menuCursor.clearMenuFlag();
              display.clearDisplay();
            }                 
            //Up - set
            if (menuCursor.readPress(10) == 2) {
              adc.setR2(adc.getR2() + 50);                  
            }                
            //Down - set
            if (menuCursor.readPress(10) == 3) {
              adc.setR2(adc.getR2() - 50);                  
            }
          }
          break;

        case 7: //Set pre defined values for 24v - 36v - 48v
          //Hover animation               
          displayCall(false, 1, 12, true);
          display.print(menuOptions[7]);
          display.display();
          //Enter Menu
          while (menuCursor.getMenuFlag()) {
            menuCursor.updateCursor(menuCursor.readPress(20), true);            
            displayCall(true, 1, 1);
            display.print(menuOptions[8]);
            displayCall(false, 1, 12);
            display.print(menuOptions[9]);
            displayCall(false, 1, 22);
            display.print(menuOptions[10]);
            //Button state checker
            switch (menuCursor.getCursorPositionInsideMenu()) {
              case 0: //Set pre defined 24v
                //Hover animation
                displayCall(false, 1, 1, true);
                display.print(menuOptions[8]);
                display.display();
                //Enter Menu
                while (menuCursor.getSubMenuFlag()) {
                  //Set 24V
                  battery.setStartVoltage(23.6);
                  battery.setEndVoltage(29.5);
                  battery.setMaxChargeTime(10);

                  menuCursor.clear();
                  menuPage = 0;
                }
                break;

              case 1: //Set pre defined 36v
                //Hover animation
                displayCall(false, 1, 12, true);
                display.print(menuOptions[9]);
                display.display();
                //Enter Menu
                while (menuCursor.getSubMenuFlag()) {
                  //Set 36V
                  battery.setStartVoltage(35.4);
                  battery.setEndVoltage(44.1);
                  battery.setMaxChargeTime(10);

                  menuCursor.clear();
                  menuPage = 0;
                }
                break;

              case 2: //Set pre defined 48v
                //Hover animation
                displayCall(false, 1, 22, true);
                display.print(menuOptions[10]);
                display.display();
                //Enter Menu
                while (menuCursor.getSubMenuFlag()) {
                  //Set 48V
                  battery.setStartVoltage(47.2);
                  battery.setEndVoltage(59.5);
                  battery.setMaxChargeTime(10);

                  menuCursor.clear();
                  menuPage = 0;
                }
                break;

              case 3:
                menuCursor.clear();
                menuPage = 0;
                break;

              default:
                break;
            }
          }
          break;
        
        case 8://Set Operation Mode
          //Hover animation               
          displayCall(false, 1, 22, true);
          display.print(menuOptions[14]);
          display.display();
          //Enter Menu
          while (menuCursor.getMenuFlag()) {
            menuCursor.updateCursor(menuCursor.readPress(50), true);            
            displayCall(true, 1, 1);
            display.print(menuOptions[15]);
            displayCall(false, 1, 12);
            display.print(menuOptions[16]);
            displayCall(false, 1, 22);
            display.print(menuOptions[18]);
            //Button state checker
            switch (menuCursor.getCursorPositionInsideMenu()) {
              case 0: //Set Operation Mode - Auto
                //Hover animation
                displayCall(false, 1, 1, true);
                display.print(menuOptions[15]);
                display.display();
                //Enter Menu
                while (menuCursor.getSubMenuFlag()) {
                  //Set Auto
                  systemMode.setMode(1);
                  systemMode.setCountdownFlag(true);
                  menuCursor.clear();
                  menuPage = 0;
                }
                break;

              case 1: //Set Operation Mode - Bloq
                //Hover animation
                displayCall(false, 1, 12, true);
                display.print(menuOptions[16]);
                display.display();
                //Enter Menu
                while (menuCursor.getSubMenuFlag()) {
                  //Set Bloq
                  systemMode.setMode(2);                  
                  systemMode.setCountdownFlag(false);
                  menuCursor.clear();
                  menuPage = 0;
                }
                break;

              case 2: //Reserved
                //Hover animation
                displayCall(false, 1, 22, true);
                display.print(menuOptions[17]);
                display.display();
                //Enter Menu
                while (menuCursor.getSubMenuFlag()) {
                  //Set Reserved
                  //systemMode.setMode() ?
                  menuCursor.clear();
                  menuPage = 0;
                }
                break;

              case 3:
              menuCursor.clear();
              menuPage = 0;
              break;

              default:
              break;
            }
          }
        break;

        case 9:
          menuPage = 3;
          break;

        default:
          break;
      }
    }  
    //Page 4
    while (menuPage == 3) {
      //Menu display
      displayCall(true, 1, 1);
      display.print(menuOptions[17]);
      displayCall(false, 1, 12);
      display.print(menuOptions[20]);
      displayCall(false, 1, 22); 
      display.print(menuOptions[18]);     
      displayCall(false, 115, 12);
      display.print(menuCursor.getMenuCursorPosition());
      //Button Checker
      menuCursor.updateCursor(menuCursor.readPress(20), false);
      //Menu Selection
      switch (menuCursor.getMenuCursorPosition()) {
        case 8:
          menuPage = 2;
          break;

        case 9: //Set countdown Time
          //Hover animation               
          displayCall(false, 1, 1, true);
          display.print(menuOptions[17]);
          display.display();
          //Enter Menu
          while (menuCursor.getMenuFlag()) {
            displayCall(true, 1, 1, false);
            display.print(menuOptions[19]);
            display.print((cycle.getCountdownTimeCfg() / (double)1000));
            display.display();
            //Button state checker
            if (menuCursor.readPress(30) == 1) {
              menuCursor.clearMenuFlag();
              display.clearDisplay();
            }                 
            //Up - set
            if (menuCursor.readPress(10) == 2) {
              cycle.setCountdownTimeCfg(cycle.getCountdownTimeCfg() + 1000);                  
            }                
            //Down - set
            if (menuCursor.readPress(10) == 3) {
              cycle.setCountdownTimeCfg(cycle.getCountdownTimeCfg() - 1000);                  
            }
          }
          break;

        case 10: //Timer EQ set countdown       
          //Hover animation               
          displayCall(false, 1, 12, true);
          display.print(menuOptions[20]);
          display.display();
          //Enter Menu
          while (menuCursor.getMenuFlag()) {
            displayCall(true, 1, 12, false);
            display.print(menuOptions[21]);
            display.print((cycle.getEqCountdownTimeCfg() / (double)1000)/60);
            display.display();
            //Button state checker
            if (menuCursor.readPress(30) == 1) {
              menuCursor.clearMenuFlag();
              display.clearDisplay();
            }                 
            //Up - set
            if (menuCursor.readPress(10) == 2) {
              cycle.setEqCountdownTimeCfg(cycle.getEqCountdownTimeCfg() + 60000);                  
            }                
            //Down - set
            if (menuCursor.readPress(10) == 3) {
              cycle.setEqCountdownTimeCfg(cycle.getEqCountdownTimeCfg() - 60000);                  
            }
          }
          break;

        case 11: //Reserved
          //Hover animation               
          displayCall(false, 1, 22, true);
          display.print(menuOptions[18]);
          display.display(); 
          //Enter Menu
          while (menuCursor.getMenuFlag()) {
            //Reserved
            menuCursor.clear();
            menuPage = 0;
          }
          break;

        case 12:
          menuCursor.clear();
          menuPage = 0;
          break;

        default:
          break;
      }
    }    
  }
}
//
