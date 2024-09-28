#ifndef IMAGE
#define IMAGE

class Image{
    public:
        Image(int width, int height, int depth = 3);
        Image(Image& source);
        ~Image();
        unsigned char getPixelChannel(int x, int y, int channel);
        void setPixelChannel(int x, int y, int channel, unsigned char color);
        void setPixel(int x, int y, unsigned char color[3]);
        void printImageAsBMP(char* targetFileName);
    private:
        unsigned char* imageBuffer;
        int _width, _height, _depth;
};

#endif