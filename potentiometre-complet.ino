//1er prog avec entrée sortie sur circuit imprimé poubelle
const int capteur_inductif = 13;
const int avance = 17;
const int arriere = 16;
const int enable = 4;   //sortie PWM avec LedC pour faire varier la vitesse
const int valeur_verre = 1410;
const int valeur_plastique = 1393;
int courant = 0;
int sortie_boucle = 0 ;
//potentiomètre palpeur sur A4
//courant palpeur sur A5

// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0

#define LEDC_TIMER_13_BIT  13

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     10000    //Dans l'exemple la fréquence de 5000 produisait un bruit important dans le moteur


void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(capteur_inductif, INPUT);
  pinMode(avance, OUTPUT);
  pinMode(arriere, OUTPUT);
  digitalWrite(avance, LOW);
  digitalWrite(arriere, LOW);
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(enable, LEDC_CHANNEL_0);

}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  // print out the value you read:
  //Serial.println(sensorValue);
  sortie_boucle = 0 ;
  while (( analogRead(A4) < 4000 ) && (sortie_boucle == 0))  //*************sortie Palpeur ( capteur_inductif == 0)
  { digitalWrite(avance, HIGH);
    ledcWrite( LEDC_CHANNEL_0, 6500);   //rapport cyclique de 0 à 8191 (6000 est le minimum pour déplacer le curseur)
    courant = analogRead(A5);
    Serial.println(courant);
    if (1380 < courant && courant <= valeur_plastique ) {
      Serial.println("direction bac plastique");  //direction bac plastique
      sortie_boucle++;
      Serial.println(courant);
    } else if ((valeur_plastique > courant) && (courant >= valeur_verre)) {
      Serial.println("direction bac verre"); //direction bac verre
      sortie_boucle++;
      Serial.println(courant);
    }
  }
    digitalWrite(avance, LOW);
    ledcWrite( LEDC_CHANNEL_0, 0);   //rapport cyclique de 0 à 8191
    delay(5000);
    if ( capteur_inductif == 1)
    {
      //direction bac metal

    }

    while ( analogRead(A4) > 100 )  //*************Rentrée Palpeur
    { digitalWrite(arriere, HIGH);
      ledcWrite( LEDC_CHANNEL_0, 6500);   //rapport cyclique de 0 à 8191 (6000 est le minimum pour déplacer le curseur)
    }
    digitalWrite(arriere, LOW);
    ledcWrite( LEDC_CHANNEL_0, 0);   //rapport cyclique de 0 à 8191
    delay(5000);


  }
