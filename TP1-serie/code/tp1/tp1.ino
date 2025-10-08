#include <LiquidCrystal.h>
#include "io.h"


LiquidCrystal lcd (18, 17, 16, 15, 14, 19) ;

void setup() {
  pinMode (LED_BUILTIN, OUTPUT);
  digitalWrite (LED_BUILTIN, HIGH) ;
  Serial.begin (9600) ;
}

void loop() {
  // put your main code here, to run repeatedly:
}
