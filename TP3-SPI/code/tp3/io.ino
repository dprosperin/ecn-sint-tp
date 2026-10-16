#include "io.h"

int etatButton = 0;
int encoder    = 0;

bool pushButton (const byte inButton) {
  if ((etatButton & 0x1) == inButton) 
  {
    return 1;
  } 
  else if ((etatButton & 0x2) == inButton)
  {
    return 1;

  } 
  else if ((etatButton & 0x4) == inButton) 
  {
    return 1;
  
  } 
  else if ((etatButton & 0x8) == inButton) 
  {
    return 1;
  
  }else { 
    return 0;
  }
}

byte encoderValue (void) {
  return encoder;
}

void receptionUART(void) {
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
}

void ecritureI2C(uint8_t ID, uint8_t registre, uint8_t data)
{
  Wire.beginTransmission(ID);
  Wire.write(registre);
  Wire.write(data);
  Wire.endTransmission(true);
}

uint8_t lectureI2C(uint8_t ID, uint8_t registre)
{
  uint8_t c;
  Wire.beginTransmission(ID);
  Wire.write(registre);
  Wire.requestFrom(ID, 1);

  while(Wire.available())
  {
    c = Wire.read();
  }
  return c;
}

void setSensibilite(accel_glvl_reg_e sensi)
{
  mode_control_reg_u mode_control_reg;  

  mode_control_reg.reg =  lectureI2C(ACCEL_ID_SLAVE, ACCEL_ADDR_MODE_CONTROL);

  mode_control_reg.glvl = (uint8_t)sensi;

  ecritureI2C(ACCEL_ID_SLAVE, ACCEL_ADDR_MODE_CONTROL, mode_control_reg.reg);
}

void setMode(accel_mode_reg_e mode) {
  mode_control_reg_u mode_control_reg;  

  mode_control_reg.reg =  lectureI2C(ACCEL_ID_SLAVE, ACCEL_ADDR_MODE_CONTROL);

  mode_control_reg.mode = (uint8_t)mode;

  ecritureI2C(ACCEL_ID_SLAVE, ACCEL_ADDR_MODE_CONTROL, mode_control_reg.reg);
}

bool isReady()
{
  status_reg_u status_reg;
  status_reg.reg = lectureI2C(ACCEL_ID_SLAVE, ACCEL_ADDR_STATUS);
  return status_reg.drdy;
}

void initialiserSPILogiciel(void)
{
  pinMode(MISO_PIN, INPUT); // set the SS pin as an output
  pinMode(MOSI_PIN, OUTPUT); 
  pinMode(SS_PIN, OUTPUT); 
  pinMode(SCK_PIN, OUTPUT); 
  
  digitalWrite(SS_PIN, HIGH);
  digitalWrite(SCK_PIN,LOW);
}

void echangeSPI(const byte inDonneeEmise)
{
  // Mode 0 
  // CPOL = 0 CPHA = 0
  digitalWrite(SS_PIN, LOW);

  for(int i = 7; i >= 0 ; i--)
  {
    inDonneeEmise << yty
    digitalWrite(MOSI, inDonneeEmise);
    delayMicroseconds(1);
    digitalRead(MISO);
    digitalWrite(SCK_PIN,HIGH);
    delayMicroseconds(1);
    digitalWrite(SCK_PIN,LOW);
  }
}