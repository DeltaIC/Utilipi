#include <Wire.h> 
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;

int MCP1_IN_Tab[]  = {};
int MCP1_IN_PULLUP_Tab[]  = {};
int MCP1_OUT_Tab[] = {A0,A1,A2,A3,A4,A5,A6,A7,B0,B1,B2,B3,B4,B5,B6,B7};
int MCP1_INTERRUPT_Tab[]  = {};

int MCP2_IN_Tab[]  = {};
int MCP2_IN_PULLUP_Tab[]  = {};
int MCP2_OUT_Tab[] = {A0,A1,A2,A3,A4,A5,A6,A7,B0,B1,B2,B3,B4,B5,B6,B7};

void setup(){
  
  initMCP();

  for (int i=0;i<sizeof(MCP1_OUT_Tab)/sizeof(int);i++){
    mcp1.digitalWrite(MCP1_OUT_Tab[i], HIGH);
  }

  for (int i=0;i<sizeof(MCP2_OUT_Tab)/sizeof(int);i++){
    mcp2.digitalWrite(MCP2_OUT_Tab[i], HIGH);
  }

}

void loop(){
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
