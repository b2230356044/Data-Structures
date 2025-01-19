#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>
#include <iostream>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    GrayscaleImage copyImage (image) ;
   int kernelX=(kernelSize-1)/2;
   int kernelY=(kernelSize-1)/2;

   for(int i=0;i<image.get_height();i++){
       for(int j=0;j<image.get_width();j++){
           int sum=0;
           for(int k=-kernelX;k<=kernelX;k++){
               for(int l=-kernelY;l<=kernelY;l++){
                   int x=i+k;
                   int y= j+l;
                   if(y>=0 && y<image.get_width() && x>=0 && x<image.get_height()){
                       sum+=copyImage.get_pixel(x,y);
                     }else{
                          sum+=0;

                   }

               }
           }

           int mean=sum/(kernelSize*kernelSize);
           image.set_pixel(i,j,mean);


           }



   }



   }

    // 1. Copy the original image for reference.
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    // 3. Update each pixel with the computed mean.


// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // 1. Create a Gaussian kernel based on the given sigma value.
    // 2. Normalize the kernel to ensure it sums to 1.
    // 3. For each pixel, compute the weighted sum using the kernel.
    // 4. Update the pixel values with the smoothed results.
    GrayscaleImage copyImage(image);
    int kernelX = (kernelSize - 1) / 2;
    int kernelY = (kernelSize - 1) / 2;

    auto **kernel = new double *[kernelSize];
    for(int a=0;a<kernelSize;a++){
        kernel[a]=new double [kernelSize];}

    double kernelSum = 0.0;

    for (int k = -kernelX; k <= kernelX; k++) {
        for (int l = -kernelY; l <= kernelY; l++) {
            kernel[k+kernelX][l+kernelY]= (exp(-(k * k + l * l) / (2 * sigma * sigma)) / (2 *M_PI * sigma * sigma));
            kernelSum += kernel[k + kernelX][l + kernelY];

        }
    }for (int k = -kernelX; k <= kernelX; k++) {
        for (int l = -kernelY; l <= kernelY; l++) {
            kernel[k + kernelX][l + kernelY] /= kernelSum;

        }
    }


    for (int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++) {
            double Gaussian = 0.0;
            for (int k = -kernelX; k <= kernelX; k++) {
                for (int l = -kernelY; l <= kernelY; l++) {
                    int x = i + k;
                    int y = j + l;

                    if (y >= 0 && y < image.get_width() && x >= 0 && x < image.get_height()) {
                        if(kernel[k + kernelX][l + kernelY] * copyImage.get_pixel(x, y)<255&&kernel[k + kernelX][l + kernelY] * copyImage.get_pixel(x, y)>0){
                            Gaussian +=( kernel[k + kernelX][l + kernelY] * copyImage.get_pixel(x, y));}
                        else if(kernel[k + kernelX][l + kernelY] * copyImage.get_pixel(x, y)>255){
                            Gaussian+=255;
                        }


                    }
                }

            }
            image.set_pixel(i, j, Gaussian );

        }
    }
    for(int i=0;i<kernelSize;i++){
        delete[] kernel[i];}
    delete[] kernel;
}




// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage &image, int kernelSize, double amount) {
    GrayscaleImage copyImage(image);
    apply_gaussian_smoothing(copyImage, kernelSize, 1.0);
    for (int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++) {
            int original = image.get_pixel(i, j);
            int blurred = copyImage.get_pixel(i, j);
            int sharpened = original + amount * (original - blurred);
            if ( sharpened > 255) {
                image.set_pixel(i, j, 255);
            } else if (sharpened < 0) {
                image.set_pixel(i, j, 0);
            } else {
                image.set_pixel(i, j, sharpened);
            }
        }
    }
}
// 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
// 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
// 3. Clip values to ensure they are within a valid range [0-255].