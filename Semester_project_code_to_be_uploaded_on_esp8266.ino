//Made by Ali Raza MIT License
/*this code is a general version in which we assume that all the sensors are connected with*/ 
/*single esp8266, This could chane in future by only publishing single sensor data in single feed using single 
esp8266 after its done practically. For the part of where we get the data from is we gonna use the Arduino uno 
Serial communication and esp serial communication, therefore using arduino uno we gonna get the data from sensors 
and then from that arduino we gonna use the uart pins to send that sensor data of variables into the esp 8266 and 
then upload it with mqtt protocol to adafruit io */


#include <SoftwareSerial.h>
#include <ArduinoJson.h>

//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6, D5);



 //importing libraries
 //this code is a general version in which we assume that all the sensors are connected with single esp8266, This could chane in future by only publishing single sensor data in single feed using single esp8266 after its done practically.
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

//declaring the variables


/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "test"  //ssid of the wifi u are connecting with
#define WLAN_PASS       "pakistan"  //password

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "lionheart222"  //Adrafuit io key
#define AIO_KEY         "aio_zpyS091QvndA5YozHHCJ5zqkI1OU"  //adafruit io key to acesss the dashboard

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client; //client is object of class wifi client to connect with the mqtt server of adafruit 
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);  //instatiate the object of Adafruit_MQTT_Clientlibrary with data to connect with adafruit mqtt client 

/****************************** Feeds ***************************************/
//here comes the important part we edit the code below to get to the required feed in whic hwe are going to collect the data
// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname> (important)!!!!!!!!
Adafruit_MQTT_Publish temprature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temprature"); //here we make feed
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity"); //dht11
Adafruit_MQTT_Publish temp_bmp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temp_bmp"); //bmo
Adafruit_MQTT_Publish alltitude_bmp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/alltitude_bmp");
Adafruit_MQTT_Publish Gpressure = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Gpressure");
Adafruit_MQTT_Publish Apressure = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Apressure");
// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff"); //this to keep log of esp if it was on or off

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect(); //

void setup() {
  Serial.begin(9600);
  nodemcu.begin(9600);
  Serial.println("This is printing");
  while (!Serial) continue; //buad rate rate at which we gonna transfer he data this may be changed
  delay(10);  //delay 

  Serial.println(F("Adafruit MQTT implementation"));  //serial print
 
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);  //this bit helps us to connect esp with wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onoffbutton);
}

uint32_t x=0;  // unit32_t is a numeric type that guarantees 32 bits. The value is unsigned, meaning that the range of values goes from 0 to 232 - 1.

void loop() {

//serial com btw the arduino and esp
// put your main code here, to run repeatedly:
StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(nodemcu);

  if (data == JsonObject::invalid()) {
    //Serial.println("Invalid Json Object");
    jsonBuffer.clear();
    return;
  }

  Serial.println("JSON Object Recieved");
  Serial.print("Recieved Humidity:  ");
   double hum = data["humidity"];
  Serial.println(hum);
  Serial.print("Recieved Temperature:  ");
   double temp = data["temperature"];
  Serial.println(temp);
  Serial.print("Recieved ntemp_bmp:  ");
   double T = data["temp_bmp"];
  Serial.println(T);
  Serial.print("Recieved alltitude_bmp:  ");
   double a = data["alltitude_bmp"];
  Serial.println(a);
  Serial.print("Recieved Apressure:  ");
   double P = data["Apressure"];
  Serial.println(a);
  Serial.print("Recieved Gpressure:  ");
   double p0 = data["Gpressure"];
  Serial.println(a);
  Serial.println("-----------------------------------------");


  
  
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
    }
  }


  

  
  // publish the data on seriel monitor as well to monitor it
  Serial.println(F("\nTemperature: "));
  Serial.println(temp);
  Serial.println(F("\nHumidity: "));
  Serial.println(hum);
  Serial.println(F("\ntemp_bmp: "));
  Serial.println(T);
  Serial.println(F("\nalltitude_bmp: "));
  Serial.println(a);
  Serial.println(F("\nApressure: "));
  Serial.println(P);
  Serial.println(F("\nGpressure: "));
  Serial.println(p0);
  
// sending data to feed at Adafruit.io with pusblish method
  delay(90*1000); //delay of 3 minutes
  temprature.publish(temp);
  humidity.publish(hum);
  temp_bmp.publish(T);
  alltitude_bmp.publish(a);
  Gpressure.publish(P);
  Apressure.publish(p0);

  

  // Now we can publish stuff!
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  if (! temprature.publish(x++)) {
    Serial.println(F("Failed to send temp"));
  } else {
    Serial.println(F("OK!"));
  }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
