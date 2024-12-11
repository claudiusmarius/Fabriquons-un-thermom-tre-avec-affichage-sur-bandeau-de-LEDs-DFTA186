//-----------------------------Fabriquons un thermomètre avec affichage sur bandeau de LEDs--------------------------------
// Claude DFOURMONT
// Mon YouTube : https://www.youtube.com/c/ClaudeDufourmont
// Mon GitHub : https://github.com/claudiusmarius
// Vidéo numérotée : DFT_#A186
// Utilisation d'un ATtiny85 d'un LM35DZ ainsi qu'un bandeau de 10 LEDs adressables WS2812B pour la démo
// Afin d'afficher une température sur ce bandeau.
// Attention le bandeau a une échelle inversée.
// ========================================================================================================================
// ****** N'hésitez pas à liker, commenter, partager et à vous abonner, cela contribue à promouvoir la chaine, merci ******
// ========================================================================================================================

  #include <Adafruit_NeoPixel.h>

  // Configuration des broches et du bandeau LED
  #define LED_PIN 0          // PB0 (D0) connecté au DIN du bandeau
  #define NUM_LEDS 10        // Nombre de LEDs dans le bandeau
  #define LM35_PIN A3        // Entrée analogique A3 (PB3) où le LM35DZ est connecté

  Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

  // Paramètre de correction de température (en °C)
  float correction = -1.0;  // Ajuster selon vos besoins

  void setup() 
  {
  // Initialisation du bandeau LED
  strip.begin();
  strip.show(); // Éteindre toutes les LEDs au démarrage
  }

  void loop() 
  {
  // Lecture de la température
  int analogValue = analogRead(LM35_PIN);
  float voltage = analogValue * (5.0 / 1023.0); // Conversion de 0-1023 en tension (0-5V)
  float temperature = voltage * 100.0;         // LM35 : 10mV/°C, donc tension * 100 = température en °C

  temperature += correction; // Appliquer la correction à la température

  // Mettre à jour les LEDs
  updateLEDs(temperature);

  delay(1000);
  }

  void updateLEDs(float temperature) 
  {
  // Extinction de toutes les LEDs
  strip.clear();

  // Conversion de la température en index de LED
  int ledIndex = map(temperature, 18, 27, NUM_LEDS - 1, 0); // Échelle inversée pour le bandeau
  ledIndex = constrain(ledIndex, 0, NUM_LEDS - 1);          // Limiter à la plage des LEDs

  // Mise à jour des LEDs
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < ledIndex) {
      // LEDs en dessous de la température actuelle (bleu, luminosité réduite)
      strip.setPixelColor(i, strip.Color(0, 0, 2)); // Bleu
    } else if (i == ledIndex) {
      // LED correspondant à la température actuelle (jaune, moyenne luminosité)
      strip.setPixelColor(i, strip.Color(20, 20, 0)); // Jaune
    } else {
      // LEDs au-dessus de la température actuelle (rouge, luminosité réduite)
      strip.setPixelColor(i, strip.Color(2, 0, 0)); // Rouge
    }
  }

  // Mise à jour du bandeau
  strip.show();
  }
