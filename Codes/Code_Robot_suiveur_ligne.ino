#include <QTRSensors.h>
QTRSensors qtr;

#define Kp 0.018
#define Kd 0.0001
#define MaxSpeed 180
#define BaseSpeed 150
#define SpeedTurn 150
#define CheckPoint 800

const uint8_t SensorCount = 4;
// IR Sensors Count 
uint16_t sensorValues[SensorCount];

int lastError = 0;

void setup()
{   
    qtr.setTypeAnalog();
    qtr.setSensorPins((const uint8_t[]){A3, A2, A1, A0}, SensorCount);

    Serial.begin(9600);
    delay(500);
    
    pinMode(LED_BUILTIN, OUTPUT);

    // LED turns ON to indicate the start of the calibration
    digitalWrite(LED_BUILTIN, HIGH);
    delay(3000);
    
    for (int i = 0; i < 400; i++)
    {   
        qtr.calibrate();
        delay(20);
    }

    digitalWrite(LED_BUILTIN, LOW);
    delay(3000); 
}  

void loop()
{  
  followLine();
}

void followLine(){

    uint16_t position = qtr.readLineBlack(sensorValues);  
    for (uint8_t i = 0; i < SensorCount; i++)
    {
      Serial.print(sensorValues[i]);
      Serial.print('\t');
    }
    Serial.println(position);

    // too Right, Robot will go left
    if(position>3800)  
    {
      Serial.println("Aller à gauche");
      return;    
    }

    // too Left, Robot will go right
    if(2000 < position<2900)
    {  
      Serial.println("Aller à droite");
      return;
    }

    int error = position - 3500;
    int motorSpeed = Kp * error + Kd * (error - lastError);
    lastError = error;

    Serial.print("Error: ");
    Serial.println(error);
}


