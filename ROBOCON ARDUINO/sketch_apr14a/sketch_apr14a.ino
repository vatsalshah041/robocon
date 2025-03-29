
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
PS5BT PS5(&Btd);
bool printAngle = false, printTouch = false;
uint16_t lastMessageCounter = -1;
uint8_t player_led_mask = 0;
bool microphone_led = false;
uint32_t ps_timer;

int pin1=31;
int pin2=33;
int dir[] = { 17, 15};  //17,13 are right wheel
int pwm[] = { 13, 12};  //15,12 are left wheel
bool arr[2] = { 1, 1 };
int m[] = {0,0};

int motor3[] = {8, 9};

int topspeed = 100;

//int dir2[] = {17, 15};
//int d2[] = {1,0};
//int pwm2[] = {13, 12};
int lhyUp, lhyDown, rhxRight, rhxLeft, x = 0, y = 0;
//int clk, anticlk;

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
  }
  pinMode(pin1,OUTPUT);
  pinMode(pin2,OUTPUT);

  pinMode(motor3[0],OUTPUT);
  pinMode(motor3[1],OUTPUT);
//  digitalWrite(dir2[0], d2[0]);
//  digitalWrite(dir2[1], d2[1]);
}

void loop() {
  Usb.Task();

  if (PS5.connected() && lastMessageCounter != PS5.getMessageCounter()) {

    lastMessageCounter = PS5.getMessageCounter();
    if (PS5.getButtonPress(PS)) {
      if (millis() - ps_timer > 1000)
        PS5.disconnect();
    }

    if(PS5.getAnalogHat(LeftHatY) < 117){
    y = map(PS5.getAnalogHat(LeftHatY), 0,117,topspeed,0);
    //forward
    }else if(PS5.getAnalogHat(LeftHatY) > 137){
    y = map(PS5.getAnalogHat(LeftHatY), 137, 255, 0, -topspeed);
    //backward
    }
    else{
      y = 0;
    }

    if(PS5.getAnalogHat(RightHatX) < 117){
    x = map(PS5.getAnalogHat(RightHatX), 117, 0, 0, topspeed);
    //forward
    }else if(PS5.getAnalogHat(RightHatX) > 137){
    x = map(PS5.getAnalogHat(RightHatX), 137, 255, 0, -topspeed);
    //backward
    }
    else{
      x=0;
    }
    Serial.print(y);
    Serial.print("   ");
    Serial.println(x);
 
    m[0] = y - x;
    m[1] = y + x;


    for(int i=0;i<2;i++){
      if(m[i] >= 0){
        
        digitalWrite(dir[i], arr[i]);
      }
      else if(m[i] < 0){
        digitalWrite(dir[i], !arr[i]);
      }
      if(m[i] > topspeed || m[i] < -topspeed){
        m[i] = topspeed;
      }
      analogWrite(pwm[i], abs(m[i]));
      
    }

    
    if (PS5.getAnalogButton(L2)>10) {
      digitalWrite(motor3[0], HIGH);
      analogWrite(motor3[1], 50);
    }
    else if(PS5.getAnalogButton(R2)>10)
    {
      digitalWrite(motor3[0], LOW);
      analogWrite(motor3[1], 50);
    }
    else{
      analogWrite(motor3[1], 0);
    }
      
    if (PS5.getButtonClick(TRIANGLE)) {
        digitalWrite(pin1,LOW);
        digitalWrite(pin2,HIGH);
        delay(3000);
        digitalWrite(pin1,LOW);
        digitalWrite(pin2,LOW);
    }
    if (PS5.getButtonClick(SQUARE)) {
        digitalWrite(pin1,HIGH);
        digitalWrite(pin2,LOW);
        delay(3000);
        digitalWrite(pin1,LOW);
        digitalWrite(pin2,LOW);
    }
    
  }
}
