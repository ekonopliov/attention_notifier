#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

// Connecting to the internet
char * ssid = "iPhone X";
char * password = "12345678";

// Controlling the WS2812B

// Setting up the websocket client
WebSocketsClient webSocket;

void setup() {

  pinMode(BUILTIN_LED, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while(WiFi.status()!=WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initializing the websocket communication
// webSocket.begin("172.20.10.12", 8000, "/subscribe");

  webSocket.begin("attentionnotifier999.herokuapp.com", 80, "/subscribe");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(50); 
}

void loop() {
  // put your main code here, to run repeatedly:
  webSocket.loop();
  

} 

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      Serial.printf("[WSc] Connected to url: %s\n", payload);
      break;
    case WStype_TEXT:
      if((char)payload[0] == '1'){
      Serial.printf("Received button push!");
      webSocket.sendTXT("done");
      blinkTheLed();
      }
      if((char)payload[0] == '3'){
      Serial.printf("Received connection!");
      webSocket.sendTXT("connected");
      blinkTheLed();
      }
            
      break;
  }
}

void blinkTheLed(){
   for(int i = 0; i < 6; i++){
     digitalWrite(BUILTIN_LED, HIGH);
     delay(300);
     digitalWrite(BUILTIN_LED, LOW);
     delay(300);
    }
  }
