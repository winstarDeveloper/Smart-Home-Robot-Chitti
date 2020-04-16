// Program for Smart Home Robot on UNO
// v0.1 Locomotion of Robot by Three Omni wheels and Johnson motors and Serial Communication - 12/3/2018

#include <SoftwareSerial.h>
SoftwareSerial mySerial(A0, A1); // RX, TX

#define LEFT_MOTOR_DIR 5
#define LEFT_MOTOR_PWM 9

#define RIGHT_MOTOR_DIR 8
#define RIGHT_MOTOR_PWM 10

#define BACK_MOTOR_IN_1 A2
#define BACK_MOTOR_IN_2 A3
#define BACK_MOTOR_PWM 11

#define trig_R 12
#define echo_R 13

#define trig_L 3
#define echo_L 4

#define trig_F 6
#define echo_F 7

char c = -1;
String voice = "";
int max_speed = 100;
int speed_levels = 0, robot_motion = 0, obstacle_distance = 10, rotation_dir = 0;
boolean motion_changed = false, timer_flag = false, is_obstacle = false, backToCommandMode = false;
unsigned long timer = 0, forward_timer = 0, reverse_time = 1000, forward_timer_expiry = 2000;

long duration;
int distance_front, distance_left, distance_right;

char command = -1;
int startPwm = 30, acceleration = startPwm;
int accTime = 2000, delayVal = accTime / (max_speed - startPwm);

void forward(int pwm) {
  digitalWrite(LEFT_MOTOR_DIR, LOW);
  analogWrite(LEFT_MOTOR_PWM, pwm);

  digitalWrite(RIGHT_MOTOR_DIR, HIGH);
  analogWrite(RIGHT_MOTOR_PWM, pwm);

  //  digitalWrite(BACK_MOTOR_DIR, LOW);
  analogWrite(BACK_MOTOR_PWM, 0);
}

void reverse(int pwm) {
  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  analogWrite(LEFT_MOTOR_PWM, pwm);

  digitalWrite(RIGHT_MOTOR_DIR, LOW);
  analogWrite(RIGHT_MOTOR_PWM, pwm);

  //  digitalWrite(BACK_MOTOR_DIR, LOW);
  analogWrite(BACK_MOTOR_PWM, 0);
}

void left(int pwm) {
  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  analogWrite(LEFT_MOTOR_PWM, pwm);

  digitalWrite(RIGHT_MOTOR_DIR, HIGH);
  analogWrite(RIGHT_MOTOR_PWM, pwm);

  pwm = pwm * sqrt(3);
  if (pwm > max_speed) pwm = max_speed;
  digitalWrite(BACK_MOTOR_IN_1, LOW);
  digitalWrite(BACK_MOTOR_IN_2, HIGH);
  analogWrite(BACK_MOTOR_PWM, pwm);
}

void right(int pwm) {
  digitalWrite(LEFT_MOTOR_DIR, LOW);
  analogWrite(LEFT_MOTOR_PWM, pwm);

  digitalWrite(RIGHT_MOTOR_DIR, LOW);
  analogWrite(RIGHT_MOTOR_PWM, pwm);

  pwm = pwm * sqrt(3);
  if (pwm > max_speed) pwm = max_speed;
  digitalWrite(BACK_MOTOR_IN_1, HIGH);
  digitalWrite(BACK_MOTOR_IN_2, LOW);
  analogWrite(BACK_MOTOR_PWM, pwm);
}

void forward_left(int pwm) {
  //  digitalWrite(LEFT_MOTOR_DIR, LOW);
  analogWrite(LEFT_MOTOR_PWM, 0);

  digitalWrite(RIGHT_MOTOR_DIR, HIGH);
  analogWrite(RIGHT_MOTOR_PWM, pwm);

  digitalWrite(BACK_MOTOR_IN_1, LOW);
  digitalWrite(BACK_MOTOR_IN_2, HIGH);
  analogWrite(BACK_MOTOR_PWM, pwm);
}

void forward_right(int pwm) {
  digitalWrite(LEFT_MOTOR_DIR, LOW);
  analogWrite(LEFT_MOTOR_PWM, pwm);

  //  digitalWrite(RIGHT_MOTOR_DIR, HIGH);
  analogWrite(RIGHT_MOTOR_PWM, 0);

  digitalWrite(BACK_MOTOR_IN_1, HIGH);
  digitalWrite(BACK_MOTOR_IN_2, LOW);
  analogWrite(BACK_MOTOR_PWM, pwm);
}

void reverse_left(int pwm) {
  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  analogWrite(LEFT_MOTOR_PWM, pwm);

  //  digitalWrite(RIGHT_MOTOR_DIR, HIGH);
  analogWrite(RIGHT_MOTOR_PWM, 0);

  digitalWrite(BACK_MOTOR_IN_1, LOW);
  digitalWrite(BACK_MOTOR_IN_2, HIGH);
  analogWrite(BACK_MOTOR_PWM, pwm);
}

