//
// Created by slowbro on 03.04.17.
//
#include <iostream>
#include <string>
#include <stdio.h>
#ifndef COLORS_CONSTANTS_H
#define COLORS_CONSTANTS_H

static const size_t X = 400;
static const size_t Y = 400;
static const size_t MinImageSize = 800;
static const size_t RESIZESTEP = 4;
static const size_t MyScreanY = 100;
static const size_t MyScreanX = 300;
static const std::string FINWINDOWNAME = "Colors";
static const std::string RecFile = "Rec";
static const int RECT_COLORS_SIZE = 10;
static const uint BlocksWeightIgnore = 10;    //[0, 100) % of ignore Blocks

enum {cBLACK = 0, cWHITE, cGREY, cRED, cORANGE, cYELLOW, cGREEN, cAQUA,  cBLUE, cPURPLE, NUM_COLOR_TYPE};

static const std::string sCtypes[NUM_COLOR_TYPE] = {"Black", "White", "Grey", "Red", "Orange", "Yellow", "Green", "Aqua", "Blue", "Purrpule"};
static const uchar cCH[NUM_COLOR_TYPE] =     {  0,      0,        0,     0,       20,       30,     60,     85,     120,    140 };
static const uchar cCS[NUM_COLOR_TYPE] =     {  0,      0,        0,     255,     255,      255,    255,    255,    255,    255};
static const uchar cCV[NUM_COLOR_TYPE] =     {  0,      255,      120,   255,     255,      255,    255,    255,    255,    255};



#endif //COLORS_CONSTANTS_H
