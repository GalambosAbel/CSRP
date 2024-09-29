#include <iostream>
#include <stdio.h>
#include <string>
#include "BMPGenerator.h"
#include "GraphReader.h"
#include "Image.h"
#include "SquareMatrixF.h"
#include "ColorScheme.h"
#include "Tests.h"

using namespace std;

int main ()
{
    runAllTests();
    SquareMatrixF mat = GraphReader::readTsp_Explicit_FullMatrix("..\\testFiles\\tspTest.tsp");
    mat.toImage(100, ColorScheme::spectral()).printImageAsBMP("..\\testFiles\\tspTestMatrix.bmp");
    mat.toTspFullMatrix("..\\testFiles\\writeTest.tsp");
}
