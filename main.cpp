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
int blocks = 10;
int UnitFilter = 1;

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

void UnitFilterTrackBar(int uf, void *){
    UnitFilter = uf;
}


int main(int argc, char** argv){

    Mat srcframe;
    string filename;
    int accurK = 10;

    if (argc < 2){
        cout<<"[i] PLease input file name"<<endl;
        return 0;
    }else {
        filename  = string(argv[1]);
    }
    if (argc == 3){
        accurK = atoi(argv[2]);
    }
    cout<<"[i] file: "<<filename<<endl;

    vector<pair<uint, Scalar> > rgb_colors;
    vector <Scalar> MaxFreqColors;

    VideoCapture capture(filename);
    int c = 0;
    int w = 0;
    capture >> srcframe;

    while(!srcframe.empty()) {  //!srcfame.empty()
        if (c % accurK == 0) {
            w++;
            rgb_colors = GetColors(srcframe, 10);
            MaxFreqColors.push_back(rgb_colors[0].second);

            if (c % 1000 == 0) {
                cout << "%";
            }
        }
        capture >> srcframe;
        c++;
    }

    cout<<endl<<c<<endl;
    Mat StockImage(Size(w, HEIGHT), srcframe.type());


    for (int i = 0; i < MaxFreqColors.size(); i++){
        rectangle(StockImage, cvPoint(i, 0), cvPoint(i + 1, HEIGHT), MaxFreqColors[i], -1);
    }

    resize(StockImage, StockImage, Size(WIDTH, HEIGHT), 0, 0, INTER_CUBIC);
    imwrite("/home/slowbro/Stock.jpg", StockImage);

    namedWindow("distance", CV_WINDOW_AUTOSIZE);
    createTrackbar("dist", "distance", &dif, 20, DistanceTrackBar);
    createTrackbar("nums", "distance", &DistConst,300, DistanceConstkTrackBar);
    createTrackbar("Destroy", "distance", &PercentDestroy, 100, PercentDestroyTrackBar);
    createTrackbar("UF", "distance", &UnitFilter, 300, UnitFilterTrackBar);

    createTrackbar("block", "distance", &blocks, 20, BlockTrackBar);


    while(1){

        DistanceTrackBar(dif, NULL);
        DistanceConstkTrackBar(DistConst, NULL);
        PercentDestroyTrackBar(PercentDestroy, NULL);
        UnitFilterTrackBar(UnitFilter, NULL);
        vector<pair<uint, Scalar> > RecColors2 = BuildRangeVector(MaxFreqColors, dif, DistConst, PercentDestroy);
        vector<pair<uint, Scalar> > RecColors1 = UnitColors(RecColors2, UnitFilter);

        uint MaxWidth = 0;
        for (int i = 0; i < RecColors1.size(); i++){
            MaxWidth += RecColors1[i].first;
        }
        Mat DistanceImage(Size(MaxWidth, HEIGHT), srcframe.type());
        int NowX = 0;
        for (int i = 0; i < RecColors1.size(); i++){
            rectangle(DistanceImage, cvPoint(NowX, 0), cvPoint(NowX + RecColors1[i].first, HEIGHT), RecColors1[i].second, -1);
            NowX += RecColors1[i].first;
        }
        resize(DistanceImage, DistanceImage, Size(WIDTH, HEIGHT), 0, 0, INTER_CUBIC);

        BlockTrackBar(blocks, NULL);
        vector<pair<uint, Scalar> > FinalColors = GetMaxFreqColors(RecColors1, static_cast<unsigned int>(blocks));

        MaxWidth = 0;
        for (int i = 0; i < FinalColors.size(); i++){
            MaxWidth += FinalColors[i].first;
        }
        int Delta = (MaxWidth / blocks) / 4;
        Mat FinalImageWrite(Size(MaxWidth + Delta * (blocks + 1), HEIGHT * 2), srcframe.type(), Scalar(255, 255, 255));
        Mat FinalImageShow(Size(MaxWidth + Delta * (blocks + 1), HEIGHT), srcframe.type(), Scalar(255, 255, 255));

        NowX = 0;
        for (int i = 0; i < FinalColors.size(); i++){
            rectangle(FinalImageWrite, cvPoint(Delta + NowX, HEIGHT / 2), cvPoint(Delta + NowX + FinalColors[i].first, trunc(HEIGHT * 1.5)), FinalColors[i].second, -1);
            rectangle(FinalImageShow, cvPoint(Delta + NowX, 0), cvPoint(Delta + NowX + FinalColors[i].first, HEIGHT), FinalColors[i].second, -1);
            NowX += FinalColors[i].first + Delta;
        }


        resize(FinalImageShow, FinalImageShow, Size(WIDTH, HEIGHT), 0, 0, INTER_CUBIC);

        Mat FinImage(Size(WIDTH, HEIGHT *  3), srcframe.type(), Scalar(255, 255, 255));

        for (int x = 0; x < WIDTH; x++){
            for (int y = 0; y < HEIGHT - 20; y++){
                FinImage.at<Scalar>(y, x) = StockImage.at<Scalar>(y, x);
                FinImage.at<Scalar>(y + HEIGHT, x) = DistanceImage.at<Scalar>(y, x);
                FinImage.at<Scalar>(y + HEIGHT + HEIGHT, x) = FinalImageShow.at<Scalar>(y, x);
            }
        }
        imshow("distance", FinImage);

        char key = waitKey(33);
        if (key == 27) {
            imwrite("/home/slowbro/dist.jpg", DistanceImage);
            imwrite("/home/slowbro/FinalImage.jpg", FinalImageWrite);
            break;
        }
    }
    destroyAllWindows();
    return 0;
}