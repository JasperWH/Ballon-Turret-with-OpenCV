using namespace cv;

void Track_Object(int* arr, Mat imgOriginal, std::vector<int> HSV, int* VC) {

    // Assign HSV values to variables
    int iLowH = HSV[0];
    int iHighH = HSV[1];
    int iLowS = HSV[2];
    int iHighS = HSV[3];
    int iLowV = HSV[4];
    int iHighV = HSV[5];

    Mat for_Rotation = getRotationMatrix2D(Point(imgOriginal.cols / 2, imgOriginal.rows / 2), 180, 1);// Affine transformation matrix for the 2D rotation
    Mat after_Rotating;                                                                               // Declaring matrix for image after rotation
    warpAffine(imgOriginal, after_Rotating, for_Rotation, imgOriginal.size());                        // Applying affine transformation

    Mat imgHSV;                                                                                       // Declaring matrix for image after colour transfomation applied
    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);                                                     // Convert the captured frame from BGR to HSV

    Mat imgThresholded;                                                                               // Declaring matrix for image after threshold applied
    inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);     // Threshold the image

    // Morphological opening (removes small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    // Morphological closing (removes small holes from the foreground)
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    // Calculate the moments of the thresholded image
    Moments oMoments = moments(imgThresholded);
    double dM01 = oMoments.m01;
    double dM10 = oMoments.m10;
    double dArea = oMoments.m00;

    // If the area <= 10000, then consider that the there are no object in the image and it's because of the noise, the area is not zero, then default back to centre
    if (dArea > 10000){
        // Calculate the position of the object
        arr[0] = dM10 / dArea;
        arr[1] = dM01 / dArea;
    }
    else{
        // Camera horizontal and vertical centre global variable
        arr[0] = VC[0];
        arr[1] = VC[1];
    }


}
