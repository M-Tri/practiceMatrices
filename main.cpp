#include <iostream>
#include <cstdio> 
#include <regex>
#include <cstdlib> 
#include <Eigen/Dense>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include "textToMatrix.h"

using namespace Eigen ;
// Function to print the matrix
void printResults(const std::string& text, const MatrixXd& A_matrix, const VectorXd& b_vector,  const VectorXd&  solution );

//Reconize and solve the system of equations.
int main(){
    // Create a Tesseract object
    tesseract::TessBaseAPI *tess = new tesseract::TessBaseAPI();
    
    //Need const char* for function arguments.
    const char* imageFileName = "/Users/amine/Documents/dev/projects/practiceMatrices/equationsImage2.jpg";
    const char* tessdataFileLocation = "/opt/homebrew/Cellar/tesseract/5.4.1/share/tessdata";

    // Initialize Tesseract-ocr with English (eng) language
    if (tess->Init(tessdataFileLocation, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        delete tess;  // Clean up
        exit(1);
    }

    Pix *image = pixRead(imageFileName);
    if (!image) {
        fprintf(stderr, "Could not read image file.\n");
        tess->End(); // Cleanup
        delete tess;
        return 1;
    }

    // Set the image for Tesseract to process
    tess->SetImage(image);

    // Get the OCR result as UTF-8 text
    char *output = tess->GetUTF8Text();
    if (!output) {
        fprintf(stderr, "Could not extract text from image.\n");
        pixDestroy(&image); // Cleanup
        tess->End(); // Cleanup
        delete tess;
        return 1;
    }

    // Convert the extracted text to a std::string
    std::string text(output);
    delete[] output; // Free the memory allocated for the output text

    // Clean up the image
    pixDestroy(&image);

    //Get the coefficients from the text.
    //Here we need an integer or the function will extract 2.0 as 2 0 and not 2, we will have extra numbers.
    std::vector< std::vector<int> > coefficentMatrix = extractInfo(text);
    int rows = coefficentMatrix.size();
    int cols = coefficentMatrix[0].size();
   
    std::vector<double> A ;
    std::vector<double> b ;
    //Place the coeffiecient of variables into a matrix.
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols-1; ++j) {
            A.push_back(static_cast<double>(coefficentMatrix[i][j]));
        }
    }
    //Place the last column into a vector. 
    for (int i = 0; i < rows; ++i) {
        for (int j = cols-1; j < cols; ++j) {
            b.push_back(static_cast<double>(coefficentMatrix[i][j]));
        }
    }

    // Create Eigen matrices with the specified dimensions
    MatrixXd A_matrix(rows, cols - 1);
    VectorXd b_vector(rows); // b is a column vector

    // Map std::vector to Eigen::MatrixXd
    Map<MatrixXd> A_map(A.data(), rows, cols - 1);
    Map<VectorXd> b_map(b.data(),rows);

    // // Copy the data from the mapped vectors to the Eigen matrices
    A_matrix = A_map;
    b_vector = b_map;
    VectorXd solution = A_matrix.colPivHouseholderQr().solve(b_vector);

    printResults(text, A_matrix, b_vector, solution);

    // Clean up
    tess->End();
    delete tess;
    return 0;
}

void printResults(const std::string& text, const MatrixXd& A_matrix, const VectorXd& b_vector,  const VectorXd&  solution ) {
    
    std::cout << std::endl ;
    std::cout << "Extracted Text:\n" << text << std::endl;
    std::cout << "Matrix A is :" << std::endl ;
    std::cout << A_matrix << std :: endl ;
    std::cout << std::endl ;
    std::cout << "Vector b is :" << std::endl ;
    std::cout << b_vector << std :: endl ;
    std::cout << std::endl ;
    std::cout << "Answer based on solving A*X = b : Vector X is " << std::endl ;
    std::cout << solution << std::endl;
    std::cout << std::endl ;

}