#include "Arduino.h"
#include "Controller.h"
#include "Definitions.h"
#include "./ComponentsLib/src/ComponentsDefinitions.h"

Controller::Controller()
{
  _lcdRefresh = 0;
  this->_initComponents();
  this->_runTest();
}
void Controller::loop()
{
  _sasBtn->loop(), _rcsBtn->loop(), _chutesBtn->loop(), _lightBtn->loop(), _ladderBtn->loop(), _lgBtn->loop(), _solarBtn->loop(), _brakesBtn->loop();
  _stageBtn->loop(), _igniteBtn->loop(), _throttle->loop(), _rotationJoy->loop(), _translationJoy->loop();
}
void Controller::dumpValues()
{
  Serial.print("SAS: "); Serial.println(_sasBtn->isOn() ? "ON" : "OFF");
  //RCS
  Serial.print("RCS: "); Serial.println(_rcsBtn->isOn() ? "ON" : "OFF");
  //Chutes
  Serial.print("CHUTES: "); Serial.println(_chutesBtn->isOn() ? "ON" : "OFF");
  //Ladder
  Serial.print("LADDER: "); Serial.println(_ladderBtn->isOn() ? "ON" : "OFF");
  //Light
  Serial.print("LIGHT: "); Serial.println(_lightBtn->isOn() ? "ON" : "OFF");
  //Solar
  Serial.print("SOLAR: "); Serial.println(_solarBtn->isOn() ? "ON" : "OFF");
  //Landing Gear
  Serial.print("LANDINGGEAR: "); Serial.println(_lgBtn->isOn() ? "ON" : "OFF");
  //Breaks
  Serial.print("BRAKES: "); Serial.println(_brakesBtn->isOn() ? "ON" : "OFF");
  //Throttle
  Serial.print("THROTTLE: "); Serial.println(_throttle->getCurrentValue());
  //Ignite
  Serial.print("IGNITE: "); Serial.println(_igniteBtn->isPressed());
  //Stage
  Serial.print("STAGE: "); Serial.println(_stageBtn->isPressed());
  //Rotation
  JoystickValue rot = _rotationJoy->getCurrentValue();
  Serial.print("ROTATION: L:"); Serial.print(rot.leftValue); Serial.print(" R:"); Serial.print(rot.rightValue);
  Serial.print(" T:"); Serial.print(rot.topValue); Serial.print(" B:"); Serial.println(rot.bottomValue);
  //Rotation
  JoystickValue tran = _translationJoy->getCurrentValue();
  Serial.print("TRANSLATION: L:"); Serial.print(tran.leftValue); Serial.print(" R:"); Serial.print(tran.rightValue);
  Serial.print(" T:"); Serial.print(tran.topValue); Serial.print(" B:"); Serial.println(tran.bottomValue);
  //
  Serial.println("---------------------------");
  Serial.println();
}
SerializedVesselControls* Controller::getSerializedValues()
{
  SerializedVesselControls *control = new SerializedVesselControls();
  SetControlStatus(control, RCS, !_rcsBtn->isOn()); //inverted
  SetControlStatus(control, SAS, !_sasBtn->isOn()); //inverted
  SetControlStatus(control, Light, !_lightBtn->isOn()); //inverted
  SetControlStatus(control, Breaks, !_brakesBtn->isOn()); //inverted
  SetControlStatus(control, Gear, !_lgBtn->isOn()); //inverted
  SetControlStatus(control, Stage, ( _stageBtn->isPressed() || _igniteBtn->isPressed()));
  control->Throttle = 1000.f - _throttle->getCurrentValue(); //inverted
  //
  JoystickValue tran = _translationJoy->getCurrentValue();
  if (tran.leftValue > tran.rightValue) control->Yaw = tran.leftValue * -1;
  else control->Yaw = tran.rightValue;
  if (tran.bottomValue > tran.topValue) control->Pitch = tran.bottomValue * -1;
  else control->Pitch = tran.topValue;
  //
  JoystickValue rol = _rotationJoy->getCurrentValue();
  if (rol.leftValue > rol.rightValue) control->Roll = rol.leftValue * -1;
  else control->Roll = rol.rightValue;
  //
  return control;
}
void Controller::setValues(VesselData vesselData)
{
  _rcsLED->setState(GetControlStatus(vesselData, RCS) == 1);
  _sasLED->setState(GetControlStatus(vesselData, SAS) == 1);
  _evaLED->setState(vesselData.Vsurf > 1200);
  _dfxLED->setState(vesselData.G > 3.5);
  _sflLED->setState((vesselData.SolidFuel*100)/vesselData.SolidFuelTot <= 10);
  _lflLED->setState((vesselData.LiquidFuel*100)/vesselData.LiquidFuelTot <= 10);
  _eflLED->setState((vesselData.ECharge*100)/vesselData.EChargeTot <= 10);
  //LCD
  if (millis() - _lcdRefresh > LCD_REFRESH_DELAY) {
    _topLCD->clearDisplay();
    _topLCD->writeOnLine("SPEED: " + String(vesselData.Vsurf > 999 ? String((int)(vesselData.Vsurf)) : String(vesselData.Vsurf)) + "m/s", 1);
    _topLCD->writeOnLine("VSPEED: " + (vesselData.VVI > 99 ? String((int)(vesselData.VVI)) : String(vesselData.VVI)) + "m/s", 2);
    _midLCD->clearDisplay();
    _midLCD->writeOnLine("LAT: " + String(vesselData.Lat), 1);
    _midLCD->writeOnLine("LONG: " + String(vesselData.Lon), 2);
    _bottomLCD->clearDisplay();
    _bottomLCD->writeOnLine("ALT: " + String(vesselData.Alt), 1);
    _bottomLCD->writeOnLine("ATMO DENS.: " + String(vesselData.Density), 2);
    _lcdRefresh = millis();
  }
}


