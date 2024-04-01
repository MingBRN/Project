#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN_1 10
#define RST_PIN_1 9
#define SS_PIN_2 8
#define RST_PIN_2 7
#define Relay 6

MFRC522 mfrc522_1(SS_PIN_1, RST_PIN_1);  // Create MFRC522 instance for reader 1
MFRC522 mfrc522_2(SS_PIN_2, RST_PIN_2);  // Create MFRC522 instance for reader 2

void setup() {
  Serial.begin(9600);  // Initialize serial communications
  SPI.begin();         // Init SPI bus
  mfrc522_1.PCD_Init();  // Init MFRC522 for reader 1
  mfrc522_2.PCD_Init();  // Init MFRC522 for reader 2
  pinMode(Relay, OUTPUT);

}

void loop() {
  // Look for new cards for reader 1
  if (mfrc522_1.PICC_IsNewCardPresent() && mfrc522_1.PICC_ReadCardSerial()) {
    printCardUID(mfrc522_1.uid);
    if (checkIfCorrectCard(mfrc522_1)) {
      // Output digital signal high for reader 1
      digitalWrite(Relay, HIGH);
      delay(2000); // Wait for 2 second
      digitalWrite(Relay, LOW);
    }
    // Halt PICC for reader 1
    mfrc522_1.PICC_HaltA();
    // Stop encryption on PCD for reader 1
    mfrc522_1.PCD_StopCrypto1();
  }

  // Look for new cards for reader 2
  if (mfrc522_2.PICC_IsNewCardPresent() && mfrc522_2.PICC_ReadCardSerial()) {
    printCardUID(mfrc522_2.uid);
    if (checkIfCorrectCard(mfrc522_2)) {
      // Output digital signal high for reader 2
      digitalWrite(Relay, HIGH);
      delay(2000); // Wait for 2 second
      digitalWrite(Relay, LOW);
    }
    // Halt PICC for reader 2
    mfrc522_2.PICC_HaltA();
    // Stop encryption on PCD for reader 2
    mfrc522_2.PCD_StopCrypto1();
  }
}

bool checkIfCorrectCard(MFRC522 &mfrc522) {
  // Check the UID of the card against the correct UID
  byte correctUID[] = {0x30, 0xA8, 0xE7, 0x03}; // UID "30 A8 E7 03"
  if (mfrc522.uid.size != sizeof(correctUID)) {
    return false;
  }
 
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] != correctUID[i]) {
      return false;
    }
  }
  return true;
}

void printCardUID(MFRC522::Uid uid) {
  Serial.print("UID Value: ");
  for (byte i = 0; i < uid.size; i++) {
    Serial.print(uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(uid.uidByte[i], HEX);
  }
  Serial.println();
}