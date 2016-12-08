# ARDROID
A generic robot controller for the Arduino101 board with an Adafruit v2 motor shield. [Here's a demo robot on Thingiverse](http://www.thingiverse.com/thing:1674749) (has an old version of the app and Arduino code).

There are two parts to this: an Android app, written with MIT App Inventor 2, serving as a remote control for the robot; and an Arduino sketch that receives signals from the controller over Bluetooth and moves motors accordingly. The app allows customization of the controls for moving motors and for driving. It also has a rudimentary scripting language; see the `ArdroidScript` directory.

## Arduino Sketch Installation Instructions
- Download the [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- Download the `ardroid.ino` file from this repository. It must be in a folder called `ardroid` (all lowercase).
- Go to Tools Menu > Board > Boards Manager... and install "Intel Curie Boards."
- Go to Sketch > Include Library > Manage Libraries... and install "Adafruit Motor Shield V2 Library."
- If you are on Linux, open a terminal and run:
    ```
    cd ~/.arduino15/packages/Intel/tools/arduino101load/1.6.9+1.28/scripts
    sudo ./create_dfu_udev_rule
    sudo usermod -a -G dialout YOUR_USER_NAME
    ```
    and log out and log back in.
- Plug in the Arduino, select the correct port in the Tools > Port menu, and upload your sketch by clicking the Upload button (right arrow in the top left corner).
