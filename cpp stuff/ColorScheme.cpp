#include <cstring>
#include <stdlib.h>
#include "ColorScheme.h"
#include <iostream>

using namespace std;

ColorScheme *ColorScheme::_spectral{nullptr};
ColorScheme *ColorScheme::_greyscale{nullptr};

ColorScheme::ColorScheme(unsigned char colors[][3], int len) {
    _len = len;
    _colors = (unsigned char*)malloc(sizeof(unsigned char) * _len * 3);
    for (int i = 0; i < _len; i++)
    {
        _colors[3 * i] = colors[i][0];
        _colors[3 * i + 1] = colors[i][1];
        _colors[3 * i + 2] = colors[i][2];
    }
}

ColorScheme::ColorScheme(ColorScheme& source) {
    _len = source._len;
    _colors = (unsigned char*)malloc(sizeof(unsigned char) * _len * 3);
    memcpy(_colors, source._colors, sizeof(unsigned char) * _len * 3);
}

ColorScheme::~ColorScheme() {
    free(_colors);
}

void ColorScheme::getColor(unsigned char out[3], double ratio, double min, double max) {
    //scale ratio between 0 and 1
    ratio -= min; 
    ratio /= (max - min);
    //scale ratio between 0 and len
    ratio *= (_len - 1);

    //so we dont index out of bounds later
    if(ratio >= _len - 1) { //color is max value
        out[0] = _colors[3*_len - 3];
        out[1] = _colors[3*_len - 2];
        out[2] = _colors[3*_len - 1];
        return;
    }
    if(ratio <= 0) { //color is negative value
        out[0] = _colors[0];
        out[1] = _colors[1];
        out[2] = _colors[2];
        return;
    }
    
    //get the first of the two colors
    int lowerColor = (int)ratio;
    
    //contribution of the upper color
    double upperContribution = ratio - (double)lowerColor;

    //contributions of lower values
    double lowR = ((double)1 - upperContribution) * (double)_colors[3*lowerColor];
    double lowG = ((double)1 - upperContribution) * (double)_colors[3*lowerColor + 1];
    double lowB = ((double)1 - upperContribution) * (double)_colors[3*lowerColor + 2];

    //contributions of upper values
    double highR = upperContribution * (double)_colors[3*lowerColor + 3];
    double highG = upperContribution * (double)_colors[3*lowerColor + 4];
    double highB = upperContribution * (double)_colors[3*lowerColor + 5];
    
    //populate return array
    out[0] = (unsigned char)(int) (lowR + highR);
    out[1] = (unsigned char)(int) (lowG + highG);
    out[2] = (unsigned char)(int) (lowB + highB);
}

ColorScheme* ColorScheme::greyscale(){
    if(!_greyscale) {
        //initialise
        unsigned char tmpGreyscaleColors[2][3] = {
            {0,0,0}, //black
            {255,255,255}}; //white
        unsigned char(* greyscaleColors)[3] = tmpGreyscaleColors;
        _greyscale = new ColorScheme(greyscaleColors, 2);
    }

    return _greyscale;
}

ColorScheme* ColorScheme::spectral(){
    if(!_spectral) {
        unsigned char tmpSpectralColors[11][3] = {
            {94,79,162},   //c11
            {50,136,189},   //c10
            {102,194,165},  //c9
            {171,221,164},  //c8
            {230,245,152},  //c7
            {255,255,191},  //c6
            {254,224,139},  //c5
            {253,174,97},   //c4
            {244,109,67},   //c3
            {213,62,79},    //c2
            {158,1,66}};     //c1
        unsigned char(* spectralColors)[3] = tmpSpectralColors;
        _spectral = new ColorScheme(spectralColors, 11);
    }

    return _spectral;
}

void ColorScheme::meaningLegend(int meaningNum, unsigned char color[]){
    switch (meaningNum)
    {
    case 1:
        color[0] = 186;
        color[1] = 221;
        color[2] = 116;
        break;
    case 2:
        color[0] = 241;
        color[1] = 135;
        color[2] = 117;
        break;
    case 3:
        color[0] = 109;
        color[1] = 163;
        color[2] = 207;
        break;
    case 4:
        color[0] = 188;
        color[1] = 128;
        color[2] = 189;
        break;
    case 5:
        color[0] = 141;
        color[1] = 211;
        color[2] = 199;
        break;
    case 6:
        color[0] = 136;
        color[1] = 136;
        color[2] = 136;
        break;
    case 7:
        color[0] = 255;
        color[1] = 243;
        color[2] = 116;
        break;
    case 8:
        color[0] = 0;
        color[1] = 136;
        color[2] = 138;
        break;
    default:
        break;
    }
}

void ColorScheme::speechLegend(int partOfSpeechNum, unsigned char color[]){
    switch (partOfSpeechNum)
    {
    case 1: // noun
        color[0] = 143;
        color[1] = 143;
        color[2] = 237;
        break;
    case 2: // adjective
        color[0] = 255;
        color[1] = 217;
        color[2] = 255;
        break;
    case 3: // verb
        color[0] = 51;
        color[1] = 194;
        color[2] = 164;
        break;
    default:
        break;
    }
}