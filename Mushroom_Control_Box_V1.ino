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
float humidity;
int setTime = 150;
int dt = 1000;

// Buttons
bool lastUpState = HIGH;
bool lastDownState = HIGH;
unsigned long lastButtonMillis = 0; // debounce timer
const unsigned long debounceTime = 150; // 150 ms debounce

// The value the buttons modify
int setHumidity = 85;

// Fan relays
const unsigned long onTime  = 5UL  * 60UL * 1000UL;   // 5 minutes
const unsigned long offTime = 55UL * 60UL * 1000UL;  // 55 minutes
bool fanState = false;

// Light
unsigned long lightOnTime = 9UL * 60UL * 60UL * 1000UL;  // 9 hours
unsigned long lightOffTime = 15UL * 60UL * 60UL * 1000UL; // 15 hours

// Timing
// Fan timing
unsigned long previousMillisFan = 0;

// Light timing
unsigned long previousMillisLight = 0;
bool lightState = false;

void setup() {
  // Humidity sensor
  HT.begin();

  // Buttons
  pinMode(buttonPinUp, INPUT_PULLUP);
  pinMode(buttonPinDown, INPUT_PULLUP);

  // Fan Relays (active LOW, so HIGH = OFF)
  pinMode(Fan1RelayPin, OUTPUT);
  digitalWrite(Fan1RelayPin, HIGH);  // Start with relay OFF
  pinMode(Fan2RelayPin, OUTPUT);
  digitalWrite(Fan2RelayPin, HIGH);  // Start with relay OFF

  // Humidifier Relay (active LOW)
  pinMode(HumidifierRelayPin, OUTPUT);
  digitalWrite(HumidifierRelayPin, HIGH);  // Start OFF

  // Light Relay (active LOW)
  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, HIGH); // Start OFF

  Serial.begin(9600);
}

void loop() {
  // Buttons
  bool upState = digitalRead(buttonPinUp);
  bool downState = digitalRead(buttonPinDown);
  unsigned long currentMillis = millis();

  // Humidity sensor
  humidity = HT.readHumidity();
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" Set Humidity: ");
  Serial.println(setHumidity);

  // Up button
  if (upState == LOW && lastUpState == HIGH && currentMillis - lastButtonMillis > debounceTime) {
    setHumidity++;
    setHumidity = constrain(setHumidity, 0, 100);
    lastButtonMillis = currentMillis;
    Serial.println(setHumidity);
  }

  // Down button
  if (downState == LOW && lastDownState == HIGH && currentMillis - lastButtonMillis > debounceTime) {
    setHumidity--;
    setHumidity = constrain(setHumidity, 0, 100);
    lastButtonMillis = currentMillis;
    Serial.print("New setHumidity: ");
    Serial.println(setHumidity);
  }

  lastUpState = upState;
  lastDownState = downState;

// Fan Relays
  if (!fanState && currentMillis - previousMillisFan >= offTime) {
    // Turn fans ON (active LOW)
    digitalWrite(Fan1RelayPin, LOW);
    digitalWrite(Fan2RelayPin, LOW);
    fanState = true;
    previousMillisFan = currentMillis;
  } else if (fanState && currentMillis - previousMillisFan >= onTime) {
    // Turn fans OFF
    digitalWrite(Fan1RelayPin, HIGH);
    digitalWrite(Fan2RelayPin, HIGH);
    fanState = false;
    previousMillisFan = currentMillis;
  }

  // Humidity relay (active LOW)
  if (humidity < setHumidity) {
    digitalWrite(HumidifierRelayPin, LOW);  // Turn ON
  } else {
    digitalWrite(HumidifierRelayPin, HIGH); // Turn OFF
  }

  // Light relay (active LOW)
  if (!lightState && currentMillis - previousMillisLight >= lightOffTime) {
    digitalWrite(lightPin, LOW);  // Turn ON
    lightState = true;
    previousMillisLight = currentMillis;
  } else if (lightState && currentMillis - previousMillisLight >= lightOnTime) {
    digitalWrite(lightPin, HIGH); // Turn OFF
    lightState = false;
    previousMillisLight = currentMillis;
  }

  // Humidity display
  int displayValue = constrain(setHumidity, 0, 100);
}