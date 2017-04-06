#include <iostream>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types_c.h>
#include "HelpFunctions.h"


using namespace std;
using namespace cv;


Size MySize(X,Y);


int main(int argc, char** argv){



/*    if (argc != 2){
        cout<<"Please enter filename" << endl;
        return 1;
    }*/

    Mat srcframe;
    Mat buffframe;
   // Mat dstframe1(Size(X,Y), CV_8UC3);
    Mat hsv;
    //Mat color_indexes(Size(X,Y), CV_8UC1);


   // char* filename = argv[1];
   string filename = "/windows/Zagruzki/LastMan/E05 Des carrés dan des carrés.mkv";
    string filename1 = "/home/slowbro/Pictures/Wallpapers/12.jpg";
    cout<<"[i] file: "<<filename<<endl;

//    namedWindow(FINWINDOWNAME, CV_WINDOW_AUTOSIZE);
//    moveWindow(FINWINDOWNAME, MyScreanX + 400, MyScreanY);
    //namedWindow(RecFile, CV_WINDOW_AUTOSIZE);

    //srcframe = imread(filename);
    unsigned int Count[10];


    vector<pair<uint, Scalar> > rgb_colors;
    vector <Scalar> MaxFreqColors;
    VideoCapture capture(filename);
    Mat bufframe;
    int c=0;

    while(1){
        int i = 0, j = 0, x = 0, y = 0;
        capture >> srcframe;

        //resize(srcframe, bufframe, Size(800, 600), 0, 0, INTER_CUBIC);
        rgb_colors = GetColors(srcframe, 10);
        MaxFreqColors.push_back(rgb_colors[0].second);





//        Mat Rectangles(MySize, srcframe.type());
//        int h = Y / RECT_COLORS_SIZE;
//        int w = X;
//        for(i=0; i<RECT_COLORS_SIZE; i++ ){
//            rectangle(Rectangles, cvPoint(0, i*h), cvPoint(w, i*h+h), rgb_colors[i].second, -1);
//        }
//        imshow(RecFile, Rectangles);


//        imshow(FINWINDOWNAME, Fin);
        c++;
        if (c % 1000 == 0){
            cout<<c<<" ";
        }
        if (srcframe.empty()) {
            cout<<c<<endl;
            Mat ColorPic(Size(MaxFreqColors.size(), 600), srcframe.type());
            for (int i = 0; i < MaxFreqColors.size(); i++){
                rectangle(ColorPic, cvPoint(i, 0), cvPoint(1+i, 600), MaxFreqColors[i]);
            }
            imwrite("/home/slowbro/Pictures/Wallpapers/SuperFinStock.png", ColorPic);
            Mat ResizeColorPicck;
            resize(ColorPic, ResizeColorPicck, Size(800, 600), 0, 0, INTER_CUBIC);
            imwrite("/home/slowbro/Pictures/Wallpapers/SuperFinResize.png", ResizeColorPicck);
            blur(ColorPic, ColorPic, Size(MaxFreqColors.size() / 50, 10));
            imwrite("/home/slowbro/Pictures/Wallpapers/SuperFinBlur.png", ColorPic);
            blur(ResizeColorPicck, ResizeColorPicck, Size(30, 10));
            imwrite("/home/slowbro/Pictures/Wallpapers/SuperFinResizeBlur.png", ResizeColorPicck);
            break;
        }
    }
    //}
//    Mat ColorPic(Size(MaxFreqColors.size(), 101), srcframe.type());
//
//    for (int i = 0; i < MaxFreqColors.size(); i++){
//        rectangle(ColorPic, cvPoint(i, 0), cvPoint(1+i, 100), MaxFreqColors[i], -1);
//    }
//    imwrite("/home/slowbro/Pictures/Wallpapers/Fin.jpg", MaxFreqColors);
//
//    blur(MaxFreqColors, MaxFreqColors, Size(5, 5));
//
//    imwrite("/home/slowbro/Pictures/Wallpapers/FinBlur.jpg", MaxFreqColors);
//

    //cvReleaseCapture(&capture);
    //cvDestroyAllWindows();
    destroyAllWindows();


    return 0;
}