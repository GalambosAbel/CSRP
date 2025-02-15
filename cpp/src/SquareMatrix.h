#ifndef SQUARE_MATRIX
#define SQUARE_MATRIX

#include "Image.h"
#include "ColorScheme.h"
#include <vector>
#include <string>

using namespace std;

class SquareMatrix
{
public:
    SquareMatrix(int size);
    SquareMatrix(SquareMatrix &source);
    ~SquareMatrix();

    void setElement(int x, int y, double value);
    void setMaxValue(double value) { maxValue = value; };
    void setMinValue(double value) { minValue = value; };

    // Set the meaning of the word in sentence x
    void setMeaning(int x, int value) { wordMeanings[x] = value; };

    // Set the part of speech of the word in sentence x
    void setPartOfSpeech(int x, int value) { partOfSpeech[x] = value; };

    double getElement(int x, int y) { return matrixBuffer[x + y * _size]; };
    double getMaxValue() { return maxValue; };
    double getMinValue() { return minValue; };
    int getSize() { return _size; };

    // Get the meaning of the word in sentence x
    int getMeaning(int x) { return wordMeanings[x]; };

    // Get the part of speech of the word in sentence x
    int getPartOfSpeech(int x) { return partOfSpeech[x]; };

    // Order the matrix according to the order vector permutation
    void order(vector<int> order);

    // Output the matrix as an image with the provided color scheme
    Image toImage(double maxDist, int offset = 0, bool flipVertical = true, bool flipHorizontal = false);
    Image toImage(double maxDist, ColorScheme *colorScheme, int offset = 0, bool flipVertical = true, bool flipHorizontal = false);

    // Output the matrix as an image with the provided color scheme and labeled word meanings and parts of speech
    Image toDetailedImage(double maxDist, ColorScheme* colorScheme, int offset = 0, bool flipVertical = false, bool flipHorizontal = false);

    // Output the matrix as a .TSP file that can be used with the Concorde TSP solver
    string toTspFullMatrix(string tspName = "tsplib from matrix", string comment = "");

    SquareMatrix toNeosInput();

    // Output the matrix as a text file
    void toInFullMatrix(char *fileNameWithPath);

    // Swap 2 columns (or rows since matrices are assumed to be symmetric) while updating Moran's I
    void swap(int i, int j);

    // Calculate the Moran's I of the matrix
    double moransI();

    // Calculate the entropy score of the word meanings based on the current order
    double entropyScore();

    // Calculate the switching score of the word meanings based on the current order
    double switchingScore();

    // Tries to optimize Moran's I of matrix with Simulated Annealing (used to find counter examples to previous orders)
    double simAnnealingOrderMoransI(int iters, double startTemp);

    // Creates a new matrix with distances which can be used to optimize the Moran's I of the original matrix (TSP)
    SquareMatrix moransIDistanceMatrix();

    SquareMatrix KevinsDistanceMatrix();

    // Order the matrix according to the TSP order provided by the Concorde TSP solver
    void orderTSPRaw();

    // Order the matrix according to the TSP order provided by the Concorde TSP solver applied to the Moran's I distance matrix
    void orderTSPMoransI();

    
    void orderTSPKevin();
private:
    vector<int> getTSPOrder();

    void flipVertical();
    void flipHorizontal();
    void offset(int offset);

    vector<int> extraNodeOrderToNormal(vector<int> extraOrder);

    int _size;
    double maxValue;
    double minValue;

    int *wordMeanings; // Keeps information for the visualization
    int *partOfSpeech; //

    double *matrixBuffer;

    double _morans_mean = -1;
    double _morans_variance = -1;
    double _moransI = -1;
};

#endif