/* Project Name: Sistema de Controle Automatico
 * Author: Marcos Vinicius Pereira da Silva
 * Date: 09/09/22
 * Description: Firmware to use in LUF800R00, this programm aims to give a easy
 * setup and installation for the user, it can configure its main charge characteristics
 * such as end voltage and maximum charge time. The main goal of this programm
 * is to replace and be the standard for all automatic chargers of the company.
 * To use this system is advised to install the Oled display on the i2c port, without
 * it will be very difficult to caliber the system, it can be used with only the serial port
 * but we recommend otherwise.  
 * Any changes on this code is not permited, under lincesed use only
 */

#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Display CFG
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define LOGO_HEIGHT   32
#define LOGO_WIDTH    128
static const unsigned char PROGMEM logo_bmp[] =
{     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
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

//----- Pre - Defined CFG ----
#define ADC_CH  A1
const float ADC_FIX = 0.002;
const float END_VOLTAGE = 14.70;
const float START_VOLTAGE = 11.80;
const byte MAX_CHARGE_TIME = 1;
//-----------------------

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
          unsigned long currentTimeSeconds = (millis() - offsetMillis + 3593000L) /1000;
          short currentTimeMinutes = 0;
          short currentTimeHoursFormated = 0;
          short currentTimeSecondsFormated = 0;
          short currentTimeMinutesFormated = 0;
          boolean hasTimeLeft = true;
          
          //Seconds processing
          while(hasTimeLeft) {
            if ((currentTimeSeconds/60) >= 1) {
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
            if ((currentTimeMinutes/60) >= 1) {
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
      float voltage = 0;
      float endVoltage = END_VOLTAGE;
      float startVoltage = START_VOLTAGE;
      byte maxChargeTime = MAX_CHARGE_TIME;
      
      public:
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
    
      void  setStartVoltage (float voltage){
        startVoltage = voltage;
        }
    
      void  setEndVoltage (float voltage){
        endVoltage = voltage;
        }

      void setMaxChargeTime (byte newChargeTime) {
        maxChargeTime = newChargeTime;
        }

      byte getMaxChargeTime () {
        return maxChargeTime;
        }
        
  };
  
class AdConverter {
      private:
      const float R1 = 98400;
      const float R2 = 4550;
      float adcFix = 0.002;
      byte adcChannel = A1;
      float aref = 0.00342 ;
      
      public:
      adConverter (byte channelNumber, float fixValue, float arefValue) {
        adcChannel = channelNumber;
        adcFix = fixValue;
        aref = arefValue;
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
            adcRead = adcRead / (R2/(R1+R2)); // Vout / (R2/(R1+R2)) - currently -> 4550 / 102.950 = 0,04419
            adcReadRaw = (adcRead*getAdcFix()); // minor % fix on readings
            adcRead -= adcReadRaw;
            
            return adcRead;
        }
    
      void setAdcFix (int fixValue) {
        adcFix = fixValue / 1000;
        }
        
      float getAdcFix () {
        return adcFix;
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
    // -------- Display verifier
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        while(true) { //Lock program if display fail
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
  while(true) { //Power On Routine -- TODO: change to State Machina -> switch case?
    
    while(chargeStatus == 0) {       // On Charge Routine -- TODO: change to State Machine -> switch case?
        //Led system on
        !digitalRead(LED_BUILTIN) ? digitalWrite(LED_BUILTIN, HIGH) :digitalWrite(LED_BUILTIN, LOW) ;  
        
        //voltage reading and battery setting
        battery.setVoltage(adc.getReading(8000));
        
        //Display routine    
        display.drawBitmap(( display.width()  - LOGO_WIDTH ) / 2, (display.height() - LOGO_HEIGHT) / 2, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
        display.setTextColor(SSD1306_WHITE);
        display.fillRect(34,0,80,32,SSD1306_BLACK);
        display.setTextSize(1); 
        display.setCursor(52,16);
        display.println(F("Tempo atual"));
        display.setCursor(52,24);
        display.println(cycle.getCurrentTimeFormated());
        display.setTextSize(2);      
        display.setCursor(52,1);
        display.println(battery.getVoltage(),2);
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
        
        if(battery.getEndVoltage() < battery.getVoltage() || cycle.getCurrentTimeHours() == battery.getMaxChargeTime ()) { // time or voltage - charge complete checker          
          lastTime = cycle.getCurrentTimeFormated(); 
          chargeStatus = 2; // set status          
          display.clearDisplay();
          previousMillis = millis();
          }
          
    }   

   while(chargeStatus == 2) { // Charge Complete Routine -- TODO: change to State Machine -> switch case ?
          
          display.setTextSize(1);      
          display.setCursor(1,1);
          display.print(F("Completa: ciclo "));
          display.print(cycle.getCurrentCycle());
          display.setTextSize(2);
          display.setCursor(1,15);
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
            chargeStatus = 0; // set status
            display.clearDisplay();
            cycle.setOffsetMillis(millis());
             }  
          
          }

  }
}
