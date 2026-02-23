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

struct packet {
    uint8_t LeftJoystickX;
    uint8_t LeftJoystickY;
    uint8_t RightJoystickX;
    uint8_t RightJoystickY;
    uint8_t Joystick;
    uint8_t R2;
    uint8_t L2;
    uint8_t DPad;
    uint16_t Buttons;
};

// char* decToBinChar(int n) {
//     static char bin[13];
//     for (int i = 0; i < 13 && n>0; i++) {
//         bin[i] = 48 + n%2;
//         n /= 2;
//     }
//     return bin;
// }

packet Inputs;
Motor motor0 = Motor(4,23,22);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  while (!Serial && !Serial1) {}
  Serial.println("Arduino Up");
}


void loop() {
  //Test condiiton
  if (Serial1.available()) {
    Serial.println("HO");
    //String reception = Serial1.readString();
    //char reception = Serial1.read();
    //this still carries the \n tho, but also another whitespace?? .trim() fixes it
    String reception = Serial1.readStringUntil("\n"); 
    reception.trim();
    Serial.println(reception);

    if (reception == "Hi Arduino") {
      Serial.println("Running Motor1");
    } 
  }

  //reading the struct (condition)
  if (Serial.available() >= sizeof(Inputs)) {

    Serial.readBytes((uint8_t*)&Inputs, sizeof(Inputs));
  }

}
