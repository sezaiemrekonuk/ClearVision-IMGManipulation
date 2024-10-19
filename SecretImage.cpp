#include "SecretImage.h"
#include "fstream"

// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage &image)
{
    // TODO: Your code goes here.
    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
    // int upper_size;
    // int lower_size;

    // for (int i = 0; i < image.get_height(); i++){
    //     for (int j = i; j < image.get_width(); j++){
    //         upper_size++;
    //     }
    // }

    // for (int i = 0; i < image.get_height(); i++){
    //     for (int j = 0; j < i; j++){
    //         lower_size++;
    //     }
    // }

    // upper_triangular = new int[upper_size];
    // lower_triangular = new int[lower_size];

    // it has higher space complexity but easier to access, so i chose this.
    upper_triangular = new int[image.get_width() * image.get_height()];
    lower_triangular = new int[image.get_width() * image.get_height()];

    // 2. Fill both matrices with the pixels from the GrayscaleImage.
    // upper part:
    for (int i = 0; i < image.get_height(); i++)
    {
        for (int j = i; j < image.get_width(); j++)
        {
            upper_triangular[i * image.get_width() + j] = image.get_pixel(j, i);
        }
    }

    // lower part:
    for (int i = 0; i < image.get_height(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            lower_triangular[i * image.get_width() + j] = image.get_pixel(j, i);
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int *upper, int *lower)
{
    // TODO: Your code goes here.
    // Since file reading part should dynamically allocate upper and lower matrices.
    // You should simply copy the parameters to instance variables.
    width = w;
    height = h;
    upper_triangular = upper;
    lower_triangular = lower;
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

    // TODO: Your code goes here.
    // fitting the upper part
    for (int i = 0; i < height; i++)
    {
        for (int j = i; j < width; j++)
        {
            image.set_pixel(j, i, upper_triangular[(width * i) + j]);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < i; j++)
        {
            image.set_pixel(j, i, lower_triangular[(width * i) + j]);
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

    // upper part
    for (int i = 0; i < height; i++)
    {
        for (int j = i; j < width; j++)
        {
            upper_triangular[(width * i) + j] = image.get_pixel(j, i);
        }
    }

    // lower part
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < i; j++)
        {
            lower_triangular[(width * i) + j] = image.get_pixel(j, i);
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
    // 2. Write the upper_triangular array to the second line.
    for (int i = 0; i < height; i++)
    {
        for (int j = i; j < width; j++)
        {
            file << upper_triangular[(width * i) + j] << " ";
        }
    }
    file << std::endl;

    // Ensure that the elements are space-separated.
    // If there are 15 elements, write them as: "element1 element2 ... element15"
    // 3. Write the lower_triangular array to the third line in a similar manner
    // as the second line.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < i; j++)
        {
            file << lower_triangular[(width * i) + j] << " ";
        }
    }
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
    std::string line;
    std::getline(file, line);

    // getting width and height from the line, first converting it to a string stream
    std::istringstream iss(line);

    int w, h;
    iss >> w >> h;
    // 2. Calculate the sizes of the upper and lower triangular arrays.
    int upper_size = w * h, lower_size = w * h;

    // 3. Allocate memory for both arrays.
    int *upper = new int[upper_size];
    int *lower = new int[lower_size];

    // 4. Read the upper_triangular array from the second line, space-separated.
    std::getline(file, line);
    std::istringstream issut(line);

    for (int i = 0; i < h; i++)
    {
        for (int j = i; j < w; j++)
        {
            iss >> upper[(w * i) + j];
        }
    }

    std::getline(file, line);
    std::istringstream isslt(line);

    // 5. Read the lower_triangular array from the third line, space-separated.
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < i; j++)
        {
            iss >> lower[(w * i) + j];
        }
    }
    // 6. Close the file and return a SecretImage object initialized with the
    file.close();

    //    width, height, and triangular arrays.
    SecretImage secret_image(w, h, upper, lower);
    return secret_image;
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
