#include "SecretImage.h"
#include <fstream>
#include <iostream>
#include "Filter.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    this->width=image.get_width();
    this->height=image.get_height();
    upper_triangular=new int[width*(width+1)/2];
    lower_triangular=new int[height*(height-1)/2];

    for(int i=0; i < image.get_height() ; i++){
        for(int j=0;j<image.get_width(); j++){
            if(i<=j){
                upper_triangular[i * (2 * width - i + 1) / 2 + (j - i)]=image.get_pixel(i,j);
            }
            else{
                lower_triangular[(i - 1) * i / 2 + j]=image.get_pixel(i,j);
            }
        }
    }

    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
    // 2. Fill both matrices with the pixels from the GrayscaleImage.

}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    this->width=w;
    this->height=h;
    upper_triangular=upper;
    lower_triangular=lower;
   // Adjust size based on triangular matrix
    // Since file reading part should dynamically allocate upper and lower matrices.
    // You should simply copy the parameters to instance variables.
}


// Destructor: free the arrays
SecretImage::~SecretImage(){
    delete[] upper_triangular;
    delete[] lower_triangular;

    // Simply free the dynamically allocated memory
    // for the upper and lower triangular matrices.
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(i<=j) {
                image.set_pixel(i, j, upper_triangular[i * (2 * width - i + 1) / 2 + (j - i)]);
            }
            else{
                image.set_pixel(i, j, lower_triangular[(i-1)*i/2+j]);
            }
        }
    }

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i <= j) {
                upper_triangular[i * (2 * width - i + 1) / 2 + (j - i)] = image.get_pixel(i, j);}
            else {
                    lower_triangular[(i+1)*i/2+j] = image.get_pixel(i, j);
                }
            }
        }

    // Update the lower and upper triangular matrices
    // based on the GrayscaleImage given as the parameter.
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << width << " " << height << std::endl;
        // Write the upper_triangular array to the second line
        int upperSize = width * (width + 1) / 2;
        for (int i = 0; i < upperSize; i++) {
            file << upper_triangular[i];
            if (i < upperSize - 1) {
                file << " ";
            }
        }
        file << std::endl;

        // Write the lower_triangular array to the third line
        int lowerSize = (height * (height - 1) / 2);
        for (int i = 0; i < lowerSize; i++) {
            file << lower_triangular[i];
            if (i < lowerSize - 1) {
                file << " ";
            }
        }
        file << std::endl;

        file.close();

    }
}


// 1. Write width and height on the first line, separated by a single space.
// 2. Write the upper_triangular array to the second line.
// Ensure that the elements are space-separated.
// If there are 15 elements, write them as: "element1 element2 ... element15"
// 3. Write the lower_triangular array to the third line in a similar manner
// as the second line.


// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        int w, h;
        file >> w >> h;
        int size_upper = (w * (w + 1)) / 2;
        int size_lower = ((h * (h -1)) / 2);

        int *upper = new int[size_upper];
        int *lower = new int[size_lower];

        // Load the upper triangular matrix
        for (int i = 0; i < size_upper; ++i) {
            file >> upper[i];
        }

        // Load the lower triangular matrix
        for (int i = 0; i < size_lower; ++i) {
            file >> lower[i];
        }
        file.close();
        return SecretImage(w, h, upper, lower);
    }




    // 1. Open the file and read width and height from the first line, separated by a space.
    // 2. Calculate the sizes of the upper and lower triangular arrays.
    // 3. Allocate memory for both arrays.
    // 4. Read the upper_triangular array from the second line, space-separated.
    // 5. Read the lower_triangular array from the third line, space-separated.
    // 6. Close the file and return a SecretImage object initialized with the
    //    width, height, and triangular arrays.

}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
