#include <LiquidCrystal_PCF8574.h>

#include <LCD_I2C.h>
#include <Wire.h> 
LCD_I2C lcd(0x27);

void setup() {
  lcd.begin();
  lcd.backlight(); 
  lcd.clear();

  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  digitalWrite(2,LOW);  
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
}

void loop() {
  digitalWrite(2,LOW); 
  delay(1000);
  digitalWrite(2,HIGH);
/*lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("2, high"); 
*/
  delay(1000);
  digitalWrite(3,LOW); 
  delay(1000);
  digitalWrite(3,HIGH); 
/*lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("3, high");
*/
  delay(1000);
  digitalWrite(4,LOW); 
  delay(1000);
  digitalWrite(4,HIGH);
/*lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("4, high"); 
*/
  delay(1000);
  digitalWrite(5,LOW); 
  delay(1000);
  digitalWrite(5,HIGH); 
/*lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("5, high");
*/
  delay(1000);
}
