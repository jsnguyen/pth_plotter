#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_HTU21DF.h>
   
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
Adafruit_HTU21DF htu = Adafruit_HTU21DF();

const char* ssid="NETGEAR73";
const char* password = "thirstymango373";

IPAddress ip(10, 0, 0, 44); // set static IP Address
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

const int capacity = JSON_OBJECT_SIZE(3);
StaticJsonDocument<capacity> doc;

void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );
  
  WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid,password);

  Serial.println();
  Serial.print("Connecting");
  
  while( WiFi.status() != WL_CONNECTED ){  
      Serial.print(".");
      delay(500);
  }

  Serial.println();

  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );

  server.on("/", handleOnConnect);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  initBMP180();
  initHTU21DF();
}

void loop() {
  server.handleClient();
}

void handleOnConnect() {
  Serial.println("Client connected!");
    
  float pressure;
  readBMP180(&pressure);
  
  float temperature, humidity;
  readHTU21DF(&temperature, &humidity);

  doc["pressure"].set(pressure);
  doc["temperature"].set(temperature);
  doc["humidity"].set(humidity);  
  server.send(200, "text/plain", doc.as<String>());
}


void handleNotFound(){
  server.send(404, "text/plain", "404 not found!");
}

void initBMP180(){
  if(!bmp.begin()){
    Serial.print("ERROR: initializing BMP180 sensor");
    while(1);
  }
}

void initHTU21DF(){
  if (!htu.begin()){
    Serial.println("ERROR: initializing HTU21DF sensor");
    while(1);
  }
}

void readBMP180(float *pressure){
  sensors_event_t bmp_event;
  bmp.getEvent(&bmp_event);
  if(bmp_event.pressure){
    *pressure = bmp_event.pressure; /* pressure in units of hPa */
  } else {
    Serial.println("ERROR: BMP180 sensor");
  } 
}

void readHTU21DF(float *temperature, float *humidity){
  *temperature = htu.readTemperature();
  *humidity = htu.readHumidity();
}
