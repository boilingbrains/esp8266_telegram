#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define WIFI_SSID "XXXXXXXXXXXXXXXXXXXXXXXX"
#define WIFI_PASSWORD "XXXXXXXXXXXXXXXXXXXXXXXXX"
#define BOTtoken "XXXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
#define CHAT_ID "XXXXXXXXX"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOTtoken, secured_client);

const long interval = 1000;
long lastTime = 0;

const int relay = 13;
bool relayStatus = false;

void handleNewMessages(int numNewMessages) {

  for (int i = 0; i < numNewMessages; i++)
  {
    String text = bot.messages[i].text;
    
    if (text == "/on")
    {
      digitalWrite(relay, HIGH);
      relayStatus = true;
      bot.sendMessage(CHAT_ID, "Relay ON", "");
    }

    if (text == "/off")
    {
      digitalWrite(relay, LOW);
      relayStatus = false;
      bot.sendMessage(CHAT_ID, "Relay OFF", "");
    }

    if (text == "/status")
    {
      if (relayStatus)
      {
        bot.sendMessage(CHAT_ID, "Relay status: ON", "");
      }
      else
      {
        bot.sendMessage(CHAT_ID, "Relay status: OFF", "");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("");

  pinMode(relay, OUTPUT);

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
  if (millis() - lastTime > interval)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTime = millis();
  }
}
