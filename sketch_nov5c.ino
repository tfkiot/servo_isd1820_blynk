#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3Z1nghYGrAV"
#define BLYNK_TEMPLATE_NAME "pet feeder"
#define BLYNK_AUTH_TOKEN "cmrbbqHLpa-X-j8OIPeNtL0JZPyhfu-2D"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

#define playPin D3
#define recPin D2

Servo servo;
BlynkTimer timer;

int lastPosition = 0;
int onnPos = 2050;
int offPos = 500;

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "TP-Link_339C";
char pass[] = "04051980";

bool servoOn = false;

void setup() {
Serial.begin(9600);

Blynk.begin(auth, ssid, pass);

servo.attach(D4);
pinMode(playPin, OUTPUT);
pinMode(recPin, OUTPUT);

timer.setInterval(30 * 60 * 1000, controlServo);
}

void loop() {
Blynk.run();
timer.run();
}

void controlServo() {

if (servoOn) {
servo.attach(D4);
servo.writeMicroseconds(offPos);
delay(500);
servo.detach();
lastPosition = offPos;
servoOn = false;
} 
else {
servo.attach(D4);
servo.writeMicroseconds(onnPos);
delay(500);
servo.detach();
lastPosition = onnPos;
servoOn = true;
timer.setTimeout(30 * 1000, turnOffServo);
}
}


void turnOffServo() {
servo.attach(D4);
servo.writeMicroseconds(offPos);
delay(500);
servo.detach();
lastPosition = offPos;
servoOn = false;
}

BLYNK_WRITE(V0) {
int value = param.asInt();
if (value == 1) {
servo.attach(D4);
servo.writeMicroseconds(onnPos);
delay(500);
servo.detach();
lastPosition = onnPos;
servoOn = true;
timer.setTimeout( 30 * 1000, turnOffServo);
} 
else {
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
if (value == 1) {
digitalWrite(recPin, HIGH);
delay(5000);
digitalWrite(recPin, LOW);
delay(500);
}
}

BLYNK_WRITE(V2) {
int value = param.asInt();
if (value == 1) {
digitalWrite(playPin, HIGH);
delay(5000);
digitalWrite(playPin, LOW);
delay(500);
}
}