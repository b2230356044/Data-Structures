#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> least_significant;
    GrayscaleImage image = secret_image.reconstruct();
    int image_size = image.get_width() * image.get_height();
    int total_bits = message_length * 7;
    if (image_size < total_bits) {
        throw std::invalid_argument("Image does not have enough pixels to extract the message.");
    }

    int starting_pixel = image_size - total_bits;

    for (int i = starting_pixel; i < image_size; ++i) {
        int x = i / image.get_width();
        int y = i % image.get_width();
        least_significant.push_back(image.get_pixel(x, y) & 1);
    }

    return least_significant;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& least_significant) {
    std::string message;

    if (least_significant.size() % 7 != 0) {
        throw std::invalid_argument("Array size is not a multiple of 7.");
    }

    for (size_t i = 0; i < least_significant.size(); i += 7) {
        std::bitset<7> bits;

        for (size_t j = 0; j < 7; ++j) {
            bits[j] = least_significant[i + (6 - j)];
        }

        char decrypted_char = static_cast<char>(bits.to_ulong());
        if (decrypted_char == '\0') {
            break;
        }

        message += decrypted_char;
    }

    return message;
}




// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> least_significant;
    for(char c : message){
        std::bitset<7> bits(c);
        for(int i=6;i>=0;--i){
            least_significant.push_back(bits[i]);
        }
    }

    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    // 2. Collect the bits into the LSB array.
    // 3. Return the array of bits.

    return least_significant;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& least_significant) {
    int image_size = image.get_width() * image.get_height();
    int total_bits = least_significant.size();
    if(image_size < total_bits) {
        throw std::invalid_argument("Image does not have enough pixels to embed the message.");
    }for(int i=0;i<total_bits;i++){
        int x = (image_size - total_bits + i) / image.get_width();
        int y = (image_size - total_bits + i) % image.get_width();
        int pixel = image.get_pixel(x, y);
        pixel = (pixel & 0b11111110) | least_significant[i];
        image.set_pixel(x, y, pixel);
    }
    return SecretImage(image);

    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    // 2. Find the starting pixel based on the message length knowing that  
    //    the last LSB to embed should end up in the last pixel of the image.
    // 3. Iterate over the image pixels, embedding LSBs from the array.
    // 4. Return a SecretImage object constructed from the given GrayscaleImage 
    //    with the embedded message.


}
