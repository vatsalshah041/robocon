int trig=10;
int echo=11;
void setup()
{
  Serial.begin(9600);
}

void loop()
{
   long duration, inches, cm;
   pinMode(trig, OUTPUT);
   digitalWrite(trig, LOW);
   delayMicroseconds(2);
   digitalWrite(trig, HIGH);
   delayMicroseconds(10);
   digitalWrite(trig, LOW);
   pinMode(echo, INPUT);
   duration = pulseIn(echo, HIGH);
   inches = mi(duration);
   cm = mc(duration);
   Serial.print(inches);
   Serial.print("in, ");
   Serial.print(cm);
   Serial.print("cm");
   Serial.println();
   delay(100);
}
long mi(long microseconds) {
   return microseconds / 74 / 2;
}

long mc(long microseconds) {
   return microseconds / 29 / 2;
}
