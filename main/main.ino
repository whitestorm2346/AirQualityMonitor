#include <Adafruit_PM25AQI.h>
#include <DHT.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
//#include "DFPlayer_mini.h"

// module pin
#define PM25_PIN 0
#define DHT_PIN 2
#define MQ9_A_PIN 2
#define MQ9_D_PIN 3
#define RECV_PIN 4
#define ISD_REC_PIN 5
#define ISD_PLAY_E_PIN 6
#define ISD_PLAY_L_PIN 7
#define LCD_PIN 8
#define LED_PIN 9

// remote control button pin
#define BTN_1 16753245
#define BTN_2 16736925
#define BTN_3 16769565
#define BTN_4 16720605
#define BTN_5 16712445
#define BTN_6 16761405
#define BTN_7 16769055
#define BTN_8 16754775
#define BTN_9 16748655
#define BTN_0 16750695
#define BTN_STAR    16738455
#define BTN_HASHTAG 16756815
#define BTN_OK      16726215
#define BTN_UP      16718055
#define BTN_RIGHT   16734885
#define BTN_DOWN    16730805
#define BTN_LEFT    16716015

// program state pin
#define IDLE 0


// modules set up
Adafruit_PM25AQI aqi;
PM25_AQI_Data data;

DHT dht = DHT(DHT_PIN, DHT22);

IRrecv irrecv(RECV_PIN);
decode_results result;

LiquidCrystal_I2C LCD(0x27, 16, 2);

// state init 
int state = IDLE;

namespace print {
  void pm25();
  void dht22();
  void mq9();
}

namespace report {
  void pm25();
  void dht22();
  void mq9();
}

namespace test {
  void pm25();
  void dht22();
  void mq9();
  void lcd();
  void dfplayer();
}

void report(int code);
void print(int code);

// modules initial 
void setup() {
  Serial.begin(9600);

  dht.begin();
  LCD.init();
  LCD.backlight();
  irrecv.enableIRIn();
}

// main loop
void loop() {
  if (irrecv.decode(&result)) {
    Serial.print(result.value);
    
    switch(result.value){
      case BTN_1: test::pm25(); break;
      case BTN_2: test::dht22(); break;
      case BTN_3: test::mq9(); break;
      case BTN_4: break;
      case BTN_5: break;
      case BTN_6: break;
      case BTN_7: break;
      case BTN_8: break;
      case BTN_9: break;
      case BTN_0: break;
      case BTN_STAR: break;
      case BTN_HASHTAG: break;
      case BTN_UP: break;
      case BTN_RIGHT: break;
      case BTN_DOWN: break;
      case BTN_LEFT: break;    
      case BTN_OK: LCD.clear(); break;
      default: break;
    }
    
    delay(100);
    irrecv.resume();
  }
}

namespace print {
  void pm25(){

  }
  void dht22(){

  }
  void mq9(){

  }
}

namespace report {
  void pm25(){

  }
  void dht22(){

  }
  void mq9(){
    
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
  void dht22(){
    // Read the humidity in %:
    float h = dht.readHumidity();
    
    // Read the temperature as Celsius:
    float t = dht.readTemperature();
    
    // Read the temperature as Fahrenheit:
    float f = dht.readTemperature(true);
  
    // Check if any reads failed and exit early (to try again):
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  
    // Compute heat index in Fahrenheit (default):
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius:
    float hic = dht.computeHeatIndex(t, h, false);
  
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" % ");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" \xC2\xB0");
    Serial.print("C | ");
    Serial.print(f);
    Serial.print(" \xC2\xB0");
    Serial.print("F ");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print(" \xC2\xB0");
    Serial.print("C | ");
    Serial.print(hif);
    Serial.print(" \xC2\xB0");
    Serial.println("F");

    LCD.clear();
    LCD.home();
    
    LCD.print("T: ");
    LCD.print(t);
    LCD.setCursor(0, 1);
    LCD.print("H: ");
    LCD.print(h);
    
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
  void lcd(){
    LCD.clear();
    LCD.home();
    LCD.print("Hello World");
    LCD.setCursor(0, 1);
    LCD.print("TEST LCD");
  }
  void dfplayer(){

  }
}
