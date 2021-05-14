// Cilk parallel program for Quick Sort Algorithm....
#include <iostream>
#include <cilk/cilk.h>
#include <algorithm>
#include <vector>
#include <chrono>

bool print=false;

void quick_sort(std::vector<int>::iterator beg, std::vector<int>::iterator end);
void quick_sort(std::vector<int>::iterator beg, std::vector<int>::iterator end) {
    if (beg < end) {
		end--;
        auto pivot = partition(beg, end, bind2nd(std::less<int>(), *end));
        std::swap(*end, *pivot);
        cilk_spawn quick_sort(beg, pivot);
        pivot++;
        end++;
        quick_sort(pivot, end);
		cilk_sync;
    }
}

void quick_sort_arr(int * beg, int * end);
void quick_sort_arr(int * beg, int * end) {
    if (beg < end) {
		end--;
        int * pivot = partition(beg, end, bind2nd(std::less<int>(), *end));
        std::swap(*end, *pivot);
        cilk_spawn quick_sort_arr(beg, pivot);
        pivot++;
        end++;
        quick_sort_arr(pivot, end);
		cilk_sync;
    }
}

int main(){ 
    int n;
    std::cout<<"Enter total number of elements that are to be sorted: ";
    std::cin>>n;
    int*  a = new int[n];
	std::vector<int> A = std::vector<int>();
	A.resize(n);
    cilk_for (int x = 0; x< n; x++){A[x] = x / 3; }
    cilk_for (int x = 0; x< n; x++){a[x] = x / 3; }
	std::random_shuffle(A.begin(), A.end());
	//std::random_shuffle(a, a+n);
    std::cout << "Sorting " << n << " elements" << std::endl;
	if (print) 
		{
	std::cout << '{';
	for (int num : A) {
		std::cout << num << ' ';
	}
	std::cout << '}' << std::endl;
	std::cout << '{';
	for (int i = 0; i < n; i++) {
		std::cout << a[i] << ' ';
	}
	std::cout << '}' << std::endl;
		}
	
    auto start = std::chrono::high_resolution_clock::now();
    quick_sort(A.begin(), A.end());
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	//quick_sort_arr(a, a + n);
	//std::cout << "Quicksort on " << n << " items succeeded." << std::endl;
    std::cout << "Quicksot on size " << n << " dataset took " << duration.count() << " microseconds" << std::endl;
	if (print) 
		{
	std::cout << '{';
	for (int num : A) {
		std::cout << num << ' ';
	}
	std::cout << '}' << std::endl;
	std::cout << '{';
	for (int i = 0; i < n; i++) {
		std::cout << a[i] << ' ';
	}
	std::cout << '}' << std::endl;
		}
    return 0;
}
