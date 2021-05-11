//Two dimensional version
#include <iostream>
#include <utility>
#include <set>
#include <map>
#include "seqhull.h"
using namespace std;
#define point2D pair<int, int>
#define facet2D pair<point2D, point2D>
#define ridge2D point2D

namespace Sequential2DCH {
  
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
    cout << "(" << x << ", " << y << ")" <<std::endl;
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

int convexHull2D(point2D* points, int size, point2D* output, bool debug) {
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
    // Initialize hull
    set<facet2D> H;
    map<ridge2D, pair<facet2D, facet2D>> ridge_facets;
    for (int i = 0; i < 3; i++) {
        // Adopting convention that facets face outwards.
        if (visible2D(points[(i+2) % 3], {points[i % 3], points[(i+1) % 3]})) {
            H.insert({points[(i+1) % 3], points[i % 3]});
        } else {
            H.insert({points[i % 3], points[(i+1) % 3]});
        }
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
        ridge_facets.insert(pair<point2D, pair<facet2D, facet2D>>(points[i], {f1, f2}));
    }
    if (debug) {
      cout << "Starting Hull" << std::endl;
      printFacetSet2D(H);
    }
    // Initialize conflicting visible points map
    map<facet2D, set<point2D>> C;
    map<point2D, set<facet2D>> C_inv;
    set<facet2D>::iterator itr;
    for (int i = 0; i < size; i++) {
        point2D p = points[i];
        C_inv.insert(pair<point2D, set<facet2D>>(p, {}));
    }
    for (itr = H.begin(); itr != H.end(); itr++) {
        facet2D f = *itr;
        C.insert(pair<facet2D, set<point2D>>(f, {}));
        for (int i = 0; i < size; i++) {
            point2D p = points[i];
            if (visible2D(p, f)) {
                (C[f]).insert(p);
                (C_inv[p]).insert(f);
            }
        }
    }
    for (int i = 2+1; i < size; i++) {
        point2D p = points[i];
        set<facet2D> R = C_inv[p];
        //printFacetSet2D(R);
        //Find ridges
        set<facet2D>::iterator itr;
        ridge2D ridge1;
        ridge2D ridge2;
        set<ridge2D> boundary;
        //Trick to find boundary in 2D
        for (itr = R.begin(); itr != R.end(); itr++) {
            ridge1 = (*itr).first;
            ridge2 = (*itr).second;
            if (boundary.find(ridge1) != boundary.end()) {
                boundary.erase(ridge1);
            } else {
                boundary.insert(ridge1);
            }
            if (boundary.find(ridge2) != boundary.end()) {
                boundary.erase(ridge2);
            } else {
                boundary.insert(ridge2);
            }
        }
        set<point2D>::iterator pt_itr;
        // There are only 2 on the boundary in 2D, but many more are possible in higher dimensions.
        for (pt_itr = boundary.begin(); pt_itr != boundary.end(); pt_itr++) {
            ridge2D r = *pt_itr;
            facet2D f1 = ridge_facets[r].first;
            if (H.find(f1) == H.end()) {
                f1 = facetSwap(f1);
                cout << "Whoops: vertex " << i << std::endl;
                printFacet2D(f1);
            }
            facet2D f2 = ridge_facets[r].second;
            if (H.find(f2) == H.end()) {
                f2 = facetSwap(f2);
                cout << "Whoops: vertex " << i << std::endl;
                printFacet2D(f2);
            }
            // Make f1 visible and f2 invisible from p
            if (!visible2D(p, f1)) {
                facet2D swap;
                swap = f1;
                f1 = f2;
                f2 = swap;
            }
            facet2D new_f = {r, p};
            point2D q = f1.first;
            if (p == q) {
                q = f1.second;
            }
            if (visible2D(q, new_f)) {
                new_f = facetSwap(new_f);
            }
            // Update everything to include new facet and erase old one.
            set<point2D> new_v;
            C[new_f] = new_v;
            set<point2D>::iterator vis_itr;
            for (vis_itr = C[f1].begin(); vis_itr != C[f1].end(); vis_itr++) {
                if (visible2D(*vis_itr, new_f)) {
                    C[new_f].insert(*vis_itr);
                    C_inv[*vis_itr].insert(new_f);
                }
            }
            for (vis_itr = C[f2].begin(); vis_itr != C[f2].end(); vis_itr++) {
                if (C[new_f].find(*vis_itr) == C[new_f].end() && visible2D(*vis_itr, new_f)) {
                    C[new_f].insert(*vis_itr);
                    C_inv[*vis_itr].insert(new_f);
                }
            }
            H.insert(new_f);
            ridge_facets[r] = {new_f, f2};
        }
        // Delete old facets
        for (itr = R.begin(); itr != R.end(); itr++) {
            H.erase(*itr);
            set<point2D>::iterator del_itr;
            for (del_itr = C[*itr].begin(); del_itr != C[*itr].end(); ++del_itr) {
                C_inv[*del_itr].erase(*itr);
            }
            C.erase(*itr);
        }
        // Delete p from maps;

        // Insert new ridge-facets from adding point! TODO generalize to nD later.
        set<facet2D>::iterator h_itr;
        facet2D new_f1;
        facet2D new_f2;
        for (h_itr = H.begin(); h_itr != H.end(); ++h_itr) {
            if ((*h_itr).first == p) {
                new_f1 = *h_itr;
            }
            if ((*h_itr).second == p) {
                new_f2 = *h_itr;
            }
        }
        ridge_facets.insert(pair<point2D, pair<facet2D, facet2D>>(points[i], {new_f1, new_f2}));
        //cout << "CMAPCMAPCMAP" << std::endl;
        //for (h_itr = H.begin(); h_itr != H.end(); ++h_itr) {
        //    printFacet2D(*h_itr);
        //    printPointSet2D(C[*h_itr]);
        //}
        if (debug) {
          cout << "Hull " << i << std::endl;
          printFacetSet2D(H);
        }
    }

    cout << "Final Hull" << std::endl;
    printFacetSet2D(H);
    return size;
}

}
