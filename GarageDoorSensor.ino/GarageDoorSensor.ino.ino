// Enable debug prints
//#define MY_DEBUG

// Enable and select radio type attached
//#define MY_RADIO_NRF24
//#define MY_RADIO_NRF5_ESB
#define MY_RADIO_RFM69
//#define MY_RADIO_RFM95

#define MY_RFM69_FREQUENCY RFM69_433MHZ
#define MY_IS_RFM69HW

#include <MySensors.h>

#define SKETCH_NAME "Binary Sensor"
#define SKETCH_MAJOR_VER "1"
#define SKETCH_MINOR_VER "0"

#define PRIMARY_CHILD_ID 3

#define PRIMARY_BUTTON_PIN 3   // Arduino Digital I/O pin for button/reed switch


int BATTERY_SENSE_PIN = A0;  // select the input pin for the battery sense point
int oldBatteryPcnt = 0;

// Change to V_LIGHT if you use S_LIGHT in presentation below
MyMessage msg(PRIMARY_CHILD_ID, V_TRIPPED);





void setup()
{
  // Setup the buttons
  pinMode(PRIMARY_BUTTON_PIN, INPUT_PULLUP);
  #if defined(__AVR_ATmega2560__)
  analogReference(INTERNAL1V1);
#else
  analogReference(INTERNAL);
#endif
}









void presentation()
{
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo(SKETCH_NAME, SKETCH_MAJOR_VER "." SKETCH_MINOR_VER);

  // Register binary input sensor to sensor_node (they will be created as child devices)
  // You can use S_DOOR, S_MOTION or S_LIGHT here depending on your usage.
  // If S_LIGHT is used, remember to update variable type you send in. See "msg" above.
  present(PRIMARY_CHILD_ID, S_DOOR);
}

// Loop will iterate on changes on the BUTTON_PINs
void loop()
{
  int sensorValue = analogRead(BATTERY_SENSE_PIN);
  int batteryPcnt = sensorValue / 10;
  Serial.println(sensorValue);
  
    if (oldBatteryPcnt != batteryPcnt) {
    // Power up radio after sleep
    sendBatteryLevel(batteryPcnt);
    oldBatteryPcnt = batteryPcnt;
  }
  #ifdef MY_DEBUG
  float batteryV  = sensorValue * 0.003363075;
  Serial.print("Battery Voltage: ");
  Serial.print(batteryV);
  Serial.println(" V");

  Serial.print("Battery percent: ");
  Serial.print(batteryPcnt);
  Serial.println(" %");
#endif







  
  uint8_t value;
  static uint8_t sentValue=2;

  // Short delay to allow buttons to properly settle
  sleep(5000);

  value = digitalRead(PRIMARY_BUTTON_PIN);

  if (value != sentValue) {
    // Value has changed from last transmission, send the updated value
    send(msg.set(value==HIGH));
    sentValue = value;
  }

  // Sleep until something happens with the sensor
  sleep(PRIMARY_BUTTON_PIN-2, CHANGE, 7200000);
}


