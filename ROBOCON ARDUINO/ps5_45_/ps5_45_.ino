#include <PS5BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb);  // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
PS5BT PS5(&Btd);  // This will just create the instance
//PS5BT PS5(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

bool printTemperature, printAngle;
bool arr[4] = { 0, 1, 1, 0 };
bool clockwise[4] = { 1, 1, 1, 1 };
bool arr1[4] = { 1, 1, 0, 0 };

int dir[4] = { 17, 10, 8, 15 };
int pwm[4] = { 13, 11, 9, 12 };

//int dir[][]={{E,W},{N,S}};

int dwnwd, upwd, rtwd, lftwd, clk, anticlk;
void setup() {
  Serial.begin(115200);
#if !defined(_MIPSEL_)
  while (!Serial)
    ;  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1)
      ;  //halt
  }
  Serial.print(F("\r\nPS5 Bluetooth Library Started"));
  for (int i = 0; i < 4; i++) {
    pinMode(dir[i], OUTPUT);
    pinMode(pwm[i], OUTPUT);
  }
}
void loop() {
  Usb.Task();

  // if (PS5.PS5Connected || PS5.PS5NavigationConnected) {
  //   if (PS5.getAnalogHat(LeftHatX) > 137 || PS5.getAnalogHat(LeftHatX) < 117 || PS5.getAnalogHat(LeftHatY) > 137 || PS5.getAnalogHat(LeftHatY) < 117 || PS5.getAnalogHat(RightHatX) > 137 || PS5.getAnalogHat(RightHatX) < 117 || PS5.getAnalogHat(RightHatY) > 137 || PS5.getAnalogHat(RightHatY) < 117) {


  if (PS5.PS5Connected || PS5.PS5NavigationConnected) {
    upwd = map(PS5.getAnalogHat(LeftHatY), 137, 255, 0, 100);
    dwnwd = map(PS5.getAnalogHat(LeftHatY), 117, 0, 0, 100);
    rtwd = map(PS5.getAnalogHat(RightHatX), 137, 255, 0, 100);
    lftwd = map(PS5.getAnalogHat(RightHatX), 117, 0, 0, 100);
    clk = map(PS5.getAnalogButton(L2), 0, 255, 0, 100);
    anticlk = map(PS5.getAnalogButton(R2), 0, 255, 0, 100);
    if (PS5.getAnalogButton(L2) < 10 || PS5.getAnalogButton(R2) < 10) {  //stopping condition
      if (PS5.getAnalogHat(LeftHatY) < 137 && PS5.getAnalogHat(LeftHatY) > 117) {
        for (int i = 0; i < 4; i = i + 2) {
          analogWrite(pwm[i], 0);
        }
      }

      if (PS5.getAnalogHat(RightHatX) < 137 && PS5.getAnalogHat(RightHatX) > 117) {
        for (int i = 1; i < 4; i = i + 2) {
          analogWrite(pwm[i], 0);
        }
      }
    }

    if (PS5.getAnalogHat(LeftHatY) > 137)
    //forward
    {
      for (int i = 0; i < 4; i = i + 3) {
        digitalWrite(dir[i], !arr[i]);
        analogWrite(pwm[i], upwd);
      }
      for (int i = 1; i < 4; i++) {
        digitalWrite(dir[i], arr[i]);
        analogWrite(pwm[i], upwd);
      }
    }
    if (PS5.getAnalogHat(LeftHatY) < 117)
    //backward
    {
      for (int i = 0; i < 4; i = i + 3) {
        digitalWrite(dir[i], arr[i]);
        analogWrite(pwm[i], dwnwd);
      }
      for (int i = 1; i < 4; i++) {
        digitalWrite(dir[i], !arr[i]);
        analogWrite(pwm[i], dwnwd);
      }
    }

    if (PS5.getAnalogHat(RightHatX) > 137)
    //rightwards
    {
      for (int i = 0; i < 4; i++) {
        digitalWrite(dir[i], arr1[i]);
        analogWrite(pwm[i], rtwd);
      }
    }
    if (PS5.getAnalogHat(RightHatX) < 117) {
      //leftwards
      for (int i = 0; i < 4; i++) {
        digitalWrite(dir[i], !arr1[i]);
        analogWrite(pwm[i], lftwd);
      }
    }

    if (PS5.getAnalogButton(L2) > 10) {
      //clockwise
      for (int i = 0; i < 4; i++) {
        digitalWrite(dir[i], clockwise[i]);
        analogWrite(pwm[i], clk);
      }
    }
    if (PS5.getAnalogButton(R2) > 10) {
      //anticlockwise
      for (int i = 0; i < 4; i++) {
        digitalWrite(dir[i], !clockwise[i]);
        analogWrite(pwm[i], anticlk);
      }
    }
  }
}
