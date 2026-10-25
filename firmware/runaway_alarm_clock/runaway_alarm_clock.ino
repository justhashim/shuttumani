// Runaway Alarm Clock: one-motor obstacle response for an ESP32.
// Invalid or out-of-range distance readings stop the motor for safety.

#define ENA 25
#define IN1 26
#define IN2 27

#define TRIG_PIN 32
#define ECHO_PIN 33

const int MOTOR_SPEED = 180;
const float STOP_DISTANCE = 20.0;
const float MAX_DISTANCE = 100.0;

float readDistance();
void forward();
void backward();
void stopMotor();

void setup() {
  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);
  stopMotor();

  Serial.println("Runaway Alarm Clock - Obstacle Test");
}

void loop() {
  const float distance = readDistance();

  if (distance < 0) {
    Serial.println("No echo or out of range - stopping");
    stopMotor();
    delay(300);
    return;
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance <= STOP_DISTANCE) {
    Serial.println("Obstacle detected! Reversing.");

    stopMotor();
    delay(200);

    backward();
    delay(500);

    stopMotor();
    delay(500);
  } else {
    forward();
  }

  delay(100);
}

float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(3);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long duration = pulseIn(ECHO_PIN, HIGH, 6000);

  if (duration == 0) {
    return -1;
  }

  const float distance = duration * 0.0343 / 2.0;

  if (distance < 2.0 || distance > MAX_DISTANCE) {
    return -1;
  }

  return distance;
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, MOTOR_SPEED);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, MOTOR_SPEED);
}

void stopMotor() {
  analogWrite(ENA, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}
