/*************************************************** 
 This is a library for the MCP23S17 i2c port expander

 These displays use I2C to communicate, 2 pins are required to
 interface
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries.
 BSD license, all text above must be included in any redistribution
 ****************************************************/

#ifdef __AVR
  #include <avr/pgmspace.h>
#elif defined(ESP8266)
  #include <pgmspace.h>
#endif
#include "Adafruit_MCP23S17.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/**
 * Bit number associated to a give Pin
 */
uint8_t Adafruit_MCP23S17::bitForPin(uint8_t pin){
	return pin%8;
}

/**
 * Register address, port dependent, for a given PIN
 */
uint8_t Adafruit_MCP23S17::regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr){
	return(pin<8) ?portAaddr:portBaddr;
}

/**
 * Reads a given register
 */
uint8_t Adafruit_MCP23S17::readRegister(uint8_t addr){	
	::digitalWrite(spics, LOW);
    SPI.transfer(MCP23S17_ADDRESS | (spiaddr << 1) | 1);
    SPI.transfer(addr);
	uint8_t data = SPI.transfer(0xFF);
    ::digitalWrite(spics, HIGH);
	return data;
}


/**
 * Writes a given register
 */
void Adafruit_MCP23S17::writeRegister(uint8_t regAddr, uint8_t regValue){	
    ::digitalWrite(spics, LOW);
    SPI.transfer(MCP23S17_ADDRESS | spiaddr);
    SPI.transfer(regAddr);
    SPI.transfer(regValue);
    ::digitalWrite(spics, HIGH);
}


/**
 * Helper to update a single bit of an A/B register.
 * - Reads the current register value
 * - Writes the new register value
 */
