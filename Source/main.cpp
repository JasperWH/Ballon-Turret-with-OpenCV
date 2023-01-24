#include <iostream>
#include <vector>
#include <thread>
//Servo
//---------------------------------------------------
#include <pigpio.h>
#define SERVO_V_PIN 17    // GPIO pin 17 vertical servo
#define SERVO_H_PIN 4     // GPIO pin 4 Horizontal servo
#define SERVO_OFF 0
#define SERVO_VC_PW 2100  // vertical   Pulse Width max      clockwise = 2100
#define SERVO_VAC_PW 800  // vertical   Pulse Width max anti-clockwise = 800
#define SERVO_HC_PW 2300  // Horizontal Pulse Width max      clockwise = 2100
#define SERVO_HAC_PW 700  // Horizontal Pulse Width max anti-clockwise = 700
#define kp 0.2            // PID servo tuning
#define ki 0              // PID servo tuning (0 as only using PD controller)
#define kd 0.1            // PID servo tuning
//Clock
//---------------------------------------------------
#include <chrono>
using CClock = std::chrono::high_resolution_clock;
CClock::time_point previousTime;
CClock::time_point currentTime;
//Opencv & Camera
//---------------------------------------------------
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
//Header Files
//---------------------------------------------------
#include "Track_object.hpp"
#include "Fire.hpp"
#include "PID.hpp"
#include "Threshold.hpp"

void worker_funcFire(void);
void worker_funcMain(std::vector<int>);

// To run, build, go to project debug file directory and run executable with 'sudo ./Turret'
int main(){

//Determin HSV used:
//---------------------------------------------------
// Loop until user enters a valid input
bool valid_input = false;
std::vector<int> Target = {0,0,0,0,0,0};              // Initialise HSV colour vector
do {
    std::cout << "Would you like to threshold the image? [Y/N]: ";
    char choice;
    std::cin >> choice;

    switch (choice){
        case 'Y':
            Threshold_Img(Target);
            std::cout << "Running main programme...\n";
            valid_input = true;                        // Break loop if valid input
            break;
        case 'N':
            std::cout << "Running main programme...\n";
            Target = {28,121,7,234,94,255};            // HSV color values of a blue balloon
            valid_input = true;                        // Break loop if valid input
            break;
        default:                                       // Otherwise tell the user what went wrong
            std::cerr << "That input is invalid.  Please try again.\n";
    }
} while (valid_input == false);

//Run threads:
//---------------------------------------------------
   std::thread worker_threadA(worker_funcFire);        // passing the function to thread
   std::thread worker_threadB(worker_funcMain,Target); // passing the function and vector to thread

   worker_threadA.join();                              // Wait for Thread First to finish
   worker_threadB.join();                              // Wait for Thread Second to finish

   return 0;

}

void worker_funcFire(void)
{

std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Sleep for 1.00 second while camera initialises
std::cout << "ready to fire\n";
fire();                                         // Start engagment sequence

}

void worker_funcMain(std::vector<int> Target)
{
//PID Setup:
//---------------------------------------------------
double PID_V[4] = {0,0,0,0}; // error | cumError | lastError | PID output, for Veritcal position
double PID_H[4] = {0,0,0,0}; // error | cumError | lastError | PID output, for Horizontal position

//Servo Setup:
//---------------------------------------------------
if (gpioInitialise() < 0) {  // Error if PIGPIO cannot initialise
    std::cerr << "ERROR: Unable to initialise GPIO" << std::endl;
}
int Servo_V = (SERVO_VC_PW+SERVO_VAC_PW)/2;           // Vertical Centre   (+-1 = +-1deg)
int Servo_H = (SERVO_HC_PW+SERVO_HAC_PW)/2;           // Horizontal Centre (+-1 = +-1deg)
gpioServo(SERVO_V_PIN, Servo_V);                      // Initialise servo to range centre
gpioServo(SERVO_H_PIN, Servo_H);                      // Initialise servo to range centre

//Video Setup
//---------------------------------------------------
VideoCapture cap(0);                                   // Declaring an object to capture stream of frames from default camera -- cap(0);  opens RaspiCam or WebCam
int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);   // Default resolutions of the frame are obtained.The default resolutions are system dependent.
int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
int Tar[2] = {0,0};                                    // Create array for Horizontal and Vertical Target centre point
int VC[2]  = {frame_width/2,frame_height/2};           // Create array for Horizontal and Vertical Video centre point
/*
  0|
   |
240|      x
   |
480|_____________
   0     320    640
*/

