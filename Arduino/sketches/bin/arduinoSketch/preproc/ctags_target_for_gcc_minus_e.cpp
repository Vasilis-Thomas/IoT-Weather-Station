# 1 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
# 2 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 2
# 3 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 2
# 4 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 2
# 5 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 2
# 6 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 2
# 15 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
const int measurementsNum = 7;

DHT dht(4 /* Digital pin connected to the DHT sensor*/, 22 /* DHT 22*/);

// Set up a new SoftwareSerial object
SoftwareSerial espSerial = SoftwareSerial(10, 11);

Adafruit_BMP280 bmp; // I2C Interface

unsigned long previousMillis = 0; // Variable to store the last time measurements were taken
const long interval = 15000; // Interval between measurements in milliseconds (8 seconds)

void setup()
{
 // Define pin modes for TX and RX
 pinMode(10, 0x0);
 pinMode(11, 0x1);
 pinMode(4 /* Digital pin connected to the DHT sensor*/, 0x0);
 pinMode(A0, 0x0);
 dht.begin();

 Serial.begin(115200); // Open serial communication with computer
 espSerial.begin(115200); // Set baud rate for ESP01 communication (match ESP01 code)

 if (!bmp.begin(0x76, 0x58))
 {
  Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 41 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 41 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
                "Could not find a valid BMP280 sensor, check wiring!"
# 41 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
                ); &__c[0];}))
# 41 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
                )));
  while (1);
 }

 /* Default settings from datasheet. */
 bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, /* Operating Mode. */
     Adafruit_BMP280::SAMPLING_X2, /* Temp. oversampling */
     Adafruit_BMP280::SAMPLING_X16, /* Pressure oversampling */
     Adafruit_BMP280::FILTER_X16, /* Filtering. */
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
  Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 97 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 97 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
                "Failed to read from DHT sensor!"
# 97 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
                ); &__c[0];}))
# 97 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
                )));
 }
 else
 {
  Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 101 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 101 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
              "Humidity: "
# 101 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
              ); &__c[0];}))
# 101 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
              )));
  Serial.print(humidity);
  Serial.println("%");
  measurements[0]=humidity;

  Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 106 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 106 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
              "Temperature: "
# 106 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
              ); &__c[0];}))
# 106 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
              )));
  Serial.print(temperature);
  Serial.println("*C");
  measurements[1]=temperature;
 }

 // ToDo check if nan and choose temperature to send
 // BMP Measurements
 Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 114 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 114 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
             "Temperature: "
# 114 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
             ); &__c[0];}))
# 114 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
             )));
 temperature = bmp.readTemperature();
 Serial.print(temperature);
 Serial.println("*C");
 measurements[2]=temperature;

 Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 120 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 120 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
             "Pressure: "
# 120 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
             ); &__c[0];}))
# 120 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
             )));
 float pressure = bmp.readPressure() / 100;
 Serial.print(pressure);
 Serial.println("hPa");
 measurements[3]=pressure;

 Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 126 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 126 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
             "Approx altitude: "
# 126 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
             ); &__c[0];}))
# 126 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
             )));
 float altitude = bmp.readAltitude(1019.66);
 Serial.print(altitude); // The "1019.66" is the pressure(hPa) at sea level in day in your region
 Serial.println("m");
 measurements[4]=altitude;

 // ToDo add value and check if nan and if the sensor is plugged in
 // MQ-7 Measurements
 int mq7CO = analogRead(A0);
 Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 135 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 135 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
             "Carbon Monoxide: "
# 135 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
             ); &__c[0];}))
# 135 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
             )));
 Serial.print(mq7CO);
 Serial.println("ppm");
 measurements[5]=mq7CO;

 // LDR Measurements
 int ldr = analogRead(A1);
 int brightnessPercentage = map(ldr, 0, 1023, 0, 100);
 Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 143 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
             (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 143 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
             "Light Intensity: "
# 143 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino" 3
             ); &__c[0];}))
# 143 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\arduinoSketch\\arduinoSketch.ino"
             )));
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
    espSerial.read(); // Read and discard all available bytes
  }
}
