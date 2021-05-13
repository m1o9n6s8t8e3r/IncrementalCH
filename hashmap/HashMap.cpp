#include <iostream>
#include "HashMap.h"

typedef std::pair<int, int> point2D;
typedef std::pair<point2D, point2D> facet2D;
typedef point2D ridge2D;

/*
HashMap::HashMap(int capacity)
{
    capacity_ = capacity;
    table_ = new Entry[capacity];
}
*/

int HashMap::hash_ridge(ridge2D key)
{
    int sum = key.first + key.second;
    int cantor = sum * (sum + 1) / 2 + key.second;
	if (cantor < 0) {
		cantor = 0-cantor;
	}
    return cantor % capacity_;
}

int HashMap::hash_facet(facet2D key)
{
    int hash1 = hash_ridge(key.first);
    int hash2 = hash_ridge(key.second);
    return hash_ridge(std::make_pair(first, second));
}

bool HashMap::insert(facet2D key, std::vector<int> points)
{
    int start = hash_facet(key);
    int i = start;
    bool expect_false = false;
    //std::cout << "Start: " << i << std::endl;
	//while (!std::atomic_compare_exchange_strong(&(table_[i].taken_), &expect_false, true)) {
	while (!((table_[i].taken_).compare_exchange_strong(expect_false, true))) {
		//std::cout << "Find spot: " << i << std::endl;
    	expect_false = false;
        i = advance(i);
    }
	//std::cout << "Putting at" << i << std::endl;
    table_[i].key_ = key;
    table_[i].data_ = points;
    i = start;
    while (table_[i].taken_.load()) {
		//std::cout << "Find not in use: " << i << std::endl;
        if (table_[i].key_ == key) {
            if (table_[i].check_.test_and_set()) {
                return false;
            }
        }
        i = advance(i);
    }
    return true;
}
std::vector<int> HashMap::get_value(facet2D key) {
    std::vector<int> ret;
    int i = hash_facet(key);
    while (table_[i].taken_.load()) {
        if (table_[i].data_.first == key) {
            ret = table_[i].data_.second;
            return ret;
        }
        i = advance(i);
    }
    return ret;
}

facet2D HashMap::get_key_by_idx(int i)
{
    return table_[i].key_;
}
std::vector<int> HashMap::get_value_by_idx(int i)
{
    return table_[i].data_;
}
bool HashMap::get_taken_by_idx(int i)
{
	return table_[i].taken_.load();
}
int HashMap::get_size()
{
	int ret = 0;
	for (int i = 0; i < capacity_; i++) {
		if (table_[i].taken_.load()) ret++;
	}
	return ret;
}
