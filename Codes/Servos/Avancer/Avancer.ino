#include <Servo.h>

Servo myservo1;  // create Servo object to control a servo
Servo myservo2;

int pos = 2;    // variable to store the servo position

void setup() {
  myservo1.attach(9);  // attaches the servo on pin 9 to the Servo object
  myservo2.attach(8);
  Serial.begin(9600);
}

int marche = 1;
void loop() {
  Serial.println(marche);
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
    if (marche == 1) {
      // in steps of 1 degree
      myservo1.write(-pos);              // tell servo to go to position in variable 'pos'
      myservo2.write(pos);
      delay(15);                       // waits 15 ms for the servo to reach the position
      Serial.println("Avancer");
      Serial.println(pos);
      marche = 0;
      Serial.println("Terminer");
    } else {
      Serial.println("Arrêt");
      myservo1.write(90);              // tell servo to go to position in variable 'pos'
      myservo2.write(90);
    }
  }
  /*for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo1.write(-pos);              // tell servo to go to position in variable 'pos'
    myservo2.write(pos);
    delay(15);                       // waits 15 ms for the servo to reach the position
    Serial.println("Reculer");
  }*/
}
