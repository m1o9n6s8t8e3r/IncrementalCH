#include <atomic>
#include <utility>
#include <vector>
typedef std::pair<int, int> point2D;
typedef std::pair<point2D, point2D> facet2D;
typedef point2D ridge2D;

struct Entry {
    std::atomic<bool> taken_{false};
    std::atomic_flag check_ = ATOMIC_FLAG_INIT;
    facet2D key_;
    std::vector<int> data_;
};

class HashMap
{
    private:
    //std::atomic<int> size_{0};
    Entry* table_ = nullptr;
    int advance(int i) {return (i + 1) % capacity_;}

    public:
    int capacity_{0};
    MultiMap(int capacity) {
        capacity_ = capacity;
        table_ = new Entry[capacity];
    }
    int hash_ridge(ridge2D key);
    int hash_facet(facet2D key);
    // Atomic
    bool insert(facet2D key, std::vector<int> points);
    std::vector<int> get_value(facet2D key);
    //int get_size() {return size_.load();}
    int get_capacity() {return capacity_;}
    /*******************************************************************
    get_key_by_idx and get_value_by_idx do not have atomicity guarantees
    ********************************************************************/
    facet2D get_key_by_idx(int i);
    std::vector<int> get_value_by_idx(int i);
	bool get_taken_by_idx(int i);
	int get_size();
};
