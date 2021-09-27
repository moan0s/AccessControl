#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>

#define RST_PIN         4          // Change to your configuration
#define SS_PIN          15         // Change to your configuration

char* ssid     = "DEMO-WIFI";      // Change to your configuration
char* password = "16x7<Z82";       // Change to your configuration

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(115200);   // Initialize serial communications with the PC
  SPI.begin();            // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  Serial.println(WiFi.status());
  while (WiFi.status() != WL_CONNECTED) {

    //WiFi.begin(ssid, password);
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  String cardnumber = content.substring(1);
  cardnumber.replace(" ", "");  //Remove spaces
  Serial.println(cardnumber);
}