void Adafruit_MCP23S17::updateRegisterBit(uint8_t pin, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr) {
	uint8_t regValue;
	uint8_t regAddr=regForPin(pin,portAaddr,portBaddr);
	uint8_t bit=bitForPin(pin);
	regValue = readRegister(regAddr);

	// set the value for the particular bit
	bitWrite(regValue,bit,pValue);

	writeRegister(regAddr,regValue);
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Initializes the MCP23S17 given its HW selected address, see datasheet for Address selection.
 */
void Adafruit_MCP23S17::begin(uint8_t addr, uint8_t cs) {
	if (addr > 7) {
		addr = 7;
	}
	spiaddr = addr;
	spics = cs;
	
	SPI.begin();
	
	::pinMode(spics, OUTPUT);
	::digitalWrite(spics, HIGH);
	
	// Disable slew rate control bit for SDA output and enable hardware address 
	::digitalWrite(spics, LOW);
    SPI.transfer(MCP23S17_ADDRESS);
    SPI.transfer(MCP23S17_IOCONA);
    SPI.transfer(0x18);
    ::digitalWrite(spics, HIGH);
	
	::digitalWrite(spics, LOW);
    SPI.transfer(MCP23S17_ADDRESS);
    SPI.transfer(MCP23S17_IOCONB);
    SPI.transfer(0x18);
    ::digitalWrite(spics, HIGH);

	// set defaults!
	// all inputs on port A and B
	writeRegister(MCP23S17_IODIRA,0xff);
	writeRegister(MCP23S17_IODIRB,0xff);
}

/**
 * Initializes the default MCP23S17, with 000 for the configurable part of the address
 */
void Adafruit_MCP23S17::begin(uint8_t cs) {
	begin(0, cs);
}

/**
 * Sets the pin mode to either INPUT or OUTPUT
 */
void Adafruit_MCP23S17::pinMode(uint8_t p, uint8_t d) {
	updateRegisterBit(p,(d==INPUT),MCP23S17_IODIRA,MCP23S17_IODIRB);
}

/**
 * Reads all 16 pins (port A and B) into a single 16 bits variable.
 */
uint16_t Adafruit_MCP23S17::readGPIOAB() {
	uint16_t ba = 0;
	uint8_t a;

	// read the current GPIO output latches
	::digitalWrite(spics, LOW);
    SPI.transfer((MCP23S17_ADDRESS | spiaddr)<<1);
    SPI.transfer(MCP23S17_GPIOA);
	a  = SPI.transfer(0xFF);
	ba = SPI.transfer(0xFF);
    ::digitalWrite(spics, HIGH);
	
	ba <<= 8;
	ba |= a;

	return ba;
}

/**
 * Read a single port, A or B, and return its current 8 bit value.
 * Parameter b should be 0 for GPIOA, and 1 for GPIOB.
 */
uint8_t Adafruit_MCP23S17::readGPIO(uint8_t b) {

	// read the current GPIO output latches
	if (b == 0)
		return readRegister(MCP23S17_GPIOA);
	else {
		return readRegister(MCP23S17_GPIOB);
	}
}

/**
 * Writes all the pins in one go. This method is very useful if you are implementing a multiplexed matrix and want to get a decent refresh rate.
 */
void Adafruit_MCP23S17::writeGPIOAB(uint16_t ba) {
	::digitalWrite(spics, LOW);
    SPI.transfer((MCP23S17_ADDRESS | spiaddr)<<1);
    SPI.transfer(MCP23S17_GPIOA);
    SPI.transfer(ba & 0xFF);
	SPI.transfer(ba >> 8);
    ::digitalWrite(spics, HIGH);
}

void Adafruit_MCP23S17::digitalWrite(uint8_t pin, uint8_t d) {
	uint8_t gpio;
	uint8_t bit=bitForPin(pin);


	// read the current GPIO output latches
	uint8_t regAddr=regForPin(pin,MCP23S17_OLATA,MCP23S17_OLATB);
	gpio = readRegister(regAddr);

	// set the pin and direction
	bitWrite(gpio,bit,d);

	// write the new GPIO
	regAddr=regForPin(pin,MCP23S17_GPIOA,MCP23S17_GPIOB);
	writeRegister(regAddr,gpio);
}

void Adafruit_MCP23S17::pullUp(uint8_t p, uint8_t d) {
	updateRegisterBit(p,d,MCP23S17_GPPUA,MCP23S17_GPPUB);
}

uint8_t Adafruit_MCP23S17::digitalRead(uint8_t pin) {
	uint8_t bit=bitForPin(pin);
	uint8_t regAddr=regForPin(pin,MCP23S17_GPIOA,MCP23S17_GPIOB);
	return (readRegister(regAddr) >> bit) & 0x1;
}

/**
 * Configures the interrupt system. both port A and B are assigned the same configuration.
 * Mirroring will OR both INTA and INTB pins.
 * Opendrain will set the INT pin to value or open drain.
 * polarity will set LOW or HIGH on interrupt.
 * Default values after Power On Reset are: (false,flase, LOW)
 * If you are connecting the INTA/B pin to arduino 2/3, you should configure the interupt handling as FALLING with
 * the default configuration.
 */
void Adafruit_MCP23S17::setupInterrupts(uint8_t mirroring, uint8_t openDrain, uint8_t polarity){
	// configure the port A
	uint8_t ioconfValue=readRegister(MCP23S17_IOCONA);
	bitWrite(ioconfValue,6,mirroring);
	bitWrite(ioconfValue,2,openDrain);
	bitWrite(ioconfValue,1,polarity);
	writeRegister(MCP23S17_IOCONA,ioconfValue);

	// Configure the port B
	ioconfValue=readRegister(MCP23S17_IOCONB);
	bitWrite(ioconfValue,6,mirroring);
	bitWrite(ioconfValue,2,openDrain);
	bitWrite(ioconfValue,1,polarity);
	writeRegister(MCP23S17_IOCONB,ioconfValue);
}

/**
 * Set's up a pin for interrupt. uses arduino MODEs: CHANGE, FALLING, RISING.
 *
 * Note that the interrupt condition finishes when you read the information about the port / value
 * that caused the interrupt or you read the port itself. Check the datasheet can be confusing.
 *
 */
void Adafruit_MCP23S17::setupInterruptPin(uint8_t pin, uint8_t mode) {

	// set the pin interrupt control (0 means change, 1 means compare against given value);
	updateRegisterBit(pin,(mode!=CHANGE),MCP23S17_INTCONA,MCP23S17_INTCONB);
	// if the mode is not CHANGE, we need to set up a default value, different value triggers interrupt

	// In a RISING interrupt the default value is 0, interrupt is triggered when the pin goes to 1.
	// In a FALLING interrupt the default value is 1, interrupt is triggered when pin goes to 0.
	updateRegisterBit(pin,(mode==FALLING),MCP23S17_DEFVALA,MCP23S17_DEFVALB);

	// enable the pin for interrupt
	updateRegisterBit(pin,HIGH,MCP23S17_GPINTENA,MCP23S17_GPINTENB);

}

uint8_t Adafruit_MCP23S17::getLastInterruptPin(){
	uint8_t intf;

	// try port A
	intf=readRegister(MCP23S17_INTFA);
	for(int i=0;i<8;i++) if (bitRead(intf,i)) return i;

	// try port B
	intf=readRegister(MCP23S17_INTFB);
	for(int i=0;i<8;i++) if (bitRead(intf,i)) return i+8;

	return MCP23S17_INT_ERR;

}
uint8_t Adafruit_MCP23S17::getLastInterruptPinValue(){
	uint8_t intPin=getLastInterruptPin();
	if(intPin!=MCP23S17_INT_ERR){
		uint8_t intcapreg=regForPin(intPin,MCP23S17_INTCAPA,MCP23S17_INTCAPB);
		uint8_t bit=bitForPin(intPin);
		return (readRegister(intcapreg)>>bit) & (0x01);
	}

	return MCP23S17_INT_ERR;
}
