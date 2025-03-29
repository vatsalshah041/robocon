/*
  Example sketch for the PS5 Bluetooth library - developed by Kristian Sloth Lauszus
  For more information visit the Github repository: github.com/felis/USB_Host_Shield_2.0 or
  send me an e-mail: lauszus@gmail.com
*/

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

/* You can create the instancNe of the PS5BT class in two ways */
// This will start an inquiry and then pair with the PS5 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS5 controller will then start to blink rapidly indicating that it is in pairing mode
//  PS5BT PS5(&Btd, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
PS5BT PS5(&Btd);

bool printAngle = false, printTouch = false;
uint16_t lastMessageCounter = -1;
uint8_t player_led_mask = 0;
bool microphone_led = false;
uint32_t ps_timer;
int dir[] = { 17, 15};  //17,13 are right wheel
int pwm[] = { 13, 12 };  //15,12 are left wheel
bool arr[2] = { 1, 1 };
bool clockwise[2] = { 0, 1 };
int pins[2] = { 35, 37 };
int dwnwd, upwd, rtwd, lftwd, clk, anticlk;
int prev = 0; int a = 0;
int dir1[] = {17, 15};
int d[] = {1, 0};
int pwm1[] = {13, 12};
// int motor3 = ;

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial)
    ;  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1)
      ;  // Halt
  }
  Serial.print(F("\r\nPS5 Bluetooth Library Started"));

  for (int i = 0; i < 2; i++) {
    pinMode(dir[i], OUTPUT);
    pinMode(pwm[i], OUTPUT);
    pinMode(pins[i], OUTPUT);
  }
  digitalWrite(dir1[0], d[0]);
    digitalWrite(dir1[1], d[1]);
}

void loop() {
  Usb.Task();

  if (PS5.connected() && lastMessageCounter != PS5.getMessageCounter()) {

    lastMessageCounter = PS5.getMessageCounter();
    if (PS5.getButtonPress(PS)) {
      if (millis() - ps_timer > 1000)
        PS5.disconnect();
    }

    upwd = map(PS5.getAnalogHat(LeftHatY), 0, 117, 100, 0);
    dwnwd = map(PS5.getAnalogHat(LeftHatY), 137, 255, 0, 100);
    rtwd = map(PS5.getAnalogHat(RightHatX), 117, 0, 0, 100);
    lftwd = map(PS5.getAnalogHat(RightHatX), 137, 255, 0, 100);
    clk = map(PS5.getAnalogButton(L2), 0, 255, 0, 100);
    anticlk = map(PS5.getAnalogButton(R2), 0, 255, 0, 100);
    Serial.print(lftwd); Serial.print(" ");
    Serial.print(rtwd);
    Serial.println("");
    if (PS5.getAnalogButton(L2) < 10 || PS5.getAnalogButton(R2) < 10) {
      if (PS5.getAnalogHat(LeftHatY) < 137 && PS5.getAnalogHat(LeftHatY) > 117) {
        for (int i = 0; i < 2; i++) {
          analogWrite(pwm[i], 0);
        }
      }

      if (PS5.getAnalogHat(RightHatX) < 137 && PS5.getAnalogHat(RightHatX) > 117) {
        for (int i = 0; i < 2; i++) {
          analogWrite(pwm[i], 0);
        }
      }
    }

    if (PS5.getAnalogHat(LeftHatY) < 117)
      //forward
    {
      for (int i = 0; i < 2; i++) {
        digitalWrite(dir[i], arr[i]);
        analogWrite(pwm[i], upwd);
      }
    }
    if (PS5.getAnalogHat(LeftHatY) > 137)
      //backward
    {
      for (int i = 0; i < 2; i++) {
        digitalWrite(dir[i], !arr[i]);
        analogWrite(pwm[i], dwnwd);
      }
    }
    if (PS5.getAnalogHat(RightHatX) < 117)
    {
      digitalWrite(dir[1], clockwise[1]);
      analogWrite(pwm[1], rtwd);
    }
    if (PS5.getAnalogHat(RightHatX) > 137)
    {
      digitalWrite(dir[0], !clockwise[0]);
      analogWrite(pwm[0], lftwd);
    }

    if (PS5.getAnalogButton(L2) > 10) {
      //clockwise

      for (int i = 0; i < 2; i++) {
        digitalWrite(dir[i], clockwise[i]);
        analogWrite(pwm[i], clk);
      }
    }
    if (PS5.getAnalogButton(R2) > 10) {
      //anticlockwise
      for (int i = 0; i < 2; i++) {
        digitalWrite(dir[i], !clockwise[i]);
        analogWrite(pwm[i], anticlk);
      }
    }

    if (PS5.getButtonPress(PS)) {
      if (millis() - ps_timer > 1000)
        PS5.disconnect();
    } else
      ps_timer = millis();

    if (PS5.getButtonClick(PS))
      Serial.print(F("\r\nPS"));

  }
}
