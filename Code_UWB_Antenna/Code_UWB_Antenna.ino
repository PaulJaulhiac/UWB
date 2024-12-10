#include "require_cpp11.h"
#include <SPI.h>
#include <DW1000.h>

// Configuration des broches pour le DWM1001
constexpr uint8_t PIN_RST = 33; // Broche de reset
constexpr uint8_t PIN_IRQ = 19; // Broche IRQ
constexpr uint8_t PIN_SS = 24; // Broche SPI_CS (Chip Select)

// Définition de l'état de transmission
enum class TransmissionState : uint8_t {
  SENDER,
  RECEIVER
};
TransmissionState trxToggle = TransmissionState::RECEIVER;

// Variables globales
volatile boolean trxAck = false; // Confirmation de transmission
volatile boolean rxError = false; // Indicateur d'erreur de réception
String msg; // Message à envoyer

// === Initialisation ===
void setup() {
  // Initialisation du moniteur série
  Serial.begin(9600);
  Serial.println(F("### DWM1001 - Envoi & Réception Ping-Pong ###"));

  // Initialisation du module DW1000
  DW1000.begin(PIN_IRQ, PIN_RST);
  DW1000.select(PIN_SS);
  Serial.println(F("DW1000 initialisé."));

  // Configuration générale
  DW1000.newConfiguration();
  DW1000.setDefaults();
  DW1000.setDeviceAddress(2); // Adresse de l'appareil
  DW1000.setNetworkId(10); // ID réseau
  DW1000.commitConfiguration();
  Serial.println(F("Configuration validée."));

  // Affichage des informations du module
  printModuleInfo();
  
  // Attacher les gestionnaires d'événements
  DW1000.attachSentHandler(handleSent);
  DW1000.attachReceivedHandler(handleReceived);
  DW1000.attachReceiveFailedHandler(handleReceiveFailed);
  
  // Initialisation selon l'état
  if (trxToggle == TransmissionState::SENDER) {
    msg = "Ping ...";
    receiver(); // Préparez le mode récepteur pour l'écoute
    sendMessage(msg); // Envoyer un premier message
  } else {
    msg = "... and Pong";
    receiver(); // Préparez le module en tant que récepteur
  }
}

// === Boucle principale ===
void loop() {
  if (rxError) {
    Serial.println(F("Erreur de réception détectée !"));
    rxError = false;
    return;
  }
  
  if (!trxAck) {
    return; // Attendre la confirmation de transmission ou de réception
  }
  
  trxAck = false; // Réinitialiser l'indicateur de confirmation

  // Basculer entre émetteur et récepteur
  trxToggle = (trxToggle == TransmissionState::SENDER) ? TransmissionState::RECEIVER : TransmissionState::SENDER;
  
  if (trxToggle == TransmissionState::SENDER) {
    // L'appareil est maintenant un émetteur
    sendMessage("Ping depuis DWM1001 !");
  } else {
    // L'appareil est maintenant un récepteur
    Serial.println(F("En attente de messages..."));
    receiver();
  }
}

// === Fonction pour envoyer un message ===
void sendMessage(const String& message) {
  DW1000.newTransmit();
  DW1000.setDefaults();
  DW1000.setData(message.c_str()); // Convertir le message en chaîne de caractères C
  DW1000.startTransmit(); // Lancer la transmission
  Serial.print(F("Message envoyé : "));
  Serial.println(message);
}

// === Fonction pour configurer le récepteur ===
void receiver() {
  DW1000.newReceive();
  DW1000.setDefaults();
  DW1000.receivePermanently(true); // Activer la réception continue
  DW1000.startReceive(); // Lancer la réception
}

// === Gestionnaire d'événement : Transmission réussie ===
void handleSent() {
  trxAck = true; // Marquer la transmission comme réussie
  Serial.println(F("Transmission réussie."));
}

// === Gestionnaire d'événement : Réception réussie ===
void handleReceived() {
  trxAck = true; // Marquer la réception comme réussie
  String receivedMessage;
  DW1000.getData(receivedMessage); // Récupérer les données reçues
  Serial.print(F("Message reçu : "));
  Serial.println(receivedMessage);
}

// === Gestionnaire d'événement : Erreur de réception ===
void handleReceiveFailed() {
  rxError = true; // Marquer une erreur de réception
  Serial.println(F("Erreur lors de la réception du message."));
}

// === Fonction pour afficher les informations du module ===
void printModuleInfo() {
  char msgInfo[128];
  DW1000.getPrintableDeviceIdentifier(msgInfo);
  Serial.print(F("ID de l'appareil : "));
  Serial.println(msgInfo);

  DW1000.getPrintableExtendedUniqueIdentifier(msgInfo);
  Serial.print(F("ID unique : "));
  Serial.println(msgInfo);

  DW1000.getPrintableNetworkIdAndShortAddress(msgInfo);
  Serial.print(F("ID réseau & adresse courte : "));
  Serial.println(msgInfo);

  DW1000.getPrintableDeviceMode(msgInfo);
  Serial.print(F("Mode de l'appareil : "));
  Serial.println(msgInfo);
}