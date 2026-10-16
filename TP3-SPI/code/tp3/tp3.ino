#include <LiquidCrystal.h>
#include "io.h"
#include <Wire.h>
#include <Spi.h>

#define SS_PIN   6
#define SCK_PIN  12
#define MOSI_PIN 11
#define MISO_PIN 10

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
  Serial.begin (9600) ;
  beginIO();
  ledWrite(0, 0);
  ledWrite(1, 0);
  ledWrite(2, 0);
  ledWrite(3, 0);

  delay(300);
}

void loop() {


}
