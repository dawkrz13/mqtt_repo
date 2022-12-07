# mqtt_repo
Sample MQTT project with Raspberry Pi as broker and ESP8266 as clients.

1. Plik -> Preferencje -> Dodatkowe adresy URL...: https://arduino.esp8266.com/stable/package_esp8266com_index.json
2. Narzędzia -> Menadżer płytek -> ESP8266
3. Narzędzia -> Płytka -> Generic ESP8266 Module
4. Szkic -> Dołącz bibliotekę -> Zarządzaj bibliotekami: PubSubClient

Lab WiFi login data:\
ssid: Lab301\
password: l@b30!l@b30!

MQTT broker test:\
ssh pi@raspberrypi.local

Terminal 1: Subscribe to topic:\
mosquitto_sub -d -t topic_name

Terminal 2: Publish message:\
mosquitto_pub -d -t topic_name -m "message"
