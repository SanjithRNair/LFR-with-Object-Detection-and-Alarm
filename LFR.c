// ===============================
// Optimized Line Follower Robot
// With Smooth Turns + Anti-Sway
// ===============================

// IR sensor pins
#define LEFT_IR  12
#define RIGHT_IR 11

// Motor driver pins (L298N)
#define ENA 6     // Right motor speed
#define IN1 7
#define IN2 8

#define ENB 5     // Left motor speed
#define IN3 9
#define IN4 10

// ----- Motor Speeds -----
int baseSpeedLeft  = 155;   // increase if left is weak
int baseSpeedRight = 145;   // increase if right is weak

// Smoothing filter
int smooth(int currentValue, int &lastValue) {
  if (currentValue == lastValue) return currentValue;
  lastValue = currentValue;
  delay(5);
  return currentValue;
}

int lastLeft = 1, lastRight = 1;

void setup() {
  Serial.begin(9600);

  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();
}

// ===============================
// MAIN LOOP
// ===============================
void loop() {

  int leftRaw = digitalRead(LEFT_IR);
  int rightRaw = digitalRead(RIGHT_IR);

  // Filter flicker
  int leftValue = smooth(leftRaw, lastLeft);
  int rightValue = smooth(rightRaw, lastRight);

  Serial.print("L: ");
  Serial.print(leftValue);
  Serial.print("  R: ");
  Serial.println(rightValue);

  // HIGH = WHITE, LOW = BLACK

  // Both on the line → go straight
  if (leftValue == HIGH && rightValue == HIGH) {
    moveForward();
  }
  // Need to steer left
  else if (leftValue == HIGH && rightValue == LOW) {
    softLeft();
  }
  // Need to steer right
  else if (leftValue == LOW && rightValue == HIGH) {
    softRight();
  }
  // Lost the line → keep going straight instead of stopping
  else {
    moveForward();
  }
}


// ================== MOTOR CONTROL ===================

void moveForward() {
  analogWrite(ENA, baseSpeedRight);
  analogWrite(ENB, baseSpeedLeft);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Smooth left
void softLeft() {
  analogWrite(ENA, baseSpeedRight + 30);
  analogWrite(ENB, baseSpeedLeft - 40);
}

// Smooth right
void softRight() {
  analogWrite(ENA, baseSpeedRight - 40);
  analogWrite(ENB, baseSpeedLeft + 30);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
