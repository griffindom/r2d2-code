#include <AccelStepper.h>

#define speakerPin 13
#define ledPin 12
#define echoPin 25
#define trigPin 23
#define FULLSTEP 4
#define motorPin1 5
#define motorPin2 6

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
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  randomSeed(analogRead(0));
  myStepper.setMaxSpeed(5000.0);
  myStepper.setCurrentPosition(0);
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
      speedControl(data);
    default:
      break;
  }

  if (forceMode) {
    useForce();
  }

  int pace = int(motion[0] * 255);
  if (pace > 0) {
    analogWrite(motorPin1, pace);
  } else if (pace < 0) {
    analogWrite(motorPin2, pace);
  } else {
    analogWrite(motorPin1, 0);
    analogWrite(motorPin2, 0);
  }
  
//  Serial.print("[");
//  Serial.print(motion[0]);
//  Serial.print(", ");
//  Serial.print(motion[1]);
//  Serial.print("]");
//  Serial.print("\n");
}

void speedControl(String cmd) {
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
    motion[0] = 1.0;
  } else if (distance > 10 && distance < 50) {
    motion[0] = -1.0;
  } else {
    motion[0] = 0.0;
    Serial.println("No force detected.");
  }
}

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
