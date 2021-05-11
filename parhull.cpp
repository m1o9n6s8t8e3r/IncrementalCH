//Two dimensional version
#include <iostream>
#include <utility>
#include <set>
#include <map>
#include <mutex>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
using namespace std;

#define point2D pair<int, int>
#define facet2D pair<point2D, point2D>
#define ridge2D point2D

#define MAXSIZE 100000000

int setMin(set<int> S) {
    if (S.size() == 0) {
        return MAXSIZE;
    }
    else {
        return *S.begin();
    }
}

int minSet(set<int> S) {
    return setMin(S);
}

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


// Map from facets to point indices.
map<facet2D, set<int>> C;
set<facet2D> H;
std::mutex H_lock;
std::mutex C_lock;
std::mutex M_lock;
map<ridge2D, pair<facet2D, facet2D>> H_ridges;
// TODO change M as described in paper.
map<ridge2D, pair<facet2D, facet2D>> M;
point2D BAD1 = {420420420, -420420420};
point2D BAD2 = {-420420420, 420420420};
facet2D BAD = {BAD1, BAD2};
/* InsertAndSet:
 *      Maintains a map R as a hash table
 *      using ridges as keys and values as ridge-facet pairs
 *      R uses linear probing for conflicts.
 *
 * Output: If r is in M, return false.
 *      Otherwise, return true and map r to t in M.
*/
bool InsertAndSet(ridge2D r, facet2D t) {
    if (M.count(r) > 0) {
        return false;
    } else {
        // TODO(Sylvia) change the sync of M as needed
        M_lock.lock();
        M.insert(pair<ridge2D, pair<facet2D, facet2D>>(r, {t, BAD}));
        M_lock.unlock();
        return true;
    }
}

/* GetValue:
 * 
 *
 * Output: A value t' associated with r in M 
 *         which is not t
*/
facet2D GetValue(ridge2D r, facet2D t) {
    return M[r].first;
}

void ProcessRidge(facet2D t1, ridge2D r, facet2D t2, point2D* points) {
    //cout << "Processing Ridge(t1,r,t2)" << std::endl;
    //cout << "t1=\t";
    //printFacet2D(t1);
    //cout << "r=\t";
    //printPoint2D(r);
    //cout << "t2=\t";
    //printFacet2D(t2);
        
    // If both are emtpy then this is a final facet!
    if (C[t1].size() == 0 && C[t2].size() == 0) {
        //cout << "Final facets found" << std::endl;
        return;
    }
    // If just one is empty, this means just one is final.
    /*else if (C[t1].size() == 0 && C[t2].size() != 0) {
        ProcessRidge(t2, r, t1, points);
    }
    else if (C[t1].size() != 0 && C[t2].size() == 0) {
        cout << "AHHHHH" << std::endl;
    }*/
    // ABOVE IS OUTDATED DUE TO MAXSIZE UPDATE
    // If covered by the same point, then we delete them
    else if (minSet(C[t2]) == minSet(C[t1])) {
        //DELETE t1 and t2 from H
        H_lock.lock();
        H.erase(t1);
        H.erase(t2);
        //cout << "Deleting facets" << std::endl;
        H_lock.unlock();
    }
    // Changing order so that minSet(C[t1]) < minSet(C[t2])
    else if (minSet(C[t2]) < minSet(C[t1])) {
        cilk_spawn ProcessRidge(t2, r, t1, points);
    }
    // Otherwise we know that -1 < minSet(C[t1]) < minSet(C[t2])
    else {
        //cout << "branching" << std::endl;
        point2D p = points[minSet(C[t1])];
        facet2D t = {r, p};
        point2D q = t1.first;
        if (p == q) {
            q = t1.second;
        }
        if (visible2D(q, t)) {
            t = facetSwap(t);
        }
        //Create C[t]
        set<int> new_set;
        // TODO(Sylvia): Is this C locking scheme correct? (concurrency specs of C++)
        C_lock.lock();
        C[t] = new_set; 
        C_lock.unlock();
        for (auto it = C[t1].begin(); it != C[t1].end(); ++it) {
            int i = *it;
            point2D v = points[i];
            if (visible2D(v, t)) {
                C.at(t).insert(i);
            }
        }
        for (auto it = C[t2].begin(); it != C[t2].end(); ++it) {
            int i = *it;
            point2D v = points[i];
            if (C.at(t).find(i) == C.at(t).end() && visible2D(v, t)) {
                C.at(t).insert(i);
            }
        }
        // delete t1 and add t
        H_lock.lock();
        H.erase(t1);
        H.insert(t);
        H_lock.unlock();
        //In 2D only two points on the boundary but can still be split into tasks
        ridge2D r1;
        ridge2D r2;
        r1 = t.first;
        r2 = t.second;
        //task
        {
            if (r1 == r) {
                cilk_spawn ProcessRidge(t, r, t2, points);
            }
            else if (!InsertAndSet(r1, t)) {
                facet2D s = GetValue(r1, t);
                cilk_spawn ProcessRidge(t, r1, s, points);
            }
        }
        //task
        {
            if (r2 == r) {
                cilk_spawn ProcessRidge(t, r, t2, points);
            }
            else if (!InsertAndSet(r2, t)) {
                facet2D s = GetValue(r2, t);
                cilk_spawn ProcessRidge(t, r2, s, points);
            }
        }
    }
    
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
        H_ridges.insert(pair<point2D, pair<facet2D, facet2D>>(points[i], {f1, f2}));
    }
    
    // parallel foreach t in H do
    for (auto it = H.begin(); it != H.end(); ++it) {
        // C(t) <- {v in V | visible(v, t)}
        facet2D t = *it;
        set<int> S_new;
        C[t] = S_new;
        // parallel foreach v in V
        for (int i = 3; i < size; i++) {
            point2D v = points[i];
            if (visible2D(v, t)) {
                (C[t]).insert(i);
            }
        }
        //cout << C[t].size() << std::endl;
    }
    // parallel for each t1,t2 sharing ridge r
    for (auto it = H_ridges.begin(); it != H_ridges.end(); ++it) {
        ridge2D r = it->first;
        facet2D t1 = (it->second).first;
        facet2D t2 = (it->second).second;
        ProcessRidge(t1, r, t2, points);
    }

    cout << "Final Hull" << std::endl;
    printFacetSet2D(H);
    return size;
}

int main()
{
    /*
    int size = 8;
    point2D points[size];
    points[0] = {-100, 0};
    points[1]= {100, 0};
    points[2] = {0, 50};
    points[3] = {1, 60};
    points[4] = {2, 71};
    points[5] = {0, 100};
    points[6] = {60, 80};
    points[7] = {80, 60};
    
    cout << "Input size: " << size << std::endl;
    printPoints2D(points, size);
    point2D* hull = (point2D*)calloc(size, sizeof(point2D));
    */
    __cilkrts_set_param("nworkers", "4");
    int size = 0;
    std::cin >> size;
    point2D *points = new point2D[size];
    for (int i = 0; i < size; i++) {
      std::cin >> points[i].first >> points[i].second;
    }
    //printPoints2D(points, size);
    point2D *hull = new point2D[size];
    auto start = std::chrono::high_resolution_clock::now();
    int hull_size = convexHull2D(points, size, hull);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Parallel execution on size " << size << " dataset took " << duration.count() << " microseconds" << std::endl;
    //if (hull_size > 0) {
    //    cout << "Output size: " << hull_size << std::endl;
    //    printPoints2D(hull, hull_size);
    //}
    return 0;
}