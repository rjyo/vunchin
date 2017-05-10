#include <Wire.h>
#include <MMA7660.h>

const int ledPin = 8; // LED to D8
const int btnPin = 2; // BUTTON to D2
volatile bool alarmOn = false;
volatile bool alarming = false;

//Accelerometer limits
float xMin; //Minimum x Value
float xMax; //Maximum x Value
float yMin; //Minimum y Value
float yMax; //Maximum y Value
float zMin; //Minimum z Value
float zMax; //Maximum z Value

bool calibrated = false;
float tolerance = 0.05;

MMA7660 acc;

void setup() {
  acc.init();
  Serial.begin(38400);

  pinMode(ledPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(btnPin), btnStateChanged, CHANGE);
}

void loop() {
  if (alarmOn && !calibrated) {
    calibrateAccel();
    Serial.println("------------------------");
    Serial.println("Alarm on");
  }
  if (alarmOn && calibrated) {
    if (checkMotion()) {
      alarming = true;
      calibrated = false;
      alarm(50);
      alarmOn = false;
    }
  }
}

//This is the function used to sound the buzzer
void alarm(int reps) {
  for (int i = 0; i < reps; i++) {
    if (!alarming) return;

    analogWrite(ledPin, 900);
    delay(100);
    analogWrite(ledPin, 0);
    delay(100);
  }
}

void printValues() {
  Serial.print("xMin=");
  Serial.print(xMin);
  Serial.print(", xMax=");
  Serial.print(xMax);
  Serial.println();

  Serial.print("yMin=");
  Serial.print(yMin);
  Serial.print(", yMax=");
  Serial.print(yMax);
  Serial.println();

  Serial.print("zMin=");
  Serial.print(zMin);
  Serial.print(", zMax=");
  Serial.print(zMax);
  Serial.println();

  Serial.println("------------------------");
}

void btnStateChanged() {
  // debounce
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 250) {
    if (digitalRead(btnPin)) {
      if (!alarmOn) {
        Serial.println("------------------------");
        Serial.println("Start calibrating");
        alarmOn = true;
      } else {
        if (alarming) {
          alarming = false;
        }
        Serial.println("------------------------");
        Serial.println("Alarm off");
        alarmOn = false;
      }
    }
  }
  last_interrupt_time = interrupt_time;
}

//This is the function used to sound the buzzer
void buzz(int reps, int rate) {
  for (int i = 0; i < reps; i++) {
    analogWrite(ledPin, 900);
    delay(100);
    analogWrite(ledPin, 0);
    delay(rate);
  }
}
