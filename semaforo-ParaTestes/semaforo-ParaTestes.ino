const int leds[] = {13, A5, A4};
const int length = sizeof(leds) / sizeof(leds[0]);

void setup(){
  for(int i = 0; i < length; i++){
    pinMode(led[i], OUTPUT);
  }
}

void loop(){
  for(int i = 0; i < length; i++){
    digitalWrite(leds[i], HIGH);
    delay(500);
    digitalWrite(leds[i], LOW);
    delay(500);
  }
}
