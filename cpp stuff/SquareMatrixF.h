#ifndef SQUARE_MATRIX_F
#define SQUARE_MATRIX_F

#include "Image.h"
#include "ColorScheme.h"

class SquareMatrixF{
    public:
        SquareMatrixF(int size);
        SquareMatrixF(SquareMatrixF& source);
        ~SquareMatrixF();
        void setElement(int x, int y, float value);
        float getElement(int x, int y);
        int getSize();
        void order(int* order);
        //add argument for colorscale, also redesign
        Image toImage(float maxDist, int offset = 0, bool flipVertical = true, bool flipHorizontal = false);
        Image toImage(float maxDist, ColorScheme* colorScheme, int offset = 0, bool flipVertical = true, bool flipHorizontal = false);
    private:
        void flipVertical();
        void flipHorizontal();
        void offset(int offset);
        int _size;
        float* matrixBuffer;
};

#endif