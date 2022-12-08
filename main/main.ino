#include <Adafruit_PM25AQI.h>
#include <SimpleDHT.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
#include "ISD1820.h"

// object pin
#define PM25_PIN 0
#define DHT_PIN 1
#define MQ9_A_PIN 2
#define MQ9_D_PIN 3
#define RECV_PIN 4
#define ISD_REC_PIN 5
#define ISD_PLAY_E_PIN 6
#define ISD_PLAY_L_PIN 7
#define LCD_PIN 8
#define LED_PIN 9

// remote control decode
#define REPORT_ALL 0
#define REPORT_PM25 1
#define REPORT_DHT 2
#define REPORT_GAS 3

// PM25 init
Adafruit_PM25AQI aqi;
PM25_AQI_Data data;

// DHT11 init
SimpleDHT11 dht(DHT_PIN);

// MQ9 init

// IRremote init
IRrecv irrecv(RECV_PIN);
decode_results result;

// ISD1820 init
ISD1820 isd(ISD_REC_PIN, ISD_PLAY_E_PIN, ISD_PLAY_L_PIN);

// IC_LCD init
LiquidCrystal_I2C LCD(0x27, 16, 2);

namespace test {
  void pm25();
  void dht11();
  void mq9();
  void isd1820();
  void lcd();
}

void report(int code);
void print(int code);

// module initial part
void setup() {
  Serial.begin(115200);

  LCD.init();
  LCD.backlight();
  irrecv.enableIRIn();
}

// main loop
void loop() {
  if (irrecv.decode(&result)) {
    Serial.print(result.value, HEX);
    LCD.print(result.value);
    delay(3000);
    LCD.clear();
    LCD.home();
    irrecv.resume();
  }
}

void report(int code){
  switch(code){
    case REPORT_ALL:
        break;

    case REPORT_PM25:
      break;

    case REPORT_DHT:
      break;

    case REPORT_GAS:
      break;

    default: break;
  }
}

void print(int code){
  switch(code){
    case REPORT_ALL:
        break;

    case REPORT_PM25:
      break;

    case REPORT_DHT:
      break;

    case REPORT_GAS:
      break;

    default: break;
  }
}

namespace test {
  void pm25(){
    if(!aqi.read(&data)){
      Serial.println("Could not read from AQI");
      delay(500);  // try again in a bit!
      return;
    }

    Serial.println("AQI reading success\n");
    Serial.println(F("---------------------------------------"));
    Serial.println(F("Concentration Units (standard)"));
    Serial.println(F("---------------------------------------"));
    Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_standard);
    Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_standard);
    Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_standard);
    Serial.println(F("Concentration Units (environmental)"));
    Serial.println(F("---------------------------------------"));
    Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_env);
    Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_env);
    Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_env);
    Serial.println(F("---------------------------------------"));
    Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(data.particles_03um);
    Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(data.particles_05um);
    Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(data.particles_10um);
    Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(data.particles_25um);
    Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(data.particles_50um);
    Serial.print(F("Particles > 10 um / 0.1L air:")); Serial.println(data.particles_100um);
    Serial.println(F("---------------------------------------"));
  }
  void dht11(){
    byte temperature = 0;
    byte humidity = 0;
    int err = SimpleDHTErrSuccess;

    if((err = dht.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) return;

    Serial.println("========================================");
    Serial.println("Sample DHT11...");
    Serial.print("Sample OK: ");
    Serial.print(static_cast<int>(temperature)); Serial.print(" *C, ");
    Serial.print(static_cast<int>(humidity)); Serial.println(" H");
  }
  void mq9(){
    int mqBite=0;
    int mqVal=0;
    float mqVot=0;

    mqVal = analogRead(MQ9_A_PIN);
    mqVot = mqVal * 0.0049;
    Serial.println(mqVot);
    mqBite = digitalRead(MQ9_D_PIN);

    if(mqBite == 1) Serial.println("CO 濃度正常!");
    else Serial.println("CO 濃度超標!");
  }
  void isd1820(){
    isd.playSignal();
    isd.record(3000);              
    isd.playSignal_L (1000);
  }
  void lcd(){
    LCD.clear();
    LCD.home();
    LCD.print("Hello World");
    LCD.setCursor(0, 1);
    LCD.print("TEST LCD");
  }
}