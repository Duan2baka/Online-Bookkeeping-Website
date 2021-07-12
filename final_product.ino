/*****************************************************************************
Smart IOT Tag to show staff's status
Using ESP8266

This sketch connects the ESP8266 to a MQTT broker and subscribes to the topic 
/IC/TRIAL. When the button is pressed, the client will toggle among publishing
"Available", "Busy", "Online" and "Leave". When the Json message is received, 
the LED matrix displays "A", "B", "O" and "L", respectively. 
*******************************************************************************/

#include <SPI.h>
#include <ButtonDebounce.h>     // Button Debounce library
#include <ESP8266WiFi.h>        // 8266 Wifi driver
#include <PubSubClient.h>       // MQTT server library
#include <ArduinoJson.h>        // JSON library
#include "LedMatrix.h"          // LED control library
#include <LEDMatrixDriver.hpp>
#define NUMBER_OF_DEVICES 1
#define CS_PIN D4

#define red_light_pin D0    // red light is connected to D0
#define green_light_pin D8  // green light is connected to D8
#define blue_light_pin D3   // blue light is connected to D3
#define TRIG D2             // swith is connected to D2
#define ID 5

LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);

// MQTT and WiFi set-up
WiFiClient espClient;
PubSubClient client(espClient); 
ButtonDebounce trigger(TRIG, 100);

const char *ssid = "icw502g";                           
const char *password = "8c122ase";
const char *mqtt_server = "ia.ic.polyu.edu.hk"; 
char *mqttTopic = "iot/Sensor";                   

byte reconnect_count = 0;
int count = 0;
long int currentTime = 0;

char msg[200];
String ipAddress;
String macAddr;
String recMsg="";

