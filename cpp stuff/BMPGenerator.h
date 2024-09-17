class BMPGenerator{
    public:
        static void generateBitmapImage(unsigned char* image, int height, int width, char* imageFileName, int bitDepth = 3);
    private:
        static unsigned char* createBitmapFileHeader(int height, int stride);
        static unsigned char* createBitmapInfoHeader(int height, int width, int bitDepth);
        static const int FILE_HEADER_SIZE = 14;
        static const int INFO_HEADER_SIZE = 40;
};