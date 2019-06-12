/**
 * Basic ADC reading example.
 * - connects to ADC
 * - reads value from channel
 * - converts value to analog voltage
 */

#include <SPI.h>
#include <Mcp3208.h>

#define ADC_VREF    4096     // 4,096V Vref
#define ADC_CLK     2000000  // SPI clock 2MHz (max speed for MCP3208)

MCP3208 adc(ADC_VREF, PIN_SPI_CE_ADC);

void setup() {

  // configure PIN mode
  pinMode(PIN_SPI_CE_ADC, OUTPUT);

  // set initial PIN state
  digitalWrite(PIN_SPI_CE_ADC, HIGH);

  // initialize serial
  Serial.begin(115200);

  // initialize SPI interface for MCP3208
  SPISettings settings(ADC_CLK, MSBFIRST, SPI_MODE0);
  SPI.begin();
  SPI.beginTransaction(settings);
}

void loop() {

  uint32_t t1;
  uint32_t t2;

  // start sampling
  Serial.println("Reading...");

  t1 = micros();
  uint16_t raw = adc.read(MCP3208::SINGLE_0);
  t2 = micros();

  // get analog value
  uint16_t val = adc.toAnalog(raw);

  // readed value
  Serial.print("value: ");
  Serial.print(raw);
  Serial.print(" (");
  Serial.print(val);
  Serial.println(" mV)");

  // sampling time
  Serial.print("Sampling time: ");
  Serial.print(static_cast<double>(t2 - t1) / 1000, 4);
  Serial.println("ms");

  delay(2000);
}
