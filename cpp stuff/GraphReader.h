#ifndef GRAPH_READER
#define GRAPH_READER

#include <string>
#include <vector>

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
         * Reads a .tsp file that has format `EUC_2D`, and outputs a vector with the x- and y-coordinates of the points.
         * The order of the elements in the vector is the same order that they appear in, the indicies are ignored.
         * @param tspFileNameWithPath This is the path and name of the .tsp file.
         * 
         * @returns A vector containing the coordinates of the verticies.
        */
        static vector<vector<float>> readTsp_EUC_2D(char* tspFileNameWithPath);
        
        /**
         * Reads a .cyc file, and returns a vector with the permutation.
         * @param tourFileNameWithPath This is the path and name of the file that contains a permutation of the cities.
         * 
         * @returns A vector of ints, that contains the cities from the .cyc file in the same order.
         */
        static vector<int> readTour(char* tourFileNameWithPath);
    
    private:
        /**
         * Reads an arbitrary file, and outputs its contents as a string.
         * 
         * @throws An error is thrown if the file doesn't exist.
         * 
         * TODO: The note.
         * @note Find source of the function implementation on the internet.
         * @note Its here: https://cplusplus.com/reference/cstdio/fread/
         */
        static string fileToString(char* fileNameWithPath);
};

#endif