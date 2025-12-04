#pragma once

#include <atomic>

/**
 * Statistics collected during rendering
 * This helps identify performance bottlenecks
 */
class RenderStats {
public:
    static std::atomic<long long> raysGenerated;
    static std::atomic<long long> intersectionTests;
    static std::atomic<long long> intersectionsFound;
    
    static void reset() {
        raysGenerated = 0;
        intersectionTests = 0;
        intersectionsFound = 0;
    }
    
    static void printStats() {
        long long rays = raysGenerated.load();
        long long tests = intersectionTests.load();
        long long hits = intersectionsFound.load();
        
        printf("\nPerformance Statistics:\n");
        printf("  Rays generated:        %lld\n", rays);
        printf("  Intersection tests:    %lld\n", tests);
        printf("  Intersections found:   %lld\n", hits);
        
        if (rays > 0) {
            printf("  Tests per ray:         %.2f\n", (double)tests / rays);
        }
        
        if (tests > 0) {
            printf("  Hit rate:              %.2f%%\n", (double)hits * 100.0 / tests);
        }
    }
};

// Initialize static members
std::atomic<long long> RenderStats::raysGenerated{0};
std::atomic<long long> RenderStats::intersectionTests{0};
std::atomic<long long> RenderStats::intersectionsFound{0};
