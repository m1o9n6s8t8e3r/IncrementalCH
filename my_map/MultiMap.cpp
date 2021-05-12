#include "MultiMap.h"

typedef std::pair<int, int> point2D;
typedef std::pair<point2D, point2D> facet2D;
typedef point2D ridge2D;

MultiMap::MultiMap(int capacity)
{
    _capacity = capacity;
    _table = new std::atomic<std::pair<ridge2D, facet2D>>* [capacity]();
}

int MultiMap::hash_ridge(const ridge2D& key)
{
    int sum = key.first + key.second;
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
        if (_table[i]->load().first == key) {
            return false;
        }
        advance(i);
    }
    _size++;
    return true;
}

ridge2D MultiMap::get_key_by_idx(int i)
{
    std::pair<ridge2D, facet2D> expected = _table[i]->load();
    return expected.first;
}
facet2D get_value_by_idx(int i)
{
    std::pair<ridge2D, facet2D> expected = _table[i]->load();
    return expected.second;
}