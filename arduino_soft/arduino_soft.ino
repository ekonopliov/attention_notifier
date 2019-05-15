#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

// Connecting to the internet
char * ssid = "iPhone X";
char * password = "12345678";
int notificationPin = 7;

// Controlling the WS2812B

// Setting up the websocket client
WebSocketsClient webSocket;

void setup() {

  pinMode(BUILTIN_LED, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while(WiFi.status()!=WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("Connected to WiFi.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
// webSocket.begin("172.20.10.12", 8000, "/subscribe");

  webSocket.begin("attentionnotifier999.herokuapp.com", 80, "/subscribe");
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
      break;
    case WStype_TEXT:
      if((char)payload[0] == '1'){
      Serial.println("Received button push!");
      webSocket.sendTXT("done");
      blinkTheLed(3);
      notify();
      }
      else if((char)payload[0] == '3'){
      Serial.println("New connection!");
      webSocket.sendTXT("connected");
      blinkTheLed(6);
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

void notify(){
     for(int i = 0; i < 5; i++){
       digitalWrite(notificationPin, HIGH);
       delay(250);
       digitalWrite(notificationPin, LOW);
       delay(250);
      }
  }
