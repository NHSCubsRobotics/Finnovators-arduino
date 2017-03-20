int incomingByte = 0;
int identifierByte = 0; // for incoming serial data
int powerByte = 0;
bool lightOn = true;
bool clawClosed = true;

int SYNC_MARKER = 128;

int LEFT_MOTOR_PWM = 2;
int RIGHT_MOTOR_PWM = 3;
int UP_MOTOR_PWM = 4;

int LEFT_MOTOR_DIGITAL = 22;
int RIGHT_MOTOR_DIGITAL = 23;
int UP_MOTOR_DIGITAL = 24;

int CLAW_PIN_DIGITAL = 50;
int LIGHT_PIN_DIGITAL = 51;

int CLAW_ROTATE_DIGITAL = 40;
int CLAW_STEP_DIGITAL = 41;

unsigned long curMillis;
unsigned long prevStepMillis = 0;
unsigned long millisBetweenSteps = 25; // milliseconds

void setup() {
  // put your setup code here, to run once:
 Serial.begin(19200);     // opens serial port, sets data rate to 9600 bp

 pinMode(LEFT_MOTOR_PWM, OUTPUT);   //opens pins for motors
 pinMode(RIGHT_MOTOR_PWM, OUTPUT);
 pinMode(UP_MOTOR_PWM, OUTPUT);
 
 pinMode(LEFT_MOTOR_DIGITAL, OUTPUT);
 pinMode(RIGHT_MOTOR_DIGITAL, OUTPUT);
 pinMode(UP_MOTOR_DIGITAL, OUTPUT);
 
 pinMode(CLAW_PIN_DIGITAL, OUTPUT);  //opens pins for claw
 pinMode(LIGHT_PIN_DIGITAL, OUTPUT);  //opens pins for lights
 
 pinMode(CLAW_ROTATE_DIGITAL, OUTPUT);  
 pinMode(CLAW_STEP_DIGITAL, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  curMillis = millis();
  
  if (Serial.available() > 2) {
    // read the incoming byte:
    incomingByte = Serial.read();
    if (incomingByte == SYNC_MARKER) {
      identifierByte = Serial.read();
      powerByte = Serial.read();
      
      if (identifierByte == 0) {
        //control left motors:
        if (powerByte > 128) {
          powerByte = powerByte - 256;
        }
        Serial.print("controlling left motor, power: ");
        Serial.print(powerByte, DEC);
        Serial.println();
        if (powerByte > 0){
          digitalWrite(LEFT_MOTOR_DIGITAL, HIGH);
        } else {
          digitalWrite(LEFT_MOTOR_DIGITAL, LOW);
        }
        analogWrite(LEFT_MOTOR_PWM, abs(powerByte * 2));
      }
      
      if (identifierByte == 1) {
        //control right motors:
        if (powerByte > 128) {
          powerByte = powerByte - 256;
        }
//        Serial.print("controlling right motor, power: ");
//        Serial.print(powerByte, DEC);
//        Serial.println();
        if (powerByte > 0){
          digitalWrite(RIGHT_MOTOR_DIGITAL, HIGH);
        } else {
          digitalWrite(RIGHT_MOTOR_DIGITAL, LOW);
        }
        analogWrite(RIGHT_MOTOR_PWM, abs(powerByte * 2));
      }
      
      if (identifierByte == 2) {
        //control up/down motors:
        if (powerByte > 128) {
          powerByte = powerByte - 256;
        }
//        Serial.print("controlling up/down motor, power: ");
//        Serial.print(powerByte, DEC);
//        Serial.println();
        if (powerByte > 0){
          digitalWrite(UP_MOTOR_DIGITAL, HIGH);
        } else {
          digitalWrite(UP_MOTOR_DIGITAL, LOW);
        }
        analogWrite(UP_MOTOR_PWM, abs(powerByte * 2));
      }
      
      if (identifierByte == 3) {
        //control claw rotation:
        if (powerByte > 128) {
          powerByte = powerByte - 256;
        }
//        Serial.print("controlling up/down motor, power: ");
//        Serial.print(powerByte, DEC);
//        Serial.println();
        if (powerByte > 0){
          digitalWrite(CLAW_ROTATE_DIGITAL, HIGH);
        } else {
          digitalWrite(CLAW_ROTATE_DIGITAL, LOW);
        }
        singleStep();
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

void singleStep() {
  if (curMillis - prevStepMillis >= millisBetweenSteps) {
    prevStepMillis = curMillis;
    digitalWrite(CLAW_STEP_DIGITAL, HIGH);
    digitalWrite(CLAW_STEP_DIGITAL, LOW);
  }
}
