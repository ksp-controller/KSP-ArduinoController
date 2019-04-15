#ifndef Controller_h
#define Controller_h

#include "Arduino.h"
//comps
#include "./ComponentsLib/src/LightController.h"
#include "./ComponentsLib/src/SwitchButton.h"
#include "./ComponentsLib/src/PushButton.h"
#include "./ComponentsLib/src/Potentiometer.h"
//
#include "./SerialLib/src/serialLib/GameDefinitions.h"
//
class Controller
{
  public:
    Controller();
    void loop();
    void dumpValues();
    SerializedVesselControls* getSerializedValues();
  private:
    LightController *_sasLED, *_rcsLED, *_evaLED, *_dfxLED, *_sflLED, *_lflLED, *_eflLED;
    SwitchButton *_sasBtn, *_rcsBtn, *_chutesBtn, *_lightBtn, *_ladderBtn, *_lgBtn, *_solarBtn, *_brakesBtn;
    PushButton *_stageBtn, *_igniteBtn;
    Potentiometer *_throttle;
    //LCD *_topLCD, *_midLCD, *_bottomLCD;
    //SevenSegment *_topLCD;
    //Joystick *_rotationJoy, *_translationJoy;
    void _initComponents();
    void _runTest();
};
#endif
