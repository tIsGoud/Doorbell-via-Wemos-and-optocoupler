/*
 * Doorbell project to determine if the doorbell was pressed.
 * 
 * The "signal" to the physical doorbell is intercepted and send to a WEMOS D1 mini on pin D3 (GPIO0).
 * 
 * In this version an optocoupler is used to decouple the Wemos from the doorbell. 
 * This idea was originally published in the German c't magazine [17/2017].
 * 
 * The Wemos publishes a message to an MQTT instance.
 * 
 * Wi-Fi and MQTT configuration are set in the Homie config file (data/homie/config.json).
 * 
 * Used libraries: 
 * - Homie 2.0.0 
 * - ArduinoJSon 5.11.1
 * 
 * Parameters:
 * - readDelay (milliseconds), sets the time between reading a value
 * - buttonPressDelay (milliseconds) , is used to prevent multiple reads (very pragmatic but works fine)
 * 
 * Author: Albert W. Alberts
 * Date:   14-08-2017
 * 
 */
#include <Homie.h>

int readDelay = 50;           // 50 is the max, higher makes it unreliable 
int buttonPressDelay = 3000;  // 3000 = 3 seconds 
int inputPin = D3;            // D3 or 0 (for GPIO0)
int intputValue = 0;

HomieNode doorbellNode("button", "button");

void loopHandler() {
  // Read the input value on the input pin
  inputValue = digitalRead(inputPin);
  
  // Turn on the internal led, internal led is active low
  digitalWrite(BUILTIN_LED, inputValue);

  if (inputValue == 0) {
    Serial.println("Doorbell pressed ...");
    doorbellNode.setProperty("pressed").send("true");
    delay(buttonPressDelay);
  }
  delay(readDelay);
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Pull-up to prevent "floating" input
  pinMode(inputPin, INPUT_PULLUP);
  pinMode(BUILTIN_LED, OUTPUT);
    
  Homie_setBrand("wemos");
  Homie_setFirmware("Doorbell-MQTT", "2.0.0");
  
  Homie.setLoopFunction(loopHandler);
  
  Homie.setup();
}

void loop() {
  Homie.loop();
}

