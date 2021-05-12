#include <atomic>
#include <utility>
typedef std::pair<int, int> point2D;
typedef std::pair<point2D, point2D> facet2D;
typedef point2D ridge2D;

class MultiMap
{
    private:
    std::atomic<int> _size{0};
    std::atomic<std::pair<ridge2D, facet2D>>** _table;
    int advance(int i) {return (i + 1) % _capacity;}

    public:
    MultiMap(int capacity);
    int _capacity;
    int hash_ridge(const ridge2D& key);
    // Atomic
    bool insert_and_set(const ridge2D& key, std::pair<ridge2D, facet2D>* t);
    facet2D get_value(const ridge2D& key, facet2D* wrong_value);
    int get_size() {return _size.load();}
    int get_capacity() {return _capacity;}
    /*******************************************************************
    get_key_by_idx and get_value_by_idx do not have atomicity guarantees
    ********************************************************************/
    ridge2D get_key_by_idx(int i);
    facet2D get_value_by_idx(int i);

};