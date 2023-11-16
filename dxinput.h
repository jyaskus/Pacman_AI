// Beginning Game Programming, Second Edition
// dxinput.h - DirectInput framework header file



#ifndef _DXINPUT_H
#define _DXINPUT_H 1

#ifndef DIRECTINPUT_VERSION
     #define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>

//function prototypes
int Init_DirectInput(HWND);
int Init_Keyboard(HWND);
void Poll_Keyboard();
int Key_Down(int);
void Kill_Keyboard();
void Poll_Mouse();
int Init_Mouse(HWND);
int Mouse_Button(int);
int Mouse_X();
int Mouse_Y();
void Kill_Mouse();


//DirectInput objects, devices, and states
extern LPDIRECTINPUT8 dinput;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
extern DIMOUSESTATE mouse_state;


#endif

