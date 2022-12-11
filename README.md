# mqtt_repo
Sample MQTT project with Raspberry Pi as broker and ESP8266 as clients.

## Project overview
The diagram below ilustrates our system components and flow.

![](/images/system_diagram.jpg "system diagram")

Here comes a few assumptions:
1. All devices are connected to the same local network.
2. The Mosquitto MQTT broker is installed on Raspberry Pi.
3. ESP8266 publisher board publish a message to the topic when the flash button is pressed.
4. Another ESP8266 board is subscribed to the same topic and flashes the LED when receives a message.

## Dependencies
Before we start with practical part, we need to install some dependencies in Arduino IDE.
1. In Arduino IDE, navigate to *File -> Preferences* and enter https://arduino.esp8266.com/stable/package_esp8266com_index.json into the "Additional Boards Manager URLs".
2. Navigate to *Tools -> Board -> Boards Manager* and install "esp8266 by ESP8266 Community" library.
3. Choose "Generic ESP8266 Module" in *Tools -> Boards*.
4. Navigate to *Sketch -> Incluide Library -> Manage libraries* and install "PubSubClient by Nick O-Leary" library.

## Part 1
Mosquitto is a lightweight open source message broker that implements MQTT. It has been installed on our Raspberry Pi. Let's see if it works.\
Open terminal and connect to Raspberry Pi using ssh:
```
ssh pi@raspberrypi.local
```
Authenticate with password:
```
raspberry
```
Subscribe to an MQTT topic:
```
mosquitto_sub -d -t topic_name
```
Open another terminal and publish a sample message to *testTopic*:
```
pi@raspberry:~$ mosquitto_pub -d -t topic_name -m "message"
```
The message should appear in the first terminal.

## Part 2
In this part we will flash some code to our ESP boards. You can find two files in the repository - *esp8266_pub.ino* and *esp8266_sub.ino* - that contain publisher and subscriber code respectively. Choose one and open in Arduino IDE.\
Firstly, we need to define our network credentials. Spot that part in the code and insert following credentials:
| SSID          | Password     |
| ------------- | ------------ |
| lab301        | l@b30!l@b30! |

Next, find a block of code where MQTT data is defined and fill it with proper data. Provide MQTT broker address in particular (assign it to `const char *mqtt_broker` variable).

Examine the code and flash it to your devices. Try to communicate between publishers and subscribers within the network. Use the peripherals defined in the code. Watch serial monitor as well.
