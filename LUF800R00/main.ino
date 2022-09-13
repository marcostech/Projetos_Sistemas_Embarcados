
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
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

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

//----- Pre - Defined ADC CFG ----
#define ADC_CH  A1
const float ADC_FIX = 0.002;
const float END_VOLTAGE = 14.70;
const float START_VOLTAGE = 11.80;
const byte MAX_CHARGE_TIME = 10;
//-----------------------
//Digital CFG
#define buttonCursorUp 7
#define buttonCursorDown 8
#define buttonEnter 9
uint8_t cursorDelayTime = 0; //cursors debounce delay
uint8_t cursorDelayTimeMenuEntry = 5; //"delay" to enter cfg menu
boolean  menuEnabled = false;
//
//EEPROM CFG address
int endVoltageADDR = 10;
int startVoltageADDR = 20;
int maxChargeTimeADDR = 30;
int R1ADDR = 40;
int R2ADDR = 50;
int adcFixADDR = 60;
//-------------
//function to check memory
extern unsigned int __bss_end;
extern void *__brkval;
int freeMemory() {
  int free_memory;
  if ((int) __brkval)
    return ((int) &free_memory) - ((int) __brkval);
  return ((int) &free_memory) - ((int) &__bss_end);
}
//-----------------------------------

void sendSerialJson(float batteryVoltage, String cycleTime, int cycleCurrent, String cycleStatus) {
  StaticJsonDocument<96> doc;
  char bufferArr[6];
  dtostrf(batteryVoltage, 4, 2, bufferArr);
  doc["batteryVoltage"] = bufferArr;
  doc["chargeTime"] = cycleTime;
  doc["currentCycle"] = cycleCurrent;
  doc["chargeStatus"] = cycleStatus;
  Serial.println();
  serializeJsonPretty(doc, Serial);
  Serial.flush();
}

class ChargeCycle  {
  private:
    short currentTimeHours;
    byte currentCycle;
    unsigned long  offsetMillis;

  public:
    ChargeCycle () {
      currentCycle = 1;
      offsetMillis = 0;
      currentTimeHours = 0;
    }

