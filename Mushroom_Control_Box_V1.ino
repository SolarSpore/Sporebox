#include <DHT.h>
#define Type DHT11

// Pins
const int HumidifierRelayPin = 2;
const int lightPin = 3;
const int buttonPinUp = 4;
const int buttonPinDown = 5;
const int Fan1RelayPin = 6;
const int Fan2RelayPin = 7;
const int sensorPin = 8;

// Humidity Sensor
DHT HT(sensorPin, Type);
float humidity = 0;

// Buttons
bool lastUpState = HIGH;
bool lastDownState = HIGH;
unsigned long lastButtonMillis = 0; // debounce timer
const unsigned long debounceTime = 150; // 150 ms debounce

// humidity set
int setHumidity = 85;

// Fan relays
const unsigned long onTime  = 5UL  * 60UL * 1000UL;   // 5 minutes
const unsigned long offTime = 55UL * 60UL * 1000UL;  // 55 minutes
bool fanState = false;

// Light
unsigned long lightOnTime  = 9UL  * 60UL * 60UL * 1000UL;  // 9 hours
unsigned long lightOffTime = 15UL * 60UL * 60UL * 1000UL; // 15 hours

// Timing
// Fan timing
unsigned long previousMillisFan = 0;

// Light timing
unsigned long previousMillisLight = 0;
bool lightState = true;

// DHT timing
unsigned long previousMillisDHT = 0;
const unsigned long dhtInterval = 2000; // 2 seconds

void setup() {
  // Humidity sensor
  HT.begin();

  // Buttons
  pinMode(buttonPinUp, INPUT_PULLUP);
  pinMode(buttonPinDown, INPUT_PULLUP);

  // Fan Relays (active LOW, so HIGH = OFF)
  pinMode(Fan1RelayPin, OUTPUT);
  digitalWrite(Fan1RelayPin, HIGH);  // Start OFF
  pinMode(Fan2RelayPin, OUTPUT);
  digitalWrite(Fan2RelayPin, HIGH);  // Start OFF

  // Humidifier Relay (active LOW)
  pinMode(HumidifierRelayPin, OUTPUT);
  digitalWrite(HumidifierRelayPin, HIGH);  // Start OFF

  // Light Relay (active LOW)
  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, LOW); // Start ON

  // Initialize timers
  previousMillisFan = millis();
  previousMillisLight = millis();

  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();

  // Humidity sensor
  if (currentMillis - previousMillisDHT >= dhtInterval) {
    humidity = HT.readHumidity();
    previousMillisDHT = currentMillis;

    Serial.print("Humidity: ");
    Serial.println(humidity);
  }

  // Humidity relay (active LOW)
  // ON < 80%, OFF > 85%
  if (!isnan(humidity)) {
    if (humidity < 80.0) {
      digitalWrite(HumidifierRelayPin, LOW);   // Turn ON
    } 
    else if (humidity > 85.0) {
      digitalWrite(HumidifierRelayPin, HIGH);  // Turn OFF
    }
  }

  // Fan Relays
  // 5 min ON / 55 min OFF
  if (!fanState && currentMillis - previousMillisFan >= offTime) {
    digitalWrite(Fan1RelayPin, LOW);
    digitalWrite(Fan2RelayPin, LOW);
    fanState = true;
    previousMillisFan = currentMillis;
  } 
  else if (fanState && currentMillis - previousMillisFan >= onTime) {
    digitalWrite(Fan1RelayPin, HIGH);
    digitalWrite(Fan2RelayPin, HIGH);
    fanState = false;
    previousMillisFan = currentMillis;
  }

  // Light relay (active LOW)
  // 9 hours ON / 15 hours OFF
  if (lightState && currentMillis - previousMillisLight >= lightOnTime) {
    digitalWrite(lightPin, HIGH); // Turn OFF
    lightState = false;
    previousMillisLight = currentMillis;
  } 
  else if (!lightState && currentMillis - previousMillisLight >= lightOffTime) {
    digitalWrite(lightPin, LOW);  // Turn ON
    lightState = true;
    previousMillisLight = currentMillis;
  }
}
