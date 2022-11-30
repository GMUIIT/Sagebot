class TwoWayMotor {
  private:
    int dirPinA, dirPinB, enPin;
  public:
    TwoWayMotor(int dirPinA, int dirPinB, int enPin):
      dirPinA(dirPinA), dirPinB(dirPinB), enPin(enPin) {
      }

      void init() {
        pinMode(dirPinA, OUTPUT);
        pinMode(dirPinB, OUTPUT);
        pinMode(enPin, OUTPUT);
        digitalWrite(dirPinA, LOW);
        digitalWrite(dirPinB, LOW);
        digitalWrite(enPin, LOW);
      }

      void write(int amount){
        if (amount < -255 || amount > 255) {
          return;
        }
        if (amount < 0) {
          digitalWrite(dirPinA, HIGH);
          digitalWrite(dirPinB, LOW);
          amount *= -1;
        } else if (amount > 0) {
          digitalWrite(dirPinA, LOW);
          digitalWrite(dirPinB, HIGH);
        } else {
          digitalWrite(dirPinA, LOW);
          digitalWrite(dirPinB, LOW);
        }
        analogWrite(enPin, amount);
      }
};sj fnskdfj kdsfnjkds jknsdf
