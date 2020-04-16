// "hi Chitti" - a : says Hi Sir
// "describe yourself Chitti" - b : speaks the robot dialogue
//
/*
   if (mySerial.available()) {
    command = mySerial.read();
    //Serial.print("Got Command : ");
    //Serial.println(command);

    if (command == 'S') {
      acceleration = startPwm;
      _stop();
      command = -1;
      max_speed = 100;
      //Serial.println("Autonomous Mode Stopped");
    }
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
  if (distance_front < obstacle_distance || distance_left < obstacle_distance || distance_right < obstacle_distance) {
    robot_motion = random(3, 5);
    motion_changed = true;
    is_obstacle = true;
  }

  if ((millis() - timer > reverse_time) && timer_flag) {
    reverse_time = random(500, 5000);
    robot_motion = 0;
    motion_changed = true;
    timer_flag = false;
  }

  if (millis() - forward_timer > forward_timer_expiry) {
    robot_motion = random(0, 9);
    motion_changed = true;
    forward_timer_expiry = random(2000, 10000);
  }

  if (motion_changed) {
    acceleration = startPwm;
    _stop();
    delay(1000);
    timer = millis();
    speed_levels = random(0, 3);
    switch (speed_levels) {
      case 0: max_speed = 35; break;
      case 1: max_speed = 80; break;
      case 2: max_speed = 150; break;
      case 3: max_speed = 255; break;
    }
    delayVal = accTime / (max_speed - startPwm);
    motion_changed = false;
    timer_flag = true;

    if (is_obstacle) {
      rotation_dir = random(0, 1);
      if (rotation_dir) {
        rotate_left(max_speed);
      } else {
        rotate_right(max_speed);
      }
      is_obstacle = false;
      delay(random(100, 900));
    }
  }
 */ 
