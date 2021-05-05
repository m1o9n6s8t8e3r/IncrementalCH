//Two dimensional version
#include <iostream>
#include <utility>
#include <set>
#include <map>
using namespace std;
#define point2D pair<int, int>
#define facet2D pair<point2D, point2D>
#define ridge2D point2D

namespace Sequential2DCH {
    void printFacet2D(facet2D f);
    void printFacetSet2D(set<facet2D> F);
    void printPoint2D(point2D p);
    void printPointSet2D(set<point2D> P);
    void printPoints2D(point2D* points, int size);
    facet2D facetSwap(facet2D f);
    bool visible2D(point2D v, facet2D t);
    int convexHull2D(point2D* points, int size, point2D* output, bool debug);
}
