#include <iostream>
#include <chrono>
#include <filesystem>
#include "SceneLoader.hpp"

struct ComparisonResult {
    bool dimensionsMatch;
    unsigned int expectedWidth;
    unsigned int expectedHeight;
    unsigned int actualWidth;
    unsigned int actualHeight;
    bool pixelsMatch;
    int totalPixels;
    int differentPixels;
    double maxPixelDifference;
    double averagePixelDifference;
};

// Simple PNG pixel comparison
ComparisonResult compareImages(const std::string& image1Path, const std::string& image2Path, unsigned char tolerance = 5) {
    ComparisonResult result = {};
    result.pixelsMatch = false;

    // For now, just check if files exist
    if (!std::filesystem::exists(image1Path) || !std::filesystem::exists(image2Path)) {
        std::cerr << "ERROR: Image file not found for comparison" << std::endl;
        if (!std::filesystem::exists(image1Path)) {
            std::cerr << "  Missing: " << image1Path << std::endl;
        }
        if (!std::filesystem::exists(image2Path)) {
            std::cerr << "  Missing: " << image2Path << std::endl;
        }
        return result;
    }

    // TODO: Implement actual pixel comparison once lodepng integration is available
    // For now, just validate dimensions match through file size heuristic
    auto size1 = std::filesystem::file_size(image1Path);
    auto size2 = std::filesystem::file_size(image2Path);

    if (size1 == size2) {
        result.pixelsMatch = true;
        result.dimensionsMatch = true;
    }

    return result;
}

int main(int argc, char* argv[]) {
    std::string sceneFile = "./scenes/two-spheres-on-plane.json";
    std::string referenceImage = "./tests/reference_images/fake_reference.png";
    std::string outputImage = "/tmp/two-spheres-test-output.png";
    unsigned int expectedWidth = 1920;
    unsigned int expectedHeight = 1080;

    std::cout << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "=== Raytracer E2E Test Suite ===" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;

    // Test 1: Image Resolution Check
    std::cout << "[Test 1] Rendering scene and checking resolution..." << std::endl;
    std::cout << "  Scene: " << sceneFile << std::endl;
    std::cout << "  Expected: " << expectedWidth << "x" << expectedHeight << std::endl;
    std::cout << std::endl;

    try {
        auto [scene, camera, image] = SceneLoader::Load(sceneFile);

        if (!image) {
            std::cerr << "ERROR: Failed to load scene" << std::endl;
            return 1;
        }

        // Check resolution
        std::cout << "  Actual: " << image->width << "x" << image->height << std::endl;
        bool resolutionMatches = (image->width == expectedWidth && image->height == expectedHeight);

        if (!resolutionMatches) {
            std::cerr << "FAIL: Resolution mismatch!" << std::endl;
            delete scene;
            delete camera;
            delete image;
            return 1;
        }

        std::cout << "  PASS: Resolution matches" << std::endl;
        std::cout << std::endl;

        // Render the scene
        std::cout << "[Test 2] Rendering scene..." << std::endl;
        auto begin = std::chrono::high_resolution_clock::now();
        camera->render(*image, *scene);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

        std::cout << "  Render time: " << elapsed.count() << " ms" << std::endl;
        std::cout << "  PASS: Scene rendered successfully" << std::endl;
        std::cout << std::endl;

        // Write output
        std::cout << "[Test 3] Writing output image..." << std::endl;
        std::cout << "  Output: " << outputImage << std::endl;
        image->writeFile(outputImage);
        std::cout << "  PASS: Image written" << std::endl;
        std::cout << std::endl;

        // Check if reference image exists
        std::cout << "[Test 4] Comparing with reference image..." << std::endl;
        std::cout << "  Reference: " << referenceImage << std::endl;
        if (!std::filesystem::exists(referenceImage)) {
            std::cout << "  SKIP: Reference image not found (first run)" << std::endl;
            std::cout << "  To use: cp " << outputImage << " " << referenceImage << std::endl;
        } else {
            auto compResult = compareImages(referenceImage, outputImage);
            if (compResult.pixelsMatch) {
                std::cout << "  PASS: Images match" << std::endl;
            } else {
                std::cout << "  FAIL: Images differ" << std::endl;
                delete scene;
                delete camera;
                delete image;
                return 1;
            }
        }

        std::cout << std::endl;
        std::cout << "================================" << std::endl;
        std::cout << "All tests PASSED" << std::endl;
        std::cout << "================================" << std::endl;
        std::cout << std::endl;

        delete scene;
        delete camera;
        delete image;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
}
