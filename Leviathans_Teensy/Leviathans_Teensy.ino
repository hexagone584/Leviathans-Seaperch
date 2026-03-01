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

packet Inputs;
void setup() {
  //Serial Communications
  Serial.begin(9600);
  Serial1.begin(9600);
  while (!Serial && !Serial1.availableForWrite()) {}; // wait for Teensy Serial Monitor
  Serial1.flush();

  //Controller Setup
  myusb.begin();
  Serial.println(controller.available());
  while (!controller.available()) {

  };
  Serial.println("Controller's up!");

  //Debug
  Serial.println(sizeof(Inputs));
  Serial.println("Teensy up");
}



void loop() {
  // //Still Testing
  //Serial.println("HEY!!");
  // //SEND DATA
  // Serial1.println("Hi");
  // Serial1.flush();
  // delay(1000);

  // Actual Inputs
  myusb.Task();
  if (controller.available() && Serial1.availableForWrite()) {
    Inputs.Buttons = controller.getButtons();
    //Offsetted by 128 to get the signage matching up with 
    //ideally I should have the teensy do the heavy calculations but I'm not sure how it works so :p
    //https://www.desmos.com/calculator/mrwmjkqzsv

    //Multiplied by 2 because we want a range of -254-256 (256 is fine)

    Inputs.LeftJoystickX = 2 * constrain(controller.getAxis(0) - 128, -127, 127);
    Inputs.LeftJoystickY = controller.getAxis(1);//2 * constrain(controller.getAxis(1) - 128, -127, 127);
    Inputs.RightJoystickX = 2 * constrain(controller.getAxis(2) - 128, -127, 127);
    Inputs.RightJoystickY = 2 * constrain(controller.getAxis(5) - 128, -127, 127);
    Inputs.R2 = controller.getAxis(3);
    Inputs.L2 = controller.getAxis(4);
    Inputs.DPad = controller.getAxis(9);
    Serial.println(Inputs.LeftJoystickY);
    Serial1.write((uint8_t*)&Inputs, sizeof(packet));
    Serial1.flush();
  }

  //3 lines for debugging
  //Serial.print("Buttons: ");
  //Serial.print(decToBinary(controller.getButtons()));
  //Serial.print("\t");

  //technical
  //Serial.printf(" Axes: A0: %u\tA1: %u\tA2: %u\tA3: %u\tA4: %u\tA5: %u\tA9: %u\n",controller.getAxis(0),controller.getAxis(1),controller.getAxis(2),controller.getAxis(3),controller.getAxis(4),controller.getAxis(5),controller.getAxis(9));
  //abstract
  // if (controller.available()) {
  //   Serial.printf("XLeftStick: %03u YLeftStick: %03u XRightStick: %03u YRightStick: %03u L2: %03u R2: %03u DPad: %03u\n", controller.getAxis(0),controller.getAxis(1),controller.getAxis(2),controller.getAxis(5),controller.getAxis(3),controller.getAxis(4),controller.getAxis(9));
  // }
  delay(500);
}
