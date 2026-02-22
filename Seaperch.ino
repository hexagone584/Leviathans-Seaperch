#include "USBHost_t36.h"

USBHost myusb;
USBHub hub1(myusb);
USBHIDParser hid1(myusb); //apparently they use this
JoystickController controller(myusb); //instead of that

  //Bitmap:
  // 0 = nothing pressed
  // 1st place: left button (square?)
  // 2: down button (x?)
  // 3: right button (o)
  // 4: up button (triangle)
  // 5: R1
  //apparently it has joysticks that are mapped at 0, 1, 2, 3, 4, 5, 9
  //i should probably check if the serial is correct, but wtv lol
  //A4: R2
  //A3: L2
  //A0: Left Joystick (x axis, 0 is left, 255 is right)
  //A1: Left Joystick (y axis, 0 is up, 255 is down)
  //A2: Right Joystick (x axis, 0 is left, 255 is right)
  //A5: Right Joystick (y axis, 0 is up, 255 is down)
  //A9: DPad Configs (0-7 for directions, starting at up and going clockwise. 8 is nothing's pressed)


//this will read from right to left (increases base(?)) instead of left to right traditionally
struct packet {
  uint16_t Buttons; //we'll let the arduino handle the for loop. 
  uint8_t LeftJoystickX;
  uint8_t LeftJoystickY;
  uint8_t RightJoystickX;
  uint8_t RightJoystickY;
  uint8_t Joystick;
  uint8_t R2;
  uint8_t L2;
  uint8_t DPad;
};

// String decToBinary(int n) {
//   //do I need to handle this case? I'm thinking Buttons would just be null
//   //if (n == 0) { return "0"; }
//   int binaryplaces = 0;
//   String bin;
//   while (n > 0) {
//     int bit = n%2;
//     bin.append(bit);
//     n /= 2;
//     binaryplaces++;
//   }
//   for (int i = 0; i < 14-binaryplaces; i++) {
//     bin.append("0");
//   }
//   return bin;
// }

char* decToBinChar(int n) {
    static char bin[13];
    for (int i = 0; i < 13 && n>0; i++) {
        bin[i] = 48 + n%2;
        n /= 2;
    }
    return bin;
}

void setup() {
  //Serial Communications
  Serial.begin(2000000);
  Serial1.begin(9600);
  while (!Serial && !Serial1.availableForWrite()) ; // wait for Teensy Serial Monitor
  
  //Controller Setup
  myusb.begin();
  while (!controller.available());
  Serial.println("Controller's up!");

  //Debug
  Serial.println("Teensy up");
}

packet Inputs;
void loop() {

  //Still Testing
  Serial.println("HEY!!");
  //SEND DATA
  Serial1.println("Hi Arduino");
  Serial1.flush();
  delay(1000);
  //Serial.write((uint8_t*)&p, sizeof(p));

  //Actual Inputs
  //myusb.Task();
  Inputs.Buttons = controller.getButtons();
  Inputs.LeftJoystickX = controller.getAxis(0);
  Inputs.LeftJoystickY = controller.getAxis(1);
  Inputs.RightJoystickX = controller.getAxis(2);
  Inputs.RightJoystickY = controller.getAxis(5);
  Inputs.R2 = controller.getAxis(3);
  Inputs.L2 = controller.getAxis(4);
  Inputs.DPad = controller.getAxis(9);
  //Serial.write((uint8_t*)&p, sizeof(p));



  //3 lines for debugging
  //Serial.print("Buttons: ");
  //Serial.print(decToBinary(controller.getButtons()));
  //Serial.print("\t");

  //technical
  //Serial.printf(" Axes: A0: %u\tA1: %u\tA2: %u\tA3: %u\tA4: %u\tA5: %u\tA9: %u\n",controller.getAxis(0),controller.getAxis(1),controller.getAxis(2),controller.getAxis(3),controller.getAxis(4),controller.getAxis(5),controller.getAxis(9));
  //abstract
  //Serial.printf("XLeftStick: %03u YLeftStick: %03u XRightStick: %03u YRightStick: %03u L2: %03u R2: %03u DPad: %03u\n", controller.getAxis(0),controller.getAxis(1),controller.getAxis(2),controller.getAxis(5),controller.getAxis(3),controller.getAxis(4),controller.getAxis(9));
  
}