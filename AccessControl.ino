#include <SPI.h>
#include <MFRC522.h>
#include <WiFiClientSecure.h>

#define RST_PIN         22         // Change to your configuration
#define SS_PIN          21         // Change to your configuration

#define LOCKER_PIN      2         //2: Built-In LED
#define GREEN_PIN       13
#define RED_PIN         12
#define BUZZER_PIN      14

#define LOCKER_TIME     5000      //In ms
#define BUZZER_TIME     5000      //In ms
#define BLINK_TIME      1000      //In ms

char* ssid     = "DEMO-WIFI";      // Change to your configuration
char* password = "16x7<Z82";       // Change to your configuration

const char* server = "www.fs-medtech.de";
char* path = "/bib_test/";  //Slash is necessary

String accessKeyValue = "123456";


//www.fs-medtech.de
const char* root_ca = \
                      "-----BEGIN CERTIFICATE-----\n" \
                      "MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n" \
                      "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
                      "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n" \
                      "WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n" \
                      "ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n" \
                      "MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n" \
                      "h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n" \
                      "0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n" \
                      "A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n" \
                      "T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n" \
                      "B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n" \
                      "B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n" \
                      "KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n" \
                      "OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n" \
                      "jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n" \
                      "qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n" \
                      "rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n" \
                      "HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n" \
                      "hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n" \
                      "ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n" \
                      "3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n" \
                      "NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n" \
                      "ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n" \
                      "TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n" \
                      "jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n" \
                      "oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n" \
                      "4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n" \
                      "mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n" \
                      "emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n" \
                      "-----END CERTIFICATE-----\n";

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

WiFiClientSecure client;

void setup() {
  pinMode(LOCKER_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(115200);   // Initialize serial communications with the PC
  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    //WiFi.begin(ssid, password);
    delay(1000);
    Serial.print(".");
  }

  Serial.print("Connected to ");
  Serial.println(ssid);

  client.setCACert(root_ca);
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

  if (WiFi.status() == WL_CONNECTED) {
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

    digitalWrite(RED_PIN, HIGH);
    Serial.println("\nStarting connection to server...");
    if (!client.connect(server, 443))
      Serial.println("Connection failed!");
    else {
      Serial.println("Connected to server!");
      // Make a HTTP request:
      String httpRequestData = "index.php/?ac=check_access&access_key=" + accessKeyValue + "&UID=" + cardnumber;
      Serial.println("https://" + String(server) + String(path) + httpRequestData);
      client.println("GET https://" + String(server) + String(path) + httpRequestData + " HTTP/1.1");
      client.println("Host:" + String(server));
      client.println("Connection: close");
      client.println();
      digitalWrite(RED_PIN, LOW);

      String responseCode = getResponseCode();
      Serial.print("Response Code: ");
      Serial.println(responseCode);
      client.stop();

      if (responseCode == "200") {    //Access
        Serial.println("Access");
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(LOCKER_PIN, HIGH);
        delay(LOCKER_TIME);
        digitalWrite(LOCKER_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
      }
      else if (responseCode == "401") { //No access
        Serial.println("No Access");
        digitalWrite(BUZZER_PIN, HIGH);
        for (int i = 0; i <= round(BUZZER_TIME / (BLINK_TIME * 2)); i++) {
          digitalWrite(RED_PIN, HIGH);
          delay(BLINK_TIME);
          digitalWrite(RED_PIN, LOW);
          delay(BLINK_TIME);
        }
        digitalWrite(BUZZER_PIN, LOW);
      }
      else { //Error, try again
        digitalWrite(RED_PIN, HIGH);
        delay(BUZZER_TIME);
        digitalWrite(RED_PIN, LOW);
      }
    }
  }
}

String getResponseCode() {
  String responseCode = "";
  int abc = 0;
  while (client.connected()) {
    if (client.available()) {
      String response = client.readStringUntil('\n');
      abc++;
      if (responseCode == "") {
        if (abc >= 23 && abc <= 25) {
          responseCode = response.substring(0, 3);
          if (responseCode.toInt() != 0) { //If response code is a number
             return responseCode;          //Valid number found
          }
        }
      }
    }
  }
  return "0"; // No valid number found
}
