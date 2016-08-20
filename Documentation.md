# Ardroid app documentation

## Bluetooth LE Characteristics
- 01 - 04: DC motors
  - -255 - 255: Set speed
  - Set 01 to 256 for All Off signal.
- 05 - 06: Stepper motors
  - 0 - 65535: Set max speed in RPM
  - -65536 - 0: Set steps per rotation
- 07 - 08: Stepper motors
  - -65535 - 65535: Set position, 0 centered
- 09 - 0A: Stepper motors
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
