#include "HardwareSerial.h"
// 1 - Toujours commencer par ce pragma
#pragma once

// 2 - Inclure le fichier tp_sysit_feather.h
#include "tp_sysit_feather.h"

// 3 - Si besoin, faire apparaitre ici les #include <...h>

// 4 - Déclaration des constantes
const byte LED_RED = 0 ;
const byte LED_GREEN = 1 ;
const byte LED_YELLOW = 2 ;
const byte LED_BLUE = 3 ;
const byte BUTTON_P0 = 0 ;
const byte BUTTON_P1 = 1 ;
const byte BUTTON_P2 = 2 ;
const byte ENCODER_CLIC = 3 ;

// 5 - Déclaration des fonctions

void beginIO (void) {
    Serial1.begin(19200, SERIAL_8N1);
}

void ledWrite (const byte inLed, const bool inLevel) {
  const int byteInLevel = inLevel << 2;
  const int byte = byteInLevel + inLed;
  
  // Serial.print("inLevel : ");
  // Serial.println(inLevel);

  // Serial.print("inLed : ");
  // Serial.println(inLed);


  // Serial.print("byteInLevel : ");
  // Serial.println(byteInLevel);

  // Serial.print("byte : ");
  // Serial.println(byte);

  Serial1.write(byte);
}

bool pushButton (const byte inButton);

byte encoderValue (void) ;
