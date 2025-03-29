int ir=8;
int led=13;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(ir,INPUT);
pinMode(led,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int s=digitalRead(ir);
  if(s==1)
  {
    digitalWrite(led,HIGH);
    Serial.println("1");
  }
  else
  {
    digitalWrite(led,LOW);
    Serial.println("0");
  }
}
