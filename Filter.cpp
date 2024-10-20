#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>

double gaussian_2d_weight_finder(int x, int y, double sigma)
{
    return (1.0 / (2 * M_PI * sigma * sigma)) * exp(-(x * x + y * y) / (2 * sigma * sigma));
}

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage &image, int kernelSize)
{
    // TODO: Your code goes here.
    // 1. Copy the original image for reference.
    GrayscaleImage copyImage(image);
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    int halfKernelSize = kernelSize / 2;

    for (int i = 0; i < image.get_height(); i++)
    {
        for (int j = 0; j < image.get_width(); j++)
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
                    if (a >= 0 && a < image.get_height() && b >= 0 && b < image.get_width())
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

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage &image, int kernelSize, double sigma)
{
    // TODO: Your code goes here.
    // 1. Create a Gaussian kernel based on the given sigma value.
    // 2. Normalize the kernel to ensure it sums to 1.
    // 3. For each pixel, compute the weighted sum using the kernel.
    // 4. Update the pixel values with the smoothed results.

    GrayscaleImage copyImage = image;

    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize)); // should be a 2d vector to store the weights of the kernel according to the distance from the center.

    // calculating the weights of the kernel
    for (int i = 0; i < kernelSize; i++)
    {
        for (int j = 0; j < kernelSize; j++)
        {
            kernel[i][j] = gaussian_2d_weight_finder(i - kernelSize / 2, j - kernelSize / 2, sigma);
        }
    }

    // accumulate doesn't work??
    double sum = 0.0;
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

    for (int i = 0; i < image.get_height(); i++)
    {
        for (int j = 0; j < image.get_width(); j++)
        {
            double weightedSum = 0.0;

            for (int a = i - kernelSize / 2; a <= i + kernelSize / 2; a++)
            {
                for (int b = j - kernelSize / 2; b <= j + kernelSize / 2; b++)
                {
                    if (a >= 0 && a < image.get_height() && b >= 0 && b < image.get_width())
                    {
                        weightedSum += copyImage.get_pixel(a, b) * kernel[a - i + kernelSize / 2][b - j + kernelSize / 2];
                    }
                }
            }

            image.set_pixel(i, j, static_cast<int>(std::floor(weightedSum)));
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
