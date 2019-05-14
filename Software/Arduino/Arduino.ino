
// Enable debug prints
#define MY_DEBUG

#define MY_RADIO_NRF5_ESB

// Enable passive mode
#define MY_PASSIVE_NODE

// Passive mode requires static node ID
#define MY_NODE_ID 100

#include <MySensors.h>

#define CHILD_ID 0   // Id of the sensor child

// Initialize general message
MyMessage msg(CHILD_ID, V_TEMP);

int i;

// Initialize general message


void setup()
{
  /*for(i=2;i++;i<31){
    if (i!=21) {pinMode(i, OUTPUT);}
    
  }*/
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  
  pinMode(PIN_T1, OUTPUT);
  pinMode(PIN_T2, OUTPUT);
  pinMode(PIN_T3, OUTPUT);
  pinMode(PIN_T4, OUTPUT);
  pinMode(PIN_T5, OUTPUT);
  pinMode(PIN_T6, OUTPUT);
  pinMode(PIN_T7, OUTPUT);
  pinMode(PIN_T8, OUTPUT);

  digitalWrite(PIN_T1, LOW);
  digitalWrite(PIN_T2, LOW);
  digitalWrite(PIN_T3, HIGH);
  digitalWrite(PIN_T4, HIGH);
  digitalWrite(PIN_T5, HIGH);
  digitalWrite(PIN_T6, HIGH);
  digitalWrite(PIN_T7, HIGH);
  digitalWrite(PIN_T8, HIGH);
  digitalWrite(PIN_LED1, HIGH);
  digitalWrite(PIN_LED2, HIGH);
  /*digitalWrite(PIN_T2, LOW);
  digitalWrite(PIN_T3, LOW);
  digitalWrite(PIN_T4, LOW);
  digitalWrite(PIN_T5, LOW);
  digitalWrite(PIN_T6, LOW);
  digitalWrite(PIN_T7, LOW);
  digitalWrite(PIN_T8, LOW);
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);*/

  delay(3000);

  //digitalWrite(PIN_T1, HIGH);

/*
  pinMode(PIN_SERIAL_RX, OUTPUT);
  pinMode(PIN_SERIAL_TX, OUTPUT);

  pinMode(MISO, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(CE1,OUTPUT);
  pinMode(CE2,OUTPUT);
  pinMode(CE3,OUTPUT);
  pinMode(CE4,OUTPUT);
  pinMode(CE_ADC,OUTPUT);

  pinMode(SDA,OUTPUT);
  pinMode(SCL,OUTPUT);

  pinMode(PIN_RPI1,OUTPUT);
  pinMode(PIN_RPI2,OUTPUT);
  pinMode(PIN_RPI3,OUTPUT);
  pinMode(PIN_RPI4,OUTPUT);
  pinMode(PIN_RPI_INT1, OUTPUT);
  pinMode(PIN_RPI_INT2,OUTPUT);*/
}

void presentation()
{
  // Send the sketch version information to the gateway and controller
  sendSketchInfo("Passive node", "1.0");

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID, S_TEMP);
}

void loop()
{
  
  // generate some random data
  send(msg.set(25.0+random(0,30)/10.0,2));
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);
  digitalWrite(PIN_T1, HIGH);
  
  
  /*digitalWrite(PIN_SERIAL_RX, LOW);
  digitalWrite(PIN_SERIAL_TX, LOW);
  
  digitalWrite(MOSI, LOW);
  digitalWrite(MISO, LOW);
  digitalWrite(SCK, LOW);
  digitalWrite(CE1, LOW);
  digitalWrite(CE2, LOW);
  digitalWrite(CE3, LOW);
  digitalWrite(CE4, LOW);
  digitalWrite(CE_ADC, LOW);
  
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, LOW);

  digitalWrite(PIN_RPI1, LOW);
  digitalWrite(PIN_RPI2, LOW);
  digitalWrite(PIN_RPI3, LOW);
  digitalWrite(PIN_RPI4, LOW);
  digitalWrite(PIN_RPI_INT1, LOW);
  digitalWrite(PIN_RPI_INT2, LOW);*/
  /*for(i=2;i++;i<31){
    if (i!=21) {digitalWrite(i, LOW);}
    
  }*/
  delay(1500);
  /*for(i=2;i++;i<31){
    if (i!=21) {digitalWrite(i, HIGH);}
    
  }*/
  digitalWrite(PIN_LED1, HIGH); 
  digitalWrite(PIN_LED2, HIGH);
  digitalWrite(PIN_T1, LOW);
  
  /*digitalWrite(PIN_T1, HIGH);
  digitalWrite(PIN_T2, HIGH);
  digitalWrite(PIN_T3, HIGH);
  digitalWrite(PIN_T4, HIGH);
  digitalWrite(PIN_T5, HIGH);
  digitalWrite(PIN_T6, HIGH);
  digitalWrite(PIN_T7, HIGH);
  digitalWrite(PIN_T8, HIGH);
  
  digitalWrite(PIN_SERIAL_RX, HIGH);
  digitalWrite(PIN_SERIAL_TX, HIGH);
  
  digitalWrite(MOSI, HIGH);
  digitalWrite(MISO, HIGH);
  digitalWrite(SCK, HIGH);
  digitalWrite(CE1, HIGH);
  digitalWrite(CE2, HIGH);
  digitalWrite(CE3, HIGH);
  digitalWrite(CE4, HIGH);
  digitalWrite(CE_ADC, HIGH);
  
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, HIGH);

  digitalWrite(PIN_RPI1, HIGH);
  digitalWrite(PIN_RPI2, HIGH);
  digitalWrite(PIN_RPI3, HIGH);
  digitalWrite(PIN_RPI4, HIGH);
  digitalWrite(PIN_RPI_INT1, HIGH);
  digitalWrite(PIN_RPI_INT2, HIGH);*/
  delay(3000);
}
