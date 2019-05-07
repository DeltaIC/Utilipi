
// Enable debug prints
#define MY_DEBUG

#define MY_RADIO_NRF5_ESB

// Enable passive mode
#define MY_PASSIVE_NODE

#define MY_NODE_ID 100

#include <MySensors.h>

#define CHILD_ID 0   // Id of the sensor child

int i;

// Initialize general message
MyMessage msg(CHILD_ID, V_TEMP);

void setup()
{
/* MWU Enable -- This is workaround suggested by Aryan on the Nordic Developer Zone */
NRF_MWU->REGIONENSET
    = ((MWU_REGIONENSET_RGN0WA_Set << MWU_REGIONENSET_RGN0WA_Pos) 
     | (MWU_REGIONENSET_PRGN0WA_Set << MWU_REGIONENSET_PRGN0WA_Pos));

/* Turn on flash write enable and wait until the NVMC is ready: */
NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos);
while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}

/* write to the register */
NRF_UICR->NFCPINS = 0xFFFFFFFE;
while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}
    
/* Turn off flash write enable and wait until the NVMC is ready: */
NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos);
while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}
    
/* MWU Disable -- This is workaround suggested by Aryan on the Nordic Developer Zone */
NRF_MWU->REGIONENCLR
    = ((MWU_REGIONENCLR_RGN0WA_Clear << MWU_REGIONENCLR_RGN0WA_Pos) 
     | (MWU_REGIONENCLR_PRGN0WA_Clear << MWU_REGIONENCLR_PRGN0WA_Pos));
  /*for(i=2;i++;i<31){
    if (i!=21) {pinMode(i, OUTPUT);}
    
  }*/
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  pinMode(T1, OUTPUT);
  pinMode(T2, OUTPUT);
  pinMode(T3, OUTPUT);
  pinMode(T4, OUTPUT);
  pinMode(T5, OUTPUT);
  pinMode(T6, OUTPUT);
  pinMode(T7, OUTPUT);
  pinMode(T8, OUTPUT);

  pinMode(SERIAL_RX, OUTPUT);
  pinMode(SERIAL_TX, OUTPUT);

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

  pinMode(RPI1,OUTPUT);
  pinMode(RPI2,OUTPUT);
  pinMode(RPI3,OUTPUT);
  pinMode(RPI4,OUTPUT);
  pinMode(RPI_INT1, OUTPUT);
  pinMode(RPI_INT2,OUTPUT);
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
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  
  digitalWrite(T1, LOW);
  digitalWrite(T2, LOW);
  digitalWrite(T3, LOW);
  digitalWrite(T4, LOW);
  digitalWrite(T5, LOW);
  digitalWrite(T6, LOW);
  digitalWrite(T7, LOW);
  digitalWrite(T8, LOW);
  
  digitalWrite(SERIAL_RX, LOW);
  digitalWrite(SERIAL_TX, LOW);
  
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

  digitalWrite(RPI1, LOW);
  digitalWrite(RPI2, LOW);
  digitalWrite(RPI3, LOW);
  digitalWrite(RPI4, LOW);
  digitalWrite(RPI_INT1, LOW);
  digitalWrite(RPI_INT2, LOW);
  /*for(i=2;i++;i<31){
    if (i!=21) {digitalWrite(i, LOW);}
    
  }*/
  delay(1500);
  /*for(i=2;i++;i<31){
    if (i!=21) {digitalWrite(i, HIGH);}
    
  }*/
  digitalWrite(LED1, HIGH); 
  digitalWrite(LED2, HIGH);
  
  digitalWrite(T1, HIGH);
  digitalWrite(T2, HIGH);
  digitalWrite(T3, HIGH);
  digitalWrite(T4, HIGH);
  digitalWrite(T5, HIGH);
  digitalWrite(T6, HIGH);
  digitalWrite(T7, HIGH);
  digitalWrite(T8, HIGH);
  
  digitalWrite(SERIAL_RX, HIGH);
  digitalWrite(SERIAL_TX, HIGH);
  
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

  digitalWrite(RPI1, HIGH);
  digitalWrite(RPI2, HIGH);
  digitalWrite(RPI3, HIGH);
  digitalWrite(RPI4, HIGH);
  digitalWrite(RPI_INT1, HIGH);
  digitalWrite(RPI_INT2, HIGH);
  delay(3000);
}
