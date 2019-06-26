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

  mcp.pinMode(A0,OUTPUT);

}

void loop() {
  mcp.writeGPIOAB(0b00000000);
  delayMicroseconds(1);
  mcp.writeGPIOAB(0b00000001);
  delayMicroseconds(1);
}
