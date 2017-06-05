#include "BasicStepperDriver.h"
int incomingByte = 0;
int identifierByte = 0; // for incoming serial data
int powerByte = 0;
int FORE_PORT_MOTOR = 0;
int FORE_STARBOARD_MOTOR = 0;
int AFT_PORT_MOTOR = 0;
int AFT_STARBOARD_MOTOR = 0;

bool lightOn = true;
bool clawClosed = true;

int SYNC_MARKER = 128;

int FORE_PORT_MOTOR_PWM = 3;
int FORE_STARBOARD_MOTOR_PWM = 5;
int UP_MOTOR_PWM = 7;

int FORE_PORT_MOTOR_DIGITAL = 27;
int FORE_STARBOARD_MOTOR_DIGITAL = 31;
int UP_MOTOR_DIGITAL = 35;

int AFT_PORT_MOTOR_PWM = 2;
int AFT_STARBOARD_MOTOR_PWM = 4;
int UP_MOTOR_PWM2 = 6;

int AFT_PORT_MOTOR_DIGITAL = 26;
int AFT_STARBOARD_MOTOR_DIGITAL = 30;
int UP_MOTOR_DIGITAL2 = 34;

int CLAW_PIN_DIGITAL = 24;
int LIGHT_PIN_DIGITAL = 25;

int CLAW_ROTATE_DIGITAL = 40;
int CLAW_STEP_DIGITAL = 41;

#define MOTOR_STEPS 200
#define MICROSTEPS 1
BasicStepperDriver stepper(MOTOR_STEPS, CLAW_ROTATE_DIGITAL, CLAW_STEP_DIGITAL);
unsigned long curMillis;
unsigned long prevStepMillis = 0;
unsigned long millisBetweenSteps = 5; // milliseconds

void setup() {
  // put your setup code here, to run once:
 Serial.begin(19200);     // opens serial port, sets data rate to 19200 bp

 pinMode(FORE_PORT_MOTOR_PWM, OUTPUT);   //opens pins for motors
 pinMode(FORE_STARBOARD_MOTOR_PWM, OUTPUT);
 pinMode(AFT_PORT_MOTOR_PWM, OUTPUT);   
 pinMode(AFT_STARBOARD_MOTOR_PWM, OUTPUT);
 
 pinMode(UP_MOTOR_PWM, OUTPUT);
 pinMode(UP_MOTOR_PWM2, OUTPUT);
  
 pinMode(FORE_PORT_MOTOR_DIGITAL, OUTPUT);
 pinMode(FORE_STARBOARD_MOTOR_DIGITAL, OUTPUT);
 pinMode(AFT_PORT_MOTOR_DIGITAL, OUTPUT);
 pinMode(AFT_STARBOARD_MOTOR_DIGITAL, OUTPUT);
 
 pinMode(UP_MOTOR_DIGITAL2, OUTPUT);
 pinMode(UP_MOTOR_DIGITAL, OUTPUT);
 
 pinMode(CLAW_PIN_DIGITAL, OUTPUT);  //opens pins for claw
 pinMode(LIGHT_PIN_DIGITAL, OUTPUT);  //opens pins for lights
 
 pinMode(CLAW_ROTATE_DIGITAL, OUTPUT);  
 pinMode(CLAW_STEP_DIGITAL, OUTPUT);
 stepper.setRPM(70);

}

