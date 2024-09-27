#include <Arduino.h>
#line 1 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\nodeMcuSketch\\nodeMcuSketch.ino"
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DHTPIN D1     // Pin where the DHT11 is connected
#define DHTTYPE DHT11 // DHT 11

// MQTT broker details
const char *mqtt_server_ip = "192.168.147.33";
const int mqtt_port = 1883;
const char *mqtt_username = "administrator";
const char *mqtt_password = "administrator";

const int measurementsNum = 2;

void mqttConnection();
void connectToWiFi();

const char *ssid = "A52s";
const char *password = "knto2371";

unsigned long previousMillis = 0; // Variable to store the last time connection attemption
const long interval = 5000;        // Interval between connection in 5 seconds

const char *mqttTopics[] = {
  "nodeMcu/humidity/dht22",
  "nodeMcu/temprerature/dht22"
};

// Create a WiFiClient object to connect to the MQTT broker
WiFiClient nodeMCUClient;
PubSubClient client(nodeMCUClient);

DHT dht(DHTPIN, DHTTYPE);

#line 36 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\nodeMcuSketch\\nodeMcuSketch.ino"
void setup();
#line 45 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\nodeMcuSketch\\nodeMcuSketch.ino"
void loop();
#line 36 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\nodeMcuSketch\\nodeMcuSketch.ino"
void setup() {
    Serial.begin(9600);
    WiFi.mode(WIFI_STA);
    connectToWiFi();
    delay(2100); // Delay to let the serial console settle
    client.setServer(mqtt_server_ip, mqtt_port);
    dht.begin();
}

void loop() {
    connectToWiFi();
    if (!client.connected())
    {
        mqttConnection();
        delay(2000);
    }
    client.loop();
    
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
        float measurements[measurementsNum];
		Serial.print(F("Humidity: "));
		Serial.print(humidity);
        measurements[0]=humidity;
		Serial.println("%");

		Serial.print(F("Temperature: "));
		Serial.print(temperature);
        measurements[1]=temperature;
		Serial.println("*C");

        for (int i = 0; i < measurementsNum; i++) {
            char charArray[10]; // Character array to hold the string representation
            dtostrf(measurements[i], 6, 2, charArray); // Parameters: value, width, precision, buffer

            client.publish(mqttTopics[i], charArray);
            delay(500);
        }

	}
    ESP.deepSleep(30e6);
}

void connectToWiFi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Connecting to WiFi...");
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(2000);
            Serial.println("Connecting...");
        }
        Serial.println("Connected to WiFi");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    }
}

void mqttConnection()
{
    // Loop until we're reconnected to the MQTT broker
    while (!client.connected())
    {
        unsigned long currentMillis = millis();

        if (currentMillis - previousMillis >= interval)
        {
            Serial.print("Attempting MQTT connection...");
            // Attempt to connect
            if (client.connect("NodeMCUClient", mqtt_username, mqtt_password))
            {
                Serial.println("Connected!");
            }
            else
            {
                Serial.print("failed, rc=");
                Serial.print(client.state());
                Serial.println(" try again in 5 seconds");
            }

            previousMillis = currentMillis;
        }
    }
}