if (!cap.isOpened()) {       // Prompt an error message if no video stream is found
    std::cerr << "ERROR: Unable to open the camera" << std::endl;
}
Mat frame;                   // Declaring a matrix 'frame' to load the frames

VideoWriter video("outcpp.avi", cv::VideoWriter::fourcc('M','J','P','G'), 10, Size(frame_width,frame_height)); // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file.

//Taking an everl asting loop to show the video
//---------------------------------------------------
while(1) {
//  Take video Frame
//  -------------------------------------------------
    cap >> frame;
    if (frame.empty()) {    // Breaking the loop if no video frame is detected
        std::cerr << "ERROR: Unable to grab from the camera" << std::endl;
        break;
    }

//  Get object position
//  -------------------------------------------------
    Track_Object(Tar, frame, Target, VC);               // Calculate centre point of Target in image and save to Tar, if not found return camera centre point

//  Control Servo
//  ------------------------------------------------
    // Increasse or decrease servo PW using PID control system until target centre is the same as object centre
    PID_H[0] = Tar[0] - VC[0];                          // Horizontal position error
    PID_V[0] = Tar[1] - VC[1];                          // Vertical position error
    computePID(PID_V,previousTime);                     // Get PID value for servo control and errors for next PID calculation
    computePID(PID_H,previousTime);                     // Get PID value for servo control and errors for next PID calculation
    previousTime = currentTime;                         // Remember time to compute elasped time between inputs

    if( PID_V[0] <10 && PID_V[0]>-10 ){
        gpioServo(SERVO_V_PIN, 0);                      // Turn servo off if within bounds of target
    }
    else{
        Servo_V -= PID_V[3]*0.5;                        // Move servo by calculated PID value (note: negative as axis inverted and further tuning to prevent overshoot)
        if (Servo_V < SERVO_VC_PW && Servo_V > SERVO_VAC_PW )
            gpioServo(SERVO_V_PIN, Servo_V);            // If calculated PID value is greated than the range keep servo in the last poisition
    }
    if( PID_H[0] <10 && PID_H[0]>-10 ){
        gpioServo(SERVO_H_PIN, 0);                      // Turn servo off if within bounds of target
    }
    else{
        Servo_H += PID_H[3];                            // Move servo by calculated PID value
        if (Servo_H < SERVO_HC_PW && Servo_H > SERVO_HAC_PW )
            gpioServo(SERVO_H_PIN, Servo_H);            // If calculated PID value is greated than the range keep servo in the last poisition
    }

//  Show Image + Crosshair
//  -------------------------------------------------
    Point center(VC[0], VC[1]);// Declaring the image center point
    int radius = 30;             // Declaring the radius
    Scalar line_Color(0, 0, 255);// Color of the circle
    int thickness = 2;           // thickens of the line
    // Drawing crosshair:
    circle(frame, center,radius, line_Color, thickness);
    line(frame, Point(Tar[0]-50, Tar[1]), Point(Tar[0]+50, Tar[1]), line_Color, thickness);
    line(frame, Point(Tar[0], Tar[1]-50), Point(Tar[0], Tar[1]+50), line_Color, thickness);


    video.write(frame);          // Write the frame into the file 'outcpp.avi'
    imshow("Live",frame);        // Showing the video
    int key = cv::waitKey(20);   // Allowing 20 milliseconds frame processing time and initiating break condition
    if (key== 27)
        break;
}

//End program
//---------------------------------------------------
    std::cout << "Closing the camera" << std::endl;
    cap.release();               // Releasing the buffer memory
    video.release();             // Releasing the buffer memory
    destroyAllWindows();         // Closes all the frames
    gpioTerminate();             // Termination of servo GPIO

}



