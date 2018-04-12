//1er prog avec entrée sortie sur circuit imprimé poubelle
#include <Arduino.h>
#include "BasicStepperDriver.h"
const int capteur_inductif = 13;
const int avance = 17;
const int arriere = 16;
const int enable = 4;   //sortie PWM avec LedC pour faire varier la vitesse
const int valeur_verre = 1410;
const int valeur_plastique = 1393;
int courant = 0;
int courant1 = 0;
int courant2 = 0;
int sortie_boucle = 0 ;
//potentiomètre palpeur sur A4
//courant palpeur sur A5

#define MOTOR_STEPS 200
#define RPM 120

#define MICROSTEPS 1 // 1 = Full step, 2 = half step

#define DIR 15
#define STEP 2
#define ENABLE 0

BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

//Uncomment line to use enable/disable functionality
//BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, ENABLE);

// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0

#define LEDC_TIMER_13_BIT  13

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     10000    //Dans l'exemple la fréquence de 5000 produisait un bruit important dans le moteur


void setup() {
  // initialize serial communication at 9600 bits per second:
  stepper.begin(RPM, MICROSTEPS);
  Serial.begin(9600);
  pinMode(capteur_inductif, INPUT);
  pinMode(avance, OUTPUT);
  pinMode(arriere, OUTPUT);
  digitalWrite(avance, LOW);
  digitalWrite(arriere, LOW);
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(enable, LEDC_CHANNEL_0);

}

void loop() {
  // read the input on analog pin 0:
  // print out the value you read:
  //Serial.println(sensorValue);
  sortie_boucle = 0 ;
  courant = 0;
  courant1 = 0;
  courant2 = 0;
  while (( analogRead(A4) < 4000 ) && (sortie_boucle == 0))  //*************sortie Palpeur ( capteur_inductif == 0)
  { digitalWrite(avance, HIGH);
    ledcWrite( LEDC_CHANNEL_0, 6500);   //rapport cyclique de 0 à 8191 (6000 est le minimum pour déplacer le curseur)
    courant = analogRead(A5);
    Serial.println(courant);
    delay(200);
    if ( courant > 5 ) {
      courant1 = analogRead(A5);
      ledcWrite(LEDC_CHANNEL_0, 8191);
      delay(500);
      courant2 = analogRead(A5);
      Serial.println(courant1);
      Serial.println(courant2);
      Serial.print("difference:");
      Serial.println(courant2 - courant1);
      if ( 977 <= (courant2 - courant1) && (courant2 - courant1) <= 1000) {
        Serial.println("direction bac verre1");  //direction bac verre
        sortie_boucle++;
      } else if ( 970 <= (courant2 - courant1) && (courant2 - courant1) <= 976) {
        Serial.println("direction bac plastique2"); //direction bac plastique
        sortie_boucle++;

      }
    }
    digitalWrite(avance, LOW);
    ledcWrite( LEDC_CHANNEL_0, 0);
    sortie_boucle = 0; //rapport cyclique de 0 à 8191
    delay(5000);
    if ( capteur_inductif == 1)
    {
      /*
        stepper.rotate(360);
        stepper.move(-MOTOR_STEPS*MICROSTEPS);
        delay(5000);
      */
    }

    while ( analogRead(A4) > 100 )  //*************Rentrée Palpeur
    { digitalWrite(arriere, HIGH);
      ledcWrite( LEDC_CHANNEL_0, 6500);   //rapport cyclique de 0 à 8191 (6000 est le minimum pour déplacer le curseur)
      delay(100);
    }
    digitalWrite(arriere, LOW);
    ledcWrite( LEDC_CHANNEL_0, 0);   //rapport cyclique de 0 à 8191
    delay(5000);


  }
}
