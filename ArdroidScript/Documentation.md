# ArdroidScript Documentation

## Syntax
Newlines are used to separate commands.

### Commands
Nested items represent optional modifiers that can be added in any order to the end of the command. Parentheses represent arguments. Brackets represent optional sections.
- `Script name: (name)`
- `wait (number) seconds`
- `drive (forward/back/left/right) [and (forward/back/left/right)]`
  - `(speed)`
  - `for (number) seconds`
- `turn left/right`
  - `(speed)`
  - `for (number) seconds`
- `move (device) forward/back`
  - `(speed)`
  - One of these:
    - `for (number) seconds`
    - `(rotate amount)`
- `stop [all/driving/(device)]`
  - `for (number) seconds`
- `do script (name)`
  - `(number) times`

### Speeds
- `very slow`
- `slow`
- `fast`
- `very fast`
- `at (number)%`

### Rotate amounts
- `(number) rotations`
- `(number) steps`

### Devices
- `dc motor (port)`
- `stepper motor (port)`

## Compiled commands:
  Commands are separated by spaces. Numbers can sometimes have a negative sign - this counts towards the number of digits

- `w(5 digits milliseconds)` : Wait
- `d(4 digits)` : Set drive speed, from -255 to 255
- `s(4 digits)` : Set steer speed, -255 (left) to 255 (right)
- `v(2 digit device #)(4 digits speed)` : Set speed of control. Devices above 50 are stepper motors.
- `p(2 digit device #)(3 digits speed)(7 digits steps)` : Move position of control in steps. Use negative steps to move backwards.
- `P(2 digit device #)(3 digits speed)(7 digits milli-rotations)` : Move position of control in rotations*1000 (milli-rotations).
- `r(name)` : Run script with name. Return to this script when done.
- `x` : Stop all motors
