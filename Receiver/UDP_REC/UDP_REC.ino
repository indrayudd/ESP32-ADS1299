#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "SSID";
const char* password = "PASSWORD";
const int udpPort = 1234; // Change to the same UDP port used by the sender

WiFiUDP udp;
float values[6]; // Array to store the received float values

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
//    Serial.print(".");
  }

//  Serial.println("WiFi connected!");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());

  udp.begin(udpPort);
}

void loop() {
  if (udp.parsePacket()) {
    int packetSize = udp.read((byte*)values, sizeof(float) * 6);

    if (packetSize == sizeof(float) * 6) {
      // Process the received values (e.g., print to Serial monitor)
//      for (int i = 0; i < 6; i++) {
//        Serial.print(values[i], 2); // Print with 2 decimal places
//        Serial.print(" ");
//      }
//      Serial.println();

      Serial.print(-12.5);
      Serial.print('\t');
      Serial.print(values[0]);
      Serial.print("\t");
      Serial.print(values[1]);
      Serial.print("\t");
      Serial.print(values[2]);
      Serial.print("\t");
      Serial.print(values[3]);
      Serial.print("\t");
      Serial.print(values[4]);
      Serial.print("\t");
      Serial.print(values[5]);
      Serial.print('\t');
      Serial.println(12.5);

    }
  }
}
