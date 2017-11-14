//Nicolas Stoian
//CS780 Image Processing
//Project 8.1 - Line Drawing

//This program needs 2 command line arguments
//argv[1] "output1" for the result of the line-drawing in a 128 X 128 binary image array with header information
//argv[2] "output2" for pretty print of the result of the binary image array

#include <iostream>
#include <fstream>
#include <cmath>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
using namespace std;

class ImageProcessing{ friend class LineDrawing;
private:
    int** imgAry;
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int rowTranslate;
    int colTranslate;

public:
    ImageProcessing();
    ~ImageProcessing();
    static string mapInt2Char(int theInt);
    void prettyPrint(ofstream& outFile);
    void printImage(ofstream& outFile);
};

class LineDrawing{
private:
    ImageProcessing* image;
    double angleInDegree;
    double angleInRadians;

public:
    LineDrawing();
    LineDrawing(ImageProcessing* image);
    ~LineDrawing();
    void drawLine(double angle);
    int translateRow(int row);
    int translateCol(int col);
    int translateRowBack(int row);
    int translateColBack(int col);
};

int main(int argc, char* argv[]){
    ImageProcessing* image = new ImageProcessing();
    LineDrawing* lineDraw = new LineDrawing(image);
    double angle;
    string ans = "y";
    while(ans == "y"){
        cout << "Enter an angle for the line you would like to draw: ";
        cin >> angle;
        lineDraw->drawLine(angle);
        cout << "Would you like to draw another line? (y/n): ";
        cin >> ans;
    }
    ofstream outFile1;
    outFile1.open(argv[1]);
    image->printImage(outFile1);
    outFile1.close();
    ofstream outFile2;
    outFile2.open(argv[2]);
    image->prettyPrint(outFile2);
    outFile2.close();
}

ImageProcessing::ImageProcessing(): numRows(128), numCols(128), minVal(0), maxVal(1){
    imgAry = new int* [numRows];
    for(int i = 0; i < numRows; i++){
        imgAry[i] = new int [numCols];
    }
    for(int row = 0; row < numRows; row++){
        for(int col = 0; col < numCols; col++){
            imgAry[row][col] = 0;
        }
    }
    rowTranslate = numRows / 2;
    colTranslate = numCols / 2;
}

ImageProcessing::~ImageProcessing(){
    if(imgAry != NULL){
        for(int i = 0; i < numRows; i++){
            delete [] imgAry[i];
        }
    }
    delete [] imgAry;
}

string ImageProcessing::mapInt2Char(int theInt){
    char toReturn [33];
    sprintf(toReturn, "%d", theInt);
    return toReturn;
}

void ImageProcessing::prettyPrint(ofstream& outFile){
    for(int row = numRows - 1; row >= 0; row--){
        for(int col = 0; col < numCols; col++){
            if(imgAry[row][col] <= 0){
                outFile << " " << " ";
            }
            else{
                outFile << "*" << " ";
            }
        }
        outFile << endl;
    }
}

void ImageProcessing::printImage(ofstream& outFile){
    outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
    for(int row = numRows - 1; row >= 0; row--){
        for(int col = 0; col < numCols; col++){
            outFile << mapInt2Char(imgAry[row][col]) << " ";
        }
        outFile << endl;
    }
}

LineDrawing::LineDrawing(): image(NULL), angleInDegree(0.0){
    angleInRadians = angleInDegree * M_PI / 180.0;
}

LineDrawing::LineDrawing(ImageProcessing* image): angleInDegree(0.0){
    this->image = image;
    angleInRadians = angleInDegree * M_PI / 180.0;
}

LineDrawing::~LineDrawing(){
    image = NULL;
    delete image;
}

void LineDrawing::drawLine(double angle){
    angleInDegree = angle;
    angleInRadians = angleInDegree * M_PI / 180.0;
    double slope = tan(angleInRadians);
    for(int x = translateCol(0); x < translateCol(image->numCols); x++){
        double y = x * slope;
        if(translateRowBack((int)(round(y))) >= 0 && translateRowBack((int)(round(y))) < image->numRows){
            image->imgAry[translateRowBack((int)(round(y)))][translateColBack(x)] = 1;
        }
    }
}

int LineDrawing::translateRow(int row){
    return row - image->rowTranslate;
}

int LineDrawing::translateCol(int col){
    return col - image->colTranslate;
}

int LineDrawing::translateRowBack(int row){
    return row + image->rowTranslate;
}

int LineDrawing::translateColBack(int col){
    return col + image->colTranslate;
}
