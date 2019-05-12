#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

// Connecting to the internet
char * ssid = "iPhone X";
char * password = "12345678";
char * returnedString;

bool garageClosed = "false";
bool gateClosed = "false";


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
  webSocket.begin("172.20.10.12", 8000, "/subscribe");

  //webSocket.begin("attentionnotifier999.herokuapp.com", 80, "/subscribe");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(300); 
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
      blinkTheLed(5);
      break;
    case WStype_TEXT:
      if((char)payload[0] == '1' || (char)payload[0] == '2' || (char)payload[0] == '3' || (char)payload[0] == '4'){
      performAction((char)payload[0]);
      blinkTheLed(2);
      }
      break;
  }
}

void performAction(char actionNumber){
  switch(actionNumber){
    
    case '1':
     Serial.println("Gate action received!");
     if(!gateClosed){
      webSocket.sendTXT("gate closing");
      gatePush();
      webSocket.sendTXT("gate closed");
      gateClosed = true;
      }
      else{
      webSocket.sendTXT("gate opening");
      gatePush();
      webSocket.sendTXT("gate opened");
      gateClosed = false;
      }
     break;
     
    case '2':
     Serial.println("Garage action received!");
     if(!garageClosed){
      webSocket.sendTXT("garage closing");
      garagePush();
      webSocket.sendTXT("garage closed");
      garageClosed = true;
      }
      else{
      webSocket.sendTXT("garage opening");
      garagePush();
      webSocket.sendTXT("garage opened");
      garageClosed = false;
      }
     break;

     case '3':
     checkStatus();
     break;

     case '4':
     gateClosed = true;
     garageClosed = true;
     checkStatus();
     blinkTheLed(3);
     break;
    }
  }


void checkStatus(){
  if(garageClosed){
       webSocket.sendTXT("garage closed");
      }
      if(!garageClosed){
       webSocket.sendTXT("garage opened");
      }
      if(gateClosed){
       webSocket.sendTXT("gate closed");
      }
      if(!gateClosed){
       webSocket.sendTXT("gate opened");
      }
  }
  
void gatePush(){
  delay(2000);
  }

void garagePush(){
  delay(2000);
  }

void blinkTheLed(int times){
   for(int i = 0; i < times; i++){
     digitalWrite(BUILTIN_LED, HIGH);
     delay(200);
     digitalWrite(BUILTIN_LED, LOW);
     delay(200);
    }
  }
