/*
 Created by Gianluigi Ronchi, June 5, 2018.
  Released into the public domain.
*/
#ifndef ISD1820_h
#define ISD1820_h

#include "Arduino.h"

class ISD1820 
{
	public:
	ISD1820 (int REC, int PLAY_E, int PLAY_L);
	void record(int t);
	void playSignal ();
	void playSignal_L (int l);
	int recordTime;
	
	private:
	int _REC;
	int _PLAY_E;
	int _PLAY_L;
	int _recordTime;
};


#endif