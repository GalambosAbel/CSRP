#ifndef GRAPH_READER
#define GRAPH_READER

#include <string>

using namespace std;

/**
 * Class for opening files and parsing their outputs.
 * 
 * @warning This class is far from finished, use with care! 
 * 
 * TODO:
 * Extend this class a whole bunch.
 */
class GraphReader {
    public:
        /**
         * Reads a .tsp file that has format `EUC_2D`, and outputs a vector with the x-coordinates and a vector with the y-coordinates.
         * @param tspFileNameWithPath This is the path and name of the .tsp file.
         * @param xs This is the array where the x-coordinates will be output to.
         * @param ys This is the array where the y-coordinates will be output to.
         * @param length Is the number of cities in the tsp file that is being read.
         * 
         * @warning It is required that length(`xs`), length(`ys`)  >= `length`, but no error is thrown!
         * 
         * TODO: The notes.
         * @note This function should be reworked such that the return type is `float*[2]`, and the parameters `xs`, `ys` and `length` are not included.
         * @note Also the function should be extended to be able to read differnet configurations of a .tsp file, and return values accordingly.
         */
        static void readTsp(char* tspFileNameWithPath, float* xs, float* ys, int length);
        
        /**
         * Reads a .cyc file, and outputs in the `order` parameter the order of verticies.
         * @param tourFileNameWithPath This is the path and name of the file that contains a permutation of (1..`length`).
         * @param order This is the array where the order will be output to.
         * @param length Is the number of cities in the tour that is being read.
         * 
         * @warning It is required that length(`order`) >= `length`, but no error is thrown!
         * 
         * TODO: The note.
         * @note This function should be reworked such that the return type is `int*`, and the parameters `order` and `length` are not included.
         */
        static void readTour(char* tourFileNameWithPath, int* order, int length);
    
    private:
        /**
         * Reads an arbitrary file, and outputs its contents as a string.
         * 
         * @throws An error is thrown if the file doesn't exist.
         * 
         * TODO: The note.
         * @note Find source of the function implementation on the internet.
         */
        static string fileToString(char* fileNameWithPath);
};

#endif