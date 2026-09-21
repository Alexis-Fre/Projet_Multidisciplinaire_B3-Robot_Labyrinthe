# FS90R Calibration Guide / Guide de Calibration du FS90R
## Français : Étapes de Calibration

### Option A : Avec une carte Arduino
1. **Brancher le servo :** Connectez le fil marron au `GND`, le fil rouge au `5V`, et le fil orange (signal) à une broche PWM (par ex. la `Broche 9`).
2. **Téléverser le code d'arrêt :** Envoyez une impulsion de 1500µs (ou position `90`) pour donner l'ordre d'arrêt.
   ```cpp
   #include <Servo.h>
   Servo myServo;
   void setup() {
     myServo.attach(9);
     myServo.writeMicroseconds(1500); // Signal d'arrêt standard
   }
   void loop() {}
   ```
3. **Ajuster le potentiomètre :** Si le moteur tourne lentement, utilisez un tournevis miniature pour tourner délicatement la vis de réglage du potentiomètre interne (située sous le boîtier) jusqu'à l'arrêt complet.

### Option B : Avec un Générateur Basse Fréquence (GBF)
1. **Configurer le signal :** Réglez votre GBF pour délivrer un **Signal Carré** d'une fréquence de **50 Hz** (période de 20ms).
2. **Régler le rapport cyclique :** Ajustez la largeur d'impulsion (temps haut) précisément à **1,5 ms** (1500 microsecondes), ce qui correspond à un rapport cyclique de 7,5 %.
3. **Câblage :** Reliez la sortie signal du GBF au fil orange et la masse du GBF au fil marron. *Attention : Alimentez le servo (fil rouge +5V / fil marron GND) avec une alimentation continue externe en reliant bien toutes les masses (GND).*
4. **Calibrer :** Tournez la vis de réglage située sous le servo jusqu'à ce que l'axe s'arrête complètement de tourner et que les vibrations cessent.

---

## English: Calibration Steps

### Option A: Using an Arduino
1. **Connect the Servo:** Connect the brown wire to `GND`, the red wire to `5V`, and the orange signal wire to a PWM pin (e.g., `Pin 9`).
2. **Upload the Stop Code:** Upload a sketch sending a 1500µs pulse (or position `90`) to force a stop command.
   ```cpp
   #include <Servo.h>
   Servo myServo;
   void setup() {
     myServo.attach(9);
     myServo.writeMicroseconds(1500); // Standard stop signal
   }
   void loop() {}
   ```
3. **Adjust the Potentiometer:** If the motor creeps or spins, use a miniature screwdriver to slowly turn the internal potentiometer adjustment screw (located in the bottom access hole) until the motor stops completely.

### Option B: Using a Function Generator (GLF)
1. **Configure the Signal:** Set your GLF to output a **Square Wave** with a frequency of **50 Hz** (a 20ms period). 
2. **Set the Duty Cycle:** Adjust the pulse width (High time) exactly to **1.5 ms** (1500 microseconds). This represents a 7.5% duty cycle.
3. **Power & Connect:** Connect the generator's signal output to the orange wire and the generator's ground to the brown wire. *Note: Power the red (+5V) and brown (GND) wires using an external DC power supply, ensuring a shared ground.*
4. **Calibrate:** Turn the servo's bottom trimmer screw until the rotation drops to zero and any humming noise disappears.
