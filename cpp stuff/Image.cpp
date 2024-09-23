#include "Image.h"
#include <stdlib.h>
#include <cstring>
#include "BMPGenerator.h"

using namespace std;

Image::Image(int width, int height, int depth) {
    imageBuffer = (unsigned char*) malloc(sizeof(unsigned char) * width * height * depth);
    _width = width;
    _height = height;
    _depth = depth;
}

Image::Image(Image& source) {
    _width = source._width;
    _height = source._height;
    _depth = source._depth;
    imageBuffer = (unsigned char*) malloc(sizeof(unsigned char) * _width * _height * _depth);
    memcpy(imageBuffer, source.imageBuffer, sizeof(unsigned char) * _width * _height * _depth);
}

Image::~Image(){
    free(imageBuffer);
}

unsigned char Image::getPixelChannel(int x, int y, int channel) {
    return imageBuffer[channel + y * _depth + x * _width * _depth];
}

unsigned char* Image::getPixel(int x, int y) {
    unsigned char arr[3];
    arr[0] = getPixelChannel(x, y, 0);
    arr[1] = getPixelChannel(x, y, 1);
    arr[2] = getPixelChannel(x, y, 2);
    return arr;
}

void Image::setPixelChannel(int x, int y, int channel, unsigned char color) {
    imageBuffer[channel + y * _depth + x * _width * _depth] = color;
}

void Image::setPixel(int x, int y, unsigned char color[3]) {
    setPixelChannel(x, y, 0, color[0]);
    setPixelChannel(x, y, 1, color[1]);
    setPixelChannel(x, y, 2, color[2]);
}

void Image::printImageAsBMP(char* targetFileName) {
    BMPGenerator::generateBitmapImage(imageBuffer, _height, _width, targetFileName, _depth);
}