#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <Servo.h>

//Declaring servo
Servo servo;

// Connecting to the internet
char * ssid = "Your wifi SSID";
char * password = "Your wifi password";
int servo_position = 0;

// Setting up the websocket client
WebSocketsClient webSocket;

void setup() {

  pinMode(BUILTIN_LED, OUTPUT);
  servo.attach(D6);
  Serial.begin(115200);
  // WiFi.mode(WIFI_STA);  //use this option, if you are connecting to personal hotspot
  WiFi.begin(ssid, password);
  Serial.println("");
  while(WiFi.status()!=WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  blinkTheLed(2);
  servo.write(servo_position);
  
  Serial.println("");
  Serial.println("Connected to WiFi.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
// webSocket.begin("172.20.10.12", 8000, "/subscribe");  //webSocket for local testing

  webSocket.begin("Your Heroku cloud app name.herokuapp.com", 80, "/subscribe");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(10);
  
}

void loop() {
  webSocket.loop();
} 

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("Socket disconnected!\n");
      break;
    case WStype_CONNECTED:
      Serial.printf("Socket connected to url: %s\n", payload);
      servo.detach();
      break;
    case WStype_TEXT:
      if((char)payload[0] == '1'){
      Serial.println("Received button push!");
      webSocket.sendTXT("done");
      notify();
      }
      else if((char)payload[0] == '3'){
      Serial.println("New connection!");
      webSocket.sendTXT("connected");
      }
            
      break;
  }
}

void blinkTheLed(int howManyTimes){
   for(int i = 0; i < howManyTimes; i++){
     digitalWrite(BUILTIN_LED, HIGH);
     delay(150);
     digitalWrite(BUILTIN_LED, LOW);
     delay(150);
    }
  }

//Method that is called when notification arrives
void notify(){
     int a = 320;  //blinking start interval
  
     for(int i = 0; i < 15; i++){
       digitalWrite(BUILTIN_LED, HIGH);
       delay(a);
       digitalWrite(BUILTIN_LED, LOW);
       delay(a);
       a = a - 18;
      }
     
      
       digitalWrite(BUILTIN_LED, HIGH);
       wave(3);
       delay(6000);
       digitalWrite(BUILTIN_LED, LOW);
  }

  //Method that is used to simulate heart waving with servo
  void wave(int howManyTimes){
   servo.attach(D6);

 for(int i = 0; i < howManyTimes; i++){
  for (servo_position = 0; servo_position <=165; servo_position +=1){
    servo.write(servo_position);
    delay(3);

  }
  
  for (servo_position=165; servo_position >= 0; servo_position -=1){
    servo.write(servo_position);
    delay(3);
     }   
    }
    servo.detach();  //important to use this line. Otherwise servo motor will make noise after certain time.
  }
