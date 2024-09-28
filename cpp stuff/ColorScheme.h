#ifndef COLOR_SCHEME
#define COLOR_SCHEME

class ColorScheme {
    public:
        ColorScheme(unsigned char colors[][3], int len);
        ColorScheme(ColorScheme& source);
        ~ColorScheme();
        void getColor(unsigned char out[3], float ratio, float min = 0, float max = 1);
        static ColorScheme* greyscale();
        static ColorScheme* spectral();
    private:
        int _len;
        unsigned char* _colors;
        static ColorScheme* _greyscale;
        static ColorScheme *_spectral;
};

#endif