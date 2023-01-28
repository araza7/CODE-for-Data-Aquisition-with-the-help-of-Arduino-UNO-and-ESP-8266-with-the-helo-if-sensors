# CODE-for-Data-Aquisition-with-the-help-of-Arduino-UNO-and-ESP-8266-with-the-helo-if-sensors
This Code is written for Arduino IDE, for the Data aquisition for our Semester project of Predictive maintainence with AI. in this Code we use and Arduino UNO and Esp 8266 in such a way that Arduino UNO gets data from the sensors and then with the help of Serialization we send that data to ESP 8266 and then store it on feeds of Adafruit.io
# The Code that is to be uploaded on Arduino Uno
It is a sketch written in the Arduino programming language for the Arduino Uno board. It uses the SoftwareSerial library to communicate with a NodeMCU board, the DHT library to read temperature and humidity data from a DHT11 sensor, the SFE_BMP180 library to read pressure data from a BMP180 sensor, and the ArduinoJson library to parse JSON data. The sketch initializes the sensor, read the temperature, humidity and pressure data in the loop and print them to the serial monitor. It also calculates the altitude from the pressure data by providing a known baseline pressure.

# The code that is to be uploaded on esp 8266 
it is an Arduino sketch that utilizes the ESP8266 WiFi module to connect to an Adafruit IO MQTT server and publish sensor data. The sketch uses the SoftwareSerial library to communicate with the ESP8266, the ArduinoJson library to parse JSON data, the ESP8266WiFi library to connect to a WiFi network, and the Adafruit MQTT library to interact with the Adafruit IO MQTT server. The sketch defines several global variables for the WiFi network SSID and password, the Adafruit IO server and port, the Adafruit IO username and key, and several Adafruit_MQTT_Publish and Adafruit_MQTT_Subscribe objects for each sensor and for the on/off button. The setup() function configures the serial communication and connects to the WiFi network, and the MQTT_connect() function connects to the Adafruit IO MQTT server and subscribes to the on/off button feed.



