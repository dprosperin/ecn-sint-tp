#include "HardwareSerial.h"
#include <Wire.h>

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

typedef enum {
  GLVL_8G = 0b00,
  GLVL_2G = 0b01,
  GLVL_4G = 0b10
} accel_glvl_reg_e;

typedef enum {
  STANDBY_MODE         = 0b00,
  MEASUREMENT_MODE     = 0b01,
  LEVEL_DETECTION_MODE = 0b10,
  PULSE_DETECTION_MODE = 0b11
} accel_mode_reg_e;

typedef union {
  uint8_t reg;

  struct {
    uint8_t mode : 2;
    uint8_t glvl : 2;
    uint8_t ston : 1;
    uint8_t spi3w: 1;
    uint8_t drpd : 1;  
  };
} mode_control_reg_u;

typedef union {
  uint8_t reg;

  struct {
    uint8_t drdy     : 1;
    uint8_t dovr     : 1;
    uint8_t perr     : 1;
    uint8_t reserved : 5;
  };
} status_reg_u;

#define ACCEL_ID_SLAVE 0x1D
#define ACCEL_ADDR_MODE_CONTROL 0x16

#define ACCEL_ADDR_XOUT8 0x06
#define ACCEL_ADDR_YOUT8 0x07
#define ACCEL_ADDR_ZOUT8 0x08

#define ACCEL_ADDR_STATUS 0x09
#define ACCEL_ADDR_TOUT 0x0B

// 5 - Déclaration des fonctions

void beginIO (void) {
    Serial1.begin(19200, SERIAL_8N1);

    // Initialisation du bus I2C
    Wire.begin(); 
    Wire.setClock (400000); // On veut une fréquence de 400KHz
}

void ledWrite (const byte inLed, const bool inLevel) {
  const int byteInLevel = inLevel << 2;
  const int byte = byteInLevel | inLed;

  Serial1.write(byte);
}

bool pushButton (const byte inButton);

byte encoderValue (void) ;

void receptionUART(void);

void ecritureI2C(uint8_t ID, uint8_t registre, uint8_t data);

int8_t lectureI2C(uint8_t ID, uint8_t registre);

void setSensibilite(accel_glvl_reg_e sensi);

void setMode(accel_mode_reg_e mode);

bool isReady();