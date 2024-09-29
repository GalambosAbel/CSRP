#ifndef SQUARE_MATRIX_F
#define SQUARE_MATRIX_F

#include "Image.h"
#include "ColorScheme.h"

class SquareMatrixF{
    public:
        SquareMatrixF(int size);
        SquareMatrixF(SquareMatrixF& source);
        ~SquareMatrixF();
        void setElement(int x, int y, double value);
        double getElement(int x, int y);
        int getSize();
        void order(int* order);
        Image toImage(double maxDist, int offset = 0, bool flipVertical = true, bool flipHorizontal = false);
        Image toImage(double maxDist, ColorScheme* colorScheme, int offset = 0, bool flipVertical = true, bool flipHorizontal = false);
        void toTspFullMatrix(char* fileNameWithPath, char* tspName = "tsplib from matrix", char* comment = "");
    private:
        void flipVertical();
        void flipHorizontal();
        void offset(int offset);
        int _size;
        double* matrixBuffer;
};

#endif