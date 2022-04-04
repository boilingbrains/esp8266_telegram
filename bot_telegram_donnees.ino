#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <DHT.h>

#define WIFI_SSID "XXXXXXXXXXXXXXXXXXXXXXXX"
#define WIFI_PASSWORD "XXXXXXXXXXXXXXXXXXXXXXXXX"
#define BOTtoken "XXXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
#define CHAT_ID "XXXXXXXXX"

#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht (DHTPIN, DHTTYPE);

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOTtoken, secured_client);

const long interval = 1000;
long lastTime = 0;


String data_sensor() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  String message = "Temperature = " + String(temperature) + "°C\n";
  message += "Humidity = " + String(humidity) + "%\n";
  return message;
}

void handleNewMessages(int numNewMessages) {

  for (int i = 0; i < numNewMessages; i++)
  {
    String text = bot.messages[i].text;

    if (text == "/sensor")
    {
      bot.sendMessage(CHAT_ID, data_sensor(), "");
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("");

  dht.begin();

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
