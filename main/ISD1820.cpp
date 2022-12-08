/*
 Created by Gianluigi Ronchi, June 5, 2018.
  Released into the public domain.
*/
#include "Arduino.h"
#include "ISD1820.h"

ISD1820::ISD1820 (int REC , int PLAY_E,int PLAY_L)
{
	pinMode(REC,OUTPUT);
	pinMode(PLAY_E,OUTPUT);
	pinMode(PLAY_L,OUTPUT);
	
	_REC = REC;
	_PLAY_E = PLAY_E;
	_PLAY_L = PLAY_L;
	
}

void ISD1820::record(int t)
{
	  recordTime = t;
	  _recordTime = recordTime;
      digitalWrite(_REC, HIGH); //For the time that the pin is set to high the module will record a sound (max 10 sec)
      Serial.println("Recording started");
      delay(t);
      digitalWrite(_REC, LOW);
      Serial.println("Recording Stopped ");
}

void ISD1820::playSignal ()
{
	digitalWrite(_PLAY_E, HIGH); //If the pin is placed high it will play back all the recorded sound (50 us signal is sufficient for the reproduction of the recorded sound)
	delay(50);
	digitalWrite(_PLAY_E, LOW);  
    Serial.println("Playbak Started"); 
    delay(_recordTime);
    
	Serial.println("Playbak Ended");
  
}

void ISD1820::playSignal_L (int l) //l is how long the recorded sound has to be produced (in us)
{
     digitalWrite(_PLAY_L, HIGH); //For the time that the pin is set to high the module will produce the recorded sound
     Serial.println("Playbak L Started");  
     delay(l);
     digitalWrite(_PLAY_L, LOW);
     Serial.println("Playbak L Ended");    
}
