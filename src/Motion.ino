// Function used to calibrate the Accelerometer
void calibrateAccel() {
  float ax,ay,az;
  acc.getAcceleration(&ax,&ay,&az);

  //initialise x,y,z variables
  xMin = ax;
  xMax = ax;
  yMin = ay;
  yMax = ay;
  zMin = az;
  zMax = az;

  // Calibration sequence initialisation sound - 3 seconds before calibration begins
  buzz(2, 1000);

  //calibrate the Accelerometer (should take about 0.5 seconds)
  for (int i = 0; i < 50; i++) {
    if(acc.getAcceleration(&ax,&ay,&az) && az != 0) {
      Serial.print("get data ok: ");
    } else {
      i--;
      continue;
    }

    Serial.print(" ax = ");
    Serial.print(ax);
    Serial.print(" ay = ");
    Serial.print(ay);
    Serial.print(" az = ");
    Serial.println(az);

    // Calibrate X Values
    if (ax > xMax) {
      xMax = ax;
    } else if (ax < xMin) {
      xMin = ax;
    }

    // Calibrate Y Values
    if (ay > yMax) {
      yMax = ay;
    } else if (ay < yMin) {
      yMin = ay;
    }

    // Calibrate Z Values
    if (az > zMax) {
      zMax = az;
    } else if (az < zMin) {
      zMin = az;
    }

    //Delay 10msec between readings
    delay(15);
  }

  //End of calibration sequence sound. ARMED.
  buzz(3, 40);
  printValues(); //Only useful when connected to computer- using serial monitor.
  calibrated = true;
}

//Function used to detect motion. Tolerance variable adjusts the sensitivity of movement detected.
boolean checkMotion() {
  boolean moved = false;
  float ax,ay,az;

  if(acc.getAcceleration(&ax,&ay,&az) && az != 0) {
    // Serial.print("get data ok: ");
  } else {
    return moved;
  }

  // Serial.print(" ax = ");
  // Serial.print(ax);
  // Serial.print(" ay = ");
  // Serial.print(ay);
  // Serial.print(" az = ");
  // Serial.println(az);

  if (ax > (xMax + tolerance) || ax < (xMin - tolerance)) {
    moved = true;
    Serial.print("X Failed = ");
    Serial.println(ax);
  }

  if (ay > (yMax + tolerance) || ay < (yMin - tolerance)) {
    moved = true;
    Serial.print("Y Failed = ");
    Serial.println(ay);
  }

  if (az > (zMax + tolerance) || az < (zMin - tolerance)) {
    moved = true;
    Serial.print("Z Failed = ");
    Serial.println(az);
  }

  delay(10);

  return moved;
}
