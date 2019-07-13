#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;


std::string gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

int main()
{
    int capture_width = 1920 ;
    int capture_height = 1080 ;
    int display_width = 1280 ;
    int display_height = 720 ;
    int framerate = 60 ;
    int flip_method = 2 ;

    std::string pipeline = gstreamer_pipeline(capture_width,
	capture_height,
	display_width,
	display_height,
	framerate,
	flip_method);

    std::cout << "Using pipeline: \n\t" << pipeline << "\n";

    CascadeClassifier haar_cascade;
    haar_cascade.load("/usr/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml");

    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

    if(!cap.isOpened())
        return -1;

    int thing = 20;
    bool rev = 0;
    for(;;)
    {
        Mat frame;
        cap >> frame;
        Mat original = frame.clone();
        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        vector< Rect_<int> > faces;
        haar_cascade.detectMultiScale(gray, faces);
        for(int i = 0; i < faces.size(); i++) {
            Rect face_i = faces[i];
            rectangle(original, face_i, CV_RGB(0, 255,0), 1);
         }
        imshow("Picture", original);
        if(waitKey(30) >= 0) break;
    }
    return 0;
}



