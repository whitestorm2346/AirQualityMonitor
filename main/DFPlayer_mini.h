#ifndef DFPLAYER_MINI_H_INCLUDED
#define DFPLAYER_MINI_H_INCLUDED

#include "Arduino.h"
#include "SoftwareSerial.h"

#define Start_Byte 0x7E
#define Version_Byte 0xFF
#define Command_Length 0x06
#define End_Byte 0xEF
#define Acknowledge 0x00  //Returns info with command 0x41 [0x01: info, 0x00: no info]


class DFPlayer_mini{
private:
    bool isPlaying;


public:
    DFPlayer_mini();
    void play();
    void pause();
    void play_next();
    void play_prev();
    void volume_up();
    void volume_down();
    void set_volume();
};

#endif // DFPLAYER_MINI_H_INCLUDED
