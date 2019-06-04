//#define MY_DEBUG

#define MY_RADIO_NRF5_ESB

#define MY_RF24_PA_LEVEL RF24_PA_HIGH

#define MY_NODE_ID 15

#include <MySensors.h>
#include <Wire.h> 
#include "Adafruit_MCP23017.h"

#define ANGLE_PAR_FRONT 360/500

//Constantes
#define SECU_OK           A0            // Sortie OK sécurité outil presse
#define SHUNT_SECU        A1            // Shunt défaut pour réglage
#define CAPTEUR_BANDE     A2            // Capteur présence bande
#define LED_GREEN         A3            // à définir
#define LED_YELLOW        A4            // à définir
#define LED_RED           A5            // à définir
#define EMBRAYAGE         A6            // Embrayage (la presse fonctionne si le moteur tourne et embrayé)
#define ENCODEURA         PIN_RPI4      // Signal A du codeur
#define ENCODEURB         PIN_RPI2      // Signal B du codeur

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;

int MCP1_IN_Tab[]  = {SHUNT_SECU};
int MCP1_IN_PULLUP_Tab[]  = {SECU_OK,CAPTEUR_BANDE};
int MCP1_OUT_Tab[] = {LED_GREEN, LED_YELLOW, LED_RED, EMBRAYAGE};
int MCP1_INTERRUPT_Tab[]  = {};

int MCP2_IN_Tab[]  = {};
int MCP2_IN_PULLUP_Tab[]  = {};
int MCP2_OUT_Tab[] = {};

int Transistors_Tab[] = {PIN_T1,PIN_T2,PIN_T3,PIN_T4,PIN_T5,PIN_T6,PIN_T7,PIN_T8};
bool TransistorsState_Tab[]={0,0,0,0,0,0,0,0};

volatile int compteur = 0;
volatile bool interrupt = false;

void setup(){
  //Serial.begin(115200);     // non needed if mysensor.h is called
  Serial.println("Starting...");

  initMCP();
  mcp1.digitalWrite(EMBRAYAGE,LOW);

  pinMode(ENCODEURA,INPUT);
  pinMode(ENCODEURB,INPUT);
  //attachInterrupt(digitalPinToInterrupt(PIN_RPI_INTx), handleInterrupt, polarity)
  attachInterrupt(digitalPinToInterrupt(ENCODEURA),handleInterrupt,RISING);
  
  for (int i=0; i<=sizeof(Transistors_Tab); i++){
    pinMode(Transistors_Tab[i], OUTPUT);
    digitalWrite(Transistors_Tab[i], HIGH);
  }

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("Ready!");

}

// Initialize general message
void before(){
  NRF_RADIO->TXPOWER = RADIO_TXPOWER_TXPOWER_Pos3dBm;
}

void presentation(){
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Presse", "0.1");
  present(PIN_LED1,S_BINARY);
  present(PIN_LED2,S_BINARY);
}

void loop(){
  /*if(interrupt){
    Serial.println(compteur*ANGLE_PAR_FRONT);
    interrupt = false;
  }*/
  /*mcp1.digitalWrite(EMBRAYAGE,LOW);
  delay(3000);
  mcp1.digitalWrite(EMBRAYAGE,HIGH);
  delay(3000);*/
  
  if(!mcp1.digitalRead(SECU_OK)){
    mcp1.digitalWrite(LED_GREEN,HIGH);
  }else{
    mcp1.digitalWrite(LED_GREEN,LOW);
  }
  
  if(!mcp1.digitalRead(SHUNT_SECU)){
    mcp1.digitalWrite(LED_YELLOW,HIGH);
  }else{
    mcp1.digitalWrite(LED_YELLOW,LOW);
  }
  
  if(!mcp1.digitalRead(CAPTEUR_BANDE)){
    mcp1.digitalWrite(LED_RED,HIGH);
  }else{
    mcp1.digitalWrite(LED_RED,LOW);
  }
  delay(100);
}

void receive(const MyMessage &message)
{
  /* We only expect one type of message from controller. But we better check anyway.
  if (message.type==V_STATUS) {
      // Change relay state
      if (message.getBool()) ledState=0;
      else ledState=1;
      digitalWrite(ledPin, ledState);
     
      
      // Store state in eeprom
      saveState(message.sensor, message.getBool());
      // Write some debug info
      /*Serial.print("Incoming change for sensor:");
      Serial.print(message.sensor);
      Serial.print(", New status: ");
      Serial.println(message.getBool());
  }*/
}

void handleInterrupt(){
  /*
   * To achieve max speed for interrupt, you have to optimize read operation on I2C bus.
   * Since we know which pin are configured as interrupt and which port triggered the callback,
   * read ONLY the pin(s) on the port who actually triggered the interrupt
   * 
   * I do not advice to use the getLastInterruptPin() and getLastInterruptPinValue() function 
   * from the adafruit library for high speed interrupt (>1KHz)
   */
  
  if(digitalRead(ENCODEURB)){
    compteur++;
  }else{
    compteur--;
  }
  interrupt = true;
}

void initMCP(){
  mcp1.begin();       // use default address 0
  mcp2.begin(4);      // use default address 4
  Wire.setClock(400000);  // change from 100KHz to 400KHz (max speed)
  pinMode(PIN_WIRE_SDA, INPUT);   //disable internal 13k pullup, better have 5k pullup external
  pinMode(PIN_WIRE_SCL, INPUT);   //idem as SDA
  
  for (int i=0;i<sizeof(MCP1_IN_Tab)/sizeof(int);i++){
    mcp1.pinMode(MCP1_IN_Tab[i], INPUT);
    mcp1.pullUp(MCP1_IN_Tab[i], LOW);                   // Normal input
  }

  for (int i=0;i<sizeof(MCP1_IN_PULLUP_Tab)/sizeof(int);i++){
    mcp1.pinMode(MCP1_IN_PULLUP_Tab[i], INPUT);
    mcp1.pullUp(MCP1_IN_PULLUP_Tab[i], HIGH);           // turn on a 100K pullup internally
  }
  
  for (int i=0;i<sizeof(MCP1_OUT_Tab)/sizeof(int);i++){
    mcp1.pinMode(MCP1_OUT_Tab[i], OUTPUT);
  }

  if(sizeof(MCP1_INTERRUPT_Tab)){
    //mcpx.setupInterrupts(mirroring, openDrain, polarity) --> default is false/false/LOW
    mcp1.setupInterrupts(false, false, HIGH);
    for (int i=0;i<sizeof(MCP1_INTERRUPT_Tab)/sizeof(int);i++){
      //mcpx.setupInterruptPin(pin, polarity)
      mcp1.setupInterruptPin(MCP1_INTERRUPT_Tab[i],CHANGE);
    }
  }
  
  for (int i=0;i<sizeof(MCP2_IN_Tab)/sizeof(int);i++){
    mcp2.pinMode(MCP2_IN_Tab[i], INPUT);
    mcp2.pullUp(MCP2_IN_Tab[i], LOW);                   // Normal input
  }
  
  for (int i=0;i<sizeof(MCP2_IN_PULLUP_Tab)/sizeof(int);i++){
    mcp2.pinMode(MCP2_IN_PULLUP_Tab[i], INPUT);
    mcp2.pullUp(MCP2_IN_PULLUP_Tab[i], HIGH);           // turn on a 100K pullup internally
  }
  
  for (int i=0;i<sizeof(MCP2_OUT_Tab)/sizeof(int);i++){
    mcp2.pinMode(MCP2_OUT_Tab[i], OUTPUT);
  }
}
