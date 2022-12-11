#include <DHT.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
#include "GP2Y1010AU0F.h"
#include "mq9.h"
//#include "DFPlayer_mini.h"

// module pin
#define PM25_PIN A0
#define PM25_LED_PIN 2
#define DHT_PIN 12
#define MQ9_A_PIN A1
#define MQ9_D_PIN 8
#define RECV_PIN 13

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
int samplingTime = 280; //the starting time of  LED is 280μs
int deltaTime = 40; //the whole pulse is 320μs. So we have to wait for 40μs
int sleepTime = 9680;

GP2Y1010AU0F GP2Y1010AU0F(PM25_LED_PIN, PM25_PIN);
DHT dht(DHT_PIN, DHT22);
IRrecv irrecv(RECV_PIN);
decode_results result;
LiquidCrystal_I2C LCD(0x27, 16, 2);

// state init 
int state = IDLE;

// functions declaration
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
  void dfplayer();
}

// modules initial 
void setup() {
  Serial.begin(9600);

  pinMode(PM25_LED_PIN, OUTPUT);
  pinMode(MQ9_D_PIN, INPUT); 

  dht.begin();
  LCD.init();
  LCD.backlight();
  irrecv.enableIRIn();
}

// main loop
void loop() {
  if (irrecv.decode(&result)) {
    Serial.println(result.value);
    
    switch(result.value){
      case BTN_1: print::pm25(); break;
      case BTN_2: print::dht22(); break;
      case BTN_3: print::mq9(); break;
      case BTN_4: test::dfplayer(); break;
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
    digitalWrite(PM25_LED_PIN, LOW); 
    delayMicroseconds(samplingTime); 
    
    float voMeasured = analogRead(PM25_PIN); 

    delayMicroseconds(deltaTime);
    digitalWrite(PM25_LED_PIN, HIGH); 
    delayMicroseconds(sleepTime);

    float calcVoltage = voMeasured * (5.0 / 1024.0);
    float dustDensity = 0.17 * calcVoltage - 0.1;

    LCD.home();
    LCD.clear();

    LCD.print("Dust Density: ");
    LCD.setCursor(0, 1);
    LCD.print(dustDensity);
  }
  void dht22(){
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    LCD.home();
    LCD.clear();

    LCD.print("T: ");
    LCD.print(t);
    LCD.print("*C");
    LCD.setCursor(0, 1);
    LCD.print("H: ");
    LCD.print(h);
    LCD.print("%");
  }
  void mq9(){
    float sensorValue = static_cast<float>(analogRead(MQ9_A_PIN)); 
    float sensor_volt = sensorValue / 1024 * 5.0;
    float RS_gas = (5.0 - sensor_volt) / sensor_volt;
    float R0 = 1.97;  // R0 in 1000 ppm LPG 
    float ratio = RS_gas / R0;

    LCD.home();
    LCD.clear();

    LCD.print("Ratio: ");
    LCD.print(ratio);

    if(ratio < 1.5){
      LCD.setCursor(0, 1);
      LCD.print("Warning!");
    }
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
  void dfplayer(){
    
  }
}
