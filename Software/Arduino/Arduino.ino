
// Enable debug prints
#define MY_DEBUG

#define MY_RADIO_NRF5_ESB

#define MY_RF24_PA_LEVEL RF24_PA_MAX

#define MY_NODE_ID 11

#include <MySensors.h>
#include <BH1750.h>
#include <Wire.h> 

int RELAY_PIN[] = {PIN_T1,PIN_T2,PIN_T3,PIN_T4,PIN_T5,PIN_T6,PIN_T7,PIN_T8};
#define NUMBER_OF_RELAYS 8 // Total number of attached relays
#define RELAY_ON 0  // GPIO value to write to turn on attached relay
#define RELAY_OFF 1 // GPIO value to write to turn off attached relay
#define CHILD_ID_LIGHT 20
BH1750 lightSensor;
int i;

MyMessage msg(CHILD_ID_LIGHT, V_LIGHT_LEVEL);
// MyMessage msg(CHILD_ID_LIGHT, V_LEVEL);  
uint16_t lastlux;

// Initialize general message
void before()
{
  for (int sensor=1, pin=0; sensor<=NUMBER_OF_RELAYS; sensor++, pin++) {
    // Then set relay pins in output mode
    hwPinMode(RELAY_PIN[pin], OUTPUT_H0H1);
    //pinMode(RELAY_PIN[pin], OUTPUT);
    // Set relay to last known state (using eeprom storage)
    digitalWrite(RELAY_PIN[pin], loadState(sensor)?RELAY_ON:RELAY_OFF);
  }
  pinMode(LED_BUILTIN, OUTPUT);
}

void setup()
{
  Wire.begin();
  lightSensor.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
  
  digitalWrite(LED_BUILTIN,LOW);
}

void presentation()
{
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Utilipy", "0.3");

  for (int sensor=1, pin=0; sensor<=NUMBER_OF_RELAYS; sensor++, pin++) {
    // Register all sensors to gw (they will be created as child devices)
    present(sensor, S_BINARY);
  }
  present(CHILD_ID_LIGHT, S_LIGHT_LEVEL);
}

void loop()
{
  uint16_t lux = lightSensor.readLightLevel();// Get Lux value
  if (lux != lastlux) {
      Serial.println(lux);
      send(msg.set(lux));
      lastlux = lux;
  }

  delay(2000);
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
