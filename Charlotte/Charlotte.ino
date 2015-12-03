#include <Servo.h>

int inVal, currVal;
const int LEGS = 3;
Servo servo[LEGS];

int pos[LEGS] = { 20, 20, 20 };
int avg[LEGS] = {  0,  0,  0 };
int  in[LEGS] = {  0,  1,  7 };
int out[LEGS] = {  2, 10, 12 };

long seconds=0;
float speed = 0.002;

void setup() {
  for(int i=0; i<LEGS; i++) {
    servo[i].attach(out[i]);
  }
  Serial.begin(9600);
}

void loop() {

  // update avg once per second
  if (millis() / 1000 > seconds){
    for(int i=0; i<LEGS; i++) {
      avg[i] += analogRead(in[i]);
      avg[i] /= 2;
    }
    seconds++;
    //speed = random(300) / 100000.0;
  }  

  for(int i=0; i<LEGS; i++) {
    inVal = analogRead(in[i]) - avg[i];
    inVal = map(val, -800, 800, 0, 179);
    pos[i] += constrain(inVal, 50, 140);
    pos[i] /= 2;
    
    // currVal = pos[i];
    
    // yoga
    // currVal = int(95 + 55 * (sin(millis() * speed)));
    
    // turn around. Sign of +i decides direction
    // the 0.005 decides the rotation speed
    // currVal = int(95 + 55 * sin(millis() * 0.005 + i * 2.07));
    
    servo[i].write(currVal);
    
    delay(15);
  }

  Serial.print(pos[0]);
  Serial.print(", ");
  Serial.print(pos[1]);
  Serial.print(", ");
  Serial.print(pos[2]);
  Serial.println();

}



