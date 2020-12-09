#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>

#define RST_PIN         0          // Reset pin connected to D3
#define SS_PIN          15         // SDA pin connected to D8

#define ERR_Pin         4          // Pin Connected to red led
#define SUC_Pin          5         // Pin Connected to blue led

MFRC522 mfrc522(SS_PIN, RST_PIN);  // MFRC522 instance

const char *ssid =".";
const char *password = "nahibeja";
const char *host = "http://192.168.137.253:3000/api/attendances/postAttendance";
HTTPClient http; //Declare an object of class HTTPClient
WiFiClient client;


String uuid = "";
String postData;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(ERR_Pin, OUTPUT);
    pinMode(SUC_Pin, OUTPUT);
    
    Serial.begin(115200);   // Initialize serial communications with the PC
    delay(4);
    while (!Serial);
    connectToWifi();
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
    uuid += String(mfrc522.uid.uidByte[i], HEX) ;
    uuid.toUpperCase();
   }

   Serial.println(uuid);
   sendPostR(uuid);
   

    Serial.println("");
    delay(1000);
    uuid = "";
}

void connectToWifi(){
  WiFi.begin(ssid, password); //Connect to your WiFi
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); // Loop untill the wifi is connected
    Serial.print(".");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
    }

  Serial.println("");
  Serial.print("IP address — ");
  Serial.println(WiFi.localIP()); //IP address assigned to your ESP8266

}

void sendPostR(String rid) {

postData = "rfid=" + rid ;
http.begin(client, host);
http.addHeader("Content-Type", "application/x-www-form-urlencoded");
int httpCode = http.POST(postData);
String payload = http.getString();
Serial.println(httpCode);
http.end(); 
if(httpCode == 500){
  Serial.println("Not registered");
  digitalWrite(ERR_Pin, HIGH);
  delay(3000);
  digitalWrite(ERR_Pin, LOW);
}
else {
  Serial.println("Attendance taken! ");
  digitalWrite(SUC_Pin, HIGH);
  delay(3000);
  digitalWrite(SUC_Pin, LOW);
}

}
