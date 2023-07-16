char command;

#define echoPinFrontSensor A0
#define trigPinFrontSensor A1
#define echoPinRightFrontSensor A2
#define trigPinRightFrontSensor A3
#define echoPinRightBackSensor A4
#define trigPinRightBackSensor A5
#define lightPin 9
#define soundPin 13
NewPing rightFrontSensor(trigPinRightFrontSensor, echoPinRightFrontSensor, 200);
NewPing rightBackSensor(trigPinRightBackSensor, echoPinRightBackSensor, 200);
NewPing frontSensor(trigPinFrontSensor, echoPinFrontSensor, 200);

void setup()
{
  bluetoothSerial.begin(9600);
  Serial.begin(9600);
  pinMode(trigPinFrontSensor, OUTPUT);
  pinMode(echoPinFrontSensor, INPUT);
  pinMode(echoPinRightFrontSensor, INPUT);
  pinMode(trigPinRightFrontSensor, OUTPUT);
  pinMode(echoPinRightBackSensor, INPUT);
  pinMode(trigPinRightBackSensor, OUTPUT);
  pinMode(lightPin, OUTPUT);
  pinMode(soundPin, OUTPUT);
  digitalWrite(soundPin, HIGH);
}

void setParkingSpeed() {
  motor1.setSpeed(100);
  motor2.setSpeed(100);
  motor3.setSpeed(100);
  motor4.setSpeed(100);
}

void setMaximumSpeed()
{
  motor1.setSpeed(220);
  motor2.setSpeed(220);
  motor3.setSpeed(200);
  motor4.setSpeed(180);
}

void setHalfSpeed()
{
  motor1.setSpeed(125);
  motor2.setSpeed(125);
  motor3.setSpeed(125);
  motor4.setSpeed(125);
}

void loop() {         
  if (bluetoothSerial.available() > 0) {
    command = bluetoothSerial.read();
    
    switch (command) {
      case 'F':
        forward(1);
        break;
      case 'B':
        back(1);
        break;
      case 'L':
        left(1);
        break;
      case 'R':
        right(1);
        break;
      case 'X':
        Park();
        break;
      case 'W':
        digitalWrite(lightPin, HIGH);
        break;
      case 'w':
        digitalWrite(lightPin, LOW);
        break;
      case 'V':
        NewTone(soundPin, 300);
        break;
      case 'v':
        noNewTone(soundPin);
        digitalWrite(soundPin, HIGH);
        break;
      case 'S':
        Stop();
        break;
    }
  }
}

void forward(int mode)
{  
  Serial.println(frontSensor.ping_cm());
  Serial.println(rightFrontSensor.ping_cm());
  Serial.println(rightBackSensor.ping_cm());

  if(mode == 1){
    setMaximumSpeed();
  }
  if (mode == 0) {
    setHalfSpeed();
  }
  if (mode == 2){
    setParkingSpeed();
  }
  if(frontSensor.ping_cm() > 3){
  Serial.println("forward");
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  }
  else
  {
   Serial.println("Collision detected");
   Stop();
  }
}

void back(int mode)
{
  if(mode == 1){
    setMaximumSpeed();
  }
  if (mode == 0) {
    setHalfSpeed();
  }
  if (mode == 2){
    setParkingSpeed();
  }
  Serial.println("backward");
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void left(int mode)
{
  if(mode == 1){
    setMaximumSpeed();
  }
  if (mode == 0) {
    setHalfSpeed();
  }
  if (mode == 2){
    setParkingSpeed();
  }
  Serial.println("left");
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void right(int mode)
{ 
   if(mode == 1){
    setMaximumSpeed();
  }
  if (mode == 0) {
    setHalfSpeed();
  }
  if (mode == 2){
    setParkingSpeed();
  }
  Serial.println("right");
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}
void rotateLeft45Degrees()
{
  left(2);
  delay(600);
  Stop();
}

void rotateRight45Degrees()
{
  right(2);
  delay(600);
  Stop();
}

void backParking() {
  forward(2);
  delay(600);
  Stop();
  delay(1000);
  rotateLeft45Degrees();
  delay(1000);
  back(2);
  delay(800);
  Stop();
  delay(1000);
  rotateRight45Degrees();
}

void frontParking() {
  back(2);
  delay(550);
  Stop();
  delay(1000);
  rotateRight45Degrees();
  delay(1000);
  forward(2);
  delay(800);
  Stop();
  delay(1000);
  rotateLeft45Degrees();
}

void Park()
{
  boolean searching = true;

  int rightFrontDistance = rightFrontSensor.ping_cm();
  int rightBackDistance = rightBackSensor.ping_cm();
  int frontDistance = frontSensor.ping_cm();

  if (rightFrontDistance > 20 && rightBackDistance > 20) {
    searching = false;
    if (frontDistance <= 15) {
      frontParking();
    } else {
      backParking();
    }
  } else {
    if (frontDistance <= 6){
      searching = false;
      return;
    } else {
    forward(2);
    }
  }

  while(searching) {
    rightFrontDistance = rightFrontSensor.ping_cm();
    rightBackDistance = rightBackSensor.ping_cm();

    if (rightFrontDistance > 20 && rightBackDistance > 20) {
      Stop();
      searching = false;
      delay(1000);
      frontDistance = frontSensor.ping_cm();
      if (frontDistance <= 15) {
        frontParking();
      } else {
         backParking();
      }
    }

    if (frontDistance <= 6) {
      Stop();
      searching = false;
    }    
  }
}

void Stop()
{
  motor1.setSpeed(0);
  motor1.run(RELEASE);
  motor2.setSpeed(0);
  motor2.run(RELEASE);
  motor3.setSpeed(0);
  motor3.run(RELEASE);
  motor4.setSpeed(0);
  motor4.run(RELEASE);
}
