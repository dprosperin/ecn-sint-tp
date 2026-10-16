#include <LiquidCrystal.h>
#include "io.h"


LiquidCrystal lcd (18, 17, 16, 15, 14, 19) ;

extern int etatButton;
extern int encoder;

void setup() {
  pinMode (LED_BUILTIN, OUTPUT);
  digitalWrite (LED_BUILTIN, HIGH) ;
  Serial.begin (9600) ;
  beginIO();
  ledWrite(0, 0);
  ledWrite(1, 0);
  ledWrite(2, 0);
  ledWrite(3, 0);
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
  }

}
