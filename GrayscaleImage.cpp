#include "GrayscaleImage.h"
#include <iostream>
#include <cstring> // For memcpy
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>

// Constructor: load from a file
GrayscaleImage::GrayscaleImage(const char *filename)
{

    // Image loading code using stbi
    int channels;
    unsigned char *image = stbi_load(filename, &width, &height, &channels, STBI_grey);

    if (image == nullptr)
    {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        exit(1);
    }

    std::cout << "Loaded image " << filename << " with dimensions " << width << "x" << height << std::endl;

    // TODO: Your code goes here.
    // Dynamically allocate memory for a 2D matrix based on the given dimensions.
    data = new int *[height];
    for (int i = 0; i < height; ++i)
    {
        data[i] = new int[width];
    }

    // Fill the matrix with pixel values from the image
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            data[i][j] = image[i * width + j];
        }
    }

    // Check for if its loaded
    std::cout << "Test GrayscaleImage: " << data[0][0] << std::endl;

    // Free the dynamically allocated memory of stbi image
    stbi_image_free(image);
}

// Constructor: initialize from a pre-existing data matrix
GrayscaleImage::GrayscaleImage(int **inputData, int h, int w) : width(w), height(h)
{
    // TODO: Your code goes here.
    // Initialize the image with a pre-existing data matrix by copying the values.
    // Don't forget to dynamically allocate memory for the matrix.

    data = new int *[height];
    for (int i = 0; i < height; ++i)
    {
        data[i] = new int[width];
    }

    // value copying
    for (int i = 0; i < height; ++i)
    {
        std::memcpy(data[i], inputData[i], width * sizeof(int));
    }
}

// Constructor to create a blank image of given width and height
GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h)
{
    // TODO: Your code goes here.
    // Just dynamically allocate the memory for the new matrix.
    data = new int *[h];
    for (int i = 0; i < h; ++i)
    {
        data[i] = new int[w];
    }

    std::memset(data[0], 0, width * height * sizeof(int));
}

// Copy constructor
GrayscaleImage::GrayscaleImage(const GrayscaleImage &other) : width(other.width), height(other.height)
{
    // TODO: Your code goes here.
    // Copy constructor: dynamically allocate memory and
    // copy pixel values from another image.

    // data should be copied using a loop, because it is a pointer to a pointer.
    data = new int *[height];
    for (int i = 0; i < height; ++i)
    {
        data[i] = new int[width];
    }

    // copying the values
    for (int i = 0; i < height; ++i)
    {
        std::memcpy(data[i], other.data[i], width * sizeof(int));
    }
}

// Destructor
GrayscaleImage::~GrayscaleImage()
{
    // TODO: Your code goes here.
    // Destructor: deallocate memory for the matrix.

    // First clearing the inner arrays
    for (int i = 0; i < height; ++i)
    {
        delete[] data[i];
    }

    // Then clearing the outer array
    delete[] data;
}

// Equality operator
bool GrayscaleImage::operator==(const GrayscaleImage &other) const
{
    // TODO: Your code goes here.
    // Check if two images have the same dimensions and pixel values.
    // If they do, return true.
    // Check if two images have the same dimensions
    if (other.width != width || other.height != height)
    {
        return false;
    }

    // Compare pixel values
    for (int i = 0; i < height; ++i)
    {
        if (std::memcmp(data[i], other.data[i], width * sizeof(int)) != 0)
        {
            return false;
        }
    }

    return true;
}

// Addition operator
GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage &other) const
{
    // Create a new image for the result
    GrayscaleImage result(width, height);

    // TODO: Your code goes here.
    // Add two images' pixel values and return a new image, clamping the results.

    // Note: if it overflows 255, make it 255 again.

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            result.data[i][j] = std::min(data[i][j] + other.data[i][j], 255);
        }
    }

    return result;
}

// Subtraction operator
GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage &other) const
{
    // Create a new image for the result
    GrayscaleImage result(width, height);

    // TODO: Your code goes here.
    // Subtract pixel values of two images and return a new image, clamping the results.

    // Note: if it underflows 0, make it 0 again.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            result.data[i][j] = std::max(data[i][j] - other.data[i][j], 0);
        }
    }

    return result;
}

// Get a specific pixel value
int GrayscaleImage::get_pixel(int row, int col) const
{
    return data[row][col];
}

// Set a specific pixel value
void GrayscaleImage::set_pixel(int row, int col, int value)
{
    data[row][col] = value;
}

// Function to save the image to a PNG file
void GrayscaleImage::save_to_file(const char *filename) const
{
    // Create a buffer to hold the image data in the format stb_image_write expects
    unsigned char *imageBuffer = new unsigned char[width * height];

    // Fill the buffer with pixel data (convert int to unsigned char)
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            imageBuffer[i * width + j] = static_cast<unsigned char>(data[i][j]);
        }
    }

    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        std::cerr << "Error: Cannot open file " << filename << " for writing." << std::endl;
    }
    else
    {
        fclose(file);
    }

    // Write the buffer to a PNG file
    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width))
    {
        std::cerr << "Error: Could not save image to file " << filename << std::endl;
    }

    // Clean up the allocated buffer
    delete[] imageBuffer;
}
