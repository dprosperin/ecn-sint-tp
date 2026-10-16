#include <LiquidCrystal.h>
#include "io.h"


LiquidCrystal lcd (18, 17, 16, 15, 14, 19) ;

extern int etatButton;
extern int encoder;
static unsigned long led_1hz = 0;

void setup() {
  pinMode (LED_BUILTIN, OUTPUT);
  digitalWrite (LED_BUILTIN, HIGH) ;
  Serial.begin (9600) ;
  beginIO();
  ledWrite(0, 0);
  ledWrite(1, 0);
  ledWrite(2, 0);
  ledWrite(3, 0);
  lcd.begin(20, 4);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available() > 0) {
    etatButton = Serial1.read();

    if (etatButton == 0x10) {
      encoder++;
    } else if (etatButton == 0x20) {
      encoder--;
    }

    if (encoder < 0) {
      encoder = 63;
    } else if (encoder > 63) {
      encoder = 0;
    }

    Serial.print("etat recu : ");
    Serial.println(etatButton);
    Serial.print("enc : ");
    Serial.println(encoder);

    lcd.setCursor(0, 3);
    lcd.printf("enc : %d", encoderValue());
  }

  
    // Builtin led 1 hz
     if (millis() - led_1hz >= 500)
    {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      led_1hz = millis();
    }

    if (pushButton(8)) {
      ledWrite(3, 1);
    } else if (pushButton(1)) {
      ledWrite(2, 1);
    } else if (pushButton(2)) {
      ledWrite(1, 1);
    } else if (pushButton(4)) {
      ledWrite(0, 1);
    } else {
      ledWrite(0, 0);
      ledWrite(1, 0);
      ledWrite(2, 0);
      ledWrite(3, 0);
    }
}
