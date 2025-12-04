#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "../src/lodepng/lodepng.h"

// Structure to hold image data
struct ImageData {
    std::vector<unsigned char> pixels;
    unsigned int width;
    unsigned int height;
};

// Load a PNG image
bool loadImage(const std::string& filename, ImageData& image) {
    unsigned error = lodepng::decode(image.pixels, image.width, image.height, filename);
    
    if (error) {
        std::cerr << "Error loading image " << filename << ": " 
                  << lodepng_error_text(error) << std::endl;
        return false;
    }
    
    return true;
}

// Compare two images with a tolerance threshold
bool compareImages(const ImageData& img1, const ImageData& img2, double tolerance = 0.01) {
    // Check dimensions
    if (img1.width != img2.width || img1.height != img2.height) {
        std::cerr << "Image dimensions do not match: " 
                  << img1.width << "x" << img1.height << " vs " 
                  << img2.width << "x" << img2.height << std::endl;
        return false;
    }
    
    // Check pixel count
    if (img1.pixels.size() != img2.pixels.size()) {
        std::cerr << "Image buffer sizes do not match" << std::endl;
        return false;
    }
    
    unsigned int pixelCount = img1.width * img1.height;
    unsigned int differentPixels = 0;
    double totalDifference = 0.0;
    
    // Compare each pixel
    for (unsigned int i = 0; i < pixelCount; i++) {
        unsigned int offset = i * 4;
        
        // Calculate difference for each channel (RGB, ignore alpha)
        double rDiff = std::abs(img1.pixels[offset] - img2.pixels[offset]) / 255.0;
        double gDiff = std::abs(img1.pixels[offset + 1] - img2.pixels[offset + 1]) / 255.0;
        double bDiff = std::abs(img1.pixels[offset + 2] - img2.pixels[offset + 2]) / 255.0;
        
        double pixelDiff = (rDiff + gDiff + bDiff) / 3.0;
        
        if (pixelDiff > tolerance) {
            differentPixels++;
            totalDifference += pixelDiff;
        }
    }
    
    // Calculate percentage of different pixels
    double percentDifferent = (double)differentPixels / pixelCount * 100.0;
    double avgDifference = differentPixels > 0 ? totalDifference / differentPixels : 0.0;
    
    std::cout << "Comparison results:" << std::endl;
    std::cout << "  Total pixels: " << pixelCount << std::endl;
    std::cout << "  Different pixels: " << differentPixels << " (" 
              << percentDifferent << "%)" << std::endl;
    
    if (differentPixels > 0) {
        std::cout << "  Average difference: " << avgDifference << std::endl;
    }
    
    // Consider images equal if less than 0.1% of pixels differ
    if (percentDifferent < 0.1) {
        std::cout << "Images are considered EQUAL (within tolerance)" << std::endl;
        return true;
    } else {
        std::cout << "Images are DIFFERENT (exceeds tolerance)" << std::endl;
        return false;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <image1.png> <image2.png>" << std::endl;
        return 1;
    }
    
    std::string file1 = argv[1];
    std::string file2 = argv[2];
    
    std::cout << "Comparing images:" << std::endl;
    std::cout << "  Image 1: " << file1 << std::endl;
    std::cout << "  Image 2: " << file2 << std::endl;
    std::cout << std::endl;
    
    ImageData img1, img2;
    
    if (!loadImage(file1, img1)) {
        return 1;
    }
    
    if (!loadImage(file2, img2)) {
        return 1;
    }
    
    bool result = compareImages(img1, img2);
    
    return result ? 0 : 1;
}
