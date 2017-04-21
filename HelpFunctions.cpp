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

Scalar BGR2LAB(Scalar &rgb){
    Scalar XYZ;
    rgb.val[0] = rgb.val[0] / 255;
    rgb.val[0] = rgb.val[0] / 255;
    rgb.val[0] = rgb.val[0] / 255;

    XYZ.val[0] = rgb.val[2] * 0.4124 + rgb.val[1] * 0.3576 + rgb.val[0] * 0.1805;
    XYZ.val[1] = rgb.val[2] * 0.2126 + rgb.val[1] * 0.7152 + rgb.val[0] * 0.0722;
    XYZ.val[2] = rgb.val[2] * 0.0193 + rgb.val[1] * 0.1192 + rgb.val[0] * 0.9505;

    return XYZ;
}

double XYZdistance(Scalar c_1, Scalar c_2){
    Scalar c1 = BGR2LAB(c_1);
    Scalar c2 = BGR2LAB(c_2);
    return sqrt(pow(c1.val[0] - c2.val[0], 2) + pow(c1.val[1] - c2.val[1], 2) + pow(c1.val[2] - c2.val[2], 2));
}



vector <pair <uint, Scalar> > BlockThis(vector <pair<uint, Scalar> > Src, unsigned int BlockSize){
    vector<pair <uint, Scalar> > Dst(Src);
    uint MinBlockSize = Dst.back().first;
    //cout<<"MinBlockSize= "<<MinBlockSize<<endl;
    size_t i;

    for (i = 0; i < Dst.size(); i++){
        if (MinBlockSize > Dst[i].first){
            MinBlockSize = Dst[i].first;
        }
    }

    while (MinBlockSize < BlockSize) {
        for (i = 0 + 1; i < Dst.size() - 1; i++) {
            uint NowSize = Dst[i].first;
            if (NowSize < BlockSize) {

                double RColorDistance = XYZdistance(Dst[i].second, Dst[i + 1].second);
                double LColorDistance = XYZdistance(Dst[i].second, Dst[i + 1].second);

                if (LColorDistance != RColorDistance) {
                    if (LColorDistance < RColorDistance) {
                        i--;
                    }
                    Dst[i].second.val[0] = (Dst[i].first * Dst[i].second.val[0] + Dst[i + 1].first * Dst[i + 1].second.val[0]) /
                            (Dst[i + 1].first + Dst[i].first);
                    Dst[i].second.val[1] = (Dst[i].first * Dst[i].second.val[1] + Dst[i + 1].first * Dst[i + 1].second.val[1]) /
                            (Dst[i + 1].first + Dst[i].first);
                    Dst[i].second.val[2] = (Dst[i].first * Dst[i].second.val[2] + Dst[i + 1].first * Dst[i + 1].second.val[2]) /
                            (Dst[i + 1].first + Dst[i].first);

                    Dst[i].first += Dst[i + 1].first;

                    Dst.erase(Dst.begin() + i + 1);
                } else {
                    i--;
                    Dst[i].first = Dst[i].first + Dst[i + 1].first + Dst[i + 2].first;
                    Dst[i].second.val[0] =
                            (Dst[i].first * Dst[i].second.val[0] + Dst[i + 1].first * Dst[i + 1].second.val[0] +
                             Dst[i + 2].first * Dst[i + 2].second.val[0]) /
                                    (Dst[i + 2].first + Dst[i + 1].first + Dst[i].first);
                    Dst[i].second.val[1] =
                            (Dst[i].first * Dst[i].second.val[1] + Dst[i + 1].first * Dst[i + 1].second.val[1] +
                             Dst[i + 2].first * Dst[i + 2].second.val[1]) /
                                    (Dst[i + 2].first + Dst[i + 1].first + Dst[i].first);
                    Dst[i].second.val[2] =
                            (Dst[i].first * Dst[i].second.val[2] + Dst[i + 1].first * Dst[i + 1].second.val[2] +
                             Dst[i + 2].first * Dst[i + 2].second.val[2]) /
                                    (Dst[i + 2].first + Dst[i + 1].first + Dst[i].first);
                    Dst.erase(Dst.begin() + i + 1);
                    Dst.erase(Dst.begin() + i + 2);
                }
            i++;
            }
        }
        MinBlockSize = Dst.front().first;
        for (i = 0; i < Dst.size() - 1; i++){
            if (MinBlockSize > Dst[i].first){
                MinBlockSize = Dst[i].first;
            }
        }
        //cout<<MinBlockSize<<endl;
    }
    //cout<<MinBlockSize<<endl;
    return Dst;

}


vector <pair<uint, Scalar> > GetMaxFreqColors(vector<pair<uint, Scalar> >  Src, unsigned int _Nums){

    vector <pair<uint, Scalar> > BufSrc(Src);
    vector <pair<uint, Scalar> > FinalVector;
    unsigned int Nums = _Nums;
    if (Nums < 2) {
        Nums = 2;
    }
    FinalVector.reserve(Nums);

    sort(BufSrc.begin(), BufSrc.end(), colors_sort);
    for (size_t i = 0; i < Src.size(); i++){
        for (size_t j = 0; j < (Nums < Src.size() ? Nums : Src.size()); j++) {
            if (Src[i] == BufSrc[j]) {
                FinalVector.push_back(Src[i]);
            }
        }
    }
    return FinalVector;
};


