#include <LiquidCrystal.h>
#include "io.h"
#include <Wire.h>

// D1 = SDA
// D2 = SCL
LiquidCrystal lcd (18, 17, 16, 15, 14, 19) ;

extern int etatButton;
extern int encoder;
static unsigned long accel_affichage = 0;

float accel_x = 0, accel_y = 0, accel_z = 0;
int tmp = 0;

void setup() {
  pinMode (LED_BUILTIN, OUTPUT);
  digitalWrite (LED_BUILTIN, HIGH) ;
  Serial.begin (115200) ;
  beginIO();
  ledWrite(0, 0);
  ledWrite(1, 0);
  ledWrite(2, 0);
  ledWrite(3, 0);

  lcd.begin(20, 4);  
  
  lcd.setCursor(0, 0);
  lcd.printf("David Noah Angelique");
  
  lcd.setCursor(0, 1);
  lcd.print(" X     Y      Z");

  lcd.setCursor(2, 4);
  lcd.print("C");
  
  setSensibilite(GLVL_2G);
  setMode(MEASUREMENT_MODE);
  delay(300);
}

void loop() {
  if (millis() - accel_affichage >= 10)
  {
    lcd.setCursor(0, 2);
    lcd.print(accel_x);

    lcd.setCursor(6, 2);
    lcd.print(accel_y);

    lcd.setCursor(12, 2);
    lcd.print(accel_z);

    lcd.setCursor(0, 4);
    lcd.print(tmp, DEC);

    // Serial.print(accel_x);
    // Serial.print(", ");
    // Serial.print(accel_y);
    // Serial.print(", ");
    // Serial.println(accel_z);

    accel_affichage = millis();
  }

  while(!isReady()) {}
  accel_x = (int8_t)lectureI2C(ACCEL_ID_SLAVE, ACCEL_ADDR_XOUT8) / 64.0f;
  accel_y = (int8_t)lectureI2C(ACCEL_ID_SLAVE, ACCEL_ADDR_YOUT8) / 64.0f;
  accel_z = (int8_t)lectureI2C(ACCEL_ID_SLAVE, ACCEL_ADDR_ZOUT8) / 64.0f;

  tmp     = lectureI2C(ACCEL_ID_SLAVE, ACCEL_ADDR_TOUT);
}
