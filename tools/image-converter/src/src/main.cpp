#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void createHeaderFile(const std::string& imagePath, const std::string& outputFileName) {
    int width, height, channels;
    unsigned char* image_data = stbi_load(imagePath.c_str(), &width, &height, &channels, 4);

    if (!image_data) {
        std::cerr << "Error loading image: " << imagePath << std::endl;
        return;
    }

    std::ofstream headerFile(outputFileName);

    if (!headerFile.is_open()) {
        std::cerr << "Error creating header file: " << outputFileName << std::endl;
        return;
    }

    headerFile << "#pragma once" << std::endl;
    headerFile << std::endl;
    headerFile << "static int image_width = " << width << ";" << std::endl;
    headerFile << "static int image_height = " << height << ";" << std::endl;
    headerFile << std::endl;
    headerFile << "static unsigned char image_data[] = {" << std::endl;

    // Write pixel data as an array of unsigned char
    for (int i = 0; i < width * height * channels; ++i) {
        headerFile << static_cast<int>(image_data[i]);
        if (i < width * height * channels - 1) {
            headerFile << ", ";
        }

        // Add line breaks for better readability
        if ((i + 1) % 16 == 0) {
            headerFile << std::endl;
        }
    }

    headerFile << "};" << std::endl;
    headerFile.close();

    stbi_image_free(image_data);

    std::cout << "Header file created: " << outputFileName << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_image_path> <output_header_file_name>" << std::endl;
        return 1;
    }

    std::string inputImagePath = argv[1];
    std::string outputHeaderFileName = argv[2];

    createHeaderFile(inputImagePath, outputHeaderFileName);

    return 0;
}
