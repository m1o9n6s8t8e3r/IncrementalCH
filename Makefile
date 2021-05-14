#Put the path to your OpenCilk build here
BUILD_PREFIX = /home/isaac/Documents/csclasses/15418/OpenCilk

PAR_CFLAGS= -fcilkplus
CC = $(BUILD_PREFIX)/OpenCilk/build/bin/clang++
all:
	$(CC) $(PAR_CFLAGS) my_map/MultiMap.cpp parhull.cpp -o parhull
	$(CC) seqhull.cpp 2Drunner.cpp -o sequhull
	$(CC) $(PAR_CFLAGS) quick_sort.cpp -o quicksort
