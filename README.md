# Background:
## OpenCV:
OpenCV is an open-source library for computer vision and machine learning. It provides a wide range of tools for image and video processing, including feature detection, object detection, and image segmentation. It also includes a number of machine learning algorithms for tasks such as classification and regression. OpenCV can be used in C++, Python, and Java and runs on a variety of platforms including Windows, Linux, and MacOS. It is widely used in both academia and industry for tasks such as image recognition, object tracking, and autonomous navigation.

## Multi-threading:
In C++, multithreading refers to the ability of a program to execute multiple threads, or sequences of instructions, simultaneously. This allows a program to perform multiple tasks at the same time, increasing its overall efficiency and responsiveness. C++ provides the std::thread class for creating and managing threads. The std::thread class provides a constructor that takes a function or callable object, which is executed as a separate thread when the thread object is created.

## Human-in-the-Loop Weapon System:
These are weapons systems that involve human decision-making at some point in the engagement process, typically, and in this case this means that a human operator must give final approval before a weapon is fired.

Human in the loop weapons are used to ensure that a weapon is only used in the appropriate circumstances, and to prevent accidental or unintended engagements. This concept is used to balance the speed and efficiency of autonomous systems with the accountability and responsibility of human operators.

## PID Control System:
A PID (Proportional-Integral-Derivative) control system is a control loop feedback mechanism that is widely used in industrial control systems and other applications that require continuously modulated control.

A PID controller continuously calculates an error value as the difference between a desired setpoint and a measured process variable, and applies a correction based on proportional, integral, and derivative terms. These terms are used to adjust the control output in order to bring the process variable as close as possible to the setpoint.

The proportional term (P) of the controller adjusts the control output in proportion to the error, which helps to bring the process variable closer to the setpoint. The integral term (I) of the controller helps to eliminate the residual error by accumulating the error over time and adjusting the control output accordingly. The derivative term (D) of the controller anticipates future errors by measuring the rate of change of the error and adjusting the control output accordingly.

# Implementation:

# Hardware:
- Raspberry Pi 4B (to run image processing you'll need a Pi with good RAM)
  - 5v USB power supply
  - USB Mouse/Keyboard
  - Cable to computer monitor   
- V398 Missile Launcher
- PT Pan/Tilt Servo Camera Mount and 2x Servos
- Raspberry Pi Camera Module
- NPN transistor
- Diode
- 10 ohm resistor
- 560 ohm resistor
- Jumper wires
- Pin Headers


![Turret_Circuit_Diagram](https://github.com/JasperWH/Turret-with-OpenCV/blob/main/Pictures/Turret_Circuit_Diagram.png)

Note this worked fine for me but it is generally recommended to power the servos from the powers source and not through the Pi

![V398 Missile Launcher](https://github.com/JasperWH/Turret-with-OpenCV/blob/main/Pictures/V398_Missile_Launcher.PNG)

Note the firing motor can be wire in any polarity as can the PTM switch. The PTM switch provides feeedback to the Pi that the motor has rotated and  the dart has been fired.

# Software:

### Setup:
Setting up OpenCV on the Raspberry Pi can be dificult, this is why I recommend using the SD image provided by Qengineering for a complete working Raspberry Pi 4 dedicated to deep learning: https://qengineering.eu/opencv-c-examples-on-raspberry-pi.html
This site will also give you a step by step guide for setup and goes deeper into image processing with object recognition.

Along with the OpneCV library, this project also requires the pigpio library for operating the servos etc. which you can download from here: https://abyz.me.uk/rpi/pigpio/
To get the code to run in CodeBlocks you'll need to point to these libraries as follow (pointing to where ever you saved the library):

![setup1](https://github.com/JasperWH/Turret-with-OpenCV/blob/main/Pictures/Setup1.png)

![setup2](https://github.com/JasperWH/Turret-with-OpenCV/blob/main/Pictures/Setup2.png)

### Running Code:
You need system administator privliages to run code using GPIO, unfortunatly there is no way (as of Jan 2023) to set up CodeBlocks to do this, as such you will have to run the code from the terminal by navigating to the executable then running with sudo:
>cd /home/pi/[...directory to your project...]/bin/Debug

![CommandWindow1](https://github.com/JasperWH/Turret-with-OpenCV/blob/main/Pictures/Command_Window1.png)

Once started you'll be given the option to threshld the image, this is important as variations in light conditions and colour of the target will effect the abilty to locate the target centre point.

![Thresholding](https://github.com/JasperWH/Turret-with-OpenCV/blob/main/Pictures/Thresholding.png)

Once done you be given the option to fire, when the crosshair is lined up with the circile you can fire or abort.
![CommandWindow](https://github.com/JasperWH/Turret-with-OpenCV/blob/main/Pictures/Command_Window.png)

# End Result:
![Turret_Circuit_Diagram](https://github.com/JasperWH/Turret-with-OpenCV/blob/main/Pictures/Engagment.gif)

![setup](https://github.com/JasperWH/Turret-with-OpenCV/blob/main/Pictures/Setup.jpg)
