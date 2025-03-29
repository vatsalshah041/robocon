#include <PS3BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
PS3BT PS3(&Btd); // This will just create the instance
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

bool printTemperature, printAngle;
int driver[2][2] = {{8,15}, {17,10}};
int pwm[2][2] = {{9,12},{13,11}};
int direct[2][2] = {{1, 0}, {0, 0}};
int clockwise[4] = {0, 0, 0, 0};
int axis = 0, sign = 0;
int count = 0;
int lhyUp, lhyDown, rhxRight, rhxLeft, l2, r2;
void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did ! start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      pinMode(driver[i][j], OUTPUT);
    }
  }
   for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      pinMode(pwm[i][j], OUTPUT);
    }
  }
}
void loop() {
  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    lhyUp = map(PS3.getAnalogHat(LeftHatY), 137, 255, 30, 100);
    //forward
    lhyDown = map(PS3.getAnalogHat(LeftHatY), 0, 117, 100, 30);
    //backward
    rhxRight = map(PS3.getAnalogHat(RightHatX), 137, 255, 30, 100);
    //forward
    rhxLeft = map(PS3.getAnalogHat(RightHatX), 0, 117, 100, 30);
    //backward
    l2 = map(PS3.getAnalogButton(L2), 10, 255, 30, 100);
    r2 = map(PS3.getAnalogButton(R2), 10, 255, 30, 100);


    if (PS3.getAnalogButton(L2) < 10 && PS3.getAnalogButton(R2) < 10) {
      if (( PS3.getAnalogHat(LeftHatY) < 137 && PS3.getAnalogHat(LeftHatY) > 117 )) {
        //stop
        analogWrite(pwm[axis][sign], 0);
        analogWrite(pwm[axis][!sign], 0);
      }
      if (( PS3.getAnalogHat(RightHatX) < 137 && PS3.getAnalogHat(RightHatX) > 117 )) {
        //stop
        analogWrite(pwm[!axis][sign], 0);
        analogWrite(pwm[!axis][!sign], 0);
      }
    }
    if ( PS3.getAnalogHat(LeftHatY) > 137 ) {
      //forward
        digitalWrite(driver[axis][sign], !sign);
        analogWrite(pwm[axis][sign], lhyUp);
        digitalWrite(driver[axis][!sign], sign);
        analogWrite(pwm[axis][!sign], lhyUp);
      //      digitalWrite(driver[1][0], HIGH);
      //      digitalWrite(driver[3][0], LOW);
      //      analogWrite(driver[1][1], lhyUp);
      //      analogWrite(driver[3][1], lhyUp);
    }
    if ( PS3.getAnalogHat(LeftHatY) < 117 ) {
      //backward
        digitalWrite(driver[axis][sign], !sign);
        analogWrite(pwm[axis][sign], lhyDown);
        digitalWrite(driver[axis][!sign], sign);
        analogWrite(pwm[axis][!sign], lhyDown);
      
      //      digitalWrite(driver[1][0], LOW);
      //      digitalWrite(driver[3][0], HIGH);
      //      analogWrite(driver[1][1], lhyDown);
      //      analogWrite(driver[3][1], lhyDown);
    }
    if ( PS3.getAnalogHat(RightHatX) > 137 ) {
      //right
//      for (int i = 0; i < 4; i = i + 2) {
//        digitalWrite(driver[i][0], direct[i]);
//        analogWrite(driver[i][1], rhxRight);
//      }
        digitalWrite(driver[!axis][sign], sign);
        analogWrite(pwm[!axis][sign], rhxRight);
        digitalWrite(driver[!axis][!sign], sign);
        analogWrite(pwm[!axis][!sign], rhxRight);
    }
    if ( PS3.getAnalogHat(RightHatX) < 117 ) {
      //left
        digitalWrite(driver[!axis][sign], sign);
        analogWrite(pwm[!axis][sign], rhxLeft);
        digitalWrite(driver[!axis][!sign], sign);
        analogWrite(pwm[!axis][!sign], rhxLeft);
      //      digitalWrite(driver[0][0], HIGH);
      //      digitalWrite(driver[2][0], HIGH);
      //      analogWrite(driver[0][1], rhxLeft);
      //      analogWrite(driver[2][1], rhxLeft);
    }

    if (PS3.getAnalogButton(L2) > 10) {
      //clockwise
      for (int i = 0; i < 4; i++) {
        digitalWrite(driver[i][0], clockwise[i]);
        analogWrite(driver[i][1], l2);
      }
      //      for (int i = 0; i < 4; i++) {
      //        analogWrite(driver[i][1], l2);
      //      }
    }
    if (PS3.getAnalogButton(R2) > 10) {
      //anti-clockwise
      for (int i = 0; i < 4; i++) {
        digitalWrite(driver[i][0], !(clockwise[i]));
        analogWrite(driver[i][1], r2);
      }
      //      for (int i = 0; i < 4; i++) {
      //        analogWrite(driver[i][1], r2);
      //      }
    }
    if (PS3.getButtonClick(L1)) {
        sign = !(axis ^ sign);
        axis = !axis;
    }
    if (PS3.getButtonClick(R1)) {
        sign = (axis ^ sign);
        axis = !axis;
    }
  }
}