int buttonState;      // variable to hold the button state
int Mode = 0;         // what mode is the light in?
boolean keypress = 1;
const char* TEMP="0";
const char* Node_id="0";
const char* Loc="0";
const char* Humidity="0";
const char* Light="0";
const char* Sound="0";
const char* part="0";
const char* moved="0";
//The text
const uint8_t LEDMATRIX_CS_PIN = D4;
const int LEDMATRIX_SEGMENTS = 4;
const int LEDMATRIX_WIDTH    = LEDMATRIX_SEGMENTS * 8;
LEDMatrixDriver lmd(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN);
const int ANIM_DELAY = 70;
int x=0, y=0;   // start top left
byte font[95][8] = { {0,0,0,0,0,0,0,0}, // SPACE
                     {0x10,0x18,0x18,0x18,0x18,0x00,0x18,0x18}, // EXCL
                     {0x28,0x28,0x08,0x00,0x00,0x00,0x00,0x00}, // QUOT
                     {0x00,0x0a,0x7f,0x14,0x28,0xfe,0x50,0x00}, // #
                     {0x10,0x38,0x54,0x70,0x1c,0x54,0x38,0x10}, // $
                     {0x00,0x60,0x66,0x08,0x10,0x66,0x06,0x00}, // %
                     {0,0,0,0,0,0,0,0}, // &
                     {0x00,0x10,0x18,0x18,0x08,0x00,0x00,0x00}, // '
                     {0x02,0x04,0x08,0x08,0x08,0x08,0x08,0x04}, // (
                     {0x40,0x20,0x10,0x10,0x10,0x10,0x10,0x20}, // )
                     {0x00,0x10,0x54,0x38,0x10,0x38,0x54,0x10}, // *
                     {0x00,0x08,0x08,0x08,0x7f,0x08,0x08,0x08}, // +
                     {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x08}, // COMMA
                     {0x00,0x00,0x00,0x00,0x7e,0x00,0x00,0x00}, // -
                     {0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06}, // DOT
                     {0x00,0x04,0x04,0x08,0x10,0x20,0x40,0x40}, // /
                     {0x00,0x38,0x44,0x4c,0x54,0x64,0x44,0x38}, // 0
                     {0x04,0x0c,0x14,0x24,0x04,0x04,0x04,0x04}, // 1
                     {0x00,0x30,0x48,0x04,0x04,0x38,0x40,0x7c}, // 2
                     {0x00,0x38,0x04,0x04,0x18,0x04,0x44,0x38}, // 3
                     {0x00,0x04,0x0c,0x14,0x24,0x7e,0x04,0x04}, // 4
                     {0x00,0x7c,0x40,0x40,0x78,0x04,0x04,0x38}, // 5
                     {0x00,0x38,0x40,0x40,0x78,0x44,0x44,0x38}, // 6
                     {0x00,0x7c,0x04,0x04,0x08,0x08,0x10,0x10}, // 7
                     {0x00,0x3c,0x44,0x44,0x38,0x44,0x44,0x78}, // 8
                     {0x00,0x38,0x44,0x44,0x3c,0x04,0x04,0x78}, // 9
                     {0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x00}, // :
                     {0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x08}, // ;
                     {0x00,0x10,0x20,0x40,0x80,0x40,0x20,0x10}, // <
                     {0x00,0x00,0x7e,0x00,0x00,0xfc,0x00,0x00}, // =
                     {0x00,0x08,0x04,0x02,0x01,0x02,0x04,0x08}, // >
                     {0x00,0x38,0x44,0x04,0x08,0x10,0x00,0x10}, // ?
                     {0x00,0x30,0x48,0xba,0xba,0x84,0x78,0x00}, // @
                     {0x00,0x1c,0x22,0x42,0x42,0x7e,0x42,0x42}, // A
                     {0x00,0x78,0x44,0x44,0x78,0x44,0x44,0x7c}, // B
                     {0x00,0x3c,0x44,0x40,0x40,0x40,0x44,0x7c}, // C
                     {0x00,0x7c,0x42,0x42,0x42,0x42,0x44,0x78}, // D
                     {0x00,0x78,0x40,0x40,0x70,0x40,0x40,0x7c}, // E
                     {0x00,0x7c,0x40,0x40,0x78,0x40,0x40,0x40}, // F
                     {0x00,0x3c,0x40,0x40,0x5c,0x44,0x44,0x78}, // G
                     {0x00,0x42,0x42,0x42,0x7e,0x42,0x42,0x42}, // H
                     {0x00,0x7c,0x10,0x10,0x10,0x10,0x10,0x7e}, // I
                     {0x00,0x7e,0x02,0x02,0x02,0x02,0x04,0x38}, // J
                     {0x00,0x44,0x48,0x50,0x60,0x50,0x48,0x44}, // K
                     {0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x7c}, // L
                     {0x00,0x82,0xc6,0xaa,0x92,0x82,0x82,0x82}, // M
                     {0x00,0x42,0x42,0x62,0x52,0x4a,0x46,0x42}, // N
                     {0x00,0x3c,0x42,0x42,0x42,0x42,0x44,0x38}, // O
                     {0x00,0x78,0x44,0x44,0x48,0x70,0x40,0x40}, // P
                     {0x00,0x3c,0x42,0x42,0x52,0x4a,0x44,0x3a}, // Q
                     {0x00,0x78,0x44,0x44,0x78,0x50,0x48,0x44}, // R
                     {0x00,0x38,0x40,0x40,0x38,0x04,0x04,0x78}, // S
                     {0x00,0x7e,0x90,0x10,0x10,0x10,0x10,0x10}, // T
                     {0x00,0x42,0x42,0x42,0x42,0x42,0x42,0x3e}, // U
                     {0x00,0x42,0x42,0x42,0x42,0x44,0x28,0x10}, // V
                     {0x80,0x82,0x82,0x92,0x92,0x92,0x94,0x78}, // W
                     {0x00,0x42,0x42,0x24,0x18,0x24,0x42,0x42}, // X
                     {0x00,0x44,0x44,0x28,0x10,0x10,0x10,0x10}, // Y
                     {0x00,0x7c,0x04,0x08,0x7c,0x20,0x40,0xfe}, // Z
                            };   

