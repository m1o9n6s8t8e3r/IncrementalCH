//Two dimensional version
#include <iostream>
#include <utility>
#include <set>
#include <map>
using namespace std;

#define point2D pair<int, int>
#define facet2D pair<point2D, point2D>
#define ridge2D point2D

void printFacet2D(facet2D f) {
    int x1 = f.first.first;
    int x2 = f.second.first;
    int y1 = f.first.second;
    int y2 = f.second.second;
    cout << "(" << x1 << ", " << y1 << ") -> (" 
         << x2 << ", " << y2 << ")" << std::endl;
}

void printFacetSet2D(set<facet2D> F) {
    cout << "Facets with size : " << F.size() << std::endl;
    set<facet2D>::iterator itr;
    for (itr = F.begin(); itr != F.end(); itr++) {
        facet2D f = *itr;
        printFacet2D(f);
    }
}

void printPoint2D(point2D p) {
    int x = p.first;
    int y = p.second;
    cout << "(" << x << ", " << y << ")" << std::endl;
}

void printPointSet2D(set<point2D> P) {
    cout << "Points: " << std::endl;
    set<point2D>::iterator itr;
    for (itr = P.begin(); itr != P.end(); itr++) {
        point2D p = *itr;
        printPoint2D(p);
    }
}

void printPoints2D(point2D* points, int size) {
    for (int i = 0; i < size; i++) {
        printPoint2D(points[i]);
    }
}

facet2D facetSwap(facet2D f) {
    return {f.second, f.first};
}

bool visible2D(point2D v, facet2D t) {
    point2D p1 = t.first;
    point2D p2 = t.second;
    
    int a = p2.first - p1.first;
    int b  = p2.second - p1.second;
    int c = v.first - p1.first;
    int d = v.second - p1.second;
    int cross_product = (a * d) - (b * c);

    // I am assuming it cannot be 0 for now, as in section 5.
    return cross_product > 0;
}


// TODO change to ordered set so can take min.
map<facet2D, set<point2D>> C;
set<facet2D> H;
set<ridge2D> H_ridges;
// TODO change M as described in paper.
map<ridge2D, pair<facet2D, facet2D>> M;

/* InsertAndSet:
 *      Maintains a map R as a hash table
 *      using ridges as keys and values as ridge-facet pairs
 *      R uses linear probing for conflicts.
 *
 * Output: If r is in M, return false.
 *      Otherwise, return true and map r to t in M.
*/
void InsertAndSet(ridge2D r, facet2D t) {
    return;
}

/* GetValue:
 * 
 *
 * Output: A value t' associated with r in M 
 *         which is not t
*/
void GetValue(ridge2D r, facet2D t) {
    return;
}

void ProcessRidge(facet2D t1, ridge2D r, facet2D t2) {
    if (C[t1].size() == 0 && C[t2].size() == 0) {
        return;
    }
    //minstuff
}

int convexHull2D(point2D* points, int size, point2D* output) {
    if (size < 0) {
        cout << "Size must be nonnegative" << std::endl;
        return -1;
    }
    if (size <= 3) {
        for (int i = 0; i < size; i++) {
            output[i] = points[i];
        }
        return size;
    }
    //Initializing Hull 
    for (int i = 0; i < 3; i++) {
        // Adopting convention that facets face outwards.
        if (visible2D(points[(i+2) % 3], {points[i % 3], points[(i+1) % 3]})) {
            H.insert({points[(i+1) % 3], points[i % 3]});
        } else {
            H.insert({points[i % 3], points[(i+1) % 3]});
        }
        H_ridges.insert(points[i]);
    }
    for (int i = 0; i < 3; i++) {
        facet2D f1 = {points[i], points[(i+1) % 3]};
        if (H.find(f1) == H.end()) {
            f1 = facetSwap(f1);
        }
        facet2D f2 = {points[i], points[(i+2) % 3]};
        if (H.find(f2) == H.end()) {
            f2 = facetSwap(f2);
        }
        M.insert(pair<point2D, pair<facet2D, facet2D>>(points[i], {f1, f2}));
    }
    
    // parallel foreach t in H do
    for (auto it = H.begin(); it != H.end(); ++it) {
        // C(t) <- {v in V | visible(v, t)}
        facet2D t = *it;
        set<point2D> S_new;
        C[t] = S_new;
        // parallel foreach v in V
        for (int i = 0; i++; i < size) {
            point2D v = points[i];
            if (visible2D(v, t)) {
                (C[t]).insert(v);
            }
        }
    }
    // parallel for each t1,t2 sharing ridge r
    for (auto it = H_ridges.begin(); it != H_ridges.end(); ++it) {
        ridge2D r = *it;
        facet2D t1 = M[r].first;
        facet2D t2 = M[r].second;
        //ProcessRidge();
    }

    cout << "Final Hull" << std::endl;
    printFacetSet2D(H);
    return size;
}

int main()
{
    int size = 8;
    point2D points[size];
    points[0] = {-100, 0};
    points[1]= {100, 0};
    points[2] = {0, 50};
    points[3] = {1, 60};
    points[4] = {33, 37};
    points[5] = {0, 100};
    points[6] = {60, 80};
    points[7] = {80, 60};
    cout << "Input size: " << size << std::endl;
    printPoints2D(points, size);
    point2D* hull = (point2D*)calloc(size, sizeof(point2D));
    int hull_size = convexHull2D(points, size, hull);
    //if (hull_size > 0) {
    //    cout << "Output size: " << hull_size << std::endl;
    //    printPoints2D(hull, hull_size);
    //}
    return 0;
}