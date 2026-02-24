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

struct __attribute__((packed)) packet {
    int16_t LeftJoystickX;
    int16_t LeftJoystickY;
    int16_t RightJoystickX;
    int16_t RightJoystickY;
    uint8_t R2;
    uint8_t L2;
    int8_t DPad;
    uint16_t Buttons;
};
// struct __attribute__((packed)) packet {
//     int LeftJoystickX;
//     int LeftJoystickY;
//     int RightJoystickX;
//     int RightJoystickY;
//     int R2;
//     int L2;
//     int DPad;
//     uint16_t Buttons;
// };
// char* decToBinChar(int n) {
//     static char bin[13];
//     for (int i = 0; i < 13 && n>0; i++) {
//         bin[i] = 48 + n%2;
//         n /= 2;
//     }
//     return bin;
// }

packet Inputs;
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
  Serial.begin(9600);
  Serial1.begin(9600);
  while (!Serial && !Serial1) {}
  Serial.println("Arduino Up");
  Serial.println(sizeof(Inputs));
}


void loop() {
  //Test condiiton
  // if (Serial1.available()) {
  //   Serial.println("HO");
  //   //String reception = Serial1.readString();
  //   //char reception = Serial1.read();
  //   //this still carries the \n tho, but also another whitespace?? .trim() fixes it
  //   String reception = Serial1.readStringUntil("\n"); 
  //   reception.trim();
  //   Serial.println(reception);

  //   if (reception == "Hi") {
  //     Serial.println("Running Motor1");
  //   } 
  // }
  
  //reading the struct (condition), and assuming this works
  if (Serial1.available() >= sizeof(packet)) {
    Serial1.readBytes((uint8_t*)&Inputs, sizeof(packet));   
    Serial.println(Inputs.LeftJoystickX);

    //https://www.desmos.com/calculator/mrwmjkqzsv
    motor0.setSpeed(Inputs.LeftJoystickY);
    motor1.setSpeed(Inputs.LeftJoystickY);
    motor2.setSpeed(Inputs.RightJoystickY);
    motor3.setSpeed(Inputs.RightJoystickY);
    motor4.setSpeed(Inputs.R2 - Inputs.L2);
    motor5.setSpeed(Inputs.R2 - Inputs.L2);
    motor6.setSpeed(Inputs.R2 - Inputs.L2);
    motor7.setSpeed(Inputs.R2 - Inputs.L2);
  }

  delay(500);
}
