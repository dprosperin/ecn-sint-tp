#define PIN_LED0 3
#define PIN_LED1 4

static unsigned long led0 = 0;
static unsigned long led1 = 0;
static unsigned long duree = 0;


void setup() {
  pinMode(PIN_LED0, OUTPUT);
  pinMode(PIN_LED1, OUTPUT);  
  randomSeed(analogRead(0)) ;
}

void loop() {
  if (millis() - led0 >= 250)
  {
    digitalToggle(PIN_LED0);
    led0 = millis();
  }

  if ((millis() - led1) >= duree)
  {
   
    digitalToggle(PIN_LED1);
    led1 = millis();
    duree = 200+random(500);
  }
}
