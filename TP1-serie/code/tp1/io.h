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

void beginIO (void) ;

void ledWrite (const byte inLed, const bool inLevel) ;

bool pushButton (const byte inButton) ;

byte encoderValue (void) ;
