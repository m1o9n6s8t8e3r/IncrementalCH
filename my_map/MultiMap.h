#include <atomic>

#define point2D std::pair<int, int>
#define facet2D std::pair<point2D, point2D>
#define ridge2D std::point2D

class MultiMap
{
    private:
    int _size;
    std::atomic<std::pair<ridge2D, facet2D>>** _table;
    int advance(int i) {return (i + 1) % _capacity;}

    public:
    int _capacity;
    int hash_ridge(const ridge2D& key);
    // Atomic
    bool insert_and_set(const ridge2D& key, std::pair<ridge2D, facet2D>* t);
    facet2D get_value(const ridge2D& key, facet2D* wrong_value);
    int get_size() {return size;}
    int get_capacity() {return capacity;}
    /*******************************************************************
    get_key_by_idx and get_value_by_idx do not have atomicity guarantees
    ********************************************************************/
    int get_key_by_idx(int i) {return _table[i].first;}
    int get_value_by_idx(int i) {return *(_table[i].second);}

}