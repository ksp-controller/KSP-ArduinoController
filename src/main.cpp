#include <Arduino.h>
#include "./SerialLib/src/serialLib/SerialController.h"
#include "./ComponentsLib/src/Potentiometer.h"

static SerialController *serialCom;
static Potentiometer *throttle;
#define PIN1 20
#define PIN2 21

void setup() {
  serialCom = new SerialController();
  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);
  throttle = new Potentiometer(A2,0,1000);
}
void loop() {
  serialCom->loop();
  if (GetControlStatus(serialCom->getVesselData(), RCS) == 1) {
    digitalWrite(PIN1, HIGH);
  }else {
    digitalWrite(PIN1, LOW);
  }
  if (GetControlStatus(serialCom->getVesselData(), SAS) == 1) {
    digitalWrite(PIN2, HIGH);
  }else {
    digitalWrite(PIN2, LOW);
  }
  throttle->loop();
  SerializedVesselControls *control = new SerializedVesselControls();
  control->Throttle = throttle->getCurrentValue();
  serialCom->setVesselControls(*control);
}
