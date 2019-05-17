
// Enable debug prints
#define MY_DEBUG

#define MY_RADIO_NRF5_ESB

#define MY_RF24_PA_LEVEL RF24_PA_HIGH

#define MY_NODE_ID 15

#include <MySensors.h>
#include <BH1750.h>
#include <Wire.h> 
#include "Adafruit_MCP23017.h"

int RELAY_PIN[] = {PIN_T1,PIN_T2,PIN_T3,PIN_T4,PIN_T5,PIN_T6,PIN_T7,PIN_T8};
#define NUMBER_OF_RELAYS 8 // Total number of attached relays
#define RELAY_ON 0  // GPIO value to write to turn on attached relay
#define RELAY_OFF 1 // GPIO value to write to turn off attached relay
#define CHILD_ID_LIGHT 20

BH1750 lightSensor;
Adafruit_MCP23017 mcp1;

#define mcpPinA1 6
#define mcpPinA2 5
#define mcpPinB 9

#define SavedPos 100

int i;

volatile boolean awakenByInterrupt = false;

MyMessage msg(CHILD_ID_LIGHT, V_LIGHT_LEVEL);
uint16_t lastlux;

// Initialize general message
void before()
{
  NRF_RADIO->TXPOWER = RADIO_TXPOWER_TXPOWER_Pos3dBm;
  for (int sensor=1, pin=0; sensor<=NUMBER_OF_RELAYS; sensor++, pin++) {
    // Then set relay pins in output mode
    hwPinMode(RELAY_PIN[pin], OUTPUT_H0H1);
    // Set relay to last known state (using eeprom storage)
    digitalWrite(RELAY_PIN[pin], loadState(sensor)?RELAY_ON:RELAY_OFF);
  }

}

void setup()
{
  //Wire.begin();
  lightSensor.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
  
  mcp1.begin();      // use default address 0
  mcp1.setupInterrupts(true,false,LOW);
  mcp1.pinMode(mcpPinA1, INPUT);
  mcp1.pullUp(mcpPinA1, HIGH);  // turn on a 100K pullup internally
  mcp1.setupInterruptPin(mcpPinA1,FALLING);
  
  //mcp1.pinMode(mcpPinA2, INPUT);
  //mcp1.pullUp(mcpPinA2, HIGH);  // turn on a 100K pullup internally
  //mcp1.setupInterruptPin(mcpPinA2,FALLING);

  mcp1.pinMode(mcpPinB, INPUT);
  mcp1.pullUp(mcpPinB, HIGH);  // turn on a 100K pullup internally
  mcp1.setupInterruptPin(mcpPinB,FALLING);

  while( ! (mcp1.digitalRead(mcpPinB) && mcp1.digitalRead(mcpPinA1) ));

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_RPI_INT1,INPUT);

  for(int i=0;i<loadState(SavedPos);i++){  
    delay(400);
    digitalWrite(LED_BUILTIN,0);
    delay(400);
    digitalWrite(LED_BUILTIN,1);
  }
  
}

/*
void intCallBack(){
  awakenByInterrupt = true;
}*/


void presentation()
{
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Utilipy", "0.4");

  for (int sensor=1, pin=0; sensor<=NUMBER_OF_RELAYS; sensor++, pin++) {
    // Register all sensors to gw (they will be created as child devices)
    present(sensor, S_BINARY);
  }
  present(CHILD_ID_LIGHT, S_LIGHT_LEVEL);
}

void handleInterrupt(){  
  // Get more information from the MCP from the INT
  uint8_t pin=mcp1.getLastInterruptPin();
  uint8_t val=mcp1.getLastInterruptPinValue();
  
  uint8_t flashes=4; 
  if(pin==mcpPinA1) flashes=1;
  if(pin==mcpPinB) flashes=2;
  if(val!=LOW) flashes=3;

  saveState(SavedPos, flashes);
  
  awakenByInterrupt=false;
}

void loop()
{
  //attachInterrupt(digitalPinToInterrupt(PIN_RPI_INT1),intCallBack,CHANGE);
  
  //Enter deep Sleep
  int sleeptrig = smartSleep(digitalPinToInterrupt(PIN_RPI_INT1),CHANGE,10000);

  //Gestion de l'interuption
  if (sleeptrig == 7){
    handleInterrupt();
    mcp1.digitalRead(mcpPinB);
    mcp1.digitalRead(mcpPinA1);
  }

  //Envoi des données capteur I2C LIGHT
  uint16_t lux = lightSensor.readLightLevel();// Get Lux value
  if ((lux != lastlux) & (lux != 0)) {
      Serial.println(lux);
      send(msg.set(lux));
      lastlux = lux;
  }

  
}

void receive(const MyMessage &message)
{
  // We only expect one type of message from controller. But we better check anyway.
  if (message.type==V_STATUS) {
    // Change relay state
    digitalWrite(RELAY_PIN[message.sensor-1], message.getBool()?RELAY_ON:RELAY_OFF);
    // Store state in eeprom
    saveState(message.sensor, message.getBool());
    // Write some debug info
    Serial.print("Incoming change for sensor:");
    Serial.print(message.sensor);
    Serial.print(", New status: ");
    Serial.println(message.getBool());
  }
}
