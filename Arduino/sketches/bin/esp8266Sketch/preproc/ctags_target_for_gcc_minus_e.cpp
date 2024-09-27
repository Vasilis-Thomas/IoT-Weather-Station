# 1 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\esp8266Sketch\\esp8266Sketch.ino"
# 2 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\esp8266Sketch\\esp8266Sketch.ino" 2
# 3 "D:\\Files\\Desktop\\Arduino Weather Station\\Arduino\\sketches\\esp8266Sketch\\esp8266Sketch.ino" 2

// MQTT broker details
const char *mqtt_server_ip = "192.168.147.33";
const int mqtt_port = 1883;
const char *mqtt_username = "administrator";
const char *mqtt_password = "administrator";

void mqttConnection();
void connectToWiFi();
void receiveData();
void clearSerialBuffer();

const char *ssid = "A52s";
const char *password = "knto2371";

unsigned long previousMillis = 0; // Variable to store the last time connection attemption
const long interval = 100; // Interval between connection in milliseconds (100 ms)

const char *mqttTopics[] = {
  "arduino/humidity/dht22",
  "arduino/temprerature/dht22",
  "arduino/temprerature/bmp280",
  "arduino/pressure/bmp280",
  "arduino/altitude/bmp280",
  "arduino/carbon_monoxide/mq7",
  "arduino/light_intensity/ldr"
};

// Create a WiFiClient object to connect to the MQTT broker
WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    connectToWiFi();
    delay(2100); // Delay to let the serial console settle
    client.setServer(mqtt_server_ip, mqtt_port);
}

void loop()
{
    connectToWiFi();
    if (!client.connected())
    {
        mqttConnection();
        delay(2000);
    }
    client.loop();

    receiveData();
}

void connectToWiFi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        clearSerialBuffer();
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
            clearSerialBuffer();
            Serial.print("Attempting MQTT connection...");
            // Attempt to connect
            if (client.connect("ESP8266Client", mqtt_username, mqtt_password))
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

void receiveData()
{
    String incomingData = "";
    if (Serial.available() > 0)
    {
        delay(2000);
        incomingData = Serial.readStringUntil('\n');

        clearSerialBuffer();
        Serial.println("Esp received data:" + incomingData);

        float values[7];
        // Parse received string into float values
        int valueIndex = 0;
        int startIndex = 0;
        for (int i = 0; i < incomingData.length(); i++) {
            if (incomingData.charAt(i) == ',') {
                values[valueIndex++] = incomingData.substring(startIndex, i).toFloat(); // Extract substring and convert to float
                startIndex = i + 1; // Update start index for next substring
            }
        }

        // Handle the last float value after the last comma
        if (valueIndex < 7) {
            values[valueIndex] = incomingData.substring(startIndex).toFloat();
        }

        for (int i = 0; i < 7; i++) {
            char charArray[10]; // Character array to hold the string representation
            dtostrf(values[i], 7, 2, charArray); // Parameters: value, width, precision, buffer

            client.publish(mqttTopics[i], charArray);
        }
    }
}


void clearSerialBuffer() {
    while (Serial.available() > 0) {
        Serial.read(); // Read and discard all available bytes
    }
}
