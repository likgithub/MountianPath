#include <iostream>
#include <fstream>
#include "functions.h"
using namespace std;

// Normally you should not use global variables. However, the stack is
//   limited in size and will not allow arrays of this size. We'll learn
//   later how to put these arrays on the heap instead. Regardless, for now
//   we'll use these and treat them as if they were declared in main. So
//   when used in functions, we'll pass them as we would any other array.
static int elevations[MAX_ROWS][MAX_COLS];
static Pixel image[MAX_ROWS][MAX_COLS];

int main() {
    int rows = -1;
    int columns = -1;
    int max = 0;
    int min = 0;
    string filename;
    string pixelImageFile;
    int elevationShort = 0; //keeps track of lowest elevation
    int elevationNew = 0; //keeps track of elevation in each run through
    int index = 0; //keeps index of lowest elevation data

    cin >> rows; 

    // following 2 if statements ensure number of rows and columns are positive numbers 
    if ((cin.fail())|| (rows <= 0)) { //checks for error input of rows
        cout << "Error: Problem reading in rows and columns.";
        return 0;
    }
    
    cin >> columns; 

    if ((cin.fail()) || (columns <= 0)) { //checks for error input of columns
        cout << "Error: Problem reading in rows and columns.";
        return 0;
    
    }
    cin >> filename;


    ifstream elevationDat(filename);
    if (!elevationDat.is_open()) { //checks for file opening error
        cout << "Error: Unable to open file <filename>.";
        return 0;
    }

    loadData(elevations, rows, columns, elevationDat);
    findMaxMin(elevations, rows, columns, max, min);
    loadGreyscale(image, elevations, rows, columns, max, min);

    pixelImageFile = filename.append(".ppm");
    ofstream pixelFile(pixelImageFile);

    elevationShort = colorPath(elevations, image, rows, columns, {252,25,63}, 0);
    elevationNew = colorPath(elevations, image, rows, columns, {252,25,63}, 0);
    
    for (int i = 0; i < rows; i++) { //colors shortest greedy path in each row red
        elevationNew = colorPath(elevations, image, rows, columns, {252,25,63}, i);

        if (elevationNew < elevationShort) {
            elevationShort = elevationNew;
            index = i;
        }
    }

    colorPath(elevations, image, rows, columns, {31,253,13}, index); //colors shortest path green
    
    outputImage(image, rows, columns, pixelFile);



    
    


}
