#include <SoftwareSerial.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define rxPin 10
#define txPin 11

#define mq7Pin A0
#define ldrPin A1
#define dhtPin 4	  // Digital pin connected to the DHT sensor
#define dhtType DHT22 // DHT 22

const int measurementsNum = 7;

DHT dht(dhtPin, dhtType);

// Set up a new SoftwareSerial object
SoftwareSerial espSerial = SoftwareSerial(rxPin, txPin);

Adafruit_BMP280 bmp; // I2C Interface

unsigned long previousMillis = 0; // Variable to store the last time measurements were taken
const long interval = 8000;	// Interval between measurements in milliseconds (8 seconds)

void setup()
{
	// Define pin modes for TX and RX
	pinMode(rxPin, INPUT);
	pinMode(txPin, OUTPUT);
	pinMode(dhtPin, INPUT);
	pinMode(mq7Pin, INPUT);
	dht.begin();

	Serial.begin(115200);	 // Open serial communication with computer
	espSerial.begin(115200); // Set baud rate for ESP01 communication (match ESP01 code)

	if (!bmp.begin(0x76, 0x58))
	{
		Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
		while (1);
	}

	/* Default settings from datasheet. */
	bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,	  /* Operating Mode. */
					Adafruit_BMP280::SAMPLING_X2,	  /* Temp. oversampling */
					Adafruit_BMP280::SAMPLING_X16,	  /* Pressure oversampling */
					Adafruit_BMP280::FILTER_X16,	  /* Filtering. */
					Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */


	
}

void loop()
{
	if(espSerial.available()>0){
		delay(100);
		String incomingString = "";
		boolean StringReady = false;
		
		while (espSerial.available() > 0)
		{
			incomingString = espSerial.readString();
			StringReady = true;
		}

		if(StringReady){
			Serial.println(incomingString);
		}
	}

	unsigned long currentMillis = millis(); // Get the current time

	// Check if it's time to take measurements
	if (currentMillis - previousMillis >= interval)
	{
		// Save the last time measurements were taken
		previousMillis = currentMillis;

		takeMeasurements();
	}
}

void takeMeasurements()
{
	float measurements[measurementsNum];

	// DHT Measurements
	float humidity = dht.readHumidity();
	float temperature = dht.readTemperature();

	// Check if any reads failed and exit early (to try again).
	if (isnan(humidity) || isnan(temperature))
	{
		Serial.println(F("Failed to read from DHT sensor!"));
	}
	else
	{
		Serial.print(F("Humidity: "));
		Serial.print(humidity);
		Serial.println("%");
		measurements[0]=humidity;

		Serial.print(F("Temperature: "));
		Serial.print(temperature);
		Serial.println("*C");
		measurements[1]=temperature;
	}

	// ToDo check if nan and choose temperature to send
	// BMP Measurements
	Serial.print(F("Temperature: "));
	temperature = bmp.readTemperature();
	Serial.print(temperature);
	Serial.println("*C");
	measurements[2]=temperature;

	Serial.print(F("Pressure: "));
	float pressure = bmp.readPressure() / 100;
	Serial.print(pressure);
	Serial.println("hPa");
	measurements[3]=pressure;

	Serial.print(F("Approx altitude: "));
	float altitude = bmp.readAltitude(1019.66);
	Serial.print(altitude); // The "1019.66" is the pressure(hPa) at sea level in day in your region
	Serial.println("m");
	measurements[4]=altitude;

	// ToDo add value and check if nan and if the sensor is plugged in
	// MQ-7 Measurements
	int mq7CO = analogRead(mq7Pin);
	Serial.print(F("Carbon Monoxide: "));
	Serial.print(mq7CO);
	Serial.println("ppm");
	measurements[5]=mq7CO;

	// LDR Measurements
	int ldr = analogRead(ldrPin);
	int brightnessPercentage = map(ldr, 0, 1023, 0, 100);
	Serial.print(F("Light Intensity: "));
	Serial.print(brightnessPercentage);
	Serial.println("%");
	measurements[6]=brightnessPercentage;

	clearSerialBuffer();
	for(int i=0; i<measurementsNum; i++){
		espSerial.print(measurements[i], 2);
		if(i !=6){
			espSerial.print(",");
		}else{
			espSerial.print('\n');
		}		
		delay(100);
	}

	Serial.println('\n');

	if (espSerial.overflow()) {
		Serial.println("SoftwareSerial overflow!");
		clearSerialBuffer();
	}
}

void clearSerialBuffer() {
  while (espSerial.available() > 0) {
    espSerial.read();  // Read and discard all available bytes
  }
}