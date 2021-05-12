#include <atomic>
#include <utility>
typedef std::pair<int, int> point2D;
typedef std::pair<point2D, point2D> facet2D;
typedef point2D ridge2D;

struct Entry {
    std::atomic<bool> taken_{false};
    std::atomic_flag check_ = ATOMIC_FLAG_INIT;
    std::pair<ridge2D, facet2D> data_;
};

class MultiMap
{
    private:
    std::atomic<int> size_{0};
    Entry* table_ = nullptr;
    int advance(int i) {return (i + 1) % capacity_;}

    public:
    int capacity_{0};
    MultiMap(int capacity) {
        capacity_ = capacity;
        table_ = new Entry[capacity];
    }
    int hash_ridge(ridge2D key);
    // Atomic
    bool insert_and_set(ridge2D key, facet2D t);
    facet2D get_value(ridge2D key, facet2D t);
    int get_size() {return size_.load();}
    int get_capacity() {return capacity_;}
    /*******************************************************************
    get_key_by_idx and get_value_by_idx do not have atomicity guarantees
    ********************************************************************/
    ridge2D get_key_by_idx(int i);
    facet2D get_value_by_idx(int i);

};