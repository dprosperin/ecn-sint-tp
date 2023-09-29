#define PIN_LED0 3
#define PIN_LED1 4

void setup() {
  pinMode(PIN_LED0, OUTPUT);
  pinMode(PIN_LED1, OUTPUT);  
  randomSeed(analogRead(0)) ;

}

void loop() {
  digitalWrite (PIN_LED1, 1) ;
  delay(500);
  digitalWrite (PIN_LED0, 0) ;
  delay(200+random(500));
}
