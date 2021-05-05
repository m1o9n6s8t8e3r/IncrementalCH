#include <iostream>
#include <chrono>
#include "seqhull.h"
#define point2D pair<int, int>
#define facet2D pair<point2D, point2D>
#define ridge2D point2D
int main(int argc, char** argv)
{
    bool debug = false;
    if (argc > 1 && argv[1] == "-debug") {
      debug = true;
    }
    int size = 0;
    std::cin >> size;
    point2D *points = new point2D[size];
    for (int i = 0; i < size; i++) {
      std::cin >> points[i].first >> points[i].second;
    }
    Sequential2DCH::printPoints2D(points, size);
    point2D *hull = new point2D[size];
    auto start = std::chrono::high_resolution_clock::now();
    int hull_size = Sequential2DCH::convexHull2D(points, size, hull, debug);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Execution on size " << size << " dataset took " << duration.count() << " microseconds" << std::endl;
    //if (hull_size > 0) {
    //    cout << "Output size: " << hull_size << std::endl;
    //    printPoints2D(hull, hull_size);
    //}
    return 0;
}
