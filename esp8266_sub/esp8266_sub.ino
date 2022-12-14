#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define BLUE_LED_PIN 2

// WiFi
const char *ssid = "";
const char *password = "";

// MQTT Broker
const char *mqtt_broker = "";
const char *topic = "";
const char *mqtt_username = "foobar";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  // Initialise the BLUE_LED_PIN as an output
  pinMode(BLUE_LED_PIN, OUTPUT);
  // Set baudrate to 115200;
  Serial.begin(115200);
  // Connect to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi network.");
  // Connect to a MQTT broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected())
  {
      String client_id = "esp8266-sub-";
      client_id += String(WiFi.macAddress());
      Serial.printf("Client %s connected to the public MQTT broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
      {
          Serial.println("Public mosquitto MQTT broker connected.");
      }
      else
      {
          Serial.print("Error: client state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
  // subscribe the topic
  client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.println("-----------------------");
  Serial.print("New message in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (int i = 0; i < length; i++)
  {
      Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");

  // Blink LED
  for (int i = 0; i < 10; i++)
  {
      digitalWrite(BLUE_LED_PIN, !digitalRead(BLUE_LED_PIN));
      delay(500);
  }
}

void loop()
{
  client.loop();
}
