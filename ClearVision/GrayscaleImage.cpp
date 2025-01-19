#include "GrayscaleImage.h"
#include <iostream>
#include <cstring>  // For memcpy
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>
using namespace std;

// Constructor: load from a file
GrayscaleImage::GrayscaleImage(const char* filename) {

    // Image loading code using stbi
    int channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_grey);

    if (image == nullptr) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        exit(1);

    } data=new int * [height];
    for(int i=0;i<height;i++){
       data[i]=new int[width];
    }for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            data[i][j]=image[i*width+j];
        }
    }
    // Dynamically allocate memory for a 2D matrix based on the given dimensions.
    // Fill the matrix with pixel values from the image

    // Free the dynamically allocated memory of stbi image
    stbi_image_free(image);
}

// Constructor: initialize from a pre-existing data matrix
GrayscaleImage::GrayscaleImage(int** inputData, int h, int w) {
    height=h;
    width=w;
    data=new int*[h];
    for(int i=0;i<h;i++){
       data[i]=new int[w];
    }for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
         data[i][j]=inputData[i][j];
        }
    // Initialize the image with a pre-existing data matrix by copying the values.
    // Don't forget to dynamically allocate memory for the matrix.
}}

// Constructor to create a blank image of given width and height
GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h) {
    data=new int*[h];
    for(int i=0;i<h;i++){
        data[i]=new int[w];}

    // Just dynamically allocate the memory for the new matrix.
}

// Copy constructor
GrayscaleImage::GrayscaleImage(const GrayscaleImage& other) {
    this->width=other.width;
    this->height=other.height;
    data=new int * [height];
    for(int i=0;i<height;i++){
        this->data[i]=new int[width];}
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            this->data[i][j]=other.data[i][j];
        }
    }

    // Copy constructor: dynamically allocate memory and 
    // copy pixel values from another image.
}

// Destructor
GrayscaleImage::~GrayscaleImage() {
    for (int i = 0; i < height; ++i) {
        delete[] data[i];
    }
    delete[] data;


}

// Equality operator
bool GrayscaleImage::operator==(const GrayscaleImage& other) const {
    if (this->width != other.width || this->height != other.height) {
        return false;
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (this->data[i][j] != other.data[i][j]) {
                return false;
            }
        }
    }
    // Check if two images have the same dimensions and pixel values.
    // If they do, return true.
    return true;
}

// Addition operator
GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage& other) const {
    // Create a new image for the result
    GrayscaleImage result(width, height);
    for(int i=0;i<other.height;i++){
        for(int j=0;j<other.width;j++){
            if (this->data[i][j] + other.data[i][j]<255) {
                result.data[i][j]=this->data[i][j] + other.data[i][j] ;
            }else{
                result.data[i][j]=255;
            }
        }
    }
    // Add two images' pixel values and return a new image, clamping the results.

    return result;
}

// Subtraction operator
GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage& other) const {
    // Create a new image for the result
    GrayscaleImage result(width, height);
    for(int i=0;i<other.height;i++){
        for(int j=0;j<other.width;j++){
            if (this->data[i][j] - other.data[i][j]>0) {
                result.data[i][j] = this->data[i][j] - other.data[i][j];
            }else{
            result.data[i][j]=0;}
        }}
    // Subtract pixel values of two images and return a new image, clamping the results.

    return result;
}

// Get a specific pixel value
int GrayscaleImage::get_pixel(int row, int col) const {
    return data[row][col];
}

// Set a specific pixel value
void GrayscaleImage::set_pixel(int row, int col, int value) {
    data[row][col] = value;
}

// Function to save the image to a PNG file
void GrayscaleImage::save_to_file(const char* filename) const {
    // Create a buffer to hold the image data in the format stb_image_write expects
    auto* imageBuffer = new unsigned char[width * height];

    // Fill the buffer with pixel data (convert int to unsigned char)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageBuffer[i * width + j] = static_cast<unsigned char>(data[i][j]);
        }
    }

    // Write the buffer to a PNG file
    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width)) {
        std::cerr << "Error: Could not save image to file " << filename << std::endl;
    }

    // Clean up the allocated buffer
    delete[] imageBuffer;
}
