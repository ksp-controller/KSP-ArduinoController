#include "Arduino.h"
#include "Controller.h"
#include "Definitions.h"
#include "./ComponentsLib/src/ComponentsDefinitions.h"

Controller::Controller()
{
  this->_initComponents();
  this->_runTest();
}
void Controller::loop()
{
  _sasLED->loop(), _rcsLED->loop(), _evaLED->loop(), _dfxLED->loop(), _sflLED->loop(), _lflLED->loop(), _eflLED->loop();
  _sasBtn->loop(), _rcsBtn->loop(), _chutesBtn->loop(), _lightBtn->loop(), _ladderBtn->loop(), _lgBtn->loop(), _solarBtn->loop(), _brakesBtn->loop();
  _stageBtn->loop(), _igniteBtn->loop(), _throttle->loop();
}
void Controller::dumpValues()
{
  Serial.print("SAS: ");
  Serial.print("BTN-");
  Serial.println(_sasBtn->isOn() ? "ON" : "OFF");
  //RCS
  Serial.print("RCS: ");
  Serial.print("BTN-");
  Serial.println(_rcsBtn->isOn() ? "ON" : "OFF");
  //Chutes
  Serial.print("CHUTES: ");
  Serial.println(_chutesBtn->isOn() ? "ON" : "OFF");
  //Ladder
  Serial.print("LADDER: ");
  Serial.println(_ladderBtn->isOn() ? "ON" : "OFF");
  //Light
  Serial.print("LIGHT: ");
  Serial.println(_lightBtn->isOn() ? "ON" : "OFF");
  //Solar
  Serial.print("SOLAR: ");
  Serial.println(_solarBtn->isOn() ? "ON" : "OFF");
  //Landing Gear
  Serial.print("LANDINGG: ");
  Serial.println(_lgBtn->isOn() ? "ON" : "OFF");
  //Breaks
  Serial.print("BRAKES: ");
  Serial.println(_brakesBtn->isOn() ? "ON" : "OFF");
  //Throttle
  Serial.print("THROTTLE: ");
  Serial.println(_throttle->getCurrentValue());
  //Ignite
  Serial.print("IGNITE: ");
  Serial.println(_igniteBtn->isPressed());
  //Stage
  Serial.print("STAGE: ");
  Serial.println(_stageBtn->isPressed());
  //
  Serial.println("---------------------------");
  Serial.println();
}
SerializedVesselControls* Controller::getSerializedValues()
{
  return NULL;
}


void Controller::_initComponents() {
  _sasLED = new LightController(SAS_LED_PIN), _rcsLED = new LightController(RCS_LED_PIN), _evaLED = new LightController(EVA_LED_PIN);
  _dfxLED = new LightController(DFX_LED_PIN), _sflLED = new LightController(SFL_LED_PIN), _lflLED = new LightController(LFL_LED_PIN);
  _eflLED = new LightController(EFL_LED_PIN), _sasBtn = new SwitchButton(SAS_BUTTON_PIN), _rcsBtn = new SwitchButton(RCS_BUTTON_PIN);
  _chutesBtn = new SwitchButton(CHUTES_BUTTON_PIN), _lightBtn = new SwitchButton(LIGHT_BUTTON_PIN), _ladderBtn = new SwitchButton(LADDER_BUTTON_PIN);
  _lgBtn = new SwitchButton(LANDINGGEAR_BUTTON_PIN), _solarBtn = new SwitchButton(SOLAR_BUTTON_PIN), _brakesBtn = new SwitchButton(BRAKES_BUTTON_PIN);
  _stageBtn = new PushButton(STAGE_BUTTON_PIN), _igniteBtn = new PushButton(IGNITE_BUTTON_PIN);
  _throttle = new Potentiometer(THROTTLE_PIN, THROTTLE_MIN_VALUE, THROTTLE_MAX_VALUE);
}
void Controller::_runTest() {
  _sasLED->setState(ON), _rcsLED->setState(ON), _evaLED->setState(ON), _dfxLED->setState(ON), _sflLED->setState(ON), _lflLED->setState(ON), _eflLED->setState(ON);
  delay(INITIAL_TEST_DELAY);
  //LED?
  //7SEG?
}
