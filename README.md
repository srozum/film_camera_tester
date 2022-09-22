
# Film Camera Tester

The idea of using Arduino for measuring camera shutter timings is not new. What I've tried to do here, is to make a modular system with a set of features normally offered in professional testers, at the same time to keep its design simple and available to reproduce using off-the-shelf components.

Usually, time counting and/or light integration methods are used to test cameras. For the time counting method, you need a light sensor, a time counter, and a simple light source. With such setup you can measure exposure time(s) provided by a shutter, curtains travel speed of a focal plane shutter, flash sync contact delays, etc.
For the second method, you'll need a photosensor able to measure the amount of light and a calibrated light source. With such setup, you'll be able to measure actual exposure produced by a system "camera+lens", light meter accuracy, operation of auto-exposure cameras, etc.

This tester allows you to perform both time-based and light integration tests and calibrations of film cameras.


![Film Camera Tester](assets/images/product-small-1.jpg)



### Features and characteristics


- Measures exposure time at the center of a frame of simple (leaf) and focal plane shutters of 35mm and Medium Format (6x6 or 6x45 using adapter) cameras.
  - For leaf shutters of size #00 and #0 calculates estimated effective speed according to efficiency curve.
- Measures timings of focal plane shutters with curtains traveling vertically or horizontally.
  - Results can be represented in a few different ways to help you detect different flaws in a shutter operation, such as curtains capping or tapering, uneven travel speed, etc.
- Measures delays between flash contact and shutter opening.
  - Automatically identifies X, M, or FP modes.
- Takes a series of measurements, and calculates average stats.
  - Number of tests in series can be selected in the Settings menu between 3, 5, 10, or 20.
- Automatically detects the type of connected Sensors, and notifies you if wrong sensor is connected for a specific Mode.
- Automatically detects target speed used to calculate the error.
  - Target speed can be set to "Auto" or pre-set from a list of standard speeds.
  - You also can select between two sets of standard speeds (Old style like 1/50 or New style like 1/60), depending on the type of shutter you are testing.
- Automatically re-calculates curtains travel time according to selected frame width or height.
- Range of measured speeds from 1s to 1/8000
- Accuracy 10µs (measured in the pre-set environment, using a signal generator and specialized light source)
  - Accuracy may vary during real-life measurements with different light sources and cameras.
- Allows to check accuracy and calibrate light meters and light meters built into a cameras.
  - Light Source brightness is adjustable within 4-16EV range
  - Light Source can be switched between K12.5 and K14.03
- Measures actual exposure at the film plane, which allows to test fully automatic, aperture or shutter priority cameras.
  - Accuracy ±0.1EV within 6-14EV range, although full range 4-16EV of a Light Unit can be used for testing.
  - Resolution -0.03/+0.01EV


More information on [Wiki](https://github.com/srozum/film_camera_tester/wiki).


### Buy or DIY

The easiest way to get the hardware is to purchase a Kit (assembled or not assembled) from my [Tindie store](https://www.tindie.com/products/srozum/film-camera-tester/).


### Big Thanks to

Similar projects that helped me to start my own

 - [https://github.com/cameradactyl/Shutter-Timer](https://github.com/cameradactyl/Shutter-Timer)
 - [https://github.com/ianonahillroad/CameraShutterTimer](https://github.com/ianonahillroad/CameraShutterTimer)
 - [https://github.com/c-s-1/shutter-tester](https://github.com/c-s-1/shutter-tester)
 - [https://github.com/sokol07/LaserShutterTester](https://github.com/sokol07/LaserShutterTester)


---
This work is licensed under a [Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
