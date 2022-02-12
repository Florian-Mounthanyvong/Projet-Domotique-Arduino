#include <Servo.h>

Servo monServomoteur;
int x = 0;
const int L1 = 3;
void setup() {


  monServomoteur.attach(9);
  // Initialise la communication avec le PC
  Serial.begin(9600);
  pinMode(L1, OUTPUT);
}

// Fonction appelée continuellement tant que la carte Arduino est alimentée
void loop() {
  // On mesure la tension sur la broche A0
  int valeur = analogRead(A0);
  //FONCTION JOUR/OFF:
  //On va considérer qu'à partir de 200 lux captés, il fait jour. On blinde pour que cette fonction "jour" ne se relance pas tant qu'il fait jour/tant qu'il ne fait pas nuit
  if (valeur > 200 && x == 0)
  {
    digitalWrite(L1, HIGH);//La Led s'éteint
    for (int position = 0; position <= 180; position++) //Le servomoteur effectue un demi-tour
    {
      monServomoteur.write(position);//Le servomoteur bouge progressivement
    }

    x = 1; //Blindage Jour->Nuit
  }
  //FONCTION NUIT/ON
  //Blindage pour que la fonction "nuit" ne se lance pas la nuit. On considère que si on détecte 200 lux ou moins, il fait nuit
  if (valeur <= 200 && x == 1)
  {
    digitalWrite(L1, LOW);//La Led s'allume
    for (int position = 180; position > 0; position--)//Le servomoteur fait un demi-tour en arrière
    {
      monServomoteur.write(position);//Le servomoteur effectue le demi-tour progressivement
    }

    x = 0; //Blindage
  }
  // Envoi la mesure au PC pour affichage
  Serial.println(valeur);
}