void reverse_right(int pwm) {
  //  digitalWrite(LEFT_MOTOR_DIR, LOW);
  analogWrite(LEFT_MOTOR_PWM, 0);

  digitalWrite(RIGHT_MOTOR_DIR, LOW);
  analogWrite(RIGHT_MOTOR_PWM, pwm);

  digitalWrite(BACK_MOTOR_IN_1, HIGH);
  digitalWrite(BACK_MOTOR_IN_2, LOW);
  analogWrite(BACK_MOTOR_PWM, pwm);
}

void rotate_left(int pwm) {
  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  analogWrite(LEFT_MOTOR_PWM, 60);//pwm);

  digitalWrite(RIGHT_MOTOR_DIR, HIGH);
  analogWrite(RIGHT_MOTOR_PWM, 60);//pwm);

  digitalWrite(BACK_MOTOR_IN_1, HIGH);
  digitalWrite(BACK_MOTOR_IN_2, LOW);
  analogWrite(BACK_MOTOR_PWM, 60);//pwm);
}

void rotate_right(int pwm) {
  digitalWrite(LEFT_MOTOR_DIR, LOW);
  analogWrite(LEFT_MOTOR_PWM, 60);//pwm);

  digitalWrite(RIGHT_MOTOR_DIR, LOW);
  analogWrite(RIGHT_MOTOR_PWM, 60);//pwm);

  digitalWrite(BACK_MOTOR_IN_1, LOW);
  digitalWrite(BACK_MOTOR_IN_2, HIGH);
  analogWrite(BACK_MOTOR_PWM, 60);//pwm);
}

void _stop() {
  //  digitalWrite(LEFT_MOTOR_DIR, LOW);
  analogWrite(LEFT_MOTOR_PWM, 0);

  //  digitalWrite(RIGHT_MOTOR_DIR, HIGH);
  analogWrite(RIGHT_MOTOR_PWM, 0);

  analogWrite(BACK_MOTOR_PWM, 0);
}

void freeLocomotion() {
  forward(50);
  delay(1000);
  _stop();
  delay(800);
  reverse(50);
  delay(1000);
  _stop();
  delay(800);
  left(50);
  delay(1000);
  _stop();
  delay(800);
  right(50);
  delay(1000);
  _stop();
  delay(800);
  forward_left(50);
  delay(1000);
  _stop();
  delay(800);
  reverse_right(50);
  delay(1000);
  _stop();
  delay(800);
  forward_right(50);
  delay(1000);
  _stop();
  delay(800);
  reverse_left(50);
  delay(1000);
  _stop();
  delay(800);
  rotate_left(50);
  delay(1000);
  _stop();
  delay(800);
  rotate_right(acceleration);
  _stop();
  delay(800);
}

void checkVoiceCommand() {
  while (mySerial.available()) {
    delay(10);
    char c = mySerial.read();
    if (c == '#') {
      break;
    }
    voice += c;
  }

  if (voice.length() > 0) {
    if (voice == "*hi Chitti") {
      Serial.print('a');
    } else if (voice == "*describe yourself Chitti") {
      Serial.print('b');
    } else if (voice == "*Chitti please demonstrate your locomotion") {
      Serial.print('c');
      delay(1000);
      freeLocomotion();
    } else if (voice == "*tell me the room temperature Chitti") {
      Serial.print('d');
    } else if (voice == "*Chitti go play") {
      Serial.print('e');
      robot_motion = 0;
      timer = millis();
      autonomousRun();
    } else if (voice == "*Chitti play my favourite song") {
      Serial.print('f');
    } else if (voice == "*Chitti take a photo") {
      Serial.print('g');
    } else if (voice == "*Chitti stop") {
      Serial.print('h');
      backToCommandMode = true;
    }
    voice = "";
  }
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);

  pinMode(LEFT_MOTOR_DIR, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);

  pinMode(RIGHT_MOTOR_DIR, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);

  pinMode(BACK_MOTOR_IN_1, OUTPUT);
  pinMode(BACK_MOTOR_IN_2, OUTPUT);
  pinMode(BACK_MOTOR_PWM, OUTPUT);

  pinMode(trig_R, OUTPUT);
  pinMode(echo_R, INPUT);

  pinMode(trig_L, OUTPUT);
  pinMode(echo_L, INPUT);

  pinMode(trig_F, OUTPUT);
  pinMode(echo_F, INPUT);
}

void getUltrasonicData() {
  digitalWrite(trig_L, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_L, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_L, LOW);

  duration = pulseIn(echo_L, HIGH);

  distance_left = duration * 0.034 / 2;

  //Serial.print("Distance Left: ");
  //Serial.print(distance_left);

  digitalWrite(trig_R, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_R, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_R, LOW);

  duration = pulseIn(echo_R, HIGH);

  distance_right = duration * 0.034 / 2;

  //Serial.print("\tDistance Right: ");
  //Serial.print(distance_right);

  digitalWrite(trig_F, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_F, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_F, LOW);

  duration = pulseIn(echo_F, HIGH);

  distance_front = duration * 0.034 / 2;

  //Serial.print("\tDistance Back: ");
  //Serial.println(distance_front);
}

