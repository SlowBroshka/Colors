//
// Created by slowbro on 03.04.17.
//


#ifndef COLORS_HELPFUNCTIONS_H
#define COLORS_HELPFUNCTIONS_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types_c.h>
#include "Constants.h"

using namespace cv;
using namespace std;


//unsigned int colorCount[NUM_COLOR_TYPE] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int GetPixelColorType(int H, int S, int V);


bool colors_sort(std::pair<uint, Scalar> a, std::pair<uint, Scalar> b);

vector<pair<uint, Scalar> > GetColors(Mat const &image, size_t n);

vector <pair<uint, Scalar> > BlockThis(size_t BlockSize);


#endif //COLORS_HELPFUNCTIONS_H


//        vector <pair<int, uint> > colors;
//        colors.reserve(NUM_COLOR_TYPE);
//
//        for (i = 0; i < NUM_COLOR_TYPE; i++){
//            pair <int, uint> color;
//            color.first = i;
//            color.second = colorCount[i];
//            colors.push_back(color);
//        }
//
//        sort(colors.begin(), colors.end(), colors_sort);
//
//        for (i = 0; i < colors.size(); i++){
//            cout<<"[i]color "<<colors[i].first << " ("
//                <<sCtypes[colors[i].first]<<") - "<<colors[i].second<<endl;
//        }
//        cout<<"--"<<endl;
//
//        cout<<"--"<<endl;
//    Mat Fin(Size(hsv.rows, hsv.cols), hsv.type());
//        for (y = 0; y <Fin.cols; y++){
//            for (x = 0; x <Fin.rows; x++){
//                Fin.at<Vec3b>(x,y)[0] = static_cast<uchar>(rgb_colors[color_indexes.at<Vec3b>(x,y)[0]].val[0]);
//                Fin.at<Vec3b>(x,y)[1] = static_cast<uchar>(rgb_colors[color_indexes.at<Vec3b>(x,y)[0]].val[1]);
//                Fin.at<Vec3b>(x,y)[2] = static_cast<uchar>(rgb_colors[color_indexes.at<Vec3b>(x,y)[0]].val[2]);
//            }
//        }