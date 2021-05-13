#include <iostream>
#include "MultiMap.h"

typedef std::pair<int, int> point2D;
typedef std::pair<point2D, point2D> facet2D;
typedef point2D ridge2D;

void print_table(MultiMap M, int capacity) {
	for (int i = 0; i < capacity; i++) {
		ridge2D k = M.get_key_by_idx(i);
		facet2D v = M.get_value_by_idx(i);
		std::cout << M.get_taken_by_idx(i) << ": ";
		std::cout << '(' << k.first << ',' << k.second << ')' << '\t';
		std::cout << "{(" << v.first.first << ',' << v.first.second << ") -> (";
		std::cout << v.second.first << "," << v.second.second << ")}" << std::endl;
	}
}

int main()
{
    int capacity = 13;
    MultiMap M = MultiMap(capacity);
	ridge2D p0 = {1,1};
	ridge2D p1 = {2,4};
	ridge2D p2 = {0,0};
	facet2D t1 = {p0, p1};
	facet2D t2 = {p1, p2};
	facet2D t3 = {p2, p0};
	bool succ1 = M.insert_and_set(p0, t1);
	std::cout << "First insert: " << succ1 << std::endl;
	print_table(M, capacity);
	bool succ2 = M.insert_and_set(p1, t2);
	std::cout << "Second insert: " << succ2 << std::endl;
	print_table(M, capacity);
	bool succ3 = M.insert_and_set(p0, t2);
	std::cout << "Third insert: " << succ3 << std::endl;
	print_table(M, capacity);
	facet2D f = M.get_value(p0, t2);
	std::cout << '(' << p0.first << ',' << p0.second << ')' << '\t';
	std::cout << "{(" << f.first.first << ',' << f.first.second << ") -> (";
	std::cout << f.second.first << "," << f.second.second << ")}" << std::endl;
	return 0;
}
