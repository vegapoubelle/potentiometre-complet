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
int position1 = 0;
int position2 = 0;
int bac_plastique = 0;
int bac_verre = 0;
int bac_metal = 0;
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
  Serial.println(analogRead(A4));
}

void loop() {
  // read the input on analog pin 0:
  // print out the value you read:
  //Serial.println(sensorValue);
  bac_verre = 0;
  bac_plastique = 0;
  bac_metal = 0;
  courant = 0;
  position1 = 0;
  position2 = 0;
  Serial.println(analogRead(A4));
  while ( analogRead(A4) < 4000 ) //*************sortie Palpeur ( capteur_inductif == 0)
  { digitalWrite(avance, HIGH);
    ledcWrite( LEDC_CHANNEL_0, 8191);   //rapport cyclique de 0 à 8191 (6000 est le minimum pour déplacer le curseur)
    courant = analogRead(A5);
    Serial.println(courant);
    delay(300);
    if ( courant > 590 ) {
      position1 = analogRead(A4);
      Serial.print(position1);
      ledcWrite(LEDC_CHANNEL_0, 8191);
      delay(100);
      position2 = analogRead(A4);
      Serial.print(" ; ");
      Serial.println(position2);
      if (position2 - position1 > 0 ) {
        Serial.println("direction bac plastique");  //direction bac verre
        bac_verre++;
      } else if (position2 - position1 == 0 ) {
        Serial.println("direction bac verre"); //direction bac plastique
        bac_plastique++;


      }
    }
  }
  digitalWrite(avance, LOW);
  ledcWrite( LEDC_CHANNEL_0, 0); //rapport cyclique de 0 à 8191
  delay(5000);
  /* if ( capteur_inductif == 1)
//    {

       stepper.rotate(360);
       stepper.move(-MOTOR_STEPS*MICROSTEPS);
       delay(5000);
     }
  */


  while ( analogRead(A4) > 3500 )  //*************Rentrée Palpeur
  { Serial.println(analogRead(A4));
    Serial.println("rentree palpeur");
    digitalWrite(arriere, HIGH);
    ledcWrite( LEDC_CHANNEL_0, 8191);   //rapport cyclique de 0 à 8191 (6000 est le minimum pour déplacer le curseur)
    delay(100);
  }
  digitalWrite(arriere, LOW);
  ledcWrite( LEDC_CHANNEL_0, 0);   //rapport cyclique de 0 à 8191
  delay(5000);


}

