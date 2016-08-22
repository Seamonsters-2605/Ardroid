# ARDROID
A generic robot controller for the Arduino101 board with an Adafruit v2 motor shield. [Here's a demo robot on Thingiverse](http://www.thingiverse.com/thing:1674749) (has an old version of the app and Arduino code).

There are two parts to this: an Android app, written with MIT App Inventor 2, serving as a remote control for the robot; and an Arduino sketch that receives signals from the controller over Bluetooth and moves motors accordingly. The app allows customization of the controls for moving motors and for driving. A rudimentary scripting system is also in progress; see the `ArdroidScript` directory.

## To do:
This is a work in progress.
- Create/edit/run scripts
- Allow stacked motor shields
