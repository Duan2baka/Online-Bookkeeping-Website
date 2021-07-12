//IOT motion sensor

#include <WiFi.h>               // Wifi driver
#include <M5StickC.h>
#include <SPI.h>
#include <ButtonDebounce.h>     // Button Debounce library
#include <PubSubClient.h>       // MQTT server library
#include <ArduinoJson.h>        // JSON library
#include "LedMatrix.h"          // LED control library
#include <LEDMatrixDriver.hpp>
#define ID 9
#define TRIG 39
#define FUNC_KEY 37   
// MQTT and WiFi set-up
WiFiClient espClient;
PubSubClient client(espClient);
//Neotimer mytiot/Sensorimer(900000); // Set timer interrupt to 15min
// Key debounce set-up 
ButtonDebounce trigger(TRIG, 100);//IO debouncing
ButtonDebounce function_key(FUNC_KEY, 100); //IO debouncing 
const char *ssid = "icw502g";      // Your SSID             
const char *password = "8c122ase";  // Your Wifi password
const char *mqtt_server = "ia.ic.polyu.edu.hk"; // MQTT server name
const char* TEMP="0";
const char* Node_id="0";
const char* Loc="0";
const char* Humidity="0";
const char* Light="0";
const char* Sound="0";
const char* part="0";
const char* moved="0";
char *mqttTopic = "iot/Sensor";
float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float pitch = 0.0F;
float roll = 0.0F;
float yaw = 0.0F;

byte reconnect_count = 0;
long currentTime = 0;

char msg[100];
String ipAddress;
String macAddr;
String recMsg="";
String s1="";
String s2="";
StaticJsonDocument<1000> Jsondata; // Create a JSON document of 200 characters max
StaticJsonDocument<1000> jsonBuffer; 
//Set up the Wifi connection
void setup_wifi() {
  byte count = 0;
  
  WiFi.disconnect();
  delay(100);
  // We start by connecting to a WiFi network
  Serial.printf("\nConnecting to %s\n", ssid);
  WiFi.begin(ssid, password); // start the Wifi connection with defined SSID and PW

  // Indicate "......" during connecting
  // Restart if WiFi cannot be connected for 30sec
  currentTime = millis();
  M5.Lcd.setCursor(0,0);
  M5.Lcd.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    M5.Lcd.print(".");
    count++;
    if (count == 6) {
      count = 0;
      M5.Lcd.setCursor(0,0);
      M5.Lcd.print("Connecting       "); //clear the dots
      M5.Lcd.setCursor(0,0);
    }
      
    if (millis()-currentTime > 30000){
      ESP.restart();
    }
  }
  // Show "WiFi connected" once linked and light up LED1
  Serial.printf("\nWiFi connected\n");
  // Show IP address and MAC address
  ipAddress=WiFi.localIP().toString();
  Serial.printf("\nIP address: %s\n", ipAddress.c_str());
  macAddr=WiFi.macAddress();
  Serial.printf("MAC address: %s\n", macAddr.c_str());
  
  //Show in the small TFT
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.print("WiFi connected!");
  delay(3000);
}

// Routine to receive message from MQTT server
void callback(char* topic, byte* payload, unsigned int length) {
  
  recMsg ="";
  for (int i = 0; i < length; i++) {
    recMsg = recMsg + (char)payload[i];
  }

    DeserializationError error = deserializeJson(jsonBuffer, recMsg);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  Node_id=jsonBuffer["node_id"];
  s1=Node_id;
  if(s1=="A01")
  {
  Loc=jsonBuffer["loc"];
  TEMP=jsonBuffer["temp"];
  Humidity=jsonBuffer["hum"];
  Light=jsonBuffer["light"];
  Sound=jsonBuffer["snd"];
  s2=Loc;
  Serial.println(Node_id);
  Serial.println(Loc);
  Serial.println(TEMP);
  Serial.println(Humidity);
  Serial.println(Light);
  Serial.println(Sound);
  }
  part=jsonBuffer["PART"];
  moved=jsonBuffer["MOVED"];
  Serial.println(part);
  Serial.println(moved);
}


// Reconnect mechanism for MQTT Server
void reconnect() {
  // Loop until we're reconnected
  
  while (!client.connected()) {
    Serial.printf("Attempting MQTT connection...");
    // Attempt to connect
    //if (client.connect("ESP32Client")) {
    if (client.connect(macAddr.c_str())) {
      Serial.println("Connected");
      // Once connected, publish an announcement...
      snprintf(msg, 75, "IoT System (%s) is READY", ipAddress.c_str());
      client.subscribe(mqttTopic);
  
      delay(1000);
      client.publish(mqttTopic, msg);
      reconnect_count = 0;
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      reconnect_count++;
      
      //reconnect wifi by restart if retrial up to 5 times
      if (reconnect_count == 5){
        ESP.restart();//reset if not connected to server 
      }
        
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void check_Accel(){
  if ((abs(accX)>0.2)|(abs(accY)>0.2)|(abs(accZ-1)>0.2)) {
   Jsondata["MOVED"] = "Yes";
   //Serial.println("The part is moved!!");
   
   // Packing the JSON message into msg
   serializeJson(Jsondata, Serial);
   serializeJson(Jsondata, msg);
      
   //Publish msg to MQTT server
 client.publish(mqttTopic, msg);
   Serial.println();
  }
  else
    Jsondata["MOVED"] = "No";
    //Serial.println("No movement detected!!");
   
    // Packing the JSON message into msg
   serializeJson(Jsondata, Serial);
    serializeJson(Jsondata, msg);
      
    //Publish msg to MQTT server
    client.publish(mqttTopic, msg);
    Serial.println();
}

void setup() {
  //pinMode(TRIG, INPUT);
  //pinMode(FUNC_KEY, INPUT);
  //pinMode(LED1, OUTPUT);

  //digitalWrite(LED1, LOW);
  
  Serial.begin(115200); 
  Serial.println("System Start!");  

  M5.begin();
  M5.IMU.Init();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);

  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  //Initalize Json message
  Jsondata["PART"] = ID;
  Jsondata["MOVED"] = "No"; 

  M5.Lcd.setCursor(0, 10);
  M5.Lcd.println("node_id   loc    temp");
  M5.Lcd.setCursor(0, 50);
  M5.Lcd.println("  hum    light   sound");
  
}

void loop() {
  M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
  M5.IMU.getAccelData(&accX,&accY,&accZ);
  M5.IMU.getAhrsData(&pitch,&roll,&yaw);
  float a=atof(Humidity);
  int b=atoi(Light);
  int c=atoi(Sound);
  float d=atof(TEMP);
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.printf("  %s    %s   %3.1f ", s1, s2, d);
  M5.Lcd.setCursor(0, 60);
  M5.Lcd.printf(" %5.1f  %5d  %5d", a, b,c);


 check_Accel();
  
   if (!client.connected()){
    reconnect();
   }
   client.loop();

}
