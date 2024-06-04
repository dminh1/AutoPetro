#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <TFT_eSPI.h>
#include <SPI.h>

RTC_DS3231 rtc;
Adafruit_BME280 bme; 
TFT_eSPI tft = TFT_eSPI(); 
#define SEALEVELPRESSURE_HPA (1013.25)
#define GAS_AO_PIN 14 
#define relay1 13
#define sensorPin 27

int delayTime;
volatile long pulse;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // MQ2 setup
  Serial.println("Warming up the MQ2 sensor");
  delay(20000);  // wait for the MQ2 to warm up
  Serial.println(F("BME280 test"));

  //RTC
  
  if (! rtc.begin()) {
  Serial.println("RTC module is NOT found");
  while (1);
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // BME280
  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();
  
  //relay

  pinMode(relay1,OUTPUT);

  //LƯU LƯỢNG NƯỚC
  pinMode(sensorPin,INPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin),increase, RISING);

  // hiển thị màn hình
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK); 
  tft.setTextColor(ILI9341_WHITE); 
  tft.setTextSize(1); 
  tft.setCursor(0, 0);

}

int getGasValue(){
  return analogRead(GAS_AO_PIN);
}
void printValues_BME() {
  tft.print("Temperature = ");
  tft.print(bme.readTemperature());
  tft.println(" *C");
  
  // Convert temperature to Fahrenheit
  /*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/
  
  tft.print("Pressure = ");
  tft.print(bme.readPressure() / 100.0F);
  tft.println(" hPa");

  tft.print("Approx. Altitude = ");
  tft.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  tft.println(" m");

  tft.print("Humidity = ");
  tft.print(bme.readHumidity());
  tft.println(" %");

  tft.println();
}
void PrintRTC(){
  DateTime now = rtc.now();
  tft.print("RTC Date Time: ");
  tft.print(now.year(), DEC); 
  tft.print('/');
  tft.print(now.month(), DEC);
  tft.print('/');
  tft.print(now.day(), DEC);
  tft.print(" (");
  tft.print(now.dayOfTheWeek());
  tft.print(") ");
  tft.print(now.hour(), DEC);
  tft.print(':');
  tft.print(now.minute(), DEC);
  tft.print(':');
  tft.print(now.second(), DEC);

  delay(1000); // delay 1 seconds
}

void increase()
{
  pulse++;
}

void loop() {
  printValues_BME();
  tft.print(getGasValue());
  PrintRTC();

  tft.fillScreen(ILI9341_BLACK); // Clear the screen with black color
  tft.setCursor(0, 0); 

  tft.println(pulse);
  Serial.println(pulse);
  delay(500);

  tft.fillScreen(ILI9341_BLACK); // Clear the screen with black color
  tft.setCursor(0, 0); 

  digitalWrite(relay1,LOW);
  delay(1000);
  digitalWrite(relay1,HIGH);
  delay(1000);

}




