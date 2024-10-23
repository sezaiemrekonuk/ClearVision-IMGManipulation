#include "SecretImage.h"
#include "fstream"

// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage &image) : width(image.get_width()), height(image.get_height())
{
    // TODO: Your code goes here.
    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.

    // width = image.get_width(); // lol forgot these and lost like 30 minutes using        ^ is better
    // height = image.get_height();
    int upper_size = (width * (width + 1)) / 2;
    int lower_size = (height * (height - 1)) / 2;

    upper_triangular = new int[upper_size];
    lower_triangular = new int[lower_size];

    int upper_index = 0, lower_index = 0;
    // filling both at once:
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i <= j)
            {
                upper_triangular[upper_index++] = image.get_pixel(i, j);
            }
            else
            {
                lower_triangular[lower_index++] = image.get_pixel(i, j);
            }
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int *upper, int *lower) : width(w), height(h)
{
    // TODO: Your code goes here.
    // Since file reading part should dynamically allocate upper and lower matrices.
    // You should simply copy the parameters to instance variables.
    lower_triangular = lower;
    upper_triangular = upper;
}

// Destructor: free the arrays
SecretImage::~SecretImage()
{
    // TODO: Your code goes here.
    // Simply free the dynamically allocated memory
    // for the upper and lower triangular matrices.
    delete[] upper_triangular;
    delete[] lower_triangular;
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const
{
    GrayscaleImage image(width, height);

    int upper_index = 0, lower_index = 0;

    // both at once
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i <= j)
            {
                image.set_pixel(i, j, upper_triangular[upper_index++]);
            }
            else
            {
                image.set_pixel(i, j, lower_triangular[lower_index++]);
            }
        }
    }

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage &image)
{
    // TODO: Your code goes here.
    // Update the lower and upper triangular matrices
    // based on the GrayscaleImage given as the parameter.
    width = image.get_width();
    height = image.get_height();

    int upper_index = 0, lower_index = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i <= j)
            {
                upper_triangular[upper_index++] = image.get_pixel(i, j);
            }
            else
            {
                lower_triangular[lower_index++] = image.get_pixel(i, j);
            }
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string &filename)
{
    // TODO: Your code goes here.
    std::ofstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: File could not be opened." << std::endl;
        return;
    }
    // 1. Write width and height on the first line, separated by a single space.
    file << width << " " << height << std::endl;

    int upper_size = (width * (width + 1)) / 2;
    int lower_size = (width * (width - 1)) / 2;

    // 2. Write the upper_triangular array to the second line.
    for (int i = 0; i < upper_size; i++)
    {
        file << upper_triangular[i] << (i == upper_size - 1 ? "" : " ");
    }
    file << std::endl;

    // Ensure that the elements are space-separated.
    // If there are 15 elements, write them as: "element1 element2 ... element15"
    // 3. Write the lower_triangular array to the third line in a similar manner
    // as the second line.
    for (int i = 0; i < lower_size; i++)
    {
        file << lower_triangular[i] << (i == lower_size - 1 ? "" : " ");
    }
    file << std::endl;

    file.close();
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string &filename)
{
    // TODO: Your code goes here.
    // 1. Open the file and read width and height from the first line, separated by a space.
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: File could not be opened." << std::endl;
        return SecretImage(nullptr);
    }

    // getting the line

    // getting width and height from the line, first converting it to a string stream
    int w, h;
    file >> w >> h;
    // 2. Calculate the sizes of the upper and lower triangular arrays.
    int upper_size = (w * (w + 1)) / 2;
    int lower_size = (h * (h - 1)) / 2;

    // 3. Allocate memory for both arrays.
    int *upper = new int[upper_size];
    int *lower = new int[lower_size];

    // 4. Read the upper_triangular array from the second line, space-separated.

    for (int i = 0; i < upper_size; i++)
    {
        file >> upper[i];
    }

    // 5. Read the lower_triangular array from the third line, space-separated.
    for (int i = 0; i < lower_size; i++)
    {
        file >> lower[i];
    }

    // 6. Close the file and return a SecretImage object initialized with the
    file.close();

    //    width, height, and triangular arrays.

    return SecretImage(w, h, upper, lower);
}

// Returns a pointer to the upper triangular part of the secret image.
int *SecretImage::get_upper_triangular() const
{
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int *SecretImage::get_lower_triangular() const
{
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const
{
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const
{
    return height;
}
