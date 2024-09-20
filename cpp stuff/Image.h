#ifndef IMAGE
#define IMAGE

class Image{
    public:
        Image(int width, int height, int depth = 3);
        ~Image();
        unsigned char getPixelChannel(int x, int y, int channel);
        void writePixelChannel(int x, int y, int channel, unsigned char color);
        void printImageAsBMP(char* targetFileName);
    private:
        unsigned char* imageBuffer;
        int _width, _height, _depth;
};

#endif