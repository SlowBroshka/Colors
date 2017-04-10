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

int dif = 4;    //4 8
int DistConst = 7;  //7    11
int PercentDestroy = 5; //5
int blocks = 1;
int COLOR_NUMS = 10;

void DistanceTrackBar(int d, void *){
    dif = d;
}
void BlockTrackBar(int b, void *){
    blocks = b;
}

void DistanceConstkTrackBar(int c, void *){
    DistConst = c;
}

void PercentDestroyTrackBar(int pd, void *){
    PercentDestroy = pd;
}


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
    string filename = "/windows/Denis/MVI_3300.MOV";
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
        //int i = 0, j = 0, x = 0, y = 0;
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
        cout<<c<<" ";
        //if (srcframe.empty()) {
        if (c == 1000){
            cout<<c<<endl;

            Mat StockImage(Size(c, 200), srcframe.type());
            for (int i = 0; i < MaxFreqColors.size(); i++){
                rectangle(StockImage, cvPoint(i, 0), cvPoint(i + 1, 200), MaxFreqColors[i], -1);
            }

            imwrite("/home/slowbro/Pictures/Stock.jpg", StockImage);

            vector<pair<uint, Scalar> > RecColors1 = BuildRangeVector(MaxFreqColors, dif, DistConst, PercentDestroy);
            uint MaxWidth = 0;
            for (int i = 0; i < RecColors1.size(); i++){
                MaxWidth += RecColors1[i].first;
            }
            Mat DistanceImage(Size(MaxWidth, 200), srcframe.type());
            int NowX = 0;
            for (int i = 0; i < RecColors1.size(); i++){
                rectangle(DistanceImage, cvPoint(NowX, 0), cvPoint(NowX + RecColors1[i].first, 200), RecColors1[i].second, -1);
                NowX += RecColors1[i].first;
            }
            imwrite("/home/slowbro/Pictures/dist.jpg", DistanceImage);

            vector<pair<uint, Scalar> > FinalColors = GetMaxFreqColors(RecColors1, COLOR_NUMS);

            MaxWidth = 0;
            for (int i = 0; i < FinalColors.size(); i++){
                cout<<FinalColors[i].first<<" ";
                MaxWidth += FinalColors[i].first;
            }
            int Delta = (MaxWidth / COLOR_NUMS) / 4;
            Mat FinalImage(Size(MaxWidth + Delta * (COLOR_NUMS + 1), 600), srcframe.type(), Scalar(255, 255, 255));

            NowX = 0;
            for (int i = 0; i < FinalColors.size(); i++){
                rectangle(FinalImage, cvPoint(Delta + NowX, 200), cvPoint(Delta + NowX + FinalColors[i].first, 400), FinalColors[i].second, -1);
                NowX += FinalColors[i].first + Delta;
            }
            imwrite("/home/slowbro/Pictures/Final.jpg", FinalImage);
            break;
        }
    }


    return 0;
}