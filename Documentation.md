# Ardroid app documentation

## Bluetooth LE Characteristics
Service UUID: `19B20000-E8F2-537E-4F6C-D104768A1214`

Characteristic UUID's (16-bit): `00XX`. First digit is the shield number. Second digit is:

- 1 - 4: DC motors
  - -255 - 255: Set speed
  - Set 01 to 256 for All Off signal.
- 5 - 6: Stepper motors
  - 0 - 65535: Set max speed in RPM
  - -65536 - 0: Set steps per rotation
- 7 - 8: Stepper motors
  - -65535 - 65535: Set position, 0 centered
- 9 - A: Stepper motors
  - -65535 - 65535: Set speed in steps per second
  
## TinyDB Tags
TinyDB is used internally by the app to store data.
Nested tags represent namespaces separated by `.`s, for example, this:
- `tag1`
- `tag2.`
  - `tag3`
  - `tag4`

describes 3 tags, called `tag1`, `tag2.tag3` and `tag2.tag4`.

- `addr`: Address of selected device. Temporary.
- `selectedControl`: Index of selected control to configure, starting at 1. Temporary.
- `selectedScript`: Script number to edit. Temporary.
- `config.`
  - `driveMode`: Can be `None`, `Tank`, or `Steer`. Presence of this tag is used to determine if settings screen has been opened.
  - `tank.`
    - `l1port`: Port number for left motor 1
    - `l1reverse`: Boolean: reverse direction?
    - `l2port`
    - `l2reverse`
    - `r1port`
    - `r1reverse`
    - `r2port`
    - `r2reverse`
    - `scale`: Max speed scale, 0 to 1
  - `steer.`
    - `d1port`: Drive motors
    - `d1reverse`
    - `d2port`
    - `d2reverse`
    - `dScale`
    - `s1port`: Steering stepper motor (only 1)
    - `s1reverse`
    - `sScale`: Max speed in RPMs.
    - `sSteps`: Number of steps per rotation
    - `sRotations`: Rotations to fully steer to the right or left
  - `numControls`: Number of custom controls, usually 4.
  - `control1.`, `control2.`, etc.
    - `name`: Custom control name
    - `mode`: Motor type, either `DC`, `Stepper position`, or `Stepper speed`
    - `m1port`: First motor
    - `m1reverse`
    - `m2port`
    - `m2reverse`
    - `dc.`
      - `scale`: Max speed scale, 0 to 1
    - `stepper.`
      - `steps`: Number of steps per rotation
      - `scale`: Max speed in RPMs.
      - `position.`
        - `fRotations`: Rotations available forward from starting position
        - `bRotations`: Backward
  - `scripts`: List of script numbers. Scripts are assigned a random number when they are created.
  - `script1234.`, `script5678.`, etc. for each script number
    - `source`: The source code of the script, as a CSV list of lines
    - `name`: The name of the script
    - `compiled`: The compiled form of the script

## App Screens
App Inventor divides apps into Screens, which have limited communication between each other (temporary TinyDB tags are used for this).
- `Screen1`: The main, initial app screen, with 3 buttons to Scan for Bots, adjust Bot Settings, and get Help.
- `HelpScreen`: Has information about the app.
- `ControlScreen`: The remote control screen for the bot. On startup it creates custom controls, connects to the bot and sends settings over bluetooth. It has sliders and accelerometer control for driving and moving motors, and is able to run scripts.
- `ConfigScreen`: The main settings screen, with options to configure drive mode and driving options, and to edit custom controls or scripts using other screens.
- `ControlConfigScreen`: Settings for the custom control specified by the tag `selectedControl`.
- `ScriptScreen`: The script editor. Loads and saves from the script specified by the tag `selectedScript`. Before closing the script is compiled - if there are errors, the screen cannot be closed until it is fixed and compiled correctly.
- `ResetScreen`: The Developer Tools screen. To open this screen, go to the Help page, long-press on the Close button, then scroll to the bottom of the page and there will be a "Dev Tools" button. Press this button and choose "Yes." Options on this screen:
  - `Reset App`: Delete all TinyDB tags. This will remove all bot configuration.
  - `Open Screen`: Enter the name of a screen to open.
  - `Get All Tags`: Display all TinyDB tags and values in a list below. The list will not be updated until this button is pressed again. Tapping the list does nothing.
  - `Edit Tag`: Enter the new value for a tag.
