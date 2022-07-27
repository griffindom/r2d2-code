#include <AccelStepper.h>

#define speakerPin 12
#define ledPin 13
#define echoPin 25
#define trigPin 23
#define FULLSTEP 4
#define motorRPin1 48
#define motorRPin2 49
#define motorRPower 6
#define motorLPin1 47
#define motorLPin2 46
#define motorLPower 5

float motion[] = {0, 0};
int action;
String data;
long duration;
int distance;
boolean forceMode = false;

AccelStepper myStepper(FULLSTEP, 8, 10, 9, 11);

void setup() {
  Serial.begin(9600);
  pinMode(speakerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(motorRPower, OUTPUT);
  pinMode(motorLPower, OUTPUT);
  pinMode (motorRPin1, OUTPUT);
  pinMode (motorRPin2, OUTPUT);
  pinMode (motorLPin1, OUTPUT);
  pinMode (motorLPin2, OUTPUT);
  randomSeed(analogRead(0));
  myStepper.setMaxSpeed(5000.0);
  myStepper.setCurrentPosition(0);
  Serial.println("Starting R2-D2 Program");
}

void loop() {
  action = 0;
  if (Serial.available() > 0) {
    data = Serial.readStringUntil('\n');
    action = data.substring(0, 1).toInt();
    data = data.substring(3, data.length());
  }

  switch (action) {
    case 1:
      speak();
      break;
    case 2:
      toggleForce();
      break;
    case 3:
      lookLeft();
      break;
    case 4:
      lookRight();
      break;
    case 5:
      motionControl(data);
      break;
    default:
      break;
  }

  if (forceMode) {
    useForce();
  }

  moveR2();
}

// Speed control functions
// forward = y pos
// back = y neg
// left = x pos
// right = x neg
void moveR2() {
  int xPace = int(motion[0] * 255 * 0.4) / 2;
  int yPace = int(motion[1] * 255 * 0.6) / 2;
  if (xPace > 0) { // turning left
    if (yPace > 0) { // forward left
      analogWrite(motorRPower, yPace + xPace);
      analogWrite(motorLPower, yPace);
      rForward();
      lForward();
    } else if (yPace < 0) { // back left
      analogWrite(motorRPower, (-1 * yPace) + xPace);
      analogWrite(motorLPower, -1 * yPace);
      rBack();
      lBack();
    } else { // stationary left
      analogWrite(motorRPower, xPace);
      analogWrite(motorLPower, xPace);
      rForward();
      lBack();
    }
  } else if (xPace < 0) { // turning right
    if (yPace > 0) { // forward right
      analogWrite(motorRPower, yPace);
      analogWrite(motorLPower, yPace + (-1 * xPace));
      rForward();
      lForward();
    } else if (yPace < 0) { // back right
      analogWrite(motorRPower, -1 * yPace);
      analogWrite(motorLPower, -1 * (yPace + xPace));
      rBack();
      lBack();
    } else { // stationary right
      analogWrite(motorLPower, -1 * xPace);
      analogWrite(motorRPower, -1 * xPace);
      rBack();
      lForward();
    }
  } else { // straight
    if (yPace > 0) { // forward
      analogWrite(motorRPower, yPace);
      analogWrite(motorLPower, yPace);
      rForward();
      lForward();
    } else if (yPace < 0) { // back
      analogWrite(motorRPower, -1 * yPace);
      analogWrite(motorLPower, -1 * yPace);
      rBack();
      lBack();
    } else { // stationary
      analogWrite(motorRPower, 0);
      analogWrite(motorLPower, 0);
    }
  }
}

void rForward() {
  digitalWrite(motorRPin1, LOW);
  digitalWrite(motorRPin2, HIGH);
}

void rBack() {
  digitalWrite(motorRPin1, HIGH);
  digitalWrite(motorRPin2, LOW);
}

void lForward() {
  digitalWrite(motorLPin1, LOW);
  digitalWrite(motorLPin2, HIGH);
}

void lBack() {
  digitalWrite(motorLPin1, HIGH);
  digitalWrite(motorLPin2, LOW);
}

void motionControl(String cmd) {
  if (!forceMode) {
    int left = cmd.indexOf('[');
    int split = cmd.indexOf(',');
    int right = cmd.indexOf(']');
    float xSpeed = cmd.substring(left + 1, split).toFloat();
    float ySpeed = cmd.substring(split + 2, right).toFloat();
    motion[0] = xSpeed;
    motion[1] = ySpeed;
//    Serial.print("[");
//    Serial.print(motion[0]);
//    Serial.print(", ");
//    Serial.print(motion[1]);
//    Serial.print("]");
//    Serial.print("\n");
    } else {
      Serial.println("Force mode active.");
    }
}


// Ultrasonic sensor functions
void toggleForce() {
  if (forceMode) {
    forceMode = false;
    Serial.println("Disabling the force"); 
  } else {
    forceMode = true;
    Serial.println("Activating the force");
  }
}

void useForce() {
  Serial.println("Sensing the force");
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back) - in cm

  if (distance >= 0 && distance <= 10) {
    motion[1] = 0.5;
  } else if (distance > 10 && distance < 50) {
    motion[1] = -0.5;
  } else {
    motion[1] = 0.0;
    Serial.println("No force detected.");
  }
}


