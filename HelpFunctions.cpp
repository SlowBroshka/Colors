//
// Created by slowbro on 03.04.17.
//
#include "HelpFunctions.h"


using namespace std;
using namespace cv;

int GetPixelColorType(int H, int S, int V){
    int color = cBLACK;

    if (V < 70)
        color = cBLACK;
    else if (V > 190 && S < 27)
        color = cWHITE;
    else if (S < 53 && V < 185)
        color = cGREY;
    else{
        if (H < 7)
            color = cRED;
        else if (H < 25)
            color = cORANGE;
        else if (H < 34)
            color = cYELLOW;
        else if (H < 73)
            color = cGREEN;
        else if (H < 102)
            color = cAQUA;
        else if (H < 140)
            color = cBLUE;
        else if (H < 170)
            color = cPURPLE;
        else
            color = cRED;
    }
    return color;
}

bool colors_sort(std::pair<uint, Scalar> a, std::pair<uint, Scalar> b){
    return (a.first > b.first);
}

vector<pair<uint, Scalar> > GetColors(Mat const &image, size_t n){
    size_t N = n;
    if (n > 10 || n == 0){
        N = 10;
    }

    std::vector<pair<uint, Scalar> > rgb_colors;
    pair <uint, Scalar> tmp;
    rgb_colors.reserve(N);
    unsigned int Count[N];
    for (int i = 0; i < N; i++){
        tmp.first = 0;
        tmp.second = Scalar(0, 0, 0);
        rgb_colors.push_back(tmp);
        Count[i] = 0;
    }

    Mat resizeimage(image);
    if (image.rows > MinImageSize){     //Y height
        resize(image, resizeimage, Size(image.cols / RESIZESTEP, image.rows / RESIZESTEP), 0, 0, INTER_CUBIC);
    }else{
        resizeimage = image;
    }


    Mat hsv;
    cvtColor(resizeimage, hsv, CV_BGR2HSV);
    Mat NImage(Size(hsv.rows, hsv.cols), hsv.type());
    Mat color_indexes(Size(hsv.rows, hsv.cols), 1);


    for (int y = 0; y < hsv.cols; y++){
        for (int x = 0; x < hsv.rows; x++){
            int H = hsv.at<Vec3b>(x,y)[0];
            int S = hsv.at<Vec3b>(x,y)[1];
            int V = hsv.at<Vec3b>(x,y)[2];

            int ctype = GetPixelColorType(H, S, V);

            NImage.at<Vec3b>(x,y)[0] = cCH[ctype];
            NImage.at<Vec3b>(x,y)[1] = cCS[ctype];
            NImage.at<Vec3b>(x,y)[2] = cCV[ctype];

            rgb_colors[ctype].second.val[0] += resizeimage.at<Vec3b>(x,y)[0];
            rgb_colors[ctype].second.val[1] += resizeimage.at<Vec3b>(x,y)[1];
            rgb_colors[ctype].second.val[2] += resizeimage.at<Vec3b>(x,y)[2];

            color_indexes.at<Vec3b>(x,y)[0] = static_cast<uchar>(ctype);

            Count[ctype]++;
        }
    }
    //middle colors
    for (int i = 0; i < N; i++){
        rgb_colors[i].first = Count[i];
        rgb_colors[i].second.val[0] /= Count[i];
        rgb_colors[i].second.val[1] /= Count[i];
        rgb_colors[i].second.val[2] /= Count[i];
    }
    sort(rgb_colors.begin(), rgb_colors.end(), colors_sort);

    return rgb_colors;
}

vector <pair<uint, Scalar> > BuildRangeVector(vector<Scalar> Src){

    vector <pair<uint, Scalar> > FinalVector;
    pair<uint, Scalar> temp;
    temp.first = 1;
    temp.second = Src[0];
    FinalVector.push_back(temp);

    for (vector<Scalar>::iterator it = Src.begin()+1; it < Src.end(); it++){
        if (FinalVector.back().second == (*it)){
            FinalVector.back().first++;
        }else{
            temp.second = (*it);
            temp.first = 1;
            FinalVector.push_back(temp);
        }
    }
    return FinalVector;

    };

vector <pair<uint, Scalar> > BlockThis(vector <pair <uint, Scalar> > Src, size_t BlockSize){

    pair <uint, Scalar> Ltemp;
    Ltemp.first = 0, Ltemp.second = Scalar(0,0,0);
    pair <uint, Scalar> Rtemp;
    Rtemp.first = 1, Rtemp.second = Scalar(0,0,0);

    for (size_t i = 1; i <= BlockSize; i++){

        Ltemp.first = 0, Ltemp.second = Scalar(0,0,0);
        Rtemp.first = 0, Rtemp.second = Scalar(0,0,0);

        for (vector <pair <uint, Scalar> >::iterator it = Src.begin(); it < Src.end()-1; it++){     //vozmogno +i

            if ((*it).second != (*(it+1)).second){
                while (((*it).second == (*(it + Rtemp.first)).second) && ((it + Rtemp.first) < Src.end()) ){
                    Rtemp.first++;
                }
            }else{
                Ltemp.first++;
                Ltemp.second = (*it).second;
            }

            if (Ltemp.first > Rtemp.first){
                for (size_t k = 0; k < i; k++){
                    (*(it + k)).second = Ltemp.second;
                }
            }else{
                for (size_t k = 0; k < i; k++){
                    (*(it + k)).second = Rtemp.second;
                }
            }

        }

    }

};