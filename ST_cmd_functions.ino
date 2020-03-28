// Implementation of the Yaesu GS232B Protocol commands
// B              - Report elevation                                EL=eee
// C              - Report azimuth                                  AZ=aaa
// C2             - Report azimuth and elevation                    AZ=888 EL=eee
// S              - Stop all rotation
// A              - Stop azimuth rotation
// E              - Stop elevation rotation
// L              - Rotate azimuth left (CCW)
// R              - Rotate azimuth right (CW)
// D              - Rotate elevation down
// U              - Rotate elevation up
// Mxxx           - Move to azimuth
// Wxxx yyy       - Move to azimuth xxxx and elevation yyy
// X1             - Change to azimuth rotation speed setting 1
// X2             - Change to azimuth rotation speed setting 2
// X3             - Change to azimuth rotation speed setting 3
// X4             - Change to azimuth rotation speed setting 4
// O              - Azimuth offset calibration
// F              - Azimuth full scale calibration
// O2             - Elevation offset calibration
// F2             - Elevation full scale calibration
// P36            - Switch to 360 degree mode
// P45            - Switch to 450 degree mode
// Z              - Toggle north / south centered mode
// H              - Help

// Function for moving to a given EL
void cmd_moveToEL(String wantedEL){
  int ELasInt = wantedEL.toInt();

  if(ELasInt <= 90){
    if(ELasInt < pub_EL){             // EL Down    / AZ CCW
      while(ELasInt != pub_EL ){
        moveStepperOneDeg(setup_EL_PulPin, setup_EL_DirPin, false);
        pub_EL--;
      } 
    } else {
      while(ELasInt != pub_EL ){      // EL Up      / AZ CW
        moveStepperOneDeg(setup_EL_PulPin, setup_EL_DirPin, true);
        pub_EL++;
      }
    }
  }
  //moveStepperOneDeg(setup_EL_PulPin, setup_EL_DirPin, false);
  //moveStepperOneDeg(setup_EL_PulPin, setup_EL_DirPin, false);
  Serial.print("Set EL: ");
  Serial.println( ELasInt );
}

// Function for moving to a given AZ
void cmd_moveToAZ(String wantedAZ){
  int AZasInt = wantedAZ.toInt();

  // when AZ at 359° and 0 / 1 is wanted don't go around
  if(pub_AZ == 359 && AZasInt == 0){
    moveStepperOneDeg(setup_AZ_PulPin, setup_AZ_DirPin, true);
    pub_AZ = 0;
  } else
  if(pub_AZ == 0 && AZasInt == 359){
    moveStepperOneDeg(setup_AZ_PulPin, setup_AZ_DirPin, false);
    pub_AZ = 359;
  } else
  if(AZasInt <= 359){
    if(AZasInt < pub_AZ){             // AZ CCW
      while(AZasInt != pub_AZ ){
        moveStepperOneDeg(setup_AZ_PulPin, setup_AZ_DirPin, false);
        pub_AZ--;
      } 
    } else {
      while(AZasInt != pub_AZ ){      // AZ CW
        moveStepperOneDeg(setup_AZ_PulPin, setup_AZ_DirPin, true);
        pub_AZ++;
      }
    }
  }
  
  Serial.print("Set AZ: ");
  Serial.println( AZasInt );
}

// Function to stop the EL axis
void cmd_stopEL(){
  pub_stopEL = true;
}

// Function to stop the AZ axis
void cmd_stopAZ(){
  pub_stopAZ = true;
}

// Function to zero out the EL axis
void cmd_Offset_cal_EL(){
  if(digitalRead(setup_EL_endstop) == HIGH){
    // go up until out of Endstop (1°)
    while(digitalRead(setup_EL_endstop) == HIGH){
      moveStepperOneDeg(setup_EL_PulPin, setup_EL_DirPin, true);
    }
    moveStepperOneDeg(setup_EL_PulPin, setup_EL_DirPin, false);
  } else {
    // go back until Endstop (0°)
    while(digitalRead(setup_EL_endstop) == LOW){
      moveStepperOneDeg(setup_EL_PulPin, setup_EL_DirPin, false);
    }
  }
  pub_EL = 0;
  Serial.println("Calibration on EL axis done.");
}

// Function to zero out the AZ axis
void cmd_Offset_cal_AZ(){
  if(digitalRead(setup_AZ_endstop) == HIGH){
    // go up until out of Endstop (1°)
    while(digitalRead(setup_AZ_endstop) == HIGH){
      moveStepperOneDeg(setup_AZ_PulPin, setup_AZ_DirPin, true);
    }
    moveStepperOneDeg(setup_AZ_PulPin, setup_AZ_DirPin, false);
  } else {
    // go back until Endstop (0°)
    while(digitalRead(setup_AZ_endstop) == LOW){
      moveStepperOneDeg(setup_AZ_PulPin, setup_AZ_DirPin, false);
    }
  }
  pub_AZ = 0;
  Serial.println("Calibration on AZ axis done."); 
}


// FUll calibration to get STepps per 360°
// Function to calibrate the trackers EL axis
void cmd_calibrateEL(){
  int deg = 0;
  unsigned long allSteps;
  
  Serial.println("Calibrating EL 360° ...");
  
  if(digitalRead(setup_EL_endstop) == HIGH){
    Serial.println("Clearing Endstop...");
    // 20° up to clear endstop if in it
    while(deg <= 20){
      moveStepperOneDeg(setup_EL_PulPin, setup_EL_DirPin, true);
      deg++;
    }
  }

  Serial.println("Going back until hitting the Endstop....");
  // move back until endstop
  while(digitalRead(setup_EL_endstop) == LOW){
    moveStepperOneDeg(setup_EL_PulPin, setup_EL_DirPin, false);
  }

  Serial.println("Moving up until Endstop...");
  // move up until at endstop again (around)
  // 20° up to clear endstop if in it
  deg = 0;
  while(deg <= 20){
    moveStepperOneDeg(setup_EL_PulPin, setup_EL_DirPin, true);
    allSteps = allSteps + 213;
    deg++;
  }
  Serial.println(allSteps);
  while(digitalRead(setup_EL_endstop) == LOW){
    moveStepperOneStep(setup_EL_PulPin, setup_EL_DirPin, true);
    allSteps++;
  }
  Serial.println(allSteps);
  Serial.println("At Endstop...moving to the end of the Endstop...");

  // turn until at end of Endstop to get 360 degrees
  while(digitalRead(setup_EL_endstop) == HIGH){
    moveStepperOneStep(setup_EL_PulPin, setup_EL_DirPin, true);
    allSteps++;
  }


  // Report stepps needed
  Serial.print("Stepps for 360° on EL: ");
  Serial.println(allSteps);
}

// Function to calibrate the trackers AZ axis
void cmd_calibrateAZ(){
  
}


