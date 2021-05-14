PAR_CFLAGS= -fcilkplus

#Path to OpenCilk clang++
CC = /home/isaac/Documents/csclasses/15418/OpenCilk/build/bin/clang++
all:
	$(CC) $(PAR_CFLAGS) my_map/MultiMap.cpp parhull.cpp -o parhull
	$(CC) seqhull.cpp 2Drunner.cpp -o sequhull
	$(CC) $(PAR_CFLAGS) quick_sort.cpp -o quicksort
