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
