#define BLYNK_TEMPLATE_ID "TMPLh79JG--h"
#define BLYNK_DEVICE_NAME "OnOffLED"
#define BLYNK_AUTH_TOKEN "TOKEN"
#define LINE_TOKEN "TOKEN"

#define BLYNK_PRINT Serial

#include <TridentTD_LineNotify.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "WIFINAME";
char pass[] = "PASSWORDWIFI";

#define DHTPIN D1
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
  
BLYNK_WRITE(V0){
  int pinValue = param.asInt();
  if (pinValue == 1) {
    Serial.println(pinValue);
    LINE.notify("ความชื้น : "+ String (dht.readHumidity())+ " %");
    LINE.notify("อุณหภูมิ : "+ String (dht.readTemperature())+ " Celsius");
    LINE.notify("Raindrop : "+ String (map(analogRead(A0), 0, 1023, 100, 0))+ " %");
    LINE.notify("Soil : "+ String (map(digitalRead(D0), 0, 0, 1, 0))+ " *หากเป็น 1 แสดงว่าดินแห้ง");
    LINE.notifySticker(3, 240);
  }
}

void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int r = analogRead(A0);
  r = map(r, 0, 1023, 100, 0);
  int s = digitalRead(D0);
  if (isnan(h) || isnan(t) || isnan(r) || isnan(s)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V1, r);
  Blynk.virtualWrite(V2, s);
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void setup(){
  LINE.setToken(LINE_TOKEN);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
}

void loop(){
  Blynk.run();
  timer.run();
}
