//#define MY_DEBUG
#define MY_RADIO_NRF5_ESB
#define MY_RF24_PA_LEVEL RF24_PA_HIGH
#define MY_NODE_ID 14
#include <MySensors.h>

#include <Wire.h> 
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;

int MCP1_IN_Tab[]  = {};
int MCP1_IN_PULLUP_Tab[]  = {};
int MCP1_OUT_Tab[] = {};
int MCP1_INTERRUPT_Tab[]  = {};

int MCP2_IN_Tab[]  = {};
int MCP2_IN_PULLUP_Tab[]  = {};
int MCP2_OUT_Tab[] = {};

int Transistors_Tab[] = {PIN_T1,PIN_T2,PIN_T3,PIN_T4,PIN_T5,PIN_T6,PIN_T7,PIN_T8};
bool TransistorsState_Tab[]={0,0,0,0,0,0,0,0};
#define NUMBER_OF_TRANSISTORS 8   // Total number of attached relays
#define TRANSISTOR_ON         0   // GPIO value to write to turn on 
#define TRANSISTOR_OFF        1   // GPIO value to write to turn off 
#define SavedPos              100


void setup(){

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //enable_reset();
  //disable_nfc();

  //Serial.begin(115200);     // not needed if mysensor.h is called
  Serial.println("Starting...");

  initMCP();
  
  /*for (int i=0; i<=sizeof(Transistors_Tab); i++){
    pinMode(Transistors_Tab[i], OUTPUT);
    digitalWrite(Transistors_Tab[i], HIGH);
  }*/

  pinMode(PIN_RPI1, OUTPUT);
  pinMode(PIN_RPI2, OUTPUT);
  pinMode(PIN_RPI3, OUTPUT);
  pinMode(PIN_RPI4, OUTPUT);

  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("Ready!");

}

void loop(){

  for (int i=0; i<=sizeof(Transistors_Tab); i++){
    digitalWrite(Transistors_Tab[i], TRANSISTOR_ON);
  }
  digitalWrite(PIN_RPI1, HIGH);
  digitalWrite(PIN_RPI2, HIGH);
  digitalWrite(PIN_RPI3, HIGH);
  digitalWrite(PIN_RPI4, HIGH);
  delay(2000);

  for (int i=0; i<=sizeof(Transistors_Tab); i++){
    digitalWrite(Transistors_Tab[i], TRANSISTOR_OFF);
  }
  digitalWrite(PIN_RPI1, LOW);
  digitalWrite(PIN_RPI2, LOW);
  digitalWrite(PIN_RPI3, LOW);
  digitalWrite(PIN_RPI4, LOW);
  delay(2000);
    
}

// Initialize general message
void before(){
  NRF_RADIO->TXPOWER = RADIO_TXPOWER_TXPOWER_Pos3dBm;
  for (int sensor=1, pin=0; sensor<=NUMBER_OF_TRANSISTORS; sensor++, pin++) {
    // Then set relay pins in output mode
    hwPinMode(Transistors_Tab[pin], OUTPUT_H0H1);
    // Set relay to last known state (using eeprom storage)
    digitalWrite(Transistors_Tab[pin], loadState(sensor)?TRANSISTOR_ON:TRANSISTOR_OFF);
  }
}

void presentation(){
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Utilipi", "1.0");
  
  for (int sensor=1, pin=0; sensor<=NUMBER_OF_TRANSISTORS; sensor++, pin++) {
    // Register all sensors to gw (they will be created as child devices)
    present(sensor, S_BINARY);
  }
}

void receive(const MyMessage &message){
  if (message.type==V_STATUS) {
    // Change relay state
    digitalWrite(Transistors_Tab[message.sensor-1], message.getBool()?TRANSISTOR_ON:TRANSISTOR_OFF);
    // Store state in eeprom
    saveState(message.sensor, message.getBool());
    // Write some debug info
    Serial.print("Incoming change for sensor:");
    Serial.print(message.sensor);
    Serial.print(", New status: ");
    Serial.println(message.getBool());
  }
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
