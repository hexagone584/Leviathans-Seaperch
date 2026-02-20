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
String decToBinary(int n) {
  int binaryplaces = 0;
  String bin;
  while (n > 0) {
    int bit = n%2;
    bin.append(bit);
    n /= 2;
    binaryplaces++;
  }
  for (int i = 0; i < 14-binaryplaces; i++) {
    bin.append("0");
  }
  return bin;
}

void setup() {
  Serial.begin(2000000);
  while (!Serial) ; // wait for Teensy Serial Monitor
  Serial.flush();
  myusb.begin();
  while (!controller.available());
  Serial.println("Controller's up!");

}

void loop() {
  myusb.Task();
  //3 lines for debugging
  Serial.print("Buttons: ");
  Serial.print(decToBinary(controller.getButtons()));
  Serial.print("\t");

  //technical
  //Serial.printf(" Axes: A0: %u\tA1: %u\tA2: %u\tA3: %u\tA4: %u\tA5: %u\tA9: %u\n",controller.getAxis(0),controller.getAxis(1),controller.getAxis(2),controller.getAxis(3),controller.getAxis(4),controller.getAxis(5),controller.getAxis(9));
  //abstract
  Serial.printf("XLeftStick: %03u YLeftStick: %03u XRightStick: %03u YRightStick: %03u L2: %03u R2: %03u DPad: %03u\n", controller.getAxis(0),controller.getAxis(1),controller.getAxis(2),controller.getAxis(5),controller.getAxis(3),controller.getAxis(4),controller.getAxis(9));
  
}