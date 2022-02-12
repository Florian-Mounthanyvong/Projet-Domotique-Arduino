#include <Servo.h> //Librairie pour le servomoteur
#include <IRremote.h> //Librairie pour la télécommande

//Broche où récepteur infrarouge est connecté
const char DIN_RECEPTEUR_INFRAROUGE = 2;
//On déclare l'utilisation d'un module infrarouge
IRrecv monRecepteurInfraRouge (DIN_RECEPTEUR_INFRAROUGE);
//Variable pour stocker le message reçu
decode_results messageRecu;

Servo monServomoteur;

int x = 0; //Variable pour blindage
const int L1 = 3; //On relie la Led à l'entrée 3


void setup() {

  // Initialise la communication avec le PC
  Serial.begin(9600);
  //On relie le servomoteur à l'entrée 9
  monServomoteur.attach(9);
  //On active le module infrarouge
  monRecepteurInfraRouge.enableIRIn();
  pinMode(L1, OUTPUT);
}

// Fonction appelée continuellement tant que la carte Arduino est alimentée
void loop() {
  //Fonction jour/OFF
  if (monRecepteurInfraRouge.decode(&messageRecu))//Détecte la présence d'un message sur la transmission
  {
    if (x == 0) //Blindage pour que la fonction OFF ne peux pas être relancée si elle est déjà en place
    {
      if (messageRecu.value == 0xFD30CF)//Si la touche 0 de la télécommande est appuyée (0xHEX)
      {
        Serial.println("Touche 0");//Pour vérifier via le moniteur série que tout va bien
        digitalWrite(L1, HIGH);//On éteint la Led

        for (int position = 0; position <= 180; position++)//Le servomoteur effectue un demi-tour/On ouvre les volets
        {
          monServomoteur.write(position);//Fait bouger le servomoteur progressivement
        }
        x = 1;//Blindage
      }
    }
    if (x == 1)//Blindage pour que la fonction ON ne peux pas être relancée si elle est déjà en place
    {
      if (messageRecu.value == 0xFD08F7)//Si la touche 1 de la télécommande est appuyée (FD08F7 est la touche 1 en HEX)
      {
        Serial.println("Touche 1");//Pour vérifier via le moniteur série que tout va bien
        digitalWrite(L1, LOW);//On allume la Led

        for (int position = 180; position > 0; position--)//Le servomoteur effectue un demi-tour en arrière/On ferme les volets
        {
          monServomoteur.write(position);//Fait bouger le servomoteur progressivement
        }
        x = 0;//Blindage
      }
    }
    delay(500);//On rajoute un délai de 500 ms
    monRecepteurInfraRouge.resume();//On reset la saisie de touches, le récepteur peut reçevoir de nouveaux messages

  }
}
