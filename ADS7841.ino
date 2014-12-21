#include <SPI.h>

int ADC_CLK = 13;      // DCLK
int ADC_MISO = 12;     // DIN
int ADC_MOSI = 11;     // DOUT
int ADC_CS = 10;       // CS

byte commandByte;
byte result[2];
byte inputs[4] = { 1, 5, 2, 6 }; //Input selector according to datasheet

void setup() {
	Serial.begin(9600);
	//Set up SPI pins
	pinMode(ADC_CS, OUTPUT);
	pinMode(ADC_MOSI, OUTPUT);
	pinMode(ADC_MISO, INPUT);
	pinMode(ADC_CLK, OUTPUT);
	//Turn off device
	digitalWrite(ADC_CS, HIGH);
	//Set up SPI
	SPI.begin();
	SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
}

void loop() {
	byte commandByte;
	byte result[2];
	for (byte x = 0; x < 4; x++) {
		//Set start bit and and input selector
		commandByte = 8 | inputs[x];
		commandByte = commandByte << 4;
		//Set SGL/DIF bit
		commandByte = commandByte + 4;

		//Enable device
		digitalWrite(ADC_CS, LOW);

		//Send in command byte
		SPI.transfer(commandByte);

		//Get two result bytes
		result[0] = SPI.transfer(0);
		result[1] = SPI.transfer(0);

		//Disable device
		digitalWrite(ADC_CS, HIGH);

		//Convert result bytes to actual value
		int resultInt = result[0];
		resultInt = resultInt << 8;
		resultInt = resultInt | result[1];
		resultInt = resultInt >> 3;

		Serial.print("AD");
		Serial.print(x);
		Serial.print(" = ");
		Serial.println(resultInt);

		delay(100);
	}

	Serial.println();
	delay(1000);
}