void Controller::_initComponents() {
  _sasLED = new LightController(SAS_LED_PIN), _rcsLED = new LightController(RCS_LED_PIN), _evaLED = new LightController(EVA_LED_PIN);
  _dfxLED = new LightController(DFX_LED_PIN), _sflLED = new LightController(SFL_LED_PIN), _lflLED = new LightController(LFL_LED_PIN);
  _eflLED = new LightController(EFL_LED_PIN), _sasBtn = new SwitchButton(SAS_BUTTON_PIN), _rcsBtn = new SwitchButton(RCS_BUTTON_PIN);
  _chutesBtn = new SwitchButton(CHUTES_BUTTON_PIN), _lightBtn = new SwitchButton(LIGHT_BUTTON_PIN), _ladderBtn = new SwitchButton(LADDER_BUTTON_PIN);
  _lgBtn = new SwitchButton(LANDINGGEAR_BUTTON_PIN), _solarBtn = new SwitchButton(SOLAR_BUTTON_PIN), _brakesBtn = new SwitchButton(BRAKES_BUTTON_PIN);
  _stageBtn = new PushButton(STAGE_BUTTON_PIN), _igniteBtn = new PushButton(IGNITE_BUTTON_PIN);
  _throttle = new Potentiometer(THROTTLE_PIN, THROTTLE_MIN_VALUE, THROTTLE_MAX_VALUE);
  _topLCD = new LCD(LCD_TOP_PIN); _midLCD = new LCD(LCD_MID_PIN); _bottomLCD = new LCD(LCD_BOT_PIN);
  _rotationJoy = new Joystick(JOYSTICK1_LEFT_PIN, JOYSTICK1_RIGHT_PIN, JOYSTICK1_TOP_PIN, JOYSTICK1_BOTTOM_PIN, JOYSTICK_MIN, JOYSTICK_MAX);
  _translationJoy = new Joystick(JOYSTICK2_LEFT_PIN, JOYSTICK2_RIGHT_PIN, JOYSTICK2_TOP_PIN, JOYSTICK2_BOTTOM_PIN, JOYSTICK_MIN, JOYSTICK_MAX);
}
void Controller::_runTest() {
  _sasLED->setState(HIGH), _rcsLED->setState(HIGH), _evaLED->setState(HIGH), _dfxLED->setState(HIGH), _sflLED->setState(HIGH), _lflLED->setState(HIGH), _eflLED->setState(HIGH);
  _topLCD->clearDisplay(); _topLCD->writeOnLine("SPEED: 0m/s", 1); _topLCD->writeOnLine("VSPEED: 0m/s", 2);
  _midLCD->clearDisplay(); _midLCD->writeOnLine("LAT: 0", 1); _midLCD->writeOnLine("LONG: 0", 2);
  _bottomLCD->clearDisplay(); _bottomLCD->writeOnLine("ALT: 0", 1); _bottomLCD->writeOnLine("ATMOS DENS.: 0", 2);
  delay(INITIAL_TEST_DELAY);
  //7SEG?
}
