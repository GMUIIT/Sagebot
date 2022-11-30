#ifndef TWO_WAY_MOTOR_H
#define TWO_WAY_MOTOR_H

/**
 * The TwoWayMotor class allows for control of a DC motor
 * H-bridge/driver board.  The constructor accepts the pin
 * numbers of the two direction and the enable pin, and the
 * write() function accepts a value from -255 to 255 as the
 * motor speed to output.  Note that you must call .init()
 * on the objects before using them, in order to setup their
 * resources (PWM and pins).
 */
class TwoWayMotor {
  private:
    int dirPinA, dirPinB, enPin;
  public:
    TwoWayMotor(int dirPinA, int dirPinB, int enPin):
      dirPinA(dirPinA), dirPinB(dirPinB), enPin(enPin) {}

      void init();
      void write(int amount);
};

#endif
