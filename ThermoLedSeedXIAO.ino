//-----------------------------Fabriquons un thermomètre avec affichage sur bandeau de LEDs--------------------------------
// Claude DFOURMONT
// Mon YouTube : https://www.youtube.com/c/ClaudeDufourmont
// Mon GitHub : https://github.com/claudiusmarius
// Vidéo numérotée : DFT_#A186
// Utilisation d'un Seeeduino XIAO d' un LM35DZ ainsi qu'un bandeau de 10 LEDs adressables WS2812B pour la démo
// Afin d'afficher une température sur ce bandeau
// ========================================================================================================================
// ****** N'hésitez pas à liker, commenter, partager et à vous abonner, cela contribue à promouvoir la chaine, merci ******
// ========================================================================================================================

  #include <Adafruit_NeoPixel.h>

  #define LED_PIN D3                                   // Broche connectée au bandeau
  #define NUM_LEDS 10                                  // Nombre de LEDs dans le bandeau
  #define LM35_PIN A1                                  // Broche analogique A1 où le LM35DZ est connecté
  
  Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

  float correction = -1.0;                             // Correction positive ou négative (en °C)

  void setup() 
  {
  Serial.begin(9600);                                   // Initialisation série
  analogReadResolution(12);                             // Configuration de la résolution ADC
  strip.begin();                                        // Initialisation du bandeau LED
  strip.show();                                         // Extinction de toutes les LEDs au démarrage
  }

  void loop() 
  {
  int analogValue = analogRead(LM35_PIN);               // Lecture de la température
  float voltage = analogValue * (3.3 / 4095.0);
  float temperature = voltage * 100.0;

  temperature += correction;                            // Appliquer la correction à la température

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  updateLEDs(temperature);                              // Mettre à jour les LEDs
  delay(1000);
  }

  void updateLEDs(float temperature) 
  {
  //------------------------------------------------------Extinction de toutes les LEDs-----------------------------------------------------
  strip.clear();

  //-----------------------------------------------Conversion de la température en index de LED---------------------------------------------
  int ledIndex = map(temperature, 18, 27, 0, NUM_LEDS - 1); // Cette échelle de température est adaptée au bandeau de 10 LEDs expérimental
  ledIndex = constrain(ledIndex, 0, NUM_LEDS - 1); // S'assurer que l'index reste dans les limites

  //-------------------------------------------Mise à jour des LEDs en fonction de la température--------------------------------------------
  for (int i = 0; i < NUM_LEDS; i++) 
  {
    if (i < ledIndex) {
      // LEDs en dessous de la température actuelle (rouge, luminosité réduite)
      strip.setPixelColor(i, strip.Color(2, 0, 0)); // Rouge
    } else if (i == ledIndex) {
      // LED correspondant à la température actuelle (Jaune, moyenne luminosité)
      strip.setPixelColor(i, strip.Color(20, 20, 0)); // Jaune
    } else {
      // LEDs au-dessus de la température actuelle (bleu, luminosité réduite)
      strip.setPixelColor(i, strip.Color(0, 0, 2)); // Bleu
    }
  }

  //--------------------------------------------------------------Mise à jour du bandeau------------------------------------------------------
  strip.show();
  }
