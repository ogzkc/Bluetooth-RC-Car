#include <SoftwareSerial.h>

#define MOTOR_A_IN4 4
#define MOTOR_A_IN3 5
#define MOTOR_B_ENABLE 10
#define MOTOR_A_IN2 6
#define MOTOR_A_IN1 7
#define MOTOR_A_ENABLE 9

char delimeter = '#';
byte mode = 0;
int motorSpeed = 255;
String command = "";

SoftwareSerial btSerial (2, 3);

void setup() {
  pinMode(MOTOR_A_ENABLE, OUTPUT);
  pinMode(MOTOR_A_IN1, OUTPUT);
  pinMode(MOTOR_A_IN2, OUTPUT);
  pinMode(MOTOR_B_ENABLE, OUTPUT);
  pinMode(MOTOR_A_IN3, OUTPUT);
  pinMode(MOTOR_A_IN4, OUTPUT);
  analogWrite(MOTOR_A_ENABLE, motorSpeed);
  analogWrite(MOTOR_B_ENABLE, motorSpeed);

  btSerial.begin(9600);
}

void loop() {
  if (btSerial.available()) {
    char readChar = btSerial.read();
    if (readChar != delimeter) {
      command = command + readChar;
    } else {
      doCommand();
    }
  }
  if (mode == 1) {

  }
}

void doCommand() {
  if (command == "forward_down") {
    moveForward();
  }
  if (command == "forward_up") {
    turnOffMotors();
  }
  if (command == "backward_down") {
    moveBackward();
  }
  if (command == "backward_up") {
    turnOffMotors();
  }

  if (command.indexOf("=") > -1) {
    String variableName = command.substring(0, command.indexOf("="));
    String variableValueString = command.substring(command.indexOf("=") + 1, command.length());
    if (variableName == "speed") {
      int variableValue = variableValueString.toInt();
      motorSpeed = variableValue;
      analogWrite(MOTOR_A_ENABLE, motorSpeed);
      analogWrite(MOTOR_B_ENABLE, motorSpeed);
    }
    else if (variableName == "mode") {
      int variableValue = variableValueString.toInt();
      mode = variableValue;
      turnOffMotors();
    }
    else if (variableName == "turn") {
      int variableValue = variableValueString.toInt();
      if (variableValue > 0)
        turnRight(variableValue);
      else if (variableValue < 0)
        turnLeft(abs(variableValue));
    }
  }
  command = "";
}

void turnRight(int threshold) {
  analogWrite(MOTOR_B_ENABLE, motorSpeed - threshold);
}
void turnLeft(int threshold) {
  analogWrite(MOTOR_A_ENABLE, motorSpeed - threshold);
}

void turnOffMotors() {
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_A_IN3, LOW);
  digitalWrite(MOTOR_A_IN4, LOW);
}
void moveBackward() {
  analogWrite(MOTOR_A_ENABLE, motorSpeed);
  analogWrite(MOTOR_B_ENABLE, motorSpeed);

  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, HIGH);
  digitalWrite(MOTOR_A_IN3, LOW);
  digitalWrite(MOTOR_A_IN4, HIGH);
}
void moveForward() {
  analogWrite(MOTOR_A_ENABLE, motorSpeed);
  analogWrite(MOTOR_B_ENABLE, motorSpeed);

  digitalWrite(MOTOR_A_IN1, HIGH);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_A_IN3, HIGH);
  digitalWrite(MOTOR_A_IN4, LOW);
}
