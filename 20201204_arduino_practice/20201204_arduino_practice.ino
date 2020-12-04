#include <WiFi.h>
#include <SimpleDHT.h>
#include "ThingSpeak.h"
#include "secrets.h"

// for DHT11,
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 17;
SimpleDHT11 dht11(pinDHT11);

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int keyIndex = 0;            // your network key index number (needed only for WEP)

WiFiClient  client;

void setup() {
  pinMode(36, INPUT);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);
}

void loop() {
  int data = analogRead(36);
  int temp, hum;
  byte temperature = 0;
  byte humidity = 0;

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  int httpCode = ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);
  delay(1500);
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err); delay(1000);
    return;
  }
  
  else
  {
    temp = (int)temperature;
//    httpCode = ThingSpeak.writeField(myChannelNumber, 2, temp, myWriteAPIKey);
//    delay(3000);
    hum = (int)humidity;
//    httpCode = ThingSpeak.writeField(myChannelNumber, 3, hum, myWriteAPIKey);
//    delay(3000);
    Serial.print(" DHT: ");
    Serial.print("Sample OK: ");
    Serial.print((int)temperature); Serial.print(" *C, ");
    Serial.print((int)humidity); Serial.println(" H");
  }
  Serial.print(" light: ");
  Serial.println(data);
  Serial.println("=================================");
  if (httpCode == 200) {
    Serial.println("Channel write successful.");
  }
  else {
    Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  }
  ThingSpeak.setField(1, data);
  ThingSpeak.setField(2, temp);
  ThingSpeak.setField(3, hum);
  
}
