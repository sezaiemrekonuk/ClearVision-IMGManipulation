#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>

/**
 * @brief Calculate the weight of a 2D Gaussian function at a given position, which is in a kernel.
 * 
 * @param x 
 * @param y 
 * @param sigma 
 * @return double, the weight
 */
double gaussian_2d_weight_finder(int x, int y, double sigma)
{
    return (1.0 / (2 * M_PI * sigma * sigma)) * exp(-(x * x + y * y) / (2 * sigma * sigma));
}

/**
 * @brief Apply a mean filter to the image using the given kernel size.
 * 
 * @param image 
 * @param kernelSize 
 */
void Filter::apply_mean_filter(GrayscaleImage &image, int kernelSize)
{
    // TODO: Your code goes here.
    // 1. Copy the original image for reference.
    GrayscaleImage copyImage(image);
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    int halfKernelSize = kernelSize / 2;
    int width = image.get_width();
    int height = image.get_height();

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sum = 0;
            int count = 0;
            // notes bcz im a little bit stupid
            // i - kernelSize / 2 plus j - kernelSize / 2 == top left
            // i + kernelSize / 2 plus j + kernelSize / 2 == bottom right
            // end of notes
            for (int a = i - halfKernelSize; a <= i + halfKernelSize; a++)
            {
                for (int b = j - halfKernelSize; b <= j + halfKernelSize; b++)
                {
                    // checking if the pixel is inside the image, if so add the pixel value to vector.
                    if (a >= 0 && a < height && b >= 0 && b < width)
                    {
                        sum += copyImage.get_pixel(a, b);
                    }
                    count++;
                }
            }

            // 3. Update each pixel with the computed mean.
            int mean = sum / count;
            // copyImage.set_pixel(i, j, mean); // wrong image
            image.set_pixel(i, j, mean);
        }
    }
}

/**
 * @brief Gaussian smoothing filter
 * 
 * @param image reference
 * @param kernelSize 
 * @param sigma 
 */
void Filter::apply_gaussian_smoothing(GrayscaleImage &image, int kernelSize, double sigma)
{
    // TODO: Your code goes here.
    // 1. Create a Gaussian kernel based on the given sigma value.
    // 2. Normalize the kernel to ensure it sums to 1.
    // 3. For each pixel, compute the weighted sum using the kernel.
    // 4. Update the pixel values with the smoothed results.

    GrayscaleImage copyImage = image;

    int height = image.get_height();
    int width = image.get_width();

    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize)); // should be a 2d vector to store the weights of the kernel according to the distance from the center.

    int halfKernelSize = kernelSize / 2;
    double sum = 0.0;

    // calculating the weights of the kernel
    for (int i = 0; i < kernelSize; i++)
    {
        for (int j = 0; j < kernelSize; j++)
        {
            kernel[i][j] = gaussian_2d_weight_finder(i - halfKernelSize, j - halfKernelSize, sigma);
            sum += kernel[i][j];
        }
    }

    // normalizing the kernel
    double inverseSum = 1.0 / sum;
    for (int i = 0; i < kernelSize; i++)
    {
        for (int j = 0; j < kernelSize; j++)
        {
            kernel[i][j] *= inverseSum;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double weightedSum = 0.0;

            for (int a = i - halfKernelSize; a <= i + halfKernelSize; a++)
            {
                for (int b = j - halfKernelSize; b <= j + halfKernelSize; b++)
                {
                    if (a >= 0 && a < height && b >= 0 && b < width)
                    {
                        weightedSum += copyImage.get_pixel(a, b) * kernel[a - i + halfKernelSize][b - j + halfKernelSize];
                    }
                }
            }

            image.set_pixel(i, j, static_cast<int>(std::floor(weightedSum)));
        }
    }

    for (int i = 0; i < kernelSize; i++)
    {
        kernel[i].clear();
    }
    kernel.clear();
}

/**
 * @brief Unsharp masking filter
 * 
 * @param image reference
 * @param kernelSize 
 * @param amount 
 */
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
            double unsharpMask = original + amount * (original - blurred); // Ensure this is a double

            // 3. Clip values to ensure they are within a valid range [0-255].
            if (unsharpMask < 0)
            {
                unsharpMask = 0;
            }
            else if (unsharpMask > 255.0)
            {
                unsharpMask = 255.0;
            }
            image.set_pixel(i, j, static_cast<int>(std::floor(unsharpMask)));
        }
    }
}
