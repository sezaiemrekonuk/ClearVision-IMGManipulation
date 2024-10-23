#include "Crypto.h"
#include "GrayscaleImage.h"

// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage &secret_image, int message_length)
{
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Reconstruct the SecretImage to a GrayscaleImage. (##pre-existing matrix)
    GrayscaleImage reconstructed_image = secret_image.reconstruct();

    // 2. Calculate the image dimensions.
    int image_width = reconstructed_image.get_width();
    int image_height = reconstructed_image.get_height();

    // 3. Determine the total bits required based on message length.
    // (PDF) Convert the ASCII characters of the message into a binary bitstream, where each character is represented by 7 bits.

    int total_bits = message_length * 7;

    // 4. Ensure the image has enough pixels; if not, throw an error.
    int total_pixels = image_width * image_height;
    if (total_bits > total_pixels)
    {
        throw std::invalid_argument("Lol ur message is too long, get gud.");
    }

    // 5. Calculate the starting pixel from the message_length knowing that
    //    the last LSB to extract is in the last pixel of the image.
    int starting_pixel = total_pixels - total_bits;

    // 6. Extract LSBs from the image pixels and return the result.
    for (int i = 0; i < total_bits; i++)
    {

        int curr_pixel = starting_pixel + i;
        int row = curr_pixel / image_width;
        int col = curr_pixel % image_width;

        int pixel = reconstructed_image.get_pixel(row, col);

        // The value at the least significant bit position = x & 1 (<3 stackoverflow)
        int lsb = pixel & 1;

        LSB_array.push_back(lsb);
    }

    return LSB_array;
}

// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int> &LSB_array)
{
    std::string message;
    // TODO: Your code goes here.

    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    if (LSB_array.size() % 7 != 0)
    {
        throw std::invalid_argument("Array size bad (should be a multiple of 7).");
    }
    // 2. Convert each group of 7 bits into an ASCII character.
    for (int i = 0; i < LSB_array.size(); i += 7)
    {
        std::bitset<7> bits;
        for (int j = 0; j < 7; j++)
        {
            bits[6 - j] = LSB_array[i + j];
        }

        // 3. Collect the characters to form the decrypted message.
        char character = static_cast<char>(bits.to_ulong());
        message += character;
    }

    // 4. Return the resulting message.
    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string &message)
{
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    // for (int i = 0; i < message.size(); i++)
    for (char c : message)
    {
        std::bitset<7> bits(c);

        // std::cout << bits << std::endl;

        // 2. Collect the bits into the LSB array.
        for (int j = 6; j >= 0; --j) // changed the loop direction
        {
            LSB_array.push_back(bits[j]);
        }
    }

    // 3. Return the array of bits.
    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage &image, const std::vector<int> &LSB_array)
{
    // TODO: Your code goes here.

    // std::cout << "#sf test1" << std::endl;
    int width = image.get_width();
    int height = image.get_height();
    int lsb_size = LSB_array.size();

    int total_pixels = width * height;
    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    if (lsb_size > total_pixels)
    {
        throw std::invalid_argument("Image too small, cant store the message");
    }

    // 2. Find the starting pixel based on the message length knowing that
    //    the last LSB to embed should end up in the last pixel of the image.
    int starting_pixel = total_pixels - lsb_size;

    // 3. Iterate over the image pixels, embedding LSBs from the array.
    for (size_t i = 0; i < lsb_size; i++)
    {
        int curr_pixel = starting_pixel + i;
        int row = curr_pixel / width;
        int col = curr_pixel % width;

        int pixel = image.get_pixel(row, col);

        // clearing the lsb
        pixel &= ~1;

        // setting the lsb of pixel using array
        pixel |= LSB_array[i];

        image.set_pixel(row, col, pixel);
    }

    // 4. Return a SecretImage object constructed from the given GrayscaleImage
    //    with the embedded message.
    // std::cout << ":sf test1" << std::endl;

    return SecretImage(image);
}
