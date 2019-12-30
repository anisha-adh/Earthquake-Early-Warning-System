
#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#include <Wire.h>
#include "Adafruit_MMA8451.h"
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>

#define MMA8451_DEFAULT_ADDRESS (0x1C)

Adafruit_MMA8451 mma = Adafruit_MMA8451();

struct MMA8451_Sensor{
  float x, y, z;
  float ax, ay, az;
  String orientation;
} sensorData;

void setup() {
  Serial.begin(9600);

  // Sensor setup
 Serial.println("Adfruit MMA8451 test ...");
  if(!mma.begin()){
   Serial.println("Couldn't Start");
    while(1);
  }
 Serial.println("MMA8451 found!");
  mma.setRange(MMA8451_RANGE_2_G);
 Serial.print("Range: "); 
 Serial.print(2 << mma.getRange());
 Serial.println("G");
  delay(4000);
  // Wifi setup
  WiFi.begin("researchlab","CLB3569F7B");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
   Serial.println("Waiting for connection ...");
  }
}

struct MMA8451_Sensor getSensorData(){
  // Read the raw data in 14 bits counts
  mma.read();
  sensorData.x = mma.x;
  sensorData.y = mma.y;
  sensorData.z = mma.z;
 Serial.println("\n##########Sensor Readings Start###########\n");
 Serial.print(sensorData.x);Serial.print("\t");
 Serial.print(sensorData.y);Serial.print("\t");
 Serial.println(sensorData.z);
  // new sensor event
  sensors_event_t event;
  mma.getEvent(&event);
  // acceleration results
  sensorData.ax = event.acceleration.x;
  sensorData.ay = event.acceleration.y;
  sensorData.az = event.acceleration.z;
 Serial.print(sensorData.ax);Serial.print("\t");
 Serial.print(sensorData.ay);Serial.print("\t");
 Serial.println(sensorData.az);
  // orientation
  uint8_t o = mma.getOrientation();
  switch (o) {
    case MMA8451_PL_PUF:
     Serial.println("Portrait Up Front");
      sensorData.orientation = "PUF";
      break;
    case MMA8451_PL_PUB:
     Serial.println("Portrait Up Back");
      sensorData.orientation = "PUB";
      break;
    case MMA8451_PL_PDF:
     Serial.println("Portrait Down Front");
      sensorData.orientation = "PDF";
      break;
    case MMA8451_PL_PDB:
     Serial.println("Portrait Down Back");
      sensorData.orientation = "PDB";
      break;
    case MMA8451_PL_LRF:
     Serial.println("Landscape Right Front");
      sensorData.orientation = "LRF";
      break;
    case MMA8451_PL_LRB:
     Serial.println("Landscape Right Back");
      sensorData.orientation = "LRB";
      break;
    case MMA8451_PL_LLF:
     Serial.println("Landscape Left Front");
      sensorData.orientation = "LLF";
      break;
    case MMA8451_PL_LLB:
     Serial.println("Landscape Left Back");
      sensorData.orientation = "LLB";
      break;
  }
 Serial.println("\n##########Sensor Readings End###########\n");
  return sensorData;
}

void loop() {
  struct MMA8451_Sensor sensorReadings = getSensorData();
   Serial.println("\n##########Network and Sharing Start###########\n");

  if (WiFi.status() == WL_CONNECTED){
    StaticJsonDocument<256> jsonBuffer;
    JsonObject root = jsonBuffer.to<JsonObject>();
    // json format of data 
    JsonObject raw = root.createNestedObject("raw");
    raw["x"] = sensorReadings.x;
    raw["y"] = sensorReadings.y;
    raw["z"] = sensorReadings.z;
    JsonObject acclr = root.createNestedObject("acclro");
    acclr["ax"] = sensorReadings.ax;
    acclr["ay"] = sensorReadings.ay;
    acclr["az"] = sensorReadings.az;
    root["orientation"] = sensorReadings.orientation;

    char JSONmessageBuffer[256];
    serializeJsonPretty(root, JSONmessageBuffer);
   Serial.println(JSONmessageBuffer);

    // http connection setup
    HTTPClient http;
    http.begin("http://192.168.1.87:5000/eqdata");
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(JSONmessageBuffer);
    if(httpResponseCode>0){
    String response = http.getString();  //Get the response to the request
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
    }else{
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
    }
    http.end();
  }else{
   Serial.println("Error in Wifi Connection");
  }
 Serial.println("\n##########Network and Sharing End###########\n");
  delay(1000);
}
