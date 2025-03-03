#include <Adafruit_Fingerprint.h>

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(2, 3);  // Comunicação serial para o sensor de impressões digitais
#else
#define mySerial Serial1 // Para placas que não são AVR, usa-se o Serial1
#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
//----------------PARA RELEMBRAR-------------------------------
//HIGH E LOWS ESTÃO INVERTIDOS POR CONTA DO LED RGB PASSIVO
void setup() {
  // Inicializa a comunicação serial
  Serial.begin(9600);
  while (!Serial);  // Aguarda a conexão serial
  delay(100);
  Serial.println("\nAdafruit Fingerprint Test");
  Serial.println("Digite '1' para registrar uma nova impressão digital.");
  Serial.println("Digite '2' para verificar uma impressão digital.");

  // Inicializa o sensor de impressões digitais
  finger.begin(57600);
  delay(5);

  if (finger.verifyPassword()) {
    Serial.println("Sensor de impressões digitais encontrado!");
  } else {
    Serial.println("Não foi possível encontrar o sensor de impressões digitais. Verifique a conexão.");
    while (1) { delay(1); }
  }

  // Configura os pinos dos LEDs
  pinMode(8, OUTPUT); // LED de sucesso
  pinMode(9, OUTPUT); // LED de falha

  digitalWrite(8, HIGH); // Certifica-se de que ambos os LEDs estão apagados no início
  digitalWrite(9, HIGH);
}

void loop() {
  // Verifica se há entrada no monitor serial
  if (Serial.available() > 0) {
    char option = Serial.read();  // Lê o comando do usuário

    if (option == '1') {
      Serial.println("Modo de registro de impressão digital.");
      registerFingerprint();  // Chama a função para registrar uma nova impressão digital
    } else if (option == '2') {
      Serial.println("Modo de verificação de impressão digital.");
      verifyFingerprint();  // Chama a função para verificar a impressão digital
    } else {
      Serial.println("Comando inválido. Digite '1' para registrar ou '2' para verificar.");
    }
  }
}

// Função para verificar se o dedo corresponde a algum já registrado
void verifyFingerprint() {
  Serial.println("Coloque o dedo no sensor para verificar.");

  // Aguarda até que um dedo seja detectado
  while (finger.getImage() == FINGERPRINT_NOFINGER) {
    delay(100);
  }

  int result = getFingerprintID();

  if (result != -1) {
    Serial.print("Impressão digital reconhecida com sucesso! ID: ");
    Serial.println(finger.fingerID);
    digitalWrite(8, LOW); // Acende o LED de sucesso
    digitalWrite(9, HIGH);  // Apaga o LED de falha
  } else {
    Serial.println("Impressão digital não reconhecida.");
    digitalWrite(8, HIGH);  // Apaga o LED de sucesso
    digitalWrite(9, LOW); // Acende o LED de falha
  }

  delay(2000);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
}

// Função para registrar uma nova impressão digital, substituindo a anterior, não ocupando memória pelo fato de ser projeto para exposição
void registerFingerprint() {
  uint8_t p = -1;

  Serial.println("Coloque o dedo no sensor para registrar.");
  
  // Aguarda até que o dedo seja detectado no sensor
  while (finger.getImage() == FINGERPRINT_NOFINGER) {
    delay(100); // Espera ativa até que o dedo seja colocado
  }

  // Captura a imagem da impressão digital
  p = finger.getImage();
  if (p != FINGERPRINT_OK) {
    Serial.println("Erro ao capturar imagem! Tente novamente.");
    return;
  }

  // Processa a imagem capturada
  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) {
    Serial.println("Erro ao processar imagem! Certifique-se de que o dedo está posicionado corretamente.");
    return;
  }

  Serial.println("Remova o dedo.");
  while (finger.getImage() != FINGERPRINT_NOFINGER) {
    delay(100); // Aguarda até que o dedo seja removido
  }

  Serial.println("Coloque o mesmo dedo novamente para confirmar.");
  while (finger.getImage() == FINGERPRINT_NOFINGER) {
    delay(100); // Aguarda até que o dedo seja colocado novamente
  }

  // Captura a segunda imagem para confirmação
  p = finger.getImage();
  if (p != FINGERPRINT_OK) {
    Serial.println("Erro ao capturar imagem! Tente novamente.");
    return;
  }

  // Processa a segunda imagem
  p = finger.image2Tz(2);
  if (p != FINGERPRINT_OK) {
    Serial.println("Erro ao processar segunda imagem! Tente manter o dedo na mesma posição.");
    return;
  }

  // Cria o modelo com as duas imagens
  p = finger.createModel();
  if (p != FINGERPRINT_OK) {
    Serial.println("Erro ao criar modelo! As duas leituras não correspondem.");
    return;
  }

  // Armazena o modelo na posição 1
  p = finger.storeModel(1);
  if (p != FINGERPRINT_OK) {
    Serial.println("Erro ao armazenar o modelo!");
    return;
  }

  Serial.println("Impressão digital registrada com sucesso!");
  digitalWrite(8, LOW); // Acende o LED de sucesso
  delay(2000);
  digitalWrite(8, HIGH);
}

// Função auxiliar para capturar a ID de uma impressão digital
int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p == FINGERPRINT_NOFINGER) {
    return -1; // Retorna código específico se não houver dedo
  }
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerSearch();
  if (p != FINGERPRINT_OK) return -1;

  Serial.print("ID encontrada: "); Serial.print(finger.fingerID);
  Serial.print(" com confiança de "); Serial.println(finger.confidence);
  return finger.fingerID;
}