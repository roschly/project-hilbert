


void simple(){
  if (sf > 250){
    while (getIR(2) > 20){
      //printf("sf: %i\n", sf);
      moveBackward(speed);
      //_delay_ms(1000);
    }

  }
  else {
    if (sf > 150){
      // turn left or right
      if (sl > sr){
        turnRightHard(500);
      }
      if (sl <= sr){
        turnLeftHard(500);
      }
    }
    else{
      if (slr > 200){
        // turn left or right
        if (sl > sr){
          turnRightSoft(speed, 200);
        }
        if (sl <= sr){
          turnLeftSoft(speed, 200);
        }
      }
      else{
        moveForward(speed);
      }
    }
  }

}


// hardcoded behaviour plan
/*
// if not on collision course AND NOT attachedToWall
if (slr <= 350 && attachedToWall == 0) {
  moveForward(speed);

  // TODO: avoid collision based on side sensors
}

// when first in front-proximity of wall
if (slr > 350 && attachedToWall == 0){
  attachedToWall = 1;
  //
  // if closer to left wall, attach to it
  //
  if (sl > sr){
    attachmentSide = 'L';
  }
  if (sl <= sr){
    attachmentSide = 'R';
  }
}

  if (attachmentSide == 'L'){
    // LEFT LEFT
    // if on collision course
    if (slr > 350){
      // set speed to 0
      moveForward(0);
      // get sensor and turn away from it
      while ( getIR(5) > 150 ){
        turnRightHard(500);
      }
    }

    // if not on collision course AND attachedToWall
    if (slr <= 350 && attachedToWall == 1) {
      moveForward(speed);

      // if in proximity of wall
      if (slw > 50 && sl > 20){
        // if too close to wall
        if (sl > 75 ){
          turnRightSoft(speed, 200);
        }
        // if too far from wall
        if (sl < 30 ){
          turnLeftSoft(speed, 200);
        }
        // if
      }
      // if NOT in proximity of wall
      if (slw <= 50){
        while (getIR(5) < 150 && getIR(1) < 80){
          turnLeftSoft(speed, 180);
        }
      }
    }
  }

  if (attachmentSide == 'R'){
    // RIGHT RIGHT
    // if on collision course
    if (slr > 350){
      // set speed to 0
      moveForward(0);
      // get sensor and turn away from it
      while ( getIR(5) > 150 ){
        turnLeftHard(500);
      }
    }

    // if not on collision course AND attachedToWall
    if (slr <= 350 && attachedToWall == 1) {
      moveForward(speed);

      // if in proximity of wall
      if (srw > 50 && sr > 20){
        // if too close to wall
        if (sr > 75 ){
          turnLeftSoft(speed, 200);
        }
        // if too far from wall
        if (sr < 30 ){
          turnRightSoft(speed, 200);
        }
        // if
      }
      // if NOT in proximity of wall
      if (srw <= 50){
        while (getIR(5) < 150 && getIR(6) < 80){
          turnRightSoft(speed, 180);
        }
      }
    }
  }

}

*/
