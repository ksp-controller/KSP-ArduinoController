#include <Arduino.h>
#include "./SerialLib/src/serialLib/SerialController.h"
#include "./program/Controller.h"
#include "./program/Definitions.h"

static SerialController *serialCom;
static Controller *controller;

void setup() {

  controller = new Controller();
  if (!DEBUG_CONTROLLER) { serialCom = new SerialController(); }
  else { Serial.begin(9600); }
}
void loop() {
  if (!DEBUG_CONTROLLER) {
    serialCom->loop();
    controller->loop();
    SerializedVesselControls *control = controller->getSerializedValues();
    controller->setValues(serialCom->getVesselData());
    serialCom->setVesselControls(*control);
    free(control);
  } else {
    controller->loop();
    controller->dumpValues(); delay(1000);
  }
}
