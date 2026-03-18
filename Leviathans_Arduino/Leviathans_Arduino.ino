#include <PS4USB.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

//Abstract Motor class to get rid of copy-paste hassles
class Motor {
  int pwm;
  int in1;
  int in2;
public:
  //in1, in2, pwm are the pin numbers
  Motor(int pwm, int in1, int in2, bool reversed = false) {
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(pwm, OUTPUT);
    this->pwm = pwm;
    if (reversed) {
      this->in1 = in1;
      this->in2 = in2;
    } else {
      this->in1 = in2;
      this->in2 = in1;
    }
  }

  void setSpeed(int speed) {
    if (speed > 0) {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      analogWrite(pwm, abs(speed));
    } else {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      analogWrite(pwm, abs(speed));
    }
  }

  void brake() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(pwm, 0);
  }
};


USB Usb;
PS4USB PS4(&Usb);
bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

//https://www.desmos.com/calculator/mrwmjkqzsv
//0-4 back motors; m0,1 left motors top to bottom; m0,1 right motors top to bottom
//4-7 front motors; m4,5 front top motors left to right; m6,7 back top motors left to right
Motor motor0 = Motor(4,22,23);
Motor motor1 = Motor(5,24,25);
Motor motor2 = Motor(6,26,27);
Motor motor3 = Motor(7,28,29);
Motor motor4 = Motor(8,30,31);
Motor motor5 = Motor(9,32,33);
Motor motor6 = Motor(12,38,39);
Motor motor7 = Motor(13,40,41);

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 USB Library Started"));
}


void loop() {
  Usb.Task();
}
