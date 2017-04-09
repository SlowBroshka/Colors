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

int dif = 8;    //4
int DistConst = 11;  //7
int PercentDestroy = 5; //5
int blocks = 1;

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
    string filename = "/windows/Zagruzki/LastMan/E24 Encaisser et pas mourir.mkv";
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
        //if (srcframe.empty()) {
        if (c == 2000){
            cout<<c<<endl;
            namedWindow("Stock", CV_WINDOW_AUTOSIZE);
            moveWindow("Stok", 100, 0);


            Mat StockImage(Size(c, 200), srcframe.type());
            for (int i = 0; i < MaxFreqColors.size(); i++){
                rectangle(StockImage, cvPoint(i, 0), cvPoint(i + 1, 200), MaxFreqColors[i], -1);
            }
            imwrite("/home/slowbro/Pictures/Stock.jpg", StockImage);
            imshow("Stock", StockImage);

            namedWindow("distance", CV_WINDOW_AUTOSIZE);
            moveWindow("distance", 100, 400);
            createTrackbar("dist", "distance", &dif, 20, DistanceTrackBar);
            createTrackbar("nums", "distance", &DistConst,300, DistanceConstkTrackBar);
            createTrackbar("Destroy", "distance", &PercentDestroy, 100, PercentDestroyTrackBar);

            namedWindow("distance + blocks", CV_WINDOW_AUTOSIZE);
            moveWindow("distance + blocks", 100, 600);
            createTrackbar("block", "distance + blocks", &blocks, 500, BlockTrackBar);


            while(1){

                DistanceTrackBar(dif, NULL);
                DistanceConstkTrackBar(DistConst, NULL);
                PercentDestroyTrackBar(PercentDestroy, NULL);
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


                imshow("distance", DistanceImage);

                BlockTrackBar(blocks, NULL);
                vector<pair<uint, Scalar> > RecColors = BlockThis(RecColors1, blocks);

                MaxWidth = 0;
                for (int i = 0; i < RecColors.size(); i++){
                    MaxWidth += RecColors[i].first;
                }
                cout<<"MaxWidth: "<<MaxWidth<<endl;
                Mat BlockImage(Size(MaxWidth, 200), srcframe.type());

                NowX = 0;
                for (int i = 0; i < RecColors.size(); i++){
                    rectangle(BlockImage, cvPoint(NowX, 0), cvPoint(NowX + RecColors[i].first, 200), RecColors[i].second, -1);
                    NowX += RecColors1[i].first;
                }


               // BlockTrackBar(blocks, NULL);
                imshow("distance + blocks", BlockImage);


                char c = waitKey(33);
                if (c == 27) {
                    imwrite("/home/slowbro/Pictures/dist.jpg", DistanceImage);
                    imwrite("/home/slowbro/Pictures/block.jpg", BlockImage);
                    break;
                }
            }

//            vector<pair<uint, Scalar> > RecColors1 = BuildRangeVector(MaxFreqColors);
//            vector<pair<uint, Scalar> > RecColors = BlockThis(RecColors1, 40);
//            uint MaxWidth = 0;
//            for (int i = 0; i < RecColors.size(); i++){
//                MaxWidth += RecColors[i].first;
//            }
//            cout<<MaxWidth<<endl;
//            int NowX = 0;
//            Mat ColorPic(Size(MaxWidth, 600), srcframe.type());
//            for (int i = 0; i < RecColors.size(); i++){
//                cout<<i<<"- "<<RecColors[i].first<<endl;
//                rectangle(ColorPic, cvPoint(NowX, 0), cvPoint(NowX + RecColors[i].first, 600), RecColors[i].second, -1);
//                NowX += RecColors[i].first;
//            }
//            imwrite("/home/slowbro/Pictures/Wallpapers/New3.jpg", ColorPic);



//            Mat ColorPic(Size(MaxFreqColors.size(), 600), srcframe.type());
//            for (int i = 0; i < MaxFreqColors.size(); i++){
//                rectangle(ColorPic, cvPoint(i, 0), cvPoint(1+i, 600), MaxFreqColors[i]);
//            }
//            imwrite("/home/slowbro/Pictures/Wallpapers/SuperFinStock.png", ColorPic);
//            Mat ResizeColorPicck;
//            resize(ColorPic, ResizeColorPicck, Size(800, 600), 0, 0, INTER_CUBIC);
//            imwrite("/home/slowbro/Pictures/Wallpapers/SuperFinResize.png", ResizeColorPicck);
//            blur(ColorPic, ColorPic, Size(MaxFreqColors.size() / 50, 10));
//            imwrite("/home/slowbro/Pictures/Wallpapers/SuperFinBlur.png", ColorPic);
//            blur(ResizeColorPicck, ResizeColorPicck, Size(30, 10));
//            imwrite("/home/slowbro/Pictures/Wallpapers/SuperFinResizeBlur.png", ResizeColorPicck);
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
    //destroyAllWindows();


    return 0;
}