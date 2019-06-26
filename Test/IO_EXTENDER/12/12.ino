#include <SPI.h>
#include "Adafruit_MCP23S17.h"

#define FREQUENCY 8000000

Adafruit_MCP23S17 mcp;

void setup() {

  //enable_reset();
  //disable_nfc();
  
	mcp.begin(PIN_SPI_CE);
  SPISettings settings(FREQUENCY, MSBFIRST, SPI_MODE0);
  SPI.beginTransaction(settings);

  mcp.pinMode(A7,INPUT);
  mcp.setupInterrupts(false, false, HIGH);
  mcp.setupInterruptPin(A7,CHANGE);
  
  pinMode(11,INPUT);
  attachInterrupt(digitalPinToInterrupt(11),HandleInterrupt,RISING);

  mcp.digitalRead(A7);

}

void loop() {
}

void HandleInterrupt(){
  /*
   * To achieve max speed for interrupt, you have to optimize read operation on SPI bus.
   * Since we know which pin are configured as interrupt and which port triggered the callback,
   * read ONLY the pin(s) on the port who actually triggered the interrupt
   */

  mcp.digitalRead(A7);

}
