#include <iostream>
#include <fstream>
#include <cmath>
#include "functions.h"
using namespace std;

void findMaxMin(const int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, int& max, int& min) {

    max = elevations[0][0];
    min = elevations[0][0];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {

            if (max < elevations[i][j]) {
                max = elevations[i][j];
            }

            if (min > elevations[i][j]) {
                min = elevations[i][j];
            }
        }
    }

}

void loadData(int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, istream& inData) {
    int elevationVal;
    int counterRow = 0;
    int counterCol = 0;
    

    while(!inData.eof()) {

        inData >> elevationVal;
        if (counterRow == rows) { //reads for too many data points
            cout << "Error: Too many data points."; 
            return;
        }

        
        if (inData.fail()) { //checks for non integer value
            cout << "Error: Read a non-integer value.";
            return;
        }
        elevations[counterRow][counterCol] = elevationVal;
        counterCol += 1;

        if (counterCol == cols) { //gies ti next line of code
            counterRow += 1;
            counterCol = 0;
        }

        if ((inData.eof()) && ((counterRow < rows - 1) || (counterCol < cols))) { //checks if file finished early
            cout << "Error: End of file reached prior to getting all the required data."; 
            return;
        }

    
    }

}

int scaleValue(int value, int max, int min) {

    double shadeOfGrey = ((static_cast<double>(value) - min)/(max - min)) * 255; 
    if (shadeOfGrey - static_cast<int>(shadeOfGrey) >= 0.5) {
        shadeOfGrey += 1;
    }

    return shadeOfGrey;

}

void loadGreyscale(Pixel image[MAX_ROWS][MAX_COLS], const int elevations[MAX_ROWS][MAX_COLS], 
                    int rows, int cols, int max, int min) {

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) { //adds value of pixel to each variable
            image[i][j].red = scaleValue(elevations[i][j], max, min);
            image[i][j].blue = scaleValue(elevations[i][j], max, min);
            image[i][j].green = scaleValue(elevations[i][j], max, min);

        }
    }
    
}

void outputImage(const Pixel image[MAX_ROWS][MAX_COLS], int rows, int cols, ostream& outData) {
    outData << "P3" << endl; 
    outData << cols << " " << rows << endl;
    outData << 255 <<endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            outData << image[i][j].red << endl << image[i][j].green << endl << image[i][j].blue << endl;
        }
    }

}

int colorPath(const int elevations[MAX_ROWS][MAX_COLS], Pixel image[MAX_ROWS][MAX_COLS], int rows, int cols, Pixel color, int start_row) {
    int elevation = 0;
    int currentRow = start_row;
    int NE = 0;
    int E = 0;
    int SE = 1;
    int path1;
    int path2;
    int path3;

    for (int column = 0; column < cols - 1 ; column++) {
        if (currentRow == 0) { //case for top row
            int NE = 0;
            int E = 0;
            int SE = 1;

            if (abs(elevations[SE][column + 1] - elevations[E][column]) < abs(elevations[E][column + 1] - elevations[E][column])) { //southeast is less the straight
                elevation += abs(elevations[SE][column + 1] - elevations[E][column]);
                image[SE][column + 1] = color;
                image[E][column] = color;

                NE = 0;
                E = 1;
                SE = 2;
                currentRow = E;

            }

            else {
               elevation += abs(elevations[E][column + 1] - elevations[E][column]);
               image[E][column + 1] = color;
               image[E][column] = color;

               NE = 0;
               E = 0;
               SE = 1;
               currentRow = E;

            }
        }

        else if (currentRow == rows - 1) { //case for bottom row
            NE = rows - 2; //makes sure values are holding right numbers
            E = rows - 1;
            SE = rows;
            if (abs(elevations[NE][column + 1] - elevations[E][column]) < abs(elevations[E][column + 1] - elevations[E][column])) { //Northeast is less the straight
                elevation += abs(elevations[NE][column + 1] - elevations[E][column]);
                
                image[NE][column + 1] = color;
                image[E][column] = color;

                NE = rows - 3;
                E = rows - 2;
                SE = rows - 1;
                currentRow = E;

            }

            else {
               elevation += abs(elevations[E][column + 1] - elevations[E][column]);
               
               image[E][column + 1] = color;
               image[E][column] = color;

               NE = rows - 2;
               E = rows - 1;
               SE = rows;
               currentRow = E;

               

            }

        }

        else { //case for middle rows
            NE = currentRow - 1;
            E = currentRow;
            SE = currentRow + 1;
            path1 = abs(elevations[NE][column + 1] - elevations[E][column]);
            path2 = abs(elevations[E][column + 1] - elevations[E][column]);
            path3 = abs(elevations[SE][column + 1] - elevations[E][column]);

            if ((path1 < path2) && (path1 < path3)) { //NE movement
                elevation += path1;

                NE = currentRow - 2;
                E = currentRow - 1;
                SE = currentRow;

                image[E][column + 1] = color;
                image[currentRow][column] = color;

                currentRow = E;


            }

            else if ((path3 < path2) && (path3 <= path1)) { //SE movement
                elevation += path3;

                NE = currentRow;
                E = currentRow + 1;
                SE = currentRow + 2;

                image[E][column + 1] = color;
                image[currentRow][column] = color;

                currentRow = E;
            }

            else { //E movement
                elevation += path2;
                image[E][column + 1] = color;
                image[currentRow][column] = color;

                

            }





        }

    }

    
    return elevation;
}