void loop() {
  // put your main code here, to run repeatedly:
  curMillis = millis();
  
  if (Serial.available() > 2) {
    // read the incoming byte:
    incomingByte = Serial.read();
      identifierByte = Serial.read();
      powerByte = Serial.read();
      
      Serial.println("=========");
      Serial.println(incomingByte, DEC);
    if (incomingByte == SYNC_MARKER) {
      Serial.println(identifierByte, DEC);
      Serial.println(powerByte, DEC);
      Serial.println("=========");
      
      if (identifierByte == 0) {
        while (Serial.available() < 4){delay(10);}
        FORE_PORT_MOTOR = powerByte;
        FORE_STARBOARD_MOTOR = Serial.read();
        AFT_PORT_MOTOR = Serial.read();
        AFT_STARBOARD_MOTOR = Serial.read();
        //control left motors:
        if (FORE_PORT_MOTOR > 128) {
          FORE_PORT_MOTOR = FORE_PORT_MOTOR - 256;
        }
        if (FORE_STARBOARD_MOTOR > 128) {
          FORE_STARBOARD_MOTOR = FORE_STARBOARD_MOTOR - 256;
        }
        if (AFT_PORT_MOTOR > 128) {
          AFT_PORT_MOTOR = AFT_PORT_MOTOR - 256;
        }
        if (AFT_STARBOARD_MOTOR > 128) {
          AFT_STARBOARD_MOTOR = AFT_STARBOARD_MOTOR - 256;
        }
        Serial.print("controlling motors, power: ");
        Serial.print(FORE_PORT_MOTOR, DEC);
        Serial.print("  ");
        Serial.print(FORE_STARBOARD_MOTOR, DEC);
        Serial.print("  ");
        Serial.print(AFT_PORT_MOTOR, DEC);
        Serial.print("  ");
        Serial.print(AFT_STARBOARD_MOTOR, DEC);
        Serial.println();
        
        if (FORE_PORT_MOTOR != 0){
          if(FORE_PORT_MOTOR > 0){
           digitalWrite(FORE_PORT_MOTOR_DIGITAL, HIGH);
          } 
          else {
           digitalWrite(FORE_PORT_MOTOR_DIGITAL, LOW);
          }
         analogWrite(FORE_PORT_MOTOR_PWM, abs(FORE_PORT_MOTOR * 2));
        }

        if (FORE_STARBOARD_MOTOR != 0){
          if(FORE_STARBOARD_MOTOR > 0){
           digitalWrite(FORE_STARBOARD_MOTOR_DIGITAL, HIGH);
          } 
          else {
           digitalWrite(FORE_STARBOARD_MOTOR_DIGITAL, LOW);
          }
         analogWrite(FORE_STARBOARD_MOTOR_PWM, abs(FORE_STARBOARD_MOTOR * 2));
        }

        if (AFT_PORT_MOTOR != 0){
          if(AFT_PORT_MOTOR > 0){
           digitalWrite(AFT_PORT_MOTOR_DIGITAL, HIGH);
          } 
          else {
           digitalWrite(AFT_PORT_MOTOR_DIGITAL, LOW);
          }
         analogWrite(AFT_PORT_MOTOR_PWM, abs(AFT_PORT_MOTOR * 2));
        }

        if (AFT_STARBOARD_MOTOR != 0){
          if(AFT_STARBOARD_MOTOR > 0){
           digitalWrite(AFT_STARBOARD_MOTOR_DIGITAL, HIGH);
          } 
          else {
           digitalWrite(AFT_STARBOARD_MOTOR_DIGITAL, LOW);
          }
         analogWrite(AFT_STARBOARD_MOTOR_PWM, abs(AFT_STARBOARD_MOTOR * 2));
        }
        
      }
            
      if (identifierByte == 2) {
        //control up/down motors:
        if (powerByte > 128) {
          powerByte = powerByte - 256;
        }
        Serial.print("controlling up/down motor, power: ");
        Serial.print(powerByte, DEC);
        Serial.println();
        if (powerByte > 0){
          digitalWrite(UP_MOTOR_DIGITAL, HIGH);
          digitalWrite(UP_MOTOR_DIGITAL2, HIGH);
        } else {
          digitalWrite(UP_MOTOR_DIGITAL, LOW);
          digitalWrite(UP_MOTOR_DIGITAL2, LOW);
        }
        analogWrite(UP_MOTOR_PWM, abs(powerByte * 2));
        analogWrite(UP_MOTOR_PWM2, abs(powerByte * 2));
      }
      
      if (identifierByte == 3) {
        //control claw rotation:
        if (powerByte > 128) {
          powerByte = powerByte - 256;
        }
        Serial.print("controlling claw rotation: ");
        Serial.print(powerByte, DEC);
        Serial.println();
        if (powerByte > 0){
          stepper.move(25*MICROSTEPS);
        } else {
          stepper.move(-25*MICROSTEPS);
        }
       // singleStep();
      }
      
      if (identifierByte == 4) {
        //control claw open/close:
        clawClosed = !clawClosed; 
        digitalWrite(CLAW_PIN_DIGITAL, clawClosed ? HIGH : LOW); 
      }
      
      if (identifierByte == 5) {
        //light on/off:
        lightOn = !lightOn; 
        digitalWrite(LIGHT_PIN_DIGITAL, lightOn ? HIGH : LOW); 
      }
    }  
  }
//  if (Serial.available() > 0) {
//    incomingByte = Serial.read();
//    Serial.print("Received: ");
//    Serial.print(incomingByte, DEC);
//  }
}
/*
void singleStep() {
  if (curMillis - prevStepMillis >= millisBetweenSteps) {
    prevStepMillis = curMillis;
    digitalWrite(CLAW_STEP_DIGITAL, HIGH);
    digitalWrite(CLAW_STEP_DIGITAL, LOW);
    delay(5);
    digitalWrite(CLAW_STEP_DIGITAL, HIGH);
    digitalWrite(CLAW_STEP_DIGITAL, LOW);
    delay(5);
    digitalWrite(CLAW_STEP_DIGITAL, HIGH);
    digitalWrite(CLAW_STEP_DIGITAL, LOW);
    delay(5);
    digitalWrite(CLAW_STEP_DIGITAL, HIGH);
    digitalWrite(CLAW_STEP_DIGITAL, LOW);
    delay(5);
    digitalWrite(CLAW_STEP_DIGITAL, HIGH);
    digitalWrite(CLAW_STEP_DIGITAL, LOW);
    delay(5);
  }
 }
 */
