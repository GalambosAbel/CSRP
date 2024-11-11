#ifndef COLOR_SCHEME
#define COLOR_SCHEME

class ColorScheme {
    public:
        ColorScheme(unsigned char colors[][3], int len);
        ColorScheme(ColorScheme& source);
        ~ColorScheme();
        void getColor(unsigned char out[3], double ratio, double min = 0, double max = 1);
        static ColorScheme* greyscale();
        static ColorScheme* spectral();
        static void meaningLegend(int meaningNum, unsigned char color[]);
        static void speechLegend(int partOfSpeechNum, unsigned char color[]);
    private:
        int _len;
        unsigned char* _colors;
        static ColorScheme* _greyscale;
        static ColorScheme* _spectral;
};

#endif