#include <Wire.h> 
#include "Adafruit_MCP23017.h"

#define ANGLE_PAR_FRONT 360/500

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

volatile int compteur = 0;
volatile bool interrupt = false;

void setup()
{
      
  Serial.begin(115200);
  Serial.println("Starting...");

  //initMCP();

  pinMode(PIN_RPI2,INPUT);
  pinMode(PIN_RPI4,INPUT);
  //attachInterrupt(digitalPinToInterrupt(PIN_RPI_INTx), handleInterrupt, polarity)
  attachInterrupt(digitalPinToInterrupt(PIN_RPI4),handleInterrupt,RISING);

  // Cleared int flag
  //mcp1.digitalRead(A0);
  
  for (int i=0; i<=sizeof(Transistors_Tab); i++){
    pinMode(Transistors_Tab[i], OUTPUT);
    digitalWrite(Transistors_Tab[i], HIGH);
  }

  pinMode(PIN_LED1, OUTPUT);
  digitalWrite(PIN_LED1, HIGH);
  pinMode(PIN_LED2, OUTPUT);
  digitalWrite(PIN_LED2, LOW);
  delay(1000);
  digitalWrite(PIN_LED2, HIGH);

  Serial.println("Ready!");
  
}

void loop()
{
  if(interrupt){
    Serial.println(compteur*ANGLE_PAR_FRONT);
    interrupt = false;
  }
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
  

  if(digitalRead(PIN_RPI2)){
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
  
  for (int i=0;i<sizeof(MCP1_IN_Tab);i++){
    mcp1.pinMode(MCP1_IN_Tab[i], INPUT);
    mcp1.pullUp(MCP1_IN_Tab[i], LOW);                   // Normal input
  }

  for (int i=0;i<sizeof(MCP1_IN_PULLUP_Tab);i++){
    mcp1.pinMode(MCP1_IN_PULLUP_Tab[i], INPUT);
    mcp1.pullUp(MCP1_IN_PULLUP_Tab[i], HIGH);           // turn on a 100K pullup internally
  }
  
  for (int i=0;i<sizeof(MCP1_OUT_Tab);i++){
    mcp1.pinMode(MCP1_OUT_Tab[i], OUTPUT);
  }

  if(sizeof(MCP1_INTERRUPT_Tab)){
    //mcpx.setupInterrupts(mirroring, openDrain, polarity) --> default is false/false/LOW
    mcp1.setupInterrupts(false, false, HIGH);
    for (int i=0;i<sizeof(MCP1_INTERRUPT_Tab);i++){
      //mcpx.setupInterruptPin(pin, polarity)
      mcp1.setupInterruptPin(MCP1_INTERRUPT_Tab[i],CHANGE);
    }
  }
  
  for (int i=0;i<sizeof(MCP2_IN_Tab);i++){
    mcp2.pinMode(MCP2_IN_Tab[i], INPUT);
    mcp2.pullUp(MCP2_IN_Tab[i], LOW);                   // Normal input
  }
  
  for (int i=0;i<sizeof(MCP2_IN_PULLUP_Tab);i++){
    mcp2.pinMode(MCP2_IN_PULLUP_Tab[i], INPUT);
    mcp2.pullUp(MCP2_IN_PULLUP_Tab[i], HIGH);           // turn on a 100K pullup internally
  }
  
  for (int i=0;i<sizeof(MCP2_OUT_Tab);i++){
    mcp2.pinMode(MCP2_OUT_Tab[i], OUTPUT);
  }
}
