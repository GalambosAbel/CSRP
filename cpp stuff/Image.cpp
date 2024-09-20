#include "Image.h"
#include <stdlib.h>
#include "BMPGenerator.h"

using namespace std;

Image::Image(int width, int height, int depth) {
    imageBuffer = (unsigned char*) malloc(sizeof(unsigned char) * width * height * depth);
    _width = width;
    _height = height;
    _depth = depth;
}

Image::~Image(){
    free(imageBuffer);
}

unsigned char Image::getPixelChannel(int x, int y, int channel) {
    return imageBuffer[channel + y * _depth + x * _width * _depth];
}

void Image::writePixelChannel(int x, int y, int channel, unsigned char color) {
    imageBuffer[channel + y * _depth + x * _width * _depth] = color;
}

void Image::printImageAsBMP(char* targetFileName) {
    BMPGenerator::generateBitmapImage(imageBuffer, _height, _width, targetFileName, _depth);
}