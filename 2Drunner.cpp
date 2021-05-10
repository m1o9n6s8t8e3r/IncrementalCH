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

    /*
    std::unordered_set<point2D> hull_set = std::unordered_set<point2D>();
    for (int i = 0; i < hull_size; i++) {
      hull_set.insert(hull[i]);
    }
    point2D *par_hull = new point2D[size];
    auto par_start = std::chrono:high_resolution_clock::now();
    int par_hull_size = Parallel2DCH:convexHull2D(points, size, par_hull, debug);
    auto par_stop = std::chrono:high_resolution_clock::now();
    auto par_duration = std::chrono::duration_cast<std::chrono::microseconds>(par_stop - par_start);
    std::cout << "Parallel execution on size " << size << " dataset took " << par_duration.count() << " microseconds" << std::endl;
    bool correct = true;
    if (par_hull_size != hull_size) {
      correct = false;
    } else {
      for (int i = 0; i < par_hull_size; i++) {
        if (hull_set.find(par_hull[i]) == hull_set.end()) {
          correct = false;
        }
      }
    }
    if (!correct) {
      std::cout << "Parallel solution INCORRECT" << std::endl;
    }
    */
    return 0;
}
