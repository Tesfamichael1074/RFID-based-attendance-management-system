#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         0          // Reset pin connected to D3
#define SS_PIN          15         // SDA pin connected to D8

MFRC522 mfrc522(SS_PIN, RST_PIN);  // MFRC522 instance
String uuid = "";
void setup() {
  Serial.begin(115200);   // Initialize serial communications with the PC
  while (!Serial){
    Serial.print(".");
  }
  Serial.println("");
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
}

void loop() {
  // Reset the loop if no new card present.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  // Returns true if a UID could be read.
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

   for (int i = 0; i < 4; i++){
    uuid = String(mfrc522.uid.uidByte[i], HEX) ;
    uuid.toUpperCase();
    Serial.print(uuid);
   }
   

    Serial.println("");
    delay(100);
}
