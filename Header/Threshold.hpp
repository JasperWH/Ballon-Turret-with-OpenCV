using namespace cv;

void Threshold_Img(std::vector<int> &Target) {

//Video Setup
//---------------------------------------------------
VideoCapture cap(0);         // Declaring an object to capture stream of frames from default camera -- cap(0);  opens RaspiCam or WebCam
if (!cap.isOpened()) {       // Prompt an error message if no video stream is found
    std::cerr << "ERROR: Unable to open the camera" << std::endl;
}
Mat imgOriginal;             // Declaring a matrix 'frame' to load the frames

// Threshold value setup
//---------------------------------------------------
int iLowH  = 0;
int iHighH = 179;
int iLowS  = 0;
int iHighS = 255;
int iLowV  = 0;
int iHighV = 255;

//Create trackbars in "Control" window
namedWindow("Control");                           // Create a window called "Control"
createTrackbar("LowH", "Control", &iLowH, 179);   // Hue (0 - 179)
createTrackbar("HighH", "Control", &iHighH, 179);
createTrackbar("LowS", "Control", &iLowS, 255);   // Saturation (0 - 255)
createTrackbar("HighS", "Control", &iHighS, 255);
createTrackbar("LowV", "Control", &iLowV, 255);   // Value (0 - 255)
createTrackbar("HighV", "Control", &iHighV, 255);

//Taking an everlasting loop to show the video
//---------------------------------------------------
while(1) {

    cap >> imgOriginal;
    if (imgOriginal.empty()) {    // Breaking the loop if no video frame is detected
        std::cerr << "ERROR: Unable to grab from the camera" << std::endl;
        break;
    }

    Mat imgHSV;                                                                                   // Declaring matrix for image after colour transfomation applied
    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);                                                 // Convert the captured frame from BGR to HSV

    Mat imgThresholded;                                                                           // Declaring matrix for image after threshold applied
    inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); // Threshold the image

    //morphological opening (remove small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //morphological closing (fill small holes in the foreground)
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    // Show thresholded and original image
    imshow("Thresholded Image", imgThresholded);
    imshow("Original", imgOriginal);

    // Allowing 20 milliseconds frame processing time and initiating break condition
    int key = cv::waitKey(20);
    key = (key==255) ? -1 : key;
    if (key>=0)
        break;
}

// Close windows and releasing the buffer memory
std::cout << "Closing the camera" << std::endl;
cap.release();
destroyAllWindows();

// Save new HSV colour values of the target and print results
Target = {iLowH,iHighH, iLowS, iHighS, iLowV, iHighV};
std::cout << "Threshold values used are:\n";
for(int i = 0; i < Target.size();i++)
    std::cout << Target[i] << "  ";
std::cout << std::endl;

}
