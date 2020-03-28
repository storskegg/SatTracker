// Abstraction of stepper motor definitions.
//
// Originally this project was built around NEMA23 steppers, which come in a variety of flavors. This is
// where I'll start.

#ifndef __STEPPER_MOTOR_DEFS
  #define __STEPPER_MOTOR_DEFS

  #define __T_PULLEY_MOTOR 20
  #define __T_PULLEY_WORM 40

  #define __T_GEAR_WORM 1 // per the worm's STL file
  #define __T_GEAR_SPUR 27 // per the spur gear's STL file

  // Supported Motors:
  //   NEMA23 (Std 1.8deg step): __NEMA23_STD
  //   NEMA23 (Nonstd 0.9deg step): __NEMA23_09
  #define __NEMA23_STD

  // TODO: Start adding metrics for various stepper motors
  #define __STEP_DEGREES 1.8 // arbitrary default
  #ifdef __NEMA23_STD
      #undef __STEP_DEGREES
      #define __STEP_DEGREES 1.8
  #endif

  #ifdef __NEMA23_09
      #undef __STEP_DEGREES
      #define __STEP_DEGREES 0.9
  #endif

  // Pulley ratio is Tw / Tm
  #define RATIO_PULLEY_DRIVE __T_PULLEY_WORM / __T_PULLEY_MOTOR
  // Worm ratio is Ts / Tw
  #define RATIO_WORM_DRIVE  __T_GEAR_SPUR / __T_GEAR_WORM
  // Motor steps per full rotation is 360 / step angle
  #define STEPS_360 360 / __STEP_DEGREES

  //#define setup_EL_steps 76800              // Steps for 360 degrees on the Elevation axis
  //#define setup_AZ_steps 76800              // Steps for 360 degrees on the Azimuth axis

  // Motor Steps * pulley ratio * worm ratio
  // #define setup_steps STEPS_360 * RATIO_PULLEY_DRIVE * RATIO_WORM_DRIVE // this doesn't match the 86000 below
  #define setup_steps 86000                   // Steps for 360 degrees on each axis
#endif
