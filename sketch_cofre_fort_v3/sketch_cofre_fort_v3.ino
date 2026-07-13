#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// CONFIG WIFI
const char* ssid = "Coffre_V3";     // Nom du WiFi créé par l'ESP32
const char* password = "12345678";  // MDP du WiFi, 8 caractères mini

// PINS
#define SERVO_PIN 18
#define LED_VERTE 19  
#define LED_ROUGE 21
#define BUZZER 22

// VARIABLES
Servo monServo;
WebServer server(80);
String codePIN = "1234";        // Code par défaut
int mauvaisEssais = 0;
bool bloque = false;
unsigned long tempsBlocage = 0;
const int TEMPS_BLOCAGE = 60000; // 1 minute

void setup() {
  Serial.begin(115200);
  pinMode(LED_VERTE, OUTPUT);
  pinMode(LED_ROUGE, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  monServo.attach(SERVO_PIN);
  fermerCoffre();
  
  // Créer le point d'accès WiFi
  WiFi.softAP(ssid, password);
  Serial.print("IP du coffre: "); Serial.println(WiFi.softAPIP());
  
  // Routes page web
  server.on("/", handleRoot);
  server.on("/ouvrir", handleOuvrir);
  server.on("/changer", handleChanger);
  server.begin();
}

void loop() {
  server.handleClient();
  
  // Débloquer après 1min
  if (bloque && millis() - tempsBlocage > TEMPS_BLOCAGE) {
    bloque = false;
    mauvaisEssais = 0;
    digitalWrite(LED_ROUGE, LOW);
    Serial.println("Débloqué");
  }
}

void handleRoot() {
  String html = R"rawliteral(
  <!DOCTYPE html><html><head>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <title>Coffre V3</title>
  <style>
    body {font-family: Arial; text-align: center; background:#1a1a1a; color:white;}
    input {padding:10px; font-size:20px; margin:5px;}
    button {padding:15px 30px; font-size:18px; background:#007bff; color:white; border:none;}
    .rouge {background:#dc3545;}
  </style></head><body>
  <h1> Coffre V3</h1>
  <form action='/ouvrir' method='POST'>
    <h3>Entrer le code PIN</h3>
    <input type='password' name='pin' maxlength='4' required><br>
    <button type='submit'>OUVRIR</button>
  </form><br><hr><br>
  <form action='/changer' method='POST'>
    <h3>Changer le code PIN</h3>
    Ancien: <input type='password' name='ancien' maxlength='4' required><br>
    Nouveau: <input type='password' name='nouveau' maxlength='4' required><br>
    <button type='submit' class='rouge'>CHANGER</button>
  </form>
  </body></html>
  )rawliteral";
  server.send(200, "text/html", html);
}

void handleOuvrir() {
  if (bloque) {
    server.send(200, "text/plain", "BLOQUE 1 MIN - Attendez");
    return;
  }
  
  String pinRecu = server.arg("pin");
  
  if (pinRecu == codePIN) {
    mauvaisEssais = 0;
    ouvrirCoffre();
    server.send(200, "text/plain", "COFFRE OUVERT");
    delay(5000);
    fermerCoffre();
  } else {
    mauvaisEssais++;
    digitalWrite(LED_ROUGE, HIGH);
    tone(BUZZER, 2000, 200);
    delay(200);
    digitalWrite(LED_ROUGE, LOW);
    
    if (mauvaisEssais >= 3) {
      bloque = true;
      tempsBlocage = millis();
      server.send(200, "text/plain", "3 ECHECS - BLOQUE 1 MIN");
    } else {
      server.send(200, "text/plain", "CODE FAUX - Essai " + String(mauvaisEssais) + "/3");
    }
  }
}

void handleChanger() {
  String ancien = server.arg("ancien");
  String nouveau = server.arg("nouveau");
  
  if (ancien == codePIN && nouveau.length() == 4) {
    codePIN = nouveau;
    server.send(200, "text/plain", "CODE CHANGE EN: " + nouveau);
  } else {
    server.send(200, "text/plain", "ERREUR: Ancien code faux ou nouveau != 4 chiffres");
  }
}

void ouvrirCoffre() {
  digitalWrite(LED_VERTE, HIGH);
  monServo.write(90); // Ouvre
  tone(BUZZER, 1000, 100);
  Serial.println("Ouvert");
}

void fermerCoffre() {
  digitalWrite(LED_VERTE, LOW);
  monServo.write(0); // Ferme
}