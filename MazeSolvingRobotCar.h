int leftMotor1 = 9;
int leftMotor2 = 10;
int rightMotor1 = 5;
int rightMotor2 = 6;

int buttonPin = 13;
int buttonState = 0;

const int trigPin = 2;
const int echoPin = 3;
long duration = 0;
int distance = 20;

int completedLED = 12;

bool turning = false;
bool buttonHit = false;

int ir_R = 7;
int ir_L = 8;
int hit_R = HIGH;  // High = 1 = NO WALL. LOW = 0 = CLOSE TO WALL
int hit_L = HIGH;
int directionCount = 0;
int turnCount = 0;

//0 left turn, 1 right turn
int directions[4] = {};
int directionsForward[4] = {0, 1, 1, 0};
int directionsBack[4] = {1, 0, 0, 1};

void setup() {
  pinMode(ir_R, INPUT);
  pinMode(ir_L, INPUT);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  pinMode(completedLED, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT);

  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  analogWrite(leftMotor1, 0);
  analogWrite(rightMotor1, 0);
  analogWrite(leftMotor2, 0);
  analogWrite(rightMotor2, 0);
  digitalWrite(LED_BUILTIN, LOW);

  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);

  if(buttonState==1){
	buttonHit = true;
	memcpy(directions, directionsBack, sizeof directions);
	//directions[4] = {0, 1, 1, 0};
  }else{
	//directions[4] = {1, 0, 0, 1};
	memcpy(directions, directionsForward, sizeof directions);
  }

  delay(15);

  Serial.begin(9600);

}

void loop() {
  hit_R = digitalRead(ir_R);
  hit_L = digitalRead(ir_L);
  //Serial.println(hit_L);
 

  if (directionCount >= 4) {

    if (turnCount < 150) {
  	straightenOut();
  	turnCount++;
    } else {
  	digitalWrite(completedLED, 1);
  	analogWrite(rightMotor1, 0);
  	analogWrite(leftMotor2, 0);
  	analogWrite(rightMotor2, 0);
  	analogWrite(leftMotor1, 0);
    }
  } else if (turning || distance < 5) {
	turning = true;
    if (directions[directionCount] == 0) {
  	//turn left
  	analogWrite(rightMotor1, 220);
  	analogWrite(leftMotor2, 180);
    } else {
  	analogWrite(rightMotor2, 220);
  	analogWrite(leftMotor1, 180);
    }
	turnCount++;
  if (turnCount == 25) {
  	turning = false;
  	directionCount++;
  	turnCount = 0;
	}
  } else {
	straightenOut();
  }
  delay(20);
  analogWrite(rightMotor1, 0);
  analogWrite(leftMotor1, 0);
  analogWrite(rightMotor2, 0);
  analogWrite(leftMotor2, 0);
  delay(10);
  calculateDistance();
}

void straightenOut() {
  if (directionCount == 2 && !buttonHit) {
    if (!hit_L) {
  	driftRight();
    } else {
      driftLeft();
    }
  } else if (directionCount == 3 && !buttonHit) {
    if (!hit_R) {
  	driftLeft();
    } else {
      driftRight();
    }
  }
  else if (directionCount == 0 && buttonHit) {
    straight();
  }
  else if (directionCount == 2 && buttonHit) {
    if (hit_R) {
      driftRight();
    } else {
      driftLeft();
    }
  } else if (directionCount == 3 && buttonHit) {
    if (hit_L) {
      driftLeft();
    } else {
      driftRight();
    }
  }
  else {
    if (!hit_R) {
      driftLeft();
    } else if (!hit_L) {
      driftRight();
    }
    else {
      straight();
    }
  }
}

void straight() {
  analogWrite(rightMotor1, 190);
  analogWrite(leftMotor1, 140);
}

void driftRight() {
  analogWrite(rightMotor1, 150);
  analogWrite(leftMotor1, 190);
    
}

void driftLeft() {
  analogWrite(rightMotor1, 200);
  analogWrite(leftMotor1, 140);
}

void calculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
}
