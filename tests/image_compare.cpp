#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "../src/lodepng/lodepng.h"

struct ImageCompareResult {
    bool identical;
    double mse;           // Mean Squared Error
    double psnr;          // Peak Signal-to-Noise Ratio
    int diffPixels;       // Number of different pixels
    double maxDiff;       // Maximum difference for any pixel
};

ImageCompareResult compareImages(const std::string& file1, const std::string& file2, double tolerance = 0.0) {
    ImageCompareResult result;
    result.identical = false;
    result.mse = 0.0;
    result.psnr = 0.0;
    result.diffPixels = 0;
    result.maxDiff = 0.0;

    std::vector<unsigned char> image1, image2;
    unsigned width1, height1, width2, height2;

    // Load images
    unsigned error1 = lodepng::decode(image1, width1, height1, file1);
    unsigned error2 = lodepng::decode(image2, width2, height2, file2);

    if (error1) {
        std::cerr << "Error loading image 1: " << lodepng_error_text(error1) << std::endl;
        return result;
    }

    if (error2) {
        std::cerr << "Error loading image 2: " << lodepng_error_text(error2) << std::endl;
        return result;
    }

    // Check dimensions
    if (width1 != width2 || height1 != height2) {
        std::cerr << "Images have different dimensions!" << std::endl;
        std::cerr << "Image 1: " << width1 << "x" << height1 << std::endl;
        std::cerr << "Image 2: " << width2 << "x" << height2 << std::endl;
        return result;
    }

    // Compare pixels
    double sumSquaredError = 0.0;
    size_t totalPixels = width1 * height1;

    for (size_t i = 0; i < image1.size(); i++) {
        double diff = std::abs(static_cast<int>(image1[i]) - static_cast<int>(image2[i]));
        
        if (diff > tolerance) {
            result.diffPixels++;
        }
        
        sumSquaredError += diff * diff;
        result.maxDiff = std::max(result.maxDiff, diff);
    }

    // Calculate MSE and PSNR
    result.mse = sumSquaredError / (totalPixels * 4); // 4 channels (RGBA)
    
    if (result.mse > 0) {
        result.psnr = 10.0 * std::log10((255.0 * 255.0) / result.mse);
    } else {
        result.psnr = INFINITY;
    }

    // Check if identical within tolerance
    result.identical = (result.diffPixels == 0);

    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <image1.png> <image2.png> [tolerance]" << std::endl;
        std::cerr << "  tolerance: optional, default=0.0 (0-255)" << std::endl;
        return 1;
    }

    std::string file1 = argv[1];
    std::string file2 = argv[2];
    double tolerance = 0.0;

    if (argc >= 4) {
        tolerance = std::stod(argv[3]);
    }

    std::cout << "Comparing images:" << std::endl;
    std::cout << "  Reference: " << file1 << std::endl;
    std::cout << "  Test:      " << file2 << std::endl;
    std::cout << "  Tolerance: " << tolerance << std::endl;
    std::cout << std::endl;

    ImageCompareResult result = compareImages(file1, file2, tolerance);

    std::cout << "Results:" << std::endl;
    std::cout << "  Identical:        " << (result.identical ? "YES" : "NO") << std::endl;
    std::cout << "  Different pixels: " << result.diffPixels << std::endl;
    std::cout << "  MSE:              " << result.mse << std::endl;
    std::cout << "  PSNR:             " << result.psnr << " dB" << std::endl;
    std::cout << "  Max difference:   " << result.maxDiff << std::endl;
    std::cout << std::endl;

    // Return 0 if images are identical within tolerance, 1 otherwise
    return result.identical ? 0 : 1;
}
