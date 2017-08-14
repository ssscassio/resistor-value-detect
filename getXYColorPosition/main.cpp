////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

enum color_code {
    purple,
    black,
    brown,
};

color_code hashit (std::string const& inString) {
    if (inString == "purple") return purple;
    if (inString == "black") return black;
    if (inString == "brown") return brown;
}

Scalar getScalar(string color, int HIGH){

    switch(hashit(color)){
        case purple:
            if(HIGH){
                return Scalar(150, 194, 164);
            } else {
                return Scalar(121, 159, 97);
            }
            break;
        case black:
            if(HIGH){
                return Scalar(179, 121, 57);
            } else {
                return Scalar(0, 0, 0);
            }
            break;
    }
}

struct colorFounded {
    double posX;
    double posY;
    string color;
};


struct colorFounded getXYColorPosition (string color, Mat imgHSV) {
    Mat imgThresholded;
    struct colorFounded found;
    inRange(imgHSV, getScalar(color, 0 ) , getScalar(color, 1 ) , imgThresholded);

    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    Moments oMoments = moments(imgThresholded);

    double dM01 = oMoments.m01;
    double dM10 = oMoments.m10;
    double dArea = oMoments.m00;
    found = {dM10 / dArea, dM01 / dArea, color};
    if (dArea > 10000)
        return found;
}

 int main( int argc, char** argv )
 {
     Mat imgOriginal;

     imgOriginal = imread( "resistor.jpg", 1 );

     //Create a black image with the size as the camera output
     Mat points = Mat::zeros( imgOriginal.size(), CV_8UC3 );;


    while (true)
    {

    Mat imgHSV;

    string color = "black";

    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

    struct colorFounded found = getXYColorPosition(color, imgHSV);

    circle(points, Point(found.posX, found.posY), 3,  Scalar(0,255,0), 2);

    cout << "POSX: " << found.posX << "POSY: " << found.posY << "COLOR: " << color;

    imgOriginal = imgOriginal + points;
    imshow("Original", imgOriginal); //show the original image

       if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break;
       }
    }

   return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
