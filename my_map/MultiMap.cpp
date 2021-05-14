#include <iostream>
#include "MultiMap.h"

typedef std::pair<int, int> point2D;
typedef std::pair<point2D, point2D> facet2D;
typedef point2D ridge2D;

/*
MultiMap::MultiMap(int capacity)
{
    capacity_ = capacity;
    table_ = new Entry[capacity];
}
*/

int MultiMap::hash_ridge(ridge2D key)
{
    int sum = key.first + key.second;
    int cantor = sum * (sum + 1) / 2 + key.second;
	if (cantor < 0) {
		cantor = 0-cantor;
	}
    return cantor % capacity_;
}

bool MultiMap::insert_and_set(ridge2D key, facet2D t)
{
    int start = hash_ridge(key);
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
    table_[i].data_ = std::make_pair(key, t);
    i = start;
    while (table_[i].taken_.load()) {
		//std::cout << "Find not in use: " << i << std::endl;
        if (table_[i].data_.first == key) {
            if (table_[i].check_.test_and_set()) {
                return false;
            }
        }
        i = advance(i);
    }
    return true;
}
facet2D MultiMap::get_value(ridge2D key, facet2D t) {
    facet2D ret;
    int i = hash_ridge(key);
    while (table_[i].taken_.load()) {
        if (table_[i].data_.first == key) {
            ret = table_[i].data_.second;
            if (ret != t) {
                return ret;
            }
        }
        i = advance(i);
    }
    point2D BAD1 = {420420420, -420420420};
    point2D BAD2 = {-420420420, 420420420};
    ret = {BAD1, BAD2};
    return ret;
}

ridge2D MultiMap::get_key_by_idx(int i)
{
    return table_[i].data_.first;
}
facet2D MultiMap::get_value_by_idx(int i)
{
    return table_[i].data_.second;
}
bool MultiMap::get_taken_by_idx(int i)
{
	return table_[i].taken_.load();
}
int MultiMap::get_size()
{
	int ret = 0;
	for (int i = 0; i < capacity_; i++) {
		if (table_[i].taken_.load()) ret++;
	}
	return ret;
}
