
# Film Camera Tester

The idea of using Arduino for measuring camera shutter timings is not new. What I've tried to do here, is to make a modular system with set of features normally offered in professional testers, at the same time to keep it's design simple and available to reproduce using off-the-shelf components.

Usually, time counting and/or light accumulating methods are used to test cameras. For time counting method you need a light sensor, a time counter and a simple light source. With such setup you can measure exposure time(s) provided by a shutter, curtains travel speed of a focal plane shutter, flash sync contact delays, etc.
For the second method you'll need a photo sensor able to measure amount of light and calibrated light source. With such setup you'll be able to measure actual exposure produced by a system "camera+lens", light meter accuracy, operation of auto-exposure cameras, etc.

This camera tester kit allows you to perform various **time based** tests and calibrations of film cameras. I continue to work on production design of a light source as well as additional sensors for light measuring modes and enclosure options.


![Film Camera Tester](assets/images/product-small-1.jpg)



### Features and characteristics


- Measures exposure time at the center of a frame of simple (leaf) and focal plane shutters of 35mm and Medium Format (6x6 or 6x45 using adapter) cameras.
- Measures timings of focal plane shutters with cutrains traveling vertically or horizontally.
  - Results can be represented in few different ways to help you detect different flaws in a shutter operation, such as curtains capping or tapering, uneven travel speed, etc.
- Measures delays between flash contact and shutter opening.
  - Automatically identifies X, M or FP modes.
- Takes series of measurements, and calculates average stats.
  - Number of tests in series can be selected in Settings menu between 3, 5, 10 or 20.
- Automatically detects type of connected Sensors, and notifies you if wrong sensor connected for a specific Mode.
- Automatically detects target speed used to calculate the error.
  - Target speed can be set to "Auto" or pre-set from a list of standard speeds.
  - You aslo can select between two sets of standard speeds (Old style like 1/50 or New style like 1/60), depending on type of shutter you are testing.
- Automatically re-calculates curtains travel time according to selected frame width or height.
- Range of measured speeds from 1s to 1/8000
- Accuracy 10µs (measured in pre-set environment, using signal generator and specialized light source)
  - Accuracy may vary during real life measurements with different light sources and cameras.


More information on [Wiki](https://github.com/srozum/film_camera_tester/wiki).


### Features in development

- Calibrated Light Source
  - Light Unit socket has pins to control brightness of light source, fan and receiving feedback signal. Few versions of Light Units were designed and work on production version is in progress.
- Measuring modes based on light accumulating method to measure exposure values and test automatic cameras
  - Socket B has I2C bus pins connected to currently not used pins. Those pins are going to be used by light measuring sensor.


### Buy or DIY

The easiest way to get the hardware is to purchase a Kit (assembled or not assembled) from my [Tindie store]().


### Big Thanks to

Similar projects which helped me to start my own

 - [https://github.com/cameradactyl/Shutter-Timer](https://github.com/cameradactyl/Shutter-Timer)
 - [https://github.com/ianonahillroad/CameraShutterTimer](https://github.com/ianonahillroad/CameraShutterTimer)
 - [https://github.com/c-s-1/shutter-tester](https://github.com/c-s-1/shutter-tester)
 - [https://github.com/sokol07/LaserShutterTester](https://github.com/sokol07/LaserShutterTester)


