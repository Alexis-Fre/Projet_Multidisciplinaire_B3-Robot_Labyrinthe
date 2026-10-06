#include <Wire.h>  // Bibliothèque I2C
#include "Adafruit_VL6180X.h"  // Bibliothèque / Driver pour le capteur de distance / lumière

Adafruit_VL6180X capteur_1 = Adafruit_VL6180X(); // Créer un objet I2C pour le capteur 1
Adafruit_VL6180X capteur_2 = Adafruit_VL6180X(); // Créer un objet I2C pour le capteur 1
Adafruit_VL6180X capteur_3 = Adafruit_VL6180X(); // Créer un objet I2C pour le capteur 1


int const num_capteur[] = { 0, 1, 2 }

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    delay(1);
  }
  
  Serial.println("VL6180x test!");
  if (! capteur_1.begin()) { // Initialisation du capteur
    Serial.println("Failed to find sensor");
    while (1);
  }
  Serial.println("Sensor found!");
}

void loop() {
  float lux = capteur_1.readLux(VL6180X_ALS_GAIN_5); // Lecture de la lumière mais permet de mesurer la distance aussi. 
                                                     // Il faut une valeur de gain faible pour une meilleur estimation de la distance avec une luminosité haute et inversement
  // Variable de gain :
  // VL6180X_ALS_GAIN_1
  // VL6180X_ALS_GAIN_1_25
  // VL6180X_ALS_GAIN_1_67
  // VL6180X_ALS_GAIN_2_5
  // VL6180X_ALS_GAIN_5
  // VL6180X_ALS_GAIN_10
  // VL6180X_ALS_GAIN_20
  // VL6180X_ALS_GAIN_40

  Serial.print("Lux: "); Serial.println(lux);
  
  uint8_t range = capteur_1.readRange(); // Lecture de la distance (entre 0 et 200)
  uint8_t status = capteur_1.readRangeStatus(); // Permet d'avoir des erreurs de déjà renseigné

  if (status == VL6180X_ERROR_NONE) {
    Serial.print("Range: "); Serial.println(range);
  }

  // Some error occurred, print it out!
  
  if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    Serial.println("System error");
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    Serial.println("ECE failure");
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    Serial.println("No convergence");
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    Serial.println("Ignoring range");
  }
  else if (status == VL6180X_ERROR_SNR) {
    Serial.println("Signal/Noise error");
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    Serial.println("Raw reading underflow");
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    Serial.println("Raw reading overflow");
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    Serial.println("Range reading underflow");
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    Serial.println("Range reading overflow");
  }
  delay(50);
}
