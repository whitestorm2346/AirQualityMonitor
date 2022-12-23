#include <DHT.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
#include "GP2Y1010AU0F.h"
#include "MQ9.h"

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
#define IDLE  0
#define PM25  1
#define DHT22 2
#define MQ9   3

// modules set up
GP2Y1010AU0F pm25(PM25_LED_PIN, PM25_PIN);
DHT dht(DHT_PIN, DHT22);
MQ9 mq9;
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

// modules initial 
void setup() {
  Serial.begin(9600);

  mq9.inits();
  mq9.calibrate();

  dht.begin();

  LCD.init();
  LCD.backlight();

  irrecv.enableIRIn();
}

// main loop
void loop() {
  if (irrecv.decode(&result)) {
    // Serial.println(result.value);

    if(state == IDLE){
      switch(result.value){
        case BTN_1: print::pm25(); state = PM25; break;      
        case BTN_2: print::dht22(); break;  
        case BTN_3: print::mq9(); break;     
        case BTN_OK: LCD.clear(); break;   
        default: break;
      }
    }
    
    delay(100);
    irrecv.resume();
  }
}

namespace print {
  void pm25(){
    double outputV = ::pm25.getOutputV(); //採樣獲取輸出電壓
    double ugm3 = ::pm25.getDustDensity(outputV); //計算灰塵濃度
    double aqi = ::pm25.getAQI(ugm3); //計算aqi
    int gradeInfo = ::pm25.getGradeInfo(aqi); //計算級別
    
    LCD.home();
    LCD.clear();

    LCD.print("Dust D: ");
    LCD.print(ugm3);
    LCD.setCursor(0, 1);

    switch(gradeInfo){
      case GRADE_PERFECT: LCD.print("PERFECT"); break;
      case GRADE_GOOD: LCD.print("GOOD"); break;
      case GRADE_POLLUTED_MILD: LCD.print("POLLUTED MILD"); break;
      case GRADE_POLLUTED_MEDIUM: LCD.print("POLLUTED MEDIUM"); break;
      case GRADE_POLLUTED_HEAVY: LCD.print("POLLUTED HEAVY"); break;
      case GRADE_POLLUTED_SEVERE: LCD.print("POLLUTED SEVERE"); break;
      default: break;
    }
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
    double data[3]; // CO LPG CH4

    ::mq9.getValue(false, 'C', data);

    LCD.home();
    LCD.clear();

    LCD.print("CO: ");
    LCD.print(data[0]);
    LCD.print("ppm");

    if(::mq9.thrValue('C', 35.f)){
      LCD.setCursor(0, 1);
      LCD.print("Warning!");
    }
  }
}
