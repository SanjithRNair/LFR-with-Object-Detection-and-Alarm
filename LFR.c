// Line Follower + Ultrasonic + Buzzer
// IR FIXED: HIGH = BLACK, LOW = WHITE
// IR sensor pins
#define LEFT_IR  12
#define RIGHT_IR 11
// Ultrasonic sensor pins
#define TRIG 2
#define ECHO 3
// Buzzer
#define BUZZER 4
// Motor driver pins (L298N)
#define ENA 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10
#define ENB 5
// ---------- SPEED TUNING (SLOWED DOWN) ----------
const uint8_t BASE_SPEED  = 78;   // slower forward speed
const uint8_t START_BOOST = 90;   // gentler start
const uint16_t BOOST_MS   = 80;    // shorter boost
const uint16_t DIST_ALERT_CM = 15;
// ultrasonic debounce
const uint8_t DIST_CONFIRM_COUNT = 2;
void setup() {
  Serial.begin(9600);

  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  stopMotorsForce();
}

void loop() {
  long distance = getDistance();
  static uint8_t consecutive = 0;

  // ---------- OBSTACLE DETECTION ----------
  if (distance > 0 && distance <= DIST_ALERT_CM) {
    consecutive++;
  } else {
    consecutive = 0;
  }

  if (consecutive >= DIST_CONFIRM_COUNT) {
    stopMotorsForce();
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
    return;
  }

  digitalWrite(BUZZER, LOW);

  // Read IR sensors
  int leftValue  = digitalRead(LEFT_IR);
  int rightValue = digitalRead(RIGHT_IR);

  Serial.print("L:");
  Serial.print(leftValue);
  Serial.print(" R:");
  Serial.print(rightValue);
  Serial.print(" D:");
  Serial.println(distance);

  // ============================================
  // IR LOGIC (YOUR SENSORS)
  // HIGH = BLACK, LOW = WHITE
  // ============================================

  // Both BLACK → forward
  if (leftValue == HIGH && rightValue == HIGH) {
    moveForward();
  }
  // Left BLACK, Right WHITE → turn left
  else if (leftValue == HIGH && rightValue == LOW) {
    turnLeftSoft();
  }
  // Left WHITE, Right BLACK → turn right
  else if (leftValue == LOW && rightValue == HIGH) {
    turnRightSoft();
  }
  // Both WHITE → stop
  else {
    stopMotorsForce();
  }

  delay(10);
}

// ===================== MOTOR FUNCTIONS =====================
void applyStartBoost(uint8_t pwmPin, uint8_t pwmVal) {
  analogWrite(pwmPin, START_BOOST);
  delay(BOOST_MS);
  analogWrite(pwmPin, pwmVal);
}

void moveForward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  applyStartBoost(ENA, BASE_SPEED);
  applyStartBoost(ENB, BASE_SPEED);
}

void turnLeftSoft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, BASE_SPEED + 50);
  analogWrite(ENB, BASE_SPEED - 70);
}

void turnRightSoft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, BASE_SPEED - 70);
  analogWrite(ENB, BASE_SPEED + 50);
}
void stopMotorsForce() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
// ================== ULTRASONIC ==================
long getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW)
  long duration = pulseIn(ECHO, HIGH, 25000);
  if (duration == 0) return -1;
  long distance = duration * 0.034 / 2;
  return distance;
}
