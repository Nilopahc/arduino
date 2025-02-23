const int led1 = 13;
const int led2 = A5;
const int led3 = A4;


void setup(){
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop(){
  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  delay(1000);

  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
  digitalWrite(led3, HIGH);
  delay(1000);

  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, LOW);
  delay(1000);
}