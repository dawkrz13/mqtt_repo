#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define BLUE_LED_PIN 2
#define DHTPIN 5
#define DHTTYPE DHT11
#define MSG_MAX_SIZE 40

// WiFi
const char *ssid = "";
const char *password = "";

// MQTT Broker
const char *mqtt_broker = "";
const char *topic_req = "dht11_req";
const char *topic_rsp = "dht11_rsp";
const char *mqtt_username = "esp_1";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

// temperature and humidity
struct DHT_info
{
  float t = 0.0;
  float h = 0.0;
};
DHT_info dht_i;
// last time DHT was updated
unsigned long prev_time_ms = 0;
// DHT update interval (10s)
const long update_interval = 10000;

char msg[MSG_MAX_SIZE];

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
  client.subscribe(topic_req);
  // Initialise DHT
  dht.begin();
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

  // Send response
  snprintf(msg, MSG_MAX_SIZE, "temperature = %2.2f, humidity = %2.2f", dht_i.t, dht_i.h);
  client.publish(topic_rsp, msg);
}

void loop()
{
  unsigned long current_time_ms = millis();
  if (current_time_ms - prev_time_ms >= update_interval)
  {
    prev_time_ms = current_time_ms;
    float latest_t = dht.readTemperature();
    if (isnan(latest_t))
    {
      Serial.println("Temperature read failed.");
    }
    else
    {
      dht_i.t = latest_t;
    }
    float latest_h = dht.readHumidity();
    if (isnan(latest_h))
    {
      Serial.println("Humidity read failed.");
    }
    else
    {
      dht_i.h = latest_h;
    }
  }

  client.loop();
}
