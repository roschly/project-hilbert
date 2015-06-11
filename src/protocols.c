


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
