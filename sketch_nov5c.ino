// For debug logs
#define BLYNK_PRINT Serial

// Your Blynk credentials
#define BLYNK_TEMPLATE_ID "TMtytrnYGrAV"
#define BLYNK_TEMPLATE_NAME "petfeeder"
#define BLYNK_AUTH_TOKEN "cmrbcqfhfhj8OIPeNtL0JZPkru-2D"

//Necessary Libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <BlynkTimer.h>
#include <SoftwareSerial.h> // Add this include for software serial

SoftwareSerial isdSerial(D2, D3); // Create a software serial object for ISD1820

const int isd1820RecPin = D2; // Connect ISD1820’s REC pin to D2 on NodeMCU
const int isd1820PlayPin = D3; // Connect ISD1820’s PLAYE pin to D3 on NodeMCU

Servo servo; // Create a servo object
BlynkTimer timer; // Create BlynkTimer Object

int lastPosition = 0;  // To track the Position of servo
int onnPos = 2050; // clockwise direction
int offPos = 500;  // counter-clockwise direction


char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Link_339C";
char pass[] = "12345678";

bool servoOn = false;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  servo.attach(D4);
  pinMode(isd1820RecPin, OUTPUT);
  pinMode(isd1820PlayPin, OUTPUT);
  isdSerial.begin(9600); // Initialize software serial for ISD1820

  // Schedule the servo control function every 4 hours
  timer.setInterval(4 * 60 * 60 * 1000, controlServo);
 // Serial.println("Combined Code Setup");
}

void loop() {
  Blynk.run();
  timer.run();

  // Your loop code for ISD1820 can go here
  // For example, you can record and play audio in this section.
  // Ensure it doesn't block the Blynk or servo operations for an extended period.
  
  // Check for incoming data from ISD1820 (to know when recording is done)
  while (isdSerial.available()) {
    char receivedChar = isdSerial.read();
    if (receivedChar == 'R')    //receiving 'R'  is a common convention to trigger the recording process
     {
      Serial.println("Recording completed.");
    }
  }
}

void controlServo() {
  // Your servo control code here
  if (servoOn) {
    // If the servo is already on, turn it off
    servo.attach(D4);
    servo.writeMicroseconds(offPos);
    delay(500);
    servo.detach();
    lastPosition = offPos;
    servoOn = false;
  } else {

    // If the servo is off, turn it on for 5 minutes
    servo.attach(D4);
    servo.writeMicroseconds(onnPos);
    delay(500);
    servo.detach();
    lastPosition = onnPos;
    servoOn = true;
    // Schedule turning off the servo after 5 minutes
    timer.setTimeout(5 * 60 * 1000, turnOffServo);
  }
}

void turnOffServo() {
  // Your existing servo control code to turn it off here
  servo.attach(D4);
  servo.writeMicroseconds(offPos);
    delay(500);
    servo.detach();
    lastPosition = offPos;
    servoOn = false;
}

BLYNK_WRITE(V0) {
  int value = param.asInt();
  Serial.println(value);

  if (value == 1) {
    // If the button is pressed, manually turn on the servo
    servo.attach(D4);
    servo.writeMicroseconds(onnPos);
    delay(500);
    servo.detach();
    lastPosition = onnPos;
    servoOn = true;
    // Schedule turning off the servo after 5 minutes
    timer.setTimeout(5 * 60 * 1000, turnOffServo);
  } else {
    // If the button is released, manually turn off the servo
    servo.attach(D4);
    servo.writeMicroseconds(offPos);
    delay(500);
    servo.detach();
    lastPosition = offPos;
    servoOn = false;
  }
}

BLYNK_WRITE(V1) {
  int value = param.asInt();
  Serial.println(value);

  if (value == 1) {
    // If the button for recording is pressed, start recording on ISD1820
    digitalWrite(isd1820RecPin, HIGH); // Activate ISD1820's recording mode
    isdSerial.println("R"); // Send a character "R" to indicate recording start
    delay(500); // Delay for recording (adjust as needed)
    digitalWrite(isd1820RecPin, LOW); // Deactivate recording mode
  }
}

BLYNK_WRITE(V2) {
  int value = param.asInt();
  Serial.println(value);

  if (value == 1) {
    // If the button for playing is pressed, start playing audio on ISD1820
    digitalWrite(isd1820PlayPin, HIGH); // Activate ISD1820's playback mode
    delay(500); // Delay for playback (adjust as needed)
    digitalWrite(isd1820PlayPin, LOW); // Deactivate playback mode
  }
}