    String getCurrentTimeFormated () {
      unsigned long currentTimeSeconds = (millis() - offsetMillis ) / 1000;
      short currentTimeMinutes = 0;
      short currentTimeHoursFormated = 0;
      short currentTimeSecondsFormated = 0;
      short currentTimeMinutesFormated = 0;
      boolean hasTimeLeft = true;

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
      //String formater
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
    float voltage = 0.00f;
    float endVoltage;
    float startVoltage;
    byte maxChargeTime;

  public:
    Battery() {
     endVoltage = EEPROM.get(endVoltageADDR, endVoltage);
     delay(10);
     if(isnan(endVoltage)) {
          endVoltage = END_VOLTAGE;
      }

     startVoltage = EEPROM.get(startVoltageADDR, startVoltage);
     delay(10);
      if(isnan(startVoltage)) {
          startVoltage = START_VOLTAGE;
      }

     maxChargeTime = EEPROM.get(maxChargeTimeADDR, maxChargeTime);   
     delay(10);  
     if(isnan(maxChargeTime)) {
          maxChargeTime = MAX_CHARGE_TIME;
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
    float R1;
    float R2;
    float adcFix;
    byte adcChannel = A1;
    float aref = 0.00342 ;

  public:
    adConverter () {    
     R1 = EEPROM.get(R1ADDR, R1);
     delay(10);
     if(isnan(R1)) {
          R1 = 98400;
      }
     R2 = EEPROM.get(R2ADDR, R2);
     delay(10);
     if(isnan(R2)) {
          R2 = 4550;
      }
     adcFix = EEPROM.get(adcFixADDR, adcFix);
     delay(10);
     if(isnan(adcFix)) {
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
      adcRead = adcRead / (R2 / (R1 + R2)); // Vout / (R2/(R1+R2)) - currently -> 4550 / 102.950 = 0,04419
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
      R1 = R1Value;
      EEPROM.put(R1ADDR, R1);
    }

    float getR1 () {
      return R1 ;
    }

    void setR2 (float R2Value) {
      R2 = R2Value;
      EEPROM.put(R2ADDR, R2);
    }

    float getR2 () {
      return R2 ;
    }

    void setAdcChannel (byte NewAdcChannel) {
      adcChannel = NewAdcChannel;
    }

    byte getAdcChannel () {
      return adcChannel;
    }
};

//---------------------------------

void setup() {
  analogReference(EXTERNAL);
  //Wire.setClock(100000); //make i2c less sensitive
  pinMode (LED_BUILTIN, OUTPUT);
  pinMode(buttonCursorUp, INPUT_PULLUP);
  pinMode(buttonCursorDown, INPUT_PULLUP);
  pinMode(buttonEnter, INPUT_PULLUP);
  // -------- Display verifier
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    while (true) { //Lock program if display fail
      digitalWrite(LED_BUILTIN, HIGH);
      delay(300);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
  }
  // -------- Display Template
  display.clearDisplay(); //Clear display data
  display.drawBitmap(( display.width()  - LOGO_WIDTH ) / 2, (display.height() - LOGO_HEIGHT) / 2, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  display.setTextColor(SSD1306_WHITE);
  delay(100);
  // --------
}

void loop() {
  AdConverter adc;
  Battery battery;
  ChargeCycle cycle;
  Serial.begin(115200);
  unsigned long previousMillis = 0;
  byte chargeStatus = 0;
  String lastTime;
  while (true) { //Power On Routine -- TODO: change to State Machina -> switch case?
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
    //Menu Entry loop
    while (menuEnabled) {            // Menu CFG Routine -- TODO: change to State Machine -> switch case?
      uint8_t menuUse = 0;
      uint8_t menuCursorPosition = 0;
      uint8_t menuPage = 0;
      uint8_t cursorDelayTimeUp = 0;
      uint8_t cursorDelayTimeDown = 0;
      uint8_t cursorDelayTimeInterval = 1;
      uint8_t cursorDelayTimeIntervalMenu = 2;

      display.clearDisplay();
      display.display();
      delay(1000);
      while (menuEnabled) {
        //Page 1
        while (menuPage == 0) {
          //Menu display
          display.clearDisplay();
          display.setTextSize(1);
          display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
          display.setCursor(1, 1);
          display.print(F("Menu Prog .Sair"));
          display.setCursor(1, 12);
          display.print(F("1 - V de REcarga"));
          display.setCursor(1, 22);
          display.print(F("2 - V final de carga"));
          //Button state checker
          if (digitalRead(buttonCursorUp) == LOW) {
            cursorDelayTimeUp++;
          } else {
            cursorDelayTimeUp = 0;
          }
          if (cursorDelayTimeUp > cursorDelayTimeInterval) {
            menuCursorPosition++;
            cursorDelayTimeUp = 0;
          }

          if (digitalRead(buttonCursorDown) == LOW) {
            cursorDelayTimeDown++;
          } else {
            cursorDelayTimeDown = 0;
          }
          if (cursorDelayTimeDown > cursorDelayTimeInterval) {
            if (menuCursorPosition > 0) {
              menuCursorPosition--;
            } else {
              menuCursorPosition = 0;
            }
            cursorDelayTimeDown = 0;
          }
          //Menu Selection
          switch (menuCursorPosition) {
            case 0: //Set minimum pre-charge time Value
              //Hover animation
              if (menuCursorPosition == 0) {
                display.setCursor(1, 1);
                display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //Animation set
                display.print(F("Menu Prog .Sair"));
                display.display();
              }
              //--------------
              //Button state checker
              if (digitalRead(buttonEnter) == LOW) {
                cursorDelayTime++;
              } else {
                cursorDelayTime = 0;
              }
              if (cursorDelayTime > cursorDelayTimeIntervalMenu) {
                menuUse = 1;
                cursorDelayTime = 0;
              }
              //Enter Menu
              while (menuUse == 1) {
                display.clearDisplay();
                display.setCursor(1, 22);
                display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);  //Animation remove
                display.print(F("Dentro do Menu:"));
                display.print(menuCursorPosition);
                display.display();
                //Set value
                //Button state checker
                if (digitalRead(buttonEnter) == LOW) {
                  cursorDelayTime++;
                } else {
                  cursorDelayTime = 0;
                }
                if (cursorDelayTime > cursorDelayTimeIntervalMenu) {
                  menuUse = 0;
                  menuPage = 5; //number higher just to escape menu page loop
                  cursorDelayTime = 0;
                  menuEnabled = false;
                  //Exit function
                  display.clearDisplay();
                }
              }

              break;
            case 1: //Set charging start voltage Value
              //Hover animation
              if (menuCursorPosition == 1) {
                display.setCursor(1, 12);
                display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //Animation set
                display.print(F("1 - V de REcarga"));
                display.display();
              }
              //--------------
              //Button state checker
              if (digitalRead(buttonEnter) == LOW) {
                cursorDelayTime++;
              } else {
                cursorDelayTime = 0;
              }
              if (cursorDelayTime > cursorDelayTimeIntervalMenu) {
                menuUse = 1;
                cursorDelayTime = 0;
              }
              //Enter Menu
              while (menuUse == 1) {
                display.clearDisplay();
                display.setCursor(1, 1);
                display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);  //Animation remove
                display.print(F("Valor atual:"));
                display.print(battery.getStartVoltage());
                display.display();
                //Set value
                //Button state checker
                if (digitalRead(buttonEnter) == LOW) {
                  cursorDelayTime++;
                } else {
                  cursorDelayTime = 0;
                }
                if (cursorDelayTime > cursorDelayTimeIntervalMenu ) {
                  menuUse = 0;
                  menuPage = 0;
                  cursorDelayTime = 0;
                }

                //Button state checker
                if (digitalRead(buttonCursorUp) == LOW) {
                  cursorDelayTimeUp++;
                } else {
                  cursorDelayTimeUp = 0;
                }
                if (cursorDelayTimeUp > (cursorDelayTimeInterval + 2)) {
                  battery.setStartVoltage(battery.getStartVoltage() + 0.05);
                  cursorDelayTimeUp = 0;
                }

                if (digitalRead(buttonCursorDown) == LOW) {
                  cursorDelayTimeDown++;
                } else {
                  cursorDelayTimeDown = 0;
                }
                if (cursorDelayTimeDown > (cursorDelayTimeInterval + 2)) {
                  battery.setStartVoltage(battery.getStartVoltage() - 0.05);
                  cursorDelayTimeDown = 0;
                }
              }
              break;

            case 2: //Set charging end voltage Value
              //Hover animation
              if (menuCursorPosition == 2) {
                display.setCursor(1, 22);
                display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //Animation set
                display.print(F("2 - V final de carga"));
                display.display();
              }
              //--------------
              //Button state checker
              if (digitalRead(buttonEnter) == LOW) {
                cursorDelayTime++;
              } else {
                cursorDelayTime = 0;
              }
              if (cursorDelayTime > cursorDelayTimeIntervalMenu) {
                menuUse = 1;
                cursorDelayTime = 0;
              }
              //Enter Menu
              while (menuUse == 1) {
                display.clearDisplay();
                display.setCursor(1, 1);
                display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);  //Animation remove
                display.print(F("Valor Atual:"));
                display.print(battery.getEndVoltage());
                display.display();
                //Set value
                //Button state checker
                if (digitalRead(buttonEnter) == LOW) {
                  cursorDelayTime++;
                } else {
                  cursorDelayTime = 0;
                }
                if (cursorDelayTime > cursorDelayTimeIntervalMenu) {
                  menuUse = 0;
                  menuPage = 0;
                  cursorDelayTime = 0;
                }

                //Button state checker
                if (digitalRead(buttonCursorUp) == LOW) {
                  cursorDelayTimeUp++;
                } else {
                  cursorDelayTimeUp = 0;
                }
                if (cursorDelayTimeUp > (cursorDelayTimeInterval + 2)) {
                  battery.setEndVoltage(battery.getEndVoltage() + 0.05);
                  cursorDelayTimeUp = 0;
                }

                if (digitalRead(buttonCursorDown) == LOW) {
                  cursorDelayTimeDown++;
                } else {
                  cursorDelayTimeDown = 0;
                }
                if (cursorDelayTimeDown > (cursorDelayTimeInterval + 2)) {
                  battery.setEndVoltage(battery.getEndVoltage() - 0.05);
                  cursorDelayTimeDown = 0;
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
          //Button state checker
          if (digitalRead(buttonCursorUp) == LOW) {
            cursorDelayTimeUp++;
          } else {
            cursorDelayTimeUp = 0;
          }
          if (cursorDelayTimeUp > cursorDelayTimeInterval) {
            menuCursorPosition++;
            cursorDelayTimeUp = 0;
          }

          if (digitalRead(buttonCursorDown) == LOW) {
            cursorDelayTimeDown++;
          } else {
            cursorDelayTimeDown = 0;
          }
          if (cursorDelayTimeDown > cursorDelayTimeInterval) {
            if (menuCursorPosition > 0) {
              menuCursorPosition--;
            } else {
              menuCursorPosition = 0;
            }
            cursorDelayTimeDown = 0;
          }
          //------------
          //Menu display
          display.clearDisplay();
          display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
          display.setCursor(1, 1);
          display.print(F("3 - T maximo de carga"));
          display.setCursor(1, 12);
          display.print(F("4 - Corrigir ADC"));
          display.setCursor(1, 22);
          display.print(F("5 - Setar R1"));
          //----------

          switch (menuCursorPosition) {
            case 2:
              menuPage = 0;
              break;
            case 3: //Set charging maximum time Value
              //Hover animation
              if (menuCursorPosition == 3) {
                display.setCursor(1, 1);
                display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //Animation set
                display.print(F("3 - T maximo de carga"));
                display.display();
              }
              //--------------
              //Button state checker
              if (digitalRead(buttonEnter) == LOW) {
                cursorDelayTime++;
              } else {
                cursorDelayTime = 0;
              }
              if (cursorDelayTime > cursorDelayTimeIntervalMenu) {
                menuUse = 1;
                cursorDelayTime = 0;
              }
              //Enter Menu
              while (menuUse == 1) {
                display.clearDisplay();
                display.setCursor(1, 1);
                display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);  //Animation remove
                display.print(F("Valor atual em Horas:"));
                display.print(battery.getMaxChargeTime());
                display.display();
                //Set value
                //Button state checker
                if (digitalRead(buttonEnter) == LOW) {
                  cursorDelayTime++;
                } else {
                  cursorDelayTime = 0;
                }
                if (cursorDelayTime > cursorDelayTimeIntervalMenu ) {
                  menuUse = 0;
                  menuPage = 1;
                  cursorDelayTime = 0;
                }

                //Button state checker
                if (digitalRead(buttonCursorUp) == LOW) {
                  cursorDelayTimeUp++;
                } else {
                  cursorDelayTimeUp = 0;
                }
                if (cursorDelayTimeUp > (cursorDelayTimeInterval + 2)) {
                  battery.setMaxChargeTime(battery.getMaxChargeTime() + 1);
                  cursorDelayTimeUp = 0;
                }

                if (digitalRead(buttonCursorDown) == LOW) {
                  cursorDelayTimeDown++;
                } else {
                  cursorDelayTimeDown = 0;
                }
                if (cursorDelayTimeDown > (cursorDelayTimeInterval + 2)) {
                  battery.setMaxChargeTime(battery.getMaxChargeTime() - 1);
                  cursorDelayTimeDown = 0;
                }
              }
              break;

            case 4: //Set Adc fix Value
              //Hover animation
              if (menuCursorPosition == 4) {
                display.setCursor(1, 12);
                display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //Animation set
                display.print(F("4 - Corrigir ADC"));
                display.display();
              }
              //--------------
              //Button state checker
              if (digitalRead(buttonEnter) == LOW) {
                cursorDelayTime++;
              } else {
                cursorDelayTime = 0;
              }
              if (cursorDelayTime > cursorDelayTimeIntervalMenu) {
                menuUse = 1;
                cursorDelayTime = 0;
              }
              //Enter Menu
              while (menuUse == 1) {
                display.clearDisplay();
                display.setCursor(1, 1);
                display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);  //Animation remove
                display.print(F("Valor em % multiplicado por 1000:"));
                display.setCursor(1, 22);
                display.print(adc.getAdcFix() * 1000);
                display.display();
                //Set value
                //Button state checker
                if (digitalRead(buttonEnter) == LOW) {
                  cursorDelayTime++;
                } else {
                  cursorDelayTime = 0;
                }
                if (cursorDelayTime > cursorDelayTimeIntervalMenu ) {
                  menuUse = 0;
                  menuPage = 1;
                  cursorDelayTime = 0;
                }

                //Button state checker
                if (digitalRead(buttonCursorUp) == LOW) {
                  cursorDelayTimeUp++;
                } else {
                  cursorDelayTimeUp = 0;
                }
                if (cursorDelayTimeUp > (cursorDelayTimeInterval + 2)) {
                  adc.setAdcFix(adc.getAdcFix() + 0.001);
                  cursorDelayTimeUp = 0;
                }

                if (digitalRead(buttonCursorDown) == LOW) {
                  cursorDelayTimeDown++;
                } else {
                  cursorDelayTimeDown = 0;
                }
                if (cursorDelayTimeDown > (cursorDelayTimeInterval + 2)) {
                  adc.setAdcFix(adc.getAdcFix() - 0.001);
                  cursorDelayTimeDown = 0;
                }
              }
              break;

            case 5: //Set Adc R1 Value
              //Hover animation
              if (menuCursorPosition == 5) {
                display.setCursor(1, 22);
                display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //Animation set
                display.print(F("5 - Setar R1"));
                display.display();
              }
              //--------------
              //Button state checker
              if (digitalRead(buttonEnter) == LOW) {
                cursorDelayTime++;
              } else {
                cursorDelayTime = 0;
              }
              if (cursorDelayTime > cursorDelayTimeInterval) {
                menuUse = 1;
                cursorDelayTime = 0;
              }
              //Enter Menu
              while (menuUse == 1) {
                display.clearDisplay();
                display.setCursor(1, 1);
                display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);  //Animation remove
                display.print(F("Valor Atual:"));
                display.print(adc.getR1());
                display.display();
                //Set value
                //Button state checker
                if (digitalRead(buttonEnter) == LOW) {
                  cursorDelayTime++;
                } else {
                  cursorDelayTime = 0;
                }
                if (cursorDelayTime > cursorDelayTimeIntervalMenu) {
                  menuUse = 0;
                  menuPage = 1;
                  cursorDelayTime = 0;
                }

                //Button state checker
                if (digitalRead(buttonCursorUp) == LOW) {
                  cursorDelayTimeUp++;
                } else {
                  cursorDelayTimeUp = 0;
                }
                if (cursorDelayTimeUp > (cursorDelayTimeInterval + 2)) {
                  adc.setR1(adc.getR1() + 100);
                  cursorDelayTimeUp = 0;
                }

                if (digitalRead(buttonCursorDown) == LOW) {
                  cursorDelayTimeDown++;
                } else {
                  cursorDelayTimeDown = 0;
                }
                if (cursorDelayTimeDown > (cursorDelayTimeInterval + 2)) {
                  adc.setR1(adc.getR1() - 100);
                  cursorDelayTimeDown = 0;
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
          //Button state checker
          if (digitalRead(buttonCursorUp) == LOW) {
            cursorDelayTimeUp++;
          } else {
            cursorDelayTimeUp = 0;
          }
          if (cursorDelayTimeUp > cursorDelayTimeInterval) {
            menuCursorPosition++;
            cursorDelayTimeUp = 0;
          }

          if (digitalRead(buttonCursorDown) == LOW) {
            cursorDelayTimeDown++;
          } else {
            cursorDelayTimeDown = 0;
          }
          if (cursorDelayTimeDown > cursorDelayTimeInterval) {
            if (menuCursorPosition > 0) {
              menuCursorPosition--;
            } else {
              menuCursorPosition = 0;
            }
            cursorDelayTimeDown = 0;
          }
          //-------------
          //Menu display
          display.clearDisplay();
          display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
          display.setCursor(1, 1);
          display.print(F("6 - Setar R2"));
          display.setCursor(1, 12);
          display.print(F("7 - Reservado"));
          display.setCursor(1, 22);
          display.print(F("8 - Reservado"));
          //----------
          switch (menuCursorPosition) {
            case 5:
              menuPage = 1;
              break;

            case 6: //Set Adc R2 Value
              //Hover animation
              if (menuCursorPosition == 6) {
                display.setCursor(1, 1);
                display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //Animation set
                display.print(F("6 - Setar R2"));
                display.display();
              }
              //--------------
              //Button state checker
              if (digitalRead(buttonEnter) == LOW) {
                cursorDelayTime++;
              } else {
                cursorDelayTime = 0;
              }
              if (cursorDelayTime > cursorDelayTimeInterval) {
                menuUse = 1;
                cursorDelayTime = 0;
              }
              //Enter Menu
              while (menuUse == 1) {
                display.clearDisplay();
                display.setCursor(1, 1);
                display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);  //Animation remove
                display.print(F("Valor Atual:"));
                display.print(adc.getR2());
                display.display();
                //Set value
                //Button state checker
                if (digitalRead(buttonEnter) == LOW) {
                  cursorDelayTime++;
                } else {
                  cursorDelayTime = 0;
                }
                if (cursorDelayTime > cursorDelayTimeIntervalMenu) {
                  menuUse = 0;
                  menuPage = 2;
                  cursorDelayTime = 0;
                }

                //Button state checker
                if (digitalRead(buttonCursorUp) == LOW) {
                  cursorDelayTimeUp++;
                } else {
                  cursorDelayTimeUp = 0;
                }
                if (cursorDelayTimeUp > (cursorDelayTimeInterval + 2)) {
                  adc.setR2(adc.getR2() + 50);
                  cursorDelayTimeUp = 0;
                }

                if (digitalRead(buttonCursorDown) == LOW) {
                  cursorDelayTimeDown++;
                } else {
                  cursorDelayTimeDown = 0;
                }
                if (cursorDelayTimeDown > (cursorDelayTimeInterval + 2)) {
                  adc.setR2(adc.getR2() - 50);
                  cursorDelayTimeDown = 0;
                }
              }
              break;
            case 7: //Reserved - TODO: Set pre defined values for 12v -24v - 36v - 48v
              //Hover animation
              if (menuCursorPosition == 7) {
                display.setCursor(1, 12);
                display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //Animation set
                display.print(F("7 - Reservado"));
                display.display();
              }
              //--------------
              //Button state checker
              if (digitalRead(buttonEnter) == LOW) {
                cursorDelayTime++;
              } else {
                cursorDelayTime = 0;
              }
              if (cursorDelayTime > cursorDelayTimeInterval) {
                menuUse = 1;
                cursorDelayTime = 0;
              }
              //Enter Menu
              while (menuUse == 1) {
                display.clearDisplay();
                display.setCursor(1, 22);
                display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);  //Blink animation remove
                display.print(F("Dentro do Menu:"));
                display.print(menuCursorPosition);
                display.display();
                //Set value
                //Button state checker
                if (digitalRead(buttonEnter) == LOW) {
                  cursorDelayTime++;
                } else {
                  cursorDelayTime = 0;
                }
                if (cursorDelayTime > cursorDelayTimeIntervalMenu) {
                  menuUse = 0;
                  menuPage = 2;
                  cursorDelayTime = 0;
                }
              }
              break;

            case 8: //Reserved
              //Hover animation
              if (menuCursorPosition == 8) {
                display.setCursor(1, 22);
                display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); //Blink animation set
                display.print(F("8 - Reservado"));
                display.display();
              }
              //--------------
              //Button state checker
              if (digitalRead(buttonEnter) == LOW) {
                cursorDelayTime++;
              } else {
                cursorDelayTime = 0;
              }
              if (cursorDelayTime > cursorDelayTimeInterval) {
                menuUse = 1;
                cursorDelayTime = 0;
              }
              //Enter Menu
              while (menuUse == 1) {
                display.clearDisplay();
                display.setCursor(1, 22);
                display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);  //Blink animation remove
                display.print(F("Dentro do Menu:"));
                display.print(menuCursorPosition);
                display.display();
                //Set value
                //Button state checker
                if (digitalRead(buttonEnter) == LOW) {
                  cursorDelayTime++;
                } else {
                  cursorDelayTime = 0;
                }
                if (cursorDelayTime > cursorDelayTimeIntervalMenu) {
                  menuUse = 0;
                  menuPage = 2;
                  cursorDelayTime = 0;
                }
              }
              break;
            case 9:
              menuPage = 0;
              menuCursorPosition = 0;
              break;
            default:
              break;
          }
        }
      }
    }
    //--------------

    while (chargeStatus == 0) {     // Self-Test Routine -- TODO: change to State Machine -> switch case?
      chargeStatus = 1;
      //load variables or selftest
    }

    while (chargeStatus == 1) {      // On Charge Routine -- TODO: change to State Machine -> switch case?
      //Button state checker - menu entry
      if (digitalRead(buttonEnter) == LOW) {
        cursorDelayTime++;
        Serial.println("Hello");
      } else {
        cursorDelayTime = 0;
      }
      if (cursorDelayTime > cursorDelayTimeMenuEntry) {
        menuEnabled = true;
        cursorDelayTime = 0;
        chargeStatus = 0;
      }
      //---
      //Led system on
      !digitalRead(LED_BUILTIN) ? digitalWrite(LED_BUILTIN, HIGH) : digitalWrite(LED_BUILTIN, LOW) ;

      //voltage reading and battery setting
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
      //

      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= 200) {
        previousMillis = currentMillis;
        //Send Json
        sendSerialJson(battery.getVoltage(), cycle.getCurrentTimeFormated(), cycle.getCurrentCycle(), F("Em Carga"));
        //-----------------
        delay(50);
      }

      if (battery.getEndVoltage() < battery.getVoltage() || cycle.getCurrentTimeHours() == battery.getMaxChargeTime ()) { // time or voltage - charge complete checker
        lastTime = cycle.getCurrentTimeFormated();
        chargeStatus = 2; // set status
        display.clearDisplay();
        previousMillis = millis();
      }

    }

    while (chargeStatus == 2) {      // Charge Complete Routine -- TODO: change to State Machine -> switch case ?
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
      //---
      display.setTextSize(1);
      display.setCursor(1, 1);
      display.print(F("Completa: ciclo "));
      display.print(cycle.getCurrentCycle());
      display.setTextSize(2);
      display.setCursor(1, 15);
      display.println(lastTime);
      display.display();
      battery.setVoltage(adc.getReading(8000));

      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= 200) {
        previousMillis = currentMillis;
        //Send Json
        sendSerialJson(battery.getVoltage(), lastTime, cycle.getCurrentCycle(), F("Completa"));
        //-----------------
        delay(50);
      }

      if (battery.getStartVoltage() > battery.getVoltage()) {
        cycle.addCurrentCycle();
        chargeStatus = 1; // set status
        display.clearDisplay();
        cycle.setOffsetMillis(millis());
      }
    }
  }
}