// Speak functions
void phrase1() {
    
    int k = random(1000,2000);
    digitalWrite(ledPin, HIGH);
    for (int i = 0; i <=  random(100,2000); i++){
        
        tone(speakerPin, k+(-i*2));          
        delay(random(.9,2));             
    } 
    digitalWrite(ledPin, LOW);   
    for (int i = 0; i <= random(100,1000); i++){
        
        tone(speakerPin, k + (i * 10));          
        delay(random(.9,2));             
    } 
}

void phrase2() {
    
    int k = random(1000,2000);
    digitalWrite(ledPin, HIGH);  
    for (int i = 0; i <= random(100,2000); i++){
        
        tone(speakerPin, k+(i*2));          
        delay(random(.9,2));             
    } 
    digitalWrite(ledPin, LOW);   
    for (int i = 0; i <= random(100,1000); i++){
        
        tone(speakerPin, k + (-i * 10));          
        delay(random(.9,2));             
    } 
}

void speak() {
    Serial.println("Speaking");
    int K = 2000;
    switch (random(1,7)) {
        
        case 1:phrase1(); break;
        case 2:phrase2(); break;
        case 3:phrase1(); phrase2(); break;
        case 4:phrase1(); phrase2(); phrase1();break;
        case 5:phrase1(); phrase2(); phrase1(); phrase2(); phrase1();break;
        case 6:phrase2(); phrase1(); phrase2(); break;
    }
    for (int i = 0; i <= random(3, 9); i++){
        
        digitalWrite(ledPin, HIGH);  
        tone(speakerPin, K + random(-1700, 2000));          
        delay(random(70, 170));  
        digitalWrite(ledPin, LOW);           
        noTone(speakerPin);         
        delay(random(0, 30));             
    } 
    noTone(speakerPin);         
}

// Look functions
void lookRight() {
  myStepper.setCurrentPosition(0);
  myStepper.moveTo(500);
  myStepper.setSpeed(300);
  myStepper.runSpeedToPosition();
  
  while (myStepper.distanceToGo() != 0) {
    myStepper.runSpeedToPosition();
  }
  speak();

  myStepper.setCurrentPosition(0);
  myStepper.moveTo(-500);
  myStepper.setSpeed(300);
  myStepper.runSpeedToPosition();

  while (myStepper.distanceToGo() != 0) {
    myStepper.runSpeedToPosition();
  }
}


void lookLeft() {
  myStepper.setCurrentPosition(0);
  myStepper.moveTo(-500);
  myStepper.setSpeed(300);
  myStepper.runSpeedToPosition();
  
  while (myStepper.distanceToGo() != 0) {
    myStepper.runSpeedToPosition();
  }
  speak();

  myStepper.setCurrentPosition(0);
  myStepper.moveTo(500);
  myStepper.setSpeed(300);
  myStepper.runSpeedToPosition();

  while (myStepper.distanceToGo() != 0) {
    myStepper.runSpeedToPosition();
  }
}