void drawString(char* text, int len, int x, int y )
{
  for( int idx = 0; idx < len; idx ++ )
  {
    int c = text[idx] - 32;

    // stop if char is outside visible area
    if( x + idx * 8  > LEDMATRIX_WIDTH )
      return;

    // only draw if char is visible
    if( 8 + x + idx * 8 > 0 )
      drawSprite( font[c], x + idx * 8, y, 8, 8 );
  }
}

void drawSprite( byte* sprite, int x, int y, int width, int height )
{
  // The mask is used to get the column bit from the sprite row
  byte mask = B10000000;

  for( int iy = 0; iy < height; iy++ )
  {
    for( int ix = 0; ix < width; ix++ )
    {
      lmd.setPixel(x + ix, y + iy, (bool)(sprite[iy] & mask ));

      // shift the mask by one pixel to the right
      mask = mask >> 1;
    }

    // reset column mask
    mask = B10000000;
  }
}

StaticJsonDocument<1000> Jsondata; 
StaticJsonDocument<1000> jsonBuffer; 


//Set up the Wifi connection
void setup_wifi() {
  WiFi.disconnect();
  delay(100);
  Serial.printf("\nConnecting to %s\n", ssid);
  WiFi.begin(ssid, password); // start the Wifi connection with defined SSID and PW

  // Indicate "......" during connecting and flashing LED1
  // Restart if WiFi cannot be connected for 30sec
  currentTime = millis();
  
  // Show IP address and MAC address
  ipAddress=WiFi.localIP().toString();
  Serial.printf("\nIP address: %s\n", ipAddress.c_str());
  macAddr=WiFi.macAddress();
  Serial.printf("MAC address: %s\n", macAddr.c_str());
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
  String s=Node_id;
  if(s=="A01")
  {
  Loc=jsonBuffer["loc"];
  TEMP=jsonBuffer["temp"];
  Humidity=jsonBuffer["hum"];
  Light=jsonBuffer["light"];
  Sound=jsonBuffer["snd"];

  Serial.println(Node_id);
  Serial.println(Loc);
  Serial.println(TEMP);
  Serial.println(Humidity);
  Serial.println(Light);
  Serial.println(Sound);
  keypress=0;
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
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      reconnect_count++;
      
      //Reconnect wifi by restart if retrial up to 5 times
      if (reconnect_count == 5){
        ESP.restart(); // Reset if not connected to server 
      }
        
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Button control
void buttonChanged(int state){
  if (digitalRead(TRIG)== 0 && keypress==1) {  // If key is pressed and last key is processed 
    Mode++;
    if (Mode == 4) Mode=0;      // Reset Mode value
    keypress=0;
    Serial.print(Mode);
    //ledMatrix.clear();

  }
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
void setup() {
  lmd.setEnabled(true);
  lmd.setIntensity(2);   // 0 = low, 10 = high
  pinMode(TRIG, INPUT_PULLUP);          // Configure TRIG as an pull-up input
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
  
  digitalWrite(red_light_pin, HIGH);
  digitalWrite(green_light_pin, HIGH);
  digitalWrite(blue_light_pin, HIGH);

  buttonState = digitalRead(TRIG);      // Read the initial state
  
  Serial.begin(115200);                 // State serial communication at 115200 baud
  Serial.println("System Start!");

  //Initiate the display first
  ledMatrix.init();                             // Initialize the SPI interface
  ledMatrix.setIntensity(4);                    // Light intensity: 0 - 15
  ledMatrix.setTextAlignment(TEXT_ALIGN_LEFT);  // Text is aligned to left side of the display

  //Initial state is available
  digitalWrite(green_light_pin, LOW); 
  digitalWrite(red_light_pin, LOW);
  digitalWrite(blue_light_pin, LOW);
  
  client.setCallback(callback);
  trigger.setCallback(buttonChanged);

  setup_wifi();                         // Connect to network
  client.setServer(mqtt_server, 1883);
ledMatrix.clear();

}
void light()
{
    digitalWrite(green_light_pin, HIGH); 
  digitalWrite(red_light_pin, HIGH);
  digitalWrite(blue_light_pin, HIGH);   
  String Moved=moved;
char text[] = "ATTENTION";
  int len = strlen(text);
    digitalWrite(red_light_pin,HIGH); 

       delay(60); 

       digitalWrite(red_light_pin,LOW); 

       delay(60);
         drawString(text, len, x, 0);
  lmd.display();
    if( --x < len * -8 ) {
        x = LEDMATRIX_WIDTH;
    }
  
    
}
void loop() {
  trigger.update();
  if (!client.connected()){  // Reconnect if connection is lost
    reconnect();
  }
  client.loop();
String Moved=moved;
if(Moved=="Yes")
{
  light();
  digitalWrite(green_light_pin, HIGH); 
  digitalWrite(red_light_pin, HIGH);
  digitalWrite(blue_light_pin, HIGH);   
  ledMatrix.clear();
}
if(keypress==0)
{
  if(Mode==2)
{
  //light test
  int a=atoi(Light);
  if(a>60)
  {
     
        digitalWrite(green_light_pin, HIGH); 
        digitalWrite(red_light_pin, LOW);
        digitalWrite(blue_light_pin, HIGH);   
  // Draw the text to the current position
  char text[] = "LIGHT:STRONG";
  int len = strlen(text);
  int times=0;
  while(times<=1)
  {
  drawString(text, len, x, 0);
  lmd.display();
  delay(ANIM_DELAY);
    if( --x < len * -8 ) {
        x = LEDMATRIX_WIDTH;
        times++;
    }
  }
   keypress=1;
}
  if(a<=60&&a>=30)
  {
     
        digitalWrite(green_light_pin, LOW); 
        digitalWrite(red_light_pin, HIGH);
        digitalWrite(blue_light_pin, HIGH);   
  // Draw the text to the current position
  char text[] = "LIGHT:NORMAL";
  int len = strlen(text);
  int times=0;
  while(times<=1)
  {
  drawString(text, len, x, 0);
  lmd.display();
  delay(ANIM_DELAY);
    if( --x < len * -8 ) {
        x = LEDMATRIX_WIDTH;
        times++;
    }
  }
 keypress=1;
}

  if(a<30)
  {
     
        digitalWrite(green_light_pin, HIGH); 
        digitalWrite(red_light_pin, HIGH);
        digitalWrite(blue_light_pin, LOW);   
  // Draw the text to the current position
  char text[] = "LIGHT:WEAK";
  int len = strlen(text);
  int times=0;
  while(times<=1)
  {
  drawString(text, len, x, 0);
  lmd.display();
  delay(ANIM_DELAY);
    if( --x < len * -8 ) {
        x = LEDMATRIX_WIDTH;
        times++;
    }
  }
   keypress=1;
}
}
if(Mode==0)
{
//temp test
float a=atof(TEMP);
if(a>30.0)
  {
        digitalWrite(green_light_pin, HIGH); 
        digitalWrite(red_light_pin, LOW);
        digitalWrite(blue_light_pin, HIGH);   
  // Draw the text to the current position
  char text[] = "TEMPERATURE:HOT";
  int len = strlen(text);
   int times=0;
  while(times<=1)
  {
  drawString(text, len, x, 0);
  lmd.display();
  delay(ANIM_DELAY);
    if( --x < len * -8 ) {
        x = LEDMATRIX_WIDTH;
        times++;
    }
  }
  keypress=1;
}
if(a>24.0&&a<=30.0)
  {
        digitalWrite(green_light_pin, LOW); 
        digitalWrite(red_light_pin, HIGH);
        digitalWrite(blue_light_pin, HIGH);   
  char text[] = "TEMPERATURE:COMFORTABLE";
  int len = strlen(text);
  int times=0;
  while(times<=1)
  {
  drawString(text, len, x, 0);
  lmd.display();
  delay(ANIM_DELAY);
    if( --x < len * -8 ) {
        x = LEDMATRIX_WIDTH;
        times++;
    }
  }
  keypress=1;
}
if(a<=24.0)
  {
        digitalWrite(green_light_pin,HIGH); 
        digitalWrite(red_light_pin, HIGH);
        digitalWrite(blue_light_pin, LOW);   
  char text[] = "TEMPERATURE:COLD";
  int len = strlen(text);
  int times=0;
  while(times<=1)
  {
  drawString(text, len, x, 0);
  lmd.display();
  delay(ANIM_DELAY);
    if( --x < len * -8 ) {
        x = LEDMATRIX_WIDTH;
        times++;
    }
  }
  keypress=1;
}

}
  

if(Mode==1)
{
 //humidity test
 float a=atof(Humidity);
 if(a>60.0)
  {
     
        digitalWrite(green_light_pin, HIGH); 
        digitalWrite(red_light_pin, LOW);
        digitalWrite(blue_light_pin, HIGH);   
  // Draw the text to the current position
  char text[] = "HUMIDITY:WET";
  int len = strlen(text);
  int times=0;
  while(times<=1)
  {
  drawString(text, len, x, 0);
  lmd.display();
  delay(ANIM_DELAY);
    if( --x < len * -8 ) {
        x = LEDMATRIX_WIDTH;
        times++;
    }
  }
   keypress=1;
}
  if(a<=60.0&&a>=30.0)
  {
     
        digitalWrite(green_light_pin, LOW); 
        digitalWrite(red_light_pin, HIGH);
        digitalWrite(blue_light_pin, HIGH);   
  // Draw the text to the current position
  char text[] = "HUMIDITY:COMFORTABLE";
  int len = strlen(text);
  int times=0;
  while(times<=1)
  {
  drawString(text, len, x, 0);
  lmd.display();
  delay(ANIM_DELAY);
    if( --x < len * -8 ) {
        x = LEDMATRIX_WIDTH;
        times++;
    }
  }
 keypress=1;
}

  if(a<30.0)
  {
     
        digitalWrite(green_light_pin, HIGH); 
        digitalWrite(red_light_pin, HIGH);
        digitalWrite(blue_light_pin, LOW);   
  // Draw the text to the current position
  char text[] = "HUMIDITY:DRY";
  int len = strlen(text);
  int times=0;
  while(times<=1)
  {
  drawString(text, len, x, 0);
  lmd.display();
  delay(ANIM_DELAY);
    if( --x < len * -8 ) {
        x = LEDMATRIX_WIDTH;
        times++;
    }
  }
   keypress=1;
}

}


if(Mode==3)
{
 //snd test
 int a=atoi(Sound);
 if(a>55)
  {
     
        digitalWrite(green_light_pin, HIGH); 
        digitalWrite(red_light_pin, LOW);
        digitalWrite(blue_light_pin, HIGH);   
  // Draw the text to the current position
  char text[] = "SOUND:NOISY";
  int len = strlen(text);
  int times=0;
  while(times<=1)
  {
  drawString(text, len, x, 0);
  lmd.display();
  delay(ANIM_DELAY);
    if( --x < len * -8 ) {
        x = LEDMATRIX_WIDTH;
        times++;
    }
  }
   keypress=1;
}
  if(a>-20&&a<=55)
  {
     
        digitalWrite(green_light_pin, LOW); 
        digitalWrite(red_light_pin, HIGH);
        digitalWrite(blue_light_pin, HIGH);   
  // Draw the text to the current position
  char text[] = "SOUND:NORMAL";
  int len = strlen(text);
  int times=0;
  while(times<=1)
  {
  drawString(text, len, x, 0);
  lmd.display();
  delay(ANIM_DELAY);
    if( --x < len * -8 ) {
        x = LEDMATRIX_WIDTH;
        times++;
    }
  }
 keypress=1;
}

  if(a<=-20)
  {
     
        digitalWrite(green_light_pin, HIGH); 
        digitalWrite(red_light_pin, HIGH);
        digitalWrite(blue_light_pin, LOW);   
  // Draw the text to the current position
  char text[] = "SOUND:QUIET";
  int len = strlen(text);
  int times=0;
  while(times<=1)
  {
  drawString(text, len, x, 0);
  lmd.display();
  delay(ANIM_DELAY);
    if( --x < len * -8 ) {
        x = LEDMATRIX_WIDTH;
        times++;
    }
  }
   keypress=1;
}
}

}
}


  
