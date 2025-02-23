#include <Adafruit_Fingerprint.h>

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(3, 2);
#else
#define mySerial Serial1
#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  // Inicialização da comunicação serial
  Serial.begin(9600);
  while (!Serial);
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // Inicialização do sensor
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  // Configuração dos pinos dos LEDs
  pinMode(8, OUTPUT); // LED de sucesso
  pinMode(9, OUTPUT); // LED de falha

  // Certifica-se de que ambos os LEDs estão desligados no início
 // digitalWrite(8, HIGH);
 // digitalWrite(9, HIGH);
}

void loop() {
  int result = getFingerprintID();

  if (result == FINGERPRINT_NOFINGER) {
    // Se não houver dedo, todos os LEDs ficam apagados
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
  } else if (result != -1) {
    // Se o dedo for reconhecido, acende o LED 1 (sucesso)
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    delay(2000);
  } else {
    // Se o dedo não for reconhecido, acende o LED 2 (falha)
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
  }

  delay(100);
}

int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p == FINGERPRINT_NOFINGER) {
    return FINGERPRINT_NOFINGER; // Retorna um código específico se não houver dedo
  }
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerSearch();
  if (p != FINGERPRINT_OK) return -1;

  // Imprime a ID do dedo e retorna a ID se uma correspondência for encontrada
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
