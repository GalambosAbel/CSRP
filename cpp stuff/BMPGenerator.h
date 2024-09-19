#ifndef BMP_GENERATOR
#define BMP_GENERATOR


/**
 * A class containing static methods needed to generate an image.
 *
 * TODO: The note.
 * @note Find source of the function implementations in this class on the internet.
 */
class BMPGenerator{
    public:
        /**
         * Generates a `.bmp` images from the provided image matrix, and saves it to a file with the provided name.
         * 
         * @param image The matrix containing all the info of all the pixels.
         * @param height Height of image.
         * @param width Width of image.
         * @param imageFileName The name of the target file, extension included.
         * @param bytesPerPixel The number of bytes per pixel, values other than `3` may or may not work.
         * 
         * TODO: The note.
         * @note Find Source of the function implementation on the internet.
         */
        static void generateBitmapImage(unsigned char* image, int height, int width, char* imageFileName, int bytesPerPixel = 3);
    
    private:
        /**
         * Generates part of the required header for `.bmp` files.
         * 
         * TODO: The note.
         * @note Find source of the function implementation on the internet.
         */
        static unsigned char* createBitmapFileHeader(int height, int stride);
        
        /**
         * Generates part of the required header for `.bmp` files.
         * 
         * TODO: The note.
         * @note Find source of the function implementation on the internet.
         */
        static unsigned char* createBitmapInfoHeader(int height, int width, int bytesPerPixel);
        
        static const int FILE_HEADER_SIZE = 14;
        static const int INFO_HEADER_SIZE = 40;
};

#endif