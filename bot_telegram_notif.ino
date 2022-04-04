#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define WIFI_SSID "XXXXXXXXXXXXXXXXXXXXXXXX"
#define WIFI_PASSWORD "XXXXXXXXXXXXXXXXXXXXXXXXX"
#define BOTtoken "XXXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
#define CHAT_ID "XXXXXXXXX"

const int motionSensor = 13;
int sensorState = 0;

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOTtoken, secured_client);

void setup() {
  Serial.begin(115200);
  Serial.println("");

  pinMode(motionSensor, INPUT);

  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  Serial.print("Connecting to Wifi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  bot.sendMessage(CHAT_ID, "Bot started up", "");
}

void loop() {
  sensorState = digitalRead(motionSensor);

  if (sensorState == HIGH) {
    bot.sendMessage(CHAT_ID, "Motion Detected", "");
    delay(4000);
  }
  delay(1000);
}
