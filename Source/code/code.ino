#include <WiFi.h>
#include <ESP32Firebase.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define FIREBASE_HOST "fir-demo-75dcf-default-rtdb.firebaseio.com" 
#define WIFI_SSID "Quay thuoc Bich Chau"
#define WIFI_PASSWORD "bichchau68"

#define DHTPIN 5    
#define DHTTYPE  DHT11

#define BUZ 26
#define PIR_PIN 25 
#define MQ_PIN 32 
#define FAN 23    

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

DHT_Unified dht(DHTPIN, DHTTYPE);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String path = "/ESP32_Device";

Firebase firebase(FIREBASE_HOST);

uint32_t delayMS;
float prev_temp;
float prev_humidity;
int pir;
int prev_pir;
int mq;
int prev_mq;

String buz_data;

void setup() {
  Serial.begin(115200);
  firebase.setString(path + "/buzzer", "0");
  initDrawing();
  initWifi();
}

void loop() {
  delay(delayMS);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  pir = digitalRead(PIR_PIN);
  Serial.print(F("PIR: "));
  Serial.println(pir);
  updatePIR(pir);

  mq = analogRead(MQ_PIN);
  Serial.print(F("CO: "));
  Serial.println(mq);
  updateMQ(mq);

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Edât temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    updateTemp(event.temperature);
  }
  
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    updateHumidity(event.relative_humidity);
  }

  buz_data = firebase.getString(path + "/buzzer");
  Serial.println(buz_data);

  display.display();
}

void updateTemp(float temp){
  if(prev_temp != temp){
    prev_temp = temp;
    firebase.setFloat(path + "/Temperature", temp);
  }
  display.setCursor(0,0);
  display.print("TEMP: ");
  display.print(temp);
  display.println("C");
}

void updateHumidity(float humidity){
  if(prev_humidity != humidity){
    prev_humidity = humidity;
    firebase.setFloat(path + "/Humidity", humidity);
  }
  display.setCursor(0,10);
  display.print("HUMI: ");
  display.print(humidity);
  display.println("%");
}

void updatePIR(int pir){
  if(prev_pir != pir){
    prev_pir = pir;
    firebase.setString(path + "/State PIR", pir == HIGH ? "YES" : "NO");
  }
  display.setCursor(0,20);
  display.print("State PIR: ");
  display.println(pir == HIGH ? "YES" : "NO");

  if(pir == HIGH) {
    digitalWrite(BUZ, HIGH);
    firebase.setString(path + "/buzzer", "1");
  }

   if(buz_data == "0"){
    digitalWrite(BUZ, LOW);
  }
}

void updateMQ(int mq){
  if(prev_mq != mq){
    prev_mq = mq;
    firebase.setInt(path + "/CO", mq);
  }
  display.setCursor(0,30);
  display.print("CO: ");
  display.print(mq);

  if(mq > 500) {
    digitalWrite(BUZ, HIGH);
    digitalWrite(FAN, HIGH);
    firebase.setString(path + "/buzzer", "1");
  } else{
    digitalWrite(FAN, LOW);
  }

  if(buz_data == "0"){
    digitalWrite(BUZ, LOW);
  }
}

void initDrawing(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Màn hình OLED không kết nối được! Kiểm tra kết nối."));
    while (true);
  }
  display.clearDisplay();
  display.display();

  dht.begin();

  pinMode(PIR_PIN, INPUT);
  pinMode(MQ_PIN, INPUT);
  pinMode(BUZ, OUTPUT);
  pinMode(FAN, OUTPUT);
  
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  
  delayMS = sensor.min_delay / 1000;
}

void initWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("-");
    delay(300);
  }
  Serial.println();
  Serial.print("Wifi connected!");
}


