#include "MultiMap.h"

#define point2D std::pair<int, int>
#define facet2D std::pair<point2D, point2D>
#define ridge2D std::point2D

MultiMap::MultiMap(int capacity)
{
    _capacity = capacity;
    _table = new std::atomic<std::pair<ridge2D, facet2D>>**[capacity]();
}

int MultiMap::hash_ridge(const ridge2D& key)
{
    int sum = key.first + key.second
    int cantor = sum * (sum + 1) / 2 + key.second;
    return cantor % _capacity;
}

bool MultiMap::insert_and_set(const ridge2D& key, std::pair<ridge2D, facet2D>* t)
/* InsertAndSet(r, t):
 * i <- hash(r)
 * while not CAS(M[i], null, (r, t)) do
 *   if key at M[i] is r then
 *     return false
 *   next(i)
 * return true
 bool atomic_compare_exchange_strong( std::atomic<T>* obj,
                                     typename std::atomic<T>::value_type* expected,
                                     typename std::atomic<T>::value_type desired ) noexcept;
 */
{
    int i = hash_ridge(key);
    while (!std::atomic_compare_exchange_strong(_table[i], nullptr, *t)) {
        if (_table[i]->first == key) {
            return false;
        }
        advance(i);
    }
    return true;
}