void loop() {
  //  getUltrasonicData();
  checkVoiceCommand();

  if (mySerial.available()) {
    command = mySerial.read();
    //Serial.print("Got Command : ");
    //Serial.println(command);
  }

  if (command == 'X') {
    forward_timer = millis();
    autonomousRun();
    Serial.print('S');
  }
  else if (command == 'F') {
    if (acceleration < max_speed)
      acceleration = acceleration + 1;
    forward(acceleration);
    delay(delayVal);
    //Serial.print("Forward : ");
    //Serial.println(acceleration);
  } else if (command == 'B') {
    if (acceleration < max_speed)
      acceleration = acceleration + 1;
    reverse(acceleration);
    delay(delayVal);
    //Serial.print("Reverse : ");
    //Serial.println(acceleration);
  } else if (command == 'W') {
    if (acceleration < max_speed)
      acceleration = acceleration + 1;
    left(acceleration);
    delay(delayVal);
    //Serial.print("Left : ");
    //Serial.println(acceleration);
  } else if (command == 'U') {
    if (acceleration < max_speed)
      acceleration = acceleration + 1;
    right(acceleration);
    delay(delayVal);
    //Serial.print("Right : ");
    //Serial.println(acceleration);
  } else if (command == 'G') {
    if (acceleration < max_speed)
      acceleration = acceleration + 1;
    forward_left(acceleration);
    delay(delayVal);
    //Serial.print("Forward Left : ");
    //Serial.println(acceleration);
  } else if (command == 'I') {
    if (acceleration < max_speed)
      acceleration = acceleration + 1;
    forward_right(acceleration);
    delay(delayVal);
    //Serial.print("Forward Right: ");
    //Serial.println(acceleration);
  } else if (command == 'H') {
    if (acceleration < max_speed)
      acceleration = acceleration + 1;
    reverse_left(acceleration);
    delay(delayVal);
    //Serial.print("Reverse Left : ");
    //Serial.println(acceleration);
  } else if (command == 'J') {
    if (acceleration < max_speed)
      acceleration = acceleration + 1;
    reverse_right(acceleration);
    delay(delayVal);
    //Serial.print("Reverse Right : ");
    //Serial.println(acceleration);
  } else if (command == 'L') {
    if (acceleration < max_speed)
      acceleration = acceleration + 1;
    rotate_left(acceleration);
    delay(delayVal);
    //Serial.print("Rotate Left : ");
    //Serial.println(acceleration);
  } else if (command == 'R') {
    if (acceleration < max_speed)
      acceleration = acceleration + 1;
    rotate_right(acceleration);
    delay(delayVal);
    //Serial.print("Rotate Right : ");
    //Serial.println(acceleration);
  } else if (command == 'S' || command == 'w' || command == 'u') {
    acceleration = startPwm;
    _stop();
    command = -1;
    max_speed = 100;
    //Serial.println("Stopped");
  }
}

void autonomousRun() {
  while (true) {
    checkVoiceCommand();
    if (backToCommandMode) {
      backToCommandMode = false;
      acceleration = startPwm;
      _stop();
      command = -1;
      max_speed = 100;
      break;
    }

    switch (robot_motion) {
      case 0: forward(acceleration);
        break;
      case 1: forward_left(acceleration);
        break;
      case 2: forward_right(acceleration);
        break;
      case 3: reverse(acceleration);
        break;
      case 4: reverse_left(acceleration);
        break;
      case 5: reverse_right(acceleration);
        break;
      case 6: left(acceleration);
        break;
      case 7: right(acceleration);
        break;
      case 8: rotate_left(acceleration);
        break;
      case 9: rotate_right(acceleration);
        break;
    }

    if (acceleration < max_speed) {
      acceleration = acceleration + 1;
      delay(delayVal);
    }

    getUltrasonicData();
    obstacle_distance = random(5, 30);
    if (distance_front < obstacle_distance) {
      _stop();
      delay(500);

      reverse(60);
      delay(700);
      rotation_dir = random(0, 1);
      if (rotation_dir) {
        rotate_left(40);
      } else {
        rotate_right(40);
      }
      delay(random(100, 900));

      speed_levels = random(0, 3);
      switch (speed_levels) {
        case 0: max_speed = 35; break;
        case 1: max_speed = 80; break;
        case 2: max_speed = 150; break;
        case 3: max_speed = 255; break;
      }
    }

    if (millis() - timer > 5000) {
      timer = millis();
      robot_motion = random(1, 9);

      while (true) {
        if (millis() - timer > 1000) {
          timer = millis();
          break;
        }
        switch (robot_motion) {
          case 0: forward(acceleration);
            break;
          case 1: forward_left(acceleration);
            break;
          case 2: forward_right(acceleration);
            break;
          case 3: reverse(acceleration);
            break;
          case 4: reverse_left(acceleration);
            break;
          case 5: reverse_right(acceleration);
            break;
          case 6: left(acceleration);
            break;
          case 7: right(acceleration);
            break;
          case 8: rotate_left(acceleration);
            break;
          case 9: rotate_right(acceleration);
            break;
        }

        if (acceleration < max_speed) {
          acceleration = acceleration + 1;
          delay(delayVal);
        }
      }
    }
  }
}
