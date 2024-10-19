#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>

float gaussian_2d_weight_finder(int x, int y, double sigma)
{
    return (1.0 / (2 * M_PI * sigma * sigma)) * exp(-(x * x + y * y) / (2 * sigma * sigma));
}

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage &image, int kernelSize)
{
    // TODO: Your code goes here.
    // 1. Copy the original image for reference.
    GrayscaleImage copyImage = image;
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    for (int i = 0; i < image.get_height(); i++)
    {
        for (int j = 0; j < image.get_width(); j++)
        {
            // creating a vector to store the pixel values of the neighbours
            // i am using a vector because the number of neighbours can differ from place to place (for example edges).
            std::vector<int> neighbours;

            // iterating over the image using kernel size.

            // notes bcz im a little bit stupid
            // i - kernelSize / 2 plus j - kernelSize / 2 == top left
            // i + kernelSize / 2 plus j + kernelSize / 2 == bottom right
            // end of notes
            for (int a = i - kernelSize / 2; a <= i + kernelSize / 2; a++)
            {
                for (int b = j - kernelSize / 2; b <= j + kernelSize / 2; b++)
                {
                    // checking if the pixel is inside the image, if so add the pixel value to vector.
                    if (a >= 0 && a < image.get_height() && b >= 0 && b < image.get_width())
                    {
                        neighbours.push_back(copyImage.get_pixel(a, b));
                    }
                }
            }

            // mean
            // using accumulate (i learned it from stackoverflow xD its a cool function)
            int mean = std::accumulate(neighbours.begin(), neighbours.end(), 0) / neighbours.size(); // its a c++3 function and might not work will try later.

            // 3. Update each pixel with the computed mean.
            // copyImage.set_pixel(i, j, mean); // wrong image
            image.set_pixel(i, j, mean);
        }
    }
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage &image, int kernelSize, double sigma)
{
    // TODO: Your code goes here.
    // 1. Create a Gaussian kernel based on the given sigma value.
    // 2. Normalize the kernel to ensure it sums to 1.
    // 3. For each pixel, compute the weighted sum using the kernel.
    // 4. Update the pixel values with the smoothed results.

    GrayscaleImage copyImage = image;

    std::vector<std::vector<float>> kernel(kernelSize, std::vector<float>(kernelSize)); // should be a 2d vector to store the weights of the kernel according to the distance from the center.

    // calculating the weights of the kernel
    for (int i = 0; i < kernelSize; i++)
    {
        for (int j = 0; j < kernelSize; j++)
        {
            kernel[i][j] = gaussian_2d_weight_finder(i - kernelSize / 2, j - kernelSize / 2, sigma);
        }
    }

    // accumulate doesn't work??
    unsigned int sum = 0;
    for (int i = 0; i < kernelSize; i++)
    {
        for (int j = 0; j < kernelSize; j++)
        {
            sum += kernel[i][j];
        }
    }

    // normalizing the kernel
    for (int i = 0; i < kernelSize; i++)
    {
        for (int j = 0; j < kernelSize; j++)
        {
            kernel[i][j] /= sum;
        }
    }

    // applying the filter
    for (int i = 0; i < image.get_height(); i++)
    {
        for (int j = 0; j < image.get_width(); j++)
        {
            int **neighbours = new int *[kernelSize];
            for (int a = 0; a < kernelSize; a++)
            {
                neighbours[a] = new int[kernelSize];
            }

            // setting the values to neighbours array
            for (int a = i - kernelSize / 2; a <= i + kernelSize / 2; a++)
            {
                for (int b = j - kernelSize / 2; b <= j + kernelSize / 2; b++)
                {
                    // if a and b are inside the image seting the pixel value to the neighbours matrix
                    if (a >= 0 && a < image.get_height() && b >= 0 && b < image.get_width())
                    {
                        neighbours[a - i + kernelSize / 2][b - j + kernelSize / 2] = copyImage.get_pixel(a, b);
                    }
                    else
                    {
                        // if a and b are outside the image set the pixel value to 0
                        neighbours[a - i + kernelSize / 2][b - j + kernelSize / 2] = 0;
                    }
                }
            }

            // calculating the weighted sum
            int weightedSum = 0;
            for (int a = 0; a < kernelSize; a++)
            {
                for (int b = 0; b < kernelSize; b++)
                {
                    weightedSum += neighbours[a][b] * kernel[a][b];
                }
            }

            // updating the current images pixel value
            image.set_pixel(i, j, (int)weightedSum);
        }
    }
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage &image, int kernelSize, double amount)
{
    // TODO: Your code goes here.
    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    GrayscaleImage copyImage = image;
    Filter::apply_gaussian_smoothing(copyImage, kernelSize, 1.0);
    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    for (int i = 0; i < image.get_height(); i++)
    {
        for (int j = 0; j < image.get_width(); j++)
        {
            int original = image.get_pixel(i, j);
            int blurred = copyImage.get_pixel(i, j);
            int unsharpMask = original + amount * (original - blurred);
            // 3. Clip values to ensure they are within a valid range [0-255].
            if (unsharpMask < 0)
            {
                unsharpMask = 0;
            }
            else if (unsharpMask > 255)
            {
                unsharpMask = 255;
            }
            image.set_pixel(i, j, unsharpMask);
        }
    }
}