vector <pair<uint, Scalar> > UnitColors(vector<pair<uint, Scalar> > Src, int K){
    vector <pair<uint, Scalar> > FinalVector(Src);
    uint ColorSum;
    for (int i = 0; i < FinalVector.size() - 1; i++){
        if (XYZdistance(FinalVector[i].second, FinalVector[i + 1].second) < K){

            ColorSum = FinalVector[i].first + FinalVector[i + 1].first;

            FinalVector[i].second.val[0] = (FinalVector[i].first * FinalVector[i].second.val[0] +
                                            FinalVector[i + 1].first * FinalVector[i + 1].second.val[0]) / (ColorSum);
            FinalVector[i].second.val[1] = (FinalVector[i].first * FinalVector[i].second.val[1] +
                                            FinalVector[i + 1].first * FinalVector[i + 1].second.val[1]) / (ColorSum);
            FinalVector[i].second.val[2] = (FinalVector[i].first * FinalVector[i].second.val[2] +
                                            FinalVector[i + 1].first * FinalVector[i + 1].second.val[2]) / (ColorSum);
            FinalVector[i].first += FinalVector[i + 1].first;
            FinalVector.erase(FinalVector.begin() + i + 1);
            i--;
        }
    }
    return FinalVector;
};

vector <pair<uint, Scalar> > BuildRangeVector(vector<Scalar> Src, int dist, int DistNums, int PerDestroy){

    vector <pair<uint, Scalar> > FinalVector;
    pair<uint, Scalar> temp;


    for (size_t i = 0; i < Src.size(); i++){
        temp.first = 1;
        temp.second = Src[i];
        FinalVector.push_back(temp);
    }
    uint MinBlockDist  = uint((Src.size() * PerDestroy) / 1000);
    if (dist == 1 || DistNums == 0){
        return  FinalVector;
    }
    for (int idist = 1; idist < DistNums; idist++){
        for (int i = 0; i < FinalVector.size() - 1; i++){
            if (XYZdistance(FinalVector[i].second, FinalVector[i+1].second) <= (idist * dist)){

                FinalVector[i].second.val[0] = (FinalVector[i].first * FinalVector[i].second.val[0] +
                        FinalVector[i + 1].first * FinalVector[i + 1].second.val[0]) / (FinalVector[i].first + FinalVector[i + 1].first);
                FinalVector[i].second.val[1] = (FinalVector[i].first * FinalVector[i].second.val[1] +
                        FinalVector[i + 1].first * FinalVector[i + 1].second.val[1]) / (FinalVector[i].first + FinalVector[i + 1].first);
                FinalVector[i].second.val[2] = (FinalVector[i].first * FinalVector[i].second.val[2] +
                        FinalVector[i + 1].first * FinalVector[i + 1].second.val[2]) / (FinalVector[i].first + FinalVector[i + 1].first);
                FinalVector[i].first += FinalVector[i + 1].first;
                FinalVector.erase(FinalVector.begin() + i + 1);
                //i--;
            }
        }
    }
    for (int i = 0; i < FinalVector.size() - 1; i++) {
        if (FinalVector[i + 1].first <= MinBlockDist && XYZdistance(FinalVector[i].second, FinalVector[i+1].second) >= (DistNums * dist)) {
            FinalVector[i].first += FinalVector[i + 1].first;
            FinalVector.erase(FinalVector.begin() + i + 1);
        }
    }
    return FinalVector;

};

vector <pair<uint, Scalar> > BuildRangeVectorV0(vector<Scalar> Src, int dist){

    vector <pair<uint, Scalar> > FinalVector;
    pair<uint, Scalar> temp;
    temp.first = 1;
    temp.second = Src[0];
    FinalVector.push_back(temp);

    for (vector<Scalar>::iterator it = Src.begin() + 1; it < Src.end(); it++) {
        if (XYZdistance(FinalVector.back().second, (*it)) <= dist) {
            FinalVector.back().second.val[0] = (FinalVector.back().first * FinalVector.back().second.val[0] +
                                                (*it).val[0]) / (FinalVector.back().first + 1);
            FinalVector.back().second.val[1] = (FinalVector.back().first * FinalVector.back().second.val[1] +
                                                (*it).val[1]) / (FinalVector.back().first + 1);
            FinalVector.back().second.val[2] = (FinalVector.back().first * FinalVector.back().second.val[2] +
                                                (*it).val[2]) / (FinalVector.back().first + 1);
            FinalVector.back().first++;
        } else {
            temp.second = (*it);
            temp.first = 1;
            FinalVector.push_back(temp);
        }
    }

    return FinalVector;
};

vector <pair<uint, Scalar> > BlockThis11(vector <pair <uint, Scalar> > Src, size_t BlockSize){

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